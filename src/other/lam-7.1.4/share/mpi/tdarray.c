/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: tdarray.c,v 1.6 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- create distributed array derived datatype
 *	Accepts:	- size of process group
 *			- rank in process group
 *			- number of array dimensions
 *			- array of global sizes
 *			- array of distributions
 *			- array of distribution arguments
 *			- size of process grid in each dimension
 *			- storage order
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 *
 *	Notes:		- this code is derived from code in the ROMIO
 *			  distibution authored by Rajeev Thakur
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <typical.h>

static int		createblock(int *gsizes, int dim, int ndims, int nprocs, 
                                    int rank, int darg, int order, MPI_Aint extent, 
                                    MPI_Datatype oldtype, MPI_Datatype *newtype, 
                                    MPI_Aint *offset);
static int		createcyclic(int *gsizes, int dim, int ndims, int nprocs, 
                                     int rank, int darg, int order, MPI_Aint extent, 
                                     MPI_Datatype oldtype, MPI_Datatype *newtype, 
                                     MPI_Aint *offset);

/*@
    MPI_Type_create_darray - create distributed array derived datatype

Input Parameters:
+ size - Size of process group (positive integer)
. rank - rank in process group (nonnegative integer)
. ndmis - Number of array dimensions (positive integer)
. gsizes - Array of global sizes (array of positive integers)
. distribs - Array of distributions (array of state)
. dargs - Array of distibution arguments (array of positive integers)
. psizes - Size of process grid in each dimension (array of positive integers)
. order - Storage order (state)
- oldtype - Old datatype (handle)

Output Parameter:
+ newtype - New datatype (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_ARG
.N MPI_ERR_OTHER

.N WEB
@*/

int
MPI_Type_create_darray(int size, int rank, int ndims, int *gsizes,
		       int *distribs, int *dargs, int *psizes, 
		       int order, MPI_Datatype oldtype,
		       MPI_Datatype *newtype)
{
    MPI_Datatype	types[1];
    MPI_Datatype	ntype;			/* new datatype */
    MPI_Datatype	intype;			/* datatype passed in */
    int			blklens[1];
    MPI_Aint		disps[1];
    MPI_Aint		*offsets;
    MPI_Aint		extent;
    MPI_Aint		ub;
    int			*coords;
    int			err = MPI_SUCCESS;
    int			pi;
    int			n, r, s;
    int			i;

    lam_initerr_m();
    lam_setfunc_m(BLKMPITCREATEDARRAY);
/*
 * Check the arguments.
 */
    if (size <= 0 || rank < 0 || ndims <= 0 || newtype == 0) {
	return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCREATEDARRAY, lam_mkerr(MPI_ERR_ARG, EINVAL)));
    }

    if (oldtype == MPI_DATATYPE_NULL) {
	return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCREATEDARRAY, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
    }

    for (i = 0, pi = 1; i < ndims; ++i) {

	if (gsizes[i] <= 0 || psizes[i] <= 0
			|| distribs[i] < MPI_DISTRIBUTE_BLOCK
			|| distribs[i] > MPI_DISTRIBUTE_NONE
			|| (dargs[i] <= 0 &&
			    (dargs[i] != MPI_DISTRIBUTE_DFLT_DARG
			     && distribs[i] != MPI_DISTRIBUTE_NONE))) {

	    return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCREATEDARRAY, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (distribs[i] == MPI_DISTRIBUTE_BLOCK
		&& dargs[i] != MPI_DISTRIBUTE_DFLT_DARG) {

	    if (dargs[i] * psizes[i] < gsizes[i]) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATEDARRAY,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	    }
	} else if (distribs[i] == MPI_DISTRIBUTE_NONE) {
	    if (psizes[i] != 1) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATEDARRAY,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
	    }
	}

	pi *= psizes[i];
    }

    if (pi != size) {
	return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATEDARRAY,
					lam_mkerr(MPI_ERR_ARG, EINVAL)));
    }
