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
dnl $Id: lam_rpi_tcp_setup.m4,v 1.3 2003/07/28 17:23:17 jsquyres Exp $
dnl

AC_DEFUN([LAM_RPI_TCP_SETUP],[
#
# Arguments: none
#
# Dependencies: None
#
# Common setup for the TCP RPIs
#

# Tunable parameters

TCPSHORT_DEFAULT=65536
TCPSHORT="$TCPSHORT_DEFAULT"
AC_ARG_WITH(tcpshort, 
    AC_HELP_STRING([--with-tcpshort=BYTES],
		   [obsolete synonym for --with-rpi-tcp-short]),
    [TCPSHORT=$withval])
AC_ARG_WITH(rpi-tcp-short, 
    AC_HELP_STRING([--with-rpi-tcp-short=BYTES],
		   [use BYTES as the size of the longest TCP short message (default $TCPSHORT_DEFAULT)]),
    [TCPSHORT=$withval])

AC_MSG_CHECKING([for TCP short message length])
AC_MSG_RESULT([$TCPSHORT bytes])

# Set the tunable params so that the RPI compiles with them

AC_SUBST(TCPSHORT)])dnl
