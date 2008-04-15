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
 * $Id: gsize.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- return the size of the group
 *	Accepts:	- group
 *			- ptr to size (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Group_size - Returns the size of a group

Input Parameters:
+ group - group (handle) 

Output Parameter:
- psize - number of processes in the group (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_GROUP
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Group_size(MPI_Group group, int *psize)
{
	lam_initerr();
	lam_setfunc(BLKMPIGROUPSIZE);

	if ((group != MPI_GROUP_NULL) && (psize != 0)) {
		*psize = group->g_nprocs;
	}
	else if (group == MPI_GROUP_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPSIZE, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}
	else {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIGROUPSIZE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	lam_resetfunc(BLKMPIGROUPSIZE);
	return(MPI_SUCCESS);
}
