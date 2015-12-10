/*******************************************************************************
 * \file	global.cpp
 * \brief	Global Stylus file (see global.hpp and stylus.h for details)
 *
 * This file implements the C interface to Stylus and globally accessible
 * methods.
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

static const char* s_aryLEVELS[STLL_ALL] =
{
	"ERROR",
	"WARN ",
	"INFO ",
	"DEBUG",
	"TRACE",
};
static const size_t s_cbLEVELSTRING = 5;
static const size_t s_cbMAXFILENAME = 30;
static const size_t s_cbTRACEINDENT = 2;

static const char s_szWHITESPACE[] = " \n\r\t\f\v";
static const char s_szTIMEFORMAT[] = "%Y-%m-%dT%H:%M:%S.";
static const char s_achNUMBERS[] = "0123456789";
static const long long s_nUSECPERSEC = 1000000;

static char s_achBuffer[Constants::s_cbmaxBUFFER];

extern "C"
{
	/*
	 * Function: stGetLastError
	 * 
	 * Note:
	 * - This routine avoids the standard public macros so that, even if an error occurs, the "last"
	 *   error remains unmodified
	 */
	ST_RETCODE
	stGetLastError(ST_RETCODE* prc, char* pszDetails, size_t* pcchDetails)
	{
		ST_RETCODE rc = ST_RCERROR;
		ENTER(GLOBAL,stGetLastError);

		try
		{
			if (!VALID(prc) || !VALID(pszDetails) || !VALID(pcchDetails) || *pcchDetails <= 0)
				rc = ST_RCBADARGUMENTS;

			else
			{
				const Error& e = Error::getLastError();
				*prc = e.getRetcode();
				rc = copyString(pszDetails, pcchDetails, e.toCString());
			}
		}
		catch (...)
		{
			rc = ST_RCERROR;
		}

		return rc;
	}

	/*
	 * Function: stGetLastErrorDescription
	 * 
	 * Note:
	 * - This routine avoids the standard public macros so that, even if an error occurs, the "last"
	 *   error remains unmodified
	 */
	ST_RETCODE
	stGetLastErrorDescription(ST_RETCODE* prc, char* pszDetails, size_t* pcchDetails)
	{
		ST_RETCODE rc = ST_RCERROR;
		ENTER(GLOBAL,stGetLastError);

		try
		{
			if (!VALID(prc) || !VALID(pszDetails) || !VALID(pcchDetails) || *pcchDetails <= 0)
				rc = ST_RCBADARGUMENTS;

			else
			{
				const Error& e = Error::getLastError();
				*prc = e.getRetcode();
				rc = copyString(pszDetails, pcchDetails, e.getDescription());
			}
		}
		catch (...)
		{
			rc = ST_RCERROR;
		}

		return rc;
	}


	/*
	 * Function: stInitialize
	 *
	 */
	ST_RETCODE
	stInitialize()
	{
		ENTERPUBLIC(GLOBAL,stInitialize);
		
		Han::initialize();
		XMLDocument::initialize();
		RGenerator::initialize(RandomC::s_strUUID);
		Genome::initialize();

		Globals::setInitialized(true);

		LOGINFO((LLINFO, "Stylus initialized - %s", Globals::s_szVersion));
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stInitialize);
	}

	/*
	 * Function: stTerminate
	 *
	 */
	ST_RETCODE
	stTerminate()
	{
		ENTERPUBLIC(GLOBAL,stTerminate);

		LOGINFO((LLINFO, "Stylus terminating - %s", Globals::s_szVersion));
		Globals::setInitialized(false);

		Genome::terminate();
		RGenerator::terminate();
		XMLDocument::terminate();
		Han::terminate();

		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stTerminate);
	}
	
	/*
	 * Function: stSetGlobals
	 * 
	 */
	ST_RETCODE
	stSetGlobals(const char* pxmlGlobals)
	{
		ENTERPUBLIC(GLOBAL,stSetGlobals);
		RETURN_NOTINITIALIZED();
		
		if (!VALID(pxmlGlobals))
			RETURN_BADARGS();

		Globals::load(pxmlGlobals);
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stSetGlobals);
	}
	

	/*
	 * Function: stSetScope
	 * 
	 */
	ST_RETCODE
	stSetScope(const char* pszURLHan, const char* pszURLXMLSchema)
	{
		ENTERPUBLIC(GLOBAL,stSetScope);

		if (!VALID(pszURLHan) || !VALID(pszURLXMLSchema))
			RETURN_BADARGS();
			
		if (EMPTYSZ(pszURLHan))
			pszURLHan = Constants::s_strURL_DEFAULTSCOPE.c_str();
			
		if (EMPTYSZ(pszURLXMLSchema))
			pszURLXMLSchema = Constants::s_strURL_DEFAULTSCOPE.c_str();

		Han::setScope(pszURLHan);
		XMLDocument::setScope(pszURLXMLSchema);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetScope);
	}

	/*
	 * Function: stSetLogLevel
	 *
	 */
	ST_RETCODE
	stSetLogLevel(ST_LOGLEVEL ll)
	{
		ENTERPUBLIC(GLOBAL,stSetLogLevel);
		RETURN_NOTINITIALIZED();

		Globals::setLogLevel(ll);
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stSetLogLevel);
	}

	/*
	 * Function: stGetLogLevel
	 *
	 */
	ST_RETCODE
	stGetLogLevel(ST_LOGLEVEL* pll)
	{
		ENTERPUBLIC(GLOBAL,stGetLogLevel);
		RETURN_NOTINITIALIZED();

		if (!VALID(pll))
			RETURN_BADARGS();

		*pll = Globals::getLogLevel();
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetLogLevel);
	}

	/*
	 * Function: stSetLogOptions
	 *
	 */
	ST_RETCODE
	stSetLogOptions(STFLAGS grfLogOptions)
	{
		ENTERPUBLIC(GLOBAL,stSetLogOptions);
		RETURN_NOTINITIALIZED();

		Globals::setLogOptions(grfLogOptions);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetLogOptions);
	}

	/*
	 * Function: stGetLogOptions
	 *
	 */
	ST_RETCODE
	stGetLogOptions(STFLAGS* pgrfLogOptions)
	{
		ENTERPUBLIC(GLOBAL,stGetLogOptions);
		RETURN_NOTINITIALIZED();

		if (!VALID(pgrfLogOptions))
			RETURN_BADARGS();

		*pgrfLogOptions = Globals::getLogOptions();
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetLogOptions);
	}

	/*
	 * Function: stSetLogRate
	 *
	 */
	ST_RETCODE
	stSetLogRate(size_t cLogRate)
	{
		ENTERPUBLIC(GLOBAL,stSetLogRate);
		RETURN_NOTINITIALIZED();

		Globals::setLogRate(cLogRate);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetLogRate);
	}

	/*
	 * Function: stGetLogRate
	 *
	 */
	ST_RETCODE
	stGetLogRate(size_t* pcLogRate)
	{
		ENTERPUBLIC(GLOBAL,stGetLogRate);
		RETURN_NOTINITIALIZED();

		if (!VALID(pcLogRate))
			RETURN_BADARGS();

		*pcLogRate = Globals::getLogRate();
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetLogRate);
	}

    /*
     * Function: stSetLogFile
     *
     */
    ST_RETCODE
    stSetLogFile(const char * filename)
    {
        ENTERPUBLIC(GLOBAL,stSetLogFile);
		RETURN_NOTINITIALIZED();

		if (!VALID(filename))
			RETURN_BADARGS();

		Globals::setLogFile(filename);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetLogFile);
    }


	/*
	 * Function: stClearTraceRegions
	 *
	 */
	ST_RETCODE
	stClearTraceRegions(STFLAGS grfRegions)
	{
		ENTERPUBLIC(GLOBAL,stClearTraceRegions);
		RETURN_NOTINITIALIZED();

		Globals::setTraceRegions(Globals::getTraceRegions() & ~grfRegions);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stClearTraceRegions);
	}

	/*
	 * Function: stEnableTraceRegions
	 *
	 */
	ST_RETCODE
	stEnableTraceRegions(STFLAGS grfRegions)
	{
		ENTERPUBLIC(GLOBAL,stEnableTraceRegions);
		RETURN_NOTINITIALIZED();

		Globals::setTraceRegions(Globals::getTraceRegions() | grfRegions);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stEnableTraceRegions);
	}

	/*
	 * Function: stSetTraceRegions
	 *
	 */
	ST_RETCODE
	stSetTraceRegions(STFLAGS grfRegions)
	{
		ENTERPUBLIC(GLOBAL,stSetTraceRegions);
		RETURN_NOTINITIALIZED();

		Globals::setTraceRegions(grfRegions);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetTraceRegions);
	}

	/*
	 * Function: stGetTraceRegions
	 *
	 */
	ST_RETCODE
	stGetTraceRegions(STFLAGS* pgrfRegions)
	{
		ENTERPUBLIC(GLOBAL,stGetTraceRegions);
		RETURN_NOTINITIALIZED();

		if (!VALID(pgrfRegions))
			RETURN_BADARGS();

		*pgrfRegions = Globals::getTraceRegions();
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetTraceRegions);
	}

	/*
	 * Function: stClearTraceLevels
	 *
	 */
	ST_RETCODE stClearTraceLevels()
	{
		ENTERPUBLIC(GLOBAL,stClearTraceLevels);
		RETURN_NOTINITIALIZED();

		for (size_t tc=0; tc < STTC_MAX; ++tc)
			Globals::setTraceLevel(static_cast<ST_TRACECATEGORY>(tc), STTL_NONE);

		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stClearTraceLevels);
	}

	/*
	 * Function: stSetTraceLevel
	 *
	 */
	ST_RETCODE
	stSetTraceLevel(ST_TRACECATEGORY tc, ST_TRACELEVEL tl)
	{
		ENTERPUBLIC(GLOBAL,stSetTraceLevel);
		RETURN_NOTINITIALIZED();

		if (tc < 0 || tc >= STTC_MAX || tl < 0 || tl > STTL_MAX)
			RETURN_BADARGS();

		Globals::setTraceLevel(tc, tl);
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stSetTraceLevel);
	}

	/*
	 * Function: stGetTraceLevel
	 *
	 */
	ST_RETCODE
	stGetTraceLevel(ST_TRACECATEGORY tc, ST_TRACELEVEL* ptl)
	{
		ENTERPUBLIC(GLOBAL,stGetTraceLevel);
		RETURN_NOTINITIALIZED();

		if (tc < 0 || tc >= STTC_MAX || !VALID(ptl))
			RETURN_BADARGS();

		*ptl = Globals::getTraceLevel(tc);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetTraceLevel);
	}

	/*
	 * Function: stSetTraceTrial
	 *
	 */
	ST_RETCODE
	stSetTraceTrial(size_t iTrial)
	{
		ENTERPUBLIC(GLOBAL,stSetTraceTrial);
		RETURN_NOTINITIALIZED();

		Globals::setTraceTrial(iTrial);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetTraceTrial);
	}

	/*
	 * Function: stGetTraceTrial
	 *
	 */
	ST_RETCODE
	stGetTraceTrial(size_t* piTrial)
	{
		ENTERPUBLIC(GLOBAL,stGetTraceTrial);
		RETURN_NOTINITIALIZED();

		if (!VALID(piTrial))
			RETURN_BADARGS();

		*piTrial = Globals::getTraceTrial();
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetTraceTrial);
	}

	/*
	 * Function: stSetTraceAttempt
	 *
	 */
	ST_RETCODE
	stSetTraceAttempt(size_t iAttempt)
	{
		ENTERPUBLIC(GLOBAL,stSetTraceAttempt);
		RETURN_NOTINITIALIZED();

		Globals::setTraceAttempt(iAttempt);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetTraceAttempt);
	}

	/*
	 * Function: stGetTraceAttempt
	 *
	 */
	ST_RETCODE
	stGetTraceAttempt(size_t* piAttempt)
	{
		ENTERPUBLIC(GLOBAL,stGetTraceAttempt);
		RETURN_NOTINITIALIZED();

		if (!VALID(piAttempt))
			RETURN_BADARGS();

		*piAttempt = Globals::getTraceAttempt();
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetTraceAttempt);
	}
	
	/*
	 * Function: stSetRecordRate
	 * 
	 */
	ST_RETCODE
	stSetRecordRate(size_t cRecordRate, STFLAGS grfRecordDetail, const char* pszRecordDirectory, bool fRecordHistory)
	{
		ENTERPUBLIC(GLOBAL,stSetRecordRate);
		RETURN_NOTINITIALIZED();
		
		struct stat st;

		if (grfRecordDetail != STRD_NONE && !VALID(pszRecordDirectory))
			RETURN_BADARGS();

		if (fRecordHistory && !VALID(pszRecordDirectory))
			RETURN_BADARGS();
		
		if (VALID(pszRecordDirectory) && ::stat(pszRecordDirectory, &st) != 0)
			RETURN_BADARGS();

		Genome::setRecordRate(cRecordRate, grfRecordDetail, pszRecordDirectory, fRecordHistory);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stSetRecordRate);
	}
	
	/*
	 * Function: stSetGenome
	 *
	 */
	ST_RETCODE
	stSetGenome(const char* pxmlGenome, const char* pszAuthor)
	{
		ENTERPUBLIC(GLOBAL,stSetGenome);
		RETURN_NOTINITIALIZED();

		if (!VALID(pxmlGenome) || EMPTYSZ(pxmlGenome))
			RETURN_BADARGS();
		
		Error::clearErrors();
		
		TRACEDOIF(GLOBAL,DATA,L2,Unit::logConstants());

		Genome::setGenome(pxmlGenome, pszAuthor);
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stSetGenome);
	}

	/*
	 * Function: stGetGenome
	 *
	 */
	ST_RETCODE
	stGetGenome(char* pxmlGenome, size_t* pcchGenome, STFLAGS grfRecordDetail)
	{
		ENTERPUBLIC(GLOBAL,stGetGenome);
		RETURN_NOTINITIALIZED();

		if (!VALID(pxmlGenome) || !VALID(pcchGenome))
			RETURN_BADARGS();
		Genome::getGenome(pxmlGenome, pcchGenome, grfRecordDetail);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetGenome);
		return ST_RCSUCCESS;
	}

	/*
	 * Function: stGetGenomeBases
	 *
	 */
	ST_RETCODE
	stGetGenomeBases(char* pbBases, size_t* pcbBases)
	{
		ENTERPUBLIC(GLOBAL,stGetGenomeBases);
		RETURN_NOTINITIALIZED();

		if (!VALID(pbBases) || !VALID(pcbBases))
			RETURN_BADARGS();

		const std::string& strBases = Genome::getBases();
		if (*pcbBases < (strBases.length()+1))
		{
			*pcbBases = strBases.length()+1;
			RETURN_RC(BUFFERTOOSMALL);
		}
		*pcbBases = strBases.length()+1;
		::memcpy(pbBases, strBases.c_str(), *pcbBases);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetGenomeBases);
	}
	
	/*
	 * Function: stExecutePlan
	 * 
	 */
	ST_RETCODE
	stExecutePlan(const char* pxmlPlan, size_t iTrial, size_t cTrials, ST_PFNSTATUS pfnStatus, size_t cStatusRate)
	{
		ENTERPUBLIC(GLOBAL,stExecutePlan);
		RETURN_NOTINITIALIZED();
		
		if (!VALID(pxmlPlan))
			RETURN_BADARGS();
			
		Genome::executePlan(pxmlPlan, iTrial, cTrials, pfnStatus, cStatusRate);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stExecutePlan);
	}

	ST_RETCODE
	stExecutePlanForMutations(const char* pxmlPlan, size_t iTrial, size_t cTrials, ST_PFNSTATUS pfnStatus, size_t cStatusRate)
	{
		ENTERPUBLIC(GLOBAL,stExecutePlan);
		RETURN_NOTINITIALIZED();
		
		if (!VALID(pxmlPlan))
			RETURN_BADARGS();
			
        Genome::setMutationCallback(pfnStatus);
		Genome::executePlan(pxmlPlan, iTrial, cTrials, NULL, cStatusRate);
        Genome::setMutationCallback(NULL);
		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stExecutePlan);
	}

	/*
	 * Function: stGetStatistics
	 *
	 */
	ST_RETCODE stGetStatistics(ST_STATISTICS* pStatistics)
	{
		ENTERPUBLIC(GLOBAL,stGetStatistics);
		RETURN_NOTINITIALIZED();

		if (!VALID(pStatistics))
			RETURN_BADARGS();

		Genome::getStatistics(pStatistics);

		RETURN_SUCCESS();
		
		EXITPUBLIC(GLOBAL,stGetStatistics);
	}

	/*
	 * Function: stGetGenomeState
	 *
	 */
	ST_RETCODE
	stGetGenomeState(ST_GENOMESTATE* pgs)
	{
		ENTERPUBLIC(GLOBAL,stGetGenomeState);
		RETURN_NOTINITIALIZED();

		if (!VALID(pgs))
			RETURN_BADARGS();

		*pgs = Genome::getState();
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stGetGenomeState);
	}

	/*
	 * Function: stGetGenomeTermination
	 * 
	 */
	ST_RETCODE
	stGetGenomeTermination(ST_GENOMETERMINATION* pgt, ST_GENOMEREASON* pgr, char* pszTermination, size_t* pcchTermination)
	{
		ENTERPUBLIC(GLOBAL,stGetGenomeTermination);
		RETURN_NOTINITIALIZED();

		if (!VALID(pgt) || !VALID(pgr))
			RETURN_BADARGS();

		Genome::getTermination(pgt, pgr, pszTermination, pcchTermination);
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL,stGetGenomeTermination);
	}

    ST_RETCODE stSetSeed(const char * seed)
    {
		ENTERPUBLIC(GLOBAL,stSetSeed);
		RETURN_NOTINITIALIZED();
        if (!VALID(seed))
            RETURN_BADARGS();
        RGenerator::setSeed(seed);
        RETURN_SUCCESS();
		EXITPUBLIC(GLOBAL,stSetSeed);
    }
}

