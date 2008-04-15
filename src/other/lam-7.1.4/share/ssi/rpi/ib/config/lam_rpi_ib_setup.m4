dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2006 The Trustees of Indiana University.  
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

AC_DEFUN([LAM_RPI_IB_SETUP],[
#
# Arguments: none
#
# Dependencies: None
#
# Setup the ib RPI
#
# Sets:
#
# IBSHORT: subst
# IBTINY: subst
# IBPORT: subst
#

#
# Where is vapi.h/libvapi located?
#

IBDIR=
AC_ARG_WITH(ib, 
	    AC_HELP_STRING([--with-ib],
			   [obsolete synonym for --with-rpi-ib]),
            [IBDIR=$withval])
AC_ARG_WITH(rpi-ib, 
	    AC_HELP_STRING([--with-rpi-ib],
			   [directory where the ib software was installed]),
            [IBDIR=$withval])

# Cover simple case where just --with-ib is given
if test "$IBDIR" = "yes" ; then
    IBDIR=
fi

#
# Check that we have all the right flags to compile with ib
#

WRAPPER_EXTRA_LDFLAGS=
WRAPPER_EXTRA_LIBS=" -lmosal -lmpga -lmtl_common -lvapi"

if test -n "$IBDIR"; then

    # Some IB implementations put vapi.h in a weird place, such as
    # $IBDIR/include/vapi (e.g., Topspin).  So look in $IBDIR/include
    # and $IBDIR/include/vapi.

    if test -f "$IBDIR/include/vapi.h"; then
        CPPFLAGS="$CPPFLAGS -I$IBDIR/include"
    elif test -f "$IBDIR/include/vapi/vapi.h"; then
        CPPFLAGS="$CPPFLAGS -I$IBDIR/include/vapi"
    else
        AC_MSG_ERROR([*** Cannot find working vapi.h])
    fi
    AC_CHECK_HEADERS(vapi.h,,
	AC_MSG_ERROR([*** Cannot find working vapi.h.]))

    # Note that it is possible to find the library even if -L is not
    # specified, if the LD_LIBRARY_PATH includes the directory where
    # the shared ib library is kept.  Hence, we unset LD_LIBRARY_PATH
    # before running this test.

    LD_LIBRARY_PATH_save="$LD_LIBRARY_PATH"
    unset LD_LIBRARY_PATH

    # The libraries may be in $IBDIR/lib or $IBDIR/lib64.  Try them
    # both.

    LIBS_save="$LIBS"
    LDFLAGS_save="$LDFLAGS"
    LIBS="$LIBS -lmosal -lmpga -lmtl_common"
    LIBS_orig="$LIBS"

    if test -d "$IBDIR/lib"; then
        WRAPPER_EXTRA_LDFLAGS="-L$IBDIR/lib"
        LDFLAGS="$LDFLAGS $WRAPPER_EXTRA_LDFLAGS"
        AC_CHECK_LIB(vapi, VAPI_open_hca)
    fi

    if test "$LIBS" = "$LIBS_orig" -a -d "$IBDIR/lib64"; then
        WRAPPER_EXTRA_LDFLAGS="-L$IBDIR/lib64"
        LDFLAGS="$LDFLAGS $WRAPPER_EXTRA_LDFLAGS"
        AC_CHECK_LIB(vapi, VAPI_query_hca_cap)
    fi

    if test "$LIBS" = "$LIBS_orig"; then
      AC_MSG_ERROR([*** Cannot find working libvapi.])
    fi
    LD_LIBRARY_PATH="$LD_LIBRARY_PATH_save"
#    LIBS="$LIBS_save"
else
    AC_CHECK_LIB(vapi, VAPI_open_hca,[],
      AC_MSG_ERROR([*** Cannot find working libvapi.]))
    AC_CHECK_HEADERS(vapi.h,,
      AC_MSG_ERROR([*** Cannot find working vapi.h]))
fi

# Get a size.  This destroys the API independence, but since IBAL
# never panned out, this is much less of a concern.  Open MPI handles
# both VAPI and OpenIB properly; LAM will only ever handle VAPI.

AC_CHECK_SIZEOF([VAPI_rkey_t], [], [#include <vapi.h>])

AC_SUBST(WRAPPER_EXTRA_LDFLAGS)
AC_SUBST(WRAPPER_EXTRA_LIBS)

## Check command line args
## config/lam_rpi_ib_params.m4

LAM_SSI_RPI_IB_PARAMS
])dnl
