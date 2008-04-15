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
 * $Id: crsize.c,v 6.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- return the size of the remote group
 *	Accepts:	- inter-communicator
 *			- ptr to size (returned value)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Comm_remote_size - Determines the size of the remote group
associated with an inter-communictor

Input Parameter:
. comm - communicator (handle - must be intercommunicator)

Output Parameter:
. psize - number of processes in the group of 'comm'  (integer) 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_remote_size(MPI_Comm comm, int *psize)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMRSIZE);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMRSIZE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTRA(comm)) {
		return(lam_errfunc(comm,
			BLKMPICOMMRSIZE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (psize == 0) {
		return(lam_errfunc(comm,
			BLKMPICOMMRSIZE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*psize = comm->c_rgroup->g_nprocs;

	lam_resetfunc(BLKMPICOMMRSIZE);
	return(MPI_SUCCESS);
}
