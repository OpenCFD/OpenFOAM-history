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
 * $Id: wcreatekey.c,v 1.4 2003/11/23 15:01:23 jsquyres Exp $
 *
 *	Function:	- create a window attribute key
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
MPI_Win_create_keyval - Create a window attribute key

Input Parameters:
+ cpyfunc - copy callback function for win_keyval (function) 
. delfunc - delete callback function for win_keyval (function) 
- extra - extra state for callback functions 

Output Parameter:
+ key - key value for future access (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER
.N MPI_ERR_ARG

.seealso MPI_Win_free_keyval(3), MPI_Win_get_attr(3), MPI_Win_set_attr(3), MPI_Win_delete_attr(3)

.N WEB
  @*/


int MPI_Win_create_keyval(MPI_Win_copy_attr_function *cpyfunc,
			  MPI_Win_delete_attr_function *delfunc,
			  int *key,
			  void *extra)
{
        int 		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINCREATEKEYVAL);

	err = lam_keyval_create((lam_void_fn_t) cpyfunc, 
                                (lam_void_fn_t) delfunc, 
				key, extra, LAM_AKWIN);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWINCREATEKEYVAL, err));
	}

	lam_resetfunc_m(BLKMPIWINCREATEKEYVAL);
	return(MPI_SUCCESS);
}
