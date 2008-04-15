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
dnl $Id: lam_config_threads.m4,v 1.20 2004/01/20 03:41:47 jsquyres Exp $
dnl

AC_DEFUN([LAM_CONFIG_THREADS],[
#
# Arguments: none
#
# Dependencies: None
#
# Modifies:
#  none - see called tests
#
# configure threads
#

# create templates
AH_TEMPLATE([LAM_HAVE_SOLARIS_THREADS], 
    [Do we have native Solaris threads])
AH_TEMPLATE([LAM_HAVE_POSIX_THREADS], 
    [Do we have POSIX threads])

#
# Check for thread types - add your type here...
#
LAM_CONFIG_POSIX_THREADS(HAVE_POSIX_THREADS=1, HAVE_POSIX_THREADS=0)
AC_MSG_CHECKING([for working POSIX threads package])
if test "$HAVE_POSIX_THREADS" = "1" ; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi
export HAVE_POSIX_THREADS

LAM_CONFIG_SOLARIS_THREADS(HAVE_SOLARIS_THREADS=1, HAVE_SOLARIS_THREADS=0)
AC_MSG_CHECKING([for working Solaris threads package])
if test "$HAVE_SOLARIS_THREADS" = "1" ; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi
export HAVE_SOLARIS_THREADS

#
# Ask what threading we want (allow solaris / pthread right now)
#
AC_MSG_CHECKING([for type of thread support])
AC_ARG_WITH(threads, 
  	AC_HELP_STRING([--with-threads],
		       [Set thread type (solaris / posix)]),
	[THREAD_TYPE=$withval])

if test "$THREAD_TYPE" = "solaris"; then

    if test "$HAVE_SOLARIS_THREADS" = "0"; then
	AC_MSG_WARN(["*** You have chosen Solaris threads, which are not"])
	AC_MSG_WARN(["*** available on your system "])
	AC_MSG_ERROR(["*** Can not continue"])
    fi
elif test "$THREAD_TYPE" = "posix"; then

    if test "$HAVE_POSIX_THREADS" = "0"; then
	AC_MSG_WARN(["*** You have chosen POSIX threads, which are not"])
	AC_MSG_WARN(["*** available on your system "])
	AC_MSG_ERROR(["*** Can not continue"])
    fi
elif test "$THREAD_TYPE" = "no"; then
    THREAD_TYPE="none"
elif test "$THREAD_TYPE" = ""; then

    # Actual logic here - properly set THREAD_TYPE - we go for system
    # optimized where ever possible
    case "$host" in
	*solaris*)
	    if test "$HAVE_SOLARIS_THREADS" = "1"; then
		THREAD_TYPE="solaris"
	    elif test "$HAVE_POSIX_THREADS" = "1"; then
		THREAD_TYPE="posix"
	    else
		THEAD_TYPE="none found"
	    fi
	    ;;
	*)
	    if test "$HAVE_POSIX_THREADS" = "1"; then
		THREAD_TYPE="posix"
	    else
		THREAD_TYPE="none found"
	    fi
	    ;;
    esac
else

    AC_MSG_WARN(["*** You have specified a thread type that I do not"])
    AC_MSG_WARN(["*** understand.  Valid options are posix and solaris"])
    AC_MSG_ERROR(["*** Can not continue."])
fi
AC_MSG_RESULT($THREAD_TYPE)


#
# Ok, now run the configuration for that thread package.
#
# Blah - this should be made better, but I don't know how...
#
if test "$THREAD_TYPE" = "solaris"; then
    AC_DEFINE(LAM_HAVE_SOLARIS_THREADS, 1)
    AC_DEFINE(LAM_HAVE_POSIX_THREADS, 0)
    AC_DEFINE(LAM_THREADS_HAVE_DIFFERENT_PIDS, 0)

    THREAD_CFLAGS="$STHREAD_CFLAGS"
    THREAD_FFLAGS="$STHREAD_FFLAGS"
    THREAD_CXXFLAGS="$STHREAD_CXXFLAGS"
    THREAD_CPPFLAGS="$STHREAD_CPPFLAGS"
    THREAD_CXXCPPFLAGS="$STHREAD_CXXCPPFLAGS"
    THREAD_LDFLAGS="$STHREAD_LDFLAGS"
    THREAD_LIBS="$STHREAD_LIBS"
elif test "$THREAD_TYPE" = "posix"; then
    AC_DEFINE(LAM_HAVE_SOLARIS_THREADS, 0)
    AC_DEFINE(LAM_HAVE_POSIX_THREADS, 1)

    THREAD_CFLAGS="$PTHREAD_CFLAGS"
    THREAD_FFLAGS="$PTHREAD_FFLAGS"
    THREAD_CXXFLAGS="$PTHREAD_CXXFLAGS"
    THREAD_CPPFLAGS="$PTHREAD_CPPFLAGS"
    THREAD_CXXCPPFLAGS="$PTHREAD_CXXCPPFLAGS"
    THREAD_LDFLAGS="$PTHREAD_LDFLAGS"
    THREAD_LIBS="$PTHREAD_LIBS"

    LAM_CHECK_PTHREAD_PIDS
elif test "$THREAD_TYPE" = "none"; then
    AC_DEFINE(LAM_HAVE_SOLARIS_THREADS, 0)
    AC_DEFINE(LAM_HAVE_POSIX_THREADS, 0)

    TRHEAD_CFLAGS=
    THREAD_FFLAGS=
    THREAD_CXXFLAGS=
    THREAD_CPPFLAGS=
    THREAD_CXXCPPFLAGS=
    THREAD_LDFLAGS=
    THREAD_LIBS=
else
    cat <<EOF

************************************************************************

LAM/MPI was unable to find threading support on your system.  In the
near future, the LAM development team is considering requiring
threading support for proper LAM execution.  This is in part because
we are not aware of any users that do not have thread support - so we
need you to e-mail us at lam@lam-mpi.org and let us know about this
problem.

To build this version of LAM/MPI without thread support, re-run
configure with the '--without-threads' option.

************************************************************************

EOF
    AC_MSG_ERROR(["*** Can not continue."])
fi
])

