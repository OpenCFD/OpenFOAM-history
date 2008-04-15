/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: icmerge.c,v 6.10 2003/07/17 19:12:38 jsquyres Exp $
 *
 *	Function:	- merge inter-communicator into intra-communicator
 *	Accepts:	- inter-communicator
 *			- high/low flag
 *			- new intra-communicator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <lam_config.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lam-ssi-coll.h>


/*@

MPI_Intercomm_merge - Creates an intracommuncator from an intercommunicator

Input Parameters:
+ comm - Intercommunicator
- high - Used to order the groups of the two intracommunicators within comm
  when creating the new communicator.  

Output Parameter:
. newcomm - Created intracommunicator

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED

.seealso: MPI_Intercomm_create, MPI_Comm_free

.N ACK
@*/
int MPI_Intercomm_merge(MPI_Comm comm, int high, 
			MPI_Comm *newcomm)
{
	MPI_Group	newgroup;		/* new group */
	MPI_Group	g1, g2;			/* 2 groups to merge */
	int		cid;			/* global context ID */
	int		rhigh;			/* remote high value */
	int		err;			/* error code */
	int             lower;                  /* used only during merging
                                                 * during coll init for an
						 * intercomm 
						 */
	int             rsize, rank, i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIICOMMMERGE);
/*
 * Check the arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIICOMMMERGE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTRA(comm)) {
		return(lam_errfunc(comm,
			BLKMPIICOMMMERGE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (newcomm == 0) {
		return(lam_errfunc(comm,
			BLKMPIICOMMMERGE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	LAM_TRACE(lam_tr_cffstart(BLKMPIICOMMMERGE));

#if LAM_WANT_IMPI

	/* Remove this when IMPI collectives are implemented */

        if (LAM_IS_IMPI(comm)) {
	  return lam_err_comm(comm, MPI_ERR_COMM, ENOSYS, 
			      "Collectives not yet implemented on IMPI communicators");
	}
#endif

	/* Collectively decide on a new CID.  All processes in both
	   groups get the new CID */

	err = lam_coll_alloc_inter_cid(comm, 0, comm, 0, MPI_MERGE_TAG, 
				       BLKMPIICOMMMERGE, &cid);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(comm, BLKMPIICOMMMERGE, err));

	/* Send high value to all the remote processes */
	MPI_Comm_remote_size(comm, &rsize);
	MPI_Comm_rank(comm, &rank);
	lower = (lam_proccmp(comm->c_group->g_procs[0],
			     comm->c_rgroup->g_procs[0]) < 0) ? 1 : 0;
	
	if (lower) {
	  if (rank == 0) {
	    for (i = 0; i < rsize; ++i) {
	      MPI_Send(&high, 1, MPI_INT, i, BLKMPIICOMMMERGE, comm);
	    }
	  }
	  MPI_Recv(&rhigh, 1, MPI_INT, 0, BLKMPIICOMMMERGE, comm, 
		   MPI_STATUS_IGNORE);
	} else {
	  MPI_Recv(&rhigh, 1, MPI_INT, 0, BLKMPIICOMMMERGE, comm,
		   MPI_STATUS_IGNORE);
	  if (rank == 0) {
	    for (i = 0; i < rsize; ++i) {
	      MPI_Send(&high, 1, MPI_INT, i, BLKMPIICOMMMERGE, comm);
	    }
	  }
	}

/*
 * All procs know the "high" for local and remote groups and the context ID.
 * Create the properly ordered union group.  In case of equal high values,
 * the group that has the leader with the "lowest process" goes first.
 */
	if (high && (!rhigh)) {
		g1 = comm->c_rgroup;
		g2 = comm->c_group;
	} else if ((!high) && rhigh) {
		g1 = comm->c_group;
		g2 = comm->c_rgroup;
	} else if (lam_proccmp(comm->c_group->g_procs[0],
			       comm->c_rgroup->g_procs[0]) < 0) {
		g1 = comm->c_group;
		g2 = comm->c_rgroup;
	} else {
		g1 = comm->c_rgroup;
		g2 = comm->c_group;
	}

	err = MPI_Group_union(g1, g2, &newgroup);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIICOMMMERGE, err));
	}
/*
 * Create the new communicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, newgroup, MPI_GROUP_NULL, 0, newcomm)) {
		return(lam_errfunc(comm, BLKMPIICOMMMERGE,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	(*newcomm)->c_errhdl = comm->c_errhdl;
	comm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		return(lam_errfunc(comm, BLKMPIICOMMMERGE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	if (lam_tr_comm(*newcomm)) {
		return(lam_errfunc(comm, BLKMPIICOMMMERGE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}

	lam_setcid(cid);

	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  It'll likely be the
	   same as the one that's on this communicator, but we still
	   give them the option to do something different anyway... */

	if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
	  return(lam_errfunc(comm, BLKMPIICOMMMERGE,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPIICOMMMERGE, -1, comm, 0, 0));

	lam_resetfunc(BLKMPIICOMMMERGE);
	return(MPI_SUCCESS);
}
