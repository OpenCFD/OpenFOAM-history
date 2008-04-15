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
 *	$Id: infogetnth_f.c,v 6.5 2002/10/09 20:59:05 brbarret Exp $
 *
 *	Function:	- MPI_Info_get_nthkey F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_info_get_nthkey_(i,	n, k, ierr, nk)

int	*i, *n,	*ierr;
char	*k;
int	nk;

{
	char key[MPI_MAX_INFO_KEY + 1];
	
	*ierr =	MPI_Info_get_nthkey(GETHDL(*i),	*n, key);
	if (*ierr == MPI_SUCCESS) {
		lam_C2F_string(key, k, nk);
	}
}
