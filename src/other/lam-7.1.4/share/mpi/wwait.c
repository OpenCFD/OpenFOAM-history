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
 * $Id: wwait.c,v 1.4 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- one sided window wait
 *	Accepts:	- window
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*@
MPI_Win_wait - Completes an RMA exposure epoch

Input Parameter:
+ win -  window object (handle) 

Notes:

Completes an RMA exposure epoch started by a call to MPI_WIN_POST on
win. This call matches calls to MPI_WIN_COMPLETE(win) issued by each
of the origin processes that were granted access to the window during
this epoch. The call to MPI_WIN_WAIT will block until all matching
calls to MPI_WIN_COMPLETE have occurred. This guarantees that all
these origin processes have completed their RMA accesses to the local
window. When the call returns, all these RMA accesses will have
completed at the target window.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_EPOCH

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Accumulate(3)

.N WEB
  @*/


int MPI_Win_wait(MPI_Win win)
{
	MPI_Comm	comm;
	int		err;			/* error code */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINWAIT);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINWAIT, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	comm = win->w_comm;

	if (!(win->w_flags & LAM_WFPOSTED)) {
		return(lam_errfunc(comm,
			BLKMPIWINWAIT, lam_mkerr(MPI_ERR_EPOCH, EINVAL)));
	}
/*
 * Loop until everybody has completed.
 */
	_mpi_req_blkclr_m();
	_mpi_req_blkset_m(win->w_req);

	while (win->w_ncomplete > 0) {

		err = lam_osd_blkadv(win);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(comm, BLKMPIWINWAIT, err));
		}
	}
/*
 * Complete all remote one-sided requests in the window.
 */
	err = lam_osd_complete(win, LAM_RQFOSTARG);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINWAIT, err));
	}

	win->w_flags &= ~LAM_WFPOSTED;

        lam_resetfunc_m(BLKMPIWINWAIT);
	return(MPI_SUCCESS);
}
