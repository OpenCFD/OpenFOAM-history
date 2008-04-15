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
 *	$Id: wait_f.c,v 6.7 2002/10/09 20:59:09 brbarret Exp $
 *
 *	Function:	- MPI_Wait F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_wait_(rq, st, ierr)

int			*rq, *st, *ierr;

{
	MPI_Request	req;
	MPI_Status	*stat;

	if ((void *) st == lam_F_status_ignore) {
		stat = LAM_MPI_C_STATUS_IGNORE;
	} else if ((void *) st == lam_F_statuses_ignore) {
		lam_setfunc(BLKMPIWAIT);
		*ierr = lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, 0,
					"MPI_STATUSES_IGNORE");
		return;
	} else {
		stat = (MPI_Status *) st;
	}

	req = GETHDL(*rq);
	*ierr =	MPI_Wait(&req, stat);
	
	if (*ierr == MPI_SUCCESS && req	== LAM_MPI_C_REQUEST_NULL) {
		*rq = -1;
	}
}
