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
strpt.py

This script is the Python front-end for generating output from Stylus experiment data.

NOTE:
This script assumes a single gene, compared against a single Han definition, within an
genome XML file. It further assumes that all genome files within the data directory
reference the same Han definition.

Stylus, Copyright 2006-2009 Biologic Institute.
'''

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

#==============================================================================
# Global Constants
#==============================================================================
_HTML_HEADER = '''<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>
<html xmlns='http://www.w3.org/1999/xhtml'>
	<head>
		<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
		<title>Stylus - Trials %d to %d</title>
		<style type='text/css'>@import url(stylus.css);</style>
		<script type='text/javascript' src='expanded.js'></script>
		<script type='text/javascript' src='stylus.js'></script>
	</head>
    <body>
'''

_HTML_TITLE_SECTION = '''
<div class='title'>
    <h1>Genome (based on %s)- Trials %d to %d</h1>
    <table>
        <tr><th>Experiment Date</th><td>%s &mdash; %s UTC</td></tr>
        <tr><th>Author</th><td>%s</td></tr>
    </table>
    <div class='navigation'>%s</div>
</div>
'''

_HTML_HEADER_SECTION = '''
<div class='header'>
	<table>
	    <tr><th>Fitness</th><td><span class='statistic'>%r</span>&nbsp;max<span class='statistic'>[%r in trial %d]</span>&nbsp;min<span class='statistic'>[%r in trial %d]</span></td></tr>
		<tr><th>Score</th><td><span class='statistic'>%r</span>&nbsp;max<span class='statistic'>[%r in trial %d]</span>&nbsp;min<span class='statistic'>[%r in trial %d]</span></td></tr>
		<tr><th>Rollbacks</th><td>total<span class='statistic'>[%d]</span> max<span class='statistic'>[%d in trial %d]</span> min<span class='statistic'>[%d in trial %d]</span></td></tr>
		<tr><th>Mutations</th><td>changes<span class='statistic'>[%d total, %d silent]</span> copies<span class='statistic'>[%d]</span> deletions<span class='statistic'>[%d]</span> insertions<span class='statistic'>[%d]</span> transpositions<span class='statistic'>[%d]</span></td></tr>
	</table>
</div>
'''

_HTML_DEFAULT_MAIN = '''
<div class='main'><div class='icons'>
%s
</div></div>
'''

_HTML_DEFAULT_ROW = '''
<table><tr>%s</tr><tr><td colspan='%d'><embed width='%d' height='%d' src='%s' /></td></tr></table>
'''

_HTML_TRIAL_MAIN = '''
<div class='main'><ul>%s</ul></div>
'''

_HTML_TRIAL_ROW = '''
<li><a id='a-%s' href='javascript:toggle("%s");'>[%c]</a><div class='toggleItem'><h2>%s</h2>
%s
</div></li>
'''

_HTML_TRIAL_CANVAS = '''
<div id='%s' class='canvas' style='display: %s;'>
%s
</div>
'''

_HTML_TRIAL_SCORES = '''
<h3>Genome - %r</h3>
<table>
	<tr><th class='narrow'>bases</th><th class='wide'>units</th></tr>
	<tr><td>%d</td><td>%r</td></tr>
</table>

<h3>Gene - %r</h3>
<table>
	<tr><th class='wide'>scale</th><th class='wide'>placement</th><th class='narrow'>illegal overlaps</th><th class='narrow'>missing overlaps</th><th class='narrow'>marks</th></tr>
	<tr><td>%r</td><td>%r</td><td>%d</td><td>%d</td><td>%r</td></tr>
</table>
<h3>Groups</h3>
<ol class='groups'>%s</ol>

<h3>Strokes</h3>
<ol class='strokes'>%s</ol>
'''

_HTML_TRIAL_GROUP_SCORE = '''
<li>%r
	<table>
		<tr><th class='wide'>deviation</th><th class='wide'>extra length</th></tr>
		<tr><td>%r</td><td>%r</td></tr>
	</table>
	<table>
		<tr><th class='wide'>scale</th><th class='wide'>placement</th><th class='narrow'>illegal overlaps</th><th class='narrow'>missing overlaps</th><th class='narrow'>dropouts</th></tr>
		<tr><td>%r</td><td>%r</td><td>%d</td><td>%d</td><td>%d</td></tr>
	</table>
</li>
'''

_HTML_TRIAL_STROKE_SCORE = '''
<li>&nbsp;<table>
    <tr><th class='wide'>deviation</th><th class='wide'>extra length</th><th class='narrow'>dropouts</th></tr>
    <tr><td>%f</td><td>%r</td><td>%d</td></tr>
</table></li>
'''

_HTML_TRIAL_CODONS = '''
<p>%d bases (%d codons), %d (%d) in strokes, %d (%d) in moves&nbsp;&mdash;&nbsp;%d strokes</p>
<h3>Stroke Ranges</h3>
%s
%s
<div class='sequence'>
%s
</div>
'''

_HTML_TRIAL_CODON_ROW = '''
%04d-%04d&nbsp;<span class='index'>%s</span><br />
<span class='bases'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%s</span><br />
<span class='acids'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%s</span><br />
<br />
'''

_HTML_FOOTER = '''
    </body>
</html>
'''

_SVG_HEADER = '''<?xml version='1.0' standalone='no'?><?xml-stylesheet href='stylus.css' type='text/css'?><!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.0//EN' 'http://www.w3.org/Graphics/SVG/1.0/DTD/svg10.dtd'>
<svg width='%dpx' height='%dpx' viewBox='0 0 %d %d' version='1.0' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'>
<script type='text/javascript' xlink:href='stylus-svg.js' />
'''

_SVG_TRANSFORM_GROUP = '''
<g class='%s' transform='translate(%d,%d) scale(%r)' style='stroke-width: %rpx;'>
%s
</g>
'''

_SVG_DISPLAY_GROUP = '''
<g id='%s' opacity='%f'>
%s
</g>
'''

_SVG_DEFAULT_ICON = '''
<rect class='icon' x='%r' y='0' width='%r' height='%r' />
<g transform='translate(%d,%d) scale(%r)' style='stroke-width: %rpx;'>
%s
</g>
<g transform='translate(%d,130)'><text class='icon' x='0' y='0'>%10.8f</text></g>
'''

_SVG_TRIAL_CONTROLS = '''
<rect class='gene' x='1' y='1' width='698px' height='498px' />
<rect class='han' x='500' y='0' width='200px' height='345px' />
<rect class='han' x='500' y='345' width='200px' height='100px' />
<g class='control' transform='translate(508,350)'><a onclick='showHide("coherent");'><circle id='coherent-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10' class='coherent'>Strokes</text></a></g>
<g class='control' transform='translate(508,365)'><a onclick='showHide("incoherent");'><circle id='incoherent-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10' class='incoherent'>Moves</text></a></g>
<g class='control' transform='translate(508,380)'><a onclick='showHide("marks");'><circle id='marks-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10' class='mark'>Marks</text></a></g>
<g class='control' transform='translate(508,395)'><a onclick='showHide("dropouts");'><circle id='dropouts-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10' class='dropout'>Dropouts</text></a></g>
<g class='control' transform='translate(508,410)'><a onclick='showHide("mutations");'><circle id='mutations-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10' class='mutation'>Mutations</text></a></g>
<g class='control' transform='translate(508,425)'><a onclick='showHide("overlaps");'><circle id='overlaps-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10'>Overlaps</text></a></g>
<g class='control' transform='translate(590,350)'><a onclick='showHide("labels");'><circle id='labels-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10'>Labels</text></a></g>
<g class='control' transform='translate(590,365)'><a onclick='showHide("vertices");'><circle id='vertices-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10'>Vertices</text></a></g>
<g class='control' transform='translate(590,380)'><a onclick='showHide("grid");'><circle id='grid-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10'>Grid</text></a></g>
<g class='control' transform='translate(590,395)'><a onclick='showHide("bounding-group");'><circle id='bounding-group-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10'>Group Boxes</text></a></g>
<g class='control' transform='translate(590,410)'><a onclick='showHide("bounding-stroke");'><circle id='bounding-stroke-control' fill='%s' cx='5' cy='5' r='4' /><text x='12' y='10'>Stroke Boxes</text></a></g>
<rect class='key' x='500' y='445' width='200px' height='55px' />
<g transform='translate(510,462)'>
	<text class='key' x='0' y='0'>s =</text>
	<text class='key' x='0' y='15'>m =</text>
	<text class='key' x='0' y='30'>l =</text>
	<g transform='translate(35,-5)'>
		<line class='key' x1='0' y1='0' x2='%r' y2='0' />
		<line class='key' x1='0' y1='15' x2='%r' y2='15' />
		<line class='key' x1='0' y1='30' x2='%r' y2='30' />
	</g>