/*
 * Calculate position in Cartesian grid as MPI would (row-major
 * ordering)
 */
    coords = (int *) malloc(ndims * sizeof(int));
    offsets = (MPI_Aint *) malloc(ndims * sizeof(MPI_Aint));

    if (coords == 0 || offsets == 0) {
	return(lam_errfunc(MPI_COMM_WORLD,
			   BLKMPITCREATEDARRAY, 
			   lam_mkerr(MPI_ERR_OTHER, EINVAL)));
    }

    for (s = size, r = rank, i = 0; i < ndims; i++) {
	s /= psizes[i];
	coords[i] = r / s;
	r = r % s;
    }

    MPI_Type_extent(oldtype, &extent);
    intype = oldtype;

    if (order == MPI_ORDER_FORTRAN) {
/*
 * Dimension 0 changes fastest.
 */
	for (i = 0; i < ndims; i++) {
	    switch (distribs[i]) {

	    case MPI_DISTRIBUTE_BLOCK:
		err = createblock(gsizes, i, ndims, psizes[i], coords[i],
					dargs[i], order, extent, oldtype,
					&ntype, offsets + i);
		break;

	    case MPI_DISTRIBUTE_CYCLIC:
		err = createcyclic(gsizes, i, ndims, psizes[i], coords[i],
					dargs[i], order, extent, oldtype,
					&ntype, offsets + i);
		break;

	    case MPI_DISTRIBUTE_NONE:
/*
 * Treat it as a block distribution on 1 process.
 */
		err = createblock(gsizes, i, ndims, 1, 0,
					MPI_DISTRIBUTE_DFLT_DARG, 
		    			order, extent, oldtype,
					&ntype, offsets + i);
		break;
	    }

	    if (err != MPI_SUCCESS) {
		return(err);
	    }

	    if (i > 0 && !(oldtype->dt_flags & LAM_PREDEF)) {
		MPI_Type_free(&oldtype);
	    }

	    oldtype = ntype;
	}
/*
 * Add displacement and UB.
 */
	disps[0] = offsets[0];
	for (n = 1, i = 1; i < ndims; i++) {
	    n *= gsizes[i-1];
	    disps[0] += n * offsets[i];
	}
    }
    else if (order == MPI_ORDER_C) {
/*
 * Dimension ndims-1 changes fastest.
 */
	for (i = ndims - 1; i >= 0; i--) {
	    switch(distribs[i]) {

	    case MPI_DISTRIBUTE_BLOCK:
		err = createblock(gsizes, i, ndims, psizes[i], coords[i],
					dargs[i], order, extent, oldtype,
					&ntype, offsets + i);
		break;

	    case MPI_DISTRIBUTE_CYCLIC:
		err = createcyclic(gsizes, i, ndims, psizes[i], coords[i],
					dargs[i], order, extent, oldtype,
					&ntype, offsets + i);
		break;

	    case MPI_DISTRIBUTE_NONE:
/*
 * Treat it as a block distribution on 1 process.
 */
		err = createblock(gsizes, i, ndims, psizes[i], coords[i],
					MPI_DISTRIBUTE_DFLT_DARG, order, extent,
					oldtype, &ntype, offsets + i);
		break;
	    }

	    if (err != MPI_SUCCESS) {
		return(err);
	    }

	    if (i > 0 && !(oldtype->dt_flags & LAM_PREDEF)) {
		MPI_Type_free(&oldtype);
	    }

	    oldtype = ntype;
	}
/*
 * Add displacement and UB.
 */
	disps[0] = offsets[ndims-1];
	for (n = 1, i = ndims - 2; i >= 0; i--) {
	    n *= gsizes[i+1];
	    disps[0] += n * offsets[i];
	}
    }
    else {
/*
 * Invalid order.
 */
	return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCREATEDARRAY, lam_mkerr(MPI_ERR_ARG, EINVAL)));
    }

    for (ub = extent, i = 0; i < ndims; i++) {
	ub *= gsizes[i];
    }

    disps[0] *= extent;
    blklens[0] = 1;
    types[0] = ntype;

    err = MPI_Type_create_struct(1, blklens, disps, types, &ntype);
    if (err != MPI_SUCCESS) {
	return(err);
    }

    MPI_Type_free(&types[0]);

    ntype->dt_flags &= ~(LAM_DTHASLB | LAM_DTHASUB);
    ntype->dt_lower = 0;
    ntype->dt_upper = ub;
/*
 * Set the no extent adjustment flag if the upper and lower bounds match
 * exactly the upper and lower limits of the data.
 */
    if (ntype->dt_upper == ntype->dt_dataup
		&& ntype->dt_lower == ntype->dt_datalow) {
	ntype->dt_flags |= LAM_DTNOXADJ;
    } else {
	ntype->dt_flags &= ~LAM_DTNOXADJ;
    }

    ntype->dt_format = LAM_DTDARRAY;

    free(offsets);
    free(coords);
