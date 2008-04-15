/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: ad_pfs_write.c,v 1.3 2003/03/25 22:40:07 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_pfs.h"
#ifdef PROFILE
#include "mpe.h"
#endif

void ADIOI_PFS_WriteContig(ADIO_File fd, void *buf, int count, 
                     MPI_Datatype datatype, int file_ptr_type,
                     ADIO_Offset offset, ADIO_Status *status, int *error_code)
{
    int err=-1, datatype_size, len;
#ifndef PRINT_ERR_MSG
    static char myname[] = "ADIOI_PFS_WRITECONTIG";
#endif

    MPI_Type_size(datatype, &datatype_size);
    len = datatype_size * count;

    if (file_ptr_type == ADIO_EXPLICIT_OFFSET) {
        if (fd->fp_sys_posn != offset) {
#ifdef PROFILE
	    MPE_Log_event(11, 0, "start seek");
#endif
            lseek(fd->fd_sys, offset, SEEK_SET);
#ifdef PROFILE
	    MPE_Log_event(12, 0, "end seek");
#endif
	}
#ifdef PROFILE
	MPE_Log_event(5, 0, "start write");
#endif
        err = _cwrite(fd->fd_sys, buf, len);
#ifdef PROFILE
	MPE_Log_event(6, 0, "end write");
#endif
        fd->fp_sys_posn = offset + err;
         /* individual file pointer not updated */        
    }
    else { /* write from curr. location of ind. file pointer */
        if (fd->fp_sys_posn != fd->fp_ind) {
#ifdef PROFILE
	    MPE_Log_event(11, 0, "start seek");
#endif
            lseek(fd->fd_sys, fd->fp_ind, SEEK_SET);
#ifdef PROFILE
	    MPE_Log_event(12, 0, "end seek");
#endif
	}
#ifdef PROFILE
	MPE_Log_event(5, 0, "start write");
#endif
        err = _cwrite(fd->fd_sys, buf, len);
#ifdef PROFILE
	MPE_Log_event(6, 0, "end write");
#endif
        fd->fp_ind += err;
        fd->fp_sys_posn = fd->fp_ind;
    }

#ifdef HAVE_STATUS_SET_BYTES
    if (err != -1) MPIR_Status_set_bytes(status, datatype, err);
#endif

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


void ADIOI_PFS_WriteStrided(ADIO_File fd, void *buf, int count,
                       MPI_Datatype datatype, int file_ptr_type,
                       ADIO_Offset offset, ADIO_Status *status, int
                       *error_code)
{
    ADIOI_GEN_WriteStrided(fd, buf, count, datatype, file_ptr_type,
                        offset, status, error_code);
}
