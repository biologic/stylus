/*
 * StylusEngine.i
 * 
 * This file is the SWIG interface definition file that guides building the StylusEngine module.
 * See SWIG (http://www.swig.org/) for more details.
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
 */
 
//-----------------------------------------------------------------------------
//
// Includes, Constants, and Control statements
//
//-----------------------------------------------------------------------------
%module stylusengine
%include "cstring.i" 
%{
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <ostream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const unsigned long DEFAULT_BUFFERSIZE = 4096;
bool g_fStylusInitialized = false;

#define ARRAY_LENGTH(x)		((size_t)(sizeof(x)/sizeof(x[0])))
#define EMPTYSZ(x)			(::strlen(x) <= 0)
#define VALID(x)			(x != NULL)

#include <stylus.h>
%}

%naturalvar;
%immutable;

// Include the Stylus interface file
// - This hides most parts since the true interface is defined by special purpose functions
%ignore STYLUS_H;
%ignore ST_ISALLSET;
%ignore ST_ISANYSET;
%ignore ST_ISCLEAR;

%ignore ST_RETCODE;
%ignore ST_MASKTYPECODE;
%ignore ST_MASKERRORCODE;
%ignore ST_ISTYPEERROR;
%ignore ST_ISTYPEWARNING;
%ignore ST_RCCODE;
%ignore ST_ISSUCCESS;
%ignore ST_ISERROR;
%ignore ST_ISRC;

%ignore stGetLastError;
%ignore stGetLastErrorDescription;
%ignore stInitialize;
%ignore stTerminate;
%ignore stSetScope;
%ignore stGetVersion;

%ignore STLL_ERROR;
%ignore STLL_WARNING;
%ignore STLL_INFO;
%ignore STLL_DEBUG;
%ignore STLL_TRACE;
%ignore STLL_ALL;
%ignore ST_LOGLEVEL;

%ignore STLO_USESTDOUT;
%ignore STLO_SHORTFORMAT;
%ignore ST_LOGOPTION;

%ignore stSetLogLevel;
%ignore stGetLogLevel;
%ignore stSetLogOptions;
%ignore stGetLogOptions;
%ignore stSetLogRate;
%ignore stGetLogRate;
%ignore stSetLogFile;

%ignore STTR_NONE;
%ignore STTR_GLOBAL;
%ignore STTR_GENOME;
%ignore STTR_HAN;
%ignore STTR_MUTATION;
%ignore STTR_PLAN;
%ignore STTR_VALIDATION;
%ignore STTR_SCORING;
%ignore STTR_XML;
%ignore ST_TRACEREGION;

%ignore STTC_FLOW;
%ignore STTC_DATA;
%ignore STTC_MAX;
%ignore ST_TRACECATEGORY;

%ignore STTL_NONE;
%ignore STTL_L1;
%ignore STTL_L2;
%ignore STTL_L3;
%ignore STTL_L4;
%ignore STTL_L5;
%ignore STTL_MIN;
%ignore STTL_STD;
%ignore STTL_MAX;
%ignore ST_TRACELEVEL;

%ignore stClearTraceRegions;
%ignore stEnableTraceRegions;
%ignore stSetTraceRegions;
%ignore stGetTraceRegions;
%ignore stClearTraceLevels;
%ignore stSetTraceLevel;
%ignore stGetTraceLevel;
%ignore stSetTraceTrial;
%ignore stGetTraceTrial;

%ignore STRD_DIMENSIONS;
%ignore STRD_GENES;
%ignore STRD_LINEAGE;
%ignore STRD_LINEAGEALL;
%ignore STRD_SCORE;
%ignore STRD_SCOREALL;
%ignore STRD_SEGMENTS;
%ignore STRD_STATISTICS;
%ignore STRD_NONE;
%ignore STRD_RESTART;
%ignore STRD_STANDARD;
%ignore STRD_SUMMARY;
%ignore STRD_ALL;
%ignore ST_RECORDDETAIL;
%ignore DEFAULT_RECORDDETAIL;

%ignore stSetRecordRate;
%ignore stSetGenome;
%ignore stGetGenome;
%ignore stGetGenomeBases;

%ignore ST_PFNSTATUS;

