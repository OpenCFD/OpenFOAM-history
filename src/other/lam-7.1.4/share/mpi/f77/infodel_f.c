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
 *	Copyright 1996 The Ohio	State University
 *	NJN
 *
 *	$Id: infodel_f.c,v 6.4 2002/10/09 20:59:04 brbarret Exp $
 *
 *	Function:	- MPI_Info_delete F77 wrapper
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void
mpi_info_delete_(i, k, ierr, nk)

int	*i, *ierr;
char	*k;
int	nk;

{
	char *key;

	key = lam_F2C_string(k,	nk);
	if (key	== 0) {
		lam_setfunc(BLKMPIINFODEL);
		*ierr =	lam_errfunc(MPI_COMM_WORLD, BLKMPIINFODEL,
					lam_mkerr(MPI_ERR_OTHER, errno));
		return;
	}

	*ierr =	MPI_Info_delete(GETHDL(*i), key);

	free(key);
}
