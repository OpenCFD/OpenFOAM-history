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
 * $Id: ssi_coll_lam_basic_scatter.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
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
 *	scatter
 *
 *	Function:	- scatter operation
 *	Accepts:	- same arguments as MPI_Scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_scatter(void *sbuf, int scount, MPI_Datatype sdtype,
			       void *rbuf, int rcount, MPI_Datatype rdtype,
			       int root, MPI_Comm comm)
{
  int i;
  int rank;
  int size;
  int err;
  char *ptmp;
  MPI_Aint incr;
  struct _gps *p;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  /* Remember required parameters. */

  p = &(comm->c_group->g_procs[root]->p_gps);
  lam_setparam(BLKMPISCATTER, root | (p->gps_grank << 16),
	       (p->gps_node << 16) | p->gps_idx);

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* If not root, receive data. */

  if (rank != root) {
    err = MPI_Recv(rbuf, rcount, rdtype,
		   root, BLKMPISCATTER, comm, MPI_STATUS_IGNORE);
    lam_mkpt(comm);
    return err;
  }

  /* I am the root, loop sending data. */

  MPI_Type_extent(sdtype, &incr);
  incr *= scount;
  for (i = 0, ptmp = (char *) sbuf; i < size; ++i, ptmp += incr) {

    /* simple optimization */

    if (i == rank) {
      err = lam_dtsndrcv(ptmp, scount, sdtype, rbuf,
			 rcount, rdtype, BLKMPISCATTER, comm);
    } else {
      err = MPI_Send(ptmp, scount, sdtype, i, BLKMPISCATTER, comm);
    }
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
 *	scatter
 *
 *	Function:	- intercommunicator scatter operation
 *	Accepts:	- same arguments as MPI_Scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_scatter_inter(void *sbuf, int scount, 
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
    if ((MPI_Send(sbuf, scount * remote_size, sdtype, 0, 
		  BLKMPISCATTER, intercomm)) 
	!= MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }

  } else {
    /* remote group */
    /* rank 0 scatters the data in the local comm. First allocate a temproary
       buffer*/

    if (local_rank == 0) {
      MPI_Comm_size(intercomm, &local_size);
      if ((lam_dtbuffer(sdtype, scount * local_size, &local_buffer,
			&local_origin)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      if ((MPI_Recv(local_origin, scount * local_size, sdtype, root, 
		    BLKMPISCATTER, intercomm, MPI_STATUS_IGNORE)) 
	  != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      /* local scatter */
      if ((MPI_Scatter(local_origin, scount, sdtype, rbuf, rcount, rdtype,
		       0, local_comm)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
      free(local_origin);
    } else {
    /* local scatter */
      if ((MPI_Scatter(sbuf, scount, sdtype, rbuf, rcount, rdtype,
		       0, local_comm)) != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
    }
  }
  
  lam_mkpt(intercomm);
  return MPI_SUCCESS;
}

