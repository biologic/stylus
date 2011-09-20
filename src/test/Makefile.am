#--------------------------------------------------------------------------------
#	AUTOMAKE input script
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
#--------------------------------------------------------------------------------

include $(top_srcdir)/Makerules.in

AM_CPPFLAGS = -I@builddir@ $(ST_INCLUDE) $(randomcINCLUDE) $(XML_CPPFLAGS)
if DEBUG
AM_CFLAGS   = $(ST_DEBUG_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(ST_DEBUG_CXXFLAGS)
else
if PROFILE
AM_CFLAGS   = $(ST_PROF_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(ST_CXXFLAGS)
if MACOSX
ST_PROF_LIB = -lSaturn
else
ST_PROF_LIB = 
endif
else
AM_CFLAGS   = $(ST_CFLAGS)
AM_CXXFLAGS = $(AM_CFLAGS) $(ST_CXXFLAGS)
endif
endif


my_sources = \
  test_main.cpp  \
  test_eventstack.cpp

my_headers =  \
	@top_builddir@/config.h \
	verify.hpp
	

bin_PROGRAMS = run_tests
run_tests_SOURCES = $(my_sources)
run_tests_LDADD  = $(ST_LIB)
	

