/*******************************************************************************
 * \file	helpers.hpp
 * \brief	Stylus helper classes
 *
 * NOTE:
 * - None of the helper classes trace their flow in or out
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

#ifndef HELPERS_HPP
#define HELPERS_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Range class
	 * \remarks
	 * - Externally, the range includes the start and the end (i.e., [s,e] )
	 * - The largest possible external range is [0, std::numeric_limits<long>::max()-2]
	 * - Internally, the range includes the start and excludes the end (i.e., [s,e) )
	 * - The largest possible internal range is [0, std::numeric_limits<long>::max()-1)
	 * - A range of [x,x) is considered empty; std::numeric_limits<long>::max() is reserved for empty ranges
	 * - Empty ranges cannot be equal or intersect, they do have a length of zero
	 */
	class Range
	{
	public:
		Range();
		Range(const std::string& strRange);
		Range(long iStart);
		Range(long iStart, long iEnd);
		Range(const Range& rg);

		bool operator==(const Range& rg) const;
		bool operator!=(const Range& rg) const;
		bool operator<(const Range& rg) const;
		bool operator<=(const Range& rg) const;
		bool operator>(const Range& rg) const;
		bool operator>=(const Range& rg) const;

		Range& operator=(const Range& rg);
		Range& operator+=(long d);
		Range& operator-=(long d);

		void set(const std::string& strRange);
		void set(long iStart);
		void set(long iStart, long iEnd);
		void set(const Range& rg);

		void setStart(long iStart);
		long getStart() const;

		void setEnd(long iEnd);
		long getEnd() const;

		bool isEmpty() const;

		size_t getLength() const;

		void clear();

		void extend(long d);
		void move(long d);

		bool contains(long i) const;
		bool contains(const Range& rg) const;
		bool intersects(const Range& rg) const;

		void merge(long i);
		void merge(const Range& rg);

		std::string toString(bool fAsPair = true) const;

	private:
		long _iStart;			///< Start of the range (inclusive)
		long _iEnd;				///< End of the range (exclusive)

		int compare(const Range& rg) const;
	};

	/**
	 * \brief Unit is an intelligent holder of the basic Stylus unit type (UNIT)
	 * 
	 * This class handles and encapsulates all floating-point arithmetic issues,
	 * adding some ease-of-use methods. To ensure correct floating-point
	 * comparisons, it is *critical* that this class appear on the left-hand
	 * side. (Providing global operators to accept UNIT on the left-hand side,
	 * introduces too many ambiguities for the compiler.)
	 *
	 * Units are equal if they lie within a small delta of each other
	 * (s_nPositiveZero / s_nNegativeZero values below). Selecting this value is
	 * somewhat sensitive since if it is too coarse values that should not be
	 * equal (such as those calculated during overlap detection) will not be and
	 * if it is too fine values that should be the same will not be (such as
	 * endpoint comparisons).
	 *
	 * Code should declare and use instances of Unit (this class) rather than
	 * UNIT (the base type it wraps), but should pass values as UNIT. This will
	 * ensure that value passing uses a basic type (e.g., double) while
	 * obtaining the benefits of the full wrapper class.
	 */
	class Unit
	{
	public:
		static UNIT getUndefined();
		static bool isDefined(UNIT n);
		static void logConstants();
		
		Unit();
		Unit(UNIT n);
		Unit(const std::string& str);

		operator UNIT () const;

		Unit& operator=(UNIT n);
		Unit& operator=(const std::string& str);

		Unit operator-() const;

		Unit& operator++();
		Unit& operator--();

		bool operator==(UNIT n) const;
		bool operator!=(UNIT n) const;
		bool operator<(UNIT n) const;
		bool operator<=(UNIT n) const;
		bool operator>(UNIT n) const;
		bool operator>=(UNIT n) const;

		Unit& operator+=(UNIT n);
		Unit& operator-=(UNIT n);
		Unit& operator*=(UNIT n);
		Unit& operator/=(UNIT n);
		
		bool isZero() const;
		bool isBelowZero() const;
		bool isAboveZero() const;

		void setToMin();
		void setToMax();
		
		bool isMin() const;
		bool isMax() const;

		bool isDefined() const;
		void setUndefined();

        static void beginImpreciseMode();
        static void endImpreciseMode();

	private:
		static UNIT s_nNegativeZero;
		static UNIT s_nPositiveZero;

		UNIT _n;

		UNIT strToUNIT(const std::string& str);
	};

	/**
	 * \Brief Point class
	 *
	 */
	class Point
	{
		friend class Rectangle;
		friend Point operator-(const Point& pt1, const Point& pt2);
		friend Point operator+(const Point& pt1, const Point& pt2);
		friend Point operator*(const Point& pt1, const Point& pt2);
		friend Point operator/(const Point& pt1, const Point& pt2);
		
	public:
		Point();
		Point(UNIT x, UNIT y);
		Point(const Point& pt);

		bool operator==(const Point& pt) const;
		bool operator!=(const Point& pt) const;
		bool operator<(const Point& pt) const;
		bool operator<=(const Point& pt) const;
		bool operator>(const Point& pt) const;
		bool operator>=(const Point& pt) const;

		Point& operator=(const Point& pt);

		Point& operator+=(UNIT n);
		Point& operator+=(const Point& pt);

		Point& operator-=(UNIT n);
		Point& operator-=(const Point& pt);

		Point& operator*=(UNIT n);
		Point& operator*=(const Point& pt);
		
		Point& operator/=(UNIT n);
		Point& operator/=(const Point& pt);

		void clear();
		
		void set(UNIT x, UNIT y);
		void set(const Point& pt);

		void load(XMLDocument* pxd, xmlNodePtr pxnPoint);

		void move(UNIT dx, UNIT dy);
		void move(const Point& pt);
		
		Unit& x();
		const Unit& x() const;
		Unit& y();
		const Unit& y() const;

		void scale(UNIT sx, UNIT sy);
		void scale(const Point& pt);

		UNIT squareDeviationFrom(const Point& pt) const;

		bool isDefined() const;

		std::string toString() const;

	protected:
		Unit _x;							///< X-coordinate
		Unit _y;							///< Y-coordinate

		int compare(const Point& pt) const;
	};

	Point operator-(const Point& pt1, const Point& pt2);
	Point operator+(const Point& pt1, const Point& pt2);
	Point operator*(const Point& pt1, const Point& pt2);
	Point operator/(const Point& pt1, const Point& pt2);
	
	Point getPointBetween(const Point& ptStart, const Point& ptEnd, UNIT dFractionalDistance);

	/**
	 * \brief PointDistance class
	 *
	 */
	class PointDistance : public Point
	{
		friend class Point;
		
	public:
		PointDistance();
		PointDistance(UNIT x, UNIT y, UNIT d);
		PointDistance(const Point& pt);
		PointDistance(const Point& pt, UNIT d);
		PointDistance(const PointDistance& ptd);
		virtual ~PointDistance();

		PointDistance& operator=(const PointDistance& ptd);
		PointDistance& operator=(const Point& pt);
		
		void set(UNIT x, UNIT y, UNIT d);
		void set(const PointDistance& ptd);
		void set(const Point& pt, UNIT d = 0);

		void load(XMLDocument* pxd, xmlNodePtr pxnPointDistance);

		void setDistance(UNIT d);
		UNIT getDistance() const;
		
		std::string toString() const;

	private:
		Unit _nDistance;
	};
	
	PointDistance getPointBetween(const PointDistance& ptStart, const PointDistance& ptEnd, UNIT dFractionalDistance);

	/**
	 * \brief Line class
	 *
	 * \remarks
	 * - Canonical lines are ordered such that the x-coordinate of the start point
	 *   occurs before that of the end point or, if the x-coordinates are equal,
	 *   the y-coordinate of the start point is greater than that of the end point.
	 */
	class Line
	{
	public:
		Line(const Point& ptStart, const Point& ptEnd, size_t id = 0, size_t idOwner = 0, bool fCanonical = false);
		Line(const Line& ln);

		bool operator==(const Line& ln) const;
		bool operator!=(const Line& ln) const;
		bool operator<(const Line& ln) const;
		bool operator<=(const Line& ln) const;
		bool operator>(const Line& ln) const;
		bool operator>=(const Line& ln) const;

		Line& operator=(const Line& ln);

		Line& operator+=(const Point& pt);
		Line& operator+=(const Unit& co);

		Line& operator-=(const Point& pt);
		Line& operator-=(const Unit& n);

		void makeCanonical();
		bool isCanonical() const;

		void set(const Point& ptStart, const Point& ptEnd, bool fCanonical = false);
		void set(const Line& ln);

		void move(const Point& pt);
		void move(const Unit& d);

		void setStart(const Point& ptStart);
		const Point& getStart() const;

		void setEnd(const Point& ptEnd);
		const Point& getEnd() const;

		size_t getID() const;
		size_t getOwnerID() const;

		const Unit& dx() const;
		const Unit& dy() const;
		const Unit& getSlope() const;
		const Unit& getYIntercept() const;
		const Unit& getLength() const;

		UNIT yAt(UNIT x) const;

		int compareAt(const Line& ln, UNIT x) const;

		bool intersectsAt(const Line& ln, UNIT x, Point& ptIntersection) const;

		std::string toString() const;

	private:
		bool _fCanonical;					///< Enforce canonical point ordering
		size_t _id;							///< Unique ID (e.g., associated codon index)
		size_t _idOwner;					///< Owner ID (e.g., stroke)
		Point _ptStart;						///< Start point
		Point _ptEnd;						///< End point
		Unit _dx;							///< Change in x
		Unit _dy;							///< Change in y
		Unit _nSlope;						///< Slope - infinite for vertical slopes
		Unit _yIntercept;					///< Y-intercept value
		Unit _nLength;						///< Length of the line

		int compare(const Line& ln) const;
	};

	/**
	 * \brief Rectangle class
	 *
	 */
	class Rectangle
	{
	public:
		enum RECTANGLEITEMS
		{
			RI_BOUNDS = 0,
			RI_TOP,
			RI_LEFT,
			RI_BOTTOM,
			RI_RIGHT,
			RI_WIDTH,
			RI_HEIGHT,
			RI_XMIDPOINT,
			RI_YMIDPOINT,

			RI_MAX
		};
		static const char* s_aryRECTANGLEITEMS[RI_MAX];

		Rectangle();
		Rectangle(UNIT yTop, UNIT xLeft, UNIT yBottom, UNIT xRight);
		Rectangle(Point& ptTopLeft, Point& ptBottomRight);
		Rectangle(XMLDocument* pxd, xmlNodePtr pxnBounds);
		Rectangle(const Rectangle& r);

		bool operator==(const Rectangle& r) const;
		bool operator!=(const Rectangle& r) const;

		Rectangle& operator=(const Rectangle& r);

		Rectangle& operator+=(UNIT n);
		Rectangle& operator+=(const Point& pt);

		Rectangle& operator-=(UNIT n);
		Rectangle& operator-=(const Point& pt);

		Rectangle& operator*=(UNIT n);
		Rectangle& operator*=(const Point& pt);

		Rectangle& operator/=(UNIT n);
		Rectangle& operator/=(const Point& pt);

		void clear();

		void set(UNIT yTop, UNIT xLeft, UNIT yBottom, UNIT xRight);
		void set(const Point& ptTopLeft, const Point& ptBottomRight);
		void set(const Rectangle& r);
		void set(const Point& pt);

		void load(XMLDocument* pxd, xmlNodePtr pxnBounds);

		const Point& getTopLeft() const;
		const Point& getBottomRight() const;
		const Point& getCenter() const;

		UNIT getWidth() const;
		UNIT getHeight() const;
		
		bool isEmpty() const;
		
		bool contains(const Point& pt) const;
		
		Rectangle intersect(const Rectangle& r) const;
		
		void expand(UNIT dyTop, UNIT dxLeft, UNIT dyBottom, UNIT dxRight);

		void combine(UNIT yTop, UNIT xLeft, UNIT yBottom, UNIT xRight);
		void combine(const Point& ptTopLeft, const Point& ptBottomRight);
		void combine(const Point& pt);
		void combine(const Rectangle& r);

		void move(UNIT dx, UNIT dy);
		void move(UNIT d);
		void move(const Point& pt);

		void scale(UNIT sx, UNIT sy);
		void scale(UNIT s);
		void scale(const Point& pt);

		std::string toString() const;
		void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	private:
		Point _ptTopLeft;
		Point _ptBottomRight;
		Point _ptCenter;
		Unit _dxWidth;
		Unit _dyHeight;

		void ensureDimensions();
	};

	/**
	 * \brief IModification interface
	 *
	 * Objects implementing IModification describe some kind of undo-able modification,
	 * either internal or external, to the genome.
	 */
	class IModification
	{
	public:
		inline IModification() {};
		inline virtual ~IModification() {};

		virtual void undo() = 0;
		virtual size_t length() const = 0;
		virtual std::string toString() const = 0;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const = 0;
	};
	
} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // HELPERS_HPP
