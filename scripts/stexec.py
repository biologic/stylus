#!/usr/bin/env python
# encoding: utf-8
# 
# Stylus, Copyright 2006-2009 Biologic Institute
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
'''
stexec.py

This script is the Python front-end for running Stylus experiments.

Stylus, Copyright 2006-2008 Biologic Institute.
'''

import getopt
import os
import random
import re
import shutil
import stylus.common as Common
import stylus.xmldict as XMLDict
import stylusengine as Stylus
import sys
import time
import urllib2
import urlparse

#==============================================================================
# Globals
#==============================================================================
_FD_STDIN = 0
_FD_STDOUT = 1
_FD_STDERR = 2

class Constants:
    optLogLevels = Stylus.getLogLevels()
    optRecordDetails = Stylus.getRecordDetails()
    optStatistics = [ 'summary', 'all' ]
    optTraceRegions = Stylus.getTraceRegions()

class Globals:
    names = Common.Names()
    urls = Common.URLs()
    urlGlobals = ''
    aryRecord = [ 500, ['all'], True ]
    aryLog = [ 500, 'info', '', '', False ]
    aryStatistics = []
    aryTrace = []
    strAuthor = ''
    fVersion = False

#==============================================================================
# Classes
#==============================================================================
#------------------------------------------------------------------------------
# Class: Usage
#
#------------------------------------------------------------------------------
class Usage(Common.BiologicError):
    __strHelpMessage = '''
\t[(-d|--datapath) <data output path>] - The path for experiment data
\t[(-n|--name) <experiment name>[,asis|dir|file|unicode|uuid][,plan]] - Optional experiment name
\t[(-e|--expand) <trial number>[,repeat]] - Trial to expand

\t[[-u|--urls] [<Genome URL>][,<Han URL>][,<HTML URL>][,<Plan URL>][,<XML Schema URL>]] - Set the URLs for obtaining files

\t[[-g|--genome] <genome path/url>] - Genome to load
\t[[-p|--plan] <plan path/url>[,<number of trials to execute>[,<first trial associated with plan>]]] - Plan to execute
\t[[-c|--constants] <constants path/url>] - Optional global constants to load

\t[[-f|--frequency] <record rate>[,(%s)+][,[true|false]]] - Set record rate, detail, and history recording
\t[[-l|--log] <rate>[,(%s)][,<log file path>[,echo|silent]][,long|short] - Set log rate and output level
\t[[-s|--statistics] (%s)[,time]]
\t[[-t|--trace] (%s)+,[<flow trace level (1-5)>][,<data trace level (1-5)>[,<trial at which to start tracing>[,<attempt at which to start tracing>]]]]

\t[-a|--author] - Experiment author
\t[-q|--quiet] - Silence all output (except Stylus log and trace messages)
\t[-v|--version] - Print Stylus version
\t[-h|--help] - Print this help

Related environment variables:
STYLUS_EXECARGS  - The default argument string to parse (command-line overrides duplicate arguments)

STYLUS_DATAPATH  - The default path for experiments (to which non-absolute experiment names are appended)

STYLUS_GENOMEURL - The default URL from which to obtain genome files
STYLUS_HANURL    - The URL from which to obtain Han definition files
STYLUS_PLANURL   - The default URL from which to obtain plan files
STYLUS_SCHEMAURL - The URL from which to obtain XML schemas

STYLUS_AUTHOR    - Experiment author

See Stylus documentation for more details.
'''
    def __init__(self, msg):
        self.msg = ''
        if msg and len(msg) > 0:
            self.msg = 'Error: ' + msg + '\n'
        self.msg += 'Usage: ' + sys.argv[0].split("/")[-1] + '\n' + (self.__strHelpMessage % ('+'.join(Constants.optRecordDetails),
                                                                                            '+'.join(Constants.optLogLevels),
                                                                                            '+'.join(Constants.optStatistics),
                                                                                            '+'.join(Constants.optTraceRegions)))
    
    def __str__(self):
        return self.msg
        
