dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
dnl $Id: lam_rpi_gm_setup.m4,v 1.15 2004/02/15 14:02:29 jsquyres Exp $
dnl

AC_DEFUN([LAM_RPI_GM_SETUP],[
#
# Arguments: none
#
# Dependencies: None
#
# Setup the gm RPI
#
# Sets:
#
# GMSHORT: subst
# GMTINY: subst
# GMMAXPORT: subst
# GMPORT: subst
#

#
# Where is libgm/gm.h located?
#

GMDIR=
AC_ARG_WITH(gm, 
	    AC_HELP_STRING([--with-gm],
			   [obsolete synonym for --with-rpi-gm]),
            [GMDIR=$withval])
AC_ARG_WITH(rpi-gm, 
	    AC_HELP_STRING([--with-rpi-gm],
			   [directory where the gm software was installed]),
            [GMDIR=$withval])
GMLIBDIR=
AC_ARG_WITH(rpi-gm-lib, 
	    AC_HELP_STRING([--with-rpi-gm-lib],
			   [directory where the gm library can be found, if it is not in $GMDIR/lib or $GMDIR/binary/lib]),
            [GMLIBDIR=$withval])

# Cover simple case where just --with-gm is given
if test "$GMDIR" = "yes" ; then
    GMDIR=
fi

#
# Check that we have all the right flags to compile with gm
#

WRAPPER_EXTRA_LDFLAGS=
WRAPPER_EXTRA_LIBS="-lgm"

if test -n "$GMDIR" -o -n "$GMLIBDIR"; then
    CPPFLAGS="$CPPFLAGS -I$GMDIR/include"
    AC_CHECK_HEADERS(gm.h,,
	AC_MSG_ERROR([*** Cannot find working gm.h.]))

    # Note that it is possible to find the library even if -L is not
    # specified, if the LD_LIBRARY_PATH includes the directory where
    # the shared gm library is kept.  Hence, we unset LD_LIBRARY_PATH
    # before running this test.

    LD_LIBRARY_PATH_save="$LD_LIBRARY_PATH"
    unset LD_LIBRARY_PATH

    LIBS_save="$LIBS"
    LDFLAGS_save="$LDFLAGS"

    if test -n "$GMLIBDIR" -a -d "$GMLIBDIR"; then
	WRAPPER_EXTRA_LDFLAGS="-L$GMLIBDIR"
    elif test -d "$GMDIR/lib"; then
	WRAPPER_EXTRA_LDFLAGS="-L$GMDIR/lib"
    elif test -d "$GMDIR/binary/lib"; then
	WRAPPER_EXTRA_LDFLAGS="-L$GMDIR/binary/lib"
    else
	AC_MSG_WARN([*** No directory $GMDIR/lib or $GMDIR/binary/lib])
	AC_MSG_ERROR([*** Cannot continue])
    fi

    LDFLAGS="$LDFLAGS $WRAPPER_EXTRA_LDFLAGS"
    AC_CHECK_LIB(gm, gm_init)
    if test "$LIBS" = "$LIBS_save"; then
	WRAPPER_EXTRA_LDFLAGS="-L$GMDIR/binary/lib"
	LDFLAGS="$LDFLAGS_save $WRAPPER_EXTRA_LDFLAGS"
	AC_CHECK_LIB(gm, gm_exit,[],
	    AC_MSG_ERROR([*** Cannot find working libgm.]) )
    fi

    LD_LIBRARY_PATH="$LD_LIBRARY_PATH_save"
else
    AC_CHECK_LIB(gm, main,[], 
	AC_MSG_ERROR([*** Cannot find working libgm.]))
    AC_CHECK_HEADERS(gm.h,, 
	AC_MSG_ERROR([*** Cannot find working gm.h]))
fi

AC_SUBST(WRAPPER_EXTRA_LDFLAGS)
AC_SUBST(WRAPPER_EXTRA_LIBS)

#
# See if we have GM_API_VERSION.  If we do, use it.  If not, find the
# highest one available.  It seems that GM_API_VERSION was introduced
# somewhere after 1.3 but before 1.6. :-\
#

AC_MSG_CHECKING(for GM_API_VERSION)
AC_TRY_COMPILE([#include<gm.h>],
[int foo = GM_API_VERSION;], 
    have_gm_api_ver_msg=yes gm_api_ver=GM_API_VERSION,
    have_gm_api_ver_msg=no  gm_api_ver="")
AC_MSG_RESULT([$have_gm_api_ver_msg])
if test "$gm_api_ver" = ""; then
    found=0
    for val in 5 4 3; do
	if test "$found" = "0"; then
	    var="GM_API_VERSION_1_$val"
	    AC_MSG_CHECKING(for $var)
	    AC_TRY_COMPILE([#include<gm.h>],
		[int foo = $var;], 
		msg=yes found=1 gm_api_ver=$var,
		msg=no found=0 gm_api_ver="")
	    AC_MSG_RESULT($msg)
	fi
    done
fi
if test "$gm_api_ver" = ""; then
    AC_MSG_WARN([*** Could not find a supported GM_API_VERSION])
    AC_MSG_ERROR([*** Cannot continue])
fi
AC_DEFINE_UNQUOTED(LAM_SSI_RPI_GM_API_VERSION, $gm_api_ver,
    [Version of the GM API to use])
unset gm_api_ver have_gm_api_ver_msg found val msg


#
# Do we have gm_put()?
# gm_put() was introduced in gm 2.0, and is exactly identical to gm
# 1.6's gm_directed_send_with_callback().  The name was simply changed
# for consistency/symmtery with gm_get().
#

AC_MSG_CHECKING([for gm_put()])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include<gm.h>
]], 
[[gm_put(0, 0, 0, 0, 0, 0, 0, 0, 0);]])], 
    [HAVE_RDMA_PUT=1 MSG=yes], 
    [HAVE_RDMA_PUT=0 MSG="no, use gm_directed_send_with_callback()"])
