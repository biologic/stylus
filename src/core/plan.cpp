/*******************************************************************************
 * \file	plan.cpp
 * \brief	Stylus Plan and helper classes
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
// TrialValue
//
//--------------------------------------------------------------------------------
 
/*
 * Function: load
 * 
 */
void
TrialValue::load(XMLDocument* pxd, xmlNodePtr pxnValue)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnValue));
	
	string str;
	
	pxd->getAttribute(pxnValue, xmlTag(XT_VALUE), str);
	_nValueOriginal = str;
	_nValue = str;

	if (pxd->getAttribute(pxnValue, xmlTag(XT_LIKELIHOOD), str))
		_nLikelihood = str;
	
	if (pxd->getAttribute(pxnValue, xmlTag(XT_FACTOR), str))
		_nFactor = str;
}

/*
 * Function: toXML
 * 
 */
void
TrialValue::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_VALUE));
	xs.writeAttribute(xmlTag(XT_VALUE), _nValueOriginal);
	if (_nLikelihood != 1.0)
		xs.writeAttribute(xmlTag(XT_LIKELIHOOD), _nLikelihood);
	if (_nFactor != 1.0)
		xs.writeAttribute(xmlTag(XT_FACTOR), _nFactor);
	xs.closeStart(false);
}


//--------------------------------------------------------------------------------
//
// TrialCondition
//
//--------------------------------------------------------------------------------
const char* TrialCondition::s_aryTRIALCONDITIONMODE[TCM_MAX] =
{
	"none",
	"decrease",
	"increase",
	"maintain"
};

/*
 * Function: evaluatePerformance
 * 
 */
UNIT
TrialCondition::evaluatePerformance(UNIT nValue) const
{
	ENTER(VALIDATION,evaluatePerformance);
	
    switch (_tcm)
    {
    case TCM_INCREASE:
        return nValue;

    case TCM_DECREASE:
        return -nValue;
    
    default:
        return 0.0;
    }
}

UNIT
TrialCondition::getPerformancePrecision() const
{
    ASSERT( _vecValues.size() == 1 );
    return _vecValues[0].getFactor();
}


/*
 * Function: evaluate
 * 
 */
bool
TrialCondition::evaluate(UNIT nValue) 
{
	ENTER(VALIDATION,evaluate);
	
	bool fSuccess = true;

	if (_tcm != TCM_NONE)
	{
		ASSERT(_vecValues.size() > 0);

		size_t iValue = 0;
		if (_vecValues.size() > 1)
		{
			size_t iValue = 0;
			Unit pLikelihood = RGenerator::getUniform();
			Unit pProbability(0);
			while ((pProbability + _vecValues[iValue].getLikelihood()) < pLikelihood)
				pProbability += _vecValues[iValue++].getLikelihood();
		}
		
		TrialValue& tv = _vecValues[iValue];

		TFLOW(PLAN,L2,(LLTRACE, "Selected %6.4f %s value", tv.getValue(), xmlTag(_xt)));

		switch (_tcm)
		{
		case TCM_MAINTAIN:
			if (tv.getValue() >= 0.0)
				fSuccess = (Unit(nValue) >= tv.getValue());
			else
				fSuccess = (Unit(nValue) <= -tv.getValue());
			break;

		case TCM_INCREASE:
			fSuccess = (Unit(nValue) > tv.getValue());
			if (fSuccess)
				tv.setValue(nValue);
			break;

		case TCM_DECREASE:
			fSuccess = (Unit(nValue) < tv.getValue());
			if (fSuccess)
				tv.setValue(nValue);
			break;
		
		default:
			break;
		}

		if (!fSuccess)
		{
			Genome::recordAttempt(ST_FILELINE, STTR_SCORING,
								"%s failed: Value(%0.15f) Threshold(%0.15f) Mode(%s)",
								xmlTag(_xt), nValue, tv.getValue(), TrialCondition::s_aryTRIALCONDITIONMODE[_tcm]);
			Genome::recordTermination(NULL, STGT_SCORE, _stgr,
								"%s failed: Value(%0.15f) Threshold(%0.15f) Mode(%s)",
								xmlTag(_xt), nValue, tv.getValue(), TrialCondition::s_aryTRIALCONDITIONMODE[_tcm]);
		}
	}
	
	return fSuccess;
}

/*
 * Function: initialize
 * 
 */
void
TrialCondition::initialize(UNIT nValueDefault)
{
	ENTER(PLAN,initialize);

	// Ensure at least one value exists
	if (_vecValues.size() <= 0)
		_vecValues.push_back(TrialValue(1.0, nValueDefault));

	// If increasing and the default value is above the supplied value, use the default
	if (_tcm == TCM_INCREASE)
	{
		if (Unit(_vecValues[0].getValue()) < nValueDefault)
		{
			_vecValues[0].setValue(nValueDefault);
			LOGINFO((LLINFO, "Default value is greater than supplied value to increase - adjusting value"));
		}
	}

	// If decreasing and the default value is below the supplied value, use the default
	else if (_tcm == TCM_DECREASE)
	{
		if (Unit(_vecValues[0].getValue()) > nValueDefault)
		{
			_vecValues[0].setValue(nValueDefault);
			LOGINFO((LLINFO, "Default value is less than supplied value to decrease - adjusting value"));
		}
	}
}

/*
 * Function: load
 * 
 */
void
TrialCondition::load(XMLDocument* pxd, xmlNodePtr pxnTrialCondition)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnTrialCondition));

	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;
	string str;

	pxd->getAttribute(pxnTrialCondition, xmlTag(XT_MODE), str);
	_tcm = (str == xmlTag(XT_MAINTAIN)
			? TCM_MAINTAIN
			: (str == xmlTag(XT_INCREASE)
				? TCM_INCREASE
				: (str == xmlTag(XT_DECREASE)
					? TCM_DECREASE
					: TCM_NONE)));

	// Set the context to the thresholdCondition
	spxpc->node = pxnTrialCondition;
	
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_VALUE));
	if (XMLDocument::isXPathSuccess(spxpo.get()))
		loadFromXML<TRIALVALUEARRAY>(_vecValues, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);
		
	if ((_tcm == TCM_INCREASE || _tcm == TCM_DECREASE) && _vecValues.size() > 1)
		THROWRC((RC(XMLERROR), "Condition mode %s cannot have more than one value", s_aryTRIALCONDITIONMODE[_tcm]));
		
	if (_tcm != TCM_INCREASE && _tcm != TCM_DECREASE && _vecValues[0].getFactor() != 1.0)
		THROWRC((RC(XMLERROR), "A factor is allowed only with increase or decrease mode"));
	
	Unit cTotalLikelihood = 0.0;
	for (size_t iValue=0; iValue < _vecValues.size(); ++iValue)
	{
 		cTotalLikelihood += _vecValues[iValue].getLikelihood();
		if ((_tcm != TCM_MAINTAIN) && (_vecValues[iValue].getValue() < 0.0))
			THROWRC((RC(XMLERROR),
					"The value (%6.4f) is illegal - it must be positive",
					_vecValues[iValue].getValue()));
	}

	if (cTotalLikelihood != 1.0)
		THROWRC((RC(XMLERROR),
				"The sum of all value likelihoods (%6.4f) is illegal - it must equal 1.0",
				static_cast<UNIT>(cTotalLikelihood)));
}

/*
 * Function: toXML
 * 
 */
void
TrialCondition::toXML(XMLStream& xs, size_t iGene)
{
	ENTER(PLAN,toXML);

	xs.openStart(xmlTag(_xt));
	if (iGene > 0)
		xs.writeAttribute(xmlTag(XT_GENE), iGene);
	xs.writeAttribute(xmlTag(XT_MODE), TrialCondition::s_aryTRIALCONDITIONMODE[_tcm]);
	xs.closeStart();
	
	for (size_t iValue=0; iValue < _vecValues.size(); ++iValue)
		_vecValues[iValue].toXML(xs);
	
	xs.writeEnd(xmlTag(_xt));
}


//--------------------------------------------------------------------------------
//
// MutationsPerAttempt
//
//--------------------------------------------------------------------------------

/*
 * Function: load
 * 
 */
void
MutationsPerAttempt::load(XMLDocument* pxd, xmlNodePtr pxnMutationsPerAttempt)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnMutationsPerAttempt));
	
	string str;
	
	pxd->getAttribute(pxnMutationsPerAttempt, xmlTag(XT_LIKELIHOOD), str);
	_nLikelihood = str;
	
	pxd->getAttribute(pxnMutationsPerAttempt, xmlTag(XT_COUNT), str);
	_cMutations = ::atol(str.c_str());

	TDATA(PLAN,L2,(LLTRACE, "Plan will execute %ld mutations per trial with a %6.4f likelihood", _cMutations, static_cast<UNIT>(_nLikelihood)));
}

/*
 * Function: toXML
 * 
 */
void
MutationsPerAttempt::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_MUTATIONSPERATTEMPT));
	xs.writeAttribute(xmlTag(XT_LIKELIHOOD), _nLikelihood);
	xs.writeAttribute(xmlTag(XT_COUNT), _cMutations);
	xs.closeStart(false);
}