#------------------------------------------------------------------------------
# Function: stylusError
# 
#------------------------------------------------------------------------------
def stylusError(rc):
    ga = Stylus.getTermination()
    if rc != Stylus.ST_RCSUCCESS and ga != Stylus.STGT_NONE:
        return ('Stylus Terminated: action(0x%X) reason(0x%X) description(%s)') % (Stylus.getTermination(), Stylus.getTerminationReason(), Stylus.getTerminationDescription())
    else:
        return ('Stylus Error: ' + Stylus.errorToString(rc))

#------------------------------------------------------------------------------
# Class: StylusError
#
#------------------------------------------------------------------------------
class StylusError(Common.BiologicError):
    def __init__(self,rc):
        self.rc = rc
        self.termination = Stylus.getTermination()
        self.reason = Stylus.getTerminationReason()
        self.terminationDescription = Stylus.getTerminationDescription()
        self.msg = stylusError(rc)
    
    def __str__(self):
        return self.msg

#==============================================================================
# Helpers
#==============================================================================
#------------------------------------------------------------------------------
# Function: getArguments
#
# Read and process all incoming arguments
#------------------------------------------------------------------------------
def getArguments():
    # Start with command-line arguments and append those passed via the environment
    # (The command-line overrides the environment)
    args = Common.readEnvironment('$STYLUS_EXECARGS')
    argv = args and args.split() or []
    argv += sys.argv[1:]

    Globals.strAuthor = Common.readEnvironment('$STYLUS_AUTHOR')

    try:
        opts, remaining = getopt.getopt(argv, Common.Constants.strShortArgs, Common.Constants.aryLongArgs)
        if len(remaining) > 0:
            remaining[0].strip()
            if len(remaining) > 1 or remaining[0]:
                raise Usage(' '.join(remaining) + ' contains unexpected arguments')
    except getopt.GetoptError, err:
        raise Usage(' '.join(argv[1:]) + ' contains unknown arguments')

    for option, value in opts:
        try:
            if option in ('-d', '--datapath'):
                Globals.names.set(Common.Names.DATA, value)

            if option in ('-n', '--name'):
                Globals.names.set(Common.Names.NAME, value)

            if option in ('-e', '--expand'):
                Globals.names.set(Common.Names.EXPAND, value)

            if option in ('-u', '--urls'):
                Globals.urls.set(value)

            if option in ('-g', '--genome'):
                Globals.names.set(Common.Names.GENOME, value)
                
            if option in ('-p', '--plan'):
                Globals.names.set(Common.Names.PLAN, value)
        except Common.BiologicError, err:
            raise Usage(str(err))

        if option in ('-c', '--constants'):
            Globals.urlGlobals = value

        if option in ('-f', '--frequency'):
            aryArg = value.split(',')
            if len(aryArg) < 1 or len(aryArg) > 3:
                raise Usage('frequency requires one to three arguments')
            Globals.aryRecord[0] = Common.ensureInteger(aryArg[0], Globals.aryRecord[0], 'frequency requires an integer rate')
            if len(aryArg) > 1 and aryArg[1]:
                Globals.aryRecord[1] = aryArg[1].split('+')
                for value in Globals.aryRecord[1]:
                    if not value in Constants.optRecordDetails:
                        raise Usage(value + ' is an illegal value for frequency')
            if len(aryArg) > 2 and aryArg[2]:
                if aryArg[2] != 'true' and aryArg[2] != 'false':
                    raise Usage(aryArg[2] + ' is an illegal frequency option')
                Globals.aryRecord[2] = (aryArg[2] == 'true')

        if option in ('-l', '--log'):
            aryArg = value.split(',')
            if len(aryArg) < 1 or len(aryArg) > 5:
                raise Usage('log requires between one and five values')
            Globals.aryLog[0] = Common.ensureInteger(aryArg[0], Globals.aryLog[0], 'log requires an integer for rate')
            if len(aryArg) > 1 and aryArg[1]:
                if not aryArg[1] in Constants.optLogLevels:
                    raise Usage(aryArg[1] + ' is an illegal log level')
                Globals.aryLog[1] = aryArg[1]
            if len(aryArg) > 2 and aryArg[2]:
                Globals.aryLog[2] = aryArg[2]
            if len(aryArg) > 3 and aryArg[3]:
                if aryArg[3] == 'echo':
                    if not Globals.aryLog[2]:
                        raise Usage("log 'echo' requires specifying a log file")
                elif aryArg[3] != 'silent':
                    raise Usage(aryArg[3] + ' is an illegal output modifier')
                Globals.aryLog[3] = aryArg[3]
            if len(aryArg) > 4 and aryArg[4]:
                if aryArg[4] == 'long':
                    Globals.aryLog[4] = True
                elif aryArg[4] == 'short':
                    Globals.aryLog[4] = False
                else:
                    raise Usage(aryArg[4] + ' is an illegal log format option')
                
        if option in ('-s', '--statistics'):
            if not Globals.aryStatistics:
                Globals.aryStatistics = [ "none", False ]
            aryArgs = value.split(',')
            if len(aryArgs) < 1 or len(aryArgs) > 2:
                raise Usage('statistics requires one or two arguments')
            if aryArgs[0]:
                if not aryArgs[0] in Constants.optStatistics:
                    raise Usage(aryArgs[0] + ' is an illegal value for statistics')
                Globals.aryStatistics[0] = aryArgs[0]
            if len(aryArgs) > 1 and aryArgs[1]:
                if aryArgs[1] != 'time':
                    raise Usage(aryArgs[1] + ' is an illegal value for statistics')
                Globals.aryStatistics[1] = True

        if option in ('-t', '--trace'):
            if not Globals.aryTrace:
                Globals.aryTrace = [ '', 2, 0, sys.maxint, sys.maxint ]
            aryArgs = value.split(',')
            if len(aryArgs) < 1 or len(aryArgs) > 5:
                raise Usage('trace requires one to five arguments')
            Globals.aryTrace[0] = aryArgs[0].split('+')
            for value in Globals.aryTrace[0]:
                if not value in Constants.optTraceRegions:
                    raise Usage(value + ' is an illegal value for trace')
            if len(aryArgs) > 1:
                Globals.aryTrace[1] = Common.ensureInteger(aryArgs[1], Globals.aryTrace[1], 'trace requires an integer for flow level')
            if len(aryArgs) > 2:
                Globals.aryTrace[2] = Common.ensureInteger(aryArgs[2], Globals.aryTrace[2], 'trace requires an integer for data level')
            if len(aryArgs) > 3:
                Globals.aryTrace[3] = Common.ensureInteger(aryArgs[3], Globals.aryTrace[3], 'trace requires an integer for the trace trial')
            if len(aryArgs) > 4:
                Globals.aryTrace[4] = Common.ensureInteger(aryArgs[4], Globals.aryTrace[4], 'trace requires an integer for the trace attempt')
            if Globals.aryTrace[3] == sys.maxint and Globals.aryTrace[4] == sys.maxint:
                Globals.aryTrace[3] = 0
                Globals.aryTrace[4] = 0

        if option in ('-a', '--author'):
            Globals.strAuthor = value

        if option in ('-q', '--quiet'):
            Common.Globals.fQuiet = True

        if option in ('-v', '--version'):
            Globals.fVersion = True

        if option in ('-h', '--help'):
            raise Usage('')

    try:
        Globals.urls.validate(Common.URLs.HAN | Common.URLs.SCHEMA)
        Globals.names.validate(Globals.urls, Common.Names.DATA | Common.Names.GENOME | Common.Names.PLAN)
    except Common.BiologicError, err:
        raise Usage(str(err))

    if Globals.urlGlobals:
        Globals.urlGlobals = Common.pathToURL(Globals.urlGlobals, Globals.urls.urlPlan)

    Common.replacePath(Globals.names.pathData)

    if Globals.aryLog[2]:
        if not os.path.isabs(Globals.aryLog[2]):
            Globals.aryLog[2] = os.path.abspath(os.path.join(Globals.names.pathData, Globals.aryLog[2]))
        Globals.aryLog[2] = os.path.normpath(os.path.normcase(Globals.aryLog[2]))
        
