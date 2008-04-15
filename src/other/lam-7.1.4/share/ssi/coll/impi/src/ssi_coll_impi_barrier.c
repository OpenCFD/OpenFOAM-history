/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: ssi_coll_impi_barrier.c,v 1.1 2003/02/04 18:32:25 jsquyres Exp $
 *
 *	Function:	- IMPI collective routines
 */

#include <lam_config.h>
#include <lam-ssi-coll-impi-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-impi.h>
#include <mpi.h>
#include <impi.h>
#include <impi-defs.h>
#include <blktype.h>
#include <mpisys.h>


/*
 *	barrier
 *
 *	Function:       - Almost identical to the pseudocode in section
 *	                  4.10 of the IMPI standard
 *	Accepts:	- same as MPI_Barrier()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_impi_barrier(MPI_Comm comm)
{
  int i, nmasters, myrank, mynum, dim, hibit, mask, peer, err;

  lam_setfunc_m(BLKIMPIBARRIER);
  myrank = comm->c_group->g_myrank;

  /* local phase - MPI_Comm on our shadow comm for the local lamnodes */

  err = MPI_Barrier(comm->c_shadow);
  if (err != MPI_SUCCESS)
    return(lam_errfunc(comm, BLKMPIBARRIER, err));

  /* Change to use the IMPI collective context ID */

  lam_mkimpicoll(comm);

  /* global phase */

  if (IMPI_Is_master(myrank, comm)) {
    nmasters = IMPI_Num_masters(comm);
    if (nmasters <= IMPI_MAX_LINEAR_BARRIER) {
      /* linear barrier among the masters */
      if (myrank == 0) {
	for (i = 1; i < nmasters; i++) {
	  err = MPI_Recv(MPI_BOTTOM, 0, MPI_BYTE, IMPI_Master_rank(i, comm), 
		       IMPI_BARRIER_TAG, comm, MPI_STATUS_IGNORE);
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIBARRIER, err));
	}
	for (i = 1; i < nmasters; i++) {
	  err = MPI_Send(MPI_BOTTOM, 0, MPI_BYTE, IMPI_Master_rank(i, comm),
			 IMPI_BARRIER_TAG, comm);
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIBARRIER, err));
	}
      } else {
	err = MPI_Send(MPI_BOTTOM, 0, MPI_BYTE, 0, IMPI_BARRIER_TAG, comm);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIBARRIER, err));
	err = MPI_Recv(MPI_BOTTOM, 0, MPI_BYTE, 0, IMPI_BARRIER_TAG, comm, 
		       MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIBARRIER, err));
      }
    } else {
      /* tree barrier among the masters */
      mynum = IMPI_Master_num(myrank, comm);
      dim   = lam_cubedim(nmasters);
      hibit = lam_hibit(mynum, dim);
      --dim;
      
      /* receive from children */
      for (i = dim, mask = 1 << i; i > hibit; --i, mask >>= 1) {
	peer = mynum | mask;
	if (peer < nmasters) {
	  err = MPI_Recv(MPI_BOTTOM, 0, MPI_BYTE, IMPI_Master_rank(peer, comm),
			 IMPI_BARRIER_TAG, comm, MPI_STATUS_IGNORE);
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIBARRIER, err));
	}
      }
      
      /* send to and receive from parent */
      if (mynum > 0) {
	peer = IMPI_Master_rank(mynum & ~(1 << hibit), comm);
	err = MPI_Send(MPI_BOTTOM, 0, MPI_BYTE, peer, IMPI_BARRIER_TAG, comm);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIBARRIER, err));
	err = MPI_Recv(MPI_BOTTOM, 0, MPI_BYTE, peer, IMPI_BARRIER_TAG, comm,
		       MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIBARRIER, err));
      }
      
      /* send to children */
      for (i = hibit + 1, mask = 1 << i; i <= dim; i++, mask <<= 1) {
	peer = myrank | mask;
	if (peer < nmasters) {
	  err = MPI_Send(MPI_BOTTOM, 0, MPI_BYTE, IMPI_Master_rank(peer, comm),
		   IMPI_BARRIER_TAG, comm);
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(comm, BLKMPIBARRIER, err));
	}
      }
    }
  }
  
  /* Change back to the pt2pt context ID */

  lam_mkpt(comm);

  /* local phase on the shadow comm */

  err = MPI_Barrier(comm->c_shadow);  
  if (err != MPI_SUCCESS)
    return(lam_errfunc(comm, BLKMPIBARRIER, err));

  lam_resetfunc_m(BLKIMPIBARRIER);
  return (MPI_SUCCESS);
}
