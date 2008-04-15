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
 * $Id: put.c,v 1.6 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- put data into a window
 *	Accepts:	- origin address
 *			- origin count
 *			- origin datatype
 *			- target rank
 *			- target displacement
 *			- target count
 *			- target datatype
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
MPI_Put - One Sided data placement operation

Input Parameters:
+ orgaddr -  initial address of origin buffer (choice) 
. orgcnt -  number of entries in origin buffer (nonnegative integer) 
. orgtype -  datatype of each entry in origin buffer (handle) 
. rank -  rank of target (nonnegative integer) 
. targdisp -  displacement from start of window to target buffer (nonnegative integer) 
. targcnt -  number of entries in target buffer (nonnegative integer) 
. targtype -  datatype of each entry in target buffer (handle) 
- win -  window object used for communication (handle) 

Notes:

The target_datatype argument is a handle to a datatype object that is
defined at the origin process, even though it defines a data layout in
the target process memory.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_RANK
.N MPI_ERR_OTHER
.N MPI_ERR_EPOCH
.N MPI_ERR_INTERN
.N MPI_ERR_TYPENOTSUP

.seealso MPI_Accumulate(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
  @*/
int
MPI_Put(void *orgaddr, int orgcnt, MPI_Datatype orgtype, int rank, 
	MPI_Aint targdisp, int targcnt, MPI_Datatype targtype, MPI_Win win)
{
	struct _osdhdr	*header;		/* 1-sided protocol header */
	int		err;			/* error code */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIPUT);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_WIN, EINVAL,
			"null handle"));
	}

	if (orgtype == MPI_DATATYPE_NULL || targtype == MPI_DATATYPE_NULL) {
		return(lam_err_win(win, MPI_ERR_TYPE, 0, "null handle"));
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
		return(lam_err_win(win, MPI_ERR_OTHER, errno,
			"allocating header"));
	}

	err = lam_osd_fillheader(LAM_OSD_PUT, 0,
			targdisp, targcnt, targtype, header);
	if (err != MPI_SUCCESS) {
		return(lam_err_win(win, err, 0, ""));
	}

	err = lam_osd_send(header, 1, lam_osdhdr_type, rank,
			LAM_OSD_HEADER, win->w_comm, TRUE, TRUE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIPUT, err));
	}

	err = lam_osd_send(orgaddr, orgcnt, orgtype, rank,
			LAM_OSD_PUT, win->w_comm, FALSE, FALSE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIPUT, err));
	}
/*
 * Make progress.
 */
	err = lam_osd_push();
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(win->w_comm, BLKMPIPUT, err));
	}

        lam_resetfunc_m(BLKMPIPUT);
	return(MPI_SUCCESS);
}
