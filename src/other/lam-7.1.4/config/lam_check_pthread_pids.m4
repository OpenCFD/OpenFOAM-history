dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
dnl $Id: lam_check_pthread_pids.m4,v 1.7 2003/09/27 04:09:04 jsquyres Exp $
dnl

define(LAM_CHECK_PTHREAD_PIDS,[
#
# Arguments: none
#
# Dependencies: None
#
# Sets:
#  LAM_THREADS_HAVE_DIFFERENT_PIDS (variable)
#
# Test for Linux-like threads in the system. We will need to handle things like
# getpid() differently in the case of a Linux-like threads model.
#

AH_TEMPLATE([LAM_THREADS_HAVE_DIFFERENT_PIDS],
    [Do threads have different pids (pthreads on linux)])

AC_MSG_CHECKING([if threads have different pids (pthreads on linux)])
CPPFLAGS_save="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $THREAD_CPPFLAGS"
CFLAGS_save="$CFLAGS"
CFLAGS="$CFLAGS $THREAD_CFLAGS"
LDFLAGS_save="$LDFLAGS"
LDFLAGS="$LDFLAGS $THREAD_LDFLAGS"
LIBS_save="$LIBS"
LIBS="$LIBS $THREAD_LIBS"
AC_TRY_RUN([#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
void *checkpid(void *arg);
int main(int argc, char* argv[]) {
  pthread_t thr;
  int pid, retval;
  pid = getpid();
  pthread_create(&thr, NULL, checkpid, &pid);
  pthread_join(thr, (void **) &retval);
  exit(retval);
}
void *checkpid(void *arg) {
   int ret;
   int ppid = *((int *) arg);
   if (ppid == getpid())
     ret = 0;
   else
     ret = 1;
     pthread_exit((void *) ret);
}], 
[MSG=no LAM_THREADS_HAVE_DIFFERENT_PIDS=0], 
[MSG=yes LAM_THREADS_HAVE_DIFFERENT_PIDS=1])

CFLAGS="$CFLAGS_save"
CPPFLAGS="$CPPFLAGS_save"
LDFLAGS="$LDFLAGS_save"
LIBS="$LIBS_save"

AC_MSG_RESULT([$MSG])
AC_DEFINE_UNQUOTED(LAM_THREADS_HAVE_DIFFERENT_PIDS, $LAM_THREADS_HAVE_DIFFERENT_PIDS)

#
# if pthreads is not available, then the system does not have an insane threads
# model
#
unset MSG])dnl
