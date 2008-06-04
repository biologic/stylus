/*******************************************************************************
 * \file	codon.inl
 * \brief	Stylus Codon class inline methods
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
// Acid
//
//--------------------------------------------------------------------------------
inline const Acid& Acid::typeToAcid(ACIDTYPE at) { return s_aryACIDS[at]; }

inline ACIDTYPE Acid::vectorToType(const char* pszVector)
{
	ASSERT(VALID(pszVector));

	for (size_t at=ACID_STP; at < ACID_MAX; ++at)
	{
		if (!::strncmp(pszVector, s_aryACIDS[at]._pszName, Codon::s_cchCODON))
			return (ACIDTYPE)at;
	}

	return ACID_MAX;
}

inline bool Acid::isStop() const { return _nDirection == DIR_STOP; }
inline UNIT Acid::getDX() const { return _dx; }
inline UNIT Acid::getDY() const { return _dy; }
inline UNIT Acid::getLength() const { return _nLength; }
inline DIRECTION Acid::getDirection() const { return _nDirection; }
inline const char* Acid::getName() const { return _pszName; }

inline bool Acid::isHorizontal() const { return ((_nDirection & 0x3) ^ 0x3) == 0; }
inline bool Acid::isVertical() const { return ((_nDirection & 0x3) ^ 0x1) == 0; }
inline bool Acid::isDiagonal() const { return (_nDirection & 0x1) == 0; }

inline Point& stylus::operator+(Point& pt, ACIDTYPE at)
{
	pt.move(Acid::s_aryACIDS[at]._dx, Acid::s_aryACIDS[at]._dy);
	return pt;
}

inline Point& stylus::operator+=(Point& pt, ACIDTYPE at)
{
	return operator+(pt,at);
}

inline Point& stylus::operator-(Point& pt, ACIDTYPE at)
{
	pt.move(-Acid::s_aryACIDS[at]._dx, -Acid::s_aryACIDS[at]._dy);
	return pt;
}

inline Point& stylus::operator-=(Point& pt, ACIDTYPE at)
{
	return operator-(pt,at);
}

//--------------------------------------------------------------------------------
//
// ScaledLength
//
//--------------------------------------------------------------------------------
inline ScaledLength::ScaledLength(UNIT sx, UNIT sy, UNIT sxy) { clear(); setScale(sx,sy,sxy); }
inline ScaledLength::ScaledLength(const ScaledLength& sl) { *this = sl; }

inline ScaledLength& ScaledLength::operator=(const ScaledLength& sl)
{
	_dxVectors = sl._dxVectors;
	_dyVectors = sl._dyVectors;
	_dxyVectors = sl._dxyVectors;

	_sx = sl._sx;
	_sy = sl._sy;
	_sxy = sl._sxy;

	return *this;
}

inline ScaledLength& ScaledLength::operator+=(ACIDTYPE at) { return operator+=(Acid::typeToAcid(at)); }
inline ScaledLength& ScaledLength::operator+=(const Acid& acid)
{
	if (acid.isHorizontal())
	{
		ASSERT(std::abs(acid.getDX()) == acid.getLength());
		_dxVectors += acid.getLength();
	}
	else if (acid.isVertical())
	{
		ASSERT(std::abs(acid.getDY()) == acid.getLength());
		_dyVectors += acid.getLength();
	}
	else
	{
		ASSERT(acid.isDiagonal());
		ASSERT(std::abs(acid.getDX()) == std::abs(acid.getDY()));
		_dxyVectors += std::abs(acid.getDX());
	}

	return *this;
}

inline ScaledLength& ScaledLength::operator-=(ACIDTYPE at) { return operator-=(Acid::typeToAcid(at)); }
inline ScaledLength& ScaledLength::operator-=(const Acid& acid)
{
	if (acid.isHorizontal())
	{
		ASSERT(std::abs(acid.getDX()) == acid.getLength());
		_dxVectors -= acid.getDX();
	}
	else if (acid.isVertical())
	{
		ASSERT(std::abs(acid.getDY()) == acid.getLength());
		_dyVectors -= acid.getDY();
	}
	else
	{
		ASSERT(acid.isDiagonal());
		ASSERT(std::abs(acid.getDX()) == std::abs(acid.getDY()));
		_dxyVectors -= std::abs(acid.getDX());
	}

	return *this;
}

inline void ScaledLength::setScale(UNIT sx, UNIT sy, UNIT sxy) { _sx = sx; _sy = sy; _sxy = sxy; }

inline UNIT ScaledLength::getLength() const
{
	ASSERT(_dxVectors >= static_cast<UNIT>(0));
	ASSERT(_dyVectors >= static_cast<UNIT>(0));
	ASSERT(_dxyVectors >= static_cast<UNIT>(0));
	return (_dxVectors * _sx)
		+  (_dyVectors * _sy)
		+  (_dxyVectors * _sxy);
}

inline void ScaledLength::clear()
{
	_dxVectors = 0.0; _dyVectors = 0.0; _dxyVectors = 0.0;
	_sx = 1.0; _sy = 1.0; _sxy = 1.0;
}

//--------------------------------------------------------------------------------
//
// Codon
//
//--------------------------------------------------------------------------------
inline bool Codon::isCoherent(ACIDTYPE atFirst, ACIDTYPE atSecond, ACIDTYPE atThird)
{
	return s_aryCOHERENCE[atFirst][atSecond][atThird];
}

inline Acid Codon::codonToAcid(const char* pszCodon)
{
	return Acid::typeToAcid(Genome::codonToType(pszCodon));
}

inline bool Codon::onCodonBoundary(size_t iBase) { return ((iBase % s_cchCODON) == 0); }
inline size_t Codon::toCodonBoundary(size_t iBase) { return (iBase - (iBase % s_cchCODON)); }
inline size_t Codon::toCodonIndex(size_t iBase) { return (iBase / s_cchCODON); }
inline size_t Codon::toCodonOffset(size_t iBase) { return (iBase % s_cchCODON); }

inline bool Codon::hasWholeCodons(long cbBases) { return ((cbBases % static_cast<long>(s_cchCODON)) == 0); }
inline long Codon::numWholeCodons(long cbBases) { return (cbBases / static_cast<long>(s_cchCODON)); }
inline long Codon::numAffectedCodons(size_t iBase, long cbBases)
{
	return numWholeCodons(cbBases) + (hasWholeCodons(cbBases) ? 0 : 1) + (onCodonBoundary(iBase) ? 0 : 1);
}
inline long Codon::numFrameShift(long cbBases) { return (cbBases % static_cast<long>(s_cchCODON)); }

inline bool Codon::isStart(const std::string& strCodon) { return isStart(strCodon.c_str()); }
inline bool Codon::isStart(const char* pszCodon)
{
	ASSERT(pszCodon);
	ASSERT(s_strSTART.length() <= ::strlen(pszCodon));
	for (size_t i=0; i < s_strSTART.length(); ++i, ++pszCodon)
		if (s_strSTART[i] != *pszCodon)
			return false;
	return true;
}

inline bool Codon::isStop(const std::string& strCodon) { return isStop(strCodon.c_str()); }
inline bool Codon::isStop(const char* pszCodon)
{
	return (Genome::codonToType(pszCodon) == ACID_STP);
}

inline bool Codon::isSilentChange(const std::string& strBases, size_t iBaseChange, const std::string& strBasesChange)
{
	string strBasesBefore(strBases.substr(toCodonBoundary(iBaseChange), numAffectedCodons(iBaseChange, strBasesChange.length()) * s_cchCODON));
	string strBasesAfter(strBasesBefore);
	strBasesAfter.replace(toCodonOffset(iBaseChange), strBasesChange.length(), strBasesChange);
	
	const char* pszBasesBefore = strBasesBefore.c_str();
	const char* pszBasesAfter = strBasesAfter.c_str();

	for (size_t iBase=0; iBase < strBasesBefore.length(); iBase += s_cchCODON)
	{
		if (Genome::codonToType(pszBasesBefore+iBase) != Genome::codonToType(pszBasesAfter+iBase))
			return false;
	}

	return true;
}
