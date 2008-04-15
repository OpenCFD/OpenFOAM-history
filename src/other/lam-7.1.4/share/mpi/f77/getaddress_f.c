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
 *	NJN
 *
 *	$Id: getaddress_f.c,v 1.2 2002/10/09 20:59:03 brbarret Exp $
 *
 *	Function:	-  F77 wrapper for MPI_Get_address
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void mpi_get_address_(l, a, ierr)

char			*l;
MPI_Aint		*a;
int			*ierr;

{
	*ierr =	MPI_Get_address(BUF(l), a);
}
