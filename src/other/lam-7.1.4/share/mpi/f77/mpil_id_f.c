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
 *	$Id: mpil_id_f.c,v 6.4 2002/10/09 20:59:05 brbarret Exp $
 *
 *	Function:	- MPIL_Comm_id,	MPIL_Comm_gps, MPIL_Type_id F77	wrappers
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpil_comm_id_(c, id, ierr)

int	*c, *id, *ierr;

{
	*ierr =	MPIL_Comm_id(GETHDL(*c), id);
}


void 
mpil_comm_gps_(c, r, n,	p, ierr)

int	*c, *r,	*n, *p,	*ierr;

{
	*ierr =	MPIL_Comm_gps(GETHDL(*c), *r, n, p);
}


void 
mpil_type_id_(t, id, ierr)

int	*t, *id, *ierr;

{
	*ierr =	MPIL_Type_id(GETHDL(*t), id);
}
