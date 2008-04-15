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
 * $Id: attrdel.c,v 6.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- delete attribute
 *			- detach attribute from communicator
 *	Accepts:	- communicator
 *			- attribute key
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>


/*@

MPI_Attr_delete - Deletes communicator attribute value associated with a key

Input Parameters:
+ comm - communicator to which attribute is attached (handle) 
- keyval - The key value of the deleted attribute (integer) 

.N Wrapper MPI_Comm_delete_attr

.N ACK
@*/
int MPI_Attr_delete(MPI_Comm comm, int key)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIATTRDEL);

	err = MPI_Comm_delete_attr(comm, key);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIATTRDEL, err));
	}

	lam_resetfunc_m(BLKMPIATTRDEL);
	return(MPI_SUCCESS);
}
