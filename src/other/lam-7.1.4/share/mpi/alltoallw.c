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
 * $Id: alltoallw.c,v 6.3 2004/03/02 03:43:28 jsquyres Exp $
 *
 *	Function:	- send/recv var. len. data from each node to all nodes
 *	Accepts:	- send buffer
 *			- send counts
 *			- send displacements
 *			- send datatype
 *			- recv buffer
 *			- recv counts
 *			- recv displacements
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

MPI_Alltoallw - Sends data from all to all processes, with a displacement

Input Parameters:
+ sbuf - starting address of send buffer (choice) 
. scounts - integer array equal to the group size 
specifying the number of elements to send to each processor 
. sdisps - integer array (of length group size). Entry 
 'j'  specifies the displacement (relative to sendbuf  from
which to take the outgoing data destined for process  'j'  
. sdtype - data type of send buffer elements (handle) 
. rcounts - integer array equal to the group size 
specifying the maximum number of elements that can be received from
each processor 
. rdisps - integer array (of length group size). Entry 
 'i'  specifies the displacement (relative to recvbuf  at
which to place the incoming data from process  'i'  
. rdtypes - data types of receive buffer elements (handle) 
- comm - communicator (handle) 

Output Parameter:
. rbuf - address of receive buffer (choice) 

Notes:

The receive buffer 'rbuf' needs to be large enough to hold the data
that will be received from `all` processes.

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
int MPI_Alltoallw(void *sbuf, int *scounts, int *sdisps, 
		  MPI_Datatype *sdtypes, void *rbuf, int *rcounts, 
		  int *rdisps, MPI_Datatype *rdtypes, 
		  MPI_Comm comm)
{
        int err, size, rank, i;
	lam_ssi_coll_alltoallw_fn_t func;

	lam_initerr();
	lam_setfunc(BLKMPIALLTOALLW);
/*
 * Check for invalid arguments.
 */
	if (comm == MPI_COMM_NULL) {
	  return(lam_errfunc(comm, BLKMPIALLTOALLW,
			     lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &rank);

	func = comm->c_ssi_coll.lsca_alltoallw;

	for (i = 0; i < size; ++i) {
	  if (rdtypes[i] == MPI_DATATYPE_NULL) {
	    return(lam_errfunc(comm, BLKMPIALLTOALLW,
			       lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	  }
	  if (sdtypes[i] == MPI_DATATYPE_NULL) {
	    return(lam_errfunc(comm, BLKMPIALLTOALLW,
			       lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	  }
	}

	if ((scounts == 0) || (rcounts == 0)) {
	  return(lam_errfunc(comm, BLKMPIALLTOALLW,
			     lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((sdisps == 0) || (rdisps == 0)) {
	  return(lam_errfunc(comm, BLKMPIALLTOALLW,
			     lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (func == NULL) {
	  return(lam_errfunc(comm, BLKMPIALLTOALLW,
			     lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIALLTOALLW));

	/* Call the coll SSI to actually perform the barrier */
    
	if ((err = func(sbuf, scounts, sdisps, sdtypes, rbuf, rcounts,
			rdisps, rdtypes, comm)) != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIALLTOALLW,
			     lam_mkerr(MPI_ERR_COMM, err)));

	/* All done */

	LAM_TRACE(lam_tr_cffend(BLKMPIALLTOALLW, -1, comm, sdtypes[rank], 0));
	lam_resetfunc(BLKMPIALLTOALLW);
	return(MPI_SUCCESS);
}
