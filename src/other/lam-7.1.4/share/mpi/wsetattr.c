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
 * $Id: wsetattr.c,v 1.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- put value in attribute
 *			- assign attribute to window
 *	Accepts:	- window
 *			- attribute key
 *			- attribute value
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
    MPI_Win_set_attr - Set attribute on a window

Input Parameters:
+ win - Window to which the attribute will be attached
. key - Key value
- value - attribute value

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_WIN
.N MPI_ERR_KEYVAL

.seealso MPI_Win_create_keyval(3), MPI_Win_free_keyval(3), MPI_Win_get_attr(3), MPI_Win_delete_attr(3)

.N WEB
@*/
int
MPI_Win_set_attr(MPI_Win win, int key, void* value)
{
	struct _attrkey *p;
	struct _attr    *pk;
	int             err;

	lam_initerr_m();
	lam_setfunc_m(BLKMPIWINSETATTR);
/*
* Check the arguments.
*/
	if (win == MPI_WIN_NULL) {
		return(lam_errfunc(MPI_COMM_WORLD,
			BLKMPIWINSETATTR, lam_mkerr(MPI_ERR_WIN, EINVAL)));
	}

	p = lam_getattr(key);
	if (p == 0 || (p->ak_flags & LAM_PREDEF)
			|| !(p->ak_flags & LAM_AKWIN)) {
		return(lam_errfunc(win->w_comm,
			BLKMPIWINSETATTR, lam_mkerr(MPI_ERR_KEYVAL, EINVAL)));
	}
/*
* If the key is not attached to the window, attach and fill it.
*/
	pk = (struct _attr *) lam_getkey(win->w_keys, key);
	if (pk == 0) {
		if (lam_putkey(&win->w_keys, key)) {
			return(lam_errfunc(win->w_comm, BLKMPIWINSETATTR,
				lam_mkerr(MPI_ERR_OTHER, errno)));
		}

		pk = (struct _attr *) lam_getkey(win->w_keys, key);
	}
/*
 * Old attribute, delete previous value and fill new one.
 */
	else if (((MPI_Win_delete_attr_function *) (p->ak_del))
	    		!= MPI_WIN_NULL_DELETE_FN) {

		if (p->ak_flags & LAM_LANGF77) {
			(*((MPI_F_delete_function *) (p->ak_del)))
				(&win->w_f77handle, &key,
				(int *) &pk->a_value,
				(int *) &p->ak_extra, &err);
		} else {
			err = (*((MPI_Win_delete_attr_function *) (p->ak_del)))
				(win, key, pk->a_value, p->ak_extra);
		}

		if (err != MPI_SUCCESS) {
			return(lam_err_win(win, err, 0,
					"deleting old attribute"));
		}
	}

	pk->a_value = value;

	lam_resetfunc_m(BLKMPIWINSETATTR);
	return(MPI_SUCCESS);
}                      
