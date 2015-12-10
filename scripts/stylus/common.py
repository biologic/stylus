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
common.py

Stylus, Copyright 2006-2009 Biologic Institute.
'''

import os
import os.path
import re
import shutil
import sys
import time
import urllib2
import urlparse
import xmldict as XMLDict

#==============================================================================
# Global Constants and Variables
#==============================================================================
class Constants:
    basenamePlan = 'plan.xml'

    filenameInitial = 'initial'
    filenameTrial = 'trial'
    filenameFinal = 'final'

    globNotHidden = '[!.]*'
    globTrials = 'trial*.xml'
    
    reTRIALFILE = re.compile(r'trial(\d+).*')

    schemeFile = 'file://'
    urlSeparator = '/'

    extGene = '.gene'
    extCodonTable = '.table'
    extHan = '.han'
    extHCF = '.hcf'
    extXHTML = '.html'
    extXML = '.xml'

    reBASENAMEUNICODE = re.compile(r'([\dA-Fa-f]{4,5}).*\.(\w+)')
    reUUID = re.compile(r'([A-F\d]{8}\-[A-F\d]{4}\-[A-F\d]{4}\-[A-F\d]{4}\-[A-F\d]{12})')
    
    # strExec 'd:n:e:u:g:p:c:f:l:s:t:a:qvh'
    # aryExec [ 'datapath=', 'name=', 'expand=', 'urls=', 'genome=', 'plan=', 'constants=', 'frequency=', 'log=', 'statistics=', 'trace=', 'author=', 'quiet', 'version', 'help' ]
    # strRpt 'r:d:n:e:u:g:p:i:x:m:qh'
    # aryRpt [ 'reportpath=', 'datapath=', 'name=', 'expand=', 'urls=', 'genome=', 'plan=', 'images=', 'xhtml=', 'mpdb', 'quiet', 'help' ]
    strShortArgs = 'd:r:n:e:u:g:p:c:f:l:s:t:a:i:x:m:qvh'
    aryLongArgs = [ 'datapath=', 'reportpath=', 'name=', 'expand=', 'urls=', 'genome=', 'plan=', 'constants=', 'frequency=', 'log=', 'statistics=', 'trace=', 'author=', 'images=', 'xhtml=', 'mpdb', 'quiet', 'version', 'help' ]

    
class Globals:
    fQuiet = False
    fEchoStderr = False

#==============================================================================
# Classes
#==============================================================================
#------------------------------------------------------------------------------
# Class: BiologicError
#
#------------------------------------------------------------------------------
class BiologicError(Exception):
    def __init__(self, msg):
        self.msg = ''
        if msg and len(msg) > 0:
            self.msg = 'Error: ' + msg
            
    def __str__(self):
        return self.msg

#------------------------------------------------------------------------------
# Class: URLs
#
#------------------------------------------------------------------------------
class URLs:
    GENOME = 0x0001
    HAN    = 0x0002
    HTML   = 0x0004
    PLAN   = 0x0008
    SCHEMA = 0x0010

    urlGenome = ''
    urlHan = ''
    urlHTML = ''
    urlPlan = ''
    urlSchema = ''
    
    def __init__(self):
        self.urlGenome = readEnvironment('$STYLUS_GENOMEURL')
        self.urlHan = readEnvironment('$STYLUS_HANURL')
        self.urlHTML = readEnvironment('$STYLUS_HTMLURL')
        self.urlPlan = readEnvironment('$STYLUS_PLANURL')
        self.urlSchema = readEnvironment('$STYLUS_SCHEMAURL')

    def set(self, strURLs):
        aryArgs = strURLs.split(',')
        if len(aryArgs) > 5:
            raise BilogicError('URLs requires no more than five arguments')
        if len(aryArgs) > 0 and aryArgs[0]:
            self.urlGenome = aryArgs[0]
        if len(aryArgs) > 1 and aryArgs[1]:
            self.urlHan = aryArgs[1]
        if len(aryArgs) > 2 and aryArgs[2]:
            self.urlHTML = aryArgs[2]
        if len(aryArgs) > 3 and aryArgs[3]:
            self.urlPlan = aryArgs[3]
        if len(aryArgs) > 4 and aryArgs[4]:
            self.urlSchema = aryArgs[4]
            
    def validate(self, grf):
        if grf & self.GENOME:
            if not self.urlGenome:
                raise BiologicError('Required Genome URL is missing')
            self.urlGenome = self.__pathToURL__(self.urlGenome, Constants.schemeFile)

        if grf & self.HAN:
            if not self.urlHan:
                raise BiologicError('Required Han URL is missing')
            self.urlHan = self.__pathToURL__(self.urlHan, Constants.schemeFile)
            self.__validateScheme__(self.urlHan)

        if grf & self.HTML:
            if not self.urlHTML:
                raise BiologicError('Required HTML URL is missing')
            self.urlHTML = self.__pathToURL__(self.urlHTML, Constants.schemeFile)

        if grf & self.PLAN:
            if not self.urlPlan:
                raise BiologicError('Required Plan URL is missing')
            self.urlPlan = self.__pathToURL__(self.urlPlan, Constants.schemeFile)

        if grf & self.SCHEMA:
            if not self.urlSchema:
                raise BiologicError('Required Schema URL is missing')
            self.urlSchema = self.__pathToURL__(self.urlSchema, Constants.schemeFile)
            self.__validateScheme__(self.urlSchema)
            
    def __pathToURL__(self, strPath, strURL):
        strURL = pathToURL(strPath, strURL)
        if strURL[len(strURL)-1] <> Constants.urlSeparator:
            strURL += Constants.urlSeparator
        return strURL
    
    def __validateScheme__(self, url):
        aryURL = urlparse.urlsplit(url.lower())
        if aryURL[0] != 'http' and aryURL[0] != 'file':
            raise BiologicError(url + ' uses an unaccepted scheme - only http: or file: may be used')

#------------------------------------------------------------------------------
# Class: Names
#
#------------------------------------------------------------------------------
class Names:
    DATA   = 0x0001
    REPORT = 0x0002
    NAME   = 0x0004
    EXPAND = 0x0008
    GENOME = 0x0010
    PLAN   = 0x0020

    def __init__(self):
        self.strDataPath = readEnvironment('$STYLUS_DATAPATH')
        self.strReportPath = readEnvironment('$STYLUS_RPTPATH')
        self.strName = ''
        self.strNameType = 'file'
        self.fAppendPlan = False
        self.strExpand = ''
        self.fExpandRepeat = False
        self.strGenome = ''
        self.strPlan = ''

        self.pathData = ''
        self.pathReports = ''
        
        self.pathExpandData = ''
        self.pathExpandReport = ''
        
        self.urlGenome = ''
        self.idGenome = ''

        self.urlPlan = ''
        self.idPlan = ''
        self.nFirstPlanTrial = 0
        self.cPlanTrialsToExecute = 0
        
    def set(self, grf, strValue):
        if grf & self.DATA:
            self.strDataPath = strValue
            
        elif grf & self.REPORT:
            self.strReportPath = strValue
            
        elif grf & self.NAME:
            aryArg = strValue.split(',')
            self.strName = aryArg[0]
            if len(aryArg) > 1 and aryArg[1]:
                if not aryArg[1] in [ 'asis', 'dir', 'file', 'unicode', 'uuid' ]:
                    raise BiologicError(aryArg[1] + ' is an unknown option for the name argument')
                self.strNameType = aryArg[1]
            if len(aryArg) > 2 and aryArg[2]:
                if aryArg[2] != 'plan':
                    raise BiologicError(aryArg[2] + ' is an unknown option for the name argument')
                self.fAppendPlan = True
                
        elif grf & self.EXPAND:
            aryArgs = strValue.split(',')

            aryArgs[0] = ensureInteger(aryArgs[0], 0, 'expand requires a positive integer value')
            if aryArgs[0] <= 0:
                raise BiologicError('expand requires a positive integer value - rather than "%s"' % argArgs[0])
            self.strExpand = aryArgs[0]

            if len(aryArgs) > 1:
                if aryArgs[1] != 'repeat':
                    raise BiologicError(aryArgs[1] + ' is an unknown option for the expand argument')
                self.fExpandRepeat = True
                
        elif grf & self.GENOME:
            self.strGenome = strValue
            
        elif grf & self.PLAN:
            aryArgs = strValue.split(',')
            if len(aryArgs) < 1 or len(aryArgs) > 3:
                raise BiologicError('plan requires between one and three arguments')
            if aryArgs[0]:
                self.strPlan = aryArgs[0]
            if len(aryArgs) > 1:
                self.cPlanTrialsToExecute = ensureInteger(aryArgs[1], self.cPlanTrialsToExecute, 'plan requires an integer for the number of trials to execute')
            if len(aryArgs) > 2:
                self.nFirstPlanTrial = ensureInteger(aryArgs[2], self.nFirstPlanTrial, 'plan requires an integer for the first trial')
                
    def validate(self, urls, grf):
        if self.strNameType == 'asis' and not self.strName:
            raise BiologicError('asis requires specifying a name')
            
        if (grf & self.GENOME) or self.strNameType != 'dir':
            if not self.strGenome:
                raise BiologicError('Required genome was not supplied')
            urls.validate(URLs.GENOME)
            self.urlGenome = pathToURL(makeHanPath(resolvePath(self.strGenome)), urls.urlGenome)
            
        if grf & (self.DATA | self.REPORT):
            if self.strNameType == 'asis' or self.strNameType == 'file':
                self.idGenome = os.path.splitext(os.path.basename(self.urlGenome))[0]
            elif self.strNameType == 'uuid':
                if not Constants.reUUID.match(self.strGenome):
                    xmlDict = XMLDict.XMLDict({ XMLDict.XMLDict.ignore : [ 'seed', 'bases', 'statistics', 'lineage', 'genes' ] })
                    try: self.idGenome = str(xmlDict.load(self.urlGenome)['genome']['uuid']).upper()
                    except LookupError, err: raise BiologicError('Genome (%s) is missing a UUID' % self.urlGenome)
            elif self.strNameType == 'unicode':
                aryParts = os.path.split(self.urlGenome)
                mo = Constants.reBASENAMEUNICODE.match(aryParts[1])
                if mo and mo.groups():
                    self.idGenome = mo.groups()[0]
                else:
                    raise BiologicError('unicode name patterns required a genome file whose name begins with a unicode value')
            
        if grf & self.PLAN or self.fAppendPlan:
            if not self.strPlan:
                raise BiologicError('Required plan was not supplied')
            urls.validate(URLs.PLAN)
            self.urlPlan = pathToURL(resolvePath(self.strPlan), urls.urlPlan)
            self.idPlan = os.path.splitext(os.path.basename(self.urlPlan))[0]

        if grf & self.DATA:
            if not self.strDataPath:
                raise BiologicError('Required datapath was not supplied')
            self.pathData = resolvePath(self.strDataPath)
            if self.strNameType == 'asis':
                self.pathData = os.path.join(self.pathData, self.strName)
            else:
                idGenome = self.fAppendPlan and (self.idGenome + '--' + self.idPlan) or self.idGenome
                self.pathData = os.path.join(self.pathData, self.strName, idGenome)

            # Expand the data path unless expanding reports (since report expansion will set the data path as needed)
            if self.strExpand and not grf & self.REPORT:
                self.urlGenome, self.pathExpandData = self.__expandGenome__(self.pathData, Constants.extXML)
                self.pathData = os.path.join(self.pathData, self.pathExpandData)
                if not self.nFirstPlanTrial:
                    self.nFirstPlanTrial = 1

            self.pathData = os.path.normpath(os.path.normcase(self.pathData))
                
        if grf & self.REPORT:
            if not self.strReportPath:
                raise BiologicError('Required reportpath was not supplied')
            self.pathReport = resolvePath(self.strReportPath)
            if self.strNameType == 'asis':
                self.pathReport = os.path.join(self.pathReport, self.strName)
            elif self.strNameType == 'dir':
                strHead, strTail = os.path.split(self.strDataPath)
                if not strTail:
                    strHead, strTail = os.path.split(strHead)
                strTail, strExt = os.path.splitext(strTail)
                self.pathReport = os.path.join(self.pathReport, strTail)
            else:
                idGenome = self.fAppendPlan and (self.idGenome + '--' + self.idPlan) or self.idGenome
                self.pathReport = os.path.join(self.pathReport, self.strName, idGenome)

            if self.strNameType != 'dir' and self.strExpand:
                strNotUsed, self.pathExpandReport = self.__expandGenome__(self.pathReport, Constants.extXML)
                self.pathReport = os.path.join(self.pathReport, self.pathExpandReport)
                
                # Map the report path to the data path (since the data may have already been expanded once or more)
                self.pathData = os.path.normpath(os.path.normcase(os.path.join(self.pathData, self.pathExpandReport)))

            self.pathReport = os.path.normpath(os.path.normcase(self.pathReport))
            
    def __expandGenome__(self, strPath, strExt):
        urlParent = ''
        pathExpand = ''

        for root, dirs, files in os.walk(resolvePath(strPath)):
            if self.fExpandRepeat or not dirs:
                fileGenome = Constants.filenameInitial + strExt
                files.sort(cmpDataFilenames)
                for f in files:
                    mo = Constants.reTRIALFILE.match(f)
                    if mo and mo.groups():
                        iTrial = int(mo.groups()[0])
                        if self.strExpand == iTrial:
                            urlParent = pathToURL(os.path.join(root, fileGenome), Constants.schemeFile)
                        else:
                            fileGenome = f

            if urlParent:
                break

            if dirs:
                dirs.sort(cmpDataFilenames)
                for d in dirs:
                    mo = Constants.reTRIALFILE.match(d)
                    if mo and mo.groups():
                        iTrial = int(mo.groups()[0])
                        if iTrial >= self.strExpand:
                            pathExpand = os.path.join(pathExpand, d)
                            dirs = [ d ]
                            break

        if not urlParent:
            raise BiologicError('Unable to find trial %ld within %s' % (self.strExpand, strPath))
        pathExpand = os.path.join(pathExpand, '%s%ld' % (Constants.filenameTrial, self.strExpand))
        return urlParent, pathExpand

#==============================================================================
# Global Functions
#==============================================================================
#------------------------------------------------------------------------------
# Function: cmpDataFilenames
# 
#------------------------------------------------------------------------------
def cmpDataFilenames(strPath1, strPath2):
    ary1 = os.path.split(strPath1)
    ary2 = os.path.split(strPath2)
    if ary1[0] != ary2[0]:
        return cmp(strPath1, strPath2)
    elif ary1[1] == ary2[1]:
        return 0
    else:
        if ary1[1].startswith(Constants.filenameFinal):
            return 1
        if ary2[1].startswith(Constants.filenameFinal):
            return -1

        isTrial1 = ary1[1].startswith(Constants.filenameTrial)
        isTrial2 = ary2[1].startswith(Constants.filenameTrial)
        if isTrial1 and not isTrial2:
            return 1
        if not isTrial1 and isTrial2:
            return -1
        if isTrial1 and isTrial2:
            ary1 = os.path.splitext(ary1[1])
            ary2 = os.path.splitext(ary2[1])
            return cmp(toInteger(ary1[0][len(Constants.filenameTrial):]), toInteger(ary2[0][len(Constants.filenameTrial):]))
        else:
            return cmp(ary1[1], ary2[1])
    
#------------------------------------------------------------------------------
# Function: copyURLToFile
# 
#------------------------------------------------------------------------------
def copyURLToFile(strURL, strFile):
    str = readFile(strURL)
    file = open(strFile, 'w')
    file.write(str)
    file.close()
    return
    
#------------------------------------------------------------------------------
# Function: ensureInteger
# 
#------------------------------------------------------------------------------
def ensureInteger(strValue, nDefault, strError):
    if strValue:
        try: return int(strValue)
        except ValueError, err: raise BiologicError(strError + ' - rather than "' + strValue + '"')
    else:
        return nDefault
        
#------------------------------------------------------------------------------
# Function: ensurePath
# 
#------------------------------------------------------------------------------
def ensurePath(strPath):
    if not os.path.exists(strPath):
        try: os.makedirs(strPath)
        except OSError: raise BiologicError('Unable to create ' + strPath)
        
#------------------------------------------------------------------------------
# Function: isDir
# 
#------------------------------------------------------------------------------
def isDir(strPath):
    return os.path.exists(strPath) and os.path.isdir(strPath)
    
#------------------------------------------------------------------------------
# Function: makeHanPath
# 
#------------------------------------------------------------------------------
def makeHanPath(strPath):
    if not os.path.isabs(strPath) and not urlparse.urlparse(strPath)[0]:
        aryParts = os.path.split(strPath)
        if aryParts[1]:
            mo = Constants.reBASENAMEUNICODE.match(aryParts[1])
            if mo and mo.groups():
                strUnicode = mo.groups()[0]
                strPath = os.path.join(aryParts[0], strUnicode[:len(strUnicode)-3] + '000', aryParts[1])
    return strPath

#------------------------------------------------------------------------------
# Function: pathToURL
# 
#------------------------------------------------------------------------------
def pathToURL(strPath, strURL):
    strPath = resolvePath(strPath)
    return os.path.isabs(strPath) and urlparse.urljoin(Constants.schemeFile, strPath) or urlparse.urljoin(strURL, strPath)
        
#------------------------------------------------------------------------------
# Function: readEnvironment
# 
#------------------------------------------------------------------------------
def readEnvironment(strVariable):
    strValue = os.path.expandvars(strVariable)
    if strValue == strVariable:
        strValue = ''
    return strValue

#------------------------------------------------------------------------------
# Function: readFile
# 
#------------------------------------------------------------------------------
def readFile(strURL):
    try:
        file = urllib2.urlopen(strURL)
        str = file.read()
        file.close()
        return str

    except:
        raise BiologicError('Unable to read ' + strURL)
        
#------------------------------------------------------------------------------
# Function: redirectStderr
# 
#------------------------------------------------------------------------------
def redirectStderr(strFile):
    sys.stderr = open(strFile, 'a+')
    Globals.fEchoStderr = True

#------------------------------------------------------------------------------
# Function: replacePath
# 
#------------------------------------------------------------------------------
def replacePath(strPath):
    if os.path.exists(strPath):
        if not os.path.isdir(strPath):
            raise BiologicError(strPath + ' exists and is not a directory')
        try: shutil.rmtree(strPath)
        except OSError, err: raise BiologicError('Unable to replace ' + strPath)
    ensurePath(strPath)
        
#------------------------------------------------------------------------------
# Function: resolvePath
# 
#------------------------------------------------------------------------------
def resolvePath(strPath):
    if strPath:
        if len(strPath) >= 2 and strPath[0] == '.':
            if strPath[1] == os.sep:
                strPath = os.path.join(os.getcwd(), strPath[2:])
            elif strPath[1] == '.':
                strPath = os.path.join(os.path.dirname(os.getcwd()),strPath[3:])
        strPath = os.path.expanduser(strPath)
        if not urlparse.urlparse(strPath)[0]:
            fPreserveSeparator = (strPath[-1] == os.sep)
            strPath = os.path.normpath(os.path.normcase(strPath))
            if fPreserveSeparator:
                strPath += os.sep
    return strPath
    
#------------------------------------------------------------------------------
# Function: roundTo
# 
#------------------------------------------------------------------------------
def roundTo(f, n, fUp):
    f = fUp and (f+n) or (f-n)
    return int(f / n) * n;

#------------------------------------------------------------------------------
# Function: say, sayError
# 
#------------------------------------------------------------------------------
def say(strMsg, fNewline=True, fIgnoreQuiet=False):
    if not Globals.fQuiet or fIgnoreQuiet:
        sys.stdout.write(strMsg)
        if fNewline:
            sys.stdout.write('\n')
        else:
            sys.stdout.flush()
    return True
    
def sayError(strMsg):
    if not Globals.fQuiet:
        print >> sys.stderr, strMsg
        if Globals.fEchoStderr:
            print strMsg
    return True

#------------------------------------------------------------------------------
# Function: toFloat
# 
#------------------------------------------------------------------------------
def toFloat(strValue):
    try: return float(strValue)
    except ValueError, err: raise BiologicError('A floating-point number is required rather than "' + strValue + '"')

#------------------------------------------------------------------------------
# Function: toInteger
# 
#------------------------------------------------------------------------------
def toInteger(strValue):
    try: return int(strValue)
    except ValueError, err: raise BiologicError('An integer is required rather than "' + strValue + '"')
    
#------------------------------------------------------------------------------
# Function: trimDirectory
# 
#------------------------------------------------------------------------------
def trimDirectory(strPath):
    iSep = strPath.index(os.sep)
    return iSep >= 0 and strPath[iSep+1:] or strPath

if __name__ == '__main__':
    pass