//--------------------------------------------------------------------------------
//
// MutationTrialCondition
//
//--------------------------------------------------------------------------------

/*
 * Function: getMutationsPerAttempt
 * 
 */
size_t
MutationTrialCondition::getMutationsPerAttempt() const
{
	ENTER(PLAN,getMutationsPerAttempt);
	ASSERT(_vecMutationsPerAttempt.size() > 0);
	
	Unit pLikelihood(1.0);
	size_t cMutationsPerAttempt;
	
	if (_vecMutationsPerAttempt.size() == 1)
		cMutationsPerAttempt = _vecMutationsPerAttempt[0].getCount();
		
	else
	{
		size_t iMutationsPerAttempt = 0;
		Unit pProbability(0);
		pLikelihood = RGenerator::getUniform();
		while ((pProbability + _vecMutationsPerAttempt[iMutationsPerAttempt].getLikelihood()) < pLikelihood)
			pProbability += _vecMutationsPerAttempt[iMutationsPerAttempt++].getLikelihood();
		cMutationsPerAttempt = _vecMutationsPerAttempt[iMutationsPerAttempt].getCount();
	}
	TFLOW(PLAN,L2,(LLTRACE, "Selected %ld mutations per trial (%6.4f likelihood)",
							cMutationsPerAttempt, static_cast<UNIT>(pLikelihood)));
	
	return cMutationsPerAttempt;
}



/*
 * Function: load
 * 
 */
void
MutationTrialCondition::load(XMLDocument* pxd, xmlNodePtr pxnMutationTrialCondition)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnMutationTrialCondition));

	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;

	// Set the context to the mutationCondition
	spxpc->node = pxnMutationTrialCondition;
    _vecMutationsPerAttempt.clear();
	
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_MUTATIONSPERATTEMPT));
	if (XMLDocument::isXPathSuccess(spxpo.get()))
		loadFromXML<MUTATIONSPERATTEMPTARRAY>(_vecMutationsPerAttempt, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

    if( _vecMutationsPerAttempt.empty() )
        clear();
	
	Unit cTotalLikelihood = 0.0;
	for (size_t iMutationsPerAttempt=0; iMutationsPerAttempt < _vecMutationsPerAttempt.size(); ++iMutationsPerAttempt)
		cTotalLikelihood += _vecMutationsPerAttempt[iMutationsPerAttempt].getLikelihood();
	if (cTotalLikelihood != 1.0)
		THROWRC((RC(XMLERROR),
				"The sum of all mutationsPerAttempt likelihoods (%6.4f) is illegal - it must equal 1.0",
				static_cast<UNIT>(cTotalLikelihood)));

    std::string mode_string;
    pxd->getAttribute(pxnMutationTrialCondition, xmlTag(XT_MODE), mode_string );
    _fExhaustive = (mode_string == xmlTag(XT_EXHAUSTIVE));
}

void
MutationTrialCondition::produceMutations(MutationSource & source, MutationSelector & selector)  const
{
    Mutation m;
    bool fSuccess = true;
    size_t cMutationsPerAttempt = getMutationsPerAttempt();
    ASSERT( cMutationsPerAttempt == 1 || !_fExhaustive );
    if( _fExhaustive )
    {
        selector.startMutations(false);
        source.produceMutations(selector);
    }
    else
    {
        selector.startMutations(true);
        // Apply the selected number of mutations for this attempt
        for (size_t cMutationsApplied=0; fSuccess && cMutationsApplied < cMutationsPerAttempt; ++cMutationsApplied)
        {
            // Get the next mutation from the step and note if a rollback is possible
            source.getMutation(m);
            TFLOW(PLAN,L2,(LLTRACE, "Executing a %s mutation for mutation %lu of %lu in trial %lu",
                m.toString(true).c_str(), (cMutationsApplied+1), cMutationsPerAttempt, Genome::getTrial()));

            // Apply the mutation
            fSuccess = selector.addMutation(m);

            if(!fSuccess)
            {
                TDATA(PLAN,L3,(LLTRACE, "Mutation %s failed", m.toString().c_str()));
            }
        }
        selector.mutationFinalize();
    }
}

/*
 * Function: toXML
 * 
 */
void
MutationTrialCondition::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_MUTATIONCONDITION));
    xs.writeAttribute( xmlTag(XT_MODE), _fExhaustive ? "exhaustive" : "random" );
    xs.closeStart();

	for (size_t iMutationsPerAttempt=0; iMutationsPerAttempt < _vecMutationsPerAttempt.size(); ++iMutationsPerAttempt)
		_vecMutationsPerAttempt[iMutationsPerAttempt].toXML(xs);
	
	xs.writeEnd(xmlTag(XT_MUTATIONCONDITION));
}


//--------------------------------------------------------------------------------
//
// TrialConditions
//
//--------------------------------------------------------------------------------
 
/*
 * Function: initialize
 * 
 */
void
TrialConditions::initialize()
{
	ENTER(PLAN,initialize);
	
	if (ST_ISALLSET(_grfConditions, PC_TRIALCOST))
		_cc.initialize();
	if (ST_ISALLSET(_grfConditions, PC_TRIALFITNESS))
		_fc.initialize();
	if (ST_ISALLSET(_grfConditions, PC_TRIALSCORE))
		_sc.initialize();
}
 
/*
 * Function: load
 * 
 */
void
TrialConditions::load(XMLDocument* pxd, xmlNodePtr pxnTrialConditions)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnTrialConditions));
	
	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;

	spxpc->node = pxnTrialConditions;

	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_TRIALCOST));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		_grfConditions |= PC_TRIALCOST;
		_cc.load(pxd, spxpo->nodesetval->nodeTab[0]);
	}
	
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_TRIALFITNESS));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		_grfConditions |= PC_TRIALFITNESS;
		_fc.load(pxd, spxpo->nodesetval->nodeTab[0]);
	}
	
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_TRIALSCORE));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		_grfConditions |= PC_TRIALSCORE;
		_sc.load(pxd, spxpo->nodesetval->nodeTab[0]);
	}
	
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_TRIALMUTATION));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		_grfConditions |= PC_TRIALMUTATION;
		_mc.load(pxd, spxpo->nodesetval->nodeTab[0]);
	}
}

/*
 * Function: toXML
 * 
 */
void
TrialConditions::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);

	if (ST_ISANYSET(_grfConditions, PC_TRIALCOST | PC_TRIALFITNESS | PC_TRIALSCORE | PC_TRIALMUTATION))
	{
		xs.writeStart(xmlTag(XT_TRIALCONDITIONS));
		
		if (ST_ISALLSET(_grfConditions, PC_TRIALCOST))
			_cc.toXML(xs);
		if (ST_ISALLSET(_grfConditions, PC_TRIALFITNESS))
			_fc.toXML(xs);
		if (ST_ISALLSET(_grfConditions, PC_TRIALSCORE))
			_sc.toXML(xs);
		if (ST_ISALLSET(_grfConditions, PC_TRIALMUTATION))
			_mc.toXML(xs);

		xs.writeEnd(xmlTag(XT_TRIALCONDITIONS));
	}
}


//--------------------------------------------------------------------------------
//
// DurationTerminationCondition
//
//--------------------------------------------------------------------------------

/*
 * Function: evaluateScore
 * 
 */
bool
DurationTerminationCondition::evaluate()
{
	ENTER(VALIDATION,evaluate);
	bool fSuccess = true;

	if (_fHasTrialLimit && (Genome::getTrial() - _iTrialInitial) >= _cTrials)
	{
		Genome::recordTermination(ST_FILELINE, STGT_DURATION, STGR_TRIALS, "Maximum plan trial duration met - plan ending after %lu trials", _cTrials);
		fSuccess = false;
	}

	if (_fHasAttemptLimit && (Genome::getTrialAttempts() - _cAttemptsInitial) >= _cAttempts)
	{
		Genome::recordTermination(ST_FILELINE, STGT_DURATION, STGR_ATTEMPTS, "Maximum plan attempt duration met - plan ending after %lu attempts", _cAttempts);
		fSuccess = false;
	}
	
	return fSuccess;
}

/*
 * Function: load
 * 
 */
void
DurationTerminationCondition::load(XMLDocument* pxd, xmlNodePtr pxnDurationTerminationCondition)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnDurationTerminationCondition));

	string str;
	
	_iTrialInitial = Genome::getTrial();

	if (pxd->getAttribute(pxnDurationTerminationCondition, xmlTag(XT_TRIALS), str))
	{
		_fHasTrialLimit = true;
		_cTrials = ::atol(str.c_str());
	}
	
	_cAttemptsInitial = Genome::getTrialAttempts();
	if (pxd->getAttribute(pxnDurationTerminationCondition, xmlTag(XT_ATTEMPTS), str))
	{
		_fHasAttemptLimit = true;
		_cAttempts = ::atol(str.c_str());
	}
}

/*
 * Function: toXML
 * 
 */
void
DurationTerminationCondition::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_DURATIONCONDITION));
	if (_fHasTrialLimit)
		xs.writeAttribute(xmlTag(XT_TRIALS), _cTrials);
	if (_fHasAttemptLimit)
		xs.writeAttribute(xmlTag(XT_ATTEMPTS), _cAttempts);
	xs.closeStart(false);
}


