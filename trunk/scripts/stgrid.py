#!/usr/bin/env python
# encoding: utf-8
# 
# Stylus, Copyright 2006-2008 Biologic Institute
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
stgrid.py

This script launches one or more instances of the underlying execution and report scripts.

Stylus, Copyright 2006-2008 Biologic Institute.
'''

import datetime
import fnmatch
import getopt
import os
import os.path
import stylus.common as Common
import sys
import tempfile

#==============================================================================
# Global Constants
#==============================================================================
_STYLUS_ENVIRONMENT = [
            'STYLUS_EXECARGS',
            'STYLUS_RPTARGS',
            'STYLUS_AUTHOR'
            ]
            
_SCRIPT_EXEC = 1
_SCRIPT_REPORT = 2

_XGRID_JOB = '''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
    <array>
        <dict>
            <key>name</key>
            <string>%s</string>
                    
            %s

            <key>taskSpecifications</key>
            <dict>
                <key>0</key>
                <dict>
                    <key>command</key>
                    <string>%s</string>
        
                    <key>arguments</key>
                    <array>
                        <string>-c</string>
                        <string>cd %s &amp;&amp; ./stylus r x %s</string>
                    </array>
        
                    <key>environment</key>
                    <dict>
                        %s
                    </dict>
                </dict>
            </dict>
        </dict>
    </array>
</plist>
'''

_XGRID_NOTIFY = '<key>notificationEmail</key><string>%s</string>'
_XGRID_ENVIRONMENT = '<key>%s</key><string>%s</string>'

#==============================================================================
# Globals
#==============================================================================
class Globals:
    strLocalPath = ''

    aryArgs = []

    fRunExec = False
    fRunReport = False
    aryXgridJob = ''

    fLaunchJobs = False
    strGenomes = ''
    strPlans = ''

    strXgridVolume = ''
    strXgridNotify = ''
    strXgridShell = ''
    strXgridController = ''
    strXgridPassword = ''

    fUseXgrid = False

#==============================================================================
# Classes
#==============================================================================
#------------------------------------------------------------------------------
# Class: Usage
#
#------------------------------------------------------------------------------
class Usage(Common.BiologicError):
    __strHelpMessage = '''
