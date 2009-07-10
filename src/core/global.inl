/*******************************************************************************
 * \file	global.inl
 * \brief	Stylus global inline methods
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

inline string& stylus::terminatePath(std::string& strPath)
{
	ASSERT(!EMPTYSTR(strPath));
	if (strPath[strPath.length()-1] != Constants::s_chFORWARDSLASH)
		strPath += Constants::s_chFORWARDSLASH;
	return strPath;
}

//--------------------------------------------------------------------------------
//
// Global
//
//--------------------------------------------------------------------------------
inline bool Globals::isInitialized() { return _fInitialized; }
inline void Globals::setInitialized(bool fInitialized) { _fInitialized = fInitialized; }

inline bool Globals::isSupplied() { return _fSupplied; }

inline bool Globals::logIf(ST_LOGLEVEL ll) throw() { return (_ll >= ll); }
inline bool Globals::logIfRate(ST_LOGLEVEL ll, size_t iTrial) throw() { return (_ll >= ll && ((iTrial % _cLogRate) == 0)); }

inline void Globals::setLogLevel(ST_LOGLEVEL ll) { _ll = ll; }
inline ST_LOGLEVEL Globals::getLogLevel() { return _ll; }

inline void Globals::setLogOptions(STFLAGS grfLogOptions) { _grfLogOptions = grfLogOptions; }
inline STFLAGS Globals::getLogOptions() { return _grfLogOptions; }

inline void Globals::setLogRate(size_t cLogRate) { _cLogRate = cLogRate; }
inline size_t Globals::getLogRate() { return _cLogRate; }

inline void Globals::setTraceRegions(STFLAGS grfTR) { _grfTR = grfTR; }
inline STFLAGS Globals::getTraceRegions() { return _grfTR; }

inline void Globals::setTraceLevel(ST_TRACECATEGORY tc, ST_TRACELEVEL tl) { _tl[tc] = tl; }
inline ST_TRACELEVEL Globals::getTraceLevel(ST_TRACECATEGORY tc) { return _tl[tc]; }

inline size_t Globals::getTraceIndents() { return _cTraceindents; }

inline void Globals::setTraceTrial(size_t iTrial) { _iTrialTrace = iTrial; enableTracing(Genome::getTrial(), Genome::getTrialAttempts()); }
inline size_t Globals::getTraceTrial() { return _iTrialTrace; }
inline void Globals::setTraceAttempt(size_t iAttempt) { _iAttemptTrace = iAttempt; enableTracing(Genome::getTrial(), Genome::getTrialAttempts()); }
inline size_t Globals::getTraceAttempt() { return _iAttemptTrace; }
inline void Globals::enableTracing(size_t cTrial, size_t cAttempts) { _fAtTraceTrialOrAttempt = (cTrial >= _iTrialTrace) || (cAttempts >= _iAttemptTrace); }

inline bool Globals::traceIf(ST_TRACEREGION tr, ST_TRACECATEGORY tc, ST_TRACELEVEL tl) throw()
{
	return (	_fAtTraceTrialOrAttempt
		   &&	(_grfTR & tr)
		   &&	_tl[tc] >= tl
		   );
}

inline void Globals::traceIn() throw() { _cTraceindents++; }
inline void Globals::traceOut() throw() { if (_cTraceindents > 0) _cTraceindents--; }

inline bool Globals::isGroupScoreMode(GROUPSCOREMODE gsm) { ASSERT(gsm < GSM_MAX); return (_gsm == gsm); }

inline UNIT Globals::getGenomeWeight(SCORECOMPONENT sc) { ASSERT(sc < SC_GENOMEMAX); return _aryGenomeWeights[sc]; }

inline UNIT Globals::getGeneWeight(SCORECOMPONENT sc) { ASSERT(sc < SC_GENEMAX); return _aryGeneWeights[sc]; }
inline UNIT Globals::getGeneSetpoint(SCORECOMPONENT sc) { ASSERT(sc < SC_GENEMAX); return _aryGeneSetpoints[sc]; }

inline UNIT Globals::getGroupWeight(SCORECOMPONENT sc) { ASSERT(sc < SC_GROUPMAX); return _aryGroupWeights[sc]; }
inline UNIT Globals::getGroupSetpoint(SCORECOMPONENT sc) { ASSERT(sc < SC_GROUPMAX); return _aryGroupSetpoints[sc]; }

//--------------------------------------------------------------------------------
//
// CFlow
//
//--------------------------------------------------------------------------------
inline CFlow::CFlow(ST_TRACEREGION tr, const char* pszFileline, const char* pszWhere) throw() :
	_tr(tr), _pszFileline(pszFileline), _pszWhere(pszWhere)
{
	if (org_biologicinstitute_stylus::Globals::traceIf(_tr, STTC_FLOW, STTL_L3) && org_biologicinstitute_stylus::Globals::logIf(STLL_TRACE))
	{
		org_biologicinstitute_stylus::Globals::log(STLL_TRACE, _pszFileline, Constants::s_strENTER.c_str(), _pszWhere);
		org_biologicinstitute_stylus::Globals::traceIn();
	}
}

inline CFlow::~CFlow() throw()
{
	if (org_biologicinstitute_stylus::Globals::traceIf(_tr, STTC_FLOW, STTL_L3) && org_biologicinstitute_stylus::Globals::logIf(STLL_TRACE))
	{
		org_biologicinstitute_stylus::Globals::traceOut();
		org_biologicinstitute_stylus::Globals::log(STLL_TRACE, _pszFileline, Constants::s_strEXIT.c_str(), _pszWhere);
	}
}

#ifdef ST_TRACE
//--------------------------------------------------------------------------------
//
// TIndent
//
//--------------------------------------------------------------------------------
inline TIndent::TIndent(const char* pszScope) throw() : _pszScope(pszScope) { org_biologicinstitute_stylus::Globals::traceIn(); }
inline TIndent::~TIndent() throw() { org_biologicinstitute_stylus::Globals::traceOut(); }
#endif

