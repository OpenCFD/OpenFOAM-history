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
 * $Id: testcancel.c,v 6.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- test if cancellation succeeded
 *	Accepts:	- ptr status structure
 *			- ptr flag (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
  MPI_Test_cancelled - Tests to see if a request was cancelled

Input Parameter:
. pstat - status object (Status) 

Output Parameter:
. pflag - true (1) if the request has been cancelled, false (0)
otherwise (logical)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Test_cancelled(MPI_Status *pstat, int *pflag)
{
	lam_setfunc(BLKMPITESTCANCEL);

	if ((pstat == 0) || (pflag == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITESTCANCEL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pflag = (pstat->MPI_SOURCE == MPI_CANCEL_SOURCE) ? 1 : 0;

	lam_resetfunc(BLKMPITESTCANCEL);
	return(MPI_SUCCESS);
}
