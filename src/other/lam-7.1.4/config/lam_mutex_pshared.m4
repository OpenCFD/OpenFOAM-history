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
dnl $Id: lam_mutex_pshared.m4,v 1.3 2002/10/09 20:55:12 brbarret Exp $
dnl

define(LAM_MUTEX_PSHARED,[
AC_MSG_CHECKING(for process shared pthread mutex)
AC_TRY_RUN([#include <pthread.h>
int main(int argc, char* argv[]) {
  pthread_mutex_t m; pthread_mutexattr_t mattr;
  if (pthread_mutexattr_init(&mattr)) return(1);
  if (pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED)) return(1);
  if (pthread_mutex_init(&m, &mattr)) return(-1);
  return(0);
}], lam_ac_mutex_pshared=yes, lam_ac_mutex_pshared=no, lam_ac_mutex_pshared=no)
AC_MSG_RESULT($lam_ac_mutex_pshared)
])
