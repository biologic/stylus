/*******************************************************************************
 * \file	plan.hpp
 * \brief	Stylus Plan and helper classes
 *
 * Each instance of a Plan class encapsulates an executable plan. (See stylus.xsd
 * for more details.)
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

#ifndef PLAN_HPP
#define PLAN_HPP

namespace org_biologicinstitute_stylus
{
	enum PLANCONDITION
	{
		PC_NONE					= 0x0000,	///< No conditions were supplied
		PC_TRIALCOST			= 0x0001,	///< A CostTrialCondition was supplied
		PC_TRIALFITNESS			= 0x0002,	///< A FitnessTrialCondition was supplied
		PC_TRIALSCORE			= 0x0004,	///< A ScoreTrialCondition was supplied
		PC_TRIALMUTATION		= 0x0008,	///< A MutationTrialCondition was supplied
		PC_TERMINATIONDURATION	= 0x0010,	///< A DurationTerminationCondition was supplied
		PC_TERMINATIONROLLBACK	= 0x0020,	///< A RollbackTerminationCondition was supplied
		PC_TERMINATIONFITNESS	= 0x0040	///< A FitnessTerminationCondition was supplied
	};

	/**
	 * \brief Base-class for a single trial condition value used in scoring evaluation
	 * 
	 */
	class TrialValue
	{
	public:
		TrialValue(UNIT nLikelihood = 1.0, UNIT nValue = 1.0, UNIT nFactor = 1.0);
		TrialValue(const TrialValue& tv);
		
		TrialValue& operator=(const TrialValue& tv);
		
		void clear();
		
		UNIT getLikelihood() const;
		UNIT getValue() const;
		void setValue(UNIT nValue);
		UNIT getFactor() const;
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
		
	protected:
		Unit _nLikelihood;
		Unit _nValueOriginal;
		Unit _nValue;
		Unit _nFactor;
	};
	
	/**
	 * \brief Base-class for a trial condition applied in scoring evalution
	 * 
	 */
	class TrialCondition
	{
	public:
		TrialCondition(XMLTAG xt = XT_UNKNOWNVALUE, ST_GENOMEREASON stgr = STGR_NONE);
		TrialCondition(const TrialCondition& tc);
		
		TrialCondition& operator=(const TrialCondition& tc);
		
		void clear();

		bool evaluate(UNIT nValue);
		UNIT evaluatePerformance(UNIT nValue) const;
        bool active() const;
        UNIT getPerformancePrecision() const;
        const MutationTrialCondition * getMutationTrialCondition() const;
		
	protected:
		enum TRIALCONDITIONMODE
		{
			TCM_NONE = 0,
			TCM_DECREASE,
			TCM_INCREASE,
			TCM_MAINTAIN,

			TCM_MAX
		};

		static const char* s_aryTRIALCONDITIONMODE[TCM_MAX];

		TRIALCONDITIONMODE _tcm;
		TRIALVALUEARRAY _vecValues;
		
		void initialize(UNIT nValueDefault);

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs, size_t iGene = 0);
		
	private:
		XMLTAG _xt;
		ST_GENOMEREASON _stgr;
	};
	
	/**
	 * \brief The cost condition applied in scoring evalution
	 * 
	 */
	class CostTrialCondition : public TrialCondition
	{
	public:
		CostTrialCondition();
		
		void initialize();

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
	};
	
	/**
	 * \brief The fitness condition applied in scoring evalution
	 * 
	 */
	class FitnessTrialCondition : public TrialCondition
	{
	public:
		FitnessTrialCondition();
		
		void initialize();

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
	};
	
	/**
	 * \brief The score condition applied in scoring evalution
	 * 
	 */
	class ScoreTrialCondition : public TrialCondition
	{
	public:
		ScoreTrialCondition();
		
		void initialize();

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
	};
	
	/**
	 * \brief The number of mutations to apply per trial
	 * 
	 */
	class MutationsPerAttempt
	{
	public:
		MutationsPerAttempt();
		MutationsPerAttempt(const MutationsPerAttempt& mpt);
		
		MutationsPerAttempt& operator=(const MutationsPerAttempt& mpt);
		
		UNIT getLikelihood() const;
		size_t getCount() const;
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
		
	private:
		Unit _nLikelihood;
		size_t _cMutations;
	};
	
	/**
	 * \brief The mutation condition evaluated when applying mutations
	 * 
	 */
	class MutationTrialCondition
	{
	public:
		MutationTrialCondition();
		
		void clear();
		
		size_t getMutationsPerAttempt() const;
        void produceMutations(MutationSource & source, MutationSelector & selector) const;
        bool generatesSingleMutation() const;
        bool isExhaustive() const;
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
		
	private:
		MUTATIONSPERATTEMPTARRAY _vecMutationsPerAttempt;
        bool _fExhaustive;
	};
	
	/**
	 * \brief The collection of conditions defining a trial
	 * 
	 */
	class TrialConditions
	{
	public:
		TrialConditions();
		TrialConditions(const TrialConditions& tc);
		
		TrialConditions& operator=(const TrialConditions& tc);
		
		void clear();
		
		bool hasConditions(STFLAGS grfConditions) const;
		void initialize();
		
		bool evaluate(PLANCONDITION pc, UNIT nValue);
		size_t getMutationsPerAttempt() const;
        void produceMutations(MutationSource & source, MutationSelector & selector);
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);

        TrialCondition * getTrialCondition(PLANCONDITION pc);
        const MutationTrialCondition * getMutationTrialCondition() const;
		
	private:
		STFLAGS _grfConditions;				///< Set of PLANCONDITION flags

		CostTrialCondition _cc;
		FitnessTrialCondition _fc;
		ScoreTrialCondition _sc;
		MutationTrialCondition _mc;
	};
	
	/**
	 * \brief The maximum duration a plan should execute condition
	 * 
	 */
	class DurationTerminationCondition
	{
	public:
		DurationTerminationCondition();

		void clear();

		void initialize(size_t iTrialInitial, size_t cAttemptsInitial);
		bool evaluate();

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);

	private:
		bool _fHasTrialLimit;
		size_t _iTrialInitial;
		size_t _cTrials;

		bool _fHasAttemptLimit;
		size_t _cAttemptsInitial;
		size_t _cAttempts;
	};

	/**
	 * \brief The maximum number of rollbacks a plan should execute condition
	 * 
	 */
	class RollbackTerminationCondition
	{
	public:
		RollbackTerminationCondition();

		void clear();

		void initialize();
		bool evaluate(size_t nRollbacks);

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);

	private:
		size_t _cRollbacks;
	};

	/**
	 * \brief The maximum and/or minimum fitness a plan attain condition
	 * 
	 */
	class FitnessTerminationCondition
	{
	public:
		FitnessTerminationCondition();

		void clear();

		bool evaluate(UNIT nFitness);

		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);

	private:
		Unit _nMaximum;
		Unit _nMinimum;
	};
	
	/**
	 * \brief Encapsulation of user-supplied index range limits
	 * 
	 */
	class IndexRange
	{
	public:
		IndexRange();
		
		IndexRange& operator=(const IndexRange& ir);
		
		bool isSet() const;

		Range getRange() const;
		
		// Note:
		// The range indicates the positions at which mutations may occur; generally
		// the range values are correct as stated by the user. An exception involves
		// insertions since they occur *before* the supplied index value. Again, the
		// supplied values are correct *unless* the range is bound to a gene or stroke.
		// In those cases, the range must be extend by one position to allow for
		// insertions up through the last position occupied by the gene/stroke.
		// This routine will extend those types of ranges.
		void adjustRangeForInsert(Range& rg) const;
		
		void clear();
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);

		std::string toString() const;
		
	private:
		enum FIELDSUPPLIED
		{
			FS_NONE			= 0x0000,		///< No values were supplied
			FS_INDEXRANGE	= 0x0001,		///< indexRange was supplied
			FS_GENERANGE	= 0x0002,		///< geneRange was supplied
			FS_HANSTROKE	= 0x0004		///< hanStrokeRange was supplied
		};
		STFLAGS _grfSupplied;				///< Flags noting which fields were supplied (vs generated)
		
		Range _rgIndex;						///< Base index range
		bool _fPercentageIndex;				///< Base index range expressed as percentages
		size_t _iGene;						///< Gene whose range to use
		std::string _strUnicode;			///< Han Unicode value
		size_t _iHanStroke;					///< Han stroke to use
	};

	/**
	 * \brief The base class for all mutations
	 * 
	 * Plans build mutations from the content of plan steps, filling in
	 * missing fields as necessary.
	 */
	class Mutation
	{
		friend class Step;

	public:
		Mutation(MUTATIONTYPE mt = MT_ILLEGAL);
		Mutation(const Mutation& m);
		
		Mutation& operator=(const Mutation& m);

		bool isCopy() const;				///< Returns TRUE if a copy mutation
		bool isChange() const;				///< Returns TRUE if a change mutation
		bool isDelete() const;				///< Returns TRUE if a delete mutation
		bool isInsert() const;				///< Returns TRUE if an insert mutation
		bool isTranspose() const;			///< Returns TRUE if a transpose mutation
		
		bool allFieldsSupplied() const;		///< Returns TRUE if all required fields were supplied (in the plan)
		bool hasSourceIndex() const;
		bool hasTargetIndex() const;
		bool hasTandemIndex() const;
		bool hasCountBases() const;
		bool hasBases() const;
		bool hasRange() const;
		bool hasTransversion() const;

        bool needsSourceIndex() const;
        bool needsBases() const;
		
		size_t sourceIndex() const;
		size_t targetIndex() const;
		size_t countBases() const;
		const std::string& bases() const;
		const IndexRange& range() const;
		
		std::string toString(bool fTypeOnly = false) const;

	protected:
		static const char* s_aryMUTATIONTYPE[MT_MAX];
		static const Unit s_nTRANSVERSIONLIKELIHOOD;

		MUTATIONTYPE _mt;					///< Mutation type

		enum FIELDSUPPLIED
		{
			FS_SOURCE = 0x0001,				///< Source index was supplied
			FS_TARGET = 0x0002,				///< Target index was supplied
			FS_TANDEM = 0x0004,				///< Tandem target was supplied
			FS_COUNT  = 0x0008,				///< Count of bases was supplied
			FS_BASES  = 0x0010,				///< Bases were supplied
			FS_TRANSVERSION = 0x0020,		///< Transversion probability was supplied

			// These flag sets represent those fields that fully specify a mutation
			// (meaning, no values were randomly generated that could be retried)
			FS_COPY   = FS_SOURCE			///< Fields required by a copy
						| FS_TARGET
						| FS_COUNT,
						
			FS_COPYTANDEM = FS_SOURCE
						| FS_TANDEM
						| FS_COUNT,
						
			FS_CHANGE = FS_TARGET			///< Fields required by a change
						| FS_COUNT
						| FS_BASES,
						
			FS_CHANGETRANSVERSION = FS_TARGET
						| FS_TRANSVERSION,
						
			FS_DELETE = FS_TARGET			///< Fields required by a delete
						| FS_COUNT,
						
			FS_INSERT = FS_TARGET			///< Fields required by an insert
						| FS_COUNT
						| FS_BASES,
						
			FS_TRANSPOSE = FS_SOURCE		///< Fields required by a transpose
						| FS_TARGET
						| FS_COUNT
		};
		STFLAGS _grfSupplied;				///< Flags noting which fields were supplied (vs generated)

		size_t _iSource;					///< Source index of the mutation
		size_t _iTarget;					///< Target index of the mutation
		size_t _cbBases;					///< Number of bases affected by the mutation
		std::string _strBases;				///< Incoming bases (series of T, C, A, or G)
		IndexRange _ir;						///< Range to limit randomly generated index values
		Unit _nTransversionLikelihood;		///< Likelihood that a random change will result in a transversion
		
		void initialize(MUTATIONTYPE mt = MT_ILLEGAL);
	};

	/**
	 * \brief A step mutation combines a probability with a mutation
	 * 
	 */
	class StepMutation : public Mutation
	{
		friend class Step;

	public:
		StepMutation(MUTATIONTYPE mt = MT_ILLEGAL);
		StepMutation(const StepMutation& sm);
		StepMutation& operator=(const StepMutation& sm);
		
		UNIT getLikelihood() const;
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
		
	private:
		Unit _nLikelihood;
	};
	
	/**
	 * \brief A single step of a plan
	 * 
	 */
	class Step
	{
	public:
		enum STEPOPTIONS
		{
			SO_NONE					= 0x0000,
			SO_ENSUREINFRAME		= 0x0001,
			SO_ENSUREWHOLECODONS	= 0x0002
		};

		Step();
		Step(const Step& st);
		
		Step& operator=(const Step& st);
		
		void initialize();
		
		void getMutation(Mutation& m, STFLAGS grfOptions, size_t iTrialInStep) const;
		size_t getTrials() const;

		bool hasConditions(STFLAGS grfConditions) const;
		bool evaluateCondition(PLANCONDITION pc, UNIT nValue);
        bool evaluatePerformance(PLANCONDITION pc, UNIT nValue, UNIT & nOutput);
		size_t getMutationsPerAttempt() const;
        void produceMutations(MutationSource & source, MutationSelector & selector);

        void produceMutations(MutationSelector & selector, STFLAGS grfOptions, size_t iTrialInStep);
		
		void load(XMLDocument* pxd, xmlNodePtr pxn);
		void toXML(XMLStream& xs);
		
        TrialCondition * getTrialCondition(PLANCONDITION pc);
        const MutationTrialCondition * getMutationTrialCondition() const;
        void checkSupportsExhaustive();
	private:
		size_t _cTrials;
		long _dIndex;
		IndexRange _ir;

		TrialConditions _tc;

		STEPMUTATIONARRAY _vecMutations;
		
		void clear();
	};

    class MutationSource
    {
        public:
            MutationSource( Step & step, STFLAGS grfConditions, size_t iTrialInStep);

            void getMutation(Mutation & mutation);
            void produceMutations(MutationSelector & selector);
        private:
            Step & _step;
            STFLAGS _grfConditions;
            size_t _iTrialInStep;
    };

    class MutationSelector
    {
    public:
        MutationSelector(Plan & plan);
        bool addMutation(Mutation & mutation);
        void mutationFinalize();
        bool selectMutation();
        bool getRollbackPossible();


    private:

        typedef std::vector<Mutation> MUTATIONVECTOR;
        struct Consideration
        {
            Consideration();
            Unit value;
            bool fValidMutations;
            bool fValidated;
            MUTATIONVECTOR mutations;
        };
        typedef std::vector< Consideration > CONSIDERATIONVECTOR;

        Consideration & _current();
        size_t _pickMutation();

        Plan & _plan;

        CONSIDERATIONVECTOR _considerations;
        bool _fFieldsMissing;
        bool _fAcceptedMutation;
        UNIT _best;
    };
	
	/**
	 * \brief An executable plan
	 * 
	 * A plan encapsulates options and commands to be executed on the active genome.
	 */
	class Plan
	{
		friend class PlanScope;

	public:
		Plan();
		
		void clear();
		
		void execute(size_t iTrialFirst, size_t cTrials, ST_PFNSTATUS pfnStatus, size_t cStatusRate);
		bool isExecuting() const;
		
		bool evaluateCondition(PLANCONDITION pc, UNIT nValue);
        UNIT evaluatePerformance();
        bool evaluateConditions();
        bool applyMutation(Mutation & mutation);
		
		void load(const char* pxmlPlan);
		void toXML(XMLStream& xs);

		size_t getActualTrialCount(size_t cTrials, size_t iTrialFirst);
        UNIT getPerformancePrecision();

	private:
		bool _fExecuting;					///< Plan is actively executing

		size_t _cTrials;					///< Total number of trials in the plan

		bool _fAccumulateMutations;
		bool _fPreserveGenes;
		bool _fEnsureInFrame;
		bool _fEnsureWholeCodons;
		bool _fRejectSilent;

		TrialConditions _tc;

		STFLAGS _grfTermination;			///< Collection of PLANCONDITION flags
		DurationTerminationCondition _dc;
		RollbackTerminationCondition _rc;
		FitnessTerminationCondition _fc;

		size_t _iStep;
		STEPARRAY _vecSteps;
		
		void initialize();
		void beginExecution();
		void endExecution();
        
        void verify();

		size_t getMutationsPerAttempt() ;
        bool evaluatePerformance(PLANCONDITION pc, UNIT nValue, UNIT & nOutput);
        void produceMutations(MutationSource & source, MutationSelector & selector);
        UNIT getPerformancePrecision(PLANCONDITION pc);
        TrialCondition * getTrialCondition(PLANCONDITION pc);
        TrialCondition * getTrialCondition(PLANCONDITION pc, size_t iStep);
        TrialCondition * getPrimaryTrialCondition();
        const MutationTrialCondition * getMutationTrialCondition(size_t iStep);
	};
	
	/**
	 * \brief A class to manage plan execution status
	 * 
	 */
	class PlanScope
	{
	public:
		PlanScope(Plan& plan);
		~PlanScope();
		
	private:
		Plan& _plan;
	};

} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // PLAN_HPP