#------------------------------------------------------------------------------
# Function: getUUIDSeeds
#
# Generate seeds for UUID generation
#------------------------------------------------------------------------------
def getUUIDSeeds():
    # Get randomness from urandom or the 'random' module.
    intbytecount = 1

    while long(''.join(['7f'] + ['ff' for i in range(intbytecount)]), 16) <= sys.maxint:
        intbytecount += 1

    try:
        seeds = [os.urandom(intbytecount) for i in range(2)]
    except:
        seeds = [''.join([chr(random.randrange(256)) for j in range(intbytecount)]) for i in range(2)]

    acc1 = -sys.maxint - 1
    acc2 = -sys.maxint - 1

    mult = 1

    for i in range(intbytecount):
        acc1 += ord(seeds[0][i]) * mult
        mult *= 256

    mult = 1

    for i in range(intbytecount):
        acc2 += ord(seeds[1][i]) * mult
        mult *= 256

    if acc1 < 0:
        acc1 += sys.maxint
        
    if acc1 > ((sys.maxint + 1) / 2):
        acc1 /= 2

    if acc2 < 0:
        acc2 += sys.maxint

    if acc2 > ((sys.maxint + 1) / 2):
        acc2 /= 2

    return '%d %d' % (acc1 , acc2)
        
#==============================================================================
# Main
#==============================================================================
def main(argv=None):
    try:
        nCompletionCode = 0

        getArguments()
        
        Common.say('Data Directory: %s' % Globals.names.pathData)
        if Globals.urlGlobals:
            Common.say('Globals URL   : %s' % Globals.urlGlobals)
        Common.say('Genome URL    : %s' % Globals.names.urlGenome)
        Common.say('Plan URL      : %s' % Globals.names.urlPlan)
        Common.say('Han URL       : %s' % Globals.urls.urlHan)
        Common.say('Schema URL    : %s' % Globals.urls.urlSchema)
        
        if Globals.aryLog[2] or Globals.aryLog[3] == 'silent':
            os.close(_FD_STDERR)
            if Globals.aryLog[3] != 'silent':
                Common.redirectStderr(Globals.aryLog[2])

        if Globals.fVersion:
            Common.say('Version       : %s' % Stylus.getVersion())

        strGenome = Common.readFile(Globals.names.urlGenome)
        strPlan = Common.readFile(Globals.names.urlPlan)
        strGlobals = Globals.urlGlobals and Common.readFile(Globals.urlGlobals) or ''
    
        if Globals.aryTrace:
            Stylus.setLogLevel(Globals.aryLog[0], 'trace', Globals.aryLog[3] == 'echo', Globals.aryLog[4])
            rc = Stylus.setTrace(Globals.aryTrace[0], Globals.aryTrace[1], Globals.aryTrace[2], Globals.aryTrace[3], Globals.aryTrace[4])
            if rc:
                raise StylusError(rc)

        if Globals.aryLog and not Globals.aryTrace:
            rc = Stylus.setLogLevel(Globals.aryLog[0], Globals.aryLog[1], Globals.aryLog[3] == 'echo', Globals.aryLog[4])
            if rc:
                raise StylusError(rc)

        rc = Stylus.setScope(Globals.urls.urlHan, Globals.urls.urlSchema)
        if rc:
            raise StylusError(rc)
            
        rc = Stylus.setRecordRate(Globals.aryRecord[0], Globals.aryRecord[1], Globals.names.pathData, Globals.aryRecord[2])
        if rc:
            raise StylusError(rc)
            
        if strGlobals:
            rc = Stylus.setGlobals(strGlobals)
            if rc:
                raise StylusError(rc)
            
        rc = Stylus.setGenome(strGenome, Globals.strAuthor)
        if rc:
            raise StylusError(rc)

        seeds = ''.join(getUUIDSeeds())

        rc = Stylus.setUUIDSeeds(seeds)
        if rc:
            raise StylusError(rc)

        tStart = time.clock()
        rc = Stylus.executePlan(strPlan, Globals.names.nFirstPlanTrial, Globals.names.cPlanTrialsToExecute)
        tEnd = time.clock()
        if Globals.aryStatistics and Globals.aryStatistics[1]:
            statistics = Stylus.getStatistics()
            Common.say('Plan executed %d trials in %f seconds' % ((statistics._iTrialCurrent-statistics._iTrialInitial), (tEnd - tStart)))
        if rc != Stylus.ST_RCSUCCESS:
            if not Stylus.errorIsType(rc, Stylus.ST_INFOTYPE):
                raise StylusError(rc)
            else:
                Common.say('Plan terminated - %s' % stylusError(rc))
                nCompletionCode = 1
            
        if Globals.aryStatistics and len(Globals.aryStatistics) >= 1 and Globals.aryStatistics[0] != "none":
            statistics = Stylus.getStatistics()
            Common.say('Statistics for %d trials (%d to %d)' % ((statistics._iTrialCurrent-statistics._iTrialInitial), statistics._iTrialInitial, statistics._iTrialCurrent))
            if Globals.aryStatistics[0] == "summary":
                Common.say('    Fitness: %0.15f - rollbacks(%d) silent(%d) changes(%d) inserted(%d) deleted(%d)' %
                        (statistics._nFitness, statistics._cTotalRollbacks, statistics._cSilent, statistics._cbBasesChanged, statistics._cbBasesInserted, statistics._cbBasesDeleted))
            else:
                Common.say('    Fitness: current(%0.15f) max(%0.15f in trial %d) min(%0.15f in trial %d)' %
                        (statistics._nFitness, statistics._tfMax._nValue, statistics._tfMax._iTrial, statistics._tfMin._nValue, statistics._tfMin._iTrial))
                Common.say('    Score: current(%0.15f) max(%0.15f in trial %d) min(%0.15f in trial %d)' %
                        (statistics._nScore, statistics._tsMax._nValue, statistics._tsMax._iTrial, statistics._tsMin._nValue, statistics._tsMin._iTrial))
                Common.say('    Size: current(%d) max(%d in trial %d) min(%d in trial  %d)' %
                        (statistics._cbBases, statistics._tzMax._cbBases, statistics._tzMax._iTrial, statistics._tzMin._cbBases, statistics._tzMin._iTrial))
                Common.say('    Rollbacks: total(%d) current(%d) max(%d in trial %d) min(%d in trial %d)' %
                        (statistics._cTotalRollbacks, statistics._cRollbacks, statistics._trMax._cRollbacks, statistics._trMax._iTrial, statistics._trMin._cRollbacks, statistics._trMin._iTrial))
                Common.say('    Mutations: silent(%d) attempts(%d) accepted(%d) changed(%d) inserted(%d) deleted(%d)' %
                        (statistics._cSilent, statistics._cAttempted, statistics._cAccepted, statistics._cbBasesChanged, statistics._cbBasesInserted, statistics._cbBasesDeleted))
                if statistics._atChanged._cAttempted:
                    Common.say('      Changed: attempted(%d) accepted(%d) bases-affected(%d)' %
                            (statistics._atChanged._cAttempted, statistics._atChanged._cAccepted, statistics._atChanged._cbBases))
                if statistics._atCopied._cAttempted:
                    Common.say('      Copied: attempted(%d) accepted(%d) bases-affected(%d)' %
                            (statistics._atCopied._cAttempted, statistics._atCopied._cAccepted, statistics._atCopied._cbBases))
                if statistics._atDeleted._cAttempted:
                    Common.say('      Deleted: attempted(%d) accepted(%d) bases-affected(%d)' %
                            (statistics._atDeleted._cAttempted, statistics._atDeleted._cAccepted, statistics._atDeleted._cbBases))
                if statistics._atInserted._cAttempted:
                    Common.say('      Inserted: attempted(%d) accepted(%d) bases-affected(%d)' %
                            (statistics._atInserted._cAttempted, statistics._atInserted._cAccepted, statistics._atInserted._cbBases))
                if statistics._atTransposed._cAttempted:
                    Common.say('      Transposed: attempted(%d) accepted(%d) bases-affected(%d)' %
                            (statistics._atTransposed._cAttempted, statistics._atTransposed._cAccepted, statistics._atTransposed._cbBases))

        return nCompletionCode

    except Usage, err:
        Common.sayError(err)
        return 0

    except Common.BiologicError, err:
        Common.sayError(str(err))
        return 2

if __name__ == "__main__":
    sys.exit(main())
