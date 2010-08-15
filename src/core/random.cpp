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

	// if (strUUID == Randlib::s_strUUID)
	// 	s_prgen = ::new Randlib();
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
 * Function: getUUIDv4
 *
 */
std::string 
RGenerator::getUUIDv4()
{
	ENTER(GLOBAL,getUUIDv4);

	long bytes[16];
	ostringstream ostr;

	// Get random bytes for UUID
	for (size_t index = 0; index < 16; index += 1) {
		bytes[index] = getUniform((long)0, (long)255);
	}

	// Set the UUID variant to be RFC4122
	bytes[7] &= ~0xc0;
	bytes[7] |= 0x80;

	// Set the UUID version number to 4
	bytes[9] &= ~0xf0;
	bytes[9] |= 0x40;

	// Format UUID string
	ostr << uppercase << hex << setfill('0');
	ostr << setw(2) << bytes[15] << setw(2) << bytes[14];
	ostr << setw(2) << bytes[13] << setw(2) << bytes[12] << '-';
	ostr << setw(2) << bytes[11] << setw(2) << bytes[10] << '-';
	ostr << setw(2) << bytes[9]  << setw(2) << bytes[8]  << '-';
	ostr << setw(2) << bytes[7]  << setw(2) << bytes[6]  << '-';
	ostr << setw(2) << bytes[5]  << setw(2) << bytes[4];
	ostr << setw(2) << bytes[3]  << setw(2) << bytes[2];
	ostr << setw(2) << bytes[1]  << setw(2) << bytes[0];

	return ostr.str();
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
