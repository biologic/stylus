/*******************************************************************************
 * \file	overlap.cpp
 * \brief	Stylus Gene class (stroke overlap detection)
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
// LineEvent
//
//--------------------------------------------------------------------------------

/*
 * Function: compare
 *
 * Return -1, 0, or 1 if the first event is less then, equal to, or greater than
 * the second event.
 */
int
LineEvent::compare(const LineEvent& le) const
{
	DASSERT(_ln.isCanonical());
	DASSERT(le._ln.isCanonical());
	
	// First, sort ascending by the x-coordinate value
	// - Move left to right with ascending x-coordinate values
	if (_pt.x() < le._pt.x())
		return -1;
	if (_pt.x() > le._pt.x())
		return 1;
		
	// Then, sort ascending by the event type
	// - This becomes ENTER before SWAP before EXIT
	// - The code assumes the LINEEVENTTYPE enum reflects this order
	if (_let < le._let)
		return -1;
	if (_let > le._let)
		return 1;
		
	// Next, sort descending by the y-coordinate value
	if (_pt.y() > le._pt.y())
		return -1;
	if (_pt.y() < le._pt.y())
		return 1;
		
	// Finally, sort ascending by line IDs
	if (_ln.getID() < le._ln.getID())
		return -1;
	if (_ln.getID() > le._ln.getID())
		return 1;

	return 0;
}

/*
 * Function: toString
 *
 */
string LineEvent::toString() const
{
	ostringstream ostr;
	ostr << (_let == LET_ENTER
			 ? "ENTER("
			 : (_let == LET_SWAP
				? "SWAP("
				: "EXIT("))
		 << _ln.getID() << ") pt"
		 << _pt.toString();
	return string(ostr.str());
}

//--------------------------------------------------------------------------------
//
// EventStack
//
//--------------------------------------------------------------------------------

/*
 * Function: EventStack
 *
 * Add ENTER and EXIT events for all lines. Also, add SWAP events where two lines (vectors)
 * connect since these can never be an intersection between strokes (overlap detection
 * also skips over these and assumes the SWAP events exist).
 * 
 * Note:
 * - Line IDs are indexes into the Gene Acid/Point arrays; adjacent IDs reflect adjacent
 *   vectors, which must come from the same stroke.
 */
void
EventStack::pushLines(const vector<Line>& vecLines)
{
	_llEvents.clear();
	for (size_t iLine=0; iLine < vecLines.size(); ++iLine)
	{
		const Line& ln = vecLines[iLine];
		_llEvents.push_front(LineEvent(LET_ENTER, ln.getStart(), ln));
		_llEvents.push_front(LineEvent(LET_EXIT, ln.getEnd(), ln));
	}
	_llEvents.sort();
}

#ifdef ST_TRACE
/*
 * Function: traceStack
 *
 */
void
EventStack::traceStack() const
{
	LOGTRACE((LLTRACE, "EventStack contains %ld events", _llEvents.size()));
	for (list<LineEvent>::const_iterator it = _llEvents.begin(); it != _llEvents.end(); ++it)
	{
		LOGTRACE((LLTRACE, (*it).toString().c_str()));
	}
}

/*
 * Function: validate
 * 
 * Ensure the event stack is correctly ordered
 */
bool
EventStack::validate() const
{
	list<LineEvent>::const_iterator itFirst = _llEvents.begin();
	list<LineEvent>::const_iterator itSecond = ++itFirst;
	while (itSecond != _llEvents.end())
	{
		if ((*itFirst) > (*itSecond))
		{
			LOGTRACE((LLTRACE, "%s event precedes %s in trial %lu, attempt %lu",
								(*itFirst).toString().c_str(), (*itSecond).toString().c_str(), Genome::getTrial(), Genome::getTrialAttempts()));
			return false;
		}
		itFirst = itSecond;
		++itSecond;
	}
	
	return true;
}

/*
 * Function: validateSwap
 * 
 * Check for exit events occuring before the passed swap event
 */

bool
EventStack::validateSwap(const LineEvent& leSwap) const
{
	list<LineEvent>::const_iterator itSwap = _llEvents.begin();
	for (; itSwap != _llEvents.end() && !((*itSwap) == leSwap); ++itSwap);
	for (list<LineEvent>::const_iterator it = _llEvents.begin(); it != itSwap; ++it)
	{
		const LineEvent& le = *it;
		if (	le.getLine().getID() == leSwap.getLine().getID()
			&&	le.getType() == LET_EXIT)
		{
			LOGTRACE((LLTRACE, "%s event precedes %s in trial %lu, attempt %lu",
								le.toString().c_str(), leSwap.toString().c_str(), Genome::getTrial(), Genome::getTrialAttempts()));
			return false;
		}
	}
	return true;
}
#endif