//--------------------------------------------------------------------------------
//
// RollbackTerminationCondition
//
//--------------------------------------------------------------------------------

/*
 * Function: evaluateScore
 * 
 */
bool
RollbackTerminationCondition::evaluate(size_t nRollbacks)
{
	ENTER(VALIDATION,evaluate);
	
	bool fSuccess = (nRollbacks <= _cRollbacks);
	if (!fSuccess)
		LOGINFO((LLINFO, "Maximum number of rollbacks executed in trial %lu - plan ending after %lu rollbacks", Genome::getTrial(), _cRollbacks));

	return fSuccess;
}

/*
 * Function: load
 * 
 */
void
RollbackTerminationCondition::load(XMLDocument* pxd, xmlNodePtr pxnRollbackTerminationCondition)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnRollbackTerminationCondition));

	string str;
	
	pxd->getAttribute(pxnRollbackTerminationCondition, xmlTag(XT_ROLLBACKLIMIT), str);
	_cRollbacks = (str == xmlTag(XT_INFINITE)
			? numeric_limits<size_t>::max()
			: ::atol(str.c_str()));
}

/*
 * Function: toXML
 * 
 */
void
RollbackTerminationCondition::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_ROLLBACKCONDITION));
	xs.writeAttribute(xmlTag(XT_ROLLBACKLIMIT), _cRollbacks);
	xs.closeStart(false);
}


//--------------------------------------------------------------------------------
//
// FitnessTerminationCondition
//
//--------------------------------------------------------------------------------

/*
 * Function: evaluateScore
 * 
 */
bool
FitnessTerminationCondition::evaluate(UNIT nFitness)
{
	ENTER(VALIDATION,evaluate);

	bool fAboveMinimum = true;
	bool fBelowMaximum = true;
	
	if (_nMinimum.isDefined())
	{
		fAboveMinimum = (nFitness > _nMinimum);
		if (!fAboveMinimum)
			Genome::recordTermination(ST_FILELINE, STGT_FITNESS, STGR_MINIMUM, "Fitness fell below requested minimum in trial %lu (%f)",
								Genome::getTrial(), static_cast<UNIT>(Genome::getFitness()));
	}
	
	if (_nMaximum.isDefined())
	{
		fBelowMaximum = (nFitness < _nMaximum);
		if (!fBelowMaximum)
			Genome::recordTermination(ST_FILELINE, STGT_FITNESS, STGR_MAXIMUM, "Fitness rose above requested maximum in trial %lu (%f)",
								Genome::getTrial(), static_cast<UNIT>(Genome::getFitness()));
	}

	return fAboveMinimum && fBelowMaximum;
}

/*
 * Function: load
 * 
 */
void
FitnessTerminationCondition::load(XMLDocument* pxd, xmlNodePtr pxnFitnessTerminationCondition)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnFitnessTerminationCondition));

	string str;
	
	if (pxd->getAttribute(pxnFitnessTerminationCondition, xmlTag(XT_MAXIMUM), str))
		_nMaximum = str;

	if (pxd->getAttribute(pxnFitnessTerminationCondition, xmlTag(XT_MINIMUM), str))
		_nMinimum = str;
}

/*
 * Function: toXML
 * 
 */
void
FitnessTerminationCondition::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_FITNESSCONDITION));
	xs.writeAttribute(xmlTag(XT_MAXIMUM), _nMaximum);
	xs.writeAttribute(xmlTag(XT_MINIMUM), _nMinimum);
	xs.closeStart(false);
}


//--------------------------------------------------------------------------------
// 
// IndexRange
// 
//--------------------------------------------------------------------------------

/*
 * Function: load
 * 
 */
void
IndexRange::load(XMLDocument* pxd, xmlNodePtr pxn)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxn));

	string str;

	if (pxd->getAttribute(pxn, xmlTag(XT_INDEXRANGE), str))
	{
		_grfSupplied |= FS_INDEXRANGE;
		
		size_t iPercent = str.find(Constants::s_chPERCENT);
		if (iPercent != string::npos)
		{
			ASSERT(str.find(Constants::s_chPERCENT, iPercent+1) < str.length());

			_fPercentageIndex = true;
			
			str[iPercent] = Constants::s_chBLANK;
			str[str.find(Constants::s_chPERCENT, iPercent+1)] = Constants::s_chBLANK;
		}

		_rgIndex.set(str);

		if (_fPercentageIndex)
		{
			if (_rgIndex.getStart() < 0 || _rgIndex.getEnd() > 100)
				THROWRC((RC(XMLERROR), "%s has an illegal indexRange of (%d,%d) - percentage values must range from 0 to 100", pxn->name, _rgIndex.getStart(), _rgIndex.getEnd()));
		}
		else
		{
			_rgIndex.move(-1);

			if (!Codon::onCodonBoundary(_rgIndex.getStart()))
				THROWRC((RC(XMLERROR), "%s has an illegal indexRange of (%d,%d) - index ranges must start on a codon boundary", pxn->name, _rgIndex.getStart() + 1, _rgIndex.getEnd() + 1));
		}

		if (_rgIndex.isEmpty())
			THROWRC((RC(XMLERROR), "%s has an illegal indexRange of %s", pxn->name, str.c_str()));
	}
	
	if (pxd->getAttribute(pxn, xmlTag(XT_GENERANGE), str))
	{
		if (ST_ISALLSET(_grfSupplied, FS_INDEXRANGE))
			THROWRC((RC(XMLERROR), "%s has both indexRange and geneRange - only one is allowed", pxn->name));
			
		_grfSupplied |= FS_GENERANGE;
		_iGene = ::atol(str.c_str())-1;
		if (_iGene > 0)
			THROWRC((RC(XMLERROR), "This version of Stylus required geneRange to be one - %ld was supplied on %s", (_iGene+1), pxn->name));
	}
	
	if (pxd->getAttribute(pxn, xmlTag(XT_HANSTROKERANGE), str))
	{
		if (!ST_ISALLSET(_grfSupplied, FS_GENERANGE))
			THROWRC((RC(XMLERROR), "%s specifies a hanStrokeRange without the required geneRange", pxn->name));
		_grfSupplied |= FS_HANSTROKE;

		Gene& gene = Genome::getGenes()[0];
		size_t iComma = str.find(Constants::s_chCOMMA);

		if (iComma >= str.length())
			THROWRC((RC(XMLERROR), "%s has a malformed hanStrokeRange of %s - it should contain a Unicode number and a stroke index separated by a comma",
									pxn->name, str.c_str()));

		_strUnicode = str.substr(0,iComma);
		if (_strUnicode != gene.getUnicode())
			THROWRC((RC(XMLERROR), "%s has an illegal Unicode reference of %s specified by hanStrokeRange - it should be %s",
									pxn->name, _strUnicode.c_str(), gene.getUnicode().c_str()));

		long iHanStroke = ::atol(str.substr(iComma+1).c_str());
		if (iHanStroke <= 0 || iHanStroke == LONG_MAX || iHanStroke == LONG_MIN)
			THROWRC((RC(XMLERROR), "%s has a malformed hanStrokeRange of %s - it should contain a Unicode number and a stroke index separated by a comma",
									pxn->name, str.c_str()));

		if (static_cast<size_t>(iHanStroke) > gene.getStrokes().size())
			THROWRC((RC(XMLERROR), "%s specifies a hanStrokeRange with an illegal stroke value of %ld - it must range from 1 to %ld", pxn->name, iHanStroke, gene.getStrokes().size()));
		_iHanStroke = iHanStroke - 1;
	}
}

/*
 * Function: toXML
 * 
 */
void
IndexRange::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	if (ST_ISALLSET(_grfSupplied, FS_INDEXRANGE))
	{
		if (_fPercentageIndex)
		{
			ostringstream ostr;
			ostr << _rgIndex.getStart() << Constants::s_chPERCENT << Constants::s_chBLANK << _rgIndex.getEnd() << Constants::s_chPERCENT;
			xs.writeAttribute(xmlTag(XT_INDEXRANGE), ostr.str());
		}
		else
		{
			_rgIndex.move(1);
			xs.writeAttribute(xmlTag(XT_INDEXRANGE), _rgIndex.toString(false));
			_rgIndex.move(-1);
		}
	}
	if (ST_ISALLSET(_grfSupplied, FS_GENERANGE))
		xs.writeAttribute(xmlTag(XT_GENERANGE), _iGene);
	if (ST_ISALLSET(_grfSupplied, FS_HANSTROKE))
	{
		ostringstream ostr;
		ostr << _strUnicode << "," << (_iHanStroke+1);
		xs.writeAttribute(xmlTag(XT_HANSTROKERANGE), ostr.str());
	}
}

/*
 * Function: toString
 * 
 */
