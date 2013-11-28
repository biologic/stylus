/*******************************************************************************
 * \file stylus.h
 * \brief Public Stylus methods.
 *
 * This file declares the public methods through which callers interact with the
 * Stylus engine. See Stylus documentation for full details.
 *
 * \page overview Stylus Overview
 * \par Stylus is an engine to load, mutate, and analyze \em genomes that
 * contain \em genes corresponding to one or more Han glyphs. (See Stylus
 * documentation for full details.) The engine is designed to execute as
 * a task within a grid-network or cluster with one task per-CPU.
 * \par Stylus holds at most one genome whose state is represents the
 * active \em trial. Stylus moves from one trial to the next through a
 * sequence of mutation and validation.
 *
 * \section using Using Stylus
 * \par Callers must always initialize Stylus (via stInitialize()) before calling
 * any other routines and terminate Stylus (see stTerminate()) when finished.
 * \par After initialization, callers load a genome which may then be in one
 * of several possible states (e.g., DEAD, ALIVE). Callers interact with
 * Stylus by executing plans that create a series of trials.
 *
 * \section output Stylus Output
 * \par Stylus records several types of data as it process: Log records,
 * containing status messages (and trace statements in DEBUG builds), is written
 * to STDERR. Stylus will record, if requested, the genome at the end of 
 * selected trials. Provided enough details is included, these may be used to
 * re-execute a subpart of a plan (e.g., trials 1000-2000) recording
 * additional detail.
 *
 * \section errors Error Handling
 * \par Internally, Stylus tends to use C++ exceptions. At the interface boundary
 * it converts all of these into a ST_RETCODE value. Since the meaning of
 * specific values may change, callers should rely on constants and macros Stylus
 * provides for evaluation. Among others, these include methods to check for
 * success, failure, and obtain the details of the last error.
 *
 * \section threads Thread Safety
 * \par Stylus is \em not thread-safe. Stylus is designed to be used as task within a
 * distributed grid. Nor does Stylus support mechanisms for even creating mutliple
 * genomes per process.
 *
 * \section memory Memory Handling
 * \par When Stylus needs to return to the caller data of indeterminate size (e.g.,
 * a genome), the caller must provide a pre-allocated buffer large enough to hold
 * the requested object. If the buffer is not large-enough, Stylus will return the
 * number of bytes necessary and no data. The caller should, then, allocate a
 * larger buffer and re-invoke Stylus.
 *
 * \section tracing Trace Control
 * \par Stylus can write extremely detailed messages covering both the internal
 * flow of control and data structures. Stylus divides itself into \em regions
 * (e.g., XML processing, mutation handling) and categorizes trace statements
 * as either \em flow or \em data records. Further, trace records have an
 * associated numeric level ranging from all records disabled to all
 * records written. Lastly, callers can disable tracing until a particular
 * trial to minimize 'noise.' Stylus writes trace records to the log file (i.e.,
 * STDERR). Stylus starts with all tracing disabled.
 *
 * \section misc Miscellaneous Notes
 * \par
 * - Stylus uses 0-based indexes (i.e., the first position is index 0)
 * - Stylus uses the C/C++ \c char type for all character data
 * - All XML data is null-terminated, UTF-8 encoded strings (passed as \c char)
 * - Stylus requires C callers to include <stdbool.h>
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

#ifndef STYLUS_H
#define STYLUS_H

#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * \brief Standard Stylus return code
	 */
	typedef unsigned long ST_RETCODE;

	/**
	 * \brief Base types for Stylus units
	 *
	 * The UNIT type is the floating-point type Stylus uses in all numeric calculations.
	 * 
	 * Stylus generates values via addition/subtraction and multiplication/division that should
	 * be equal, strictly less than, or strictly greater than. However, due to cumulative error
	 * inherent in floating point (see Knuth, Vol 1, 4.2), these conditions may not hold. Stylus
	 * considers values equivalent if they are sufficiently close to one another.
	 */
	typedef double UNIT;

	/**
	 * \brief A sequence of bitflags
	 *
	 */
	typedef unsigned long STFLAGS;

