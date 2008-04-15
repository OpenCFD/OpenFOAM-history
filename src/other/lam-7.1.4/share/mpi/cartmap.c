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
 * $Id: cartmap.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- map processes to cartesian coordinates
 *	Accepts:	- communicator
 *			- # dimensions
 *			- # procs per dimension
 *			- dimension periodicity
 *			- caller's new rank (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Cart_map - Maps process to Cartesian topology information 

Input Parameters:
+ comm - input communicator (handle) 
. ndims - number of dimensions of cartesian structure (integer) 
. dims - integer array of size 'ndims' specifying the number of
processes in each coordinate direction
- periods - logical array of size 'ndims' specifying the periodicity
specification in each coordinate direction

Output Parameter:
. newrank - reordered rank of the calling process; 'MPI_UNDEFINED' if
calling process does not belong to grid (integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_DIMS
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_map(MPI_Comm comm, int ndims, int *dims, 
		 int *periods, int *newrank)
{
	int		nprocs;
	int		rank;
	int		size;
	int		errcode;
	int		i;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPICARTMAP);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTMAP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTMAP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (ndims < 1) {
		return(lam_errfunc(comm,
			BLKMPICARTMAP, lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}

	if ((dims == 0) || (periods == 0) || (newrank == 0)) {
		return(lam_errfunc(comm,
			BLKMPICARTMAP, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Compute the # of processes in the grid.
 */
	nprocs = 1;
	for (i = 0, p = dims; i < ndims; ++i, ++p) {
		if (*p <= 0) {
			return(lam_errfunc(comm, BLKMPICARTMAP,
					lam_mkerr(MPI_ERR_DIMS, EINVAL)));
		}

		nprocs *= *p;
	}
/*
 * Check that number of processes <= size of communicator.
 */
	errcode = MPI_Comm_size(comm, &size);
	if (errcode != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICARTMAP, errcode));
	}

	if (nprocs > size) {
		return(lam_errfunc(comm, BLKMPICARTMAP,
					lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}
/*
 * Compute my new rank.
 */
	errcode = MPI_Comm_rank(comm, &rank);
	if (errcode != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICARTMAP, errcode));
	}

	*newrank = ((rank < 0) || (rank >= nprocs)) ? MPI_UNDEFINED : rank;

	lam_resetfunc(BLKMPICARTMAP);
	return(MPI_SUCCESS);
}
