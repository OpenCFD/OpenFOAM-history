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
 * $Id: keycreate.c,v 6.9 2003/11/23 15:01:23 jsquyres Exp $
 *
 *	Function:	- create an attribute key
 *	Accepts:	- copy function
 *			- delete function
 *			- key value (out)
 *			- extra state info
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>


/*@

MPI_Keyval_create - Generates a new communicator attribute key

Input Parameters:
. cpyfunc - Copy callback function for 'keyval' 
. delfunc - Delete callback function for 'keyval' 
. extra - Extra state for callback functions 

Output Parameter:
. key - key value for future access (integer) 

.N Wrapper MPI_Comm_create_keyval

.N ACK
@*/
int MPI_Keyval_create(MPI_Copy_function *cpyfunc, 
		      MPI_Delete_function *delfunc, 
		      int *key, void *extra)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIKEYCREATE);

	err = lam_keyval_create((lam_void_fn_t) cpyfunc, 
                                (lam_void_fn_t) delfunc, 
				key, extra, LAM_AKCOMM);

	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIKEYCREATE, err));
	}

	lam_resetfunc_m(BLKMPIKEYCREATE);
	return(MPI_SUCCESS);
}
