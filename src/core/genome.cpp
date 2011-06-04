/*******************************************************************************
 * \file	genome.cpp
 * \brief	Stylus Genome class
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
// CodonTable
//
//--------------------------------------------------------------------------------

const char* CodonTable::s_aryCodonToName[Constants::s_nmaxCODONS] =
{
	"TTT", // 00
	"TTC", // 01
	"TTA", // 02
	"TTG", // 03

	"TCT", // 04
	"TCC", // 05
	"TCA", // 06
	"TCG", // 07

	"TAT", // 08
	"TAC", // 09
	"TAA", // 10
	"TAG", // 11

	"TGT", // 12
	"TGC", // 13
	"TGA", // 14
	"TGG", // 15

	"CTT", // 16
	"CTC", // 17
	"CTA", // 18
	"CTG", // 19

	"CCT", // 20
	"CCC", // 21
	"CCA", // 22
	"CCG", // 23

	"CAT", // 24
	"CAC", // 25
	"CAA", // 26
	"CAG", // 27

	"CGT", // 28
	"CGC", // 29
	"CGA", // 30
	"CGG", // 31

	"ATT", // 32
	"ATC", // 33
	"ATA", // 34
	"ATG", // 35

	"ACT", // 36
	"ACC", // 37
	"ACA", // 38
	"ACG", // 39

	"AAT", // 40
	"AAC", // 41
	"AAA", // 42
	"AAG", // 43

	"AGT", // 44
	"AGC", // 45
	"AGA", // 46
	"AGG", // 47

	"GTT", // 48
	"GTC", // 49
	"GTA", // 50
	"GTG", // 51

	"GCT", // 52
	"GCC", // 53
	"GCA", // 54
	"GCG", // 55

	"GAT", // 56
	"GAC", // 57
	"GAA", // 58
	"GAG", // 59

	"GGT", // 60
	"GGC", // 61
	"GGA", // 62
	"GGG"  // 63
};

const std::string CodonTable::s_strUUID("9CF8BD5B-860C-4D7D-AED6-270C6EC14790");

const ACIDTYPE CodonTable::s_mapCodonToType[Constants::s_nmaxCODONS] =
{
	ACID_Nol, // 00 - TTT
	ACID_Nol, // 01 - TTC
	ACID_Nom, // 02 - TTA
	ACID_Nom, // 03 - TTG

	ACID_Sem, // 04 - TCT
	ACID_Sem, // 05 - TCC
	ACID_Sem, // 06 - TCA
	ACID_Sem, // 07 - TCG

	ACID_Sol, // 08 - TAT
	ACID_Sol, // 09 - TAC
	ACID_STP, // 10 - TAA
	ACID_STP, // 11 - TAG

	ACID_Nwm, // 12 - TGT
	ACID_Nwm, // 13 - TGC
	ACID_STP, // 14 - TGA
	ACID_Nwm, // 15 - TGG

	ACID_Nos, // 16 - CTT
	ACID_Nos, // 17 - CTC
	ACID_Nos, // 18 - CTA
	ACID_Nos, // 19 - CTG

	ACID_Ses, // 20 - CCT
	ACID_Ses, // 21 - CCC
	ACID_Ses, // 22 - CCA
	ACID_Ses, // 23 - CCG

	ACID_Som, // 24 - CAT
	ACID_Som, // 25 - CAC
	ACID_Sos, // 26 - CAA
	ACID_Sos, // 27 - CAG

	ACID_Nws, // 28 - CGT
	ACID_Nws, // 29 - CGC
	ACID_Nws, // 30 - CGA
	ACID_Nws, // 31 - CGG

	ACID_Nem, // 32 - ATT
	ACID_Nem, // 33 - ATC
	ACID_Nem, // 34 - ATA
	ACID_Nem, // 35 - ATG

	ACID_Eal, // 36 - ACT
	ACID_Eal, // 37 - ACC
	ACID_Eam, // 38 - ACA
	ACID_Eam, // 39 - ACG

	ACID_Swm, // 40 - AAT
	ACID_Swm, // 41 - AAC
	ACID_Swm, // 42 - AAA
	ACID_Swm, // 43 - AAG

	ACID_Wel, // 44 - AGT
	ACID_Wel, // 45 - AGC
	ACID_Wem, // 46 - AGA
	ACID_Wem, // 47 - AGG

	ACID_Nes, // 48 - GTT
	ACID_Nes, // 49 - GTC
	ACID_Nes, // 50 - GTA
	ACID_Nes, // 51 - GTG

	ACID_Eas, // 52 - GCT
	ACID_Eas, // 53 - GCC
	ACID_Eas, // 54 - GCA
	ACID_Eas, // 55 - GCG

	ACID_Sws, // 56 - GAT
	ACID_Sws, // 57 - GAC
	ACID_Sws, // 58 - GAA
	ACID_Sws, // 59 - GAG

	ACID_Wes, // 60 - GGT
	ACID_Wes, // 61 - GGC
	ACID_Wes, // 62 - GGA
	ACID_Wes  // 63 - GGG
};

/*
 * Function: load
 *
 * Note:
 * - The codon table contains the default values upon entering this routine
 */
void
CodonTable::load(XMLDocument* pxd, xmlNodePtr pxnCodonTable)
{
	ENTER(GENOME,load);
	ASSERT(VALID(pxd));
	ASSERT(VALID(pxnCodonTable));
	
	// First, load the codon table UUID and creation attributes
	pxd->getAttribute(pxnCodonTable, xmlTag(XT_UUID), _strUUID);
	pxd->getAttribute(pxnCodonTable, xmlTag(XT_AUTHOR), _strAuthor);
	pxd->getAttribute(pxnCodonTable, xmlTag(XT_CREATIONDATE), _strCreationDate);
	pxd->getAttribute(pxnCodonTable, xmlTag(XT_CREATIONTOOL), _strCreationTool);
	TFLOW(GENOME,L2,(LLTRACE, "Loading codon table %s", _strUUID.c_str()));

	// Next, clear the codon-to-type map and load the supplied entry elements
	for (size_t iCodon=0; iCodon < Constants::s_nmaxCODONS; ++iCodon)
		_mapCodonToType[iCodon] = ACID_MAX;

	xmlXPathContextSPtr spxpc(pxd->createXPathContext());
	spxpc->node = pxnCodonTable;

	xmlXPathObjectSPtr spxpoEntry(pxd->evalXPath(spxpc.get(), xmlXPath(XP_ENTRY)));
	if (XMLDocument::isXPathSuccess(spxpoEntry.get()))
	{
		TFLOW(GENOME,L3,(LLTRACE, "Codon table contains %d entries", spxpoEntry->nodesetval->nodeNr));
		for (int iEntry=0; iEntry < spxpoEntry->nodesetval->nodeNr; ++iEntry)
		{
			xmlNodePtr pxnEntry = spxpoEntry->nodesetval->nodeTab[iEntry];
			std::string strCodon;
			std::string strVector;
			size_t iCodon;

			pxd->getAttribute(pxnEntry, xmlTag(XT_CODON), strCodon);
			pxd->getAttribute(pxnEntry, xmlTag(XT_VECTOR), strVector);
			TFLOW(GENOME,L3,(LLTRACE, "Mapping codon %s to vector %s", strCodon.c_str(), strVector.c_str()));
			
			iCodon = codonToIndex(strCodon.c_str());
			if (_mapCodonToType[iCodon] != ACID_MAX)
				THROWRC((RC(XMLERROR), "Codon table contains multiple entries for codon %s", strCodon.c_str()));
			
			_mapCodonToType[iCodon] = Acid::vectorToType(strVector.c_str());
		}
	}

	// Finally, ensure that each acid occurs at least once in the codon table
	bool grfAcid[ACID_MAX];
	::memset(grfAcid, 0, ARRAY_SIZE(grfAcid));
	
	for (size_t iCodon=0; iCodon < Constants::s_nmaxCODONS; ++iCodon)
		grfAcid[_mapCodonToType[iCodon]] = true;
	
	for (size_t iAcid=0; iAcid < ACID_MAX; ++iAcid)
	{
		if (!grfAcid[iAcid])
			THROWRC((RC(XMLERROR), "Codon table did not include an entry for vector %s", Acid::typeToAcid((ACIDTYPE)iAcid).getName()));
	}

	LOGINFO((LLINFO, "Loaded codon table %s", _strUUID.c_str()));
}

/*
 * Function: toXML
 * 
 */
