/*******************************************************************************
 * \file	scoring.cpp
 * \brief	Stylus Gene class (scoring methods)
 *
 * Stylus, Copyright 2006-2009 Biologic Institute
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/


// Includes ---------------------------------------------------------------------
#include "headers.hpp"

using namespace std;
using namespace stylus;

//--------------------------------------------------------------------------------
//
// Stroke
//
//--------------------------------------------------------------------------------

/*
 * Function: calcScore
 *
 * A stroke has a good fit to the corresponding Han stroke when it follows
 * it closely. Measuring the fit, though, may be done in one of two ways:
 * A traditional RMSD measure or by tracking maximum deviation along with excess
 * length.
 * 
 * While RMSD is a good measure, for it to work well the stroke must be
 * properly positioned over the Han, and something that is more difficult than it
 * appears for linear strokes. Our present algorithm to align the midpoints of 
 * bounding boxes produces poor alignment for linear strokes that contain a small
 * number of angular vectors: The alignment would magnify an RMSD error by measuring
 * deviation at every point. Other aligments are possible, but more costly and
 * may not work well with non-linear strokes.
 * 
 * Measured maximum deviation is a good approximation of stroke fit, but is insufficient
 * on its own. Two strokes might have the same maximum deviation, but one wanders much
 * more relative to the Han stroke than the other. Since no stroke can do better than
 * the Han (by definition), factoring in excess length will differentiate between these
 * cases. (And doesn't impact curved strokes where drawn with less length than the Han;
 * those will distinguish themselves properly solely through measure maximum deviation.)
 * 
 * Maximum deviation is measured at every stroke and Han vertex (since midpoints
 * cannot be a maximum deviated point and the maximum may occur at either a stroke
 * or Han vertex).
 * 
 * NOTE:
 * - Since the pre-computed points of the Gene are not scaled (and cannot be as
 *   scaling is a feature of a child element), scaling *must* be applied to the
 *   sums rather than accumulating scaled lengths.
 */
