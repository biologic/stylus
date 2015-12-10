/*******************************************************************************
 * \file	genome.inl
 * \brief	Stylus Genome class inline methods
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

using namespace std;
using namespace stylus;

//--------------------------------------------------------------------------------
//
// ModificationStack
//
//--------------------------------------------------------------------------------
inline ModificationStack::ModificationStack() {}
inline ModificationStack::ModificationStack(const ModificationStack& cs) { operator=(cs); }
inline ModificationStack::~ModificationStack() { clear(); }

inline ModificationStack& ModificationStack::operator=(const ModificationStack& cs)
{
	_strDescription = cs._strDescription;
	_vecModifications = cs._vecModifications;
	return *this;
}

inline void ModificationStack::recordModification(IModificationSRPtr& srpModification)
{
	ASSERT(VALIDSP(srpModification));
	_vecModifications.push_back(srpModification);
}
inline void ModificationStack::recordDescription(const char* pszDescription)
{
	ASSERT(VALID(pszDescription));
	_strDescription = pszDescription;
}

inline bool ModificationStack::isEmpty() const { return (_vecModifications.size() <= 0); }
inline size_t ModificationStack::length() const { return _vecModifications.size(); }

inline void ModificationStack::clear() { _strDescription.clear(); _vecModifications.clear(); }

inline std::string ModificationStack::toString() const { return std::string(_strDescription); }

//--------------------------------------------------------------------------------
//
// MutationModification
// ChangeModification, CopyModification, DeleteModification, InsertModification, TransposeModification
//
//--------------------------------------------------------------------------------
inline MutationModification::MutationModification(size_t iGene, size_t iTarget, const char* pbBases) :
	_iGene(iGene), _iTarget(iTarget), _strBases(pbBases) {}
inline MutationModification::~MutationModification() {}
inline size_t MutationModification::length() const { return _strBases.length(); }

inline ChangeModification::~ChangeModification() {}
inline CopyModification::~CopyModification() {}
inline DeleteModification::~DeleteModification() {}
inline InsertModification::~InsertModification() {}
inline TransposeModification::~TransposeModification() {}

//--------------------------------------------------------------------------------
//
// StateGuard
//
//--------------------------------------------------------------------------------
inline StateGuard::StateGuard(ST_GENOMESTATE gsTarget, ST_GENOMESTATE gsError) :
	_gsTarget(gsTarget), _gsError(gsError) {}
inline StateGuard::~StateGuard()
{
	if (!Genome::isState(STGS_DEAD) && !Genome::isState(_gsTarget))
		Genome::enterState(_gsError);
}

//--------------------------------------------------------------------------------
//
// CodonTable
//
//--------------------------------------------------------------------------------
inline CodonTable::CodonTable() { reset(); }

inline void CodonTable::reset()
{
	_strUUID = s_strUUID;
	::memcpy(_mapCodonToType, s_mapCodonToType, ARRAY_SIZE(_mapCodonToType));
}

inline ACIDTYPE CodonTable::codonToType(const char* pszCodon) { return _mapCodonToType[codonToIndex(pszCodon)]; }

inline size_t CodonTable::codonToIndex(const char* pszCodon)
{
	ASSERT(Codon::s_cchCODON <= ::strlen(pszCodon));
	size_t iCodon = 0;

	for (size_t i=0; i < Codon::s_cchCODON; i += 1, pszCodon += 1)
	{
		iCodon *= 4;
		iCodon += (*pszCodon == Constants::s_chBASET
				   ? 0
				   : (*pszCodon == Constants::s_chBASEC
					  ? 1
					  : (*pszCodon == Constants::s_chBASEA
						 ? 2
						 : 3)));
	}
	
	return iCodon;
}

//--------------------------------------------------------------------------------
//
// Genome
//
//--------------------------------------------------------------------------------
inline void Genome::recordModification(IModification* pModification)
{
	IModificationSRPtr srp(pModification);
	_msModifications.recordModification(srp);
}

inline Gene& Genome::getGeneById(size_t id) { ASSERT(id < _vecGenes.size()); return _vecGenes[id]; }
inline GENEARRAY& Genome::getGenes() { return _vecGenes; }

inline bool Genome::isState(ST_GENOMESTATE gs) { return gs == _gsCurrent; }
inline ST_GENOMESTATE Genome::getState() { return _gsCurrent; }
inline const char* Genome::stateToString(ST_GENOMESTATE gs) { return s_aryGENOMESTATES[gs]; }

inline bool Genome::isRollbackAllowed() { return _fReady; }

inline ACIDTYPE Genome::codonToType(const char* pszCodon) { return _ct.codonToType(pszCodon); }

inline const std::string& Genome::getBases() { return _strBases; }
inline UNIT Genome::getGeneScore(size_t iGene) { return _stats._nScore; }
inline UNIT Genome::getGeneUnits(size_t iGene) { return _stats._nUnits; }
inline UNIT Genome::getCost() { return _stats._nCost; }
inline UNIT Genome::getFitness() { return _stats._nFitness; }
inline UNIT Genome::getScore() { return _stats._nScore; }
inline void Genome::getStatistics(ST_STATISTICS* pStatistics) { ASSERT(VALID(pStatistics)); ::memcpy(pStatistics, &_stats, sizeof(ST_STATISTICS)); }
inline size_t Genome::getTrial() { return _stats._iTrialCurrent; }
inline size_t Genome::getTrialAttempts() { return _stats._cTrialAttempts; }

inline const Plan& Genome::getPlan() { return _plan; }
		
inline bool Genome::isLoaded() { return (_tLoaded != 0); }
inline void Genome::setRollbackType(ROLLBACKTYPE rt) { _rollbackType = rt; }

inline bool Genome::isRecording() { return (_grfRecordDetail != STRD_NONE && _strRecordDirectory.length() > 0); }
inline bool Genome::isRecordingTrial() { return (_fReady && _cRecordRate && (getTrial() % _cRecordRate) == 0); }
inline bool Genome::isRecordingHistory() { return (_fReady && _fRecordHistory); }

inline size_t Genome::recordingRate() { return _cRecordRate; }

inline size_t Genome::indexToGene(size_t iBase)
{
	size_t iGene = 0;
	for (; iGene < _vecGenes.size() && !_vecGenes[iGene].getRange().contains(iBase); ++iGene);
	return iGene;
}
inline const char * Genome::getMutationDescription() { return _mutationFullString.c_str(); }
