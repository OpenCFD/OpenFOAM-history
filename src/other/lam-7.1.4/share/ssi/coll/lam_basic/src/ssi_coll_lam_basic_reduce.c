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
 * $Id: ssi_coll_lam_basic_reduce.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
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
 *	reduce_lin
 *
 *	Function:	- reduction using O(N) algorithm
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_reduce_lin(void *sbuf, void *rbuf, int count,
				  MPI_Datatype dtype, MPI_Op op,
				  int root, MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int err;
  char *buffer = 0;
  char *origin = 0;
  char *inbuf;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  lam_mkcoll(comm);

  /* If not root, send data to the root. */

  if (rank != root) {
    err = MPI_Send(sbuf, count, dtype, root, BLKMPIREDUCE, comm);
    lam_mkpt(comm);
    return err;
  }

  /* Root receives and reduces messages.  Allocate buffer to receive
     messages. */

  if (size > 1) {
    err = lam_dtbuffer(dtype, count, &buffer, &origin);
    if (err != MPI_SUCCESS)
      return err;
  }

  /* Initialize the receive buffer. */

  if (rank == (size - 1)) {
    err = lam_dtsndrcv(sbuf, count, dtype, rbuf, count,
		       dtype, BLKMPIREDUCE, comm);
  } else {
    err = MPI_Recv(rbuf, count, dtype, size - 1,
		   BLKMPIREDUCE, comm, MPI_STATUS_IGNORE);
  }
  if (err != MPI_SUCCESS) {
    if (buffer)
      free(buffer);
    lam_mkpt(comm);
    return err;
  }

  /* Loop receiving and calling reduction function (C or Fortran). */

  for (i = size - 2; i >= 0; --i) {
    if (rank == i) {
      inbuf = sbuf;
    } else {
      err = MPI_Recv(origin, count, dtype, i, BLKMPIREDUCE, comm, 
		     MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	if (buffer)
	  free(buffer);
	lam_mkpt(comm);
	return err;
      }

      inbuf = origin;
    }

    /* Call reduction function. */

    if (op->op_flags & LAM_LANGF77) {
      (op->op_func)
	(inbuf, rbuf, &count, &dtype->dt_f77handle);
    } else {
      (op->op_func) (inbuf, rbuf, &count, &dtype);
    }
  }

  if (buffer)
    free(buffer);

  /* All done */

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}


/*
 *	reduce_log
 *
 *	Function:	- reduction using O(log N) algorithm
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_reduce_log(void *sbuf, void *rbuf, int count,
				  MPI_Datatype dtype, MPI_Op op,
				  int root, MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int vrank;
  int err;
  int peer;
  int dim;
  int mask;
  int fl_recv;
  char *buf1;
  char *buf2;
  char *origin1;
  char *origin2;
  void *inmsg;
  void *resmsg;

  /* Allocate the incoming and resulting message buffers. */

  err = lam_dtbuffer(dtype, count, &buf1, &origin1);
  if (err != MPI_SUCCESS)
    return err;

  err = lam_dtbuffer(dtype, count, &buf2, &origin2);
  if (err != MPI_SUCCESS) {
    if (buf1)
      free(buf1);
    return err;
  }

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  vrank = (op->op_commute) ? (rank - root + size) % size : rank;
  lam_mkcoll(comm);
  dim = comm->c_cube_dim;

  /* Loop over cube dimensions. High processes send to low ones in the
     dimension. */

  inmsg = origin1;
  resmsg = origin2;
  fl_recv = 0;
  for (i = 0, mask = 1; i < dim; ++i, mask <<= 1) {

    /* A high-proc sends to low-proc and stops. */

    if (vrank & mask) {
      peer = vrank & ~mask;
      if (op->op_commute)
	peer = (peer + root) % size;

      err = MPI_Send((fl_recv) ? resmsg : sbuf, count,
		     dtype, peer, BLKMPIREDUCE, comm);
      if (err != MPI_SUCCESS) {
	if (buf1)
	  free(buf1);
	if (buf2)
	  free(buf2);
	lam_mkpt(comm);
	return err;
      }

      break;
    }

    /* A low-proc receives, reduces, and moves to a higher
       dimension. */

    else {
      peer = vrank | mask;
      if (peer >= size)
	continue;
      if (op->op_commute)
	peer = (peer + root) % size;

      fl_recv = 1;
      err = MPI_Recv(inmsg, count, dtype, peer,
		     BLKMPIREDUCE, comm, MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	if (buf1)
	  free(buf1);
	if (buf2)
	  free(buf2);
	lam_mkpt(comm);
	return err;
      }

      if (op->op_flags & LAM_LANGF77) {
	(*op->op_func) ((i > 0) ? resmsg : sbuf,
			inmsg, &count, &dtype->dt_f77handle);
      } else {
	(*op->op_func) ((i > 0) ? resmsg : sbuf, inmsg, &count, &dtype);
      }

      if (inmsg == origin1) {
	resmsg = origin1;
	inmsg = origin2;
      } else {
	resmsg = origin2;
	inmsg = origin1;
      }
    }
  }

  /* Get the result to the root if needed. */

  err = MPI_SUCCESS;
  if (vrank == 0) {
    if (root == rank) {
      lam_dtcpy(rbuf, (i > 0) ? resmsg : sbuf, count, dtype);
    } else {
      err = MPI_Send((i > 0) ? resmsg : sbuf, count,
		     dtype, root, BLKMPIREDUCE, comm);
    }
  } else if (rank == root) {
    err = MPI_Recv(rbuf, count, dtype, 0, BLKMPIREDUCE, comm, 
		   MPI_STATUS_IGNORE);
  }

  if (buf1)
    free(buf1);
  if (buf2)
    free(buf2);

  /* All done */

  lam_mkpt(comm);
  return err;
}


/*
 *	reduce
 *
 *	Function:	- intercomm reduction using O(N) algorithm
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_reduce_inter(void *sbuf, void *rbuf, int count,
				    MPI_Datatype dtype, MPI_Op op,
				    int root, MPI_Comm intercomm)
{
  int local_rank, local_size;
  char* local_origin = NULL, *local_buffer = NULL; 
  MPI_Comm local_comm;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;

  MPI_Comm_rank(intercomm, &local_rank);

  lam_mkcoll(intercomm);
  if (root == MPI_PROC_NULL) {
    lam_mkpt(intercomm);
    return MPI_SUCCESS;
  } else if (root == MPI_ROOT) {

    /* recieve data from rank 0 of the remote group */
    if ((MPI_Recv(rbuf, count, dtype, 0, BLKMPIREDUCE, intercomm, 
		  MPI_STATUS_IGNORE)) != MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  } else {
    /* remote group */
    /* rank 0 does a local reduce and sends data to the root. First allocate
       space to hold the result */
    MPI_Comm_size(local_comm, &local_size);
    if (local_rank == 0) {
      if ((lam_dtbuffer(dtype, count * local_size, &local_buffer,
			&local_origin)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
    }
    MPI_Reduce(sbuf, local_origin, count, dtype, op, 0, local_comm);
    
    if (local_rank == 0) {
      if ((MPI_Send(local_origin, count, dtype, root, BLKMPIREDUCE, intercomm))
	  != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      free(local_origin);
    }
  }
  lam_mkpt(intercomm);
  return MPI_SUCCESS;
}
