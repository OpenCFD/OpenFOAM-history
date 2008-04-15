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
 * $Id: cgetparent.c,v 1.3 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get parent communicator
 *	Accepts:	- communicator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
   MPI_Comm_get_parent - Get the parent communicator of a spawned MPI rank

Output Parameter:
. comm - parent communicator

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso MPI_Comm_spawn(3), MPI_Comm_spawn_multiple(3)

.N WEB
@*/
int
MPI_Comm_get_parent(MPI_Comm *parent)
{
	lam_initerr_m();
	lam_setfunc_m(BLKMPICOMMGETPARENT);
/*
 * Check the argument.
 */
	if (parent == 0) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, EINVAL,
			"null pointer"));
	}

	*parent = lam_comm_parent;

	lam_resetfunc_m(BLKMPICOMMGETPARENT);
	return(MPI_SUCCESS);
}