#define ST_ISALLSET(grf,f)	(((grf)&(f))==(f))
#define ST_ISANYSET(grf,f)	(((grf)&(f))!=0)
#define ST_ISCLEAR(grf,f)	(((grf)&(f))==0)

	/**
	 * \brief Public Stylus return codes
	 */
	//{@
	const ST_RETCODE ST_MASKTYPECODE		= 0xF0000000;					///< Mask for isolating error type
	const ST_RETCODE ST_MASKERRORCODE		= 0x0FFFFFFF;					///< Mask for isolating error code

	const ST_RETCODE ST_ERRORTYPE			= 0x10000000;
	const ST_RETCODE ST_WARNINGTYPE			= 0x20000000;
	const ST_RETCODE ST_INFOTYPE			= 0x40000000;

#define ST_ISTYPEERROR(rc)		((rc & ST_MASKTYPECODE) == ST_ERRORTYPE)
#define ST_ISTYPEINFO(rc)		((rc & ST_MASKTYPECODE) == ST_INFOTYPE)
#define ST_ISTYPEWARNING(rc)	((rc & ST_MASKTYPECODE) == ST_WARNINGTYPE)
#define ST_RCCODE(rc)			(rc & ST_MASKERRORCODE)

	const ST_RETCODE ST_RCSUCCESS			= 0x00000000;					///< Success code
	const ST_RETCODE ST_RCERROR				= (ST_ERRORTYPE | 0x00000001);	///< Generic error
	const ST_RETCODE ST_RCNOTINITIALIZED	= (ST_ERRORTYPE | 0x00000002);	///< Stylus has not been initialized
	const ST_RETCODE ST_RCBUFFERTOOSMALL	= (ST_INFOTYPE	| 0x00000003);	///< Incoming buffer is too small
	const ST_RETCODE ST_RCOUTOFMEMORY		= (ST_ERRORTYPE | 0x00000004);	///< Generic out-of-memory error
	const ST_RETCODE ST_RCBADARGUMENTS		= (ST_ERRORTYPE | 0x00000005);	///< Generic bad arguments error
	const ST_RETCODE ST_RCINEXECUTION		= (ST_ERRORTYPE | 0x00000006);	///< Illegal recursive call to genome
	const ST_RETCODE ST_RCXMLERROR			= (ST_ERRORTYPE | 0x00000007);	///< Error in XML/XSLT processing
	const ST_RETCODE ST_RCINVALIDSTATE		= (ST_ERRORTYPE | 0x00000008);	///< Call made during an invalid state
	const ST_RETCODE ST_RCPLAN				= (ST_INFOTYPE  | 0x00000009);	///< A plan failed to complete
	const ST_RETCODE ST_RCDEAD				= (ST_ERRORTYPE | 0x0000000A);	///< Genome is dead

