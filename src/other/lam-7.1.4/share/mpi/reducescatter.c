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
 * $Id: reducescatter.c,v 6.12 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- reduce and scatter data to all processes
 *	Accepts:	- send buffer
 *			- receive buffer
 *			- counts of elements to recv
 *			- type of elements
 *			- operation to perform
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@

MPI_Reduce_scatter - Combines values and scatters the results

Input Parameters:
+ sbuf - starting address of send buffer (choice) 
. rcounts - integer array specifying the 
number of elements in result distributed to each process.
Array must be identical on all calling processes. 
. dtype - data type of elements of input buffer (handle) 
. op - operation (handle) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - starting address of receive buffer (choice) 

.N IMPI

.N fortran

.N collops

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_IMPI
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER
.N MPI_ERR_OP
.N MPI_ERR_BUFFER
.N MPI_ERR_BUFFER_ALIAS

.N ACK
@*/
int MPI_Reduce_scatter(void *sbuf, void *rbuf, int *rcounts, 
		       MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
        int rank;
	int err;
	lam_ssi_coll_reduce_scatter_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIREDUCESCATTER);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return (lam_errfunc(comm, BLKMPIREDUCESCATTER,
			      lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_reduce_scatter;

	if (dtype == MPI_DATATYPE_NULL) {
	  return(lam_errfunc(comm, BLKMPIREDUCESCATTER,
			     lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (op == MPI_OP_NULL) {
	  return(lam_errfunc(comm, BLKMPIREDUCESCATTER,
			     lam_mkerr(MPI_ERR_OP, EINVAL)));
	}

	if (rcounts == 0) {
	  return (lam_errfunc(comm, BLKMPIREDUCESCATTER,
			      lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIREDUCESCATTER,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIREDUCESCATTER));
        MPI_Comm_rank(comm, &rank);

	/* Call the coll SSI to actually perform the operation */
	
	if ((err = func(sbuf, rbuf, rcounts, dtype, op,
			comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIREDUCESCATTER,
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIREDUCESCATTER,
				-1, comm, dtype, rcounts[rank]));
	lam_resetfunc(BLKMPIREDUCESCATTER);
	return(MPI_SUCCESS);
}
