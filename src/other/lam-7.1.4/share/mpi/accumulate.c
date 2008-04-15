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
 * $Id: accumulate.c,v 1.6 2002/12/11 19:15:09 jsquyres Exp $
 *
 *	Function:	- accumulate data in a window
 *	Accepts:	- origin address
 *			- origin count
 *			- origin datatype
 *			- target rank
 *			- target displacement
 *			- target count
 *			- target datatype
 *			- operation
 *			- window
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <typical.h>

/*@
MPI_Accumulate - Accumulates data into a window

Input Parameters:
+ orgaddr - initial address of buffer (choice) 
. orgcnt - number of entries in buffer (nonnegative integer) 
. orgtype - datatype of each buffer entry (handle) 
. rank - rank of target (nonnegative integer) 
. targdisp - displacement from start of window to beginning of target buffer (nonnegative integer) 
. targtype - datatype of each entry in target buffer (handle) 
. op - reduce operation (handle) 
- win - window object (handle) 

Notes:

MPI_PUT is a special case of MPI_ACCUMULATE, with the operation
MPI_REPLACE. Note, however, that MPI_PUT and MPI_ACCUMULATE have
different constraints on concurrent updates. ( End of advice to
users.)

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_TYPE
.N MPI_ERR_EPOCH
.N MPI_ERR_INTERN
.N MPI_ERR_TYPENOTSUP

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB

@*/
int MPI_Accumulate(void *orgaddr, int orgcnt, MPI_Datatype orgtype, int rank,
		   MPI_Aint targdisp, int targcnt, MPI_Datatype targtype, 
		   MPI_Op op, MPI_Win win)
{
	struct _osdhdr	*header;		/* 1-sided protocol header */
	int		err;			/* error code */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIACCUMULATE);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_WIN, EINVAL,
			"null window handle"));
	}

	if (orgtype == MPI_DATATYPE_NULL || targtype == MPI_DATATYPE_NULL) {
		return(lam_err_win(win, MPI_ERR_TYPE, 0, "null handle"));
	}

	if (!(op->op_flags & LAM_PREDEF)) {
		return(lam_err_win(win, MPI_ERR_OP, 0, "must be predefined"));
	}
/*
 * Trivial case.
 */
	if (rank == MPI_PROC_NULL) return(MPI_SUCCESS);

	if (rank < 0 || rank >= win->w_comm->c_group->g_nprocs) {
		return(lam_err_win(win, MPI_ERR_RANK, 0, "out of range"));
	}
/*
 * Check epoch.
 */
	if (!((win->w_flags & LAM_WFACCEPOCH)
			|| win->w_pstate[rank] & LAM_WFSTARTED)) {

		if (win->w_flags & LAM_WFNOOUT) {
			return(lam_err_win(win, MPI_ERR_EPOCH, 0, ""));
		} else {
			win->w_flags |= LAM_WFACCEPOCH | LAM_WFEXPEPOCH;
		}
	}
/*
 * Sender protocol header followed by data.
 */
	header = (struct _osdhdr *) malloc(sizeof(struct _osdhdr));
	LAM_ZERO_ME(*header);
	if (header == 0) {
		return(lam_err_win(win, MPI_ERR_OTHER, errno, "")) ;
	}

	err = lam_osd_fillheader(LAM_OSD_ACCUMULATE, op->op_f77handle,
				targdisp, targcnt, targtype, header);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIACCUMULATE, err));
	}

	err = lam_osd_send(header, 1, lam_osdhdr_type, rank,
			LAM_OSD_HEADER, win->w_comm, TRUE, TRUE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIACCUMULATE, err));
	}

	err = lam_osd_send(orgaddr, orgcnt, orgtype, rank,
			LAM_OSD_ACCUMULATE, win->w_comm, FALSE, FALSE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIACCUMULATE, err));
	}
/*
 * Make progress.
 */
	err = lam_osd_push();
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIACCUMULATE, err));
	}

        lam_resetfunc_m(BLKMPIACCUMULATE);
	return(MPI_SUCCESS);
}
