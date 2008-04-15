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
 * $Id: iccreate.c,v 6.12 2003/02/20 19:45:27 jsquyres Exp $
 *
 *	Function:	- create a new inter-communicator
 *	Accepts:	- local intra-communicator
 *			- local leader
 *			- peer communicator
 *			- peer leader
 *			- tag
 *			- new inter-communicator (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <lam_config.h>
#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>
#include <lam-ssi-coll.h>


/*@

MPI_Intercomm_create - Creates an intercommuncator from two
intracommunicators

Input Paramters:
+ lcomm - Local (intra)communicator
. lleader - Rank in local_comm of leader (often 0)
. pcomm - Remote communicator
. pleader - Rank in peer_comm of remote leader (often 0)
- tag - Message tag to use in constructing intercommunicator; if
multiple 'MPI_Intercomm_creates' are being made, they should use
different tags (more precisely, ensure that the local and remote
leaders are using different tags for each 'MPI_intercomm_create').

Output Parameter:
. newcomm - Created intercommunicator

Notes:

The MPI 1.1 Standard contains two mutually exclusive comments on the
input intracommunicators.  One says that their repective groups must
be disjoint; the other that the leaders can be the same process.
After some discussion by the MPI Forum, it has been decided that the
groups must be disjoint.  Note that the `reason` given for this in the
standard is `not` the reason for this choice; rather, the `other`
operations on intercommunicators (like 'MPI_Intercomm_merge') do not
make sense if the groups are not disjoint.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TAG
.N MPI_ERR_ARG
.N MPI_ERR_EXHAUSTED
.N MPI_ERR_RANK

.seealso: MPI_Intercomm_merge, MPI_Comm_free, MPI_Comm_remote_group, 
          MPI_Comm_remote_size

.N ACK
@*/
int MPI_Intercomm_create(MPI_Comm lcomm, int lleader, 
			 MPI_Comm pcomm, int pleader, int tag, 
			 MPI_Comm *newcomm)
{
	MPI_Status	stat;			/* message status */
	MPI_Group	lgroup;			/* local group */
	MPI_Group	rgroup;			/* remote group */
	int		myrank;			/* my rank */
	int		cid;			/* global context ID */
	int		err;			/* error code */
	int		i;			/* favourite index */
	int		lgsize;			/* local group size */
	int		rgsize;			/* remote group size */
	int		lgbytes;		/* local group GPS size */
	int		rgbytes;		/* remote group GPS size */
	struct _gps	*lprocs;		/* local process GPS */
	struct _gps	*rprocs;		/* remote process GPS */
	struct _proc	*p;			/* favourite pointer */
	struct _proc	**pp;			/* another pointer */
	struct _gps	*pgp;			/* yet another pointer */

	lam_initerr();
	lam_setfunc(BLKMPIICOMMCREATE);
/*
 * Check the arguments.
 */
	if (lcomm == MPI_COMM_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIICOMMCREATE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (LAM_IS_INTER(lcomm)) {
		return(lam_errfunc(lcomm,
			BLKMPIICOMMCREATE, lam_mkerr(MPI_ERR_COMM, EINVAL)));
	}

	if (lcomm->c_group->g_myrank == lleader) {
		if (pcomm == MPI_COMM_NULL) {
			return(lam_errfunc(lcomm, BLKMPIICOMMCREATE, 
					lam_mkerr(MPI_ERR_COMM, EINVAL)));
		}

		rgroup = (LAM_IS_INTER(pcomm))
			? pcomm->c_rgroup : pcomm->c_group;
		
		if ((pleader < 0) || (pleader >= rgroup->g_nprocs)) {
			return(lam_errfunc(pcomm, BLKMPIICOMMCREATE, 
					lam_mkerr(MPI_ERR_RANK, EINVAL)));
		}
	}

	if (tag < 0) {
		return(lam_errfunc(lcomm,
			BLKMPIICOMMCREATE, lam_mkerr(MPI_ERR_TAG, EINVAL)));
	}

	if ((lleader < 0) || (lleader >= lcomm->c_group->g_nprocs)) {
		return(lam_errfunc(lcomm,
			BLKMPIICOMMCREATE, lam_mkerr(MPI_ERR_RANK, EINVAL)));
	}

	if (newcomm == 0) {
		return(lam_errfunc(lcomm,
			BLKMPIICOMMCREATE, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

#if LAM_WANT_IMPI

	/* Remove this when IMPI collectives are implemented */

        if (LAM_IS_IMPI(lcomm)) {
	  return lam_err_comm(lcomm, MPI_ERR_COMM, EINVAL, 
			      "Collectives not yet implemented on IMPI communicators");
	}
	if (LAM_IS_IMPI(pcomm)) {
	  return lam_err_comm(pcomm, MPI_ERR_COMM, ENOSYS, 
			      "Collectives not yet implemented on IMPI communicators");
	}
#endif

	LAM_TRACE(lam_tr_cffstart(BLKMPIICOMMCREATE));

	lgroup = lcomm->c_group;
	lgsize = lgroup->g_nprocs;
	myrank = lgroup->g_myrank;

	/* Collectively decide on a new CID.  All processes in both
	   groups get the new CID */

	err = lam_coll_alloc_inter_cid(lcomm, lleader, pcomm, pleader,
				       tag, BLKMPIICOMMCREATE, &cid);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(lcomm, BLKMPIICOMMCREATE, err));

	/* Get the size of the remote group and distribute it to the
	   local group */

	if (lleader == myrank) {
	  err = MPI_Sendrecv(&lgsize, 1, MPI_INT, pleader, tag,
			     &rgsize, 1, MPI_INT, pleader, tag,
			     pcomm, MPI_STATUS_IGNORE);
	  if (err != MPI_SUCCESS)
	    return(lam_errfunc(lcomm, BLKMPIICOMMCREATE, err));
	}

	err = MPI_Bcast(&rgsize, 1, MPI_INT, lleader, lcomm);
	if (err != MPI_SUCCESS)
	  return(lam_errfunc(lcomm, BLKMPIICOMMCREATE, err));

/*
 * Allocate remote group process GPS array.
 */
	rgbytes = rgsize * sizeof(struct _gps);
	rprocs = (struct _gps *) malloc((unsigned) rgbytes);
	if (rprocs == 0) {
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}
/*
 * Leaders exchange process GPS arrays and broadcast them to their group.
 */
	if (lleader == myrank) {

		lgbytes = lgsize * sizeof(struct _gps);
		lprocs = (struct _gps *) malloc((unsigned) lgbytes);
		if (lprocs == 0) {
			return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
/*
 * Fill local process GPS.
 */
		for (i = 0, pp = lgroup->g_procs; i < lgsize; ++i, ++pp) {
			lprocs[i] = (*pp)->p_gps;
		}

		err = MPI_Sendrecv(lprocs, lgbytes/sizeof(int), MPI_INT,
					pleader, tag, rprocs,
					rgbytes/sizeof(int), MPI_INT,
					pleader, tag, pcomm, &stat);

		free((char *) lprocs);

		if (err != MPI_SUCCESS) {
			free((char *) rprocs);
			return(lam_errfunc(lcomm, BLKMPIICOMMCREATE, err));
		}
	}

	err = MPI_Bcast(rprocs, rgbytes/sizeof(int), MPI_INT, lleader, lcomm);
	if (err != MPI_SUCCESS) {
		free((char *) rprocs);
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE, err));
	}
/*
 * Create the remote group.
 */
	rgroup = (MPI_Group) malloc((unsigned) sizeof(struct _group) +
					(rgsize * sizeof(struct _proc **)));
	if (rgroup == 0) {
		free((char *) rprocs);
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	rgroup->g_nprocs = rgsize;
	rgroup->g_myrank = MPI_UNDEFINED;
	rgroup->g_refcount = 1;
	rgroup->g_f77handle = -1;
	rgroup->g_procs = (struct _proc **)
				((char *) rgroup + sizeof(struct _group));

	for (i = 0, pgp = rprocs; i < rgsize; ++i, ++pgp) {
		if ((p = lam_procadd(pgp)) == 0) {
			return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
					lam_mkerr(MPI_ERR_OTHER, errno)));
		}
		if (!(p->p_mode & LAM_PRPIINIT)) {
			p->p_mode |= LAM_PCLIENT;
		}
		p->p_refcount++;
		rgroup->g_procs[i] = p;
	}

	free((char *) rprocs);
/*
 * Create the new communicator.
 */
	*newcomm = 0;
	if (lam_comm_new(cid, lgroup, rgroup, LAM_CINTER, newcomm)) {
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	lgroup->g_refcount++;
	(*newcomm)->c_errhdl = lcomm->c_errhdl;
	lcomm->c_errhdl->eh_refcount++;

	if (!al_insert(lam_comms, newcomm)) {
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}
	
	if (lam_tr_comm(*newcomm)) {
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
				lam_mkerr(MPI_ERR_INTERN, errno)));
	}
	
	lam_setcid(cid);
/*
 * setup any new processes
 */
	if (RPI_ADDPROCS(rgroup->g_procs, rgroup->g_nprocs)) {
		return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
				lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	/* Let the collective SSI modules battle over who will do
	   collectives on this new communicator.  It'll likely be the
	   same as the one that's on this communicator, but we still
	   give them the option to do something different anyway... */

	if (lam_ssi_coll_base_init_comm(*newcomm) != 0)
	  return(lam_errfunc(lcomm, BLKMPIICOMMCREATE,
			     lam_mkerr(MPI_ERR_INTERN, ENOSYS)));

	LAM_TRACE(lam_tr_cffend(BLKMPIICOMMCREATE, lleader, lcomm, 0, 0));

	lam_resetfunc(BLKMPIICOMMCREATE);
	return(MPI_SUCCESS);
}
