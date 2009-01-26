/*******************************************************************************
 * \file	random.inl
 * \brief	Stylus RGenerator and Randlib classes inline methods
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
// RGenerator
//
//--------------------------------------------------------------------------------
inline const std::string& RGenerator::getUUID() throw() { ASSERT(s_prgen); return s_prgen->getUUID(); }
		
inline void RGenerator::setSeed(const std::string& strSeed) { ASSERT(s_prgen); s_prgen->setSeed(strSeed); }
inline std::string RGenerator::getSeed() { ASSERT(s_prgen); return s_prgen->getSeed(); }
inline void RGenerator::getSeed(XMLStream& xs, STFLAGS grfRecordDetail) { ASSERT(s_prgen); return s_prgen->getSeed(xs, grfRecordDetail); }

inline UNIT RGenerator::getUniform(UNIT nLow, UNIT nHigh) { ASSERT(s_prgen); return s_prgen->getUniform(nLow,nHigh); }
inline long RGenerator::getUniform(long nLow, long nHigh) { ASSERT(s_prgen); return s_prgen->getUniform(nLow,nHigh); }
inline UNIT RGenerator::getUniform() { ASSERT(s_prgen); return s_prgen->getUniform(); }

