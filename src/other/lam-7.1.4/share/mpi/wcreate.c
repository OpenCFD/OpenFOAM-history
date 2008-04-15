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
 * $Id: wcreate.c,v 1.6 2003/06/02 02:58:27 jsquyres Exp $
 *
 *	Function:	- create a new window
 *	Accepts:	- window base
 *			- window size
 *			- window displacement unit
 *			- info
 *			- communicator
 *			- new window (out)
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*@
MPI_Win_create - Specifies an window accessible by remote processes

Input Parameters:
+ base -  initial address of window (choice) 
. size -  size of window in bytes (nonnegative integer) 
. disp_unit -  local unit size for displacements, in bytes (positive integer) 
. info -  info argument (handle) 
. comm -  communicator (handle) 
- newwin -  window object returned by the call (handle) 

Notes:

Common choices for disp_unit are 1 (no scaling), and (in C syntax)
sizeof(type), for a window that consists of an array of elements of
type type. The later choice will allow one to use array indices in RMA
calls, and have those scaled correctly to byte displacements, even in
a heterogeneous environment. ( End of advice to users.)  The info
argument provides optimization hints to the runtime about the expected
usage pattern of the window.

The various processes in the group of comm may specify completely
different target windows, in location, size, displacement units and
info arguments. As long as all the get, put and accumulate accesses to
a particular process fit their specific target window this should pose
no problem. The same area in memory may appear in multiple windows,
each associated with a different window object. However, concurrent
communications to distinct, overlapping windows may lead to erroneous
results.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG
.N MPI_ERR_OTHER
.N MPI_ERR_INTERN

.seealso MPI_Put(3), MPI_Get(3), MPI_Accumulate(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Win_get_group(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
  @*/

int
MPI_Win_create(void *base, MPI_Aint size, int disp_unit,
	       MPI_Info info, MPI_Comm comm, MPI_Win *newwin)
{
	struct _attrkey *keystate;		/* key state */
	MPI_Comm	wincomm;		/* window communicator */
	MPI_Win		win;			/* new window */
	int		err;

	lam_initerr_m();
	lam_setfunc(BLKMPIWINCREATE);
/*
 * Check arguments.
 */
	if (comm == MPI_COMM_NULL) {
		return(lam_err_comm(MPI_COMM_NULL,
				MPI_ERR_COMM, 0, "null handle"));
	}

	if (newwin == 0 || size < 0 || disp_unit <= 0) {
		return(lam_err_comm(comm, MPI_ERR_ARG, EINVAL, ""));
	}

	if ((err = MPI_Comm_dup(comm, &wincomm)) != MPI_SUCCESS) {
		return(err);
	}
/*
 * Create the new window.
 */
	win = (MPI_Win) malloc(sizeof(struct _window));
	if (win == 0) {
		return(lam_err_comm(comm, MPI_ERR_OTHER, errno, ""));
	}

	win->w_pstate = (int *) calloc(comm->c_group->g_nprocs, sizeof(int));
	if (win->w_pstate == 0) {
		return(lam_err_comm(comm, MPI_ERR_OTHER, errno, ""));
	}

	win->w_comm = wincomm;
	win->w_base = base;
	win->w_size = size;
	win->w_disp_unit = disp_unit;
	win->w_errhdl = MPI_ERRORS_ARE_FATAL;
	win->w_flags = LAM_WFNOOUT;
	win->w_f77handle = -1;
	win->w_keys = 0;
	win->w_ngroup = 0;
	win->w_ncomplete = 0;
	win->w_nfence = comm->c_group->g_nprocs;
	win->w_req = MPI_REQUEST_NULL;
/*
 * Set the pre-defined attributes.  Toggle pre-defined state since these
 * are marked as pre-defined in MPI_Init().
 */
	if ((keystate = lam_getattr(MPI_WIN_BASE)) == 0) {
		return(lam_errfunc(comm,
			BLKMPIWINCREATE, lam_mkerr(MPI_ERR_INTERN, EINVAL)));
	}
	keystate->ak_flags &= ~LAM_PREDEF;

	err = MPI_Win_set_attr(win, MPI_WIN_BASE, win->w_base);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCREATE, err));
	}
	keystate->ak_flags |= LAM_PREDEF;

	if ((keystate = lam_getattr(MPI_WIN_SIZE)) == 0) {
		return(lam_errfunc(comm,
			BLKMPIWINCREATE, lam_mkerr(MPI_ERR_INTERN, EINVAL)));
	}
	keystate->ak_flags &= ~LAM_PREDEF;

	err = MPI_Win_set_attr(win, MPI_WIN_SIZE, 
			       (void *) LAM_INT_TO_VOID_P win->w_size);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCREATE, err));
	}
	keystate->ak_flags |= LAM_PREDEF;

	if ((keystate = lam_getattr(MPI_WIN_DISP_UNIT)) == 0) {
		return(lam_errfunc(comm,
			BLKMPIWINCREATE, lam_mkerr(MPI_ERR_INTERN, EINVAL)));
	}
	keystate->ak_flags &= ~LAM_PREDEF;

	err = MPI_Win_set_attr(win, MPI_WIN_DISP_UNIT,
				(void *) LAM_INT_TO_VOID_P win->w_disp_unit);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCREATE, err));
	}
	keystate->ak_flags |= LAM_PREDEF;
/*
 * Create and link in the background protocol header receive quest.
 */
	err = _mpi_req_build(&win->w_header, 1, lam_osdhdr_type, 
			     MPI_ANY_SOURCE, LAM_OSD_HEADER, win->w_comm, 
			     LAM_RQIRECV, &win->w_req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCREATE, err));
	}

	win->w_req->rq_marks |= LAM_RQFPERSIST;
	win->w_req->rq_hdlr = lam_osd_handler;
	win->w_req->rq_extra = win;

	err = _mpi_req_start(win->w_req);
	if (err != MPI_SUCCESS) {
		return(lam_errfunc(comm, BLKMPIWINCREATE, err));
	}

	_mpi_req_add(win->w_req);

	wincomm->c_window = win;
	*newwin = win;

	/* Must advance in a non-blocking fashion here soas to allow
           the RPI's to process the start of the receive */

	_mpi_req_blkclr_m();
	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) 
	  return(err);

	lam_resetfunc_m(BLKMPIWINCREATE);
	return(MPI_SUCCESS);
}
