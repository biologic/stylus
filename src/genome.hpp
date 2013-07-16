/*******************************************************************************
 * \file	genome.hpp
 * \brief	Stylus Genome class
 *
 * A Genome is the core Stylus class: It holds the genetic data, performs
 * mutations, and evaluates the contained Scribbles (if any) against the
 * appropriate Han forms (if any).
 *
 * The current implementation supports only one Genome per Stylus instance.
 * This is enforced by making the Genome, essentially, a static object.
 * (Using static members intead of a single static object avoids start-up and
 * shut-down issues.)
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

#ifndef GENOME_HPP
#define GENOME_HPP

namespace org_biologicinstitute_stylus
{
	enum MUTATIONTYPE
	{
		MT_COPY = 0,						///< A copy mutation
		MT_CHANGE,							///< A change mutation
		MT_DELETE,							///< A delete mutation
		MT_INSERT,							///< An insert mutation
		MT_TRANSPOSE,						///< A transpose mutation
		MT_ILLEGAL,							///< Illegal/Unknown type

		MT_MAX
	};

    enum ROLLBACKTYPE
    {
        RT_ATTEMPT = 0x1,
        RT_CONSIDERATION = 0x2,
        RT_COMBINED = RT_ATTEMPT | RT_CONSIDERATION
    };

	/**
	 * \brief A stack of modification records
	 *
	 */
	class ModificationStack
	{
	public:
		ModificationStack();
		ModificationStack(const ModificationStack& ms);
		~ModificationStack();
		
		ModificationStack& operator=(const ModificationStack& ms);

		void recordModification(IModificationSRPtr& srpModification);
		void recordDescription(const char* pszDescription);
		
		void undo();

		bool isEmpty() const;
		size_t length() const;

		void clear();

		std::string toString() const;
		void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;
		
	private:
		std::string _strDescription;			///< Description of stack (if any)
		MODIFICATIONARRAY _vecModifications;	///< Stack of modifications
	};
	
	/**
	 * \brief A record of some kind of mutation
	 * 
	 */
	class MutationModification : public IModification
	{
	public:
		MutationModification(size_t iGene, size_t iTarget, const char* pbBases);
		virtual ~MutationModification();
		
		virtual size_t length() const;

	protected:
		size_t _iGene;
		size_t _iTarget;
		std::string _strBases;
	};
	
	/**
	 * \brief A record of a base change mutation
	 *
	 */
	class ChangeModification : public MutationModification
	{
	public:
		ChangeModification(size_t iGene, size_t iTarget, const char* pbBasesBefore, const char* pbBasesAfter, bool fSilent);
		virtual ~ChangeModification();

		virtual void undo();
		virtual std::string toString() const;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	protected:
		bool _fSilent;
		std::string _strBasesAfter;
	};

	/**
	 * \brief A record of a base copy mutation
	 *
	 */
	class CopyModification : public MutationModification
	{
	public:
		CopyModification(size_t iGene, size_t iSource, size_t iTarget, const char* pbBases);
		virtual ~CopyModification();

		virtual void undo();
		virtual std::string toString() const;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	protected:
		size_t _iSource;
	};

	/**
	 * \brief A record of a base deletion mutation
	 *
	 */
	class DeleteModification : public MutationModification
	{
	public:
		DeleteModification(size_t iGene, size_t iTarget, const char* pbBases);
		virtual ~DeleteModification();

		virtual void undo();
		virtual std::string toString() const;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;
	};

	/**
	 * \brief A record of a base insertion mutation
	 *
	 */
	class InsertModification : public MutationModification
	{
	public:
		InsertModification(size_t iGene, size_t iTarget, const char* pbBases);
		virtual ~InsertModification();

		virtual void undo();
		virtual std::string toString() const;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;
	};

	/**
	 * \brief A record of a base transposition mutation
	 *
	 */
	class TransposeModification : public MutationModification
	{
	public:
		TransposeModification(size_t iGeneSource, size_t iGeneTarget, size_t iSource, size_t iTarget, const char* pbBases);
		virtual ~TransposeModification();

		virtual void undo();
		virtual std::string toString() const;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	protected:
		size_t _iGeneSource;
		size_t _iSource;
	};

	/**
	 * \brief A helper class to ensure the genome always ends in a valid state
	 * 
	 */
	class StateGuard
	{
	public:
		StateGuard(ST_GENOMESTATE gsTarget, ST_GENOMESTATE gsError);
		~StateGuard();

	private:
		ST_GENOMESTATE _gsTarget;
		ST_GENOMESTATE _gsError;
	};
	
	/**
	 * \brief A codon table that maps a codon to the associated ACIDTYPE (vector)
	 * 
	 * The codon table is a map from a three character codon into the corresponding
	 * acid-type. The map is in T, C, A, G order; that is, TTT is the first codon
	 * and GGG is the last.
	 * 
	 * When writing XML, the codon table will only generate content if what it
	 * contains is not the default codon table and then only for entries that
	 * differ from the default table.
	 */
	class CodonTable
	{
	public:
		CodonTable();
		
		void load(XMLDocument* pxd, xmlNodePtr pxnCodonTable);
		void reset();
		
		ACIDTYPE codonToType(const char* pszCodon);
		
		void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	private:
		static const char* s_aryCodonToName[Constants::s_nmaxCODONS];
		static const std::string s_strUUID;
		static const ACIDTYPE s_mapCodonToType[Constants::s_nmaxCODONS];

		std::string _strUUID;
		std::string _strAuthor;
		std::string _strCreationDate;
		std::string _strCreationTool;
		ACIDTYPE _mapCodonToType[Constants::s_nmaxCODONS];

		static size_t codonToIndex(const char* pszCodon);
	};
	
	/**
	 * \brief The Stylus Genome object
	 *
	 * The Genome is a singleton object that, after loading, holds and evaluates genes.
	 * Its singleton nature is enforced by making all data members static.
	 */
	class Genome
	{
		friend class ModificationStack;
		
		friend class ChangeModification;
		friend class CopyModification;
		friend class DeleteModification;
		friend class InsertModification;
		friend class TransposeModification;
		
		friend class Plan;
        friend class MutationSelector;
		friend class Step;
		
		friend class StateGuard;
		
	public:
		static const size_t s_maxGENES = 128;

		static void initialize();
		static void terminate();

		/**
		 * \brief Core Genome methods
		 *
		 * These are analogues to the methods defined in stylus.h.
		 */
		//{@
		static void setGenome(const char* pxmlGenome, const char* pszAuthor);
		static void getGenome(char* pxmlGenome, size_t* pcchGenome, STFLAGS grfRecordDetail);

		static void setUUIDSeeds(const char* pszSeeds);

		static void setTraceTrial(size_t iTrialTrace);
		static size_t getTraceTrial();
		
		static void setRecordRate(size_t cRecordRate, STFLAGS grfRecordDetail, const char* pszRecordDirectory, bool fRecordHistory);

		static void executePlan(const char* pxmlPlan, size_t iTrial, size_t cTrials, ST_PFNSTATUS pfnStatus, size_t cStatusRate);
		//@}

		static void recordModification(IModification* pModification);

		/*
		 * Note:
		 * - Attempts are rejected changes made to the genome.
		 * - Terminations are reasons why a plan ceases execution.
		 * - All attempts terminate, but not all terminations arise from attempts.
		 */
		static void recordAttempt(const char* pszFileline, ST_TRACEREGION tr, const char* pszFormat, ...) throw();
		static void recordTermination(const char* pszFileline, ST_GENOMETERMINATION ga, ST_GENOMEREASON gr, const char* pszFormat, ...) throw();
		static void getTermination(ST_GENOMETERMINATION* pgt, ST_GENOMEREASON* pgr, char* pszTermination, size_t* pcchTermination);

		static Gene& getGeneById(size_t id);
		static GENEARRAY& getGenes();
		
		static bool isState(ST_GENOMESTATE gs);
		static ST_GENOMESTATE getState();
		
		static bool isRollbackAllowed();
		
		static ACIDTYPE codonToType(const char* pszCodon);
		
		static const std::string& getBases();
		static UNIT getGeneScore(size_t iGene = 1);
		static UNIT getGeneUnits(size_t iGene = 1);
		static UNIT getCost();
		static UNIT getFitness();
		static UNIT getScore();
		static void getStatistics(ST_STATISTICS* pStatistics);
		static size_t getTrial();
		static size_t getTrialAttempts();
		
		static const Plan& getPlan();

		static std::string toString();
		static void toXML(XMLStream& xs, STFLAGS grfRecordDetail, bool fUseTrialStatistics = false);
        static void writeConsiderations(XMLStream& xs, STFLAGS grfRecordDetail);

        static void setRollbackType(ROLLBACKTYPE rollback_type);
        static void removeConsideration(size_t iConsideration);

  	private:
		static const char* s_aryGENOMESTATES[STGS_MAX];
		
		//--------------------------------------------------------------------------------
		// General properties
		//--------------------------------------------------------------------------------
		static std::string _strAuthor;			///< Author name string (may be empty)
		static CodonTable _ct;					///< Codon table for the bases
		static std::string _strBases;			///< String of T, C, A, and G
		static utime _tLoaded;					///< Time at when the genome was loaded

		static std::string _strUUID;			///< Genome universally unique ID
		static std::string _strStrain;			///< User-supplied strain identifier (unused and uninterpreted)
		static std::string _strAncestors;		///< User-supplied ancestor identifiers (unused and uninterpreted)

		static std::string _strUUIDSeeds;		///< Seed for UUID generation
		static STRINGARRAY _vecUUIDs;			///< Array of unique IDs to use when recording

		static bool _fReady;					///< Genome is ready for use
        static ROLLBACKTYPE _rollbackType;      ///< Current rollback type
		
		static Plan _plan;						///< Current plan

		static ST_STATISTICS _stats;			///< Statistics since load
		static ST_STATISTICS _statsRecordRate;	///< Statistics since last written

		static size_t _cRecordRate;
		static STFLAGS _grfRecordDetail;
		static std::string _strRecordDirectory;

		static size_t _fRecordHistory;

		static bool _fGenesAssigned;			///< Flag indicating if genes were assigned or discovered
		static GENEARRAY _vecGenes;				///< Array of genes within the genome
		static std::bitset<s_maxGENES> _grfGenesInvalid;	///< Bit-flags indicating invalid genes

		static ModificationStack _msModifications;	///< Stack of modifications
		static MODIFICATIONSTACKARRAY _vecAttempts;	///< Stack of failed attempts (each as a ModificationStack)
		static MODIFICATIONSTACKARRAY _vecConsiderations;	///< Stack of considerations (each as a ModificationStack)

		static ST_GENOMESTATE _gsCurrent;			///< Current genome state
		
		static ST_GENOMETERMINATION _gaTermination;	///< Last failed action
		static ST_GENOMEREASON _grTermination;		///< Reason code associated with last failed action
		static std::string _strTermination;			///< Human-readable string describing the action

		//--------------------------------------------------------------------------------
		// Internal Methods
		//--------------------------------------------------------------------------------
		static void clearStatistics(ST_STATISTICS& stats, size_t iTrial);

		static bool enterState(ST_GENOMESTATE gs);
		static bool exitState(bool (*pfnTransition)() = NULL);
		static const char* stateToString(ST_GENOMESTATE gs = _gsCurrent);
		
		static bool isLoaded();

		static void advanceTrial(bool fRestore);
		static void advanceTrialAttempts();

		static bool doCompilation();
		static bool doRecording();
		static bool doRestore();
		static bool doRollback();
		static bool doScoring();
		static bool doSpawn();
		static bool doValidation();
		
		enum RECORDTYPE
		{
			RT_INITIAL,
			RT_TRIAL,
			RT_FINAL
		};
		static void record(RECORDTYPE rt);
		static void recordHistory(RECORDTYPE rt);
		static bool validate(bool fPreserveErrors = false);
        static bool rollback();
        static bool recordStatistics(bool fPreserveErrors = false);
		
		static bool isRecording();
		static bool isRecordingTrial();
		static bool isRecordingHistory();

		static size_t recordingRate();

		static size_t indexToGene(size_t iBase);
		
		static void recordStatistics(MUTATIONTYPE mt, size_t cbBases, bool fSilent);
		static void recordStatistics(ST_STATISTICS & stats, MUTATIONTYPE mt, size_t cbBases, bool fSilent);
		static void recordStatistics(ST_ATTEMPTS & attempts, size_t bases);
		static void undoStatistics(MUTATIONTYPE mt, size_t cbBases, bool fSilent);
		static void undoStatistics(ST_STATISTICS & stats, MUTATIONTYPE mt, size_t cbBases, bool fSilent);
        static void undoAttempts(ST_ATTEMPTS & attempts, size_t cbBases);

		static bool handleChange(const Mutation& mt, bool fPreserveGenes, bool fRejectSilent);
		static bool handleCopy(const Mutation& mt, bool fPreserveGenes);
		static bool handleDelete(const Mutation& mt, bool fPreserveGenes);
		static bool handleInsert(const Mutation& mt, bool fPreserveGenes);
		static bool handleTranspose(const Mutation& mt, bool fPreserveGenes);

		static void changeBases(size_t iGene, size_t iTarget, const std::string& strBases, bool fSilent);
		static void deleteBases(size_t iGene, size_t iTarget, size_t cbBases);
		static void insertBases(size_t iGene, size_t iTarget, const std::string& strBases);

		static void purgeModifications(bool fPreserveAttempts = false);

		static size_t rgenIndex(const Range& rgBases, bool fInFrame = false);
		static size_t rgenLength(size_t cbLength, bool fWholeCodons = false);
		static std::string& rgenTransversion(size_t iTarget, std::string& strBases, UNIT nTransversionLikelihood);
		static std::string& rgenBases(std::string& strBases, size_t cbLength);

#ifdef ST_DEBUG
		static Unit _nFitnessPassing;
		static bool _fTestingRollback;
		static bool testRollback();
#endif
	};

#define THROWIFEXECUTING(x)		{ if (_plan.isExecuting()) THROWRC((RC(INVALIDSTATE), "Illegal nested call to " ST_STRINGIFY(x))); }

#define REQUIREALIVE(x)			{ if (!isState(STGS_ALIVE)) THROWRC((RC(INVALIDSTATE), "Illegal call to " ST_STRINGIFY(x) " while in state %s", stateToString())); }
#define REQUIRENOTDEAD(x)		{ if (isState(STGS_DEAD)) THROWRC((RC(DEAD), "Illegal call to " ST_STRINGIFY(x) " on a dead genome")); }

} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // GENOME_HPP
