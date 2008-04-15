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
dnl $Id: lam_config_fd_passing.m4,v 1.10 2003/10/10 15:54:58 pkambadu Exp $
dnl

define(LAM_CONFIG_FD_PASSING,[
#
# Arguments: none
#
# Dependencies: None
#
# Sets:
# none - but check underlying tests
#
# configure fd passing.  Should abort if we can't get anything.

LAM_CHECK_BSD43_FD_PASSING
LAM_CHECK_BSD44_FD_PASSING
LAM_CHECK_SVR4_FD_PASSING

have_fd_passing=0
if test "$LAM_HAVE_BSD43_FD_PASSING" = "0" &&
	test "$LAM_HAVE_RFC2292_FD_PASSING" = "0" &&
	test "$LAM_HAVE_POSIX_1G_FD_PASSING" = "0" &&
	test "$LAM_HAVE_SVR4_FD_PASSING" = "0" ; then
	cat <<EOF

*******************************************************************************
WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
*******************************************************************************

Your system does not appear to have supoprt for file descriptor passing.
While it is possible to use LAM/MPI on such a system, some functionality
will be missing - most notably, stdout/stderr of remote MPI processes.

If you believe that your system does actually have file descriptor passing
support, please contact the LAM/MPI mailing list (lam@lam-mpi.org).

*******************************************************************************
WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
*******************************************************************************

EOF
	sleep 5
else
    have_fd_passing=1
fi

AC_DEFINE_UNQUOTED(LAM_HAVE_FD_PASSING, $have_fd_passing,
    [Do we have some type of file descriptor passing support])

unset have_fd_passing])dnl


define(LAM_TRY_RUN_FD_PASSING,[
#
# ARGUMENTS: program to try, succeed action, failed action, cross-compile
#
CPPFLAGS_SAVE="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I."
echo "#define LAM_RUNNING_CONFIGURE_TEST 1" > conftest_fd_include.c
echo "#define LAM_ZERO_ME(a)" >> conftest_fd_include.c
cat $srcdir/share/etc/srfd.c >> conftest_fd_include.c

AC_TRY_RUN([$1], [$2], [$3], [$4])
CPPFLAGS="$CPPFLAGS_SAVE"
unset CPPFLAGS_SAVE])
