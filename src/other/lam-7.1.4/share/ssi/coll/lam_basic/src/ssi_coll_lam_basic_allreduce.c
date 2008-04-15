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
 * $Id: ssi_coll_lam_basic_allreduce.c,v 1.5 2004/03/02 03:43:36 jsquyres Exp $
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
 *	allreduce
 *
 *	Function:	- allreduce using other MPI collectives
 *	Accepts:	- same as MPI_Allreduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_allreduce(void *sbuf, void *rbuf, int count,
				 MPI_Datatype dtype, MPI_Op op,
				 MPI_Comm comm)
{
  int err;

  /* Reduce to 0 and broadcast. */

  err = comm->c_ssi_coll.lsca_reduce(sbuf, rbuf, count, dtype, 
				     op, 0, comm);
  if (err != MPI_SUCCESS)
    return err;

  return comm->c_ssi_coll.lsca_bcast(rbuf, count, dtype, 0, comm);
}

/*
 *	allreduce
 *
 *	Function:	- intercomm allreduce using other MPI collectives
 *	Accepts:	- same as MPI_Allreduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_allreduce_inter(void *sbuf, void *rbuf, int count,
				       MPI_Datatype dtype, MPI_Op op,
				       MPI_Comm intercomm)
{
  int local_rank, root;
  MPI_Comm local_comm;
  int is_low;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;
  is_low = intercomm->c_ssi_coll_lam_basic_data->is_low;

  /* Do a intercommunicator Reduce and then do a local broadcast. This does
     not need temporary buffer */
  MPI_Comm_rank(intercomm, &local_rank);
  if (is_low) {
    /* Lower group */
    root = (local_rank == 0) ? MPI_ROOT : MPI_PROC_NULL;
    if (MPI_Reduce(sbuf, rbuf, count, dtype, op, root, intercomm) 
	!= MPI_SUCCESS) {
      return LAMERROR;
    }
    root = 0;
    if (MPI_Reduce(sbuf, rbuf, count, dtype, op, root, intercomm) 
	!= MPI_SUCCESS) {
      return LAMERROR;
    }
  } else {
    /* Higher group */
    root = 0;
    if (MPI_Reduce(sbuf, rbuf, count, dtype, op, root, intercomm) 
	!= MPI_SUCCESS) {
      return LAMERROR;
    }
    
    root = (local_rank == 0) ? MPI_ROOT : MPI_PROC_NULL;
    if (MPI_Reduce(sbuf, rbuf, count, dtype, op, root, intercomm) 
	!= MPI_SUCCESS) {
      return LAMERROR;
    }
  }
  
  /* Now Broadcast the result to all processes in the local comm */
  if ((MPI_Bcast(rbuf, count, dtype, 0, local_comm)) != MPI_SUCCESS) {
    return LAMERROR;
  }

  return MPI_SUCCESS;
}