void
CodonTable::toXML(XMLStream& xs, STFLAGS grfRecordDetail) const
{
	ENTER(GLOBAL,codonTableToXML);
	
	if (_strUUID == s_strUUID)
		return;
	
	xs.openStart(xmlTag(XT_CODONTABLE));
	xs.writeAttribute(xmlTag(XT_UUID), _strUUID);
	if (!EMPTYSTR(_strAuthor))
		xs.writeAttribute(xmlTag(XT_AUTHOR), _strAuthor);
	if (!EMPTYSTR(_strCreationDate))
		xs.writeAttribute(xmlTag(XT_CREATIONDATE), _strCreationDate);
	if (!EMPTYSTR(_strCreationTool))
		xs.writeAttribute(xmlTag(XT_CREATIONTOOL), _strCreationTool);
	xs.closeStart();
	
	for (size_t iCodon=0; iCodon < Constants::s_nmaxCODONS; ++iCodon)
	{
		if (_mapCodonToType[iCodon] != s_mapCodonToType[iCodon])
		{
			xs.openStart(xmlTag(XT_ENTRY));
			xs.writeAttribute(xmlTag(XT_CODON), s_aryCodonToName[iCodon]);
			xs.writeAttribute(xmlTag(XT_VECTOR), Acid::typeToAcid(_mapCodonToType[iCodon]).getName());
			xs.closeStart(false);
		}
	}

	xs.writeEnd(xmlTag(XT_CODONTABLE));
}

//--------------------------------------------------------------------------------
//
// Genome
//
//--------------------------------------------------------------------------------

const char* Genome::s_aryGENOMESTATES[STGS_MAX] =
{
	"ALIVE",
	"COMPILED",
	"COMPILING",
	"DEAD",
	"INVALID",
	"LOADING",
	"MUTATING",
	"RECORDING",
	"ROLLBACK",
	"RESTORING",
	"SCORED",
	"SCORING",
	"SPAWNING",
	"VALIDATED",
	"VALIDATING"
};

const size_t Genome::s_maxGENES;

std::string Genome::_strAuthor;
CodonTable Genome::_ct;
std::string Genome::_strBases;
utime Genome::_tLoaded = 0L;
std::string Genome::_strUUID;
std::string Genome::_strStrain;
std::string Genome::_strAncestors;

std::string Genome::_strUUIDSeeds;
STRINGARRAY Genome::_vecUUIDs;

bool Genome::_fReady;
ROLLBACKTYPE Genome::_rollbackType;

Plan Genome::_plan;

ST_STATISTICS Genome::_stats;
ST_STATISTICS Genome::_statsRecordRate;

size_t Genome::_cRecordRate = 0;
STFLAGS Genome::_grfRecordDetail = STRD_NONE;
std::string Genome::_strRecordDirectory;
size_t Genome::_fRecordHistory;

bool Genome::_fGenesAssigned;
GENEARRAY Genome::_vecGenes;
std::bitset<Genome::s_maxGENES> Genome::_grfGenesInvalid;

ModificationStack Genome::_msModifications;
MODIFICATIONSTACKARRAY Genome::_vecAttempts;
MODIFICATIONSTACKARRAY Genome::_vecConsiderations;

ST_GENOMESTATE Genome::_gsCurrent;

ST_GENOMETERMINATION Genome::_gaTermination = STGT_NONE;
ST_GENOMEREASON Genome::_grTermination = STGR_NONE;
std::string Genome::_strTermination;

#ifdef ST_DEBUG
Unit Genome::_nFitnessPassing;
bool Genome::_fTestingRollback = false;
#endif

/*
 * Function: initialize
 *
 * Initializes all internal state, including values through public methods.
 */
void
Genome::initialize()
{
	ENTER(GENOME,initialize);
	ASSERT(!_plan.isExecuting());

	_ct.reset();
	_strBases.clear();
	_tLoaded = 0L;
	_strUUID.clear();
	_strStrain.clear();
	_strAncestors.clear();

	_fReady = false;

	_plan.clear();

	clearStatistics(_stats, 0);
	clearStatistics(_statsRecordRate, 0);

	_fGenesAssigned = false;
	_vecGenes.clear();
	_grfGenesInvalid.set();

	purgeModifications();

	_gsCurrent = STGS_DEAD;
	
	_gaTermination = STGT_NONE;
	_grTermination = STGR_NONE;
	_strTermination.clear();

    _rollbackType = RT_ATTEMPT;

	Globals::enableTracing(getTrial(), getTrialAttempts());
	
#ifdef ST_DEBUG
	_nFitnessPassing = 0;
	_fTestingRollback = false;
#endif
}

/*
 * Function: clearStatistics
 *
 */
void
Genome::clearStatistics(ST_STATISTICS& stats, size_t iTrial)
{
	ENTER(GENOME,clearStatistics);
	
	::memset(&stats, 0, sizeof(stats));
	
	stats._nScore = 0.0;
	stats._nUnits = 0.0;
	stats._nCost = 0.0;
	stats._nFitness = 0.0;

	stats._iTrialInitial =
	stats._iTrialCurrent =
	stats._tsMax._iTrial =
	stats._tsMin._iTrial =
	stats._tuMax._iTrial =
	stats._tuMin._iTrial =
	stats._tcMax._iTrial =
	stats._tcMin._iTrial =
	stats._tfMax._iTrial =
	stats._tfMin._iTrial =
	stats._trMax._iTrial =
	stats._trMin._iTrial =
	stats._tzMax._iTrial =
	stats._tzMin._iTrial = iTrial;

	stats._tsMax._nValue = 0.0;
	stats._tsMin._nValue = 1.0;
	
	stats._tuMax._nValue = 0.0;
	stats._tuMin._nValue = numeric_limits<size_t>::max();
	
	stats._tcMax._nValue = 0.0;
	stats._tcMin._nValue = numeric_limits<size_t>::max();
	
	stats._tfMax._nValue = 0.0;
	stats._tfMin._nValue = 1.0;
	
	stats._trMin._cRollbacks = numeric_limits<size_t>::max();
	
	stats._cbBases = _strBases.length();
	stats._tzMax._cbBases = stats._cbBases;
	stats._tzMin._cbBases = stats._tzMax._cbBases;
}

/*
 * Function: terminate
 *
 */
void
Genome::terminate()
{
	ENTER(GENOME,terminate);
	ASSERT(!_plan.isExecuting());
}

/*
 * Function: setGenome
 *
 * NOTE:
 * - This routine performs no inline error checking of the XML document. Instead
 *   it relies on XML parsing to catch any XML or schema validation errors.
 */
