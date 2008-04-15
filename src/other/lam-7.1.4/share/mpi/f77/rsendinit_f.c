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
 *	$Id: rsendinit_f.c,v 6.5 2002/10/09 20:59:07 brbarret Exp $
 *
 *	Function:	- MPI_Rsend_init F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <rpisys.h>

void
mpi_rsend_init_(b, n, t, d, tag, c, rq, ierr)

char	*b;
int	*n, *t, *d, *tag, *c, *rq, *ierr;


{
	MPI_Request req;

	*ierr = MPI_Rsend_init(BUF(b), *n, GETHDL(*t),
			       *d, *tag, GETHDL(*c), &req);

	if (lam_F_make_hdl(rq, ierr, req, BLKMPIRSINIT)) {
		req->rq_f77handle = *rq;
	}
}
