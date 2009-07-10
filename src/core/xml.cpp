/*******************************************************************************
 * \file    xml.cpp
 * \brief	XML helper routines (see xml.hpp for details)
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

static const char* s_aryXMLDomains[] =
{
	"NONE",
	"PARSER",
	"TREE",
	"NAMESPACE",
	"DTD",
	"HTML",
	"MEMORY",
	"OUTPUT",
	"IO",
	"FTP",
	"HTTP",
	"XINCLUDE",
	"XPATH",
	"XPOINTER",
	"REGEXP",
	"DATATYPE",
	"SCHEMASP",
	"SCHEMASV",
	"RELAXNGP",
	"RELAXNGV",
	"CATALOG",
	"C14N",
	"XSLT",
	"VALID",
	"CHECK",
	"WRITER",
	"MODULE",
	"I18N"
};

const char* XMLDocument::s_aryXMLTAG[XT_MAX] =
{
	"accepted",
	"acceptedMutations",
	"accumulateMutations",
	"ancestors",
	"attempt",
	"attempted",
	"attempts",
	"author",
	"average",
	"baseFirst",
	"baseLast",
	"bases",
	"basesAfter",
	"bottom",
	"bounds",
	"change",
	"changed",
	"changes",
	"codon",
	"codonTable",
	"coherent",
	"containedStrokes",
	"copied",
	"copies",
	"copy",
	"correspondsTo",
	"cost",
	"costCondition",
	"costPerBase",
	"costPerUnit",
	"costRange",
	"count",
	"countBases",
	"countRollbacks",
	"creationDate",
	"creationParameter",
	"creationTool",
	"decrease",
	"delete",
	"deleted",
	"deletions",
	"deltaIndex",
	"description",
	"deviation",
	"dropouts",
	"durationCondition",
	"dxToHan",
	"dyToHan",
	"extraLength",
	"ensureInFrame",
	"ensureWholeCodons",
	"entry",
	"factor",
	"firstStroke",
	"fitness",
	"fitnessCondition",
	"fitnessRange",
	"fixedCost",
	"forward",
	"fractionalDistance",
	"gene",
	"geneRange",
	"genes",
	"genome",
	"globals",
	"group",
	"groups",
	"groupScoreMode",
	"hanDefinition",
	"hanReference",
	"hanReferences",
	"hanStrokeRange",
	"height",
	"history",
	"illegalOverlaps",
	"increase",
	"indexRange",
	"infinite",
	"insert",
	"inserted",
	"insertions",
	"left",
	"length",
	"likelihood",
	"lineage",
	"maintain",
	"marks",
	"maximum",
	"minimum",
	"minimumStrokeLength",
	"missingOverlaps",
	"mode",
	"mutationCondition",
	"mutations",
	"mutationsPerAttempt",
	"none",
	"options",
	"origin",
	"overlap",
	"overlaps",
	"placement",
	"plan",
	"point",
	"points",
	"preserveGenes",
	"processorID",
	"reasonCode",
	"rejectedMutations",
	"rejectSilent",
	"required",
	"reverse",
	"right",
	"rollbackCondition",
	"rollbackLimit",
	"rollbacks",
	"scale",
	"score",
	"scoreCondition",
	"scoreRange",
	"secondStroke",
	"seed",
	"segment",
	"segments",
	"silent",
	"sourceIndex",
	"sp-deviation",
	"sp-dropouts",
	"sp-extraLength",
	"sp-illegalOverlaps",
	"sp-marks",
	"sp-missingOverlaps",
	"sp-placement",
	"sp-scale",
	"statistics",
	"step",
	"steps",
	"strain",
	"stroke",
	"strokes",
	"sxToHan",
	"syToHan",
	"tandem",
	"targetIndex",
	"termination",
	"terminationCode",
	"terminationConditions",
	"threshold",
	"thresholdCondition",
	"top",
	"translation",
	"transpose",
	"transposed",
	"transpositions",
	"transversionLikelihood",
	"trial",
	"trialAttempts",
	"trialConditions",
	"trialFirst",
	"trialLast",
	"trials",
	"trialStart",
	"unicode",
	"units",
	"unitsRange",
	"uuid",
	"value",
	"vector",
	"weightedCenter",
	"weights",
	"width",
	"x",
	"x-midpoint",
	"xmlns",
	"y",
	"y-midpoint",
	"unknown value"
};

const char* XMLDocument::s_aryXMLXPATH[XP_MAX] =
{
	"/st:genome",
	"/st:genome/st:seed",
	"/st:genome/st:codonTable",
	"st:entry",
	"/st:genome/st:bases",
	"/st:geneome/st:lineage",
	"/st:genome/st:statistics",
	"/st:genome/st:genes/st:gene",
	"st:origin",
	"st:hanReferences/st:hanReference",
	"st:strokes/st:stroke",

	"/st:hanDefinition",
	"/st:hanDefinition/st:groups/st:group",
	"/st:hanDefinition/st:strokes/st:stroke",
	"/st:hanDefinition/st:overlaps/st:overlap",
	"st:points/st:forward/st:pointDistance",
	"st:points/st:reverse/st:pointDistance",

	"/st:plan/st:options",
	"st:terminationConditions/st:durationCondition",
	"st:terminationConditions/st:rollbackCondition",
	"st:terminationConditions/st:fitnessCondition",
	"/st:plan/st:steps/st:step",
	"st:trialConditions",
	"st:costCondition",
	"st:fitnessCondition",
	"st:scoreCondition",
	"st:mutationCondition",
	"st:value",
	"st:mutationsPerAttempt",
	"st:insert|st:delete|st:copy|st:change|st:transpose",

	"/st:globals",
	"/st:globals/st:weights/st:genome",
	"/st:globals/st:weights/st:gene",
	"/st:globals/st:weights/st:group"
};

const char XMLDocument::s_szStylusPrefix[] = "st";
const char XMLDocument::s_szStylusNamespacePrerelease[] = "http://biologicinstitute.org/schemas/stylus/0.1";
const char XMLDocument::s_szStylusNamespace1_0[] = "http://biologicinstitute.org/schemas/stylus/1.0";
const char XMLDocument::s_szStylusNamespace1_3[] = "http://biologicinstitute.org/schemas/stylus/1.3";
const char* XMLDocument::s_szStylusNamespace = XMLDocument::s_szStylusNamespace1_3;

SCHEMA XMLDocument::s_arySCHEMAS[] =
{
	{ XMLDocument::s_szStylusNamespacePrerelease, "stylus-0.1.xsd", NULL, false },
	{ XMLDocument::s_szStylusNamespace1_0, "stylus-1.0.xsd", NULL, true },
	{ XMLDocument::s_szStylusNamespace1_3, "stylus-1.3.xsd", NULL, true }
};

string XMLDocument::s_strScope;
char XMLDocument::s_szXMLError[Constants::s_cbmaxBUFFER];

/*
 * Function: xmlFreeChar
 *
 */
