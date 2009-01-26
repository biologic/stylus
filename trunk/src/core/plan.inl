/*******************************************************************************
 * \file	plan.inl
 * \brief	Stylus Plan and helper classes inline methods
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
// TrialValue
//
//--------------------------------------------------------------------------------
inline TrialValue::TrialValue(UNIT nLikelihood, UNIT nValue, UNIT nFactor) :
	_nLikelihood(nLikelihood), _nValueOriginal(nValue), _nValue(nValue), _nFactor(nFactor) {}
inline TrialValue::TrialValue(const TrialValue& tv) { operator=(tv); }

inline TrialValue& TrialValue::operator=(const TrialValue& tv)
{
	_nLikelihood = tv._nLikelihood;
	_nValueOriginal = tv._nValueOriginal;
	_nValue = tv._nValue;
	_nFactor = tv._nFactor;
	return *this;
}

inline void TrialValue::clear()
{
	_nLikelihood = 1.0;
	_nValueOriginal = 1.0;
	_nValue = 1.0;
	_nFactor = 1.0;
}

inline UNIT TrialValue::getLikelihood() const { return _nLikelihood; }
inline UNIT TrialValue::getValue() const { return _nValue * _nFactor; }
inline void TrialValue::setValue(UNIT nValue) { _nValue = nValue; }
inline UNIT TrialValue::getFactor() const { return _nFactor; }

//--------------------------------------------------------------------------------
//
// TrialCondition
//
//--------------------------------------------------------------------------------
inline TrialCondition::TrialCondition(XMLTAG xt, ST_GENOMEREASON stgr) : _xt(xt), _stgr(stgr) { clear(); }
inline TrialCondition::TrialCondition(const TrialCondition& tc) { operator=(tc); }

inline TrialCondition& TrialCondition::operator=(const TrialCondition& tc)
{
	_tcm = tc._tcm;
	_vecValues = tc._vecValues;

	_xt = tc._xt;
	_stgr = tc._stgr;
	
	return *this;
}
	
inline void TrialCondition::clear()
{
	_tcm = TCM_NONE;
	_vecValues.clear();
}

//--------------------------------------------------------------------------------
//
// CostTrialCondition
//
//--------------------------------------------------------------------------------
inline CostTrialCondition::CostTrialCondition() : TrialCondition(XT_COSTCONDITION, STGR_COST) {}
inline void CostTrialCondition::initialize() { TrialCondition::initialize(Genome::getCost()); }
inline void CostTrialCondition::load(XMLDocument* pxd, xmlNodePtr pxn) { TrialCondition::load(pxd, pxn); }
inline void CostTrialCondition::toXML(XMLStream& xs) { TrialCondition::toXML(xs); }

//--------------------------------------------------------------------------------
//
// FitnessTrialCondition
//
//--------------------------------------------------------------------------------
inline FitnessTrialCondition::FitnessTrialCondition() : TrialCondition(XT_FITNESSCONDITION, STGR_FITNESS) {}
inline void FitnessTrialCondition::initialize() { TrialCondition::initialize(Genome::getFitness()); }
inline void FitnessTrialCondition::load(XMLDocument* pxd, xmlNodePtr pxn) { TrialCondition::load(pxd, pxn); }
inline void FitnessTrialCondition::toXML(XMLStream& xs) { TrialCondition::toXML(xs); }

//--------------------------------------------------------------------------------
//
// ScoreTrialCondition
//
//--------------------------------------------------------------------------------
inline ScoreTrialCondition::ScoreTrialCondition() : TrialCondition(XT_SCORECONDITION, STGR_SCORE) {}
inline void ScoreTrialCondition::initialize() { TrialCondition::initialize(Genome::getGeneScore()); }
inline void ScoreTrialCondition::load(XMLDocument* pxd, xmlNodePtr pxn) { TrialCondition::load(pxd, pxn); }
inline void ScoreTrialCondition::toXML(XMLStream& xs) { TrialCondition::toXML(xs, 1); }

//--------------------------------------------------------------------------------
//
// MutationsPerAttempt
//
//--------------------------------------------------------------------------------
inline MutationsPerAttempt::MutationsPerAttempt() :
	_nLikelihood(1.0), _cMutations(1) {}
inline MutationsPerAttempt::MutationsPerAttempt(const MutationsPerAttempt& mpt) { operator=(mpt); }

inline MutationsPerAttempt& MutationsPerAttempt::operator=(const MutationsPerAttempt& mpt)
{
	_nLikelihood = mpt._nLikelihood;
	_cMutations = mpt._cMutations;
	return *this;
}

inline UNIT MutationsPerAttempt::getLikelihood() const { return _nLikelihood; }
inline size_t MutationsPerAttempt::getCount() const { return _cMutations; }

//--------------------------------------------------------------------------------
//
// MutationTrialCondition
//
//--------------------------------------------------------------------------------
inline MutationTrialCondition::MutationTrialCondition() { clear(); }
inline void MutationTrialCondition::clear()
{
	_vecMutationsPerAttempt.clear();
	_vecMutationsPerAttempt.push_back(MutationsPerAttempt());
}

//--------------------------------------------------------------------------------
//
// TrialConditions
//
//--------------------------------------------------------------------------------
inline TrialConditions::TrialConditions() { clear(); }
inline TrialConditions::TrialConditions(const TrialConditions& tc) { operator=(tc); }

inline TrialConditions& TrialConditions::operator=(const TrialConditions& tc)
{
	_grfConditions = tc._grfConditions;

	_cc = tc._cc;
	_fc = tc._fc;
	_sc = tc._sc;
	_mc = tc._mc;
	
	return *this;
}

inline void TrialConditions::clear()
{
	_grfConditions = PC_NONE;
	
	_cc.clear();
	_fc.clear();
	_sc.clear();
	_mc.clear();
}

inline bool TrialConditions::hasConditions(STFLAGS grfConditions) const { return ST_ISALLSET(_grfConditions, grfConditions); }
inline bool TrialConditions::evaluate(PLANCONDITION pc, UNIT nValue)
{
	switch (pc)
	{
	case PC_TRIALCOST: return _cc.evaluate(nValue);
	case PC_TRIALFITNESS: return _fc.evaluate(nValue);
	case PC_TRIALSCORE: return _sc.evaluate(nValue);
	default: ASSERT(false); return false;
	}
}
inline size_t TrialConditions::getMutationsPerAttempt() const { return _mc.getMutationsPerAttempt(); }

//--------------------------------------------------------------------------------
//
// DurationTerminationCondition
//
//--------------------------------------------------------------------------------
inline DurationTerminationCondition::DurationTerminationCondition() { clear(); }
	
inline void DurationTerminationCondition::clear()
{
	_fHasTrialLimit = false;
	_iTrialInitial = Genome::getTrial();
	_cTrials = std::numeric_limits<size_t>::max();
	
	_fHasAttemptLimit = false;
	_cAttemptsInitial = Genome::getTrialAttempts();
	_cAttempts = std::numeric_limits<size_t>::max();
}

inline void DurationTerminationCondition::initialize(size_t iTrialInitial, size_t cAttemptsInitial)
{
	_iTrialInitial = iTrialInitial;
	_cAttemptsInitial = cAttemptsInitial;
}

//--------------------------------------------------------------------------------
//
// RollbackTerminationCondition
//
//--------------------------------------------------------------------------------
inline RollbackTerminationCondition::RollbackTerminationCondition() { clear(); }
	
inline void RollbackTerminationCondition::clear()
{
	_cRollbacks = Genome::getBases().length();
}

inline void RollbackTerminationCondition::initialize()
{
	_cRollbacks = Genome::getBases().length();
}

//--------------------------------------------------------------------------------
//
// FitnessTerminationCondition
//
//--------------------------------------------------------------------------------
inline FitnessTerminationCondition::FitnessTerminationCondition() { clear(); }
	
inline void FitnessTerminationCondition::clear()
{
	_nMaximum.setUndefined();
	_nMinimum.setUndefined();
}

//--------------------------------------------------------------------------------
//
// IndexRange
//
//--------------------------------------------------------------------------------
inline IndexRange::IndexRange() { clear(); }

inline bool IndexRange::isSet() const { return (_grfSupplied != FS_NONE); }

inline void IndexRange::adjustRangeForInsert(Range& rg) const
{
	if (!ST_ISALLSET(_grfSupplied, FS_INDEXRANGE))
		rg.extend(1);
}

inline void IndexRange::clear()
{
	_grfSupplied = FS_NONE;
	_rgIndex.clear();
	_fPercentageIndex = false;
	_iGene = 0;
	_strUnicode.clear();
	_iHanStroke = 0; 
}

inline IndexRange& IndexRange::operator=(const IndexRange& ir)
{
	_grfSupplied = ir._grfSupplied;
	_rgIndex = ir._rgIndex;
	_fPercentageIndex = ir._fPercentageIndex;
	_iGene = ir._iGene;
	_strUnicode = ir._strUnicode;
	_iHanStroke = ir._iHanStroke;
	return *this;
}

//--------------------------------------------------------------------------------
//
// Mutation
//
//--------------------------------------------------------------------------------
inline Mutation::Mutation(MUTATIONTYPE mt) { initialize(mt); }
inline Mutation::Mutation(const Mutation& m) { operator=(m); }

inline Mutation& Mutation::operator=(const Mutation& m)
{
	_mt = m._mt;
	_grfSupplied = m._grfSupplied;
	_iSource = m._iSource;
	_iTarget = m._iTarget;
	_cbBases = m._cbBases;
	_strBases = m._strBases;
	_ir = m._ir;
	_nTransversionLikelihood = m._nTransversionLikelihood;
	return *this;
}

inline bool Mutation::isCopy() const { return (_mt == MT_COPY); }
inline bool Mutation::isChange() const { return (_mt == MT_CHANGE); }
inline bool Mutation::isDelete() const { return (_mt == MT_DELETE); }
inline bool Mutation::isInsert() const { return (_mt == MT_INSERT); }
inline bool Mutation::isTranspose() const { return (_mt == MT_TRANSPOSE); }

inline bool Mutation::allFieldsSupplied() const
{
	return (isCopy()
			? ST_ISALLSET(_grfSupplied, FS_COPY) || ST_ISALLSET(_grfSupplied, FS_COPYTANDEM)
			: (isChange()
				? ST_ISALLSET(_grfSupplied, FS_CHANGE) || (ST_ISALLSET(_grfSupplied, FS_CHANGETRANSVERSION) && _nTransversionLikelihood == 0.0)
				: (isDelete()
					? ST_ISALLSET(_grfSupplied, FS_DELETE)
					: (isInsert()
						? ST_ISALLSET(_grfSupplied, FS_INSERT)
						: ST_ISALLSET(_grfSupplied, FS_TRANSPOSE)))));
}
inline bool Mutation::hasSourceIndex() const { return ST_ISALLSET(_grfSupplied, FS_SOURCE); }
inline bool Mutation::hasTargetIndex() const { return ST_ISALLSET(_grfSupplied, FS_TARGET); }
inline bool Mutation::hasTandemIndex() const { return ST_ISALLSET(_grfSupplied, FS_TANDEM); }
inline bool Mutation::hasCountBases() const { return ST_ISALLSET(_grfSupplied, FS_COUNT); }
inline bool Mutation::hasBases() const { return ST_ISALLSET(_grfSupplied, FS_BASES); }
inline bool Mutation::hasRange() const { return _ir.isSet(); }
inline bool Mutation::hasTransversion() const { return ST_ISALLSET(_grfSupplied, FS_TRANSVERSION); }

inline size_t Mutation::sourceIndex() const { return _iSource; }
inline size_t Mutation::targetIndex() const { return _iTarget; }
inline size_t Mutation::countBases() const  { return _cbBases; }
inline const std::string& Mutation::bases() const { return _strBases; }
inline const IndexRange& Mutation::range() const { return _ir; }

//--------------------------------------------------------------------------------
//
// StepMutation
//
//--------------------------------------------------------------------------------
inline StepMutation::StepMutation(MUTATIONTYPE mt) :
	Mutation(mt), _nLikelihood(1.0) {}
inline StepMutation::StepMutation(const StepMutation& sm) { operator=(sm); }

inline StepMutation& StepMutation::operator=(const StepMutation& sm)
{
	_nLikelihood = sm._nLikelihood;
	Mutation::operator=(*this);
	return *this;
}

inline UNIT StepMutation::getLikelihood() const { return _nLikelihood; }

//--------------------------------------------------------------------------------
//
// Step
//
//--------------------------------------------------------------------------------
inline Step::Step() { clear(); }
inline Step::Step(const Step& st) { operator=(st); }

inline Step& Step::operator=(const Step& st)
{
	_cTrials = st._cTrials;
	_dIndex = st._dIndex;
	_ir = st._ir;

	_tc = st._tc;

	_vecMutations = st._vecMutations;
	return *this;
}

inline void Step::initialize() { _tc.initialize(); }

inline size_t Step::getTrials() const { return _cTrials; }

inline bool Step::hasConditions(STFLAGS grfConditions) const { return _tc.hasConditions(grfConditions); }
inline bool Step::evaluateCondition(PLANCONDITION pc, UNIT nValue) { return _tc.evaluate(pc, nValue); }
inline size_t Step::getMutationsPerAttempt() const { return _tc.getMutationsPerAttempt(); }

//--------------------------------------------------------------------------------
//
// Plan
//
//--------------------------------------------------------------------------------
inline Plan::Plan() { initialize(); }

inline void Plan::clear() { initialize(); }

inline bool Plan::isExecuting() const { return _fExecuting; }

inline bool Plan::evaluateCondition(PLANCONDITION pc, UNIT nValue)
{
	return (_vecSteps.size() && _vecSteps[_iStep].hasConditions(pc)
		? _vecSteps[_iStep].evaluateCondition(pc, nValue)
		: _tc.evaluate(pc, nValue));
}
inline size_t Plan::getMutationsPerAttempt() const
{
	return (_vecSteps.size() && _vecSteps[_iStep].hasConditions(PC_TRIALMUTATION)
		? _vecSteps[_iStep].getMutationsPerAttempt()
		: _tc.getMutationsPerAttempt());
}

inline void Plan::beginExecution() { _fExecuting = true; }
inline void Plan::endExecution() { _fExecuting = false; }

//--------------------------------------------------------------------------------
//
// PlanScope
//
//--------------------------------------------------------------------------------
inline PlanScope::PlanScope(Plan& plan) :
	_plan(plan)
{
	_plan.beginExecution();
}
inline PlanScope::~PlanScope()
{
	_plan.endExecution();
}
