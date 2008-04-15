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
dnl $Id: lam_get_shmmax.m4,v 1.4 2002/10/09 20:55:12 brbarret Exp $
dnl

AC_DEFUN([LAM_GET_SHMMAX],[
lam_shmpool=[$]$2
# Determine maximum shared segment size.
AC_TRY_RUN([
#include <stddef.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define LAM_SHMSHORT $1
#define LAM_SHMPOOL $lam_shmpool
#define MB (1024*1024)

int
test_alloc(int size)
{
    char		*base;
    int			shmid;
    shmid = shmget(IPC_PRIVATE, size, 0600 | IPC_CREAT);
    if (shmid < 0) return(0);
    base = (char *) shmat(shmid, (char *) 0, 0);
    if (base == (char *) -1) {
	shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
	return(0);
    }
    shmdt(base);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    return(1);
}
int
main(int argc, char* argv[])
{
    int	poolsize, maxalloc;
    FILE *f=fopen("conftestval", "w");
    if (!f) return 1;
    if (LAM_SHMPOOL > 0) {
	if (LAM_SHMPOOL >= 2 * MB) maxalloc = MB;
	else maxalloc = LAM_SHMPOOL;
        fprintf(f, "%d\t%d\t%d\n", 0, LAM_SHMPOOL, maxalloc);
	fclose(f);
	return(0);    
    }
    if (test_alloc(32*MB)) { poolsize = 16*MB; maxalloc = MB; }
    else if (test_alloc(16*MB)) { poolsize = 14*MB; maxalloc = MB; }
    else if (test_alloc(8*MB)) { poolsize = 6*MB; maxalloc = MB; }
    else if (test_alloc(4*MB)) { poolsize = 2*MB; maxalloc = MB; }
    else if (test_alloc(2*MB)) { poolsize = MB; maxalloc = MB/2; }
    else if (test_alloc(MB)) { 
	poolsize = MB - (8*7 * (LAM_SHMSHORT+128)) - 256; 
	if (poolsize < 0) poolsize =  MB - (4*3 * (LAM_SHMSHORT+128)) - 256;
	if (poolsize < 0) poolsize = 0; 
        maxalloc = poolsize; }
    else { poolsize = 0; maxalloc = 0; }
    fprintf(f, "%d\t%d\n", poolsize, maxalloc);
    fclose(f);
    return(0);
}], [HAPPY=1], [HAPPY=0], [HAPPY=0])

if test "$HAPPY" = "1"; then
    if test [$]$2 -eq 0; then
	$2=`cat conftestval | cut -f1`
    fi
    if test [$]$3 -eq 0; then
	$3=`cat conftestval | cut -f2`
    fi
else
    AC_MSG_WARN(error compiling/running conftest)
    echo "using default of"
    if test [$]$2 -eq 0; then
	$2=524288
    fi
    if test [$]$3 -eq 0; then
	$3=524288
    fi
fi
unset HAPPY lam_shpool
/bin/rm -f conftest*])dnl
