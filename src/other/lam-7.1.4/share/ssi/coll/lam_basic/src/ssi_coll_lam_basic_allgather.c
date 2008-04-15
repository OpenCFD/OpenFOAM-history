/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 *
 * $Id: ssi_coll_lam_basic_allgather.c,v 1.5 2004/03/02 03:43:36 jsquyres Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpi.h>
#include <mpisys.h>


/*
 *	allgather
 *
 *	Function:	- allgather using other MPI collections
 *	Accepts:	- same as MPI_Allgather()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_lam_basic_allgather(void *sbuf, int scount, 
				 MPI_Datatype sdtype, void *rbuf, 
				 int rcount, MPI_Datatype rdtype, 
				 MPI_Comm comm)
{
  int size;
  int err;

  /* Gather and broadcast. */

  MPI_Comm_size(comm, &size);

  err = comm->c_ssi_coll.lsca_gather(sbuf, scount, sdtype, rbuf, rcount, 
				     rdtype, 0, comm);
  if (err != MPI_SUCCESS)
    return err;
  
  err = comm->c_ssi_coll.lsca_bcast(rbuf, rcount * size, rdtype, 
				    0, comm);
  return err;
}


/*
 *	allgather
 *
 *	Function:	- intercomm allgather using other MPI collections
 *	Accepts:	- same as MPI_Allgather()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_lam_basic_allgather_inter(void *sbuf, int scount, 
				       MPI_Datatype sdtype, void *rbuf, 
				       int rcount, MPI_Datatype rdtype, 
				       MPI_Comm intercomm)
{
  int local_rank, local_size, remote_size, root;
  char* local_origin = NULL, *local_buffer = NULL;
  MPI_Comm local_comm;
  int is_low;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;
  is_low = intercomm->c_ssi_coll_lam_basic_data->is_low;

  MPI_Comm_rank(intercomm, &local_rank);
  MPI_Comm_size(intercomm, &local_size);
  MPI_Comm_remote_size(intercomm, &remote_size);
  if (local_rank == 0) {
    /* Do a local gather, first allocate space for temprary buffer */
      if ((lam_dtbuffer(sdtype, scount * local_size, &local_buffer,
			&local_origin)) != MPI_SUCCESS) {
	return LAMERROR;
      }
  }
  if ((MPI_Gather(sbuf, scount, sdtype, local_origin, scount, sdtype, 0,
		  local_comm)) != MPI_SUCCESS) {
    return LAMERROR;
  }

  /* Now intercommunicator broadcast for both the groups */
  if (is_low) {
    /* lower group */
    root = (local_rank == 0) ? MPI_ROOT : MPI_PROC_NULL;
    if ((MPI_Bcast(local_origin, local_size * scount, sdtype, root,
		   intercomm)) != MPI_SUCCESS) {
      return LAMERROR;
    }
    
    root = 0;
    if ((MPI_Bcast(rbuf, remote_size * rcount, rdtype, root,
		   intercomm)) != MPI_SUCCESS) {
      return LAMERROR;
    }

  } else {
    /* higher group */
    root = 0;
    if ((MPI_Bcast(rbuf, remote_size * rcount, rdtype, root,
		   intercomm)) != MPI_SUCCESS) {
      return LAMERROR;
    }
 
    root = (local_rank == 0) ? MPI_ROOT : MPI_PROC_NULL;
    if ((MPI_Bcast(local_origin, local_size * scount, sdtype, root,
		   intercomm)) != MPI_SUCCESS) {
      return LAMERROR;
    }
  }
  if (local_rank == 0) 
    free(local_buffer);

  return MPI_SUCCESS;
}


