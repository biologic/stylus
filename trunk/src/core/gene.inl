/*******************************************************************************
 * \file	gene.inl
 * \brief	Stylus Gene class inline methods
 *
 * Stylus, Copyright 2006-2008 Biologic Institute
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

using namespace std;
using namespace stylus;

//--------------------------------------------------------------------------------
//
// StrokeRange
//
//--------------------------------------------------------------------------------
inline StrokeRange::StrokeRange() {}
inline StrokeRange::StrokeRange(const StrokeRange& sr) { *this = sr; }

inline StrokeRange& StrokeRange::operator=(const StrokeRange& sr)
{
	Range::operator=(sr);
	_nCorrespondsTo = sr._nCorrespondsTo;
	return *this;
}

inline size_t StrokeRange::getCorrespondsTo() const { return _nCorrespondsTo; }

//--------------------------------------------------------------------------------
//
// Segment
//
//--------------------------------------------------------------------------------
inline Segment::Segment() : _fIsCoherent(false), _nLength(0) {}
inline Segment::Segment(const Segment& sg) { *this = sg; }

inline Segment& Segment::operator=(const Segment& sg)
{
	_rgAcids = sg._rgAcids;
	_fIsCoherent = sg._fIsCoherent;
	_nLength = sg._nLength;
	return *this;
}

inline Segment& Segment::operator=(ACIDTYPE at)
{
	_nLength = Acid::typeToAcid(at).getLength();
	return *this;
}

inline bool Segment::operator==(const Segment& sg) const { return (compare(sg) == 0); }
inline bool Segment::operator!=(const Segment& sg) const { return (compare(sg) != 0); }
inline bool Segment::operator<(const Segment& sg) const { return (compare(sg) < 0); }
inline bool Segment::operator>(const Segment& sg) const { return (compare(sg) > 0); }

inline void Segment::setRange(const Range& rgAcids) { _rgAcids.set(rgAcids); }
inline const Range& Segment::getRange() const { return _rgAcids; }

inline void Segment::setStart(long d) { _rgAcids.setStart(d); }
inline long Segment::getStart() const { return _rgAcids.getStart(); }

inline void Segment::setEnd(long d) { _rgAcids.setEnd(d); }
inline long Segment::getEnd() const { return _rgAcids.getEnd(); }

inline void Segment::extendRange(long d) { _rgAcids.extend(d); }
inline void Segment::moveRange(long d) { _rgAcids.move(d); }

inline bool Segment::isCoherent() const { return _fIsCoherent; }
inline void Segment::setCoherent(bool fIsCoherent) { _fIsCoherent = fIsCoherent; }

inline UNIT Segment::length() const { return _nLength; }

inline int Segment::compare(const Segment& sg) const
{
	return (_rgAcids < sg._rgAcids
			? -1
			: (_rgAcids > sg._rgAcids
			   ? 1
			   : (		_fIsCoherent == sg._fIsCoherent
					&&	_nLength == sg._nLength)
					? 0
					: 1));
}

inline Segment& stylus::operator+(Segment& sg, ACIDTYPE at)
{
	sg._nLength += Acid::typeToAcid(at).getLength();
	return sg;
}

inline Segment& stylus::operator+=(Segment& sg, ACIDTYPE at)
{
	return operator+(sg,at);
}

inline Segment& stylus::operator-(Segment& sg, ACIDTYPE at)
{
	sg._nLength -= Acid::typeToAcid(at).getLength();
	return sg;
}

inline Segment& stylus::operator-=(Segment& sg, ACIDTYPE at)
{
	return operator-(sg,at);
}

//--------------------------------------------------------------------------------
//
// Stroke
//
//--------------------------------------------------------------------------------
inline Stroke::Stroke() { invalidate(); }
inline Stroke::Stroke(const Stroke& st) { *this = st; }

inline bool Stroke::operator==(const Stroke& stroke) const { return (compare(stroke) == 0); }
inline bool Stroke::operator!=(const Stroke& stroke) const { return (compare(stroke) != 0); }
inline bool Stroke::operator<(const Stroke& stroke) const { return (compare(stroke) < 0); }
inline bool Stroke::operator>(const Stroke& stroke) const { return (compare(stroke) > 0); }

inline void Stroke::setID(size_t id) { _id = id; }
inline size_t Stroke::getID() const { return _id; }

inline void Stroke::setRange(const Range& rgAcids) { _rgAcids.set(rgAcids); }
inline const Range& Stroke::getRange() const { return _rgAcids; }

inline void Stroke::setStart(long d) { _rgAcids.setStart(d); }
inline long Stroke::getStart() const { return _rgAcids.getStart(); }

inline void Stroke::setEnd(long d) { _rgAcids.setEnd(d); }
inline long Stroke::getEnd() const { return _rgAcids.getEnd(); }

inline void Stroke::extendRange(long d) { _rgAcids.extend(d); }
inline void Stroke::moveRange(long d) { _rgAcids.move(d); }

inline void Stroke::incSegments(size_t n) { _cSegments += n; }
inline size_t Stroke::getSegments() const { return _cSegments; } 

inline void Stroke::incDropouts(size_t n) { _cDropouts += n; }
inline size_t Stroke::getDropouts() const { return _cDropouts; }

inline const Rectangle& Stroke::getBounds() const { return _rectBounds; }

inline const bool Stroke::sxIsInherited() const { return _sxIsInherited; }
inline const bool Stroke::syIsInherited() const { return _syIsInherited; }

inline const UNIT Stroke::sxToHan() const { return _sxToHan; }
inline const UNIT Stroke::syToHan() const { return _syToHan; }
inline const UNIT Stroke::sxyToHan() const { return _sxyToHan; }
inline const UNIT Stroke::dxToHan() const { return _dxToHan; }
inline const UNIT Stroke::dyToHan() const { return _dyToHan; }
inline const UNIT Stroke::dxParentToHan() const { return _dxParentToHan; }
inline const UNIT Stroke::dyParentToHan() const { return _dyParentToHan; }

inline const UNIT Stroke::deviation() const { return _nDeviation; }
inline const UNIT Stroke::extraLength() const { return _nExtraLength; }

inline int Stroke::compare(const Stroke& stroke) const
{
	return (_rgAcids < stroke._rgAcids
			? -1
			: (_rgAcids > stroke._rgAcids
			   ? 1
			   : (		_cSegments == stroke._cSegments
					&&	_cDropouts == stroke._cDropouts
					&&	_rectBounds == stroke._rectBounds
					&&	_sxToHan == stroke._sxToHan
					&&	_syToHan == stroke._syToHan
					&&	_sxyToHan == stroke._sxyToHan
					&&	_dxToHan == stroke._dxToHan
					&&	_dyToHan == stroke._dyToHan)
					? 0
					: 1));
}

//--------------------------------------------------------------------------------
//
// Group
//
//--------------------------------------------------------------------------------
inline Group::Group() { invalidate(); }
inline Group::Group(const Group& grp) { *this = grp; }

inline bool Group::operator==(const Group& grp) const { return (compare(grp) == 0); }
inline bool Group::operator!=(const Group& grp) const { return (compare(grp) != 0); }

inline void Group::setID(size_t id) { _id = id; }
inline size_t Group::getID() const { return _id; }

inline void Group::incIllegalOverlaps() { ++_aryScoreExponents[SC_ILLEGALOVERLAPS]; }
inline void Group::incMissingOverlaps() { ++_aryScoreExponents[SC_MISSINGOVERLAPS]; }

inline const Rectangle& Group::getBounds() const { return _rectBounds; }

inline const bool Group::sxIsInherited() const { return _sxIsInherited; }
inline const bool Group::syIsInherited() const { return _syIsInherited; }

inline const UNIT Group::sxToHan() const { return _sxToHan; }
inline const UNIT Group::syToHan() const { return _syToHan; }
inline const UNIT Group::dxToHan() const { return _dxToHan; }
inline const UNIT Group::dyToHan() const { return _dyToHan; }
inline const UNIT Group::dxParentToHan() const { return _dxParentToHan; }
inline const UNIT Group::dyParentToHan() const { return _dyParentToHan; }

inline const UNIT Group::exponent(SCORECOMPONENT sc) const { ASSERT(sc < SC_GROUPMAX); return _aryScoreExponents[sc]; }
inline const UNIT Group::score() const { return _nScore; }

inline int Group::compare(const Group& grp) const
{
	return (	_rectBounds == grp._rectBounds
			&&	_sxToHan == grp._sxToHan
			&&	_syToHan == grp._syToHan
			&&	_dxToHan == grp._dxToHan
			&&	_dyToHan == grp._dyToHan
			&&	_dxParentToHan == grp._dxParentToHan
			&&	_dyParentToHan == grp._dyParentToHan
			&& _nScore == grp._nScore)
			? 0
			: 1;
}

//--------------------------------------------------------------------------------
//
// StrokeRangeChange
//
//--------------------------------------------------------------------------------
inline StrokeRangeChange::~StrokeRangeChange() {}

inline size_t StrokeRangeChange::length() const { return 0; }
inline void StrokeRangeChange::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const {}

//--------------------------------------------------------------------------------
//
// Gene
//
//--------------------------------------------------------------------------------
inline Gene::Gene() { markInvalid(GI_ALL); }
inline Gene::Gene(const Gene& gene) { *this = gene; }

inline bool Gene::operator==(const Gene& gene) const { return (compare(gene) == 0); }
inline bool Gene::operator!=(const Gene& gene) const { return (compare(gene) != 0); }

inline void Gene::setID(size_t id) { _id = id; }
inline size_t Gene::getID() const { return _id; }

inline const Range& Gene::getRange() const { return _rgBases; }

inline void Gene::extendRange(long d) { _rgBases.extend(d); markInvalid(GI_ALL); }
inline void Gene::moveRange(long d) { _rgBases.move(d); }

inline bool Gene::intersectsRange(const Range& rg) const { return _rgBases.intersects(rg); }

inline const ACIDTYPEARRAY& Gene::getAcids() const { return _vecAcids; }
inline const POINTARRAY& Gene::getPoints() const { return _vecPoints; }
inline const SEGMENTARRAY& Gene::getSegments() const { return _vecSegments; }
inline const std::string& Gene::getUnicode() const { return _strUnicode; }
inline const STROKEARRAY& Gene::getStrokes() const { return _vecStrokes; }
inline STROKEARRAY& Gene::getStrokes() { return _vecStrokes; }

inline void Gene::setOrigin(const Point& ptOrigin) { _ptOrigin = ptOrigin; markInvalid(GI_POINTS); }

inline size_t Gene::mapHanToStroke(size_t iHStroke) const { ASSERT(iHStroke < _mapHanToStroke.size()); return _mapHanToStroke[iHStroke]; }
inline size_t Gene::mapStrokeToHan(size_t iStroke) const { ASSERT(iStroke < _mapStrokeToHan.size()); return _mapStrokeToHan[iStroke]; }
inline size_t Gene::mapStrokeToGroup(size_t iStroke) const { ASSERT(iStroke < _mapStrokeToGroup.size()); return _mapStrokeToGroup[iStroke]; }

inline const Rectangle& Gene::getBounds() const { return _rectBounds; }
inline const UNIT Gene::sxToHan() const { return _sxToHan; }
inline const UNIT Gene::syToHan() const { return _syToHan; }
inline const UNIT Gene::dxToHan() const { return _dxToHan; }
inline const UNIT Gene::dyToHan() const { return _dyToHan; }

inline const UNIT Gene::score() const { return _nScore; }
inline const UNIT Gene::units() const { return _nUnits; }

inline void Gene::markInvalid() { markInvalid(GI_ALL); }

inline bool Gene::isInvalid(STFLAGS grf) const { return (_grfInvalid & grf); }
inline bool Gene::isValid(STFLAGS grf) const { return !(_grfInvalid & grf); }
inline void Gene::markValid(STFLAGS grf) { _grfInvalid &= ~grf; }

inline Range Gene::codonToBaseRange(const Range& rgCodons) const
{
	ASSERT(!rgCodons.isEmpty());
	return Range((rgCodons.getStart() * Codon::s_cchCODON) + _rgBases.getStart(),
				 (rgCodons.getEnd() * Codon::s_cchCODON) + _rgBases.getStart() + 2);
}
inline Range Gene::baseToCodonRange(const Range& rgBases) const
{
	ASSERT(!rgBases.isEmpty());
	return Range((rgBases.getStart() - _rgBases.getStart()) / Codon::s_cchCODON,
				 (rgBases.getEnd() - _rgBases.getStart() - 2) / Codon::s_cchCODON);
}

inline int Gene::compare(const Gene& gene) const
{
	return (_rgBases < gene._rgBases
			? -1
			: (_rgBases > gene._rgBases
				? 1
				: (		_vecAcids == gene._vecAcids
					&&	_vecPoints == gene._vecPoints
					&&	_vecCoherent == gene._vecCoherent
					&&	_vecSegments == gene._vecSegments
					&&	_vecStrokes == gene._vecStrokes
					&&	_vecGroups == gene._vecGroups
					&&	_vecMarks == gene._vecMarks)
					? 0
					: 1));
}
