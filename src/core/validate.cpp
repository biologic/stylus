/*******************************************************************************
 * \file	validate.cpp
 * \brief	Stylus Gene class (validation methods)
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
 * Function: calcDimensions
 *
 */
void
Stroke::calcDimensions(Gene& gene)
{
	ENTER(VALIDATION,calcDimensions);

	const ACIDTYPEARRAY& vecAcids = gene.getAcids();
	const POINTARRAY& vecPoints = gene.getPoints();
	long iAcid = _rgAcids.getStart()-1;

	ASSERT(iAcid >= 0);

	// Strokes begin where the previous vector ends
	Point ptTopLeft(vecPoints[iAcid]);
	Point ptBottomRight(vecPoints[iAcid]);

	// Sum the lengths of vectors contained in the stroke
	ASSERT(_slVectors.getLength() == 0);
	for (++iAcid; iAcid <= _rgAcids.getEnd(); ++iAcid)
	{
		const Point& pt = vecPoints[iAcid];
		if (pt.x() < ptTopLeft.x())
			ptTopLeft.x() = pt.x();
		if (pt.y() > ptTopLeft.y())
			ptTopLeft.y() = pt.y();
		if (pt.x() > ptBottomRight.x())
			ptBottomRight.x() = pt.x();
		if (pt.y() < ptBottomRight.y())
			ptBottomRight.y() = pt.y();

		_slVectors += vecAcids[iAcid];
	}

	// Set the bounds to the extreme top/left and bottom/right encountered
	_rectBounds.set(ptTopLeft, ptBottomRight);
	TDATA(VALIDATION,L3,(LLTRACE, "Stroke %ld has bounds %s", (_id+1), _rectBounds.toString().c_str()));
}

/*
 * Function: calcScale
 *
 */
bool
Stroke::calcScale(Gene& gene, const HStroke& hst)
{
	ENTER(VALIDATION,calcScale);

	bool fDimensionMissing = false;
	const Rectangle& rectHBounds = hst.getBounds();

	// Calculate the x/y scale factors
	// - If either the stroke or Han is missing profile along a dimension, skip it
	//   (the stroke will eventually inherit the scale factor from its containing group)
	if (rectHBounds.getWidth() > 0 && _rectBounds.getWidth() > 0)
		_sxToHan = rectHBounds.getWidth() / _rectBounds.getWidth();
	else
	{
		fDimensionMissing = true;
		_sxToHan.setUndefined();
	}

	if (rectHBounds.getHeight() > 0 && _rectBounds.getHeight() > 0)
		_syToHan = rectHBounds.getHeight() / _rectBounds.getHeight();
	else
	{
		fDimensionMissing = true;
		_syToHan.setUndefined();
	}

	// Calculate the scale factor used with diagonal lengths
	if (!fDimensionMissing)
		_sxyToHan = ::sqrt((_sxToHan*_sxToHan)+(_syToHan*_syToHan));
	else
		_sxyToHan.setUndefined();

	TDATA(VALIDATION,L3,(LLTRACE,
						 "Stroke %ld has sx/sy/sxy(%6.15f,%6.15f,%6.15f)",
						 (_id+1),
						 static_cast<UNIT>(_sxToHan),
						 static_cast<UNIT>(_syToHan),
						 static_cast<UNIT>(_sxyToHan)));

	ASSERT(fDimensionMissing || (_sxToHan.isDefined() && _syToHan.isDefined() && _sxyToHan.isDefined()));
	return fDimensionMissing;
}

/*
 * Function: promoteScale
 *
 */
void
Stroke::promoteScale(UNIT sxToHan, UNIT syToHan)
{
	ENTER(VALIDATION,promoteScale);

	// If a dimension lacked profile, take the passed scale for the dimension
	if (!_sxToHan.isDefined())
	{
		TDATA(VALIDATION,L3,(LLTRACE, "Stroke %ld taking sxToHan(%f) from Group", (_id+1), sxToHan));
		_sxIsInherited = true;
		_sxToHan = sxToHan;
	}
	if (!_syToHan.isDefined())
	{
		TDATA(VALIDATION,L3,(LLTRACE, "Stroke %ld taking syToHan(%f) from Group", (_id+1), syToHan));
		_syIsInherited = true;
		_syToHan = syToHan;
	}
	if (!_sxyToHan.isDefined())
		_sxyToHan = ::sqrt((_sxToHan*_sxToHan)+(_syToHan*_syToHan));
}

/*
 * Function: calcOffsets
 *
 */
void
Stroke::calcOffsets(Group& grp, const HStroke& hst)
{
	ENTER(VALIDATION,calcOffsets);

	const Rectangle& rectHBounds = hst.getBounds();
	
	_dxToHan = rectHBounds.getCenter().x() - (_rectBounds.getCenter().x() * _sxToHan);
	_dyToHan = rectHBounds.getCenter().y() - (_rectBounds.getCenter().y() * _syToHan);

	_dxParentToHan = rectHBounds.getCenter().x() - (_rectBounds.getCenter().x() * grp.sxToHan());
	_dyParentToHan = rectHBounds.getCenter().y() - (_rectBounds.getCenter().y() * grp.syToHan());

	TDATA(VALIDATION,L3,(LLTRACE,
						 "Stroke %ld has dx/dy(%6.15f,%6.15f) dx/dyParent(%6.15f,%6.15f)",
						 (_id+1),
						 static_cast<UNIT>(_dxToHan),
						 static_cast<UNIT>(_dyToHan),
						 static_cast<UNIT>(_dxParentToHan),
						 static_cast<UNIT>(_dyParentToHan)));
}

