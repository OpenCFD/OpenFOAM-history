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
 *	Copyright 1997 The Ohio	State University
 *	NJN
 *
 *	$Id: wgetname_f.c,v 1.2 2002/10/09 20:59:10 brbarret Exp $
 *
 *	Function:	- MPI_Win_get_name F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_win_get_name_(w, n, l, ierr, nn)

int			*w, *l, *ierr;
char			*n;
int 			nn;

{
	char		name[MPI_MAX_OBJECT_NAME];

	*ierr = MPI_Win_get_name(GETHDL(*w), name, l);

	if (*ierr == MPI_SUCCESS) {
	    lam_C2F_string(name, n, nn);

	    if (nn < *l) {
		*l = nn;
	    }
	}
}