void stylus::xmlFreeChar(xmlChar* _pv) throw()
{
	::xmlFree(_pv);
}

/*
 * Function: xmlHTTPClose
 *
 */
void
stylus::xmlHTTPClose(unsigned char* _pv) throw()
{
	::xmlNanoHTTPClose((void*)_pv);
}

//--------------------------------------------------------------------------------
//
//	XMLDocument
//
//--------------------------------------------------------------------------------

/*
 * Function: initialize
 *
 */
void
XMLDocument::initialize()
{
	ENTER(XML,initialize);

	clearErrors();
	
	::xmlInitParser();
	::xmlLineNumbersDefault(1);
	::xmlSubstituteEntitiesDefault(1);

	xmlGenericErrorFunc pf = &handlerGenericError;
	::initGenericErrorDefaultFunc(&pf);

	::xmlNanoHTTPInit();
	
	s_strScope = Constants::s_strDEFAULTSCOPE;
	terminatePath(s_strScope);
}

/*
 * Function: terminate
 *
 */
void
XMLDocument::terminate()
{
	ENTER(XML,terminate);

	freeSchemas();
	
	::xmlNanoHTTPCleanup();
	::xmlCleanupParser();
}

/*
 * Function: setScope
 * 
 */
void
XMLDocument::setScope(const char* pszScope)
{
	ENTER(XML,setScope);
	
	ASSERT(VALID(pszScope) && !EMPTYSZ(pszScope));
	s_strScope.assign(pszScope);
	terminatePath(s_strScope);

	freeSchemas();
	loadSchemas();
}

/*
 * Function: loadSchemas
 *
 */