//--------------------------------------------------------------------------------
//
// Group
//
//--------------------------------------------------------------------------------

/*
 * Function: calcDimensions
 *
 */
void
Group::calcDimensions(Gene& gene, const HGroup& hgrp)
{
	ENTER(VALIDATION,calcDimensions);

	STROKEARRAY& vecStrokes = gene.getStrokes();
	const NUMERICARRAY& vecContainedHStrokes = hgrp.getStrokes();

	ASSERT(vecContainedHStrokes.size() > 0);

	// Set group bounds to the union of all contained strokes
	for (size_t iContainedStroke=0; iContainedStroke < vecContainedHStrokes.size(); ++iContainedStroke)
	{
		Stroke& st = vecStrokes[gene.mapHanToStroke(vecContainedHStrokes[iContainedStroke])];
		st.calcDimensions(gene);
		if (iContainedStroke == 0)
			_rectBounds = st.getBounds();
		else
			_rectBounds.combine(st.getBounds());
	}
	TDATA(VALIDATION,L3,(LLTRACE, "Group %ld has bounds %s", (_id+1), _rectBounds.toString().c_str()));
}

/*
 * Function: calcScale
 *
 */
bool
Group::calcScale(Gene& gene, const Han& han, const HGroup& hgrp)
{
	ENTER(VALIDATION,calcScale);

	STROKEARRAY& vecStrokes = gene.getStrokes();
	const HSTROKEARRAY& vecHStrokes = han.getStrokes();
	const NUMERICARRAY& vecContainedHStrokes = hgrp.getStrokes();

	Unit nxToHan = 0;
	Unit nyToHan = 0;
	Unit sxToHan = 0;
	Unit syToHan = 0;

	bool fDimensionMissing = false;

	// Compute the mean sx/sy scale factors for the contained strokes
	// - Only strokes with profile along a dimension contribute to the mean
	for (size_t iContainedStroke=0; iContainedStroke < vecContainedHStrokes.size(); ++iContainedStroke)
	{
		size_t iHStroke = vecContainedHStrokes[iContainedStroke];
		Stroke& st = vecStrokes[gene.mapHanToStroke(iHStroke)];
		const HStroke& hst = vecHStrokes[iHStroke];

		fDimensionMissing = st.calcScale(gene, hst) || fDimensionMissing;

		if (hst.getBounds().getWidth() > 0 && st.getBounds().getWidth() > 0)
		{
			sxToHan += hst.getBounds().getWidth() * st.sxToHan();
			nxToHan += hst.getBounds().getWidth();
		}
		else
			fDimensionMissing = true;

		if (hst.getBounds().getHeight() > 0 && st.getBounds().getHeight() > 0)
		{
			syToHan += hst.getBounds().getHeight() * st.syToHan();
			nyToHan += hst.getBounds().getHeight();
		}
		else
			fDimensionMissing = true;
	}

	if (nxToHan > static_cast<UNIT>(0.0))
		_sxToHan = sxToHan / nxToHan;
	else
	{
		fDimensionMissing = true;
		_sxToHan.setUndefined();
	}

	if (nyToHan > static_cast<UNIT>(0.0))
		_syToHan = syToHan / nyToHan;
	else
	{
		fDimensionMissing = true;
		_syToHan.setUndefined();
	}

	ASSERT(fDimensionMissing || (_sxToHan.isDefined() && _syToHan.isDefined()));
	TDATA(VALIDATION,L3,(LLTRACE,
						 "Group %ld has sx/sy(%6.15f,%6.15f)",
						 (_id+1),
						 static_cast<UNIT>(_sxToHan),
						 static_cast<UNIT>(_syToHan)));
	return fDimensionMissing;
}

/*
 * Function: promoteScale
 *
 */
void
Group::promoteScale(Gene& gene, const HGroup& hgrp, UNIT sxToHan, UNIT syToHan)
{
	ENTER(VALIDATION,promoteScale);

	STROKEARRAY& vecStrokes = gene.getStrokes();
	const NUMERICARRAY& vecContainedHStrokes = hgrp.getStrokes();

	// If a dimension lacked profile, take the passed scale for the dimension
	if (!_sxToHan.isDefined())
	{
		TDATA(VALIDATION,L3,(LLTRACE, "Group %ld taking sxToHan(%f) from Gene", (_id+1), sxToHan));
		_sxIsInherited = true;
		_sxToHan = sxToHan;
	}
	if (!_syToHan.isDefined())
	{
		TDATA(VALIDATION,L3,(LLTRACE, "Group %ld taking syToHan(%f) from Gene", (_id+1), syToHan));
		_syIsInherited = true;
		_syToHan = syToHan;
	}

	// Promote the group scale factors to any strokes missing scale factors
	TDATA(VALIDATION,L3,(LLTRACE,
						 "Group %ld promoting sx/sy(%f,%f) to strokes",
						 (_id+1), static_cast<UNIT>(_sxToHan), static_cast<UNIT>(_syToHan)));
	for (size_t iContainedStroke=0; iContainedStroke < vecContainedHStrokes.size(); ++iContainedStroke)
	{
		vecStrokes[gene.mapHanToStroke(vecContainedHStrokes[iContainedStroke])].promoteScale(_sxToHan, _syToHan);
	}
}

