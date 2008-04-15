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
 * $Id: tsubarray.c,v 1.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create local array derived datatype
 *	Accepts:	- number of array dimensions
 *			- array of sizes
 *			- array of sub sizes
 *			- array of starting coordinates
 *			- storage order
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 *
 *	Notes:		- this code is derived from code in the ROMIO
 *			  distibution authored by Rajeev Thakur.
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*
 * Local function
 */
static void freetype(MPI_Datatype *type);

/*@
   MPI_Type_create_subarray - Create local array derived datatype

Input Parameters:
+ ndims - Number of array dimensions
. sizes - Number of elements of type oldtype in each dimension of the full array
. subsizes - Number of elements of type oldtype in each dimension of the subarray
. starts - Starting coordinates of the subarray in each dimension
. order - array storage order flag 
- oldtype - array element datatype

Output Parameter:
+ newtype - new datatype

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE
.N MPI_ERR_OTHER

.N WEB
@*/

int
MPI_Type_create_subarray(int ndims, int *sizes, int *subsizes,
			int *starts, int order, 
			 MPI_Datatype oldtype, MPI_Datatype *newtype)
{
    MPI_Datatype	types[1];
    MPI_Aint		disps[1];
    int			blklens[1];
    MPI_Aint		ub;
    MPI_Datatype	ntype;
    MPI_Datatype	tmptype;
    MPI_Aint		extent;
    int			size;
    int			err;
    int			i;

    lam_initerr_m();
    lam_setfunc_m(BLKMPITCREATESUBARRAY);
/*
 * Check the arguments.
 */
    if (ndims <= 0 || sizes == 0
		|| subsizes == 0 || starts == 0 || newtype == 0) {
	return(lam_errfunc(MPI_COMM_WORLD,
			   BLKMPITCREATESUBARRAY, 
			   lam_mkerr(MPI_ERR_ARG, EINVAL)));
    }

    if (oldtype == MPI_DATATYPE_NULL) {
	return(lam_errfunc(MPI_COMM_WORLD,
			   BLKMPITCREATESUBARRAY, 
			   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
    }

    for (i = 0; i < ndims; i++) {
	if (sizes[i] <= 0 || subsizes[i] <= 0 || starts[i] < 0) {
	    return(lam_errfunc(MPI_COMM_WORLD,
			       BLKMPITCREATESUBARRAY, 
			       lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
    }

    MPI_Type_extent(oldtype, &extent);

    if (order == MPI_ORDER_FORTRAN) {
/*
 * Dimension 0 changes fastest.
 */
	if (ndims == 1) {
	    err = MPI_Type_contiguous(subsizes[0], oldtype, &ntype);
	    if (err != MPI_SUCCESS) {
		return(err);
	    }
	} else {
	    err = MPI_Type_vector(subsizes[1], subsizes[0],
			    		sizes[0], oldtype, &ntype);
	    if (err != MPI_SUCCESS) {
		return(err);
	    }

	    size = sizes[0] * extent;

	    for (i = 2; i < ndims; i++) {
		size *= sizes[i-1];
		
		err = MPI_Type_create_hvector(subsizes[i], 1, size,
		    		ntype, &tmptype);
		if (err != MPI_SUCCESS) {
		    return(err);
		}

		freetype(&ntype);
		ntype = tmptype;
	    }
	}
/*
 * Add displacement and UB.
 */
	disps[0] = starts[0];
	size = 1;
	for (i = 1; i < ndims; i++) {
	    size *= sizes[i-1];
	    disps[0] += size * starts[i];
	}
    }
    else if (order == MPI_ORDER_C) {
/*
 * Dimension ndims-1 changes fastest.
 */
	if (ndims == 1) {
	    err = MPI_Type_contiguous(subsizes[0], oldtype, &ntype);
	    if (err != MPI_SUCCESS) {
		return(err);
	    }
	} else {
	    err = MPI_Type_vector(subsizes[ndims-2], subsizes[ndims-1],
			    		sizes[ndims-1], oldtype, &ntype);
	    if (err != MPI_SUCCESS) {
		return(err);
	    }

	    size = sizes[ndims-1] * extent;

	    for (i = ndims - 3; i >= 0; i--) {
		size *= sizes[i+1];

		err = MPI_Type_create_hvector(subsizes[i], 1, size,
		    		ntype, &tmptype);
		if (err != MPI_SUCCESS) {
		    return(err);
		}

		freetype(&ntype);
		ntype = tmptype;
	    }
	}
/*
 * Add displacement and UB.
 */
	disps[0] = starts[ndims-1];
	size = 1;
	for (i = ndims - 2; i >= 0; i--) {
	    size *= sizes[i+1];
	    disps[0] += size * starts[i];
	}
    }
    else {
	return(lam_errfunc(MPI_COMM_WORLD,
			   BLKMPITCREATESUBARRAY, 
			   lam_mkerr(MPI_ERR_ARG, EINVAL)));
    }

    for (ub = extent, i = 0; i < ndims; i++) {
	ub *= sizes[i];
    }

    disps[0] *= extent;
    blklens[0] = 1;
    types[0] = ntype;

    err = MPI_Type_create_struct(1, blklens, disps, types, &ntype);
    if (err != MPI_SUCCESS) {
	return(err);
    }

    freetype(&types[0]);

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

    ntype->dt_format = LAM_DTSUBARRAY;
/*
 * Record user arguments.
 */
    ntype->dt_dtype = oldtype;
    oldtype->dt_refcount++;

    ntype->dt_uargs = (int *) malloc((3*ndims + 2) * sizeof(int));
    if (ntype->dt_uargs == 0) {
	return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESUBARRAY,
	    			lam_mkerr(MPI_ERR_OTHER, errno)));
    }

    ntype->dt_uargs[0] = ndims;
    ntype->dt_uargs[3*ndims + 1] = order;

    for (i = 0; i < ndims; i++) {
	ntype->dt_uargs[i + 1] = sizes[i];
	ntype->dt_uargs[ndims + i + 1] = subsizes[i];
	ntype->dt_uargs[2*ndims + i + 1] = starts[i];
    }

    *newtype = ntype;

    lam_resetfunc_m(BLKMPITCREATESUBARRAY);
    return(MPI_SUCCESS);
}

static void
freetype(MPI_Datatype *type)
{
    if (!((*type)->dt_flags & LAM_PREDEF)) {
	MPI_Type_free(type);
    }
}
