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
 *	$Id: wfree.c,v 1.4 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- free window
 *	Accepts:	- window (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*@
MPI_Win_free - Frees a window object

Input Parameter:
+ win -  window object (handle) 

Notes: 

Frees the window object win and returns a null handle (equal to
MPI_WIN_NULL). This is a collective call executed by all processes in
the group associated with win. MPI_WIN_FREE(win) can be invoked by a
process only after it has completed its involvement in RMA
communications on window win: i.e., the process has called
MPI_WIN_FENCE, or called MPI_WIN_WAIT to match a previous call to
MPI_WIN_POST or called MPI_WIN_COMPLETE to match a previous call to
MPI_WIN_START or called MPI_WIN_UNLOCK to match a previous call to
MPI_WIN_LOCK. When the call returns, the window memory can be freed.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_EPOCH

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Accumulate(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
  @*/
int
MPI_Win_free(MPI_Win *win)
{
	MPI_Win		w;			/* window being freed */
	struct _attrkey *keystate;		/* key state */
	struct _attr	*p;			/* attribute */
	int		key;
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINFREE);

	if (win == 0 || *win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINFREE, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	w = *win;
/*
 * Make sure all processes are participating.
 */
	err = MPI_Barrier(w->w_comm);
	if (err != MPI_SUCCESS) {
	    return(lam_errfunc(w->w_comm, BLKMPIWINFREE, err));
	}
/*
 * Destroy the background request.
 */
	if (w->w_req->rq_state == LAM_RQSACTIVE) {
		return(lam_errfunc(w->w_comm, BLKMPIWINFREE,
				lam_mkerr(MPI_ERR_EPOCH, EINVAL)));
	} else {
		_mpi_req_rem_m(w->w_req);
		err = _mpi_req_destroy(&w->w_req);
		if (err != MPI_SUCCESS) {
			return(lam_errfunc(w->w_comm, BLKMPIWINFREE, err));
		}
	}
/*
 * Free attributes and keys which are not pre-defined.
 */
	if (w->w_keys) {
		p = (struct _attr *) ah_top(w->w_keys);
		while (p) {
			key = p->a_key;
			keystate = lam_getattr(key);

			err = MPI_Win_delete_attr(w, key);
			if (err != MPI_SUCCESS) {
				return(lam_errfunc(w->w_comm,
						BLKMPIWINFREE, err));
			}

			if (!(keystate->ak_flags & LAM_PREDEF)) {
				err = MPI_Win_free_keyval(&key);
				if (err != MPI_SUCCESS) {
					return(lam_errfunc(w->w_comm,
							BLKMPIWINFREE, err));
				}
			}

			p = (struct _attr *) ah_next(w->w_keys, (void *) p);
		}
		ah_free(w->w_keys);
	}
/*
 * Remove the fortran handle if the user called MPI_Win_f2c on this
 * handle
 */
	if (w->w_f77handle >= 0)
	  lam_F_free_hdl(w->w_f77handle);

/*
 * Free the underlying communicator.
 */
	err = MPI_Comm_free(&w->w_comm);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPIWINFREE, err));
	}

	free((char *) w->w_pstate);
	free((char *) w);
	*win = MPI_WIN_NULL;

        lam_resetfunc_m(BLKMPIWINFREE);
	return(MPI_SUCCESS);
}
