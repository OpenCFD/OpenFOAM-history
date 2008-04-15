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
 * $Id: thvector.c,v 6.7 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- create hvector derived datatype
 *	Accepts:	- # blocks
 *			- block length
 *			- block stride (in bytes)
 *			- element datatype
 *			- new datatype (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
    MPI_Type_hvector - Creates a vector (strided) datatype with offset in bytes

Input Parameters:
+ count - number of blocks (nonnegative integer) 
. length - number of elements in each block 
(nonnegative integer) 
. stride - number of bytes between start of each block (integer) 
- oldtype - old datatype (handle) 

Output Parameter:
. newtype - new datatype (handle) 

.N Wrapper MPI_Type_create_hvector

.N ACK
@*/
int MPI_Type_hvector(int count, int length, 
		     MPI_Aint stride, MPI_Datatype oldtype, 
		     MPI_Datatype *newtype)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPITHVECTOR);

	err = MPI_Type_create_hvector(count, length, stride, oldtype, newtype);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITHVECTOR, err));
	}

	(*newtype)->dt_format = LAM_DTHVECTOR;

	lam_resetfunc_m(BLKMPITHVECTOR);
	return(MPI_SUCCESS);
}
