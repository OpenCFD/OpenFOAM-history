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
 *	Copyright 1997 The Ohio	State University
 *	RBD/NJN
 *
 *	$Id: attrput_f.c,v 6.6 2002/10/09 20:59:00 brbarret Exp $
 *
 *	Function:	- MPI_Attr_put F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_attr_put_(c, k, a, ierr)

int	*c, *k,	*a, *ierr;

{
  *ierr = MPI_Attr_put(GETHDL(*c), *k, (void *) LAM_INT_TO_VOID_P *a);
}
