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
 * $Id: ssi_coll_shmem_barrier.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
 *
 *	Function:	- shmem collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-shmem-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-shmem.h>
#include <mpi.h>
#include <mpisys.h>


/*
 *	barrier
 *
 *	Function:	- barrier using shmem algorithm
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_shmem_barrier(MPI_Comm comm)
{
  int rank;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  MPI_Comm_rank(comm, &rank);

  /* barrier is done with respect to the NUM_SEGMENTS which is default for
     a regular barrier that is not related to any other shmem collective 
     call */
  return lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
}
