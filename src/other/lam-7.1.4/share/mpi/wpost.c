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
 * $Id: wpost.c,v 1.4 2003/08/22 02:33:18 brbarret Exp $
 *
 *	Function:	- post a window
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

/*@
MPI_Win_post - Starts an RMA exposure epoch for a local window

Input Parameters:
+ group -  group of origin processes (handle) 
. assertion -  program assertion (integer) 
- win -  window object (handle) 

Notes: 

Starts an RMA exposure epoch for the local window associated with
win. Only processes in group should access the window with RMA calls
on win during this epoch. Each process in group must issue a matching
call to MPI_WIN_START. MPI_WIN_POST does not block.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_GROUP
.N MPI_ERR_EPOCH

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
  @*/

int MPI_Win_post(MPI_Group group, int assertion, MPI_Win win)
{
	int		rank;			/* rank in window */
	int		err;
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINPOST);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINPOST, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	if (group == MPI_GROUP_NULL || group == MPI_GROUP_EMPTY) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINPOST, lam_mkerr(MPI_ERR_GROUP, EINVAL)));

	}
/*
 * Check epoch.
 */
	if (win->w_flags & (LAM_WFEXPEPOCH | LAM_WFPOSTED)) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINPOST, lam_mkerr(MPI_ERR_EPOCH, EINVAL)));
	}
/*
 * Send "i-post-u" messages.
 */
	win->w_flags |= LAM_WFPOSTED | LAM_WFNOOUT;
	win->w_ncomplete = group->g_nprocs;

	for (i = 0; i < group->g_nprocs; i++) {
		rank = lam_mpi_rank_in_group(win->w_comm->c_group,
						group->g_procs[i]);

		if (rank == MPI_UNDEFINED) {
			return(lam_errfunc(win->w_comm,
					   BLKMPIWINPOST, 
					   lam_mkerr(MPI_ERR_GROUP, EINVAL)));
		}

		win->w_pstate[rank] |= LAM_WFPOSTED;

		err = lam_osd_send(0, 0, MPI_BYTE, rank,
				LAM_OSD_POST, win->w_comm, TRUE, FALSE);

		if (err != MPI_SUCCESS) {
			return(lam_errfunc(win->w_comm, BLKMPIWINPOST, err));
		}
	}

        lam_resetfunc_m(BLKMPIWINPOST);
	return(MPI_SUCCESS);
}
