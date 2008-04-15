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
 *	$Id: getprocname_f.c,v 6.7 2002/10/09 20:59:03 brbarret Exp $
 *
 *	Function:	- MPI_Get_processor_name F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_get_processor_name_(fname, l, ierr, n)

char	*fname;
int	*l, *ierr;
int	n;

{
	char cname[MPI_MAX_PROCESSOR_NAME + 1];

	if (n < MPI_MAX_PROCESSOR_NAME - 1) {
		lam_setfunc(BLKMPIGETPNAME);
		*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPIGETPNAME,
					lam_mkerr(MPI_ERR_ARG, 0));
		return;
	}

	*ierr = MPI_Get_processor_name(cname, l);

	if (*ierr == MPI_SUCCESS) {
		lam_C2F_string(cname, fname, n);
	}
}