void
Genome::setGenome(const char* pxmlGenome, const char* pszAuthor)
{
	ENTER(GENOME,setGenome);
	THROWIFEXECUTING(setGenome);

	ASSERT(VALID(pxmlGenome));

	initialize();

	// Transition to the loading state
	if (!enterState(STGS_LOADING))
		THROWRC((RC(INVALIDSTATE),
				 "Attempt to load a new genome during an invalid state (%s)", stateToString()));
				
	// Load the definition, failure results in a DEAD genome
	{
		StateGuard sg(STGS_INVALID, STGS_DEAD);		// Ensure state exits as INVALID or DEAD

		timeNow(&_tLoaded);

		XMLDocumentSPtr spxd(XMLDocument::createInstance(pxmlGenome));
		xmlXPathContextSPtr spxpc(spxd->createXPathContext());
		xmlXPathObjectSPtr spxpo;
		xmlNodePtr pxn;

		// Read Genome element and properties
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_GENOME));
		if (!XMLDocument::isXPathSuccess(spxpo.get(), 1))
			THROWRC((RC(XMLERROR), "Unexpected number of genome elements"));

		pxn = spxpo->nodesetval->nodeTab[0];
		spxd->getAttribute(pxn, xmlTag(XT_UUID), _strUUID);
		spxd->getAttribute(pxn, xmlTag(XT_AUTHOR), _strAuthor);

		// Read the random number seed and properties
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_SEED));
		if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
			RGenerator::loadSeed(spxd.get(), spxpo->nodesetval->nodeTab[0]);

		// Read and set the current trial and trial attempts from the contained statistics (if any)
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_STATISTICS));
		if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
		{
			string str;

			if (spxd->getAttribute(spxpo->nodesetval->nodeTab[0], xmlTag(XT_TRIALLAST), str))
			{
				_stats._iTrialInitial =
				_stats._iTrialCurrent =
				_stats._tsMax._iTrial =
				_stats._tsMin._iTrial =
				_stats._tuMax._iTrial =
				_stats._tuMin._iTrial =
				_stats._tcMax._iTrial =
				_stats._tcMin._iTrial =
				_stats._tfMax._iTrial =
				_stats._tfMin._iTrial =
				_stats._trMax._iTrial =
				_stats._trMin._iTrial =
				_stats._tzMax._iTrial =
				_stats._tzMin._iTrial = ::atol(str.c_str());
			}
			
			if (spxd->getAttribute(spxpo->nodesetval->nodeTab[0], xmlTag(XT_TRIALATTEMPTS), str))
			{
				_stats._cTrialAttempts = ::atol(str.c_str());
			}
		}
		
		// Read the codon table
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_CODONTABLE));
		if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
			_ct.load(spxd.get(), spxpo->nodesetval->nodeTab[0]);

		// Read the bases
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_BASES));
		if (!XMLDocument::isXPathSuccess(spxpo.get(), 1))
			THROWRC((RC(XMLERROR), "Unexpected number of bases elements"));

		pxn = spxpo->nodesetval->nodeTab[0];
		spxd->getContent(pxn, _strBases);
		
		if (_strBases.length() <= (2 * Codon::s_cchCODON))
			THROWRC((RC(XMLERROR), "Too few bases supplied - must contain at least %ld bases", (2 * Codon::s_cchCODON)));

		_stats._cbBases = _strBases.length();
		_stats._tzMax._cbBases = _stats._cbBases;
		_stats._tzMin._cbBases = _stats._cbBases;
		
		_statsRecordRate = _stats;
		
		// Read and preserve supplied strain and ancestor details
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_LINEAGE));
		if (XMLDocument::isXPathSuccess(spxpo.get(), 1))
		{
			pxn = spxpo->nodesetval->nodeTab[0];
			spxd->getAttribute(pxn, xmlTag(XT_STRAIN), _strStrain);
			spxd->getAttribute(pxn, xmlTag(XT_ANCESTORS), _strAncestors);
		}

		// Load each gene
		spxpo = spxd->evalXPath(spxpc.get(), xmlXPath(XP_GENE));	
		if (!XMLDocument::isXPathSuccess(spxpo.get(), static_cast<int>(s_maxGENES)))
			THROWRC((RC(XMLERROR), "This version of Stylus requires between 1 and %ld genes", s_maxGENES));

		_fGenesAssigned = true;
		_vecGenes.resize(spxpo->nodesetval->nodeNr);
		for (size_t iGene=0; iGene < _vecGenes.size(); ++iGene)
			_vecGenes[iGene].setID(iGene);
		_grfGenesInvalid.set();
		loadFromXML<GENEARRAY>(_vecGenes, spxd.get(), spxpo->nodesetval->nodeNr, spxpo->nodesetval->nodeTab);

		LOGINFO((LLINFO, "Loaded genome %s containing %d genes - trial set to %lu", _strUUID.c_str(), _vecGenes.size(), getTrial()));

		// Finalize the load state
		exitState();
	}

	// Fail if the genome failed to load or transition to INVALID
	if (!isState(STGS_INVALID))
	{
		enterState(STGS_DEAD);
		THROWRC((RC(XMLERROR), "Loaded Genome definition failed to load"));
	}

	// Validate the genome
	else if (!validate())
	{
		enterState(STGS_DEAD);
		THROWRC((RC(XMLERROR), "Loaded Genome definition failed validation"));
	}

	// If validation resulted in any change records, fail the load (since change records at
	// this point imply disagreements between the XML and calculated values)
	else if (!_msModifications.isEmpty())
	{
		enterState(STGS_DEAD);
		THROWRC((RC(XMLERROR), "Loaded Genome definition does not match validated structure"));
	}
	
	// If everything loaded successfully, finally set the author name
	// - This preserves the original author's name with the loaded genome, but uses the new
	//   name for all subsequent genomes
	if (VALID(pszAuthor))
		_strAuthor = pszAuthor;

	// Mark the genome ready for use if it is alive
	if (isState(STGS_ALIVE))
		_fReady = true;
}

/*
 * Function: setUUIDSeeds
 *
 */
void
Genome::setUUIDSeeds(const char* pszSeeds)
{
	ENTER(GENOME,setUUIDSeeds);

	_strUUIDSeeds = pszSeeds;
}

/*
 * Function: getGenome
 *
 */
void
Genome::getGenome(char* pxmlGenome, size_t* pcchGenome, STFLAGS grfRecordDetail)
{
	ENTER(GENOME,getGenome);

	ASSERT(VALID(pxmlGenome));
	ASSERT(VALID(pcchGenome));
	
	ostringstream ostr;
	XMLStream xs(ostr);
	toXML(xs, grfRecordDetail);
	
	string strGenome = ostr.str();
	if (*pcchGenome < (strGenome.length()+1))
	{
		*pcchGenome = strGenome.length() + 1;
		THROWRC((RC(BUFFERTOOSMALL), "Genome buffer is too small - must be at least %ld bytes", *pcchGenome));
	}

	THROWIFERROR(copyBytes(pxmlGenome, pcchGenome, strGenome.c_str(), strGenome.length()));
	*(pxmlGenome+*pcchGenome) = Constants::s_chNULL;
	++(*pcchGenome);
}

/*
 * Function: setRecordRate
 * 
 */
void
Genome::setRecordRate(size_t cRecordRate, STFLAGS grfRecordDetail, const char* pszRecordDirectory, bool fRecordHistory)
{
	ENTER(GENOME,setRecordRate);
	ASSERT(grfRecordDetail == STRD_NONE || VALID(pszRecordDirectory));
	ASSERT(!fRecordHistory || VALID(pszRecordDirectory));
	
	_cRecordRate = cRecordRate;
	_grfRecordDetail = grfRecordDetail;
	if (VALID(pszRecordDirectory))
	{
		_strRecordDirectory.assign(pszRecordDirectory);
		terminatePath(_strRecordDirectory);
	}
	else
		_strRecordDirectory.clear();
		
	_fRecordHistory = fRecordHistory;
}

/*
 * Function: recordAttempt
 *
 */
void
Genome::recordAttempt(const char* pszFileline, ST_TRACEREGION tr, const char* pszFormat, ...) throw()
{
	ENTER(GENOME,recordAttempt);

	ASSERT(VALID(pszFileline));
	ASSERT(VALID(pszFormat));

	char szTermination[Constants::s_cbmaxBUFFER];
	char* pszTermination = szTermination;
	size_t cbRemaining = SZ_OF(szTermination);
	size_t cbWritten;
	
	va_list ap;
	va_start(ap, pszFormat);

	cbWritten = copyFileline(pszTermination, cbRemaining, pszFileline);
	pszTermination += cbWritten;
	cbRemaining -= cbWritten;

	*pszTermination++ = Constants::s_chBLANK;
	cbRemaining -= 1;
	
	::vsnprintf(pszTermination, cbRemaining, pszFormat, ap);

	va_end(ap);

	_msModifications.recordDescription(szTermination);
	if (Globals::traceIf(tr, STTC_FLOW, STTL_L2)) LOGTRACE((LLTRACE, szTermination));
}

/*
 * Function: recordTermination
 *
 */
void
Genome::recordTermination(const char* pszFileline, ST_GENOMETERMINATION ga, ST_GENOMEREASON gr, const char* pszFormat, ...) throw()
{
	ENTER(GENOME,recordTermination);

	ASSERT(VALID(pszFormat));

	char szTermination[Constants::s_cbmaxBUFFER];
	char* pszTermination = szTermination;
	size_t cbRemaining = SZ_OF(szTermination);
	size_t cbWritten;
	
	_gaTermination = ga;
	_grTermination = gr;
	
	va_list ap;
	va_start(ap, pszFormat);

	if (VALID(pszFileline))
	{
		cbWritten = copyFileline(pszTermination, cbRemaining, pszFileline);
		pszTermination += cbWritten;
		cbRemaining -= cbWritten;

		*pszTermination++ = Constants::s_chBLANK;
		cbRemaining -= 1;
	}
	
	::vsnprintf(pszTermination, cbRemaining, pszFormat, ap);

	va_end(ap);

	_strTermination = szTermination;
	if (VALID(pszFileline))
		LOGINFO((LLINFO, szTermination));
}

/*
 * Function: ç
 * 
 */
