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
 * $Id: csize.c,v 6.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- return the size of the communicator's group
 *	Accepts:	- communicator
 *			- ptr to size (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Comm_size - Determines the size of the group associated with a
communictor

Input Parameter:
. comm - communicator (handle - must be intracommunicator) 

Output Parameter:
. psize - number of processes in the group of 'comm'  (integer) 

Notes:

'MPI_COMM_NULL' is `not` considered a valid argument to this function.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_size(MPI_Comm comm, int *psize)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMSIZE);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMSIZE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (psize == 0) {
		return(lam_errfunc(comm,
			BLKMPICOMMSIZE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*psize = comm->c_group->g_nprocs;

	lam_resetfunc(BLKMPICOMMSIZE);
	return(MPI_SUCCESS);
}
