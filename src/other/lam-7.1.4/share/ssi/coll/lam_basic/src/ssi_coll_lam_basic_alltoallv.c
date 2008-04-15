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
 * $Id: ssi_coll_lam_basic_alltoallv.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
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

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpi.h>
#include <mpisys.h>
#include <blktype.h>


/*
 *	alltoallv
 *
 *	Function:	- MPI_Alltoallv for non-lamd RPIs
 *	Accepts:	- same as MPI_Alltoallv()
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_lam_basic_alltoallv(void *sbuf, int *scounts, int *sdisps,
				 MPI_Datatype sdtype, void *rbuf,
				 int *rcounts, int *rdisps,
				 MPI_Datatype rdtype, MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int nreqs;
  int err;
  char *psnd;
  char *prcv;
  MPI_Aint sndextent;
  MPI_Aint rcvextent;
  MPI_Request *req;
  MPI_Request *preq;

  /* Initialize. */

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  MPI_Type_extent(sdtype, &sndextent);
  MPI_Type_extent(rdtype, &rcvextent);

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

  psnd = ((char *) sbuf) + (sdisps[rank] * sndextent);
  prcv = ((char *) rbuf) + (rdisps[rank] * rcvextent);
  err = lam_dtsndrcv(psnd, scounts[rank], sdtype,
		     prcv, rcounts[rank], rdtype, BLKMPIALLTOALLV, comm);
  if (err != MPI_SUCCESS) {
    if (req)
      free((char *) req);
    lam_mkpt(comm);
    return err;
  }

  /* If only one process, generate run time trace and we're done. */

  if (size == 1) {
    lam_mkpt(comm);
    LAM_TRACE(lam_tr_cffend(BLKMPIALLTOALLV, -1, comm, sdtype, 0));
    lam_resetfunc(BLKMPIALLTOALLV);
    return (MPI_SUCCESS);
  }

  /* Initiate all send/recv to/from others. */

  preq = req;
  for (i = 0; i < size; ++i) {
    if (i == rank)
      continue;

    prcv = ((char *) rbuf) + (rdisps[i] * rcvextent);
    err = MPI_Recv_init(prcv, rcounts[i], rdtype,
			i, BLKMPIALLTOALLV, comm, preq++);
    if (err != MPI_SUCCESS) {
      free((char *) req);
      lam_mkpt(comm);
      return err;
    }
  }

  for (i = 0; i < size; ++i) {
    if (i == rank)
      continue;

    psnd = ((char *) sbuf) + (sdisps[i] * sndextent);
    err = MPI_Send_init(psnd, scounts[i], sdtype,
			i, BLKMPIALLTOALLV, comm, preq++);
    if (err != MPI_SUCCESS) {
      free((char *) req);
      lam_mkpt(comm);
      return err;
    }
  }

  /* Start all requests. */

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

  /* Free the requests. */

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
 *	lamd_alltoallv
 *
 *	Function:	- MPI_Alltoallv for the LAMD RPI
 *	Accepts:	- same as MPI_Alltoallv
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_lam_basic_alltoallv_lamd(void *sbuf, int *scounts, 
				      int *sdisps, MPI_Datatype sdtype,
				      void *rbuf, int *rcounts, 
				      int *rdisps, MPI_Datatype rdtype,
				      MPI_Comm comm)
{
  int i;
  int size;
  int rank;
  int err;
  char *psnd;
  char *prcv;
  MPI_Aint sndextent;
  MPI_Aint rcvextent;

  /* Initialize. */

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  MPI_Type_extent(sdtype, &sndextent);
  MPI_Type_extent(rdtype, &rcvextent);

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* simple optimization */

  psnd = ((char *) sbuf) + (sdisps[rank] * sndextent);
  prcv = ((char *) rbuf) + (rdisps[rank] * rcvextent);
  err = lam_dtsndrcv(psnd, scounts[rank], sdtype,
		     prcv, rcounts[rank], rdtype, BLKMPIALLTOALLV, comm);
  if (err != MPI_SUCCESS) {
    lam_mkpt(comm);
    return err;
  }

  /* Do sendrecv's with others if any. */

  for (i = 0; i < size; ++i) {
    if (i == rank)
      continue;

    psnd = ((char *) sbuf) + (sdisps[i] * sndextent);
    prcv = ((char *) rbuf) + (rdisps[i] * rcvextent);
    err = MPI_Sendrecv(psnd, scounts[i], sdtype, i,
		       BLKMPIALLTOALLV, prcv, rcounts[i], rdtype, i,
		       BLKMPIALLTOALLV, comm, MPI_STATUS_IGNORE);
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
 *	alltoallv for intercomm
 *
 *	Function:	- intercomm alltoallv
 *	Accepts:	- same as MPI_Alltoallv()
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_lam_basic_alltoallv_inter(void *sbuf, int *scounts, 
				       int *sdisps, MPI_Datatype sdtype,
				       void *rbuf, int *rcounts, 
				       int *rdisps, MPI_Datatype rdtype,
				       MPI_Comm intercomm)
{
  int i, local_rank, local_size, remote_size, max_size;
  int src, dst, send_count, recv_count;
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
      recv_count = 0;
    } else {
      recv_buffer = (char*) rbuf + rdisps[src] * rextent;
      recv_count = rcounts[src];
    }
    if (dst >= remote_size) {
      dst = MPI_PROC_NULL;
      send_buffer = NULL;
      send_count = 0;
    } else {
      send_buffer = (char*) sbuf + sdisps[dst] * sextent;
      send_count = scounts[dst];
    }
    if (MPI_Sendrecv(send_buffer, send_count, sdtype, dst, BLKMPIALLTOALL, 
		     recv_buffer, recv_count, rdtype, src, BLKMPIALLTOALL, 
		     intercomm, MPI_STATUS_IGNORE)) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  }
  lam_mkpt(intercomm);
  return (MPI_SUCCESS);
}
