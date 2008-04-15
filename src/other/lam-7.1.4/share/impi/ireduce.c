/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	$Id: ireduce.c,v 1.7 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	- IMPI gather collective
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- recv buffer
 *			- recv count
 *			- recv datatype
 *			- root
 *			- communicator
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */

#include "lam_config.h"

#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <impi.h>
#include <impi-defs.h>
#include <blktype.h>


/*
 * Local functions
 */
static int reduce_commutative(void *sbuf, void *rbuf, int count, 
			      MPI_Datatype dtype, MPI_Op op, int root, 
			      MPI_Comm comm);
static int reduce_noncommutative(void *sbuf, void *rbuf, int count, 
				 MPI_Datatype dtype, MPI_Op op, int root, 
				 MPI_Comm comm);


/*
 * Almost identical to the pseudocode in section 4.16 of the IMPI
 * standard
 *
 * This function should only be called from the main MPI_Reduce
 * function.
 */
int 
IMPI_Reduce(void *sbuf, void *rbuf, int count, MPI_Datatype dtype, 
	    MPI_Op op, int root, MPI_Comm comm)
{
  int ret;

  lam_setfunc_m(BLKIMPIREDUCE);

  if (op->op_commute)
    ret = reduce_commutative(sbuf, rbuf, count, dtype, op, root, comm);
  else
    ret = reduce_noncommutative(sbuf, rbuf, count, dtype, op, root, comm);

  lam_resetfunc_m(BLKIMPIREDUCE);
  return ret;
}


static int 
reduce_commutative(void *sbuf, void *rbuf, int count, MPI_Datatype dtype, 
		   MPI_Op op, int root, MPI_Comm comm)
{
  int myrank, mynum, rootnum, vnum, dim, mask;
  int peer, nmasters, i;
  void *tmpbuf = 0, *redbuf = 0;
  MPI_Aint extent;

  nmasters = IMPI_Num_masters(comm);
  MPI_Comm_rank(comm, &myrank);
  MPI_Type_extent(dtype, &extent);

  /* local phase */
  if (IMPI_Are_local(myrank, root, comm))
    /* perform a local reduction to root's rbuf; */
    /* LAM local */
    MPI_Reduce(sbuf, rbuf, count, dtype, op, 
	       IMPI_Rank_impi2shadow(root, comm), comm->c_shadow);
  else {
    /* perform a local reduction to a temporary buffer redbuf in
       the local master; */
    /* LAM local */
    if (IMPI_Is_master(myrank, comm) == 1)
      redbuf = malloc(extent * count);

    i = IMPI_Local_master_rank(myrank, comm);
    i = IMPI_Rank_impi2shadow(i, comm);
    MPI_Reduce(sbuf, redbuf, count, dtype, op, i, comm->c_shadow);
  }

  /* global phase */
  if ((myrank == root) || (IMPI_Is_master(myrank, comm) 
      && !IMPI_Are_local(myrank, root, comm))) {
  
    /* allocate a temporary buffer tmpbuf large enough for
       count copies of dtype; */
    MPI_Type_extent(dtype, &extent);
    tmpbuf = malloc(extent * count);

    if (nmasters <= IMPI_MAX_LINEAR_REDUCE) {
      /* linear reduction to root */
      if (myrank == root) {
        for (i = 0; i < nmasters; i++) {
          if (i == IMPI_Local_master_num(root, comm)) continue;
          MPI_Recv(tmpbuf, count, dtype, IMPI_Master_rank(i, comm), 
                   IMPI_REDUCE_TAG, comm, MPI_STATUS_IGNORE);
        }

        /* call reduction function op on tmpbuf (invec) 
	   and rbuf (inoutvec); */
	/* LAM local */
	if (op->op_flags & LAM_LANGF77) 
	  (op->op_func)(tmpbuf, rbuf, &count, &dtype->dt_f77handle);
	else
	  (op->op_func)(tmpbuf, rbuf, &count, &dtype);
      } else {
        MPI_Send(redbuf, count, dtype, root, IMPI_REDUCE_TAG, comm); 
      }
    } else {
      /* tree reduction to root */
      mynum = IMPI_Local_master_num(myrank, comm);
      rootnum = IMPI_Local_master_num(root, comm);
      vnum = (mynum - rootnum + nmasters) % nmasters;
      dim = lam_cubedim(nmasters);

      /* loop over cube dimensions */
      for (i = 0, mask = 1; i < dim; ++i, mask <<= 1) {
        /* a high-proc sends to low-proc and stops */
        if (vnum & mask) {
          peer = IMPI_Master_rank(((vnum & ~mask) + rootnum) % nmasters, comm);
          if (IMPI_Are_local(peer, root, comm))
            peer = root;

          MPI_Send(redbuf, count, dtype, peer, IMPI_REDUCE_TAG, comm);
          break;
        }

        /* a low-proc receives, reduces, and moves 
         * to a higher dimension */
        else {
          peer = vnum | mask;
          if (peer >= nmasters) continue;
          peer = IMPI_Master_rank((peer + rootnum) % nmasters, comm);

          MPI_Recv(tmpbuf, count, dtype, peer, 
                   IMPI_REDUCE_TAG, comm, MPI_STATUS_IGNORE);

          if (myrank == root) {	
            /* call reduction function op on tmpbuf (invec) 
	       and rbuf (inoutvec); */
	    /* LAM local */
	    if (op->op_flags & LAM_LANGF77) 
	      (op->op_func)(tmpbuf, rbuf, &count, &dtype->dt_f77handle);
	    else
	      (op->op_func)(tmpbuf, rbuf, &count, &dtype);
          } else {
            /* call reduction function op on tmpbuf (invec) 
	       and redbuf (inoutvec); */
	    /* LAM local */
	    if (op->op_flags & LAM_LANGF77) 
	      (op->op_func)(tmpbuf, redbuf, &count, &dtype->dt_f77handle);
	    else
	      (op->op_func)(tmpbuf, redbuf, &count, &dtype);
          }
        }
      }
    }
  }

  /* free all temporary buffers; */
  /* LAM local */
  if (tmpbuf != 0)
    free(tmpbuf);
  if (redbuf != 0)
    free(redbuf);
  return MPI_SUCCESS;
}

