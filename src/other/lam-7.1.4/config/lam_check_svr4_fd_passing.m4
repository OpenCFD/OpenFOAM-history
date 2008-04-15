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
dnl $Id: lam_check_svr4_fd_passing.m4,v 1.10 2003/10/10 15:54:58 pkambadu Exp $
dnl


define(LAM_CHECK_SVR4_FD_PASSING,[
#
# Arguments: none
#
# Dependencies: None
#
# Sets:
# LAM_HAVE_SVR4_FD_PASSING (variable)
# LAM_HAVE_SVR4_FD_PASSING (ac_define_un)
#
# check for SVR4-style fd passing.  Pretty much copied out of 
# Stevens APUE, pg 481-483, modified to make sure it cleans up after
# itself...

# Save the current language (just in case)
AC_LANG_SAVE
AC_LANG_C

lam_old_LIBS="$LIBS"
LIBS="$LIBLAM_EXTRA_LIBS"

lam_have_svr4=0
lam_have_working_svr4=0
lam_out_msg=""


#
# Simple first test - do we have the right header files?
#
AC_MSG_CHECKING([for System V Release 4 for struct strrecvfd])
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stropts.h>], [struct strrecvfd c; c.fd = 0;],
lam_have_svr4=1 lam_out_msg="yes", lam_have_svr4=0 lam_out_msg="no")
AC_MSG_RESULT([$lam_out_msg])


#
# Only do this if we need to - ie, we passed the above test
# Try a very simple example.  We don't actually try to use the
# fds that are passed, but if we get as far as this test goes, 
# we are probably ok.
#
if test "$lam_have_svr4" = "1"; then
AC_MSG_CHECKING([System V Release 4 fd passing example])
LAM_TRY_RUN_FD_PASSING([#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stropts.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define LAM_HAVE_SVR4_FD_PASSING 1
#include <conftest_fd_include.c>

/*
 * Global so that we can kill the child if the parent barfs
 */
pid_t child_pid = 0;

int
main(int argc, char **argv)
{
  int fd[2];
  int status = 0;

  if (pipe(fd)) {
    perror("pipe");
    exit(1);
  }

  if ((child_pid = fork()) < 0) {
    perror("fork");
  } else if (child_pid > 0) {
    /* I am the parent... */
    if (sfh_send_fd(fd[0], 0)) {
      /* Renice?  You must mean kill -9. */
      kill(child_pid, 9);
      waitpid(child_pid, &status, 0);
      return (2);
    }
    waitpid(child_pid, &status, 0);
  } else {
    /* I am the child... */
    if (sfh_recv_fd(fd[1]) == -1) {
      exit(3);
    }
  }
  if (WEXITSTATUS(status) == 0) {
    return 0; /* chile recieved the fd */
  }
  else {
    return 1;
  }
}], 
lam_have_working_svr4=1 lam_out_msg="passed",
lam_have_working_svr4=0 lam_out_msg="failed",
lam_have_working_svr4=1 lam_out_msg="passed")
AC_MSG_RESULT($lam_out_msg)
fi


#
# The final "test", just to make it obvious what happened
#
if test "$lam_have_working_svr4" = "1"; then
  lam_out_msg="yes"
else
  lam_out_msg="no"
fi
AC_MSG_CHECKING([for System V Release 4 fd passing support])
AC_MSG_RESULT($lam_out_msg)


LAM_HAVE_SVR4_FD_PASSING=$lam_have_working_svr4
AC_DEFINE_UNQUOTED(LAM_HAVE_SVR4_FD_PASSING, $LAM_HAVE_SVR4_FD_PASSING,
    [Whether we have SVR4-stle file descriptor passing or not])

# Restore
LIBS=$lam_old_LIBS

unset lam_out_msg lam_have_svr4 lam_have_working_svr4
AC_LANG_RESTORE])dnl
