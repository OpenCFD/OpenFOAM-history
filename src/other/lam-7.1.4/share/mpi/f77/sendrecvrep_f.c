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
 *	$Id: sendrecvrep_f.c,v 6.6 2002/10/09 20:59:07 brbarret Exp $
 *
 *	Function:	- MPI_Sendrecv_replace F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_sendrecv_replace_(b, n, t, d, stag, s, rtag, c, fst, ierr)

char	*b;
int	*n, *t, *d, *stag, *s, *rtag, *c, *fst, *ierr;

{
	MPI_Status	*stat;

	if ((void *) fst == lam_F_status_ignore) {
		stat = LAM_MPI_C_STATUS_IGNORE;
	} else if ((void *) fst == lam_F_statuses_ignore) {
		lam_setfunc(BLKMPISENDRECVREP);
		*ierr = lam_err_comm(MPI_COMM_WORLD, MPI_ERR_ARG, 0,
					"MPI_STATUSES_IGNORE");
		return;
	} else {
		stat = (MPI_Status *) fst;
	}

	*ierr = MPI_Sendrecv_replace(BUF(b), *n, GETHDL(*t), *d, *stag,
				     *s, *rtag, GETHDL(*c), stat);
}
