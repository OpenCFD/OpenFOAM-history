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
 * $Id: gatherv.c,v 6.11 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- gather vectored buffers at root in process rank order
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- recv buffer
 *			- recv counts
 *			- displacements
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

/*@

MPI_Gatherv - Gathers into specified locations from all processes in a group

Input Parameters:
+ sendbuf - starting address of send buffer (choice) 
. sendcount - number of elements in send buffer (integer) 
. sendtype - data type of send buffer elements (handle) 
. recvcounts - integer array (of length group size) 
containing the number of elements that are received from each process
(significant only at 'root') 
. displs - integer array (of length group size). Entry 
 'i'  specifies the displacement relative to recvbuf  at
which to place the incoming data from process  'i'  (significant only
at root) 
. recvtype - data type of recv buffer elements 
(significant only at 'root') (handle) 
. root - rank of receiving process (integer) 
- comm - communicator (handle) 

Output Parameter:
. recvbuf - address of receive buffer (choice, significant only at 'root') 

.N IMPI

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_IMPI
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER
.N MPI_ERR_ROOT
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Gatherv(void *sbuf, int scount, MPI_Datatype sdtype,
		void *rbuf, int *rcounts, int *disps, 
		MPI_Datatype rdtype, int root, MPI_Comm comm)
{
        int rank;
	int size;
	int err;
	lam_ssi_coll_gatherv_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIGATHERV);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return(lam_errfunc(comm, BLKMPIGATHERV,
			     lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_gatherv;

	if (LAM_IS_INTRA(comm)) {
	  MPI_Comm_rank(comm, &rank);
	  MPI_Comm_size(comm, &size);
	  if ((root >= size) || (root < 0)) {
	    return(lam_errfunc(comm, BLKMPIGATHERV,
			       lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	  }

	  if ((sdtype == MPI_DATATYPE_NULL) ||
	      (rank == root && rdtype == MPI_DATATYPE_NULL)) {
	    return(lam_errfunc(comm, BLKMPIGATHERV,
			     lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	  }
	  

	  if ((rank == root) && (disps == 0)) {
	    return(lam_errfunc(comm, BLKMPIGATHERV,
			       lam_mkerr(MPI_ERR_ARG, EINVAL)));
	  }
	  
	  if ((scount < 0) || (rank == root && rcounts == 0)) {
	    return(lam_errfunc(comm, BLKMPIGATHERV,
			       lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	  }
	} else {
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
	  return(lam_errfunc(comm, BLKMPIGATHERV,
			       lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIGATHERV));

	/* Call the coll SSI to actually perform the barrier */
	
	if ((err = func(sbuf, scount, sdtype, rbuf, rcounts, disps,
			rdtype, root, comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIGATHERV, 
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIGATHERV, root, comm, sdtype, scount));
	lam_resetfunc(BLKMPIGATHERV);
	return(MPI_SUCCESS);
}
