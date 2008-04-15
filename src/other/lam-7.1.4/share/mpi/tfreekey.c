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
 * $Id: tfreekey.c,v 1.4 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- free datatype attribute key
 *	Accepts:	- key value (in-out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
    MPI_Type_free_keyval - Free datatype attribuet key

Input Parameter:
+ key - Key value (integer)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_TYPE
.N MPI_ERR_KEYVAL
.N MPI_ERR_ARG

.seealso MPI_Type_create_keyval(3), MPI_Type_get_attr(3), MPI_Type_set_attr(3), MPI_Type_delete_attr(3)

.N WEB
@*/


int
MPI_Type_free_keyval(int *key)
{
	int		err;
	
	lam_initerr();
	lam_setfunc(BLKMPITFREEKEYVAL);
/*
 * Check the arguments.
 */
	if (key == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITFREEKEYVAL, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}
/*
 * Free the key, if it is valid.
 */
	if ((err = lam_freekey(*key, LAM_AKTYPE)) != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPITFREEKEYVAL, err));
	}

	*key = MPI_KEYVAL_INVALID;

	lam_resetfunc(BLKMPITFREEKEYVAL);
	return(MPI_SUCCESS);
}
