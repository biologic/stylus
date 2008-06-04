/*******************************************************************************
 * \file	han.inl
 * \brief	Stylus Han and helper classes inline methods
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
// HDimensions
//
//--------------------------------------------------------------------------------
inline HDimensions::HDimensions() {}
inline HDimensions::HDimensions(const HDimensions& hd) { *this = hd; }

inline HDimensions& HDimensions::operator=(const HDimensions& hd) { set(hd); return *this; }

inline UNIT HDimensions::getLength() const { return _nLength; }
inline const Rectangle& HDimensions::getBounds() const { return _rectBounds; }

//--------------------------------------------------------------------------------
//
// HGroup
//
//--------------------------------------------------------------------------------
inline HGroup::HGroup() {}
inline HGroup::HGroup(const HGroup& hg) { *this = hg; }

inline HGroup& HGroup::operator=(const HGroup& hg) { set(hg); return *this; }

inline const Point& HGroup::getWeightedCenter() const { return _ptWeightedCenter; }
inline const NUMERICARRAY& HGroup::getStrokes() const { return _vecStrokes; }

//--------------------------------------------------------------------------------
//
// HStroke
//
//--------------------------------------------------------------------------------
inline HStroke::HStroke() {}
inline HStroke::HStroke(const HStroke& hs) { *this = hs; }

inline HStroke& HStroke::operator=(const HStroke& hs) { set(hs); return *this; }

inline const HPOINTARRAY& HStroke::getPointsForward() const { return _vecPointsForward; }
inline const HPOINTARRAY& HStroke::getPointsReverse() const { return _vecPointsReverse; }

//--------------------------------------------------------------------------------
//
// HOverlap
//
//--------------------------------------------------------------------------------
inline HOverlap::HOverlap() {}
inline HOverlap::HOverlap(const HOverlap& ho) { *this = ho; }

inline HOverlap& HOverlap::operator=(const HOverlap& ho) { set(ho); return *this; }

inline void HOverlap::set(const HOverlap& ho)
{
	_iStroke1 = ho._iStroke1;
	_iStroke2 = ho._iStroke2;
	_fRequired = ho._fRequired;
}

inline bool HOverlap::operator==(const HOverlap& hol) const { return (compare(hol) == 0); }
inline bool HOverlap::operator<(const HOverlap& hol) const { return (compare(hol) < 0); }
inline bool HOverlap::operator<=(const HOverlap& hol) const { return (compare(hol) <= 0); }
inline bool HOverlap::operator>(const HOverlap& hol) const { return (compare(hol) > 0); }
inline bool HOverlap::operator>=(const HOverlap& hol) const { return (compare(hol) >= 0); }

// Sort by ascending by first stroke, then second stroke, and the required before allowed overlaps
inline int HOverlap::compare(const HOverlap& hol) const
{
	// Assert canonical stroke ordering in the overlaps
	ASSERT(_iStroke1 <= _iStroke2);
	ASSERT(hol.getFirst() <= hol.getSecond());
	return (_iStroke1 < hol.getFirst()
			? -1
			: (_iStroke1 > hol.getFirst()
			   ? 1
			   : (_iStroke2 < hol.getSecond()
				  ? -1
				  : (_iStroke2 > hol.getSecond()
					 ? 1
					 : (_fRequired && !hol.isRequired()
						? -1
						: (!_fRequired && hol.isRequired()
							? 1
							: 0))))));
}

inline bool HOverlap::operator==(const STROKEOVERLAP& ol) const { return (compare(ol) == 0); }
inline bool HOverlap::operator<(const STROKEOVERLAP& ol) const { return (compare(ol) < 0); }
inline bool HOverlap::operator<=(const STROKEOVERLAP& ol) const { return (compare(ol) <= 0); }
inline bool HOverlap::operator>(const STROKEOVERLAP& ol) const { return (compare(ol) > 0); }
inline bool HOverlap::operator>=(const STROKEOVERLAP& ol) const { return (compare(ol) >= 0); }

// Sort by ascending by first stroke, then second stroke
inline int HOverlap::compare(const STROKEOVERLAP& ol) const
{
	// Assert canonical stroke ordering in the overlaps
	ASSERT(_iStroke1 <= _iStroke2);
	ASSERT(ol.getFirst() <= ol.getSecond());
	return (_iStroke1 < ol.getFirst()
			? -1
			: (_iStroke1 > ol.getFirst()
			   ? 1
			   : (_iStroke2 < ol.getSecond()
				  ? -1
				  : (_iStroke2 > ol.getSecond()
					? 1
					: 0))));
}

inline size_t HOverlap::getFirst() const { return _iStroke1; }
inline size_t HOverlap::getSecond() const { return _iStroke2; }

inline bool HOverlap::isRequired() const { return _fRequired; }

inline void HOverlap::convertStrokes(const NUMERICMAP& mapHanToStroke)
{
	ASSERT(_iStroke1 < mapHanToStroke.size());
	ASSERT(_iStroke2 < mapHanToStroke.size());
	_iStroke1 = mapHanToStroke[_iStroke1];
	_iStroke2 = mapHanToStroke[_iStroke2];
	if (_iStroke1 > _iStroke2)
		swap<size_t>(_iStroke1, _iStroke2);
}

//--------------------------------------------------------------------------------
//
// Han
//
//--------------------------------------------------------------------------------
inline Han::Han() {}
inline Han::Han(const Han& han) { *this = han; }

inline Han& Han::operator=(const Han& han) { set(han); return *this; }

inline bool Han::operator==(const Han& han) const { return _strUnicode == han._strUnicode; }
inline bool Han::operator!=(const Han& han) const { return _strUnicode != han._strUnicode; }
inline bool Han::operator<(const Han& han) const { return _strUnicode < han._strUnicode; }
inline bool Han::operator<=(const Han& han) const { return _strUnicode <= han._strUnicode; }
inline bool Han::operator>(const Han& han) const { return _strUnicode > han._strUnicode; }
inline bool Han::operator>=(const Han& han) const { return _strUnicode >= han._strUnicode; }

inline bool Han::operator==(const std::string& strUnicode) const { return _strUnicode == strUnicode; }
inline bool Han::operator!=(const std::string& strUnicode) const { return _strUnicode != strUnicode; }
inline bool Han::operator<(const std::string& strUnicode) const { return _strUnicode < strUnicode; }
inline bool Han::operator<=(const std::string& strUnicode) const { return _strUnicode <= strUnicode; }
inline bool Han::operator>(const std::string& strUnicode) const { return _strUnicode > strUnicode; }
inline bool Han::operator>=(const std::string& strUnicode) const { return _strUnicode >= strUnicode; }

inline const std::string& Han::getUUID() const { return _strUUID; }
inline const std::string& Han::getUnicode() const { return _strUnicode; }

inline UNIT Han::getMinimumStrokeLength() const { return _nMinimumStrokeLength; }

inline const HGROUPARRAY& Han::getGroups() const { return _vecGroups; }
inline const HSTROKEARRAY& Han::getStrokes() const { return _vecStrokes; }
inline const HOVERLAPARRAY& Han::getOverlaps() const { return _vecOverlaps; }

inline const HGroup& Han::mapStrokeToGroup(size_t iStroke) const
{
	ASSERT(iStroke < _mapStrokeToGroup.size());
	ASSERT(_mapStrokeToGroup.size() == _vecStrokes.size());
	return _vecGroups[_mapStrokeToGroup[iStroke]];
}

