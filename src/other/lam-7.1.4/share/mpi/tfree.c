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
 * $Id: tfree.c,v 6.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- mark a datatype for deallocation
 *	Accepts:	- datatype (inout)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*
 * external functions
 */
extern int		lam_dtype_free();

/*@
    MPI_Type_free - Frees the datatype

Input/Output Parameter:
. dtype - datatype that is freed (handle) 

Predefined types:

The MPI standard states that (in Opaque Objects)

.vb
MPI provides certain predefined opaque objects and predefined, static
handles to these objects. Such objects may not be destroyed.
.ve

Thus, it is an error to free a predefined datatype.  The same section
makes it clear that it is an error to free a null datatype.

Upon successful completion, 'dtype' is set to 'MPI_DATATYPE_NULL'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Type_free(MPI_Datatype *dtype)
{
	int		err;			/* error code */

	lam_initerr();
	lam_setfunc(BLKMPITFREE);
/*
 * Check the arguments.
 */
	if (dtype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITFREE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	if (*dtype == MPI_DATATYPE_NULL || ((*dtype)->dt_flags & LAM_PREDEF)) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITFREE, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
/*
 * If last reference really free the datatype otherwise just null the handle.
 */
	if (--((*dtype)->dt_refcount) == 0) {

		err = lam_type_free(*dtype);
		*dtype = MPI_DATATYPE_NULL;

		if (err != MPI_SUCCESS) {
			return(lam_errfunc(MPI_COMM_WORLD,
						BLKMPITFREE, err));
		}
	} else {
		*dtype = MPI_DATATYPE_NULL;
	}

	lam_resetfunc(BLKMPITFREE);
	return(MPI_SUCCESS);
}
