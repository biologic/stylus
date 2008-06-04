/*******************************************************************************
 * \file	overlap.hpp
 * \brief	Stylus Overlaps and helper classes
 *
 * Overlaps encapsulates stroke overlap detection.
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

#ifndef OVERLAP_HPP
#define OVERLAP_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief LineEvent types
	 *
	 * \remarks
	 * - The event order is \em critical; LineEvent::compare relies on the events
	 *   having the following ascending order
	 * - SWAP events move lines down toward smaller y-values
	 */
	enum LINEEVENTTYPE
	{
		LET_ENTER = 1,							///< A line is entering scope
		LET_SWAP,								///< A line is moving down in y-order
		LET_EXIT								///< A line is exiting scope
	};

	/**
	 * \brief LineEvent class
	 *
	 * LineEvents describe significant state changes during overlap detection.
	 * These events modify the state of the LineStack (see below) used to determine
	 * which lines may intersect.
	 */
	class LineEvent
	{
	public:
		LineEvent(LINEEVENTTYPE let, const Point& pt, const Line& ln);
		LineEvent(const LineEvent& le);

		LineEvent& operator=(const LineEvent& le);

		bool operator==(const LineEvent& le) const;
		bool operator<(const LineEvent& le) const;
		bool operator<=(const LineEvent& le) const;
		bool operator>(const LineEvent& le) const;
		bool operator>=(const LineEvent& le) const;

		LINEEVENTTYPE getType() const;
		const Point& getPoint() const;
		const Line& getLine() const;

		std::string toString() const;

	private:
		LINEEVENTTYPE _let;						///< LineEvent type (see LINEEVENTTYPE)
		Point _pt;								///< Point at which the event occurs
		const Line& _ln;						///< Reference to affected Line

		int compare(const LineEvent& le) const;	///< Return -1, 0, or 1 for less-than, equal-to, or greater-than
	};

	/**
	 * \brief EventStack class
	 *
	 * The EventStack is used by the overlap detection code to hold and manage
	 * all events line events.
	 */
	class EventStack
	{
	public:
		EventStack();

		void pushLines(const std::vector<Line>& vecLines);
		void push(const LineEvent& le);
		const LineEvent pop();

		bool isEmpty() const;

#ifdef ST_TRACE
		void traceStack() const;
		bool validate() const;
		bool validateSwap(const LineEvent& le) const;
#endif

	private:
		std::list<LineEvent> _llEvents;
		std::list<LineEvent>::iterator _it;
	};

	/**
	 * \brief LineStack class
	 *
	 * A LineStack is a 'stack' of lines ordered by their y-coordinate (at a given
	 * x-coordinate). It is a helper structure, much like LineEvent above, used
	 * during overlap detection.
	 *
	 * \remarks
	 * - The stack is kept in descending order of the y-coordinate value for a
	 *   given x-coordinate
	 */
	class LineStack
	{
	public:
		LineStack();

		/**
		 * \brief Make the passed line the active line of the stack
		 *
		 */
		void makeActive(const Line& ln);

		/**
		 * \brief LINEEVENTTYPE handlers
		 *
		 */
		//{@
		void handleEnter(const Line& ln);
		void handleSwap();
		void handleExit();
		//@}

		/**
		 * \brief Obtain neighboring lines
		 *
		 * \remarks
		 * - These routines will return NULL when no neighbor exists
		 */
		//{@
		const Line* getAbove();
		const Line* getBelow();
		//@}

#ifdef ST_TRACE
		void traceStack() const;
#endif

	private:
		std::list<const Line*> _ll;				///< Stack of lines
		std::list<const Line*>::iterator _it;	///< Current iterator
	};

	/**
	 * \brief Overlap template
	 *
	 */
	template<class Type>
	class Overlap
	{
	public:
		Overlap(const Type t1, const Type t2, const Point& pt);

		bool operator==(const Overlap<Type>& ol) const;
		bool operator<(const Overlap<Type>& ol) const;
		bool operator>(const Overlap<Type>& ol) const;

		const Type getFirst() const;
		const Type getSecond() const;
		const Point& getPoint() const;

	private:
		const Type _t1;							///< First overlapping item
		const Type _t2;							///< Second overlapping item
		Point _pt;								///< Location of the overlap

		int compare(const Overlap<Type>& ol) const;
	};

	/**
	 * \brief Overlaps class
	 *
	 * This object detects and exposes overlaps in the set of passed strokes.
	 */
	class Overlaps
	{
	public:
		Overlaps(const std::vector<ACIDTYPE>& vecAcids,
				 const std::vector<Point>& vecPoints,
				 const std::vector<Stroke>& vecStrokes);

		const STROKEOVERLAPS& getOverlaps();

	private:
		const std::vector<ACIDTYPE>& _vecAcids;	///< Acids underlying the strokes
		const std::vector<Point>& _vecPoints;	///< Points associated with each acid
		const std::vector<Stroke>& _vecStrokes;	///< Strokes to analyze

		std::vector<Line> _vecLines;			///< Array of lines from all strokes
		LINEOVERLAPS _setLineOverlaps;			///< Set of overlapping lines
		STROKEOVERLAPS _setOverlaps;			///< Set of overlapping strokes (as indexes into Gene stroke array)

		EventStack _events;						///< Active line event stack

		void buildStrokeLines();
		void checkIntersection(const Line& lnAbove, const Line& lnBelow, const Point& pt);
		void ensureOverlaps();

#ifdef ST_TRACE
		void traceStrokeLines() const;
		void traceLineOverlaps() const;
#endif
	};

} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // OVERLAP_HPP
