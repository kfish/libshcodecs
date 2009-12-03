################################################################################
# libshcodecs: A library for controlling SH-Mobile hardware codecs             #
# Copyright (C) 2009 Renesas Technology Corp.                                  #
#                                                                              #
# This library is free software; you can redistribute it and/or                #
# modify it under the terms of the GNU Library General Public                  #
# License as published by the Free Software Foundation; either                 #
# version 2 of the License, or (at your option) any later version.             #
#                                                                              #
# This library is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            #
# Library General Public License for more details.                             #
#                                                                              #
# You should have received a copy of the GNU Library General Public            #
# License along with this library; if not, write to the Free Software          #
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA #
################################################################################

# Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([src/libshcodecs/shcodecs_decoder.c])

AC_PREREQ(2.53)

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(libshcodecs, 0.9.9)
AM_CONFIG_HEADER(config.h)

################################################################################
# Set the shared versioning info, according to section 6.3 of the libtool info #
# pages. CURRENT:REVISION:AGE must be updated immediately before each release: #
#                                                                              #
#   * If the library source code has changed at all since the last             #
#     update, then increment REVISION (`C:R:A' becomes `C:r+1:A').             #
#                                                                              #
#   * If any interfaces have been added, removed, or changed since the         #
#     last update, increment CURRENT, and set REVISION to 0.                   #
#                                                                              #
#   * If any interfaces have been added since the last public release,         #
#     then increment AGE.                                                      #
#                                                                              #
#   * If any interfaces have been removed since the last public release,       #
#     then set AGE to 0.                                                       #
#                                                                              #
################################################################################
SHARED_VERSION_INFO="7:0:0"
SHLIB_VERSION_ARG=""

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_C_CONST

dnl Add parameters for aclocal
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4")

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h inttypes.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_REALLOC
AC_CHECK_FUNCS([])

# Check for pkg-config
AC_CHECK_PROG(HAVE_PKG_CONFIG, pkg-config, yes)

# Check for doxygen
AC_CHECK_PROG(HAVE_DOXYGEN, doxygen, true, false)
AM_CONDITIONAL(HAVE_DOXYGEN,$HAVE_DOXYGEN)
if test $HAVE_DOXYGEN = "false"; then
             AC_MSG_WARN([*** doxygen not found, docs will not be built])
fi

# Check for valgrind
VALGRIND_ENVIRONMENT=""
ac_enable_valgrind=no
AC_ARG_ENABLE(valgrind-testing,
     [  --enable-valgrind-testing     enable running of tests inside Valgrind ],
     [ ac_enable_valgrind=yes ], [ ac_enable_valgrind=no] )

if test "x${ac_enable_valgrind}" = xyes ; then
  if test "x${enable_shared}" = xyes ; then
    VALGRIND_ENVIRONMENT="libtool --mode=execute "
  fi

  AC_CHECK_PROG(HAVE_VALGRIND, valgrind, yes, no)
  if test "x$HAVE_VALGRIND" = xyes ; then
    VALGRIND_ENVIRONMENT="$VALGRIND_ENVIRONMENT valgrind -q --leak-check=yes --show-reachable=yes --num-callers=100"
    AC_SUBST(VALGRIND_ENVIRONMENT)
    TESTS_INFO="Test suite will be run under:
    ${VALGRIND_ENVIRONMENT}"
  else
    TESTS_INFO="(Valgrind not found)"
  fi
else
  TESTS_INFO="(Valgrind testing not enabled)"
fi

dnl Checks for libraries.
LIBS=""

dnl The following depend on the functions in src/libshcodecs/m4driver.c
dnl AC_CHECK_LIB(m4iph, m4iph_vpu4_decode, HAVE_M4IPH="yes", , [-lm4vsd])
dnl AC_CHECK_LIB(m4vsd, m4vsd_start_decoding, HAVE_M4VSD="yes", , [-lm4iph])
dnl AC_CHECK_LIB(avcbd, avcbd_decode_picture, HAVE_AVCBD="yes", , [-lm4vsd -lm4iph])

VPU4_BASE_LIBS="-lm4iph"
VPU4_DEC_LIBS="$VPU4_BASE_LIBS -lavcbd -lm4vsd"
VPU4_ENC_LIBS="$VPU4_BASE_LIBS -lavcbe -lm4vse"
AC_SUBST(VPU4_DEC_LIBS)
AC_SUBST(VPU4_ENC_LIBS)

dnl
dnl Check for libuiomux
dnl

HAVE_UIOMUX=no
UIOMUX_SUPPORT=no

ac_enable_uiomux=yes
AC_ARG_ENABLE(uiomux,
  AC_HELP_STRING([--disable-uiomux], [disable building of UIOMux support]),
  [ ac_enable_uiomux=no ], [ ac_enable_uiomux=yes ])

if test "x${ac_enable_uiomux}" = xyes ; then
  if test "x$HAVE_PKG_CONFIG" = "xyes" ; then
    PKG_CHECK_MODULES(UIOMUX, uiomux, HAVE_UIOMUX=yes, HAVE_UIOMUX=no)
  fi

  if test "x$HAVE_UIOMUX" = "xno" ; then
    AC_CHECK_LIB(uiomux, uiomux_init, HAVE_UIOMUX="maybe", , [-l...])
    if test "x$HAVE_UIOMUX" = xmaybe ; then
      AC_CHECK_HEADER([uiomux/uiomux.h], HAVE_UIOMUX="yes", HAVE_UIOMUX="no")
    fi
    if test "x$HAVE_UIOMUX" = xyes ; then
      UIOMUX_LIBS="-luiomux"
    fi
  fi

  if test "x$HAVE_UIOMUX" = "xyes" ; then
    AC_DEFINE(HAVE_UIOMUX, [1], [Define to 1 if you have libuiomux])
    UIOMUX_SUPPORT="yes"
  fi
