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
 *	$Id: exscan_f.c,v 6.1 2003/10/22 23:44:33 adharurk Exp $
 *
 *	Function:	- MPI_Scan F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_exscan_(sb, rb, n, t, op, comm, ierr)

char	*sb, *rb;
int	*n, *t, *op, *comm, *ierr;

{
	*ierr = MPI_Exscan(BUF(sb), BUF(rb), *n,
			   GETHDL(*t), GETHDL(*op), GETHDL(*comm));
}
