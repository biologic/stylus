/*******************************************************************************
 * \file	helpers.inl
 * \brief	Stylus helper classes inline methods
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

using namespace std;
using namespace stylus;

//--------------------------------------------------------------------------------
//
// Range
//
//--------------------------------------------------------------------------------
inline Range::Range() { clear(); }
inline Range::Range(const std::string& strRange) { set(strRange); }
inline Range::Range(long iStart) { set(iStart,iStart); }
inline Range::Range(long iStart, long iEnd) { set(iStart, iEnd); }
inline Range::Range(const Range& rg) { set(rg); }

inline bool Range::operator==(const Range& rg) const { return (compare(rg) == 0); }
inline bool Range::operator!=(const Range& rg) const { return (compare(rg) != 0); }
inline bool Range::operator<(const Range& rg) const { return (compare(rg) < 0); }
inline bool Range::operator<=(const Range& rg) const { return (compare(rg) <= 0); }
inline bool Range::operator>(const Range& rg) const { return (compare(rg) > 0); }
inline bool Range::operator>=(const Range& rg) const { return (compare(rg) >= 0); }

inline Range& Range::operator=(const Range& rg) { set(rg); return *this; }
inline Range& Range::operator+=(long d) { move(d); return *this; }
inline Range& Range::operator-=(long d) { move(-d); return *this; }

inline void Range::set(const std::string& strRange)
{
	const char* pszEnd;
	long iStart = ::strtol(strRange.c_str(), const_cast<char**>(&pszEnd), 10);
	long iEnd = ::strtol(pszEnd, NULL, 10);
	if (iStart <= iEnd)
		set(iStart, iEnd);
	else
		set(iEnd, iStart);
}
inline void Range::set(long iStart) { set(iStart, iStart); }		
inline void Range::set(long iStart, long iEnd)
{
	DASSERT(iStart <= iEnd);
	DASSERT(iStart < std::numeric_limits<long>::max()-1);
	DASSERT(iEnd < std::numeric_limits<long>::max()-1);
	_iStart = iStart;
	_iEnd = iEnd + 1;
}
inline void Range::set(const Range& rg) { if (rg.isEmpty()) clear(); else set(rg.getStart(), rg.getEnd()); }

inline void Range::setStart(long iStart)
{
	DASSERT(!isEmpty());
	DASSERT(iStart < std::numeric_limits<long>::max()-1);
	DASSERT(iStart < _iEnd);
	_iStart = iStart;
}
inline long Range::getStart() const { DASSERT(!isEmpty()); return _iStart; }

inline void Range::setEnd(long iEnd)
{
	DASSERT(!isEmpty());
	DASSERT(iEnd < std::numeric_limits<long>::max()-1);
	DASSERT(iEnd >= _iStart);
	_iEnd = iEnd + 1;
}
inline long Range::getEnd() const { DASSERT(!isEmpty()); return _iEnd-1; }

inline bool Range::isEmpty() const
{
	DASSERT(_iStart != _iEnd || _iStart == std::numeric_limits<long>::max());
	return (_iStart == _iEnd);
}

inline size_t Range::getLength() const
{
	DASSERT(_iStart <= _iEnd);
	return static_cast<size_t>(_iEnd - _iStart);
}

inline void Range::clear()
{
	_iStart = std::numeric_limits<long>::max();
	_iEnd = std::numeric_limits<long>::max();
}

inline void Range::extend(long d)
{
	DASSERT(!isEmpty());
	DASSERT(d <= 0 || _iEnd <= (std::numeric_limits<long>::max()-1-d));
	DASSERT(_iStart < (_iEnd+d));
	_iEnd += d;
}

inline void Range::move(long d)
{
	DASSERT(!isEmpty());
	DASSERT(d <= 0 || _iEnd <= (std::numeric_limits<long>::max()-1-d));
	_iStart += d;
	_iEnd += d;
}

inline bool Range::contains(long i) const
{
	return (isEmpty()
			? false
			: (i >= _iStart && i < _iEnd));
}

inline bool Range::contains(const Range& rg) const
{
	return contains(rg.getStart()) && contains(rg.getEnd());
}

inline bool Range::intersects(const Range& rg) const
{
	return (	!isEmpty()
		   &&	(	(	rg._iStart >= _iStart
					&&	rg._iStart < _iEnd-1)
				||	(	rg._iEnd >= _iStart
					&&	rg._iEnd < _iEnd)));
}

inline void Range::merge(long i)
{
	DASSERT(!isEmpty());
	DASSERT(i != std::numeric_limits<long>::max());
	DASSERT((i+1) != std::numeric_limits<long>::max());
	if (i < _iStart) _iStart = i;
	if (i >= _iEnd) _iEnd = i + 1;
}

inline void Range::merge(const Range& rg)
{
	DASSERT(!isEmpty());
	if (rg._iStart < _iStart) _iStart = rg._iStart;
	if (rg._iEnd > _iEnd) _iEnd = rg._iEnd;
}

inline int Range::compare(const Range& rg) const
{
	return (isEmpty() && rg.isEmpty()
			? 0
			: (isEmpty() && !rg.isEmpty()
			   ? -1
			   : (!isEmpty() && rg.isEmpty()
				  ? 1
				  : (_iStart < rg._iStart
					 ? -1
					 : (_iStart > rg._iStart
						? 1
						: (_iEnd < rg._iEnd
						   ? -1
						   : (_iEnd > rg._iEnd
							  ? 1
							  : 0)))))));
}

//--------------------------------------------------------------------------------
//
// Unit
//
//--------------------------------------------------------------------------------
inline UNIT Unit::getUndefined() { return numeric_limits<UNIT>::infinity(); }
inline bool Unit::isDefined(UNIT n)
{
	return	(n >= 0 && numeric_limits<UNIT>::infinity() != n)
		||	(n < 0 && -numeric_limits<UNIT>::infinity() != n);
}

inline void Unit::beginImpreciseMode()
{
    s_nNegativeZero = -Constants::s_nERROR_MARGIN;
    s_nPositiveZero = Constants::s_nERROR_MARGIN;
}

inline void Unit::endImpreciseMode()
{
    s_nNegativeZero = s_nPositiveZero = 0;
}

inline Unit::Unit() { _n = 0; }
inline Unit::Unit(UNIT n) { _n = n; }
inline Unit::Unit(const std::string& str) { _n = strToUNIT(str); }

inline Unit::operator UNIT() const { return _n; }

inline Unit& Unit::operator=(UNIT n) { _n = n; return *this; }
inline Unit& Unit::operator=(const std::string& str) { _n = strToUNIT(str); return *this; }

inline Unit Unit::operator-() const { return Unit(-1*_n); } 

inline Unit& Unit::operator++() { ++_n; return *this; }
inline Unit& Unit::operator--() { --_n; return *this; }

inline bool Unit::isAboveZero() const { return (_n > s_nPositiveZero); }
inline bool Unit::isBelowZero() const { return (_n < s_nNegativeZero); }

inline bool Unit::operator==(UNIT n) const
{
	// Equality iff equal within the error margin
	if (std::abs(static_cast<UNIT>(_n) - static_cast<UNIT>(n)) < s_nPositiveZero)
		return true;

	return false;
}

inline bool Unit::operator!=(UNIT n) const { return !operator==(n); }

inline bool Unit::operator<(UNIT n) const
{
	// Less than iff less than value plus the error margin
	if (static_cast<UNIT>(_n) + s_nPositiveZero < static_cast<UNIT>(n))
		return true;

	return false;
}

inline bool Unit::operator<=(UNIT n) const
{
	// Less or equal than iff less than value minus the error margin
	if (static_cast<UNIT>(_n) - s_nPositiveZero < static_cast<UNIT>(n))
		return true;

	return false;
}

inline bool Unit::operator>(UNIT n) const { return !operator<=(n); }
inline bool Unit::operator>=(UNIT n) const { return !operator<(n); }

inline Unit& Unit::operator+=(UNIT n) { _n += n; return *this; }
inline Unit& Unit::operator-=(UNIT n) { _n -= n; return *this; }
inline Unit& Unit::operator*=(UNIT n) { _n *= n; return *this; }
inline Unit& Unit::operator/=(UNIT n) { _n /= n; return *this; }

inline void Unit::setToMin() { _n = -numeric_limits<UNIT>::max(); }
inline void Unit::setToMax() { _n = numeric_limits<UNIT>::max(); }

inline bool Unit::isMin() const { return (_n == -numeric_limits<UNIT>::max()); }
inline bool Unit::isMax() const { return (_n == numeric_limits<UNIT>::max()); };

inline bool Unit::isDefined() const { return isDefined(_n); }
inline void Unit::setUndefined() { _n = getUndefined(); } 

inline UNIT Unit::strToUNIT(const std::string& str)
{
	DASSERT(!EMPTYSTR(str));
	double dValue = ::atof(str.c_str());
	if (errno == ERANGE)
		THROWRC((RC(ERROR), "Illegal conversion to Unit value from %s", str.c_str()));
	return static_cast<UNIT>(dValue);
}

//--------------------------------------------------------------------------------
//
// Point
//
//--------------------------------------------------------------------------------
inline Point::Point() {}
inline Point::Point(UNIT x, UNIT y) { set(x,y); }
inline Point::Point(const Point& pt) { DASSERT(pt.isDefined()); set(pt); }

inline bool Point::operator==(const Point& pt) const { return (compare(pt) == 0); }
inline bool Point::operator!=(const Point& pt) const { return (compare(pt) != 0); }
inline bool Point::operator<(const Point& pt) const { return (compare(pt) < 0); }
inline bool Point::operator<=(const Point& pt) const { return (compare(pt) <= 0); }
inline bool Point::operator>(const Point& pt) const { return (compare(pt) > 0); }
inline bool Point::operator>=(const Point& pt) const { return (compare(pt) >= 0); }

inline Point& Point::operator=(const Point& pt) { set(pt._x,pt._y); return *this; }

inline Point& Point::operator+=(UNIT n) { move(n,n); return *this; }
inline Point& Point::operator+=(const Point& pt) { move(pt); return *this; }

inline Point& Point::operator-=(UNIT n) { move(-n,-n); return *this; }
inline Point& Point::operator-=(const Point& pt) { move(-pt._x, -pt._y); return *this; }

inline Point& Point::operator*=(UNIT n) { scale(n,n); return *this; }
inline Point& Point::operator*=(const Point& pt) { scale(pt._x,pt._y); return *this; }
		
inline Point& Point::operator/=(UNIT n) { scale(1/n,1/n); return *this; }
inline Point& Point::operator/=(const Point& pt) { scale(1/static_cast<UNIT>(pt._x),1/static_cast<UNIT>(pt._y)); return *this; }

inline void Point::clear() { _x.setUndefined(); _y.setUndefined(); }

inline void Point::set(UNIT x, UNIT y) { _x = x; _y = y; }
inline void Point::set(const Point& pt) { _x = pt._x; _y = pt._y; }

inline void Point::move(UNIT dx, UNIT dy) { DASSERT(isDefined()); _x += dx; _y += dy; }
inline void Point::move(const Point& pt) { move(pt._x, pt._y); }
		
inline Unit& Point::x() { DASSERT(isDefined()); return _x; }
inline const Unit& Point::x() const { DASSERT(isDefined()); return _x; }

inline Unit& Point::y() { DASSERT(isDefined()); return _y; }
inline const Unit& Point::y() const { DASSERT(isDefined()); return _y; }

inline void Point::scale(UNIT sx, UNIT sy) { DASSERT(isDefined()); _x *= sx; _y *= sy; }
inline void Point::scale(const Point& pt) { DASSERT(isDefined()); DASSERT(pt.isDefined()); _x *= pt._x; _y *= pt._y; }

inline UNIT Point::squareDeviationFrom(const Point& pt) const
{
	Unit dx = _x - pt._x;
	Unit dy = _y - pt._y;
	return ((dx*dx) + (dy*dy));
}

inline bool Point::isDefined() const { return _x.isDefined() && _y.isDefined(); }

inline int Point::compare(const Point& pt) const
{
	return (!isDefined() && !pt.isDefined()
			? 0
			: (!isDefined() && pt.isDefined()
			   ? -1
			   : (isDefined() && !pt.isDefined()
				  ? 1
				  : (_x < pt._x
					 ? -1
					 : (_x > pt._x
						? 1
						: (_y < pt._y
						   ? -1
						   : (_y > pt._y
							  ? 1
							  : 0)))))));
}

inline Point stylus::operator-(const Point& pt1, const Point& pt2) { return Point(pt1._x - pt2._x, pt1._y - pt2._y); }
inline Point stylus::operator+(const Point& pt1, const Point& pt2) { return Point(pt1._x + pt2._x, pt1._y + pt2._y); }
inline Point stylus::operator*(const Point& pt1, const Point& pt2) { return Point(pt1._x * pt2._x, pt1._y * pt2._y); }
inline Point stylus::operator/(const Point& pt1, const Point& pt2) { return Point(pt1._x / pt2._x, pt1._y / pt2._y); }

inline Point stylus::getPointBetween(const Point& ptStart, const Point& ptEnd, UNIT dFractionalDistance)
{
	return Point(ptStart.x() + ((ptEnd.x() - ptStart.x()) * dFractionalDistance),
				ptStart.y() + ((ptEnd.y() - ptStart.y()) * dFractionalDistance));
}

//--------------------------------------------------------------------------------
//
// PointDistance
//
//--------------------------------------------------------------------------------
inline PointDistance::PointDistance() {}
inline PointDistance::PointDistance(UNIT x, UNIT y, UNIT d) { set(x,y,d); }
inline PointDistance::PointDistance(const Point& pt) { *this = pt; }
inline PointDistance::PointDistance(const Point& pt, UNIT d) { *this = pt; _nDistance = d; }
inline PointDistance::PointDistance(const PointDistance& ptd) { *this = ptd; }
inline PointDistance::~PointDistance() {}

inline PointDistance& PointDistance::operator=(const PointDistance& ptd) { set(ptd); return *this; }
inline PointDistance& PointDistance::operator=(const Point& pt) { set(pt); return *this; }

inline void PointDistance::set(UNIT x, UNIT y, UNIT d) { _x = x; _y = y; _nDistance = d; }
inline void PointDistance::set(const PointDistance& ptd)
{
	Point::set(ptd);
	_nDistance = ptd._nDistance;
}
inline void PointDistance::set(const Point& pt, UNIT d)
{
	Point::set(pt);
	_nDistance = d;
}

inline void PointDistance::setDistance(UNIT d) { _nDistance = d; }
inline UNIT PointDistance::getDistance() const { return _nDistance; }

inline PointDistance stylus::getPointBetween(const PointDistance& ptStart, const PointDistance& ptEnd, UNIT dFractionalDistance)
{
	return PointDistance(getPointBetween(dynamic_cast<const Point&>(ptStart), dynamic_cast<const Point&>(ptEnd), dFractionalDistance),
						ptStart.getDistance() + ((ptEnd.getDistance() - ptStart.getDistance()) * dFractionalDistance));
}

//--------------------------------------------------------------------------------
//
// Line
//
//--------------------------------------------------------------------------------
inline Line::Line(const Point& ptStart, const Point& ptEnd, size_t id, size_t idOwner, bool fCanonical)
{
	_id = id;
	_idOwner = idOwner;
	set(ptStart,ptEnd,fCanonical);
}
inline Line::Line(const Line& ln) { _id = ln._id; _idOwner = ln._idOwner; set(ln); }

inline bool Line::operator==(const Line& ln) const { return (compare(ln) == 0); }
inline bool Line::operator!=(const Line& ln) const { return (compare(ln) != 0); }
inline bool Line::operator<(const Line& ln) const { return (compare(ln) < 0); }
inline bool Line::operator<=(const Line& ln) const { return (compare(ln) <= 0); }
inline bool Line::operator>(const Line& ln) const { return (compare(ln) > 0); }
inline bool Line::operator>=(const Line& ln) const { return (compare(ln) >= 0); }

inline Line& Line::operator=(const Line& ln) { _idOwner = ln._idOwner; set(ln); return *this; }

inline Line& Line::operator+=(const Point& pt) { move(pt); return *this; }
inline Line& Line::operator+=(const Unit& n) { move(n); return *this; }

inline Line& Line::operator-=(const Point& pt) { move(Point(-pt.x(),-pt.y())); return *this; }
inline Line& Line::operator-=(const Unit& n) { move(-n); return *this; }

inline void Line::makeCanonical() { set(_ptStart, _ptEnd, true); }
inline bool Line::isCanonical() const { return _fCanonical; }

inline void Line::set(const Line& ln)
{
	DASSERT(_ptStart.isDefined());
	DASSERT(_ptEnd.isDefined());

	_fCanonical = ln._fCanonical;
	_ptStart = ln._ptStart;
	_ptEnd = ln._ptEnd;
	_dx = ln._dx;
	_dy = ln._dy;
	_nSlope = ln._nSlope;
	_yIntercept = ln._yIntercept;
	_nLength = ln._nLength;
}

inline void Line::move(const Unit& co) { move(Point(co,co)); }

inline void Line::setStart(const Point& ptStart) { set(ptStart,_ptEnd); }
inline const Point& Line::getStart() const { DASSERT(_ptStart.isDefined()); return _ptStart; }

inline void Line::setEnd(const Point& ptEnd) { set(_ptStart,ptEnd); }
inline const Point& Line::getEnd() const { DASSERT(_ptEnd.isDefined()); return _ptEnd; }

inline size_t Line::getID() const { return _id; }
inline size_t Line::getOwnerID() const { return _idOwner; }

inline const Unit& Line::dx() const { return _dx; }
inline const Unit& Line::dy() const { return _dy; }
inline const Unit& Line::getSlope() const { return _nSlope; }
inline const Unit& Line::getYIntercept() const { return _yIntercept; }
inline const Unit& Line::getLength() const { return _nLength; }

inline UNIT Line::yAt(UNIT x) const
{
	DASSERT(_ptStart.isDefined());
	DASSERT(_ptEnd.isDefined());
	return _ptStart.x() == x
		? static_cast<UNIT>(_ptStart.y())
		: (_ptEnd.x() == x 
		   ? static_cast<UNIT>(_ptEnd.y())
		   : static_cast<UNIT>((_nSlope * x) + _yIntercept));
}

inline int Line::compareAt(const Line& ln, UNIT x) const
{
	DASSERT(_ptStart.isDefined());
	DASSERT(_ptEnd.isDefined());
	DASSERT(ln._ptStart.isDefined());
	DASSERT(ln._ptEnd.isDefined());

	// Return equal if the lines have the same ID
	if (_id == ln._id)
		return 0;

	// Otherwise, compare the y-values of the lines at the passed x-coordinate
	Unit yThis = yAt(x);
	Unit yThat = ln.yAt(x);

	return (yThis > yThat
			? 1
			: (yThis < yThat
			   ? -1
			   : 0));
}

/*
 * This routines checks for intersection at or beyond the passed x-coordinate
 *
 * NOTES:
 * - This routine is defined only for canonical lines
 * - Floating-point calculations are not guaranteed to produce equivalent values unless the
 *   calculation order is also equivalent. Because vector computed values take precedence
 *   over those computed by other means, this routine will force an intersection point
 *   occurring at the start or end point of one of or both lines to be exactly equal (binary
 *   equality) to the that point.
 */
