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
 *	$Id: tcreatestruct_f.c,v 1.2 2002/10/09 20:59:08 brbarret Exp $
 *
 *	Function:	- MPI_Type_create_struct F77 wrapper
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_type_create_struct_(c, b, d, t, nt, ierr)

int			*c, *b, *t, *nt, *ierr;
MPI_Aint		*d;

{
	MPI_Datatype	dtype;
	MPI_Datatype	*tp;
	int		i;

	if (*c > 0) {
		tp = (MPI_Datatype *) malloc((*c) * sizeof(MPI_Datatype));
		if (tp == 0) {
			lam_setfunc(BLKMPITCREATESTRUCT);
			*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPITCREATESTRUCT,
					lam_mkerr(MPI_ERR_OTHER, errno));
			return;
		}
	} else {
		tp = 0;
	}
	
	for (i = 0; i < *c; ++i) {
		tp[i] = GETHDL(t[i]);
	}

	*ierr = MPI_Type_create_struct(*c, b, d, tp, &dtype);

	if (lam_F_make_hdl(nt, ierr, dtype, BLKMPITCREATESTRUCT)) {
		dtype->dt_f77handle = *nt;
	}

	free((char *) tp);
}