//--------------------------------------------------------------------------------
//
// LineStack
//
//--------------------------------------------------------------------------------

#ifdef ST_TRACE
/*
 * Function: traceStack
 *
 */
void
LineStack::traceStack() const
{
	LOGTRACE((LLTRACE, "LineStack contains %ld lines", _ll.size()));
	for (list<const Line*>::iterator it = (const_cast<LineStack*>(this))->_ll.begin();
		 it != (const_cast<LineStack*>(this))->_ll.end();
		 ++it)
	{
		LOGTRACE((LLTRACE, (*it)->toString().c_str()));
	}
}
#endif

//--------------------------------------------------------------------------------
//
// Overlaps
//
//--------------------------------------------------------------------------------

/*
 * Function: buildStrokeLines
 *
 * This routine first determines what strokes might intersect by finding those
 * whose bounding rectangles intersect. Then, it builds one line for each vector
 * from those strokes whose start and/or end point is contained within one or
 * more of the intersecting rectangles. This routine includes the vectors from
 * incoherent regions within a stroke (i.e., dropouts).
 *
 * NOTES:
 * - Point values are those *after* applying the vector; the point associated
 *   with the start acid (i.e., _vecPoints[0]) is where the gene begins.
 * - Use vector indexes as the line IDs.
 * - An STL bitset is used to flag those strokes that may intersect with another;
 *   sadly, this container requires a constant size at compile time. The routine
 *   assumes (and asserts) a large value (larger than the number of strokes in
 *   any known Han glyph).
 */
void
Overlaps::buildStrokeLines()
{
	ENTER(VALIDATION,buildStrokeLines);

	ASSERT(_vecLines.empty());
	ASSERT(_vecStrokes.size() <= 100);

	// First, find all intersecting regions between all stroke pairs
	// - The actual region is expanded on all sides by the length of one long vector so
	//   that intersecting vectors may be detected via point containment (rather than 
	//   having to evaluate if the vector would overlap the rectangle in anyway)
	bitset<100> grfStrokes;
	vector<Rectangle> vecIntersects;
	for (size_t iStroke1=0; iStroke1 < (_vecStrokes.size()-1); ++iStroke1)
	{
		const Rectangle& rStroke1 = _vecStrokes[iStroke1].getBounds();

		for (size_t iStroke2=iStroke1+1; iStroke2 < _vecStrokes.size(); ++iStroke2)
		{
			Rectangle rIntersect = rStroke1.intersect(_vecStrokes[iStroke2].getBounds());
			if (!rIntersect.isEmpty())
			{
				grfStrokes.set(iStroke1);
				grfStrokes.set(iStroke2);

				TDATA(VALIDATION,L4,(LLTRACE, "Strokes %ld and %ld may intersect within %s", (iStroke1+1), (iStroke2+1), rIntersect.toString().c_str()));

				rIntersect.expand(Constants::s_nVECTOR_MEDIUM, -Constants::s_nVECTOR_MEDIUM, -Constants::s_nVECTOR_MEDIUM, Constants::s_nVECTOR_MEDIUM);
				vecIntersects.push_back(rIntersect);
			}
		}
	}

	// Then, create lines for each vector within each intersecting region
	// - A vector lies within an intersection region if one or both of its
	//   end-points lie within the region (since all intersection regions
	//   were expanded by the length of the longest possible vector, checks
	//   for point containment are sufficient)
	// - The first vector of a stroke is bounded by the point immediately
	//   before and that associated with the first vector; the check needs
	//   to begin at the acid/point just before where the stroke starts
	// Note:
	// - The inner loops could be enhanced by only evaluating stroke vectors
	//   against intersection regions involving those strokes; that information
	//   is not presently kept, so the loops evaluate vectors against all regions
	if (!vecIntersects.empty())
	{
		for (size_t iStroke=0; iStroke < _vecStrokes.size(); ++iStroke)
		{
			// Only consider vectors from strokes that may have intersected with another
			if (!grfStrokes.test(iStroke))
				continue;

			// Create lines for each vector whose start/end point are within an intersection region
			const Stroke& st = _vecStrokes[iStroke];
			long iAcid = st.getStart();
			for (size_t iIntersect=0; iIntersect < vecIntersects.size(); ++iIntersect)
			{
				if (vecIntersects[iIntersect].contains(_vecPoints[iAcid-1]))
				{
					_vecLines.push_back(Line(_vecPoints[iAcid-1], _vecPoints[iAcid], iAcid, iStroke, true));
					++iAcid;
					break;
				}
			}

			for (; iAcid <= st.getEnd(); ++iAcid)
			{
				for (size_t iIntersect=0; iIntersect < vecIntersects.size(); ++iIntersect)
				{
					if (vecIntersects[iIntersect].contains(_vecPoints[iAcid]))
					{
						_vecLines.push_back(Line(_vecPoints[iAcid-1], _vecPoints[iAcid], iAcid, iStroke, true));
						break;
					}
				}
			}
		}
	}
	
	TDATA(VALIDATION,L4,(LLTRACE, "Intersection evaluation will involve %ld vectors/lines", _vecLines.size()));
	TRACEDOIF(VALIDATION,DATA,L4,traceStrokeLines());
}

