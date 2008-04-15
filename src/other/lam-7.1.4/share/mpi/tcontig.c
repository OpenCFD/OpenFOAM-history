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
 * $Id: tcontig.c,v 6.8 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- create contiguous derived datatype
 *	Accepts:	- # elements
 *			- element datatype
 *			- ptr new datatype
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
    MPI_Type_contiguous - Creates a contiguous datatype

Input Parameters:
+ count - replication count (nonnegative integer) 
- oldtype - old datatype (handle) 

Output Parameter:
. newtype - new datatype (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE
.N MPI_ERR_COUNT
.N MPI_ERR_EXHAUSTED

.N ACK
@*/
int MPI_Type_contiguous(int count, MPI_Datatype oldtype, 
			MPI_Datatype *newtype)
{
	MPI_Datatype	ntype;			/* new datatype */

	lam_initerr();
	lam_setfunc(BLKMPITCONTIG);
/*
 * Check the arguments.
 */
	if (oldtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCONTIG, lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}

	if (count < 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCONTIG, lam_mkerr(MPI_ERR_COUNT, EINVAL)));
	}

	if (newtype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCONTIG, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Create the new datatype.
 */
	ntype = (MPI_Datatype) malloc(sizeof(struct _dtype));
	if (ntype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITCONTIG, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	LAM_ZERO_ME(*ntype);
	lam_type_setdefaults(ntype);

	ntype->dt_format = LAM_DTCONTIG;
	ntype->dt_count = count;
	ntype->dt_dtype = oldtype;
	oldtype->dt_refcount++;

	lam_dtblock(ntype, oldtype, count, 0);
	lam_dtalign(ntype);

	*newtype = ntype;

	lam_resetfunc(BLKMPITCONTIG);
	return(MPI_SUCCESS);
}