bool
Stroke::calcScore(Gene& gene, const HStroke& hst)
{
	ENTER(SCORING,calcScore);

	ASSERT(_slVectors.getLength() > 0);

	ASSERT(_sxToHan.isDefined());
	ASSERT(_syToHan.isDefined());
	ASSERT(_sxyToHan.isDefined());

	TFLOW(SCORING,L5,(LLTRACE, "Stroke %ld - Computing score over range %s", (_id+1), _rgAcids.toString().c_str()));
	
	// Calculate the total stroke length and any extra length (compared to the Han stroke)
	_slVectors.setScale(_sxToHan, _syToHan, _sxyToHan);

	Unit nStrokeLength = _slVectors.getLength();
	_nExtraLength = max<UNIT>(0, (nStrokeLength - hst.getLength()));

	// Obtain the Han points (forward or reversed) that best fit the stroke
	const HPOINTARRAY& vecHPoints = determineStrokeOrientation(gene, hst);
	size_t iHPoint = 0;

	// Obtain the stroke points
	// Note: Points contain the value *after* vector application; the first point is the one prior to the initial acid
	ASSERT(_rgAcids.getStart() > 0);
	const ACIDTYPEARRAY& vecAcids = gene.getAcids();
	const POINTARRAY& vecPoints = gene.getPoints();
	ScaledLength slTraversed(_sxToHan, _syToHan, _sxyToHan);
	long iPoint = _rgAcids.getStart()-1;

	// Walk the stroke points and midpoints measuring deviation
	// - Track maximum deviation for stroke and Han points

	PointDistance ptdPrev(vecPoints[iPoint]);
	ptdPrev.scale(_sxToHan, _syToHan);
	ptdPrev.move(_dxToHan, _dyToHan);

	ASSERT(iHPoint < vecHPoints.size());
	
	HPoint hptPrev(vecHPoints[iHPoint]);
	Unit nDeviation = hptPrev.squareDeviationFrom(ptdPrev);
	_nDeviation = nDeviation;
	++iHPoint;

	TDATA(SCORING,L5,(LLTRACE,
					"DEVIATION (%0.6f): stroke(%6.9f,%6.9f) han(%6.9f,%6.9f) both-deviation(%6.9f)",
					static_cast<UNIT>(0),
					static_cast<UNIT>(ptdPrev.x()), static_cast<UNIT>(ptdPrev.y()),
					static_cast<UNIT>(hptPrev.x()), static_cast<UNIT>(hptPrev.y()),
					static_cast<UNIT>(nDeviation)));

	for (++iPoint; iPoint <= _rgAcids.getEnd() && iHPoint < vecHPoints.size(); ++iPoint)
	{
		ASSERT(Unit(ptdPrev.getDistance()) == slTraversed.getLength());
		slTraversed += vecAcids[iPoint];

		ASSERT(static_cast<size_t>(iPoint) < vecPoints.size());
		PointDistance ptdEndpoint(vecPoints[iPoint], slTraversed.getLength());
		ptdEndpoint.scale(_sxToHan, _syToHan);
		ptdEndpoint.move(_dxToHan, _dyToHan);
		
		// Evaluate at the current point
		// - Distance on stroke points is total (scaled) vector distance
		// - Distance on Han points is the fractional distance
		Unit dTraversedCurrent = ptdEndpoint.getDistance() / nStrokeLength;
		HPoint hptEndpoint(vecHPoints[iHPoint]);
			
		// Measure against all Han points occurring before the point under evaluation
		// - This loop terminates with the Han point at or beyond the current stroke point
		while (dTraversedCurrent > hptEndpoint.getDistance())
		{
			Unit dProportion = ((nStrokeLength * hptEndpoint.getDistance()) - ptdPrev.getDistance()) / (ptdEndpoint.getDistance() - ptdPrev.getDistance());
			PointDistance ptd(getPointBetween(ptdPrev, ptdEndpoint, dProportion));
			nDeviation = hptEndpoint.squareDeviationFrom(ptd);
			_nDeviation = max<UNIT>(_nDeviation, nDeviation);

			TDATA(SCORING,L5,(LLTRACE,
							"DEVIATION (%0.6f): stroke(%6.9f,%6.9f) han(%6.9f,%6.9f) max-deviation(%6.9f)",
							static_cast<UNIT>(hptEndpoint.getDistance()),
							static_cast<UNIT>(ptd.x()), static_cast<UNIT>(ptd.y()),
							static_cast<UNIT>(hptEndpoint.x()), static_cast<UNIT>(hptEndpoint.y()),
							static_cast<UNIT>(nDeviation)));

			hptPrev = hptEndpoint;
			if (++iHPoint >= vecHPoints.size())
				goto CHECKPOINTS;
			hptEndpoint = vecHPoints[iHPoint];
		}

		// Measure deviation at the stroke point
		ASSERT(Unit(hptPrev.getDistance()) < hptEndpoint.getDistance());
		ASSERT(Unit(hptPrev.getDistance()) < dTraversedCurrent);
		ASSERT(Unit(hptEndpoint.getDistance()) >= dTraversedCurrent);
		HPoint hptCurrent(dTraversedCurrent < hptEndpoint.getDistance()
						? getPointBetween(hptPrev, hptEndpoint, (dTraversedCurrent - hptPrev.getDistance()) / (hptEndpoint.getDistance() - hptPrev.getDistance()))
						: hptEndpoint);
		nDeviation = hptCurrent.squareDeviationFrom(ptdEndpoint);
		_nDeviation = max<UNIT>(_nDeviation, nDeviation);

		TDATA(SCORING,L5,(LLTRACE,
						"DEVIATION (%0.6f): stroke(%6.9f,%6.9f) han(%6.9f,%6.9f) deviation(%6.9f)",
						static_cast<UNIT>(dTraversedCurrent),
						static_cast<UNIT>(ptdEndpoint.x()), static_cast<UNIT>(ptdEndpoint.y()),
						static_cast<UNIT>(hptCurrent.x()), static_cast<UNIT>(hptCurrent.y()),
						static_cast<UNIT>(nDeviation)));

		// Advance the Han point if measured against
		if (dTraversedCurrent == hptEndpoint.getDistance())
		{
			hptPrev = hptEndpoint;
			++iHPoint;
		}

		// Advance to the next stroke points
		ptdPrev = ptdEndpoint;
	}

	// Ensure comparison occurred at all points
CHECKPOINTS:
	if (iHPoint > vecHPoints.size() && iPoint <= _rgAcids.getEnd())
	{
		Genome::recordAttempt(ST_FILELINE, STTR_SCORING, "Stroke %ld ran out of Han points during deviation measure", (_id+1));
		Genome::recordTermination(NULL, STGT_SCORE, STGR_STROKE, "Stroke %ld ran out of Han points during deviation measure", (_id+1));
		return false;
	}
	if (iPoint > _rgAcids.getEnd() && iHPoint < vecHPoints.size())
	{
		Genome::recordAttempt(ST_FILELINE, STTR_SCORING, "Stroke %ld ran out of points during deviation measure", (_id+1));
		Genome::recordTermination(NULL, STGT_SCORE, STGR_STROKE, "Stroke %ld ran out of points during deviation measure", (_id+1));
		return false;
	}

	_nDeviation = ::sqrt(_nDeviation);
	TDATA(SCORING,L5,(LLTRACE, ""));
	return true;
}

