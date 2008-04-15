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
 * $Id: cartcreate.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- create a communicator with cartesian mapping
 *	Accepts:	- old communicator
 *			- # dimensions
 *			- # procs per dimension
 *			- dimension periodicity
 *			- rank reordering flag
 *			- ptr new communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@

MPI_Cart_create - Makes a new communicator to which topology information
                  has been attached

Input Parameters:
+ comm - input communicator (handle) 
. ndims - number of dimensions of cartesian grid (integer) 
. dims - integer array of size ndims specifying the number of processes in 
  each dimension 
. periods - logical array of size ndims specifying whether the grid is 
  periodic (true) or not (false) in each dimension 
- reorder - ranking may be reordered (true) or not (false) (logical) 

Output Parameter:
. pnewcomm - communicator with new cartesian topology (handle) 

Algorithm:

LAM/MPI currently ignores the 'reorder' flag.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_DIMS
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_create(MPI_Comm comm, int ndims, int *dims, 
		    int *periods, int reorder, MPI_Comm *pnewcomm)
{
	MPI_Comm	newcomm;
	MPI_Group	newgroup;
	int		size;
	int		nprocs;
	int		rank;
	int		err;
	int		range[1][3];
	int		i;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPICARTCREATE);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTCREATE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTCREATE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (ndims < 1) {
		return(lam_errfunc(comm,
			BLKMPICARTCREATE, lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}

	if ((dims == 0) || (periods == 0) || (pnewcomm == 0)) {
		return(lam_errfunc(comm,
			BLKMPICARTCREATE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPICARTCREATE));
/*
 * Compute the # of processes in the grid.
 */
	nprocs = 1;
	for (i = 0, p = dims; i < ndims; ++i, ++p) {
		if (*p <= 0) {
			return(lam_errfunc(comm, BLKMPICARTCREATE,
					lam_mkerr(MPI_ERR_DIMS, EINVAL)));
		}

		nprocs *= *p;
	}
/*
 * Create the group for the new communicator.
 */
	err = MPI_Comm_size(comm, &size);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICARTCREATE, err));
	}

	if (nprocs > size) {
		return(lam_errfunc(comm, BLKMPICARTCREATE,
				lam_mkerr(MPI_ERR_DIMS, EINVAL)));
	}

	if (nprocs == size) {
		err = MPI_Comm_group(comm, &newgroup);
	} else {
		range[0][0] = 0;
		range[0][1] = nprocs - 1;
		range[0][2] = 1;
		err = MPI_Group_range_incl(comm->c_group, 1, range, &newgroup);
	}

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICARTCREATE, err));
	}
/*
 * Create the new communicator.
 */
	err = MPI_Comm_create(comm, newgroup, pnewcomm);
	if (err != MPI_SUCCESS) {
		MPI_Group_free(&newgroup);
		return(lam_errfunc(comm, BLKMPICARTCREATE, err));
	}
/*
 * Fill the communicator with topology information. 
 */
	newcomm = *pnewcomm;
	if (newcomm != MPI_COMM_NULL) {
		newcomm->c_topo_type = MPI_CART;
		newcomm->c_topo_nprocs = nprocs;
		newcomm->c_topo_ndims = ndims;

		newcomm->c_topo_dims = (int *)
				malloc((unsigned) 2 * ndims * sizeof(int));
		if (newcomm->c_topo_dims == 0) {
			return(lam_errfunc(comm, BLKMPICARTCREATE,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
		newcomm->c_topo_coords = newcomm->c_topo_dims + ndims;

		for (i = 0, p = newcomm->c_topo_dims; i < ndims; ++i, ++p) {
			*p = (*periods) ? -(*dims) : *dims;
			++dims;
			++periods;
		}
/*
 * Compute the caller's coordinates.
 */
		err = MPI_Comm_rank(newcomm, &rank);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICARTCREATE, err));
		}

		err = MPI_Cart_coords(newcomm, rank,
					ndims, newcomm->c_topo_coords);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICARTCREATE, err));
		}
	}

	err = MPI_Group_free(&newgroup);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICARTCREATE, err));
	}

	LAM_TRACE(lam_tr_cffend(BLKMPICARTCREATE, -1, comm, 0, 0));

	lam_resetfunc(BLKMPICARTCREATE);
	return(MPI_SUCCESS);
}
