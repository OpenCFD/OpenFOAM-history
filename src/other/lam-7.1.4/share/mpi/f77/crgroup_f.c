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
 *
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	RBD/NJN
 *
 *	$Id: crgroup_f.c,v 6.5 2002/10/09 20:59:02 brbarret Exp $
 *
 *	Function:	- MPI_Comm_remote_group F77 wrapper
 */

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_comm_remote_group_(comm, g, ierr)

int	*comm, *g, *ierr;

{
	MPI_Group group;

	*ierr = MPI_Comm_remote_group(GETHDL(*comm), &group);

	if (*ierr != MPI_SUCCESS || !group) {
		return;
	}

	if (group->g_f77handle >= 0) {
		*g = group->g_f77handle;
	}
	else if (lam_F_make_hdl(g, ierr, group, BLKMPICOMMRGROUP)) {
		group->g_f77handle = *g;
	}
}
