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
 * $Id: ssi_coll_lam_basic_scatterv.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
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
 *	scatterv
 *
 *	Function:	- scatterv operation
 *	Accepts:	- same arguments as MPI_Scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_scatterv(void *sbuf, int *scounts,
				int *disps, MPI_Datatype sdtype,
				void *rbuf, int rcount,
				MPI_Datatype rdtype, int root,
				MPI_Comm comm)
{
  int i;
  int rank;
  int size;
  int err;
  char *ptmp;
  MPI_Aint extent;
  struct _gps *p;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  /* Remember required parameters. */

  p = &(comm->c_group->g_procs[root]->p_gps);
  lam_setparam(BLKMPISCATTERV, root | (p->gps_grank << 16),
	       (p->gps_node << 16) | p->gps_idx);

  /* Switch to collective communicator. */

  lam_mkcoll(comm);

  /* If not root, receive data. */

  if (rank != root) {
    err = MPI_Recv(rbuf, rcount, rdtype,
		   root, BLKMPISCATTERV, comm, MPI_STATUS_IGNORE);
    lam_mkpt(comm);
    return err;
  }

  /* I am the root, loop sending data. */

  MPI_Type_extent(sdtype, &extent);
  for (i = 0; i < size; ++i) {
    ptmp = ((char *) sbuf) + (extent * disps[i]);

    /* simple optimization */

    if (i == rank) {
      err = lam_dtsndrcv(ptmp, scounts[i], sdtype, rbuf,
			 rcount, rdtype, BLKMPISCATTERV, comm);
    } else {
      err = MPI_Send(ptmp, scounts[i], sdtype, i, BLKMPISCATTERV, comm);
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
 *	scatterv
 *
 *	Function:	- intercomm scatterv operation
 *	Accepts:	- same arguments as MPI_Scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_scatterv_inter(void *sbuf, int *scounts,
				      int *disps, MPI_Datatype sdtype,
				      void *rbuf, int rcount,
				      MPI_Datatype rdtype, int root,
				      MPI_Comm intercomm)
{
  int local_rank, remote_size;
  int i;
  MPI_Aint sextent;
  
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
    MPI_Type_extent(sdtype, &sextent);

    /* One by one send the data to remote group */
    for (i = 0; i < remote_size ; ++i) {
      if ((MPI_Send((char*)sbuf + (sextent * disps[i]), scounts[i], sdtype, 
		    i, BLKMPISCATTERV, intercomm)) 
	  != MPI_SUCCESS) {
	lam_mkpt(intercomm);
	return LAMERROR;
      }
    }
  } else {
    /* remote group */
    
    if ((MPI_Recv(rbuf, rcount, rdtype, root, BLKMPISCATTERV, intercomm, 
		  MPI_STATUS_IGNORE)) 
	!= MPI_SUCCESS) {
      lam_mkpt(intercomm);
      return LAMERROR;
    }
  }
  
  lam_mkpt(intercomm);
  return MPI_SUCCESS;
}
