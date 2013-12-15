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

const UNIT Constants::s_nVECTOR_SHORT = 1;
const UNIT Constants::s_nVECTOR_MEDIUM = 1.648721270700128;
const UNIT Constants::s_nVECTOR_LONG = M_E;
const UNIT Constants::s_nSHORT_DIAGONALSTEP = M_SQRT1_2;
const UNIT Constants::s_nMEDIUM_DIAGONALSTEP = 1.165821990798562;
const UNIT Constants::s_nERROR_MARGIN = 0.0000000001;

const std::string Constants::s_strBASES("TACG");
const std::string Constants::s_strBASESTC("TC");
const std::string Constants::s_strBASESAG("AG");

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

