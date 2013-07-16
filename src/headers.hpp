/*******************************************************************************
 *  \file   headers.hpp
 *  \brief  Private Stylus header
 *
 *  All Stylus .CPP files should include this header rather than their own.
 *  Files may judiciously include additional headers not generally used by other
 *  .CPP files.
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

#ifndef HEADERS_HPP
#define HEADERS_HPP

// Prefix header ----------------------------------------------------------------
#include "prefix.hpp"

// Internal Headers ------------------------------------------------------------
#include "smartptr.hpp"

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Vector DIRECTIONs
	 * \remarks
	 * - Code depends on these values remaining in this order
	 * - Each direction corresponds to one of the eight directions around a compass
	 *   starting with one assigned to North, moving clockwise around to Northwest
	 *   (assigned the value of eight). Stop is assigned zero and does not occur on
	 *   the compass.
	 */
	enum DIRECTION
	{
		DIR_STOP = 0,
		DIR_NORTH,
		DIR_NORTHEAST,
		DIR_EAST,
		DIR_SOUTHEAST,
		DIR_SOUTH,
		DIR_SOUTHWEST,
		DIR_WEST,
		DIR_NORTHWEST
	};
	
	/**
	 * \brief ACID type enumeration
	 * \remarks
	 * - The acids are ordered by DIRECTION (see above)
	 * - Each major axis supports Short, Medium, and Long vectors;
	 *   Each minor axis supports Short and Medium vectors
	 */
	enum ACIDTYPE
	{
		ACID_STP = 0,

		ACID_Nos,
		ACID_Nom,
		ACID_Nol,

		ACID_Nes,
		ACID_Nem,

		ACID_Eas,
		ACID_Eam,
		ACID_Eal,

		ACID_Ses,
		ACID_Sem,

		ACID_Sos,
		ACID_Som,
		ACID_Sol,

		ACID_Sws,
		ACID_Swm,

		ACID_Wes,
		ACID_Wem,
		ACID_Wel,

		ACID_Nws,
		ACID_Nwm,

		ACID_MAX
	};

	class Acid;
	class ChangeModification;
	class Codon;
	class CodonTable;
	class Constants;
	class CopyModification;
	class CostTrialCondition;
	class DeleteModification;
	class Error;
	class EventStack;
	class FitnessTrialCondition;
	class Gene;
	class Genome;
	class Globals;
	class Group;
	class Han;
	class HDimensions;
	class HGroup;
	class HOverlap;
	class HStroke;
	class IModification;
	class IModificationStack;
	class IndexRange;
	class InsertModification;
	class IRandom;
	class Line;
	class LineEvent;
	class LineStack;
	class ModificationStack;
	class Mutation;
	class MutationModification;
	class MutationTrialCondition;
	class MutationModification;
    class MutationSelector;
    class MutationSource;
	class MutationsPerAttempt;
	template <class Type> class Overlap;
	class Overlaps;
	class Plan;
	class PlanScope;
	class Point;
	class PointDistance;
	class RandomC;
	class Range;
	class Rectangle;
	class RGenerator;
	class ScaledLength;
	class ScoreTrialCondition;
	class Segment;
	class StateGuard;
	class Step;
	class StepMutation;
	class Stroke;
	class StrokeRange;
	class StrokeRangeChange;
	class TrialCondition;
	class TrialConditions;
	class TrialValue;
#ifdef ST_TRACE
	class TIndent;
#endif
	class TransposeModification;
	class Unit;
	class XMLDocument;
	class XMLStream;
	
	typedef PointDistance HPoint;	///< Friendly typename used with Han points

	typedef long long utime;		///< 64-bit time in microseconds since January 1, 1970
	
	typedef std::vector<unsigned int> UINTARRAY;
	typedef std::vector<size_t> NUMERICMAP;
	typedef std::vector<size_t> NUMERICARRAY;

	typedef std::vector<std::string> STRINGARRAY;

	typedef std::pair<size_t,size_t> NUMERICPAIR;
	typedef std::vector<NUMERICPAIR> NUMERICPAIRARRAY;

	typedef std::vector<Point> POINTARRAY;
	typedef std::vector<PointDistance> POINTDISTANCEARRAY;
	
	typedef std::vector<TrialValue> TRIALVALUEARRAY;
	
	typedef std::vector<MutationsPerAttempt> MUTATIONSPERATTEMPTARRAY;
	typedef std::vector<StepMutation> STEPMUTATIONARRAY;
	typedef std::vector<Step> STEPARRAY;

	typedef smartref_ptr<IModification> IModificationSRPtr;
	typedef std::vector<IModificationSRPtr> MODIFICATIONARRAY;
	typedef std::vector<ModificationStack> MODIFICATIONSTACKARRAY;

	typedef std::vector<ACIDTYPE> ACIDTYPEARRAY;

	typedef std::vector<StrokeRange> STROKERANGEARRAY;
	typedef std::vector<Segment> SEGMENTARRAY;
	typedef std::vector<Stroke> STROKEARRAY;
	typedef std::vector<Group> GROUPARRAY;
	typedef std::vector<Gene> GENEARRAY;

	typedef std::vector<HPoint> HPOINTARRAY;
	typedef std::vector<HStroke> HSTROKEARRAY;
	typedef std::vector<HGroup> HGROUPARRAY;
	typedef std::vector<HOverlap> HOVERLAPARRAY;

	typedef Overlap<const Line&> LINEOVERLAP;
	typedef std::set<LINEOVERLAP> LINEOVERLAPS;

	typedef Overlap<size_t> STROKEOVERLAP;
	typedef std::set<STROKEOVERLAP> STROKEOVERLAPS;
}
#include "stylus.h"

#include "randomc/randomc.h"

#include "constants.hpp"
#include "error.hpp"
#include "global.hpp"
#include "xml.hpp"
#include "helpers.hpp"

#include "codon.hpp"
#include "gene.hpp"
#include "genome.hpp"
#include "han.hpp"
#include "overlap.hpp"
#include "plan.hpp"
#include "random.hpp"
#include "randomc.hpp"

#include "global.inl"
#include "codon.inl"
#include "error.inl"
#include "gene.inl"
#include "genome.inl"
#include "han.inl"
#include "helpers.inl"
#include "overlap.inl"
#include "plan.inl"
#include "random.inl"
#include "xml.inl"

#endif  // HEADERS_HPP
