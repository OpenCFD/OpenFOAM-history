/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   $Id: status_setb.c,v 1.8 2003/04/21 21:17:19 nsanghvi Exp $    
 *
 *   Copyright (C) 1997 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "adio.h"
#include "mpi.h"

#if defined(MPICH2)
/* Not quite correct, but much closer for MPI2 */
int MPIR_Status_set_bytes(MPI_Status *status, MPI_Datatype datatype, 
			  int nbytes)
{
    if (status != MPI_STATUS_IGNORE)
        MPI_Status_set_elements(status, MPI_BYTE, nbytes);
    return MPI_SUCCESS;
}
#elif defined(MPICH)

void MPID_Status_set_bytes(MPI_Status *status, int nbytes);
int MPIR_Status_set_bytes(MPI_Status *status, MPI_Datatype datatype, 
			  int nbytes);

int MPIR_Status_set_bytes(MPI_Status *status, MPI_Datatype datatype, 
			  int nbytes)
{
    if (status != MPI_STATUS_IGNORE)
        MPID_Status_set_bytes(status, nbytes);
    return MPI_SUCCESS;
}

#elif LAM_MPI

/* Added LAM version by NAS on 22 Feb 2003 */

int MPIR_Status_set_bytes(MPI_Status *status, MPI_Datatype datatype,
                          int nbytes);

int MPIR_Status_set_bytes(MPI_Status *status, MPI_Datatype datatype,
                          int nbytes)
{
  /* Bogusness to silence compiler warnings */

  if (datatype == MPI_DATATYPE_NULL);

  if (status != MPI_STATUS_IGNORE)
      status->st_length = nbytes;
  return MPI_SUCCESS;
}

#endif

