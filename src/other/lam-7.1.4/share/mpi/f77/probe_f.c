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
 *	$Id: probe_f.c,v 6.6 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Probe F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_probe_(s, tag, c, fst, ierr)

int	*s, *tag, *c, *fst, *ierr;

{
	MPI_Status	*stat;

	if ((void *) fst == lam_F_status_ignore) {
		stat = LAM_MPI_C_STATUS_IGNORE;
	} else if ((void *) fst == lam_F_statuses_ignore) {
		lam_setfunc(BLKMPIPROBE);
		*ierr = lam_err_comm(GETHDL(*c), MPI_ERR_ARG, 0,
					"MPI_STATUSES_IGNORE");
		return;
	} else {
		stat = (MPI_Status *) fst;
	}

	*ierr = MPI_Probe(*s, *tag, GETHDL(*c), stat);
}
