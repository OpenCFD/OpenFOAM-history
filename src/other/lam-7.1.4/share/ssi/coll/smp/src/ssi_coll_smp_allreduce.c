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
 * $Id: ssi_coll_smp_allreduce.c,v 1.4 2003/05/28 00:16:32 jsquyres Exp $
 *
 *	Function:	- SMP-aware collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <mpi.h>
#include <lam-ssi-coll-smp-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>
#include <mpi.h>
#include <mpisys.h>
#include <blktype.h>


/*
 * Local functions
 */
static int assoc_allreduce(void *sbuf, void *rbuf, int count,
			   MPI_Datatype dtype, MPI_Op op, MPI_Comm comm);
static int nonassoc_allreduce(void *sbuf, void *rbuf, int count,
                              MPI_Datatype dtype, MPI_Op op, MPI_Comm comm);

/*
 *	reduce
 *
 *	Function:	- multi-algorithm reduce
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_smp_allreduce(void *sbuf, void *rbuf, int count,
			   MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  /* If this communicator was marked to be associative, use the wide
     area optimal associative algorithm. */

  if (lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_ASSOCIATIVE) == 1 &&
      op->op_commute == 1)
    return assoc_allreduce(sbuf, rbuf, count, dtype, op, comm);

  /* Otherwise, look at how many bytes will be transferred by each
     process to determine whether to use the lam_basic algorithm, or
     the short MagPIe algorithm */

  if ((count * dtype->dt_size) < lcd->lcd_reduce_crossover)
    return nonassoc_allreduce(sbuf, rbuf, count, dtype, op, comm);
  else 
    return lcd->lb_functions.lsca_allreduce(sbuf, rbuf, count, dtype, 
				       op, comm);
}


static int 
assoc_allreduce(void *sbuf, void *rbuf, int count,
		MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  int err, rank;
  char *local_buffer = NULL, *local_origin;
  char *send_ptr, *recv_ptr;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  MPI_Comm_rank(comm, &rank);

  /* Set root to setup coordinator comms, etc. */

  lam_ssi_coll_smp_set_root(comm, 0);

  /* Do local reductions first */
  /* If I'm the local root, allocate a temp buffer to receive into */

  if (lcd->lcd_local_size == 1) {
    recv_ptr = sbuf;
  } else {
    if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
      err = lam_dtbuffer(dtype, count, &local_buffer, &local_origin);
      if (err != MPI_SUCCESS)
	return err;
      recv_ptr = local_origin;
    } else
      recv_ptr = NULL;
    err = MPI_Reduce(sbuf, recv_ptr, count, dtype, op, 
		     lcd->lcd_local_roots[0],
		     lcd->lcd_local_comm);
    if (err != MPI_SUCCESS)
      return err;
  }

  /* MagPIe does an alltoall between the coordinators and then
     finishes the computation locally.  However, at least in the
     linear case, this is the same (in terms of latency and bytes
     transferred) as a reduce to one of the coordinators followed by a
     bcast from that coordinator. */

  if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
    send_ptr = recv_ptr;
    if (rank == 0)
      recv_ptr = rbuf;
    else
      recv_ptr = NULL;
    err = MPI_Reduce(send_ptr, recv_ptr, count, dtype, op, 
		     lcd->lcd_coord_roots[0],
		     lcd->lcd_coord_comms[0]);
    if (err != MPI_SUCCESS)
      return err;

    /* Now do the bcast */

    err = MPI_Bcast(rbuf, count, dtype, lcd->lcd_coord_roots[0], 
		    lcd->lcd_coord_comms[0]);
  }

  /* Finally, do a local bcast to communicate the final result to my
     local peers */

  if (lcd->lcd_local_size > 1)
    MPI_Bcast(rbuf, count, dtype, lcd->lcd_local_roots[0],
	      lcd->lcd_local_comm);

  /* Free temporary buffer */

  if (local_buffer != NULL)
    free(local_buffer);

  /* All done */

  return MPI_SUCCESS;
}


static int 
nonassoc_allreduce(void *sbuf, void *rbuf, int count,
                   MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  int i, err, rank, size;
  MPI_Aint extent;
  char *local_buffer = NULL, *local_origin;
  char *inbuf;

  /* For short messages, give everyone the data and do the reduction
     locally.  Allocate a buffer to allgather into. */

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  err = lam_dtbuffer(dtype, count * size, &local_buffer, &local_origin);
  if (err != MPI_SUCCESS)
    return err;

  /* Do the allgather */

  err = MPI_Allgather(sbuf, count, dtype, local_origin, count, dtype, comm);
  if (err != MPI_SUCCESS)
    return err;

  /* Now perform the reduction locally */
  /* Initialize the receive buffer. */

  MPI_Type_extent(dtype, &extent);
  err = lam_dtsndrcv(local_origin + (extent * count * (size - 1)), 
                     count, dtype, rbuf, count, dtype, BLKMPIREDUCE, comm);
  if (err != MPI_SUCCESS) {
    free(local_origin);
    return err;
  }

  for (i = size - 2; i >= 0; --i) {

    /* Call reduction function. */

    inbuf = local_origin + (extent * count * i);
    if (op->op_flags & LAM_LANGF77) {
      (op->op_func)(inbuf, rbuf, &count, &dtype->dt_f77handle);
    } else {
      (op->op_func)(inbuf, rbuf, &count, &dtype);
    }
  }

  /* Free temporary buffer */

  if (local_buffer != NULL)
    free(local_buffer);

  /* All done */

  return MPI_SUCCESS;
}