/*
 * Function: checkIntersection
 *
 * NOTE:
 * - A tempting 'optimization' is to only check for intersections between lines belonging to
 *   different strokes (since that is the end goal). However, this 'optimization' fails
 *   because lines from the *same* stroke may hide lines from other strokes in the stack
 *   of lines. Therefore, all lines must be compared, causing SWAP events to induce the
 *   necessary position changes in the stack of active lines.
 */
void
Overlaps::checkIntersection(const Line& lnAbove, const Line& lnBelow, const Point& pt)
{
	Point ptIntersection;
	if (lnAbove.intersectsAt(lnBelow, pt.x(), ptIntersection))
	{
		TDATA(VALIDATION,L5,(LLTRACE,
							 "Line %ld and %ld intersect at x/y%s",
							 lnAbove.getID(), lnBelow.getID(), ptIntersection.toString().c_str()));
		pair<LINEOVERLAPS::iterator,bool> pairResult =
			_setLineOverlaps.insert(lnAbove <= lnBelow
									? LINEOVERLAP(lnAbove, lnBelow, ptIntersection)
									: LINEOVERLAP(lnBelow, lnAbove, ptIntersection));
		if (pairResult.second)
		{
			LineEvent le(LET_SWAP, ptIntersection, lnAbove);
			_events.push(le);

			IFDEBUG(_events.validateSwap(le));
		}
	}
}

/*
 * Function: ensureOverlaps
 *
 * Overlaps between strokes are found using a left-to-right line sweep
 * algorithm. As lines enter and exit scope, pairs of lines (those above
 * and beneath) are checked for intersection. Intersecting lines also
 * induce a scope change with the lines trading places along the y-axis at
 * the point of intersection; at these scope events, pairs of lines are also
 * discovered and checked for intersection.
 *
 * The routine works by executing an event stack: Each event marks when a line
 * starts, ends, or intersects with another line. At each event, line pairs
 * (each line with its immediate neighbors along the y-axis) are compared for
 * intersections. If an intersection does occur, an event for it is added to
 * the event stack. Events are stacked by increasing x-coordinate (thus, the
 * scan sweeps left-to-right). The event stack is initialized with the list of
 * all line start/stop events. The line stack (which lists all active lines)
 * is ordered by the y-coordinate of the lines at the given x-coordinate (with
 * line events inducing changes in that order).
 */
