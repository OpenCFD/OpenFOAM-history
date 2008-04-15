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
 *	$Id: reqfree_f.c,v 6.6 2002/10/09 20:59:07 brbarret Exp $
 *
 *	Function:	- MPI_Request_free F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_request_free_(rq, ierr)

int	*rq, *ierr;

{
	MPI_Request req;

	req = GETHDL(*rq);
	*ierr = MPI_Request_free(&req);
	*rq = -1;
}
