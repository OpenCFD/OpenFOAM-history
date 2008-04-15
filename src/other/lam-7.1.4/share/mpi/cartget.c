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
 * $Id: cartget.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- get info on cartesian topology
 *	Accepts:	- communicator
 *			- size of dimensions array
 *			- dimensions array
 *			- periodicity array
 *			- coordinates array
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Cart_get - Retrieves Cartesian topology information associated with a 
               communicator

Input Parameters:
+ comm - communicator with cartesian structure (handle) 
- maxndims - length of vectors  'dims', 'periods', and 'coords'
in the calling program (integer) 

Output Parameters:
+ dims - number of processes for each cartesian dimension (array of integer) 
. periods - periodicity (true/false) for each cartesian dimension 
(array of logical) 
- coords - coordinates of calling process in cartesian structure 
(array of integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_get(MPI_Comm comm, int maxndims, int *dims, 
		 int *periods, int *coords)
{
	int		i;
	int		*d;
	int		*c;

	lam_initerr();
	lam_setfunc(BLKMPICARTGET);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_CART(comm)) {
		return(lam_errfunc(comm, BLKMPICARTGET,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((maxndims < 0) || ((maxndims > 0) &&
			((dims == 0) || (periods == 0) || (coords == 0)))) {
		return(lam_errfunc(comm,
			BLKMPICARTGET, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Fill the dimension, period, and coordinate arrays.
 */
	d = comm->c_topo_dims;
	c = comm->c_topo_coords;
	for (i = 0; (i < comm->c_topo_ndims) && (i < maxndims); ++i) {
		if (*d > 0) {
			*dims++ = *d++;
			*periods++ = 0;
		} else {
			*dims++ = -(*d++);
			*periods++ = 1;
		}

		*coords++ = *c++;
	}

	lam_resetfunc(BLKMPICARTGET);
	return(MPI_SUCCESS);
}