AC_DEFINE_UNQUOTED(LAM_SSI_RPI_GM_HAVE_RDMA_PUT, $HAVE_RDMA_PUT,
    [Whether we have gm_put() or gm_directed_send_with_callback()])
AC_MSG_RESULT([$MSG])


#
# Do we have gm_get()?
# gm_get() was introduced in gm 2.0.
#
# 14 Jun 2004: only use gm_get() if --with-rpi-gm-get is specified due
# to potential bugs with gm_get() in GM <= 2.0.11.
#

AC_MSG_CHECKING([for gm_get()])
want_gm_get=
AC_ARG_WITH(rpi-gm-get,
    AC_HELP_STRING([--with-rpi-gm-get],
                   [enable the experimental use of gm_get() -- beware! -- for long messages (default: disabled)]),
    [want_gm_get=$withval])
if test "$want_gm_get" = "yes"; then
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include<gm.h>
]], 
    [[gm_get(0, 0, 0, 0, 0, 0, 0, 0, 0);]])], 
        [HAVE_RDMA_GET=1 MSG=yes], 
        [HAVE_RDMA_GET=0 MSG=no])
else
    HAVE_RDMA_GET=0
    MSG="--with-rpi-gm-get not specified; skipped"
fi

AC_DEFINE_UNQUOTED(LAM_SSI_RPI_GM_HAVE_RDMA_GET, $HAVE_RDMA_GET,
    [Whether we have get_put() or not])
AM_CONDITIONAL(USE_LONG_GET_PROTOCOL, [test "$HAVE_RDMA_GET" = "1"])
AC_MSG_RESULT([$MSG])


#
# Do we want checkpoint/restart support?
# We only support checkpoint/restart for >= gm 2.0, so just piggyback
# on the gm_get() results.
#

AC_MSG_CHECKING([for checkpoint/restart support])
if test "$HAVE_RDMA_GET" = 1; then
    WANT_CHECKPOINT=1
    MSG=yes
else
    WANT_CHECKPOINT=0
    MSG=no
fi
AC_DEFINE_UNQUOTED(LAM_SSI_RPI_GM_WANT_CHECKPOINT, $WANT_CHECKPOINT,
    [Whether we want checkpoing support or not not])
AM_CONDITIONAL(WANT_CHECKPOINT, [test "$WANT_CHECKPOINT" = "1"])
AC_MSG_RESULT([$MSG])


#
# Check command line args
# config/lam_rpi_gm_params.m4
#

LAM_SSI_RPI_GM_PARAMS])dnl
