/*******************************************************************************
 * \file	mutation.cpp
 * \brief	Stylus Genome class (mutation methods)
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
// ModificationStack
//
//--------------------------------------------------------------------------------

/*
 * Function: undo
 *
 */
void
ModificationStack::undo()
{
	ENTER(GENOME,undo);
	
	for (MODIFICATIONARRAY::reverse_iterator ritModification = _vecModifications.rbegin();
		ritModification != _vecModifications.rend();
		++ritModification)
	{
		TDATA(GENOME,L3,(LLTRACE, "Removing modification %s", (*ritModification)->toString().c_str()));
		(*ritModification)->undo();
	}
	TFLOW(GENOME,L3,(LLTRACE, "Removed the effect of %ld modifications", _vecModifications.size()));	
}

/*
 * Function: toXML
 *
 */
void
ModificationStack::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	for (size_t iModification=0; iModification < _vecModifications.size(); ++iModification)
		_vecModifications[iModification]->toXML(xs, grfRecordDetail);
}

//--------------------------------------------------------------------------------
//
// ChangeModification
//
//--------------------------------------------------------------------------------

/*
 * Function: ChangeModification
 *
 */
ChangeModification::ChangeModification(size_t iGene, size_t iTarget, const char* pbBasesBefore, const char* pbBasesAfter, bool fSilent) :
	MutationModification(iGene, iTarget, pbBasesBefore), _strBasesAfter(pbBasesAfter)
{
	ASSERT(!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING));
	_fSilent = fSilent;
	Genome::changeBases(_iGene, _iTarget, _strBasesAfter, _fSilent);
}

/*
 * Function: undo
 *
 */
void ChangeModification::undo()
{
	ENTER(GENOME,undo);
	
	ASSERT(Genome::isState(STGS_ROLLBACK) || Genome::isState(STGS_RESTORING));

	if (Genome::isState(STGS_ROLLBACK))
		Genome::undoStatistics(MT_CHANGE, _strBases.length(), _fSilent);

	Genome::changeBases(_iGene, _iTarget, _strBases, _fSilent);
}

/*
 * Function: toString
 *
 */
string
ChangeModification::toString() const
{
	ENTER(MUTATION,toString);

	ostringstream ostr;

	ostr << "Change mutation at "
	 	<< (_iTarget+1)
		<< " from "
		<< _strBases
		<< " to "
		<< _strBasesAfter;

	return ostr.str();
}

/*
 * Function: ChangeModification
 *
 */
void
ChangeModification::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GENOME,toXML);
	
	xs.openStart(xmlTag(XT_CHANGED));
	xs.writeAttribute(xmlTag(XT_TARGETINDEX), (_iTarget+1));
	xs.writeAttribute(xmlTag(XT_COUNTBASES), _strBases.length());
	xs.writeAttribute(xmlTag(XT_BASES), _strBases);
	xs.writeAttribute(xmlTag(XT_BASESAFTER), _strBasesAfter);
	xs.closeStart(false);
}

//--------------------------------------------------------------------------------
//
// CopyModification
//
//--------------------------------------------------------------------------------

/*
 * Function: CopyModification
 *
 */
CopyModification::CopyModification(size_t iGene, size_t iSource, size_t iTarget, const char* pbBases) :
	MutationModification(iGene, iTarget, pbBases), _iSource(iSource)
{
	ASSERT(!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING));
	Genome::insertBases(_iGene, _iTarget, _strBases);
}

/*
 * Function: undo
 *
 */
void CopyModification::undo()
{
	ENTER(GENOME,undo);
	
	ASSERT(Genome::isState(STGS_ROLLBACK) || Genome::isState(STGS_RESTORING));

	if (Genome::isState(STGS_ROLLBACK))
		Genome::undoStatistics(MT_COPY, _strBases.length(), false);

	Genome::deleteBases(_iGene, _iTarget, _strBases.length());
}

/*
 * Function: toString
 *
 */
string
CopyModification::toString() const
{
	ENTER(MUTATION,toString);

	ostringstream ostr;

	ostr << "Copy mutation from "
		<< (_iSource+1)
		<< " to "
		<< (_iTarget+1)
		<< " of "
		<< _strBases.length()
		<< " bases ("
		<< _strBases
		<< ")";

	return ostr.str();
}

/*
 * Function: CopyModification
 *
 */
void
CopyModification::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GENOME,toXML);
	
	xs.openStart(xmlTag(XT_COPIED));
	xs.writeAttribute(xmlTag(XT_SOURCEINDEX), (_iSource+1));
	xs.writeAttribute(xmlTag(XT_TARGETINDEX), (_iTarget+1));
	xs.writeAttribute(xmlTag(XT_COUNTBASES), _strBases.length());
	xs.writeAttribute(xmlTag(XT_BASES), _strBases);
	xs.closeStart(false);
}

//--------------------------------------------------------------------------------
//
// DeleteModification
//
//--------------------------------------------------------------------------------

/*
 * Function: DeleteModification
 *
 */