void
Genome::getTermination(ST_GENOMETERMINATION* pgt, ST_GENOMEREASON* pgr, char* pszTermination, size_t* pcchTermination)
{
	ENTER(GENOME,getTermination);
	
	ASSERT(VALID(pgt));
	ASSERT(VALID(pgr));
	
	*pgt = _gaTermination;
	*pgr = _grTermination;

	if (VALID(pszTermination) && VALID(pcchTermination))
	{
		if (*pcchTermination < (_strTermination.length()+1))
		{
			*pcchTermination = _strTermination.length() + 1;
			THROWRC((RC(BUFFERTOOSMALL), "Termination buffer is too small - must be at least %ld bytes", *pcchTermination));
		}

		THROWIFERROR(copyBytes(pszTermination, pcchTermination, _strTermination.c_str(), _strTermination.length()));
		*(pszTermination+*pcchTermination) = Constants::s_chNULL;
		++(*pcchTermination);
	}
}


/*
 * Function: toString
 *
 */
string
Genome::toString()
{
	return Constants::s_strEMPTY;
};

/*
 * Function: toXML
 *
 */
void
Genome::toXML(XMLStream& xs, STFLAGS grfRecordDetail, bool fUseTrialStatistics)
{
	ENTER(GENOME,toXML);
	
	char szTime[Constants::s_cchTIME];
	
	timeToString(szTime, &_tLoaded, false, true);

	// Get a new UUID for the new XML file
	_strUUID = _vecUUIDs.back();
	_vecUUIDs.pop_back();

	// Add the genome element and attributes
	xs.openStart(xmlTag(XT_GENOME));
	xs.writeAttribute(xmlTag(XT_XMLNS), XMLDocument::s_szStylusNamespace);
	xs.writeAttribute(xmlTag(XT_UUID), _strUUID);
	if (!EMPTYSTR(_strAuthor))
		xs.writeAttribute(xmlTag(XT_AUTHOR), _strAuthor);
	xs.writeAttribute(xmlTag(XT_CREATIONTOOL), Globals::s_szBuild);
	xs.writeAttribute(xmlTag(XT_CREATIONDATE), szTime);
	xs.closeStart();
	
	// Add the seed child element
	RGenerator::getSeed(xs, grfRecordDetail);
	
	// Add the codon table
	_ct.toXML(xs, grfRecordDetail);
	
	// Add the bases element
	xs.writeStart(xmlTag(XT_BASES), true, false);
	xs.writeContent(_strBases);
	xs.writeEnd(xmlTag(XT_BASES));
	
	// Add termination code if one exists
	if (_gaTermination != STGT_NONE)
	{
		xs.openStart(xmlTag(XT_TERMINATION));
		xs.writeAttribute(xmlTag(XT_TERMINATIONCODE), _gaTermination);
		xs.writeAttribute(xmlTag(XT_REASONCODE), _grTermination);
		xs.writeAttribute(xmlTag(XT_DESCRIPTION), _strTermination);
		xs.closeStart(false);
	}
	
	// Add any requested statistics
	if (ST_ISANYSET(grfRecordDetail, STRD_SCORE | STRD_SCOREALL | STRD_STATISTICS))
	{
		// Use global statistics unless RECORDING
		ST_STATISTICS& stats = (fUseTrialStatistics
								? _statsRecordRate
								: _stats);
			
		xs.openStart(xmlTag(XT_STATISTICS));
		xs.writeAttribute(xmlTag(XT_TRIALFIRST), stats._iTrialInitial);
		xs.writeAttribute(xmlTag(XT_TRIALLAST), stats._iTrialCurrent);
		xs.writeAttribute(xmlTag(XT_TRIALATTEMPTS), stats._cTrialAttempts);
		if (isState(STGS_ALIVE) || isState(STGS_RECORDING))
		{
			xs.writeAttribute(xmlTag(XT_SCORE), stats._nScore);
			xs.writeAttribute(xmlTag(XT_UNITS), stats._nUnits);
			xs.writeAttribute(xmlTag(XT_COST), stats._nCost);
			xs.writeAttribute(xmlTag(XT_FITNESS), stats._nFitness);
		}

		// Close statistics without content if only recording score
		if (ST_ISCLEAR(grfRecordDetail, STRD_SCOREALL | STRD_STATISTICS))
		{
			xs.closeStart(false);
		}

		// Otherwise, add all statistics content
		else
		{
			if (ST_ISANYSET(grfRecordDetail, STRD_STATISTICS))
			{
				ASSERT(stats._cbBases == _strBases.length());
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._cbBases);
				xs.writeAttribute(xmlTag(XT_COUNTROLLBACKS), stats._cRollbacks);
			}
			xs.closeStart();

			// Record detailed scores, units, costs, and fitness levels
			xs.writeStart(xmlTag(XT_SCORERANGE));
		
			xs.openStart(xmlTag(XT_MAXIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tsMax._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tsMax._nValue);
			xs.closeStart(false);

			xs.openStart(xmlTag(XT_MINIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tsMin._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tsMin._nValue);
			xs.closeStart(false);

			xs.writeEnd(xmlTag(XT_SCORERANGE));

			xs.writeStart(xmlTag(XT_UNITSRANGE));
		
			xs.openStart(xmlTag(XT_MAXIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tuMax._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tuMax._nValue);
			xs.closeStart(false);

			xs.openStart(xmlTag(XT_MINIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tuMin._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tuMin._nValue);
			xs.closeStart(false);

			xs.writeEnd(xmlTag(XT_UNITSRANGE));

			xs.writeStart(xmlTag(XT_COSTRANGE));
		
			xs.openStart(xmlTag(XT_MAXIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tcMax._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tcMax._nValue);
			xs.closeStart(false);

			xs.openStart(xmlTag(XT_MINIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tcMin._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tcMin._nValue);
			xs.closeStart(false);

			xs.writeEnd(xmlTag(XT_COSTRANGE));

			xs.writeStart(xmlTag(XT_FITNESSRANGE));
		
			xs.openStart(xmlTag(XT_MAXIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tfMax._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tfMax._nValue);
			xs.closeStart(false);

			xs.openStart(xmlTag(XT_MINIMUM));
			xs.writeAttribute(xmlTag(XT_TRIAL), stats._tfMin._iTrial);
			xs.writeAttribute(xmlTag(XT_VALUE), stats._tfMin._nValue);
			xs.closeStart(false);

			xs.writeEnd(xmlTag(XT_FITNESSRANGE));

			if (ST_ISANYSET(grfRecordDetail, STRD_STATISTICS))
			{
				// Record statistics on bases
				xs.openStart(xmlTag(XT_BASES));
				xs.writeAttribute(xmlTag(XT_CHANGED), stats._cbBasesChanged);
				xs.writeAttribute(xmlTag(XT_INSERTED), stats._cbBasesInserted);
				xs.writeAttribute(xmlTag(XT_DELETED), stats._cbBasesDeleted);
				xs.closeStart();

				xs.openStart(xmlTag(XT_MAXIMUM));
				xs.writeAttribute(xmlTag(XT_TRIAL), stats._tzMax._iTrial);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._tzMax._cbBases);
				xs.closeStart(false);

				xs.openStart(xmlTag(XT_MINIMUM));
				xs.writeAttribute(xmlTag(XT_TRIAL), stats._tzMin._iTrial);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._tzMin._cbBases);
				xs.closeStart(false);
			
				xs.writeEnd(xmlTag(XT_BASES));

				// Record statistics on rollbacks
				xs.openStart(xmlTag(XT_ROLLBACKS));
				xs.writeAttribute(xmlTag(XT_COUNTROLLBACKS), stats._cTotalRollbacks);
				xs.closeStart();

				xs.openStart(xmlTag(XT_MAXIMUM));
				xs.writeAttribute(xmlTag(XT_TRIAL), stats._trMax._iTrial);
				xs.writeAttribute(xmlTag(XT_COUNTROLLBACKS), stats._trMax._cRollbacks);
				xs.closeStart(false);

				xs.openStart(xmlTag(XT_MINIMUM));
				xs.writeAttribute(xmlTag(XT_TRIAL), stats._trMin._iTrial);
				xs.writeAttribute(xmlTag(XT_COUNTROLLBACKS), (stats._trMin._cRollbacks < numeric_limits<size_t>::max()
														? stats._trMin._cRollbacks
														: 0));
				xs.closeStart(false);
			
				xs.writeEnd(xmlTag(XT_ROLLBACKS));

				// Record statistics on mutations
				xs.writeStart(xmlTag(XT_MUTATIONS));

				xs.openStart(xmlTag(XT_CHANGES));
				xs.writeAttribute(xmlTag(XT_ATTEMPTED), stats._atChanged._cAttempted);
				xs.writeAttribute(xmlTag(XT_ACCEPTED), stats._atChanged._cAccepted);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._atChanged._cbBases);
				xs.writeAttribute(xmlTag(XT_SILENT), stats._cSilent);
				xs.closeStart(false);

				xs.openStart(xmlTag(XT_COPIES));
				xs.writeAttribute(xmlTag(XT_ATTEMPTED), stats._atCopied._cAttempted);
				xs.writeAttribute(xmlTag(XT_ACCEPTED), stats._atCopied._cAccepted);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._atCopied._cbBases);
				xs.closeStart(false);

				xs.openStart(xmlTag(XT_DELETIONS));
				xs.writeAttribute(xmlTag(XT_ATTEMPTED), stats._atDeleted._cAttempted);
				xs.writeAttribute(xmlTag(XT_ACCEPTED), stats._atDeleted._cAccepted);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._atDeleted._cbBases);
				xs.closeStart(false);

				xs.openStart(xmlTag(XT_INSERTIONS));
				xs.writeAttribute(xmlTag(XT_ATTEMPTED), stats._atInserted._cAttempted);
				xs.writeAttribute(xmlTag(XT_ACCEPTED), stats._atInserted._cAccepted);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._atInserted._cbBases);
				xs.closeStart(false);

				xs.openStart(xmlTag(XT_TRANSPOSITIONS));
				xs.writeAttribute(xmlTag(XT_ATTEMPTED), stats._atTransposed._cAttempted);
				xs.writeAttribute(xmlTag(XT_ACCEPTED), stats._atTransposed._cAccepted);
				xs.writeAttribute(xmlTag(XT_COUNTBASES), stats._atTransposed._cbBases);
				xs.closeStart(false);
			
				xs.writeEnd(xmlTag(XT_MUTATIONS));
			}

			xs.writeEnd(xmlTag(XT_STATISTICS));
		}
	}
	
	// Record lineage
	// TODO: Add strain and ancestors to lineage element
	if (ST_ISANYSET(grfRecordDetail, (STRD_LINEAGE | STRD_LINEAGEALL)))
	{
		xs.writeStart(xmlTag(XT_LINEAGE));
		
		if (!EMPTYSTR(_strStrain))
			xs.writeAttribute(xmlTag(XT_STRAIN), _strStrain);
		if (!EMPTYSTR(_strAncestors))
			xs.writeAttribute(xmlTag(XT_ANCESTORS), _strAncestors);
		
		if (!_msModifications.isEmpty())
		{
			xs.writeStart(xmlTag(XT_ACCEPTEDMUTATIONS));
			_msModifications.toXML(xs, grfRecordDetail);
			xs.writeEnd(xmlTag(XT_ACCEPTEDMUTATIONS));
		}
		
		if (_vecAttempts.size() && ST_ISANYSET(grfRecordDetail, STRD_LINEAGEALL))
		{
			xs.writeStart(xmlTag(XT_REJECTEDMUTATIONS));
			for (size_t iAttempt=0; iAttempt < _vecAttempts.size(); ++iAttempt)
			{
				const ModificationStack& ms = _vecAttempts[iAttempt];
				
				xs.openStart(xmlTag(XT_ATTEMPT));
				xs.writeAttribute(xmlTag(XT_DESCRIPTION), ms.toString());
				xs.closeStart();
				
				ms.toXML(xs, grfRecordDetail);
				
				xs.writeEnd(xmlTag(XT_ATTEMPT));
			}
			xs.writeEnd(xmlTag(XT_REJECTEDMUTATIONS));
		}
		
		xs.writeEnd(xmlTag(XT_LINEAGE));
	}
	
	// Record genes
	if (ST_ISANYSET(grfRecordDetail, STRD_GENES | STRD_DIMENSIONS | STRD_SEGMENTS | STRD_SCOREALL) && _vecGenes.size() > 0)
	{
		xs.writeStart(xmlTag(XT_GENES));
		for (size_t iGene=0; iGene < _vecGenes.size(); ++iGene)
			_vecGenes[iGene].toXML(xs, grfRecordDetail);
		xs.writeEnd(xmlTag(XT_GENES));
	}

	// Close the genome element
	xs.writeEnd(xmlTag(XT_GENOME));
}