string
IndexRange::toString() const
{
	ostringstream ostr;

	if (ST_ISALLSET(_grfSupplied, FS_INDEXRANGE))
	{
		ostr << _rgIndex.toString();
		if (_fPercentageIndex)
			ostr << Constants::s_chPERCENT;
	}
	else
		ostr << "-";

	if (ST_ISALLSET(_grfSupplied, FS_GENERANGE))
		ostr << "," << (_iGene+1);
	else
		ostr << ",-";

	if (ST_ISALLSET(_grfSupplied, FS_HANSTROKE))
		ostr << ",(" << _strUnicode << "," << (_iHanStroke+1) << ")";
	else
		ostr << ",-";

	return string(ostr.str());
}

/*
 * Function: getRange
 * 
 */
Range
IndexRange::getRange() const
{
	ENTER(PLAN,getRange);
	Range rg;

	if (_grfSupplied == FS_NONE)
		rg.set(Codon::s_cchCODON, (Genome::getBases().length() - 1) - Codon::s_cchCODON);
	else if (ST_ISALLSET(_grfSupplied, FS_INDEXRANGE))
	{
		if (_fPercentageIndex)
		{
			size_t cbCodons = (Genome::getBases().length() / Codon::s_cchCODON) - 2;

			rg.set((((((cbCodons * _rgIndex.getStart()) + 50) / 100) + 1) * Codon::s_cchCODON),
				   (((((cbCodons * _rgIndex.getEnd()) + 50) / 100) + 1) * Codon::s_cchCODON) - 1);
		}
		else
			rg.set(_rgIndex);

		DASSERT(Codon::onCodonBoundary(rg.getStart()));
	}
	else if (ST_ISALLSET(_grfSupplied, FS_HANSTROKE))
	{
		Gene& gene = Genome::getGenes()[_iGene];
		rg.set(gene.codonToBaseRange(gene.getStrokes()[_iHanStroke].getRange()));
	}
	else
	{
		ASSERT(ST_ISALLSET(_grfSupplied, FS_GENERANGE));
		rg.set(Genome::getGenes()[_iGene].getRange());
	}
	
	if (rg.isEmpty() || static_cast<size_t>(rg.getEnd()) >= Genome::getBases().length())
		THROWRC((RC(ERROR), "Index range (%s) is invalid for trial %lu", toString().c_str(), Genome::getTrial()));
		
	return rg;
}


//--------------------------------------------------------------------------------
// 
// Mutation
// 
//--------------------------------------------------------------------------------
const char* Mutation::s_aryMUTATIONTYPE[MT_MAX] =
{
	"copy",
	"change",
	"delete",
	"insert",
	"transpose",
	"unknown"
};

const Unit Mutation::s_nTRANSVERSIONLIKELIHOOD(2.0 / 3.0);

/*
 * Function: initialize
 * 
 */
void
Mutation::initialize(MUTATIONTYPE mt)
{
	ENTER(PLAN,initialize);
	
	_mt = mt;
	_grfSupplied = 0;
	_iSource = 0;
	_iTarget = 0;
	_cbBases = 0;
	_strBases.clear();
	_ir.clear();
	_nTransversionLikelihood = s_nTRANSVERSIONLIKELIHOOD;
}

/*
 * Function: toString
 * 
 */
string
Mutation::toString(bool fTypeOnly) const
{
	if (fTypeOnly)
		return string(s_aryMUTATIONTYPE[_mt]);
	else
	{
		ostringstream ostr;

		ostr
			<< "("
			<< s_aryMUTATIONTYPE[_mt];
			
		if (hasSourceIndex())
			ostr << "," << _iSource;
		else
			ostr << ",-";

		if (hasTargetIndex())
			ostr << "," << _iTarget;
		else
			ostr << ",-";

		if (hasCountBases())
			ostr << "," << _cbBases;
		else
			ostr << ",-";

		if (hasBases())
			ostr << "," << _strBases;
		else
			ostr << ",-";

		ostr << "," << _ir.toString();
		
		if (hasTransversion())
			ostr << "," << _nTransversionLikelihood;

		ostr << ")";

		return string(ostr.str());
	}
}


//--------------------------------------------------------------------------------
// 
// StepMutation
// 
//--------------------------------------------------------------------------------

/*
 * Function: load
 * 
 */
void
StepMutation::load(XMLDocument* pxd, xmlNodePtr pxnStepMutation)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnStepMutation));

	Range rgGenomeBases(Codon::s_cchCODON, Genome::getBases().length()-Codon::s_cchCODON);
	string str;
	
	const char *pszStylusNamespace = pxd->getStylusNamespace();
	
	_mt = (XMLDocument::isXMLTag(pxnStepMutation, xmlTag(XT_CHANGE), pszStylusNamespace)
			? MT_CHANGE
			: (XMLDocument::isXMLTag(pxnStepMutation, xmlTag(XT_COPY), pszStylusNamespace)
				? MT_COPY
				: (XMLDocument::isXMLTag(pxnStepMutation, xmlTag(XT_DELETE), pszStylusNamespace)
					? MT_DELETE
					: (XMLDocument::isXMLTag(pxnStepMutation, xmlTag(XT_INSERT), pszStylusNamespace)
						? MT_INSERT
						: MT_TRANSPOSE))));

	_nLikelihood = 1.0;
	if (pxd->getAttribute(pxnStepMutation, xmlTag(XT_LIKELIHOOD), str))
	{
		_nLikelihood = str;
	}

	if (pxd->getAttribute(pxnStepMutation, xmlTag(XT_COUNTBASES), str))
	{
		long cb = ::atol(str.c_str());
		_grfSupplied |= FS_COUNT;
		if (cb <= 0 || static_cast<size_t>(cb) > rgGenomeBases.getLength())
			THROWRC((RC(XMLERROR), "%s mutation has an illegal countBases (%ld) - it must range from 1 to %ld",
									pxnStepMutation->name, cb, rgGenomeBases.getLength()));
		if (_mt != MT_CHANGE)
		{
			if (!Codon::hasWholeCodons(cb))
				THROWRC((RC(XMLERROR), "This version of Stylus requires countBases to be in whole codon units for all mutations except change - %ld is an illegal countBases value", cb));
		}
		else
		{
			if (cb != 1 && static_cast<size_t>(cb) != Codon::s_cchCODON)
				THROWRC((RC(XMLERROR), "This version of Stylus requires countBases to be either 1 or %ld for change mutations - %ld is an illegal countBases value", Codon::s_cchCODON, cb));
		}
		_cbBases = cb;
	}

	if (	(_mt == MT_CHANGE || _mt == MT_INSERT)
		&&	pxd->getAttribute(pxnStepMutation, xmlTag(XT_BASES), str))
	{
		_grfSupplied |= FS_BASES;
		if (_mt != MT_CHANGE)
		{
			if (!Codon::hasWholeCodons(str.length()))
				THROWRC((RC(XMLERROR), "This version of Stylus requires bases to contain whole codons for all mutations except change - %s does not contain whole codons", str.c_str()));
		}
		else
		{
			if (str.length() != 1 && str.length() != Codon::s_cchCODON)
				THROWRC((RC(XMLERROR), "This version of Stylus requires bases to be either 1 or %ld for change mutations - %s contains an illegal number of bases", Codon::s_cchCODON, str.c_str()));
		}
		if (!validateBases(str.c_str(), str.length()))
			THROWRC((RC(XMLERROR), "%s mutation has illegal bases (%s) - bases must be T, C, A, or G",
									pxnStepMutation->name, str.c_str()));
		_strBases = str;

		if (!ST_ISALLSET(_grfSupplied, FS_COUNT))
		{
			_grfSupplied |= FS_COUNT;
			_cbBases = _strBases.length();
		}
	}

	if ((_mt == MT_COPY || _mt == MT_TRANSPOSE) && pxd->getAttribute(pxnStepMutation, xmlTag(XT_SOURCEINDEX), str))
	{
		long iIndex = ::atol(str.c_str()) - 1;
		_grfSupplied |= FS_SOURCE;
		if (!rgGenomeBases.contains(iIndex))
			THROWRC((RC(XMLERROR), "%s mutation has an illegal sourceIndex (%ld) - it must range from %ld to %ld",
									pxnStepMutation->name, (iIndex+1), (rgGenomeBases.getStart()+1), (rgGenomeBases.getEnd()+1)));
		if (!Codon::onCodonBoundary(iIndex))
			THROWRC((RC(XMLERROR), "This version of Stylus requires the sourceIndex for copy and transpose mutations to be on a codon boundary - %ld is an illegal sourceIndex value", iIndex));
		_iSource = iIndex;
	}

	if (pxd->getAttribute(pxnStepMutation, xmlTag(XT_TARGETINDEX), str))
	{
		if (str == xmlTag(XT_TANDEM))
		{
			if (_mt != MT_COPY)
				THROWRC((RC(XMLERROR), "%s mutation has an illegal targetIndex (tandem) - targetIndex must range from %ld and %ld",
										pxnStepMutation->name, (rgGenomeBases.getStart()+1), (rgGenomeBases.getEnd()+1)));
			_grfSupplied |= FS_TANDEM;
		}
		else
		{
			long iIndex = ::atol(str.c_str()) - 1;
			_grfSupplied |= FS_TARGET;
			if (!rgGenomeBases.contains(iIndex))
				THROWRC((RC(XMLERROR), "%s mutation has an illegal targetIndex (%ld) - it must range from %ld to %ld",
										pxnStepMutation->name, (iIndex+1), (rgGenomeBases.getStart()+1), (rgGenomeBases.getEnd()+1)));

			if (	(	_mt != MT_CHANGE
					||	(	ST_ISALLSET(_grfSupplied, FS_COUNT)
						&&	_cbBases != 1))
				&&	!Codon::onCodonBoundary(_iTarget))
				THROWRC((RC(XMLERROR), "This version of Stylus requires the targetIndex for all mutations except single base changes to be on a codon boundary - %ld is an illegal targetIndex value", _iTarget));

			_iTarget = iIndex;
		}
	}

	_ir.load(pxd, pxnStepMutation);
	
	if (_mt == MT_CHANGE && pxd->getAttribute(pxnStepMutation, xmlTag(XT_TRANSVERSIONLIKELIHOOD), str))
	{
		if (ST_ISALLSET(_grfSupplied, FS_COUNT) && _cbBases != 1)
			THROWRC((RC(XMLERROR), "%s mutation specifies a transversionLikelihood for a change of more than one base - transversionLikelihood only applies to single base changes",
									pxnStepMutation->name));

		_nTransversionLikelihood = str;
		_grfSupplied |= FS_TRANSVERSION;
	}
	
	if (ST_ISALLSET(_grfSupplied, FS_BASES | FS_COUNT) && _cbBases != _strBases.length())
		THROWRC((RC(XMLERROR), "%s mutation has a countBases (%ld) value that disagrees with the number of supplied bases (%ld)",
								pxnStepMutation->name, _cbBases, _strBases.length()));
								
	if (ST_ISALLSET(_grfSupplied, FS_SOURCE) && !ST_ISALLSET(_grfSupplied, FS_TARGET))
		THROWRC((RC(XMLERROR), "This version of Stylus requires that %s mutations either supply or withold both sourceIndex and targetIndex", pxnStepMutation->name));
		
	if (ST_ISALLSET(_grfSupplied, FS_SOURCE | FS_COUNT) && ST_ISANYSET(_grfSupplied, FS_TARGET | FS_TANDEM))
	{
		size_t iTarget = (ST_ISALLSET(_grfSupplied, FS_TARGET) ? _iTarget : _iSource + _cbBases);
		if (_cbBases > (_iSource > iTarget ? Genome::getBases().length() - _iSource : iTarget - _iSource))
			THROWRC((RC(XMLERROR), "%s mutation has a countBases (%ld) value (actual or implied) that exceeds the available bases at sourceIndex (%ld)",
									pxnStepMutation->name, _cbBases, (_iSource+1)));
	}
	
	if (_mt == MT_CHANGE && ST_ISALLSET(_grfSupplied, FS_COUNT | FS_TARGET) && (_iTarget + _cbBases) > Genome::getBases().length())
		THROWRC((RC(XMLERROR), "%s mutation has a countBases (%ld) value (actual or implied) that exceeds the available bases at targetIndex (%ld)",
								pxnStepMutation->name, _cbBases, (_iTarget+1)));
}