void
XMLDocument::loadSchemas()
{
	ENTER(XML,loadSchemas);
	ASSERT(!EMPTYSTR(s_strScope));

	string strURL = s_strScope;
	size_t cchURL = strURL.length();

	for (size_t i=0; i < ARRAY_LENGTH(s_arySCHEMAS); ++i)
	{
		ASSERT(!VALID(s_arySCHEMAS[i]._pxsSchema));
		
		strURL.resize(cchURL);
		strURL.append(s_arySCHEMAS[i]._pszFilename);

		xmlSchemaParserCtxtSPtr spxsc(::xmlSchemaNewParserCtxt(strURL.c_str()));
		if (!VALIDSP(spxsc))
			THROWXML();

		::xmlSchemaSetParserErrors(spxsc.get(), &handlerGenericError, &handlerGenericError, NULL);
		::xmlSchemaSetParserStructuredErrors(spxsc.get(), &handlerStructuredError, NULL);

		s_arySCHEMAS[i]._pxsSchema = ::xmlSchemaParse(spxsc.get());
		if (!VALID(s_arySCHEMAS[i]._pxsSchema) && s_arySCHEMAS[i]._fRequired)
			THROWXML();
	}
}

/*
 * Function: freeSchemas
 *
 */
void
XMLDocument::freeSchemas()
{
	ENTER(XML,freeSchemas);

	for (size_t i=0; i < ARRAY_LENGTH(s_arySCHEMAS); ++i)
	{
		if (VALID(s_arySCHEMAS[i]._pxsSchema))
		{
			::xmlSchemaFree(s_arySCHEMAS[i]._pxsSchema);
			s_arySCHEMAS[i]._pxsSchema = NULL;
		}
	}
}

/*
 * Function: handlerGenericError
 *
 */
void
XMLDocument::handlerGenericError(void* pv, const char* pszError, ...)
{
	return;
}

/*
 * Function handlerStructuredError
 *
 */
void
XMLDocument::handlerStructuredError(void* pv, xmlErrorPtr pxe)
{
	return;
}

/*
 * Function: setNextXMLError
 *
 */
const Error*
XMLDocument::setNextError(const char* pszFileline) throw()
{
	xmlErrorToString(::xmlGetLastError(), s_szXMLError, CCH_OF(s_szXMLError));
	const Error* pe = Error::setNextError(pszFileline, ST_RCXMLERROR, s_szXMLError);
	clearErrors();
	return pe;
}

/*
 * Function: clearErrors
 *
 */
void
XMLDocument::clearErrors() throw()
{
	s_szXMLError[0] = Constants::s_chNULL;
	::xmlResetLastError();
}

/*
 * Function: xmlErrorToString
 *
 */
size_t
XMLDocument::xmlErrorToString(xmlErrorPtr pxe, char* pszBuffer, size_t cchBuffer)
{
	size_t cchWritten = 0;

	if (VALID(pxe))
	{
		if (VALID(pxe->message))
		{
			cchWritten = ::snprintf(pszBuffer, cchBuffer, "XML error (%s)", pxe->message);
			char* psz=pszBuffer;
			for (size_t i=0; i < cchWritten; i++, psz++)
			{
				if (*psz == Constants::s_chNEWLINE)
					*psz = Constants::s_chBLANK;
			}
		}
		else
		{
			cchWritten = (VALID(pxe->file)
						  ? ::snprintf(pszBuffer, cchBuffer, "XML error in area %s at %s:%d",
									   s_aryXMLDomains[pxe->domain], pxe->file, pxe->line)
						  : ::snprintf(pszBuffer, cchBuffer, "XML error in area %s",
									   s_aryXMLDomains[pxe->domain]));
		}
	}

	return cchWritten;
}

/*
 * Function: createInstance
 *
 */
XMLDocument*
XMLDocument::createInstance(const char* pszXML)
{
	ENTER(XML,createInstance);

	ASSERT(VALID(pszXML));

	clearErrors();

	xmlParserCtxtSPtr spxpc(::xmlCreateDocParserCtxt((xmlChar*)pszXML));

	::xmlSetGenericErrorFunc(spxpc.get(), &handlerGenericError);
	::xmlSetStructuredErrorFunc(spxpc.get(), &handlerStructuredError);

	if (::xmlParseDocument(spxpc.get()) < 0)
		THROWXML();

	xmlDocSPtr spxd(spxpc->myDoc);
	
	size_t iSchema = 0;
	for (; iSchema < ARRAY_LENGTH(s_arySCHEMAS); ++iSchema)
	{
		if (VALID(s_arySCHEMAS[iSchema]._pxsSchema))
		{
			xmlSchemaValidCtxtSPtr spxsv(::xmlSchemaNewValidCtxt(s_arySCHEMAS[iSchema]._pxsSchema));
			if (!VALIDSP(spxsv))
				THROWXML();

			if (!::xmlSchemaValidateDoc(spxsv.get(), spxd.get()))
				break;
		}
	}
	if (iSchema >= ARRAY_LENGTH(s_arySCHEMAS))
		THROWRC((RC(XMLERROR), "XML document failed validation with all schemas"));

	return ::new XMLDocument(spxd.release());
}

