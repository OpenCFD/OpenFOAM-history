/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_pfs_flush.c,v 1.3 2003/03/25 22:40:06 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_pfs.h"

void ADIOI_PFS_Flush(ADIO_File fd, int *error_code)
{
    int err, np_total, np_comm;
#ifndef PRINT_ERR_MSG
    static char myname[] = "ADIOI_PFS_FLUSH";
#endif

/* fsync is not actually needed in PFS, because it uses something
   called fast-path I/O. However, it doesn't do any harm either. */
    err = fsync(fd->fd_sys);
#ifdef PRINT_ERR_MSG
    *error_code = (err == 0) ? MPI_SUCCESS : MPI_ERR_UNKNOWN;
#else
    if (err == -1) {
	*error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ADIO_ERROR,
			      myname, "I/O Error", "%s", strerror(errno));
	ADIOI_Error(fd, *error_code, myname);	    
    }
    else *error_code = MPI_SUCCESS;
#endif

/* MPI-IO requires that after an fsync all processes must see the same
   file size. In PFS M_ASYNC mode, this doesn't automatically happen.
   Therefore, if M_ASYNC mode, temporarily change it to M_UNIX mode
   and then switch back to M_ASYNC. That updates the file size! */

    MPI_Comm_size(MPI_COMM_WORLD, &np_total);
    MPI_Comm_size(fd->comm, &np_comm);
    if ((np_total == np_comm) && (!(fd->atomicity))) {
	err = _setiomode(fd->fd_sys, M_UNIX);
	err = _setiomode(fd->fd_sys, M_ASYNC);
    }
    /* else it is M_UNIX anyway. don't do anything. */
}
