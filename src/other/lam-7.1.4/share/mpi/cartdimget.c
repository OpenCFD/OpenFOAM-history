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
 * $Id: cartdimget.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- get # dimensions in cartesian topology
 *	Accepts:	- communicator
 *			- ptr # dimensions (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Cartdim_get - Retrieves Cartesian topology information associated with a 
                  communicator

Input Parameter:
. comm - communicator with cartesian structure (handle) 

Output Parameter:
. pndims - number of dimensions of the cartesian structure (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cartdim_get(MPI_Comm comm, int *pndims)
{
	lam_initerr();
	lam_setfunc(BLKMPICARTDIMGET);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTDIMGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTDIMGET, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_CART(comm)) {
		return(lam_errfunc(comm, BLKMPICARTDIMGET,
				lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if (pndims == 0) {
		return(lam_errfunc(comm,
			BLKMPICARTDIMGET, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pndims = comm->c_topo_ndims;

	lam_resetfunc(BLKMPICARTDIMGET);
	return(MPI_SUCCESS);
}
