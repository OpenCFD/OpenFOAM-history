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
 * $Id: address.c,v 6.5 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- return address of location
 *	Accepts:	- location
 *			- ptr to address (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
MPI_Address - Gets the address of a location in memory  

Input Parameters:
. loc - location in caller memory (choice) 

Output Parameter:
. paddr - address of location (handle)

.N Wrapper MPI_Get_address

.N ACK
@*/
int MPI_Address(void *loc, MPI_Aint *paddr)
{
	lam_initerr();
	lam_setfunc(BLKMPIADDRESS);

	if (paddr == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIADDRESS, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*paddr = ((MPI_Aint) loc) - ((MPI_Aint) MPI_BOTTOM);

	lam_resetfunc(BLKMPIADDRESS);
	return(MPI_SUCCESS);
}
