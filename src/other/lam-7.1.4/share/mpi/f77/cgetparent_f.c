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
 *	$Id: cgetparent_f.c,v 1.2 2002/10/09 20:59:02 brbarret Exp $
 *
 *	Function:	- MPI_Comm_get_parent F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_comm_get_parent_(c, ierr)

int			*c, *ierr;

{
	MPI_Comm	parent;

	*c = -1;
	*ierr = MPI_Comm_get_parent(&parent);

	if (*ierr == MPI_SUCCESS && parent != MPI_COMM_NULL) {
		if (parent->c_f77handle < 0) {
			lam_F_make_hdl(c, ierr, parent, BLKMPICOMMGETPARENT);
			parent->c_f77handle = *c;
		} else {
			*c = parent->c_f77handle;
		}
	}
}
