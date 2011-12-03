/*******************************************************************************
 * \file    randomc.hpp
 * \brief   RandomC implementation classes
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

#ifndef RANDOMC_HPP
#define RANDOMC_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief RandomC-based implementation of IRandom
	 *
	 * This class depends on RandomC being linked into Stylus.
	 */
	class RandomC : public IRandom
	{
		friend class RGenerator;

	private:
		CRandomMersenne* _pmersenne;

		RandomC();
		
	public:
		static const std::string s_strUUID;		///< UUID used to request RandomC

		inline virtual ~RandomC() {};

		/**
		 * \brief Implementation of IRandom (see IRandom for details)
		 *
		 */
		//{@
		virtual const std::string& getUUID() throw();

		virtual void setSeed(const std::string& strSeed);
		virtual std::string getSeed() const;
		virtual void getSeed(XMLStream& xs, STFLAGS grfRecordDetail) const;

		virtual long getUniform(long nLow, long nHigh);
		virtual UNIT getUniform();
		//@}
	};

}	// namespace org_biologicinstitute_stylus
#endif // RANDOMC_HPP
