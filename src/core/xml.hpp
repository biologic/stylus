/*******************************************************************************
 * \file    xml.hpp
 * \brief   XML helper routines
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

#ifndef XML_HPP
#define XML_HPP

namespace org_biologicinstitute_stylus
{
#define ISEQUALXMLSTR(s1,s2) (::xmlStrlen((const xmlChar*)s1) == ::xmlStrlen((const xmlChar*)s2) && !::xmlStrcmp((const xmlChar*)s1,(const xmlChar*)s2))
	
	/**
	 * \brief Wrapper around ::xmlNanoHTTPClose (to hide dealing with void*)
	 * \param _pv Pointer to HTTP context to close
	 */
	void xmlHTTPClose(unsigned char* _pv) throw();

	typedef smart_ptr<unsigned char, xmlHTTPClose> xmlHTTPContextSPtr;

	/**
	 * \brief Wrapper around ::xmlFree (which is not a true function)
	 * \param _pv Pointer to object to be freed
	 */
	void xmlFreeChar(xmlChar* _pv) throw();

	typedef smart_ptr<xmlChar, xmlFreeChar> xmlCharSPtr;
	typedef smart_ptr<xmlDoc, xmlFreeDoc> xmlDocSPtr;
	typedef smart_ptr<xmlNode, xmlFreeNode> xmlNodeSPtr;
	typedef smart_ptr<xmlNode, xmlFreeNodeList> xmlNodeListSPtr;
	typedef smart_ptr<xmlAttr, xmlFreePropList> xmlAttrSPtr;
	typedef smart_ptr<xmlURI, xmlFreeURI> xmlURISPtr;
	typedef smart_ptr<xmlParserCtxt, xmlFreeParserCtxt> xmlParserCtxtSPtr;
	typedef smart_ptr<xmlSchema, xmlSchemaFree> xmlSchemaSPtr;
	typedef smart_ptr<xmlSchemaParserCtxt, xmlSchemaFreeParserCtxt> xmlSchemaParserCtxtSPtr;
	typedef smart_ptr<xmlSchemaValidCtxt, xmlSchemaFreeValidCtxt> xmlSchemaValidCtxtSPtr;
	typedef smart_ptr<xmlXPathContext, xmlXPathFreeContext> xmlXPathContextSPtr;
	typedef smart_ptr<xmlXPathObject, xmlXPathFreeObject> xmlXPathObjectSPtr;

	/**
	 * \brief Describe a single Schema
	 *
	 */
	struct SCHEMA
	{
		const char* _pszURI;		///< XML Namespace URI
		const char* _pszFilename;	///< Default filename
		xmlSchemaPtr _pxsSchema;	///< Pointer to parsed schema
		bool _fRequired;			///< Schema must successfully load
	};

	/**
	 * \brief Describes a single XML namespace pair
	 */
	struct Namespace
	{
		const char* _pszPrefix;		///< XML Namespace identifier
		const char* _pszURI;		///< XML Namespace URI
	};

	/**
	 * \brief Structure for holding an XML qualified name
	 *
	 */
	struct QNAME
	{
		const char*	_pszName;		///< Element name
		const char* _pszURI;		///< Namespace URI
	};

	/**
	 * \brief Enumeration of known XML tags, attribute names, and values
	 * 
	 */
	enum XMLTAG
	{
		XT_ACCEPTED = 0,
		XT_ACCEPTEDMUTATIONS,
		XT_ACCUMULATEMUTATIONS,
		XT_ANCESTORS,
		XT_ATTEMPT,
		XT_ATTEMPTED,
		XT_ATTEMPTS,
		XT_AUTHOR,
		XT_AVERAGE,
		XT_BASEFIRST,
		XT_BASELAST,
		XT_BASES,
		XT_BASESAFTER,
		XT_BOTTOM,
		XT_BOUNDS,
		XT_CHANGE,
		XT_CHANGED,
		XT_CHANGES,
		XT_CODON,
		XT_CODONTABLE,
		XT_COHERENT,
		XT_CONTAINEDSTROKES,
		XT_COPIED,
		XT_COPIES,
		XT_COPY,
		XT_CORRESPONDSTO,
		XT_COST,
		XT_COSTCONDITION,
		XT_COSTPERBASE,
		XT_COSTPERUNIT,
		XT_COSTRANGE,
		XT_COUNT,
		XT_COUNTBASES,
		XT_COUNTROLLBACKS,
		XT_CREATIONDATE,
		XT_CREATIONPARAMETER,
		XT_CREATIONTOOL,
		XT_DECREASE,
		XT_DELETE,
		XT_DELETED,
		XT_DELETIONS,
		XT_DELTAINDEX,
		XT_DESCRIPTION,
		XT_DEVIATION,
		XT_DROPOUTS,
		XT_DURATIONCONDITION,
		XT_DXTOHAN,
		XT_DYTOHAN,
		XT_EXTRALENGTH,
		XT_ENSUREINFRAME,
		XT_ENSUREWHOLECODONS,
		XT_ENTRY,
		XT_FACTOR,
		XT_FIRSTSTROKE,
		XT_FITNESS,
		XT_FITNESSCONDITION,
		XT_FITNESSRANGE,
		XT_FIXEDCOST,
		XT_FORWARD,
		XT_FRACTIONALDISTANCE,
		XT_GENE,
		XT_GENERANGE,
		XT_GENES,
		XT_GENOME,
		XT_GLOBALS,
		XT_GROUP,
		XT_GROUPS,
		XT_GROUPSCOREMODE,
		XT_HANDEFINITION,
		XT_HANREFERENCE,
		XT_HANREFERENCES,
		XT_HANSTROKERANGE,
		XT_HEIGHT,
		XT_HISTORY,
		XT_ILLEGALOVERLAPS,
		XT_INCREASE,
		XT_INDEXRANGE,
		XT_INFINITE,
		XT_INSERT,
		XT_INSERTED,
		XT_INSERTIONS,
		XT_LEFT,
		XT_LENGTH,
		XT_LIKELIHOOD,
		XT_LINEAGE,
		XT_MAINTAIN,
		XT_MARKS,
		XT_MAXIMUM,
		XT_MINIMUM,
		XT_MINIMUMSTROKELENGTH,
		XT_MISSINGOVERLAPS,
		XT_MODE,
		XT_MUTATIONCONDITION,
		XT_MUTATIONS,
		XT_MUTATIONSPERATTEMPT,
		XT_NONE,
		XT_OPTIONS,
		XT_ORIGIN,
		XT_OVERLAP,
		XT_OVERLAPS,
		XT_PLACEMENT,
		XT_PLAN,
		XT_POINT,
		XT_POINTS,
		XT_PRESERVEGENES,
		XT_PROCESSORID,
		XT_REASONCODE,
		XT_REJECTEDMUTATIONS,
		XT_REJECTSILENT,
		XT_REQUIRED,
		XT_REVERSE,
		XT_RIGHT,
		XT_ROLLBACKCONDITION,
		XT_ROLLBACKLIMIT,
		XT_ROLLBACKS,
		XT_SCALE,
		XT_SCORE,
		XT_SCORECONDITION,
		XT_SCORERANGE,
		XT_SECONDSTROKE,
		XT_SEED,
		XT_SEGMENT,
		XT_SEGMENTS,
		XT_SILENT,
		XT_SOURCEINDEX,
		XT_SPDEVIATION,
		XT_SPDROPOUTS,
		XT_SPEXTRALENGTH,
		XT_SPILLEGALOVERLAPS,
		XT_SPMARKS,
		XT_SPMISSINGOVERLAPS,
		XT_SPPLACEMENT,
		XT_SPSCALE,
		XT_STATISTICS,
		XT_STEP,
		XT_STEPS,
		XT_STRAIN,
		XT_STROKE,
		XT_STROKES,
		XT_SXTOHAN,
		XT_SYTOHAN,
		XT_TANDEM,
		XT_TARGETINDEX,
		XT_TERMINATION,
		XT_TERMINATIONCODE,
		XT_TERMINATIONCONDITIONS,
		XT_THRESHOLD,
		XT_THRESHOLDCONDITION,
		XT_TOP,
		XT_TRANSLATION,
		XT_TRANSPOSE,
		XT_TRANSPOSED,
		XT_TRANSPOSITIONS,
		XT_TRANSVERSIONLIKELIHOOD,
		XT_TRIAL,
		XT_TRIALATTEMPTS,
		XT_TRIALCONDITIONS,
		XT_TRIALFIRST,
		XT_TRIALLAST,
		XT_TRIALS,
		XT_TRIALSTART,
		XT_UNICODE,
		XT_UNITS,
		XT_UNITSRANGE,
		XT_UUID,
		XT_VALUE,
		XT_VECTOR,
		XT_WEIGHTEDCENTER,
		XT_WEIGHTS,
		XT_WIDTH,
		XT_X,
		XT_XMIDPOINT,
		XT_XMLNS,
		XT_Y,
		XT_YMIDPOINT,

		XT_UNKNOWNVALUE,
		XT_MAX
	};


	/**
	 * \brief Enumeration of known XPath statements
	 * 
	 */
	enum XMLXPATH
	{
		XP_GENOME = 0,
		XP_SEED,
		XP_CODONTABLE,
		XP_ENTRY,
		XP_BASES,
		XP_LINEAGE,
		XP_STATISTICS,
		XP_GENE,
		XP_ORIGIN,
		XP_HANREFERENCE,
		XP_STROKE,

		XP_HANDEFINITION,
		XP_GROUPS,
		XP_STROKES,
		XP_OVERLAPS,
		XP_FORWARDPOINTS,
		XP_REVERSEPOINTS,

		XP_OPTIONS,
		XP_TERMINATIONDURATION,
		XP_TERMINATIONROLLBACK,
		XP_TERMINATIONFITNESS,
		XP_STEPS,
		XP_TRIALCONDITIONS,
		XP_TRIALCOST,
		XP_TRIALFITNESS,
		XP_TRIALSCORE,
		XP_TRIALMUTATION,
		XP_VALUE,
		XP_MUTATIONSPERATTEMPT,
		XP_MUTATIONS,

		XP_GLOBALS,
		XP_WEIGHTGENOME,
		XP_WEIGHTGENE,
		XP_WEIGHTGROUP,

		XP_MAX
	};

	/**
	 * \brief Helper routines for obtaining XML tags, attribute names, and values
	 * 
	 */
	static const char* xmlTag(XMLTAG xt);

	/**
	 * \brief Helper routines for obtaining XPath statements
	 * 
	 */
	static const char* xmlXPath(XMLXPATH xp);

	/**
	 * \brief XMLDocument
	 *
	 * \remarks
	 * - Routines taking a node pointer (xmlNodePtr) must be given pointers to node residing
	 *   within the same document
	 */
	class XMLDocument
	{
		friend const char* xmlTag(XMLTAG xt);
		friend const char* xmlXPath(XMLXPATH xp);
		
	public:
		static const char XMLDocument::s_szStylusPrefix[];
		static const char s_szStylusNamespacePrerelease[];
		static const char s_szStylusNamespace1_0[];
		static const char s_szStylusNamespace1_3[];
		static const char* s_szStylusNamespace;

		static bool isXMLTrue(const std::string& str);
		static bool isXMLTag(xmlNodePtr pxn, const char* pszTag, const char* pszNS = NULL);
		
		/**
		 * \brief Return true or false if the passed xmlXPathObjectPtr refers to a successful evaluation
		 * \param[in] pxpo Pointer to an xmlXPathObject
		 * \param[in] nMaximumNodes Maximum number of nodes for success (defaults to the maximum integer value)
		 * \param[in] nMinimumNodes Minimum number of nodes for success (defaults to 1)
		 */
		static bool isXPathSuccess(xmlXPathObjectPtr pxpo, int nMaximumNodes = std::numeric_limits<int>::max(), int nMinimumNodes = 1);

		static void initialize();
		static void terminate();
		
		static void setScope(const char* pszScope);

		/**
		 * \brief Retrieve the XML error details and construct an Error from them
		 *
		 * This routine formats the XML error data and then creates (via Error::setNextError)
		 * an Error object.
		 */
		static const Error* setNextError(const char* pszFileline) throw();

		static XMLDocument* createInstance(const char* pszXML = NULL);
		static XMLDocument* createInstance(const std::string& strXML);
		static XMLDocument* createInstanceFromURL(const std::string& strURL);

		static void destroyInstance(XMLDocument* pxd) throw();

		std::string toString() const;
		
		/**
		 * \brief Return the Stylus namespace in use by the document
		 * 
		 */
		const char * getStylusNamespace();

		/**
		 * \brief Set/get the content of a node
		 * \param[in] pxn Pointer context node (in the current document)
		 */
		//{@
		void setContent(xmlNodePtr pxn, std::string& strText);
		bool getContent(xmlNodePtr pxn, std::string& str);
		//@}

		/**
		 * \brief Set/get/remove node attributes
		 * \param[in] pxn Pointer context node (in the current document)
		 */
		//{@
		void setAttribute(xmlNodePtr pxn, const char* pszAttribute, std::string& strValue);
		bool getAttribute(xmlNodePtr pxn, const char* pszAttribute, std::string& str);
		void clearAttributes(xmlNodePtr pxn);
		//@}

		/** 
		 * \brief Insert a new child text node under the current node
		 * \param[in] pxn Pointer XML node (in the current document)
		 * \param[in] pszText Text content for the new node
		 * \return Pointer to inserted text node
		 */
		//@{
		xmlNodePtr addText(xmlNodePtr pxn, const char* pszText);
		xmlNodePtr addText(xmlNodePtr pxn, std::string& strText);
		//@}

		/**
		 * \brief Create a new text node
		 * \param[in] pszText Text content for the new node
		 * \return Pointer to created text node
		 */
		//@{
		xmlNodePtr createText(const char* pszText = NULL);
		xmlNodePtr createText(std::string& strText);
		//@}

		/**
		 * \brief Return the first sibling element of the passed node (the node itself is also considered)
		 * \param[in] pxn Pointer context node (in the current document)
		 * \param[in] pszSiblingTag Tag of sibling element for which to scan (optional)
		 * \param[in] pszSiblingNS Namespace URI of the sibling element for which to scan (optional)
		 * \return Pointer to first sibling (NULL if no sibling elements exist or if the first fails to match)
		 */
		//@{
		xmlNodePtr getFirstSibling(xmlNodePtr pxn, const char* pszSiblingTag = NULL, const char* pszSiblingNS = NULL);
		xmlNodePtr getFirstSibling(xmlNodePtr pxn, std::string& strSiblingTag);
		xmlNodePtr getFirstSibling(xmlNodePtr pxn, std::string& strSiblingTag, std::string& strSiblingNS);
		xmlNodePtr getFirstSibling(xmlNodePtr pxn, std::string& strSiblingTag, const char* pszSiblingNS);
		//@}

		/**
		 * \brief Return the first child element under the current node
		 * \param[in] pxn Pointer context node (in the current document)
		 * \param[in] pszChildTag Tag of child element for which to scan (optional)
		 * \param[in] pszChildNS Namespace URI of the child element for which to scan (optional)
		 * \return Pointer to first child (NULL if no child elements exist or if the first fails to match)
		 */
		//@{
		xmlNodePtr getFirstChild(xmlNodePtr pxn, const char* pszChildTag = NULL, const char* pszChildNS = NULL);
		xmlNodePtr getFirstChild(xmlNodePtr pxn, std::string& strChildTag);
		xmlNodePtr getFirstChild(xmlNodePtr pxn, std::string& strChildTag, std::string& strChildNS);
		xmlNodePtr getFirstChild(xmlNodePtr pxn, std::string& strChildTag, const char* pszChildNS);
		//@}

		/**
		 * \brief Locate the descendent of the current node along the supplied path
		 * \param[in] pxn Pointer context node (in the current document)
		 * \param[in] vecQNames Vector of QNAMEs describing the path
		 * \return Pointer to located descendent if found; NULL otherwise
		 */
		xmlNodePtr findDescendent(xmlNodePtr pxn, const std::vector<QNAME>& vecQNames);

		/**
		 * \brief Return the number of siblings of the current node
		 * \param[in] pxn Pointer context node (in the current document)
		 * \param[in] pszSiblingTag Tag of sibling element for which to scan (optional)
		 * \return Number of siblings
		 */
		//@{
		int countSiblings(xmlNodePtr pxn, const char* pszSiblingTag = NULL, const char* pszSiblingNS = NULL);
		int countSiblings(xmlNodePtr pxn, std::string& strSiblingTag);
		int countSiblings(xmlNodePtr pxn, std::string& strSiblingTag, std::string& strSiblingNS);
		int countSiblings(xmlNodePtr pxn, std::string& strSiblingTag, const char* pszSiblingNS);
		//@}

		/**
		 * \brief Return the number of children under the current node
		 * \param[in] pxn Pointer context node (in the current document)
		 * \param[in] pszChildTag Tag of child element for which to scan (optional)
		 * \return Number of children
		 */
		//@{
		int countChildren(xmlNodePtr pxn, const char* pszChildTag, const char* pszChildNS);
		int countChildren(xmlNodePtr pxn, std::string& strChildTag);
		int countChildren(xmlNodePtr pxn, std::string& strChildTag, std::string& strChildNS);
		int countChildren(xmlNodePtr pxn, std::string& strChildTag, const char* pszChildNS);
		//@}

		/**
		 * \brief Create an XPath context (for reuse with multiple XPath expressions)
		 * \remarks
		 * - The context is created using the document's Stylus namespace
		 */
		xmlXPathContextPtr createXPathContext();

		/**
		 * \brief Evaluate an XPath expression against the passed XML document
		 * \param[in] pszExpression XPath expression to evaluate
		 * \param[in] aryNS Array of xmlNS structures of namespaces to register (optional)
		 * \param[in] cNS Number of entries in aryNS
		 * \return xmlXPathObjectPtr from expression evaluation (may be NULL)
		 */
		//@{
		xmlXPathObjectPtr evalXPath(xmlXPathContextPtr pxpc, const char* pszExpression);
		xmlXPathObjectPtr evalXPath(xmlXPathContextPtr pxpc, std::string& strExpression);
		xmlXPathObjectPtr evalXPath(const char* pszExpression);
		xmlXPathObjectPtr evalXPath(std::string& strExpression);
		//@}

	private:
		static SCHEMA s_arySCHEMAS[];

		static const char* s_aryXMLTAG[XT_MAX];
		static const char* s_aryXMLXPATH[XP_MAX];

		static std::string s_strScope;
		static char s_szXMLError[Constants::s_cbmaxBUFFER];

		/**
		 * \brief Initialization helper routines to load/free schemas
		 *
		 */
		//{@
		static void loadSchemas();
		static void freeSchemas();
		//@}

		/**
		 * \brief LibXML error handlers
		 *
		 */
		//{@
		static void handlerGenericError(void* pv, const char* pszError, ...);
		static void handlerStructuredError(void* pv, xmlErrorPtr pxe);
		//@}

		/**
		 * \brief Reset global internal error state
		 *
		 */
		static void clearErrors() throw();

		/**
		 * \brief Format an xmlError object into a string
		 *
		 */
		static size_t xmlErrorToString(xmlErrorPtr pxe, char* pszBuffer, size_t cchBuffer);
		
		XMLDocument(xmlDocPtr pxd);

		xmlDocSPtr _spxd;					///< Active XML document
		const char* _pszStylusNamespace;	///< Pointer to the Stylus namespace in use
	};

	typedef smart_ptr<XMLDocument, XMLDocument::destroyInstance> XMLDocumentSPtr;

	/**
	 * \brief This class eases constructing XML strings and files
	 *
	 */
	class XMLStream
	{
	public:
		XMLStream(std::ostream& ostr, bool fInitialize = true);

		/**
		 * \brief These methods write an XML start tag
		 * \param[in] pszTag The tag name
		 * \param[in] fWithContent The tag will or will not contain content
		 * \param[in] fWithNewline Add a newline after the tag
		 */
		//{@
		void writeStart(const char* pszTag, bool fWithContent = true, bool fWithNewline = true);
		void openStart(const char* pszTag);
		void closeStart(bool fWithContent = true, bool fWithNewline = true);
		//@}

		/**
		 * \brief Write an XML end tag
		 * \param[in] pszTag The tag name
		 * \param[in] fWithNewline Add a newline after the tag
		 */
		void writeEnd(const char* pszTag, bool fWithNewline = true);
		
		/**
		 * \brief Write an attribute to an open start tag
		 * \param[in] pszAttribute The attribute name
		 * \param[in] t The parameterized value to record for the attribute
		 */
		template<class T> void writeAttribute(const char* pszAttribute, T t);
		void writeAttribute(const char* pszAttribute, Unit& nValue);
		
		/**
		 * \brief Write free-form content to a tag body
		 * \param[in] t The parameterized value to record to the tag body
		 */
		template<class T> void writeContent(T t);
		void writeContent(Unit& nValue);
		
	private:
		std::ostream& _ostr;
		
		void initialize(bool fInitialize);
	};

	/**
	 * \brief A generic function that loads items from one or more XML nodes.
	 *
	 * The routine assumes the typename is defines a standard library container that
	 * supports a basic forward iterator. Further, it assumes the container is preloaded
	 * with the elements. This does not \em create the elements, rather it \em sets them
	 * through the \c load method.
	 *
	 * \param[in] t Reference to a standard library container preloaded with elements
	 * \param[in] pxd Pointer to active XMLDocument
	 * \param[in] cNodes Number of XML nodes (and container elements) from which to load
	 * \param[in] aryNodes Array of xmlNodePtr
	 */
	template<class T> void loadFromXML(T& t, XMLDocument* pxd, int cNodes, xmlNodePtr* aryNodes);

}	// namespace org_biologicinstitute_stylus
#endif // XML_HPP
