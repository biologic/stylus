/*******************************************************************************
 *  \file   prefix.hpp
 *  \brief  Stylus pre-compiled header.
 *
 *  This file should contain only references to external headers that change
 *  infrequently.
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

#ifndef PREFIX_HPP
#define PREFIX_HPP

#include "config.h"

#if defined(__APPLE__) and defined(__MACH__)
#define ST_MACOSX 1
#endif

// Standard C/C++ ---------------------------------------------------------------
#include <algorithm>
#include <bitset>
#include <cctype>
#include <cmath>
#include <climits>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <new>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#if defined(ST_PROFILE) and defined(ST_MACOSX)
#include <Saturn.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
// LibXML2/LibXSLT --------------------------------------------------------------
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/nanohttp.h>
#include <libxml/uri.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlstring.h>

// C Headers -------------------------------------------------------------------
#include <stdarg.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/time.h>
	
#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
}
#endif

#endif  // PREFIX_HPP
