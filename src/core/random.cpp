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

	if (strUUID == Randlib::s_strUUID)
		s_prgen = ::new Randlib();
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
	ostr << hex;
	ostr << bytes[15] << bytes[14] << bytes[13] << bytes[12] << '-';
	ostr << bytes[11] << bytes[10] << '-';
	ostr << bytes[9] << bytes[8]<< '-';
	ostr << bytes[7] << bytes[6] << '-';
	ostr << bytes[5] << bytes[4] << bytes[3] << bytes[2] << bytes[1] << bytes[0];

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


//--------------------------------------------------------------------------------
//
// Randlib
//
//--------------------------------------------------------------------------------
const std::string Randlib::s_strUUID("B7C9BB5D-C495-411D-82B2-1929FD30A7A3");

/*
 * Function: Randlib
 *
 */
Randlib::Randlib()
{
	ENTER(GLOBAL,Randlib);
	// Generate an arbitrary random number for force library initialization
	// NOTE:
	// - Setting the seed will undo any effect of this call
	::ranf();
}

/*
 * Function: getUUID
 *
 */
const std::string&
Randlib::getUUID() throw()
{
	return s_strUUID;
}

/*
 * Function: setSeed
 *
 */
void
Randlib::setSeed(const std::string& strSeed)
{
	ENTER(GLOBAL,getSeed);
	ASSERT(!EMPTYSTR(strSeed));

	long nSeed1;
	long nSeed2;

	// Seeds beginning with a single or double quote are treated as phrases
	if (strSeed[0] == Constants::s_chQUOTE || strSeed[0] == Constants::s_chAPOSTROPHE)
	{
		::phrtsd(const_cast<char*>(strSeed.c_str()), &nSeed1, &nSeed2);
	}

	// Otherwise, the seed must contain two long integers
	else
	{
		char* psz;
		nSeed1 = ::strtol(strSeed.c_str(), &psz, 10);
		if (nSeed1 == LONG_MAX || nSeed1 == LONG_MIN)
			THROWRC((RC(BADARGUMENTS), "%s contains an illegal seed value", strSeed.c_str()));
		if (!VALID(psz))
			THROWRC((RC(BADARGUMENTS), "%s requires two long values separated by a space", strSeed.c_str()));

		nSeed2 = ::strtol(psz, NULL, 10);
		if (nSeed2 == LONG_MAX || nSeed2 == LONG_MIN)
			THROWRC((RC(BADARGUMENTS), "%s contains an illegal seed value - values must range from %ld to %ld", strSeed.c_str(), LONG_MAX, LONG_MIN));
	}

	::setall(nSeed1, nSeed2);
}

/*
 * Function: getSeed
 *
 */
std::string
Randlib::getSeed() const
{
	ENTER(GLOBAL,getSeed);

	long iSeed1;
	long iSeed2;

	::getsd(&iSeed1, &iSeed2);

	ostringstream ostr;

	ostr << iSeed1 << Constants::s_chBLANK << iSeed2;
	return ostr.str();
}

void
Randlib::getSeed(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GLOBAL,getSeed);
	
	xs.openStart(xmlTag(XT_SEED));
	xs.writeAttribute(xmlTag(XT_PROCESSORID), RGenerator::getUUID());
	xs.closeStart(true, false);
	xs.writeContent(getSeed());
	xs.writeEnd(xmlTag(XT_SEED));
}

/*
 * Function: getUniform
 *
 */
UNIT
Randlib::getUniform(UNIT nLow, UNIT nHigh)
{
	ASSERT(nLow < nHigh);
	return ::genunf(static_cast<double>(nLow), static_cast<double>(nHigh));
}

long
Randlib::getUniform(long nLow, long nHigh)
{
	ASSERT(nLow <= nHigh);
	return ::ignuin(nLow, nHigh);
}

UNIT
Randlib::getUniform()
{
	return getUniform(static_cast<UNIT>(0),static_cast<UNIT>(1));
}
	