/*
 * Function: determineStrokeOrientation
 *
 * The orientation selected, forward or reverse, is that which yields the smallest
 * deviation at the end points.
 */
const HPOINTARRAY&
Stroke::determineStrokeOrientation(const Gene& gene, const HStroke& hst)
{
	const POINTARRAY& vecPoints = gene.getPoints();

	Point ptStart(vecPoints[_rgAcids.getStart()-1]);
	ptStart.scale(_sxToHan, _syToHan);
	ptStart.move(_dxToHan, _dyToHan);

	Point ptEnd(vecPoints[_rgAcids.getEnd()]);
	ptEnd.scale(_sxToHan, _syToHan);
	ptEnd.move(_dxToHan, _dyToHan);

	const HPOINTARRAY& vecHPointsForward = hst.getPointsForward();
	Unit nDeviationForward = vecHPointsForward[0].squareDeviationFrom(ptStart);
	nDeviationForward += vecHPointsForward[vecHPointsForward.size()-1].squareDeviationFrom(ptEnd);

	const HPOINTARRAY& vecHPointsReverse = hst.getPointsReverse();
	Unit nDeviationReverse = vecHPointsReverse[0].squareDeviationFrom(ptStart);
	nDeviationReverse += vecHPointsReverse[vecHPointsReverse.size()-1].squareDeviationFrom(ptEnd);

	TDATA(SCORING,L4,(LLTRACE,
					"Stroke %ld is %s relative to the Han (length %6.9f)",
					(_id+1),
					(nDeviationForward < nDeviationReverse ? "forward" : "reverse"),
					hst.getLength()));

	return (nDeviationForward < nDeviationReverse
			? vecHPointsForward
			: vecHPointsReverse);
}

//--------------------------------------------------------------------------------
//
// Group
//
//--------------------------------------------------------------------------------

/*
 * Function: calcScore
 *
 */
