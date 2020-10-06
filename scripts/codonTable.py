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
codonTable.py

This script creates and verifies codon tables. It also converts a gene using
one codon table to use another.

Stylus, Copyright 2006-2009 Biologic Institute.
'''

import datetime
import fnmatch
import getopt
import os
import pprint
import re
import shutil
import stylus.codons as Codons
import stylus.common as Common
import stylus.genome as Genome
import sys
import time
import urllib.request, urllib.error, urllib.parse
import urllib.parse
import uuid

#==============================================================================
# Global Constants
#==============================================================================
_VERSION = '1.3'
_NAME = 'codonTable.py %s' % _VERSION

_CODONTABLE_DEFINITION = '''<?xml version='1.0' encoding='UTF-8' ?>
<codonTable uuid='%s'%s creationDate='%s' creationTool='%s' xmlns='http://biologicinstitute.org/schemas/stylus/1.3'>
%s
</codonTable>
'''

_CODONTABLE_ENTRY = '''\t<entry codon='%s' vector='%s' %s/>'''

_CODONTABLE_ISSTANDARD = '''hasStandardMapping='%s' '''

_GENE_DEFINITION = '''<?xml version='1.0' encoding='UTF-8' ?>
<genome uuid='%s'%s creationDate='%s' creationTool='%s' xmlns='http://biologicinstitute.org/schemas/stylus/1.3'>
<codonTable uuid='%s'%s creationDate='%s' creationTool='%s'>
%s
</codonTable>
<bases>%s</bases>
<genes>
<gene baseFirst='1' baseLast='%d'>
<origin x='%r' y='%r' />
<hanReferences>
<hanReference unicode='%s'>
<strokes>
%s
</strokes>
</hanReference>
</hanReferences>
</gene>
</genes>
</genome>
'''

_GENE_STROKE = '''\t<stroke baseFirst='%d' baseLast='%d' correspondsTo='%d' />'''

#==============================================================================
# Globals
#==============================================================================
class Constants:
    strShortArgs = 'cvxg:t:o:u:a:qh'
    aryLongArgs = [ 'create', 'validate', 'translate', 'genome=', 'table=', 'output=', 'urls=', 'author=', 'quiet', 'help' ]
    
    cmdCreate    = 0x01
    cmdValidate  = 0x02
    cmdTranslate = 0x04

class Globals:
    genome = None
    codonTable = Codons.CodonTable()
    
    urlGenomeBase = ''
    strGenome = ''
    urlGenome = ''

    urlCodonTableBase = ''
    strCodonTable = ''
    urlCodonTable = ''

    pathOutput = ''
    
    uuid = ''
    
    strAuthor = ''

    cmd = 0

#==============================================================================
# Helper Classes
#==============================================================================
#------------------------------------------------------------------------------
# Class: Usage
#
#------------------------------------------------------------------------------
class Usage(Common.BiologicError):
    __strHelpMessage = '''
\t[(-c|--create)] - Create a new codon table
\t[(-v|--validate)] - Validate the passed codon table
\t[(-x|--translate)] - Translate the passed genome to use new codon table

\t[[-g|--genome] <genome path/url>] - Genome to use
\t[[-t|--table] <codon table path/url>] - Codon table to use

\t[[-o|--output] <output path>] - Full path for the output file

\t[[-u|--urls] <Genome URL>[,<Codon Table URL>]] - Set the URLs for obtaining files

\t[-a|--author] - Codon table or Genome author name
\t[-q|--quiet] - Silence all output
\t[-h|--help] - Print this help

Creating a new codon table produces a stand-alone codon table XML file whose contents are the
same as the codon table of the passed genome or table (or default table if neither is supplied).
The new table is assigned a new UUID.

Translating the codon table used by a passed genome results in a new genome containing the
same vectors but whose codons derive from the passed table.

Translated genomes and created codon tables are given the a file name of their UUID with
the appropriate extension - %s for genes and %s for codon tables. If the output path is
missing, the results will be written to STDOUT.

Note:
- Stand-alone codon tables and those within translated genomes will contain entries for
  all codons; those that map to the same vector as in the standard table will be marked
  as such. Stylus, on the other hand, only preserves those that differ from the standard
  codon table. See Stylus documentation for more details.

Related environment variables:
STYLUS_AUTHOR        - Codon table or Genome author name
STYLUS_GENOMEURL     - The default URL from which to obtain genome files
STYLUS_CODONTABLEURL - The default URL from which to obtain codon table files

