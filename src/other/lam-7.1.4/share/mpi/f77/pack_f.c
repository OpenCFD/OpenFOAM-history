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
 *	$Id: pack_f.c,v 6.4 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Pack F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_pack_(ib, ic, t, ob, oc, p,	c, ierr)

char	*ib, *ob;
int	*ic, *t, *oc, *p, *c, *ierr;

{
	*ierr =	MPI_Pack(BUF(ib), *ic, GETHDL(*t), ob, *oc, p, GETHDL(*c));
}
