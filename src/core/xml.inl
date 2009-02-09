/*******************************************************************************
 * \file	xml.inl
 * \brief	Stylus XML helper inline methods
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
// stylus
//
//--------------------------------------------------------------------------------
inline const char* stylus::xmlTag(XMLTAG xt) { ASSERT(xt < XT_MAX); return XMLDocument::s_aryXMLTAG[xt]; }
inline const char* stylus::xmlXPath(XMLXPATH xp) { ASSERT(xp < XP_MAX); return XMLDocument::s_aryXMLXPATH[xp]; }


//--------------------------------------------------------------------------------
//
// XMLDocument
//
//--------------------------------------------------------------------------------
inline bool XMLDocument::isXMLTrue(const std::string& str) { return (str == Constants::s_strXMLTRUE); }
inline bool XMLDocument::isXMLTag(xmlNodePtr pxn, const char* pszTag, const char* pszNS)
{
	return (	VALID(pxn)
			&&	pxn->type == XML_ELEMENT_NODE
			&&	(	!VALID(pszNS)
				||	(	VALID(pxn->ns)
					&&	!::strcmp((const char*)pxn->ns->href, pszNS)))
			&&	(	!VALID(pszTag)
				||	!::strcmp((const char*)pxn->name, pszTag)));
}

inline bool XMLDocument::isXPathSuccess(xmlXPathObjectPtr pxpo, int nMaximumNodes, int nMinimumNodes)
{
	return (	VALID(pxpo)
			&&	pxpo->type == XPATH_NODESET
			&&	VALID(pxpo->nodesetval)
			&&	pxpo->nodesetval->nodeNr <= nMaximumNodes
			&&	pxpo->nodesetval->nodeNr >= nMinimumNodes);
}

inline XMLDocument* XMLDocument::createInstance(const std::string& strXML) { return createInstance(strXML.c_str()); }

inline const char* XMLDocument::getStylusNamespace() { return _pszStylusNamespace; }

inline xmlNodePtr XMLDocument::addText(xmlNodePtr pxn, std::string& strText) { return addText(pxn, strText.c_str()); }

inline xmlNodePtr XMLDocument::createText(std::string& strText) { return createText(strText.c_str()); }

inline xmlNodePtr XMLDocument::getFirstSibling(xmlNodePtr pxn, std::string& strSiblingTag) { return getFirstSibling(pxn, strSiblingTag.c_str(), NULL); }
inline xmlNodePtr XMLDocument::getFirstSibling(xmlNodePtr pxn, std::string& strSiblingTag, std::string& strSiblingNS) { return getFirstSibling(pxn, strSiblingTag.c_str(), strSiblingNS.c_str()); }
inline xmlNodePtr XMLDocument::getFirstSibling(xmlNodePtr pxn, std::string& strSiblingTag, const char* pszSiblingNS) { return getFirstSibling(pxn, strSiblingTag.c_str(), pszSiblingNS); }

inline xmlNodePtr XMLDocument::getFirstChild(xmlNodePtr pxn, const char* pszChildTag, const char* pszChildNS) { return getFirstSibling(pxn->children, pszChildTag, pszChildNS); }
inline xmlNodePtr XMLDocument::getFirstChild(xmlNodePtr pxn, std::string& strChildTag) { return getFirstChild(pxn, strChildTag.c_str(), NULL); }
inline xmlNodePtr XMLDocument::getFirstChild(xmlNodePtr pxn, std::string& strChildTag, std::string& strChildNS) { return getFirstChild(pxn, strChildTag.c_str(), strChildNS.c_str()); }
inline xmlNodePtr XMLDocument::getFirstChild(xmlNodePtr pxn, std::string& strChildTag, const char* pszChildNS) { return getFirstChild(pxn, strChildTag.c_str(), pszChildNS); }

inline int XMLDocument::countSiblings(xmlNodePtr pxn, std::string& strSiblingTag) { return countSiblings(pxn, strSiblingTag.c_str(), NULL); }
inline int XMLDocument::countSiblings(xmlNodePtr pxn, std::string& strSiblingTag, std::string& strSiblingNS) { return countSiblings(pxn, strSiblingTag.c_str(), strSiblingNS.c_str()); }
inline int XMLDocument::countSiblings(xmlNodePtr pxn, std::string& strSiblingTag, const char* pszSiblingNS) { return countSiblings(pxn, strSiblingTag.c_str(), pszSiblingNS); }

inline int XMLDocument::countChildren(xmlNodePtr pxn, const char* pszChildTag, const char* pszChildNS) { return countSiblings(pxn->children, pszChildTag, pszChildNS); }
inline int XMLDocument::countChildren(xmlNodePtr pxn, std::string& strChildTag) { return countChildren(pxn, strChildTag.c_str(), NULL); }
inline int XMLDocument::countChildren(xmlNodePtr pxn, std::string& strChildTag, std::string& strChildNS) { return countChildren(pxn, strChildTag.c_str(), strChildNS.c_str()); }
inline int XMLDocument::countChildren(xmlNodePtr pxn, std::string& strChildTag, const char* pszChildNS) { return countChildren(pxn, strChildTag.c_str(), pszChildNS); }

inline xmlXPathObjectPtr XMLDocument::evalXPath(xmlXPathContextPtr pxpc, std::string& strExpression) { return evalXPath(pxpc, strExpression.c_str()); }
inline xmlXPathObjectPtr XMLDocument::evalXPath(std::string& strExpression) { return evalXPath(strExpression.c_str()); }

template<class T> void
stylus::loadFromXML(T& t, XMLDocument* pxd, int cNodes, xmlNodePtr* aryNodes)
{
	ENTER(XML,loadFromXML);
	ASSERT(VALID(pxd));
	ASSERT(cNodes > 0);
	ASSERT(VALID(aryNodes));

	t.resize(cNodes);
	typename T::iterator it = t.begin();
	for (int iNode=0; iNode < cNodes; ++iNode, ++it)
	{
		ASSERT(it != t.end());
		it->load(pxd, aryNodes[iNode]);
	}
}

//--------------------------------------------------------------------------------
//
// XMLStream
//
//--------------------------------------------------------------------------------
inline XMLStream::XMLStream(std::ostream& ostr, bool fInitialize) : _ostr(ostr) { initialize(fInitialize); }

inline void XMLStream::writeStart(const char* pszTag, bool fWithContent, bool fWithNewline)
{
	_ostr
		<< Constants::s_chLESSTHAN
		<< pszTag;
	if (!fWithContent)
		_ostr << Constants::s_chBLANK << Constants::s_chFORWARDSLASH;
	_ostr << Constants::s_chGREATERTHAN;
	if (fWithNewline)
		_ostr << endl;
}
inline void XMLStream::openStart(const char* pszTag)
{
	_ostr
		<< Constants::s_chLESSTHAN
		<< pszTag;
}
inline void XMLStream::closeStart(bool fWithContent, bool fWithNewline)
{
	if (!fWithContent)
		_ostr << Constants::s_chBLANK << Constants::s_chFORWARDSLASH;
	_ostr << Constants::s_chGREATERTHAN;
	if (fWithNewline)
		_ostr << endl;
}

inline void XMLStream::writeEnd(const char* pszTag, bool fWithNewline)
{
	_ostr
		<< Constants::s_chLESSTHAN
		<< Constants::s_chFORWARDSLASH
		<< pszTag
		<< Constants::s_chGREATERTHAN;
	if (fWithNewline)
		_ostr << endl;
}

template<class T> inline void XMLStream::writeAttribute(const char* pszAttribute, T t)
{
	_ostr
		<< Constants::s_chBLANK
		<< pszAttribute
		<< Constants::s_chEQUAL
		<< Constants::s_chAPOSTROPHE
		<< t
		<< Constants::s_chAPOSTROPHE;
}

inline void XMLStream::writeAttribute(const char* pszAttribute, Unit& nValue)
{
	writeAttribute<UNIT>(pszAttribute, static_cast<UNIT>(nValue));
}

template<class T> inline void XMLStream::writeContent(T t) { _ostr << t; }
inline void XMLStream::writeContent(Unit& nValue) { writeContent<UNIT>(static_cast<UNIT>(nValue)); }
