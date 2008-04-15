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
 * $Id: tlb.c,v 6.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- return lower bound of datatype
 *	Accepts:	- datatype
 *			- lower bound (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_lb - Returns the lower-bound of a datatype

Input Parameters:
. dtype - datatype (handle) 

Output Parameter:
. lb - displacement of lower bound from origin, 
                             in bytes (integer) 

.N Wrapper MPI_Type_get_extent

.N ACK
@*/
int MPI_Type_lb(MPI_Datatype dtype, MPI_Aint *lb)
{
	lam_initerr();
	lam_setfunc(BLKMPITLB);

	if (dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITLB, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
	else if (lb == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITLB, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
	else {
		*lb = dtype->dt_lower;
	}

	lam_resetfunc(BLKMPITLB);
	return(MPI_SUCCESS);
}
