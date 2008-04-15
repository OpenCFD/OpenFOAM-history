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
 * $Id: tstruct.c,v 6.7 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create struct derived datatype
 *	Accepts:	- # blocks
 *			- block length array
 *			- block displacement array (in bytes)
 *			- block element datatype array
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_struct - Creates a struct datatype

Input Parameters:
+ count - number of blocks (integer) -- also number of 
entries in arrays array_of_types ,
array_of_displacements  and array_of_blocklengths  
. blocklens - number of elements in each block (array)
. indices - byte displacement of each block (array)
- old_types - type of elements in each block (array 
of handles to datatype objects) 

Output Parameter:
. newtype - new datatype (handle) 

.N Wrapper MPI_Type_create_struct

.N ACK
@*/
int MPI_Type_struct(int count, int *lengths, 
		    MPI_Aint *disps, MPI_Datatype *oldtypes, 
		    MPI_Datatype *newtype)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITSTRUCT);

	err = MPI_Type_create_struct(count, lengths, disps, oldtypes, newtype);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITSTRUCT, err));
	}

	(*newtype)->dt_format = LAM_DTSTRUCT;

	lam_resetfunc_m(BLKMPITSTRUCT);
	return(MPI_SUCCESS);
}
