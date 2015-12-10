/*******************************************************************************
 * \file	random.cpp
 * \brief	Stylus RGenerator and CRandlib classes
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

//--------------------------------------------------------------------------------
//
// RGenerator
//
//--------------------------------------------------------------------------------
IRandom* RGenerator::s_prgen = NULL;

/*
 * Function: initialize
 *
 */
void
RGenerator::initialize(const std::string& strUUID)
{
	ENTER(GLOBAL,initialize);
	ASSERT(!EMPTYSTR(strUUID));

	if (strUUID == RandomC::s_strUUID)
		s_prgen = ::new RandomC();
	else
		THROWRC((RC(BADARGUMENTS), "%s does not identify a known Random Generator", strUUID.c_str()));
}

/*
 * Function: terminate
 *
 */
void
RGenerator::terminate()
{
	ENTER(GLOBAL,terminate);

	::delete s_prgen;
}

/*
 * Function: loadSeed
 *
 */
void
RGenerator::loadSeed(XMLDocument* pxd, xmlNodePtr pxnSeed)
{
	ENTER(GLOBAL,loadSeed);

	ASSERT(s_prgen);
	ASSERT(pxd);
	ASSERT(pxnSeed);

	string strValue;
	pxd->getContent(pxnSeed, strValue);

	TDATA(GLOBAL,L2,(LLTRACE, "Initializing random generator with seed \"%s\"", strValue.c_str()));
	s_prgen->setSeed(strValue);
}
