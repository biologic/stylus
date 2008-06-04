/*******************************************************************************
 * \file    global.hpp
 * \brief   Global Stylus methods
 *
 * These methods are those globally accessible to all Stylus code (beyond
 * those described in stylus.h).
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

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

//-------------------------------------------------------------------------------
// General purpose macros
//
//-------------------------------------------------------------------------------
#define ARRAY_LENGTH(x)			((size_t)(sizeof(x)/sizeof(x[0])))
#define ARRAY_SIZE(x)			((size_t)sizeof(x))
#define CCH_OF(x)				((size_t)(sizeof((x))/sizeof((x[0]))))
#define SZ_OF(x)				(CCH_OF(x)-1)
#define VALID(x)				(x != NULL)
#define VALIDSP(x)				(VALID(x.get()))
#define EMPTYSZ(x)				(::strlen(x) <= 0)
#define EMPTYSTR(x)				(x.length() <= 0)
#define IFVALIDSET(x,y)			{ if (VALID(y)) x = y; }

#define ST_STRINGIFY(x)			ST_STRINGIFY_HELPER(x)
#define ST_STRINGIFY_HELPER(x)	#x

#define ST_FILELINE				__FILE__":"ST_STRINGIFY(__LINE__)

//-------------------------------------------------------------------------------
// Logging and Tracing
//
// Notes:
// - The argument passed to any of the LOGxxx macros must be enclosed in parentheses
//   and contain as its first value the corresponding LLxxx macro (see examples)
// - The final argument passed to the TRACEIF macro must be the same as that passed
//   to LOGTRACE (i.e., (LLTRACE, "format string", ...))
// 
// Examples:
//     LOGINFO((LLINFO, "My Data: %s %d", "Some data", 42));
//     LOGATRATE((LLRATE, "Print this every log rate trials: %s", "Hello"), _iTrial);
//     TRACEIF(GENOME,DATA,L2,(LLTRACE, "My trace data: %s, %d", "Some data", 42));
//     TDATA(GENOME,L2,(LLTRACE, "My trace data: %s, %d", "Some data", 42));
//-------------------------------------------------------------------------------
#define	LLERROR					STLL_ERROR, ST_FILELINE
#define LOGERROR(x)				{ if (org_biologicinstitute_stylus::Globals::logIf(STLL_ERROR)) { (org_biologicinstitute_stylus::Globals::log x ); } }

#define	LLWARNING				STLL_WARNING, ST_FILELINE
#define LOGWARNING(x)			{ if (org_biologicinstitute_stylus::Globals::logIf(STLL_WARNING)) { (org_biologicinstitute_stylus::Globals::log x ); } }

#define	LLINFO					STLL_INFO, ST_FILELINE
#define LOGINFO(x)				{ if (org_biologicinstitute_stylus::Globals::logIf(STLL_INFO)) { (org_biologicinstitute_stylus::Globals::log x ); } }

#define LLRATE					STLL_INFO, ST_FILELINE
#define LOGATRATE(x,t)			{ if (org_biologicinstitute_stylus::Globals::logIfRate(STLL_INFO,t)) { (org_biologicinstitute_stylus::Globals::log x ); } }

#define	LLDEBUG					STLL_DEBUG, ST_FILELINE
#define LOGDEBUG(x)				{ if (org_biologicinstitute_stylus::Globals::logIf(STLL_DEBUG)) { (org_biologicinstitute_stylus::Globals::log x ); } }

#define	LLTRACE					STLL_TRACE, ST_FILELINE
#define LOGTRACE(x)				{ if (org_biologicinstitute_stylus::Globals::logIf(STLL_TRACE)) { (org_biologicinstitute_stylus::Globals::log x ); } }

#ifdef ST_TRACE
#define TRACEIF(tr,tc,tl,x)		{ if (org_biologicinstitute_stylus::Globals::traceIf(STTR_##tr, STTC_##tc, STTL_##tl)) LOGTRACE(x); }
#define TRACEDOIF(tr,tc,tl,x)	{ if (org_biologicinstitute_stylus::Globals::traceIf(STTR_##tr, STTC_##tc, STTL_##tl)) { x; } }
#define TRACEFLOW(tr,x)			CFlow cf(STTR_##tr, ST_FILELINE, (x))
#define TDATA(tr,tl,x)			TRACEIF(tr,DATA,tl,x)
#define TFLOW(tr,tl,x)			TRACEIF(tr,FLOW,tl,x)
#else
#define TRACEIF(tr,tc,tl,x)
#define TRACEDOIF(tr,tc,tl,x)
#define TRACEFLOW(tr,x)
#define TDATA(tr,tl,x)
#define TFLOW(tr,tl,x)
#endif

#ifdef ST_DEBUG
#define IFDEBUG(x)				do { x; } while(0)
#else
#define IFDEBUG(x)
#endif

#ifdef ST_ASSERT
#define ASSERT(exp)				{ if (!(exp)) LOGDEBUG((LLDEBUG, "ASSERT: " ST_STRINGIFY(exp))); }
#ifdef ST_DEBUG
#define	DASSERT(exp)			ASSERT(exp)
#else
#define	DASSERT(exp)
#endif
#else
#define	ASSERT(exp)
#define	DASSERT(exp)
#endif

//-------------------------------------------------------------------------------
// Exception macros
//
// Notes:
// - These macros work similar to the LOGxxx macros above; the argument to THROWRC
//   must be enclosed in parentheses and contain, as its first value, the RC
//   macro passed the return code as the argument
// - The return code should only contain the unique part; e.g., BADARGUMENTS
//   instead of ST_RCBADARGUMENTS
// - No code should invoke throw directly, but always through Error::safeThrow
//   (to protect against nested exceptions)
//
// Example:
//     THROWRC((RC(BADARGUMENTS), "My Error Format: %s", "Some data"));
//-------------------------------------------------------------------------------
#define	RC(x)					ST_FILELINE,ST_RC##x
#define THROWRC(x)				Error::safeThrow(Error::setNextError x)
#define THROWIFERROR(x)			{ ST_RETCODE rc = (x); if (ST_ISERROR(rc)) Error::safeThrow(Error::setNextError(ST_FILELINE,rc,ST_STRINGIFY(x))); }
#define THROWERROR()			THROWRC((RC(ST_RCERROR), Constants::s_strEMPTY.c_str()))

#define THROWXML()				Error::safeThrow(XMLDocument::setNextError(ST_FILELINE))

//-------------------------------------------------------------------------------
// Enter/Exit helper macros
// - Code should not use these directly, but through the Enter/Exit macros below
//-------------------------------------------------------------------------------
#define PUBLICOPENTRY()			try {
#define PUBLICCLOSETRY()		} catch(const Error* pe)					\
{																			\
	rc = pe->getRetcode();													\
} catch(std::exception& e)													\
{																			\
	rc = ST_RCERROR;														\
	Error::setNextError(ST_FILELINE, rc, e.what());							\
} catch(...)																\
{																			\
	rc = ST_RCERROR;														\
	Error::setNextError(ST_FILELINE, rc, Constants::s_strUNKNOWNEXCEPTION.c_str()); \
}

//-------------------------------------------------------------------------------
// Enter/Exit macros
// - All routines should begin and end with one of the enter/exit macros
// - xxxxPUBLIC versions are for use by publicly accessbile code (i.e., defined in stylus.h);
//   all other code should use the non-PUBLIC versions
// - All public routines (defined in stylus.h) must return ST_RETCODE; all other
//   code must throw exceptions
//-------------------------------------------------------------------------------
#define ENTER(tp,x)				TRACEFLOW(tp, ST_STRINGIFY(x))

#define ENTERPUBLIC(tp,x)		ST_RETCODE rc = ST_RCERROR; ENTER(tp,x); PUBLICOPENTRY()
#define EXITPUBLIC(tp,x)		PUBLICCLOSETRY(); EXIT: return rc

#define RETURN_RC(x)				\
{									\
	rc = ST_RC##x;					\
	if (ST_ISERROR(rc))				\
	{								\
		if (ST_ISTYPEERROR(rc))		\
			LOGERROR((LLERROR,Constants::s_strRETCODE.c_str(),ST_RCCODE(rc)));		\
		if (ST_ISTYPEWARNING(rc))	\
			LOGWARNING((LLWARNING,Constants::s_strRETCODE.c_str(),ST_RCCODE(rc)));	\
		if (ST_ISTYPEINFO(rc))		\
			LOGINFO((LLINFO,Constants::s_strRETCODE.c_str(),ST_RCCODE(rc)));		\
	}								\
	goto EXIT;						\
}
#define RETURN_SUCCESS()		{ RETURN_RC(SUCCESS); }
#define RETURN_ERROR()			{ RETURN_RC(ERROR); }
#define RETURN_BADARGS()		{ RETURN_RC(BADARGUMENTS); }
#define RETURN_IFERROR(x)		{ rc = (x); if (ST_ISERROR(rc)) goto EXIT; }
#define	RETURN_NOTINITIALIZED()	{ if (!Globals::isInitialized()) RETURN_RC(NOTINITIALIZED); }
#define RETURN_IFDEAD()			{ if (Genome::isState(STGS_DEAD)) RETURN_RC(INVALIDSTATE); }

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Helper methods for copying strings and bytes
	 *
	 */
	//{@
	ST_RETCODE copyString(char* pszTarget, size_t* pcchTarget, const char* pszSource) throw();
	ST_RETCODE copyBytes(char* pbTarget, size_t* pcbTarget, const char* pbSource, size_t cbSource) throw();
	size_t copyFileline(char* pszBuffer, size_t cbRemaining, const char* pszFileline) throw();
	//@}

	/**
	 * \brief Helper methods for working with whitespace
	 *
	 */
	//{@
	char* trimWhitespace(char* psz) throw();
	char* purgeWhitespace(char* psz) throw();
	//@}

	/**
	 * \brief Methods for working with time values
	 *
	 */
	//{@
	/**
	 * \brief Retrieve the current GMT time in microseconds since January 1, 1970
	 */
	void timeNow(utime* pt) throw();

	/**
	 * \brief Convert the passed GMT time to a standard string format
	 * \param[in] pch Pointer to target buffer
	 * \param[in] pt Pointer to time to format
	 * \param[in] fLong Long/short seconds flag (XML Schema requires short form)
	 * \returns Number of bytes written
	 * \remarks
	 * - The returned string is \em not null terminated
	 * - The target buffer is assumed long enough (s_cchTIME)
	 */
	size_t timeToString(char* psz, const utime* pt, bool fLong = true, bool fTerminate = false) throw();
	//@}

	/**
	 * \brief Ensure a path string terminates with a slash character
	 * \param[in] strPath Path to inspect and, possibly, alter
	 * \returns Reference to the passed string
	 */
	std::string& terminatePath(std::string& strPath);
	
	/**
	 * \brief Ensure the passed string contains only valid bases
	 * \param[in] pbBases Pointer string of bases
	 * \param[in] cbBases Number of passed bases
	 */
	bool validateBases(const char* pbBases, size_t cbBases);

	/**
	 * \brief Shared indexes into score exponent and weight arrays
	 * 
	 * Scores components are the elements of a score including the component value
	 * and its applied weighting. Genes and Groups share values with the Group
	 * values overlapping and reusing one index.
	 */
	enum SCORECOMPONENT
	{
		// Genome-level values
		SC_FIXEDCOST = 0,
		SC_COSTPERBASE,
		SC_COSTPERUNIT,
		SC_GENOMEMAX = SC_COSTPERUNIT + 1,

		// Values common to the Gene and Groups
		SC_SCALE = 0,
		SC_PLACEMENT,
		SC_ILLEGALOVERLAPS,
		SC_MISSINGOVERLAPS,
		SC_COMMONMAX = SC_MISSINGOVERLAPS,

		// Gene-specific values
		SC_MARKS = SC_COMMONMAX + 1,
		SC_GENEMAX,

		// Group-specific values
		SC_DEVIATION = SC_COMMONMAX + 1,
		SC_EXTRALENGTH,
		SC_DROPOUTS,
		SC_GROUPMAX
	};
	
	/**
	 * \brief Different group scoring modes allowed
	 * 
	 */
	enum GROUPSCOREMODE
	{
		GSM_AVERAGE = 0,			///< Take the average group score at the gene level
		GSM_MINIMUM,				///< Take the minimum group score at the gene level (default)
		
		GSM_MAX
	};
	
	/**
	 * \brief All global variables
	 *
	 */
	class Globals
	{
		friend class TIndent;
		
	public:
		static const char s_szBuild[];
		static const char s_szVersion[];
		
		static bool isInitialized();
		static void setInitialized(bool fInitialized);

		static void load(const char* pxmlGlobals);
		static void toXML(XMLStream& xs);
		static bool isSupplied();

		/**
		 * \brief Log related methods
		 */
		//{@
		/// Determine if a log message should be written
		static bool logIf(ST_LOGLEVEL ll) throw();
		static bool logIfRate(ST_LOGLEVEL ll, size_t iTrial) throw();

		static void setLogLevel(ST_LOGLEVEL ll);
		static ST_LOGLEVEL getLogLevel();

		static void setLogOptions(STFLAGS grfLogOptions);
		static STFLAGS getLogOptions();

		static void setLogRate(size_t cLogRate);
		static size_t getLogRate();

		/// Write an entry to the log
		/// \param[in] ll Loglevel of the message
		/// \param[in] pszFileline Originating location of the message
		/// \param[in] pszFormat Format string (for use in vsnprintf)
		/// \param[in] ... Optional arguments referenced in pszFormat
		static void log(ST_LOGLEVEL ll, const char* pszFileline, const char* pszFormat, ...) throw();
		//@}

		/**
		 * \brief Trace related methods
		 * \remarks
		 * - Trace messages are log messages with a loglevel of STLL_TRACE
		 */
		//{@
		/// Set/check active trace regions
		static void setTraceRegions(STFLAGS grfTR);
		static STFLAGS getTraceRegions();

		/// Set/check trace levels
		static void setTraceLevel(ST_TRACECATEGORY tc, ST_TRACELEVEL tl);
		static ST_TRACELEVEL getTraceLevel(ST_TRACECATEGORY tc);

		static size_t getTraceIndents();

		static void setTraceTrial(size_t iTrial);
		static size_t getTraceTrial();
		static void setTraceAttempt(size_t iAttempt);
		static size_t getTraceAttempt();
		static void enableTracing(size_t cTrial, size_t cAttempts);

		/// Determine if a trace message should be written
		static bool traceIf(ST_TRACEREGION tr, ST_TRACECATEGORY tc, ST_TRACELEVEL tl) throw();

		/// Increase trace indentation
		static void traceIn() throw();

		/// Decrease trace indentation
		static void traceOut() throw();
		///@}
		
		static bool isGroupScoreMode(GROUPSCOREMODE gsm);
		
		static UNIT getGenomeWeight(SCORECOMPONENT sc);

		static UNIT getGeneWeight(SCORECOMPONENT sc);
		static UNIT getGeneSetpoint(SCORECOMPONENT sc);
		
		static UNIT getGroupWeight(SCORECOMPONENT sc);
		static UNIT getGroupSetpoint(SCORECOMPONENT sc);

	private:
		static bool _fInitialized;
		static bool _fSupplied;
		
		static ST_LOGLEVEL _ll;
		static STFLAGS _grfLogOptions;
		static size_t _cLogRate;

		static STFLAGS _grfTR;
		static ST_TRACELEVEL _tl[STTC_MAX];
		static size_t _cTraceindents;
		static size_t _iTrialTrace;				///< Trial at which to begin tracing
		static size_t _iAttemptTrace;			///< Trial attempt at which to begin tracing
		static bool _fAtTraceTrialOrAttempt;

		static STFLAGS _grfTF;

		static GROUPSCOREMODE _gsm;
		
		static Unit _aryGenomeWeights[SC_GENOMEMAX];
		
		static Unit _aryGeneWeights[SC_GENEMAX];
		static Unit _aryGeneSetpoints[SC_GENEMAX];

		static Unit _aryGroupWeights[SC_GROUPMAX];
		static Unit _aryGroupSetpoints[SC_GROUPMAX];
	};
		
	/**
	 * \brief Class used to trace routine enter/exit
	 *
	 */
	class CFlow
	{
	public:
		CFlow(ST_TRACEREGION tr, const char* pszFileline, const char* pszWhere) throw();
		~CFlow() throw();

	private:
		const ST_TRACEREGION _tr;
		const char* _pszFileline;
		const char* _pszWhere;
	};

#ifdef ST_TRACE
	/**
	 * \brief Class used to indent trace statements
	 *
	 */
	class TIndent
	{
	public:
		TIndent(const char* pszScope) throw();
		~TIndent() throw();

	private:
		const char* _pszScope;
	};
#define ENTERTRACESCOPE(x)	TIndent ti(ST_STRINGIFY(x))
#else
#define ENTERTRACESCOPE(x)
#endif

}	// namespace org_biologicinstitute_stylus
#endif // GLOBAL_HPP
