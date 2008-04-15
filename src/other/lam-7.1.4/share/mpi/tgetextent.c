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
 * $Id: tgetextent.c,v 1.4 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- get lower bound and extent of datatype
 *	Accepts:	- datatype
 *			- lower bound (out)
 *			- extent (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
   MPI_Type_get_extent - Get lower bound and extent of datatype

Input Parameter:
+ dtype - datatype to get information on

Output Parameters:
+ lb - lower bound of datatype
- extent - size of datatype

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE

.seealso MPI_Type_get_true_extent(3), MPI_Type_extent(3), MPI_Type_create_resized(3)

.N WEB
@*/

int
MPI_Type_get_extent(MPI_Datatype dtype, MPI_Aint *lb, MPI_Aint *extent)
{
	lam_initerr();
	lam_setfunc(BLKMPITGETEXTENT);

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITGETEXTENT, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
	else if (lb == 0 || extent == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITGETEXTENT, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
	else {
		*lb = dtype->dt_lower;
		*extent = dtype->dt_upper - dtype->dt_lower;
	}

	lam_resetfunc(BLKMPITGETEXTENT);
	return(MPI_SUCCESS);
}