void
Overlaps::ensureOverlaps()
{
	ENTER(VALIDATION,ensureOverlaps);

	ASSERT(_setLineOverlaps.empty());
	ASSERT(_setOverlaps.empty());

	// First, build lines from possibly intersecting strokes
	buildStrokeLines();
	if (_vecLines.empty())
		return;

	_events.pushLines(_vecLines);
	LineStack lines;

	// Process the event stack until empty
	while (!_events.isEmpty())
	{
		const LineEvent le(_events.pop());
		const Line& ln = le.getLine();
		const Line* plnAbove = NULL;
		const Line* plnBelow = NULL;
		
		IFDEBUG(_events.validate());

		TRACEDOIF(VALIDATION,DATA,L5,_events.traceStack());

		switch (le.getType())
		{
			// When a line enters scope, check for intersections with its (new) immediate neighbors
		case LET_ENTER:
			lines.handleEnter(ln);
			plnAbove = lines.getAbove();
			plnBelow = lines.getBelow();

			TFLOW(VALIDATION,L5,(LLTRACE,
								 "EVENT: %s above(%ld) below(%ld)",
								 le.toString().c_str(),
								 (plnAbove ? plnAbove->getID() : -1),
								 (plnBelow ? plnBelow->getID() : -1)));

			if (VALID(plnAbove))
				checkIntersection(*plnAbove, ln, le.getPoint());
			if (VALID(plnBelow))
				checkIntersection(ln, *plnBelow, le.getPoint());
			break;

			// When a line moves down (due to an intersection), check for an intersection between its
			// (previous) neighbors and between the line and its new neighbor below
		case LET_SWAP:
			lines.makeActive(le.getLine());
			plnAbove = lines.getAbove();
			plnBelow = lines.getBelow();

			lines.handleSwap();

			TFLOW(VALIDATION,L5,(LLTRACE,
								 "EVENT: %s above(%ld) below(%ld)",
								 le.toString().c_str(),
								 (plnAbove ? plnAbove->getID() : -1),
								 (plnBelow ? plnBelow->getID() : -1)));

			if (VALID(plnAbove) && VALID(plnBelow))
				checkIntersection(*plnAbove, *plnBelow, le.getPoint());

			plnBelow = lines.getBelow();
			if (VALID(plnBelow))
				checkIntersection(ln, *plnBelow, le.getPoint());
			break;

			// When a line leaves scope, check its (old) neighbors for a possible intersection
		case LET_EXIT:
			lines.makeActive(le.getLine());
			plnAbove = lines.getAbove();
			plnBelow = lines.getBelow();

			lines.handleExit();

			TFLOW(VALIDATION,L5,(LLTRACE,
								 "EVENT: %s above(%ld) below(%ld)",
								 le.toString().c_str(),
								 (plnAbove ? plnAbove->getID() : -1),
								 (plnBelow ? plnBelow->getID() : -1)));

			if (VALID(plnAbove) && VALID(plnBelow))
				checkIntersection(*plnAbove, *plnBelow, le.getPoint());
			break;
		}

		TRACEDOIF(VALIDATION,DATA,L5,lines.traceStack());
	}

	TRACEDOIF(VALIDATION,DATA,L4,traceLineOverlaps());

	// Reduce the found line overlaps to overlaps between different strokes
	// - Overlaps between the same stroke do not require tracking
	// - Stroke fit-deviation scoring is sufficient to score a stroke
	for (LINEOVERLAPS::iterator itOverlap = _setLineOverlaps.begin();
		 itOverlap != _setLineOverlaps.end();
		 ++itOverlap)
	{
		size_t idStroke1 = itOverlap->getFirst().getOwnerID();
		size_t idStroke2 = itOverlap->getSecond().getOwnerID();

		TFLOW(VALIDATION,L4,(LLTRACE,
							 "Mapping line overlap (%ld with %ld at %s) to strokes(%ld,%ld)",
							 itOverlap->getFirst().getID(),
							 itOverlap->getSecond().getID(),
							 itOverlap->getPoint().toString().c_str(),
							 idStroke1, idStroke2));

		if (idStroke1 != idStroke2)
		{
			if (idStroke1 > idStroke2)
				swap<size_t>(idStroke1, idStroke2);
			_setOverlaps.insert(STROKEOVERLAP(idStroke1, idStroke2, itOverlap->getPoint()));
		}
	}
}	

#ifdef ST_TRACE
/*
 * Function: traceStrokeLines
 *
 */
void
Overlaps::traceStrokeLines() const
{
	LOGTRACE((LLTRACE, "%ld stroke lines used in intersection evaluation", _vecLines.size()));
	size_t iStroke = _vecStrokes.size();
	for (size_t iLine=0; iLine < _vecLines.size(); ++iLine)
	{
		const Line& ln = _vecLines[iLine];
		if (iStroke != ln.getOwnerID())
		{
			iStroke = ln.getOwnerID();
			LOGTRACE((LLTRACE, "Lines for Stroke %ld", (iStroke+1)));
		}
		LOGTRACE((LLTRACE, "%ld - %s", iLine, ln.toString().c_str()));
	}
}

/*
 * Function: traceLineOverlaps
 *
 */
void
Overlaps::traceLineOverlaps() const
{
	LOGTRACE((LLTRACE, "%ld overlapping lines", _setLineOverlaps.size()));
	for (LINEOVERLAPS::iterator it = _setLineOverlaps.begin();
		 it != _setLineOverlaps.end();
		 ++it)
	{
		const LINEOVERLAP& ol = *it;
		const Line& ln1 = ol.getFirst();
		const Line& ln2 = ol.getSecond();
		LOGTRACE((LLTRACE,
				  "%ld overlaps %ld at %s %s (%ld,%ld)",
				  ln1.getID(), ln2.getID(),
				  ol.getPoint().toString().c_str(),
				  (ln1.getOwnerID() == ln2.getOwnerID()
				   ? "SAME STROKES"
				   : "DIFFERENT STROKES"),
				  ln1.getOwnerID(), ln2.getOwnerID()));
	}
}
#endif
