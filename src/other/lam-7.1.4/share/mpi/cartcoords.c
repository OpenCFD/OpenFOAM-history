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
 * $Id: cartcoords.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- translate rank to coordinates
 *	Accepts:	- communicator
 *			- rank
 *			- max # dimensions to fill
 *			- coordinates array (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Cart_coords - Determines process coords in cartesian topology given
                  rank in group

Input Parameters:
+ comm - communicator with cartesian structure (handle) 
. rank - rank of a process within group of 'comm' (integer) 
- maxdims - length of vector 'coords' in the calling program (integer) 

Output Parameter:
. coords - integer array (of size 'ndims') containing the cartesian
coordinates of specified process (integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_RANK
.N MPI_ERR_DIMS
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, 
		    int *coords)
{
	int		dim;
	int		remprocs;
	int		i;
	int		*d;

	lam_initerr();
	lam_setfunc(BLKMPICARTCOORDS);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTCOORDS, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTCOORDS, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_CART(comm)) {
		return(lam_errfunc(comm, BLKMPICARTCOORDS,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((rank < 0) || (rank >= comm->c_group->g_nprocs)) {
		return(lam_errfunc(comm,
			BLKMPICARTCOORDS, lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

	if ((maxdims < 0) || ((maxdims > 0) && (coords == 0))) {
		return(lam_errfunc(comm,
			BLKMPICARTCOORDS, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Loop computing the coordinates.
 */
	d = comm->c_topo_dims;
	remprocs = comm->c_topo_nprocs;
	for (i = 0; (i < comm->c_topo_ndims) && (i < maxdims); ++i, ++d) {
		dim = (*d > 0) ? *d : -(*d);
		remprocs /= dim;
		*coords++ = rank / remprocs;
		rank %= remprocs;
	}

	lam_resetfunc(BLKMPICARTCOORDS);
	return(MPI_SUCCESS);
}
