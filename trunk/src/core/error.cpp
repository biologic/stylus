/*******************************************************************************
 * \file error.cpp
 * \brief Error class (see error.hpp for details)
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

char Error::s_szBuffer[Constants::s_cbmaxBUFFER];

Error Error::s_aryErrors[10];
size_t Error::s_iError = 0;
size_t Error::s_cErrors = 0;

/*
 * Function: setNextError
 *
 */
const Error*
Error::setNextError(const char* pszFileline, ST_RETCODE rc, const char* pszFormat, ...) throw()
{
	s_cErrors += 1;
	s_iError += 1;
	if (s_iError >= ARRAY_LENGTH(s_aryErrors))
		s_iError = 0;

	Error* pe = ::new (&s_aryErrors[s_iError]) Error(rc);

	if (VALID(pszFileline))
    {
		size_t cchFileline = ::strlen(pszFileline);
		const char* psz;
		size_t cb;

		cb = CCH_OF(pe->_szFileline);
		copyBytes(pe->_szFileline, &cb, Constants::s_strBLANKS.c_str(), cb);

		cb = min<size_t>(cchFileline, ARRAY_LENGTH(pe->_szFileline));
		psz = pszFileline + cchFileline - cb;
		copyBytes(pe->_szFileline, &cb, psz, cb);
    }

	va_list ap;
	va_start(ap, pszFormat);
	::vsnprintf(pe->_szDesc, CCH_OF(pe->_szDesc), pszFormat, ap);
	va_end(ap);

	if (ST_ISTYPEERROR(rc))
		Globals::log(STLL_ERROR, pszFileline, pe->_szDesc);

	else if (ST_ISTYPEWARNING(rc))
		Globals::log(STLL_WARNING, pszFileline, pe->_szDesc);

	else if (ST_ISTYPEINFO(rc))
		Globals::log(STLL_INFO, pszFileline, pe->_szDesc);

	return pe;
}

/*
 * Function: clearErrors
 *
 */
void
Error::clearErrors() throw()
{
	for (size_t i=0; i < ARRAY_LENGTH(s_aryErrors); i++)
	{
		::new (&s_aryErrors[i]) Error();
	}
	s_iError = 0;
}

/*
 * Function: getLastError
 *
 */
const Error&
Error::getLastError() throw()
{
	return s_aryErrors[s_iError];
}

/*
 * Function: Error
 *
 */
Error::Error() throw()
{
	timeNow(&_tError);
	_rc = ST_RCERROR;

	_szFileline[0] = Constants::s_chNULL;
	_szDesc[0] = Constants::s_chNULL;
}

Error::Error(ST_RETCODE rc) throw()
{
	timeNow(&_tError);
	_rc = rc;

	_szFileline[0] = Constants::s_chNULL;
	_szDesc[0] = Constants::s_chNULL;
}

Error::Error(const Error& e) throw()
{
    *this = e;
}

/*
 * Function: operator=
 *
 */
Error&
Error::operator=(const Error& e) throw()
{
    _tError = e._tError;
	_rc = e._rc;

	size_t cch = CCH_OF(_szFileline);
	copyString(_szFileline, &cch, e._szFileline);

	cch = CCH_OF(_szDesc);
	copyString(_szDesc, &cch, e._szDesc);

	return *this;
}

/*
 * Function: toCString
 *
 */
const char*
Error::toCString() const throw()
{
	char* pszMsg = s_szBuffer;
	size_t cbRemaining = SZ_OF(s_szBuffer);
	size_t cbWritten;

	cbWritten = timeToString(pszMsg, &_tError);
	pszMsg += cbWritten;
	cbRemaining -= cbWritten;
	if (cbRemaining <= 0)
		goto EXIT;

	*pszMsg++ = Constants::s_chBLANK;
	cbRemaining -= 1;
	if (cbRemaining <= 0)
		goto EXIT;

	*pszMsg++ = Constants::s_chLBRACKET;
	cbRemaining -= 1;
	if (cbRemaining <= 0)
		goto EXIT;

	cbWritten = cbRemaining;
	copyBytes(pszMsg, &cbWritten, _szFileline, CCH_OF(_szFileline));
	pszMsg += cbWritten - 1;
	cbRemaining -= cbWritten - 1;
	if (cbRemaining <= 0)
		goto EXIT;

	*pszMsg++ = Constants::s_chRBRACKET;
	cbRemaining -= 1;
	if (cbRemaining <= 0)
		goto EXIT;

	*pszMsg++ = Constants::s_chBLANK;
	cbRemaining -= 1;
	if (cbRemaining <= 0)
		goto EXIT;

	cbWritten = cbRemaining;
	copyString(pszMsg, &cbWritten, _szDesc);
	pszMsg += cbWritten - 1;

EXIT:
	*pszMsg = Constants::s_chNULL;
	return s_szBuffer;
}
