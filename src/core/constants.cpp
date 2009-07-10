/*******************************************************************************
 * \file    constants.cpp
 * \brief   Constants used by Stylus (see constants.hpp for details)
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

const size_t Constants::s_cbmaxBUFFER;
const size_t Constants::s_cchmaxURL;
const size_t Constants::s_cchmaxFILELINE;
const size_t Constants::s_cchUUID;
const size_t Constants::s_cchTIME;

const UNIT Constants::s_nVECTOR_SHORT;
const UNIT Constants::s_nVECTOR_MEDIUM;
const UNIT Constants::s_nVECTOR_LONG;
const UNIT Constants::s_nSHORT_DIAGONALSTEP;
const UNIT Constants::s_nMEDIUM_DIAGONALSTEP;
const UNIT Constants::s_nERROR_MARGIN;

const size_t Constants::s_nMAXIMUMGENERATEDLENGTH;

const char Constants::s_chNULL;

const char Constants::s_chBASET;
const char Constants::s_chBASEC;
const char Constants::s_chBASEA;
const char Constants::s_chBASEG;
const std::string Constants::s_strBASES("TACG");
const std::string Constants::s_strBASESTC("TC");
const std::string Constants::s_strBASESAG("AG");

const char Constants::s_chAMPERSAND;
const char Constants::s_chAPOSTROPHE;
const char Constants::s_chATSIGN;
const char Constants::s_chBLANK;
const char Constants::s_chCOLON;
const char Constants::s_chCOMMA;
const char Constants::s_chDASH;
const char Constants::s_chDOT;
const char Constants::s_chEQUAL;
const char Constants::s_chFORWARDSLASH;
const char Constants::s_chGREATERTHAN;
const char Constants::s_chGMTZONE;
const char Constants::s_chLESSTHAN;
const char Constants::s_chMINUS;
const char Constants::s_chNEWLINE;
const char Constants::s_chPERCENT;
const char Constants::s_chPLUS;
const char Constants::s_chPOUND;
const char Constants::s_chQUESTION;
const char Constants::s_chQUOTE;
const char Constants::s_chSEMICOLON;

const char Constants::s_chLBRACKET;
const char Constants::s_chRBRACKET;

const char Constants::s_chLEFTPARENTHESIS;
const char Constants::s_chRIGHTPARENTHESIS;

const int Constants::s_nHTTP_CONTINUE;
const int Constants::s_nHTTP_SWITCHING_PROTOCOLS;
const int Constants::s_nHTTP_PROCESSING;
const int Constants::s_nHTTP_OK;
const int Constants::s_nHTTP_CREATED;
const int Constants::s_nHTTP_ACCEPTED;
const int Constants::s_nHTTP_NON_AUTHORITATIVE;
const int Constants::s_nHTTP_NO_CONTENT;
const int Constants::s_nHTTP_RESET_CONTENT;
const int Constants::s_nHTTP_PARTIAL_CONTENT;
const int Constants::s_nHTTP_MULTI_STATUS;
const int Constants::s_nHTTP_MULTIPLE_CHOICES;
const int Constants::s_nHTTP_MOVED_PERMANENTLY;
const int Constants::s_nHTTP_MOVED_TEMPORARILY;
const int Constants::s_nHTTP_SEE_OTHER;
const int Constants::s_nHTTP_NOT_MODIFIED;
const int Constants::s_nHTTP_USE_PROXY;
const int Constants::s_nHTTP_TEMPORARY_REDIRECT;
const int Constants::s_nHTTP_BAD_REQUEST;
const int Constants::s_nHTTP_UNAUTHORIZED;
const int Constants::s_nHTTP_PAYMENT_REQUIRED;
const int Constants::s_nHTTP_FORBIDDEN;
const int Constants::s_nHTTP_NOT_FOUND;
const int Constants::s_nHTTP_METHOD_NOT_ALLOWED;
const int Constants::s_nHTTP_NOT_ACCEPTABLE;
const int Constants::s_nHTTP_PROXY_AUTHENTICATION_REQUIRED;
const int Constants::s_nHTTP_REQUEST_TIME_OUT;
const int Constants::s_nHTTP_CONFLICT;
const int Constants::s_nHTTP_GONE;
const int Constants::s_nHTTP_LENGTH_REQUIRED;
const int Constants::s_nHTTP_PRECONDITION_FAILED;
const int Constants::s_nHTTP_REQUEST_ENTITY_TOO_LARGE;
const int Constants::s_nHTTP_REQUEST_URI_TOO_LARGE;
const int Constants::s_nHTTP_UNSUPPORTED_MEDIA_TYPE;
const int Constants::s_nHTTP_RANGE_NOT_SATISFIABLE;
const int Constants::s_nHTTP_EXPECTATION_FAILED;
const int Constants::s_nHTTP_UNPROCESSABLE_ENTITY;
const int Constants::s_nHTTP_LOCKED;
const int Constants::s_nHTTP_FAILED_DEPENDENCY;
const int Constants::s_nHTTP_INTERNAL_SERVER_ERROR;
const int Constants::s_nHTTP_NOT_IMPLEMENTED;
const int Constants::s_nHTTP_BAD_GATEWAY;
const int Constants::s_nHTTP_SERVICE_UNAVAILABLE;
const int Constants::s_nHTTP_GATEWAY_TIME_OUT;
const int Constants::s_nHTTP_VERSION_NOT_SUPPORTED;
const int Constants::s_nHTTP_VARIANT_ALSO_VARIES;
const int Constants::s_nHTTP_INSUFFICIENT_STORAGE;
const int Constants::s_nHTTP_NOT_EXTENDED;

const std::string Constants::s_strEMPTY;

const std::string Constants::s_strDEFAULTSCOPE("http://biologicinstitute.net/stylus");

const std::string Constants::s_strGLOBALS("globals");
const std::string Constants::s_strINITIAL("initial");
const std::string Constants::s_strFINAL("final");
const std::string Constants::s_strTRIAL("trial");
const std::string Constants::s_strHISTORY("history");
const std::string Constants::s_strPLAN("plan");

const std::string Constants::s_strENTER("ENTER: %s");
const std::string Constants::s_strEXIT("EXIT : %s");

const std::string Constants::s_strRETCODE("Error occurred - return code: 0x%X");
const std::string Constants::s_strSTANDARDEXCEPTION("C++ Library Exception");
const std::string Constants::s_strUNKNOWNEXCEPTION("Unknown Internal Exception");

const std::string Constants::s_strBLANKS(
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    "
	"                                                                                                    ");
const std::string Constants::s_strSEPARATOR(" - ");

const std::string Constants::s_strXMLDOCTYPE("<?xml version='1.0' encoding='UTF-8' ?>");
const std::string Constants::s_strMIMETYPE_XML("text/xml");
const std::string Constants::s_strXMLEXTENSION(".xml");
const std::string Constants::s_strXMLTRUE("true");

const std::string Constants::s_strHANEXTENSION(".han");
const std::string Constants::s_strGENEEXTENSION(".gene");

const std::string Constants::s_strENCODING_UTF8("charset=utf-8");

const std::string Constants::s_strURL_DEFAULTSCOPE("http://biologicinstitute.net/stylus");

const std::string Constants::s_strURIScheme_EMPTY("");
const std::string Constants::s_strURIScheme_HTTP("http://");
const std::string Constants::s_strURIScheme_FILE("file://");