/*
 * Function: createInstanceFromURL
 *
 */
XMLDocument*
XMLDocument::createInstanceFromURL(const std::string& strURL)
{
	ENTER(XML,createInstanceFromURL);

	ASSERT(!EMPTYSTR(strURL));
	ASSERT(strURL.length() > Constants::s_strURIScheme_HTTP.length());

	clearErrors();

	XMLDocument* pxd = NULL;

	if (!strURL.compare(0, Constants::s_strURIScheme_HTTP.length(), Constants::s_strURIScheme_HTTP))
	{
		void* pHTTPCtxt = ::xmlNanoHTTPOpen(strURL.c_str(), NULL);
		if (!VALID(pHTTPCtxt))
			THROWXML();

		char sz[Constants::s_cbmaxBUFFER];
		int cbRead;
		string strXML;
		do
		{
			cbRead = ::xmlNanoHTTPRead(pHTTPCtxt, sz, SZ_OF(sz));
			if (cbRead < 0)
				THROWXML();
			sz[cbRead] = Constants::s_chNULL;
			strXML += sz;
		} while (cbRead > 0);

		::xmlNanoHTTPClose(pHTTPCtxt);

		pxd = createInstance(strXML);
	}

	else if (!strURL.compare(0, Constants::s_strURIScheme_FILE.length(), Constants::s_strURIScheme_FILE))
	{
		string strFile(strURL.substr(Constants::s_strURIScheme_FILE.length()));

		ifstream fileXML(strFile.c_str());

		if (!fileXML)
			THROWRC((RC(BADARGUMENTS), "Unable to open file URL: %s", strFile.c_str()));

		ostringstream ostrXML;
		ostrXML << fileXML.rdbuf();

		pxd = createInstance(ostrXML.str().c_str());
	}

	else
		THROWRC((RC(BADARGUMENTS), "URL contains an unsupported schemed: %s", strURL.c_str()));

	return pxd;
}

/*
 * Function: destroyInstance
 *
 */
void
XMLDocument::destroyInstance(XMLDocument* pxd) throw()
{
	delete pxd;
}

/*
 * Function: XMLDocument
 *
 */
XMLDocument::XMLDocument(xmlDocPtr pxd) : _spxd(pxd)
{
	ENTER(XML,XMLDocument);
	ASSERT(VALIDSP(_spxd));
	
	xmlNodePtr pxnRoot = ::xmlDocGetRootElement(pxd);
	
	// Determine the Stylus namespace in use by the document
	for (size_t i=0; i < ARRAY_LENGTH(s_arySCHEMAS); ++i)
	{
		ASSERT(VALID(s_arySCHEMAS[i]._pszURI));
		
		if (::xmlSearchNsByHref(pxd, pxnRoot, (xmlChar *)s_arySCHEMAS[i]._pszURI) != NULL)
		{
			_pszStylusNamespace = s_arySCHEMAS[i]._pszURI;
			return;
		}
	}
	
	// Throw an error if no supported namespace was found
	THROWRC((RC(XMLERROR), "Document does not contain any supported Stylus namespaces"));
}

/*
 * Function: toString
 *
 */
string
XMLDocument::toString() const
{
	ENTER(XML,toString);
	
	ASSERT(VALIDSP(_spxd));

	xmlCharSPtr spxz;
	int cch;

	::xmlDocDumpMemory(_spxd.get(), &spxz, &cch);
	if (!VALIDSP(spxz))
		THROWXML();

	return string((const char *)spxz.get(), (size_t)cch);
}

/*
 * Function: setContent
 *
 */
void
XMLDocument::setContent(xmlNodePtr pxn, std::string& strText)
{
	ENTER(XML,setContent);

	ASSERT(VALID(pxn));

	::xmlNodeSetContent(pxn, (const xmlChar*)strText.c_str());
}

