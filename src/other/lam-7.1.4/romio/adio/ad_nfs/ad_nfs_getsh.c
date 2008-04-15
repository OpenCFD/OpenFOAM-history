/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_nfs_getsh.c,v 1.3 2003/03/25 22:39:59 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_nfs.h"

/* returns the current location of the shared_fp in terms of the
   no. of etypes relative to the current view, and also increments the
   shared_fp by the number of etypes to be accessed (incr) in the read
   or write following this function. */

void ADIOI_NFS_Get_shared_fp(ADIO_File fd, int incr, ADIO_Offset *shared_fp, 
			 int *error_code)
{
    ADIO_Offset new_fp;
    int err;
    MPI_Comm dupcommself;
#ifndef PRINT_ERR_MSG
    static char myname[] = "ADIOI_NFS_GET_SHARED_FP";
#endif

    if (fd->shared_fp_fd == ADIO_FILE_NULL) {
	MPI_Comm_dup(MPI_COMM_SELF, &dupcommself);
	fd->shared_fp_fd = ADIO_Open(MPI_COMM_SELF, dupcommself, fd->shared_fp_fname, 
             fd->file_system, ADIO_CREATE | ADIO_RDWR | ADIO_DELETE_ON_CLOSE, 
             0, MPI_BYTE, MPI_BYTE, M_ASYNC, MPI_INFO_NULL, 
             ADIO_PERM_NULL, error_code);
	if (*error_code != MPI_SUCCESS) return;
	*shared_fp = 0;
	ADIOI_WRITE_LOCK(fd->shared_fp_fd, 0, SEEK_SET, sizeof(ADIO_Offset));
	err = read(fd->shared_fp_fd->fd_sys, shared_fp, sizeof(ADIO_Offset));
        /* if the file is empty, the above read may return error
           (reading beyond end of file). In that case, shared_fp = 0, 
           set above, is the correct value. */
    }
    else {
	ADIOI_WRITE_LOCK(fd->shared_fp_fd, 0, SEEK_SET, sizeof(ADIO_Offset));
	lseek(fd->shared_fp_fd->fd_sys, 0, SEEK_SET);
	err = read(fd->shared_fp_fd->fd_sys, shared_fp, sizeof(ADIO_Offset));
	if (err == -1) {
	    ADIOI_UNLOCK(fd->shared_fp_fd, 0, SEEK_SET, sizeof(ADIO_Offset));
#ifdef PRINT_ERR_MSG
	    *error_code = MPI_ERR_UNKNOWN;
#else
	    *error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
			      myname, "I/O Error", "%s", strerror(errno));
	    ADIOI_Error(fd, *error_code, myname);	    
#endif
	    return;
	}
    }

    new_fp = *shared_fp + incr;

    lseek(fd->shared_fp_fd->fd_sys, 0, SEEK_SET);
    err = write(fd->shared_fp_fd->fd_sys, &new_fp, sizeof(ADIO_Offset));
    ADIOI_UNLOCK(fd->shared_fp_fd, 0, SEEK_SET, sizeof(ADIO_Offset));
#ifdef PRINT_ERR_MSG
    *error_code = (err == -1) ? MPI_ERR_UNKNOWN : MPI_SUCCESS;
#else
    if (err == -1) {
	*error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
			      myname, "I/O Error", "%s", strerror(errno));
	ADIOI_Error(fd, *error_code, myname);	    
    }
    else *error_code = MPI_SUCCESS;
#endif
}