DeleteModification::DeleteModification(size_t iGene, size_t iTarget, const char* pbBases) :
	MutationModification(iGene, iTarget, pbBases)
{
	ASSERT(!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING));
	Genome::deleteBases(_iGene, _iTarget, _strBases.length());
}

/*
 * Function: undoChange
 *
 */
void DeleteModification::undo()
{
	ENTER(GENOME,undo);
	
	ASSERT(Genome::isState(STGS_ROLLBACK) || Genome::isState(STGS_RESTORING));

	if (Genome::isState(STGS_ROLLBACK))
		Genome::undoStatistics(MT_DELETE, _strBases.length(), false);

	Genome::insertBases(_iGene, _iTarget, _strBases);
}

/*
 * Function: toString
 *
 */
string
DeleteModification::toString() const
{
	ENTER(MUTATION,toString);

	ostringstream ostr;

	ostr << "Delete mutation from "
		<< (_iTarget+1)
		<< " of "
		<< _strBases.length()
		<< " bases ("
		<< _strBases
		<< ")";

	return ostr.str();
}

/*
 * Function: DeleteModification
 *
 */
void
DeleteModification::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GENOME,toXML);
	
	xs.openStart(xmlTag(XT_DELETED));
	xs.writeAttribute(xmlTag(XT_TARGETINDEX), (_iTarget+1));
	xs.writeAttribute(xmlTag(XT_COUNTBASES), _strBases.length());
	xs.writeAttribute(xmlTag(XT_BASES), _strBases);
	xs.closeStart(false);
}

//--------------------------------------------------------------------------------
//
// InsertModification
//
//--------------------------------------------------------------------------------

/*
 * Function: InsertModification
 *
 */
InsertModification::InsertModification(size_t iGene, size_t iTarget, const char* pbBases) :
	MutationModification(iGene, iTarget, pbBases)
{
	ASSERT(!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING));
	Genome::insertBases(_iGene, _iTarget, _strBases);
}

/*
 * Function: undoChange
 *
 */
void InsertModification::undo()
{
	ENTER(GENOME,undo);
	
	ASSERT(Genome::isState(STGS_ROLLBACK) || Genome::isState(STGS_RESTORING));

	if (Genome::isState(STGS_ROLLBACK))
		Genome::undoStatistics(MT_INSERT, _strBases.length(), false);

	Genome::deleteBases(_iGene, _iTarget, _strBases.length());
}

/*
 * Function: toString
 *
 */
string
InsertModification::toString() const
{
	ENTER(MUTATION,toString);

	ostringstream ostr;

	ostr << "Insert mutation to "
		<< (_iTarget+1)
		<< " of "
		<< _strBases.length()
		<< " bases ("
		<< _strBases
		<< ")";

	return ostr.str();
}

/*
 * Function: InsertModification
 *
 */
void
InsertModification::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GENOME,toXML);
	
	xs.openStart(xmlTag(XT_INSERTED));
	xs.writeAttribute(xmlTag(XT_TARGETINDEX), (_iTarget+1));
	xs.writeAttribute(xmlTag(XT_COUNTBASES), _strBases.length());
	xs.writeAttribute(xmlTag(XT_BASES), _strBases);
	xs.closeStart(false);
}

//--------------------------------------------------------------------------------
//
// TransposeModification
//
//--------------------------------------------------------------------------------

/*
 * Function: TransposeModification
 *
 */
TransposeModification::TransposeModification(size_t iGeneSource, size_t iGeneTarget, size_t iSource, size_t iTarget, const char* pbBases) :
	MutationModification(iGeneTarget, iTarget, pbBases), _iGeneSource(iGeneSource), _iSource(iSource)
{
	ASSERT(!Genome::isState(STGS_ROLLBACK) && !Genome::isState(STGS_RESTORING));
	ASSERT(_iSource >= _iTarget || (_iSource+_strBases.length() <= _iTarget));

	if (_iSource < _iTarget)
	{
		_iTarget -= _strBases.length();
		ASSERT(_iSource <= _iTarget);
	}

	Genome::deleteBases(_iGeneSource, _iSource, _strBases.length());
	ASSERT(_iGene == Genome::indexToGene(_iTarget));

	Genome::insertBases(_iGene, _iTarget, _strBases);
}

/*
 * Function: undoChange
 *
 */
void TransposeModification::undo()
{
	ENTER(GENOME,undoChange);
	
	ASSERT(Genome::isState(STGS_ROLLBACK) || Genome::isState(STGS_RESTORING));

	if (Genome::isState(STGS_ROLLBACK))
		Genome::undoStatistics(MT_TRANSPOSE, _strBases.length(), false);
	
	Genome::deleteBases(_iGene, _iTarget, _strBases.length());
	Genome::insertBases(_iGeneSource, _iSource, _strBases);
}

/*
 * Function: toString
 *
 */
string
TransposeModification::toString() const
{
	ENTER(MUTATION,toString);

	ostringstream ostr;

	ostr << "Transpose mutation from "
		<< (_iSource+1)
		<< " to "
		<< (_iTarget+1)
		<< " of "
		<< _strBases.length()
		<< " bases ("
		<< _strBases
		<< ")";

	return ostr.str();
}

