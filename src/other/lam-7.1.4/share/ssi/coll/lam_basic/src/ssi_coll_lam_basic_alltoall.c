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
 * $Id: ssi_coll_lam_basic_alltoall.c,v 1.4 2003/05/28 00:16:28 jsquyres Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <stdlib.h>
#include <errno.h>

#include <blktype.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpi.h>
#include <mpisys.h>
#include <typical.h>

/*
 *	alltoall
 *
 *	Function:	- MPI_Alltoall for non-lamd RPI's
 *	Accepts:	- same as MPI_Alltoall()
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_lam_basic_alltoall(void *sbuf, int scount,
				MPI_Datatype sdtype, void *rbuf,
				int rcount, MPI_Datatype rdtype,
				MPI_Comm comm)
{
  int i;
  int rank;
  int size;
  int nreqs;
  int err;
  char *psnd;
  char *prcv;
  MPI_Aint sndinc;
  MPI_Aint rcvinc;
  MPI_Request *req;
  MPI_Request *preq;
  MPI_Request *qreq;

  /* Initialize. */

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  MPI_Type_extent(sdtype, &sndinc);
  MPI_Type_extent(rdtype, &rcvinc);
  sndinc *= scount;
  rcvinc *= rcount;

  /* Allocate arrays of requests. */

  nreqs = 2 * (size - 1);
  if (nreqs > 0) {
    req = (MPI_Request *) malloc(nreqs * sizeof(MPI_Request));
    if (req == 0) {
      free((char *) req);
      return ENOMEM;
    }
  } else {
    req = 0;
  }

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* simple optimization */

  psnd = ((char *) sbuf) + (rank * sndinc);
  prcv = ((char *) rbuf) + (rank * rcvinc);
  err = lam_dtsndrcv(psnd, scount, sdtype,
		     prcv, rcount, rdtype, BLKMPIALLTOALL, comm);
  if (err != MPI_SUCCESS) {
    if (req)
      free((char *) req);
    lam_mkpt(comm);
    return err;
  }

  /* If only one process, generate run time trace and we're done. */

  if (size == 1) {
    lam_mkpt(comm);
    return (MPI_SUCCESS);
  }

  /* Initiate all send/recv to/from others. */

  preq = req;
  qreq = req + size - 1;
  prcv = (char*) rbuf;
  psnd = (char*) sbuf;
  for (i = (rank + 1) % size; i != rank; 
       i = (i + 1) % size, ++preq, ++qreq) {
    err = MPI_Recv_init(prcv + (i * rcvinc), rcount, rdtype, i,
			BLKMPIALLTOALL, comm, preq);
    if (err != MPI_SUCCESS) {
      free((char *) req);
      lam_mkpt(comm);
      return err;
    }

    err = MPI_Send_init(psnd + (i * sndinc), scount, sdtype, i,
			BLKMPIALLTOALL, comm, qreq);
    if (err != MPI_SUCCESS) {
      free((char *) req);
      lam_mkpt(comm);
      return err;
    }
  }

  /* Start all the requests. */

  err = MPI_Startall(nreqs, req);
  if (err != MPI_SUCCESS) {
    free((char *) req);
    lam_mkpt(comm);
    return err;
  }

  /* Wait for them all. */

  err = MPI_Waitall(nreqs, req, MPI_STATUSES_IGNORE);
  lam_mkpt(comm);
  if (err != MPI_SUCCESS) {
    free((char *) req);
    return err;
  }

  for (i = 0, preq = req; i < nreqs; ++i, ++preq) {
    err = MPI_Request_free(preq);
    if (err != MPI_SUCCESS) {
      free((char *) req);
      return err;
    }
  }

  /* All done */

  free((char *) req);
  return (MPI_SUCCESS);
}

/*
 *	alltoall_lamd
 *
 *	Function:	- MPI_Alltoall for the LAMD RPI
 *	Accepts:	- same as MPI_Alltoall()
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_lam_basic_alltoall_lamd(void *sbuf, int scount,
				     MPI_Datatype sdtype, void *rbuf,
				     int rcount, MPI_Datatype rdtype,
				     MPI_Comm comm)
{
  int i;
  int rank;
  int size;
  int err;
  char *psnd;
  char *prcv;
  MPI_Aint sndinc;
  MPI_Aint rcvinc;

  /* Initialize. */

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  MPI_Type_extent(sdtype, &sndinc);
  MPI_Type_extent(rdtype, &rcvinc);
  sndinc *= scount;
  rcvinc *= rcount;

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* simple optimization */

  psnd = ((char *) sbuf) + (rank * sndinc);
  prcv = ((char *) rbuf) + (rank * rcvinc);
  err = lam_dtsndrcv(psnd, scount, sdtype, prcv, rcount, rdtype,
		     BLKMPIALLTOALL, comm);
  if (err != MPI_SUCCESS) {
    lam_mkpt(comm);
    return err;
  }

  /* Do sendrecv's with others if any. */
  /* This could be optimized to guarantee that all procs have a peer
     in every iteration */

  psnd = (char *) sbuf;
  prcv = (char *) rbuf;
  for (i = 0; i < size; ++i, prcv += rcvinc, psnd += sndinc) {
    if (i == rank)
      continue;

    err = MPI_Sendrecv(psnd, scount, sdtype, i, BLKMPIALLTOALL, prcv,
		       rcount, rdtype, i, BLKMPIALLTOALL, comm,
		       MPI_STATUS_IGNORE);
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* All done */

  lam_mkpt(comm);
  return (MPI_SUCCESS);
}

/*
 *	alltoall for intercomm
 *
 *	Function:	- intercomm alltoall
 *	Accepts:	- same as MPI_Alltoall()
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_lam_basic_alltoall_inter(void *sbuf, int scount,
				      MPI_Datatype sdtype, void *rbuf,
				      int rcount, MPI_Datatype rdtype,
				      MPI_Comm intercomm)
{
  int i, local_rank, local_size, remote_size, max_size;
  int src, dst;
  char *send_buffer, *recv_buffer;
  MPI_Aint sextent;
  MPI_Aint rextent;

  /* Initialize. */
  MPI_Comm_size(intercomm, &local_size);
  MPI_Comm_remote_size(intercomm, &remote_size);
  MPI_Comm_rank(intercomm, &local_rank);
  MPI_Type_extent(sdtype, &sextent);
  MPI_Type_extent(rdtype, &rextent);

  lam_mkcoll(intercomm);
  max_size = LAM_max(local_size, remote_size);
  
  for (i = 0; i < max_size; i++) {
    src = (local_rank - i + max_size) % max_size;
    dst = (local_rank + i) % max_size;
    if (src >= remote_size) {
      src = MPI_PROC_NULL;
      recv_buffer = NULL;
    } else {
      recv_buffer = (char*) rbuf + src * rcount * rextent;
    }
    if (dst >= remote_size) {
      dst = MPI_PROC_NULL;
      send_buffer = NULL;
    } else {
      send_buffer = (char*) sbuf + dst * scount * sextent;
    }
    if (MPI_Sendrecv(send_buffer, scount, sdtype, dst, BLKMPIALLTOALL, 
		     recv_buffer, rcount, rdtype, src, BLKMPIALLTOALL, 
		     intercomm, MPI_STATUS_IGNORE)) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  }
  lam_mkpt(intercomm);
  return (MPI_SUCCESS);
}