/*
 * Function: getContent
 *
 */
bool
XMLDocument::getContent(xmlNodePtr pxn, std::string& str)
{
	ENTER(XML,getContent);

	ASSERT(VALID(pxn));

	bool fSuccess = false;

	xmlCharSPtr spxz(::xmlNodeGetContent(pxn));
	if (VALIDSP(spxz))
	{
		str = ((const char*)spxz.get());
		fSuccess = true;
	}

	return true;
}

/*
 * Function: setAttribute
 *
 */
void
XMLDocument::setAttribute(xmlNodePtr pxn, const char* pszAttribute, std::string& strValue)
{
	ENTER(XML,setAttribute);

	ASSERT(VALID(pxn));

	xmlAttrPtr pxa = ::xmlHasProp(pxn, (const xmlChar*)pszAttribute);
	if (VALID(pxa))
	{
		if (::xmlRemoveProp(pxa) < 0)
			THROWXML();
	}

	if (!VALID(::xmlNewProp(pxn, (const xmlChar*)pszAttribute, (const xmlChar*)strValue.c_str())))
		THROWXML();
}

/*
 * Function: getAttribute
 *
 */
bool
XMLDocument::getAttribute(xmlNodePtr pxn, const char* pszAttribute, std::string& str)
{
	ENTER(XML,getAttribute);

	ASSERT(VALID(pxn));

	bool fSuccess = false;

	xmlCharSPtr spxz(::xmlGetProp(pxn, (const xmlChar*)pszAttribute));
	if (VALIDSP(spxz))
	{
		str = ((const char*)spxz.get());
		fSuccess = true;
	}
	
	return fSuccess;
}

/*
 * Function: clearAttributes
 *
 */
void
XMLDocument::clearAttributes(xmlNodePtr pxn)
{
	ENTER(XML,clearAttributes);

	ASSERT(VALID(pxn));

	if (VALID(pxn->properties))
	{
		xmlAttrPtr pxa = pxn->properties;

		pxn->properties = NULL;
		::xmlFreePropList(pxa);
	}
}

/*
 * Function: addText
 *
 */
xmlNodePtr
XMLDocument::addText(xmlNodePtr pxn, const char* pszText)
{
	ENTER(XML,addText);

	ASSERT(VALID(pxn));
	ASSERT(VALID(pszText));

	xmlNodePtr pxnText = NULL;

	if (!VALID(::xmlAddChild(pxn, createText(pszText))))
		THROWXML();

	return pxnText;
}

/*
 * Function: createText
 *
 */
xmlNodePtr
XMLDocument::createText(const char* pszText)
{
	ENTER(XML,createText);

	ASSERT(VALIDSP(_spxd));
	ASSERT(VALID(pszText));
	
	xmlNodePtr pxn = ::xmlNewDocText(_spxd.get(), (const xmlChar *)pszText);

	if (!VALID(pxn))
		THROWXML();

	return pxn;
}

/*
 * Function: getFirstSibling
 *
 */
xmlNodePtr
XMLDocument::getFirstSibling(xmlNodePtr pxnSibling, const char* pszSiblingTag, const char* pszSiblingNS)
{
	ENTER(XML,getFirstSibling);

	ASSERT(VALID(pxnSibling));
	
	// Search through all siblings for an element node with the passed tag and namespace
	// Note: The predicate is complex because it states when to continue looking vs.
	//		 when to stop looking. Further, the pszSiblingTag and pszSiblingNS arguments are
	//		 optional, making the tests relatively nested. The clause attempts to say:
	//		 Continue if
	//			-- The sibling node is valid
	//			-- And it is not an element node
	//			-- Or it is an element, but the name fails to match that passed
	//			-- Or it is an element, but the namespace fails to match
	while (		VALID(pxnSibling)
		  &&	(	pxnSibling->type != XML_ELEMENT_NODE
				||	(	VALID(pszSiblingTag)
					&&	::strcmp((const char*)pxnSibling->name, pszSiblingTag))
				||	(	VALID(pszSiblingNS)
					&&	(	!VALID(pxnSibling->ns)
						||	::strcmp((const char*)pxnSibling->ns->href, pszSiblingNS)))))
	{
		pxnSibling = pxnSibling->next;
	}

	return pxnSibling;
}

/*
 * Function: findDescendent
 *
 */
