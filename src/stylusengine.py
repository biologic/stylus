# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.10
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_stylusengine', [dirname(__file__)])
        except ImportError:
            import _stylusengine
            return _stylusengine
        if fp is not None:
            try:
                _mod = imp.load_module('_stylusengine', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _stylusengine = swig_import_helper()
    del swig_import_helper
else:
    import _stylusengine
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0



def stSetGlobals(*args):
  return _stylusengine.stSetGlobals(*args)
stSetGlobals = _stylusengine.stSetGlobals

def stSetTraceAttempt(*args):
  return _stylusengine.stSetTraceAttempt(*args)
stSetTraceAttempt = _stylusengine.stSetTraceAttempt

def stGetTraceAttempt(*args):
  return _stylusengine.stGetTraceAttempt(*args)
stGetTraceAttempt = _stylusengine.stGetTraceAttempt
class ST_TRIALVALUE(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ST_TRIALVALUE, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ST_TRIALVALUE, name)
    __repr__ = _swig_repr
    __swig_getmethods__["_iTrial"] = _stylusengine.ST_TRIALVALUE__iTrial_get
    if _newclass:_iTrial = _swig_property(_stylusengine.ST_TRIALVALUE__iTrial_get)
    __swig_getmethods__["_nValue"] = _stylusengine.ST_TRIALVALUE__nValue_get
    if _newclass:_nValue = _swig_property(_stylusengine.ST_TRIALVALUE__nValue_get)
    def __init__(self): 
        this = _stylusengine.new_ST_TRIALVALUE()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _stylusengine.delete_ST_TRIALVALUE
    __del__ = lambda self : None;
ST_TRIALVALUE_swigregister = _stylusengine.ST_TRIALVALUE_swigregister
ST_TRIALVALUE_swigregister(ST_TRIALVALUE)
cvar = _stylusengine.cvar
ST_ERRORTYPE = cvar.ST_ERRORTYPE
ST_WARNINGTYPE = cvar.ST_WARNINGTYPE
ST_INFOTYPE = cvar.ST_INFOTYPE
ST_RCSUCCESS = cvar.ST_RCSUCCESS
ST_RCERROR = cvar.ST_RCERROR
ST_RCNOTINITIALIZED = cvar.ST_RCNOTINITIALIZED
ST_RCBUFFERTOOSMALL = cvar.ST_RCBUFFERTOOSMALL
ST_RCOUTOFMEMORY = cvar.ST_RCOUTOFMEMORY
ST_RCBADARGUMENTS = cvar.ST_RCBADARGUMENTS
ST_RCINEXECUTION = cvar.ST_RCINEXECUTION
ST_RCXMLERROR = cvar.ST_RCXMLERROR
ST_RCINVALIDSTATE = cvar.ST_RCINVALIDSTATE
ST_RCPLAN = cvar.ST_RCPLAN
ST_RCDEAD = cvar.ST_RCDEAD

class ST_TRIALROLLBACKS(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ST_TRIALROLLBACKS, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ST_TRIALROLLBACKS, name)
    __repr__ = _swig_repr
    __swig_getmethods__["_iTrial"] = _stylusengine.ST_TRIALROLLBACKS__iTrial_get
    if _newclass:_iTrial = _swig_property(_stylusengine.ST_TRIALROLLBACKS__iTrial_get)
    __swig_getmethods__["_cRollbacks"] = _stylusengine.ST_TRIALROLLBACKS__cRollbacks_get
    if _newclass:_cRollbacks = _swig_property(_stylusengine.ST_TRIALROLLBACKS__cRollbacks_get)
    def __init__(self): 
        this = _stylusengine.new_ST_TRIALROLLBACKS()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _stylusengine.delete_ST_TRIALROLLBACKS
    __del__ = lambda self : None;
ST_TRIALROLLBACKS_swigregister = _stylusengine.ST_TRIALROLLBACKS_swigregister
ST_TRIALROLLBACKS_swigregister(ST_TRIALROLLBACKS)

class ST_TRIALSIZE(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ST_TRIALSIZE, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ST_TRIALSIZE, name)
    __repr__ = _swig_repr
    __swig_getmethods__["_iTrial"] = _stylusengine.ST_TRIALSIZE__iTrial_get
    if _newclass:_iTrial = _swig_property(_stylusengine.ST_TRIALSIZE__iTrial_get)
    __swig_getmethods__["_cbBases"] = _stylusengine.ST_TRIALSIZE__cbBases_get
    if _newclass:_cbBases = _swig_property(_stylusengine.ST_TRIALSIZE__cbBases_get)
    def __init__(self): 
        this = _stylusengine.new_ST_TRIALSIZE()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _stylusengine.delete_ST_TRIALSIZE
    __del__ = lambda self : None;
ST_TRIALSIZE_swigregister = _stylusengine.ST_TRIALSIZE_swigregister
ST_TRIALSIZE_swigregister(ST_TRIALSIZE)

class ST_ATTEMPTS(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ST_ATTEMPTS, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ST_ATTEMPTS, name)
    __repr__ = _swig_repr
    __swig_getmethods__["_cConsidered"] = _stylusengine.ST_ATTEMPTS__cConsidered_get
    if _newclass:_cConsidered = _swig_property(_stylusengine.ST_ATTEMPTS__cConsidered_get)
    __swig_getmethods__["_cAttempted"] = _stylusengine.ST_ATTEMPTS__cAttempted_get
    if _newclass:_cAttempted = _swig_property(_stylusengine.ST_ATTEMPTS__cAttempted_get)
    __swig_getmethods__["_cAccepted"] = _stylusengine.ST_ATTEMPTS__cAccepted_get
    if _newclass:_cAccepted = _swig_property(_stylusengine.ST_ATTEMPTS__cAccepted_get)
    __swig_getmethods__["_cbBases"] = _stylusengine.ST_ATTEMPTS__cbBases_get
    if _newclass:_cbBases = _swig_property(_stylusengine.ST_ATTEMPTS__cbBases_get)
    def __init__(self): 
        this = _stylusengine.new_ST_ATTEMPTS()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _stylusengine.delete_ST_ATTEMPTS
    __del__ = lambda self : None;
ST_ATTEMPTS_swigregister = _stylusengine.ST_ATTEMPTS_swigregister
ST_ATTEMPTS_swigregister(ST_ATTEMPTS)

class ST_STATISTICS(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ST_STATISTICS, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ST_STATISTICS, name)
    __repr__ = _swig_repr
    __swig_getmethods__["_iTrialInitial"] = _stylusengine.ST_STATISTICS__iTrialInitial_get
    if _newclass:_iTrialInitial = _swig_property(_stylusengine.ST_STATISTICS__iTrialInitial_get)
    __swig_getmethods__["_iTrialCurrent"] = _stylusengine.ST_STATISTICS__iTrialCurrent_get
    if _newclass:_iTrialCurrent = _swig_property(_stylusengine.ST_STATISTICS__iTrialCurrent_get)
    __swig_getmethods__["_cTrialAttempts"] = _stylusengine.ST_STATISTICS__cTrialAttempts_get
    if _newclass:_cTrialAttempts = _swig_property(_stylusengine.ST_STATISTICS__cTrialAttempts_get)
    __swig_getmethods__["_nScore"] = _stylusengine.ST_STATISTICS__nScore_get
    if _newclass:_nScore = _swig_property(_stylusengine.ST_STATISTICS__nScore_get)
    __swig_getmethods__["_nUnits"] = _stylusengine.ST_STATISTICS__nUnits_get
    if _newclass:_nUnits = _swig_property(_stylusengine.ST_STATISTICS__nUnits_get)
    __swig_getmethods__["_nCost"] = _stylusengine.ST_STATISTICS__nCost_get
    if _newclass:_nCost = _swig_property(_stylusengine.ST_STATISTICS__nCost_get)
    __swig_getmethods__["_nFitness"] = _stylusengine.ST_STATISTICS__nFitness_get
    if _newclass:_nFitness = _swig_property(_stylusengine.ST_STATISTICS__nFitness_get)
    __swig_getmethods__["_cRollbacks"] = _stylusengine.ST_STATISTICS__cRollbacks_get
    if _newclass:_cRollbacks = _swig_property(_stylusengine.ST_STATISTICS__cRollbacks_get)
    __swig_getmethods__["_cbBases"] = _stylusengine.ST_STATISTICS__cbBases_get
    if _newclass:_cbBases = _swig_property(_stylusengine.ST_STATISTICS__cbBases_get)
    __swig_getmethods__["_tsMax"] = _stylusengine.ST_STATISTICS__tsMax_get
    if _newclass:_tsMax = _swig_property(_stylusengine.ST_STATISTICS__tsMax_get)
    __swig_getmethods__["_tsMin"] = _stylusengine.ST_STATISTICS__tsMin_get
    if _newclass:_tsMin = _swig_property(_stylusengine.ST_STATISTICS__tsMin_get)
    __swig_getmethods__["_tuMax"] = _stylusengine.ST_STATISTICS__tuMax_get
    if _newclass:_tuMax = _swig_property(_stylusengine.ST_STATISTICS__tuMax_get)
    __swig_getmethods__["_tuMin"] = _stylusengine.ST_STATISTICS__tuMin_get
    if _newclass:_tuMin = _swig_property(_stylusengine.ST_STATISTICS__tuMin_get)
    __swig_getmethods__["_tcMax"] = _stylusengine.ST_STATISTICS__tcMax_get
    if _newclass:_tcMax = _swig_property(_stylusengine.ST_STATISTICS__tcMax_get)
    __swig_getmethods__["_tcMin"] = _stylusengine.ST_STATISTICS__tcMin_get
    if _newclass:_tcMin = _swig_property(_stylusengine.ST_STATISTICS__tcMin_get)
    __swig_getmethods__["_tfMax"] = _stylusengine.ST_STATISTICS__tfMax_get
    if _newclass:_tfMax = _swig_property(_stylusengine.ST_STATISTICS__tfMax_get)
    __swig_getmethods__["_tfMin"] = _stylusengine.ST_STATISTICS__tfMin_get
    if _newclass:_tfMin = _swig_property(_stylusengine.ST_STATISTICS__tfMin_get)
    __swig_getmethods__["_cTotalRollbacks"] = _stylusengine.ST_STATISTICS__cTotalRollbacks_get
    if _newclass:_cTotalRollbacks = _swig_property(_stylusengine.ST_STATISTICS__cTotalRollbacks_get)
    __swig_getmethods__["_trMax"] = _stylusengine.ST_STATISTICS__trMax_get
    if _newclass:_trMax = _swig_property(_stylusengine.ST_STATISTICS__trMax_get)
    __swig_getmethods__["_trMin"] = _stylusengine.ST_STATISTICS__trMin_get
    if _newclass:_trMin = _swig_property(_stylusengine.ST_STATISTICS__trMin_get)
    __swig_getmethods__["_tzMax"] = _stylusengine.ST_STATISTICS__tzMax_get
    if _newclass:_tzMax = _swig_property(_stylusengine.ST_STATISTICS__tzMax_get)
    __swig_getmethods__["_tzMin"] = _stylusengine.ST_STATISTICS__tzMin_get
    if _newclass:_tzMin = _swig_property(_stylusengine.ST_STATISTICS__tzMin_get)
    __swig_getmethods__["_cbBasesChanged"] = _stylusengine.ST_STATISTICS__cbBasesChanged_get
    if _newclass:_cbBasesChanged = _swig_property(_stylusengine.ST_STATISTICS__cbBasesChanged_get)
    __swig_getmethods__["_cbBasesInserted"] = _stylusengine.ST_STATISTICS__cbBasesInserted_get
    if _newclass:_cbBasesInserted = _swig_property(_stylusengine.ST_STATISTICS__cbBasesInserted_get)
    __swig_getmethods__["_cbBasesDeleted"] = _stylusengine.ST_STATISTICS__cbBasesDeleted_get
    if _newclass:_cbBasesDeleted = _swig_property(_stylusengine.ST_STATISTICS__cbBasesDeleted_get)
    __swig_getmethods__["_cSilent"] = _stylusengine.ST_STATISTICS__cSilent_get
    if _newclass:_cSilent = _swig_property(_stylusengine.ST_STATISTICS__cSilent_get)
    __swig_getmethods__["_cAttempted"] = _stylusengine.ST_STATISTICS__cAttempted_get
    if _newclass:_cAttempted = _swig_property(_stylusengine.ST_STATISTICS__cAttempted_get)
    __swig_getmethods__["_cConsidered"] = _stylusengine.ST_STATISTICS__cConsidered_get
    if _newclass:_cConsidered = _swig_property(_stylusengine.ST_STATISTICS__cConsidered_get)
    __swig_getmethods__["_cAccepted"] = _stylusengine.ST_STATISTICS__cAccepted_get
    if _newclass:_cAccepted = _swig_property(_stylusengine.ST_STATISTICS__cAccepted_get)
    __swig_getmethods__["_atChanged"] = _stylusengine.ST_STATISTICS__atChanged_get
    if _newclass:_atChanged = _swig_property(_stylusengine.ST_STATISTICS__atChanged_get)
    __swig_getmethods__["_atCopied"] = _stylusengine.ST_STATISTICS__atCopied_get
    if _newclass:_atCopied = _swig_property(_stylusengine.ST_STATISTICS__atCopied_get)
    __swig_getmethods__["_atDeleted"] = _stylusengine.ST_STATISTICS__atDeleted_get
    if _newclass:_atDeleted = _swig_property(_stylusengine.ST_STATISTICS__atDeleted_get)
    __swig_getmethods__["_atInserted"] = _stylusengine.ST_STATISTICS__atInserted_get
    if _newclass:_atInserted = _swig_property(_stylusengine.ST_STATISTICS__atInserted_get)
    __swig_getmethods__["_atTransposed"] = _stylusengine.ST_STATISTICS__atTransposed_get
    if _newclass:_atTransposed = _swig_property(_stylusengine.ST_STATISTICS__atTransposed_get)
    def __init__(self): 
        this = _stylusengine.new_ST_STATISTICS()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _stylusengine.delete_ST_STATISTICS
    __del__ = lambda self : None;
ST_STATISTICS_swigregister = _stylusengine.ST_STATISTICS_swigregister
ST_STATISTICS_swigregister(ST_STATISTICS)

STGS_ALIVE = _stylusengine.STGS_ALIVE
STGS_COMPILED = _stylusengine.STGS_COMPILED
STGS_COMPILING = _stylusengine.STGS_COMPILING
STGS_DEAD = _stylusengine.STGS_DEAD
STGS_INVALID = _stylusengine.STGS_INVALID
STGS_LOADING = _stylusengine.STGS_LOADING
STGS_MUTATING = _stylusengine.STGS_MUTATING
STGS_RECORDING = _stylusengine.STGS_RECORDING
STGS_ROLLBACK = _stylusengine.STGS_ROLLBACK
STGS_RESTORING = _stylusengine.STGS_RESTORING
STGS_SCORED = _stylusengine.STGS_SCORED
STGS_SCORING = _stylusengine.STGS_SCORING
STGS_SPAWNING = _stylusengine.STGS_SPAWNING
STGS_VALIDATED = _stylusengine.STGS_VALIDATED
STGS_VALIDATING = _stylusengine.STGS_VALIDATING
STGS_MAX = _stylusengine.STGS_MAX
STGT_NONE = _stylusengine.STGT_NONE
STGT_COMPILATION = _stylusengine.STGT_COMPILATION
STGT_VALIDATION = _stylusengine.STGT_VALIDATION
STGT_SCORE = _stylusengine.STGT_SCORE
STGT_MUTATION = _stylusengine.STGT_MUTATION
STGT_DURATION = _stylusengine.STGT_DURATION
STGT_ROLLBACK = _stylusengine.STGT_ROLLBACK
STGT_FITNESS = _stylusengine.STGT_FITNESS
STGT_CALLBACK = _stylusengine.STGT_CALLBACK
STGR_NONE = _stylusengine.STGR_NONE
STGR_STROKES = _stylusengine.STGR_STROKES
STGR_MEASUREMENT = _stylusengine.STGR_MEASUREMENT
STGR_COST = _stylusengine.STGR_COST
STGR_FITNESS = _stylusengine.STGR_FITNESS
STGR_SCORE = _stylusengine.STGR_SCORE
STGR_STROKE = _stylusengine.STGR_STROKE
STGR_CHANGE = _stylusengine.STGR_CHANGE
STGR_COPY = _stylusengine.STGR_COPY
STGR_DELETE = _stylusengine.STGR_DELETE
STGR_INSERT = _stylusengine.STGR_INSERT
STGR_TRANSPOSE = _stylusengine.STGR_TRANSPOSE
STGR_TRIALS = _stylusengine.STGR_TRIALS
STGR_ATTEMPTS = _stylusengine.STGR_ATTEMPTS
STGR_LIMIT = _stylusengine.STGR_LIMIT
STGR_MAXIMUM = _stylusengine.STGR_MAXIMUM
STGR_MINIMUM = _stylusengine.STGR_MINIMUM
STGR_TERMINATED = _stylusengine.STGR_TERMINATED

def stGetGenomeTermination(*args):
  return _stylusengine.stGetGenomeTermination(*args)
stGetGenomeTermination = _stylusengine.stGetGenomeTermination

def errorToString(*args):
  return _stylusengine.errorToString(*args)
errorToString = _stylusengine.errorToString

def errorIsType(*args):
  return _stylusengine.errorIsType(*args)
errorIsType = _stylusengine.errorIsType

def getLastError():
  return _stylusengine.getLastError()
getLastError = _stylusengine.getLastError

def getLastErrorDescription():
  return _stylusengine.getLastErrorDescription()
getLastErrorDescription = _stylusengine.getLastErrorDescription

def getLastErrorCode():
  return _stylusengine.getLastErrorCode()
getLastErrorCode = _stylusengine.getLastErrorCode

def getVersion():
  return _stylusengine.getVersion()
getVersion = _stylusengine.getVersion

def setGlobals(*args):
  return _stylusengine.setGlobals(*args)
setGlobals = _stylusengine.setGlobals

def setScope(*args):
  return _stylusengine.setScope(*args)
setScope = _stylusengine.setScope

def getLogLevels():
  return _stylusengine.getLogLevels()
getLogLevels = _stylusengine.getLogLevels

def setLogLevel(*args):
  return _stylusengine.setLogLevel(*args)
setLogLevel = _stylusengine.setLogLevel

def setLogFile(*args):
  return _stylusengine.setLogFile(*args)
setLogFile = _stylusengine.setLogFile

def getTraceRegions():
  return _stylusengine.getTraceRegions()
getTraceRegions = _stylusengine.getTraceRegions

def setTrace(*args):
  return _stylusengine.setTrace(*args)
setTrace = _stylusengine.setTrace

def getRecordDetails():
  return _stylusengine.getRecordDetails()
getRecordDetails = _stylusengine.getRecordDetails

def setRecordRate(*args):
  return _stylusengine.setRecordRate(*args)
setRecordRate = _stylusengine.setRecordRate

def setGenome(*args):
  return _stylusengine.setGenome(*args)
setGenome = _stylusengine.setGenome

def getGenome(*args):
  return _stylusengine.getGenome(*args)
getGenome = _stylusengine.getGenome

def getGenomeBases():
  return _stylusengine.getGenomeBases()
getGenomeBases = _stylusengine.getGenomeBases

def executePlan(*args):
  return _stylusengine.executePlan(*args)
executePlan = _stylusengine.executePlan
class STATISTICS(ST_STATISTICS):
    __swig_setmethods__ = {}
    for _s in [ST_STATISTICS]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, STATISTICS, name, value)
    __swig_getmethods__ = {}
    for _s in [ST_STATISTICS]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, STATISTICS, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _stylusengine.new_STATISTICS()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _stylusengine.delete_STATISTICS
    __del__ = lambda self : None;
STATISTICS_swigregister = _stylusengine.STATISTICS_swigregister
STATISTICS_swigregister(STATISTICS)


def getStatistics():
  return _stylusengine.getStatistics()
getStatistics = _stylusengine.getStatistics

def getState():
  return _stylusengine.getState()
getState = _stylusengine.getState

def getTermination():
  return _stylusengine.getTermination()
getTermination = _stylusengine.getTermination

def getTerminationReason():
  return _stylusengine.getTerminationReason()
getTerminationReason = _stylusengine.getTerminationReason

def getTerminationDescription():
  return _stylusengine.getTerminationDescription()
getTerminationDescription = _stylusengine.getTerminationDescription
# This file is compatible with both classic and new-style classes.


