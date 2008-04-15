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
 * $Id: ssi_coll_smp_reduce.c,v 1.3 2003/05/28 00:16:32 jsquyres Exp $
 *
 *	Function:	- SMP-aware collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-smp-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>
#include <mpi.h>
#include <mpisys.h>


/*
 * Local functions
 */
static int assoc_reduce(void *sbuf, void *rbuf, int count,
			MPI_Datatype dtype, MPI_Op op,
			int root, MPI_Comm comm);

/*
 *	reduce
 *
 *	Function:	- multi-algorithm reduce
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_smp_reduce(void *sbuf, void *rbuf, int count,
			MPI_Datatype dtype, MPI_Op op,
			int root, MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  /* If this communicator was marked to be associative, use the wide
     area optimal associative algorithm. */

  if (lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_ASSOCIATIVE) == 1 &&
      op->op_commute)
    return assoc_reduce(sbuf, rbuf, count, dtype, op, root, comm);

  /* Otherwise, look at how many bytes will be transferred by each
     process to determine whether to use the linear or logrithmic
     algorithms.  Both of these algorithms are implemented in the
     lam_basic module. */

  return lcd->lb_functions.lsca_reduce(sbuf, rbuf, count, dtype, 
                                       op, root, comm);
}


static int 
assoc_reduce(void *sbuf, void *rbuf, int count,
	     MPI_Datatype dtype, MPI_Op op,
	     int root, MPI_Comm comm)
{
  int err, rank;
  char *local_buffer = NULL, *local_origin;
  char *send_ptr, *recv_ptr;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  MPI_Comm_rank(comm, &rank);

  /* Set root to setup coordinator comms, etc. */

  lam_ssi_coll_smp_set_root(comm, root);

  /* Do local reductions first */
  /* If I'm the local root, allocate a temp buffer to receive into */

  if (lcd->lcd_local_size == 1) {
    recv_ptr = sbuf;
  } else {
    if (lcd->lcd_coord_comms[root] != MPI_COMM_NULL) {
      err = lam_dtbuffer(dtype, count, &local_buffer, &local_origin);
      if (err != MPI_SUCCESS)
	return err;
      recv_ptr = local_origin;
    } else
      recv_ptr = NULL;
    err = MPI_Reduce(sbuf, recv_ptr, count, dtype, op, 
		     lcd->lcd_local_roots[root],
		     lcd->lcd_local_comm);
    if (err != MPI_SUCCESS)
      return err;
  }

  /* Do a coordinator reduce to finish */

  if (lcd->lcd_coord_comms[root] != MPI_COMM_NULL) {
    send_ptr = recv_ptr;
    if (rank == root)
      recv_ptr = rbuf;
    else
      recv_ptr = NULL;
    err = MPI_Reduce(send_ptr, recv_ptr, count, dtype, op, 
		     lcd->lcd_coord_roots[root],
		     lcd->lcd_coord_comms[root]);
    if (err != MPI_SUCCESS)
      return err;
  }

  /* Free temporary buffer */

  if (local_buffer != NULL)
    free(local_buffer);

  /* All done */

  return MPI_SUCCESS;
}

