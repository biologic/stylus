/*******************************************************************************
 * \file	helpers.cpp
 * \brief	Stylus Helper classes
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


// Includes ---------------------------------------------------------------------
#include "headers.hpp"

using namespace std;
using namespace stylus;

//--------------------------------------------------------------------------------
//
// Range
//
//--------------------------------------------------------------------------------

/*
 * Function: toString
 *
 */
string
Range::toString(bool fAsPair) const
{
	ostringstream ostr;
	if (fAsPair)
	{
		if (isEmpty())
			ostr << "(empty)";
		else
			ostr << "(" << _iStart << "," << (_iEnd-1) << ")";
	}
	else
		ostr << _iStart << " " << (_iEnd-1);
	return string(ostr.str());
}

//--------------------------------------------------------------------------------
//
// Unit
//
//--------------------------------------------------------------------------------

UNIT Unit::s_nMaxRelativeError = 10000;
UNIT Unit::s_nEpsilon = numeric_limits<UNIT>::epsilon();
UNIT Unit::s_nNegativeZero = 0 - (Unit::s_nMaxRelativeError * Unit::s_nEpsilon);
UNIT Unit::s_nPositiveZero = 0 + (Unit::s_nMaxRelativeError * Unit::s_nEpsilon);
UNIT Unit::s_nLargeUnity = 1 + (Unit::s_nMaxRelativeError * Unit::s_nEpsilon);
UNIT Unit::s_nSmallUnity = 1 - (Unit::s_nMaxRelativeError * Unit::s_nEpsilon);
UNIT Unit::s_nMax = numeric_limits<UNIT>::max();
UNIT Unit::s_nMin = numeric_limits<UNIT>::min();

void Unit::logConstants()
{
	LOGINFO((LLINFO, "Unit constants"));
	LOGINFO((LLINFO, " - MaxRelativeError: %0.16f\n", s_nMaxRelativeError));
	LOGINFO((LLINFO, " - Epsilon         : %0.16f\n", s_nEpsilon));
	LOGINFO((LLINFO, " - LargeUnity      : %0.16f\n", s_nLargeUnity));
	LOGINFO((LLINFO, " - SmallUnity      : %0.16f\n", s_nSmallUnity));
	LOGINFO((LLINFO, " - Max             : %0.16f\n", s_nMax));
	LOGINFO((LLINFO, " - Min             : %0.16f\n", s_nMin));
}

//--------------------------------------------------------------------------------
//
// Point
//
//--------------------------------------------------------------------------------

/*
 * Function: load
 *
 */
void
Point::load(XMLDocument* pxd, xmlNodePtr pxnPoint)
{
	DASSERT(VALID(pxd));
	DASSERT(VALID(pxnPoint));

	string strValue;
	pxd->getAttribute(pxnPoint, xmlTag(XT_X), strValue); DASSERT(!EMPTYSTR(strValue)); _x = strValue;
	pxd->getAttribute(pxnPoint, xmlTag(XT_Y), strValue); DASSERT(!EMPTYSTR(strValue)); _y = strValue;
}

/*
 * Function: toString
 *
 */
string
Point::toString() const
{
	ostringstream ostr;
	ostr << "(" << static_cast<UNIT>(_x) << "," << static_cast<UNIT>(_y) << ")";
	return string(ostr.str());
}

//--------------------------------------------------------------------------------
//
// PointDistance
//
//--------------------------------------------------------------------------------

/*
 * Function: load
 *
 */
void
PointDistance::load(XMLDocument* pxd, xmlNodePtr pxnPointDistance)
{
	DASSERT(VALID(pxd));
	DASSERT(VALID(pxnPointDistance));

	Point::load(pxd, pxnPointDistance);

	string strValue;
	pxd->getAttribute(pxnPointDistance, xmlTag(XT_FRACTIONALDISTANCE), strValue); _nDistance = strValue;
}

/*
 * Function: toString
 *
 */
string
PointDistance::toString() const
{
	ostringstream ostr;
	ostr << "(" << static_cast<UNIT>(_x) << "," << static_cast<UNIT>(_y) << "," << static_cast<UNIT>(_nDistance) << ")";
	return string(ostr.str());
}