/*
 * Function: TransposeModification
 *
 */
void
TransposeModification::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GENOME,toXML);

	xs.openStart(xmlTag(XT_TRANSPOSED));
	xs.writeAttribute(xmlTag(XT_SOURCEINDEX), (_iSource+1));
	xs.writeAttribute(xmlTag(XT_TARGETINDEX), (_iTarget+1));
	xs.writeAttribute(xmlTag(XT_COUNTBASES), _strBases.length());
	xs.writeAttribute(xmlTag(XT_BASES), _strBases);
	xs.closeStart(false);
}


//--------------------------------------------------------------------------------
//
// Genome
//
//--------------------------------------------------------------------------------

void
Genome::recordStatistics(ST_ATTEMPTS & attempts, size_t cbBases)
{
    if( _rollbackType & RT_ATTEMPT )
        attempts._cAttempted += 1;
    if( _rollbackType & RT_CONSIDERATION )
        attempts._cConsidered += 1;

    attempts._cAccepted += 1;
    attempts._cbBases += cbBases;
}

void
Genome::recordStatistics(ST_STATISTICS & stats, MUTATIONTYPE mt, size_t cbBases, bool fSilent)
{
    if( _rollbackType & RT_ATTEMPT )
        stats._cAttempted += 1;
    if( _rollbackType & RT_CONSIDERATION )
        stats._cConsidered += 1;

	stats._cAccepted += 1;

	switch (mt)
	{
	case MT_CHANGE:
		if (fSilent)
		{
			stats._cSilent += 1;
		}

		stats._cbBasesChanged += cbBases;
        recordStatistics(stats._atChanged, cbBases);
		break;
		
	case MT_COPY:
		stats._cbBasesInserted += cbBases;
        recordStatistics(stats._atCopied, cbBases);
		break;
	
	case MT_DELETE:
		stats._cbBasesDeleted += cbBases;
        recordStatistics(stats._atDeleted, cbBases);
		break;
	
	case MT_INSERT:
        stats._cbBasesInserted += cbBases;
        recordStatistics(stats._atInserted, cbBases);
		break;
	
	case MT_TRANSPOSE:
		stats._cbBasesDeleted += cbBases;
		stats._cbBasesInserted += cbBases;
        recordStatistics(stats._atTransposed, cbBases);
		break;

	case MT_ILLEGAL:
	case MT_MAX:
		break;
	}

}

void
Genome::recordStatistics(MUTATIONTYPE mt, size_t cbBases, bool fSilent)
{
	ENTER(MUTATION,recordStatistics);
	ASSERT(mt < MT_ILLEGAL);

    recordStatistics(Genome::_statsRecordRate, mt, cbBases, fSilent);
    recordStatistics(Genome::_stats, mt, cbBases, fSilent);
}

void
Genome::undoAttempts(ST_ATTEMPTS & attempts, size_t cbBases)
{
    attempts._cAccepted -= 1;
    attempts._cbBases -= cbBases;
}

void
Genome::undoStatistics(ST_STATISTICS & stats, MUTATIONTYPE mt, size_t cbBases, bool fSilent)
{
    stats._cAccepted -= 1;

    switch (mt)
    {
    case MT_CHANGE:
        if (fSilent)
        {
            stats._cSilent -= 1;
        }

        stats._cbBasesChanged -= cbBases;
        undoAttempts(stats._atChanged, cbBases);
        break;
        
    case MT_COPY:
        stats._cbBasesInserted -= cbBases;
        undoAttempts(stats._atCopied, cbBases);
        break;
    
    case MT_DELETE:
        stats._cbBasesDeleted -= cbBases;
        undoAttempts(stats._atDeleted, cbBases);
        break;
    
    case MT_INSERT:
        stats._cbBasesInserted -= cbBases;
        undoAttempts(stats._atInserted, cbBases);
        break;
    
    case MT_TRANSPOSE:
        stats._cbBasesDeleted -= cbBases;
        stats._cbBasesInserted -= cbBases;
        undoAttempts(stats._atTransposed, cbBases);
        break;
        
    case MT_ILLEGAL:
    case MT_MAX:
        break;
    }
}

void
Genome::undoStatistics(MUTATIONTYPE mt, size_t cbBases, bool fSilent)
{
	ENTER(MUTATION,undoStatistics);
	ASSERT(mt < MT_ILLEGAL);

    undoStatistics(Genome::_statsRecordRate, mt, cbBases, fSilent);
    undoStatistics(Genome::_stats, mt, cbBases, fSilent);
}

/*
 * Function: executePlan
 *
 */