#define ST_ISSUCCESS(x)			(ST_RCCODE(x) == ST_RCSUCCESS)
#define ST_ISERROR(x)			(ST_RCCODE(x) != ST_RCSUCCESS)
#define ST_ISRC(x,rc)			(ST_RCCODE(x) == ST_RCCODE(rc))
	//@}

	/**
	 * \brief Obtain the details of the last reported error
	 *
	 * This method returns a string containing additional details (e.g.,
	 * error originating location) for the last returned error.
	 * 
	 * \param[in] prc Last return code
	 * \param[in] pszDetails Buffer to hold error string
	 * \param[in,out] pcchDetails Length of output buffer on entry, number of
	 *							  characters copied or required on return
	 */
	ST_RETCODE stGetLastError(ST_RETCODE* prc, char* pszDetails, size_t* pcchDetails);

	/**
	 * \brief Obtain the description of the last reported error
	 *
	 * This method returns a string containing just the description
	 * for the last returned error.
	 * 
	 * \param[in] prc Last return code
	 * \param[in] pszDetails Buffer to hold error string
	 * \param[in,out] pcchDetails Length of output buffer on entry, number of
	 *							  characters copied or required on return
	 */
	ST_RETCODE stGetLastErrorDescription(ST_RETCODE* prc, char* pszDetails, size_t* pcchDetails);

	/**
	 * \brief Initialize Stylus
	 * 
	 * Stylus and its internal libraries require initialization.
	 * This routine \em must be called \em before any other.
	 */
	ST_RETCODE stInitialize();

	/**
	 * \brief Terminate Stylus
	 *
	 * Stylus and its internal libraries require an opporunity to release resources.
	 * This routine should be called just prior to process termination (or release
	 * of the Stylus shared library).
	 */
	ST_RETCODE stTerminate();

	/**
	 * \brief Load Stylus global values
	 * 
	 * Stylus relies on multiple internal constant values (such as scoring weights).
	 * This routine may be used to override the built-in values.
	 * 
	 * \param[in] pxmlConstants Pointer to the XML Globals document
	 */
	ST_RETCODE stSetGlobals(const char* pxmlGlobals);
	
	/**
	 * \brief Establish the Han and XML scopes for locating external files
	 * 
	 * \param[in] pszURLHan Pointer to the null terminated string containing the
	 *						URL from which Stylus will query for Han data
	 * \param[in] pszURLXMLSchema Pointer to URL to use for obtaining XML schemas
	 *
	 * \remarks
	 * - Stylus defaults to using "http://biologicinstitute.net/stylus"
	 * - Stylus recognizes 'file:' and 'http:' URL schemes
	 * - Stylus queries for Han data by appending to the passed URL path
	 *   two steps derived from the Han Unicode value: The thousands value
	 *   from the Unicode followed by the Unicode value itself with a '.han' suffix.
	 *   For example, given the Han URL of \c http://biologicinstitute.net/stylus/han,
	 *   Stylus will form the URL \c http://biologicinstitute.net/stylus/han/4000/4E00.han
	 *   when requesting the data for the Han character whose Unicode value is \c 4E00.
	 */
	ST_RETCODE stSetScope(const char* pszURLHan, const char* pszURLXMLSchema);

	/**
	 * \brief Retrieve, as a human-readable string, the Stylus version information
	 * 
	 * \param[in] pszVersion Pointer to \c char buffer in which to store the string
	 * \param[in,out] pcchVersion Length of output buffer on entry, number of
	 *							  characters copied or required on return
	 * \param[in] fAll If True, Stylus will include the versions of internal libraries as well
	 */
	ST_RETCODE stGetVersion(char* pszVersion, size_t* pcchVersion, bool fAll);

	/**
	 * \brief Log levels used within Stylus
	 * 
	 * Logging levels control the records written to the Stylus logfile.
	 * Higher levels include all lower levels as well. For example, \c INFO
	 * includes \c WARNING, and \c ERROR.
	 */
	typedef enum
	{
		STLL_ERROR	 = 1,
		STLL_WARNING = 2,
		STLL_INFO	 = 3,
		STLL_DEBUG	 = 4,
		STLL_TRACE	 = 5,
		STLL_ALL	 = 6
	} ST_LOGLEVEL;

	/**
	 * \brief Log options used by Stylus
	 *
	 */
	typedef enum
	{
		STLO_USESTDOUT   = 0x0001,	///< Write records to STDOUT (and STDERR)
		STLO_SHORTFORMAT = 0x0002	///< Use short log format (elide location)
	} ST_LOGOPTION;

	/**
	 * \brief Methods to set/get the Stylus logging level
	 */
	//@{
	ST_RETCODE stSetLogLevel(ST_LOGLEVEL ll);
	ST_RETCODE stGetLogLevel(ST_LOGLEVEL* pll);
	ST_RETCODE stSetLogOptions(STFLAGS grfLogOptions);
	ST_RETCODE stGetLogOptions(STFLAGS* pgrfLogOptions);
	ST_RETCODE stSetLogRate(size_t cLogRate);
	ST_RETCODE stGetLogRate(size_t* pcLogRate);
    ST_RETCODE stSetLogFile(const char * logFilename);
	//@}

	/**
	 * \brief Trace regions used within Stylus
	 *
	 * Trace regions control what regions within Stylus should write internal
	 * processing messages (trace statments). The available trace regions are:
	 *
	 * - Global boundary (the boundary into and out-of Stylus)
	 * - Genome handling (genome regions outside of mutation, measurement, and scoring)
	 * - Han handling
	 * - Mutation handling
	 * - Plan loading and execution
	 * - Measurment (e.g., calculating stroke length, stroke scale factors)
	 * - Scoring (evaluating the measured values against one or more Han glyphs).
	 * - XML processing
	 */
	typedef enum
	{
		STTR_NONE		 = 0x0000,	///< Disable all trace messages
		STTR_GLOBAL		 = 0x0001,	///< Write messages related to global methods
		STTR_GENOME		 = 0x0002,	///< Write messages related to the genome itself
		STTR_HAN		 = 0x0004,	///< Write messages related to handling Han glyphs
		STTR_MUTATION	 = 0x0008,	///< Write messages related to mutations
		STTR_PLAN		 = 0x0010,	///< Write messages related to plan loading and execution
		STTR_VALIDATION	 = 0x0020,	///< Write messages related to validation
		STTR_SCORING	 = 0x0040,	///< Write messages related to scoring
		STTR_XML		 = 0x0080,	///< Write messages related to XML processing

		STTR_ALL		 = STTR_GLOBAL
						| STTR_GENOME
						| STTR_HAN
						| STTR_MUTATION
						| STTR_PLAN
						| STTR_VALIDATION
						| STTR_SCORING
						| STTR_XML
	} ST_TRACEREGION;

	/**
	 * \brief Trace categories
	 *
	 * Trace categories control what gets traced within a region:
	 * - The Data category traces data structures and calculated results
	 * - The Flow category traces processing flow and major events
	 */
	typedef enum
	{
		STTC_DATA,					///< Write data-related messages
		STTC_FLOW,					///< Write flow-related messages

		STTC_MAX
	} ST_TRACECATEGORY;

	/**
	 * \brief Trace levels
	 *
	 * Trace levels control the detail Stylus records.
	 */
	typedef enum
	{
		STTL_NONE = 0,				///< Disable all trace records (default)
		STTL_L1	  = 1,				///< Minimum trace detail
		STTL_L2	  = 2,
		STTL_L3	  = 3,
		STTL_L4	  = 4,
		STTL_L5	  = 5,				///< Maximum trace detail

		STTL_MIN  = STTL_L1,
		STTL_STD  = STTL_L2,
		STTL_MAX  = STTL_L5
	} ST_TRACELEVEL;

	/**
	 * \brief Methods to set/get the Stylus tracing level
	 * \remarks
	 * - These commands take immediate effect. To delay them, \em first set the
	 *	 trace trial to some future trial, then set the trace points and levels.
	 */
	//@{
	ST_RETCODE stClearTraceRegions(STFLAGS grfRegions);		///< Clear one or more trace regions
	ST_RETCODE stEnableTraceRegions(STFLAGS grfRegions);	///< Enable one or more trace regions
	ST_RETCODE stSetTraceRegions(STFLAGS grfRegions);		///< Set trace regions
	ST_RETCODE stGetTraceRegions(STFLAGS* pgrfRegions);		///< Return the current trace regions

	ST_RETCODE stClearTraceLevels();						///< Clear all trace levels
	ST_RETCODE stSetTraceLevel(ST_TRACECATEGORY tc, ST_TRACELEVEL tl);		///< Set trace level
	ST_RETCODE stGetTraceLevel(ST_TRACECATEGORY tc, ST_TRACELEVEL* ptl);	///< Get trace level

	ST_RETCODE stSetTraceTrial(size_t iTrial);				///< Trial at which to begin tracing
	ST_RETCODE stGetTraceTrial(size_t* piTrial);			///< Return trial at which tracing begins

	ST_RETCODE stSetTraceAttempt(size_t iAttempt);			///< Trial attempt at which to begin tracing
	ST_RETCODE stGetTraceAttempt(size_t* piAttempt);		///< Return trial attempt at which tracing begins
	//@}

	/**
	 * \brief Level of detail to write when saving/recording the genome
	 *
	 */
	typedef enum
	{
		STRD_DIMENSIONS = 0x0001,	///< Record dimensional data
		STRD_GENES		= 0x0002,	///< Record gene-related data
		STRD_LINEAGE	= 0x0004,	///< Record genome lineage (e.g., mutations)
		STRD_LINEAGEALL = 0x0008,	///< Record all genome lineage details (e.g., failed mutations)
		STRD_SCORE		= 0x0010,	///< Record basic score details
		STRD_SCOREALL	= 0x0020,	///< Record all score details
		STRD_SEGMENTS	= 0x0040,	///< Record calculated segments and points
		STRD_STATISTICS = 0x0080,	///< Record genome statistics
        STRD_SEED       = 0x0100,

		STRD_NONE		= 0x0000,	///< Record nothing

		STRD_RESTART	= STRD_GENES
						| STRD_SCORE
                        | STRD_SEED,

		STRD_STANDARD	= STRD_GENES
						| STRD_LINEAGE
						| STRD_SCORE
						| STRD_SEGMENTS
                        | STRD_SEED,

		STRD_SUMMARY	= STRD_GENES
						| STRD_LINEAGE
						| STRD_SCORE
                        | STRD_SEED,

		STRD_ALL		= STRD_DIMENSIONS
						| STRD_GENES
						| STRD_LINEAGE
						| STRD_LINEAGEALL
						| STRD_SCORE
						| STRD_SCOREALL
						| STRD_SEGMENTS
						| STRD_STATISTICS
                        | STRD_SEED,

        STRD_ALL_NOSEED = STRD_DIMENSIONS
						| STRD_GENES
						| STRD_LINEAGE
						| STRD_LINEAGEALL
						| STRD_SCORE
						| STRD_SCOREALL
						| STRD_SEGMENTS
						| STRD_STATISTICS

	} ST_RECORDDETAIL;
