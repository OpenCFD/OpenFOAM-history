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
 * $Id: tcommit.c,v 6.5 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- commit a datatype
 *	Accepts:	- ptr to datatype
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*
 * external functions
 */
extern int		lam_tr_dtype();

/*@
    MPI_Type_commit - Commits the datatype

Input Parameter:
. dtype - datatype (handle) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_TYPE

.N ACK
@*/
int MPI_Type_commit(MPI_Datatype *dtype)
{
	lam_initerr();
	lam_setfunc(BLKMPITCOMMIT);

	if (dtype == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCOMMIT, 
				   lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
	else if (*dtype == MPI_DATATYPE_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPITCOMMIT, 
				   lam_mkerr(MPI_ERR_TYPE, EINVAL)));
	}
	else {
		if (lam_tr_dtype(*dtype)) {
			return(lam_errfunc(MPI_COMM_WORLD, BLKMPITCOMMIT,
					   lam_mkerr(MPI_ERR_INTERN, errno)));
		}

		(*dtype)->dt_commit = 1;
	}

	lam_resetfunc(BLKMPITCOMMIT);
	return(MPI_SUCCESS);
}
