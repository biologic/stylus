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
xmldict.py

Stylus, Copyright 2006-2008 Biologic Institute.
'''

import os
import shutil
import sys
import time
import urllib2
import urlparse
import xml.sax
import xml.sax.handler

class XMLDictError(Exception):
    def __init__(self, msg='Internal Error'):
        self.msg = 'Error: ' + msg + '\n'

class XMLDict(object):
    '''
    This class loads XML data (from strings or files) converting the XML into a hierarchy of ordered
    dictionarires with entries for all elements and attributes. Generally, elements become dictionaries
    with attributes and sub-elements accessible by name. Textual content, if any - and stripped of
    whitespace, is saved under its own key (XMLDict.value). Lastly, recurring elements (that is, those
    appearing multiple times within a parent element) become a list of dictionaries (in the order visited).

    Callers can modify the behavior by providing a dictionary of options with the following recognized
    keys:
    
        XMLDict.toList - Store child elements as an ordered list of tuples with the form (tag, dict) accessible
                        under the key of XMLDict.children (required for elements that may have repeating content)
        XMLDict.ignore - Do not include listed elements or their descendents

    '''
    __scheme_FILE = 'file://'
    
    toList = 'toList'
    ignore = 'ignore'

    value = ':value'
    children = ':children'

    class __SAXHandler(xml.sax.handler.ContentHandler):
        def __init__(self, dictDocument, dictOptions = {}):
            self.__dictDocument = dictDocument
            self.__dictOptions = dictOptions
            self.__path = ''
            self.__stack = []
            self.__text = ''
            self.__cPruning = 0
            return

        def startDocument(self):
            self.__stack.append(self.__dictDocument)
            return

        def endDocument(self):
            self.__stack.pop()
            return

        def startElement(self, tag, attrs):
            if tag in self.__dictOptions[XMLDict.ignore]:
                self.__cPruning += 1

            if not self.__cPruning:
                dictParent = self.__stack[-1]
                dictElement = dict(attrs.items())
                
                if tag in self.__dictOptions[XMLDict.toList]:
                    dictElement[XMLDict.children] = []

                if XMLDict.children in dictParent:
                    dictParent[XMLDict.children].append((tag, dictElement))
                else:
                    dictParent[tag] = dictElement

                self.__stack.append(dictElement)
                self.__path += '/' + tag
            return

        def endElement(self, tag):
            if not self.__cPruning:
                self.__text = self.__text.strip()
                if self.__text:
                    self.__stack[-1][XMLDict.value] = self.__text
                    self.__text = ''
                i = self.__path.rfind('/')
                if i >= 0:
                    self.__path = self.__path[:i]
                self.__stack.pop()
                
            elif tag in self.__dictOptions[XMLDict.ignore]:
                self.__cPruning -= 1
            return
            
        def characters(self, str):
            self.__text += str.encode('utf-8')
            return
    
    def __init__(self, dictOptions={}):
        self.__dictOptions = dict(dictOptions)
        if not XMLDict.toList in self.__dictOptions:
            self.__dictOptions[XMLDict.toList] = []
        if not XMLDict.ignore in self.__dictOptions:
            self.__dictOptions[XMLDict.ignore] = []
        return

    def __openFile(strPath):
        if strPath[0] == '.':
            strPath = os.getcwd() + strPath[1:]
        else:
            strPath = os.path.expanduser(strPath)
        if os.path.isabs(strPath):
            strPath = urlparse.urljoin(XMLDict.__scheme_FILE, strPath)
        aryURL = urlparse.urlsplit(strPath)
        if not aryURL[0].lower() in ('http', 'https', 'file', 'ftp', 'gopher'):
            raise XMLDictError(strPath + ' contains an unknown URL scheme')
        return urllib2.urlopen(strPath)
    __openFile = staticmethod(__openFile)
        
    def load(self, strPath):
        if not strPath:
            raise XMLDictError('load was not supplied with a path or URL for the file')
        fileXML = XMLDict.__openFile(strPath)
        dictDocument = {}
        saxHandler = XMLDict.__SAXHandler(dictDocument, self.__dictOptions)
        xml.sax.parse(fileXML, saxHandler)
        return dictDocument
        
    def loads(self, strXML):
        if not strXML:
            raise XMLDictError('loads was not supplied with an XML string')
        dictDocument = {}
        saxHandler = XMLDict.__SAXHandler(dictDocument, self.__dictOptions)
        xml.sax.parseString(strXML, saxHandler)
        return dictDocument

if __name__ == '__main__':
    pass
