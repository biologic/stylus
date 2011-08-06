/*******************************************************************************
 * \file	overlap.inl
 * \brief	Stylus Overlaps and helper classes inline methods
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
// LineEvent
//
//--------------------------------------------------------------------------------
inline LineEvent::LineEvent(LINEEVENTTYPE let, const Point& pt, const Line& ln) : _let(let), _pt(pt), _ln(ln) {}
inline LineEvent::LineEvent(const LineEvent& le) : _let(le._let), _pt(le._pt), _ln(le._ln) {}

inline bool LineEvent::operator==(const LineEvent& le) const { return (compare(le) == 0); }
inline bool LineEvent::operator<(const LineEvent& le) const { return (compare(le) < 0); }
inline bool LineEvent::operator<=(const LineEvent& le) const { return (compare(le) <= 0); }
inline bool LineEvent::operator>(const LineEvent& le) const { return (compare(le) > 0); }
inline bool LineEvent::operator>=(const LineEvent& le) const { return (compare(le) >= 0); }

inline LINEEVENTTYPE LineEvent::getType() const { return _let; }
inline const Point& LineEvent::getPoint() const { return _pt; }
inline const Line& LineEvent::getLine() const { return _ln; }

//--------------------------------------------------------------------------------
//
// EventStack
//
//--------------------------------------------------------------------------------
inline EventStack::EventStack()
{
}
inline void EventStack::push(const LineEvent& le)
{
	list<LineEvent> llEvent;
	llEvent.push_front(le);
	_llEvents.merge(llEvent);
}
inline const LineEvent EventStack::pop()
{
	const LineEvent le(_llEvents.front());
	_llEvents.pop_front();
	return le;
}
inline bool EventStack::isEmpty() const { return _llEvents.empty(); }

//--------------------------------------------------------------------------------
//
// LineStack
//
//--------------------------------------------------------------------------------
inline LineStack::LineStack() { _it = _ll.begin(); }
inline void LineStack::makeActive(const Line& ln)
{
	for(_it = _ll.begin(); _it != _ll.end() && *_it != &ln; ++_it);
	DASSERT(_it != _ll.end());
}
inline void LineStack::handleEnter(const Line& ln)
{
	for (_it = _ll.begin();
		 _it != _ll.end() && ln.compareAt(*(*_it), ln.getStart().x()) < 0;
		 ++_it);
	_it = _ll.insert(_it, &ln);
}
inline void LineStack::handleSwap()
{
	DASSERT(_it != _ll.end());
	const Line* pln = *_it;
	_it = _ll.erase(_it);
	if (_it != _ll.end())
		++_it;
	_it = _ll.insert(_it, pln);
}
inline void LineStack::handleExit()
{
	DASSERT(_it != _ll.end());
	_it = _ll.erase(_it);
}
inline const Line* LineStack::getAbove()
{
	DASSERT(_it != _ll.end());
	const Line* pln = NULL;
	if (_it != _ll.begin())
	{
		--_it;
		pln = *_it;
		++_it;
	}
	return pln;
}
inline const Line* LineStack::getBelow()
{
	DASSERT(_it != _ll.end());
	const Line* pln;
	++_it;
	pln = (_it == _ll.end()
		   ? NULL
		   : *_it);
	--_it;
	return pln;
}

//--------------------------------------------------------------------------------
//
// Overlap
//
//--------------------------------------------------------------------------------
template<class Type> inline Overlap<Type>::Overlap(const Type t1, const Type t2, const Point& pt) :
	_t1(t1), _t2(t2), _pt(pt)
{
	DASSERT(_t1 <= _t2);
}
template<class Type> inline bool Overlap<Type>::operator==(const Overlap<Type>& ol) const { return (compare(ol) == 0); }
template<class Type> inline bool Overlap<Type>::operator<(const Overlap<Type>& ol) const { return (compare(ol) < 0); }
template<class Type> inline bool Overlap<Type>::operator>(const Overlap<Type>& ol) const { return (compare(ol) > 0); }

template<class Type> inline const Type Overlap<Type>::getFirst() const { return _t1; }
template<class Type> inline const Type Overlap<Type>::getSecond() const { return _t2; }
template<class Type> inline const Point& Overlap<Type>::getPoint() const { return _pt; }

template<class Type> inline int Overlap<Type>::compare(const Overlap<Type>& ol) const
{
	return (_t1 < ol._t1
			? -1
			: (_t1 > ol._t1
			   ? 1
			   : (_t2 < ol._t2
				  ? -1
				  : (_t2 > ol._t2
					 ? 1
					 : (_pt < ol._pt
						? -1
						: (_pt > ol._pt
						   ? 1
						   : 0))))));
}

//--------------------------------------------------------------------------------
//
// Overlaps
//
//--------------------------------------------------------------------------------
inline Overlaps::Overlaps(const std::vector<ACIDTYPE>& vecAcids,
						  const std::vector<Point>& vecPoints,
						  const std::vector<Stroke>& vecStrokes) :
	_vecAcids(vecAcids), _vecPoints(vecPoints), _vecStrokes(vecStrokes) {}
inline const STROKEOVERLAPS& Overlaps::getOverlaps() { ensureOverlaps(); return _setOverlaps; }