/*
 * Function: toXML
 * 
 */
void
StepMutation::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	const char* pszTag = xmlTag(_mt == MT_CHANGE
								? XT_CHANGE
								: (_mt == MT_COPY
									? XT_COPY
									: (_mt == MT_DELETE
										? XT_DELETE
										: (_mt == MT_INSERT
											? XT_INSERT
											: XT_TRANSPOSE))));
	
	xs.openStart(pszTag);
	xs.writeAttribute(xmlTag(XT_LIKELIHOOD), _nLikelihood);
	if (hasSourceIndex())
		xs.writeAttribute(xmlTag(XT_SOURCEINDEX), _iSource);
	if (hasTargetIndex())
		xs.writeAttribute(xmlTag(XT_TARGETINDEX), _iTarget);
	else if (hasTandemIndex())
		xs.writeAttribute(xmlTag(XT_TARGETINDEX), xmlTag(XT_TANDEM));
	if (hasCountBases())
		xs.writeAttribute(xmlTag(XT_COUNTBASES), _cbBases);
	if (hasBases())
		xs.writeAttribute(xmlTag(XT_BASES), _strBases);
	_ir.toXML(xs);
	if (hasTransversion())
		xs.writeAttribute(xmlTag(XT_TRANSVERSIONLIKELIHOOD), _nTransversionLikelihood);
	xs.closeStart(false);
}

//--------------------------------------------------------------------------------
// 
// Step
// 
//--------------------------------------------------------------------------------

/*
 * Function: getMutation
 * 
 */
void
Step::getMutation(Mutation& m, STFLAGS grfOptions, size_t iTrialInStep) const
{
	ENTER(PLAN,getMutation);
		
	// Select the mutation to use
	// Note: This loop assumes the total likelihoods are equal to 1.0
	size_t iMutation = 0;
	if (_vecMutations.size() > 1)
	{
		Unit pLikelihood = RGenerator::getUniform();
		Unit pProbability(0);
		while ((pProbability + _vecMutations[iMutation].getLikelihood()) < pLikelihood)
			pProbability += _vecMutations[iMutation++].getLikelihood();
		TFLOW(PLAN,L2,(LLTRACE, "Selected mutation %s (%6.4f likelihood)",
								_vecMutations[iMutation].toString(true).c_str(), static_cast<UNIT>(pLikelihood)));
	}
	
	ASSERT(iMutation < _vecMutations.size());
	m = _vecMutations[iMutation];
	bool fWillInsert = (m.isCopy() || m.isInsert() || m.isTranspose());
	Range rg;

	const IndexRange& ir = (m.hasRange() ? m.range() : _ir);
	rg.set(ir.getRange());

	// Shift step-level indexrange by delta index, if delta index supplied and
	// no per-mutation indexrange is specified
	if (_dIndex && !m.hasRange())
	{
		long codon = Codon::s_cchCODON;
		long range = Genome::getBases().length() - (2 * codon);
		long shift = (_dIndex * iTrialInStep) % range;

		if (rg.getLength() >= static_cast<size_t>(range))
			rg.set(codon, codon + range - 1);
		else
		{
			long offset = (rg.getStart() - codon);

			if (offset >= range)
				rg.move((offset % range) - offset);

			DASSERT(	(rg.getStart() >= codon)
					&&	(rg.getStart() < (codon + range)));

			if (	((rg.getEnd() + shift) < (codon + range))
				&&	((rg.getEnd() + shift) >= rg.getEnd()))
				rg.move(shift);
			else if (	((rg.getStart() + shift) >= (codon + range))
					||	((rg.getStart() + shift) < rg.getStart()))
				rg.move(shift - range);
			else
			{
				long oldlen = rg.getLength();
				rg.set(rg.getStart() + shift, codon + range - 1);

				long random = RGenerator::getUniform(0L, oldlen - 1);

				if (	((rg.getStart() + random) >= (codon + range))
					||	((rg.getStart() + random) < rg.getStart()))
					rg.set(codon, codon + (oldlen - rg.getLength()) - 1);
			}
		}
	}

	// Populate any missing fields needed by the mutation
	if (!m.hasCountBases())
	{
		ASSERT(!m.hasBases());
		ASSERT(Genome::_strBases.length() > Constants::s_nMAXIMUMGENERATEDLENGTH);
		m._cbBases = (m.isChange()
						? 1
						: Genome::rgenLength(Constants::s_nMAXIMUMGENERATEDLENGTH, ST_ISALLSET(grfOptions, SO_ENSUREWHOLECODONS)));
	}
	
	if (m.isChange() && !m.hasTransversion())
	{
		m._nTransversionLikelihood = Mutation::s_nTRANSVERSIONLIKELIHOOD;
	}

	if (m.needsSourceIndex() && !m.hasSourceIndex())
	{
		size_t iRandomTrial = 0;
		do
			m._iSource = Genome::rgenIndex(rg, ST_ISALLSET(grfOptions, SO_ENSUREINFRAME));
		while (		(m._iSource + m._cbBases) >= Genome::_strBases.length()
				&&	++iRandomTrial < Genome::_strBases.length());
		if (iRandomTrial >= Genome::_strBases.length())
			THROWRC((RC(ERROR), "Unable to successfully generate a random mutation source index for mutation %s",
					 			m.toString().c_str()));
	}

	ASSERT(!m.hasTandemIndex() || !m.hasTargetIndex());
	if (m.hasTandemIndex())
		m._iTarget = m._iSource + m._cbBases;
	else if (!m.hasTargetIndex())
	{
		size_t iRandomTrial = 0;
		bool fEnsureInFrame = ST_ISALLSET(grfOptions, SO_ENSUREINFRAME) && (!m.isChange() || Codon::hasWholeCodons(m._cbBases));
		if (fWillInsert)
			ir.adjustRangeForInsert(rg);
		do
			m._iTarget = Genome::rgenIndex(rg, fEnsureInFrame);
		while (		(	m.isCopy()
					||	m.isTranspose())
				&&	(	m._iTarget == m._iSource
					||	(	m._iSource < m._iTarget
						&&	(m._iSource + m._cbBases) > m._iTarget))
				&&	++iRandomTrial < Genome::_strBases.length());
		if (iRandomTrial >= Genome::_strBases.length())
			THROWRC((RC(ERROR), "Unable to successfully generate a random mutation target index for mutation %s",
					 			m.toString().c_str()));
	}
	
	if (m.needsBases() && !m.hasBases())
	{
		if (m.isChange() && m._cbBases == 1)
			Genome::rgenTransversion(m._iTarget, m._strBases, m._nTransversionLikelihood);
		else
		{
			do
				Genome::rgenBases(m._strBases, m._cbBases);
			while (m.isChange() && !Genome::_strBases.compare(m._iTarget, m._cbBases, m._strBases));
		}
	}
}

