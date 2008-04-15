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
 *	$Id: dupfn_f.c,v 6.5 2002/10/09 20:59:02 brbarret Exp $
 *
 *	Function:	- duplicate functions wrappers
 */

#include <mpi.h>
#include <MPISYSF.h>

void
mpi_dup_fn_(handle, key, extra, attrin, attrout, flag, ierr)

int	*handle, *key, *extra, *attrin, *attrout, *flag, *ierr;

{
	*flag =	1;
	*attrout = *attrin;
	*ierr =	MPI_SUCCESS;
}

void
mpi_comm_dup_fn_(handle, key, extra, attrin, attrout, flag, ierr)

int	*handle, *key, *extra, *attrin, *attrout, *flag, *ierr;

{
	*flag =	1;
	*attrout = *attrin;
	*ierr =	MPI_SUCCESS;
}

void
mpi_type_dup_fn_(handle, key, extra, attrin, attrout, flag, ierr)

int	*handle, *key, *extra, *attrin, *attrout, *flag, *ierr;

{
	*flag =	1;
	*attrout = *attrin;
	*ierr =	MPI_SUCCESS;
}

void
mpi_win_dup_fn_(handle, key, extra, attrin, attrout, flag, ierr)

int	*handle, *key, *extra, *attrin, *attrout, *flag, *ierr;

{
	*flag =	1;
	*attrout = *attrin;
	*ierr =	MPI_SUCCESS;
}
