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
dnl $Id: lam_check_mpiexec.m4,v 1.3 2003/07/08 15:46:15 jsquyres Exp $
dnl

define(LAM_CHECK_MPIEXEC,[
# Determine if installed perl can support mpiexec
AC_MSG_CHECKING([if installed perl can support mpiexec])
`$srcdir/tools/mpiexec/mpiexec -test >/dev/null 2>&1`
test_mpiexec_perl_compatibility=$?
if test "$test_mpiexec_perl_compatibility" = "0" ; then
mpiexec_perl_compatible='yes'
else
mpiexec_perl_compatible=='no'
fi
AM_CONDITIONAL(LAM_WANT_INSTALL_MPIEXEC, 
	       test "$test_mpiexec_perl_compatibility" = "0")
AC_MSG_RESULT([$mpiexec_perl_compatible])
unset test_mpiexec_perl_compatibility mpiexec_perl_compatible])dnl
