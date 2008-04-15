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
 *	$Id: grincl_f.c,v 6.5 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Group_range_incl F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_group_range_incl_(g, n, ranges, ng, ierr)

int	*g, *n, *ranges, *ng, *ierr;

{
	MPI_Group group;

	*ierr = MPI_Group_range_incl(GETHDL(*g), *n, (void *) ranges, &group);

	if (*ierr != MPI_SUCCESS || !group) {
		return;
	}

	if (group->g_f77handle >= 0) {
		*ng = group->g_f77handle;
	} else 	if (lam_F_make_hdl(ng, ierr, group, BLKMPIGROUPRINCL)) {
		group->g_f77handle = *ng;
	}
}
