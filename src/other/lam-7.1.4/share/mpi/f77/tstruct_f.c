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
 *	$Id: tstruct_f.c,v 6.5 2002/10/09 20:59:09 brbarret Exp $
 *
 *	Function:	- MPI_Type_struct F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_type_struct_(c, b, d, t, nt, ierr)

int			*c, *b,	*d, *t,	*nt, *ierr;

{
	MPI_Datatype	dtype;
	MPI_Datatype	*tp;
	MPI_Aint	*dp;
	int		i;

	if (*c > 0) {
		tp = (MPI_Datatype *) malloc((*c) * sizeof(MPI_Datatype));
		if (tp == 0) {
			lam_setfunc_m(BLKMPITSTRUCT);
			*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPITSTRUCT,
					lam_mkerr(MPI_ERR_OTHER, errno));
			return;
		}

		dp = (MPI_Aint *) malloc((*c) * sizeof(MPI_Aint));
		if (dp == 0) {
			free((char *) tp);
			lam_setfunc_m(BLKMPITSTRUCT);
			*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPITSTRUCT,
					lam_mkerr(MPI_ERR_OTHER, errno));
			return;
		}
	} else {
		tp = 0;
		dp = 0;
	}

	for (i = 0; i <	*c; ++i) {
		tp[i] =	GETHDL(t[i]);
		dp[i] =	(MPI_Aint) d[i];
	}

	*ierr =	MPI_Type_struct(*c, b, dp, tp, &dtype);

	if (lam_F_make_hdl(nt, ierr, dtype, BLKMPITSTRUCT)) {
		dtype->dt_f77handle = *nt;
		dtype->dt_flags |= LAM_LANGF77;
	}

	free((char *) tp);
	free((char *) dp);
}
