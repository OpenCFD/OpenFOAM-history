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
 *	$Id: icmerge_f.c,v 6.5 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Intercomm_merge F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <rpisys.h>

void
mpi_intercomm_merge_(ic, hi, nc, ierr)

int	*ic, *hi, *nc, *ierr;

{
	MPI_Comm comm;

	*ierr = MPI_Intercomm_merge(GETHDL(*ic), *hi, &comm);

	if (lam_F_make_hdl(nc, ierr, comm, BLKMPIICOMMMERGE)) {
		comm->c_f77handle = *nc;
	}
}