</g>
<g transform='translate(590,462)'><text class='key' x='0' y='0'>%10.8f</text></g>
<g id='vertices' opacity='%f'></g>
<g id='labels' opacity='%f'></g>
'''

_SVG_FOOTER = '''
</svg>
'''

# Note: PDB Records must not exceed 80 characters
#                        1         2         3         4         5         6         7         8
#               12345678901234567890123456789012345678901234567890123456789012345678901234567890
_PDB_HEADER  = 'HEADER                                                        1ABC\n'
_PDB_TITLE   = 'TITLE     %.70s\n'
_PDB_AUTHOR  = 'AUTHOR    %.70s\n'
_PDB_REMARK4 = 'REMARK   4 1ABC COMPLIES WITH FORMAT V. 2.1, 25-OCT-1996\n'
_PDB_REMARK6 = 'REMARK   6 %.69s\n'
_PDB_ORIG    = 'ORIGX1      1.000000  1.000000  1.000000        0.00000\nORIGX2      1.000000  1.000000  1.000000        0.00000\nORIGX3      1.000000  1.000000  1.000000        0.00000\n'
_PDB_SCALE   = 'SCALE1      1.000000  0.000000  0.000000        0.00000\nSCALE2      0.000000  1.000000  0.000000        0.00000\nSCALE3      0.000000  0.000000  1.000000        0.00000\n'
# Note: HETATM contains Atom, Serial #, Chain ID (as character),Residue Seq. # (monotonically increasing in a chain), x/y/z location
_PDB_HETATM  = 'HETATM%5d  C       %1s%4d    %8.3f%8.3f%8.3f  1.00  1.00           C'
_PDB_CONECT  = 'CONECT%5d%5d'
_PDB_MASTER  = 'MASTER    %5d    0    0    0    0    0    0    6%5d    0%5d    0\nEND   \n'

#==============================================================================
# Globals
#==============================================================================
class SVGOptions:
    def __init__(self):
        self.fCoherent = True
        self.fIncoherent = True
        self.fNoise = True
        self.fMutations = True
        self.fOverlaps = False
        self.fLabels = True
        self.fVertices = False
        self.fGrid = False
        self.fBounding = False
        return
    
    def set(self, strArgs, strDefault):
        if 'd' in strArgs:
            strArgs += strDefault
        self.fCoherent = 'c' in strArgs
        self.fIncoherent = 'i' in strArgs
        self.fNoise = 'n' in strArgs
        self.fMutations = 'm' in strArgs
        self.fOverlaps = 'o' in strArgs
        self.fLabels = 'l' in strArgs
        self.fVertices = 'v' in strArgs
        self.fGrid = 'g' in strArgs
        self.fBounding = 'b' in strArgs
        return
    
class Globals:
    names = Common.Names()
    urls = Common.URLs()

    fImages = False
    fXHTML = False
    aryPDB = [ '', 0.1, 1.0 ]
    
    svgImages = SVGOptions()
    svgXHTML = SVGOptions()

    fHTMLSVG = True
    fHTMLScores = False
    fHTMLMutations = True
    fHTMLCodons = False

#==============================================================================
# Helper Classes
#==============================================================================
#------------------------------------------------------------------------------
# Class: Constants
# 
#------------------------------------------------------------------------------
class Constants:
    reSVGOptions = re.compile(r'[dcinmlvgb]+')
    defaultSVGOptions = 'cinml'
    defaultImageOptions = 'cv'

    reHTMLOptions = re.compile(r'[dgsmc]')
    defaultHTMLOptions = 'gc'

    fileCSS = 'stylus.css'
    fileExpanded = 'expanded.js'
    fileScript = 'stylus.js'
    fileSVGScript = 'stylus-svg.js'
    
    cssGene = 'gene'

    cssAxis = 'axis'
    cssGridMajor = 'grid-major'
    cssGridMinor = 'grid-minor'

    cssCoherent = 'coherent'
    cssIncoherent = 'incoherent'
    cssMark = 'mark'
    cssDropout = 'dropout'
    cssMutation = 'mutation'
    cssOverlap = 'overlap'

    cssBoundingGroup = 'bounding-group'
    cssBoundingStroke = 'bounding-stroke'

    cssHan = 'han'

    fillVisible = '#999'
    fillInvisible = '#fff'

    dxTrial = 700
    dyTrial = 500
    
    dxImage = 500
    dyImage = 500

    dGridMinor = 10
    dGridMajor = 100

    dxGene = 460
    dyGene = 460
    dxGeneOffset = 20
    dyGeneOffset = 20

    dxIcon = 150
    dyIcon = 150
    dxIconOffset = 525
    dyIconOffset = 15
    dxHanIconOffset = 525
    dyHanIconOffset = 180

    cDefaultIcons = 8
    dxDefault = 110
    dyDefault = 134
    dxDefaultBorder = 10

    dxDefaultIcon = 100
    dyDefaultIcon = 100
    dxDefaultIconOffset = 5
    dyDefaultIconOffset = 5
    
    dxSmallVectorMax = 40
    dxMediumVectorMax = 160
    dxLongVectorMax = 160
    
    pxAxis = 3
    pxGridMajor = 2
    pxGridMinor = 1
    
    pxDashArrayDash = 3
    pxDashArrayGap = 5

    pxLine = 2
    pxLineScaledMax = 6
    pxOverlap = 6
    pxVertex = 2
    pxFontSize = 16
    pxFontSizeScaledMin = 0.5
    
    dBasesPerLine = 60
    dStrokeRangesPerLine = 9
    dCodonTableEntriesPerLine = 16
    dCodonTableEntriesPerSet = 4

#------------------------------------------------------------------------------
# Class: Usage
#
#------------------------------------------------------------------------------
class Usage(Common.BiologicError):
    __strHelpMessage = '''
\t[(-r|--reportpath) <report output path>] - The path for report files
\t[(-d|--datapath) <data input path>] - The path for experiment data
\t[(-n|--name) <experiment name>[,asis|dir|file|unicode][,plan]] - Optional experiment name
\t[(-e|--expand) <trial number>[,repeat]] - Trial to expand

\t[[-u|--urls] [<Genome URL>][,<Han URL>][,<HTML URL>][,<Plan URL>][,<XML Schema URL>]] - Set the URLs for obtaining files

\t[[-g|--genome] <genome path/url>] - Genome to report
\t[[-p|--plan] <plan path/url>[,<number of trials to execute>[,<first trial associated with plan>]]] - Executed plan

\t[(-i|--images) [<SVG options>]] - Create gene image (SVG) files
\t\t<SVG options> is a string of the following:
\t\t\td - Show the default set of '%s'
\t\t\tc - Show coherent sections
\t\t\ti - Show incoherent sections
\t\t\tn - Show noise (marks and dropouts)
\t\t\tm - Show mutations
\t\t\to - Show overlaps
\t\t\tl - Show labels
\t\t\tv - Show vector vertices
\t\t\tg - Show grid
\t\t\tb - Show group and stroke bounding boxes

\t[(-x|--xhtml) [<SVG options>][,<HTML options>]] - Create XHTML output files
\t\t<SVG options> is a string of the following:
\t\t\td - Show the default set of '%s'
\t\t\tc - Show coherent sections
\t\t\ti - Show incoherent sections
\t\t\tn - Show noise (marks and dropouts)
\t\t\tm - Show mutations
\t\t\to - Show overlaps
\t\t\tl - Show labels
\t\t\tv - Show vector vertices
\t\t\tg - Show grid
\t\t\tb - Show group and stroke bounding boxes
\t\t<HTML options> is a string of the following:
\t\t\td - Show the default set of '%s'
\t\t\tg - Show SVG
\t\t\ts - Show scores
\t\t\tm - Show mutations
\t\t\tc - Show codons

\t[(-m|--mpdb) [initial][+final][+<trial number>]*[,<pitch>[,<scale>]]] - Create molecular PDB files

\t[-q|--quiet] - Silence all output
\t[-h|--help] - Print this help

Related environment variables:
STYLUS_RPTARGS   - The default argument string to parse (command-line overrides duplicate arguments)

STYLUS_DATAPATH  - The default path for experiments (to which non-absolute experiment names are appended)
STYLUS_RPTPATH   - The default path for experiments (to which non-absolute experiment names are appended)