void
Genome::executePlan(const char* pxmlPlan, size_t iTrialFirst, size_t cTrials, ST_PFNSTATUS pfnStatus, size_t cStatusRate)
{
    Unit::beginImpreciseMode();

	ENTER(MUTATION,executePlan);
	THROWIFEXECUTING(executePlan);
	REQUIRENOTDEAD(executePlan);
	
	ASSERT(pxmlPlan);
	
	if (!isState(STGS_ALIVE) && !isState(STGS_INVALID))
		THROWRC((RC(INVALIDSTATE), "Attempt to mutate genome from incorrect state (%s)", stateToString()));
		
	// If the trial to associate with the plan was not specified, make it the current trial plus one
	// - The current trial is already completed, so the plan begins with the next trial
	if (iTrialFirst <= 0)
		iTrialFirst = getTrial() + 1;

	// Load the supplied plan
	_plan.load(pxmlPlan);

	// Save the initial genome and plan
	if (isRecording())
	{
		// Preserve the current seed so UUID generation does not perturb the
		// reproducibility of executing a plan
		std::string oldSeed = RGenerator::getSeed();

		// Set the seed for UUID generation
		RGenerator::setSeed(_strUUIDSeeds);

		// Generate UUIDs for each recording trial
		// (conservatively over-estimate the number of UUIDs required to
		//  simplify the logic - then exact start and end trial numbers do not
		//  need to be determined)
		long uuidCount = (_plan.getActualTrialCount(cTrials, iTrialFirst) + (Genome::recordingRate() - 1)) / Genome::recordingRate();

		// Generate UUIDs for the start and final XML files
		uuidCount += 2;

		// Adjust for any UUIDs that may already have been created due to
		// conservative over-estimation above to avoid indefinite growth of
		// surplus UUIDs
		uuidCount -= _vecUUIDs.size();

		for (long iCount=0; iCount < uuidCount; ++iCount) {
			_vecUUIDs.push_back(RGenerator::getUUIDv4());
		}

		// Save the updated UUID generation seed for future UUID generation
		// during multiple plan execution
		_strUUIDSeeds = RGenerator::getSeed();

		// Restore the plan seed to ensure reproducible plan execution
		RGenerator::setSeed(oldSeed);

		if (Globals::isSupplied())
		{
			ostringstream ostrGlobals;
			ostrGlobals
				<< _strRecordDirectory
				<< Constants::s_strGLOBALS
				<< Constants::s_strXMLEXTENSION;
				
			ofstream ofstr(ostrGlobals.str().c_str(), ios::out | ios::trunc);
			if (!ofstr || !ofstr.is_open())
				THROWRC((RC(ERROR), "Unable to create global constants file %s", ostrGlobals.str().c_str()));

			XMLStream xs(ofstr);
			Globals::toXML(xs);
		}

		record(RT_INITIAL);

		ostringstream ostrPlan;
		ostrPlan
			<< _strRecordDirectory
			<< Constants::s_strPLAN
			<< Constants::s_strXMLEXTENSION;

		ofstream ofstr(ostrPlan.str().c_str(), ios::out | ios::trunc);
		if (!ofstr || !ofstr.is_open())
			THROWRC((RC(ERROR), "Unable to create plan file %s", ostrPlan.str().c_str()));

		XMLStream xs(ofstr);
		_plan.toXML(xs);
	}
	
	// Initialize a history file if recording history
	if (isRecordingHistory())
		recordHistory(RT_INITIAL);

	// Execute the loaded plan
	_plan.execute(iTrialFirst, cTrials, pfnStatus, cStatusRate);
	
	// Save the final genome and close the history file
	if (isRecording())
		record(RT_FINAL);
	if (isRecordingHistory())
		recordHistory(RT_FINAL);

    Unit::endImpreciseMode();
}

/*
 * Function: handleChange
 *
 */
