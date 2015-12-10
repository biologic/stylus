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

    _pmersenne->SetState(strSeed);
}

/*
 * Function: getSeed
 *
 */
std::string
RandomC::getSeed() const
{
	ENTER(GLOBAL,getSeed);
    return _pmersenne->GetState();
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