static int 
reduce_noncommutative(void *sbuf, void *rbuf, int count, 
		      MPI_Datatype dtype, MPI_Op op, int root, MPI_Comm comm)
{
  int i, myrank, nprocs;
  void *inbuf = 0, *tmpbuf = 0;
  MPI_Aint extent;

  MPI_Comm_size(comm, &nprocs);
  MPI_Comm_rank(comm, &myrank);

  if (myrank != root)
    return(MPI_Send(sbuf, count, dtype, root, IMPI_REDUCE_TAG, comm));

  if (nprocs > 1) {
    /* create a temporary buffer tmpbuf large enough for count dtypes; */
    /* LAM local */
    MPI_Type_extent(dtype, &extent);
    tmpbuf = malloc(extent * count);
  }

  if (myrank == (nprocs - 1))
    MPI_Sendrecv(sbuf, count, dtype, myrank, IMPI_REDUCE_TAG,
                 rbuf, count, dtype, myrank, IMPI_REDUCE_TAG, comm, 
                 MPI_STATUS_IGNORE);
  else
    MPI_Recv(rbuf, count, dtype, nprocs - 1, IMPI_REDUCE_TAG, comm, 
             MPI_STATUS_IGNORE);

  for (i = nprocs - 2; i >= 0; --i) {
    if (myrank == i) {
       inbuf = sbuf;
    } else {
      MPI_Recv(tmpbuf, count, dtype, i, IMPI_REDUCE_TAG, comm, 
	       MPI_STATUS_IGNORE);
      inbuf = tmpbuf;
    }

    /* call reduction function op on inbuf (invec) and rbuf (inoutvec); */
    /* LAM local */
    if (op->op_flags & LAM_LANGF77) 
      (op->op_func)(inbuf, rbuf, &count, &dtype->dt_f77handle);
    else
      (op->op_func)(inbuf, rbuf, &count, &dtype);
  }

  /* free all temporary buffers; */
  /* LAM local */
  if (tmpbuf != 0)
    free(tmpbuf);
  return MPI_SUCCESS;
}