void
Step::produceMutations(MutationSelector & selector, STFLAGS grfOptions, size_t iTrialInStep) 
{
	ENTER(PLAN,getMutation);

    ASSERT( _vecMutations.size() == 1 );
	Mutation m = _vecMutations[0];
	bool fWillInsert = (m.isCopy() || m.isInsert() || m.isTranspose());
	Range rg;

	const IndexRange& ir = (m.hasRange() ? m.range() : _ir);
	rg.set(ir.getRange());

    ASSERT( _dIndex == 0 );
    if( !m.hasCountBases() )
    {
        ASSERT( m.isChange() );
        m._cbBases = 1;
    }
    ASSERT( !m.needsSourceIndex() );

    ASSERT( !m.hasTandemIndex() );
    bool fEnsureInFrame = ST_ISALLSET(grfOptions, SO_ENSUREINFRAME) && (!m.isChange() || Codon::hasWholeCodons(m._cbBases));
    if (fWillInsert)
        ir.adjustRangeForInsert(rg);

    ASSERT( !m.hasTargetIndex() );

    TFLOW(MUTATION,L2,(LLTRACE, "Sampling mutation positions from %d to %d",
            rg.getStart(), rg.getEnd()));
    for(long iTarget = rg.getStart(); iTarget < rg.getEnd(); iTarget++)
   {
        if( !fEnsureInFrame || Codon::onCodonBoundary(iTarget) )
        {

            TFLOW(MUTATION,L2,(LLTRACE, "Starting target position %d", iTarget));
            m._iTarget = iTarget;
            ASSERT(!m.isCopy());
            ASSERT(!m.isTranspose());
            ASSERT(m.needsBases() && !m.hasBases());

            ASSERT(Constants::s_strBASES.length() == 4);
            size_t counts[m._cbBases+1];
            memset(counts, 0, sizeof(counts));
            m._strBases.resize(m._cbBases);
            while(!counts[m._cbBases])
            {
                for(size_t idx = 0; idx < m._cbBases;idx++)
                {
                    m._strBases[idx] = Constants::s_strBASES[counts[idx]];
                }
                // skip silent changes
                if( Genome::_strBases.compare(m._iTarget, m._cbBases, m._strBases) )
                {
                    TFLOW(MUTATION,L2,(LLTRACE, "Mutation with bases %s", m._strBases.c_str()));
                    selector.addMutation(m);
                    selector.mutationFinalize();
                }
                size_t increment_pos = 0;
                while(true)
                {
                    counts[increment_pos]++;
                    if( counts[increment_pos] == 4 )
                    {
                        counts[increment_pos] = 0;
                        increment_pos++;
                    }
                    else
                    {
                        break;
                    }

                }

            }

        }
    }
}

void Step::checkSupportsExhaustive()
{
    if( _vecMutations.size() != 1 )
        THROWRC((RC(XMLERROR), "Only one type of mutation supported when exhaustive mode is enabled"));

    Mutation & m = _vecMutations[0];
    
    if( !m.isChange() )
        THROWRC((RC(XMLERROR), "Only change mutations are supported when exhaustive mode is enabled"));
    
    if( m.hasTargetIndex() )
        THROWRC((RC(XMLERROR), "Specifying the target position is not supported when exhaustive mode is enabled"));

    if( m.hasTransversion() )
        THROWRC((RC(XMLERROR), "Transversion likelihood is not supported when exhaustive mode is enabled"));

    if( m.hasBases() )
        THROWRC((RC(XMLERROR), "Specifying the bases is not supported when exhaustive mode is enabled"));
}


/*
 * Function: load
 * 
 */
void
Step::load(XMLDocument* pxd, xmlNodePtr pxnStep)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnStep));

	Range rgGenomeBases(Codon::s_cchCODON, Genome::_strBases.length()-Codon::s_cchCODON);
	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;
	string str;
	size_t ul;
	long nDelta;

	// Load step properties
	ul = 1;
	if (pxd->getAttribute(pxnStep, xmlTag(XT_TRIALS), str))
	{
		ul = (str == xmlTag(XT_INFINITE)
				? numeric_limits<size_t>::max()
				: ::atol(str.c_str()));
	}
	ASSERT(ul > 0);
	_cTrials = ul;

	nDelta = (pxd->getAttribute(pxnStep, xmlTag(XT_DELTAINDEX), str)
			? ::atol(str.c_str())
			: 0);
	
	size_t range = rgGenomeBases.getLength() - (2 * Codon::s_cchCODON);

	if (static_cast<size_t>(std::abs(nDelta)) >= range)
		THROWRC((RC(XMLERROR), "Step has an illegal deltaIndex of %ld - it must be range from %ld to %ld",
								nDelta, -(range-1), (range-1)));

	if (!Codon::onCodonBoundary(nDelta))
		THROWRC((RC(XMLERROR), "Step has an illegal deltaIndex of %ld - it must be a multiple of codon size (%ld)",
								Codon::s_cchCODON));

	_dIndex = nDelta;

	_ir.load(pxd, pxnStep);

	// Set the context to the step
	spxpc->node = pxnStep;
	
	// Load any trial conditions that exist
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_TRIALCONDITIONS));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
		_tc.load(pxd, spxpo->nodesetval->nodeTab[0]);

	// Load the mutations for the step
	spxpo = pxd->evalXPath(spxpc.get(), xmlXPath(XP_MUTATIONS));
	if (!XMLDocument::isXPathSuccess(spxpo.get()))
		THROWRC((RC(XMLERROR), "Step contains no mutations"));
	loadFromXML<STEPMUTATIONARRAY>(_vecMutations, pxd, spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

	Unit cTotalLikelihood = 0.0;
	for (size_t iMutation=0; iMutation < _vecMutations.size(); ++iMutation)
		cTotalLikelihood += _vecMutations[iMutation].getLikelihood();
	if (cTotalLikelihood != 1.0)
		THROWRC((RC(XMLERROR),
				"The sum of step mutation likelihoods (%6.4f) is illegal - it must equal 1.0",
				static_cast<UNIT>(cTotalLikelihood)));
}

/*
 * Function: toXML
 * 
 */
void
Step::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_STEP));
	xs.writeAttribute(xmlTag(XT_TRIALS), _cTrials);
	xs.writeAttribute(xmlTag(XT_DELTAINDEX), _dIndex);
	_ir.toXML(xs);
	xs.closeStart();

	_tc.toXML(xs);
	
	for (size_t iMutation=0; iMutation < _vecMutations.size(); ++iMutation)
		_vecMutations[iMutation].toXML(xs);
	
	xs.writeEnd(xmlTag(XT_STEP));
}

/**
 * Function: clear
 * 
 */
void
Step::clear()
{
	_cTrials = 1;
	_dIndex = 0;
	_ir.clear();

	_tc.clear();
	
	_vecMutations.clear();
}

//--------------------------------------------------------------------------------
//
// Plan
//
//--------------------------------------------------------------------------------

/*
 * Function: execute
 * 
 */
