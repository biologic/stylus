/*******************************************************************************
 * \file	han.cpp
 * \brief	Stylus Han and helper classes
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
// HDimensions
//
//--------------------------------------------------------------------------------

/*
 * Function: set
 *
 */
void
HDimensions::set(const HDimensions& hd)
{
	_nLength = hd._nLength;
	_rectBounds = hd._rectBounds;
}

/*
 * Function: load
 *
 */
void
HDimensions::load(XMLDocument* pxd, xmlNodePtr pxn)
{
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxn));

	xmlNodePtr pxnLength = pxd->getFirstChild(pxn,
											xmlTag(XT_LENGTH),
											pxd->getStylusNamespace());
	xmlNodePtr pxnBounds = pxd->getFirstChild(pxn,
											Rectangle::s_aryRECTANGLEITEMS[Rectangle::RI_BOUNDS],
											pxd->getStylusNamespace());

	if (!VALID(pxnLength))
		THROWRC((RC(XMLERROR), "Missing length element from node %s", pxn->name));
	if (!VALID(pxnBounds))
		THROWRC((RC(XMLERROR), "Missing bounds element from node %s", pxn->name));

	string strValue;
	pxd->getContent(pxnLength, strValue); _nLength = strValue;

	_rectBounds.load(pxd, pxnBounds);
}

//--------------------------------------------------------------------------------
//
// HStroke
//
//--------------------------------------------------------------------------------

/*
 * Function: set
 *
 */
void
HStroke::set(const HStroke& hs)
{
	HDimensions::set(hs);

	_vecPointsForward = hs._vecPointsForward;
	_vecPointsReverse = hs._vecPointsReverse;
}

/*
 * Function: load
 *
 */
void
HStroke::load(XMLDocument* pxd, xmlNodePtr pxnStroke)
{
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnStroke));
	ASSERT(ISEQUALXMLSTR(pxnStroke->name, xmlTag(XT_STROKE)));

	HDimensions::load(pxd, pxnStroke);

	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;

	// Set the context to the current stroke
	spxpc->node = pxnStroke;

	// Load forward points
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_FORWARDPOINTS));
	if (!XMLDocument::isXPathSuccess(spxpo.get()))
		THROWRC((RC(XMLERROR), "Illegal number of forward point elements"));

	loadFromXML<HPOINTARRAY>(_vecPointsForward, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

	// Load reverse points
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_REVERSEPOINTS));
	if (!XMLDocument::isXPathSuccess(spxpo.get()))
		THROWRC((RC(XMLERROR), "Illegal number of reverse point elements"));

	loadFromXML<HPOINTARRAY>(_vecPointsReverse, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);
}

//--------------------------------------------------------------------------------
//
// HGroup
//
//--------------------------------------------------------------------------------

/*
 * Function: set
 *
 */
void
HGroup::set(const HGroup& hg)
{
	HDimensions::set(hg);
	_ptWeightedCenter = hg._ptWeightedCenter;
	_vecStrokes = hg._vecStrokes;
}

/*
 * Function: load
 *
 */
void
HGroup::load(XMLDocument* pxd, xmlNodePtr pxnGroup)
{
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnGroup));
	ASSERT(ISEQUALXMLSTR(pxnGroup->name, xmlTag(XT_GROUP)));

	HDimensions::load(pxd, pxnGroup);

	xmlNodePtr pxn;

	// Load the weighted center value
	pxn = pxd->getFirstChild(pxnGroup,
							xmlTag(XT_WEIGHTEDCENTER),
							pxd->getStylusNamespace());
	ASSERT(VALID(pxn));
	_ptWeightedCenter.load(pxd,pxn);

	// Load and parse the list of contained strokes
	// Note:
	// - Strokes are referenced by index into the sequence of stroke elements (one-based indexing)
	pxn = pxd->getFirstChild(pxnGroup,
							xmlTag(XT_CONTAINEDSTROKES),
							pxd->getStylusNamespace());
	ASSERT(VALID(pxn));

	string strStrokes;
	pxd->getContent(pxn, strStrokes);

	const char* pszStart = strStrokes.c_str();
	const char* pszEnd = pszStart + 1;
	while (*pszStart != Constants::s_chNULL)
	{
		_vecStrokes.push_back(::strtol(pszStart, const_cast<char**>(&pszEnd), 10)-1);
		pszStart = pszEnd;
	}
}

