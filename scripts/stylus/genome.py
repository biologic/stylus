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
genome.py

This script encapsulates classes for working with Stylus Genomes and
Han defintions built from XML generated dictionaries.

NOTE:
This script assumes a single gene within the genome.

Stylus, Copyright 2006-2009 Biologic Institute.
'''

import codons as Codons
import fpconst
import math
import re
import sys
import urllib2
import xmldict

#------------------------------------------------------------------------------
# Class: BezierCurve
# 
#------------------------------------------------------------------------------
class BezierCurve(object):
    def __init__(self, ptStart, ptControl, ptEnd):
        self.ptStart = ptStart
        self.ptControl = ptControl
        self.ptEnd = ptEnd
        
        self.length = 0
        self.maxLength = Codons.Constants.vectorMedium * 3 * 5  # Assumes an HCF grid of about 500,500
        self.maxDepth = 3                                       # This is an arbitrary depth - deeper yields more points
        return
        
    #------------------------------------------------------------------------------
    # Function: expandPoints
    # 
    # This routine uses the de Casteljau algorithm relying on the fact that any bezier
    # curve may be divided into smaller bezier curves. Recursion continues until the
    # distance between the curve endpoints is small enough for a useful line approximation
    # or the maximum level of recursion is reached.
    # 
    # For a quadratic bezier curve bounded by P1 and P2 with a control point of
    # C, the algorithm divides the curve into two smaller curves as follows:
    # - Initialize the point array with P1, then
    # - If the distance from P1 to P2 is sufficiently small, save P2
    # - Otherwise
    # 	- Connect P1 to C, and P2 to C, Label P1 as L1 and P2 as R2
    # 	- Proportionally segment P1-C and P2-C in the ratio of t and (1-t)
    #     Label the dividing point on P1-C C1 and the dividing point on P2-C C2
    #     (t is the standard bezier curve parameter ranging between 0 and 1; the
    #     code below bisects the curve by using t equal to 0.5 - that is, it takes the
    #     midpoints)
    # 	- Connect C1 to C2 and segment using the same ratio as previously
    #     Label the dividing points L2 and R1
    # 	- Recurse with (L1, C1, L2) and (R1, C2, R2)
    #------------------------------------------------------------------------------
    def expandPoints(self):
        return self._expandPoints(self.ptStart, self.ptControl, self.ptEnd, 0)
        
    def _expandPoints(self, ptStart, ptControl, ptEnd, depth):
        dx = ptStart.x - ptEnd.x
        dy = ptStart.y - ptEnd.y
        length = math.sqrt((dx*dx)+(dy*dy))
        
        if depth >= self.maxDepth or length <= self.maxLength:
            return [ ptStart, ptEnd ]
            
        else:
            ptL1 = ptStart
            ptC1 = getMidpoint(ptStart, ptControl)
            ptC2 = getMidpoint(ptControl, ptEnd)
            ptR2 = ptEnd
            ptL2 = getMidpoint(ptC1, ptC2)
            ptR1 = ptL2
            
            depth += 1
            return self._expandPoints(ptL1, ptC1, ptL2, depth)[:-1] + self._expandPoints(ptR1, ptC2, ptR2, depth)
            
#------------------------------------------------------------------------------
# Class: Point
# 
#------------------------------------------------------------------------------
class Point(object):
    def __init__(self, x=None, y=None, dictPoint=None, pt=None, isControl=False):
        self.x = x
        self.y = y

        if dictPoint:
            self.x = float(dictPoint['x'])
            self.y = float(dictPoint['y'])
        elif pt:
            self.x = pt.x
            self.y = pt.y
        self.fControl = isControl
        return
        
    def __str__(self):
        return '(%r,%r)' % (self.x, self.y)
        
#------------------------------------------------------------------------------
# Function: getMidpoint
# 
#------------------------------------------------------------------------------
def getMidpoint(pt1, pt2):
    return Point(x=(pt1.x + ((pt2.x - pt1.x) / 2)), y=(pt1.y + ((pt2.y - pt1.y) / 2)))

#------------------------------------------------------------------------------
# Class: PointDistance
# 
#------------------------------------------------------------------------------
class PointDistance(Point):
    def __init__(self, x=None, y=None, distance=None, dictPoint=None, ptd=None):
        Point.__init__(self, x=x, y=y, dictPoint=dictPoint, pt=ptd)
        self.distance = distance
        if dictPoint:
            self.distance = float(dictPoint['fractionalDistance'])
        elif ptd:
            self.distance = ptd.distance
        return
        
    def __str__(self):
        return '(%r,%r,%r)' % (self.x, self.y, self.distance)


#------------------------------------------------------------------------------
# Function: getPointBetween
# 
#------------------------------------------------------------------------------
def getPointBetween(aryPtd, fractionalDistance):
    if not fractionalDistance:
        return aryPtd[0]
    elif fractionalDistance == 1:
        return aryPtd[-1]
    else:
        for i in xrange(1,len(aryPtd)):
            if aryPtd[i].distance >= fractionalDistance:
                break
        ptdStart = aryPtd[i-1]
        ptdEnd = aryPtd[i]
        return Point(x=ptdStart.x + ((ptdEnd.x - ptdStart.x) * ((fractionalDistance - ptdStart.distance) / (ptdEnd.distance - ptdStart.distance))),
                    y=ptdStart.y + ((ptdEnd.y - ptdStart.y) * ((fractionalDistance - ptdStart.distance) / (ptdEnd.distance - ptdStart.distance))))
        
#------------------------------------------------------------------------------
# Class: Line
# 
#------------------------------------------------------------------------------
class Line(object):
    __maxDiagonalSlope = (math.sin(math.radians(67.5)) / math.cos(math.radians(67.5)))
    __minDiagonalSlope = (math.sin(math.radians(22.5)) / math.cos(math.radians(22.5)))

    def __init__(self, ptStart, ptEnd):
        self.__ptStart = ptStart
        self.__ptEnd = ptEnd
        self.__clear()
        
    def __getStart(self):
        return self.__ptStart
    def __setStart(self, ptStart):
        self.__ptStart = ptStart
        self.__clear()
    ptStart = property(__getStart, __setStart)

    def __getEnd(self):
        return self.__ptEnd
    def __setEnd(self, ptEnd):
        self.__ptEnd = ptEnd
        self.__clear()
    ptEnd = property(__getEnd, __setEnd)
    
    def __clear(self):
        self.__dx = None
        self.__dy = None
        self.__length = None
        self.__slope = None
        self.__direction = None
        self.__ptMid = None
        
    def __getDX(self):
        if not self.__dx:
            self.__dx = self.__ptEnd.x - self.__ptStart.x
        return self.__dx
    dx = property(__getDX)
    
    def __getDY(self):
        if not self.__dy:
            self.__dy = self.__ptEnd.y - self.__ptStart.y
        return self.__dy
    dy = property(__getDY)
    
    def __getLength(self):
        if not self.__length:
            self.__length = math.sqrt((self.dx*self.dx)+(self.dy*self.dy))
        return self.__length
    length = property(__getLength)
    
    def __getSlope(self):
        if not self.__slope:
            try: self.__slope = self.dy / self.dx
            except ZeroDivisionError: self.__slope = self.dy < 0 and fpconst.NegInf or fpconst.PosInf
        return self.__slope
    slope = property(__getSlope)
    
    def __getDirection(self):
        if not self.__direction:
            if not self.dx and not self.dy:
                self.__direction = Codons.Directions.Stop
                
            else:
                # Infinite up is North
                if self.slope == fpconst.PosInf:
                    self.__direction = Codons.Directions.North
                    
                # Infinite down is South
                elif self.slope == fpconst.NegInf:
                    self.__direction = Codons.Directions.South
                    
                # Zero slopes head East or West
                elif not self.slope:
                    self.__direction = self.dx > 0 and Codons.Directions.East or Codons.Directions.West
                
                # Positive Slopes in quadrants I or III
                elif self.slope > 0:
                    # Tending toward infinity - treat as North or South
                    if self.slope >= Line.__maxDiagonalSlope:
                        self.__direction = self.dy > 0 and Codons.Directions.North or Codons.Directions.South
                        
                    # Tending toward zero - treat as East or West
                    elif self.slope <= Line.__minDiagonalSlope:
                        self.__direction = self.dy > 0 and Codons.Directions.East or Codons.Directions.West
                    
                    # Around 45 degrees - treat as Northeast or Southwest
                    else:
                        self.__direction = self.dy > 0 and Codons.Directions.Northeast or Codons.Directions.Southwest
                        
                # Negative Slopes in quadrants II or IV
                else: 
                    # Tending toward negative infinity - treat as North or South
                    if self.slope <= -Line.__maxDiagonalSlope:
                        self.__direction = self.dy > 0 and Codons.Directions.North or Codons.Directions.South
                        
                    # Tending toward zero - treat as East or West
                    elif self.slope >= -Line.__minDiagonalSlope:
                        self.__direction = self.dy > 0 and Codons.Directions.West or Codons.Directions.East
                        
                    # Around 45 degrees - treat as Northwest or Southeast
                    else:
                        self.__direction = self.dy > 0 and Codons.Directions.Northwest or Codons.Directions.Southeast
        return self.__direction
    direction = property(__getDirection)
    
    def __getMidpoint(self):
        if not self.__ptMid:
            self.__ptMid = getMidpoint(self.__ptStart, self.__ptEnd)
        return self.__ptMid
    ptMid = property(__getMidpoint)
    
    def __str__(self):
        return '(%s-%s, dir(%s) len(%r), slope(%r))' % (str(self.__ptStart), str(self.__ptEnd), Codons.directionToName(self.direction), self.length, self.slope)
        
#------------------------------------------------------------------------------
# Class: Range
# 
#------------------------------------------------------------------------------
class Range(object):
    def __init__(self, dictRange):
        self.baseFirst = int(dictRange['baseFirst'])
        self.baseLast = int(dictRange['baseLast'])
        return

    def __cmp__(self, rg):
        if self.baseLast < rg.baseFirst: return -1
        elif self.baseFirst > rg.baseLast: return 1
        else: return 0
            
    def __str__(self):
        return 'Range(%d,%d)' % (self.baseFirst, self.baseLast)
            
    def containsRange(self, rg):
        return (self.baseFirst <= rg.baseFirst and self.baseLast >= rg.baseLast) and True or False
            
    def containsIndex(self, baseIndex):
        return (self.baseFirst <= baseIndex and self.baseLast >= baseIndex) and True or False
        
    def length(self):
        return self.baseLast - self.baseFirst + 1

#------------------------------------------------------------------------------
# Class: Rectangle
# 
#------------------------------------------------------------------------------
class Rectangle(object):
    def __init__(self, top=-sys.maxint-1, left=sys.maxint, bottom=sys.maxint, right=-sys.maxint, dictRect=None, rect=None):
        self.__set(top, left, bottom, right)

        if dictRect:
            self.top = float(dictRect['top'])
            self.left = float(dictRect['left'])
            self.bottom = float(dictRect['bottom'])
            self.right = float(dictRect['right'])
            self.width = float(dictRect['width'])
            self.height = float(dictRect['height'])
            self.ptCenter = Point(x=float(dictRect['x-midpoint']), y=float(dictRect['y-midpoint']))
        elif rect:
            self.top = rect.top
            self.left = rect.left
            self.bottom = rect.bottom
            self.right = rect.right
            self.width = rect.width
            self.height = rect.height
            self.ptCenter.x = rect.ptCenter.x
            self.ptCenter.y = rect.ptCenter.y
        return

    def __str__(self):
        return "top='%r' left='%r' bottom='%r' right='%s' width='%r' height='%r' x-midpoint='%r' y-midpoint='%r'" % (self.top, self.left, self.bottom, self.right,
                                                                                                                    self.width, self.height,
                                                                                                                    self.ptCenter.x, self.ptCenter.y)
        
    def __set(self, top, left, bottom, right):
        self.top = top
        self.left = left
        self.bottom = bottom
        self.right = right
        self.width = self.right - self.left
        self.height = self.top - self.bottom
        self.ptCenter = Point(x=(self.left + (self.width / 2)), y=(self.bottom + (self.height / 2)))
        return

    def unionRect(self, rect):
        self.__set(max(self.top, rect.top), min(self.left, rect.left), min(self.bottom, rect.bottom), max(self.right, rect.right))
        return

    def unionPoint(self, pt):
        self.__set(max(self.top, pt.y), min(self.left, pt.x), min(self.bottom, pt.y), max(self.right, pt.x))
        return
        
#------------------------------------------------------------------------------
# Class: Group
# 
#------------------------------------------------------------------------------
class Group(object):
    def __init__(self, dictGroup):
        self.bounds = None

        self.sxToHan = None
        self.syToHan = None
        self.dxToHan = None
        self.dyToHan = None

        self.score = None
        self.scale = None
        self.placement = None
        self.deviation = None
        self.extraLength = None
        self.illegalOverlaps = None
        self.missingOverlaps = None
        self.dropouts = None

        if 'bounds' in dictGroup:
            self.bounds = Rectangle(dictRect=dictGroup['bounds'])
        
        if 'scale' in dictGroup:
            dictScale = dictGroup['scale']
            self.sxToHan = float(dictScale['sxToHan'])
            self.syToHan = float(dictScale['syToHan'])

        if 'translation' in dictGroup:
            dictTranslation = dictGroup['translation']
            self.dxToHan = float(dictTranslation['dxToHan'])
            self.dyToHan = float(dictTranslation['dyToHan'])

        if 'score' in dictGroup:
            dictScore = dictGroup['score']
            self.score = float(dictScore['score'])

            if 'scale' in dictScore:
                self.scale = float(dictScore['scale'])
            if 'placement' in dictScore:
                self.placement = float(dictScore['placement'])
            if 'deviation' in dictScore:
                self.deviation = float(dictScore['deviation'])
            if 'extraLength' in dictScore:
                self.extraLength = float(dictScore['extraLength'])
            if 'illegalOverlaps' in dictScore:
                self.illegalOverlaps = float(dictScore['illegalOverlaps'])
            if 'missingOverlaps' in dictScore:
                self.missingOverlaps = float(dictScore['missingOverlaps'])
            if 'dropouts' in dictScore:
                self.dropouts = float(dictScore['dropouts'])

        self.containedStrokes = [ int(nStroke)-1 for nStroke in dictGroup['containedStrokes'][xmldict.XMLDict.value].split() ]
        return

#------------------------------------------------------------------------------
# Class: Stroke
# 
#------------------------------------------------------------------------------
class Stroke(object):
    def __init__(self, dictStroke):
        self.rgBases = Range(dictStroke)
        self.correspondsTo = int(dictStroke['correspondsTo'])

        self.bounds = None

        self.sxToHan = None
        self.syToHan = None
        self.dxToHan = None
        self.dyToHan = None

        self.deviation = None
        self.extraLength = None
        self.dropouts = None

        if 'bounds' in dictStroke:
            self.bounds = Rectangle(dictRect=dictStroke['bounds'])
        
        if 'scale' in dictStroke:
            dictScale = dictStroke['scale']
            self.sxToHan = float(dictScale['sxToHan'])
            self.syToHan = float(dictScale['syToHan'])

        if 'translation' in dictStroke:
            dictTranslation = dictStroke['translation']
            self.dxToHan = float(dictTranslation['dxToHan'])
            self.dyToHan = float(dictTranslation['dyToHan'])

        if 'score' in dictStroke:
            dictScore = dictStroke['score']
            if 'deviation' in dictScore:
                self.deviation = float(dictScore['deviation'])
            if 'extraLength' in dictScore:
                self.extraLength = float(dictScore['extraLength'])
            if 'dropouts' in dictScore:
                self.dropouts = float(dictScore['dropouts'])
        return

#------------------------------------------------------------------------------
# Class: Overlap
# 
#------------------------------------------------------------------------------
class Overlap(object):
    def __init__(self, dictOverlap):
        self.firstStroke = dictOverlap['firstStroke']
        self.secondStroke = dictOverlap['secondStroke']
        self.pt = Point(dictPoint=dictOverlap)
        return

#------------------------------------------------------------------------------
# Class: Segment
# 
#------------------------------------------------------------------------------
class Segment(object):
    def __init__(self, dictSegment):
        self.rgBases = Range(dictSegment)
        self.coherent = dictSegment['coherent'] == 'true' and True or False
        self.aryPoints = [ Point(dictPoint=dictPoint) for tag, dictPoint in dictSegment[xmldict.XMLDict.children] ]
        return

#------------------------------------------------------------------------------
# Class: Gene
# 
#------------------------------------------------------------------------------
class Gene(object):
    def __init__(self, dictGene):
        self.rgBases = Range(dictGene)
        
        self.xOrigin = 0.0
        self.yOrigin = 0.0
        if 'origin' in dictGene:
            dictOrigin = dictGene['origin']
            self.xOrigin = 'x' in dictOrigin and float(dictOrigin['x']) or 0.0
            self.yOrigin = 'y' in dictOrigin and float(dictOrigin['y']) or 0.0
        
        self.unicode = None
        self.bounds = None

        self.sxToHan = None
        self.syToHan = None
        self.dxToHan = None
        self.dyToHan = None

        self.score = None
        self.scale = None
        self.placement = None
        self.illegalOverlaps = None
        self.missingOverlaps = None
        self.marks = None
        
        self.aryGroups = []
        self.aryStrokes = []
        self.aryOverlaps = []
        self.arySegments = []

        if 'hanReferences' in dictGene:
            dictHanReference = dictGene['hanReferences'][xmldict.XMLDict.children][0][1]
            self.unicode = dictHanReference['unicode']

            if 'bounds' in dictHanReference:
                self.bounds = Rectangle(dictRect=dictHanReference['bounds'])

            if 'scale' in dictHanReference:
                dictScale = dictHanReference['scale']
                self.sxToHan = float(dictScale['sxToHan'])
                self.syToHan = float(dictScale['syToHan'])
                
            if 'translation' in dictHanReference:
                dictTranslation = dictHanReference['translation']
                self.dxToHan = float(dictTranslation['dxToHan'])
                self.dyToHan = float(dictTranslation['dyToHan'])

            if 'score' in dictHanReference:
                dictScore = dictHanReference['score']
                self.score = float(dictScore['score'])
                if 'scale' in dictScore:
                    self.scale = float(dictScore['scale'])
                if 'placement' in dictScore:
                    self.placement = float(dictScore['placement'])
                if 'illegalOverlaps' in dictScore:
                    self.illegalOverlaps = float(dictScore['illegalOverlaps'])
                if 'missingOverlaps' in dictScore:
                    self.missingOverlaps = float(dictScore['missingOverlaps'])
                if 'marks' in dictScore:
                    self.marks = float(dictScore['marks'])

            if 'groups' in dictHanReference:
                self.aryGroups = [ Group(dictGroup) for tag, dictGroup in dictHanReference['groups'][xmldict.XMLDict.children] ]
            if 'strokes' in dictHanReference:
                self.aryStrokes = [ Stroke(dictStroke) for tag, dictStroke in dictHanReference['strokes'][xmldict.XMLDict.children] ]
            if 'overlaps' in dictHanReference:
                self.aryOverlaps = [ Overlap(dictOverlap) for tag, dictOverlap in dictHanReference['overlaps'][xmldict.XMLDict.children] ]
            if 'segments' in dictGene:
                self.arySegments = [ Segment(dictSegment) for tag, dictSegment in dictGene['segments'][xmldict.XMLDict.children] ]
        return

#------------------------------------------------------------------------------
# Class: Mutation
# 
#------------------------------------------------------------------------------
class Mutation(object):
    def __init__(self, tag, dictMutation):
        self.tag = tag

        self.sourceIndex = 'sourceIndex' in dictMutation and int(dictMutation['sourceIndex']) or None
        self.targetIndex = 'targetIndex' in dictMutation and int(dictMutation['targetIndex']) or None
        self.countBases = 'countBases' in dictMutation and int(dictMutation['countBases']) or 1
        self.bases = 'bases' in dictMutation and dictMutation['bases'] or None
        self.basesAfter = 'basesAfter' in dictMutation and dictMutation['basesAfter'] or None
        return
        
    def __str__(self):
        if self.tag == 'changed': return 'Changed %s (%d bases) to %s at %d' % (self.bases, self.countBases, self.basesAfter, self.targetIndex)
        elif self.tag == 'copied': return 'Copied %s (%d bases) from %d to %d' % (self.bases, self.countBases, self.sourceIndex, self.targetIndex)
        elif self.tag == 'deleted': return 'Deleted %s (%d bases) from %d' % (self.bases, self.countBases, self.targetIndex)
        elif self.tag == 'inserted': return 'Inserted %s (%d bases) at %d' % (self.bases, self.countBases, self.targetIndex)
        elif self.tag == 'transposed': return 'Transposed %s (%d bases) from %d to %d' % (self.bases, self.countBases, self.sourceIndex, self.targetIndex)
        else: return 'Unknown mutation'
        
#------------------------------------------------------------------------------
# Class: Mutations
# 
#------------------------------------------------------------------------------
class Mutations(object):
    _reLOGFILE = re.compile(r'\[.*\]\s')

    def __init__(self, dictMutations=None):
        self.description = 'description' in dictMutations and Mutations._reLOGFILE.sub('', dictMutations['description'], 1) or None
        self.mutations = [ Mutation(tag, dictMutation) for tag, dictMutation in dictMutations[xmldict.XMLDict.children] ]
        return
        
#------------------------------------------------------------------------------
# Class: Lineage
# 
#------------------------------------------------------------------------------
class Lineage(object):
    def __init__(self, dictGenome):
        self.strain = None
        self.ancestors = None
        self.acceptedMutations = None
        self.rejectedMutations = []

        if 'lineage' in dictGenome:
            dictLineage = dictGenome['lineage']

            if 'strain' in dictLineage:
                self.strain = dictLineage['strain']

            if 'ancestors' in dictLineage:
                self.ancestors = dictLineage['ancestors'].split()

            if 'acceptedMutations' in dictLineage:
                self.acceptedMutations = Mutations(dictLineage['acceptedMutations'])

            if 'rejectedMutations' in dictLineage:
                self.rejectedMutations = [ Mutations(dictAttempt) for tag, dictAttempt in dictLineage['rejectedMutations'][xmldict.XMLDict.children] ]
        return

#------------------------------------------------------------------------------
# Class: Termination
# 
#------------------------------------------------------------------------------
class Termination(object):
    def __init__(self, dictGenome):
        self.code = None
        self.reason = None
        self.description = ''

        if 'termination' in dictGenome:
            dictTermination = dictGenome['termination']
            self.code = dictTermination['terminationCode']
            self.reason = dictTermination['reasonCode']
            self.description = dictTermination['description']
        return

#------------------------------------------------------------------------------
# Class: ValueStatistics
# 
#------------------------------------------------------------------------------
class ValueStatistics(object):
    def __init__(self, dictValue):
        self.value = float(dictValue['value'])
        self.trial = int(dictValue['trial'])
        return

#------------------------------------------------------------------------------
# Class: SizeStatistics
# 
#------------------------------------------------------------------------------
class SizeStatistics(object):
    def __init__(self, dictSizes):
        self.countBases = int(dictSizes['countBases'])
        self.trial = int(dictSizes['trial'])
        return

#------------------------------------------------------------------------------
# Class: MutationStatistics
# 
#------------------------------------------------------------------------------
class MutationStatistics(object):
    def __init__(self, dictMutations):
        self.accepted = int(dictMutations['accepted'])
        self.attempted = int(dictMutations['attempted'])
        self.countBases = int(dictMutations['countBases'])
        return

#------------------------------------------------------------------------------
# Class: RollbackStatistics
# 
#------------------------------------------------------------------------------
class RollbackStatistics(object):
    def __init__(self, dictRollbacks):
        self.countRollbacks = int(dictRollbacks['countRollbacks'])
        self.trial = int(dictRollbacks['trial'])
        return
        
#------------------------------------------------------------------------------
# Class: Statistics
# 
#------------------------------------------------------------------------------
class Statistics(object):
    def __init__(self, dictGenome):
        self.trialFirst = None
        self.trialLast = None

        self.score = None
        self.units = None
        self.cost = None
        self.fitness = None

        self.maxScore = None
        self.minScore = None
        
        self.maxUnits = None
        self.minUnits = None
        
        self.maxCost = None
        self.minCost = None
        
        self.maxFitness = None
        self.minFitness = None

        self.countBases = None
        self.basesChanged = None
        self.basesDeleted = None
        self.basesInserted = None
        self.maxBases = None
        self.minBases = None
    
        self.changeMutations = None
        self.copyMutations = None
        self.deletionMutations = None
        self.insertionMutations = None
        self.transposeMutations = None

        self.countRollbacks = None
        self.totalRollbacks = None
        self.maxRollbacks = None
        self.minRollbacks = None

        if 'statistics' in dictGenome:
            dictStatistics = dictGenome['statistics']

            self.trialFirst = int(dictStatistics['trialFirst'])
            self.trialLast = int(dictStatistics['trialLast'])

            if 'score' in dictStatistics:
                self.score = float(dictStatistics['score'])
            if 'units' in dictStatistics:
                self.units = float(dictStatistics['units'])
            if 'cost' in dictStatistics:
                self.cost = float(dictStatistics['cost'])
            if 'fitness' in dictStatistics:
                self.fitness = float(dictStatistics['fitness'])
            if 'countBases' in dictStatistics:
                self.countBases = int(dictStatistics['countBases'])
            if 'countRollbacks' in dictStatistics:
                self.countRollbacks = int(dictStatistics['countRollbacks'])

            if 'scoreRange' in dictStatistics:
                dictScores = dictStatistics['scoreRange']
                self.maxScore = ValueStatistics(dictScores['maximum'])
                self.minScore = ValueStatistics(dictScores['minimum'])

            if 'unitsRange' in dictStatistics:
                dictUnits = dictStatistics['unitsRange']
                self.maxUnits = ValueStatistics(dictUnits['maximum'])
                self.minUnits = ValueStatistics(dictUnits['minimum'])

            if 'costRange' in dictStatistics:
                dictCosts = dictStatistics['costRange']
                self.maxCost = ValueStatistics(dictCosts['maximum'])
                self.minCost = ValueStatistics(dictCosts['minimum'])

            if 'fitnessRange' in dictStatistics:
                dictFitnessLevels = dictStatistics['fitnessRange']
                self.maxFitness = ValueStatistics(dictFitnessLevels['maximum'])
                self.minFitness = ValueStatistics(dictFitnessLevels['minimum'])

            if 'bases' in dictStatistics:
                dictBases = dictStatistics['bases']
                self.basesChanged = int(dictBases['changed'])
                self.basesDeleted = int(dictBases['deleted'])
                self.basesInserted = int(dictBases['inserted'])
                self.maxBases = SizeStatistics(dictBases['maximum'])
                self.minBases = SizeStatistics(dictBases['minimum'])

            if 'rollbacks' in dictStatistics:
                dictRollbacks = dictStatistics['rollbacks']
                self.totalRollbacks = int(dictRollbacks['countRollbacks'])
                self.maxRollbacks = RollbackStatistics(dictRollbacks['maximum'])
                self.minRollbacks = RollbackStatistics(dictRollbacks['minimum'])

            if 'mutations' in dictStatistics:
                dictMutations = dictStatistics['mutations']
                self.changeMutations = MutationStatistics(dictMutations['changes'])
                self.changeMutations.silent = int(dictMutations['changes']['silent'])
                self.copyMutations = MutationStatistics(dictMutations['copies'])
                self.deletionMutations = MutationStatistics(dictMutations['deletions'])
                self.insertionMutations = MutationStatistics(dictMutations['insertions'])
                self.transposeMutations = MutationStatistics(dictMutations['transpositions'])
        return

#------------------------------------------------------------------------------
# Class: CodonTableElement
# 
#------------------------------------------------------------------------------
class CodonTableElement(Codons.CodonTable):
    def __init__(self, dictCodonTable=None, dictGenome=None):
        super(CodonTableElement, self).__init__()
        
        if dictGenome:
            if 'codonTable' in dictGenome:
                dictCodonTable = dictGenome['codonTable']
        
        if dictCodonTable:
            self.uuid = 'uuid' in dictCodonTable and dictCodonTable['uuid'].upper() or None

            self.author = 'author' in dictCodonTable and dictCodonTable['author'] or None
            self.creationDate = 'creationDate' in dictCodonTable and dictCodonTable['creationDate'] or None
            self.creationTool = 'creationTool' in dictCodonTable and dictCodonTable['creationTool'] or None
            self.creationParameters = 'creationParameters' in dictCodonTable and dictCodonTable['creationParameters'] or None

            for tag, dictEntry in dictCodonTable[xmldict.XMLDict.children]:
                codon = 'codon' in dictEntry and dictEntry['codon'] or None
                vector = 'vector' in dictEntry and dictEntry['vector'] or None
                if not codon or not vector:
                    raise BiologicError('A codon codonTable entry is missing the codon, vector, or both')

                self.mapCodonToVector(codon, vector)
        return

#------------------------------------------------------------------------------
# Class: CodonTable
# 
#------------------------------------------------------------------------------
class CodonTable(CodonTableElement):
    def __init__(self, urlCodonTable=None):
        dictCodonTable = None
        if urlCodonTable:
            xmlDict = xmldict.XMLDict({
                                    xmldict.XMLDict.toList : [ 'codonTable' ],
                                    xmldict.XMLDict.ignore : []
                                    })
            dictCodonTable = xmlDict.load(urlCodonTable)['codonTable']

        super(CodonTable, self).__init__(dictCodonTable=dictCodonTable)
        return

#------------------------------------------------------------------------------
# Class: Genome
# 
#------------------------------------------------------------------------------
class Genome(object):
    def __init__(self, urlGenome):
        xmlDict = xmldict.XMLDict({
                                xmldict.XMLDict.toList : [ 'codonTable', 'acceptedMutations', 'rejectedMutations', 'attempt', 'genes', 'hanReferences', 'groups', 'strokes', 'overlaps', 'segments', 'segment' ],
                                xmldict.XMLDict.ignore : []
                                })
        dictGenome = xmlDict.load(urlGenome)['genome']

        self.uuid = dictGenome['uuid'].upper()

        self.author = 'author' in dictGenome and dictGenome['author'] or None
        self.creationDate = 'creationDate' in dictGenome and dictGenome['creationDate'] or None
        self.creationTool = 'creationTool' in dictGenome and dictGenome['creationTool'] or None
        self.creationParameters = 'creationParameters' in dictGenome and dictGenome['creationParameters'] or None

        if 'seed' in dictGenome:
            self.seed = dictGenome['seed'][xmldict.XMLDict.value]
            self.seedID = dictGenome['seed']['processorID']
        else:
            self.seed = None
            self.seedID = None

        self.codonTable = CodonTableElement(dictGenome=dictGenome)
        self.bases = dictGenome['bases'][xmldict.XMLDict.value]

        self.termination = Termination(dictGenome)
        self.statistics = Statistics(dictGenome)
        self.lineage = Lineage(dictGenome)
        self.gene = Gene(dictGenome['genes'][xmldict.XMLDict.children][0][1])
        return

#------------------------------------------------------------------------------
# Class: HanGroup
# 
#------------------------------------------------------------------------------
class HanGroup(object):
    def __init__(self, dictGroup):
        self.bounds = Rectangle(dictRect=dictGroup['bounds'])
        self.length = dictGroup['length'][xmldict.XMLDict.value]
        self.weightedCenter = Point(dictPoint=dictGroup['weightedCenter'])
        self.containedStrokes = [ int(strStroke)-1 for strStroke in dictGroup['containedStrokes'][xmldict.XMLDict.value].split() ]
        return

#------------------------------------------------------------------------------
# Class: HanStroke
# 
#------------------------------------------------------------------------------
class HanStroke(object):
    def __init__(self, dictStroke):
        self.bounds = Rectangle(dictRect=dictStroke['bounds'])
        self.length = dictStroke['length'][xmldict.XMLDict.value]
        
        dictPoints = dictStroke['points']
        self.aryPointsForward = [ PointDistance(dictPoint=dictPoint) for tag, dictPoint in dictPoints['forward'][xmldict.XMLDict.children] ]
        self.aryPointsReverse = [ PointDistance(dictPoint=dictPoint) for tag, dictPoint in dictPoints['reverse'][xmldict.XMLDict.children] ]
        return

#------------------------------------------------------------------------------
# Class: HanOverlap
# 
#------------------------------------------------------------------------------
class HanOverlap(object):
    def __init__(self, dictOverlap):
        self.firstStroke = dictOverlap['firstStroke']
        self.secondStroke = dictOverlap['secondStroke']
        self.required = dictOverlap['required'] == 'true' and True or False
        return

#------------------------------------------------------------------------------
# Class: Han
# 
#------------------------------------------------------------------------------
class Han(object):
    def __init__(self, urlHan):
        xmlDict = xmldict.XMLDict({ xmldict.XMLDict.toList : [ 'groups', 'strokes', 'forward', 'reverse', 'overlaps' ] })
        dictHan = xmlDict.load(urlHan)['hanDefinition']
        
        self.uuid = dictHan['uuid']
        self.unicode = dictHan['unicode']

        self.creationDate = 'creationDate' in dictHan and dictHan['creationDate'] or None
        self.creationTool = 'creationTool' in dictHan and dictHan['creationTool'] or None
        self.creationParameters = 'creationParameters' in dictHan and dictHan['creationParameters'] or None

        self.bounds = Rectangle(dictRect=dictHan['bounds'])
        self.length = dictHan['length'][xmldict.XMLDict.value]
        self.minimumStrokeLength = dictHan['minimumStrokeLength'][xmldict.XMLDict.value]

        self.aryGroups = [ HanGroup(dictGroup) for tag, dictGroup in dictHan['groups'][xmldict.XMLDict.children] ]
        self.aryStrokes = [ HanStroke(dictStroke) for tag, dictStroke in dictHan['strokes'][xmldict.XMLDict.children] ]
        self.aryOverlaps = 'overlaps' in dictHan and [ HanOverlap(dictOverlap) for tag, dictOverlap in dictHan['overlaps'][xmldict.XMLDict.children] ] or []
        return

#------------------------------------------------------------------------------
# Class: HCFError
#
#------------------------------------------------------------------------------
class HCFError(Exception):
    def __init__(self, msg):
        self.msg = ''
        if msg and len(msg) > 0:
            self.msg = 'Error: ' + msg
    
    def __str__(self):
        return self.msg

#------------------------------------------------------------------------------
# Class: HCFStroke
# 
#------------------------------------------------------------------------------
class HCFStroke(object):
    def __init__(self, aryPoints):
        
        point1 = aryPoints[0]
        iPoint = 1
        
        self.bounds = Rectangle(top=point1.y, left=point1.x, bottom=point1.y, right=point1.x)
        self.length = 0
        self.aryPointsForward = [ PointDistance(x=point1.x, y=point1.y, distance=self.length) ]
        
        while iPoint < len(aryPoints):
            point2 = aryPoints[iPoint]
            if point2.fControl:
                iPoint += 1
                pointControl = point2
                point2 = aryPoints[iPoint]
                aryPts = BezierCurve(point1, pointControl, point2).expandPoints()
            else:
                aryPts = [ point1, point2 ]

            pt1 = aryPts[0]
            for pt2 in aryPts[1:]:
                self.bounds.unionPoint(pt2)
                
                dx = pt1.x - pt2.x
                dy = pt1.y - pt2.y
                self.length += math.sqrt((dx*dx)+(dy*dy))
                self.aryPointsForward.append(PointDistance(x=pt2.x, y=pt2.y, distance=self.length))
                
                pt1 = pt2
                
            point1 = point2
            iPoint += 1

        for hcfPtd in self.aryPointsForward:
            hcfPtd.distance /= self.length
        self.aryPointsReverse = [ PointDistance(x=hcfPtd.x, y=hcfPtd.y, distance=(1-hcfPtd.distance)) for hcfPtd in self.aryPointsForward[::-1] ]
        return
        
    def __str__(self):
        return 'bounds[%s] len(%r) \nforward(%s\n) \nreverse(%s)' % (str(self.bounds), self.length, ' '.join([ ('\n%s' % str(p)) for p in self.aryPointsForward]), ' '.join([ str(p) for p in self.aryPointsReverse]))

#------------------------------------------------------------------------------
# Class: HCFGroup
# 
#------------------------------------------------------------------------------
class HCFGroup(object):
    def __init__(self, strStrokes, aryStrokes):
        self.bounds = Rectangle()
        self.length = 0
        self.containedStrokes = [ int(i)-1 for i in strStrokes.split(',') ]

        self.ptCenter = Point(x=0, y=0)
        for i in self.containedStrokes:
            hcfStroke = aryStrokes[i]
            
            self.bounds.unionRect(hcfStroke.bounds)
            self.length += hcfStroke.length

            self.ptCenter.x += hcfStroke.bounds.ptCenter.x * hcfStroke.length
            self.ptCenter.y += hcfStroke.bounds.ptCenter.y * hcfStroke.length
            
        self.ptCenter.x /= self.length
        self.ptCenter.y /= self.length
        return
        
    def __str__(self):
        return 'bounds[%s] len(%r) center%s strokes(%s)' % (str(self.bounds), self.length, str(self.ptCenter), ' '.join([ str(i+1) for i in self.containedStrokes]))

#------------------------------------------------------------------------------
# Class: HCFOverlap
# 
#------------------------------------------------------------------------------
class HCFOverlap(object):
    def __init__(self, strOverlap):
        aryOverlap = strOverlap.split(',')
        self.firstStroke = int(aryOverlap[0])
        self.secondStroke = int(aryOverlap[1])
        self.fRequired = int(aryOverlap[2]) and True or False
        return
        
    def __str__(self):
        return '(%d,%d,%s)' % (self.firstStroke, self.secondStroke, self.fRequired and 'true' or 'false')

#------------------------------------------------------------------------------
# Class: HCF
# 
#------------------------------------------------------------------------------
class HCF(object):
    _rstrUnicode = r'[A-F\d]{4,5}'
    _rstrDigits = r'[\+|\-]?\d+'

    _reHan = re.compile(r'han:(%s)(?::\d+:\d+)?' % _rstrUnicode)
    _reStroke = re.compile(r'stroke:\(\w+\)(%s,%s(?::c?%s,%s)+)' % (_rstrDigits, _rstrDigits, _rstrDigits, _rstrDigits))
    _rePoint = re.compile(r'(c)?(%s),(%s)' % (_rstrDigits, _rstrDigits))
    _reGroup = re.compile(r'group:(\d+(?:,\d+)*)')
    _reOverlap = re.compile(r'overlap:(\d+,\d+,[0|1])')
    
    def __init__(self, urlHCF):
        self.unicode = ''
        self.bounds = Rectangle()
        self.length = 0
        self.minimumStrokeLength = sys.maxint
        self.aryStrokes = []
        self.aryGroups = []
        self.aryOverlaps = []

        try: fileHCF = urllib2.urlopen(urlHCF)
        except urllib2.URLError, err: raise HCFError('Unable to open URL %s - %s' % (urlHCF, str(err)))
        
        for l in fileHCF.readlines():
            mo = HCF._reHan.match(l)
            if mo:
                self.unicode = mo.groups()[0].upper()
                continue
                
            mo = HCF._reStroke.match(l)
            if mo:
                aryPoints = []
                for strPoint in mo.groups()[0].split(':'):
                    pt = HCF._rePoint.match(strPoint).groups()
                    aryPoints.append(Point(x=float(pt[1]), y=float(pt[2]), isControl=(pt[0] and True or False)))

                if aryPoints[0].fControl:
                    raise HCFError('A stroke in %s improperly begins control point' % urlHCF)
                fWasControl = False
                for pt in aryPoints[1:]:
                    if pt.fControl and fWasControl:
                        raise HCFError('A stroke in %s contains an improperly bounded control point' % urlHCF)
                    fWasControl = pt.fControl
                if fWasControl:
                    raise HCFError('A stroke in %s improperly ends control point' % urlHCF)

                hcfStroke = HCFStroke(aryPoints)
                self.bounds.unionRect(hcfStroke.bounds)
                self.length += hcfStroke.length
                self.minimumStrokeLength = min(hcfStroke.length, self.minimumStrokeLength)
                self.aryStrokes.append(hcfStroke)
                continue
                
            mo = HCF._reGroup.match(l)
            if mo:
                self.aryGroups.append(mo.groups()[0])
                continue
                
            mo = HCF._reOverlap.match(l)
            if mo:
                self.aryOverlaps.append(HCFOverlap(mo.groups()[0]))
                continue
        
        self.aryGroups = [ HCFGroup(strStrokes, self.aryStrokes) for strStrokes in self.aryGroups ]
        
    def __str__(self):
        return 'unicode(%s) bounds[%s] len(%r) minStrokeLen(%r)\nStrokes:\n%s\nGroups:\n%s\nOverlaps:\n%s' % (self.unicode,
                                                                                                        str(self.bounds),
                                                                                                        self.length,
                                                                                                        self.minimumStrokeLength,
                                                                                                        '\n'.join([ str(s) for s in self.aryStrokes]),
                                                                                                        '\n'.join([ str(g) for g in self.aryGroups]),
                                                                                                        '\n'.join([ str(o) for o in self.aryOverlaps]))
