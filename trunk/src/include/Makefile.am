#--------------------------------------------------------------------------------
#	AUTOMAKE input script
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
#--------------------------------------------------------------------------------

include $(top_srcdir)/Makerules.in

AM_CPPFLAGS = -I@builddir@ $(ST_INCLUDE) $(PYTHON_CPPFLAGS)
if DEBUG
AM_CFLAGS   = $(ST_DEBUG_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(ST_DEBUG_CXXFLAGS)
else
if PROFILE
AM_CFLAGS   = $(ST_PROF_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(ST_CXXFLAGS)
else
AM_CFLAGS   = $(ST_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(ST_CXXFLAGS)
endif
endif

BUILT_SOURCES = stylusengine.cpp
EXTRA_DIST = stylusengine.i

my_sources = \
	stylus.h

pkgpython_PYTHON         = stylusengine.py
pkgpyexec_LTLIBRARIES    = _stylusengine.la
pkginclude_HEADERS       = stylus.h
_stylusengine_la_SOURCES = $(my_sources)
_stylusengine_la_LIBADD  = $(ST_LIB) $(PYTHON_EXTRA_LIBS)
_stylusengine_la_LDFLAGS = -module -shared -version-info @STYLUS_VERSION_INFO@ $(PYTHON_LDFLAGS) $(PYTHON_EXTRA_LDFLAGS)

nodist__stylusengine_la_SOURCES = $(BUILT_SOURCES)

stylusengine.cpp: stylusengine.i
	$(SWIG) -Wall $(SWIG_PYTHON_OPT) $(ST_INCLUDE) -o $@ $<
	
dist-hook:
	rm -f $(distdir)/stylusengine.py
