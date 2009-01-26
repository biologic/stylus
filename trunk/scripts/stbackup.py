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
stbackup.py

This script creates a compressed archive of Stylus working data

Stylus, Copyright 2006-2008 Biologic Institute.
'''

import datetime
import getopt
import glob
import os
import os.path
import string
import stylus.common as Common
import sys

#==============================================================================
# Globals
#==============================================================================
class Constants:
    strTranslate = string.maketrans('.:', '_-')

    strArchives = '*.cpgz'
    extArchive = '.cpgz'
    
    pathDefaultSrc = '/Volumes/Grid/Biologic/'
    pathDefaultDst = '/Volumes/Grid/Backups/'
    strDefaultFolders = 'Han:genes:plans:results'

class Globals:
    pathSrc = ''
    pathDst = ''
    aryFolders = []
    nRemoveAge = 7

#==============================================================================
# Classes
#==============================================================================
#------------------------------------------------------------------------------
# Class: Usage
#
#------------------------------------------------------------------------------
class Usage(Common.BiologicError):
    __strHelpMessage = '''
\t[[-s|--src] <source directory path>] - Source directory path
\t[[-d|--dst] <destination directory path>] - Destination directory path

\t[[-f|--folders] <folder>[:<folder>]] - Colon separated list of folders within the source to backup

\t[[-r|--removeage] <days>] - Age in days of old backups to remove; must be greater than zero

\t[-h|--help] - Print this help

Related environment variables:
STYLUS_BACKUPARGS - The default argument string to parse (command-line overrides duplicate arguments)
STYLUS_BACKUPSRC  - The source directory containing all data to backup
STYLUS_BACKUPDST  - The destination directory for the created archive
STYLUS_BACKUPFOLDERS - A colon separated list of folders to backup
'''
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
    args = Common.readEnvironment('$STYLUS_BACKUPARGS')
    argv = args and args.split() or []
    argv += sys.argv[1:]

    Globals.pathSrc = Common.readEnvironment('$STYLUS_BACKUPSRC') or Constants.pathDefaultSrc
    Globals.pathDst = Common.readEnvironment('$STYLUS_BACKUPDST') or Constants.pathDefaultDst

    Globals.aryFolders = Common.readEnvironment('$STYLUS_BACKUPFOLDERS') or Constants.strDefaultFolders

    try:
        opts, Globals.aryArgs = getopt.getopt(argv,
                    's:d:f:r:h',
                    [ 'src=', 'dst=', 'folders=', 'removeage=', 'help' ])
    except getopt.error, err:
        raise Usage(' '.join(argv[1:]) + ' contains unknown arguments')

    for option, value in opts:
        if option in ('-s', '--src'):
            Globals.pathSrc = value
            
        if option in ('-d', '--dst'):
            Globals.pathDst = value
            
        if option in ('-f', '--folders'):
            Globals.aryFolders = value
            
        if option in ('-r', '--removeage'):
            nDays = int(value)
            if nDays <= 0:
                raise Usage('%s is an illegal value for removeage')
            Globals.nRemoveAge = nDays

        if option in ('-h', '--help'):
            raise Usage('')

    if not Globals.pathSrc:
        raise Usage('Required source path was not specified')
    Globals.pathSrc = os.path.abspath(Common.resolvePath(Globals.pathSrc))
    if not os.path.exists(Globals.pathSrc) or not os.path.isdir(Globals.pathSrc):
        raise Usage('Source path - %s - does not exist or is not a directory' % Globals.pathSrc)

    if not Globals.pathDst:
        raise Usage('Required destination path was not specified')
    Globals.pathDst = os.path.abspath(Common.resolvePath(Globals.pathDst))
    if not os.path.exists(Globals.pathDst) or not os.path.isdir(Globals.pathSrc):
        raise Usage('Destination path - %s - does not exist or is not a directory' % Globals.pathDst)
        
    if not Globals.aryFolders:
        raise Usage('Required folders were not supplied')
    Globals.aryFolders = Globals.aryFolders.split(':')
    for folder in Globals.aryFolders:
        pathFolder = os.path.abspath(os.path.join(Globals.pathSrc, folder))
        if not os.path.exists(pathFolder) or not os.path.isdir(pathFolder):
            raise Usage('Folder %s does not exist or is not a directory' % pathFolder)
            
#------------------------------------------------------------------------------
# Function: doBackup
#
# Perform the backup
#------------------------------------------------------------------------------
def doBackup():
    Common.say('Archving folders [ %s ] in %s' % (', '.join(Globals.aryFolders), Globals.pathSrc))
    
    dtNow = datetime.datetime.now()
    dtWeekAgo = dtNow - datetime.timedelta(days=Globals.nRemoveAge)

    # Purge archives and archive directories older than one week
    for item in glob.glob(os.path.join(Globals.pathDst, Constants.strArchives)):
        stat = os.stat(item)
        if datetime.datetime.fromtimestamp(stat.st_mtime) < dtWeekAgo:
            if os.path.isdir(item):
                cc = os.system('rm -drf %s' % item)
                if cc:
                    raise Common.BiologicError('Unable to remove old directory %s' % item)
            else:
                os.remove(item)
            
    # Create a new archive directory
    strArchive = 'Stylus_%s' % dtNow.isoformat().translate(Constants.strTranslate)
    pathArchive = os.path.abspath(os.path.join(Globals.pathDst, strArchive))
    os.makedirs(pathArchive)

    # Populate the archive directory
    for folder in Globals.aryFolders:
        pathSrc = os.path.abspath(os.path.join(Globals.pathSrc, folder))
        pathDst = os.path.abspath(os.path.join(pathArchive, folder))
        cc = os.system('ditto %s %s' % (pathSrc, pathDst))
        if cc:
            raise Common.BiologicError('Unable to copy folder %s to archive directory %s - error code(%d)' % (folder, pathArchive, cc))
        Common.say('...copied %s' % folder)

    # Create the archive
    fileArchive = os.path.abspath(os.path.join(Globals.pathDst, strArchive + Constants.extArchive))
    cc = os.system('ditto -c -z %s %s' % (pathArchive, fileArchive))
    if cc:
        raise Common.BiologicError('Unable to create archive %s - error code(%d)' % (fileArchive, cc))
    Common.say('...created archive %s' % pathArchive)

    # Remove the archive directory
    cc = os.system('rm -drf %s' % pathArchive)
    if cc:
        raise Common.BiologicError('Unable to temporary archive directory %s' % fileArchive)
        
#==============================================================================
# Main
#==============================================================================
def main(argv=None):
    try:
        getArguments()
        doBackup()
        
    except Common.BiologicError, err:
        Common.sayError(str(err))
        return 2

if __name__ == "__main__":
    sys.exit(main())
