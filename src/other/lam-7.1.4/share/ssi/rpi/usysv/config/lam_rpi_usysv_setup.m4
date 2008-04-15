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
dnl $Id: lam_rpi_usysv_setup.m4,v 1.5 2004/01/28 02:30:04 jsquyres Exp $
dnl

AC_DEFUN([LAM_RPI_USYSV_SETUP],[
#
# Arguments: none
#
# Dependencies: None
#
# Common setup for the shared memory RPIs
#
# make sure we aren't using sysv on a platform without
# sysv semaphores
#

LAM_CHECK_SYSV_SHMEM
LAM_CHECK_SYSV_SEMAPHORES

if test "$LAM_HAVE_SYSV_SEMAPHORES" = "0" -o "$LAM_HAVE_SYSV_SHMEM" = "0"; then
    AC_MSG_WARN([*** usysv RPI cannot be built on a machine without])
    AC_MSG_WARN([*** SysV semaphores or shared memory.])
    AC_MSG_ERROR([*** Aborting building usysv RPI.])
fi

#
# Set the tunable params so that the RPI compiles with them
#

SHMSHORT_DEFAULT=8192
SHMSHORT="$SHMSHORT_DEFAULT"
SHMPOOL=0
SHMALLOC=0

#
# Short shared memory message size
#

AC_ARG_WITH(shmshort, 
    AC_HELP_STRING([--with-shm-short=BYTES],
		   [obsolete synonym for --with-rpi-usysv-short]),
    [SHMSHORT=$withval])
AC_ARG_WITH(rpi-usysv-short, 
    AC_HELP_STRING([--with-rpi-usysv-short=BYTES],
		   [use BYTES as the size of the longest shared memory short message (default $SHMSHORT_DEFAULT)]),
    [SHMSHORT=$withval])

AC_MSG_CHECKING([user-specified shared memory short message length])
AC_MSG_RESULT([$SHMSHORT bytes])

#
# Shared memory pool size
#

AC_ARG_WITH(shm-poolsize, 
    AC_HELP_STRING([--with-shm-poolsize=BYTES],
		   [obsolete synonym for --with-rpi-usysv-short]),
    [SHMPOOL=$withval])
AC_ARG_WITH(rpi-usysv-poolsize, 
    AC_HELP_STRING([--with-rpi-usysv-poolsize=BYTES],
		   [make the shared memory pool BYTES in size]),
    [SHMPOOL=$withval])

AC_MSG_CHECKING([user-specified shared memory pool size])
if test "$SHMPOOL" = "0" -o "$SHMPOOL" = "no" -o "$SHMPOOL" = "yes"; then
    AC_MSG_RESULT([need to calculate])
    SHMPOOL=0
else
    AC_MSG_RESULT([$SHMPOOL])
fi

#
# Maximum alloc size
#

AC_ARG_WITH(shm-maxalloc, 
    AC_HELP_STRING([--with-shm-maxalloc=BYTES],
		   [obsolete synonym for --with-rpi-usysv-maxalloc]),
    [SHMALLOC=$withval])
AC_ARG_WITH(rpi-usysv-maxalloc, 
    AC_HELP_STRING([--with-rpi-usysv-maxalloc=BYTES],
		   [make BYTES the maximum allocation from the shared pool]),
    [SHMALLOC=$withval])

AC_MSG_CHECKING([user-specified maximum shared memory allocation])
if test "$SHMALLOC" = "0" -o "$SHMALLOC" = "no" -o "$SHMALLOC" = "yes"; then
    AC_MSG_RESULT([need to calculate])
    SHMALLOC=0
else
    AC_MSG_RESULT([$SHMALLOC bytes])
fi

#
# If the user didn't specify values for SHMPOOL or SHMALLOC, calculate
# them
#

if test "$SHMPOOL" = "0"; then
    if test "$SHMALLOC" = "0"; then
	AC_MSG_CHECKING([shared memory poolsize])
	# config/lam_get_shmmax.m4
	LAM_GET_SHMMAX($SHMSHORT,SHMPOOL,SHMALLOC)
	AC_MSG_RESULT([$SHMPOOL bytes])
	AC_MSG_CHECKING([shared memory maxalloc])
	AC_MSG_RESULT([$SHMALLOC bytes])
    else
	AC_MSG_CHECKING([shared memory poolsize])
	# config/lam_get_shmmax.m4
	LAM_GET_SHMMAX($SHMSHORT,SHMPOOL,SHMALLOC)
	AC_MSG_RESULT([$SHMPOOL bytes])
    fi
else
    if test "$SHMALLOC" = "0"; then
	AC_MSG_CHECKING([shared memory maxalloc])
	# config/lam_get_shmmax.m4
	LAM_GET_SHMMAX($SHMSHORT,SHMPOOL,SHMALLOC)
	AC_MSG_RESULT([$SHMALLOC bytes])
    fi
fi

if test \( $SHMPOOL -eq 0 \) -o \( $SHMALLOC -eq 0 \); then
    AC_MSG_ERROR([determining shared memory parameters
Please check that your system supports SYSV shared 
memory segments of 1MB or more.
You can also try explicitly setting both the poolsize and 
maxalloc parameters.])
fi 

AC_SUBST(SHMSHORT)
AC_SUBST(SHMPOOL)
AC_SUBST(SHMALLOC)

#
# Did the user specify to use a shared pthread lock instead of the
# normal sysv semaphores?
#

WANT_PTHREAD_LOCK=0
WANT_PTHREAD_LOCK_STRING=no
AC_ARG_WITH(pthread_lock, 
    AC_HELP_STRING([--with-pthread-lock],
		   [use pthread mutex lock for shared memory pool]),
    [WANT_PTHREAD_LOCK_STRING=$withval])

AC_MSG_CHECKING([if want use of pthread lock])
AC_MSG_RESULT([$WANT_PTHREAD_LOCK_STRING])
if test "$with_pthread_lock" = "yes"; then
    # ../../../../config/lam_mutex_pshared.m4
    LAM_MUTEX_PSHARED
    if test "$lam_ac_mutex_pshared" = no; then
	echo 'ignoring --with-pthread-lock'
    else
	WANT_PTHREAD_LOCK=1
    fi
fi
AC_DEFINE_UNQUOTED(PTHREAD_FREELOCK, $WANT_PTHREAD_LOCK,
    [Whether to use shared pthread locks instead of sysv semaphores for the shared memory area])
])dnl