/*
 * Function: calcOffsets
 *
 */
void
Group::calcOffsets(Gene& gene, const Han& han, const HGroup& hgrp)
{
	ENTER(VALIDATION,calcOffsets);

	ASSERT(_sxToHan.isDefined());
	ASSERT(_syToHan.isDefined());
	ASSERT(Unit(gene.sxToHan()).isDefined());
	ASSERT(Unit(gene.syToHan()).isDefined());

	ASSERT(_dxToHan == static_cast<UNIT>(0.0));
	ASSERT(_dyToHan == static_cast<UNIT>(0.0));

	STROKEARRAY& vecStrokes = gene.getStrokes();
	const HSTROKEARRAY& vecHStrokes = han.getStrokes();
	const NUMERICARRAY& vecContainedHStrokes = hgrp.getStrokes();

	Unit dxToHan = 0.0;
	Unit dyToHan = 0.0;

	// Allow each stroke to determine its dx/dy translation offsets and accumulate the weighted sum of their centers
	for (size_t iContainedStroke=0; iContainedStroke < vecContainedHStrokes.size(); ++iContainedStroke)
	{
		size_t iHStroke = vecContainedHStrokes[iContainedStroke];
		Stroke& st = vecStrokes[gene.mapHanToStroke(iHStroke)];
		const HStroke& hst = vecHStrokes[iHStroke];

		st.calcOffsets(*this, hst);

		const Point& ptStrokeCenter = st.getBounds().getCenter();
		dxToHan += hst.getLength() * ptStrokeCenter.x();
		dyToHan += hst.getLength() * ptStrokeCenter.y();
	}

	// Convert the weighted sum into the dx/dy for the group by measuring it
	// against the weighted center of the Han group
	dxToHan /= hgrp.getLength();
	dyToHan /= hgrp.getLength();

	const Point& ptHanWeightedCenter = hgrp.getWeightedCenter();
	_dxToHan = ptHanWeightedCenter.x() - (dxToHan * _sxToHan);
	_dyToHan = ptHanWeightedCenter.y() - (dyToHan * _syToHan);

	_dxParentToHan = ptHanWeightedCenter.x() - (dxToHan * gene.sxToHan());
	_dyParentToHan = ptHanWeightedCenter.y() - (dyToHan * gene.syToHan());

	TDATA(VALIDATION,L3,(LLTRACE,
						 "Group %ld has dx/dy(%6.15f,%6.15f) dx/dyParent(%6.15f,%6.15f)",
						 (_id+1),
						 static_cast<UNIT>(_dxToHan),
						 static_cast<UNIT>(_dyToHan),
						 static_cast<UNIT>(_dxParentToHan),
						 static_cast<UNIT>(_dyParentToHan)));
}

//--------------------------------------------------------------------------------
//
// StrokeRangeChange
//
//--------------------------------------------------------------------------------

/*
 * Function: StrokeRangeChange
 *
 */
StrokeRangeChange::StrokeRangeChange(size_t idGene, const vector<Range>& vecStrokeRanges)
{
	ASSERT(!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING));

	_idGene = idGene;
	_vecStrokeRanges = vecStrokeRanges;
}

/*
 * Function: undoChange
 *
 */
void
StrokeRangeChange::undo()
{
	ENTER(MUTATION,undoChange);
	TFLOW(MUTATION,L4,(LLTRACE, "Undoing %s", toString().c_str()));

	ASSERT(Genome::isState(STGS_ROLLBACK) || Genome::isState(STGS_RESTORING));

	Gene& gene = Genome::getGeneById(_idGene);
	STROKEARRAY& vecStrokes = gene.getStrokes();

	ASSERT(_vecStrokeRanges.size() == vecStrokes.size());

	for (size_t iStroke=0; iStroke < vecStrokes.size(); ++iStroke)
		vecStrokes[iStroke].setRange(_vecStrokeRanges[iStroke]);

	gene.markInvalid(Gene::GI_STROKES);
}

/*
 * Function: toString
 *
 */
string
StrokeRangeChange::toString() const
{
	ENTER(MUTATION,toString);

	ostringstream ostr;

	ostr << "Stroke Ranges changed from: ";
	for (size_t iStroke=0; iStroke < _vecStrokeRanges.size(); ++iStroke)
	{
		ostr << iStroke << _vecStrokeRanges[iStroke].toString();
		if (iStroke < _vecStrokeRanges.size()-1)
			ostr << Constants::s_chBLANK;
	}

	return ostr.str();
}


//--------------------------------------------------------------------------------
//
// Gene
//
//--------------------------------------------------------------------------------

/*
 * Function: ensureAcids
 *
 * Compile the codons into ACIDTYPEs and establish the point at the end
 * of each vector (the start vector is located at _ptOrigin).
 */
