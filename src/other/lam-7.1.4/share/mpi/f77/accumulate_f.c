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
 *	$Id: accumulate_f.c,v 1.2 2002/10/09 20:59:00 brbarret Exp $
 *
 *	Function:	- MPI_Accumulate F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_accumulate_(oa, oc, ot, r, td, tc, tt, op, w, ierr)

char		*oa;
int		*oc, *ot, *r, *tc, *tt, *op, *w, *ierr;
MPI_Aint	*td;

{
	*ierr = MPI_Accumulate(BUF(oa), *oc, GETHDL(*ot), *r, *td, *tc,
			GETHDL(*tt), GETHDL(*op), GETHDL(*w));
}
