#--------------------------------------------------------------------------------
#	CONFIGURE input script
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

AC_PREREQ(2.59)


AC_INIT([Stylus], [1.5.0], [info@biologicinstitute.org], [stylus])
STYLUS_MAJOR_VERSION=1
STYLUS_MINOR_VERSION=5
STYLUS_MICRO_VERSION=0

STYLUS_VERSION=$STYLUS_MAJOR_VERSION.$STYLUS_MINOR_VERSION.$STYLUS_MICRO_VERSION
STYLUS_VERSION_INFO=`expr $STYLUS_MAJOR_VERSION + $STYLUS_MINOR_VERSION`:$STYLUS_MICRO_VERSION:$STYLUS_MINOR_VERSION
STYLUS_VERSION_NUMBER=`expr $STYLUS_MAJOR_VERSION \* 10000 + $STYLUS_MINOR_VERSION \* 100 + $STYLUS_MICRO_VERSION`

AC_DEFINE_UNQUOTED(STYLUS_VERSION, "$STYLUS_VERSION", [Stylus version number])
AC_SUBST(STYLUS_VERSION_INFO)
AC_SUBST(STYLUS_VERSION_NUMBER)

AC_CONFIG_SRCDIR([./src/core/headers.hpp])
AC_CONFIG_AUX_DIR([obj])

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CXX

AC_HEADER_STDC
     
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AM_PATH_XML2(2.6.26)

AM_PATH_PYTHON(2.3)

AC_PROG_SWIG(1.3.31)
SWIG_ENABLE_CXX
SWIG_PYTHON

AC_CONFIG_FILES([Makefile
        src/Makefile
        src/core/Makefile
        src/include/Makefile
				])

AC_ARG_ENABLE(debug,
     [  --enable-debug    Turn on debugging],
     [case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
     esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

AC_ARG_ENABLE(profile,
     [  --enable-profile    Turn on profiling],
     [case "${enableval}" in
       yes) profile=true ;;
       no)  profile=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-profile) ;;
     esac],[debug=false])
AM_CONDITIONAL(PROFILE, test x$profile = xtrue)

AM_CONDITIONAL(RELEASE, test x$profile = xfalse && test x$debug = xfalse)
AM_CONDITIONAL(MACOSX,
			[case "${OSTYPE}" in
				darwin9*) true ;;
				*) false ;;
			esac])

AC_OUTPUT
