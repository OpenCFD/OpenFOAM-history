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
 * $Id: ssi_coll_lam_basic_gather.c,v 1.3 2003/05/28 00:16:28 jsquyres Exp $
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
#include <rpisys.h>


/*
 *	gather
 *
 *	Function:	- basic gather operation
 *	Accepts:	- same arguments as MPI_Gather()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_gather(void *sbuf, int scount, MPI_Datatype sdtype, 
			      void *rbuf, int rcount, MPI_Datatype rdtype, 
			      int root, MPI_Comm comm)
{
  int i;
  int err;
  int rank;
  int size;
  char *ptmp;
  MPI_Aint incr;
  MPI_Aint extent;
  struct _gps *p;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  /* Remember required parameters. */

  p = &(comm->c_group->g_procs[root]->p_gps);
  lam_setparam(BLKMPIGATHER, root | (p->gps_grank << 16),
	       (p->gps_node << 16) | p->gps_idx);

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* Everyone but root sends data and returns. */

  if (rank != root) {
    err = MPI_Send(sbuf, scount, sdtype, root, BLKMPIGATHER, comm);
    lam_mkpt(comm);
    return err;
  }

  /* I am the root, loop receiving the data. */

  MPI_Type_extent(rdtype, &extent);
  incr = extent * rcount;
  for (i = 0, ptmp = (char *) rbuf; i < size; ++i, ptmp += incr) {

    /* simple optimization */

    if (i == rank) {
      err = lam_dtsndrcv(sbuf, scount, sdtype, ptmp,
			 rcount, rdtype, BLKMPIGATHER, comm);
    } else {
      err = MPI_Recv(ptmp, rcount, rdtype, i,
		     BLKMPIGATHER, comm, MPI_STATUS_IGNORE);
    }
    if (err != MPI_SUCCESS) {
      lam_mkpt(comm);
      return err;
    }
  }

  /* All done */
  
  lam_mkpt(comm);
  return MPI_SUCCESS;
}


/*
 *	gather
 *
 *	Function:	- inter gather operation
 *	Accepts:	- same arguments as MPI_Gather()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_gather_inter(void *sbuf, int scount, 
				    MPI_Datatype sdtype, void *rbuf, 
				    int rcount, MPI_Datatype rdtype, 
				    int root, MPI_Comm intercomm)
{
  int local_rank, local_size, remote_size;
  char *local_buffer = NULL, *local_origin = NULL;
  MPI_Comm local_comm;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;
  MPI_Comm_rank(intercomm, &local_rank);
  lam_mkcoll(intercomm);

  /* Root sends to rank 0 in the remote group and it then does a local bcast */

  if (root == MPI_PROC_NULL) {
    /* local non-root process, simply return */
    lam_mkpt(intercomm);
    return MPI_SUCCESS;
  
  } else if (root == MPI_ROOT) {
    /* Get the data from the rank 0 from the remote group. */
    MPI_Comm_remote_size(intercomm, &remote_size);
    if ((MPI_Recv(rbuf, rcount * remote_size, rdtype, 0, BLKMPIGATHER, 
		  intercomm, MPI_STATUS_IGNORE)) != MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  } else {
    /* remote group */
    /* rank 0 gathers the data in the local comm. First allocate a temproary
       buffer*/

    if (local_rank == 0) {
      MPI_Comm_size(local_comm, &local_size);
      if ((lam_dtbuffer(rdtype, rcount * local_size, &local_buffer,
			&local_origin)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      if ((MPI_Gather(sbuf, scount, sdtype, local_origin, rcount, rdtype, 0,
		      local_comm)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      if ((MPI_Send(local_origin, rcount * local_size, rdtype, root, 
		    BLKMPIGATHER, intercomm)) 
	  != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      free(local_origin);
    } else {
    /* local gather */
      if ((MPI_Gather(sbuf, scount, sdtype, local_origin, rcount, rdtype, 0,
		      local_comm)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
    }
  }
  
  lam_mkpt(intercomm);
  return MPI_SUCCESS;
}