/*
 * Function: copyString
 *
 */
ST_RETCODE
stylus::copyString(char* pszTarget, size_t* pcchTarget, const char* pszSource) throw()
{
	return copyBytes(pszTarget, pcchTarget, pszSource, ::strlen((const char*)pszSource)+1);
}

/*
 * Function: copyBytes
 *
 */
ST_RETCODE
stylus::copyBytes(char* pbTarget, size_t* pcbTarget, const char* pbSource, size_t cbSource) throw()
{
	ST_RETCODE rc = ST_RCERROR;

	if (!VALID(pbTarget) || !VALID(pcbTarget) || *pcbTarget <= 0)
		RETURN_BADARGS();

	if (*pcbTarget < cbSource)
	{
		*pcbTarget = cbSource;
		RETURN_RC(BUFFERTOOSMALL);
	}

	::memcpy(pbTarget, pbSource, cbSource);
	*pcbTarget = cbSource;
	RETURN_SUCCESS();

EXIT:
	return rc;
}

/*
 * Function: copyFileline
 *
 */
size_t
stylus::copyFileline(char* pszBuffer, size_t cbRemaining, const char* pszFileline) throw()
{
	char szFileline[Constants::s_cchmaxFILELINE];
	size_t cchFileline = ::strlen(pszFileline);
	const char* psz;
	size_t cb;

	cb = ARRAY_LENGTH(szFileline);
	copyBytes(szFileline, &cb, Constants::s_strBLANKS.c_str(), cb);

	szFileline[0] = Constants::s_chLBRACKET;
	szFileline[ARRAY_LENGTH(szFileline)-1] = Constants::s_chRBRACKET;

	cb = min<size_t>(cchFileline, ARRAY_LENGTH(szFileline) - 2);
	psz = pszFileline + cchFileline - cb;
	copyBytes(szFileline+1, &cb, psz, cb);

	if (ST_RCSUCCESS != copyBytes(pszBuffer, &cbRemaining, szFileline, ARRAY_LENGTH(szFileline)))
		cbRemaining = 0;

	return cbRemaining;
}


