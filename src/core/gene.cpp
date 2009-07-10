/*******************************************************************************
 * \file	gene.cpp
 * \brief	Stylus Gene class
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
// StrokeRange
//
//--------------------------------------------------------------------------------

/*
 * Function: load
 *
 */
void
StrokeRange::load(XMLDocument* pxd, xmlNodePtr pxnStroke)
{
	ENTER(GENOME,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnStroke));

	string str1;
	string str2;

	pxd->getAttribute(pxnStroke, xmlTag(XT_BASEFIRST), str1);
	pxd->getAttribute(pxnStroke, xmlTag(XT_BASELAST), str2);
	Range::set(::atol(str1.c_str())-1, ::atol(str2.c_str())-1);

	pxd->getAttribute(pxnStroke, xmlTag(XT_CORRESPONDSTO), str1);
	_nCorrespondsTo = ::atoi(str1.c_str())-1;

	TFLOW(GENOME,L4,(LLTRACE,
					 "Loaded hanReference stroke - range %s, corresponds-to %ld",
					 toString().c_str(), (_nCorrespondsTo+1)));
}

//--------------------------------------------------------------------------------
//
// Stroke
//
//--------------------------------------------------------------------------------

/*
 * Function: operator=
 *
 */
Stroke&
Stroke::operator=(const Stroke& st)
{
	_id = st._id;

	_rgAcids = st._rgAcids;

	_cSegments = st._cSegments;
	_cDropouts = st._cDropouts;

	_rectBounds = st._rectBounds;

	_slVectors = st._slVectors;

	_sxToHan = st._sxToHan;
	_syToHan = st._syToHan;
	_sxyToHan = st._sxyToHan;

	_dxToHan = st._dxToHan;
	_dyToHan = st._dyToHan;

	_nExtraLength = st._nExtraLength;
	_nDeviation = st._nDeviation;

	return *this;
}

/*
 * Function: invalidate
 *
 */
void
Stroke::invalidate(STFLAGS grfInvalid)
{
	if (grfInvalid & STI_DIMENSIONS)
	{
		_rectBounds.clear();

		_slVectors.clear();
		
		_sxIsInherited = false;
		_syIsInherited = false;

		_sxToHan = 1;
		_syToHan = 1;
		_sxyToHan = 1;

		_dxToHan = 0;
		_dyToHan = 0;
	}

	if (grfInvalid & STI_SEGMENTS)
	{
		_cSegments = 0;
		_cDropouts = 0;
	}

	if (grfInvalid & STI_LOCATION)
	{
		_rgAcids.clear();
	}

	if (grfInvalid & STI_SCORE)
	{
		_nExtraLength = 0;
		_nDeviation = 0;
	}
}

#ifdef ST_TRACE
/*
 * Function: traceScore
 *
 */
void
Stroke::traceScore() const
{
	if (Globals::traceIf(STTR_SCORING, STTC_DATA, STTL_L4))
	{
		LOGTRACE((LLTRACE, "Stroke %ld", (_id+1)));
		{
			ENTERTRACESCOPE(s0);
			LOGTRACE((LLTRACE, "Bounds   : width(%0.15f) height(%0.15f)", _rectBounds.getWidth(), _rectBounds.getHeight()));
			LOGTRACE((LLTRACE, "sxToHan  : %0.15f%s", static_cast<UNIT>(_sxToHan), (_sxIsInherited ? ", Inherited" : "")));
			LOGTRACE((LLTRACE, "syToHan  : %0.15f%s", static_cast<UNIT>(_syToHan), (_syIsInherited ? ", Inherited" : "")));
			LOGTRACE((LLTRACE, "dxToHan  : %0.15f", static_cast<UNIT>(_dxToHan)));
			LOGTRACE((LLTRACE, "dyToHan  : %0.15f", static_cast<UNIT>(_dyToHan)));
			LOGTRACE((LLTRACE, "deviation: %0.15f", static_cast<UNIT>(_nDeviation)));
			LOGTRACE((LLTRACE, "extraLen : %0.15f", static_cast<UNIT>(_nExtraLength)));
		}
	}
	else
	{
		LOGTRACE((LLTRACE, "deviation: %0.15f", static_cast<UNIT>(_nDeviation)));
	}
}
#endif

//--------------------------------------------------------------------------------
//
// Group
//
//--------------------------------------------------------------------------------

/*
 * Function: operator=
 *
 */
Group&
Group::operator=(const Group& grp)
{
	_id = grp._id;
	_rectBounds = grp._rectBounds;
	
	_sxIsInherited = grp._sxIsInherited;
	_syIsInherited = grp._syIsInherited;

	_sxToHan = grp._sxToHan;
	_syToHan = grp._syToHan;

	_dxToHan = grp._dxToHan;
	_dyToHan = grp._dyToHan;

	_dxParentToHan = grp._dxParentToHan;
	_dyParentToHan = grp._dyParentToHan;

	::memcpy(_aryScoreExponents, grp._aryScoreExponents, ARRAY_SIZE(_aryScoreExponents));
	_nScore = grp._nScore;

	return *this;
}

/*
 * Function: invalidate
 *
 */
void
Group::invalidate(STFLAGS grfInvalid)
{
	if (grfInvalid & GPI_DIMENSIONS)
	{
		_rectBounds.clear();
		
		_sxIsInherited = false;
		_syIsInherited = false;

		_sxToHan = 1;
		_syToHan = 1;

		_dxToHan = 0;
		_dyToHan = 0;

		_dxParentToHan = 0;
		_dyParentToHan = 0;
	}

	if (grfInvalid & GPI_SCORE)
	{
		_aryScoreExponents[SC_SCALE] = static_cast<UNIT>(0);
		_aryScoreExponents[SC_PLACEMENT] = static_cast<UNIT>(0);
		_aryScoreExponents[SC_DEVIATION] = static_cast<UNIT>(0);
		_aryScoreExponents[SC_EXTRALENGTH] = static_cast<UNIT>(0);
		_aryScoreExponents[SC_ILLEGALOVERLAPS] = static_cast<UNIT>(0);
		_aryScoreExponents[SC_MISSINGOVERLAPS] = static_cast<UNIT>(0);
		_aryScoreExponents[SC_DROPOUTS] = static_cast<UNIT>(0);
		_nScore = 0;
	}
}

#ifdef ST_TRACE
/*
 * Function: traceScore
 *
 */
