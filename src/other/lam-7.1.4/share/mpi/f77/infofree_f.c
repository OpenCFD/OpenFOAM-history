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
 *	NJN
 *
 *	$Id: infofree_f.c,v 6.6 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Info_free F77 wrapper
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_info_free_(i, ierr)

int	*i, *ierr;

{
	MPI_Info	info;

	info = GETHDL(*i);
	*ierr = MPI_Info_free(&info);
	*i = -1;
}