/*
 * Function: trimWhitespace
 *
 */
char*
stylus::trimWhitespace(char* psz) throw()
{
	ENTER(GLOBAL,trimWhitespace);

	ASSERT(VALID(psz));

	psz += ::strspn(psz, s_szWHITESPACE);
	int cch = ::strcspn(psz, s_szWHITESPACE);
	if (cch > 0)
		psz[cch] = Constants::s_chNULL;

	return psz;
}

/*
 * Function: purgeWhitespace
 *
 */
char*
stylus::purgeWhitespace(char* psz) throw()
{
	ENTER(GLOBAL,purgeWhitespace);

	ASSERT(VALID(psz));

	char* pszDst = psz;
	char* pszSrc = psz;

	for ( ; *pszSrc; pszSrc++)
	{
		if (!::isspace(*pszSrc))
			*pszDst++ = *pszSrc;
	}
	*pszDst = Constants::s_chNULL;

	return psz;
}

/*
 * Function: timeNow
 *
 */
typedef std::string utime;
void
stylus::timeNow(utime* pt) throw()
{
	if (VALID(pt))
	{

#ifdef _WIN32
		*pt = 0;
#else
		struct timeval tv;
		::gettimeofday(&tv, NULL);
		*pt = (tv.tv_sec * s_nUSECPERSEC) + tv.tv_usec;
#endif
	}
}

