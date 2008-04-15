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
 *	$Id: infoget_f.c,v 6.5 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Info_get F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_info_get_(i, k, l, v, f, ierr, nk, nv)

int	*i, *l, *f, *ierr;
char	*k, *v;
int	nk, nv;

{
	char value[MPI_MAX_INFO_VAL + 1];
	char *key;

	if (*l > nv) {
		lam_setfunc(BLKMPIINFOGET);
		*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGET,
					lam_mkerr(MPI_ERR_ARG, 0));
		return;
	}

	key = lam_F2C_string(k, nk);
	if (key == 0) {
		lam_setfunc(BLKMPIINFOGET);
		*ierr = lam_errfunc(MPI_COMM_WORLD, BLKMPIINFOGET,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr = MPI_Info_get(GETHDL(*i), key, *l, value, f);

	if (*ierr == MPI_SUCCESS && *f) {
		lam_C2F_string(value, v, nv);
	}

	free(key);
}