/*
 * Function: enterState
 *
 * This routine "grants permission" to enter the requested state.
 */
bool
Genome::enterState(ST_GENOMESTATE gs)
{
	const bool aryLEGALTRANSITIONS[STGS_MAX][STGS_MAX] =
	{
		// To
		// ALIVE  COMPILED COMPILING DEAD  INVALID LOADING MUTATING RECORDING ROLLBACK RESTORING SCORED  SCORING SPAWNING VALIDATED VALIDATING
		{  true,  false,   false,    true, false,  true,   false,   false,    true,    true,     false,  false,  true,    false,    false      }, // From ALIVE
		{  false, true,    false,    true, true,   false,  false,   false,    false,   false,    false,  false,  false,   false,    true       }, // From COMPILED
		{  false, true,    true,     true, true,   false,  false,   false,    false,   false,    false,  false,  false,   false,    false      }, // From COMPILING
		{  false, false,   false,    true, false,  true,   false,   false,    true,    false,    false,  false,  false,   false,    false      }, // From DEAD
		{  false, false,   true,     true, true,   true,   true,    false,    true,    false,    false,  false,  true,    false,    true       }, // From INVALID
		{  false, false,   false,    true, true,   true,   false,   false,    false,   false,    false,  false,  false,   false,    false      }, // From LOADING
		{  false, false,   false,    true, true,   false,  true,    false,    false,   false,    false,  false,  false,   false,    false      }, // From MUTATING
		{  true,  false,   false,    true, false,  false,  false,   true,     false,   false,    false,  false,  false,   false,    false      }, // From RECORDING
		{  false, false,   false,    true, true,   false,  false,   false,    true,    false,    false,  false,  false,   false,    false      }, // From ROLLBACK
		{  false, false,   false,    true, true,   false,  false,   false,    false,   true,     false,  false,  false,   false,    false      }, // From RESTORING
		{  false, false,   false,    true, false,  false,  false,   true,     false,   false,    true,   false,  false,   false,    false      }, // From SCORED
		{  false, false,   false,    true, true,   false,  false,   false,    false,   false,    true,   true,   false,   false,    false      }, // From SCORING
		{  false, false,   false,    true, true,   false,  false,   false,    false,   false,    false,  false,  true,    false,    false      }, // From SPAWNING
		{  false, false,   false,    true, false,  false,  false,   false,    false,   false,    false,  true,   false,   true,     false      }, // From VALIDATED
		{  false, false,   false,    true, true,   false,  false,   false,    false,   false,    false,  false,  false,   true,     true       }  // From VALIDATING
	};

	ENTER(GENOME,enterState);

	if (!aryLEGALTRANSITIONS[_gsCurrent][gs])
	{
		LOGERROR((LLERROR,
				  "Illegal state transition attempted - from %s to %s",
				  stateToString(), stateToString(gs)));
		return false;
	}

	TFLOW(GENOME,L3,(LLTRACE, "Transitioning from (%ld) %s to (%ld) %s", _gsCurrent, stateToString(), gs, stateToString(gs)));
	
	if (_gsCurrent != gs)
	{
		_gsCurrent = gs;
		if (isState(STGS_DEAD))
			LOGWARNING((LLWARNING, "Genome died after %ld trials", getTrial()));
	}

	return true;
}

/*
 * Function: exitState
 *
 * Invoke the passed transition function and exit the current state
 * based on its success or failure.
 */
bool
Genome::exitState(bool (*pfnTransition)())
{
	const ST_GENOMESTATE aryEXITTRANSITIONS[STGS_MAX][2] =
		{
			// Success			Failure
			{ STGS_ALIVE,		STGS_INVALID },	// From ALIVE
			{ STGS_COMPILED,	STGS_INVALID },	// From COMPILED
			{ STGS_COMPILED,	STGS_INVALID },	// From COMPILING
			{ STGS_DEAD,		STGS_DEAD    }, // From DEAD
			{ STGS_INVALID, 	STGS_INVALID },	// From INVALID
			{ STGS_INVALID,		STGS_DEAD    },	// From LOADING
			{ STGS_INVALID,		STGS_INVALID },	// From MUTATING
			{ STGS_ALIVE,		STGS_INVALID },	// From RECORDING
			{ STGS_INVALID,		STGS_DEAD    },	// From ROLLBACK
			{ STGS_INVALID,		STGS_DEAD    },	// From RESTORING
			{ STGS_SCORED,		STGS_INVALID },	// From SCORED
			{ STGS_SCORED,		STGS_INVALID },	// From SCORING
			{ STGS_INVALID,		STGS_DEAD    },	// From SPAWNING
			{ STGS_VALIDATED,	STGS_INVALID },	// From VALIDATED
			{ STGS_VALIDATED,	STGS_INVALID }	// From VALIDATING
		};

	ENTER(GENOME,exitState);

#if defined(ST_TRACE)
	ST_GENOMESTATE gsPrevious = _gsCurrent;
#endif
	bool fSuccess = (!VALID(pfnTransition) || (*pfnTransition)());
	enterState(aryEXITTRANSITIONS[_gsCurrent][fSuccess ? 0 : 1]);

	TFLOW(GENOME,L3,
		  (LLTRACE,
		   "State (%ld) %s %s exited - current state is (%ld) %s",
		   gsPrevious, stateToString(gsPrevious),
		   (fSuccess ? "successfully" : "unsuccessfully"),
		   _gsCurrent, stateToString()));

	return fSuccess;
}

