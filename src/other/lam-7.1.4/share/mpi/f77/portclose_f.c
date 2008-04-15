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
 *	Copyright 1996 The Ohio	State University
 *	NJN
 *
 *	$Id: portclose_f.c,v 6.5 2002/10/09 20:59:06 brbarret Exp $
 *
 *	Function:	- MPI_Close_port F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_close_port_(p, ierr, np)

char			*p;
int			*ierr;
int			np;

{
	char		*port;

	port = lam_F2C_string(p, np);
	if (port == 0) {
		lam_setfunc(BLKMPICLOSEPORT);
		*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPICLOSEPORT,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr =	MPI_Close_port(port);
	free(port);
}