inline bool Line::intersectsAt(const Line& ln, UNIT x, Point& ptIntersection) const
{
	bool fIntersects = false;
	Unit xIntersect;
	Unit yIntersect;

	DASSERT(!Unit(x).isMin());
	DASSERT(!Unit(x).isMax());

	DASSERT(_ptStart.isDefined());
	DASSERT(_ptEnd.isDefined());
	DASSERT(ln._ptStart.isDefined());
	DASSERT(ln._ptEnd.isDefined());

	DASSERT(_fCanonical);
	DASSERT(	(_ptStart.x() < _ptEnd.x())
		  ||	(	_ptStart.x() == _ptEnd.x()
				&&	_ptStart.y() >= _ptEnd.y()));

	DASSERT(ln._fCanonical);
	DASSERT(	(ln._ptStart.x() < ln._ptEnd.x())
		  ||	(	ln._ptStart.x() == ln._ptEnd.x()
				&&	ln._ptStart.y() >= ln._ptEnd.y()));
				
	// Calculate the intersection point, if any, based on the types of slopes involved
	// Note:
	// - The predicate order below is important as latter cases presuppose the earlier
	//   cases have been handled

	// Both lines have infinite slope
	// - The lines intersect if they have a common y-value
	// - The intersection is defined as the first point they share (y-descending)
	if (	!_nSlope.isDefined()
		&&	!ln._nSlope.isDefined())
	{
		if (	_ptStart.x() == ln._ptStart.x()
			&&	_ptStart.x() >= x)
		{
			if (	_ptStart.y() >= ln._ptStart.y()
				&&	_ptEnd.y() <= ln._ptStart.y())
			{
				xIntersect = ln._ptStart.x();
				yIntersect = ln._ptStart.y();
				fIntersects = true;
			}
			else if (	ln._ptStart.y() >= _ptStart.y()
					&&	ln._ptEnd.y() <= _ptStart.y())
			{
				xIntersect = _ptStart.x();
				yIntersect = _ptStart.y();
				fIntersects = true;
			}
		}
	}

	// Both lines have the same slope
	// - The lines intersect if they have at least one common point
	// - The intersection is defined as the first point they share (x-ascending)
	else if (	_nSlope.isDefined()
			&&	ln._nSlope.isDefined()
			&&	_nSlope == ln._nSlope)
	{
		if (_yIntercept == ln._yIntercept)
		{
			if (	_ptStart.x() <= ln._ptStart.x()
				&&	_ptEnd.x() >= ln._ptStart.x()
				&&	ln._ptStart.x() >= x)
			{
				xIntersect = ln._ptStart.x();
				yIntersect = ln._ptStart.y();
				fIntersects = true;
			}
			else if (	ln._ptStart.x() <= _ptStart.x()
					&&	ln._ptEnd.x() >= _ptStart.x()
					&&	_ptStart.x() >= x)
			{
				xIntersect = _ptStart.x();
				yIntersect = _ptStart.y();
				fIntersects = true;
			}
		}
	}

	// One of the lines has infinite slope
	// - The lines intersect if the sloped line crosses the vertical line
	else if (	!_nSlope.isDefined()
			||	!ln._nSlope.isDefined())
	{
		DASSERT(_nSlope.isDefined() || ln._nSlope.isDefined());
		
		const Line& lnWithSlope = (_nSlope.isDefined() ? *this : ln);
		const Line& lnWithoutSlope = (_nSlope.isDefined() ? ln : *this);

		if (	lnWithSlope._ptStart.x() <= x
			&&	lnWithSlope._ptEnd.x() >= x
			&&	lnWithoutSlope._ptStart.x() == x)
		{
			if (lnWithSlope._ptStart.x() == x)
			{
				xIntersect = lnWithSlope._ptStart.x();
				yIntersect = lnWithSlope._ptStart.y();
			}
			else if (lnWithSlope._ptEnd.x() == x)
			{
				xIntersect = lnWithSlope._ptEnd.x();
				yIntersect = lnWithSlope._ptEnd.y();
			}
			else
			{
				xIntersect = lnWithoutSlope._ptStart.x();
				yIntersect = (lnWithSlope._nSlope * xIntersect) + lnWithSlope._yIntercept;
				if (yIntersect == lnWithoutSlope._ptStart.y())
					yIntersect = lnWithoutSlope._ptStart.y();
				else if (yIntersect == lnWithoutSlope._ptEnd.y())
					yIntersect = lnWithoutSlope._ptEnd.y();
			}
			fIntersects = (lnWithoutSlope._ptStart.y() >= yIntersect && yIntersect >= lnWithoutSlope._ptEnd.y());
		}
	}

	// Otherwise, the lines intersect if they share an (x,y) point
	else
	{
		DASSERT(_nSlope.isDefined() && ln._nSlope.isDefined());
		DASSERT(_nSlope != ln._nSlope);

		xIntersect = (ln._yIntercept - _yIntercept) / (_nSlope - ln._nSlope);
		if (xIntersect >= x)
		{
			if (xIntersect == _ptStart.x())
			{
				xIntersect = _ptStart.x();
				yIntersect = _ptStart.y();
			}
			else if (xIntersect == _ptEnd.x())
			{
				xIntersect = _ptEnd.x();
				yIntersect = _ptEnd.y();
			}
			else if (xIntersect == ln._ptStart.x())
			{
				xIntersect = ln._ptStart.x();
				yIntersect = ln._ptStart.y();
			}
			else if (xIntersect == ln._ptEnd.x())
			{
				xIntersect = ln._ptEnd.x();
				yIntersect = ln._ptEnd.y();
			}
			else
				yIntersect = (_nSlope * xIntersect) + _yIntercept;

			fIntersects = (	_ptStart.x() <= xIntersect
						&&	_ptEnd.x() >= xIntersect
						&&	ln._ptStart.x() <= xIntersect
						&&	ln._ptEnd.x() >= xIntersect);
		}
	}

	if (fIntersects)
		ptIntersection.set(xIntersect, yIntersect);
	return fIntersects;
}

