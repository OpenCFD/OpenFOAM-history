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
 * $Id: ssi_coll_lam_basic_barrier.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
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
#include <blktype.h>
#include <mpisys.h>


/*
 *	barrier_lin
 *
 *	Function:	- barrier using O(N) algorithm
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_barrier_lin(MPI_Comm comm)
{
  int size;
  int rank;
  int err;
  int i;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  lam_mkcoll(comm);

  /* All non-root send & receive zero-length message. */

  if (rank > 0) {
    err = MPI_Send((void *) 0, 0, MPI_BYTE, 0, BLKMPIBARRIER, comm);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }

    err = MPI_Recv((void *) 0, 0, MPI_BYTE, 0, BLKMPIBARRIER, comm, 
		   MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* The root collects and broadcasts the messages. */

  else {
    for (i = 1; i < size; ++i) {
      err = MPI_Recv((void *) 0, 0, MPI_BYTE, MPI_ANY_SOURCE,
		     BLKMPIBARRIER, comm, MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }

    for (i = 1; i < size; ++i) {
      err = MPI_Send((void *) 0, 0, MPI_BYTE, i, BLKMPIBARRIER, comm);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }
  }

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}


/*
 *	barrier_log
 *
 *	Function:	- barrier using O(log(N)) algorithm
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_barrier_log(MPI_Comm comm)
{
  int size;
  int rank;
  int peer;
  int dim;
  int hibit;
  int mask;
  int err;
  int i;

  /* Send null-messages up and down the tree.  Synchronization at the
     root (rank 0). */

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  lam_mkcoll(comm);
  dim = comm->c_cube_dim;
  hibit = lam_hibit(rank, dim);
  --dim;

  /* Receive from children. */

  for (i = dim, mask = 1 << i; i > hibit; --i, mask >>= 1) {
    peer = rank | mask;
    if (peer < size) {
      err = MPI_Recv((void *) 0, 0, MPI_BYTE,
		     peer, BLKMPIBARRIER, comm, MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }
  }

  /* Send to and receive from parent. */

  if (rank > 0) {
    peer = rank & ~(1 << hibit);
    err = MPI_Send((void *) 0, 0, MPI_BYTE, peer, BLKMPIBARRIER, comm);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }

    err = MPI_Recv((void *) 0, 0, MPI_BYTE, peer,
		   BLKMPIBARRIER, comm, MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* Send to children. */

  for (i = hibit + 1, mask = 1 << i; i <= dim; ++i, mask <<= 1) {
    peer = rank | mask;
    if (peer < size) {
      err = MPI_Send((void *) 0, 0, MPI_BYTE, peer, BLKMPIBARRIER, comm);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }
    }
  }

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}


/*
 *	barrier_inter
 *
 *	Function:	- barrier using O(N) for intercomm
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_barrier_inter(MPI_Comm intercomm)
{
  int local_rank;
  int err;
  MPI_Comm local_comm;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;

  
  /* Rank in my local group */
  MPI_Comm_rank(intercomm, &local_rank);

  /* According to MPI standards, processes in local group can exit when
     all the processes in remote group enter in barrier. Let each process
     do a barrier in the local communicator and then rank 0 in both the 
     groups exchange the status and notify their local members. As of now
     we will use very elementary scheme that needs two barriers in local comm.
     The scheme can be improved further. 
  */

  lam_mkcoll(intercomm);
  
  /* Local barrier */
  if (MPI_Barrier(local_comm) != MPI_SUCCESS) 
    return LAMERROR;

  /* Exchange between leaders in local and remote group */
  if (local_rank == 0) {
    err = MPI_Sendrecv((void*) 0, 0, MPI_BYTE, 0, BLKMPIBARRIER,
		       (void*) 0, 0, MPI_BYTE, 0, BLKMPIBARRIER,
		       intercomm, MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return err;
    }
  }

  /* Again local barrier */
  if (MPI_Barrier(local_comm) != MPI_SUCCESS) 
    return LAMERROR;

  lam_mkpt(intercomm);
  return (MPI_SUCCESS);
}

