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
dnl $Id: lam_rpi_ib_params.m4,v 1.2 2003/12/15 03:10:24 vsahay Exp $
dnl

AC_DEFUN([LAM_SSI_RPI_IB_PARAMS],[
#
# Arguments: none
#
# Dependencies: None
#
# Setup the command line args to the ib RPI
#
# Sets:
#
# IBSHORT: subst
# IBTINY: subst
# IBPORT: subst
#

IBTINY=32768
AC_ARG_WITH(ib-tiny, 
    AC_HELP_STRING([--with-ib-tiny=BYTES],
		   [obsolete synonym for --with-rpi-ib-tiny]),
    IBTINY="$withval")
AC_ARG_WITH(rpi-ib-tiny, 
    AC_HELP_STRING([--with-rpi-ib-tiny=BYTES],
		   [use BYTES as the size of the longest IB tiny message]),
    IBTINY="$withval")
AC_MSG_CHECKING([for IB tiny message length])
AC_MSG_RESULT([$IBTINY bytes])

IBPORT=-1
AC_ARG_WITH(ib-port, 
    AC_HELP_STRING([--with-ib-port=N],
		   [obsolete synonym for --with-rpi-ib-port]),
    IBPORT="$withval")
AC_ARG_WITH(rpi-ib-port, 
    AC_HELP_STRING([--with-rpi-ib-port=N],
		   [use N as the ib port for all jobs (default -1)]),
    IBPORT="$withval")
AC_MSG_CHECKING([for fixed ib port number to use])
if test "$IBPORT" = "-1"; then
    AC_MSG_RESULT([none -- search at runtime])
else
    AC_MSG_RESULT([$IBPORT])
fi

IBNUMENVELOPES=64;
AC_ARG_WITH(ib-num-envelopes, 
    AC_HELP_STRING([--with-ib-num-envelopes=N],
		   [obsolete synonym for --with-rpi-ib-port]),
    IBNUMENVELOPES="$withval")
AC_ARG_WITH(rpi-ib-num-envelopes, 
    AC_HELP_STRING([--with-rpi-ib-num-envelopes=N],
		   [use N as the ib-num-envelopes for all jobs (default 8)]),
    IBBNUMENVELOPES="$withval")
AC_MSG_CHECKING([for number of buffers per IB queue to use])
if test "$IBNUMENVELOPES" = "8"; then
    AC_MSG_RESULT([none -- search at runtime])
else
    AC_MSG_RESULT([$IBNUMENVELOPES])
fi

# Set the tunable params so that the RPI compiles with them

AC_SUBST(IBPORT)
AC_SUBST(IBNUMENVELOPES)
AC_SUBST(IBHCAID)
AC_SUBST(IBTINY)])dnl