%ignore stExecutePlan;

%ignore stGetStatistics;

%ignore stGetGenomeState;
%include <stylus.h>

//-----------------------------------------------------------------------------
//
// Helper Code
//
//-----------------------------------------------------------------------------
#ifdef SWIGPYTHON
// Convert Python lists of strings to a vector of strings
%inline
%{
	typedef std::vector<std::string> VECSTRING;
%}

%typemap(in) VECSTRING*
{
	if (PyList_Check($input))
	{
		size_t size = PyList_Size($input);
		$1 = ::new VECSTRING(size);
		for (size_t i = 0; i < size; ++i)
		{
			PyObject *o = PyList_GetItem($input,i);
			if (PyString_Check(o))
			{
				(*$1)[i] = PyString_AsString(PyList_GetItem($input,i));
			}
			else
			{
				PyErr_SetString(PyExc_TypeError,"Stylusengine Error: The passed list must contain strings");
				::delete $1;
				return NULL;
			}
		}
	}
	else
	{
		PyErr_SetString(PyExc_TypeError,"Stylusengine Error: Call must pass a list");
		return NULL;
	}
}

%typemap(freearg) VECSTRING*
{
	::delete $1;
}

// Convert an array of strings to a Python list of strings
%typemap(out) const char**
{
	size_t cStrings=0;
	while (VALID(*($1+cStrings))) ++cStrings;
	if (cStrings != 0)
	{
		$result = PyList_New(cStrings);
		if (VALID($result))
		{
			for (size_t iString=0; iString < cStrings; ++iString)
				PyList_SET_ITEM($result, iString, PyString_FromString(($1)[iString]));
		}
	}
	else
		return NULL;
}
#endif

%{
	static const char* s_aryLOGLEVELS[] =
	{
		"error",
		"warning",
		"info",
		"debug",
		"trace",
		"all",
		NULL
	};
	static const size_t s_aryLOGLEVELFLAGS[] =
	{
		STLL_ERROR,
		STLL_WARNING,
		STLL_INFO,
		STLL_DEBUG,
		STLL_TRACE,
		STLL_ALL
	};
	static const size_t s_cLOGLEVELS = ARRAY_LENGTH(s_aryLOGLEVELS)-1;

	static const char* s_aryTRACEREGIONS[] =
	{
		"none",
		"global",
		"genome",
		"han",
		"mutation",
		"plan",
		"validation",
		"scoring",
		"xml",
		"all",
		NULL
	};
	static const size_t s_aryTRACEREGIONFLAGS[] =
	{
		STTR_NONE,
		STTR_GLOBAL,
		STTR_GENOME,
		STTR_HAN,
		STTR_MUTATION,
		STTR_PLAN,
		STTR_VALIDATION,
		STTR_SCORING,
		STTR_XML,
		STTR_ALL
	};
	static const size_t s_cTRACEREGIONS = ARRAY_LENGTH(s_aryTRACEREGIONS)-1;

	static const char* s_aryRECORDDETAILS[] =
	{
		"dimensions",
		"genes",
		"lineage",
		"lineageall",
		"score",
		"scoreall",
		"segments",
		"statistics",
		"none",
		"restart",
		"standard",
		"summary",
		"all",
		NULL
	};
	static const size_t s_aryRECORDDETAILFLAGS[] =
	{
		STRD_DIMENSIONS,
		STRD_GENES,
		STRD_LINEAGE,
		STRD_LINEAGEALL,
		STRD_SCORE,
		STRD_SCOREALL,
		STRD_SEGMENTS,
		STRD_STATISTICS,
		STRD_NONE,
		STRD_RESTART,
		STRD_STANDARD,
		STRD_SUMMARY,
		STRD_ALL
	};
	static const size_t s_cRECORDDETAILS = ARRAY_LENGTH(s_aryRECORDDETAILS)-1;
	
	/*
	 * Convert an array of strings to an STFLAGS value (given arrays of strings and corresponding flags)
	 * 
	 * NOTE:
	 * This is horribly inefficient - but it is used so seldom and over such small amounts
	 * that efficiency is not a concern.
	 */
	STFLAGS stringsToFlags(const char** arySTRINGS, const size_t* aryFLAGS, size_t cSTRINGS, VECSTRING* pvecStrings)
	{
		STFLAGS grf = 0;
		for (size_t iString=0; iString < pvecStrings->size(); ++iString)
		{
			for (size_t iSTRING=0; iSTRING < cSTRINGS; ++iSTRING)
			{
				if ((*pvecStrings)[iString] == arySTRINGS[iSTRING])
				{
					grf |= aryFLAGS[iSTRING];
					break;
				}
			}
		}
		return grf;
	}
	
	inline ST_RETCODE ensureStylus()
	{
		if (!g_fStylusInitialized)
		{
			g_fStylusInitialized = true;
			return ::stInitialize();
		}
		else
			return ST_RCSUCCESS;
	}
%}

