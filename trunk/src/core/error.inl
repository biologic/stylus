/*******************************************************************************
 * \file	error.inl
 * \brief	Stylus Error class inline methods
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
// Error
//
//--------------------------------------------------------------------------------
inline void Error::safeThrow(const Error* pe)
{
	if (!::uncaught_exception())
		throw pe;
	else
		LOGWARNING((LLWARNING, "Nested exceptions prevented"));
}
inline time_t Error::getTime() const throw() { return _tError; }
inline ST_RETCODE Error::getRetcode() const throw() { return _rc; }
inline bool Error::isRetcode(ST_RETCODE rc) const throw() { return (_rc == rc); }
inline const char* Error::getFileline() const throw() {	return _szFileline; }
inline bool Error::hasLocation() const throw() { return _szFileline[0] != Constants::s_chNULL; }
inline const char* Error::getDescription() const throw() { return _szDesc; }