/*
 * Function timeToString
 *
 */
size_t
stylus::timeToString(char* psz, const utime* pt, bool fLong, bool fTerminate) throw()
{
	size_t cchWritten = 0;

	if (VALID(psz) && VALID(pt))
	{
		time_t t = (*pt / s_nUSECPERSEC);
		size_t ut = *pt - (t * s_nUSECPERSEC);

		cchWritten = ::strftime(psz, Constants::s_cchTIME, s_szTIMEFORMAT, ::gmtime(&t));
		psz += cchWritten;

		if (fLong)
		{
			*(psz+5) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+4) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+3) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+2) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+1) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+0) = s_achNUMBERS[ut];
			psz += 6;
			cchWritten += 6;
		}
		else
		{
			ut /= 1000;
			*(psz+2) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+1) = s_achNUMBERS[ut - ((ut / 10) * 10)]; ut /= 10;
			*(psz+0) = s_achNUMBERS[ut];
			psz += 3;
			cchWritten += 3;
		}

		*psz++ = Constants::s_chGMTZONE;
		cchWritten += 1;

		if (fTerminate)
		{
			*psz = Constants::s_chNULL;
			cchWritten += 1;
		}
	}

	return cchWritten;
}

/*
 * Function: validateBases
 *
 */
bool
stylus::validateBases(const char* pbBases, size_t cbBases)
{
	ASSERT(VALID(pbBases));

	const char* pb = pbBases;
	for (size_t i=0; i < cbBases; ++i, ++pb)
	{
		switch (*pb)
		{
		case Constants::s_chBASET:
		case Constants::s_chBASEC:
		case Constants::s_chBASEA:
		case Constants::s_chBASEG:
			break;

		default:
			return false;
		}
	}
	
	return true;
}