/*
 * Function: advanceTrial
 * 
 */
void
Genome::advanceTrial(bool fRestore)
{
	ENTER(GENOME,advanceTrial);

	// First, spawn a new genome (as needed)
	if (isState(STGS_ALIVE))
	{
		StateGuard sg(STGS_INVALID, STGS_DEAD);

		if (	fRestore
			&&	(	!enterState(STGS_RESTORING)
				||	!exitState(doRestore)))
			THROWRC((RC(ERROR), "Unable to remove accumulated changes prior to spawning"));

		if (	!enterState(STGS_SPAWNING)
			||	!exitState(doSpawn))
			THROWRC((RC(ERROR), "Unable to spawn a new working copy of the genome prior to mutation"));
	}
	ASSERT(isState(STGS_INVALID));

	Globals::enableTracing(getTrial(), getTrialAttempts());
}

/*
 * Function: advanceTrialAttempts
 * 
 */
void
Genome::advanceTrialAttempts()
{
	ENTER(GENOME,advanceTrialAttempts);
	
	++_stats._cTrialAttempts;
	++_statsRecordRate._cTrialAttempts;

	Globals::enableTracing(getTrial(), getTrialAttempts());
}

/*
 * Function: doCompilation
 *
 */
bool
Genome::doCompilation()
{
	ENTER(GENOME,doCompilation);
	ASSERT(isState(STGS_COMPILING));

	bool fSuccess = true;
	for (size_t iGene=0; fSuccess && iGene < _vecGenes.size(); ++iGene)
	{
		if (_grfGenesInvalid.test(iGene))
			fSuccess = _vecGenes[iGene].ensureCompiled();
	}
	
	return fSuccess;
}

/*
 * Function: doRecording
 *
 */
bool
Genome::doRecording()
{
	ENTER(GENOME,doRecording);
	ASSERT(isState(STGS_RECORDING));
	
	if (isRecordingHistory())
		recordHistory(RT_TRIAL);

	if (isRecordingTrial())
	{
		record(RT_TRIAL);

		// Clear the record rate statistics and advance to the next recording window
		clearStatistics(_statsRecordRate, getTrial());
		++_statsRecordRate._iTrialInitial;
	}

	return true;
}

/*
 * Function: doRestore
 *
 */
bool
Genome::doRestore()
{
	ENTER(MUTATION,doRestore);
	ASSERT(isState(STGS_ROLLBACK) || isState(STGS_RESTORING));

	// Remove each recorded change
	_msModifications.undo();

	// Remove all recorded changes
	purgeModifications(isState(STGS_ROLLBACK));
	
	return true;
}

/*
 * Function: doRollback
 *
 */
bool
Genome::doRollback()
{
	ENTER(GENOME,doRollback);
	ASSERT(isState(STGS_ROLLBACK));
	
	bool fSuccess = false;
	if (_fReady)
	{
        if(_rollbackType == RT_ATTEMPT)
        {
            ++_statsRecordRate._cRollbacks;
            ++_statsRecordRate._cTotalRollbacks;
            ++_stats._cRollbacks;
            ++_stats._cTotalRollbacks;
        }

		fSuccess = doRestore();

#ifdef ST_DEBUG
		if (fSuccess)
			fSuccess = testRollback();
#endif
	}
	
	return fSuccess;
}

/**
 * \brief performs a rollback, but doesn't record stats about it
 */
bool
Genome::rollback()
{
    return (	Genome::enterState(STGS_ROLLBACK)
            &&	Genome::exitState(Genome::doRollback));
}


/*
 * Function: doScoring
 *
 */
bool
Genome::doScoring()
{
	ENTER(SCORING,doScoring);
	ASSERT(isState(STGS_SCORING));

	// Score each contained gene
	bool fSuccess = true;
	for (size_t iGene=0; fSuccess && iGene < _vecGenes.size(); ++iGene)
	{
		if (_grfGenesInvalid.test(iGene))
			fSuccess = _vecGenes[iGene].ensureScore();
	}

	// NOTE: This version of Stylus assumes and supports only a single gene
	ASSERT(_vecGenes.size() == 1);
	if (fSuccess)
	{
		const Gene& gene = _vecGenes[0];
		_statsRecordRate._nScore = gene.score();
		_statsRecordRate._nUnits = gene.units();
		_statsRecordRate._nCost = Globals::getGenomeWeight(SC_FIXEDCOST) + (Unit(Globals::getGenomeWeight(SC_COSTPERBASE)) * gene.getRange().getLength()) +  (Unit(Globals::getGenomeWeight(SC_COSTPERUNIT)) * gene.units());
		_statsRecordRate._nFitness = (_statsRecordRate._nScore / _statsRecordRate._nCost);

		_stats._nScore = _statsRecordRate._nScore;
		_stats._nUnits = _statsRecordRate._nUnits;
		_stats._nCost = _statsRecordRate._nCost;
		_stats._nFitness = _statsRecordRate._nFitness;

		TFLOW(SCORING,L1,(LLTRACE,
						  "Genome fitness for trial %ld is %f",
						  _statsRecordRate._iTrialCurrent,
						  static_cast<UNIT>(_statsRecordRate._nFitness)));

		if (_plan.isExecuting())
		{
			fSuccess =	_plan.evaluateCondition(PC_TRIALCOST, _stats._nCost)
					&&	_plan.evaluateCondition(PC_TRIALFITNESS, _stats._nFitness)
					&&	_plan.evaluateCondition(PC_TRIALSCORE, _stats._nScore);
		}

	}
	else
	{
		_statsRecordRate._nScore = Unit::getUndefined();
		_statsRecordRate._nUnits = Unit::getUndefined();
		_statsRecordRate._nCost = Unit::getUndefined();
		_statsRecordRate._nFitness = Unit::getUndefined();
		fSuccess = false;
	}
	
	return fSuccess;
}

bool Genome::recordStatistics(bool fPreserveErrors)
{
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tsMax._iTrial
        ||	_statsRecordRate._nScore > _statsRecordRate._tsMax._nValue)
    {
        _statsRecordRate._tsMax._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tsMax._nValue = _statsRecordRate._nScore;
        if (_statsRecordRate._tsMax._nValue > _stats._tsMax._nValue)
            _stats._tsMax = _statsRecordRate._tsMax;
    }
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tsMin._iTrial
        ||	_statsRecordRate._nScore < _statsRecordRate._tsMin._nValue)
    {
        _statsRecordRate._tsMin._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tsMin._nValue = _statsRecordRate._nScore;
        if (_statsRecordRate._tsMin._nValue < _stats._tsMin._nValue)
            _stats._tsMin = _statsRecordRate._tsMin;
    }

    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tuMax._iTrial
        ||	_statsRecordRate._nUnits > _statsRecordRate._tuMax._nValue)
    {
        _statsRecordRate._tuMax._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tuMax._nValue = _statsRecordRate._nUnits;
        if (_statsRecordRate._tuMax._nValue > _stats._tuMax._nValue)
            _stats._tuMax = _statsRecordRate._tuMax;
    }
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tuMin._iTrial
        ||	_statsRecordRate._nUnits < _statsRecordRate._tuMin._nValue)
    {
        _statsRecordRate._tuMin._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tuMin._nValue = _statsRecordRate._nUnits;
        if (_statsRecordRate._tuMin._nValue < _stats._tuMin._nValue)
            _stats._tuMin = _statsRecordRate._tuMin;
    }

    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tcMax._iTrial
        ||	_statsRecordRate._nCost > _statsRecordRate._tcMax._nValue)
    {
        _statsRecordRate._tcMax._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tcMax._nValue = _statsRecordRate._nCost;
        if (_statsRecordRate._tcMax._nValue > _stats._tcMax._nValue)
            _stats._tcMax = _statsRecordRate._tcMax;
    }
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tcMin._iTrial
        ||	_statsRecordRate._nCost < _statsRecordRate._tcMin._nValue)
    {
        _statsRecordRate._tcMin._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tcMin._nValue = _statsRecordRate._nCost;
        if (_statsRecordRate._tcMin._nValue < _stats._tcMin._nValue)
            _stats._tcMin = _statsRecordRate._tcMin;
    }

    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tfMax._iTrial
        ||	_statsRecordRate._nFitness > _statsRecordRate._tfMax._nValue)
    {
        _statsRecordRate._tfMax._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tfMax._nValue = _statsRecordRate._nFitness;
        if (_statsRecordRate._tfMax._nValue > _stats._tfMax._nValue)
            _stats._tfMax = _statsRecordRate._tfMax;
    }
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tfMin._iTrial
        ||	_statsRecordRate._nFitness < _statsRecordRate._tfMin._nValue)
    {
        _statsRecordRate._tfMin._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tfMin._nValue = _statsRecordRate._nFitness;
        if (_statsRecordRate._tfMin._nValue < _stats._tfMin._nValue)
            _stats._tfMin = _statsRecordRate._tfMin;
    }

    LOGATRATE((LLRATE,
               "TRIAL %ld: Fitness is %0.15f",
               _statsRecordRate._iTrialCurrent,
               static_cast<UNIT>(_statsRecordRate._nFitness)),
              _statsRecordRate._iTrialCurrent);

