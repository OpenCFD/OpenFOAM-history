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
 *	$Id: wcreate_f.c,v 1.2 2002/10/09 20:59:10 brbarret Exp $
 *
 *	Function:	- MPI_Win_create F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_win_create_(b, s, d, i, c, nw, ierr)

char			*b;
int			*s, *i, *c, *nw, *ierr;
MPI_Aint		*d;

{
	MPI_Win		win;

	*ierr = MPI_Win_create(BUF(b), *s, *d, GETHDL(*i), GETHDL(*c), &win);

	if (lam_F_make_hdl(nw, ierr, win, BLKMPIWINCREATE)) {
		win->w_f77handle = *nw;
	}
}