bool
Genome::handleChange(const Mutation& mt, bool fPreserveGenes, bool fRejectSilent)
{
	ENTER(MUTATION,handleChange);
	
	size_t iTarget = mt.targetIndex();
	size_t cbBases = mt.countBases();

	string strBases(_strBases.substr(iTarget, cbBases));
	const char* pbBasesBefore = strBases.c_str();
	const char* pbBasesAfter = mt.bases().c_str();

	ASSERT(mt.isChange());
	ASSERT(cbBases == 1 || cbBases == Codon::s_cchCODON);
	ASSERT(cbBases == 1 || Codon::onCodonBoundary(iTarget));
	ASSERT(mt.bases().length() == cbBases);
	ASSERT(fPreserveGenes);
	
	// Determine the containing gene, if any
	size_t iGene = indexToGene(iTarget);
	
	// Determine if the change is silent
	bool fSilent = Codon::isSilentChange(_strBases, iTarget, mt.bases());

	// Record mutational statistics
	recordStatistics(MT_CHANGE, cbBases, fSilent);
	
	// Reject silent mutations if requested
	// - This only prevents silent, single base changes
	if (fRejectSilent && fSilent)
		goto REJECT1;

	// Preserve existing genes and prevent introduction of new start/stop codons
	if (fPreserveGenes)
	{
		// Ensure that the change does not alter the start/stop codon for an existing gene
		if (iGene < _vecGenes.size())
		{
			const Range& rgGene = _vecGenes[iGene].getRange();
			if (	(rgGene.getStart()+Codon::s_cchCODON) > iTarget
				||	(rgGene.getEnd()-Codon::s_cchCODON) < (iTarget + cbBases - 1))
				goto REJECT2;
		}

		// Check each whole codon within the affected range to ensure no new stop (anywhere) or start (between genes) codons are introduced
		char aryCodon[Codon::s_cchCODON];
		const char* pbCodon = (cbBases == 1 ? aryCodon : pbBasesAfter);
		if (cbBases == 1)
		{
			::memcpy(aryCodon, (_strBases.c_str()+Codon::toCodonBoundary(iTarget)), Codon::s_cchCODON);
			aryCodon[Codon::toCodonOffset(iTarget)] = *pbBasesAfter;
		}
		if (iGene >= _vecGenes.size() && Codon::isStart(pbCodon))
			goto REJECT3;

		if (Codon::isStop(pbCodon))
			goto REJECT4;
	}

	// Make and record the mutation
	recordModification(::new ChangeModification(iGene, iTarget, pbBasesBefore, pbBasesAfter, fSilent));

	TFLOW(MUTATION,L2,(LLTRACE, "Mutating %ld from %s to %s in trial %ld", (iTarget+1), pbBasesBefore, pbBasesAfter, getTrial()));
	return true;

REJECT1:	
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected change at %ld from %s to %s - silent changes disallowed", (iTarget+1), pbBasesBefore, pbBasesAfter);
	recordTermination(NULL, STGT_MUTATION, STGR_CHANGE, "Rejected change at %ld from %s to %s - silent changes disallowed", (iTarget+1), pbBasesBefore, pbBasesAfter);
	goto REJECT;

REJECT2:	
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected change at %ld from %s to %s to preserve genes - gene start or stop codon affected", (iTarget+1), pbBasesBefore, pbBasesAfter);
	recordTermination(NULL, STGT_MUTATION, STGR_CHANGE, "Rejected change at %ld from %s to %s to preserve genes - gene start or stop codon affected", (iTarget+1), pbBasesBefore, pbBasesAfter);
	goto REJECT;

REJECT3:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected change at %ld from %s to %s to preserve genes - illegal start codon created", (iTarget+1), pbBasesBefore, pbBasesAfter);
	recordTermination(NULL, STGT_MUTATION, STGR_CHANGE, "Rejected change at %ld from %s to %s to preserve genes - illegal start codon created", (iTarget+1), pbBasesBefore, pbBasesAfter);
	goto REJECT;

REJECT4:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected change at %ld from %s to %s to preserve genes - illegal stop codon created", (iTarget+1), pbBasesBefore, pbBasesAfter);
	recordTermination(NULL, STGT_MUTATION, STGR_CHANGE, "Rejected change at %ld from %s to %s to preserve genes - illegal stop codon created", (iTarget+1), pbBasesBefore, pbBasesAfter);
	goto REJECT;

REJECT:
	TFLOW(MUTATION,L4,(LLTRACE, "Rejected change at %ld from %s to %s", iTarget, pbBasesBefore, pbBasesAfter));
	undoStatistics(MT_CHANGE, cbBases, fSilent);
	return false;
}

/*
 * Function: handleCopy
 *
 */