inline int Line::compare(const Line& ln) const
{
	if (_id == ln._id)
		return 0;
	else
		return (_id < ln._id
				? -1
				: (_id > ln._id
				   ? 1
				   : (_ptStart < ln._ptStart
					  ? -1
					  : (_ptStart > ln._ptStart
						 ? 1
						 : (_ptEnd < ln._ptEnd
							? -1
							: (_ptEnd > ln._ptEnd
							   ? 1
							   : 0))))));
				  
}

//--------------------------------------------------------------------------------
//
// Rectangle
//
//--------------------------------------------------------------------------------
inline Rectangle::Rectangle() {}
inline Rectangle::Rectangle(UNIT yTop, UNIT xLeft, UNIT yBottom, UNIT xRight) { set(yTop, xLeft, yBottom, xRight); }
inline Rectangle::Rectangle(Point& ptTopLeft, Point& ptBottomRight) { set(ptTopLeft, ptBottomRight); }
inline Rectangle::Rectangle(XMLDocument* pxd, xmlNodePtr pxnBounds) { load(pxd, pxnBounds); }
inline Rectangle::Rectangle(const Rectangle& r) { set(r); }

inline bool Rectangle::operator==(const Rectangle& r) const
{
	return (	!_ptTopLeft.isDefined()
		   ||	!_ptBottomRight.isDefined()
		   ||	!r._ptTopLeft.isDefined()
		   ||	!r._ptBottomRight.isDefined()
				? false
				: _ptTopLeft == r._ptTopLeft && _ptBottomRight == r._ptBottomRight);
}
inline bool Rectangle::operator!=(const Rectangle& r) const { return !operator==(r); }

