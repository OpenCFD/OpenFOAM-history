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
 *	$Id: tsubarray_f.c,v 1.2 2002/10/09 20:59:09 brbarret Exp $
 *
 *	Function:	- MPI_Type_create_subarray F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_type_create_subarray_(n, si, su, st, ord, ot, nt, ierr)

int	*n, *si, *su, *st, *ord, *ot, *nt, *ierr;

{
	MPI_Datatype dtype;

	*ierr = MPI_Type_create_subarray(*n, si, su, st,
					*ord, GETHDL(*ot), &dtype);

	if (lam_F_make_hdl(nt, ierr, dtype, BLKMPITCREATESUBARRAY)) {
		dtype->dt_f77handle = *nt;
	}
}
