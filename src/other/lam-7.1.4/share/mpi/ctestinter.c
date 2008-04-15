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
 * $Id: ctestinter.c,v 6.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- check if inter-communicator
 *	Accepts:	- communicator
 *			- flag (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Comm_test_inter - Tests to see if a comm is an inter-communicator

Input Parameter:
. comm - communicator (handle) 

Output Parameter:
. flag - (logical) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_test_inter(MPI_Comm comm, int *flag)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMTESTINTER);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMTESTINTER, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (flag == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMTESTINTER, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*flag = LAM_IS_INTER(comm) ? 1 : 0;

	lam_resetfunc(BLKMPICOMMTESTINTER);
	return(MPI_SUCCESS);
}