inline Rectangle& Rectangle::operator=(const Rectangle& r) { set(r); return *this; }

inline Rectangle& Rectangle::operator+=(UNIT n) { move(n,n); return *this; }
inline Rectangle& Rectangle::operator+=(const Point& pt) { DASSERT(pt.isDefined()); move(pt.x(),pt.y()); return *this; }

inline Rectangle& Rectangle::operator-=(UNIT n) { move(-n,-n); return *this; }
inline Rectangle& Rectangle::operator-=(const Point& pt) { DASSERT(pt.isDefined()); move(-pt.x(),-pt.y()); return *this; }

inline Rectangle& Rectangle::operator*=(UNIT n) { scale(n,n); return *this; }
inline Rectangle& Rectangle::operator*=(const Point& pt) { scale(pt.x(), pt.y()); return *this; }

inline Rectangle& Rectangle::operator/=(UNIT n) { scale(1/n,1/n); return *this; }
inline Rectangle& Rectangle::operator/=(const Point& pt)
{
	DASSERT(pt.isDefined());
	scale(1/static_cast<UNIT>(pt.x()), 1/static_cast<UNIT>(pt.y()));
	return *this;
}

inline void Rectangle::clear() { set(0, 0, 0, 0); }

inline void Rectangle::set(const Point& ptTopLeft, const Point& ptBottomRight) { set(ptTopLeft.y(), ptTopLeft.x(), ptBottomRight.y(), ptBottomRight.x()); }
inline void Rectangle::set(const Point& pt) { set(pt.y(), pt.x(), pt.y(), pt.x()); }

