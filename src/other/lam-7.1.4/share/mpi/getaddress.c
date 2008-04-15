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
 * $Id: getaddress.c,v 1.4 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get address of location
 *	Accepts:	- location
 *			- address (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
   MPI_Get_address - obtain the address of a location in caller memory

Input Parameter:
. loc - location in caller memory

Output Parameter:
. addr - address of location

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso MPI_Type_create_hvector(3), MPI_Type_create_hindexed(3), MPI_Type_create_struct(3)

.N WEB
@*/
int
MPI_Get_address(void *loc, MPI_Aint *addr)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPIGETADDRESS);

	if (addr == 0) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, EINVAL,
			"null pointer"));
	}

	*addr = ((MPI_Aint) loc) - ((MPI_Aint) MPI_BOTTOM);

	lam_resetfunc_m(BLKMPIGETADDRESS);
	return(MPI_SUCCESS);
}
