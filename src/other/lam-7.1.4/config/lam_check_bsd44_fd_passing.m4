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
dnl $Id: lam_check_bsd44_fd_passing.m4,v 1.12 2003/10/10 15:54:58 pkambadu Exp $
dnl


define(LAM_CHECK_BSD44_FD_PASSING,[
#
# Arguments: none
#
# Dependencies: None
#
# Sets:
# LAM_HAVE_RFC2292_FD_PASSING (variable)
# LAM_HAVE_RFC2292_FD_PASSING (ac_define_un)
# LAM_HAVE_POSIX_1G_FD_PASSING (variable)
# LAM_HAVE_POSIX_1G_FD_PASSING (ac_define_un)
#
# Check for BSD44-style FD passing, both using the macros defined in
# RFC2292 (advanced API for IPv6) and POSIX.1g.  Only one of the two
# will be defined to 1, with the other being defined to 0.  Preference
# is given to RFC2292, as the macros are cleaner and seem to work
# "better" on systems that support RFC2292.
#

# Save the current language (just in case)
AC_LANG_SAVE
AC_LANG_C

lam_old_LIBS="$LIBS"
LIBS="$LIBLAM_EXTRA_LIBS"

lam_have_44=0
lam_have_working_rfc=0
lam_have_working_posix=0
lam_out_msg=""


#
# Simple first test - do we have the right header files?
#
AC_MSG_CHECKING([for POSIX.1g struct msghdr])
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>], 
[struct cmsghdr c; 
struct msghdr m; 
c.cmsg_type = 0;
m.msg_control = 0;],
lam_have_44=1 lam_out_msg="yes", lam_have_44=0 lam_out_msg="no")
AC_MSG_RESULT([$lam_out_msg])


#
# Try passing some file descriptors using RFC2292 macros, if it 
# looks like we might be able to succeed
#
if test "$lam_have_44" = "1"; then
AC_MSG_CHECKING([fd passing using RFC2292 API])
LAM_TRY_RUN_FD_PASSING([#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define LAM_HAVE_RFC2292_FD_PASSING 1
#include <conftest_fd_include.c>

/*
 * Global so that we can kill the child if the parent barfs
 */
pid_t child_pid = 0;

int sfh_send_fd(int stream, int fd);
int sfh_recv_fd(int stream);

int
main(int argc, char **argv)
{
  int fd[2];
  int status = 0;

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd)) {
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

  return status;
}], 
lam_have_working_rfc=1 lam_out_msg="passed",
lam_have_working_rfc=0 lam_out_msg="failed",
lam_have_working_rfc=1 lam_out_msg="passed")
AC_MSG_RESULT($lam_out_msg)
fi


#
# If RFC didn't work, try using the older POSIX.1g API
#
if test "$lam_have_44" = "1" -a "$lam_have_working_rfc" = "0"; then
AC_MSG_CHECKING([fd passing using POSIX.1g API])
LAM_TRY_RUN_FD_PASSING([#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define LAM_HAVE_POSIX_1G_FD_PASSING 1
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

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd)) {
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
    return 0; /* child recieved the fd */
  }
  else {
   return 1;
 } 
}], 
lam_have_working_posix=1 lam_out_msg="passed",
lam_have_working_posix=0 lam_out_msg="failed",
lam_have_working_posix=1 lam_out_msg="passed")
AC_MSG_RESULT($lam_out_msg)
fi

#
# The final "test", just to make it obvious what happened
#
if test "$lam_have_working_rfc" = "1"; then
  lam_out_msg="yes (RFC2292 API)"
elif test "$lam_have_working_posix" = "1"; then
  lam_out_msg="yes (POSIX.1g)"
else
  lam_out_msg="no"
fi
AC_MSG_CHECKING([for BSD 4.4 fd passing support])
AC_MSG_RESULT($lam_out_msg)

LAM_HAVE_RFC2292_FD_PASSING=$lam_have_working_rfc
AC_DEFINE_UNQUOTED(LAM_HAVE_RFC2292_FD_PASSING,
    $LAM_HAVE_RFC2292_FD_PASSING,
    [Whether we have BSD4.4 FD passing using the RFC2292 API])

LAM_HAVE_POSIX_1G_FD_PASSING=$lam_have_working_posix
AC_DEFINE_UNQUOTED(LAM_HAVE_POSIX_1G_FD_PASSING,
    $LAM_HAVE_POSIX_1G_FD_PASSING,
    [Whether we have BSD4.4 FD passing using the POSIX.1g API])

# Restore
LIBS=$lam_old_LIBS

unset lam_out_msg lam_have_44 lam_have_working_rfc lam_have_working_posix
AC_LANG_RESTORE])dnl