xmlNodePtr
XMLDocument::findDescendent(xmlNodePtr pxn, const std::vector<QNAME>& vecQNames)
{
	ENTER(XML,findDescendent);

	ASSERT(VALID(pxn));
	
	// Search for the node as long as a node and path components exist
	const size_t cNames = vecQNames.size();
	size_t iPath = 0;
	while (		VALID(pxn)
		  &&	iPath < cNames)
	{
		const QNAME& qname = vecQNames[iPath];
		
		// The element matches if the names match and the namespaces (if any) match
		if (	pxn->type == XML_ELEMENT_NODE
			&&	!::strcmp((const char*)pxn->name, qname._pszName)
			&&	(	(	!VALID(qname._pszURI)
					&&	!VALID(pxn->ns))
				||	(	VALID(qname._pszURI)
					&&	!::strcmp((const char*)pxn->ns->href, qname._pszURI))))
		{
			// Advance to the next path component (if any) and descend into the node's children (if necessary)
			iPath += 1;
			if (iPath < cNames)
			{
				pxn = pxn->children;
			}
		}

		// Otherwise, skip over to the next sibling element node
		else
		{
			for (pxn = pxn->next; VALID(pxn) && pxn->type != XML_ELEMENT_NODE; pxn = pxn->next);
		}
	}

	return pxn;
}

/*
 * Function: countSiblings
 *
 */
int
XMLDocument::countSiblings(xmlNodePtr pxn, const char* pszSiblingTag, const char* pszSiblingNS)
{
	ENTER(XML,countSiblings);
	int cSiblings = 0;

	for (; VALID(pxn); pxn=pxn->next)
	{
		// Search through all siblings for elements with the passed tag and namespace
		// The clause attempts to say:
		//	 Count the node if
		//	   -- The sibling node is an element node
		//	   -- And the name matches (or no name was given)
		//	   -- And the namespace matches (or no namespace was given)
		if (	pxn->type == XML_ELEMENT_NODE
		   &&	(	!VALID(pszSiblingTag)
				||	(	!::strcmp((const char*)pxn->name, pszSiblingTag)
					&&	(	!VALID(pszSiblingNS)
						||	(	VALID(pxn->ns)
							&&	!::strcmp((const char*)pxn->ns->href, pszSiblingNS))))))
		{
			cSiblings++;
		}
	}

	return cSiblings;
}

/*
 * Function: createXPathContext
 *
 */
xmlXPathContextPtr
XMLDocument::createXPathContext()
{
	ENTER(XML,createXPathContext);
	
	ASSERT(VALIDSP(_spxd));

	xmlXPathContextSPtr spxpc;

	spxpc.reset(::xmlXPathNewContext(_spxd.get()));
	if (!VALIDSP(spxpc))
		THROWXML();

	if (::xmlXPathRegisterNs(spxpc.get(), (const xmlChar *)s_szStylusPrefix, (const xmlChar *)_pszStylusNamespace))
		THROWXML();

	return spxpc.release();
}

/*
 * Function: evalXPath
 *
 */
xmlXPathObjectPtr
XMLDocument::evalXPath(xmlXPathContextPtr pxpc, const char* pszExpression)
{
	ENTER(XML,evalXPath);
	
	ASSERT(VALID(pxpc));
	ASSERT(VALID(pszExpression));

	xmlXPathObjectSPtr spxpo(::xmlXPathEvalExpression((const xmlChar *)pszExpression, pxpc));

	return spxpo.release();
}

xmlXPathObjectPtr
XMLDocument::evalXPath(const char* pszExpression)
{
	ENTER(XML,evalXPath);
	
	ASSERT(VALIDSP(_spxd));
	ASSERT(VALID(pszExpression));

	xmlXPathContextSPtr spxpc(createXPathContext());
	xmlXPathObjectSPtr spxpo(evalXPath(spxpc.get(), pszExpression));

	return spxpo.release();
}

//--------------------------------------------------------------------------------
//
//	XMLStream
//
//--------------------------------------------------------------------------------

/*
 * Function: initialize
 *
 */
void
XMLStream::initialize(bool fInitialize)
{
	_ostr
		<< std::fixed
		<< std::showpoint
		<< std::setprecision(numeric_limits<UNIT>::digits10)
		<< std::boolalpha;
		
	if (fInitialize)
		_ostr
			<<	Constants::s_strXMLDOCTYPE
			<< e`ndl;
}