bool
Group::calcScore(Gene& gene, const Han& han, const HGroup& hgrp)
{
	ENTER(SCORING,calcScore);

	ASSERT(_aryScoreExponents[SC_SCALE] == static_cast<UNIT>(0));
	ASSERT(_aryScoreExponents[SC_PLACEMENT] == static_cast<UNIT>(0));
	ASSERT(_aryScoreExponents[SC_DEVIATION] == static_cast<UNIT>(0));
	ASSERT(_aryScoreExponents[SC_EXTRALENGTH] == static_cast<UNIT>(0));
	ASSERT(_aryScoreExponents[SC_DROPOUTS] == static_cast<UNIT>(0));

	STROKEARRAY& vecStrokes = gene.getStrokes();
	const HSTROKEARRAY& vecHStrokes = han.getStrokes();
	const NUMERICARRAY& vecContainedHStrokes = hgrp.getStrokes();

	// Allow each Stroke to calculate its score values, accumulate working figures along the way
	Unit nTotalLength = 0;
	Unit dToHanError = 0;
	Unit dsxToHan = 0;
	Unit nxToHan = 0;
	Unit dsyToHan = 0;
	Unit nyToHan = 0;
	bool fSuccess = false;
	for (size_t iContainedStroke=0; iContainedStroke < vecContainedHStrokes.size(); ++iContainedStroke)
	{
		size_t iHStroke = vecContainedHStrokes[iContainedStroke];
		Stroke& st = vecStrokes[gene.mapHanToStroke(iHStroke)];
		const HStroke& hst = vecHStrokes[iHStroke];
		Unit dxToHanError;
		Unit dyToHanError;

		if (!st.calcScore(gene, hst))
			goto INVALID;

		// Accumulate gross placement error
		dxToHanError = _dxToHan - st.dxParentToHan();
		dyToHanError = _dyToHan - st.dyParentToHan();
		dToHanError += hst.getLength() * ((dxToHanError*dxToHanError) + (dyToHanError*dyToHanError));

		// Accumulate static values
		nTotalLength += hst.getLength();
		_aryScoreExponents[SC_DEVIATION] += st.deviation();
		_aryScoreExponents[SC_EXTRALENGTH] += st.extraLength();
		_aryScoreExponents[SC_DROPOUTS] += st.getDropouts();

		// Calculate gross error in scale factor consistency
		if (!st.sxIsInherited())
		{
			Unit dsx = _sxToHan - st.sxToHan();
			dsxToHan += hst.getBounds().getWidth() * (dsx*dsx);
			nxToHan += hst.getBounds().getWidth();
		}

		if (!st.syIsInherited())
		{
			Unit dsy = _syToHan - st.syToHan();
			dsyToHan += hst.getBounds().getHeight() * (dsy*dsy);
			nyToHan += hst.getBounds().getHeight();
		}
	}

	// Finally, convert working figures into weighted values for use as exponents
	dToHanError /= nTotalLength;
	_aryScoreExponents[SC_PLACEMENT] = ::sqrt(dToHanError) / (hgrp.getBounds().getWidth() + hgrp.getBounds().getHeight());

	_aryScoreExponents[SC_DEVIATION] /= nTotalLength;
	_aryScoreExponents[SC_EXTRALENGTH] /= nTotalLength;

	if (nxToHan)
		dsxToHan /= nxToHan;
	if (nyToHan)
		dsyToHan /= nyToHan;
	_aryScoreExponents[SC_SCALE] = (::sqrt(dsxToHan) / _sxToHan) + (::sqrt(dsyToHan) / _syToHan);

	_nScore = ::pow(0.5,
					((Globals::getGroupWeight(SC_SCALE) * std::abs(_aryScoreExponents[SC_SCALE]-Globals::getGroupSetpoint(SC_SCALE))) +
					 (Globals::getGroupWeight(SC_PLACEMENT) * std::abs(_aryScoreExponents[SC_PLACEMENT]-Globals::getGroupSetpoint(SC_PLACEMENT))) +
					 (Globals::getGroupWeight(SC_ILLEGALOVERLAPS) * std::abs(_aryScoreExponents[SC_ILLEGALOVERLAPS]-Globals::getGroupSetpoint(SC_ILLEGALOVERLAPS))) +
					 (Globals::getGroupWeight(SC_DEVIATION) * std::abs(_aryScoreExponents[SC_DEVIATION]-Globals::getGroupSetpoint(SC_DEVIATION))) +
					 (Globals::getGroupWeight(SC_EXTRALENGTH) * std::abs(_aryScoreExponents[SC_EXTRALENGTH]-Globals::getGroupSetpoint(SC_EXTRALENGTH))) +
					 (Globals::getGroupWeight(SC_MISSINGOVERLAPS) * std::abs(_aryScoreExponents[SC_MISSINGOVERLAPS]-Globals::getGroupSetpoint(SC_MISSINGOVERLAPS))) +
					 (Globals::getGroupWeight(SC_DROPOUTS) * std::abs(_aryScoreExponents[SC_DROPOUTS]-Globals::getGroupSetpoint(SC_DROPOUTS)))));

	fSuccess = true;
	
INVALID:
	return fSuccess;
}


