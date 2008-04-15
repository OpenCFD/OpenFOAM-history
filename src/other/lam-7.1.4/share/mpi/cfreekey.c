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
 * $Id: cfreekey.c,v 1.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- free communicator attribute key
 *	Accepts:	- key value (in-out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@
    MPI_Comm_free_keyval - Free communicator attribute key

Input Parameter:
+ key - Key value

.N IMPI_MPI2

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_KEYVAL
.N MPI_ERR_ARG

.seealso MPI_Comm_create_keyval(3), MPI_Comm_set_attr(3), MPI_Comm_get_attr(3), MPI_Comm_delete_attr(3)

.N WEB
@*/

int
MPI_Comm_free_keyval(int *key)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMFREEKEYVAL);
/*
 * Check the arguments.
 */
	if (key == 0) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, EINVAL,
			"null pointer"));
	}
/*
 * Free the key, if it is valid.
 */
	if (lam_freekey(*key, LAM_AKCOMM) != MPI_SUCCESS) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_KEYVAL, 
				    EINVAL, ""));
	}

	*key = MPI_KEYVAL_INVALID;

	lam_resetfunc(BLKMPICOMMFREEKEYVAL);
	return(MPI_SUCCESS);
}
