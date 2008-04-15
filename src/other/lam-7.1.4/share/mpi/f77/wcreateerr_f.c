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
 *	$Id: wcreateerr_f.c,v 1.2 2002/10/09 20:59:10 brbarret Exp $
 *
 *	Function:	- MPI_Win_create_errhandler F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_win_create_errhandler_(func, hdl, ierr)

void	 (*func) ();
int	*hdl, *ierr;

{
	MPI_Errhandler errhdl;

	*ierr =	MPI_Win_create_errhandler(
			(MPI_Win_errhandler_fn *) func, &errhdl);

	if (lam_F_make_hdl(hdl, ierr, errhdl, BLKMPIWINCREATEERRHDL)) {
		errhdl->eh_f77handle = *hdl;
		errhdl->eh_flags |= LAM_LANGF77;
	}
}