else
  UIOMUX_SUPPORT="disabled"
fi
AM_CONDITIONAL(HAVE_UIOMUX, [test "x$HAVE_UIOMUX" = "xyes"])

# check for getopt in a separate library
HAVE_GETOPT=no
AC_CHECK_LIB(getopt, getopt, HAVE_GETOPT="yes")
if test "x$HAVE_GETOPT" = xyes ; then
  GETOPT_LIBS="-lgetopt"
  AC_SUBST(GETOPT_LIBS)
fi

# check for getopt_long in standard library
HAVE_GETOPT_LONG=no
AC_CHECK_FUNC(getopt_long, HAVE_GETOPT_LONG="yes")
if test "x$HAVE_GETOPT_LONG" = xyes ; then
  AC_DEFINE(HAVE_GETOPT_LONG, [], [Define to 1 if you have the 'getopt_long' function])
fi

dnl Overall configuration success flag
shcodecs_config_ok=yes

shcodecs_read_programs=""
shcodecs_rw_programs=""

dnl
dnl  Configuration option for building of experimental code.
dnl

ac_enable_experimental=no
AC_ARG_ENABLE(experimental,
     [  --enable-experimental   enable building of experimental code ],
     [ ac_enable_experimental=yes ])

if test "x${ac_enable_experimental}" = xyes ; then
    AC_DEFINE(SHCODECS_CONFIG_EXPERIMENTAL, [], [Define to build experimental code])
fi

# Checks for header files.

# Check for <sys/timerfd.h> (timerfd_create() etc.)
AC_CHECK_FUNC(timerfd_create, HAVE_TIMERFD="yes", HAVE_TIMERFD="no")
if test "x$HAVE_TIMERFD" = xyes ; then
  AC_DEFINE(HAVE_TIMERFD, [], [Define to 1 if you have timerfd support])
fi

# Checks for typedefs, structures, and compiler characteristics.

dnl Add some useful warnings if we have gcc.
dnl changequote(,)dnl
if test "x$ac_cv_prog_gcc" = xyes ; then
  CFLAGS="$CFLAGS -Wall -Wextra -g -std=gnu99 -Wdeclaration-after-statement -Wno-unused"
fi
dnl changequote([,])dnl

dnl
dnl  Configuration option to add -Werror to all Makefiles
dnl

AC_ARG_ENABLE(gcc-werror,
     AC_HELP_STRING([--enable-gcc-werror], [enable -Werror in all Makefiles]),
     [ ac_enable_gcc_werror=yes ], [ ac_enable_gcc_werror=no] )

if test "x${ac_enable_gcc_werror}" = xyes ; then
  CFLAGS="-Werror $CFLAGS"
fi

dnl
dnl Shared library symbol versioning and hiding
dnl

case "$target_os" in
  linux* | solaris*)
    SHLIB_VERSION_ARG="-Wl,--version-script=Version_script"
    ;;
  *)
    ;;
esac

# Checks for library functions.

dnl
dnl  Configuration tests complete -- provide summary of results.
dnl

if test $shcodecs_config_ok = no ; then

AC_MSG_RESULT([
**************************************************************

*** $PACKAGE $VERSION: Automatic configuration FAILED.

*** The file config.log has full details.

*** The following required libraries are missing or
*** misconfigured on your system:
])

dnl ...

AC_MSG_RESULT(
[*** If you install the required libraries from source, you
*** need to inform the dynamic linker of their location. If
*** you install them in a system-wide directory such as
*** /usr/local (the default), you must ensure that
*** /usr/local/lib is listed in /etc/ld.so.conf, then run
*** ldconfig to update the dynamic linking system.
*** Alternatively, you can set your LD_LIBRARY_PATH environment
*** variable to include the library installation directory.
])


AC_MSG_RESULT(
[**************************************************************
])


AC_MSG_ERROR([
***
*** After fixing the above problems, you must run ./configure again.
***
]) 

else

AS_AC_EXPAND(LIBDIR, ${libdir})
AS_AC_EXPAND(INCLUDEDIR, ${includedir})
AS_AC_EXPAND(BINDIR, ${bindir})
AS_AC_EXPAND(DOCDIR, ${datadir}/doc)

AC_SUBST(BINDIR)

AC_SUBST(SHLIB_VERSION_ARG)
AC_SUBST(SHARED_VERSION_INFO)

AC_OUTPUT([
Makefile
data/Makefile
doc/Makefile
doc/Doxyfile
include/Makefile
include/shcodecs/Makefile
src/Makefile
src/libshcodecs/Version_script
src/libshcodecs/Makefile
src/tests/Makefile
src/tools/Makefile
src/examples/Makefile
shcodecs.pc
shcodecs-uninstalled.pc
])

AC_MSG_RESULT([
------------------------------------------------------------------------
  $PACKAGE $VERSION:  Automatic configuration OK.

  General configuration:

    Experimental code: ........... ${ac_enable_experimental}
    UIOMux support: .............. ${UIOMUX_SUPPORT}

  Installation paths:

    libshcodecs: ................. ${LIBDIR}
    C header files: .............. ${INCLUDEDIR}/shcodecs
    Documentation: ............... ${DOCDIR}/$PACKAGE

  Building:

    Type 'make' to compile $PACKAGE.

    Type 'make install' to install $PACKAGE.

    Type 'make check' to test $PACKAGE using the unit and functional tests
    contained in the src/tests directory.
    ${TESTS_INFO}

  Example programs will be built but not installed.
------------------------------------------------------------------------
])


fi
