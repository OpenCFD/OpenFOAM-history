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
 * $Id: tcreatehvector.c,v 1.5 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- create extended hvector derived datatype
 *	Accepts:	- # blocks
 *			- block length
 *			- block stride (in bytes)
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_create_hvector - Create extended hvector derived datatype

Input Parameters:
+ count - Number of blocks (nonnegative integer)
. lengths - Number of elements in each block (nonnegative integer) 
. stride - Number of bytes between start of each block (integer) 
- oldtype - Old datatype (handle)

Output Parameter:
+ newtype - New datatype (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_OTHER

.seealso MPI_Type_create_hindexed(3), MPI_Type_create_struct(3), MPI_Get_address(3)

.N WEB
@*/

int
MPI_Type_create_hvector(int count, int length, MPI_Aint stride,
			MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */

	lam_initerr_m();
	lam_setfunc_m(BLKMPITCREATEHVECTOR);
/*
 * Check the arguments.
 */
	if (oldtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHVECTOR, 
				   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHVECTOR, 
				   lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((length < 0) || (newtype == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHVECTOR, 
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the new datatype.
 */
	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCREATEHVECTOR, 
				   lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	oldtype->dt_refcount++;

	LAM_ZERO_ME(*ntype);
	lam_type_setdefaults(ntype);
	
	ntype->dt_format = LAM_DTHVECTORCREAT;
	ntype->dt_count = count;
	ntype->dt_length = length;
	ntype->dt_stride = stride;
	ntype->dt_dtype = oldtype;

	if (count > 0) {
		lam_dtblock(ntype, oldtype, length, 0);

		if (count > 1 && stride != ntype->dt_size) {
			ntype->dt_flags &= ~LAM_DTNOPACK;
		}

		ntype->dt_nelem *= count;
		ntype->dt_size *= count;

		stride *= count - 1;

		if (stride < 0) {
			ntype->dt_datalow += stride;
			ntype->dt_lower += stride;
		} else {
			ntype->dt_dataup += stride;
			ntype->dt_upper += stride;
		}

		lam_dtalign(ntype);
	}
	else {
		ntype->dt_flags = LAM_DTNOPACK | LAM_DTNOXADJ;
		ntype->dt_size = 0;
	}

	*newtype = ntype;

	lam_resetfunc_m(BLKMPITCREATEHVECTOR);
	return(MPI_SUCCESS);
}
