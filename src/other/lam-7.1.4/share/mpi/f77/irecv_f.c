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
 *	$Id: irecv_f.c,v 6.5 2002/10/09 20:59:05 brbarret Exp $
 *
 *	Function:	- MPI_Irecv F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <rpisys.h>

void
mpi_irecv_(b, n, t, s, tag, c, rq, ierr)

char	*b;
int	*n, *t, *s, *tag, *c, *rq, *ierr;

{
	MPI_Request req;

	*ierr = MPI_Irecv(BUF(b), *n, GETHDL(*t), *s, *tag, GETHDL(*c), &req);

	if (lam_F_make_hdl(rq, ierr, req, BLKMPIIRECV)) {
		req->rq_f77handle = *rq;
	}
}