/*
 * Record user arguments.
 */
    ntype->dt_dtype = intype;
    intype->dt_refcount++;

    ntype->dt_uargs = (int *) malloc((4 * ndims + 4) * sizeof(int));
    if (ntype->dt_uargs == 0) {
	return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATEDARRAY,
				lam_mkerr(MPI_ERR_OTHER, errno)));
    }

    ntype->dt_uargs[0] = size;
    ntype->dt_uargs[1] = rank;
    ntype->dt_uargs[2] = ndims;

    ntype->dt_uargs[4*ndims + 3] = order;

    for (i = 0; i < ndims; i++) {
	ntype->dt_uargs[i + 3] = gsizes[i];
	ntype->dt_uargs[ndims + i + 3] = distribs[i];
	ntype->dt_uargs[2*ndims + i + 3] = dargs[i];
	ntype->dt_uargs[3*ndims + i + 3] = psizes[i];
    }

    *newtype = ntype;

    lam_resetfunc_m(BLKMPITCREATEDARRAY);
    return(MPI_SUCCESS);
}


static int
createblock(int *gsizes, int dim, int ndims, int nprocs, int rank, int darg,
            int order, MPI_Aint extent, MPI_Datatype oldtype, MPI_Datatype *newtype, 
            MPI_Aint *offset)
{
    int			blksize;
    int			size;
    int			err;
    int			i;

    if (darg == MPI_DISTRIBUTE_DFLT_DARG) {
	blksize = (gsizes[dim] + nprocs - 1) / nprocs;
    } else {
	blksize = darg;
    }

    size = gsizes[dim] - blksize * rank;
    if (size > blksize) {
	size = blksize;
    }
    if (size < 0) {
	size = 0;
    }

    if (order == MPI_ORDER_FORTRAN) {
	if (dim == 0)
	    err = MPI_Type_contiguous(size, oldtype, newtype);
	else {
	    for (i = 0; i < dim; i++) {
		extent *= gsizes[i];
	    }

	    err = MPI_Type_create_hvector(size, 1, extent, oldtype, newtype);
	}
    }
    else {
	if (dim == ndims - 1)
	    err = MPI_Type_contiguous(size, oldtype, newtype);
	else {
	    for (i = ndims - 1; i > dim; i--) {
		extent *= gsizes[i];
	    }

	    err = MPI_Type_create_hvector(size, 1, extent, oldtype, newtype);
	}
    }
/*
 * In terms of number of elements of type oldtype in this dimension.
 */
    if (size == 0) {
	*offset = 0;
    } else {
	*offset = blksize * rank;
    }

    return(err);
}


static int
createcyclic(int *gsizes, int dim, int ndims, int nprocs, int rank, int darg,
             int order, MPI_Aint extent, MPI_Datatype oldtype, MPI_Datatype *newtype, 
             MPI_Aint *offset)
{
    int			blksize;
    int			start;
    int			end;
    int			size;
    int			count;
    int			rem;
    int			err = MPI_SUCCESS;
    int			i;
    MPI_Aint extent_save = extent;
    MPI_Datatype temp;

    if (darg == MPI_DISTRIBUTE_DFLT_DARG) {
	blksize = 1;
    } else {
	blksize = darg;
    }
/*
 * Calculate the number of blocks of the block-cyclic distribution that
 * belong to this process.
 */
    start = rank * blksize;
    end = gsizes[dim] - 1;

    if (end < start) {
	size = 0;
    } else {
	rem = (end - start + 1) % (nprocs * blksize);
	size = ((end - start + 1) / (nprocs * blksize)) * blksize;
	size += (rem < blksize ? rem : blksize);
    }

    count = size / blksize;
    rem = size % blksize;

    extent *= nprocs * blksize;
    if (order == MPI_ORDER_FORTRAN) {
	for (i = 0; i < dim; i++) {
	    extent *= gsizes[i];
	}
    } else {
	for (i = ndims - 1; i > dim; i--) {
	    extent *= gsizes[i];
	}
    }

    err = MPI_Type_create_hvector(count, blksize, extent, oldtype, newtype);
    if (err != MPI_SUCCESS) {
	return(err);
    }

    if (rem) {
	MPI_Datatype	types[2];
	MPI_Aint	disps[2];
	int		blklens[2];
/*
 * If the last block is of size less than blksize, include it separately
 * using MPI_Type_struct.
 */
	types[0] = *newtype;
	types[1] = oldtype;
	disps[0] = 0;
	disps[1] = count * extent;
	blklens[0] = 1;
	blklens[1] = rem;

	err = MPI_Type_struct(2, blklens, disps, types, newtype);
	if (err != MPI_SUCCESS) {
	    return(err);
	}

	MPI_Type_free(&types[0]);
    }

    /* Need to add an upper bound and lower bound */

    extent = extent_save * gsizes[dim];
    temp = *newtype;
    MPI_Type_create_resized(temp, 0, extent, newtype);
    MPI_Type_free(&temp);

/*
 * In terms of number of elements of type oldtype in this dimension.
 */
    if (size == 0) {
	*offset = 0;
    } else {
	*offset = rank * blksize;
    }

    return(err);
}