//-----------------------------------------------------------------------------
//
// Interface Definition
//
//-----------------------------------------------------------------------------
%newobject getLastError;
%newobject getVersion;
%newobject getGenome;
%newobject getGenomeBases;

%inline %{
	const char* errorToString(unsigned long rc)
	{
		static const char* aryERRORS[ST_RCCODE(ST_RCDEAD)+1] =
		{
			"Success",
			"Internal Error",
			"Stylus is not initialized",
			"Supplied buffer was too small",
			"Stylus ran out of memory",
			"Invalid arguments were passed to Stylus",
			"Illegal call while Stylus is executing a plan",
			"An error occurred in XML processing",
			"Call is illegal for current Stylus state",
			"Plan ended execution after reaching one or more conditions",
			"Genome is dead"
		};

		rc = ST_RCCODE(rc);
		return (rc >= 0 && rc <= ARRAY_LENGTH(aryERRORS)
				? aryERRORS[rc]
				: "Illegal Stylus return code");
	}
	
	bool errorIsType(unsigned long rc, unsigned long type)
	{
		return ((ST_MASKTYPECODE & rc) == type);
	}

	const char* getLastError()
	{
		char* pszLastError = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchLastError = DEFAULT_BUFFERSIZE;
		ST_RETCODE rc;
		if (VALID(pszLastError))
		{
			*pszLastError = '\0';
			if (ST_ISSUCCESS(::ensureStylus()) && ST_ISRC(::stGetLastError(&rc, pszLastError, &cchLastError), ST_RCBUFFERTOOSMALL))
			{
				::delete[] pszLastError;
				pszLastError = ::new char[cchLastError];
				if (VALID(pszLastError))
				{
					*pszLastError = '\0';
					::stGetLastError(&rc, pszLastError, &cchLastError);
				}
			}
		}
		return pszLastError;
	}

	const char* getLastErrorDescription()
	{
		char* pszLastError = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchLastError = DEFAULT_BUFFERSIZE;
		ST_RETCODE rc;
		if (VALID(pszLastError))
		{
			*pszLastError = '\0';
			if (ST_ISSUCCESS(::ensureStylus()) && ST_ISRC(::stGetLastErrorDescription(&rc, pszLastError, &cchLastError), ST_RCBUFFERTOOSMALL))
			{
				::delete[] pszLastError;
				pszLastError = ::new char[cchLastError];
				if (VALID(pszLastError))
				{
					*pszLastError = '\0';
					::stGetLastErrorDescription(&rc, pszLastError, &cchLastError);
				}
			}
		}
		return pszLastError;
	}
	
	unsigned long getLastErrorCode()
	{
		char* pszLastError = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchLastError = DEFAULT_BUFFERSIZE;
		ST_RETCODE rc = ::ensureStylus();
		ST_RETCODE rcLastError;
		if (!ST_ISSUCCESS(rc))
			return rc;
		rc = ::stGetLastError(&rcLastError, pszLastError, &cchLastError);
		return (ST_ISSUCCESS(rc) || ST_ISRC(rc, ST_RCBADARGUMENTS)
				? rcLastError
				: rc);
	}

	const char* getVersion()
	{
		char* pszVersion = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchVersion = DEFAULT_BUFFERSIZE;
		if (VALID(pszVersion))
		{
			*pszVersion = '\0';
			if (ST_ISSUCCESS(::ensureStylus()) && ST_ISRC(::stGetVersion(pszVersion, &cchVersion, false), ST_RCBUFFERTOOSMALL))
			{
				::delete[] pszVersion;
				pszVersion = ::new char[cchVersion];
				if (VALID(pszVersion))
					::stGetVersion(pszVersion, &cchVersion, false);
			}
		}
		return pszVersion;
	}
	
	unsigned long setGlobals(const char* pszGlobals)
	{
		ST_RETCODE rc = ::ensureStylus();
		return (!ST_ISSUCCESS(rc)
				? rc
				: ::stSetGlobals(pszGlobals));
	}

	unsigned long setScope(const char* pszURLHan, const char* pszURLXMLSchema)
	{
		ST_RETCODE rc = ::ensureStylus();
		return (!ST_ISSUCCESS(rc)
				? rc
				: ::stSetScope(pszURLHan, pszURLXMLSchema));
	}
	
	const char** getLogLevels()
	{
		return s_aryLOGLEVELS;
	}

	unsigned long setLogLevel(size_t cRate, const char* pszLevel, bool fEcho, bool fLong)
	{
		VECSTRING vecLevels(1);
		vecLevels[0] = pszLevel;
		ST_RETCODE rc = ::ensureStylus();
		STFLAGS grfOptions = (fEcho ? STLO_USESTDOUT : 0) | (fLong ? 0 : STLO_SHORTFORMAT);
		if (ST_ISSUCCESS(rc))
			rc = ::stSetLogOptions(grfOptions);
		if (ST_ISSUCCESS(rc))
			rc = ::stSetLogLevel(static_cast<ST_LOGLEVEL>(stringsToFlags(s_aryLOGLEVELS, s_aryLOGLEVELFLAGS, s_cLOGLEVELS, &vecLevels)));
		if (ST_ISSUCCESS(rc))
			rc = ::stSetLogRate(cRate);
		return rc;
	}

    unsigned long setLogFile(const char * filename)
    {
        ST_RETCODE rc = ::ensureStylus();
		return (!ST_ISSUCCESS(rc)
				? rc
				: ::stSetLogFile(filename));

    }



	const char** getTraceRegions()
	{
		return s_aryTRACEREGIONS;
	}

	unsigned long setTrace(VECSTRING* pvecRegions, ST_TRACELEVEL tlFlow, ST_TRACELEVEL tlData, size_t iTrial, size_t iAttempt)
	{
		ST_RETCODE rc = ::ensureStylus();
		if (ST_ISSUCCESS(rc))
			rc = ::stSetTraceRegions(stringsToFlags(s_aryTRACEREGIONS, s_aryTRACEREGIONFLAGS, s_cTRACEREGIONS, pvecRegions));
		if (ST_ISSUCCESS(rc))
			rc = ::stSetTraceLevel(STTC_FLOW, tlFlow);
		if (ST_ISSUCCESS(rc))
			rc = ::stSetTraceLevel(STTC_DATA, tlData);
		if (ST_ISSUCCESS(rc))
			rc = ::stSetTraceTrial(iTrial);
		if (ST_ISSUCCESS(rc))
			rc = ::stSetTraceAttempt(iAttempt);
		return rc;
	}
	
	const char** getRecordDetails()
	{
		return s_aryRECORDDETAILS;
	}

	unsigned long setRecordRate(size_t cRate, VECSTRING* pvecDetail, const char* pszDirectory, bool fRecordHistory)
	{
		ST_RETCODE rc = ::ensureStylus();
		return (!ST_ISSUCCESS(rc)
				? rc
				: ::stSetRecordRate(cRate, stringsToFlags(s_aryRECORDDETAILS, s_aryRECORDDETAILFLAGS, s_cRECORDDETAILS, pvecDetail), pszDirectory, fRecordHistory));
	}

	unsigned long setGenome(const char* pszGenome, const char* pszAuthor)
	{
		ST_RETCODE rc = ::ensureStylus();
		if (!VALID(pszAuthor) || EMPTYSZ(pszAuthor))
			pszAuthor = NULL;
		return (!ST_ISSUCCESS(rc)
				? rc
				: ::stSetGenome(pszGenome, pszAuthor));
	}
	
	const char* getGenome(VECSTRING* pvecDetail)
	{
		STFLAGS grfDetail = ::stringsToFlags(s_aryRECORDDETAILS, s_aryRECORDDETAILFLAGS, s_cRECORDDETAILS, pvecDetail);
		char* pszGenome = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchGenome = DEFAULT_BUFFERSIZE;
		if (VALID(pszGenome))
		{
			*pszGenome = '\0';
			if (ST_ISSUCCESS(::ensureStylus()) && ST_ISRC(::stGetGenome(pszGenome, &cchGenome, grfDetail), ST_RCBUFFERTOOSMALL))
			{
				::delete[] pszGenome;
				pszGenome = ::new char[cchGenome];
				if (VALID(pszGenome))
				{
					*pszGenome = '\0';
					::stGetGenome(pszGenome, &cchGenome, grfDetail);
				}
			}
		}
		return pszGenome;
	}

	const char* getGenomeBases()
	{
		char* pszBases = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchBases = DEFAULT_BUFFERSIZE;
		if (VALID(pszBases))
		{
			*pszBases = '\0';
			if (ST_ISSUCCESS(::ensureStylus()) && ST_ISRC(::stGetGenomeBases(pszBases, &cchBases), ST_RCBUFFERTOOSMALL))
			{
				::delete[] pszBases;
				pszBases = ::new char[cchBases];
				if (VALID(pszBases))
				{
					*pszBases = '\0';
					::stGetGenomeBases(pszBases, &cchBases);
				}
			}
		}
		return pszBases;
	}

	unsigned long executePlan(const char* pszPlan, size_t iTrialFirst, size_t cTrials)
	{
		ST_RETCODE rc = ::ensureStylus();
		return (!ST_ISSUCCESS(rc)
				? rc
				: ::stExecutePlan(pszPlan, iTrialFirst, cTrials, NULL, 0));
	}
	
	class STATISTICS : public ST_STATISTICS
	{
	public:
		STATISTICS() {}
		~STATISTICS() {}
	};

	STATISTICS getStatistics()
	{
		STATISTICS statistics;
		if (ST_ISSUCCESS(::ensureStylus()))
			::stGetStatistics(&statistics);
		return statistics;
	}
	
	ST_GENOMESTATE getState()
	{
		ST_GENOMESTATE gs;
		::stGetGenomeState(&gs);
		return gs;
	}
	
	ST_GENOMETERMINATION getTermination()
	{
		ST_RETCODE rc = ::ensureStylus();
		ST_GENOMETERMINATION gt;
		ST_GENOMEREASON gr;
		if (ST_ISSUCCESS(rc))
			rc = ::stGetGenomeTermination(&gt, &gr, NULL, NULL);
		return (ST_ISSUCCESS(rc) || ST_ISRC(rc, ST_RCBADARGUMENTS)
				? gt
				: STGT_NONE);
	}
	
	ST_GENOMEREASON getTerminationReason()
	{
		ST_RETCODE rc = ::ensureStylus();
		ST_GENOMETERMINATION gt;
		ST_GENOMEREASON gr;
		if (ST_ISSUCCESS(rc))
			rc = ::stGetGenomeTermination(&gt, &gr, NULL, NULL);
		return (ST_ISSUCCESS(rc) || ST_ISRC(rc, ST_RCBADARGUMENTS)
				? gr
				: STGR_NONE);
	}
	
	const char* getTerminationDescription()
	{
		ST_GENOMETERMINATION gt;
		ST_GENOMEREASON gr;
		char* pszTermination = ::new char[DEFAULT_BUFFERSIZE];
		size_t cchTermination = DEFAULT_BUFFERSIZE;
		if (VALID(pszTermination))
		{
			*pszTermination = '\0';
			if (ST_ISSUCCESS(::ensureStylus()) && ST_ISRC(::stGetGenomeTermination(&gt, &gr, pszTermination, &cchTermination), ST_RCBUFFERTOOSMALL))
			{
				::delete[] pszTermination;
				pszTermination = ::new char[cchTermination];
				if (VALID(pszTermination))
					::stGetGenomeTermination(&gt, &gr, pszTermination, &cchTermination);
			}
		}
		return pszTermination;
	}
%}
