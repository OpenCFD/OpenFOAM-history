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
 *	$Id: graphget_f.c,v 6.4 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Graph_get	F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_graph_get_(c, mi, me, i, e,	ierr)

int	*c, *mi, *me, *i, *e, *ierr;

{
	*ierr =	MPI_Graph_get(GETHDL(*c), *mi, *me, i, e);
}
