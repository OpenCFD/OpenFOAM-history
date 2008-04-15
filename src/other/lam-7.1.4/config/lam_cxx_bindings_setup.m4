dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2002 The Trustees of Indiana University.  
dnl                         All rights reserved.
dnl Copyright (c) 1998-2001 University of Notre Dame. 
dnl                         All rights reserved.
dnl Copyright (c) 1994-1998 The Ohio State University.  
dnl                         All rights reserved.
dnl 
dnl This file is part of the LAM/MPI software package.  For license
dnl information, see the LICENSE file in the top level directory of the
dnl LAM/MPI source distribution.
dnl
dnl $Id: lam_cxx_bindings_setup.m4,v 1.6 2002/10/09 20:55:12 brbarret Exp $
dnl

define([LAM_CXX_BINDINGS_SETUP],[

MPI_CXX=
MPI_CXXFLAGS=
MPI_LDFLAGS=
MPI_LIBS=


#
# Compiler flags
#

NOOPT=
OPTFLAGS="$CXXFLAGS"
EXAM_OPTFLAGS=$OPTFLAGS
TEST_OPTFLAGS=$OPTFLAGS


#
# Miscellaneous flags
#

LAM_CXXFLAGS=
EXTRA_CXXFLAGS=
EXCPTN_CXXFLAGS=
EXCPTN_LDFLAGS=
ARCH=
EXFLAGS=
FORCE_EXFLAGS=


#
# Get our system type
# Set up for some screwy machines
#

case "$host" in
    *-hp-hpux*)
	if test "$GXX" = ""; then
	    CXXFLAGS="$CXXFLAGS $EXTRA_CXXFLAGS"
	    EXFLAGS="+eh"

	    NOOPT="-Wl,+vnocompatwarnings"
	    OPTFLAGS="+O1 -Wl,+vnocompatwarnings"
	    EXAM_OPTFLAGS=$OPTFLAGS
	    TEST_OPTFLAGS=$OPTFLAGS
	    
	    MPI_CXX_SAVE=$MPI_CXX
	    
	    ARCH=HPUX
	fi
        ;;

    mips-sgi-irix*)
        OPTFLAGS="-O3"
        TEST_OPTFLAGS=$OPTFLAGS
        EXAM_OPTFLAGS=$OPTFLAGS
	ARCH=IRIX
        ;;

    *-cray-unicos*)
	# Yuk!  We have to turn off all optimization in the test suite because
	# Cray's CC version 3.0.0.0 runs out of memory when compiling modules
	# like port_test.cc and request_array_test.cc.  Doh!!

	NOOPT="-O0"
	OPTFLAGS="-O3"
	TEST_OPTFLAGS=$NOOPT
	EXAM_OPTFLAGS=$OPTFLAGS

	EXFLAGS="-h exceptions"
	ARCH=CRAY
	;;
esac

#
# Now we can clear MPI_CXX
#

MPI_CXX=


#
# Set autoconf for C++
#

AC_LANG_CPLUSPLUS


#
# Hardwired for LAM -- no extra libs necessary
#

MPI_LIBS=""

#
# Check to see if the resulting bool type is the same size as an int
# Just to make it easy (and uniform), always check, even if this compiler
# does not have a native "bool" type.
#

SAVE_LDFLAGS="$LDFLAGS"
SAVE_LIBS="$LIBS"
LDFLAGS=
LIBS=

LAM_GET_SIZEOF(int, MPI2CPP_SIZEOF_INT)
LAM_GET_SIZEOF(bool, MPI2CPP_SIZEOF_BOOL_T)

LDFLAGS="$SAVE_LDFLAGS"
LIBS="$SAVE_LIBS"


#
# MPI says that attributes will be at least as large as pointers.
# If we're on a 64 bit system, the pointers may be 8 bytes, not 4.
# Attributes must be typed/sized appropriately.
#

AC_MSG_CHECKING([type of MPI attributes])
AC_TRY_RUN(int main() 
{
  if (sizeof(int) != sizeof(int *))
    return -1;
  return 0;
}, sizeof_same=yes, sizeof_same=no, sizeof_same=no)
AH_TEMPLATE([MPI2CPP_ATTR], [Type of MPI C++ attributes])
if test "$sizeof_same" = "yes"; then
    ATTR_TYPE="int"
    AC_DEFINE(MPI2CPP_ATTR, int)     
else
    ATTR_TYPE="long"
    AC_DEFINE(MPI2CPP_ATTR, long)
fi
AC_MSG_RESULT([$ATTR_TYPE])


#
# Do we have BSD or SYSV style signals?
# (or something that we haven't thought of yet?)
#

AC_MSG_CHECKING([for signal declaration])
AC_TRY_COMPILE(#include<signal.h>
extern "C" void bsd_handler(int sig, int code, struct sigcontext *scp, char *addr);
, 
signal(1, bsd_handler);
  struct sigaction n;
  n.sa_handler= bsd_handler;
, bsd_signal="yes", bsd_signal="no")
AH_TEMPLATE([MPI2CPP_BSD_SIGNAL], [Whether we have BSD-style signals or not])
AH_TEMPLATE([MPI2CPP_SYSV_SIGNAL], [Whether we have SYSV-style signals or not])
if test "$bsd_signal" = "yes"; then
    AC_DEFINE(MPI2CPP_BSD_SIGNAL, 1)
    AC_DEFINE(MPI2CPP_SYSV_SIGNAL, 0)
    AC_MSG_RESULT([BSD])
else

#
# Sanity check.  Make sure that it is SYSV
#

AC_TRY_COMPILE(#include<signal.h>
extern "C" void sysv_handler(int sig) {};
, 
signal(1, sysv_handler);
, sysv_signal="yes", sysv_signal="no")
    if test "$sysv_signal" = "yes"; then
        AC_DEFINE(MPI2CPP_SYSV_SIGNAL, 1)
	AC_DEFINE(MPI2CPP_BSD_SIGNAL, 0)
        AC_MSG_RESULT([SYSV])
    else
        AC_MSG_RESULT([not found])
        AC_MSG_ERROR([cannot determine if signals are bsd/sysv])
        exit
    fi
fi

#
# All done with C++
#

AC_LANG_C

])
