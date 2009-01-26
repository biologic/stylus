/*******************************************************************************
 * \file    error.hpp
 * \brief   Error class
 *
 * Error is a top-level class in Stylus. Unlike the standard library exceptions,
 * Stylus exceptions are not distinguished by type, but by content.
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

#ifndef ERROR_HPP
#define ERROR_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Error class
	 *
	 */
	class Error
	{
	public:
		/**
		 * \brief Set the next error in the error stack
		 *
		 * \param[in] pszFileline Location of the error
		 * \param[in] rc Associated Stylus return code
		 * \param[in] pszFormat Format string (for use in vsnprintf)
		 * \param[in] ... Optional arguments referenced in pszFormat
		 */
		static const Error* setNextError(const char* pszFileline, ST_RETCODE rc, const char* pszFormat, ...) throw();
		
		/**
		 * \brief Throw the passed Error - only if an exception is not outstanding
		 * 
		 * \param[in] pe Pointer to Error to (possibly) throw
		 */
		static void safeThrow(const Error* pe);

		/**
		 * \brief Clear the error stack
		 */
		static void clearErrors() throw();

		///< TODO: Implement return of entire error stack
		/**
		 * \brief Retrieve the last error
		 */
		static const Error& getLastError() throw();

		Error() throw();
		Error(ST_RETCODE rc) throw() ;

		Error& operator=(const Error& e) throw();

		/**
		 * \brief Methods to interrogate the error
		 *
		 */
		//{@
		time_t getTime() const throw();

		ST_RETCODE getRetcode() const throw();
		bool isRetcode(ST_RETCODE rc) const throw();

		const char* getFileline() const throw();
		bool hasLocation() const throw();

		const char* getDescription() const throw();
		//@}

		/**
		 * \brief Return the error as a string (using a global buffer)
		 * \remarks
		 * - All errors share the same global buffer; each call overwrites the data previously held
		 */
		const char* toCString() const throw();

	private:
		static const char s_szERRORHEADER[];

		static char s_szBuffer[Constants::s_cbmaxBUFFER];

		static Error s_aryErrors[10];
		static size_t s_iError;
		static size_t s_cErrors;
		
		Error(const Error& e) throw();

		utime _tError;
		ST_RETCODE _rc;
		char _szFileline[Constants::s_cchmaxFILELINE];
		char _szDesc[Constants::s_cbmaxBUFFER];
	};
}	// namespace org_biologicinstitute_stylus
#endif // ERROR_HPP