void
Plan::execute(size_t iTrialFirst, size_t cTrials, ST_PFNSTATUS pfnStatus, size_t cStatusRate)
{
	ENTER(PLAN,execute);
	
	ASSERT(Genome::isState(STGS_ALIVE));

	bool fPlanTerminated = false;
	
	// Ensure the trial being associated with the first plan step is not beyond the genome state
	// Note: The current genome trial represents the number of *completed* trials
	if ((Genome::getTrial()+1) < iTrialFirst)
		THROWRC((RC(BADARGUMENTS), "The next genome trial (%lu) is before the first trial associated with the plan (%lu)",
									(Genome::getTrial()+1), iTrialFirst));

	// First, advance to the step containing the correct trial
	size_t cTrialsInCompletedSteps = 0;
	_iStep = 0;
	while (_iStep < _vecSteps.size() && (Genome::getTrial()+1) > (iTrialFirst + cTrialsInCompletedSteps + _vecSteps[_iStep].getTrials()))
		cTrialsInCompletedSteps += _vecSteps[_iStep++].getTrials();
	if (	iTrialFirst > 0
		&&	(	(_iStep >= _vecSteps.size() && (Genome::getTrial()+1) > (iTrialFirst + cTrialsInCompletedSteps))
			||	(_iStep < _vecSteps.size() && (Genome::getTrial()+1) > (iTrialFirst + cTrialsInCompletedSteps + _vecSteps[_iStep].getTrials()))))
		THROWRC((RC(BADARGUMENTS), "Plan does not have enough steps to advance to the current trial state (%lu)", (Genome::getTrial()+1)));
	
	// If the number of trials was not specified, run all remaining steps
	// Otherwise, limit the requested number of trials to those remaining in the plan
	cTrials = getActualTrialCount(cTrials, iTrialFirst);

	LOGINFO((LLINFO, "Plan will execute %lu trials of %lu total possible starting with trial %lu in step %lu", cTrials, _cTrials, Genome::getTrial()+1, _iStep));
	LOGINFO((LLINFO, "Plan skipped execution of %lu steps to get to trial %lu", _iStep, (Genome::getTrial()+1)));
							
	// Advance the duration condition as well and initialize any plan-level trial conditions
	_dc.initialize(iTrialFirst, Genome::getTrialAttempts());
	_tc.initialize();
	
	// Move the plan into execution mode and execute the steps
	{
		PlanScope ps(*this);

		Mutation m;

		STFLAGS grfOptions = Step::SO_NONE;
		if (_fEnsureInFrame)
			grfOptions |= Step::SO_ENSUREINFRAME;
		if (_fEnsureWholeCodons)
			grfOptions |= Step::SO_ENSUREWHOLECODONS;


        MutationSelector mutationSelector(*this);

		for (; !fPlanTerminated && cTrials && _iStep < _vecSteps.size(); cTrialsInCompletedSteps += _vecSteps[_iStep].getTrials(), ++_iStep)
		{
			Step& st = _vecSteps[_iStep];
			
			// Initialize the step to align step-level trial conditions with current genome state
			st.initialize();

			// Execute the current step to completion (or exhaustion of allowed trials)
			while (!fPlanTerminated && cTrials && ((Genome::getTrial()+1-cTrialsInCompletedSteps-iTrialFirst) < st.getTrials()))
			{
				bool fTrialCompleted = false;
				size_t cRollbackAttempts = 0;

				// Advance the genome to the next trial
				Genome::advanceTrial(!_fAccumulateMutations);
				
				// Execute the current step until trial completion
				// - The number of mutations to apply varies with each *attempt* (rather than each trial)
				while (!fPlanTerminated && !fTrialCompleted)
				{
					bool fSuccess = true;

					// Advance the number of trial attempts
					Genome::advanceTrialAttempts();

                    mutationSelector.reset();

					TFLOW(PLAN,L3,(LLTRACE, "Executing trial attempt %d", Genome::getTrialAttempts()));

                    MutationSource source(st, grfOptions, Genome::getTrial()-cTrialsInCompletedSteps-iTrialFirst);
                    produceMutations(source, mutationSelector);
					
					// If all mutations applied, validate and score the genome
                    fSuccess = mutationSelector.selectMutation();

					// Convert a successful attempt into a successful trial and continue
					// - Condition or caller termination leaves the success state unchanged
					if (fSuccess)
					{
                        Genome::recordStatistics();
						fTrialCompleted = true;
						cTrials -= 1;
						
 						// If the genome meets a fitness condition, terminate the plan
						if (!_fc.evaluate(Genome::getFitness()))
							fPlanTerminated = true;

						// If a callback exists, notify the caller (and terminate if requested)
						else if (fSuccess && VALID(pfnStatus) && ((Genome::getTrial() % cStatusRate) != 0) && (*pfnStatus)())
						{
							Genome::recordTermination(ST_FILELINE, STGT_CALLBACK, STGR_TERMINATED, "Callback ended plan execution at trial %ld", Genome::getTrial());
							fPlanTerminated = true;
						}
					}
					
					// If unable to apply the mutations or validation/scoring failed, rollback changes (if allowed) and try again
					else
					{

						// Always leave the genome in the last stable state
						// - The genome should never be "alive" at this point since something has failed
						ASSERT(!Genome::isState(STGS_ALIVE));
						fSuccess = Genome::rollback();
						TFLOW(PLAN,L5,(LLTRACE, "Plan %s removed changes", (fSuccess ? "successfully" : "unsuccessfully")));

						// If rollbacks are not possible and or exhausted, terminate the plan
						if (!mutationSelector.getRollbackPossible() || !_rc.evaluate(++cRollbackAttempts))
							fPlanTerminated = true;
					}
					TFLOW(PLAN,L2,(LLTRACE, "Attempt %d completed %s", Genome::getTrialAttempts(), (fTrialCompleted ? "successfully" : "unsuccessfully")));

					// If the number of trials or attempts has been reached, terminate the plan
					if (!_dc.evaluate())
						fPlanTerminated = true;
				}
			}
		}
	}

	// If the genome is not alive, forcibly restore internal state to that which was last stable
	if (!Genome::isState(STGS_ALIVE))
		Genome::validate(true) && Genome::recordStatistics(true);
}

/*
 * Function: load
 *
 */
void
Plan::load(const char* pxmlPlan)
{
	ENTER(PLAN,load);
	ASSERT(VALID(pxmlPlan));
	ASSERT(!_fExecuting);
	
	// Create an XML document
	XMLDocumentSPtr spxd(XMLDocument::createInstance(pxmlPlan));
	xmlXPathContextSPtr spxpc(spxd->createXPathContext());
	xmlXPathObjectSPtr spxpo;
	
	initialize();

	// Find and load core options
	spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_OPTIONS));
	if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
	{
		xmlNodePtr pxnOptions = spxpo->nodesetval->nodeTab[0];

		xmlXPathContextSPtr spxpcOptions(spxd->createXPathContext());
		spxpcOptions->node = pxnOptions;

		xmlXPathObjectSPtr spxpoOption;
		string str;

		if (spxd->getAttribute(pxnOptions, xmlTag(XT_ACCUMULATEMUTATIONS), str))
			_fAccumulateMutations = XMLDocument::isXMLTrue(str);
		if (spxd->getAttribute(pxnOptions, xmlTag(XT_PRESERVEGENES), str))
			_fPreserveGenes = XMLDocument::isXMLTrue(str);
		if (spxd->getAttribute(pxnOptions, xmlTag(XT_ENSUREINFRAME), str))
			_fEnsureInFrame = XMLDocument::isXMLTrue(str);
		if (spxd->getAttribute(pxnOptions, xmlTag(XT_ENSUREWHOLECODONS), str))
			_fEnsureWholeCodons = XMLDocument::isXMLTrue(str);
		if (spxd->getAttribute(pxnOptions, xmlTag(XT_REJECTSILENT), str))
			_fRejectSilent = XMLDocument::isXMLTrue(str);

		// Check for restrictions in this version of Stylus
		// - Genes must be preserved
		// - Insertions, deletions, copies, and transpositions must be on frame boundaries and in whole codon units
		if (!_fPreserveGenes)
			THROWRC((RC(XMLERROR), "Illegal plan - This version of Stylus requires gene preservation"));
		if (!_fEnsureInFrame)
			THROWRC((RC(XMLERROR), "Illegal plan - This version of Stylus requires working on frame boundaries"));
		if (!_fEnsureWholeCodons)
			THROWRC((RC(XMLERROR), "Illegal plan - This version of Stylus requires working in whole codons"));

		// Load any supplied trial conditions
		spxpoOption = spxd->evalXPath(spxpcOptions.get(), xmlXPath(XP_TRIALCONDITIONS));
		if (XMLDocument::isXPathSuccess(spxpoOption.get(), 1))
			_tc.load(spxd.get(), spxpoOption->nodesetval->nodeTab[0]);

		// Load any termination conditions
		spxpoOption = spxd->evalXPath(spxpcOptions.get(), xmlXPath(XP_TERMINATIONDURATION));
		if (XMLDocument::isXPathSuccess(spxpoOption.get(), 1))
		{
			_grfTermination |= PC_TERMINATIONDURATION;
			_dc.load(spxd.get(), spxpoOption->nodesetval->nodeTab[0]);
		}

		_rc.initialize();
		spxpoOption = spxd->evalXPath(spxpcOptions.get(), xmlXPath(XP_TERMINATIONROLLBACK));
		if (XMLDocument::isXPathSuccess(spxpoOption.get(), 1))
		{
			_grfTermination |= PC_TERMINATIONROLLBACK;
			_rc.load(spxd.get(), spxpoOption->nodesetval->nodeTab[0]);
		}

		spxpoOption = spxd->evalXPath(spxpcOptions.get(), xmlXPath(XP_TERMINATIONFITNESS));
		if (XMLDocument::isXPathSuccess(spxpoOption.get(), 1))
		{
			_grfTermination |= PC_TERMINATIONFITNESS;
			_fc.load(spxd.get(), spxpoOption->nodesetval->nodeTab[0]);
		}
	}

	// Find and load the steps
	spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_STEPS));
	if (XMLDocument::isXPathSuccess(spxpo.get()))
		loadFromXML<STEPARRAY>(_vecSteps, spxd.get(), spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

	// Determine the full range of trials the plan supports
	for (size_t iStep=0; iStep < _vecSteps.size(); ++iStep)
		_cTrials += _vecSteps[iStep].getTrials();

    verify();
}