See Stylus documentation for more details.
'''
    def __init__(self, msg):
        self.msg = ''
        if msg and len(msg) > 0:
            self.msg = 'Error: ' + msg + '\n'
        self.msg += 'Usage: ' + sys.argv[0].split("/")[-1] + '\n' + (self.__strHelpMessage % (Common.Constants.extGene, Common.Constants.extCodonTable))
            
    def __str__(self):
        return self.msg
        
#==============================================================================
# Helper Functions
#==============================================================================
#------------------------------------------------------------------------------
# Function: getArguments
# 
#------------------------------------------------------------------------------
def getArguments():
    argv = sys.argv[1:]

    Globals.strAuthor = Common.readEnvironment('$STYLUS_AUTHOR')
    Globals.urlGenomeBase = Common.readEnvironment('$STYLUS_GENOMEURL')
    if not Globals.urlGenomeBase:
        Globals.urlGenomeBase = './'
    Globals.urlCodonTableBase = Common.readEnvironment('$STYLUS_CODONTABLEURL')
    if not Globals.urlCodonTableBase:
        Globals.urlCodonTableBase = './'
    
    try:
        opts, remaining = getopt.getopt(argv, Constants.strShortArgs, Constants.aryLongArgs)
        if len(remaining) > 0:
            remaining[0].strip()
            if len(remaining) > 1 or remaining[0]:
                raise Usage(' '.join(remaining) + ' contains unexpected arguments')
    except getopt.error as err:
        raise Usage(' '.join(argv[1:]) + ' contains unknown arguments')
        
    for option, value in opts:
        if option in ('-c', '--create'):
            if Globals.cmd:
                raise Usage('just one of create, validate, or translate may be specified')
            Globals.cmd = Constants.cmdCreate

        if option in ('-v', '--validate'):
            if Globals.cmd:
                raise Usage('just one of create, validate, or translate may be specified')
            Globals.cmd = Constants.cmdValidate

        if option in ('-x', '--translate'):
            if Globals.cmd:
                raise Usage('just one of create, validate, or translate may be specified')
            Globals.cmd = Constants.cmdTranslate

        if option in ('-g', '--genome'):
            Globals.strGenome = value

        if option in ('-t', '--table'):
            Globals.strCodonTable = value
            
        if option in ('-o', '--output'):
            Globals.pathOutput = value

        if option in ('-u', '--urls'):
            aryArgs = value.split(',')
            if len(aryArgs) > 2:
                raise Usage('urls takes no more than two arguments')
            if len(aryArgs) > 0 and aryArgs[0]:
                Globals.urlGenomeBase = aryArgs[0]
            if len(aryArgs) > 1 and aryArgs[1]:
                Globals.urlCodonTableBase = aryArgs[1]
        
        if option in ('-a', '--author'):
            Globals.strAuthor = value

        if option in ('-q', '--quiet'):
            Common.Globals.fQuiet = True

        if option in ('-h', '--help'):
            raise Usage('')

    if not Globals.cmd:
        raise Usage('create, validate, or translate are required')

    Globals.urlGenomeBase = Common.pathToURL(Globals.urlGenomeBase, Common.Constants.schemeFile)
    if Globals.urlGenomeBase[len(Globals.urlGenomeBase)-1] != Common.Constants.urlSeparator:
        Globals.urlGenomeBase += Common.Constants.urlSeparator

    Globals.urlCodonTableBase = Common.pathToURL(Globals.urlCodonTableBase, Common.Constants.schemeFile)
    if Globals.urlCodonTableBase[len(Globals.urlCodonTableBase)-1] != Common.Constants.urlSeparator:
        Globals.urlCodonTableBase += Common.Constants.urlSeparator

    if Globals.cmd != Constants.cmdTranslate and Globals.strGenome and Globals.strCodonTable:
        raise Usage('%s takes either the genome or codon table as its source - not both' % (Globals.cmd == Constants.cmdCreate and 'create' or 'validate'))
    if Globals.cmd == Constants.cmdValidate and not Globals.strGenome and not Globals.strCodonTable:
        raise Usage('validate takes either the genome or codon table as its source')
    if Globals.cmd == Constants.cmdTranslate and (not Globals.strGenome or not Globals.strCodonTable):
        raise Usage('translate requires both the genome to translate and the codon table to use')
        
    if Globals.cmd != Constants.cmdValidate:
        Globals.uuid = str(uuid.uuid4()).upper()

    if Globals.strGenome:
        Globals.urlGenome = Common.pathToURL(Common.makeHanPath(Common.resolvePath(Globals.strGenome)), Globals.urlGenomeBase)
    if Globals.strCodonTable:
        Globals.urlCodonTable = Common.pathToURL(Globals.strCodonTable, Globals.urlCodonTableBase)
        
    if Globals.cmd != Constants.cmdValidate and Globals.pathOutput:
        Globals.pathOutput = Common.resolvePath(Globals.pathOutput)
        Common.ensurePath(os.path.dirname(Globals.pathOutput))
        Globals.pathOutput = os.path.join(Globals.pathOutput, ('%s%s' % (Globals.uuid, (Globals.cmd == Constants.cmdTranslate and Common.Constants.extGene or Common.Constants.extCodonTable))))
    return

#------------------------------------------------------------------------------
# Function: createTable
# 
#------------------------------------------------------------------------------
def createTable():
    aryEntries = [ _CODONTABLE_ENTRY % (codon, vector, (_CODONTABLE_ISSTANDARD % (Globals.codonTable.hasStandardMapping(codon) and 'true' or 'false'))) for codon, vector in Globals.codonTable.entries() ]
    strCodonTable = _CODONTABLE_DEFINITION % (Globals.uuid, Globals.strAuthor and (" author='%s'" % Globals.strAuthor) or '', datetime.datetime.utcnow().isoformat(), _NAME, '\n'.join(aryEntries))
    
    if Globals.pathOutput:
        try:
            fileCodonTable = os.open(Globals.pathOutput, os.O_CREAT | os.O_TRUNC | os.O_WRONLY, 0o664)
            os.write(fileCodonTable, strCodonTable)
        except IOError as err: raise Common.BiologicError('Unable to create %s - %s' % (Globals.pathOutput, str(err)))
        os.close(fileCodonTable)

        Common.say('Codon table written to ' + Globals.pathOutput)

    else:
        Common.say(strCodonTable, False, True)
    return

#------------------------------------------------------------------------------
# Function: validateTable
# 
#------------------------------------------------------------------------------
def validateTable():
    Globals.codonTable.validate()
    Common.say('%s successfully validated' % (Globals.urlCodonTable and Globals.urlCodonTable or Globals.urlGenome))
    return

#------------------------------------------------------------------------------
# Function: translateGenome
# 
#------------------------------------------------------------------------------
def translateGenome():
    genome = Globals.genome
    
    oldCodonTable = genome.codonTable
    newCodonTable = Globals.codonTable
    
    bases = Codons.Constants.startCodon
    for i in range(3,len(genome.bases),3):
        bases += newCodonTable.vectorToCodon(oldCodonTable.codonToVector(genome.bases[i:i+3]))
    genome.bases = bases
    
    aryEntries = [ _CODONTABLE_ENTRY % (codon, vector, (_CODONTABLE_ISSTANDARD % (Globals.codonTable.hasStandardMapping(codon) and 'true' or 'false'))) for codon, vector in Globals.codonTable.entries() ]
    aryStrokes = [ _GENE_STROKE % (stroke.rgBases.baseFirst, stroke.rgBases.baseLast, stroke.correspondsTo) for stroke in genome.gene.aryStrokes ]
    strGenome = _GENE_DEFINITION % (Globals.uuid, Globals.strAuthor and (" author='%s'" % Globals.strAuthor) or '', datetime.datetime.utcnow().isoformat(), _NAME,
                                    newCodonTable.uuid,
                                    newCodonTable.author and (" author='%s'" % newCodonTable.author) or '',
                                    newCodonTable.creationDate,
                                    newCodonTable.creationTool,
                                    '\n'.join(aryEntries),
                                    genome.bases,
                                    len(genome.bases),
                                    genome.gene.xOrigin, genome.gene.yOrigin,
                                    genome.gene.str,
                                    '\n'.join(aryStrokes))
    
    if Globals.pathOutput:
        try:
            fileGenome = os.open(Globals.pathOutput, os.O_CREAT | os.O_TRUNC | os.O_WRONLY, 0o664)
            os.write(fileGenome, strGenome)
        except IOError as err: raise Common.BiologicError('Unable to create %s - %s' % (Globals.pathOutput, str(err)))
        os.close(fileGenome)
        
        Common.say('Genome written to ' + Globals.pathOutput)

    else:
        Common.say(strGenome, False, True)
    return

#==============================================================================
# Main Routine
#==============================================================================
def main(argv=None):
    try:
        getArguments()

        if Globals.urlGenome:
            Common.say('Genome     : %s' % Globals.urlGenome)
        if Globals.urlCodonTable:
            Common.say('Codon Table: %s' % Globals.urlCodonTable)
        if Globals.pathOutput:
            Common.say('Output File: %s' % Globals.pathOutput)

        if Globals.urlGenome:
            Globals.genome = Genome.Genome(Globals.urlGenome)
        
        if Globals.urlCodonTable:
            Globals.codonTable = Genome.CodonTable(Globals.urlCodonTable)
        elif Globals.urlGenome and Globals.cmd != Constants.cmdTranslate:
            Globals.codonTable = Globals.genome.codonTable

        if Globals.cmd == Constants.cmdCreate:
            createTable()
            
        elif Globals.cmd == Constants.cmdValidate:
            validateTable()
            
        elif Globals.cmd == Constants.cmdTranslate:
            translateGenome()

        return 0
        
    except Usage as err:
        Common.sayError(err)
        return 0

    except Common.BiologicError as err:
        Common.sayError(err)
        return 2

if __name__ == "__main__":
    sys.exit(main())