//--------------------------------------------------------------------------------
//
// Globals
//
//--------------------------------------------------------------------------------
static const char* s_aryGROUPSCOREMODE[GSM_MAX] =
{
	"average",
	"minimum"
};

bool Globals::_fInitialized = false;
bool Globals::_fSupplied = false;
		
ST_LOGLEVEL Globals::_ll = STLL_INFO;
STFLAGS Globals::_grfLogOptions = STLO_SHORTFORMAT;
size_t Globals::_cLogRate = 500;
std::ofstream Globals::_logFile;

STFLAGS Globals::_grfTR = 0;
ST_TRACELEVEL Globals::_tl[STTC_MAX] = { STTL_NONE, STTL_NONE };
size_t Globals::_cTraceindents = 0;
size_t Globals::_iTrialTrace = numeric_limits<size_t>::max();
size_t Globals::_iAttemptTrace = numeric_limits<size_t>::max();
bool Globals::_fAtTraceTrialOrAttempt = false;

STFLAGS Globals::_grfTF = 0;

GROUPSCOREMODE Globals::_gsm = GSM_MINIMUM;

Unit Globals::_aryGenomeWeights[SC_GENOMEMAX] =
	{
	//	FixedCost		CostPerBase			CostPerUnit
		Unit(0.00193),	Unit(0.0000238),	Unit(0.000952)
	};
