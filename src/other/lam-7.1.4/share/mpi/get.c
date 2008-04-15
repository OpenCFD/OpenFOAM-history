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
 * $Id: get.c,v 1.5 2002/12/11 19:15:10 jsquyres Exp $
 *
 *	Function:	- get data from a window
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
MPI_Get - One Sided data fetch

Input Parameters:
+ orgaddr - initial address of origin buffer (choice) 
. orgcnt - number of entries in origin buffer (nonnegative integer) 
. orgtype - datatype of each entry in origin buffer (handle) 
. rank -  rank of target (nonnegative integer) 
. targdisp -  displacement from window start to the beginning of the target buffer (nonnegative integer) 
. targcnt -  number of entries in target buffer (nonnegative integer) 
. targtype -  datatype of each entry in target buffer (handle) 
- win -  window object used for communication (handle) 


Notes:

Similar to MPI_PUT, except that the direction of data transfer is
reversed. Data are copied from the target memory to the origin. The
origin_datatype may not specify overlapping entries in the origin
buffer. The target buffer must be contained within the target window,
and the copied data must fit, without truncation, in the origin
buffer.

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

.seealso MPI_Put(3), MPI_Accumulate(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
  @*/

int
MPI_Get(orgaddr, orgcnt, orgtype, rank, targdisp, targcnt, targtype, win)

void			*orgaddr;
int			orgcnt;
MPI_Datatype		orgtype;
int			rank;
MPI_Aint		targdisp;
int			targcnt;
MPI_Datatype		targtype;
MPI_Win			win;

{
	MPI_Comm	comm;			/* underlying communicator */
	struct _osdhdr	*header;		/* 1-sided protocol header */
	int		err;			/* error code */

	lam_initerr_m();
	lam_setfunc_m(BLKMPIGET);
/*
 * Check arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD, MPI_ERR_WIN, EINVAL,
			"null handle"));
	}

	comm = win->w_comm;

	if (orgtype == MPI_DATATYPE_NULL || targtype == MPI_DATATYPE_NULL) {
		return(lam_err_win(win, MPI_ERR_TYPE, 0, "null handle"));
	}
/*
 * Trivial case.
 */
	if (rank == MPI_PROC_NULL) return(MPI_SUCCESS);

	if (rank < 0 || rank >= comm->c_group->g_nprocs) {
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
 * Send get protocol header to target.
 */
	header = (struct _osdhdr *) malloc(sizeof(struct _osdhdr));
	if (header == 0) {
		return(lam_err_win(win, MPI_ERR_OTHER, errno, ""));
	}

	LAM_ZERO_ME(*header);
	err = lam_osd_fillheader(LAM_OSD_GET, 0,
			targdisp, targcnt, targtype, header);
	if (err != MPI_SUCCESS) {
		return(lam_err_win(win, err, 0, ""));
	}

	err = lam_osd_send(header, 1, lam_osdhdr_type, rank,
			LAM_OSD_HEADER, comm, TRUE, TRUE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIGET, err));
	}
/*
 * Receive data from target.
 */
	err = lam_osd_recv(orgaddr, orgcnt, orgtype, rank,
			LAM_OSD_GET, comm, TRUE);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIGET, err));
	}
/*
 * Make progress.
 */
	err = lam_osd_push();
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIGET, err));
	}

        lam_resetfunc_m(BLKMPIGET);
	return(MPI_SUCCESS);
}
