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
 * $Id: ccreate.c,v 6.9 2003/02/20 19:45:27 jsquyres Exp $
 *
 *	Function:	- create a new communicator
 *	Accepts:	- old communicator
 *			- new group
 *			- ptr new communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <terror.h>
#include <lam-ssi-coll.h>


/*@

MPI_Comm_create - Creates a new communicator

Input Parameters:
+ comm - communicator (handle) 
- group - group, which is a subset of the group of 'comm'  (handle) 

Output Parameter:
. newcomm - new communicator (handle) 

Notes:

When a communicator is no longer being used, it should be freed with
'MPI_Comm_free'.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_GROUP
.N MPI_ERR_EXHAUSTED
.N MPI_ERR_INTERN

.seealso: MPI_Comm_free

.N ACK
@*/
int MPI_Comm_create(MPI_Comm comm, MPI_Group group, 
		    MPI_Comm *newcomm)
{
	int		cid;			/* global max context ID */
	int		rank;			/* process rank */
	int		err;			/* error code */

	lam_initerr();
	lam_setfunc(BLKMPICOMMCREATE);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(comm)) {
		return(lam_errfunc(comm, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (group == MPI_GROUP_NULL) {
		return(lam_errfunc(comm, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}

	if (newcomm == 0) {
		return(lam_errfunc(comm, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

#if LAM_WANT_IMPI
	/* Remove this when IMPI collectives are implemented */
        if (LAM_IS_IMPI(comm)) {
	  return lam_err_comm(comm, MPI_ERR_COMM, ENOSYS, 
			      "Collectives not yet implemented on IMPI communicators");
	}
#endif

/*
 * Set up tracing.
 */
	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMCREATE));

	/* Collectively decide what the new CID will be.  Processes
	   not in the group participate but do not affect the final
	   CID. */

	rank = group->g_myrank;
	err = lam_coll_alloc_intra_cid(comm, (rank == MPI_UNDEFINED) ? 0 : 1,
				       BLKMPICOMMCREATE, &cid);
	if (err != MPI_SUCCESS) {
		LAM_TRACE(lam_tr_cffend(BLKMPICOMMCREATE, -1, comm, 0, 0));
		lam_resetfunc(BLKMPICOMMCREATE);
		return(lam_errfunc(comm, BLKMPICOMMCREATE, err));
	}

	if (rank == MPI_UNDEFINED) {
		*newcomm = MPI_COMM_NULL;
		LAM_TRACE(lam_tr_cffend(BLKMPICOMMCREATE, -1, comm, 0, 0));
		lam_resetfunc(BLKMPICOMMCREATE);
		return(MPI_SUCCESS);
	}
/*
 * Create the new communicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, group, MPI_GROUP_NULL, 0, newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	group->g_refcount++;
	(*newcomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}
	
	if (lam_tr_comm(*newcomm)) {
		return(lam_errfunc(comm, BLKMPICOMMCREATE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

 	lam_setcid(cid);

	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  It'll likely be the
	   same as the one that's on this communicator, but we still
	   give them the option to do something different anyway... */

	if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
	  return(lam_errfunc(comm, BLKMPICOMMCREATE,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPICOMMCREATE, -1, comm, 0, 0));

	lam_resetfunc(BLKMPICOMMCREATE);
	return(MPI_SUCCESS);
}
