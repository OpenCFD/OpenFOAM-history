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
 * $Id: grank.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- get caller's rank in group
 *	Accepts:	- group
 *			- ptr rank
 *	Returns:	- 0 or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Group_rank - Returns the rank of this process in the given group

Input Parameters:
. group - group (handle) 

Output Parameter:
. rank - rank of the calling process in group (integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_ARG

.N ACK
@*/
int
MPI_Group_rank(MPI_Group group, int *prank)
{
	lam_initerr();
	lam_setfunc(BLKMPIGROUPRANK);

	if ((group != MPI_GROUP_NULL) && (prank != 0)) {
		*prank = group->g_myrank;
	}
	else if (group == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPRANK, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}
	else {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPRANK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_resetfunc(BLKMPIGROUPRANK);
	return(MPI_SUCCESS);
}
