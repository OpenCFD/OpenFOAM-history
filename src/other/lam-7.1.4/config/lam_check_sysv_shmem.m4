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
dnl $Id: lam_check_sysv_shmem.m4,v 1.4 2004/01/20 03:41:47 jsquyres Exp $
dnl


AC_DEFUN([LAM_SYSV_SHMEM_TRY_RUN], [
AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

AC_TRY_RUN([#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 512
#endif

int main(int argc, char **argv){
  int shmid;
  struct shmid_ds *buf;

  /* Create a private shared memory segment */
  shmid = shmget(IPC_PRIVATE, PAGE_SIZE, 0777);
  /* If successful try to de-allocate it */
  if( shmid == -1 || shmctl(shmid, IPC_RMID, buf) == -1 ){
    exit(3); /* one of them failed */
  }
  return 0; /* shmget() and shmctl() work */
}], 
[$1], [$2], [$3])

AC_LANG_POP(C)
])dnl


AC_DEFUN([LAM_CHECK_SYSV_SHMEM],[
#
# Arguments: none
#
# Dependencies: None
#
# Sets:
# LAM_HAVE_SYSV_SHMEM (variable)
# LAM_HAVE_SYSV_SHMEM (ac_define_un)
#
# check for sysv shared memory support.  
#
# Save the current language (just in case)
AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH(C)

LIBS_orig="$LIBS"

lam_have_sysv_shmem=0
lam_out_msg=""
lam_sysv_shmem_libs="-lrt -lcygipc"
LAM_SYSV_SHMEM_EXTRA_LIBS=""

#
# Simple first test - do we have the right header files?
#
AC_MSG_CHECKING([for struct shmid_ds])
AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>],
[struct shmid_ds buf; buf.shm_segsz = 0;],
lam_have_sysv_shmem=1 lam_out_msg="yes", 
lam_have_sysv_shmem=0 lam_out_msg="no")
AC_MSG_RESULT([$lam_out_msg])

#
# Only do this if we need to - ie, we passed the above test
# Try a very simple example.  We don't actually try to use the
# shared memory that is created. We try to de-allocate the 
# shared memory using shmctl() call
#
if test "$lam_have_sysv_shmem" = "1"; then
    AC_MSG_CHECKING([if SYSV shared memory works])
    LAM_SYSV_SHMEM_TRY_RUN([lam_have_sysv_shmem=1 lam_out_msg="yes"],
                           [lam_have_sysv_shmem=0 lam_out_msg="no"],
                           [lam_have_sysv_shmem=0 lam_out_msg="yes"])
    AC_MSG_RESULT([$lam_out_msg])

    # if we didn't work, try linking in some libraries, see if that helps
    if test "$lam_have_sysv_shmem" = "0"; then
        for shmemlib in $lam_sysv_shmem_libs; do
            AC_MSG_CHECKING([if SYSV shared memory works with $shmemlib])
	    LIBS="$LIBS_orig $shmemlib"
            LAM_SYSV_SHMEM_TRY_RUN([lam_have_sysv_shmem=1 lam_out_msg="yes"],
                                   [lam_have_sysv_shmem=0 lam_out_msg="no"],
                                   [lam_have_sysv_shmem=0 lam_out_msg="yes"])
            AC_MSG_RESULT([$lam_out_msg])

            if test "$lam_have_sysv_shmem" = "1"; then
                LAM_SYSV_SHMEM_EXTRA_LIBS="$shmemlib"
                break
            fi
        done
    fi
fi

#
# The final "test", just to make it obvious what happened
#
AC_MSG_CHECKING([for SYSV shared memory support])
if test "$lam_have_sysv_shmem" = "1"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
    case "$host" in
        *cygwin*)
            cat <<EOF

********************************************************************
  WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING 
********************************************************************
   We could not detect SYSV shared memory support for your 
   machine. Please check whether you have a running "ipc-daemon2".
   If NOT, please run this daemon and try configuring again.
********************************************************************
       
EOF
        ;;
        *)
            cat <<EOF

********************************************************************
  WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING 
********************************************************************
   We could not detect SYSV shared memory support for your 
   If you believe your system has SYSV shared memory support, please
   contact the LAM/MPI mailing list (lam@lam-mpi.org)
********************************************************************
       
EOF
        ;;
    esac
fi

LAM_HAVE_SYSV_SHMEM=$lam_have_sysv_shmem
AC_DEFINE_UNQUOTED(LAM_HAVE_SYSV_SHMEM, $LAM_HAVE_SYSV_SHMEM,
    [Whether we have SYSV Shared memory support or not])

# Restore
LIBS="$LIBS_orig"

unset lam_out_msg lam_have_sysv_shmem LIBS_orig
AC_LANG_POP(C)
])dnl
