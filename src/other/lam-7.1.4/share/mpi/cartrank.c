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
 * $Id: cartrank.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- translate coordinates to rank
 *	Accepts:	- communicator
 *			- coordinates array
 *			- ptr rank (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Cart_rank - Determines process rank in communicator given Cartesian
                location

Input Parameters:
+ comm - communicator with cartesian structure (handle) 
- coords - integer array (of size  'ndims') specifying the cartesian 
  coordinates of a process 

Output Parameter:
. prank - rank of specified process (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_rank(MPI_Comm comm, int *coords, int *prank)
{
	int		rank;
	int		dim;
	int		ord;
	int		factor;
	int		i;
	int		*d;
	int		*c;

	lam_initerr();
	lam_setfunc(BLKMPICARTRANK);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTRANK, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTRANK, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_CART(comm)) {
		return(lam_errfunc(comm, BLKMPICARTRANK,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((coords == 0) || (prank == 0)) {
		return(lam_errfunc(comm,
			BLKMPICARTRANK, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Loop over coordinates computing the rank.
 */
	factor = 1;
	rank = 0;
	i = comm->c_topo_ndims - 1;
	d = comm->c_topo_dims + i;
	c = coords + i;

	for (; i >= 0; --i, --c, --d) {
		dim = (*d > 0) ? *d : -(*d);
		ord = *c;
		if ((ord < 0) || (ord >= dim)) {
			if (*d > 0) {
				return(lam_errfunc(comm, BLKMPICARTRANK,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
			}

			ord %= dim;
			if (ord < 0) {
				ord += dim;
			}
		}

		rank += factor * ord;
		factor *= dim;
	}

	*prank = rank;

	lam_resetfunc(BLKMPICARTRANK);
	return(MPI_SUCCESS);
}
