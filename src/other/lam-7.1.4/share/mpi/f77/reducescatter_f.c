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
 *	$Id: reducescatter_f.c,v 6.5 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Reduce_scatter F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_reduce_scatter_(sb, rb, rn, t, op, comm, ierr)

char	*sb, *rb;
int	*rn, *t, *op, *comm, *ierr;

{
	*ierr = MPI_Reduce_scatter(BUF(sb), BUF(rb), rn,
				   GETHDL(*t), GETHDL(*op), GETHDL(*comm));
}