void
Plan::verify()
{
    for (size_t iStep=0; iStep < _vecSteps.size(); ++iStep)
    {
        const MutationTrialCondition * condition = getMutationTrialCondition(iStep);
        if( !condition->generatesSingleMutation() )
        {
            PLANCONDITION condition_types[] = {PC_TRIALCOST, PC_TRIALFITNESS, PC_TRIALSCORE};
            int condition_count = 0;
            for(int idx = 0; idx < 3; idx++)
            {
                TrialCondition * condition = getTrialCondition(condition_types[idx], iStep);
                if(condition->active() )
                    condition_count++;
            }

            if(condition_count != 1)
            {
                THROWRC((RC(XMLERROR), "Illegal plan - When multiple mutations are considered, there must be exactly one selection condition"));
            }
            
        }

        if( condition->isExhaustive() )
        {
            _vecSteps[iStep].checkSupportsExhaustive();
        }
    }
}

/*
 * Function: toXML
 * 
 */
void
Plan::toXML(XMLStream& xs)
{
	ENTER(PLAN,toXML);
	
	xs.openStart(xmlTag(XT_PLAN));
	xs.writeAttribute(xmlTag(XT_XMLNS), XMLDocument::s_szStylusNamespace);
	xs.closeStart();
	
	xs.openStart(xmlTag(XT_OPTIONS));
	xs.writeAttribute(xmlTag(XT_ACCUMULATEMUTATIONS), _fAccumulateMutations);
	xs.writeAttribute(xmlTag(XT_PRESERVEGENES), _fPreserveGenes);
	xs.writeAttribute(xmlTag(XT_ENSUREINFRAME), _fEnsureInFrame);
	xs.writeAttribute(xmlTag(XT_ENSUREWHOLECODONS), _fEnsureWholeCodons);
	xs.writeAttribute(xmlTag(XT_REJECTSILENT), _fRejectSilent);
	xs.closeStart();

	_tc.toXML(xs);

	if (ST_ISANYSET(_grfTermination, PC_TERMINATIONDURATION | PC_TERMINATIONFITNESS | PC_TERMINATIONROLLBACK))
	{
		xs.writeStart(xmlTag(XT_TERMINATIONCONDITIONS));
		if (ST_ISANYSET(_grfTermination, PC_TERMINATIONDURATION))
			_dc.toXML(xs);
		if (ST_ISANYSET(_grfTermination, PC_TERMINATIONFITNESS))
			_fc.toXML(xs);
		if (ST_ISANYSET(_grfTermination, PC_TERMINATIONROLLBACK))
			_rc.toXML(xs);
		xs.writeEnd(xmlTag(XT_TERMINATIONCONDITIONS));
	}
	
	xs.writeEnd(xmlTag(XT_OPTIONS));

	xs.writeStart(xmlTag(XT_STEPS));
	for (size_t iStep=0; iStep < _vecSteps.size(); ++iStep)
		_vecSteps[iStep].toXML(xs);
	xs.writeEnd(xmlTag(XT_STEPS));
	
	xs.writeEnd(xmlTag(XT_PLAN));
}

/*
 * Function: initialize
 * 
 */
void
Plan::initialize()
{
	_fExecuting = false;

	_cTrials = 0;
	
	_fAccumulateMutations = true;
	_fPreserveGenes = true;
	_fEnsureInFrame = true;
	_fEnsureWholeCodons = true;
	_fRejectSilent = false;

	_tc.clear();

	_grfTermination = PC_NONE;
	_dc.clear();
	_rc.clear();
	_fc.clear();

	_vecSteps.clear();
}

bool
Plan::evaluateConditions()
{
    return	    evaluateCondition(PC_TRIALCOST, Genome::getCost() )
            &&	evaluateCondition(PC_TRIALFITNESS, Genome::getFitness() )
            &&	evaluateCondition(PC_TRIALSCORE, Genome::getScore() );
}

UNIT
Plan::evaluatePerformance() 
{
    TrialCondition * primary = getPrimaryTrialCondition();
    if( primary == getTrialCondition(PC_TRIALCOST) )
        return primary->evaluatePerformance(Genome::getCost());
    else if(primary == getTrialCondition(PC_TRIALFITNESS) )
        return primary->evaluatePerformance(Genome::getFitness());
    else if(primary == getTrialCondition(PC_TRIALSCORE) )
        return primary->evaluatePerformance(Genome::getScore());
    else
    {
        ASSERT(false);
        return 0.0;
    }
}

UNIT
Plan::getPerformancePrecision()
{
    return getPrimaryTrialCondition()->getPerformancePrecision();
}


bool Plan::applyMutation(Mutation & m)
{
    return (m.isChange()
                ? Genome::handleChange(m, _fPreserveGenes, _fRejectSilent)
                : (m.isInsert()
                    ? Genome::handleInsert(m, _fPreserveGenes)
                    : (m.isDelete()
                       ? Genome::handleDelete(m, _fPreserveGenes)
                       : (m.isCopy()
                          ? Genome::handleCopy(m, _fPreserveGenes)
                          : Genome::handleTranspose(m, _fPreserveGenes)))));
}

/////////////////////////////////////////////////////////////////////////////
//
// MutationSelector
//
/////////////////////////////////////////////////////////////////////////////


void 
MutationSelector::startMutations(bool fSingleMutation)
{
    if(fSingleMutation)
        Genome::setRollbackType(RT_COMBINED);
    else
        Genome::setRollbackType(RT_CONSIDERATION);
    _fSingleMutation = fSingleMutation;
}
bool
MutationSelector::addMutation(Mutation & mutation)
{
    bool fSuccess = _plan.applyMutation(mutation);
    _fFieldsMissing = _fFieldsMissing || !mutation.allFieldsSupplied();
    _current().mutations.push_back(mutation);
    _current().fValidMutations = _current().fValidMutations && fSuccess;
    return fSuccess;
}

bool
MutationSelector::selectMutation()
{
    Genome::setRollbackType(RT_ATTEMPT);

    bool fSuccess;
    if( !_fSingleMutation )
    {
        // throw away current consideration which should be empty
        ASSERT(_considerations.back().mutations.empty());
        _considerations.pop_back();
        size_t iConsideration = _pickMutation();
        Consideration & consideration = _considerations[iConsideration];
        for(MUTATIONVECTOR::iterator m = consideration.mutations.begin(); m != consideration.mutations.end();
                m++)
        {
            _plan.applyMutation(*m);
        }
        TFLOW(PLAN,L2,(LLTRACE, "Mutation reapplied: %d", consideration.fValidMutations));

        fSuccess = consideration.fValidMutations && Genome::validate();

        Genome::removeConsideration(iConsideration);
    }
    else
    {
        Consideration & consideration = _considerations[0];
        fSuccess = consideration.fValidMutations && consideration.fValidated;
    }
    
    if(fSuccess && !_fAcceptedMutation)
    {
        // if we are reapplying a mutation
        // we need to reevaluate the conditions
        // so that the correct data gets set
        if(!_fSingleMutation)
            _plan.evaluateConditions();
        return false;
    }
    return fSuccess;
}

void
MutationSelector::mutationFinalize()
{
    if( _current().fValidMutations )
        _current().fValidated = Genome::validate();
    else
        _current().fValidated = false;

    if( _current().fValidMutations && _current().fValidated )
    {
        _fAcceptedMutation = _fAcceptedMutation || _plan.evaluateConditions();
        _current().value = _plan.evaluatePerformance();
        if( _considerations.size() == 1 || _current().value > _best)
        {
            _best = _current().value;
        }
    }
    TFLOW(PLAN,L2,(LLTRACE, "Mutation has been added to considerations, performance: %f", static_cast<UNIT>(_current().value)));
    if( _current().fValidated && _current().fValidMutations )
        Genome::recordAttempt(ST_FILELINE, STTR_PLAN, "Performance: %f", static_cast<UNIT>(_current().value));

    // if we are only going to consider a single mutation
    // we can skip the rollback
    if( !_fSingleMutation )
        Genome::rollback();
    _considerations.push_back( Consideration() );

}

size_t
MutationSelector::_pickMutation()
{
    TFLOW(PLAN,L2,(LLTRACE, "Deciding which mutation to apply, best mutation performance: %f", _best));
    size_t acceptable_count = 0;
    UNIT threshold = _best - _plan.getPerformancePrecision();
    for( CONSIDERATIONVECTOR::iterator consideration = _considerations.begin();
            consideration != _considerations.end(); consideration++)
    {
        if( consideration->value >= threshold )
        {
            acceptable_count++;
        }
    }
    TFLOW(PLAN,L2,(LLTRACE, "%d of %d were better then %f", acceptable_count, _considerations.size(), threshold));
    long choice = RGenerator::getUniform(0L, acceptable_count - 1);
    size_t iConsideration = 0;
    for( CONSIDERATIONVECTOR::iterator consideration = _considerations.begin();
            consideration != _considerations.end(); consideration++)
    {
        if( consideration->value >= threshold )
        {
            if(choice == 0)
            {
                return iConsideration;
            }
            else
            {
                choice--;
            }
        }
        iConsideration++;
    }
    ASSERT(false);
}

void
MutationSelector::reset()
{
    _considerations.clear();
    _fFieldsMissing = false;
    _fAcceptedMutation = false;
    _considerations.push_back( Consideration() );
}