STYLUS_GENOMEURL - The default URL from which to obtain genome files
STYLUS_HANURL    - The URL from which to obtain Han definition files
STYLUS_HTMLURL   - The URL from which to obtain XHTML-related files (e.g., stylus.css)

See Stylus documentation for more details.
'''
    def __init__(self, msg):
        self.msg = ''
        if msg and len(msg) > 0:
            self.msg = 'Error: ' + msg + '\n'
        self.msg += 'Usage: ' + sys.argv[0].split("/")[-1] + '\n' + (self.__strHelpMessage % (Constants.defaultImageOptions, Constants.defaultSVGOptions, Constants.defaultHTMLOptions))
            
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
    # Start with command-line arguments and append those passed via the environment
    # (The command-line overrides the environment)
    args = Common.readEnvironment('$STYLUS_RPTARGS')
    argv = args and args.split() or []
    argv += sys.argv[1:]
    
    try:
        opts, remaining = getopt.getopt(argv, Common.Constants.strShortArgs, Common.Constants.aryLongArgs)
        if len(remaining) > 0:
            remaining[0].strip()
            if len(remaining) > 1 or remaining[0]:
                raise Usage(' '.join(remaining) + ' contains unexpected arguments')
    except getopt.error as err:
        raise Usage(' '.join(argv[1:]) + ' contains unknown arguments')

    for option, value in opts:
        try:
            if option in ('-r', '--reportpath'):
                Globals.names.set(Common.Names.REPORT, value)

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
        except Common.BiologicError as err:
            raise Usage(str(err))
            
        if option in ('-i', '--images'):
            Globals.fImages = True
            Globals.svgImages.set(value, Constants.defaultImageOptions)
            
        if option in ('-x', '--xhtml'):
            aryArgs = value.split(',')
            if len(aryArgs) < 1 or len(aryArgs) > 2:
                raise Usage('xhtml requires one or two arguments')
            if len(aryArgs) < 2:
                aryArgs.append('')
            if not aryArgs[0]:
                aryArgs[0] = Constants.defaultSVGOptions
            if not Constants.reSVGOptions.match(aryArgs[0]):
                raise Usage('%s contains unrecognized SVG options' % aryArgs[0])
            if not aryArgs[1]:
                aryArgs[1] = Constants.defaultHTMLOptions
            if not Constants.reHTMLOptions.match(aryArgs[1]):
                raise Usage('%s contains unrecognized HTML options' % aryArgs[1])
            Globals.fXHTML = True
            Globals.svgXHTML.set(aryArgs[0], Constants.defaultSVGOptions)
            if 'd' in aryArgs[1]:
                aryArgs[1] += Constants.defaultHTMLOptions
            Globals.fHTMLSVG = 'g' in aryArgs[1]
            Globals.fHTMLScores = 's' in aryArgs[1]
            Globals.fHTMLMutations = 'm' in aryArgs[1]
            Globals.fHTMLCodons = 'c' in aryArgs[1]
            
        if option in ('-m', '--mpdb'):
            aryArgs = value.split(',')
            if len(aryArgs) < 1 or len(aryArgs) > 3:
                raise Usage('mpdb requires between one and three arguments')
            if not aryArgs[0]:
                raise Usage('mpdb requires at least one trial')
            Globals.aryPDB[0] = aryArgs[0].split('+')
            if len(aryArgs) > 1 and aryArgs[1]:
                Globals.aryPDB[1] = Common.toFloat(aryArgs[1])
            if len(aryArgs) > 2 and aryArgs[2]:
                Globals.aryPDB[2] = Common.toFloat(aryArgs[2])

        if option in ('-q', '--quiet'):
            Common.Globals.fQuiet = True

        if option in ('-h', '--help'):
            raise Usage('')
        
    if not Globals.fXHTML and not Globals.fImages and not Globals.aryPDB[0]:
        raise Usage('No reports or images were requested')

    try:
        Globals.urls.validate(Common.URLs.HAN | Common.URLs.HTML)
        Globals.names.validate(Globals.urls, Common.Names.DATA | Common.Names.REPORT)
    except Common.BiologicError as err:
        raise Usage(str(err))

    if not Common.isDir(Globals.names.pathData):
        raise Common.BiologicError('%s either does not exist or is not a directory' % Globals.names.pathData)

    Common.replacePath(Globals.names.pathReport)
    
#------------------------------------------------------------------------------
# Function: loadHan
# 
#------------------------------------------------------------------------------
def loadHan(strUnicode):
    urlHan = Common.pathToURL(Common.makeHanPath(strUnicode + Common.Constants.extHan), Globals.urls.urlHan)
    try: han = Genome.Han(urlHan)
    except LookupError as err: raise Common.BiologicError('%s is missing one or more required elements or attributes - %s' % (urlHan, str(err)))
    except urllib.error.URLError as err: raise Common.BiologicError('Unable to open URL %s - %s' % (urlHan, str(err)))
    except OSError as err:  raise Common.BiologicError('Unable to open URL %s - %s' % (urlHan, str(err)))
    except: raise Common.BiologicError('Unable to open URL %s' % urlHan)
    return han

#------------------------------------------------------------------------------
# Function: loadData
# 
#------------------------------------------------------------------------------
def loadData():
    aryFiles = [ 'initial.xml', 'final.xml' ]
    aryFiles.extend(fnmatch.filter(os.listdir(Globals.names.pathData), Common.Constants.globTrials))
    aryFiles.sort(Common.cmpDataFilenames)
    aryFiles = [ os.path.join(Globals.names.pathData, strFile) for strFile in aryFiles ]

    aryData = []
    strPath = ''
    Common.say('Processing %d files (initial, final, and %d trials)' % (len(aryFiles), len(aryFiles)-2), False)
    try: aryData = [ [ urlGenome, Genome.Genome(urlGenome) ] for urlGenome in aryFiles if Common.say('.',False) ]
    except LookupError as err: raise Common.BiologicError('%s is missing one or more required elements or attributes - %s' % (urlGenome, str(err)))
    except urllib.error.URLError as err: raise Common.BiologicError('Unable to open URL %s - %s' % (urlGenome, str(err)))

    Common.say('')

    return aryData

#==============================================================================
# Main Routines
#==============================================================================
#------------------------------------------------------------------------------
# Function: svgCollectGridLines
# 
# Create the lines for a grid using the passed rectangle dimensions
#------------------------------------------------------------------------------
def svgCollectGridLines(rc, sxyGene):
    dGridMajor = int(round(Constants.dGridMajor / sxyGene))
    dGridMinor = int(round(Constants.dGridMinor / sxyGene))

    left = dGridMinor and Common.roundTo(rc.left, dGridMinor, False) or rc.left
    right = dGridMinor and Common.roundTo(rc.right, dGridMinor, True) or rc.right
    top = dGridMinor and Common.roundTo(rc.top, dGridMinor, True) or rc.top
    bottom = dGridMinor and Common.roundTo(rc.bottom, dGridMinor, False) or rc.bottom

    ary = []
    if dGridMajor and dGridMinor:
        ary += [ not x % dGridMajor and
                        ('<line class="%s" x1="%r" y1="%r" x2="%r" y2="%r" style="stroke-width: %rpx;" />' % (Constants.cssGridMajor, x, top*-1, x, bottom*-1, Constants.pxGridMajor / sxyGene)) or
                        ('<line class="%s" x1="%r" y1="%r" x2="%r" y2="%r" style="stroke-width: %rpx;" />' % (Constants.cssGridMinor, x, top*-1, x, bottom*-1, Constants.pxGridMinor / sxyGene)) for x in range(left, right+1, dGridMinor) if x ]
        ary += [ not y % dGridMajor and
                        ('<line class="%s" x1="%r" y1="%r" x2="%r" y2="%r" style="stroke-width: %rpx;" />' % (Constants.cssGridMajor, left, y*-1, right, y*-1, Constants.pxGridMajor / sxyGene)) or
                        ('<line class="%s" x1="%r" y1="%r" x2="%r" y2="%r" style="stroke-width: %rpx;" />' % (Constants.cssGridMinor, left, y*-1, right, y*-1, Constants.pxGridMinor / sxyGene)) for y in range(bottom, top+1, dGridMinor) if y ]

    ary += [ ('<line class="%s" x1="%r" y1="0" x2="%r" y2="0" style="stroke-width: %rpx;" />' % (Constants.cssAxis, left, right, Constants.pxAxis / sxyGene)),
            ('<line class="%s" x1="0" y1="%r" x2="0" y2="%r" style="stroke-width: %rpx;" />' % (Constants.cssAxis, top*-1, bottom*-1, Constants.pxAxis / sxyGene)) ]

    return '\n'.join(ary)
    
#------------------------------------------------------------------------------
# Function: svgCollectLines
# 
# Build the arrays for coherent, incoherent, marks, dropouts, and mutation lines.
# Each is returned as an array of arrays of tuples: The tuples contain the x,y points
# and each sub-array contains the tuples of a single, unbroken, coherent or
# incoherent series.
# 
# Note:
# Segment points *do not* include the first codon (ATG) of the gene.
# Base references into the array must, therefore, be reduced by one.
#------------------------------------------------------------------------------
def svgCollectLines(genome):
    gene = genome.gene
    
    aryCoherent = []
    aryIncoherent = []
    aryMarks = []
    aryDropouts = []
    aryMutations = []
    
    aryStrokes = gene.aryStrokes
    itStrokes = iter(aryStrokes)
    stroke = next(itStrokes)
    
    top = -sys.maxsize - 1
    left = sys.maxsize
    bottom = sys.maxsize
    right = -sys.maxsize - 1

    aryPoints = []
    if gene.arySegments:
        for segment in gene.arySegments:
            for pt in segment.aryPoints:
                top = max(top, pt.y)
                left = min(left, pt.x)
                bottom = min(bottom, pt.y)
                right = max(right, pt.x)

            aryPoints += segment.aryPoints[:-1]
        
            try:
                while stroke and segment.rgBases > stroke.rgBases: stroke = next(itStrokes)
            except StopIteration: stroke = None
            fInStroke = (stroke and stroke.rgBases.containsRange(segment.rgBases)) and True or False
        
            if segment.coherent:
                aryCoherent.append(segment.aryPoints)
                if not fInStroke: aryMarks.append(segment.aryPoints)
            else:
                aryIncoherent.append(segment.aryPoints)
                if fInStroke: aryDropouts.append(segment.aryPoints)

        aryPoints.append(gene.arySegments[-1].aryPoints[-1])

    if genome.lineage and genome.lineage.acceptedMutations:
        aryMutations = [ aryPoints[Codons.toCodonIndex(mutation.targetIndex-1)-1:Codons.toCodonIndex(mutation.targetIndex-1)+Codons.toCodonCount(mutation.countBases)]
                                for mutation in genome.lineage.acceptedMutations.mutations if not mutation.tag == 'deleted' ]
                                
    return (Genome.Rectangle(top=top, left=left, bottom=bottom, right=right), aryCoherent, aryIncoherent, aryMarks, aryDropouts, aryMutations)

#------------------------------------------------------------------------------
# Function: svgCollectLabels
# 
# Create the text entries for stroke and move labels
#------------------------------------------------------------------------------
def svgCollectLabels(gene, sxyGene):
    aryLabels = [ [], [] ]

    if gene.arySegments:
        itSegments = iter(gene.arySegments)
        segment = next(itSegments)

        iStroke = 1
        iMove = 1
        
        pxFontSize = max(Constants.pxFontSize / sxyGene, Constants.pxFontSizeScaledMin)
    
        if not segment.coherent:
            aryLabels[1].append('<text class="%s" x="%r" y="%r" font-size="%rpx">%d</text>' % (Constants.cssIncoherent,
                                                                                        segment.aryPoints[0].x,
                                                                                        segment.aryPoints[0].y*-1,
                                                                                        pxFontSize,
                                                                                        iMove))
            iMove += 1
    
        for stroke in gene.aryStrokes:
            try:
                while segment.rgBases.baseFirst != stroke.rgBases.baseFirst: segment = next(itSegments)
                aryLabels[0].append('<text class="%s" x="%r" y="%r" font-size="%rpx">%d</text>' % (Constants.cssCoherent,
                                                                                            segment.aryPoints[0].x,
                                                                                            segment.aryPoints[0].y*-1,
                                                                                            pxFontSize,
                                                                                            iStroke))
                iStroke += 1
            except StopIteration:
                break

            try:
                while segment.rgBases.baseFirst != stroke.rgBases.baseLast+1: segment = next(itSegments)
                aryLabels[1].append('<text class="%s" x="%r" y="%r" font-size="%rpx">%d</text>' % (Constants.cssIncoherent,
                                                                                            segment.aryPoints[0].x,
                                                                                            segment.aryPoints[0].y*-1,
                                                                                            pxFontSize,
                                                                                            iMove))
                iMove += 1
            except StopIteration:
                break

    return [ '\n'.join(aryLabels[0]), '\n'.join(aryLabels[1]) ]
    
#------------------------------------------------------------------------------
# Function: buildSVG
# 
# Generate the SVG files used by all XHTML
#------------------------------------------------------------------------------
def buildSVG(aryData, han, fImages):
    Common.say('Creating SVG files', False)

    fileDefault = ''
    iDefaultSVG = 0
    iDefaultIcon = 0
    dxDefaultOffset = 0
    
    svgOptions = fImages and Globals.svgImages or Globals.svgXHTML

    for strData, genome in aryData:
        Common.say('.', False)

        if not fImages and not fileDefault:
            nIcons = min(len(aryData) - (Constants.cDefaultIcons * iDefaultSVG), Constants.cDefaultIcons)
            dx = ((Constants.dxDefault + Constants.dxDefaultBorder) * nIcons) - Constants.dxDefaultBorder

            fileDefault = open(os.path.join(Globals.names.pathReport, 'default%d.svg' % iDefaultSVG), 'w')
            fileDefault.write(_SVG_HEADER % (dx, Constants.dyDefault, dx, Constants.dyDefault))
        
            iDefaultIcon = 0
            dxDefaultOffset = 0
            iDefaultSVG += 1

        strPath, strFile = os.path.split(strData)
        svgFile = os.path.join(Globals.names.pathReport, (os.path.splitext(strFile)[0]+'.svg'))

        fileTrial = open(svgFile, 'w')
        if fImages:
            fileTrial.write(_SVG_HEADER % (Constants.dxImage, Constants.dyImage, Constants.dxImage, Constants.dyImage))
        else:
            fileTrial.write(_SVG_HEADER % (Constants.dxTrial, Constants.dyTrial, Constants.dxTrial, Constants.dyTrial))

        rcGene, aryCoherent, aryIncoherent, aryMarks, aryDropouts, aryMutations = svgCollectLines(genome)

        if rcGene.width or rcGene.height:
            sxGene = rcGene.width >= 1 and (Constants.dxGene / rcGene.width) or 1
            syGene = rcGene.height >=1 and (Constants.dyGene / rcGene.height) or 1
            sxyGene = min(sxGene, syGene)

            dxGene = ((rcGene.left * sxyGene) * -1) + Constants.dxGeneOffset
            dyGene = (rcGene.top * sxyGene) + Constants.dyGeneOffset

            strHead = '<polyline class="%s" points="' % Constants.cssCoherent
            strTail = '" />'
            strCoherent = '\n'.join([ strHead + ' '.join([ '%r,%r' % (pt.x, pt.y*-1) for pt in aryPoints ]) + strTail for aryPoints in aryCoherent ])
        
            strHead = '<polyline class="%s" style="stroke-dasharray: %r,%r;" points="' % (Constants.cssIncoherent, Constants.pxDashArrayDash / sxyGene, Constants.pxDashArrayGap / sxyGene)
            strIncoherent = '\n'.join([ strHead + ' '.join([ '%r,%r' % (pt.x, pt.y*-1) for pt in aryPoints ]) + strTail for aryPoints in aryIncoherent ])
        
            strHead = '<polyline class="%s" points="' % Constants.cssMark
            strMarks = '\n'.join([ strHead + ' '.join([ '%r,%r' % (pt.x, pt.y*-1) for pt in aryPoints ]) + strTail for aryPoints in aryMarks ])
        
            strHead = '<polyline class="%s" points="' % Constants.cssDropout
            strDropouts = '\n'.join([ strHead + ' '.join([ '%r,%r' % (pt.x, pt.y*-1) for pt in aryPoints ]) + strTail for aryPoints in aryDropouts ])
        
            strHead = '<polyline class="%s" points="' % Constants.cssMutation
            strMutations = '\n'.join([ strHead + ' '.join([ '%r,%r' % (pt.x, pt.y*-1) for pt in aryPoints ]) + strTail for aryPoints in aryMutations ])
                                        
            strHead = '<circle class="%s"' % Constants.cssOverlap
            strTail = ' r="%r" />' % (Constants.pxOverlap / sxyGene)
            strOverlaps = '\n'.join([ (strHead + (' cx="%r" cy="%r"' % (overlap.pt.x, overlap.pt.y*-1)) + strTail) for overlap in genome.gene.aryOverlaps ])
                                        
            strHead = '<circle class="%s"' % Constants.cssCoherent
            strTail = ' r="%r" />' % (Constants.pxVertex / sxyGene)
            aryVertices = [ '\n'.join([ '\n'.join([ strHead + (' cx="%r" cy="%r"' % (pt.x, pt.y*-1)) + strTail for pt in aryPoints]) for aryPoints in aryCoherent ]) ]

            strHead = '<circle class="%s"' % Constants.cssIncoherent
            aryVertices.append('\n'.join([ '\n'.join([ strHead + (' cx="%r" cy="%r"' % (pt.x, pt.y*-1)) + strTail for pt in aryPoints]) for aryPoints in aryIncoherent ]))

            strHead = '<circle class="%s"' % Constants.cssMark
            aryVertices.append('\n'.join([ '\n'.join([ strHead + (' cx="%r" cy="%r"' % (pt.x, pt.y*-1)) + strTail for pt in aryPoints]) for aryPoints in aryMarks ]))

            strHead = '<circle class="%s"' % Constants.cssDropout
            aryVertices.append('\n'.join([ '\n'.join([ strHead + (' cx="%r" cy="%r"' % (pt.x, pt.y*-1)) + strTail for pt in aryPoints]) for aryPoints in aryDropouts ]))
        
            aryLabels = svgCollectLabels(genome.gene, sxyGene)

            pxFontSize = max(Constants.pxFontSize / sxyGene, Constants.pxFontSizeScaledMin)
            strBoundingGroups = '\n'.join([ '<rect class="%s" x="%r" y="%r" width="%r" height="%r" style="stroke-width: %rpx;" /><text class="%s" x="%r" y="%r" font-size="%r">%d</text>' %
                    (Constants.cssBoundingGroup,
                    genome.gene.aryGroups[i].bounds.left,
                    genome.gene.aryGroups[i].bounds.top*-1,
                    genome.gene.aryGroups[i].bounds.width,
                    genome.gene.aryGroups[i].bounds.height,
                    Constants.pxGridMajor / sxyGene,
                    Constants.cssBoundingGroup,
                    genome.gene.aryGroups[i].bounds.left + (pxFontSize / 4),
                    genome.gene.aryGroups[i].bounds.top*-1 + pxFontSize,
                    pxFontSize,
                    i+1) for i in range(0,len(genome.gene.aryGroups)) if genome.gene.aryGroups[i].bounds ])

            strBoundingStrokes = '\n'.join([ '<rect class="%s" x="%r" y="%r" width="%r" height="%r" style="stroke-width: %rpx;" />' %
                    (Constants.cssBoundingStroke,
                    s.bounds.left,
                    s.bounds.top*-1,
                    s.bounds.width,
                    s.bounds.height,
                    Constants.pxGridMajor / sxyGene) for s in genome.gene.aryStrokes if s.bounds ])

            fileTrial.write(_SVG_TRANSFORM_GROUP %
                            (Constants.cssGene, dxGene, dyGene, sxyGene, Constants.pxLine / sxyGene, ('%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s' %
                                (_SVG_DISPLAY_GROUP % ('grid', svgOptions.fGrid and 1.0 or 0.0, svgCollectGridLines(rcGene, sxyGene)),
                                _SVG_DISPLAY_GROUP % ('coherent', svgOptions.fCoherent and 1.0 or 0.0, strCoherent),
                                _SVG_DISPLAY_GROUP % ('incoherent', svgOptions.fIncoherent and 1.0 or 0.0, strIncoherent),
                                _SVG_DISPLAY_GROUP % ('marks', svgOptions.fNoise and 1.0 or 0.0, strMarks),
                                _SVG_DISPLAY_GROUP % ('dropouts', svgOptions.fNoise and 1.0 or 0.0, strDropouts),
                                _SVG_DISPLAY_GROUP % ('mutations', svgOptions.fMutations and 1.0 or 0.0, strMutations),
                                _SVG_DISPLAY_GROUP % ('overlaps', svgOptions.fOverlaps and 1.0 or 0.0, strOverlaps),
                                _SVG_DISPLAY_GROUP % ('vertices-coherent', (svgOptions.fCoherent and svgOptions.fVertices) and 1.0 or 0.0, aryVertices[0]),
                                _SVG_DISPLAY_GROUP % ('vertices-incoherent', (svgOptions.fIncoherent and svgOptions.fVertices) and 1.0 or 0.0, aryVertices[1]),
                                _SVG_DISPLAY_GROUP % ('vertices-marks', (svgOptions.fNoise and svgOptions.fVertices) and 1.0 or 0.0, aryVertices[2]),
                                _SVG_DISPLAY_GROUP % ('vertices-dropouts', (svgOptions.fNoise and svgOptions.fVertices) and 1.0 or 0.0, aryVertices[3]),
                                _SVG_DISPLAY_GROUP % ('labels-coherent', (svgOptions.fCoherent and svgOptions.fLabels) and 1.0 or 0.0, aryLabels[0]),
                                _SVG_DISPLAY_GROUP % ('labels-incoherent', (svgOptions.fIncoherent and svgOptions.fLabels) and 1.0 or 0.0, aryLabels[1]),
                                _SVG_DISPLAY_GROUP % ('bounding-group', svgOptions.fBounding and 1.0 or 0.0, strBoundingGroups),
                                _SVG_DISPLAY_GROUP % ('bounding-stroke', svgOptions.fBounding and 1.0 or 0.0, strBoundingStrokes)))))
        
            if not fImages:
                # Build the gene icon display (display coherent portions and marks only, both as ink)
                sxIcon = rcGene.width >= 1 and Constants.dxIcon / rcGene.width or 1
                syIcon = rcGene.height >=1 and Constants.dyIcon / rcGene.height or 1
                sxyIcon = min(sxIcon, syIcon)
        
                pxLine = max(Constants.pxLine / sxyIcon, Constants.pxLine)
                if ((pxLine * sxyIcon) > Constants.pxLineScaledMax):
                    pxLine = Constants.pxLineScaledMax / sxyIcon

                dxIcon = ((rcGene.left * sxyIcon) * -1) + Constants.dxIconOffset
                dyIcon = (rcGene.top * sxyIcon) + Constants.dyIconOffset

                strHead = '<polyline class="%s" points="' % Constants.cssCoherent
                strTail = '" />'
                strCoherent = '\n'.join([ strHead + ' '.join([ '%r,%r' % (pt.x, pt.y*-1) for pt in aryPoints ]) + strTail for aryPoints in (aryCoherent+aryMarks) ])
            
                fileTrial.write(_SVG_TRANSFORM_GROUP % (Constants.cssGene, dxIcon, dyIcon, sxyIcon, pxLine, '%s' % strCoherent))

        if not fImages:
            # Build the Han icon display
            rcHan = han.bounds
            sxIcon = rcHan.width >= 1 and Constants.dxIcon / rcHan.width or 1
            syIcon = rcHan.height >=1 and Constants.dyIcon / rcHan.height or 1
            sxyIcon = min(sxIcon, syIcon)
        
            pxLine = max(Constants.pxLine / sxyIcon, Constants.pxLine)
            if ((pxLine * sxyIcon) > Constants.pxLineScaledMax):
                pxLine = Constants.pxLineScaledMax / sxyIcon

            dxIcon = ((rcHan.left * sxyIcon) * -1) + Constants.dxHanIconOffset
            dyIcon = (rcHan.top * sxyIcon) + Constants.dyHanIconOffset

            strLine = '<polyline class="%s" points="' % Constants.cssHan
            fileTrial.write(_SVG_TRANSFORM_GROUP % (Constants.cssHan, dxIcon, dyIcon, sxyIcon, pxLine,
                                            '\n'.join([ strLine + ' '.join([ str(ptd.x) + ',' + str(ptd.y*-1) for ptd in hanStroke.aryPointsForward ]) + '" />' for hanStroke in han.aryStrokes ] )))

            # Write controls
            fileTrial.write(_SVG_TRIAL_CONTROLS % (svgOptions.fCoherent and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fIncoherent and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fNoise and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fNoise and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fMutations and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fOverlaps and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fLabels and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fVertices and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fGrid and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fBounding and Constants.fillVisible or Constants.fillInvisible,
                                                    svgOptions.fBounding and Constants.fillVisible or Constants.fillInvisible,
                                                    genome.gene.bounds and min((Codons.Constants.vectorShort * sxyGene), Constants.dxSmallVectorMax) or 0,
                                                    genome.gene.bounds and min((Codons.Constants.vectorMedium * sxyGene), Constants.dxMediumVectorMax) or 0,
                                                    genome.gene.bounds and min((Codons.Constants.vectorLong * sxyGene), Constants.dxLongVectorMax) or 0,
                                                    genome.statistics.fitness or 0,
                                                    svgOptions.fVertices and 1.0 or 0.0,
                                                    svgOptions.fLabels and 1.0 or 0.0))

        fileTrial.write(_SVG_FOOTER)
        fileTrial.close()

        if not fImages:
            # Add the gene icon to the default collection
            sxIcon = rcGene.width >= 1 and Constants.dxDefaultIcon / rcGene.width or 1
            syIcon = rcGene.height >=1 and Constants.dyDefaultIcon / rcGene.height or 1
            sxyIcon = min(sxIcon, syIcon)
    
            pxLine = max(Constants.pxLine / sxyIcon, Constants.pxLine)
            if ((pxLine * sxyIcon) > Constants.pxLineScaledMax):
                pxLine = Constants.pxLineScaledMax / sxyIcon

            dxIcon = ((rcGene.left * sxyIcon) * -1) + dxDefaultOffset + Constants.dxDefaultIconOffset
            dyIcon = (rcGene.top * sxyIcon) + Constants.dyDefaultIconOffset 

            fileDefault.write(_SVG_DEFAULT_ICON % (dxDefaultOffset, Constants.dxDefault, Constants.dyDefault,
                                                    dxIcon, dyIcon, sxyIcon, pxLine,
                                                    '%s%s' % (strCoherent, strMarks),
                                                    dxDefaultOffset + Constants.dxDefaultIconOffset, genome.statistics.fitness or 0))

            iDefaultIcon += 1
            dxDefaultOffset += Constants.dxDefault + Constants.dxDefaultBorder
            if iDefaultIcon >= Constants.cDefaultIcons:
                fileDefault.write(_SVG_FOOTER)
                fileDefault.close()
                fileDefault = ''

    if fileDefault:
        fileDefault.write(_SVG_FOOTER)
        fileDefault.close()

    Common.say('')
    return
    
#------------------------------------------------------------------------------
# Function: buildExpandedLists
#
# This routine does two things: It creates the expanded.js file in the target
# directory (which contains javascript object with entries for each trial)
# and it updates its own entry in expanded.js file of the parent directory
# (if it exists).
# 
# expanded.js contains a Javascript object describing trial links. The HTML
# pages rely on it, rather than hard-coded links, so that as more HTML pages
# get created, the links updated automatically.
#------------------------------------------------------------------------------
def buildExpandedLists(aryData):
    aryTrials = [ strTrial for strTrial, genome in aryData if not Common.Constants.filenameInitial in strTrial != Common.Constants.filenameInitial and not Common.Constants.filenameFinal in strTrial ]

    fileExpanded = open(os.path.join(Globals.names.pathReport, Constants.fileExpanded), 'w')
    fileExpanded.write('var objExpanded = {\n')
    fileExpanded.write(',\n'.join([ '%s:0' % os.path.splitext(os.path.basename(strTrial))[0] for strTrial in aryTrials ]))
    fileExpanded.write('\n};\n')
    fileExpanded.close()
    
    strParent, strCurrent = os.path.split(Globals.names.pathReport)
    try:
        fileExpanded = open(os.path.join(strParent, Constants.fileExpanded), 'r+w')
        strExpanded = fileExpanded.read()
        if strExpanded:
            i = strExpanded.find(strCurrent)
            if i > 0:
                strExpanded = strExpanded[:i+len(strCurrent)+1] + '1' + strExpanded[i+len(strCurrent)+2:]
                fileExpanded.seek(0)
                fileExpanded.write(strExpanded)
        fileExpanded.close()
    except IOError as err:
        pass
    return
    
#------------------------------------------------------------------------------
# Function: htmlCollectCodons
#
# Gather codon related details for display
#------------------------------------------------------------------------------
def htmlCollectCodons(genome):
    gene = genome.gene
    aryStrokes = gene.aryStrokes
    
    cBases = len(genome.bases)
    cStrokeBases = sum([ stroke.rgBases.length() for stroke in aryStrokes ])

    strStrokeDetails = ''
    for iStrokeStart in range(0,len(aryStrokes),Constants.dStrokeRangesPerLine):
        iStrokeEnd = min(iStrokeStart + Constants.dStrokeRangesPerLine, len(aryStrokes))
        strStrokeDetails += "<table class='stroke-range'><tr><th>Stroke</th>%s</tr><tr><th>Bases</th>%s</tr><tr><th>Han #</th>%s</tr></table>" % (
                                    ''.join([ '<td><a href="#s%d">%d</a></td>' % (i, i+1) for i in range(iStrokeStart,iStrokeEnd) ]),
                                    ''.join([ '<td><a href="#s%d">%04d&mdash;%04d</a></td>' % (i, aryStrokes[i].rgBases.baseFirst, aryStrokes[i].rgBases.baseLast) for i in range(iStrokeStart,iStrokeEnd) ]),
                                    ''.join([ '<td>%d</td>' % aryStrokes[i].correspondsTo for i in range(iStrokeStart,iStrokeEnd) ]))
                                
    strDigits = ' '.join([ '%d%d%d' % (i%10,(i+1)%10,(i+2)%10) for i in range(1,Constants.dBasesPerLine,3) ])
    
    strCodonTable = ''
    if not genome.codonTable.isStandard():
        strCodonTable += '<h3>Codon Table - %s</h3>' % genome.codonTable.uuid
        aryEntries = genome.codonTable.entries()
        for iEntryRow in range(0,len(aryEntries),Constants.dCodonTableEntriesPerLine):
            aryEntriesRow = aryEntries[iEntryRow:iEntryRow+Constants.dCodonTableEntriesPerLine]
            strCodonTableRow = ''
            for iEntrySet in range(0,len(aryEntriesRow),Constants.dCodonTableEntriesPerSet):
                aryEntriesSet = aryEntriesRow[iEntrySet:iEntrySet+Constants.dCodonTableEntriesPerSet]
                strCodonTableRow += "<td><table class='codon-row'><tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>" % (aryEntriesSet[0][0], aryEntriesSet[1][0], aryEntriesSet[2][0], aryEntriesSet[3][0])
                strCodonTableRow += "<tr><td class='%s'>%s</td><td class='%s'>%s</td><td class='%s'>%s</td><td class='%s'>%s</td></tr></table></td>" % (
                                        (genome.codonTable.hasStandardMapping(aryEntriesSet[0][0]) and 'acids' or 'acid-nonstandard'),
                                        aryEntriesSet[0][1],
                                        (genome.codonTable.hasStandardMapping(aryEntriesSet[1][0]) and 'acids' or 'acid-nonstandard'),
                                        aryEntriesSet[1][1],
                                        (genome.codonTable.hasStandardMapping(aryEntriesSet[2][0]) and 'acids' or 'acid-nonstandard'),
                                        aryEntriesSet[2][1],
                                        (genome.codonTable.hasStandardMapping(aryEntriesSet[3][0]) and 'acids' or 'acid-nonstandard'),
                                        aryEntriesSet[3][1])
            strCodonTable += "<table class='codon-table'>%s</table>" % strCodonTableRow
    
    iStroke = 0
    strCodonDetails = ''    
    for i in range(0,len(genome.bases),Constants.dBasesPerLine):
        bases = genome.bases[i:i+Constants.dBasesPerLine]
        
        aryCodons = Codons.Constants.reSPLITCODONS.findall(bases)
        aryVectors = list(map(genome.codonTable.codonToVectorName, aryCodons))
        
        nBases = len(aryCodons) * 3

        while iStroke < len(aryStrokes):
            stroke = aryStrokes[iStroke]
            iBase = stroke.rgBases.baseFirst - i - 1
            if iBase < nBases:
                iCodon = max(int(iBase/3),0)
                aryCodons[iCodon] = ('<span id="s%d" class="stroke-range">' % iStroke) + aryCodons[iCodon]
            iBase = stroke.rgBases.baseLast - i - 1
            if iBase >= 0 and iBase < nBases:
                iCodon = int(iBase/3)
                aryCodons[iCodon] += '</span>'
                iStroke += 1
            else:
                break

        strCodonDetails += _HTML_TRIAL_CODON_ROW % (i+1, i+Constants.dBasesPerLine,
                                                    strDigits,
                                                     ' '.join(aryCodons),
                                                     ' '.join(aryVectors))

    return (cBases, cStrokeBases, strStrokeDetails, strCodonTable, strCodonDetails)

#------------------------------------------------------------------------------
# Function: buildXHTML
#
# Generate XHTML output
#------------------------------------------------------------------------------
def buildXHTML(aryData, strUnicode, han):
    Common.say('Creating XHTML output', False)

    aryTrials = [ [ os.path.splitext(os.path.basename(strData))[0]+'.html', genome ] for strData, genome in aryData ]

    # Build hyperlink path from the genome down
    strDefaultNavigation = '&nbsp;'
    strTrialNavigation = '&nbsp;'
    aryPathNavigation = []
    strPath = Globals.names.idGenome
    if Globals.names.pathExpandReport:
        strPath = os.path.join(strPath, Globals.names.pathExpandReport)
    strTail = os.path.split(strPath)[1]
    nDepth = 0
    while strPath:
        strPath, strDir = os.path.split(strPath)
        aryPath = [ '..' for i in range(nDepth) ]
        aryPath.append('default.html')
        aryPathNavigation.append('<a class="pathpart" href="%s">%s</a>' % (os.path.join(*aryPath), strDir))
        nDepth += 1
    aryPathNavigation.append('&nbsp;')
    aryPathNavigation.reverse()

    strDefaultNavigation = '/'.join(aryPathNavigation[:-1])
    if not Globals.names.strExpand:
        strDefaultNavigation = '%s%s' % ( '<span class="sibling-prev">&nbsp;</span><span class="sibling-down">&nbsp;</span><span class="sibling-next">&nbsp</span>',
                                            '/'.join([ strDefaultNavigation, ('<span class="pathpart">%s</span>' % strTail) ]))
    else:
        strDefaultNavigation = '%s%s' % ( '<span class="sibling-prev">&nbsp;</span><span class="sibling-down">&nbsp;</span><span class="sibling-next">&nbsp</span>',
                                            '/'.join([ strDefaultNavigation, ('<a class="pathpart" href="../%s.html">%s</a>' % (strTail, strTail)) ]))

    strTrialNavigation = '/'.join(aryPathNavigation)

    # Build the default HTML file
    fileDefault = None
    genome = aryTrials[-1][1]
    statistics = genome.statistics
    fileDefault = open(os.path.join(Globals.names.pathReport, 'default.html'), 'w')
    fileDefault.write(_HTML_HEADER % (statistics.trialFirst, statistics.trialLast))

    strDate, strTime = genome.creationDate and genome.creationDate.split('T') or [ '', '' ]
    strAuthor = genome.author or ''
    if genome.creationTool:
        strCreationTool = genome.creationTool + (genome.creationParameters and (' &mdash; ' + genome.creationParameters) or '')
        strAuthor += strAuthor and (' (%s)' % strCreationTool) or strCreationTool
    fileDefault.write(_HTML_TITLE_SECTION % (strUnicode,
                                            statistics.trialFirst, statistics.trialLast,
                                            strDate, strTime[:-1], strAuthor,
                                            strDefaultNavigation))

    fileDefault.write(_HTML_HEADER_SECTION % (statistics.fitness,
                                            statistics.maxFitness.value, statistics.maxFitness.trial,
                                            statistics.minFitness.value, statistics.minFitness.trial,
                                            statistics.score,
                                            statistics.maxScore.value, statistics.maxScore.trial,
                                            statistics.minScore.value, statistics.minScore.trial,
                                            statistics.totalRollbacks,
                                            statistics.maxRollbacks.countRollbacks, statistics.maxRollbacks.trial,
                                            statistics.minRollbacks.countRollbacks, statistics.minRollbacks.trial,
                                            statistics.changeMutations.accepted, statistics.changeMutations.silent,
                                            statistics.copyMutations.accepted,
                                            statistics.deletionMutations.accepted,
                                            statistics.insertionMutations.accepted,
                                            statistics.transposeMutations.accepted))

    strIcons = ''
    iDefaultSVG = 0
    for iTrialFirst in range(0, len(aryTrials), Constants.cDefaultIcons):
        nIcons = min(len(aryTrials) - (Constants.cDefaultIcons * iDefaultSVG), Constants.cDefaultIcons)
        dxRow = ((Constants.dxDefault + Constants.dxDefaultBorder) * nIcons) - Constants.dxDefaultBorder
        iTrialLast = min(iTrialFirst+Constants.cDefaultIcons, len(aryTrials))
        strLinks = '\n'.join([ '<th><a href="%s">%d&ndash;%d</a></th>' % (aryTrials[i][0], aryTrials[i][1].statistics.trialFirst, aryTrials[i][1].statistics.trialLast) for i in range(iTrialFirst,iTrialLast) ])
        strIcons += _HTML_DEFAULT_ROW % (strLinks, nIcons, dxRow, Constants.dyDefault, 'default%d.svg' % iDefaultSVG)
        iDefaultSVG += 1
    
    fileDefault.write(_HTML_DEFAULT_MAIN % strIcons)

    fileDefault.write(_HTML_FOOTER)

    for iTrial in range(len(aryTrials)):
        strTrial = aryTrials[iTrial][0]
        genome = aryTrials[iTrial][1]
        gene = genome.gene
        statistics = genome.statistics

        fileTrial = open(os.path.join(Globals.names.pathReport, strTrial), 'w')
        fileTrial.write(_HTML_HEADER % ((not statistics and -1 or statistics.trialFirst), (not statistics and -1 or statistics.trialLast)))

        strTrialPrev = iTrial > 1 and ('<a class="sibling-prev" href="%s">&#171;%s</a>' % (aryTrials[iTrial-1][0], Common.Constants.reTRIALFILE.match(aryTrials[iTrial-1][0]).groups()[0])) or (iTrial == 1 and '<a class="sibling-prev" href="initial.html">&#171;initial</a>' or '<span class="sibling-prev">&nbsp;</span>')
        strTrialNext = iTrial == len(aryTrials)-1 and '<span class="sibling-next">&nbsp;</span>' or (iTrial == len(aryTrials)-2 and '<a class="sibling-next" href="final.html">final&#187;</a>' or ('<a class="sibling-next" href="%s">%s&#187;</a>' % (aryTrials[iTrial+1][0], Common.Constants.reTRIALFILE.match(aryTrials[iTrial+1][0]).groups()[0])))

        strDate, strTime = genome.creationDate and genome.creationDate.split('T') or [ '', '' ]
        strAuthor = genome.author or ''
        if genome.creationTool:
            strCreationTool = genome.creationTool + (genome.creationParameters and (' &mdash; ' + genome.creationParameters) or '')
            strAuthor += strAuthor and (' (%s)' % strCreationTool) or strCreationTool
        fileTrial.write(_HTML_TITLE_SECTION % (strUnicode,
                                            (not statistics and -1 or statistics.trialFirst),
                                            (not statistics and -1 or statistics.trialLast),
                                            strDate, strTime[:-1], strAuthor,
                                            '%s%s%s%s' % (strTrialPrev, '<script type="text/javascript">document.write(getExpandedLink());</script>', strTrialNext, '/'.join([ strTrialNavigation, os.path.splitext(strTrial)[0] ]))))

        if statistics:
            fileTrial.write(_HTML_HEADER_SECTION % (statistics.fitness or 0,
                                                    statistics.maxFitness and statistics.maxFitness.value or 0,
                                                    not statistics.maxFitness and -1 or statistics.maxFitness.trial,
                                                    statistics.minFitness and statistics.minFitness.value or 0,
                                                    not statistics.minFitness and -1 or statistics.minFitness.trial,
                                                    statistics.score or 0,
                                                    statistics.maxScore and statistics.maxScore.value or 0,
                                                    not statistics.maxScore and -1 or statistics.maxScore.trial,
                                                    statistics.minScore and statistics.minScore.value or 0,
                                                    not statistics.minScore and -1 or statistics.minScore.trial,
                                                    statistics.totalRollbacks or 0,
                                                    statistics.maxRollbacks and statistics.maxRollbacks.countRollbacks or 0,
                                                    not statistics.maxRollbacks and -1 or statistics.maxRollbacks.trial,
                                                    statistics.minRollbacks and statistics.minRollbacks.countRollbacks or 0,
                                                    not statistics.minRollbacks and -1 or statistics.minRollbacks.trial,
                                                    statistics.changeMutations and statistics.changeMutations.accepted or 0,
                                                    statistics.changeMutations and statistics.changeMutations.silent or 0,
                                                    statistics.copyMutations and statistics.copyMutations.accepted or 0,
                                                    statistics.deletionMutations and statistics.deletionMutations.accepted or 0,
                                                    statistics.insertionMutations and statistics.insertionMutations.accepted or 0,
                                                    statistics.transposeMutations and statistics.transposeMutations.accepted or 0))
        else:
            fileTrial.write(_HTML_HEADER_SECTION % (0, 0, -1, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0))

        strSVG = _HTML_TRIAL_ROW % ('svg', 'svg',
                                    Globals.fHTMLSVG and '-' or '+',
                                    'Gene',
                                    _HTML_TRIAL_CANVAS % ('svg',
                                                        Globals.fHTMLSVG and 'block; border: none;' or 'none; border: none;',
                                                        '<embed id="svg" width="%d" height="%d" src="%s" />' % (Constants.dxTrial, Constants.dyTrial, os.path.splitext(strTrial)[0] + '.svg')))

        strScores = ''
        if gene.score:
            strScores = _HTML_TRIAL_ROW % ('scores', 'scores',
                                            Globals.fHTMLScores and '-' or '+',
                                            'Scores',
                                            _HTML_TRIAL_CANVAS % ('scores',
                                                            Globals.fHTMLScores and 'block' or 'none',
                                                            _HTML_TRIAL_SCORES % (statistics.fitness, statistics.countBases, statistics.units,
                                                                                gene.score, gene.scale or 0, gene.placement or 0, gene.illegalOverlaps or 0, gene.missingOverlaps or 0, gene.marks or 0,
                                                                                '\n'.join([ _HTML_TRIAL_GROUP_SCORE % (g.score or 0, g.deviation or 0, g.extraLength or 0, g.scale or 0, g.placement or 0, g.illegalOverlaps or 0, g.missingOverlaps or 0, g.dropouts or 0) for g in gene.aryGroups ]),
                                                                                '\n'.join([ _HTML_TRIAL_STROKE_SCORE % (s.deviation or 0, s.extraLength or 0, s.dropouts or 0) for s in gene.aryStrokes]))))

        strMutations = ''
        if genome.lineage and (genome.lineage.acceptedMutations or genome.lineage.rejectedMutations):
            strMutations = _HTML_TRIAL_ROW % ('mutations', 'mutations',
                                            Globals.fHTMLMutations and '-' or '+',
                                            'Mutations',
                                            _HTML_TRIAL_CANVAS % ('mutations',
                                                                Globals.fHTMLMutations and 'block' or 'none',
                                                                '<h3>Accepted</h3><ol>%s</ol><div class="rejected"><h3>Rejected - %d</h3>%s</div>' % (
                                                                                    genome.lineage.acceptedMutations and '\n'.join([ '<li>%s</li>' % str(m) for m in genome.lineage.acceptedMutations.mutations ]) or '',
                                                                                    len(genome.lineage.rejectedMutations),
                                                                                    '\n'.join([ '<div class="reject" style="background-color: %s;"><h4>%s</h4><ol>%s</ol></div>' % (
                                                                                                        (i % 2) and '#fff' or '#eee',
                                                                                                        genome.lineage.rejectedMutations[i].description,
                                                                                                        '\n'.join([ '<li>%s</li>' % str(m) for m in genome.lineage.rejectedMutations[i].mutations ])) for i in range(len(genome.lineage.rejectedMutations)) ]))))

        cBases, cStrokeBases, strStrokeDetails, strCodonTable, strCodonDetails = htmlCollectCodons(genome)
        strCodons = _HTML_TRIAL_ROW % ('codons', 'codons',
                                        Globals.fHTMLCodons and '-' or '+',
                                        'Codons',
                                        _HTML_TRIAL_CANVAS % ('codons',
                                                            Globals.fHTMLCodons and 'block' or 'none',
                                                            _HTML_TRIAL_CODONS % (cBases, (cBases / 3), cStrokeBases, (cStrokeBases / 3), (cBases-cStrokeBases), ((cBases-cStrokeBases) / 3), len(genome.gene.aryStrokes), strStrokeDetails, strCodonTable, strCodonDetails)))

        fileTrial.write(_HTML_TRIAL_MAIN % ('%s%s%s%s' % (strSVG, strScores, strMutations, strCodons)))

        fileTrial.close()
        Common.say('.', False)

    buildExpandedLists(aryData)

    Common.say('')
    buildSVG(aryData, han, False)
    return
    
#------------------------------------------------------------------------------
# Function: buildPDB
#
# Generate PDB output
#------------------------------------------------------------------------------
def buildPDB(aryData):
    Common.say('Creating PDB output for trials %s' % ','.join([ str(iTrial) for iTrial in Globals.aryPDB[0] ]))

    iData = 0
    Globals.aryPDB[0].sort(Common.cmpDataFilenames)
    
    nPitch = Globals.aryPDB[1]
    nScale = Globals.aryPDB[2]

    for strTrial in Globals.aryPDB[0]:
        strTrialFile = (strTrial != 'initial' and strTrial != 'final' and ('trial' + strTrial) or strTrial) + '.xml'

        while iData < len(aryData):
            nCmp = Common.cmpDataFilenames(strTrialFile, os.path.basename(aryData[iData][0]))
            if nCmp <= 0: 
                break;
            iData += 1

        if iData >= len(aryData) or nCmp < 0:
            Common.sayError('Skipping PDB creation for %s - %s is not a valid trial' % (strTrial, strTrial))
            continue
            
        Common.say('Creating PDB file for trial %s' % strTrial)
        urlGenome, genome = aryData[iData]

        filePDB = open(os.path.join(Globals.names.pathReport, os.path.splitext(os.path.basename(urlGenome))[0]+'.pdb'), 'w')
        filePDB.write(_PDB_HEADER)
        filePDB.write(_PDB_TITLE % "")
        if genome.statistics and genome.statistics.trialLast:
            filePDB.write(_PDB_TITLE % ('Trial %d created on %s' % (genome.statistics.trialLast, genome.creationDate or 'unknown')))
        if genome.author:
            filePDB.write(_PDB_AUTHOR % genome.author)
        filePDB.write('\n')
        filePDB.write(_PDB_REMARK4)
        filePDB.write('\n')
        filePDB.write(_PDB_ORIG)
        filePDB.write(_PDB_SCALE)
        filePDB.write('\n')

        aryHETATM = []
        aryCONECT = []
        iSerial = 1
        z = 0

        for segment in genome.gene.arySegments:
            if segment.coherent:
                cChain = 'c'
                zIncrement = 0
            else:
                cChain = 'i'
                zIncrement = nPitch

            iChain = 1
            for pt in segment.aryPoints:
                aryHETATM.append(_PDB_HETATM % (iSerial, cChain, iChain, (pt.x * nScale), (pt.y * nScale), (z * nScale)))
                aryCONECT.append(_PDB_CONECT % (iSerial-1, iSerial))
                iSerial += 1
                iChain += 1
                z += zIncrement
		
        del aryCONECT[0]

        filePDB.write('\n'.join(aryHETATM))
        filePDB.write('\n')
        filePDB.write('\n'.join(aryCONECT))
        filePDB.write('\n')

        filePDB.write(_PDB_MASTER % (1, len(aryHETATM), len(aryCONECT)))
        filePDB.close()

    return
        
def main(argv=None):
    try:
        getArguments()

        Common.say('Data Directory  : %s' % Globals.names.pathData)
        Common.say('Report Directory: %s' % Globals.names.pathReport)
        Common.say('Genome URL      : %s' % Globals.urls.urlGenome)
        Common.say('Han URL         : %s' % Globals.urls.urlHan)
        Common.say('HTML URL        : %s' % Globals.urls.urlHTML)
        
        aryData = loadData();

        if Globals.fXHTML or Globals.fImages:
            strUnicode = aryData[0][1].gene.str
            try: han = loadHan(strUnicode)
            except LookupError as err: raise XMLError(aryData[0][0])

            # TODO: Replace these copies with direct links to the content
            Common.copyURLToFile(Common.pathToURL(Constants.fileCSS, Globals.urls.urlHTML), os.path.join(Globals.names.pathReport, Constants.fileCSS))
            Common.copyURLToFile(Common.pathToURL(Constants.fileScript, Globals.urls.urlHTML), os.path.join(Globals.names.pathReport, Constants.fileScript))
            Common.copyURLToFile(Common.pathToURL(Constants.fileSVGScript, Globals.urls.urlHTML), os.path.join(Globals.names.pathReport, Constants.fileSVGScript))
            
            if Globals.fImages:
                buildSVG(aryData, han, True)
        
            if Globals.fXHTML:
                buildXHTML(aryData, strUnicode, han)
    
        if Globals.aryPDB[0]:
            buildPDB(aryData)

        return 0
        
    except Usage as err:
        Common.sayError(err)
        return 0

    except Common.BiologicError as err:
        Common.sayError(err)
        return 2

if __name__ == "__main__":
    sys.exit(main())