inline const Point& Rectangle::getTopLeft() const { return _ptTopLeft; }
inline const Point& Rectangle::getBottomRight() const { return _ptBottomRight; }
inline const Point& Rectangle::getCenter() const { return _ptCenter; }

inline UNIT Rectangle::getWidth() const { return ::max<UNIT>(_dxWidth, static_cast<UNIT>(0)); }
inline UNIT Rectangle::getHeight() const { return ::max<UNIT>(_dyHeight, static_cast<UNIT>(0)); }

inline bool Rectangle::isEmpty() const { return (_dxWidth.isBelowZero() || _dyHeight.isBelowZero()); }

inline bool Rectangle::contains(const Point& pt) const
{
	return (	pt.x() >= _ptTopLeft.x()
			&&	pt.x() <= _ptBottomRight.x()
			&&	pt.y() <= _ptTopLeft.y()
			&&	pt.y() >= _ptBottomRight.y());
}

inline Rectangle Rectangle::intersect(const Rectangle& r) const
{
	Rectangle rIntersect;
	rIntersect._ptTopLeft.set(::max<UNIT>(_ptTopLeft.x(), r._ptTopLeft.x()), ::min<UNIT>(_ptTopLeft.y(), r._ptTopLeft.y()));
	rIntersect._ptBottomRight.set(::min<UNIT>(_ptBottomRight.x(), r._ptBottomRight.x()), ::max<UNIT>(_ptBottomRight.y(), r._ptBottomRight.y()));
	rIntersect.ensureDimensions();
	return rIntersect;
}

inline void Rectangle::expand(UNIT dyTop, UNIT dxLeft, UNIT dyBottom, UNIT dxRight)
{
	_ptTopLeft.move(dxLeft, dyTop);
	_ptBottomRight.move(dxRight, dyBottom);
	ensureDimensions();
}

inline void Rectangle::combine(const Point& ptTopLeft, const Point& ptBottomRight)
{
	DASSERT(ptTopLeft.isDefined());
	DASSERT(ptBottomRight.isDefined());
	combine(ptTopLeft.y(),
		   ptTopLeft.x(),
		   ptBottomRight.y(),
		   ptBottomRight.x());
}
inline void Rectangle::combine(const Point& pt) { combine(pt, pt); }
inline void Rectangle::combine(const Rectangle& r) { combine(r._ptTopLeft, r._ptBottomRight); }

inline void Rectangle::move(UNIT n) { move(n,n); }
inline void Rectangle::move(const Point& pt) { DASSERT(pt.isDefined()); move(pt.x(), pt.y()); }

inline void Rectangle::scale(UNIT n) { scale(n,n); }
inline void Rectangle::scale(const Point& pt) { DASSERT(pt.isDefined()); scale(pt.x(),pt.y()); }