\t[(-e|--exec)] - Run stexec.py
\t[(-r|--report)] - Run strpt.py
\t[[-j|--job] <id>,[restart|results|spec] - Restart or query a previously launched Xgrid job

\t[[-g|--genomes] <genome directory>] - Genomes to load
\t[[-p|--plans] <plan directory>] - Plans to execute

\t[[-x|--xgrid]] - Launch Stylus using Xgrid
\t[[-v|--volume] <Absolute path to Stylus>] - Set the Xgrid client path to Stylus
\t[[-n|--notify] <e-mail address>] - Set the Xgrid notification e-mail address
\t[[-s|--shell] <Absolute path to shell>] - Set the shell within which to launch Stylus on an Xgrid client
\t[[-c|--controller] <Hostname>[,<password>]] - Set the Xgrid controller hostname and password

\t[-h|--help] - Print this help

\t[--] - All arguments following are passed unmodified to the execute scripts

Related environment variables:
STYLUS_GRIDARGS  - The default argument string to parse (command-line overrides duplicate arguments)
STYLUS_EXECARGS  - The default argument string to parse (command-line overrides duplicate arguments)
STYLUS_RPTARGS   - The default argument string to parse (command-line overrides duplicate arguments)

STYLUS_DATAPATH  - The default path for experiments (to which non-absolute experiment names are appended)
STYLUS_RPTPATH   - The default path for experiments (to which non-absolute experiment names are appended)

STYLUS_GENOMEURL - The default URL from which to obtain genome files
STYLUS_HANURL    - The URL from which to obtain Han definition files
STYLUS_HTMLURL   - The URL from which to obtain XHTML-related files (e.g., stylus.css)
STYLUS_PLANURL   - The default URL from which to obtain plan files
STYLUS_SCHEMAURL - The URL from which to obtain XML schemas

STYLUS_AUTHOR    - Experiment author

STYLUS_XGRID_VOLUME - The absolute root path for an Xgrid configuration (e.g., /Volumes/Grid/Biologic)
STYLUS_XGRID_EMAIL  - E-mail address to which to send job completion notification 
STYLUS_XGRID_SHELL  - Shell within which to launch Stylus on an Xgrid client
 
XGRID_CONTROLLER_HOSTNAME - Xgrid to use (IP address or DNS name) (Apple defined variable)
XGRID_CONTROLLER_PASSWORD - Xgrid password to use (Apple defined variable)

The local values of STYLUS_EXECARGS, STYLUS_RPTARGS, and STYLUS_AUTHOR are passed to Xgrid clients.

When launching Xgrid jobs, the script assumes the Stylus code resides within a subdirectory
named 'Stylus' of the supplied Xgrid volume (e.g., /Volumes/Grid/Biologic/Stylus).

See Stylus documentation for more details.
'''
# Note:
# The -l switch is a hidden switch used to force resolution of genome and plan directories (-g and -p)
# on an Xgrid client. This allows -g and -p to reference directories in the same manner as Xgrid clients.
# Users should never pass -l themselves; the script will add it as necessary.
    def __init__(self, msg):
        self.msg = ''
        if msg and len(msg) > 0:
            self.msg = 'Error: ' + msg + '\n'
        self.msg += 'Usage: ' + sys.argv[0].split("/")[-1] + '\n' + self.__strHelpMessage
    
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
    args = Common.readEnvironment('$STYLUS_GRIDARGS')
    argv = args and args.split() or []
    argv += sys.argv[1:]
    
    Globals.strLocalPath = Common.resolvePath(os.path.dirname(sys.argv[0]))
    
    Globals.strXgridVolume = Common.readEnvironment('$STYLUS_XGRID_VOLUME')
    Globals.strXgridNotify = Common.readEnvironment('$STYLUS_XGRID_EMAIL')
    Globals.strXgridShell = Common.readEnvironment('$STYLUS_XGRID_SHELL')
    Globals.strXgridController = Common.readEnvironment('$XGRID_CONTROLLER_HOSTNAME')
    Globals.strXgridPassword = Common.readEnvironment('$XGRID_CONTROLLER_PASSWORD')

    try:
        opts, Globals.aryArgs = getopt.getopt(argv,
                    'erj:lg:p:xv:n:s:c:h',
                    [ 'exec', 'report', 'job=', 'launch', 'genomes=', 'plans=', 'xgrid', 'volume=', 'notify=', 'shell=', 'controller=', 'help' ])
    except getopt.error, err:
        raise Usage(' '.join(argv[1:]) + ' contains unknown arguments')

    for option, value in opts:
        if option in ('-e', '--exec'):
            Globals.fRunExec = True

        if option in ('-r', '--report'):
            Globals.fRunReport = True
            
        if option in ('-j', '--job'):
            aryArg = value.split(',')
            if len(aryArg) != 2:
                raise Usage('job takes exactly two arguments')
            try:
                aryArg[0] = int(aryArg[0])
            except ValueError:
                raise Usage(aryArg[0] + ' is an illegal job identifier -- identifiers must be a positive integer value')
            if aryArg[0] <= 0:
                raise Usage(aryArg[0] + ' is an illegal job identifier -- identifiers must be a positive integer value')
            if not aryArg[1] in ['restart', 'results', 'spec']:
                raise Usage(aryArg[1] + ' is an illegal value for job')
            if aryArg[1] == 'spec':
                aryArg[1] = 'specification'
            Globals.aryXgridJob = aryArg
            
        if option in ('-l', '--launch'):
            Globals.fLaunchJobs = True
            
        if option in ('-g', '--genomes'):
            Globals.strGenomes = value
            
        if option in ('-p', '--plans'):
            Globals.strPlans = value
            
        if option in ('-x', '--xgrid'):
            Globals.fUseXgrid = True
            
        if option in ('-v', '--volume'):
            Globals.strXgridVolume = value
            
        if option in ('-n', '--notify'):
            Globals.strXgridNotify = value
            
        if option in ('-s', '--shell'):
            Globals.strXgridShell = value
            
        if option in ('-c', '--controller'):
            aryArgs = value.split(',')
            if aryArgs[0]:
                Globals.strXgridController = aryArgs[0]
            if len(aryArgs) > 1 and aryArgs[1]:
                Globals.strXgridPassword = aryArgs[1]

        if option in ('-h', '--help'):
            raise Usage('')
            
    if not Globals.fRunExec and not Globals.fRunReport and not Globals.aryXgridJob:
        raise Usage('Neither exec, report, nor job were specified -- at least one must be specified')

    if Globals.fUseXgrid:
        if Globals.fRunExec and Globals.fRunReport:
            raise Usage('Xgrid does not support both exec and report -- run with exec first followed by report once completed')
        if not Globals.strXgridVolume:
            raise Usage('Xgrid requires the full path to the Stylus working directoryon an Xgrid client (e.g., /Stylus)')
        if not Globals.strXgridShell:
            raise Usage('Xgrid requires the full path to the shell within which to spawn Stylus on an Xgrid client')
        if not Globals.strXgridController:
            raise Usage('Xgrid requires the controller hostname')

        if not Globals.fLaunchJobs:
            Globals.fLaunchJobs = not Globals.strGenomes and not Globals.strPlans
            
    else:
        Globals.fLaunchJobs = True
            
    if Globals.strGenomes and Globals.fLaunchJobs:
        Globals.strGenomes = Common.resolvePath(Globals.strGenomes)
        if Globals.fUseXgrid and not os.path.isabs(Globals.strGenomes):
            Globals.strGenomes = os.path.join(Globals.strXgridVolume, Globals.strGenomes)
            
    if Globals.strPlans and Globals.fLaunchJobs:
        Globals.strPath = Common.resolvePath(Globals.strPlans)
        if Globals.fUseXgrid and not os.path.isabs(Globals.strPlans):
            Globals.strPlans = os.path.join(Globals.strXgridVolume, Globals.strPlans)
            
#------------------------------------------------------------------------------
# Function: spawnStylus
#
# Spawn one or more instances of a Stylus script
#------------------------------------------------------------------------------
def spawnStylus(fScript, dictEnv):
    if (Globals.strGenomes or Globals.strPlans) and Globals.fLaunchJobs:
        iJob = 0
        strExecArgs = 'STYLUS_EXECARGS' in dictEnv and dictEnv['STYLUS_EXECARGS'] or ''
        strReportArgs = 'STYLUS_RPTARGS' in dictEnv and dictEnv['STYLUS_RPTARGS'] or ''
    
        aryGenomes = Globals.strGenomes and fnmatch.filter(os.listdir(Globals.strGenomes), Common.Constants.globNotHidden) or []
        aryPlans = Globals.strPlans and fnmatch.filter(os.listdir(Globals.strPlans), Common.Constants.globNotHidden) or []
    
        if aryGenomes and not aryPlans:
            for fileGenome in aryGenomes:
                dictEnv['STYLUS_EXECARGS'] = '%s -g %s' % (strExecArgs, os.path.join(Globals.strGenomes, fileGenome))
                dictEnv['STYLUS_RPTARGS'] = '%s -g %s' % (strReportArgs, os.path.join(Globals.strGenomes, fileGenome))
                spawnSingle(fScript, iJob, dictEnv)
                iJob += 1
    
        elif not aryGenomes and aryPlans:
            for filePlan in aryPlans:
                dictEnv['STYLUS_EXECARGS'] = '%s -n ,,plan -p %s' % (strExecArgs, os.path.join(Globals.strPlans, filePlan))
                dictEnv['STYLUS_RPTARGS'] = '%s -n ,,plan -p %s' % (strReportArgs, os.path.join(Globals.strPlans, filePlan))
                spawnSingle(fScript, iJob, dictEnv)
                iJob += 1
    
        else:
            for fileGenome in aryGenomes:
                for filePlan in aryPlans:
                    dictEnv['STYLUS_EXECARGS'] = '%s -n ,,plan -g %s -p %s' % (strExecArgs, os.path.join(Globals.strGenomes, fileGenome), os.path.join(Globals.strPlans, filePlan))
                    dictEnv['STYLUS_RPTARGS'] = '%s -n ,,plan -g %s -p %s' % (strReportArgs, os.path.join(Globals.strGenomes, fileGenome), os.path.join(Globals.strPlans, filePlan))
                    spawnSingle(fScript, iJob, dictEnv)
                    iJob += 1
        
        if not Globals.fUseXgrid:        
            Common.say('\tLaunched %d instances' % iJob)
    
    else:
        spawnSingle(fScript, 0, dictEnv)
            
#------------------------------------------------------------------------------
# Function: spawnSingle
#
# Spawn a single instance of a Stylus script
#------------------------------------------------------------------------------
def spawnSingle(fScript, iJob, dictEnv):
    if not Globals.fUseXgrid:
        strScript = fScript == _SCRIPT_EXEC and 'stexec.py' or 'strpt.py'
        cc = os.spawnvpe(os.P_WAIT, sys.executable, [ os.path.basename(sys.executable), os.path.join(Globals.strLocalPath, strScript) ], os.environ)
        if cc:
            raise Common.BiologicError('Stylus failed - error code(%d)' % cc)
        
    else:
        strXgridJob = 'Stylus - %s' % datetime.datetime.now().isoformat()

        aryXgridEnv = [ _XGRID_ENVIRONMENT % (key, dictEnv[key]) for key in dictEnv.keys() if key in _STYLUS_ENVIRONMENT ]
        if not Globals.fLaunchJobs and (Globals.strGenomes or Globals.strPlans):
            aryXgridEnv.append(_XGRID_ENVIRONMENT % ('STYLUS_GRIDARGS', ('-x -l %s %s') % ((Globals.strGenomes and ('-g ' + Globals.strGenomes) or ''), (Globals.strPlans and ('-p ' + Globals.strPlans) or ''))))
            
            
        fd, strJobPath = tempfile.mkstemp('.xml', text=True)
        os.write(fd, _XGRID_JOB % (strXgridJob,
                                Globals.strXgridNotify and (_XGRID_NOTIFY % Globals.strXgridNotify) or '',
                                Globals.strXgridShell,
                                Globals.strXgridVolume + '/Stylus',
                                fScript == _SCRIPT_EXEC and '--exec' or '--report',
                                '\n'.join(aryXgridEnv)))
        os.close(fd)

        strXgridCmd = 'xgrid -h %s %s -job batch %s' % (Globals.strXgridController, (Globals.strXgridPassword and ('-auth Password -p %s' % Globals.strXgridPassword) or ''), strJobPath)
        cc = os.system(strXgridCmd)
        os.remove(strJobPath)
        
        if cc:
            raise Common.BiologicError('Xgrid command failed - command(%s) error code(%d)' % (strXgridCmd, cc))
            
#------------------------------------------------------------------------------
# Function: queryJob
#
# Query Xgrid about a previously launched job
#------------------------------------------------------------------------------
def queryXgrid():
    strXgridCmd = 'xgrid -h %s %s -job %s -id %s' % (Globals.strXgridController, (Globals.strXgridPassword and ('-auth Password -p %s' % Globals.strXgridPassword) or ''), Globals.aryXgridJob[1], Globals.aryXgridJob[0])
    cc = os.system(strXgridCmd)
    if cc:
        raise Common.BiologicError('Xgrid command failed - command(%s) error code(%d)' % (strXgridCmd, cc))
        
#==============================================================================
# Main
#==============================================================================
def main(argv=None):
    try:
        getArguments()
        
        if Globals.aryXgridJob:
            queryXgrid()
        
        if Globals.fRunExec:
            os.environ['STYLUS_EXECARGS'] = Common.readEnvironment('$STYLUS_EXECARGS') + ' ' + ' '.join(Globals.aryArgs)
            spawnStylus(_SCRIPT_EXEC, os.environ)
            
        if Globals.fRunReport:
            os.environ['STYLUS_RPTARGS'] = Common.readEnvironment('$STYLUS_RPTARGS') + ' ' + ' '.join(Globals.aryArgs)
            spawnStylus(_SCRIPT_REPORT, os.environ)

    except Common.BiologicError, err:
        Common.sayError(str(err))
        return 2

if __name__ == "__main__":
    sys.exit(main())
