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
 * $Id: attrget.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- get attribute value
 *	Accepts:	- communicator
 *			- attribute key
 *			- attribute value (out)
 *			- found flag (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@C

MPI_Attr_get - Retrieves communicator attribute value by key

Input Parameters:
+ comm - communicator to which attribute is attached (handle) 
- key - key value (integer) 

Output Parameters:
+ value - attribute value, unless 'found' = false 
- found - true if an attribute value was extracted; false if no
  attribute is associated with the key

.N Wrapper MPI_Comm_get_attr

.N ACK
@*/
int MPI_Attr_get(MPI_Comm comm, int key, void *value, int *found)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIATTRGET);

	err = MPI_Comm_get_attr(comm, key, value, found);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIATTRGET, err));
	}

	lam_resetfunc_m(BLKMPIATTRGET);
	return(MPI_SUCCESS);
}
