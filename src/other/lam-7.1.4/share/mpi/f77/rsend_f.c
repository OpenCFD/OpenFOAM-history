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
 *	Copyright 1996 The Ohio	State University
 *	RBD/NJN
 *
 *	$Id: rsend_f.c,v 6.4 2002/10/09 20:59:07 brbarret Exp $
 *
 *	Function:	- MPI_Rsend F77	wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_rsend_(b, n, t, d, tag, c, ierr)

char	*b;
int	*n, *t,	*d, *tag, *c, *ierr;

{
	*ierr =	MPI_Rsend(BUF(b), *n, GETHDL(*t), *d, *tag, GETHDL(*c));
}
