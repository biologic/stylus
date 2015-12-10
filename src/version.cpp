/*******************************************************************************
 * \file	version.cpp
 * \brief	Version-related macros and functions
 *
 * This file is forcibly re-compiled in every build to ensure up-to-date
 * version information. Callers access version information through static
 * members of the Stylus object.
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
#include "config.h"

using namespace std;
using namespace stylus;

//  Macros ----------------------------------------------------------------------
#if defined(ST_DEBUG)
#define STYLUS_BUILDTYPE_STRING "DEBUG"
#elif defined(ST_PROFILE)
#define STYLUS_BUILDTYPE_STRING "PROFILE"
#else
#define STYLUS_BUILDTYPE_STRING "RELEASE"
#endif
#define STYLUS_DATE_STRING		__DATE__ " "  __TIME__
#define STYLUS_COPYRIGHT_STRING	"(c) 2006-2009 Biologic Institute"
#define STYLUS_BUILD_STRING		"Stylus " STYLUS_VERSION " [" STYLUS_BUILDTYPE_STRING " - " STYLUS_DATE_STRING "]"
#define STYLUS_VERSION_STRING	STYLUS_BUILD_STRING " " STYLUS_COPYRIGHT_STRING
#define STYLUS_LIBXML			"LibXML "

const char Globals::s_szBuild[] = STYLUS_BUILD_STRING;
const char Globals::s_szVersion[] = STYLUS_VERSION_STRING;

extern "C"
{
	/*
	 * Function: stGetVersion
	 *
	 */
	ST_RETCODE
	stGetVersion(char* pszVersion, size_t* pcchVersion, bool fAll)
	{
		ENTERPUBLIC(GLOBAL, stGetVersion);

		if (!VALID(pszVersion) || !VALID(pcchVersion) || *pcchVersion <= 0)
			RETURN_BADARGS();

		size_t cbTotal = 0;
		size_t cbRemaining = *pcchVersion;
		size_t cbWritten;

		if (cbTotal > cbRemaining)
		{
			*pcchVersion = cbTotal;
			RETURN_RC(BUFFERTOOSMALL);
		}

		cbWritten = cbRemaining;
		RETURN_IFERROR(copyString(pszVersion, &cbWritten, Globals::s_szVersion));
		cbWritten -= 1;
		pszVersion += cbWritten;
		cbTotal += cbWritten;
		cbRemaining -= cbWritten;

		if (fAll)
		{
			if (cbRemaining <= 1)
				RETURN_RC(BUFFERTOOSMALL);
			*pszVersion++ = Constants::s_chNEWLINE;
			cbTotal += 1;
			cbRemaining -= 1;

			cbWritten = cbRemaining;
			RETURN_IFERROR(copyString(pszVersion, &cbWritten, STYLUS_LIBXML));
			cbWritten -= 1;
			pszVersion += cbWritten;
			cbTotal += cbWritten;
			cbRemaining -= cbWritten;

			cbWritten = cbRemaining;
			RETURN_IFERROR(copyString(pszVersion, &cbWritten, LIBXML_DOTTED_VERSION));
			cbWritten -= 1;
			pszVersion += cbWritten;
			cbTotal += cbWritten;
			cbRemaining -= cbWritten;
		}

		*pszVersion = Constants::s_chNULL;
		*pcchVersion = cbTotal;
		RETURN_SUCCESS();

		EXITPUBLIC(GLOBAL, stGetVersion);
	}
}