#ifdef ST_DEBUG
    _nFitnessPassing = _statsRecordRate._nFitness;
#endif

    if (!fPreserveErrors)
    {
        _gaTermination = STGT_NONE;
        _grTermination = STGR_NONE;
        _strTermination.clear();
    }
    
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._trMax._iTrial
        ||	_statsRecordRate._cRollbacks > _statsRecordRate._trMax._cRollbacks)
    {
        _statsRecordRate._trMax._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._trMax._cRollbacks = _statsRecordRate._cRollbacks;
        if (_statsRecordRate._trMax._cRollbacks > _stats._trMax._cRollbacks)
            _stats._trMax = _statsRecordRate._trMax;
    }
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._trMin._iTrial
        ||	_statsRecordRate._cRollbacks < _statsRecordRate._trMin._cRollbacks)
    {
        _statsRecordRate._trMin._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._trMin._cRollbacks = _statsRecordRate._cRollbacks;
        if (_statsRecordRate._trMin._cRollbacks < _stats._trMin._cRollbacks)
            _stats._trMin = _statsRecordRate._trMin;
    }

    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tzMax._iTrial
        ||	_strBases.length() > _statsRecordRate._tzMax._cbBases)
    {
        _statsRecordRate._tzMax._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tzMax._cbBases = _strBases.length();
        if (_statsRecordRate._tzMax._cbBases > _stats._tzMax._cbBases)
            _stats._tzMax = _statsRecordRate._tzMax;
    }
    if (	_statsRecordRate._iTrialInitial > _statsRecordRate._tzMin._iTrial
        ||	_strBases.length() < _statsRecordRate._tzMin._cbBases)
    {
        _statsRecordRate._tzMin._iTrial = _statsRecordRate._iTrialCurrent;
        _statsRecordRate._tzMin._cbBases = _strBases.length();
        if (_statsRecordRate._tzMin._cbBases < _stats._tzMin._cbBases)
            _stats._tzMin = _statsRecordRate._tzMin;
    }

    return enterState(STGS_RECORDING) && exitState(doRecording);


}

/*
 * Function: doSpawn
 *
 */
bool
Genome::doSpawn()
{
	ENTER(GENOME,doSpawn);
	ASSERT(isState(STGS_SPAWNING));

	_grfGenesInvalid.reset();

	_statsRecordRate._cRollbacks = 0;
	_stats._cRollbacks = 0;

	++_stats._iTrialCurrent;
	_statsRecordRate._iTrialCurrent = _stats._iTrialCurrent;

	purgeModifications();
	
	return true;
}

/*
 * Function: doValidation
 *
 */
bool
Genome::doValidation()
{
	ENTER(GENOME,doValidation);
	ASSERT(isState(STGS_VALIDATING));

	bool fSuccess = true;
	for (size_t iGene=0; fSuccess && iGene < _vecGenes.size(); ++iGene)
	{
		if (_grfGenesInvalid.test(iGene))
			fSuccess = _vecGenes[iGene].ensureValid();
	}
	
	return fSuccess;
}

/*
 * Function: record
 * 
 */
void
Genome::record(RECORDTYPE rt)
{
	ENTER(GENOME,record);
	
	ASSERT(_grfRecordDetail == STRD_NONE || !EMPTYSTR(_strRecordDirectory));
	
	// Record only if requested
	if (!isRecording())
		return;

	// Generate the full path name
	ostringstream ostrPathname;
	ostrPathname
		<< _strRecordDirectory
		<< (rt == RT_TRIAL
			? Constants::s_strTRIAL
			: (rt == RT_INITIAL
				? Constants::s_strINITIAL
				: Constants::s_strFINAL));
	if (rt == RT_TRIAL)
		ostrPathname << getTrial();
	ostrPathname
		<< Constants::s_strXMLEXTENSION;

	// Return if the initial file already exists
	if (rt == RT_INITIAL)
	{
		struct stat st;
		if (::stat(ostrPathname.str().c_str(), &st) == 0)
			return;
	}

	// Write the file
	ofstream ofstr(ostrPathname.str().c_str(), ios::out | ios::trunc);
	if (!ofstr || !ofstr.is_open())
		THROWRC((RC(ERROR), "Unable to create genome record file %s", ostrPathname.str().c_str()));

	XMLStream xs(ofstr);
	toXML(xs, _grfRecordDetail, (rt == RT_TRIAL));
}

/*
 * Function: recordHistory
 * 
 */
void
Genome::recordHistory(RECORDTYPE rt)
{
	ENTER(GENOME,record);
	
	ASSERT(_fRecordHistory);
	ASSERT(!EMPTYSTR(_strRecordDirectory));
	
	// Generate the full path name
	ostringstream ostrPathname;
	ostrPathname
		<< _strRecordDirectory
		<< Constants::s_strHISTORY
		<< Constants::s_strXMLEXTENSION;
		
	// If initializing, remove any previous history file
	if (rt == RT_INITIAL)
	{
		char szTime[Constants::s_cchTIME];

		timeToString(szTime, &_tLoaded, false, true);

		ofstream ofstr(ostrPathname.str().c_str(), ios::out | ios::trunc);
		if (!ofstr || !ofstr.is_open())
			THROWRC((RC(ERROR), "Unable to create genome history file %s", ostrPathname.str().c_str()));
			
		XMLStream xs(ofstr);
		xs.openStart(xmlTag(XT_HISTORY));
		xs.writeAttribute(xmlTag(XT_XMLNS), XMLDocument::s_szStylusNamespace);
		xs.writeAttribute(xmlTag(XT_UUID), _strUUID);
		xs.writeAttribute(xmlTag(XT_CREATIONTOOL), Globals::s_szBuild);
		xs.writeAttribute(xmlTag(XT_CREATIONDATE), szTime);
		xs.closeStart();
	}

	// If finalizing, close the history file
	else if (rt == RT_FINAL)
	{
		ofstream ofstr(ostrPathname.str().c_str(), ios::out | ios::app);
		if (!ofstr || !ofstr.is_open())
			THROWRC((RC(ERROR), "Unable to open genome history file %s", ostrPathname.str().c_str()));

		XMLStream xs(ofstr, false);
		xs.writeEnd(xmlTag(XT_HISTORY));
	}
	
	// Otherwise, write modifications to the file
	else
	{
		ofstream ofstr(ostrPathname.str().c_str(), ios::out | ios::app);
		if (!ofstr || !ofstr.is_open())
			THROWRC((RC(ERROR), "Unable to open genome history file %s", ostrPathname.str().c_str()));

		XMLStream xs(ofstr, false);

		xs.openStart(xmlTag(XT_ACCEPTEDMUTATIONS));
		xs.writeAttribute(xmlTag(XT_TRIAL), getTrial());
		xs.writeAttribute(xmlTag(XT_FITNESS), getFitness());
		xs.closeStart();

		if (!_msModifications.isEmpty())
			_msModifications.toXML(xs, STRD_ALL);

		xs.writeEnd(xmlTag(XT_ACCEPTEDMUTATIONS));
	}
}

/*
 * Function: validate
 *
 */