//--------------------------------------------------------------------------------
//
// Line
//
//--------------------------------------------------------------------------------

/*
 * Function: set
 *
 */
void
Line::set(const Point& ptStart, const Point& ptEnd, bool fCanonical)
{
	DASSERT(_ptStart.isDefined());
	DASSERT(_ptEnd.isDefined());

	_fCanonical = fCanonical;
	if (	!_fCanonical
	   ||	ptStart.x() < ptEnd.x()
	   ||	(	ptStart.x() == ptEnd.x()
			&&	ptStart.y() > ptEnd.y()))
	{
		_ptStart = ptStart;
		_ptEnd = ptEnd;
	}
	else
	{
		_ptStart = ptEnd;
		_ptEnd = ptStart;
	}

	_dx = _ptEnd.x() - _ptStart.x();
	_dy = _ptEnd.y() - _ptStart.y();

	_nSlope = _dy / _dx;
	if (_nSlope.isZero())
		_nSlope = static_cast<UNIT>(0.0);

	_yIntercept = _ptStart.y() - (_nSlope * _ptStart.x());

	_nLength = ::sqrt((_dx*_dx)+(_dy*_dy));
}

/*
 * Function: move
 *
 */
void
Line::move(const Point& pt)
{
	DASSERT(_ptStart.isDefined());
	DASSERT(_ptEnd.isDefined());

	_ptStart.move(pt);
	_ptEnd.move(pt);

	_yIntercept = _ptStart.y() + (_nSlope * -_ptStart.x());
}

/*
 * Function: toString
 *
 */
string
Line::toString() const
{
	ostringstream ostr;
	ostr << "id(" << _id << ") "
		 << "start" << _ptStart.toString() << " "
		 << "end" << _ptEnd.toString() << " "
		 << "dx/dy(" << static_cast<UNIT>(_dx) << "," << static_cast<UNIT>(_dy) << ") "
		 << "slope(" << static_cast<UNIT>(_nSlope) << ") "
		 << "length(" << static_cast<UNIT>(_nLength) << ") "
		 << "y-int(" << static_cast<UNIT>(_yIntercept) << ")";
	return string(ostr.str());
}

//------------------------------------------------------------------------------
//
// Rectangle
//
//------------------------------------------------------------------------------

const char* Rectangle::s_aryRECTANGLEITEMS[RI_MAX] =
{
	"bounds",
	"top",
	"left",
	"bottom",
	"right",
	"width",
	"height",
	"x-midpoint",
	"y-midpoint"
};

/*
 * Function: set
 *
 */
void
Rectangle::set(UNIT yTop, UNIT xLeft, UNIT yBottom, UNIT xRight)
{
	Unit xMin = ::min<UNIT>(xLeft, xRight);
	Unit xMax = ::max<UNIT>(xLeft, xRight);
	Unit yMin = ::min<UNIT>(yTop, yBottom);
	Unit yMax = ::max<UNIT>(yTop, yBottom);

	_ptTopLeft.set(xMin, yMax);
	_ptBottomRight.set(xMax, yMin);

	ensureDimensions();
}

void
Rectangle::set(const Rectangle& r)
{
	_ptTopLeft = r._ptTopLeft;
	_ptBottomRight = r._ptBottomRight;
	_ptCenter = r._ptCenter;
	_dxWidth = r._dxWidth;
	_dyHeight = r._dyHeight;
}

/*
 * Function: load
 *
 */