#define DEFAULT_RECORDDETAIL STRD_STANDARD

	/**
	 * \brief Set the level, frequency, and target directory used when recording genomes
	 * 
	 * If provided with a directory, Stylus will record the genome at various intervals.
	 * Stylus will always record the initial and final genome. It will also record trials
	 * modulo the passed record rate (e.g., a record rate of 10 will record every 10th trial).
	 * 
	 * \param[in] cRecordRate Rate at which to record trials (0 records only initial and final)
	 * \param[in] grfRecordDetail Detail level to record
	 * \param[in] pszRecordDirectory Directory into which to record the trials (required for all but STRD_NONE)
	 * \param[in] fRecordHistory Write mutation history to history files (requires a record directory)
	 */
	ST_RETCODE stSetRecordRate(size_t cRecordRate, STFLAGS grfRecordDetail, const char* pszRecordDirectory, bool fRecordHistory);

	/**
	 * \brief Set the active genome
	 *
	 * A genome is the object on which Stylus operates and is built from an XML
	 * description. Stylus holds, at any one time, no more than one genome.
	 * Loading a genome replaces any previously loaded genome, including closing
	 * all associated files.
	 * 
	 * Loading the genome resets all internal state including all statistics.
	 * By default, the loaded genome is assigned to trial zero with mutations
	 * begining at trial one. However, if the loaded genome contains a
	 * statistics section, the trial is set to the highest trial associated with
	 * the genome (i.e., the trialLast attribute of the statistics element).
	 * Conceptually, callers should load the genome that is one trial before
	 * where mutations should begin (e.g., load the genome from trial ten to
	 * execute a plan producing trials eleven through twenty).
	 *
	 * \param[in] pxmlGenome Pointer to the XML genome document
	 * \param[in] pszAuthor Optional author name to save in output (may be NULL)
	 */
	ST_RETCODE stSetGenome(const char* pxmlGenome, const char* pszAuthor);

	/**
	 * \brief Retrieve the UTF-8 encoded XML document of the active genome
	 *
	 * \param[in] pchGenome Pointer to target buffer
	 * \param[in,out] pcchGenome Pointer to target buffer size on entry;
	 *							 number of bytes copied or required on return
	 * \param[in] grfRecordDetail Level of detail to include (see ST_RECORDDETAIL)
	 */
	ST_RETCODE stGetGenome(char* pxmlGenome, size_t* pcchGenome, STFLAGS grfRecordDetail);

	/**
	 * \brief Retrieve the bases in the current genome
	 *
	 */
	ST_RETCODE stGetGenomeBases(char* pbBases, size_t* pcbBases);

	/**
	 * \brief Status callback
	 *
	 * If supplied, Stylus will invoke a callback every requested number of
	 * trials. The callback code is allowed to interrogate Stylus for the active
	 * genome and change the logging or tracing values.The callback may
	 * return \c false to cancel the command execution.
	 */
	typedef bool (*ST_PFNSTATUS)();

	/**
	 * \brief Method to execute a plan on the active genome
	 * 
	 * A plans is a series of mutations applied to the genome that creates trials.
	 * See stylus.xsd for details on the XML format.
	 * 
	 * Plan execution always begins at the next trial; that is, the genome must be
	 * ALIVE prior to executing a plan and will be advanced before applying the first
	 * step of the plan.
	 * 
	 * By default, the first step of a plan is assumed to correspond with trial one;
	 * plan execution will skip steps up through that corresponding to the current
	 * trial. The caller may specify the trial to associate with the first step of
	 * a plan (this allows executing multiple plans in succession on a genome).
	 * Specifying zero for the initial trial will align the first step of the
	 * plan with the current state (with execution begining at the next trial).
	 * To rerun part of a plan (perhaps to record more detail), the caller should
	 * load the genome from the last trial before where they want to begin (e.g.,
	 * load trial ten to begin with trial eleven) and specify the trial that properly
	 * aligns the plan. (Most often, this is one, but if mutliple plans were executed,
	 * then it may be a trial greater than one and less than the current trial.)
	 * 
	 * Also, by default, execution will applying all steps within the plan. The
	 * caller may limit this by specifying a trial count that is less than the
	 * steps remaining in the plan. Specifying zero (or a value exceeding the steps
	 * remaining in the plan) will execute all remaining steps.
	 * 
	 * Lastly, the caller may receive intermittent feedback by supplying the address of a 
	 * callback function and a rate at which to be called. Stylus will invoke the callback
	 * function at the successful completion of every trial modulo the passed rate. The caller
	 * may stop plan execution by returing 'false' from the callback. From within the callback,
	 * the caller may retrieve statistics, adjust the record rate, logging, and/or tracing.
	 * 
	 * \param[in] pxmlPlan Pointer to XML plan to execute
	 * \param[in] iTrialFirst Genome trial to associate with the plan (0 selects the current trial plus one)
	 * \param[in] cTrials Number of trials to execute (0 executes all remaining trials)
	 * \param[in] pfnStatus Pointer to callback status function (may be NULL)
	 * \param[in] cStatusRate Rate at which to invoke the callback status function
	 * 
	 * Examples:
	 * - Fully execute a plan producing trial data every 10th trial
	 * \code
	 * ::stSetGenome(pxmlGenome);
	 * ::stSetRecordRate(100, STRD_STANDARD, "/Users/Sample/exp1");
	 * ::stExecutePlan(pxmlPlan, 0, 0, 10, STRD_STANDARD, "/Users/Sample/exp1", NULL, 0);
	 * \endcode
	 * 
	 * - Execute two plans, one after another, producing data every 100th trial
	 * \code
	 * ::stSetGenome(pxmlGenome);
	 * ::stSetRecordRate(100, STRD_STANDARD, "/Users/Sample/exp2");
	 * ::stExecutePlan(pxmlPlan1, 0, 0, NULL, 0);
	 * ::stExecutePlan(pxmlPlan2, 0, 0, NULL, 0);
	 * \endcode
	 * 
	 * - Execute a plan twice: First to produce data for every 100th trial, and then every 10th
	 *	 for the subset of trials beginning at trial 101
	 * \code
	 * ::stSetGenome(pxmlGenome);
	 * ::stSetRecordRate(100, STRD_STANDARD, "/Users/Sample/exp3");
	 * ::stExecutePlan(pxmlPlan, 0, 0, NULL, 0);
	 * pxmlGenome = open("/Users/Sample/exp3/trial100.xml");
	 * ::stSetGenome(pxmlGenome);
	 * ::stSetRecordRate(10, STRD_ALL, "/Users/Sample/exp3/trials101-200");
	 * ::stExeuctePlan(pxmlPlan, 1, 100, NULL, 0);
	 * \endcode
	 */
	ST_RETCODE stExecutePlan(const char* pxmlPlan,
							size_t iTrialFirst, size_t cTrials,
							ST_PFNSTATUS pfnStatus, size_t cStatusRate);

	/**
	 * \brief An association of a value and a trial
	 *
	 */
	typedef struct
	{
		size_t _iTrial;				///< Trial in which the score was acheived
		UNIT _nValue;				///< Value associated with the trial
	} ST_TRIALVALUE;

	/**
	 * \brief An association between the number of rollbacks and a trial
	 *
	 */
	typedef struct
	{
		size_t _iTrial;				///< Trial during which the rollbacks were made
		size_t _cRollbacks;			///< Number of rollbacks executed
	} ST_TRIALROLLBACKS;

	/**
	 * \brief An association of a genome size and a trial
	 *
	 */
	typedef struct
	{
		size_t _iTrial;				///< Trial in which the score was acheived
		size_t _cbBases;			///< Score associated with the trial
	} ST_TRIALSIZE;

	/**
	 * \brief A paired attempt/accept rate
	 *
	 */
	typedef struct
	{
        size_t _cConsidered;        ///< Number of considerations
		size_t _cAttempted;			///< Number of attempts
		size_t _cAccepted;			///< Number of accepted attempts
		size_t _cbBases;			///< Number of bases affected
	} ST_ATTEMPTS;

	/**
	 * \brief Various collected statistics
	 * 
	 */
	typedef struct
	{
		size_t _iTrialInitial;		///< First trial covered by the data
		size_t _iTrialCurrent;		///< Current trial
		size_t _cTrialAttempts;		///< Number of attempted trials (successful and unsuccessful)
		UNIT _nScore;				///< Current gene score (assumes a single gene per genome)
		UNIT _nUnits;				///< Current gene units (assumes a single gene per genome)
		UNIT _nCost;				///< Current genome cost
		UNIT _nFitness;				///< Current genome fitness
		size_t _cRollbacks;			///< Rollbacks executed in current trial
		size_t _cbBases;			///< Current number of bases
	
		ST_TRIALVALUE _tsMax;		///< Maximum achieved gene score
		ST_TRIALVALUE _tsMin;		///< Minimum achieved gene score

		ST_TRIALVALUE _tuMax;		///< Maximum achieved gene units
		ST_TRIALVALUE _tuMin;		///< Minimum achieved gene units

		ST_TRIALVALUE _tcMax;		///< Maximum achieved gene cost
		ST_TRIALVALUE _tcMin;		///< Minimum achieved gene cost

		ST_TRIALVALUE _tfMax;		///< Maximum achieved fitness level
		ST_TRIALVALUE _tfMin;		///< Minimum achieved fitness level

		size_t _cTotalRollbacks;	///< Total number of rollbacks executed
		ST_TRIALROLLBACKS _trMax;	///< Maximum number of rollbacks in any one trial
		ST_TRIALROLLBACKS _trMin;	///< Minimum number of rollbacks in any one trial

		ST_TRIALSIZE _tzMax;		///< Maximum genome size acheived in any one trial
		ST_TRIALSIZE _tzMin;		///< Minimum genome size acheived in any one trial

		size_t _cbBasesChanged;		///< Total number of bases changed
		size_t _cbBasesInserted;	///< Total number of bases inserted
		size_t _cbBasesDeleted;		///< Total number of bases deleted

		size_t _cSilent;			///< Number of silent mutations
		size_t _cAttempted;			///< Total number of attempted mutations
		size_t _cConsidered;		///< Total number of considered mutations
		size_t _cAccepted;			///< Total number of accepted mutations
		ST_ATTEMPTS _atChanged;		///< Number of attempted/accepted changes
		ST_ATTEMPTS _atCopied;		///< Number of attempted/accepted copies
		ST_ATTEMPTS _atDeleted;		///< Number of attempted/accepted deletions
		ST_ATTEMPTS _atInserted;	///< Number of attempted/accepted insertions
		ST_ATTEMPTS _atTransposed;	///< Number of attempted/accepted transpositions
	} ST_STATISTICS;

	/**
	 * \brief Method to obtain current genome statistics
	 * 
	 */
	ST_RETCODE stGetStatistics(ST_STATISTICS* pStatistics);

	/**
	 * \brief Genome state enumeration
	 *
	 * The Genome always is one of the ST_GENOMESTATEs. The Genome "wakes up" DEAD
	 * and is initially transitioned to VALID by calling stSetGenome. From then
	 * on, the Genome moves between various states. Control never returns to the
	 * caller with the Genome in an internal state. Methods that interrogate structure
	 * and score require that the Genome be ALIVE; other methods only require that the
	 * Genome not be DEAD. stTerminate and stLoadDefinition may be called at any time.
	 * Calling validate() in a genome moves it into the SCORED state. 
     * Client code should then call either rollback() to throw away changes
     * or recordStatistics if it wants to keep them.
     *
	 * \remarks
	 * - The current implementation does not take full advantage of multiple states,
	 *	 but leaves the infrastructure in place. Future versions may expose more
	 *	 internal states such that callers can have more operational control (e.g.,
	 *	 force a validation and rollback based on their own criteria).
	 */
	typedef enum
	{
		STGS_ALIVE = 0,		///< Genome is alive and valid - EXTERNAL
		STGS_COMPILED,		///< Genome has compiled acids
		STGS_COMPILING,		///< Genome is compiling acids
		STGS_DEAD,			///< Genome has died - EXTERNAL
		STGS_INVALID,		///< Internal structures are out-of-sync - EXTERNAL
		STGS_LOADING,		///< Genome is loading a new definition
		STGS_MUTATING,		///< Genome is undergoing a change
		STGS_RECORDING,		///< Recording (to log files) current state
		STGS_ROLLBACK,		///< Removing the effects of a bad change
		STGS_RESTORING,		///< Removing non-accumulated mutations
		STGS_SCORED,		///< Scoring is complete - EXTERNAL
		STGS_SCORING,		///< Scoring current trial
		STGS_SPAWNING,		///< Spawning a new trial
		STGS_VALIDATED,		///< Internal structures are complete
		STGS_VALIDATING,	///< Validating internal structures

		STGS_MAX
	} ST_GENOMESTATE;

	/**
	 * \brief Retrieve the current Genome state
	 *
	 */
	ST_RETCODE stGetGenomeState(ST_GENOMESTATE* pgs);
	
	/**
	 * \brief Genome termination enumeration
	 * 
	 * Whenever a process, such as loading a genome or executing a plan, terminates, the
	 * terminating action is recorded, along with an action specific reason, and a
	 * human-readable string. These values are useful for interpreting why a genome
	 * process terminated.
	 */
	typedef enum
	{
		STGT_NONE = 0,			///< No failing action is recorded
		STGT_COMPILATION,		///< Genome failed to compile
		STGT_VALIDATION,		///< Genome failed to validate
		STGT_SCORE,				///< Genome failed to meet a score condition
		STGT_MUTATION,			///< Genome could not make the requested mutation
		STGT_DURATION,			///< A plan reached a duration condition
		STGT_ROLLBACK,			///< A plan reached a rollback condition
		STGT_FITNESS,			///< A plan reached a fitness condition
		STGT_CALLBACK			///< A plan was asked to end
	} ST_GENOMETERMINATION;

	/**
	 * \brief Genome termination action reason codes
	 * 
	 * Terminating actions record an action-specific reason code.
	 */
	typedef enum
	{
		STGR_NONE = 0,			///< No failing action reason
		
		// Compliation reasons

		// Validation reasons
		STGR_STROKES = 1,		///< Failed assigning strokes
		STGR_MEASUREMENT,		///< Measurement failed

		// Scoring reasons
		STGR_COST = 1,			///< Genome failed to meet a cost condition
		STGR_FITNESS,			///< Genome failed to meet a fitness condition
		STGR_SCORE,				///< A gene failed to meet a score condition
		STGR_STROKE,			///< Stroke scoring failed

		// Mutation reasons
		STGR_CHANGE = 1,		///< Change mutation could not be made
		STGR_COPY,				///< Copy mutation could not be made
		STGR_DELETE,			///< Delete muation could not be made
		STGR_INSERT,			///< Insert mutation could not be made
		STGR_TRANSPOSE,			///< Transpose mutation could not be made

		// Duration reasons
		STGR_TRIALS = 1,		///< Maximum number of trials executed
		STGR_ATTEMPTS,			///< Maximum number of attempts made

		// Rollback reasons
		STGR_LIMIT = 1,			///< Maximum number of rollback attempts made
		
		// Fitness reasons
		STGR_MAXIMUM = 1,		///< Maximum fitness condition attained
		STGR_MINIMUM,			///< Minimum fitness condition attained
		
		// Callback reasons
		STGR_TERMINATED			///< Callback requested immediate termination

	} ST_GENOMEREASON;
	
	/**
	 * \brief Retrieve the current Genome termination details
	 * 
	 * \param[in] pgt Pointer to location for terminating action code
	 * \param[in] pgr Pointer to location for action reason code
	 * \param[in] pszTermination Buffer to hold human-readable string (may be NULL)
	 * \param[in,out] pcchTermination Length of output buffer on entry, number of
	 *							  characters copied or required on return (may be NULL)
	 */
	ST_RETCODE stGetGenomeTermination(ST_GENOMETERMINATION* pgt, ST_GENOMEREASON* pgr, char* pszTermination, size_t* pcchTermination);

    ST_RETCODE stSetSeed(const char * seed);
#ifdef __cplusplus
}
#endif

#endif	// STYLUS_H
