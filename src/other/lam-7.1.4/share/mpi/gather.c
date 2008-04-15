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
 * $Id: gather.c,v 6.16 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- gather buffers at root in process rank order
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- recv buffer
 *			- recv count
 *			- recv datatype
 *			- root
 *			- communicator
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */

#include <lam_config.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lam-ssi-coll.h>


/*@

MPI_Gather - Gathers together values from a group of processes
 
Input Parameters:
+ sbuf - starting address of send buffer (choice) 
. scount - number of elements in send buffer (integer) 
. sdtype - data type of send buffer elements (handle) 
. rcount - number of elements for any single receive (integer, 
significant only at root) 
. rdtype - data type of recv buffer elements 
(significant only at root) (handle) 
. root - rank of receiving process (integer) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - address of receive buffer (choice, significant only at 'root') 

.N IMPI

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER
.N MPI_ERR_ROOT

.N ACK
@*/
int MPI_Gather(void *sbuf, int scount, MPI_Datatype sdtype, 
	       void *rbuf, int rcount, MPI_Datatype rdtype, 
	       int root, MPI_Comm comm)
{
        int rank;
        int size;
	int err;
	lam_ssi_coll_gather_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIGATHER);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return(lam_errfunc(comm, BLKMPIGATHER,
			     lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_gather;

	if (LAM_IS_INTRA(comm)) {
	  /* conditions for intracomm */
	  MPI_Comm_size(comm, &size);
	  MPI_Comm_rank(comm, &rank);
	  if ((root >= size) || (root < 0)) {
	    return(lam_errfunc(comm, BLKMPIGATHER,
			       lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	  }
	  if ((scount < 0) || (rank == root && rcount < 0)) {
	  return(lam_errfunc(comm, BLKMPIGATHER,
			     lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	  }
	  if ((sdtype == MPI_DATATYPE_NULL) ||
	      (rank == root && rdtype == MPI_DATATYPE_NULL)) {
	    return(lam_errfunc(comm, BLKMPIGATHER,
			       lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	  }
	} else {
	  /* Conditions for intercomm */
	  MPI_Comm_remote_size(comm, &size);
	  if (((root != MPI_PROC_NULL) && (sdtype == MPI_DATATYPE_NULL)) ||
	      (root == MPI_ROOT  && rdtype == MPI_DATATYPE_NULL)) {
	    return(lam_errfunc(comm, BLKMPIGATHER,
			       lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	  }
	  if (!(((root < size) && (root >= 0)) 
		|| (root == MPI_ROOT) || (root == MPI_PROC_NULL))) {
	    return(lam_errfunc(comm, BLKMPIGATHER, 
			       lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	  }
	}

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIGATHER,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIGATHER));

	/* Call the coll SSI to actually perform the barrier */
	
	if ((err = func(sbuf, scount, sdtype, rbuf, rcount, rdtype, root,
			comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIGATHER, 
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIGATHER, root, comm, sdtype, scount));
	lam_resetfunc(BLKMPIGATHER);
	return(MPI_SUCCESS);
}
