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
dnl $Id: lam_check_sysv_semaphores.m4,v 1.4 2004/01/20 03:41:47 jsquyres Exp $
dnl

AC_DEFUN([LAM_SYSV_SEMAPHORES_TRY_CREATE], [
AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

AC_TRY_RUN([#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>

#if LAM_HAVE_UNION_SEMUN
static	union semun	semctl_arg;
#else
static union {
	int		val;
	struct semid_ds	*buf;
	unsigned short	*array;
} semctl_arg;
#endif

int main(int argc, char **argv){
  int semid, ret;
  /* Create a private semaphore set */
  semid = semget (IPC_PRIVATE, 1, 0777);
  /* if not successful, abort */
  if (semid < 0) { exit(1); }
  if(semctl(semid, 0, IPC_RMID, semctl_arg) == -1 ){
    exit(2);
  }
  return 0; /* semget() and semctl() work */
}], 
[$1], [$2], [$3])

AC_LANG_POP(C)
])dnl


AC_DEFUN([LAM_SYSV_SEMAPHORES_TRY_SET_NORMAL], [
AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

AC_TRY_RUN([#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>

#if LAM_HAVE_UNION_SEMUN
static	union semun	semctl_arg;
#else
static union {
	int		val;
	struct semid_ds	*buf;
	unsigned short	*array;
} semctl_arg;
#endif

int main(int argc, char **argv){
  int semid, ret;
  /* Create a private semaphore set */
  semid = semget (IPC_PRIVATE, 1, 0777);
  /* if not successful, abort */
  if (semid < 0) { exit(1); }
  /* try to set initial value */
  semctl_arg.val = 1;
  ret = semctl(semid, 0, SETVAL, semctl_arg);
  if (ret != 0) { 
      semctl(semid, 0, IPC_RMID, semctl_arg);
      exit(2);
  }
  /* now look up the value - should be 1 */
  ret = semctl(semid, 0, GETVAL, semctl_arg);
  if (ret != 1) {
      semctl(semid, 0, IPC_RMID, semctl_arg);
      exit(3);
  }
  /* If successful try to release it */
  if(semctl(semid, 0, IPC_RMID, semctl_arg) == -1 ){
    exit(4);
  }
  return 0; /* semget() and semctl() work */
}], 
[$1], [$2], [$3])

AC_LANG_POP(C)
])dnl

AC_DEFUN([LAM_SYSV_SEMAPHORES_TRY_SET_DARWIN], [
AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

AC_TRY_RUN([#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>

#if LAM_HAVE_UNION_SEMUN
static	union semun	semctl_arg;
#else
static union {
	int		val;
	struct semid_ds	*buf;
	unsigned short	*array;
} semctl_arg;
#endif

int main(int argc, char **argv){
  int semid, ret;
  /* Create a private semaphore set */
  semid = semget (IPC_PRIVATE, 1, 0777);
  /* if not successful, abort */
  if (semid < 0) { exit(1); }
  /* try to set initial value */
  semctl_arg.val = 1;
  ret = semctl(semid, 0, SETVAL, semctl_arg.val);
  if (ret != 0) { 
      semctl(semid, 0, IPC_RMID, semctl_arg);
      exit(2);
  }
  /* now look up the value - should be 1 */
  ret = semctl(semid, 0, GETVAL, semctl_arg);
  if (ret != 1) {
      semctl(semid, 0, IPC_RMID, semctl_arg);
      exit(3);
  }
  /* If successful try to release it */
  if(semctl(semid, 0, IPC_RMID, semctl_arg) == -1 ){
    exit(4);
  }
  return 0; /* semget() and semctl() work */
}], 
[$1], [$2], [$3])

AC_LANG_POP(C)
])dnl


AC_DEFUN([LAM_CHECK_SYSV_SEMAPHORES],[
#
# Arguments: none
#
# Dependencies: None
#
# Sets:
# LAM_HAVE_SYSV_SEMAPHORES (variable)
# LAM_HAVE_SYSV_SEMAPHORES (ac_define_un)
#
# check for sysv semaphore support.  
# 

AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

AC_MSG_CHECKING([for union semun in sys/sem.h])
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/sem.h>], [union semun s; s.val;],
[MSG=yes VALUE=1], [MSG=no VALUE=0])
AC_DEFINE_UNQUOTED(LAM_HAVE_UNION_SEMUN, $VALUE, 
    [Whether we have union semun in <sys/sem.h> or not])
AC_MSG_RESULT([$MSG])

LIBS_orig="$LIBS"

lam_have_sysv_sem=0
lam_out_msg=""
lam_sysv_sem_libs="-lrt -lcygipc"
LAM_SYSV_SEMAPHORES_EXTRA_LIBS=""

