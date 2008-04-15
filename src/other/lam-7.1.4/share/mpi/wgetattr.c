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
 * $Id: wgetattr.c,v 1.5 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- get window attribute value
 *	Accepts:	- window
 *			- attribute key
 *			- attribute value (out)
 *			- found flag (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
    MPI_Win_get_attr - Get window attribute value

Input Parameters:
+ win - Window to which attribute is attached
- key - key value

Output Parameters:
+ value - attribute value, unless found = false
- found - false if no attribute is associated with the key 

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_KEYVAL

.seealso MPI_Win_create_keyval(3), MPI_Win_free_keyval(3), MPI_Win_set_attr(3), MPI_Win_delete_attr(3)

.N WEB
@*/
int
MPI_Win_get_attr(MPI_Win win, int key, void *value, int *found)
{
        struct _attrkey *pk;
	struct _attr    *p;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINGETATTR);
/*
* Check the arguments.
*/
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINGETATTR, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	if ((value == 0) || (found == 0)) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINGETATTR, lam_mkerr(MPI_ERR_ARG, EINVAL)));
	}

	pk = lam_getattr(key);
	if (pk == 0 || !(pk->ak_flags & LAM_AKWIN)) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINGETATTR, lam_mkerr(MPI_ERR_KEYVAL, EINVAL)));
	}
/*
 * Check if the key is attached to the window.
 */
	p = lam_getkey(win->w_keys, key);
	if (p == 0) {
		*found = 0;
	} else {
		*found = 1;
/*
 * This stuff is very confusing.  Be sure to see MPI-2 4.12.7.
 */
		*((void **) value) = (pk->ak_flags & LAM_LANGF77)
			? (void *) &p->a_value : p->a_value;
	}

	lam_resetfunc_m(BLKMPIWINGETATTR);
	return(MPI_SUCCESS);
}
