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
 * $Id: wfence.c,v 1.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- window fence synchronization
 *	Accepts:	- assertion about the fence
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
MPI_Win_fence - Synchronizes one-sided communications on a window

Input Parameters:
+ assertion -  program assertion (integer) 
- win -  window object (handle) 

Notes:

The MPI call MPI_WIN_FENCE(assert, win) synchronizes RMA calls on
win. The call is collective on the group of win. All RMA operations on
win originating at a given process and started before the fence call
will complete at that process before the fence call returns. They will
be completed at their target before the fence call returns at the
target. RMA operations on win started by a process after the fence
call returns will access their target window only after MPI_WIN_FENCE
has been called by the target process.

Calls to MPI_WIN_FENCE should both precede and follow calls to put,
get or accumulate that are synchronized with fence calls.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_EPOCH
.N MPI_ERR_OTHER

seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Accumulate(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
@*/

int
MPI_Win_fence(int assertion, MPI_Win win)
{
	MPI_Comm	comm;			/* window communicator */
	MPI_Request	*reqs;
	int		np;			/* # processes in window */
	int		fence_msg[1];		/* fence message */
	int		err;			/* error code */
	int		i;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINFENCE);

	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
				   BLKMPIWINFENCE, 
				   lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	comm = win->w_comm;
/*
 * Check if started or posted.
 */
	if (win->w_flags & (LAM_WFSTARTED | LAM_WFPOSTED)) {
		return(lam_errfunc(comm,
				   BLKMPIWINFENCE, 
				   lam_mkerr(MPI_ERR_EPOCH, EINVAL)));
	}
/*
 * Send mesages to all window processes to inform them that this process
 * has entered the fence.  We can pre-allocate the request array in
 * MPI_Win_init() to trade off space for time.
 */
	np = comm->c_group->g_nprocs;

	reqs = (MPI_Request *) malloc(np * sizeof(MPI_Request));
	if (reqs == 0) {
		return(lam_errfunc(comm,
			BLKMPIWINFENCE, lam_mkerr(MPI_ERR_OTHER, errno)));
	}

	fence_msg[0] = LAM_OSD_FENCE;

	for (i = 0; i < np; i++) {
		err = MPI_Isend(fence_msg, 1, MPI_INT,
			i, LAM_OSD_HEADER, comm, reqs + i);

		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPIWINFENCE, err));
		}
	}
/*
 * Loop until everybody has entered the fence.
 */
	_mpi_req_blkclr_m();
	_mpi_req_blkset_m(win->w_req);

	while (win->w_nfence > 0) {

		err = lam_osd_blkadv(win);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPIWINFENCE, err));
		}
	}
/*
 * Complete all outstanding one-sided requests.
 */
	err = lam_osd_complete(win, LAM_RQFOSORIG | LAM_RQFOSTARG);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINFENCE, err));
	}
/*
 * Complete the "entered fence" sends.
 */
	err = MPI_Waitall(np, reqs, MPI_STATUSES_IGNORE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINFENCE, err));
	}
	free((char *) reqs);

	win->w_nfence = np;
	win->w_flags &= ~(LAM_WFACCEPOCH | LAM_WFEXPEPOCH | LAM_WFNOOUT);
/*
 * Barrier to avoid concurrent fences.
 */
	err = MPI_Barrier(win->w_comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIWINFENCE, err));
	}

        lam_resetfunc_m(BLKMPIWINFENCE);
	return(MPI_SUCCESS);
}
