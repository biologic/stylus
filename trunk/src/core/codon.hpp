/*******************************************************************************
 * \file	codon.hpp
 * \brief	Stylus Codon class
 *
 * The Codon class encapsulates basic constants and methods related to
 * Stylus codons. It also serves to abstract the rest of Stylus from
 * the specific codon table, leaving open the possibility of using other
 * codon tables.
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

#ifndef CODONS_HPP
#define CODONS_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Acid class
	 *
	 * \remarks
	 * - This is a POD class (plain old data) to allow brace-enclosed initialization
	 */
	class Acid
	{
		friend Point& operator+(Point& pt, ACIDTYPE at);
		friend Point& operator+=(Point& pt, ACIDTYPE at);
		friend Point& operator-(Point& pt, ACIDTYPE at);
		friend Point& operator-=(Point& pt, ACIDTYPE at);

	private:
		static const Acid s_aryACIDS[ACID_MAX];

	public:
		static const Acid& typeToAcid(ACIDTYPE at);

		/**
		 * \brief Convert the passed vector to the equivalent ACIDTYPE
		 *
		 * This routine converts a three character vector name into the corresponding acid.
		 * 
		 * \param[in] pszVector Pointer to first of three characters forming the vector name (e.g., NoL)
		 */
		static ACIDTYPE vectorToType(const char* pszVector);

		/**
		 * \brief Return true if the acid is a stop codon
		 */
		bool isStop() const;

		/**
		 * \brief Return dx/dy step
		 */
		//{@
		UNIT getDX() const;
		UNIT getDY() const;
		//@}

		/**
		 * \brief Return the acid length
		 */
		UNIT getLength() const;

		/**
		 * \brief Return the acid direction
		 */
		DIRECTION getDirection() const;

		/**
		 * \brief Return the acid name
		 */
		const char* getName() const;

		/**
		 * \brief Return true/false if the direction is horizontal, vertical, or diagonal
		 */
		//{@
		bool isHorizontal() const;
		bool isVertical() const;
		bool isDiagonal() const;
		//@}

		Unit _dx;
		Unit _dy;
		Unit _nLength;
		DIRECTION _nDirection;
		const char* _pszName;
	};

	/**
	 * \brief Advance the passed Point by the dx/dy for an acid
	 * \param[in] pt The Point to move
	 * \param[in] at The ACIDTYPE to retrieve
	 */
	//{@
	Point& operator+(Point& pt, ACIDTYPE at);
	Point& operator+=(Point& pt, ACIDTYPE at);
	//@}

	/**
	 * \brief Retreat the passed Point by the dx/dy for an acid
	 * \param[in] pt The Point to move
	 * \param[in] at The ACIDTYPE to retrieve
	 */
	//{@
	Point& operator-(Point& pt, ACIDTYPE at);
	Point& operator-=(Point& pt, ACIDTYPE at);
	//@}

	/**
	 * \brief ScaledLength class
	 *
	 * This class helps track the scaled length of a vector sequence.
	 *
	 * \remarks
	 * - The dimensions are held separately since each dimension is scaled separately
	 * - Diagonal lengths are calculated from the x-step of the vector. This is correct
	 *   because:
	 *   - The dx == dy for all diagonal vectors
	 *   - The scale factor (sxyToHan) is computed to create the approrpiate Han length
	 *        ((dx*sx)^2)+((dy*sy)^2) == (HanDx^2) + (HanDy^2) and, since dx == dy...
	 *        (dx^2)((sx^2)+(sy^2)) == (Han Diagonal Length)^2 and, taking square root...
	 *        dx * sxyToHan = Han Diagonal Length, where sxyToHan == sqrt((sx^2)+(sy^2))
	 * - It is \em imperative that this class apply the scale factors to the summed
	 *   vectors instead of scaling the vectors during summation. Other code (such as that
	 *   which computes the points along a Gene) must compute lengths without scaling (since
	 *   scaling is known only to the object, like the Stroke, over the range of bases).
	 *   Since that code must apply scaling to the summed lengths, so must this code. If
	 *   their order differs, round-off error will cause different values leading to errors.
	 *   See Stroke::calcScore for code that must apply scale to summed vectors (and is
	 *   highly sensitive to obtaining correct values).
	 */
	class ScaledLength
	{
	public:
		ScaledLength(UNIT sx = 1, UNIT sy = 1, UNIT sxy = 1);
		ScaledLength(const ScaledLength& al);

		ScaledLength& operator=(const ScaledLength& sl);

		ScaledLength& operator+=(ACIDTYPE at);
		ScaledLength& operator+=(const Acid& acid);

		ScaledLength& operator-=(ACIDTYPE at);
		ScaledLength& operator-=(const Acid& acid);

		void setScale(UNIT sx, UNIT sy, UNIT sxy);

		UNIT getLength() const;

		void clear();

	private:
		Unit _dxVectors;
		Unit _dyVectors;
		Unit _dxyVectors;

		Unit _sx;
		Unit _sy;
		Unit _sxy;
	};
	
	/**
	 * \brief Codon class
	 *
	 */
	class Codon
	{
	public:
		static const size_t s_cchCODON = 3;
		static const size_t s_nTRIVECTOR = 3;
		static const std::string s_strSTART;
		
	private:
		/**
		 * \brief Array of pre-computed coherence values for all possible trivector sequences
		 *
		 */
		static const bool s_aryCOHERENCE[ACID_MAX][ACID_MAX][ACID_MAX];

	public:
		/**
		 * \brief Determine if the given trivector is coherent or not
		 * \param[in] atFirst First acid in the trivector
		 * \param[in] atSecond Second acid in the trivector
		 * \param[in] atThird Third acid in the trivector
		 */
		static bool isCoherent(ACIDTYPE atFirst, ACIDTYPE atSecond, ACIDTYPE atThird);

		/**
		 * \brief Convert the passed codon to the equivalent Acid
		 *
		 * \param[in] pszCodon Pointer to first of three characters forming the codon
		 */
		static Acid codonToAcid(const char* pszCodon);

		/**
		 * \brief Return true/false if the passed index is on a codon boundary
		 * \param[in] iBase Index to evaluate
		 */
		static bool onCodonBoundary(size_t iBase);

		/**
		 * \brief Truncate the passed index to the nearest codon boundary
		 * \param[in] iBase Index to truncate
		 */
		static size_t toCodonBoundary(size_t iBase);

		/**
		 * \brief Convert the passed index into a codon-relative index
		 * \param[in] Index to convert
		 */
		static size_t toCodonIndex(size_t iBase);

		/**
		 * \brief Return the offset within a codon of the passed index
		 * \param[in] iBase Index to evaluate
		 */
		static size_t toCodonOffset(size_t iBase);

		/**
		 * \brief Return true/false if the passed length it a whole number of codons
		 * \param[in] cbBases Length to evaluate
		 */
		static bool hasWholeCodons(long cbBases);

		/**
		 * \brief Return the number of whole codons in the passed length
		 * \param[in] cbBases Length to evaluate
		 */
		static long numWholeCodons(long cbBases);
		
		/**
		 * \brief Return the number of affected codons (this rounds up to whole codons)
		 * \param[in] cbBases Length to evaluate
		 */
		static long numAffectedCodons(size_t iBase, long cbBases);

		/**
		 * \brief Return the frame-shift (positive or negative) of a length change
		 * \param[in] cbBases Length to evaluate
		 */
		static long numFrameShift(long cbBases);

		/**
		 * \brief Return true/false if the passed codon produces the Start acid
		 * \param[in] pszCodon Pointer to first base in the codon
		 */
		static bool isStart(const std::string& strCodon);
		static bool isStart(const char* pszCodon);

		/**
		 * \brief Return true/false if the passed codon produces the Stop acid
		 * \param[in] pszCodon Pointer to first base in the codon
		 */
		static bool isStop(const std::string& strCodon);
		static bool isStop(const char* pszCodon);

		/**
		 * \brief Return true/false if the passed change results in a silent mutation
		 * \param[in] strBases Base string to be changed
		 * \param[in] iBaseChange Location at to begin the change
		 * \param[in] strBasesChange Bases to which to change
		 */
		static bool isSilentChange(const std::string& strBases, size_t iBaseChange, const std::string& strBasesChange);
	};


} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // CODONS_HPP