//--------------------------------------------------------------------------------
//
// Gene
//
//--------------------------------------------------------------------------------

/*
 * Function: calcScore
 *
 */
bool
Gene::calcScore()
{
	ENTER(SCORING,calcScore);
	ASSERT(isInvalid(GI_SCORE));

	ASSERT(_aryScoreExponents[SC_SCALE] == static_cast<UNIT>(0));
	ASSERT(_aryScoreExponents[SC_PLACEMENT] == static_cast<UNIT>(0));
	ASSERT(_aryScoreExponents[SC_MARKS] == static_cast<UNIT>(0));

	const Han& han = Han::getDefinition(_strUnicode);
	const HGROUPARRAY& vecHGroups = han.getGroups();

	// First, accumulate overlap (join and intersection) errors
	scoreOverlaps(han);

	// Next, allow groups and strokes to calculate their respective scores
	Unit scoreGroup = 0;
	Unit dToHanError = 0;
	Unit dsxToHan = 0;
	Unit nxToHan = 0;
	Unit dsyToHan = 0;
	Unit nyToHan = 0;
	bool fSuccess = false;

	if (Globals::isGroupScoreMode(GSM_MINIMUM))
		scoreGroup.setToMax();

	for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
	{
		const HGroup& hgrp = vecHGroups[iGroup];
		Group& grp = _vecGroups[iGroup];

		if (!grp.calcScore(*this, han, hgrp))
			goto INVALID;

		// Accumulate gross placement error
		Unit dxToHanError = _dxToHan - grp.dxParentToHan();
		Unit dyToHanError = _dyToHan - grp.dyParentToHan();
		dToHanError += hgrp.getLength() * ((dxToHanError*dxToHanError) + (dyToHanError*dyToHanError));

		// Calculate the group score
		if (Globals::isGroupScoreMode(GSM_AVERAGE))
			scoreGroup += grp.score();
		else
		{
			ASSERT(Globals::isGroupScoreMode(GSM_MINIMUM));
			scoreGroup = min<UNIT>(grp.score(), scoreGroup);
		}

		// Calculate gross error in scale factor consistency
		if (!grp.sxIsInherited())
		{
			Unit dsx = _sxToHan - grp.sxToHan();
			dsxToHan += hgrp.getBounds().getWidth() * (dsx*dsx);
			nxToHan += hgrp.getBounds().getWidth();
		}

		if (!grp.syIsInherited())
		{
			Unit dsy = _syToHan - grp.syToHan();
			dsyToHan += hgrp.getBounds().getHeight() * (dsy*dsy);
			nyToHan += hgrp.getBounds().getHeight();
		}
	}

	if (Globals::isGroupScoreMode(GSM_AVERAGE))
		scoreGroup /= _vecGroups.size();

	// Next, convert working figures into weighted values for use as exponents
	dToHanError /= han.getLength();
	_aryScoreExponents[SC_PLACEMENT] = ::sqrt(dToHanError) / (han.getBounds().getWidth() + han.getBounds().getHeight());

	if (nxToHan)
		dsxToHan /= nxToHan;
	if (nyToHan)
		dsyToHan /= nyToHan;
	_aryScoreExponents[SC_SCALE] = (::sqrt(dsxToHan) / _sxToHan) + (::sqrt(dsyToHan) / _syToHan);

	// Finally, measure total length of all marks relative to the smallest Han stroke length
	if (_vecMarks.size())
	{
		ScaledLength al(_sxToHan, _syToHan, _sxyToHan);

		for (size_t iMark=0; iMark < _vecMarks.size(); ++iMark)
		{
			const Segment& sg = _vecSegments[_vecMarks[iMark]];

			for (long iAcid=sg.getStart(); iAcid <= sg.getEnd(); ++iAcid)
				al += _vecAcids[iAcid];
		}

		_aryScoreExponents[SC_MARKS] = al.getLength() / han.getMinimumStrokeLength();
	}

	// The gene score is the product of the group score and the gene level elements
	_nScore = scoreGroup * ::pow(0.5,
								((Globals::getGeneWeight(SC_SCALE) * std::abs(_aryScoreExponents[SC_SCALE]-Globals::getGeneSetpoint(SC_SCALE))) +
								(Globals::getGeneWeight(SC_PLACEMENT) * std::abs(_aryScoreExponents[SC_PLACEMENT]-Globals::getGeneSetpoint(SC_PLACEMENT))) +
								(Globals::getGeneWeight(SC_ILLEGALOVERLAPS) * std::abs(_aryScoreExponents[SC_ILLEGALOVERLAPS]-Globals::getGeneSetpoint(SC_ILLEGALOVERLAPS))) +
							 	(Globals::getGeneWeight(SC_MISSINGOVERLAPS) * std::abs(_aryScoreExponents[SC_MISSINGOVERLAPS]-Globals::getGeneSetpoint(SC_MISSINGOVERLAPS))) +
								(Globals::getGeneWeight(SC_MARKS) * std::abs(_aryScoreExponents[SC_MARKS]-Globals::getGeneSetpoint(SC_MARKS)))));

	fSuccess = true;

INVALID:
	return fSuccess;
}

