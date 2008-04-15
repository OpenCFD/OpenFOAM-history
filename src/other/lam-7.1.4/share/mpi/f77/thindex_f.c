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
 *	NJN/RBD
 *
 *	$Id: thindex_f.c,v 6.5 2002/10/09 20:59:09 brbarret Exp $
 *
 *	Function:	- MPI_Type_hindexed F77	wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_type_hindexed_(c, b, d, ot,	nt, ierr)

int			*c, *b,	*d, *ot, *nt, *ierr;

{
	MPI_Datatype	dtype;
	MPI_Aint	*dp;
	int		i;

	if (*c > 0) {
		dp = (MPI_Aint *) malloc((*c) * sizeof(MPI_Aint));
		if (dp == 0) {
			lam_setfunc_m(BLKMPITHINDEX);
			*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPITHINDEX,
					lam_mkerr(MPI_ERR_OTHER, errno));
			return;
		}
	} else {
		dp = 0;
	}

	for (i = 0; i <	*c; ++i) {
		dp[i] = (MPI_Aint) d[i];
	}

	*ierr =	MPI_Type_hindexed(*c, b, dp, GETHDL(*ot), &dtype);

	if (lam_F_make_hdl(nt, ierr, dtype, BLKMPITHINDEX)) {
		dtype->dt_f77handle = *nt;
		dtype->dt_flags |= LAM_LANGF77;
	}

	free((char *) dp);
}
