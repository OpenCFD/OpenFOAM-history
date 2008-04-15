/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: read_alle.c,v 1.5 2003/03/25 22:41:02 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "mpioimpl.h"

#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_File_read_all_end = PMPI_File_read_all_end
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_File_read_all_end MPI_File_read_all_end
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_File_read_all_end as PMPI_File_read_all_end
/* end of weak pragmas */
#endif

/* Include mapping from MPI->PMPI */
#define MPIO_BUILD_PROFILING
#include "mpioprof.h"
#endif

/*@
    MPI_File_read_all_end - Complete a split collective read using individual file pointer

Input Parameters:
. fh - file handle (handle)

Output Parameters:
. buf - initial address of buffer (choice)
. status - status object (Status)

.N fortran
@*/
int MPI_File_read_all_end(MPI_File fh, void *buf, MPI_Status *status)
{
#ifndef PRINT_ERR_MSG
    int error_code;
    static char myname[] = "MPI_FILE_IREAD";
#endif

     /* Bogusness to silence compiler warnings */

    if (buf == 0)
      if (status == 0);

#ifdef PRINT_ERR_MSG
    if ((fh <= (MPI_File) 0) || (fh->cookie != ADIOI_FILE_COOKIE)) {
	FPRINTF(stderr, "MPI_File_read_all_end: Invalid file handle\n");
	MPI_Abort(MPI_COMM_WORLD, 1);
    }
#else
    ADIOI_TEST_FILE_HANDLE(fh, myname);
#endif

    if (!(fh->split_coll_count)) {
#ifdef PRINT_ERR_MSG
        FPRINTF(stderr, "MPI_File_read_all_end: Does not match a previous MPI_File_read_all_begin\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
#else
	error_code = MPIR_Err_setmsg(MPI_ERR_IO, MPIR_ERR_NO_SPLIT_COLL,
                              myname, (char *) 0, (char *) 0);
	return ADIOI_Error(fh, error_code, myname);
#endif
    }

    fh->split_coll_count = 0;

    return MPI_SUCCESS;
}
