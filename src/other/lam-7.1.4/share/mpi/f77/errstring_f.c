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
 *	$Id: errstring_f.c,v 6.5 2002/10/09 20:59:03 brbarret Exp $
 *
 *	Function:	- MPI_Error_string F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_error_string_(err, str, l, ierr, ns)

int	*err;
char	*str;
int	*l, *ierr;
int	ns;

{
	char errstring[MPI_MAX_ERROR_STRING + 1];

	if (ns < MPI_MAX_ERROR_STRING) {
		lam_setfunc(BLKMPIERRSTRING);
		*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPIERRSTRING,
					lam_mkerr(MPI_ERR_ARG, 0));
		return;
	}

	*ierr =	MPI_Error_string(*err, errstring, l);

	if (*ierr == MPI_SUCCESS) {
		lam_C2F_string(errstring, str, ns);
	}
}
