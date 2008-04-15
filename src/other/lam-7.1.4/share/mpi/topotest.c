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
 * $Id: topotest.c,v 6.5 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- get topology assigned to communicator
 *	Accepts:	- communicator
 *			- ptr topology type
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Topo_test - Determines the type of topology (if any) associated with a 
                communicator

Input Parameter:
. comm - communicator (handle) 

Output Parameter:
. ptopo - topology type of communicator 'comm' (choice).

Notes:

Upon successful completion, 'ptopo' will either be 'MPI_GRAPH',
'MPI_CART', or 'MPI_UNDEFINED'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso: MPI_Graph_create, MPI_Cart_create

.N ACK
@*/
int MPI_Topo_test(MPI_Comm comm, int *ptopo)
{
	lam_initerr();
	lam_setfunc(BLKMPITOPOTEST);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITOPOTEST, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (ptopo == 0) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPITOPOTEST, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*ptopo = comm->c_topo_type;

	lam_resetfunc(BLKMPITOPOTEST);
	return(MPI_SUCCESS);
}