/*
 * Function: scoreOverlaps
 *
 * Examine the detected stroke overlaps and score the appropriate groups or the gene
 * based on whether the overlap is illegal, allowed, or unexpected.
 * 
 * Note:
 * The current routine takes no account of where an overlap occurs when scoring. Suppose two strokes could
 * have both a required and an optional overlap. This routine will assume the first discovered overlap is
 * the required overlap (since if it is missing, there is a penalty). An improvement would take into account
 * where the overlaps occur and assign them accordingly. The best approach would be to, in the Han, locate
 * each possible overlap within a grid square - choosing a grid granularity such that no two strokes ever
 * overlap more than once within a single grid square. Then, the grid would be scaled to fit the gene
 * and assigned to overlaps based on their point location. Assignment would then take into account grid
 * square location as well as the strokes involved. (If two strokes overlap multiple times within a grid
 * square, scoring should take that approach which assigns the smallest penalty.)
 */
void
Gene::scoreOverlaps(const Han& han)
{
	ENTER(SCORING,scoreOverlaps);
	ASSERT(isInvalid(GI_SCORE));

	ASSERT(_aryScoreExponents[SC_ILLEGALOVERLAPS] == static_cast<UNIT>(0));

	if (_setOverlaps.size() == 0 && _vecHOverlaps.size() == 0)
		return;

	ASSERT(_vecMissingOverlaps.empty());
	ASSERT(_vecIllegalOverlaps.empty());

	STROKEOVERLAPS::iterator itOverlap = _setOverlaps.begin();
	size_t iHOverlap = 0;

	TDATA(SCORING,L4,(LLTRACE, "Scoring Overlaps: %ld possible (han) vs. %ld actual", _vecHOverlaps.size(), _setOverlaps.size()));
	while (iHOverlap < _vecHOverlaps.size() && itOverlap != _setOverlaps.end())
	{
		const STROKEOVERLAP& ol = *itOverlap;
		const HOverlap& hol = _vecHOverlaps[iHOverlap];
		int nCompare = hol.compare(ol);

		TDATA(SCORING,L4,(LLTRACE, "Compare Overlaps: (%ld,%ld) (han) vs. (%ld,%ld) - Compared as %d", hol.getFirst(), hol.getSecond(), ol.getFirst(), ol.getSecond(), nCompare));

		// A matching overlap
		if (!nCompare)
		{
			++itOverlap;
			++iHOverlap;
		}

		// A missing overlap
		else if (nCompare < 0)
		{
			if (hol.isRequired())
			{
				TDATA(SCORING,L4,(LLTRACE, "Overlap (%ld,%ld) flagged as missing", hol.getFirst(), hol.getSecond()));
				_vecMissingOverlaps.push_back(make_pair(hol.getFirst(), hol.getSecond()));
			}
			++iHOverlap;
		}

		// An illegal overlap
		else if (nCompare > 0)
		{
			TDATA(SCORING,L4,(LLTRACE, "Overlap (%ld,%ld) flagged as illegal", ol.getFirst(), ol.getSecond()));
			_vecIllegalOverlaps.push_back(make_pair(ol.getFirst(), ol.getSecond()));
			++itOverlap;
		}
	}

	// Accumulate any remaining Gene overlaps as illegal overlaps
	for (; itOverlap != _setOverlaps.end(); ++itOverlap)
	{
		TDATA(SCORING,L4,(LLTRACE, "Overlap (%ld,%ld) flagged as illegal", (*itOverlap).getFirst(), (*itOverlap).getSecond()));
		_vecIllegalOverlaps.push_back(make_pair((*itOverlap).getFirst(), (*itOverlap).getSecond()));
	}

	// Accumulate any remaining required Han overlaps as missing overlaps
	for (; iHOverlap < _vecHOverlaps.size(); ++iHOverlap)
	{
		const HOverlap& hol = _vecHOverlaps[iHOverlap];
		if (hol.isRequired())
		{
			TDATA(SCORING,L4,(LLTRACE, "Overlap (%ld,%ld) flagged as missing", hol.getFirst(), hol.getSecond()));
			_vecMissingOverlaps.push_back(make_pair(hol.getFirst(), hol.getSecond()));
		}
	}

	TDATA(SCORING,L3,(LLTRACE, "Overlap Errors: missing(%ld) illegal(%ld)", _vecMissingOverlaps.size(), _vecIllegalOverlaps.size()));
	TRACEDOIF(SCORING,DATA,L3,traceOverlaps());

	// Finally, apply to the appropriate groups and the gene any encountered errors
	for (size_t iMissing=0; iMissing < _vecMissingOverlaps.size(); ++iMissing)
	{
		const NUMERICPAIR& np = _vecMissingOverlaps[iMissing];
		if (_mapStrokeToGroup[np.first] == _mapStrokeToGroup[np.second])
			_vecGroups[_mapStrokeToGroup[np.first]].incMissingOverlaps();
		else
			++_aryScoreExponents[SC_MISSINGOVERLAPS];
	}

	for (size_t iIllegal=0; iIllegal < _vecIllegalOverlaps.size(); ++iIllegal)
	{
		const NUMERICPAIR& np = _vecIllegalOverlaps[iIllegal];
		if (_mapStrokeToGroup[np.first] == _mapStrokeToGroup[np.second])
			_vecGroups[_mapStrokeToGroup[np.first]].incIllegalOverlaps();
		else
			++_aryScoreExponents[SC_ILLEGALOVERLAPS];
	}
}
