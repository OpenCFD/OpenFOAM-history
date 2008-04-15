dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
dnl $Id: lam_setup_cc.m4,v 1.13 2003/11/02 23:05:54 brbarret Exp $
dnl

define(LAM_SETUP_CC,[

# Modularize this setup so that sub-configure.in scripts can use this
# same setup code.

lam_show_subtitle "C compiler and preprocessor" 

#
# check for specification of C compiler
#

AC_ARG_WITH(cc, 
    AC_HELP_STRING([--with-cc=CC], 
		   [DEPRECATED: use the env variable CC instead]),
    [AC_MSG_WARN([The --with-cc flag is deprecated.  Use the CC])
     AC_MSG_WARN([environment variable instead.])
     AC_MSG_ERROR([Cannot continue])])

#
# check for specification of C compiler flags
#

AC_ARG_WITH(cflags, 
    AC_HELP_STRING([--with-cflags], 
		   [DEPRECATED: use the env variable CFLAGS instead]),
    [AC_MSG_WARN([The --with-cflags flag is deprecated.  Use the CFLAGS])
     AC_MSG_WARN([environment variable instead.])
     AC_MSG_ERROR([Cannot continue])])

#
# check for specification of LD linker flags
#

AC_ARG_WITH(ldflags,
    AC_HELP_STRING([--with-ldflags], 
		   [DEPRECATED: use the env variable LDFLAGS instead]),
    [AC_MSG_WARN([The --with-ldflags flag is deprecated.  Use the LDFLAGS])
     AC_MSG_WARN([environment variable instead.])
     AC_MSG_ERROR([Cannot continue])])

#
# Do we want debugging? - down here so we can change CFLAGS
#
AC_MSG_CHECKING([if want debugging output support])
AC_ARG_WITH(debug, 
  [  --with-debug            enable debugging output (maintainers only)])

if test -n "$with_debug" -a "$with_debug" = "yes"; then
    AC_MSG_RESULT([yes])
    WANT_DEBUG=1
else
    AC_MSG_RESULT([no])
    WANT_DEBUG=0
fi
AC_DEFINE_UNQUOTED(LAM_WANT_DEBUG, $WANT_DEBUG, [LAM debug flag])

#
# Check for the compiler
#

lam_cflags_save="$CFLAGS"
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
BASECC="`basename $CC`"
CFLAGS="$lam_cflags_save"
AC_DEFINE_UNQUOTED(LAM_CC, "$CC", [LAM underlying C compiler])

# $%@#!@#% AIX!!

AC_AIX

# Take care of some specific compiler options. 
# Force use of ANSI C under HP-UX.

AC_MSG_CHECKING([if need special C compiler/linker flags])
FLAGS="none needed"
case "$host" in
    *hpux*)	
	if test "$GCC" = "no"; then
	    FLAGS="-Ae"
	    if test "$CPPFLAGS" != ""; then
		CPPFLAGS="$CPPFLAGS $FLAGS"
	    else
		CPPFLAGS="$FLAGS"
	    fi
	    if test "$CFLAGS" != ""; then
		CFLAGS="$CFLAGS $FLAGS"
	    else
		CFLAGS="$FLAGS"
	    fi
	fi
	;;
    *irix*) 
	if test "$GCC" = "no"; then
	    FLAGS="-Wl,-woff,84"
	    LDFLAGS="$LDFLAGS $FLAGS"
	    CFLAGS="$CFLAGS $FLAGS"
	fi
	;;
    *sco-sysv*)
	# This seems to be necessary on SCO unix to get sendmsg and others
	# to compile/link/run properly.  There's a note in
	# /usr/include/socket.h on SCO about this.  Weird.
	FLAGS="-DSVR4"
	CPPFLAGS="$FLAGS $CPPFLAGS"
esac
AC_MSG_RESULT([$FLAGS])

# When building LAM, we need this everywhere

CPPFLAGS="$CPPFLAGS -DLAM_BUILDING=1"

# Do we want debugging?

if test "$WANT_DEBUG" = "1"; then
    if test "$GCC" = "yes"; then
	add="-g -Wall -Wundef -Wno-long-long"

        # see if -Wno-long-double works...
        CFLAGS_orig="$CFLAGS"
        CFLAGS="$CFLAGS -Wno-long-double"
        AC_TRY_COMPILE([], [], add="$add -Wno-long-double")
        CFLAGS="$CFLAGS_orig"

        add="$add -Werror-implicit-function-declaration "
    else
	add="-g"
    fi
    CFLAGS="$CFLAGS $add"
    LAM_UNIQ(CFLAGS)
    AC_MSG_WARN([$add has been added to CFLAGS (--with-debug)])
    unset add
fi

# Preload the optflags for the case where the user didn't specify any.
# If we're using GNU compilers, use -O3 (since it GNU doesn't require
# all compilation units to be compiled with the same level of
# optimization -- selecting a high level of optimization is not
# prohibitive).  If we're using anything else, be conservative and
# just use -O.  

if test "$GCC" = yes; then
    OPTFLAGS="-O3"
else
    OPTFLAGS="-O"
fi
LAM_CHECK_OPTFLAGS("$CFLAGS")
AC_MSG_CHECKING([for C optimization flags])
AC_MSG_RESULT([$co_result])
CFLAGS="$co_result"
])