bool
Genome::handleCopy(const Mutation& mt, bool fPreserveGenes)
{
	ENTER(MUTATION,handleCopy);
	
	size_t iSource = mt.sourceIndex();
	size_t iTarget = mt.targetIndex();
	size_t cbBases = mt.countBases();

#if defined(ST_TRACE)
	const char* pbBases = _strBases.c_str() + iTarget;
#endif

	ASSERT(mt.isCopy());
	ASSERT(cbBases < (max<size_t>(_strBases.length(),iTarget)-iSource));
	ASSERT(fPreserveGenes);
	ASSERT(Codon::onCodonBoundary(iSource));
	ASSERT(Codon::onCodonBoundary(iTarget));
	ASSERT(Codon::hasWholeCodons(cbBases));

	// Determine the containing gene, if any
	size_t iGene = indexToGene(iTarget);
	
	// Record mutational statistics
	recordStatistics(MT_COPY, cbBases, false);

	// Preserve any existing genes
	if (fPreserveGenes)
	{
		size_t iGeneSource = indexToGene(iSource);

		// Only allow copies of whole codons and from within the same gene
		if (!Codon::hasWholeCodons(cbBases))
			goto REJECT1;

		if (	iGene >= _vecGenes.size()
			||	iGeneSource >= _vecGenes.size()
			||	iGene != iGeneSource
			||	iGeneSource != indexToGene(iSource+cbBases-1))
			goto REJECT2;

		const Range& rgGene = _vecGenes[iGene].getRange();
		if (	(rgGene.getStart()+Codon::s_cchCODON) > iSource
			||	(rgGene.getEnd()-Codon::s_cchCODON) < (iSource + cbBases - 1)
			||	(rgGene.getStart()+Codon::s_cchCODON) > iTarget
			||	(rgGene.getEnd()-Codon::s_cchCODON+1) < iTarget)
			goto REJECT3;
	}

	// Make and record the mutation
	recordModification(::new CopyModification(iGene, iSource, iTarget, _strBases.substr(iSource,cbBases).c_str()));

	TFLOW(MUTATION,L2,(LLTRACE, "Copying %ld bases from %ld to %ld in trial %ld", cbBases, (iSource+1), (iTarget+1), getTrial()));
	TDATA(MUTATION,L4,(LLTRACE, "Copied %s", _strBases.substr(iSource,cbBases).c_str()));
	return true;

REJECT1:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected copy to %ld of %ld bases from %ld to preserve genes - frame-shift caused", (iTarget+1), cbBases, (iSource+1));
	recordTermination(NULL, STGT_MUTATION, STGR_COPY, "Rejected copy to %ld of %ld bases from %ld to preserve genes - frame-shift caused", (iTarget+1), cbBases, (iSource+1));
	goto REJECT;

REJECT2:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected copy to %ld of %ld bases from %ld to preserve genes - not contained within a single gene", (iTarget+1), cbBases, (iSource+1));
	recordTermination(NULL, STGT_MUTATION, STGR_COPY, "Rejected copy to %ld of %ld bases from %ld to preserve genes - not contained within a single gene", (iTarget+1), cbBases, (iSource+1));
	goto REJECT;

REJECT3:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected copy to %ld of %ld bases from %ld to preserve genes - gene start or stop codon affected", (iTarget+1), cbBases, (iSource+1));
	recordTermination(NULL, STGT_MUTATION, STGR_COPY, "Rejected copy to %ld of %ld bases from %ld to preserve genes - gene start or stop codon affected", (iTarget+1), cbBases, (iSource+1));
	goto REJECT;

REJECT:
	TFLOW(MUTATION,L4,(LLTRACE, "Rejected copy of %s", pbBases));
	undoStatistics(MT_COPY, cbBases, false);
	return false;
}

/*
 * Function: handleDelete
 *
 */
bool
Genome::handleDelete(const Mutation& mt, bool fPreserveGenes)
{
	ENTER(MUTATION,handleDelete);
	
	size_t iTarget = mt.targetIndex();
	size_t cbBases = mt.countBases();

	ASSERT(mt.isDelete());
	ASSERT(fPreserveGenes);
	ASSERT(Codon::onCodonBoundary(iTarget));
	ASSERT(Codon::hasWholeCodons(cbBases));
	
	// Determine the affected genes, if any
	size_t iGeneFirst = indexToGene(iTarget);
	size_t iGeneLast = indexToGene(iTarget+cbBases-1);
	
	// Record mutational statistics
	recordStatistics(MT_DELETE, cbBases, false);
	
	// Ensure the deletion remains within the boundaries of the genome
	if ((iTarget+cbBases) > _strBases.length())
		goto REJECT;

	// Preserve any existing genes
	if (fPreserveGenes)
	{
		// If the deletion overlaps multiple genes or causes a frame-shift, reject it
		if (iGeneFirst != iGeneLast)
			goto REJECT1;
		
		if (!Codon::hasWholeCodons(cbBases))
			goto REJECT2;

		// If the deletion removes the start or stop codon of any one gene, reject it
		if (iGeneFirst < _vecGenes.size())
		{
			const Range& rgGene = _vecGenes[iGeneFirst].getRange();
			if (	(rgGene.getStart()+Codon::s_cchCODON) > iTarget
				||	(rgGene.getEnd()-Codon::s_cchCODON) < (iTarget + cbBases - 1))
				goto REJECT3;
		}
	}

	// Make and record the mutation
	recordModification(::new DeleteModification(iGeneFirst, iTarget, _strBases.substr(iTarget,cbBases).c_str()));

	TFLOW(MUTATION,L2,(LLTRACE, "Deleting %ld bases from %ld in trial %ld", cbBases, (iTarget+1), getTrial()));
	TDATA(MUTATION,L4,(LLTRACE, "Deleted %s", _strBases.substr(iTarget,cbBases).c_str()));
	return true;

REJECT1:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected deletion at %ld of %ld bases to preserve genes - multiple genes affected", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_DELETE, "Rejected deletion at %ld of %ld bases to preserve genes - multiple genes affected", (iTarget+1), cbBases);
	goto REJECT;

REJECT2:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected deletion at %ld of %ld bases to preserve genes - frame-shift caused", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_DELETE, "Rejected deletion at %ld of %ld bases to preserve genes - frame-shift caused", (iTarget+1), cbBases);
	goto REJECT;

REJECT3:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected deletion at %ld of %ld bases to preserve genes - gene start or stop codon affected", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_DELETE, "Rejected deletion at %ld of %ld bases to preserve genes - gene start or stop codon affected", (iTarget+1), cbBases);
	goto REJECT;

REJECT:
	TFLOW(MUTATION,L4,(LLTRACE, "Rejected deletion of %s", _strBases.substr(iTarget,cbBases).c_str()));
	undoStatistics(MT_DELETE, cbBases, false);
	return false;
}

