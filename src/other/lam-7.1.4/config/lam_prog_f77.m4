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
dnl $Id: lam_prog_f77.m4,v 1.7 2003/10/12 06:24:05 brbarret Exp $
dnl

AC_DEFUN([LAM_PROG_F77],[
#
# Arguments: none
#
# Dependencies: None
#
# Find the fortran compiler
#
# Sets:
#   F77 - full pathname to fortran compiler
#   LAM_WANT_FORTRAN - to 0 if --without-fc specified, 1 otherwise
#
if test ! -z "$FC" ; then
  F77="$FC"
elif test ! -z "$F77" ; then
  FC="$F77"
fi
if test "$with_fc" = "no"; then
    LAM_WANT_FORTRAN=0
else
    LAM_WANT_FORTRAN=1
    if test -n "$FC"; then

	# AC_CHECK_PROG* only checks for the user specified compiler in the
	# users $PATH.  Arrgh!
	# It does not address the case where the user specifies an absolute
	# filename that is *not* in their $PATH.

	FC0="`echo $FC | awk '{ print $1 }'`"
	ISABS="`echo $FC0 | cut -c1`"
	if test "$ISABS" = "/" -a -x "$FC0"; then
	    F77="$FC"
	    AC_MSG_CHECKING([for FORTRAN compiler $FC0])
	    AC_MSG_RESULT([found])
	else
	    AC_CHECK_PROGS(F77, $FC0)
	fi
	if test -z "$F77"; then
	    AC_MSG_ERROR(could not find FORTRAN compiler $FC0)
	fi
    else

	# If the user did not specify one, look for all the common names

	AC_CHECK_PROGS(F77, [g77 f77 fort77 f90], no)
	if test -z "$F77" -o "$F77" = "no"; then
	    AC_MSG_WARN([*** Could not find FORTRAN compiler.  Either use])
	    AC_MSG_WARN([*** --with-fc to specify the FORTRAN compiler, or])
	    AC_MSG_WARN([*** --without-fc to specify no FORTRAN support])
	    AC_MSG_ERROR([Cannot continue current configuration without FORTRAN compiler])
	fi
	FC="$F77"
    fi
    BASEFC=`basename $FC`
    export FC
    export F77
fi])dnl
