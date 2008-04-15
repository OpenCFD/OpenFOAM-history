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
 * $Id: cartshift.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- get src and dest ranks in cartesian shift
 *	Accepts:	- communicator
 *			- dimension of shift
 *			- displacement
 *			- ptr source rank
 *			- ptr destination rank
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Cart_shift - Returns the shifted source and destination ranks, given a 
                 shift direction and amount

Input Parameters:
+ comm - communicator with cartesian structure (handle) 
. dim - coordinate dimension of shift (integer) 
- disp - displacement (> 0: upwards shift, < 0: downwards shift) (integer) 

Output Parameters:
+ psrc - rank of source process (integer) 
- pdest - rank of destination process (integer) 

Notes:

The 'dim' argument is in the range '[0,n-1]' for an n-dimensional
Cartesian mesh.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_DIMS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_shift(MPI_Comm comm, int dim, int disp, 
		   int *psrc, int *pdest)
{
	int		factor;
	int		thisdim = 0;
	int		thisperiod = 0;
	int		ord;
	int		srcord;
	int		destord;
	int		i;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPICARTSHIFT);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTSHIFT, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTSHIFT, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_CART(comm)) {
		return(lam_errfunc(comm, BLKMPICARTSHIFT,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((dim < 0) || (dim >= comm->c_topo_ndims)) {
		return(lam_errfunc(comm,
			BLKMPICARTSHIFT, lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}

	if ((psrc == 0) || (pdest == 0)) {
		return(lam_errfunc(comm,
			BLKMPICARTSHIFT, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Handle the trivial case.
 */
	ord = comm->c_group->g_myrank;
	if (disp == 0) {
		*pdest = *psrc = ord;
		lam_resetfunc(BLKMPICARTSHIFT);
		return(MPI_SUCCESS);
	}
/*
 * Compute the rank factor and ordinate.
 */
	factor = comm->c_topo_nprocs;
	p = comm->c_topo_dims;
	for (i = 0; (i < comm->c_topo_ndims) && (i <= dim); ++i, ++p) {
		if ((thisdim = *p) > 0) {
			thisperiod = 0;
		} else {
			thisperiod = 1;
			thisdim = -thisdim;
		}

		ord %= factor;
		factor /= thisdim;
	}

	ord /= factor;
/*
 * Check the displacement value and compute the new ranks.
 */
	*psrc = *pdest = MPI_UNDEFINED;

	srcord = ord - disp;
	destord = ord + disp;

	if ( ((destord < 0) || (destord >= thisdim)) && (!thisperiod) ) {
		*pdest = MPI_PROC_NULL;
	} else {
		destord %= thisdim;
		if (destord < 0) destord += thisdim;

		*pdest = comm->c_group->g_myrank + ((destord - ord) * factor);
	}

	if ( ((srcord < 0) || (srcord >= thisdim)) && (!thisperiod) ) {
		*psrc = MPI_PROC_NULL;
	} else {
		srcord %= thisdim;
		if (srcord < 0) srcord += thisdim;

		*psrc = comm->c_group->g_myrank + ((srcord - ord) * factor);
	}

	lam_resetfunc(BLKMPICARTSHIFT);
	return(MPI_SUCCESS);
}
