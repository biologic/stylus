/*******************************************************************************
 * \file	randomc.cpp
 * \brief	Stylus Randomc classes
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
// Helpers
//
//--------------------------------------------------------------------------------

/* 
 * Function: lennob
 * 
 * Returns the length of str ignoring trailing blanks but not other white space.
 * 
 * Routine copied from the Perl Math::Random-0.69 library
 */
long lennob( char *str )
{
long i, i_nb;

for (i=0, i_nb= -1L; *(str+i); i++)
    if ( *(str+i) != ' ' ) i_nb = i;
return (i_nb+1);
}

/*
 * Function: phraseToSeed
 * 
 * Routine copied from the Perl Math::Random-0.69 library
 */
void
phraseToSeed(char* phrase, int *seed1, int *seed2)
{
static int twop30 = 1073741824L;

static int i,j, ichr,lphr;
static int values[8] = { 8521739, 5266711, 3254959, 2011673, 1243273, 768389, 474899, 293507 };

	*seed1 = 1234567890L;
	*seed2 = 123456789L;
	lphr = lennob(phrase); 
	if(lphr < 1) return;
	for(i=0; i<(lphr-1); i++)
	{
		ichr = phrase[i];
		j = i % 8;
		*seed1 = ( *seed1 + (values[j] * ichr) ) % twop30;
		*seed2 = ( *seed2 + (values[7-j] * ichr) ) % twop30;
	}
}

//--------------------------------------------------------------------------------
//
// RandomC
//
//--------------------------------------------------------------------------------
const std::string RandomC::s_strUUID("4AFBBE2E-00CD-40B6-8A81-816F51B31EF6");

/*
 * Function: RandomC
 *
 */
RandomC::RandomC()
{
	ENTER(GLOBAL,RandomC);
	_pmersenne = new ::CRandomMersenne(0);
}

/*
 * Function: getUUID
 *
 */
const std::string&
RandomC::getUUID() throw()
{
	return s_strUUID;
}

/*
 * Function: setSeed
 *
 */
void
RandomC::setSeed(const std::string& strSeed)
{
	ENTER(GLOBAL,getSeed);
	ASSERT(!EMPTYSTR(strSeed));

	// Seeds beginning with a single or double quote are treated as phrases
	if (strSeed[0] == Constants::s_chQUOTE || strSeed[0] == Constants::s_chAPOSTROPHE)
	{
		phraseToSeed(const_cast<char*>(strSeed.c_str()), &_seeds[0], &_seeds[1]);
	}

	// Otherwise, the seed must contain two long integers
	else
	{
		char* psz;
		_seeds[0] = ::strtol(strSeed.c_str(), &psz, 10);
		if (_seeds[0] == INT_MAX || _seeds[0] == INT_MIN)
			THROWRC((RC(BADARGUMENTS), "%s contains an illegal seed value", strSeed.c_str()));
		if (!VALID(psz))
			THROWRC((RC(BADARGUMENTS), "%s requires two long values separated by a space", strSeed.c_str()));

		_seeds[1] = ::strtol(psz, NULL, 10);
		if (_seeds[1] == INT_MAX || _seeds[1] == INT_MIN)
			THROWRC((RC(BADARGUMENTS), "%s contains an illegal seed value - values must range from %ld to %ld", strSeed.c_str(), LONG_MAX, LONG_MIN));
	}

	_pmersenne->RandomInitByArray(_seeds, 2);
}

/*
 * Function: getSeed
 *
 */
std::string
RandomC::getSeed() const
{
	ENTER(GLOBAL,getSeed);

	ostringstream ostr;

	ostr << _seeds[0] << Constants::s_chBLANK << _seeds[1];
	return ostr.str();
}

void
RandomC::getSeed(XMLStream& xs, STFLAGS grfRecordDetail) const
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
long
RandomC::getUniform(long nLow, long nHigh)
{
	ASSERT(nLow <= nHigh);
	return static_cast<long>(_pmersenne->IRandomX(static_cast<int>(nLow), static_cast<int>(nHigh)));
}

UNIT
RandomC::getUniform()
{
	UNIT value = static_cast<UNIT>(_pmersenne->Random());
	return (value == 0.0 ? Constants::s_nERROR_MARGIN : value);
}
