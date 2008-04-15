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
 *	NJN/RBD
 *
 *	$Id: startall_f.c,v 6.5 2002/10/09 20:59:07 brbarret Exp $
 *
 *	Function:	- MPI_Startall F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_startall_(c, r, ierr)

int	*c, *r, *ierr;


{
	MPI_Request	*rq;
	int		i;

	if (*c > 0) {
		rq = (MPI_Request *) malloc((*c) * sizeof(MPI_Request));
		if (rq == 0) {
			lam_setfunc_m(BLKMPISTARTALL);
			*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPISTARTALL,
					lam_mkerr(MPI_ERR_OTHER, errno));
			return;
		}
	} else {
		rq = 0;
	}

	for (i = 0; i < *c; ++i) {
		rq[i] = GETHDL(r[i]);
	}
	
	*ierr = MPI_Startall(*c, rq);

	free((char *) rq);
}