/*
 * Function: handleInsert
 *
 */
bool
Genome::handleInsert(const Mutation& mt, bool fPreserveGenes)
{
	ENTER(MUTATION,handleInsert);
	
	size_t iTarget = mt.targetIndex();
	size_t cbBases = mt.countBases();
	const char* pbBases = mt.bases().c_str();

	ASSERT(mt.isInsert());
	ASSERT(VALID(pbBases));
	ASSERT(::strlen(pbBases) == cbBases);
	ASSERT(fPreserveGenes);
	ASSERT(Codon::onCodonBoundary(iTarget));
	ASSERT(Codon::hasWholeCodons(cbBases));
	
	// Determine the containing gene, if any
	size_t iGene = indexToGene(iTarget);
	
	// Record mutational statistics
	recordStatistics(MT_INSERT, cbBases, false);

	// Preserve any existing genes
	if (fPreserveGenes)
	{
		// Only allow insertions of whole codons
		if (!Codon::hasWholeCodons(cbBases))
			goto REJECT1;

		// Check each whole codon within the inserted bases
		for (size_t iBase=0; iBase < cbBases; iBase += Codon::s_cchCODON)
		{
			if (iGene >= _vecGenes.size() && Codon::isStart(pbBases+iBase))
				goto REJECT2;
			if (Codon::isStop(pbBases+iBase))
				goto REJECT3;
		}

		// Ensure the insertion occurs between the start and end codons
		// - Insertions are *at* the target index, so an insertion *may* occur at the gene's stop codon
		const Range& rgGene = _vecGenes[iGene].getRange();
		if (	(rgGene.getStart()+Codon::s_cchCODON) > iTarget
			||	(rgGene.getEnd()-Codon::s_cchCODON+1) < iTarget)
			goto REJECT4;
	}

	// Make and record the mutation
	recordModification(::new InsertModification(iGene, iTarget, pbBases));

	TFLOW(MUTATION,L2,(LLTRACE, "Inserting %ld bases at %ld in trial %ld", cbBases, (iTarget+1), getTrial()));
	TDATA(MUTATION,L4,(LLTRACE, "Inserted %s", string(pbBases,cbBases).c_str()));
	return true;

REJECT1:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected insertion at %ld of %ld bases to preserve genes - frame-shift caused", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_INSERT, "Rejected insertion at %ld of %ld bases to preserve genes - frame-shift caused", (iTarget+1), cbBases);
	goto REJECT;

REJECT2:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected insertion at %ld of %ld bases to preserve genes - illegal start codon added", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_INSERT, "Rejected insertion at %ld of %ld bases to preserve genes - illegal start codon added", (iTarget+1), cbBases);
	goto REJECT;

REJECT3:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected insertion at %ld of %ld bases to preserve genes - illegal stop codon added", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_INSERT, "Rejected insertion at %ld of %ld bases to preserve genes - illegal stop codon added", (iTarget+1), cbBases);
	goto REJECT;

REJECT4:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected insertion at %ld of %ld bases to preserve genes - gene start or stop codon affected", (iTarget+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_INSERT, "Rejected insertion at %ld of %ld bases to preserve genes - gene start or stop codon affected", (iTarget+1), cbBases);
	goto REJECT;

REJECT:
	TFLOW(MUTATION,L4,(LLTRACE, "Rejected insertion of %s", pbBases));
	undoStatistics(MT_INSERT, cbBases, false);
	return false;
}

/*
 * Function: handleTranspose
 *
 */
