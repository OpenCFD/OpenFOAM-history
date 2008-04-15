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
 * $Id: ssi_coll_lam_basic_scan.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
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
 *	scan
 *
 *	Function:	- basic scan operation
 *	Accepts:	- same arguments as MPI_Scan()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_scan(void *sbuf, void *rbuf, int count,
			    MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  int size;
  int rank;
  int err;
  char *tmpbuf = 0;
  char *origin;

  /* Initialize. */

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* If I'm rank 0, initialize the recv buffer. */

  if (rank == 0) {
    err = lam_dtsndrcv(sbuf, count, dtype,
		       rbuf, count, dtype, BLKMPISCAN, comm);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* Otherwise receive previous buffer and reduce. */

  else {
    if (!op->op_commute) {

      /* Allocate a temporary buffer. */

      err = lam_dtbuffer(dtype, count, &tmpbuf, &origin);
      if (err != MPI_SUCCESS) {
	lam_mkpt(comm);
	return err;
      }

      /* Copy the send buffer into the receive buffer. */

      err = lam_dtsndrcv(sbuf, count, dtype, rbuf,
			 count, dtype, BLKMPISCAN, comm);
      if (err != MPI_SUCCESS) {
	if (tmpbuf)
	  free(tmpbuf);
	lam_mkpt(comm);
	return err;
      }

      err = MPI_Recv(origin, count, dtype,
		     rank - 1, BLKMPISCAN, comm, MPI_STATUS_IGNORE);
    } else {
      origin = sbuf;

      err = MPI_Recv(rbuf, count, dtype,
		     rank - 1, BLKMPISCAN, comm, MPI_STATUS_IGNORE);
    }

    if (err != MPI_SUCCESS) {
      if (tmpbuf)
	free(tmpbuf);
      lam_mkpt(comm);
      return err;
    }

    if (op->op_flags & LAM_LANGF77) {
      (op->op_func)
	(origin, rbuf, &count, &dtype->dt_f77handle);
    } else {
      (op->op_func) (origin, rbuf, &count, &dtype);
    }

    if (tmpbuf)
      free(tmpbuf);
  }

  /* Send result to next process. */

  if (rank < (size - 1)) {
    err = MPI_Send(rbuf, count, dtype, rank + 1, BLKMPISCAN, comm);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}