//--------------------------------------------------------------------------------
//
// HOverlap
//
//--------------------------------------------------------------------------------

/*
 * Function: load
 *
 */
void
HOverlap::load(XMLDocument* pxd, xmlNodePtr pxnOverlap)
{
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnOverlap));
	ASSERT(ISEQUALXMLSTR(pxnOverlap->name, xmlTag(XT_OVERLAP)));

	string strValue;
	pxd->getAttribute(pxnOverlap, xmlTag(XT_FIRSTSTROKE), strValue); _iStroke1 = ::atol(strValue.c_str())-1;
	pxd->getAttribute(pxnOverlap, xmlTag(XT_SECONDSTROKE), strValue); _iStroke2 = ::atol(strValue.c_str())-1;

	pxd->getAttribute(pxnOverlap, xmlTag(XT_REQUIRED), strValue);
	_fRequired = XMLDocument::isXMLTrue(strValue);
}

//--------------------------------------------------------------------------------
//
// Han
//
//--------------------------------------------------------------------------------

string Han::s_strScope;
vector<Han*> Han::s_vecHan;

/*
 * Function: deleteHan
 *
 */
static void
deleteHan(const Han* pHan)
{
	::delete pHan;
}

/*
 * Function: initialize
 *
 */
void
Han::initialize()
{
	ENTER(HAN,initialize);
	s_strScope = Constants::s_strDEFAULTSCOPE;
}

/*
 * Function: terminate
 *
 */
void
Han::terminate()
{
	ENTER(HAN,terminate);
	for_each(s_vecHan.begin(), s_vecHan.end(), deleteHan);
}

/*
 * Function: setScope
 * 
 */
void
Han::setScope(const char* pszScope)
{
	ENTER(HAN,setScope);
	ASSERT(VALID(pszScope) && !EMPTYSZ(pszScope));

	s_strScope.assign(pszScope);
	terminatePath(s_strScope);
}

/*
 * Function: getDefinition
 *
 */
const Han&
Han::getDefinition(const string& strUnicode)
{
	ENTER(HAN,getDefinition);
	ASSERT(!EMPTYSTR(strUnicode));

	const Han* pHan = NULL;

	// Return a reference if the Han defintion is already loaded
	for (size_t iHan=0; iHan < s_vecHan.size(); ++iHan)
	{
		if (*s_vecHan[iHan] == strUnicode)
		{
			pHan = s_vecHan[iHan];
			break;
		}
	}

	// Otherwise load the defintion from the passed URL
	if (!VALID(pHan))
	{
		ASSERT(strUnicode.length() >= 4);

		string strURL(s_strScope);
		strURL += strUnicode.substr(0, strUnicode.length()-3) + "000/" + strUnicode + Constants::s_strHANEXTENSION;

		TFLOW(HAN,L2,(LLTRACE, "Loading Han %s from %s", strUnicode.c_str(), strURL.c_str()));
	
		XMLDocumentSPtr spxd(XMLDocument::createInstanceFromURL(strURL));
		xmlXPathContextSPtr spxpc(spxd->createXPathContext());
		xmlXPathObjectSPtr spxpo;

		// Create a Han object from the root element
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_HANDEFINITION));
		if (!XMLDocument::isXPathSuccess(spxpo.get(), 1))
			THROWRC((RC(XMLERROR), "Unexpected number of hanDefinition elements"));

		s_vecHan.push_back(::new Han());
		s_vecHan.back()->load(spxd.get(), spxpo->nodesetval->nodeTab[0]);

		// Ensure the loaded Han matches that requested
		if (strUnicode != s_vecHan.back()->_strUnicode)
		{
			string strUnicodeHan(s_vecHan.back()->_strUnicode);
			s_vecHan.erase(s_vecHan.end()-1);
			THROWRC((RC(ERROR),
					 "Loaded Han definition does not match requested unicode value (%s vs. %s)",
					 strUnicode.c_str(),
					 strUnicodeHan.c_str()));
		}

		LOGINFO((LLINFO, "Loaded Han %s - uuid(%s)",
				 s_vecHan.back()->_strUnicode.c_str(),
				 s_vecHan.back()->_strUUID.c_str()));

		pHan = s_vecHan.back();
	}

	ASSERT(VALID(pHan));
	return *pHan;
}