bool
Genome::validate(bool fPreserveErrors)
{
	ENTER(GENOME,validate);
	REQUIRENOTDEAD(validate);

	if (!isState(STGS_ALIVE) && !isState(STGS_INVALID))
		THROWRC((RC(INVALIDSTATE),
				 "Attempt to validate genome while incorrect state (%s)", stateToString()));

	if (isState(STGS_ALIVE))
		return true;

	bool fSuccess = false;
	{
		StateGuard sg(STGS_ALIVE, STGS_INVALID);

		fSuccess = (	enterState(STGS_COMPILING)
					&&	exitState(doCompilation)
					&&	enterState(STGS_VALIDATING)
					&&	exitState(doValidation)
					&&	enterState(STGS_SCORING)
					&&	exitState(doScoring));

		// If successful, track the rollback and size counters
		if (fSuccess)
		{
            recordStatistics(fPreserveErrors);
        }
	}

	return fSuccess;
}

/*
 * Function: purgeModifications
 *
 */
void
Genome::purgeModifications(bool fPreserveAttempts)
{
	ENTER(GENOME,purgeModifications);
	
	// If in ROLLBACK, save the failed changes to the failed change history stack
	if (fPreserveAttempts)
	{
		TFLOW(GENOME,L3,(LLTRACE, "Saving %ld changes to failed change history", _msModifications.length()));
		ASSERT(_msModifications.length() <= 0 || !EMPTYSTR(_msModifications.toString()))
        switch( _rollbackType)
        {
            case RT_ATTEMPT:
                _vecAttempts.push_back(_msModifications);
                break;
            case RT_CONSIDERATION:
                _vecConsiderations.push_back(_msModifications);
                break;
            default:
                ASSERT(false);
                break;
        }
	}

	// Otherwise, delete all history
	else
	{
		_vecAttempts.clear();
	}
	_msModifications.clear();

	ASSERT(_msModifications.isEmpty());
}

/*
 * Function: rgenIndex
 *
 */
size_t
Genome::rgenIndex(const Range& rgBases, bool fInFrame)
{
	ASSERT(!fInFrame || Codon::onCodonBoundary(rgBases.getStart()) || Codon::onCodonBoundary(rgBases.getEnd()) || rgBases.getLength() >= Codon::s_cchCODON);

	if (rgBases.getLength() == 1)
		return static_cast<size_t>(rgBases.getStart());
	else
	{
		long iBase;
		do
			iBase = RGenerator::getUniform(rgBases.getStart(), rgBases.getEnd());
		while (fInFrame && !Codon::onCodonBoundary(iBase));
		return static_cast<size_t>(iBase);
	}
}

/*
 * Function: rgenLength
 *
 */
size_t
Genome::rgenLength(size_t cbLength, bool fWholeCodons)
{
	ASSERT(cbLength >= 1);
	size_t cb;
	do
		cb = static_cast<size_t>(RGenerator::getUniform(1L, cbLength));
	while (fWholeCodons && !Codon::onCodonBoundary(cb));
	return cb;
}

/*
 * Function: rgenTransversion
 * 
 */
string&
Genome::rgenTransversion(size_t iTarget, string& strBases, UNIT nTransversionLikelihood)
{
	ASSERT(iTarget >= 0 && iTarget < _strBases.length());

	// Perform a transversion
	Unit pTransversionLikelihood(nTransversionLikelihood);
	char chBase = _strBases[iTarget];
	if (pTransversionLikelihood == 1.0 || pTransversionLikelihood >= RGenerator::getUniform())
	{
		strBases = (chBase == Constants::s_chBASET || chBase == Constants::s_chBASEC
					? Constants::s_strBASESAG[static_cast<size_t>(RGenerator::getUniform(0L, 1L))]
					: Constants::s_strBASESTC[static_cast<size_t>(RGenerator::getUniform(0L, 1L))]);
	}
	
	// Otherwise, perform a transition
	else
	{
		strBases = (chBase == Constants::s_chBASET
						? Constants::s_chBASEC
						: chBase == Constants::s_chBASEC
							? Constants::s_chBASET
							: chBase == Constants::s_chBASEA
								? Constants::s_chBASEG
								: Constants::s_chBASEA);
	}

	return strBases;
}

/*
 * Function: rgenBases
 *
 */
string&
Genome::rgenBases(string& strBases, size_t cbLength)
{
	ASSERT(cbLength > 0);
	ASSERT(Constants::s_strBASES.length() == 4);
	
	// Generate a string of the requested length
	strBases.resize(cbLength);
	for (size_t i=0; i < cbLength; ++i)
		strBases[i] = Constants::s_strBASES[static_cast<size_t>(RGenerator::getUniform(0L,3L))];

	return strBases;
}

#ifdef ST_DEBUG
/*
 * Function: testRollback
 * 
 * NOTE:
 * - This test *only* works when accumulating mutations. Without accumulation, the restore process that occurs at spawn
 * time does not have the data needed to test against (e.g., the last passing fitness level).
 *
 */
bool
Genome::testRollback()
{
	ASSERT(isState(STGS_ROLLBACK));
	
	if (_fTestingRollback)
		return true;
	_fTestingRollback = true;

	Unit nFitnessPassing = _nFitnessPassing;
	ST_GENOMESTATE gsCurrent = _gsCurrent;
	ST_STATISTICS stats = _stats;
	ST_STATISTICS statsRecordRate = _statsRecordRate;
	GENEARRAY vecGenesFromFixup(_vecGenes.size());
	bool fSuccess = false;

	TFLOW(GENOME,L1,(LLTRACE, "INITIATING ROLLBACK TEST -------------------------------------------------------------------------------------------"));

	for (size_t iGene=0; iGene < _vecGenes.size(); ++iGene)
	{
		vecGenesFromFixup[iGene] = _vecGenes[iGene];
		_vecGenes[iGene].markInvalid();
		_grfGenesInvalid.set(iGene);
	}

	try
	{
		fSuccess = enterState(STGS_INVALID);
		if (!fSuccess)
			TFLOW(GENOME,L2,(LLTRACE, "Rollback test unable to enter INVALID state, state is %s", stateToString()))
		else
		{
			fSuccess = enterState(STGS_COMPILING);
			if (!fSuccess)
				TFLOW(GENOME,L2,(LLTRACE, "Rollback test unable to enter COMPILING state, state is %s", stateToString()))
			else
			{
				fSuccess = exitState(doCompilation);
				if (!fSuccess)
					TFLOW(GENOME,L2,(LLTRACE, "Rollback test unable to exit COMPILING state, state is %s", stateToString()))
				else
				{
					fSuccess = enterState(STGS_VALIDATING);
					if (!fSuccess)
						TFLOW(GENOME,L2,(LLTRACE, "Rollback test unable to enter VALIDATING state, state is %s", stateToString()))
					else
					{
						fSuccess = exitState(doValidation);
						if (!fSuccess)
							TFLOW(GENOME,L2,(LLTRACE, "Rollback test unable to exit VALIDATING state, state is %s", stateToString()))
						else
						{
							fSuccess = enterState(STGS_SCORING);
							if (!fSuccess)
								TFLOW(GENOME,L2,(LLTRACE, "Rollback test unable to enter SCORING state, state is %s", stateToString()))
							else
							{
								fSuccess = exitState(doScoring);
								if (!fSuccess)
									TFLOW(GENOME,L2,(LLTRACE, "Rollback test failed to exit SCORING state, state is %s", stateToString()))
								else
								{
									fSuccess = (_nFitnessPassing == nFitnessPassing);
									if (!fSuccess)
										LOGERROR((LLERROR,
												 "Rollback test failed to produce same fitness level: %0.15f (after) vs. %0.15f (before)",
												 static_cast<UNIT>(_nFitnessPassing),
												 static_cast<UNIT>(nFitnessPassing)));
								}
							}
						}
					}
				}
			}
		}
	}

	catch (Error* perror)
	{
		LOGERROR((LLERROR, "Rollback failed with error %s", perror->toCString()));
		fSuccess = false;
	}
	
	if (!fSuccess)
	{
		for (size_t iGene=0; iGene < _vecGenes.size() && iGene < vecGenesFromFixup.size(); ++iGene)
		{
			if (_vecGenes[iGene] != vecGenesFromFixup[iGene])
				LOGERROR((LLERROR,
						"Rollback test failed - gene %lu differs after restoration", iGene+1));
		}
	}

	_nFitnessPassing = nFitnessPassing;
	_gsCurrent = gsCurrent;
	_stats = stats;
	_statsRecordRate = statsRecordRate;

	TFLOW(GENOME,L1,(LLTRACE, "COMPLETED ROLLBACK TEST --------------------------------------------------------------------------------------------"));

	_fTestingRollback = false;
	return fSuccess;
}
#endif
