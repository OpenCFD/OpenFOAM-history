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
 * $Id: keyfree.c,v 6.6 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- free attribute key
 *	Accepts:	- key value (in-out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>

/*@

MPI_Keyval_free - Frees attribute key for communicator cache attribute

Input Parameter:
. keyval - Frees the integer key value (integer) 

.N Wrapper MPI_Comm_create_keyval

.N ACK
@*/
int MPI_Keyval_free(int *key)
{
	int		err;
	
	lam_initerr();
	lam_setfunc(BLKMPIKEYFREE);

	err = MPI_Comm_free_keyval(key);

	if (err != MPI_SUCCESS) {
	    return(lam_errfunc(MPI_COMM_WORLD, BLKMPIKEYFREE, err));
	}

	lam_resetfunc(BLKMPIKEYFREE);
	return(MPI_SUCCESS);
}
