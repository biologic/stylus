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

my_sources = \
  test_main.cpp 

my_headers =  \
	@top_builddir@/config.h \
	verify.hpp
	

bin_PROGRAMS = run_tests
run_tests_SOURCES = $(my_sources)
run_tests_LDADD  = $(ST_LIB)
	