void
Group::traceScore() const
{
	if (Globals::traceIf(STTR_SCORING, STTC_DATA, STTL_L4))
	{
		LOGTRACE((LLTRACE, "Group %ld", (_id+1)));
		{
			ENTERTRACESCOPE(s0);
			LOGTRACE((LLTRACE, "nScore        : %0.15f", static_cast<UNIT>(_nScore)));
			LOGTRACE((LLTRACE, "sxToHan       : %0.15f%s", static_cast<UNIT>(_sxToHan), (_sxIsInherited ? ", Inherited" : "")));
			LOGTRACE((LLTRACE, "syToHan       : %0.15f%s", static_cast<UNIT>(_syToHan), (_syIsInherited ? ", Inherited" : "")));
			LOGTRACE((LLTRACE, "dxToHan       : %0.15f", static_cast<UNIT>(_dxToHan)));
			LOGTRACE((LLTRACE, "dyToHan       : %0.15f", static_cast<UNIT>(_dyToHan)));
			LOGTRACE((LLTRACE, "dxParentToHan : %0.15f", static_cast<UNIT>(_dxParentToHan)));
			LOGTRACE((LLTRACE, "dyParentToHan : %0.15f", static_cast<UNIT>(_dyParentToHan)));
			LOGTRACE((LLTRACE, "Scale Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_SCALE])));
			LOGTRACE((LLTRACE, "Place Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_PLACEMENT])));
			LOGTRACE((LLTRACE, "Inter Exponent: %0.15f (missing overlaps)", static_cast<UNIT>(_aryScoreExponents[SC_MISSINGOVERLAPS])));
			LOGTRACE((LLTRACE, "Joins Exponent: %0.15f (illegal overlaps)", static_cast<UNIT>(_aryScoreExponents[SC_ILLEGALOVERLAPS])));
			LOGTRACE((LLTRACE, "Drops Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_DROPOUTS])));
			LOGTRACE((LLTRACE, "Dev   Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_DEVIATION])));
			LOGTRACE((LLTRACE, "Xtra  Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_EXTRALENGTH])));
		}
	}
	else
	{
		LOGTRACE((LLTRACE, "Group %ld Score %0.15f", (_id+1), static_cast<UNIT>(_nScore)));
	}
}
#endif

//--------------------------------------------------------------------------------
//
// Gene
//
//--------------------------------------------------------------------------------

const size_t Gene::s_cDROPOUT;

/*
 * Function: operator=
 *
 */
Gene&
Gene::operator=(const Gene& gene)
{
	_id = gene._id;
	_rgBases = gene._rgBases;

	_grfInvalid = gene._grfInvalid;

	_vecAcids = gene._vecAcids;
	_vecPoints = gene._vecPoints;
	_vecCoherent = gene._vecCoherent;

	_vecSegments = gene._vecSegments;
	_cCoherent = gene._cCoherent;
	_nUnits = gene._nUnits;

	_strUnicode = gene._strUnicode;

	_vecGroups = gene._vecGroups;

	_fStrokesAssigned = gene._fStrokesAssigned;
	_vecStrokes = gene._vecStrokes;
	_mapStrokeToHan = gene._mapStrokeToHan;
	_mapHanToStroke = gene._mapHanToStroke;
	_mapStrokeToGroup = gene._mapStrokeToGroup;

	_vecMarks = gene._vecMarks;
	_setOverlaps = gene._setOverlaps;
	_vecMissingOverlaps = gene._vecMissingOverlaps;
	_vecIllegalOverlaps = gene._vecIllegalOverlaps;

	_ptOrigin = gene._ptOrigin;
	_rectBounds = gene._rectBounds;

	_sxToHan = gene._sxToHan;
	_syToHan = gene._syToHan;
	_sxyToHan = gene._sxyToHan;

	_dxToHan = gene._dxToHan;
	_dyToHan = gene._dyToHan;

	::memcpy(_aryScoreExponents, gene._aryScoreExponents, ARRAY_SIZE(_aryScoreExponents));
	_nScore = gene._nScore;

	return *this;
}

/*
 * Function: load
 *
 */
void
Gene::load(XMLDocument* pxd, xmlNodePtr pxnGene)
{
	ENTER(GENOME,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnGene));
	ASSERT(ISEQUALXMLSTR(pxnGene->name, xmlTag(XT_GENE)));

	string strBaseFirst;
	string strBaseLast;

	pxd->getAttribute(pxnGene, xmlTag(XT_BASEFIRST), strBaseFirst);
	pxd->getAttribute(pxnGene, xmlTag(XT_BASELAST), strBaseLast);
	
	const string& strBases = Genome::getBases();
	long iStart = ::atol(strBaseFirst.c_str())-1;
	long iEnd = ::atol(strBaseLast.c_str())-1;
	
	if (	iStart >= iEnd
		||	iStart < 0
		||	static_cast<size_t>(iEnd) > Genome::getBases().length())
		THROWRC((RC(XMLERROR), "Invalid base range assigned to the gene - assigned (%ld,%ld), must range from 1 to %ld", iStart+1, iEnd+1, strBases.length()));
	if (!Codon::isStart(&(strBases.c_str()[iStart])))
		THROWRC((RC(XMLERROR), "Invalid base range assigned to the gene - assigned range (%ld,%ld) does not begin with a start codon", iStart+1, iEnd+1));
	if (!Codon::isStop(&(strBases.c_str()[iEnd-2])))
		THROWRC((RC(XMLERROR), "Invalid base range assigned to the gene - assigned range (%ld,%ld) does not end with a stop codon", iStart+1, iEnd+1));

	TFLOW(GENOME,L1,(LLTRACE, "Assigning gene to range (%ld,%ld)", iStart, iEnd));
	setRange(Range(iStart, iEnd));

	xmlXPathContextSPtr spxpc(pxd->createXPathContext());

	// Read the default origin (if any)
	spxpc->node = pxnGene;
	xmlXPathObjectSPtr spxpoOrigin(pxd->evalXPath(spxpc.get(), xmlXPath(XP_ORIGIN)));
	if (XMLDocument::isXPathSuccess(spxpoOrigin.get(), 1))
		_ptOrigin.load(pxd, spxpoOrigin->nodesetval->nodeTab[0]);

	// Orient to the required single hanReference element
	spxpc->node = pxnGene;
	xmlXPathObjectSPtr spxpoHan(pxd->evalXPath(spxpc.get(), xmlXPath(XP_HANREFERENCE)));
	if (!XMLDocument::isXPathSuccess(spxpoHan.get(), 1))
		THROWRC((RC(XMLERROR), "This version of Stylus requires exactly one hanReference per gene"));

	xmlNodePtr pxnHan = spxpoHan->nodesetval->nodeTab[0];

	// Read the Han Unicode value and ensure a Han glyph exists
	pxd->getAttribute(pxnHan, xmlTag(XT_UNICODE), _strUnicode);
	const Han& han = Han::getDefinition(_strUnicode);

	// Load stroke range and assignment values
	spxpc->node = pxnHan;
	xmlXPathObjectSPtr spxpoStrokes(pxd->evalXPath(spxpc.get(), xmlXPath(XP_STROKE)));
	if (!XMLDocument::isXPathSuccess(spxpoHan.get()))
		THROWRC((RC(XMLERROR), "This version of Stylus requires hanReference elements to contain stroke elements"));

	TFLOW(GENOME,L1,(LLTRACE,
					 "Assigning Han %s with %d strokes to gene %ld",
					 _strUnicode.c_str(), spxpoStrokes->nodesetval->nodeNr, (_id+1)));

	_fStrokesAssigned = true;

	STROKERANGEARRAY vecStrokeRange(spxpoStrokes->nodesetval->nodeNr);
	loadFromXML<STROKERANGEARRAY>(vecStrokeRange, pxd,
								  spxpoStrokes->nodesetval->nodeNr,
								  spxpoStrokes->nodesetval->nodeTab);

	validateStrokeRanges(vecStrokeRange);

	_vecStrokes.resize(vecStrokeRange.size());
	_mapStrokeToHan.resize(vecStrokeRange.size());
	_mapHanToStroke.resize(vecStrokeRange.size());

	for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
	{
		const StrokeRange& sr = vecStrokeRange[iStroke];

		_vecStrokes[iStroke].setID(iStroke);
		_vecStrokes[iStroke].setRange(baseToCodonRange(sr));

		_mapStrokeToHan[iStroke] = sr.getCorrespondsTo();
		_mapHanToStroke[sr.getCorrespondsTo()] = iStroke;

		TDATA(GENOME,L2,(LLTRACE,
						 "Stroke %d assigned to base range %s, codons %s",
						 (iStroke+1),
						 sr.toString().c_str(),
						 baseToCodonRange(sr).toString().c_str()));
	}

	// Build groups for the gene based on those defined by the Han glyph
	const HGROUPARRAY vecHGroups = han.getGroups();
	_vecGroups.resize(vecHGroups.size());
	_mapStrokeToGroup.resize(_vecStrokes.size());
	for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
	{
		_vecGroups[iGroup].setID(iGroup);

		const NUMERICARRAY& vecContainedStrokes = vecHGroups[iGroup].getStrokes();
		for (size_t iContainedStroke=0; iContainedStroke < vecContainedStrokes.size(); ++iContainedStroke)
			_mapStrokeToGroup[_mapHanToStroke[vecContainedStrokes[iContainedStroke]]] = iGroup;
	}
	TRACEDOIF(GENOME,FLOW,L2,traceGroups());

	// Build the array of required and allowed overlaps converted into gene stroke indexes
	_vecHOverlaps = han.getOverlaps();
	for (size_t iHOverlap=0; iHOverlap < _vecHOverlaps.size(); ++iHOverlap)
		_vecHOverlaps[iHOverlap].convertStrokes(_mapHanToStroke);
	sort(_vecHOverlaps.begin(), _vecHOverlaps.end());
}

/*
 * Function: setRange
 *
 * NOTES:
 * - Setting the range removes all Han assignments (since the stroke locations will have
 *   changed)
 */
void
Gene::setRange(const Range& rgBases)
{
	ENTER(GENOME,setRange);

	ASSERT(rgBases.isEmpty() || Codon::isStop(Genome::getBases().c_str()+rgBases.getEnd()-2));

	_rgBases = rgBases;

	_strUnicode.clear();

	_vecGroups.clear();
	_vecStrokes.clear();

	_fStrokesAssigned = false;
	_mapStrokeToHan.clear();
	_mapHanToStroke.clear();

	_ptOrigin.set(0,0);

	_grfInvalid = GI_ALL;
	invalidate();
}

/*
 * 
 * Function: markInvalid
 *
 */
void
Gene::markInvalid(GENECHANGE gc, const Range& rg, bool fSilent)
{
	ENTER(MUTATION,markInvalid);

	ASSERT(gc != GC_CHANGE || _rgBases.contains(rg.getStart()) && _rgBases.contains(rg.getEnd()));

	STFLAGS grfInvalid = GI_ALL;

	long iAcid = Codon::toCodonIndex(Codon::toCodonBoundary(rg.getStart() - _rgBases.getStart()));
	
	// Handle silent point mutations
	// - Silent mutations do not induce any *additional* invalid regions
	// - Allow silent mutations only when acids are valid
	if (gc == GC_CHANGE && fSilent)
	{
		TFLOW(MUTATION,L2,(LLTRACE, "Silent mutation at %ld", rg.getStart()));
		if (isValid(GI_ACIDS))
			grfInvalid = _grfInvalid;
	}

	// Otherwise, adjust arrays and stroke ranges to accommodate insertions and deletions
	// - Fix-up acids and points if possible
	// - More complex fix-up (such as recalculating coherence on sub-regions) tends to be
	//   error prone and, more importantly, of very little performance benefit

	else
	{
		long cAcidChange = Codon::numAffectedCodons(rg.getStart(), rg.getLength());
		long cBasesChange = rg.getLength();

		// Respond to insertions and deletions
		if (gc == GC_INSERT || gc == GC_DELETE)
		{
			ASSERT(Codon::numFrameShift(rg.getLength()) == 0);
			
			// Resize the acids, points, and coherence arrays to match the new number of codons
			ACIDTYPEARRAY::iterator itAcids = _vecAcids.begin();
			POINTARRAY::iterator itPoints = _vecPoints.begin();
			UINTARRAY::iterator itCoherent = _vecCoherent.begin();
			ASSERT(_vecAcids.size() == _vecPoints.size());
			ASSERT(_vecAcids.size() == _vecCoherent.size());
			if (gc == GC_INSERT)
			{
				_rgBases.extend(cBasesChange);
				_vecAcids.insert(itAcids+iAcid, cAcidChange, ACID_STP);
				_vecPoints.insert(itPoints+iAcid, cAcidChange, Point());
				_vecCoherent.insert(itCoherent+iAcid, cAcidChange, 0);
			}
			else
			{
				ASSERT(iAcid-cAcidChange <= static_cast<long>(_vecAcids.size()));
				_rgBases.extend(-cBasesChange);
				_vecAcids.erase(itAcids+iAcid, itAcids+iAcid+cAcidChange);
				_vecPoints.erase(itPoints+iAcid, itPoints+iAcid+cAcidChange);
				_vecCoherent.erase(itCoherent+iAcid, itCoherent+iAcid+cAcidChange);
				cAcidChange *= -1;
				cBasesChange *= -1;
			}
			ASSERT(_vecAcids.size() == static_cast<size_t>(::abs(Codon::numWholeCodons(_rgBases.getLength()))));
			ASSERT(_vecAcids.size() == _vecPoints.size());
			ASSERT(_vecAcids.size() == _vecCoherent.size());

			// Unless within a rollback or restore, adjust stroke locations after insertions and deletions
			// - Shift the strokes by the number of whole codon units in the change
			// - The stroke that contains the target index "owns" the entire change
			//   just as if the genome originally had more/less bases at that location
			// - Existing stroke ranges are preserved in case the change is later rejected
			if (!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING))
			{
				vector<Range> vecStrokeRanges(_vecStrokes.size());

				TDATA(MUTATION,L3,(LLTRACE, "Adjusting stroke ranges by %ld acids starting at acid %ld", cAcidChange, iAcid));
				for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
				{
					Stroke& st = _vecStrokes[iStroke];
					const Range& rgStroke = st.getRange();

					vecStrokeRanges[iStroke] = rgStroke;

					TDATA(MUTATION,L4,(LLTRACE, "Stroke %lu before adjustment has range %s", (iStroke+1), rgStroke.toString().c_str()));
					if (rgStroke.getEnd() >= iAcid)
					{
						// Adjust the stroke that "owns" the change
						// - Insertions always extend the stroke end by the inserted number of acids
						// - Deletions truncate the end by the number of deleted acids but never to a
						//   point prior to the deletion
						// NOTE:
						// Stroke ranges may become invalid, but should never become less than zero.
						// Validation will validate, and reject, any invalid stroke ranges. This allows
						// multiple mutations to temporarily create invalid stroke ranges.
						if (rgStroke.contains(iAcid))
						{
							st.extendRange(cAcidChange >= 0
											? cAcidChange
											: max<long>(-(rgStroke.getEnd()-iAcid+1),cAcidChange));
							ASSERT(st.getRange().getLength() >= 0);
						}

						// Otherwise shift the entire stroke range
						else
							_vecStrokes[iStroke].moveRange(cAcidChange);
					}
					TDATA(MUTATION,L4,(LLTRACE, "Stroke %lu after adjustment has range %s", (iStroke+1), st.getRange().toString().c_str()));
				}

				Genome::recordModification(::new StrokeRangeChange(_id, vecStrokeRanges));
			}
		}

		// If acids and points are not already invalid, fix-up their values
		if (isValid(GI_ACIDS | GI_POINTS))
		{
			ensureAcids(iAcid, cAcidChange);
			grfInvalid &= ~(GI_ACIDS | GI_POINTS);
		}
	}

	markInvalid(grfInvalid);
}

/*
 * Function: ensureCompiled
 *
 */
bool
Gene::ensureCompiled()
{
	ENTER(VALIDATION,ensureCompiled);

	invalidate();

	if (isInvalid(GI_ACIDS | GI_POINTS))
		ensureAcids(0, _vecAcids.size());

	if (isValid(GI_ACIDS) && isInvalid(GI_COHERENCE))
		ensureCoherence();

	if (isValid(GI_ACIDS | GI_COHERENCE) && isInvalid(GI_SEGMENTS))
		ensureSegments();
	
	TFLOW(VALIDATION,L2,(LLTRACE,
						 "Gene %s compiled in trial %d",
						 (isValid(GI_COMPILED)
						  ? "successfully"
						  : "unsuccessfully"),
						Genome::getTrial()));
	TRACEDOIF(VALIDATION,DATA,L3,traceGene());
	return isValid(GI_COMPILED);
}

/*
 * Function: ensureScore
 *
 */
bool
Gene::ensureScore()
{
	ENTER(VALIDATION,ensureScore);
	
	ASSERT(isValid(GI_COMPILED | GI_VALIDATED));

	if (isInvalid(GI_SCORE) && calcScore())
		markValid(GI_SCORE);

	TFLOW(SCORING,L2,(LLTRACE, "Gene scored %f in trial %d", static_cast<UNIT>(_nScore), Genome::getTrial()));
	TRACEDOIF(SCORING,DATA,L3,traceScores());
	return isValid(GI_SCORED);
}

/*
 * Function: ensureValid
 *
 */
bool
Gene::ensureValid()
{
	ENTER(VALIDATION,ensureValid);
	
	ASSERT(isValid(GI_COMPILED));

	if (isValid(GI_SEGMENTS) && isInvalid(GI_STROKES))
		ensureStrokes();

	if (isValid(GI_STROKES) && isInvalid(GI_DIMENSIONS))
		ensureDimensions();

	if (isValid(GI_STROKES) && isInvalid(GI_OVERLAPS))
		ensureOverlaps();
	
	TFLOW(VALIDATION,L2,(LLTRACE,
						 "Gene %s validated in trial %d",
						 (isValid(GI_VALIDATED)
						  ? "successfully"
						  : "unsuccessfully"),
						Genome::getTrial()));
	TRACEDOIF(VALIDATION,DATA,L3,traceGene());
	return isValid(GI_VALIDATED);
}

/*
 * Function: markInvalid
 *
 */
void
Gene::markInvalid(STFLAGS grfInvalid)
{
	ENTER(VALIDATION,markInvalid);
	
	TFLOW(VALIDATION,L4,(LLTRACE, "Invalidating: current(0x%X) incoming(0x%X)", _grfInvalid, grfInvalid));

	if (grfInvalid & GI_ACIDS)
	{
		grfInvalid |= (		GI_POINTS
						&	GI_COHERENCE
						&	GI_SEGMENTS
						&	GI_STROKES
						&	GI_DIMENSIONS
						&	GI_OVERLAPS
						&	GI_SCORE);
	}

	else if (grfInvalid & GI_POINTS)
	{
		grfInvalid |= (		GI_DIMENSIONS
						&	GI_OVERLAPS
						&	GI_SCORE);
	}

	else if (grfInvalid & GI_COHERENCE)
	{
		grfInvalid |= (		GI_SEGMENTS
						&	GI_STROKES
						&	GI_DIMENSIONS
						&	GI_OVERLAPS
						&	GI_SCORE);
	}

	else if (grfInvalid & GI_SEGMENTS)
	{
		grfInvalid |= (		GI_STROKES
						&	GI_DIMENSIONS
						&	GI_OVERLAPS
						&	GI_SCORE);
	}

	else if (grfInvalid & GI_STROKES)
	{
		grfInvalid |= (		GI_DIMENSIONS
						&	GI_OVERLAPS
						&	GI_SCORE);
	}

	else if (grfInvalid & GI_DIMENSIONS)
	{
		grfInvalid |= GI_SCORE;
	}

	else if (grfInvalid & GI_OVERLAPS)
	{
		grfInvalid |= GI_SCORE;
	}

	_grfInvalid |= grfInvalid;
	TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated: current(0x%X)", _grfInvalid));
}

/*
 * Function: invalidate
 *
 * Re-initialize the invalid portions of the gene.
 * This preserves caller-supplied data (such as the assigned Han and stroke
 * locations), but invalidates calculated data.
 */
void
Gene::invalidate()
{
	ENTER(VALIDATION,invalidate);
	
	TFLOW(VALIDATION,L4,(LLTRACE, "Invalidating structures: current(0x%X)", _grfInvalid));

	if (!isValid(GI_ALL))
	{
		if (isInvalid(GI_ACIDS | GI_POINTS))
		{
			_vecAcids.clear();
			_vecPoints.clear();
			_nUnits.setUndefined();
			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated acids and points"));
		}

		if (isInvalid(GI_COHERENCE))
		{
			_vecCoherent.clear();
			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated coherence"));
		}

		if (isInvalid(GI_SEGMENTS))
		{
			_vecSegments.clear();
			_cCoherent = 0;
			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated segments"));
		}

		if (isInvalid(GI_STROKES))
		{
			_vecMarks.clear();

			if (!_fStrokesAssigned)
			{
				_vecStrokes.clear();
			}
			else
			{
				for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
					_vecStrokes[iStroke].invalidate(Stroke::STI_ALL & ~Stroke::STI_LOCATION);
			}

			for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
				_vecGroups[iGroup].invalidate(Group::GPI_ALL);

			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated strokes"));
			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated groups"));
		}

		if (isInvalid(GI_DIMENSIONS))
		{
			if (isValid(GI_STROKES))
			{
				for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
					_vecStrokes[iStroke].invalidate(Stroke::STI_DIMENSIONS);

				for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
					_vecGroups[iGroup].invalidate(Group::GPI_DIMENSIONS);
			}

			_rectBounds.clear();
			_sxToHan = 1;
			_syToHan = 1;
			_sxyToHan = 1;
			
			_dxToHan = 0;
			_dyToHan = 0;

			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated dimensions"));
		}

		if (isInvalid(GI_OVERLAPS))
		{
			_setOverlaps.clear();
			_vecMissingOverlaps.clear();
			_vecIllegalOverlaps.clear();
			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated overlaps"));
		}

		if (isInvalid(GI_SCORE))
		{
			if (isValid(GI_STROKES))
			{
				for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
					_vecStrokes[iStroke].invalidate(Stroke::STI_SCORE);

				for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
					_vecGroups[iGroup].invalidate(Group::GPI_SCORE);
			}

			_aryScoreExponents[SC_SCALE] = static_cast<UNIT>(0);
			_aryScoreExponents[SC_PLACEMENT] = static_cast<UNIT>(0);
			_aryScoreExponents[SC_ILLEGALOVERLAPS] = static_cast<UNIT>(0);
			_aryScoreExponents[SC_MISSINGOVERLAPS] = static_cast<UNIT>(0);
			_aryScoreExponents[SC_MARKS] = static_cast<UNIT>(0);
			_nScore.setUndefined();
			TFLOW(VALIDATION,L4,(LLTRACE, "Invalidated scores"));
		}
	}
}

/*
 * Function: validateStrokeRanges
 *
 */
void
Gene::validateStrokeRanges(const STROKERANGEARRAY& vecStrokeRange) const
{
	STROKERANGEARRAY vecStrokes(vecStrokeRange);

	sort(vecStrokes.begin(), vecStrokes.end());

	long iEndPrev = numeric_limits<long>::min();
	for (size_t iSR=0; iSR < vecStrokes.size(); ++iSR)
	{
		const StrokeRange& sr = vecStrokes[iSR];

		if (sr.getLength() < (Codon::s_cchCODON * Codon::s_nTRIVECTOR))
			THROWRC((RC(XMLERROR), "Assigned stroke range in %s is too small (%ld)", _strUnicode.c_str(), sr.getLength()));
		
		if (sr.getStart() < iEndPrev)
			THROWRC((RC(XMLERROR), "Assigned stroke ranges in %s overlap", _strUnicode.c_str()));

		iEndPrev = sr.getEnd();
	}

	if (vecStrokes.begin()->getStart() < _rgBases.getStart())
		THROWRC((RC(XMLERROR), "Assigned stroke ranges in %s begin outside the gene sequence", _strUnicode.c_str()));

	if ((vecStrokes.end()-1)->getEnd() > _rgBases.getEnd())
		THROWRC((RC(XMLERROR), "Assigned stroke ranges in %s end outside the gene sequence", _strUnicode.c_str()));
}

/*
 * Function: toString
 *
 */
string
Gene::toString() const
{
	return Constants::s_strEMPTY;
}

/*
 * Function: toXML
 *
 */
void
Gene::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GENOME,toXML);
	
	ASSERT(ST_ISANYSET(grfRecordDetail, STRD_GENES | STRD_DIMENSIONS | STRD_SEGMENTS | STRD_SCOREALL));
	
	// Add the gene element and attributes
	xs.openStart(xmlTag(XT_GENE));
	xs.writeAttribute(xmlTag(XT_BASEFIRST), _rgBases.getStart()+1);
	xs.writeAttribute(xmlTag(XT_BASELAST), _rgBases.getEnd()+1);
	if (ST_ISANYSET(grfRecordDetail, STRD_GENES | STRD_DIMENSIONS | STRD_SCOREALL) && isValid(GI_COMPILED))
		xs.writeAttribute(xmlTag(XT_UNITS), _nUnits);
	xs.closeStart();
	
	xs.openStart(xmlTag(XT_ORIGIN));
	xs.writeAttribute(xmlTag(XT_X), _ptOrigin.x());
	xs.writeAttribute(xmlTag(XT_Y), _ptOrigin.y());
	xs.closeStart(false);
	
	// Add the hanReference elements
	if (ST_ISANYSET(grfRecordDetail, STRD_GENES | STRD_DIMENSIONS | STRD_SCOREALL))
	{
		xs.writeStart(xmlTag(XT_HANREFERENCES));
	
		// TODO: Add support for multiple hanReference elements per gene
		xs.openStart(xmlTag(XT_HANREFERENCE));
		xs.writeAttribute(xmlTag(XT_UNICODE), _strUnicode);
		xs.closeStart();
	
		// Record dimensions
		if (ST_ISANYSET(grfRecordDetail, STRD_DIMENSIONS) && isValid(GI_VALIDATED))
		{
			_rectBounds.toXML(xs, grfRecordDetail);
		
			xs.openStart(xmlTag(XT_SCALE));
			xs.writeAttribute(xmlTag(XT_SXTOHAN), _sxToHan);
			xs.writeAttribute(xmlTag(XT_SYTOHAN), _syToHan);
			xs.closeStart(false);

			xs.openStart(xmlTag(XT_TRANSLATION));
			xs.writeAttribute(xmlTag(XT_DXTOHAN), _dxToHan);
			xs.writeAttribute(xmlTag(XT_DYTOHAN), _dyToHan);
			xs.closeStart(false);
		}
	
		// Record score details
		if (ST_ISANYSET(grfRecordDetail, STRD_SCOREALL) && isValid(GI_SCORED))
		{
			xs.openStart(xmlTag(XT_SCORE));
			xs.writeAttribute(xmlTag(XT_SCORE), _nScore);
			xs.writeAttribute(xmlTag(XT_SCALE), _aryScoreExponents[SC_SCALE]);
			xs.writeAttribute(xmlTag(XT_PLACEMENT), _aryScoreExponents[SC_PLACEMENT]);
			xs.writeAttribute(xmlTag(XT_ILLEGALOVERLAPS), static_cast<size_t>(_aryScoreExponents[SC_ILLEGALOVERLAPS]));
			xs.writeAttribute(xmlTag(XT_MISSINGOVERLAPS), static_cast<size_t>(_aryScoreExponents[SC_MISSINGOVERLAPS]));
			xs.writeAttribute(xmlTag(XT_MARKS), _aryScoreExponents[SC_MARKS]);
			xs.closeStart(false);
		}

		// Record groups
		if (ST_ISANYSET(grfRecordDetail, STRD_GENES | STRD_DIMENSIONS | STRD_SCOREALL))
		{
			const Han& han = Han::getDefinition(_strUnicode);
			const HGROUPARRAY& vecHGroups = han.getGroups();

			xs.writeStart(xmlTag(XT_GROUPS));
			for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
			{
				const Group& grp = _vecGroups[iGroup];
				const HGroup& hgrp = vecHGroups[iGroup];

				// Add the group element
				xs.writeStart(xmlTag(XT_GROUP));

				// Record dimensions
				if (ST_ISANYSET(grfRecordDetail, STRD_DIMENSIONS) && isValid(GI_VALIDATED))
				{
					grp.getBounds().toXML(xs, grfRecordDetail);
	
					xs.openStart(xmlTag(XT_SCALE));
					xs.writeAttribute(xmlTag(XT_SXTOHAN), grp.sxToHan());
					xs.writeAttribute(xmlTag(XT_SYTOHAN), grp.syToHan());
					xs.closeStart(false);

					xs.openStart(xmlTag(XT_TRANSLATION));
					xs.writeAttribute(xmlTag(XT_DXTOHAN), grp.dxToHan());
					xs.writeAttribute(xmlTag(XT_DYTOHAN), grp.dyToHan());
					xs.closeStart(false);
				}

				// Record score details
				if (ST_ISANYSET(grfRecordDetail, STRD_SCOREALL) && isValid(GI_SCORED))
				{
					xs.openStart(xmlTag(XT_SCORE));
					xs.writeAttribute(xmlTag(XT_SCORE), grp.score());
					xs.writeAttribute(xmlTag(XT_SCALE), grp.exponent(SC_SCALE));
					xs.writeAttribute(xmlTag(XT_PLACEMENT), grp.exponent(SC_PLACEMENT));
					xs.writeAttribute(xmlTag(XT_DEVIATION), grp.exponent(SC_DEVIATION));
					xs.writeAttribute(xmlTag(XT_EXTRALENGTH), grp.exponent(SC_EXTRALENGTH));
					xs.writeAttribute(xmlTag(XT_ILLEGALOVERLAPS), static_cast<size_t>(grp.exponent(SC_ILLEGALOVERLAPS)));
					xs.writeAttribute(xmlTag(XT_MISSINGOVERLAPS), static_cast<size_t>(grp.exponent(SC_MISSINGOVERLAPS)));
					xs.writeAttribute(xmlTag(XT_DROPOUTS), static_cast<size_t>(grp.exponent(SC_DROPOUTS)));
					xs.closeStart(false);
				}
			
				// Record contained strokes
				xs.writeStart(xmlTag(XT_CONTAINEDSTROKES), true, false);
				const NUMERICARRAY& vecContainedHStrokes = hgrp.getStrokes();
				for (size_t iContainedStroke=0; iContainedStroke < vecContainedHStrokes.size(); )
				{
					xs.writeContent(_mapHanToStroke[vecContainedHStrokes[iContainedStroke]]+1);
					if (++iContainedStroke < vecContainedHStrokes.size())
						xs.writeContent(Constants::s_chBLANK);
				}
				xs.writeEnd(xmlTag(XT_CONTAINEDSTROKES));

				// Close the group element
				xs.writeEnd(xmlTag(XT_GROUP));
			}
			xs.writeEnd(xmlTag(XT_GROUPS));
		}
	
		// Record strokes
		if (ST_ISANYSET(grfRecordDetail, STRD_GENES | STRD_DIMENSIONS | STRD_SCOREALL))
		{
			xs.writeStart(xmlTag(XT_STROKES));
			for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
			{
				const Stroke& st = _vecStrokes[iStroke];
				const Range rgBases(codonToBaseRange(st.getRange()));
			
				// Add the stroke element and attributes
				xs.openStart(xmlTag(XT_STROKE));
				xs.writeAttribute(xmlTag(XT_BASEFIRST), rgBases.getStart()+1);
				xs.writeAttribute(xmlTag(XT_BASELAST), rgBases.getEnd()+1);
				xs.writeAttribute(xmlTag(XT_CORRESPONDSTO), _mapStrokeToHan[iStroke]+1);
				
				if (	(	ST_ISANYSET(grfRecordDetail, STRD_DIMENSIONS)
						&&	isValid(GI_VALIDATED))
					||	(	ST_ISANYSET(grfRecordDetail, STRD_SCOREALL)
						&&	isValid(GI_SCORED)))
				{
					xs.closeStart();
	
					// Record dimensions
					if (ST_ISANYSET(grfRecordDetail, STRD_DIMENSIONS) && isValid(GI_VALIDATED))
					{
						st.getBounds().toXML(xs, grfRecordDetail);
		
						xs.openStart(xmlTag(XT_SCALE));
						xs.writeAttribute(xmlTag(XT_SXTOHAN), st.sxToHan());
						xs.writeAttribute(xmlTag(XT_SYTOHAN), st.syToHan());
						xs.closeStart(false);

						xs.openStart(xmlTag(XT_TRANSLATION));
						xs.writeAttribute(xmlTag(XT_DXTOHAN), st.dxToHan());
						xs.writeAttribute(xmlTag(XT_DYTOHAN), st.dyToHan());
						xs.closeStart(false);
					}
	
					// Record score details
					if (ST_ISANYSET(grfRecordDetail, STRD_SCOREALL) && isValid(GI_SCORED))
					{
						xs.openStart(xmlTag(XT_SCORE));
						xs.writeAttribute(xmlTag(XT_DEVIATION), st.deviation());
						xs.writeAttribute(xmlTag(XT_EXTRALENGTH), st.extraLength());
						xs.writeAttribute(xmlTag(XT_DROPOUTS), st.getDropouts());
						xs.closeStart(false);
					}

					// Close the stroke element
					xs.writeEnd(xmlTag(XT_STROKE));
				}
				else
					xs.closeStart(false);
			}
			xs.writeEnd(xmlTag(XT_STROKES));
		}
	
		// Record overlaps
		if (ST_ISANYSET(grfRecordDetail, STRD_SCOREALL) && isValid(GI_SCORED) && _setOverlaps.size())
		{
			xs.writeStart(xmlTag(XT_OVERLAPS));
			for (STROKEOVERLAPS::iterator it=_setOverlaps.begin(); it != _setOverlaps.end(); ++it)
			{
				xs.openStart(xmlTag(XT_OVERLAP));

				xs.writeAttribute(xmlTag(XT_FIRSTSTROKE), it->getFirst()+1);
				xs.writeAttribute(xmlTag(XT_SECONDSTROKE), it->getSecond()+1);
				
				const Point& pt = it->getPoint();
				xs.writeAttribute(xmlTag(XT_X), pt.x());
				xs.writeAttribute(xmlTag(XT_Y), pt.y());

				xs.closeStart(false);
			}
			xs.writeEnd(xmlTag(XT_OVERLAPS));
		}	
		xs.writeEnd(xmlTag(XT_HANREFERENCE));

		xs.writeEnd(xmlTag(XT_HANREFERENCES));
	}

	// Record segments
	if (ST_ISANYSET(grfRecordDetail, STRD_SEGMENTS) && isValid(GI_COMPILED))
	{
		xs.writeStart(xmlTag(XT_SEGMENTS));
		for (size_t iSegment=0; iSegment < _vecSegments.size(); ++iSegment)
		{
			const Segment& sg = _vecSegments[iSegment];
			Range rgSegment(codonToBaseRange(sg.getRange()));
			
			xs.openStart(xmlTag(XT_SEGMENT));
			xs.writeAttribute(xmlTag(XT_BASEFIRST), rgSegment.getStart()+1);
			xs.writeAttribute(xmlTag(XT_BASELAST), rgSegment.getEnd()+1);
			xs.writeAttribute(xmlTag(XT_COHERENT), sg.isCoherent());
			xs.closeStart();
			
			ASSERT(sg.getStart() >= 1);
			ASSERT(sg.getEnd() >= sg.getStart());

			// Record from the point before the segment through the segment end
			// - Since points contain the location *after* applying the vector,
			//   the point before marks where the segment actually begins
			for (size_t iPoint=sg.getStart()-1; iPoint <= static_cast<size_t>(sg.getEnd()); ++iPoint)
			{
				const Point& pt = _vecPoints[iPoint];
				xs.openStart(xmlTag(XT_POINT));
				xs.writeAttribute(xmlTag(XT_X), pt.x());
				xs.writeAttribute(xmlTag(XT_Y), pt.y());
				xs.closeStart(false);
			}
			
			xs.writeEnd(xmlTag(XT_SEGMENT));
		}
		xs.writeEnd(xmlTag(XT_SEGMENTS));
	}

	// Close the gene element
	xs.writeEnd(xmlTag(XT_GENE));
}

#ifdef ST_TRACE
/*
 * Function: traceGene
 *
 */
void
Gene::traceGene() const
{
	LOGTRACE((LLTRACE, "Gene start"));
	{
		ENTERTRACESCOPE(s1);

		LOGTRACE((LLTRACE, "Range: %ld codons, bases%s", Codon::numWholeCodons(_rgBases.getLength()), _rgBases.toString().c_str()));
		if (isValid(GI_SCORED))
			LOGTRACE((LLTRACE, "Score: %f", static_cast<UNIT>(_nScore)));
		if (isValid(GI_DIMENSIONS))
		{
			LOGTRACE((LLTRACE, "Bounds: %s", _rectBounds.toString().c_str()));
			LOGTRACE((LLTRACE, "Han Scale: sx/sy(%f,%f)", static_cast<UNIT>(_sxToHan), static_cast<UNIT>(_syToHan)));
			LOGTRACE((LLTRACE, "Han Translation: dx/dy(%f,%f)", static_cast<UNIT>(_dxToHan), static_cast<UNIT>(_dyToHan)));
		}

		if (Globals::getTraceLevel(STTC_DATA) >= STTL_L3)
		{
			if (isValid(GI_SEGMENTS))
				traceSegments();
			if (isValid(GI_STROKES))
			{
				traceStrokes();
				traceGroups();
			}
		}
	}
	LOGTRACE((LLTRACE, "Gene end"));
}

/*
 * Function: traceGroups
 *
 */
void
Gene::traceGroups() const
{
	const Han& han = Han::getDefinition(_strUnicode);
	const HGROUPARRAY& vecHGroups= han.getGroups();

	LOGTRACE((LLTRACE, "Groups: %ld", _vecGroups.size()));
	for (size_t i=0; i < _vecGroups.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		const NUMERICARRAY& vecContainedStrokes = vecHGroups[i].getStrokes();
		ostringstream ostr;

		for (size_t j=0; j < vecContainedStrokes.size(); ++j)
		{
			if (j)
				ostr << " ";
			ostr << _mapHanToStroke[vecContainedStrokes[j]];
		}

		if (Globals::getTraceLevel(STTC_DATA) <= STTL_L3)
		{
			LOGTRACE((LLTRACE, "Group %ld: strokes(%s)", i, ostr.str().c_str()));
		}
		else
		{
			const Group& grp = _vecGroups[i];
			
			LOGTRACE((LLTRACE, "Group Start - %ld", i));
			{
				ENTERTRACESCOPE(s2);
				LOGTRACE((LLTRACE, "Score: %f", static_cast<UNIT>(_nScore)));
				LOGTRACE((LLTRACE, "Strokes: %s", ostr.str().c_str()));
				LOGTRACE((LLTRACE, "Bounds: %s", grp.getBounds().toString().c_str()));
				LOGTRACE((LLTRACE, "Han Scale: sx/sy(%f,%f)", static_cast<UNIT>(grp.sxToHan()), static_cast<UNIT>(grp.syToHan())));
				LOGTRACE((LLTRACE, "Han Translation: dx/dy(%f,%f)", static_cast<UNIT>(grp.dxToHan()), static_cast<UNIT>(grp.dyToHan())));
				LOGTRACE((LLTRACE, "Han Parent Translation: dx/dy(%f,%f)", static_cast<UNIT>(grp.dxParentToHan()), static_cast<UNIT>(grp.dyParentToHan())));
			}
			LOGTRACE((LLTRACE, "Group End"));
		}
	}
}

/*
 * Function: traceOverlaps
 *
 */
void
Gene::traceOverlaps() const
{
	LOGTRACE((LLTRACE, "Han Overlaps %ld:", _vecHOverlaps.size()));
	for (size_t i=0; i < _vecHOverlaps.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		const HOverlap& hol = _vecHOverlaps[i];
		LOGTRACE((LLTRACE,
				  "%ld overlapping %ld is %s",
				  hol.getFirst(), hol.getSecond(),
				  (hol.isRequired() ? "required" : "allowed")));
	}

	LOGTRACE((LLTRACE, "Found Overlaps %ld:", _setOverlaps.size()));
	for (STROKEOVERLAPS::iterator it=_setOverlaps.begin(); it != _setOverlaps.end(); ++it)
	{
		ENTERTRACESCOPE(s1);
		const STROKEOVERLAP& ol = *it;
		LOGTRACE((LLTRACE,
				  "%ld overlaps %ld",
				  ol.getFirst(), ol.getSecond()));
	}

	LOGTRACE((LLTRACE, "Illegal Overlaps %ld:", _vecIllegalOverlaps.size()));
	for (size_t i=0; i < _vecIllegalOverlaps.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		const NUMERICPAIR& np = _vecIllegalOverlaps[i];
		LOGTRACE((LLTRACE,
				  "%ld overlapping %ld",
				  np.first, np.second));
	}

	LOGTRACE((LLTRACE, "Missing Overlaps %ld:", _vecMissingOverlaps.size()));
	for (size_t i=0; i < _vecMissingOverlaps.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		const NUMERICPAIR& np = _vecMissingOverlaps[i];
		LOGTRACE((LLTRACE,
				  "%ld overlapping %ld",
				  np.first, np.second));
	}
}

/*
 * Function: traceSegments
 *
 */
void
Gene::traceSegments() const
{
	LOGTRACE((LLTRACE, "Segments: %ld", _vecSegments.size()));
	if (Globals::getTraceLevel(STTC_DATA) >= STTL_L3)
	{
		ENTERTRACESCOPE(s1);
		for (size_t i=0; i < _vecSegments.size(); ++i)
		{
			const Segment& sg = _vecSegments[i];
			LOGTRACE((LLTRACE,
					  "Segment %ld: %s, codons %s, bases %s, length(%f)",
					  (i+1), (sg.isCoherent() ? "coherent" : "incoherent"),
					  sg.getRange().toString().c_str(),
					  codonToBaseRange(sg.getRange()).toString().c_str(),
					  sg.length()));
		}
	}
}

/*
 * Function: traceStrokes
 *
 */
void
Gene::traceStrokes() const
{
	LOGTRACE((LLTRACE, "Strokes: %ld", _vecStrokes.size()));
	for (size_t i=0; i < _vecStrokes.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		const Stroke& st = _vecStrokes[i];
		if (Globals::getTraceLevel(STTC_DATA) <= STTL_L3)
		{
			LOGTRACE((LLTRACE,
					  "Stroke %ld: %ld segments, %ld dropouts, codons %s, bases %s",
					  (i+1), st.getSegments(), st.getDropouts(),
					  st.getRange().toString().c_str(),
					  codonToBaseRange(st.getRange()).toString().c_str()));
		}
		else
		{
			LOGTRACE((LLTRACE, "Stroke Start - %ld", (i+1)));
			{
				ENTERTRACESCOPE(s2);
				LOGTRACE((LLTRACE, "Range: start/end%s, %ld total, %ld segments", st.getRange().toString().c_str(), st.getRange().getLength(), st.getSegments()));
				LOGTRACE((LLTRACE, "Dropouts: %ld", st.getDropouts()));
				LOGTRACE((LLTRACE, "Bounds: %s", st.getBounds().toString().c_str()));
				LOGTRACE((LLTRACE,
						  "Score: deviation(%f) extra-length(%f)",
						  static_cast<UNIT>(st.deviation()),
						  static_cast<UNIT>(st.extraLength())));
				LOGTRACE((LLTRACE, "Han Scale: sx/sy/sxy(%f,%f,%f)", static_cast<UNIT>(st.sxToHan()), static_cast<UNIT>(st.syToHan()), static_cast<UNIT>(st.sxyToHan())));
				LOGTRACE((LLTRACE, "Han Translation: dx/dy(%f,%f)", static_cast<UNIT>(st.dxToHan()), static_cast<UNIT>(st.dyToHan())));
				LOGTRACE((LLTRACE, "Han Parent Translation: dx/dy(%f,%f)", static_cast<UNIT>(st.dxParentToHan()), static_cast<UNIT>(st.dyParentToHan())));
			}
			LOGTRACE((LLTRACE, "Stroke End"));
		}
	}
}

/*
 * Function: traceStrokeOverlaps
 *
 */
void
Gene::traceStrokeOverlaps() const
{
	for (STROKEOVERLAPS::iterator it = _setOverlaps.begin();
		 it != _setOverlaps.end();
		 ++it)
	{
		const STROKEOVERLAP& ol = *it;
		LOGTRACE((LLTRACE,
				  "%ld overlaps %ld at %s",
				  ol.getFirst(),
				  ol.getSecond(),
				  ol.getPoint().toString().c_str()));
	}
}

/*
 * Function: traceScores
 *
 */
void
Gene::traceScores() const
{
	if (Globals::traceIf(STTR_SCORING, STTC_DATA, STTL_L4))
	{
		LOGTRACE((LLTRACE, "Score details for trial %ld", Genome::getTrial()));
		{
			LOGTRACE((LLTRACE, "Gene Weights:"));
			{
				ENTERTRACESCOPE(s0);
				
				LOGTRACE((LLTRACE, "Scale           : %0.15f", Globals::getGeneWeight(SC_SCALE)));
				LOGTRACE((LLTRACE, "Placement       : %0.15f", Globals::getGeneWeight(SC_PLACEMENT)));
				LOGTRACE((LLTRACE, "Illegal Overlaps: %0.15f", Globals::getGeneWeight(SC_ILLEGALOVERLAPS)));
				LOGTRACE((LLTRACE, "Missing Overlaps: %0.15f", Globals::getGeneWeight(SC_MISSINGOVERLAPS)));
				LOGTRACE((LLTRACE, "Marks           : %0.15f", Globals::getGeneWeight(SC_MARKS)));
			}

			LOGTRACE((LLTRACE, "Group Weights:"));
			{
				ENTERTRACESCOPE(s0);
				
				LOGTRACE((LLTRACE, "Scale           : %0.15f", Globals::getGroupWeight(SC_SCALE)));
				LOGTRACE((LLTRACE, "Placement       : %0.15f", Globals::getGroupWeight(SC_PLACEMENT)));
				LOGTRACE((LLTRACE, "Illegal Overlaps: %0.15f", Globals::getGroupWeight(SC_ILLEGALOVERLAPS)));
				LOGTRACE((LLTRACE, "Deviation       : %0.15f", Globals::getGroupWeight(SC_DEVIATION)));
				LOGTRACE((LLTRACE, "Extra Length    : %0.15f", Globals::getGroupWeight(SC_EXTRALENGTH)));
				LOGTRACE((LLTRACE, "Missing Overlaps: %0.15f", Globals::getGroupWeight(SC_MISSINGOVERLAPS)));
				LOGTRACE((LLTRACE, "Dropouts        : %0.15f", Globals::getGroupWeight(SC_DROPOUTS)));
			}
			
			LOGTRACE((LLTRACE, "Gene:"));

			ENTERTRACESCOPE(s0);
			LOGTRACE((LLTRACE, "nScore: %0.15f", static_cast<UNIT>(_nScore)));
			LOGTRACE((LLTRACE, "sxToHan: %0.15f", static_cast<UNIT>(_sxToHan)));
			LOGTRACE((LLTRACE, "syToHan: %0.15f", static_cast<UNIT>(_syToHan)));
			LOGTRACE((LLTRACE, "dxToHan: %0.15f", static_cast<UNIT>(_dxToHan)));
			LOGTRACE((LLTRACE, "dyToHan: %0.15f", static_cast<UNIT>(_dyToHan)));
			LOGTRACE((LLTRACE, "Scale Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_SCALE])));
			LOGTRACE((LLTRACE, "Place Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_PLACEMENT])));
			LOGTRACE((LLTRACE, "Joins Exponent: %0.15f (illegal overlaps)", static_cast<UNIT>(_aryScoreExponents[SC_ILLEGALOVERLAPS])));
			LOGTRACE((LLTRACE, "Inter Exponent: %0.15f (missing overlaps)", static_cast<UNIT>(_aryScoreExponents[SC_MISSINGOVERLAPS])));
			LOGTRACE((LLTRACE, "Marks Exponent: %0.15f", static_cast<UNIT>(_aryScoreExponents[SC_MARKS])));
		}
	}
	else
	{
		LOGTRACE((LLTRACE, "Gene Score %0.15f", static_cast<UNIT>(_nScore)));
	}

	LOGTRACE((LLTRACE, "Groups: %ld", _vecGroups.size()));
	for (size_t i=0; i < _vecGroups.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		_vecGroups[i].traceScore();
	}

	LOGTRACE((LLTRACE, "Strokes: %ld", _vecStrokes.size()));
	for (size_t i=0; i < _vecStrokes.size(); ++i)
	{
		ENTERTRACESCOPE(s1);
		_vecStrokes[i].traceScore();
	}
}
#endif

