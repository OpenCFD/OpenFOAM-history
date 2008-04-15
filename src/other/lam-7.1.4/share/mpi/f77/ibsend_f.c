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
 *	$Id: ibsend_f.c,v 6.5 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Ibsend F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <rpisys.h>

void
mpi_ibsend_(b, n, t, d, tag, c, rq, ierr)

char	*b;
int	*n, *t, *d, *tag, *c, *rq, *ierr;

{
	MPI_Request req;

	*ierr = MPI_Ibsend(BUF(b), *n, GETHDL(*t), *d, *tag, GETHDL(*c), &req);

	if (lam_F_make_hdl(rq, ierr, req, BLKMPIIBSEND)) {
		req->rq_f77handle = *rq;
	}
}
