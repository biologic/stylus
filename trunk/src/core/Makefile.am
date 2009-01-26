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

perllibDIR     = @srcdir@/Math-Random-0.69
perllibSOURCES = $(perllibDIR)/com.c $(perllibDIR)/linpack.c $(perllibDIR)/randlib.c
perllibHEADERS = $(perllibDIR)/randlib.h
perllibINCLUDE = -I$(perllibDIR)

AM_CPPFLAGS = -I@builddir@ $(ST_INCLUDE) $(perllibINCLUDE) $(XML_CPPFLAGS)
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

EXTRA_DIST = ./Math-Random-0.69/

my_sources = \
	constants.cpp \
	codon.cpp \
	error.cpp \
	gene.cpp \
	genome.cpp \
	global.cpp \
	han.cpp \
	helpers.cpp \
	mutation.cpp \
	overlap.cpp \
	plan.cpp \
	random.cpp \
	scoring.cpp \
	validate.cpp \
	version.cpp \
	xml.cpp \
	$(perllibSOURCES)

my_headers = \
	@top_builddir@/config.h \
	prefix.hpp headers.hpp \
	constants.hpp \
	codon.hpp \
	codon.inl \
	error.hpp \
	error.inl \
	gene.hpp \
	gene.inl \
	genome.hpp \
	genome.inl \
	global.hpp \
	global.inl \
	han.hpp \
	han.inl \
	helpers.hpp \
	helpers.inl \
	overlap.hpp \
	overlap.inl \
	plan.hpp \
	plan.inl \
	random.hpp \
	random.inl \
	smartptr.hpp \
	xml.hpp \
	xml.inl \
	$(perllibHEADERS)

lib_LTLIBRARIES      = libstylus.la
libstylus_la_SOURCES = $(my_sources) $(my_headers)
libstylus_la_LIBADD  = 
libstylus_la_LDFLAGS = $(XML_LIBS) $(ST_PROF_LIB) -version-info @STYLUS_VERSION_INFO@

version.lo version.o : FORCE
FORCE :
