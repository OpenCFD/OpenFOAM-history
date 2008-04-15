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
 * $Id: attrput.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- put value in attribute
 *			- assign attribute to communicator
 *	Accepts:	- communicator
 *			- attribute key
 *			- attribute value
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@

MPI_Attr_put - Stores a communicator attribute value associated with a key

Input Parameters:
+ comm - communicator to which attribute will be attached (handle) 
. key - key value, as returned by  'MPI_COMM_CREATE_KEYVAL' (integer) 
- value - attribute value 

.N Wrapper MPI_Comm_set_attr

.N ACK
@*/
int MPI_Attr_put(MPI_Comm comm, int key, void *value)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIATTRPUT);

	err = MPI_Comm_set_attr(comm, key, value);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIATTRPUT, err));
	}

	lam_resetfunc_m(BLKMPIATTRPUT);
	return(MPI_SUCCESS);
}