void
Gene::ensureAcids(size_t iAcidChange, long cAcidsChanged)
{
	ENTER(VALIDATION,ensureAcids);

	ASSERT(iAcidChange == 0 || !Genome::isState(STGS_VALIDATING));
	ASSERT(iAcidChange == 0 || static_cast<long>(_vecAcids.size()) == Codon::numWholeCodons(_rgBases.getLength()));
	ASSERT(iAcidChange == 0 || _vecAcids.size() == _vecPoints.size());

	if (_vecAcids.empty())
	{
		size_t cAcids = Codon::numWholeCodons(_rgBases.getLength());
		ASSERT(_rgBases.isEmpty() || cAcids >= 2);

		_vecAcids.resize(cAcids);
		_vecPoints.resize(cAcids);

		iAcidChange = 0;
		cAcidsChanged = _vecAcids.size();
	}

	TFLOW(VALIDATION,L3,(LLTRACE, "Creating acids from %ld for %ld codons", (iAcidChange+1), cAcidsChanged));

	ASSERT(_vecAcids.size() >= 2);
	ASSERT(_vecAcids.size() == _vecPoints.size());
	ASSERT(_vecAcids.size() == static_cast<size_t>(_rgBases.getLength() / Codon::s_cchCODON));

	// If acids were added, convert the corresponding bases to ACIDTYPEs
	if (cAcidsChanged > 0)
	{
		const char* pszBases = Genome::getBases().c_str();
		size_t iBase = _rgBases.getStart() + (iAcidChange * Codon::s_cchCODON);
		size_t iAcid = iAcidChange;
		size_t iAcidEnd = iAcidChange + cAcidsChanged;

		for (; iAcid < iAcidEnd; iBase += Codon::s_cchCODON, ++iAcid)
			_vecAcids[iAcid] = Genome::codonToType(pszBases+iBase);
	}

	// Calculate the points associated with each acid from the point of change onward
	// - The point associated with the first codon is the origin of the gene; all others
	//   are the value after applying the acid (that is, the start codon is treated as
	//   a zero-length acid)
	Point pt;

	size_t iAcid = iAcidChange;
	if (iAcid <= 0)
	{
		_vecPoints[0] = _ptOrigin;
		iAcid++;
	}

	pt = _vecPoints[iAcid-1];
	for (; iAcid < _vecPoints.size(); ++iAcid)
	{
		pt += _vecAcids[iAcid];
		_vecPoints[iAcid] = pt;
	}
	
	// Calculate the total vector length of the gene
	// - The length excludes the start and stop codons
	_nUnits = 0;
	for (iAcid=1; iAcid < _vecAcids.size()-1; iAcid++)
		_nUnits += Acid::typeToAcid(_vecAcids[iAcid]).getLength();
	TDATA(VALIDATION,L3,(LLTRACE, "Gene has %0.15f total units", static_cast<UNIT>(_nUnits)));

	markValid(GI_ACIDS | GI_POINTS);
}

/*
 * Function: ensureCoherence
 *
 * Determine the coherence of each vector by examining all possible trivectors.
 * For each vector is a coherence count, ranging from 0 to 3, indicating how
 * many trivectors within which it was found.
 */
void
Gene::ensureCoherence()
{
	ENTER(VALIDATION,ensureCoherence);

	ASSERT(isValid(GI_ACIDS));
	ASSERT(_vecAcids.size() >= Codon::s_nTRIVECTOR+2);

	if (_vecCoherent.empty())
		_vecCoherent.resize(_vecAcids.size());

	ASSERT(_vecCoherent.size() >= Codon::s_nTRIVECTOR+2);
	ASSERT(_vecAcids.size() == _vecCoherent.size());
	ASSERT(Acid::typeToAcid(_vecAcids[_vecAcids.size()-1]).isStop());

	_vecCoherent[0] = 0;
	_vecCoherent[1] = 0;
	_vecCoherent[2] = 0;

	for (long iAcid=1; !Acid::typeToAcid(_vecAcids[iAcid+2]).isStop(); ++iAcid)
	{
		_vecCoherent[iAcid+2] = 0;
		if (Codon::isCoherent(_vecAcids[iAcid+0], _vecAcids[iAcid+1], _vecAcids[iAcid+2]))
		{
			++_vecCoherent[iAcid+0];
			++_vecCoherent[iAcid+1];
			++_vecCoherent[iAcid+2];
		}
	}

	markValid(GI_COHERENCE);
}

/*
 * Function: ensureSegments
 *
 * A segment is a continuous range of coherent or incoherent vectors.
 * This routine walks the coherence array, building as many segments
 * as necessary.
 *
 * NOTES:
 * - Strokes could be built up from the coherence array without directly
 *   using the notion of segments; however, since strokes deal with coherent
 *   ranges, first dividing the vectors into segments makes sense
 */
void
Gene::ensureSegments()
{
	ENTER(VALIDATION,ensureSegments);

	ASSERT(isValid(GI_ACIDS | GI_COHERENCE));
	ASSERT(_vecAcids.size() >= Codon::s_nTRIVECTOR+2);
	ASSERT(_vecCoherent.size() >= Codon::s_nTRIVECTOR+2);
	ASSERT(_vecAcids.size() == _vecCoherent.size());
	ASSERT(Acid::typeToAcid(_vecAcids[_vecAcids.size()-1]).isStop());

	bool fWasCoherent = !(_vecCoherent[1] > 0);
	long iSegment = -1;

	_vecSegments.clear();

	// Iterate across all vectors between the start and stop codon
	for (long iAcid=1; !Acid::typeToAcid(_vecAcids[iAcid]).isStop(); ++iAcid)
	{
		bool fIsCoherent = (_vecCoherent[iAcid] > 0);
		if (fWasCoherent != fIsCoherent)
		{
			fWasCoherent = fIsCoherent;
			++iSegment;
			_vecSegments.resize(iSegment+1);
			if (fIsCoherent)
				++_cCoherent;

			Segment& sg = _vecSegments.back();
			sg.setRange(Range(iAcid,iAcid));
			sg.setCoherent(fIsCoherent);
			sg = _vecAcids[iAcid];
		}

		else
		{
			Segment& sg = _vecSegments.back();
			sg += _vecAcids[iAcid];
			sg.setEnd(iAcid);
		}
	}

	// Since, coherent and incoherent segments alternate,
	// their absolute difference should never exceed one
	ASSERT(::abs(static_cast<size_t>(_vecSegments.size() - (2 * _cCoherent))) <= 1);

	TFLOW(VALIDATION,L3,(LLTRACE, "%d segments created", _vecSegments.size()));
	TRACEDOIF(VALIDATION,DATA,L3,traceSegments());

	markValid(GI_SEGMENTS);
}

