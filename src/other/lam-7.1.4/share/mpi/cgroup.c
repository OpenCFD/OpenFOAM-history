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
 * $Id: cgroup.c,v 6.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get communicator group (create a new group)
 *	Accepts:	- communicator
 *			- ptr to group
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@

MPI_Comm_group - Accesses the group associated with given communicator

Input Parameter:
. comm - Communicator

Output Parameter:
. pgroup - Group in communicator

Using 'MPI_COMM_NULL' with 'MPI_Comm_group':

It is an error to use 'MPI_COMM_NULL' as one of the arguments to
'MPI_Comm_group'.  The relevant sections of the MPI standard are

(2.4.1 Opaque Objects)
A null handle argument is an erroneous 'IN' argument in MPI calls,
unless an exception is explicitly stated in the text that defines the
function.

(5.3.2. Group Constructors)
<no text in 'MPI_COMM_GROUP' allowing a null handle>

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.N ACK
@*/
int MPI_Comm_group(MPI_Comm comm, MPI_Group *pgroup)
{
	lam_initerr();
	lam_setfunc(BLKMPICOMMGROUP);

	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPICOMMGROUP, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (pgroup == 0) {
		return(lam_errfunc(comm,
			BLKMPICOMMGROUP, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	*pgroup = comm->c_group;
	comm->c_group->g_refcount++;

	lam_resetfunc(BLKMPICOMMGROUP);
  	return(MPI_SUCCESS);
}
