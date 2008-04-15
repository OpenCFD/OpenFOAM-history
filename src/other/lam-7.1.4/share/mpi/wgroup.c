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
 * $Id: wgroup.c,v 1.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- get window group
 *	Accepts:	- window
 *			- group (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>

/*@
MPI_Win_get_group - Returns a duplicate of the group of a window

Input Parameters:
+ win - window object (handle) 
- group - group of processes which share access to the window (handle) 

Notes:

MPI_WIN_GET_GROUP returns a duplicate of the group of the communicator
used to create the window. associated with win. The group is returned
in group.

.N fortran
.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_ARG

.seealso MPI_Put(3), MPI_Get(3), MPI_Win_create(3), MPI_Win_start(3), MPI_Win_complete(3), MPI_Win_fence(3), MPI_Win_free(3), MPI_Accumulate(3), MPI_Win_get_group(3), MPI_Win_wait(3)

.N WEB
  @*/

int MPI_Win_get_group(MPI_Win win, MPI_Group *group)
{
	lam_initerr();
	lam_setfunc(BLKMPIWINGETGROUP);

	if (win == MPI_WIN_NULL) {
		return(lam_err_comm(MPI_COMM_WORLD,
		    			MPI_ERR_WIN, 0, "null handle"));
	}

	if (group == 0) {
		return(lam_err_win(win, MPI_ERR_ARG, 0, "null group pointer"));
	}

	*group = win->w_comm->c_group;
	win->w_comm->c_group->g_refcount++;

	lam_resetfunc(BLKMPIWINGETGROUP);
  	return(MPI_SUCCESS);
}
