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
 * $Id: wstart.c,v 1.5 2003/08/22 02:33:18 brbarret Exp $
 *
 *	Function:	- start a window
 *	Accepts:	- group
 *			- assertion
 *			- window
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*@
MPI_Win_start - Starts an RMA access epoch

Input Parameters:
+ group -  group of origin processes (handle) 
. assertion -  program assertion (integer) 
- win -  window object (handle) 

Notes:

Starts an RMA access epoch for win. RMA calls issued on win during
this epoch must access only windows at processes in group. Each
process in group must issue a matching call to MPI_WIN_POST. RMA
accesses to each target window will be delayed, if necessary, until
the target process executed the matching call to
MPI_WIN_POST. MPI_WIN_START is allowed to block until the
corresponding MPI_WIN_POST calls are executed, but is not required to.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_GROUP
.N MPI_ERR_OTHER
.N MPI_ERR_EPOCH

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Accumulate(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
@*/
int MPI_Win_start(MPI_Group group, int assertion, MPI_Win win)
{
	MPI_Request	*reqs;
	int		rank;			/* rank in window */
	int		err;			/* error code */
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINSTART);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINSTART, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	if (group == MPI_GROUP_NULL || group == MPI_GROUP_EMPTY) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINSTART, lam_mkerr(MPI_ERR_GROUP, EINVAL)));
	}
/*
 * Check epoch.
 */
	if (win->w_flags & (LAM_WFACCEPOCH | LAM_WFSTARTED)) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINSTART, lam_mkerr(MPI_ERR_EPOCH, EINVAL)));
	}
/*
 * Wait to be posted by the group.
 */
	win->w_flags |= LAM_WFSTARTED | LAM_WFNOOUT;
	win->w_ngroup = group->g_nprocs;

	reqs = (MPI_Request *) malloc(group->g_nprocs * sizeof(MPI_Request));
	if (reqs == 0) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINSTART, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	for (i = 0; i < group->g_nprocs; i++) {
		rank = lam_mpi_rank_in_group(win->w_comm->c_group,
						group->g_procs[i]);

		if (rank == MPI_UNDEFINED) {
			return(lam_errfunc(win->w_comm,
					   BLKMPIWINSTART, 
					   lam_mkerr(MPI_ERR_GROUP, EINVAL)));
		}

		win->w_pstate[rank] |= LAM_WFSTARTED;

		err = MPI_Recv_init(0, 0, MPI_BYTE, rank,
				LAM_OSD_POST, win->w_comm, reqs + i);

		if (err != MPI_SUCCESS) {
			return(lam_errfunc(win->w_comm, BLKMPIWINSTART, err));
		}
	}

	err = MPI_Startall(group->g_nprocs, reqs);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIWINSTART, err));
	}

	err = MPI_Waitall(group->g_nprocs, reqs, MPI_STATUSES_IGNORE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIWINSTART, err));
	}

	free((char *) reqs);

        lam_resetfunc_m(BLKMPIWINSTART);
	return(MPI_SUCCESS);
}
