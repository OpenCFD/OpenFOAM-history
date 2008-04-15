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
 * $Id: allreduce.c,v 6.11 2004/03/02 03:43:28 jsquyres Exp $
 *
 *	Function:	- reduce and return data to all processes
 *	Accepts:	- send buffer
 *			- receive buffer
 *			- count of elements
 *			- type of elements
 *			- operation to perform
 *			- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <lam-ssi-coll.h>

/*@

MPI_Allreduce - Combines values from all processes and distribute the result
                back to all processes

Input Parameters:
+ sbuf - starting address of send buffer (choice) 
. count - number of elements in send buffer (integer) 
. dtype - data type of elements of send buffer (handle) 
. op - operation (handle) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - starting address of receive buffer (choice) 

.N IMPI_YES

.N fortran

.N collops

.N Errors
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_IMPI
.N MPI_ERR_BUFFER
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_OP

.N ACK
@*/
int MPI_Allreduce(void *sbuf, void *rbuf, int count, 
		  MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
        int err;
	lam_ssi_coll_allreduce_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIALLREDUCE);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return (lam_errfunc(comm, BLKMPIALLREDUCE,
			      lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_allreduce;

	if (dtype == MPI_DATATYPE_NULL) {
	  return(lam_errfunc(comm, BLKMPIALLREDUCE,
			     lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIALLREDUCE,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIALLREDUCE));

	/* Call the coll SSI to actually perform the barrier */
    
	if ((err = func(sbuf, rbuf, count, dtype, op, comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIALLREDUCE,
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIALLREDUCE, -1, comm, dtype, count));
	lam_resetfunc(BLKMPIALLREDUCE);
	return(MPI_SUCCESS);
}
