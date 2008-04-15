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
 *	$Id: recv_f.c,v 6.6 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Recv F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_recv_(b, n, t, s, tag, c, st, ierr)

char	*b;
int	*n, *t, *s, *tag, *c, *st, *ierr;

{
	MPI_Status	*stat;

	if ((void *) st == lam_F_status_ignore) {
		stat = LAM_MPI_C_STATUS_IGNORE;
	} else if ((void *) st == lam_F_statuses_ignore) {
		lam_setfunc(BLKMPIRECV);
		*ierr = lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, 0,
					"MPI_STATUSES_IGNORE");
		return;
	} else {
		stat = (MPI_Status *) st;
	}

	*ierr = MPI_Recv(BUF(b), *n, GETHDL(*t), *s, *tag, GETHDL(*c), stat);
}
