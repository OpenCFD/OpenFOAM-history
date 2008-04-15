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
 * $Id: wcomplete.c,v 1.3 2002/12/11 19:15:14 jsquyres Exp $
 *
 *	Function:	- complete a window
 *	Accepts:	- window
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*@
MPI_Win_complete - Completes an RMA access epoch

Input Parameter:
win -  window object (handle) 

Notes: 

Completes an RMA access epoch on win started by a call to
MPI_WIN_START. All RMA communication calls issued on win during this
epoch will have completed at the origin when the call returns.

MPI_WIN_COMPLETE enforces completion of preceding RMA calls at the
origin, but not at the target. A put or accumulate call may not have
completed at the target when it has completed at the origin.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_OTHER
.N MPI_ERR_EPOCH

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Accumulate(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB

  @*/


int MPI_Win_complete(MPI_Win win)
{
	MPI_Comm	comm;			/* window's communicator */
	MPI_Request	*reqs;			/* completing message reqs */
	MPI_Request	*r;
	int		complete_msg[1];	/* complete message */
	int		err;			/* error code */
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINCOMPLETE);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINCOMPLETE, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	comm = win->w_comm;

	if (!(win->w_flags & LAM_WFSTARTED)) {
		return(lam_errfunc(comm,
			BLKMPIWINCOMPLETE, lam_mkerr(MPI_ERR_EPOCH, EINVAL)));
	}
/*
 * Send "i-am-completing" mesages to all target processes in the 2 party
 * synchronization.  
 */
	reqs = (MPI_Request *) malloc(win->w_ngroup * sizeof(MPI_Request));
	if (reqs == 0) {
		return(lam_errfunc(comm,
			BLKMPIWINCOMPLETE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	complete_msg[0] = LAM_OSD_COMPLETE;

	for (i = 0, r = reqs; i < comm->c_group->g_nprocs; i++) {

		if (win->w_pstate[i] & LAM_WFSTARTED) {
			err = MPI_Send_init(complete_msg, 1, MPI_INT, i,
						LAM_OSD_HEADER, comm, r++);

			if (err != MPI_SUCCESS) {
				return(lam_errfunc(comm,
						BLKMPIWINCOMPLETE, err));
			}

			win->w_pstate[i] &= ~LAM_WFSTARTED;
		}
	}

	err = MPI_Startall(win->w_ngroup, reqs);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIWINCOMPLETE, err));
	}
/*
 * Complete all locally originated one-sided requests in the window.
 */
	err = lam_osd_complete(win, LAM_RQFOSORIG);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCOMPLETE, err));
	}
/*
 * Complete the "i-am-completing" messages.
 */
	err = MPI_Waitall(win->w_ngroup, reqs, MPI_STATUSES_IGNORE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCOMPLETE, err));
	}
	free((char *) reqs);

	win->w_ngroup = 0;
	win->w_flags &= ~LAM_WFSTARTED;

        lam_resetfunc_m(BLKMPIWINCOMPLETE);
	return(MPI_SUCCESS);
}
