/*******************************************************************************
 * \file    random.hpp
 * \brief   Random number generator interface and implementation classes
 *
 * IRandom defines interface to the random number generator Stylus uses.
 * Randlib implements the interface using the Randlib library.
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

#ifndef RANDOM_HPP
#define RANDOM_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Random number generator interface
	 *
	 * Random generators create psuedo-random numbers from an acceptably
	 * large sequence. The details depend upon the generator used. In all cases,
	 * re-seeding a generator restores it to state; effectively causing it
	 * to reproduce a series of numbers.
	 *
	 * \remarks
	 * - The interpretation of the seed, either as a opaque numeric string or a
	 *   text phrase, depends on the random generator used.
	 */
	class IRandom
	{
	public:
		inline virtual ~IRandom() {};
		
		/**
		 * \brief Retrieve the random generators UUID
		 *
		 */
		virtual const std::string& getUUID() throw() = 0;
		
		/**
		 * \brief Initialize the random number seed from the passed string
		 * \param[in] strSeed String containing the seed
		 */
		virtual void setSeed(const std::string& strSeed) = 0;

		/**
		 * \brief Retrieve the current seed of the random generator
		 *
		 * The seed returned may be used to reset the generator to the
		 * \em same state; effectively reproducing a previous series of
		 * random numbers.
		 */
		virtual std::string getSeed() const = 0;

		/**
		 * \brief Write the seed to an XML string
		 * \param[in] xs Target XMLStream
		 * \param[in] grfRecordDetail Detail level to include
		 */
		virtual void getSeed(XMLStream& xs, STFLAGS grfRecordDetail) const = 0;

		/**
		 * \brief Generate a uniformly distributed real random number within the range
		 * \param[in] nLow Low end of the range (exclusive)
		 * \param[in] nHigh High end of the range (exclusive)
		 */
		virtual UNIT getUniform(UNIT nLow, UNIT nHigh) = 0;

		/**
		 * \brief Generate a uniformly distributed integer random number within the range
		 * \param[in] nLow Low end of the range (inclusive)
		 * \param[in] nHigh High end of the range (inclusive)
		 */
		virtual long getUniform(long nLow, long nHigh) = 0;

		/**
		 * \brief Generate a uniformly distributed real random number between 0 and 1
		 * \remarks
		 * - Neither 0 nor 1 are ever returned
		 */
		virtual UNIT getUniform() = 0;
	};

	/**
	 * \brief Global wrapper over active random generator
	 *
	 */
	class RGenerator
	{
	public:
		/**
		 * \brief Initialize the random generator
		 *
		 * Random number generators implement the IRandom interface (of which
		 * this static method is part). Each generator is identified by its
		 * own UUID; there is no pattern or subtyping to the UUIDs. Initialization
		 * creates the selected random generator, making it available to
		 * all other code. Terminate release it.
		 */
		static void initialize(const std::string& strUUID);
		static void terminate();

		/**
		 * \brief Generate a v4 UUID as per RFC 4122
		 *
		 * Use the random number generator to generate a v4 UUID.
		 */
		static std::string getUUIDv4();

		/**
		 * \brief Methods that statically reroute to the active random generator
		 *
		 */
		//{@
		static const std::string& getUUID() throw();
		
		static void loadSeed(XMLDocument* pxd, xmlNodePtr pxnSeed);
		static void setSeed(const std::string& strSeed);
		static std::string getSeed();
		static void getSeed(XMLStream& xs, STFLAGS grfRecordDetail);

		static UNIT getUniform(UNIT nLow, UNIT nHigh);
		static long getUniform(long nLow, long nHigh);
		static UNIT getUniform();
		//@}

	private:
		static IRandom* s_prgen;				///< Active random generator
	};

	/**
	 * \brief Randlib-based implementation of IRandom
	 *
	 * This class depends on Randlib being linked into Stylus. Since Randlib maintains
	 * all its own state, this class provides an IRandom-compatible interface only
	 * and maintains no private state of its own.
	 */
	class Randlib : public IRandom
	{
		friend class RGenerator;

	private:
		Randlib();
		
	public:
		static const std::string s_strUUID;		///< UUID used to request Randlib

		inline virtual ~Randlib() {};

		/**
		 * \brief Implementation of IRandom (see IRandom for details)
		 *
		 */
		//{@
		virtual const std::string& getUUID() throw();

		virtual void setSeed(const std::string& strSeed);
		virtual std::string getSeed() const;
		virtual void getSeed(XMLStream& xs, STFLAGS grfRecordDetail) const;

		virtual UNIT getUniform(UNIT nLow, UNIT nHigh);
		virtual long getUniform(long nLow, long nHigh);
		virtual UNIT getUniform();
		//@}
	};

}	// namespace org_biologicinstitute_stylus
#endif // RANDOM_HPP
