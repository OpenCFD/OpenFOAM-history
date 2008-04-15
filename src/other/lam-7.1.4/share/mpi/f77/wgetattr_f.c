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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: wgetattr_f.c,v 1.3 2002/10/09 20:59:10 brbarret Exp $
 *
 *	Function:	- MPI_Win_get_attr F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_win_get_attr_(w, k, a, f, ierr)

int		*w, *k, *f, *ierr;
MPI_Aint	*a;

{
	struct _attrkey *pk;
	void *value;
	
	*ierr = MPI_Win_get_attr(GETHDL(*w), *k, &value, f);

	if (*ierr == MPI_SUCCESS && *f) {

		pk = lam_getattr(*k);

/*
 * This stuff is very confusing.  Be sure to see MPI-2 4.12.7.
 */
/*
 * If this attribute was created in fortran, we'll get a pointer back
 * to the integer.  And with WIN_GET_ATTR, we should have gotten a
 * fortran integer big enough to hold the value.  So just dereference
 * it.
 */
		if (pk->ak_flags & LAM_LANGF77) {
			*a = *((MPI_Aint *) value);
		} else {
/* 
 * With WIN_GET_ATTR, the fortran int argument passed in should be
 * the right size, so no conversaion should be necessary.
 */
			*a = (MPI_Aint) value;
		}
	}
}
