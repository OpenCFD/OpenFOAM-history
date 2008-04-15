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
 * $Id: wgetname.c,v 1.6 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- get window name
 *	Accepts:	- window
 *			- window name (out)
 *			- window name length (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
   MPI_Win_get_name - Get the name associated with a window

Input Parameters:
. type - MPI window

Output Parameters:
+ name - Name of the window
- length - Integer length of the resulting name

Notes:

This function returns the name of a window that was previously set
with 'MPI_Win_set_name'.  If a name was not previously associated
with a window, the returned string will be empty and 'length' will
be 0.  The retuned name will be a maximum of 'MPI_MAX_OBJECT_NAME'
characters long.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_ARG

.seealso MPI_Comm_get_name(3), MPI_Comm_set_name(3), MPI_Type_get_name(3), MPI_Type_set_name(3), MPI_Win_set_name(3)

.N WEB
@*/
int
MPI_Win_get_name(MPI_Win win, char *name, int *length)
{
	int		err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINGETNAME);
/*
 * Check the arguments.
 */
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINGETNAME, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	err = MPI_Comm_get_name(win->w_comm, name, length);

	lam_resetfunc_m(BLKMPIWINGETNAME);
	return(err);
}
