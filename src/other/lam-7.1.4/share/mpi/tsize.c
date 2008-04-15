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
 * $Id: tsize.c,v 6.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- return size of datatype
 *	Accepts:	- datatype
 *			- ptr to size (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_size - Return the number of bytes occupied by entries
                    in the datatype

Input Parameters:
. dtype - datatype (handle) 

Output Parameter:
. psize - datatype size (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Type_size(MPI_Datatype dtype, int *psize)
{
	lam_initerr();
	lam_setfunc(BLKMPITSIZE);

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITSIZE, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
	else if (psize == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITSIZE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
	else {
		*psize = dtype->dt_size;
	}

	lam_resetfunc(BLKMPITSIZE);
	return(MPI_SUCCESS);
}
