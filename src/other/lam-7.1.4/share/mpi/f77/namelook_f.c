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
 *	NJN
 *
 *	$Id: namelook_f.c,v 6.5 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Lookup_name F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_lookup_name_(s, i, p, ierr, ns, np)

char			*s, *p;
int			*i, *ierr;
int			ns, np;

{
	char		*port;
	char		*service;

	port = (char *)	malloc(np + 1);
	service	= lam_F2C_string(s, ns);

	if (port == 0 || service == 0) {
		lam_setfunc(BLKMPILOOKUPNAME);
		*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPILOOKUPNAME,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr = MPI_Lookup_name(service, GETHDL(*i), port);
	if (*ierr != MPI_SUCCESS) {
		return;
	}

	lam_C2F_string(port, p,	np);

	free(port);
	free(service);
}
