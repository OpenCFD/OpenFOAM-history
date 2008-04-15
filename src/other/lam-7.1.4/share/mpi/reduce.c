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
 * $Id: reduce.c,v 6.14 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- reduce values to root
 *	Accepts:	- send buffer
 *			- receive buffer
 *			- count of elements
 *			- type of elements
 *			- operation to perform
 *			- which process gets the result
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <lam_config.h>
#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>


/*@

MPI_Reduce - Reduces values on all processes to a single value

Input Parameters:
+ sbuf - address of send buffer (choice) 
. count - number of elements in send buffer (integer) 
. dtype - data type of elements of send buffer (handle) 
. op - reduce operation (handle) 
. root - rank of root process (integer) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - address of receive buffer (choice, 
significant only at 'root') 

.N IMPI_YES

Algorithm:

If there are 4 or less ranks involved, the root loops over receiving
from each rank, and then performs the final reduction locally.  

If there are more than 4 ranks involved, a tree-based algorithm is
used to collate the reduced data at the root (the data is reduced at
each parent in the tree so that the reduction operations are actaully
distributed).

.N fortran

.N collops

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER
.N MPI_ERR_BUFFER_ALIAS
.N MPI_ERR_ROOT

.N ACK
@*/
int MPI_Reduce(void *sbuf, void* rbuf, int count, 
	       MPI_Datatype dtype, MPI_Op op, int root, 
	       MPI_Comm comm)
{
        int size;
	int err;
	struct _gps *p;
	lam_ssi_coll_reduce_fn_t func;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIREDUCE);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return(lam_errfunc(comm, BLKMPIREDUCE,
			     lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_reduce;

	if (LAM_IS_INTRA(comm)) {
	  if (dtype == MPI_DATATYPE_NULL) {
	    return(lam_errfunc(comm, BLKMPIREDUCE, 
			       lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	  }
	  MPI_Comm_size(comm, &size);
	  if (count < 0) {
	    return(lam_errfunc(comm, BLKMPIREDUCE,
			       lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	  }
	  if ((root < 0) || (root >= size)) {
	    return(lam_errfunc(comm, BLKMPIREDUCE,
			       lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	  }
	  
	  if (op == MPI_OP_NULL) {
	    return(lam_errfunc(comm, BLKMPIREDUCE,
			       lam_mkerr(MPI_ERR_OP, EINVAL)));
	  }
	} else {
	  /* Conditions for intercommunicator */
	  if ((op == MPI_OP_NULL || count < 0) && root != MPI_PROC_NULL) {
	    return(lam_errfunc(comm, BLKMPIREDUCE,
			       lam_mkerr(MPI_ERR_OP, EINVAL)));
	  }

	  MPI_Comm_remote_size(comm, &size);
	  if (!(((root < size) && (root >= 0)) 
		|| (root == MPI_ROOT) || (root == MPI_PROC_NULL))) {
	    return(lam_errfunc(comm, BLKMPIGATHER, 
			       lam_mkerr(MPI_ERR_ROOT, EINVAL)));
	  }
	}
	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIREDUCE,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIREDUCE));
/*
 * Remember required parameters.
 */
	if (LAM_IS_INTRA(comm)) {
	  p = &(comm->c_group->g_procs[root]->p_gps);
	  lam_setparam(BLKMPIREDUCE, root | (p->gps_grank << 16),
		       (p->gps_node << 16) | p->gps_idx);
	}
/*
 * Check for zero count case.
 */
	if (count == 0 && comm->c_ssi_coll.lsca_reduce_optimization == 1) {
		LAM_TRACE(lam_tr_cffend(BLKMPIREDUCE,
					root, comm, dtype, count));
		lam_resetfunc_m(BLKMPIREDUCE);
		return(MPI_SUCCESS);
	}

	/* Call the coll SSI to actually perform the operation */
	
	if ((err = func(sbuf, rbuf, count, dtype, op, root,
			comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIREDUCE,
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIREDUCE, root, comm, dtype, count));
	lam_resetfunc_m(BLKMPIREDUCE);
	return(MPI_SUCCESS);
}
