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
 * $Id: alltoall.c,v 6.13 2004/03/02 03:43:28 jsquyres Exp $
 *
 *	Function:	- send/recv data from each node to all nodes
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- recv buffer
 *			- recv count
 *			- recv datatype
 *			- communicator
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */

#include <stdlib.h>

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>


/*@

MPI_Alltoall - Sends data from all to all processes

Input Parameters:
+ sbuf - starting address of send buffer (choice) 
. scount - number of elements to send to each process (integer) 
. sdtype - data type of send buffer elements (handle) 
. rcount - number of elements received from any process (integer) 
. rdtype - data type of receive buffer elements (handle) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - address of receive buffer (choice) 

Notes:

The 'rcount' parameter is the value to be expected from `each`
process, not the total that is expected from `all` processes.  The
receive buffer 'rbuf', however, needs to be large enough to hold the
data that will be received from `all` processes, however.

For example, 'scount' and 'sdtype' and typically the same as 'rcount'
and 'rdtype'.

.N IMPI

.N fortran

.N Errors
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_IMPI
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER

.N ACK
@*/
int MPI_Alltoall(void *sbuf, int scount, MPI_Datatype sdtype, 
		 void* rbuf, int rcount, MPI_Datatype rdtype, 
		 MPI_Comm comm)
{
	int err;
	lam_ssi_coll_alltoall_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIALLTOALL);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return(lam_errfunc(comm, BLKMPIALLTOALL,
			     lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_alltoall;

	if ((sdtype == MPI_DATATYPE_NULL) || (rdtype == MPI_DATATYPE_NULL)) {
	  return(lam_errfunc(comm, BLKMPIALLTOALL,
			     lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if ((scount < 0) || (rcount < 0)) {
	  return(lam_errfunc(comm, BLKMPIALLTOALL,
			     lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIALLTOALL,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIALLTOALL));

	/* Call the coll SSI to actually perform the barrier */
    
	if ((err = func(sbuf, scount, sdtype, rbuf, rcount, rdtype,
			comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIALLTOALL,
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIALLTOALL, -1, comm, sdtype, scount));
	lam_resetfunc(BLKMPIALLTOALL);
	return MPI_SUCCESS;
}
