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
dnl $Id: lam_check_fd_setsize.m4,v 1.3 2003/10/26 19:52:33 brbarret Exp $
dnl
dnl
dnl LAM_CHECK_FD_SETSIZE
dnl
dnl FUNCTION:
dnl     check maximum workable size of an FD_SET (as modified by FD_SETSIZE).  
dnl     Makes sure that select will still work properly and that we can open
dnl     FD_SETSIZE file descriptors
dnl
dnl RESULTS
dnl     - AC_DEFINEs LAM_FD_SETSIZE (0 if LAM_FD_SETSIZE < default FD_SETSIZE)
dnl     - adds -DFD_SETSIZE=$LAM_FD_SETSIZE to CPPFLAGS, CXXCPPFLAGS,
dnl       WRAPPER_CFLAGS, and WRAPPER_CXXFLAGS  (only if LAM_FD_SETSIZE > 
dnl       default FD_SETSIZE)


AC_DEFUN([LAM_CHECK_FD_SETSIZE],[
AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

LAM_FD_SETSIZE=0


#
# See if the user wants to set a FD_SETSIZE
#
AC_ARG_WITH([fd-setsize], 
            AC_HELP_STRING([--with-fd-setsize=SIZE],
                           [allow LAM to use SIZE file descriptors per process (defaults to system FD_SETSIZE)]),
            [LAM_FD_SETSIZE=$withval],
            [LAM_FD_SETSIZE=0])


#
# Make sure user FD_SETSIZE is greater than system default
#
if test ! "$LAM_FD_SETSIZE" = "0"; then
    AC_MSG_CHECKING([if LAM_FD_SETSIZE greater than default])
    CPPFLAGS_orig="$CPPFLAGS"
    CPPFLAGS="-DLAM_FD_SETSIZE=$LAM_FD_SETSIZE $CPPFLAGS"

    AC_COMPILE_IFELSE([#include "confdefs.h"
#include <sys/types.h>
#if LAM_FD_SETSIZE < FD_SETSIZE
#error "LAM_FD_SETSIZE is smaller than FD_SETSIZE"
#endif
    ], , [LAM_FD_SETSIZE=0])

    if test "$LAM_FD_SETSIZE" = "0" ; then
        AC_MSG_RESULT([no (increasing to default)])
    else
        AC_MSG_RESULT([yes])
    fi

    CPPFLAGS="$CPPFLAGS_orig"
fi


#
# Make sure that we can create LAM_FD_SETSIZE file descriptors (defaults to no
# but no error on cross-compiler).  Use 0 for key that we are cross compiling,
# -1 for error
#
if test ! "$LAM_FD_SETSIZE" = "0" ; then
    AC_MSG_CHECKING([if can create LAM_FD_SETSIZE file descriptors])
    CPPFLAGS_orig="$CPPFLAGS"
    CPPFLAGS="-DLAM_FD_SETSIZE=$LAM_FD_SETSIZE $CPPFLAGS"

    AC_RUN_IFELSE([#include "confdefs.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
int
main(int argc, char* argv[])
{
  int fd = 0;
  struct rlimit rlp;
  rlp.rlim_cur = rlp.rlim_max = (rlim_t) LAM_FD_SETSIZE;
  setrlimit(RLIMIT_NOFILE, &rlp);
  while (fd >= 0 && fd < LAM_FD_SETSIZE - 1) {
    fd = dup(0);
    if (fd < 0) {
      return 1;
    }
  }
  return 0;
}], , [LAM_FD_SETSIZE=-1], [LAM_FD_SETSIZE=0])

    if test "$LAM_FD_SETSIZE" = "0" ; then
        AC_MSG_RESULT([cross-compiling, setting to default])
    elif test "$LAM_FD_SETSIZE" = "-1" ; then
        AC_MSG_RESULT([no])
        AC_MSG_WARN([*** Could not open $LAM_FD_SETSIZE file descriptors.])
        AC_MSG_WARN([*** Try using a smaller FD_SETSIZE or increasing the per-process])
        AC_MSG_WARN([*** file descriptor limits.  This feature not available])
        AC_MSG_WARN([*** on all platforms - see the LAM platform notes in the])
        AC_MSG_WARN([*** Installation Guide])
        AC_MSG_ERROR([*** Can not continue])
    else
        AC_MSG_RESULT([yes])
    fi

    CPPFLAGS="$CPPFLAGS_orig"
fi


#
# Make sure that all works at LAM_FD_SETSIZE (defaults to no but no error on
# cross-compiler).  Use 0 for key that we are cross-compiling, -1 for error
#
if test ! "$LAM_FD_SETSIZE" = "0"; then
    AC_MSG_CHECKING([if select works with large FD_SETSIZE])
    CPPFLAGS_orig="$CPPFLAGS"
    CPPFLAGS="-DFD_SETSIZE=$LAM_FD_SETSIZE -DLAM_FD_SETSIZE=$LAM_FD_SETSIZE $CPPFLAGS"

    AC_RUN_IFELSE([#include "confdefs.h"
#include <sys/time.h>
#include <sys/types.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
int
main(int argc, char* argv[])
{
  int ret;
  fd_set fds;
  struct timeval t;
  t.tv_sec = 0;
  t.tv_usec = 1;
  FD_ZERO(&fds);
  FD_SET(LAM_FD_SETSIZE - 1, &fds);
  if (!FD_ISSET(LAM_FD_SETSIZE - 1, &fds)) {
    return 1;
  }
  FD_ZERO(&fds);
  if (FD_ISSET(LAM_FD_SETSIZE - 1, &fds)) {
    return 1;
  }
  FD_SET(0, &fds);
  ret = select(LAM_FD_SETSIZE - 1, &fds, NULL, NULL, &t);
  if (ret < 0) {
    return 1;
  }
  return 0;
}], , [LAM_FD_SETSIZE=-1], [LAM_FD_SETSIZE=0])

    if test "$LAM_FD_SETSIZE" = "0" ; then
        AC_MSG_RESULT([cross-compiling, setting to default])
    elif test "$LAM_FD_SETSIZE" = "-1" ; then
        AC_MSG_RESULT([no])
        AC_MSG_WARN([*** The specified FD_SETSIZE of $LAM_FD_SETSIZE does not work.])
        AC_MSG_WARN([*** Try using a smaller FD_SETSIZE.  This feature not available])
        AC_MSG_WARN([*** on all platforms - see the LAM platform notes in the])
        AC_MSG_WARN([*** Installation Guide])
        AC_MSG_ERROR([*** Can not continue])
    else
        AC_MSG_RESULT([yes])
    fi

    CPPFLAGS="$CPPFLAGS_orig"
fi


#
# Finish up - set what needs to be set
#
AC_DEFINE_UNQUOTED(LAM_FD_SETSIZE, $LAM_FD_SETSIZE, [FD_SETSIZE for LAM applications])
AC_MSG_CHECKING([for FD_SETSIZE])
if test ! "$LAM_FD_SETSIZE" = "0" ; then
    AC_MSG_RESULT([$LAM_FD_SETSIZE])

    CPPFLAGS="$CPPFLAGS -DFD_SETSIZE=$LAM_FD_SETSIZE"
    CXXCPPFLAGS="$CXXCPPFLAGS -DFD_SETSIZE=$LAM_FD_SETSIZE"
    WRAPPER_CFLAGS="$WRAPPER_CFLAGS -DFD_SETSIZE=$LAM_FD_SETSIZE"
    WRAPPER_CXXFLAGS="$WRAPPER_CXXFLAGS -DFD_SETSIZE=$LAM_FD_SETSIZE"
else
    AC_MSG_RESULT([using system defaults])
fi

AC_LANG_POP(C)
])dnl