/*
 * Function: ensureStrokes
 *
 * A stroke is a sequence of coherent vectors possibly separated by small,
 * incoherent regions (called "dropouts"). Generally, this routine maps the
 * coherent segments to the appropriate stroke (or treats them as strokes).
 * Coherent segments occurring outside of any stroke are referred to as
 * "marks".
 */
void
Gene::ensureStrokes()
{
	ENTER(VALIDATION,ensureStrokes);

	vector<Range> vecStrokeRanges(_vecStrokes.size());
	NUMERICMAP vecPotentialStrokeSegments;
	size_t iStroke = 0;
	size_t iSegment = 0;

	// NOTE:
	// - For now, stroke locations must be assigned with the Han glyph.
	//   Stroke discovery is possible, but requires mapping strokes to the Han
	//   glyph while preserving the notion of dropouts and marks.
	ASSERT(_fStrokesAssigned);
	if (!_fStrokesAssigned)
		THROWRC((RC(ERROR), "Gene requires that stroke locations be assigned with the Han glyph"));

	// Each stroke requires at least one coherent segment
	if (_cCoherent < _vecStrokes.size())
	{
		Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION,
							"Gene has too few coherent segments (%ld) for strokes (%ld)",
							 _cCoherent, _vecStrokes.size());
		Genome::recordTermination(NULL, STGT_VALIDATION, STGR_STROKES,
							"Gene has too few coherent segments (%ld) for strokes (%ld)",
							 _cCoherent, _vecStrokes.size());
		goto INVALID;
	}
	TFLOW(VALIDATION,L3,(LLTRACE,
						 "Assigning %ld segments to %ld strokes",
						 _vecSegments.size(), _vecStrokes.size()));

	// Save the current stroke ranges and ensure they are initially valid
	for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
	{
		Stroke& st = _vecStrokes[iStroke];
		if (st.getStart() <= 0 || st.getEnd() >= static_cast<long>(_vecAcids.size())-1 || st.getRange().getLength() < Codon::s_nTRIVECTOR)
		{
			Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION,
							"Stroke %ld has an invalid range of %s", (iStroke+1), st.getRange().toString().c_str());
			Genome::recordTermination(NULL, STGT_VALIDATION, STGR_STROKES,
							"Stroke %ld has an invalid range of %s", (iStroke+1), st.getRange().toString().c_str());
			goto INVALID;
		}
		vecStrokeRanges[iStroke] = st.getRange();
		TDATA(VALIDATION,L4,(LLTRACE, "Stroke %ld has an expected range of %s", (iStroke+1), st.getRange().toString().c_str()));
	}

	//--------------------------------------------------------------------------------------------
	// Generally, coherent segments map to strokes and incoherent segments to the moves between
	// strokes. However, allowing for small incoherent regions within strokes (dropouts) and
	// shifting stroke boundaries complicates matters. The relationship between a segment and a
	// stroke is characterized by their endpoints; the Stroke 'S' and the Segments 'A', 'B',
	// 'C', 'D', 'E' and 'F'...
	// 
	//                     |---- A ----|                           |---- F ----|
	//                               |---- B ----|       |---- E ----|
	//                                   |---------- C ----------|
	//                                         |---- D ----|
	//                                     |-------- S --------|
	//
	// ... have the following relationships:
	//
	//        fStartBefore fStartContained fStartAfter fEndBefore fEndContained fEndAfter
	//    A         x                                      x
	//    B         x                                                 x
	//    C         x                                                               x
	//    D                      x                                    x
	//    E                      x                                                  x
	//    F                                      x                                  x
	//
	//--------------------------------------------------------------------------------------------

	// Walk the segment array and determine stroke locations as long as strokes exist
	while (iSegment < _vecSegments.size() && iStroke < _vecStrokes.size())
	{
		const Segment& sg = _vecSegments[iSegment];
		Stroke& st = _vecStrokes[iStroke];
		bool fStrokeStarted = false;

		// Adjoining segments can never have the same coherency
		ASSERT(		iSegment >= _vecSegments.size()-1
			  ||	_vecSegments[iSegment+1].isCoherent() != sg.isCoherent());

		// Coherent segments can never be smaller than a trivector
		ASSERT(		!sg.isCoherent()
			  ||	sg.getRange().getLength() >= Codon::s_nTRIVECTOR);

		// Determine endpoint relationships (see comments above)
		bool fStartBefore = (st.getStart() - sg.getStart()) > 0;
		bool fStartAfter  = (sg.getStart() - st.getEnd()) > 0;

		bool fEndBefore   = (st.getStart() - sg.getEnd()) > 0;
		bool fEndAfter    = (sg.getEnd() - st.getEnd()) > 0;

		ASSERT(!(fStartBefore && fStartAfter));
		ASSERT(!(fEndBefore && fEndAfter));
		ASSERT(!fEndBefore || fStartBefore);
		ASSERT(!fStartAfter || fEndAfter);

		bool fStartContained = (!fStartBefore && !fStartAfter);
		bool fEndContained   = (!fEndBefore && !fEndAfter);

		bool fAdvanceSegment = true;
		bool fAdvanceStroke = false;
		
		// Handle each segment/stroke case (see above), adjusting the stroke boundaries as needed
		// - For coherent segments, stroke boundaries are extend to co-extensive with the segment
		// - For incoherent segments, stroke boundaries are extend (in the appropriate direction)
		//   one past the segment to either avoid the segment or reach past it into the next
		//   coherent segment

		// Case A: Segment lies wholly before the stroke
		if (fEndBefore)
		{
			ASSERT(!st.getSegments());
			if (sg.isCoherent())
			{
				if (	iSegment < _vecSegments.size()-2
					&&	_vecSegments[iSegment+1].getRange().getLength() <= s_cDROPOUT)
					vecPotentialStrokeSegments.push_back(iSegment);
				else
				{
					for (size_t iMark=0; iMark < vecPotentialStrokeSegments.size(); ++iMark)
					{
						_vecMarks.push_back(vecPotentialStrokeSegments[iMark]);
						TFLOW(VALIDATION,L4,(LLTRACE, "CASE A: Segment %ld is a mark and lies before stroke %ld", (vecPotentialStrokeSegments[iMark]+1), (iStroke+1)));
					}
					vecPotentialStrokeSegments.clear();
					_vecMarks.push_back(iSegment);
					TFLOW(VALIDATION,L4,(LLTRACE, "CASE A: Segment %ld is a mark and lies before stroke %ld", (iSegment+1), (iStroke+1)));
				}
			}
		}

		// Case B: Segment overlaps stroke start
		else if (fStartBefore && fEndContained)
		{
			ASSERT(!st.getSegments());
			if (sg.isCoherent())
			{
				st.setStart(sg.getStart());
				st.incSegments();
				fStrokeStarted = true;
			}
			else
			{
				st.setEnd(max<long>(sg.getEnd()+1, st.getEnd()));
				st.setStart(sg.getEnd()+1);
			}
			TFLOW(VALIDATION,L4,(LLTRACE, "CASE B: Segment %ld set the start for stroke %ld", (iSegment+1), (iStroke+1)));
		}

		// Case C: Segment overlaps entire stroke
		else if (fStartBefore && fEndAfter)
		{
			ASSERT(!st.getSegments());
			if (sg.isCoherent())
			{
				st.setRange(sg.getRange());
				st.incSegments();
				fStrokeStarted = true;
				TFLOW(VALIDATION,L4,(LLTRACE, "CASE C: Segment %ld set the range for stroke %ld", (iSegment+1), (iStroke+1)));
			}
			else
			{
				Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION,
									"Stroke %ld lost to incoherent segment %ld %s",
				 					(iStroke+1), (iSegment+1), sg.getRange().toString().c_str());
				Genome::recordTermination(NULL, STGT_VALIDATION, STGR_STROKES,
									"Stroke %ld lost to incoherent segment %ld %s",
				 					(iStroke+1), (iSegment+1), sg.getRange().toString().c_str());
				goto RECORDUNDO;
			}
		}

		// Case D: Segment is wholly contained in the stroke
		// Case E: Segment overlaps stroke end
		else if (fStartContained && (fEndContained || fEndAfter))
		{
			if (sg.isCoherent())
			{
				if (!st.getSegments())
				{
					st.setStart(sg.getStart());
					fStrokeStarted = true;
					TFLOW(VALIDATION,L4,(LLTRACE, "CASE D/E: Segment %ld set the start for stroke %ld", (iSegment+1), (iStroke+1)));
				}
				if (fEndAfter)
				{
					st.setEnd(sg.getEnd());
					TFLOW(VALIDATION,L4,(LLTRACE, "CASE D/E: Segment %ld set the end for stroke %ld", (iSegment+1), (iStroke+1)));
				}
				st.incSegments();
			}
			else
			{
				if (!st.getSegments())
				{
					if (fEndAfter)
					{
						Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION,
											 "Stroke %ld lost to incoherent segment %ld %s",
											 (iStroke+1), (iSegment+1), sg.getRange().toString().c_str());
						Genome::recordTermination(NULL, STGT_VALIDATION, STGR_STROKES,
											"Stroke %ld lost to incoherent segment %ld %s",
						 					(iStroke+1), (iSegment+1), sg.getRange().toString().c_str());
						goto RECORDUNDO;
					}
					st.setEnd(max<long>(sg.getEnd()+1, st.getEnd()));
					st.setStart(sg.getEnd()+1);
					TFLOW(VALIDATION,L4,(LLTRACE, "CASE D/E: Segment %ld set the start for stroke %ld", (iSegment+1), (iStroke+1)));
				}
				else if (	iSegment < _vecSegments.size()-1
						&&	sg.getRange().getLength() <= s_cDROPOUT)
				{
					st.setEnd(max<long>(sg.getEnd()+1, st.getEnd()));
					st.incSegments();
					st.incDropouts();
				}
				else
				{
					st.setEnd(sg.getStart()-1);
					fAdvanceSegment = false;
					fAdvanceStroke = true;
					TFLOW(VALIDATION,L4,(LLTRACE, "CASE D/E: Segment %ld set the end for stroke %ld", (iSegment+1), (iStroke+1)));
				}
			}
		}

		// Case F: Segment lies wholly after the stroke
		else
		{
			ASSERT(fStartAfter);
			ASSERT(!sg.isCoherent());
			if (!st.getSegments())
			{
				Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION,
									 "Stroke %ld lost to incoherent segment %ld %s",
									 (iStroke+1), (iSegment+1), sg.getRange().toString().c_str());
				Genome::recordTermination(NULL, STGT_VALIDATION, STGR_STROKES,
									"Stroke %ld lost to incoherent segment %ld %s",
				 					(iStroke+1), (iSegment+1), sg.getRange().toString().c_str());
				goto RECORDUNDO;
			}

			if (	iSegment < _vecSegments.size()-1
				&&	sg.getRange().getLength() <= s_cDROPOUT)
			{
				st.setEnd(sg.getEnd()+1);
				st.incSegments();
				st.incDropouts();
			}
			else
			{
				st.setEnd(sg.getStart()-1);
				fAdvanceSegment = false;
				fAdvanceStroke = true;
			}
			TFLOW(VALIDATION,L4,(LLTRACE, "CASE F: Segment %ld set the end for stroke %ld", (iSegment+1), (iStroke+1)));
		}
		
		// If the segment set the start of a stroke, add to the stroke any outstanding potential stroke segments
		// - The collection is a series of coherent segments within dropout distance of one another (including
		//   the segment that started the stroke). The number of segments added to the stroke is the total number
		//   of collected coherent segments plus the intervening incoherent segments.
		if (fStrokeStarted && vecPotentialStrokeSegments.size())
		{
			st.setStart(_vecSegments[vecPotentialStrokeSegments[0]].getStart());
			st.incSegments(vecPotentialStrokeSegments.size() * 2);
			st.incDropouts(vecPotentialStrokeSegments.size());
			TFLOW(VALIDATION,L4,(LLTRACE, "%ld possible marks assigned to stroke %ld", vecPotentialStrokeSegments.size(), (iStroke+1)));
			vecPotentialStrokeSegments.clear();
		}

		// Advance to the next segment and stroke as needed
		if (fAdvanceSegment)
			++iSegment;
		if (fAdvanceStroke)
		{
			++iStroke;
			ASSERT(vecPotentialStrokeSegments.empty());
		}
	}
	ASSERT(vecPotentialStrokeSegments.empty());

	// Gene is invalid if not all strokes received at least one coherent segment
	if (	iStroke < _vecStrokes.size()-1
		||	(	iStroke == _vecStrokes.size()-1
			&&	_vecStrokes[iStroke].getSegments() <= 0))
	{
		Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION, "Unable to assign segments to all strokes");
		Genome::recordTermination(NULL, STGT_VALIDATION, STGR_STROKES, "Unable to assign segments to all strokes");
		goto RECORDUNDO;
	}

	// Ensure the last stroke has been assigned its appropriate end position
	// - The loop above terminates a stroke when it encounters an incoherent
	//   segment exceeding the dropout length; if it exhausted segments before
	//   strokes, the last stroke will require termination
	ASSERT(		iStroke >= _vecStrokes.size()
			||	(	iStroke == _vecStrokes.size()-1
				&&	iSegment >= _vecSegments.size()
				&&	(	_vecSegments.back().isCoherent()
					||	_vecSegments.back().getRange().getLength() <= static_cast<long>(s_cDROPOUT))));
	if (iStroke == _vecStrokes.size()-1)
	{
		TFLOW(VALIDATION,L4,(LLTRACE, "Segment %ld set the end for stroke %ld", iSegment, iStroke));
		const Segment& sg = _vecSegments.back();
		_vecStrokes.back().setEnd(sg.isCoherent()
								  ? sg.getEnd()
								  : sg.getStart()-1);
	}

	// Count all coherent segments occurring after all strokes as marks
	for (; iSegment < _vecSegments.size(); ++iSegment)
	{
		if (_vecSegments[iSegment].isCoherent())
			_vecMarks.push_back(iSegment);
	}

	markValid(GI_STROKES);

	TRACEDOIF(VALIDATION,DATA,L3,traceStrokes());

