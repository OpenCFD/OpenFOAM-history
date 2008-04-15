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
 *	RBD/NJN
 *
 *	$Id: cdup_f.c,v 6.5 2002/10/09 20:59:01 brbarret Exp $
 *
 *	Function:	- MPI_Comm_dup F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_comm_dup_(c, nc, ierr)

int	*c, *nc, *ierr;

{
	MPI_Comm comm;

	*ierr = MPI_Comm_dup(GETHDL(*c), &comm);

	if (lam_F_make_hdl(nc, ierr, comm, BLKMPICOMMDUP)) {
		comm->c_f77handle = *nc;
	}
}
