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
 * $Id: allgather.c,v 6.18 2004/03/02 03:43:28 jsquyres Exp $
 *
 *	Function:	- gather buffers at all process ranks
 *	Accepts:	- send buffer
 *			- send count
 *			- send datatype
 *			- recv buffer
 *			- recv count
 *			- recv datatype
 *			- communicator
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <lam-ssi-coll.h>


/*@

MPI_Allgather - Gathers data from all tasks and distribute it to all 

Input Parameters:
+ sbuf - starting address of send buffer (choice) 
. scount - number of elements in send buffer (integer) 
. sdtype - data type of send buffer elements (handle) 
. rbuf - starting address of receive buffer (choice) 
. rcount - number of elements received from any process (integer) 
. rdtype - data type of receive buffer elements (handle) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - address of receive buffer (choice) 

Notes:
 The MPI standard (1.0 and 1.1) says 

 "The jth block of data sent from each proess is received by every
 process and placed in the jth block of the buffer recvbuf."

 This is misleading; a better description is

 "The block of data sent from the jth process is received by every
 process and placed in the jth block of the buffer recvbuf."

 This text was suggested by Rajeev Thakur.

.N IMPI

.N fortran

.N Errors
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER

.N ACK
@*/
int MPI_Allgather(void *sbuf, int scount, MPI_Datatype sdtype, 
		  void *rbuf, int rcount, MPI_Datatype rdtype, 
		  MPI_Comm comm)
{
        int err;
	lam_ssi_coll_allgather_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIALLGATHER);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return(lam_errfunc(comm, BLKMPIALLGATHER,
			     lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	func = comm->c_ssi_coll.lsca_allgather;

	if ((sdtype == MPI_DATATYPE_NULL) || (rdtype == MPI_DATATYPE_NULL)) {
	  return(lam_errfunc(comm, BLKMPIALLGATHER,
			     lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
	
	if ((scount < 0) || (rcount < 0)) {
	  return(lam_errfunc(comm, BLKMPIALLGATHER,
			     lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIALLGATHER,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIALLGATHER));

	/* Call the coll SSI to actually perform the barrier */

	if ((err = func(sbuf, scount, sdtype, rbuf, rcount, rdtype, 
			comm)) != MPI_SUCCESS)
	  return lam_errfunc(comm, BLKMPIALLGATHER,
			     lam_mkerr(MPI_ERR_COMM, err));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIALLGATHER, -1, comm, sdtype, scount));
	lam_resetfunc(BLKMPIALLGATHER);
	return(MPI_SUCCESS);
}
