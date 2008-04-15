/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: ccreatekey.c,v 1.5 2003/11/23 15:01:23 jsquyres Exp $
 *
 *	Function:	- create a communicator attribute key
 *	Accepts:	- copy function
 *			- delete function
 *			- key (out)
 *			- extra state info
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
    MPI_Comm_create_keyval - Create a communicator attribute key

Input Parameters:
+ cpyfunc - Copy callback function for comm_keyval
. delfunc - Delete callback function for comm_keyval
- extra - Extra status info

Output Parameter:
+ key - Key value for future access

.N IMPI_MPI2

.N fortran

.N Errors
.N MPI_SUCCESS

.seealso MPI_Comm_free_keyval(3), MPI_Comm_set_attr(3), MPI_Comm_get_attr(3), MPI_Comm_delete_attr(3)

.N WEB
@*/

int
MPI_Comm_create_keyval(MPI_Comm_copy_attr_function *cpyfunc, 
		       MPI_Comm_delete_attr_function *delfunc, 
		       int *key, void *extra)
{
        int 		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMCREATEKEYVAL);

	err = lam_keyval_create((lam_void_fn_t) cpyfunc, 
                                (lam_void_fn_t) delfunc, 
				key, extra, LAM_AKCOMM);

	if (err != MPI_SUCCESS) {
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMCREATEKEYVAL, err));
	}

	lam_resetfunc_m(BLKMPICOMMCREATEKEYVAL);
	return(MPI_SUCCESS);
}