#
# Simple first test - do we have the right header files?
#
AC_MSG_CHECKING([for struct semid_ds])
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>],
[struct semid_ds buf1; 
buf1.sem_nsems = 0;], 
lam_have_sysv_sem=1 lam_out_msg="yes", 
lam_have_sysv_sem=0 lam_out_msg="no")
AC_MSG_RESULT([$lam_out_msg])

#
# Only do this if we need to - ie, we passed the above test
# Try a very simple example.  We don't actually try to use the
# semaphore that is created. We just discard the semaphore using 
# semctl() call.
#
if test "$lam_have_sysv_sem" = "1"; then
    AC_MSG_CHECKING([if able to create SYSV semaphores])
    LAM_SYSV_SEMAPHORES_TRY_CREATE([lam_have_sysv_sem=1 lam_out_msg="yes"],
                                [lam_have_sysv_sem=0 lam_out_msg="no"],
                                [lam_have_sysv_sem=0 lam_out_msg="no"])
    AC_MSG_RESULT([$lam_out_msg])

    # if we didn't work, try linking in some libraries, see if that helps
    if test "$lam_have_sysv_sem" = "0"; then
        for semlib in $lam_sysv_sem_libs; do
            AC_MSG_CHECKING([if able to create SYSV semaphores work with $semlib])
            LIBS="$LIBS_orig $semlib"
            LAM_SYSV_SEMAPHORES_TRY_CREATE([lam_have_sysv_sem=1 lam_out_msg="yes"],
                                        [lam_have_sysv_sem=0 lam_out_msg="no"],
                                        [lam_have_sysv_sem=0 lam_out_msg="no"])
            AC_MSG_RESULT([$lam_out_msg])

            if test "$lam_have_sysv_sem" = "1"; then
                LAM_SYSV_SEMAPHORES_EXTRA_LIBS="$semlib"
                break
            fi
        done
    fi
fi

lam_semun_val_direct=0
if test "$lam_have_sysv_sem" = "1" ; then
    # now double check that we can use the semaphores
    # see long comment in share/include/lam_config_bottom.h as to why
    # all of this is really needed...
    AC_MSG_CHECKING([if SYSV semaphores work])
    LAM_SYSV_SEMAPHORES_TRY_SET_NORMAL([lam_have_sysv_sem=1 lam_out_msg="yes"],
                                        [lam_have_sysv_sem=0 lam_out_msg="no"],
                                        [lam_have_sysv_sem=0 lam_out_msg="no"])
    AC_MSG_RESULT([$lam_out_msg])
    if test "$lam_have_sysv_sem" = "0" ; then
        # see if the darwin way works
        AC_MSG_CHECKING([if SYSV semaphores work the Darwin way])
        LAM_SYSV_SEMAPHORES_TRY_SET_DARWIN([lam_have_sysv_sem=1 lam_out_msg="yes" lam_semun_val_direct=1],
                                        [lam_have_sysv_sem=0 lam_out_msg="no"],
                                        [lam_have_sysv_sem=0 lam_out_msg="no"])
        AC_MSG_RESULT([$lam_out_msg])
    fi 
fi
AC_DEFINE_UNQUOTED([LAM_UNION_SEMUN_VAL_DIRECT], [$lam_semun_val_direct],
                   [Whether the 4th argument to semctl (when the val argument) should be passed directly (Darwin 64bit) or through the union (everywhere else)])

#
# The final "test", just to make it obvious what happened
#
AC_MSG_CHECKING([for SYSV semaphore support])
if test "$lam_have_sysv_sem" = "1"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
    case "$host" in
        *cygwin*)
            cat <<EOF

***************************************************************
WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
***************************************************************
  We could not detect SYSV semaphore support on your machine. 
  Please check whether "ipc-daemon2" is running on the machine.
***************************************************************
WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
***************************************************************
       
EOF
        ;;
        *)
            cat <<EOF 

***************************************************************
WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
***************************************************************
  We could not detect SYSV semaphore support on your machine. 
  If you believe your system has SYSV semaphore support, please 
  contact the LAM/MPI mailing list (lam@lam-mpi.org)
***************************************************************
WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
***************************************************************
       
EOF
        ;;
    esac
fi

LAM_HAVE_SYSV_SEMAPHORES=$lam_have_sysv_sem
AC_DEFINE_UNQUOTED(LAM_HAVE_SYSV_SEMAPHORES, $LAM_HAVE_SYSV_SEMAPHORES,
                   [Whether we have SYSV Semaphores support or not])

# Restore
LIBS="${LIBS_orig}"

unset lam_out_msg lam_have_sysv_sem lam_sysv_sem_libs
AC_LANG_POP(C)

])dnl