/*
 * Function: set
 *
 */
void
Han::set(const Han& han)
{
	HDimensions::set(han);

	_strUUID = han._strUUID;
	_strUnicode = han._strUnicode;

	_nMinimumStrokeLength = han._nMinimumStrokeLength;

	_vecGroups = han._vecGroups;
	_vecStrokes = han._vecStrokes;
	_vecOverlaps = han._vecOverlaps;

	_mapStrokeToGroup = _mapStrokeToGroup;
}

/*
 * Function: load
 *
 */
void
Han::load(XMLDocument* pxd, xmlNodePtr pxnHanDefinition)
{
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnHanDefinition));
	ASSERT(ISEQUALXMLSTR(pxnHanDefinition->name, xmlTag(XT_HANDEFINITION)));

	// Load Han-level properties
	pxd->getAttribute(pxnHanDefinition, xmlTag(XT_UUID), _strUUID);
	pxd->getAttribute(pxnHanDefinition, xmlTag(XT_UNICODE), _strUnicode);

	HDimensions::load(pxd, pxnHanDefinition);

	// Load minimum stroke length
	xmlNodePtr pxnMinimumStrokeLength = pxd->getFirstChild(pxnHanDefinition,
															xmlTag(XT_MINIMUMSTROKELENGTH),
															pxd->getStylusNamespace());

	if (!VALID(pxnMinimumStrokeLength))
		THROWRC((RC(XMLERROR), "Missing minimumStrokeLength element from node %s", pxnHanDefinition->name));

	string strValue;
	pxd->getContent(pxnMinimumStrokeLength, strValue); _nMinimumStrokeLength = strValue;
	if (_nMinimumStrokeLength <= static_cast<UNIT>(0))
		THROWRC((RC(XMLERROR), "Illegal minimumStrokeLength - %f", static_cast<UNIT>(_nMinimumStrokeLength)));

	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;

	// Load groups
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_GROUPS));
	if (!XMLDocument::isXPathSuccess(spxpo.get()))
		THROWRC((RC(XMLERROR), "Illegal number of group elements"));

	loadFromXML<HGROUPARRAY>(_vecGroups, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

	// Load strokes
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_STROKES));
	if (!XMLDocument::isXPathSuccess(spxpo.get()))
		THROWRC((RC(XMLERROR), "Illegal number of stroke elements"));

	loadFromXML<HSTROKEARRAY>(_vecStrokes, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

	// Load overlaps
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_OVERLAPS));
	if (XMLDocument::isXPathSuccess(spxpo.get()))
		loadFromXML<HOVERLAPARRAY>(_vecOverlaps, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

	// Build the map from strokes to groups
	_mapStrokeToGroup.resize(_vecStrokes.size());
	for (size_t iGroup=0; iGroup < _vecGroups.size(); ++iGroup)
	{
		const vector<size_t>& vecGroupStrokes = _vecGroups[iGroup].getStrokes();
		for (size_t iGroupStroke=0; iGroupStroke < vecGroupStrokes.size(); ++iGroupStroke)
		{
			ASSERT(vecGroupStrokes[iGroupStroke] < _mapStrokeToGroup.size());
			_mapStrokeToGroup[vecGroupStrokes[iGroupStroke]] = iGroup;
		}
	}
}

