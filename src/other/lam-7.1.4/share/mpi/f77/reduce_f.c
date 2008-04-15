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
 *	$Id: reduce_f.c,v 6.5 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Reduce F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_reduce_(sb, rb, n, t, op, r, c, ierr)

char	*sb, *rb;
int	*n, *t, *op, *r, *c, *ierr;

{
	*ierr = MPI_Reduce(BUF(sb), BUF(rb), *n,
				GETHDL(*t), GETHDL(*op), *r, GETHDL(*c));
}
