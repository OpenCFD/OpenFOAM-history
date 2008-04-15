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
 * $Id: crank.c,v 6.6 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get caller's rank in communicator
 *	Accepts:	- communicator
 *			- rank (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Comm_rank - Determines the rank of the calling process in the
communicator

Input Parameters:
. comm - communicator (handle) 

Output Parameter:
. rank - rank of the calling process in group of  'comm'  (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMRANK);

	if ((comm != MPI_COMM_NULL) && (rank != 0)) {
		*rank = comm->c_group->g_myrank;
	}
	else if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMRANK, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}
	else {
		return(lam_errfunc(comm,
			BLKMPICOMMRANK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_resetfunc_m(BLKMPICOMMRANK);
	return(MPI_SUCCESS);
}
