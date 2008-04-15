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
 *	$Id: csetname_f.c,v 1.2 2002/10/09 20:59:02 brbarret Exp $
 *
 *	Function:	- MPI_Comm_set_name F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
mpi_comm_set_name_(c, n, ierr, nn)

int			*c, *ierr;
char			*n;
int 			nn;

{
	char		*name;

	name = lam_F2C_string(n, nn);
	if (name == 0) {
		lam_setfunc(BLKMPICOMMSETNAME);
		*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMSETNAME,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr =	MPI_Comm_set_name(GETHDL(*c), name);

	free(name);
}
