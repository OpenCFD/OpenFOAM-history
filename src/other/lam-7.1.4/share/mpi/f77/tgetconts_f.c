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
 *	$Id: tgetconts_f.c,v 1.2 2002/10/09 20:59:08 brbarret Exp $
 *
 *	Function:	- MPI_Type_get_contents F77 wrapper
 */

#include <errno.h>
#include <stdlib.h>

#include <lam_config.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>


void 
mpi_type_get_contents_(t, ni, na, nd, is, as, ds, ierr)

int			*t, *ni, *na, *nd, *is, *ds, *ierr;
MPI_Aint		*as;

{
	MPI_Datatype	*dtypes;
	int		i;

	dtypes = (MPI_Datatype *) malloc(*nd * sizeof(MPI_Datatype));
	if (dtypes == 0) {
		lam_setfunc_m(BLKMPITGETCONT);
		*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPITGETCONT,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr = MPI_Type_get_contents(GETHDL(*t), *ni, *na, 
					*nd, is, as, dtypes);
	if (*ierr != MPI_SUCCESS) return;

	for (i = 0; i < *nd; i++) {
		if (dtypes[i]->dt_f77handle != -1) {
			ds[i] = dtypes[i]->dt_f77handle;
		} else {
			if (lam_F_make_hdl(&ds[i], ierr, 
						dtypes[i], BLKMPITGETCONT)) {
				dtypes[i]->dt_f77handle = ds[i];
			} else {
				return;
			}
		}
	}

	free((char *) dtypes);
}