bool
Genome::handleTranspose(const Mutation& mt, bool fPreserveGenes)
{
	ENTER(MUTATION,handleTranspose);

	size_t iSource = mt.sourceIndex();
	size_t iTarget = mt.targetIndex();
	size_t cbBases = mt.countBases();

	ASSERT(mt.isTranspose());
	ASSERT((iSource+cbBases) < _strBases.length());
	ASSERT(fPreserveGenes);
	ASSERT(Codon::onCodonBoundary(iSource));
	ASSERT(Codon::onCodonBoundary(iTarget));
	ASSERT(Codon::hasWholeCodons(cbBases));

	// First, determine and copy the bases from the source location
	size_t iGeneTarget = indexToGene(iTarget);
	size_t iGeneSource = indexToGene(iSource);
	string strBasesSource(_strBases.c_str(), iSource, cbBases);
	
	// Record mutational statistics
	recordStatistics(MT_TRANSPOSE, cbBases, false);
	
	// Preserve any existing genes
	if (fPreserveGenes)
	{
		// Only allow transposition of whole codons that do not cross gene boundaries
		if (!Codon::hasWholeCodons(cbBases))
			goto REJECT1;
			
		if (	iGeneTarget >= _vecGenes.size()
			||	iGeneSource >= _vecGenes.size()
			||	iGeneTarget != iGeneSource
			||	iGeneSource != indexToGene(iSource+cbBases-1))
			goto REJECT2;

		const Range& rgGene = _vecGenes[iGeneSource].getRange();
		if (	(rgGene.getStart()+Codon::s_cchCODON) > iSource
			||	(rgGene.getEnd()-Codon::s_cchCODON) < (iSource + cbBases - 1)
			||	(rgGene.getStart()+Codon::s_cchCODON) > iTarget
			||	(rgGene.getEnd()-Codon::s_cchCODON+1) < iTarget)
			goto REJECT3;
	}

	// Make and record the mutation
	recordModification(::new TransposeModification(iGeneSource, iGeneTarget, iSource, iTarget, strBasesSource.c_str()));

	TFLOW(MUTATION,L2,(LLTRACE, "Transposing %ld bases from %ld to %ld in trial %ld", cbBases, (iSource+1), (iTarget+1), getTrial()));
	TDATA(MUTATION,L4,(LLTRACE, "Transposed %s", strBasesSource.c_str()));
	return true;
	
REJECT1:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected transposition from %ld to %ld of %ld bases to preserve genes - frame-shift caused", (iTarget+1), (iSource+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_COPY, "Rejected transposition from %ld to %ld of %ld bases to preserve genes - frame-shift caused", (iTarget+1), (iSource+1), cbBases);
	goto REJECT;

REJECT2:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected transposition from %ld to %ld of %ld bases to preserve genes - not contained within a single gene", (iTarget+1), (iSource+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_COPY, "Rejected transposition from %ld to %ld of %ld bases to preserve genes - not contained within a single gene", (iTarget+1), (iSource+1), cbBases);
	goto REJECT;

REJECT3:
	recordAttempt(ST_FILELINE, STTR_MUTATION, "Rejected transposition from %ld to %ld of %ld bases to preserve genes - gene start or stop codon affected", (iTarget+1), (iSource+1), cbBases);
	recordTermination(NULL, STGT_MUTATION, STGR_COPY, "Rejected transposition from %ld to %ld of %ld bases to preserve genes - gene start or stop codon affected", (iTarget+1), (iSource+1), cbBases);
	goto REJECT;

REJECT:
	TFLOW(MUTATION,L4,(LLTRACE, "Rejected transposition of %s", strBasesSource.c_str()));
	undoStatistics(MT_TRANSPOSE, cbBases, false);
	return false;
}

/*
 * Function: changeBase
 *
 */
void
Genome::changeBases(size_t iGene, size_t iTarget, const std::string& strBases, bool fSilent)
{
	ENTER(MUTATION,changeBases);
	
	TFLOW(MUTATION,L3,(LLTRACE, "Making a %s change in genome at %lu", (fSilent ? "silent" : "non-silent"), iTarget));

	// Change the bases
	_strBases.replace(iTarget, strBases.length(), strBases);

	// Mark the affected gene invalid
	if (iGene < _vecGenes.size())
	{
		_vecGenes[iGene].markInvalid(Gene::GC_CHANGE, Range(iTarget,iTarget+strBases.length()-1), fSilent);
		_grfGenesInvalid.set(iGene);
	}
}

/*
 * Function: deleteBases
 *
 */
void
Genome::deleteBases(size_t iGene, size_t iTarget, size_t cbBases)
{
	ENTER(MUTATION,deleteBases);
	
	TFLOW(MUTATION,L3,(LLTRACE, "Deleting %lu bases from genome at %lu", cbBases, (iTarget+1)));

	// Remove the bases
	_strBases.erase(iTarget,cbBases);
	
	// Update the statistics
	_statsRecordRate._cbBases = _strBases.length();
	_stats._cbBases = _statsRecordRate._cbBases;

	// Shrink the affected gene and mark it invalid, if any
	if (iGene < _vecGenes.size())
	{
		_vecGenes[iGene].markInvalid(Gene::GC_DELETE, Range(iTarget,iTarget+cbBases-1), false);
		_grfGenesInvalid.set(iGene);
	}

	// Re-align all subsequent genes
	for (++iGene; iGene < _vecGenes.size(); ++iGene)
		_vecGenes[iGene].moveRange(-cbBases);
}

/*
 * Function: insertBases
 *
 */
void
Genome::insertBases(size_t iGene, size_t iTarget, const string& strBases)
{
	ENTER(MUTATION,insertBases);
	
	TFLOW(MUTATION,L3,(LLTRACE, "Inserting %lu bases to the genome at %lu", strBases.length(), (iTarget+1)));
	
	size_t cbBases = strBases.length();
	
	// Insert the new bases
	_strBases.insert(iTarget, strBases);
	
	// Update the statistics
	_statsRecordRate._cbBases = _strBases.length();
	_stats._cbBases = _statsRecordRate._cbBases;

	// Extend the affected gene, if any, to encompass the inserted bases and mark it invalid
	if (iGene < _vecGenes.size())
	{
		_vecGenes[iGene].markInvalid(Gene::GC_INSERT, Range(iTarget,iTarget+cbBases-1), false);
		_grfGenesInvalid.set(iGene);
	}

	// Re-align all subsequent genes
	for (++iGene; iGene < _vecGenes.size(); ++iGene)
		_vecGenes[iGene].moveRange(cbBases);
}