RECORDUNDO:

	// If the stroke ranges changed, record the changes
	for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
	{
		if (_vecStrokes[iStroke].getRange() != vecStrokeRanges[iStroke])
		{
			Genome::recordModification(::new StrokeRangeChange(_id, vecStrokeRanges));
			if (!Genome::isRollbackAllowed())
				LOGWARNING((LLWARNING, "Change occured in stroke range for stroke %ld - %s to %s",
										iStroke+1,
										vecStrokeRanges[iStroke].toString().c_str(),
										_vecStrokes[iStroke].getRange().toString().c_str()));
			break;
		}
	}
	
INVALID:
	;
}

/*
 * Function: ensureDimensions
 *
 * Determine the bounding rectangles along with the scale factors and translation offsets
 * to align strokes, groups, and the gene against the Han.
 *
 * Processing works from the strokes up and establishes bounding rectangles, then scale
 * factors, and finally translation offsets. (Translation offsets are applied to scaled
 * coordinates.) If any element cannot calculate a scale factor (due to missing profile
 * along a dimension), a second scale pass promotes parent scale factors to children.
 */
void
Gene::ensureDimensions()
{
	ENTER(VALIDATION,ensureDimensions);

	const Han& han = Han::getDefinition(_strUnicode);
	const HGROUPARRAY& vecHGroups = han.getGroups();

	// First, calculate all group and stroke dimensions (groups will invoke strokes)
	// - Determine the gene dimensions along the way
	for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
	{
		_vecGroups[iGroup].calcDimensions(*this, vecHGroups[iGroup]);
		if (iGroup == 0)
			_rectBounds.set(_vecGroups[iGroup].getBounds());
		else
			_rectBounds.combine(_vecGroups[iGroup].getBounds());
	}
	

	Unit nxToHan = 0;
	Unit nyToHan = 0;
	Unit sxToHan = 0;
	Unit syToHan = 0;
	Unit dxToHan = 0;
	Unit dyToHan = 0;

	bool fDimensionMissing = false;

	// Then, compute the scale factors for all groups and strokes
	// - If any child, group or stroke, cannot compute a scale factor,
	//   it will return true (to indicate a missing dimension)
	for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
	{
		Group& grp = _vecGroups[iGroup];
		const HGroup& hgrp = vecHGroups[iGroup];

		fDimensionMissing = (	grp.calcScale(*this, han, hgrp)
							||	fDimensionMissing);

		if (Unit(grp.sxToHan()).isDefined())
		{
			sxToHan += hgrp.getBounds().getWidth() * grp.sxToHan();
			nxToHan += hgrp.getBounds().getWidth();
		}
		else
			fDimensionMissing = true;

		if (Unit(grp.syToHan()).isDefined())
		{
			syToHan += hgrp.getBounds().getHeight() * grp.syToHan();
			nyToHan += hgrp.getBounds().getHeight();
		}
		else
			fDimensionMissing = true;
	}

	// At least one dimension must have profile, it is an error otherwise
	if (nxToHan <= static_cast<UNIT>(0.0) && nyToHan <= static_cast<UNIT>(0.0))
	{
		Genome::recordAttempt(ST_FILELINE, STTR_VALIDATION, "Both dimensions lack profile");
		Genome::recordTermination(NULL, STGT_VALIDATION, STGR_MEASUREMENT, "Both dimensions lack profile");
		goto INVALID;
	}

	// Compute gene scale factors
	if (nxToHan > static_cast<UNIT>(0.0))
		_sxToHan = sxToHan / nxToHan;
	else
	{
		fDimensionMissing = true;
		_sxToHan.setUndefined();
	}

	if (nyToHan > static_cast<UNIT>(0.0))
		_syToHan = syToHan / nyToHan;
	else
	{
		fDimensionMissing = true;
		_syToHan.setUndefined();
	}

	// Promote scale factors, as needed, to compensate for those they could not calculate
	if (fDimensionMissing)
	{
		ASSERT(_sxToHan.isDefined() || _syToHan.isDefined());

		if (!_sxToHan.isDefined())
		{
			TDATA(VALIDATION,L3,(LLTRACE, "Gene is missing sxToHan dimension, taking syToHan(%f)", static_cast<UNIT>(_syToHan)));
			_sxToHan = _syToHan;
		}
		else if (!_syToHan.isDefined())
		{
			TDATA(VALIDATION,L3,(LLTRACE, "Gene is missing syToHan dimension, taking sxToHan(%f)", static_cast<UNIT>(_sxToHan)));
			_syToHan = _sxToHan;
		}

		TDATA(VALIDATION,L3,(LLTRACE, "Gene promoting sx/sy(%f,%f) to groups", static_cast<UNIT>(_sxToHan), static_cast<UNIT>(_syToHan)));
		for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
		{
			_vecGroups[iGroup].promoteScale(*this, vecHGroups[iGroup], _sxToHan, _syToHan);
		}
	}

	ASSERT(_sxToHan.isDefined() && _syToHan.isDefined());
	_sxyToHan = ::sqrt((_sxToHan*_sxToHan)+(_syToHan*_syToHan));

	// Finally, compute the translation offsets for all elements (groups, strokes, and the gene)
	for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
	{
		Group& grp = _vecGroups[iGroup];
		const HGroup& hgrp = vecHGroups[iGroup];
		
		grp.calcOffsets(*this, han, hgrp);

		dxToHan += hgrp.getLength() * grp.dxParentToHan();
		dyToHan += hgrp.getLength() * grp.dyParentToHan();
	}

	_dxToHan = dxToHan / han.getLength();
	_dyToHan = dyToHan / han.getLength();

	markValid(GI_DIMENSIONS);

INVALID:
	;
}

/*
 * Function: ensureOverlaps
 *
 */
void
Gene::ensureOverlaps()
{
	ENTER(VALIDATION,ensureOverlaps);

	Overlaps overlaps(_vecAcids, _vecPoints, _vecStrokes);
	_setOverlaps = overlaps.getOverlaps();

	TFLOW(VALIDATION,L3,(LLTRACE, "%ld overlapping strokes", _setOverlaps.size()));
	TRACEDOIF(VALIDATION,DATA,L3,traceStrokeOverlaps());

	markValid(GI_OVERLAPS);
}