void
Rectangle::load(XMLDocument* pxd, xmlNodePtr pxnBounds)
{
	DASSERT(VALID(pxd));
	DASSERT(VALID(pxnBounds));
	DASSERT(ISEQUALXMLSTR(pxnBounds->name, s_aryRECTANGLEITEMS[RI_BOUNDS]));

	string strValue;
	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_TOP], strValue); DASSERT(!EMPTYSTR(strValue)); _ptTopLeft._y = strValue;
	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_LEFT], strValue); DASSERT(!EMPTYSTR(strValue)); _ptTopLeft._x = strValue;

	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_BOTTOM], strValue); DASSERT(!EMPTYSTR(strValue)); _ptBottomRight._y = strValue;
	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_RIGHT], strValue); DASSERT(!EMPTYSTR(strValue)); _ptBottomRight._x = strValue;

	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_XMIDPOINT], strValue); DASSERT(!EMPTYSTR(strValue)); _ptCenter._x = strValue;
	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_YMIDPOINT], strValue); DASSERT(!EMPTYSTR(strValue)); _ptCenter._y = strValue;

	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_WIDTH], strValue); DASSERT(!EMPTYSTR(strValue)); _dxWidth = strValue;
	pxd->getAttribute(pxnBounds, s_aryRECTANGLEITEMS[RI_HEIGHT], strValue); DASSERT(!EMPTYSTR(strValue)); _dyHeight = strValue;
}

/*
 * Function: combine
 *
 */
void
Rectangle::combine(UNIT yTop, UNIT xLeft, UNIT yBottom, UNIT xRight)
{
	Unit xMin = ::min<UNIT>(xLeft, xRight);
	Unit xMax = ::max<UNIT>(xLeft, xRight);
	Unit yMin = ::min<UNIT>(yTop, yBottom);
	Unit yMax = ::max<UNIT>(yTop, yBottom);

	if (xMin < _ptTopLeft.x())
		_ptTopLeft.x() = xMin;
	if (yMax > _ptTopLeft.y())
		_ptTopLeft.y() = yMax;

	if (xMax > _ptBottomRight.x())
		_ptBottomRight.x() = xMax;
	if (yMin < _ptBottomRight.y())
		_ptBottomRight.y() = yMin;

	ensureDimensions();
}

/*
 * Function: move
 *
 */
void
Rectangle::move(UNIT dx, UNIT dy)
{
	_ptTopLeft.move(dx,dy);
	_ptBottomRight.move(dx,dy);
	_ptCenter.move(dx,dy);
}

/*
 * Function: scale
 *
 */
void
Rectangle::scale(UNIT sx, UNIT sy)
{
	_ptTopLeft.scale(sx,sy);
	_ptBottomRight.scale(sx,sy);

	ensureDimensions();
}

/*
 * Function: ensureDimensions
 *
 */
void
Rectangle::ensureDimensions()
{
	_dxWidth = _ptBottomRight.x() - _ptTopLeft.x();
	_dyHeight = _ptTopLeft.y() - _ptBottomRight.y();

	_ptCenter.set((_ptTopLeft.x() + (_dxWidth / 2)),
				  (_ptBottomRight.y() + (_dyHeight / 2)));
}

/*
 * Function: toString
 *
 */
string
Rectangle::toString() const
{
	ostringstream ostr;
	ostr << "t/l(" << static_cast<UNIT>(_ptTopLeft.y()) << "," << static_cast<UNIT>(_ptTopLeft.x()) << ") "
		 << "b/r(" << static_cast<UNIT>(_ptBottomRight.y()) << "," << static_cast<UNIT>(_ptBottomRight.x()) << ") "
		 << "x/y-mid" << _ptCenter.toString() << " "
		 << "dx/dy(" << static_cast<UNIT>(_dxWidth) << "," << static_cast<UNIT>(_dyHeight) << ")";
	return string(ostr.str());
}

/*
 * Function: toXML
 *
 */
void
Rectangle::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	xs.openStart(xmlTag(XT_BOUNDS));
	xs.writeAttribute(xmlTag(XT_TOP), _ptTopLeft.y());
	xs.writeAttribute(xmlTag(XT_LEFT), _ptTopLeft.x());
	xs.writeAttribute(xmlTag(XT_BOTTOM), _ptBottomRight.y());
	xs.writeAttribute(xmlTag(XT_RIGHT), _ptBottomRight.x());
	xs.writeAttribute(xmlTag(XT_WIDTH), _dxWidth);
	xs.writeAttribute(xmlTag(XT_HEIGHT), _dyHeight);
	xs.writeAttribute(xmlTag(XT_XMIDPOINT), _ptCenter.x());
	xs.writeAttribute(xmlTag(XT_YMIDPOINT), _ptCenter.y());
	xs.closeStart(false);
}
	
