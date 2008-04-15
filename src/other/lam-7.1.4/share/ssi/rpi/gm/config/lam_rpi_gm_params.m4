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
dnl $Id: lam_rpi_gm_params.m4,v 1.9 2003/10/25 06:01:31 jsquyres Exp $
dnl

AC_DEFUN([LAM_SSI_RPI_GM_PARAMS],[
#
# Arguments: none
#
# Dependencies: None
#
# Setup the command line args to the gm RPI
#
# Sets:
#
# GMSHORT: subst
# GMTINY: subst
# GMMAXPORT: subst
# GMPORT: subst
#

GMTINY=16384
AC_ARG_WITH(gm-tiny, 
    AC_HELP_STRING([--with-gm-tiny=BYTES],
		   [obsolete synonym for --with-rpi-gm-tiny]),
    GMTINY="$withval")
AC_ARG_WITH(rpi-gm-tiny, 
    AC_HELP_STRING([--with-rpi-gm-tiny=BYTES],
		   [use BYTES as the size of the longest GM tiny message]),
    GMTINY="$withval")
AC_MSG_CHECKING([for GM tiny message length])
AC_MSG_RESULT([$GMTINY bytes])

GMMAXPORT=32
AC_ARG_WITH(gm-max-port, 
    AC_HELP_STRING([--with-gm-max-port=N],
		   [obsolete synonym for --with-rpi-gm-max-port]),
    GMMAXPORT="$withval")
AC_ARG_WITH(rpi-gm-max-port, 
    AC_HELP_STRING([--with-rpi-gm-max-port=N],
		   [use N as the max gm port number to try]),
    GMMAXPORT="$withval")
AC_MSG_CHECKING([for GM max port number])
AC_MSG_RESULT([$GMMAXPORT])

GMPORT=-1
AC_ARG_WITH(gm-port, 
    AC_HELP_STRING([--with-gm-port=N],
		   [obsolete synonym for --with-rpi-gm-port]),
    GMPORT="$withval")
AC_ARG_WITH(rpi-gm-port, 
    AC_HELP_STRING([--with-rpi-gm-port=N],
		   [use N as the gm port for all jobs (default -1)]),
    GMPORT="$withval")
AC_MSG_CHECKING([for fixed gm port number to use])
if test "$GMPORT" = "-1"; then
    AC_MSG_RESULT([none -- search at runtime])
else
    AC_MSG_RESULT([$GMPORT])
fi

# Set the tunable params so that the RPI compiles with them

AC_SUBST(GMPORT)
AC_SUBST(GMMAXPORT)
AC_SUBST(GMSHORT)
AC_SUBST(GMTINY)])dnl