Unit Globals::_aryGeneWeights[SC_GENEMAX] =
	{
	//	Scale		Placement	IllegalOverlaps	MissingOverlaps	Marks
		Unit(1.6),	Unit(8.0),	Unit(0.32),		Unit(1.6),		Unit(0.28)
	};
Unit Globals::_aryGeneSetpoints[SC_GENEMAX] =
	{
		Unit(0.0), Unit(0.0), Unit(0.0), Unit(0.0)
	};

Unit Globals::_aryGroupWeights[SC_GROUPMAX] =
	{
	//	Scale		Placement	IllegalOverlaps	MissingOverlaps	Deviation		ExtraLength		Dropouts
		Unit(2.8),	Unit(16.0), Unit(1.6),		Unit(1.6), 		Unit(16.0),		Unit(5.2),		Unit(0.8)
	};
Unit Globals::_aryGroupSetpoints[SC_GROUPMAX] =
	{
		Unit(0.0), Unit(0.0), Unit(0.0), Unit(0.0), Unit(0.0), Unit(0.0), Unit(0.0)
	};
	
/*
 * Function: load
 * 
 */
void
Globals::load(const char* pxmlGlobals)
{
	ENTER(GLOBAL,load);
	ASSERT(VALID(pxmlGlobals));
	
	// Create an XML document
	XMLDocumentSPtr spxd(XMLDocument::createInstance(pxmlGlobals));
	xmlXPathContextSPtr spxpc(spxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;
	xmlNodePtr pxn;
	string str;
	
	_fSupplied = true;
	
	// Read Globals element and properties
	spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_GLOBALS));
	if (!XMLDocument::isXPathSuccess(spxpo.get(), 1))
		THROWRC((RC(XMLERROR), "Unexpected number of globals elements"));

	pxn = spxpo->nodesetval->nodeTab[0];
	if (spxd->getAttribute(pxn, xmlTag(XT_GROUPSCOREMODE), str))
	{
		for (size_t iGSM=GSM_AVERAGE; iGSM < GSM_MAX; iGSM += 1)
		{
			if (str == s_aryGROUPSCOREMODE[iGSM])
				_gsm = static_cast<GROUPSCOREMODE>(iGSM);
		}
	}
	
	// Find and load weights
	spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_WEIGHTGENOME));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		pxn = spxpo->nodesetval->nodeTab[0];

		if (spxd->getAttribute(pxn, xmlTag(XT_FIXEDCOST), str))
			_aryGenomeWeights[SC_FIXEDCOST] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_COSTPERBASE), str))
			_aryGenomeWeights[SC_COSTPERBASE] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_COSTPERUNIT), str))
			_aryGenomeWeights[SC_COSTPERUNIT] = str;
	}

	spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_WEIGHTGENE));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		pxn = spxpo->nodesetval->nodeTab[0];

		if (spxd->getAttribute(pxn, xmlTag(XT_SCALE), str))
			_aryGeneWeights[SC_SCALE] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPSCALE), str))
			_aryGeneSetpoints[SC_SCALE] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_PLACEMENT), str))
			_aryGeneWeights[SC_PLACEMENT] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPPLACEMENT), str))
			_aryGeneSetpoints[SC_PLACEMENT] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_ILLEGALOVERLAPS), str))
			_aryGeneWeights[SC_ILLEGALOVERLAPS] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPILLEGALOVERLAPS), str))
			_aryGeneSetpoints[SC_ILLEGALOVERLAPS] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_MISSINGOVERLAPS), str))
			_aryGeneWeights[SC_MISSINGOVERLAPS] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPMISSINGOVERLAPS), str))
			_aryGeneSetpoints[SC_MISSINGOVERLAPS] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_MARKS), str))
			_aryGeneWeights[SC_MARKS] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPMARKS), str))
			_aryGeneSetpoints[SC_MARKS] = str;
	}

	spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_WEIGHTGROUP));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		pxn = spxpo->nodesetval->nodeTab[0];

		if (spxd->getAttribute(pxn, xmlTag(XT_SCALE), str))
			_aryGroupWeights[SC_SCALE] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPSCALE), str))
			_aryGroupSetpoints[SC_SCALE] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_PLACEMENT), str))
			_aryGroupWeights[SC_PLACEMENT] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPPLACEMENT), str))
			_aryGroupSetpoints[SC_PLACEMENT] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_DEVIATION), str))
			_aryGroupWeights[SC_DEVIATION] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPDEVIATION), str))
			_aryGroupSetpoints[SC_DEVIATION] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_EXTRALENGTH), str))
			_aryGroupWeights[SC_EXTRALENGTH] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPEXTRALENGTH), str))
			_aryGroupSetpoints[SC_EXTRALENGTH] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_ILLEGALOVERLAPS), str))
			_aryGroupWeights[SC_ILLEGALOVERLAPS] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPILLEGALOVERLAPS), str))
			_aryGroupSetpoints[SC_ILLEGALOVERLAPS] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_MISSINGOVERLAPS), str))
			_aryGroupWeights[SC_MISSINGOVERLAPS] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPMISSINGOVERLAPS), str))
			_aryGroupSetpoints[SC_MISSINGOVERLAPS] = str;

		if (spxd->getAttribute(pxn, xmlTag(XT_DROPOUTS), str))
			_aryGroupWeights[SC_DROPOUTS] = str;
		
		if (spxd->getAttribute(pxn, xmlTag(XT_SPDROPOUTS), str))
			_aryGroupSetpoints[SC_DROPOUTS] = str;
	}
}

