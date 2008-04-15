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
 * $Id: crgroup.c,v 6.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get remote group (create a new group)
 *	Accepts:	- inter-communicator
 *			- ptr to group
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Comm_remote_group - Accesses the remote group associated with 
                        the given inter-communicator

Input Parameter:
. comm - Communicator (must be intercommunicator)

Output Parameter:
. pgroup - remote group of communicator

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *pgroup)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMRGROUP);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMRGROUP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTRA(comm)) {
		return(lam_errfunc(comm,
			BLKMPICOMMRGROUP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (pgroup == 0) {
		return(lam_errfunc(comm,
			BLKMPICOMMRGROUP, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pgroup = comm->c_rgroup;
	comm->c_rgroup->g_refcount++;

	lam_resetfunc(BLKMPICOMMRGROUP);
  	return(MPI_SUCCESS);
}
