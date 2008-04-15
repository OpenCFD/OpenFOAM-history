/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 * $Id: test.c,v 6.6 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- test if request is done
 *	Accepts:	- request
 *			- flag (out)
 *			- status (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*@
    MPI_Test  - Tests for the completion of a send or receive

Input Parameter:
. req - communication request (handle) 

Output Parameter:
+ flag - true if operation completed (logical) 
- stat - status object (Status), which can be the MPI constant
'MPI_STATUS_IGNORE'

.N waitstatus

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_REQUEST
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Test(MPI_Request *req, int *flag, MPI_Status *stat)
{
	int		err;
	MPI_Comm	comm;
	
	lam_initerr_m();
	lam_setfunc_m(BLKMPITEST);

	comm = (req && *req) ? (*req)->rq_comm : MPI_COMM_WORLD;
	
	err = lam_test(req, 1, flag, stat);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPITEST, err));
	}

	lam_resetfunc_m(BLKMPITEST);
	return(MPI_SUCCESS);
}