/*
 * Function: toXML
 * 
 */
void
Globals::toXML(XMLStream& xs)
{
	ENTER(GLOBAL,toXML);
	xs.openStart(xmlTag(XT_GLOBALS));
	xs.writeAttribute(xmlTag(XT_XMLNS), XMLDocument::s_szStylusNamespace);
	xs.closeStart();

	xs.writeStart(xmlTag(XT_WEIGHTS));
	
	xs.openStart(xmlTag(XT_GENOME));
	xs.writeAttribute(xmlTag(XT_FIXEDCOST), static_cast<UNIT>(_aryGenomeWeights[SC_FIXEDCOST]));
	xs.writeAttribute(xmlTag(XT_COSTPERBASE), static_cast<UNIT>(_aryGenomeWeights[SC_COSTPERBASE]));
	xs.writeAttribute(xmlTag(XT_COSTPERUNIT), static_cast<UNIT>(_aryGenomeWeights[SC_COSTPERUNIT]));
	xs.closeStart(false);

	xs.openStart(xmlTag(XT_GENE));
	xs.writeAttribute(xmlTag(XT_SCALE), static_cast<UNIT>(_aryGeneWeights[SC_SCALE]));
	xs.writeAttribute(xmlTag(XT_PLACEMENT), static_cast<UNIT>(_aryGeneWeights[SC_PLACEMENT]));
	xs.writeAttribute(xmlTag(XT_ILLEGALOVERLAPS), static_cast<UNIT>(_aryGeneWeights[SC_ILLEGALOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_MISSINGOVERLAPS), static_cast<UNIT>(_aryGeneWeights[SC_MISSINGOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_MARKS), static_cast<UNIT>(_aryGeneWeights[SC_MARKS]));
	xs.writeAttribute(xmlTag(XT_SPSCALE), static_cast<UNIT>(_aryGeneSetpoints[SC_SCALE]));
	xs.writeAttribute(xmlTag(XT_SPPLACEMENT), static_cast<UNIT>(_aryGeneSetpoints[SC_PLACEMENT]));
	xs.writeAttribute(xmlTag(XT_SPILLEGALOVERLAPS), static_cast<UNIT>(_aryGeneSetpoints[SC_ILLEGALOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_SPMISSINGOVERLAPS), static_cast<UNIT>(_aryGeneSetpoints[SC_MISSINGOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_SPMARKS), static_cast<UNIT>(_aryGeneSetpoints[SC_MARKS]));
	xs.closeStart(false);

	xs.openStart(xmlTag(XT_GROUP));
	xs.writeAttribute(xmlTag(XT_SCALE), static_cast<UNIT>(_aryGroupWeights[SC_SCALE]));
	xs.writeAttribute(xmlTag(XT_PLACEMENT), static_cast<UNIT>(_aryGroupWeights[SC_PLACEMENT]));
	xs.writeAttribute(xmlTag(XT_ILLEGALOVERLAPS), static_cast<UNIT>(_aryGroupWeights[SC_ILLEGALOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_DEVIATION), static_cast<UNIT>(_aryGroupWeights[SC_DEVIATION]));
	xs.writeAttribute(xmlTag(XT_EXTRALENGTH), static_cast<UNIT>(_aryGroupWeights[SC_EXTRALENGTH]));
	xs.writeAttribute(xmlTag(XT_MISSINGOVERLAPS), static_cast<UNIT>(_aryGroupWeights[SC_MISSINGOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_DROPOUTS), static_cast<UNIT>(_aryGroupWeights[SC_DROPOUTS]));
	xs.writeAttribute(xmlTag(XT_SPSCALE), static_cast<UNIT>(_aryGroupSetpoints[SC_SCALE]));
	xs.writeAttribute(xmlTag(XT_SPPLACEMENT), static_cast<UNIT>(_aryGroupSetpoints[SC_PLACEMENT]));
	xs.writeAttribute(xmlTag(XT_SPILLEGALOVERLAPS), static_cast<UNIT>(_aryGroupSetpoints[SC_ILLEGALOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_SPDEVIATION), static_cast<UNIT>(_aryGroupSetpoints[SC_DEVIATION]));
	xs.writeAttribute(xmlTag(XT_SPEXTRALENGTH), static_cast<UNIT>(_aryGroupSetpoints[SC_EXTRALENGTH]));
	xs.writeAttribute(xmlTag(XT_SPMISSINGOVERLAPS), static_cast<UNIT>(_aryGroupSetpoints[SC_MISSINGOVERLAPS]));
	xs.writeAttribute(xmlTag(XT_SPDROPOUTS), static_cast<UNIT>(_aryGroupSetpoints[SC_DROPOUTS]));
	xs.closeStart(false);

	xs.writeEnd(xmlTag(XT_WEIGHTS));
	
	xs.writeEnd(xmlTag(XT_GLOBALS));
}

/*
 * Function: setLogFile
 */
void
Globals::setLogFile(const char * logFilename)
{
    _logFile.open(logFilename);
}

/*
 * Function: log
 *
 */
void
Globals::log(ST_LOGLEVEL ll, const char* pszFileline, const char* pszFormat, ...) throw()
{
	char* pszMsg = s_achBuffer;
	size_t cbRemaining = SZ_OF(s_achBuffer);
	size_t cbWritten;
	ST_RETCODE rc = ST_RCSUCCESS;
	va_list ap;

	va_start(ap, pszFormat);

	utime tNow;
	timeNow(&tNow);
	cbWritten = timeToString(pszMsg, &tNow);
	pszMsg += cbWritten;
	cbRemaining -= cbWritten;

	*pszMsg++ = Constants::s_chBLANK;
	*pszMsg++ = Constants::s_chLBRACKET;
	cbRemaining -= 2;

	cbWritten = cbRemaining;
	RETURN_IFERROR(copyBytes(pszMsg, &cbWritten, s_aryLEVELS[ll-1], s_cbLEVELSTRING));
	pszMsg += cbWritten;
	cbRemaining -= cbWritten;

	*pszMsg++ = Constants::s_chRBRACKET;
	*pszMsg++ = Constants::s_chBLANK;
	cbRemaining -= 2;

	if (!(_grfLogOptions & STLO_SHORTFORMAT) && VALID(pszFileline))
	{
		cbWritten = copyFileline(pszMsg, cbRemaining, pszFileline);
		pszMsg += cbWritten;
		cbRemaining -= cbWritten;

		*pszMsg++ = Constants::s_chBLANK;
		cbRemaining -= 1;
	}

	if (ll == STLL_TRACE)
	{
		size_t cbBLANKS = (_cTraceindents > 0
						   ? min<size_t>(_cTraceindents, (Constants::s_strBLANKS.length() / s_cbTRACEINDENT)) * s_cbTRACEINDENT
						   : 0);
		if (cbBLANKS > 0)
		{
			cbWritten = cbRemaining;
			copyBytes(pszMsg, &cbWritten, Constants::s_strBLANKS.c_str(), cbBLANKS);

			pszMsg += cbWritten;
			cbRemaining -= cbWritten;
		}
	}

	::vsnprintf(pszMsg, cbRemaining, pszFormat, ap);

EXIT:
	if (_grfLogOptions & STLO_USESTDOUT)
		cout << s_achBuffer << endl;

    if( !_logFile.is_open() )
        cerr << s_achBuffer << endl;
    else
        _logFile << s_achBuffer << endl;

	va_end(ap);
	return;
}
