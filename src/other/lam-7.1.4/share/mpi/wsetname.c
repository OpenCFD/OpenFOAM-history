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
 * $Id: wsetname.c,v 1.5 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- set window name
 *	Accepts:	- window
 *			- window name
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
   MPI_Win_set_name - Set a name on a window

Input Parameters:
+ win - MPI window
- name - Pointer to the name of window

Notes:

This function associates a name with a window, which can be
useful for debugging purposes.  The function 'MPI_Win_get_name' can
be used to retrieve name that was previously set with
'MPI_Win_set_name'.

Names passed in C must include the null terminator, and can be of
maximum length 'MPI_MAX_OBJECT_NAME' (including the null terminator).
Names passed in Fortran can also have a maximum length of
'MPI_MAX_OBJECT_NAME'.

Setting a name on a window that already has a name associated
with it replaces the old name.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_ARG

.seealso MPI_Comm_set_name(3), MPI_Comm_get_name(3), MPI_Type_get_name(3), MPI_Type_set_name(3), MPI_Win_get_name(3)

.N WEB
@*/
int
MPI_Win_set_name(MPI_Win win, char *name)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINSETNAME);
/*
 * Check the arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINSETNAME, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	err = MPI_Comm_set_name(win->w_comm, name);

	lam_resetfunc_m(BLKMPIWINSETNAME);
	return(err);
}
