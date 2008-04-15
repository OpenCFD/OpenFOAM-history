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
 * $Id: tvector.c,v 6.7 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create vector derived datatype
 *	Accepts:	- # blocks
 *			- block length
 *			- block stride
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
    MPI_Type_vector - Creates a vector (strided) datatype

Input Parameters:
+ count - number of blocks (nonnegative integer) 
. length - number of elements in each block 
(nonnegative integer) 
. stride - number of elements between start of each block (integer) 
- oldtype - old datatype (handle) 

Output Parameter:
. newtype - new datatype (handle) 

Notes:

Once a datatype is no longer being used, it should be freed with
'MPI_Type_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.seealso MPI_Type_free

.N ACK
@*/
int MPI_Type_vector(int count, int length, int stride, 
		    MPI_Datatype oldtype, MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */
/*
 * Check the arguments.
 */
	if (oldtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITVECTOR, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITVECTOR, lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if ((length < 0) || (newtype == 0)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITVECTOR, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the new datatype.
 */
	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITVECTOR, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	oldtype->dt_refcount++;

	lam_type_setdefaults(ntype);
	
	ntype->dt_format = LAM_DTVECTOR;
	ntype->dt_count = count;
	ntype->dt_length = length;
	ntype->dt_stride = stride;
	ntype->dt_dtype = oldtype;

	if (count > 0) {
		lam_dtblock(ntype, oldtype, length, 0);

		if (count > 1 && (stride != length
				|| !(oldtype->dt_flags & LAM_DTNOXADJ))) {
			ntype->dt_flags &= ~LAM_DTNOPACK;
		}

		ntype->dt_nelem *= count;
		ntype->dt_size *= count;

		stride *= (oldtype->dt_upper - oldtype->dt_lower) * (count - 1);

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
		ntype->dt_flags = (LAM_DTNOPACK | LAM_DTNOXADJ);
		ntype->dt_size = 0;
	}

	*newtype = ntype;

	lam_resetfunc(BLKMPITVECTOR);
	return(MPI_SUCCESS);
}
