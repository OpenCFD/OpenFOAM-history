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
 * $Id: thindex.c,v 6.7 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create hindexed derived datatype
 *	Accepts:	- # blocks
 *			- block length array
 *			- block displacement array (in bytes)
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */


#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Type_hindexed - Creates an indexed datatype with offsets in bytes

Input Parameters:
+ count - number of blocks -- also number of entries in indices and blocklens
. lengths - number of elements in each block (array of nonnegative integers) 
. disps - byte displacement of each block (array of MPI_Aint) 
- oldtype - old datatype (handle) 

Output Parameter:
. newtype - new datatype (handle) 

.N Wrapper MPI_Type_created_hindexed

.N ACK
@*/
int MPI_Type_hindexed(int count, int *lengths, 
		      MPI_Aint *disps, MPI_Datatype oldtype, 
		      MPI_Datatype *newtype)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITHINDEX);

	err = MPI_Type_create_hindexed(count,
				lengths, disps, oldtype, newtype);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITHINDEX, err));
	}

	(*newtype)->dt_format = LAM_DTHINDEXED;

	lam_resetfunc_m(BLKMPITHINDEX);
	return(MPI_SUCCESS);
}
