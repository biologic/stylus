/*******************************************************************************
 * \file    constants.hpp
 * \brief   Constants used by Stylus
 * 
 * All constants should be defined within this file unless they are tightly
 * associated with the object.
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

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace org_biologicinstitute_stylus
{

	class Constants
	{
	public:
		static const size_t s_cbmaxBUFFER = (4096 + (2 * FILENAME_MAX));
		static const size_t s_cchmaxURL = 4096;
		static const size_t s_cchmaxFILELINE = 30;
		static const size_t s_cchUUID = 37;
		static const size_t s_cchTIME = 28;
		static const size_t s_nmaxCODONS = 64;

		static const UNIT s_nVECTOR_SHORT = 1;
		static const UNIT s_nVECTOR_MEDIUM = 1.648721270700128;
		static const UNIT s_nVECTOR_LONG = M_E;
		static const UNIT s_nSHORT_DIAGONALSTEP = M_SQRT1_2;
		static const UNIT s_nMEDIUM_DIAGONALSTEP = 1.165821990798562;

		static const size_t s_nMAXIMUMGENERATEDLENGTH = (3 * 3);	///< Maximum generated length (up to 3 trivectors)

		static const char s_chNULL = '\0';

		static const char s_chBASET = 'T';
		static const char s_chBASEC = 'C';
		static const char s_chBASEA = 'A';
		static const char s_chBASEG = 'G';
		static const std::string s_strBASES;
		static const std::string s_strBASESTC;
		static const std::string s_strBASESAG;

		static const char s_chAMPERSAND = '&';
		static const char s_chAPOSTROPHE = '\'';
		static const char s_chATSIGN = '@';
		static const char s_chBLANK = ' ';
		static const char s_chCOLON = ':';
		static const char s_chCOMMA = ',';
		static const char s_chDASH = '-';
		static const char s_chDOT = '.';
		static const char s_chEQUAL = '=';
		static const char s_chFORWARDSLASH = '/';
		static const char s_chGREATERTHAN = '>';
		static const char s_chGMTZONE = 'Z';
		static const char s_chLESSTHAN = '<';
		static const char s_chMINUS = '-';
		static const char s_chNEWLINE = '\n';
		static const char s_chPERCENT = '%';
		static const char s_chPLUS = '+';
		static const char s_chPOUND = '#';
		static const char s_chQUESTION = '?';
		static const char s_chQUOTE = '"';
		static const char s_chSEMICOLON = ';';

		static const char s_chLBRACKET = '[';
		static const char s_chRBRACKET = ']';

		static const char s_chLEFTPARENTHESIS = '(';
		static const char s_chRIGHTPARENTHESIS = ')';

		static const int s_nHTTP_CONTINUE = 100;
		static const int s_nHTTP_SWITCHING_PROTOCOLS = 101;
		static const int s_nHTTP_PROCESSING = 102;
		static const int s_nHTTP_OK = 200;
		static const int s_nHTTP_CREATED = 201;
		static const int s_nHTTP_ACCEPTED = 202;
		static const int s_nHTTP_NON_AUTHORITATIVE = 203;
		static const int s_nHTTP_NO_CONTENT = 204;
		static const int s_nHTTP_RESET_CONTENT = 205;
		static const int s_nHTTP_PARTIAL_CONTENT = 206;
		static const int s_nHTTP_MULTI_STATUS = 207;
		static const int s_nHTTP_MULTIPLE_CHOICES = 300;
		static const int s_nHTTP_MOVED_PERMANENTLY = 301;
		static const int s_nHTTP_MOVED_TEMPORARILY = 302;
		static const int s_nHTTP_SEE_OTHER = 303;
		static const int s_nHTTP_NOT_MODIFIED = 304;
		static const int s_nHTTP_USE_PROXY = 305;
		static const int s_nHTTP_TEMPORARY_REDIRECT = 307;
		static const int s_nHTTP_BAD_REQUEST = 400;
		static const int s_nHTTP_UNAUTHORIZED = 401;
		static const int s_nHTTP_PAYMENT_REQUIRED = 402;
		static const int s_nHTTP_FORBIDDEN = 403;
		static const int s_nHTTP_NOT_FOUND = 404;
		static const int s_nHTTP_METHOD_NOT_ALLOWED = 405;
		static const int s_nHTTP_NOT_ACCEPTABLE = 406;
		static const int s_nHTTP_PROXY_AUTHENTICATION_REQUIRED = 407;
		static const int s_nHTTP_REQUEST_TIME_OUT = 408;
		static const int s_nHTTP_CONFLICT = 409;
		static const int s_nHTTP_GONE = 410;
		static const int s_nHTTP_LENGTH_REQUIRED = 411;
		static const int s_nHTTP_PRECONDITION_FAILED = 412;
		static const int s_nHTTP_REQUEST_ENTITY_TOO_LARGE = 413;
		static const int s_nHTTP_REQUEST_URI_TOO_LARGE = 414;
		static const int s_nHTTP_UNSUPPORTED_MEDIA_TYPE = 415;
		static const int s_nHTTP_RANGE_NOT_SATISFIABLE = 416;
		static const int s_nHTTP_EXPECTATION_FAILED = 417;
		static const int s_nHTTP_UNPROCESSABLE_ENTITY = 422;
		static const int s_nHTTP_LOCKED = 423;
		static const int s_nHTTP_FAILED_DEPENDENCY = 424;
		static const int s_nHTTP_INTERNAL_SERVER_ERROR = 500;
		static const int s_nHTTP_NOT_IMPLEMENTED = 501;
		static const int s_nHTTP_BAD_GATEWAY = 502;
		static const int s_nHTTP_SERVICE_UNAVAILABLE = 503;
		static const int s_nHTTP_GATEWAY_TIME_OUT = 504;
		static const int s_nHTTP_VERSION_NOT_SUPPORTED = 505;
		static const int s_nHTTP_VARIANT_ALSO_VARIES = 506;
		static const int s_nHTTP_INSUFFICIENT_STORAGE = 507;
		static const int s_nHTTP_NOT_EXTENDED = 510;

		static const std::string s_strEMPTY;
		
		static const std::string s_strDEFAULTSCOPE;
		
		static const std::string s_strGLOBALS;
		static const std::string s_strINITIAL;
		static const std::string s_strFINAL;
		static const std::string s_strTRIAL;
		static const std::string s_strHISTORY;
		static const std::string s_strPLAN;
		
		static const std::string s_strENTER;
		static const std::string s_strEXIT;

		static const std::string s_strRETCODE;
		static const std::string s_strSTANDARDEXCEPTION;
		static const std::string s_strUNKNOWNEXCEPTION;

		static const std::string s_strBLANKS;
		static const std::string s_strSEPARATOR;

		static const std::string s_strXMLDOCTYPE;
		static const std::string s_strMIMETYPE_XML;
		static const std::string s_strXMLEXTENSION;
		static const std::string s_strXMLTRUE;

		static const std::string s_strHANEXTENSION;
		static const std::string s_strGENEEXTENSION;

		static const std::string s_strENCODING_UTF8;
		
		static const std::string s_strURL_DEFAULTSCOPE;

		static const std::string s_strURIScheme_EMPTY;
		static const std::string s_strURIScheme_HTTP;
		static const std::string s_strURIScheme_FILE;
	};
}      // namespace org_biologicinstitute_stylus
#endif // CONSTANTS_HPP
