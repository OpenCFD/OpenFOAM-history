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
 * $Id: barrier.c,v 6.12 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- barrier synchronization
 *	Accepts:	- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lam-ssi-coll.h>


/*@

MPI_Barrier - Blocks until all process have reached this routine.

Input Parameters:
. comm - communicator (handle) 

Notes:

Blocks the caller until all group members have called it; the call
returns at any process only after all group members have entered the
call.

.N IMPI_YES

Algorithm:  

For 4 or less ranks, a linear algorithm is used, where rank 0
first receives synchronization message from each other rank.  Rank 0
then sends out a synchronization message to each other rank.

If more than 4 ranks are involved, a tree-based algorithm is used to
receive and then send the synchronization messages to and from rank 0.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED

.N ACK
@*/
int MPI_Barrier(MPI_Comm comm)
{
	int err;
	int size;
	lam_ssi_coll_barrier_fn_t func;
/*
 * Prepare error handler if needed.
 */
	lam_initerr();
	lam_setfunc(BLKMPIBARRIER);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(comm,
			BLKMPIBARRIER, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_barrier;

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIBARRIER,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIBARRIER));

	/* Obvious case */

	MPI_Comm_size(comm, &size);
	if (size <= 1) {
	  LAM_TRACE(lam_tr_cffend(BLKMPIBARRIER, -1, comm, 0, 0));
	  lam_resetfunc(BLKMPIBARRIER);
	  return MPI_SUCCESS;
	}

	/* Call the coll SSI to actually perform the barrier */

	if ((err = func(comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIBARRIER, 
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIBARRIER, -1, comm, 0, 0));
	lam_resetfunc(BLKMPIBARRIER);
	return MPI_SUCCESS;
}
