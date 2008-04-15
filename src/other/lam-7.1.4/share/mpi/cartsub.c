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
 * $Id: cartsub.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- divide cartesian communicator into sub-grids
 *	Accepts:	- old communicator
 *			- array of kept dimensions
 *			- ptr new communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@

MPI_Cart_sub - Partitions a communicator into subgroups which 
               form lower-dimensional cartesian subgrids

Input Parameters:
+ comm - communicator with cartesian structure (handle) 
- remdims - the 'i'th entry of 'remdims' specifies whether the 'i'th
dimension is kept in the subgrid (true) or is dropped (false) (logical
vector)

Output Parameter:
. pnewcomm - communicator containing the subgrid that includes the
calling process (handle)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TOPOLOGY
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Cart_sub(MPI_Comm comm, int *remdims, MPI_Comm *pnewcomm)
{
	MPI_Comm	newcomm;
	int		errcode;
	int		colour;
	int		key;
	int		colfactor;
	int		keyfactor;
	int		rank;
	int		ndim;
	int		dim;
	int		allfalse;
	int		i;
	int		*d;
	int		*c;
	int		*r;
	int		*p;

	lam_initerr();
	lam_setfunc(BLKMPICARTSUB);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICARTSUB, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTSUB, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (!LAM_IS_CART(comm)) {
		return(lam_errfunc(comm,
			BLKMPICARTSUB, lam_mkerr(MPI_ERR_TOPOLOGY, EINVAL)));
	}

	if ((remdims == 0) || (pnewcomm == 0)) {
		return(lam_errfunc(comm,
			BLKMPICARTSUB, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPICARTSUB));
/*
 * Compute colour and key used in splitting the communicator.
 */
	colour = key = 0;
	colfactor = keyfactor = 1;
	ndim = 0;
	allfalse = 0;

	i = comm->c_topo_ndims - 1;
	d = comm->c_topo_dims + i;
	c = comm->c_topo_coords + i;
	r = remdims + i;

	for (; i >= 0; --i, --d, --c, --r) {
		dim = (*d > 0) ? *d : -(*d);

		if (*r == 0) {
			colour += colfactor * (*c);
			colfactor *= dim;
		} else {
			++ndim;
			key += keyfactor * (*c);
			keyfactor *= dim;
		}
	}
/*
 * Special case: if all of remdims were false, we need to make 
 * a cartesian communicator with just ourselves in it (you can't
 * have a communicator unless you're in it).
 */
	if (ndim == 0) {
	  MPI_Comm_rank(comm, &colour);
	  ndim = 1;
	  allfalse = 1;
	}
/*
 * Split the communicator.
 */
	errcode = MPI_Comm_split(comm, colour, key, pnewcomm);
	if (errcode != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPICARTSUB, errcode));
	}
/*
 * Fill the communicator with topology information. 
 */
	newcomm = *pnewcomm;
	if (newcomm != MPI_COMM_NULL) {
		newcomm->c_topo_type = MPI_CART;
		newcomm->c_topo_nprocs = keyfactor;
		newcomm->c_topo_ndims = ndim;

		newcomm->c_topo_dims = (int *)
				malloc((unsigned) 2 * ndim * sizeof(int));
		if (newcomm->c_topo_dims == 0) {
			return(lam_errfunc(comm, BLKMPICARTSUB,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
		newcomm->c_topo_coords = newcomm->c_topo_dims + ndim;

		if (!allfalse) {
		  p = newcomm->c_topo_dims;
		  d = comm->c_topo_dims;
		  r = remdims;
		  for (i = 0; i < comm->c_topo_ndims; ++i, ++d, ++r) {
		    if (*r) {
		      *p++ = *d;
		    }
		  }
		} else {
		  newcomm->c_topo_dims[0] = 1;
		}
/*
 * Compute the caller's coordinates.
 */
		errcode = MPI_Comm_rank(newcomm, &rank);
		if (errcode != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICARTSUB, errcode));
		}

		errcode = MPI_Cart_coords(newcomm, rank,
					ndim, newcomm->c_topo_coords);
		if (errcode != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPICARTSUB, errcode));
		}
	}

	LAM_TRACE(lam_tr_cffend(BLKMPICARTSUB, -1, comm, 0, 0));

	lam_resetfunc(BLKMPICARTSUB);
	return(MPI_SUCCESS);
}
