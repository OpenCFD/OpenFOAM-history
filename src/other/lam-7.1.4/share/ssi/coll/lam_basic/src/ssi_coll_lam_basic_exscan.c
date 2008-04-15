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
 * $Id: ssi_coll_lam_basic_exscan.c,v 1.2 2003/10/23 13:25:15 jsquyres Exp $
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
 *	exscan
 *
 *	Function:	- basic exscan operation
 *	Accepts:	- same arguments as MPI_Exccan()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_exscan(void *sbuf, void *rbuf, int count,
			      MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  int size;
  int rank;
  int err;
  char *origin, *tmpbuf = 0;
  char *gathered_buffer = 0, *gathered_origin;

  /* Initialize. */

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* Otherwise receive previous buffer and reduce. Store the recieved buffer
     in different array and then send the reduced array to the next process
  */
  
  err = lam_dtbuffer(dtype, count, &gathered_buffer, &gathered_origin);
  
  if (err != MPI_SUCCESS) {
    lam_mkpt(comm);
    return err;
  }

  if (rank != 0) {
    if (!op->op_commute) {
      
      err = lam_dtbuffer(dtype, count, &tmpbuf, &origin);
      if (err != MPI_SUCCESS) {
	if (gathered_buffer)
	  free(gathered_buffer);
	lam_mkpt(comm);
	return err;
      }
      
      /* Copy the send buffer into the receive buffer. */
      
      err = lam_dtsndrcv(sbuf, count, dtype, rbuf,
			 count, dtype, BLKMPIEXSCAN, comm);
      if (err != MPI_SUCCESS) {
	if (gathered_buffer)
	  free(gathered_buffer);
	if (tmpbuf)
	  free(tmpbuf);
	lam_mkpt(comm);
	return err;
      }
      
      err = MPI_Recv(origin, count, dtype,
		     rank - 1, BLKMPIEXSCAN, comm, MPI_STATUS_IGNORE);
      err = lam_dtsndrcv(origin, count, dtype, gathered_origin,
			 count, dtype, BLKMPIEXSCAN, comm);
    } else {
      origin = sbuf;
      
      err = MPI_Recv(rbuf, count, dtype,
		     rank - 1, BLKMPIEXSCAN, comm, MPI_STATUS_IGNORE);
      
      if (err != MPI_SUCCESS) {
	if (gathered_buffer)
	  free(gathered_buffer);
	if (tmpbuf)
	  free(tmpbuf);
	lam_mkpt(comm);
	return err;
      }
      
      err = lam_dtsndrcv(rbuf, count, dtype, gathered_origin,
			 count, dtype, BLKMPIEXSCAN, comm);
    }
    
    if (err != MPI_SUCCESS) {
      if (gathered_buffer)
	free(gathered_buffer);
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
  
  }  

  /* Send the result to next process. */
  
  if (rank < (size - 1)) {
    if (rank == 0)
      err = MPI_Send(sbuf, count, dtype, rank + 1, BLKMPIEXSCAN, comm);
    else 
      err = MPI_Send(rbuf, count, dtype, rank + 1, BLKMPIEXSCAN, comm);
    if (err != MPI_SUCCESS) {
      if (gathered_buffer)
	free(gathered_buffer);
      if (tmpbuf)
	free(tmpbuf);
      lam_mkpt(comm);
      return err;
    }
  }
  
  if (rank != 0) {
    err = lam_dtsndrcv(gathered_origin, count, dtype, rbuf,
		       count, dtype, BLKMPIEXSCAN, comm);
    
    if (err != MPI_SUCCESS) {
      if (gathered_buffer)
	free(gathered_buffer);
      if (tmpbuf)
	free(tmpbuf);
      lam_mkpt(comm);
    return err;
    }
  }
  
  if (gathered_buffer)
    free(gathered_buffer);
  if (tmpbuf)
    free(tmpbuf);

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}
