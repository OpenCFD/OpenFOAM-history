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
 *	$Id: xbuoy_f.c,v 1.2 2002/10/09 20:59:11 brbarret Exp $
 *
 *	Function:	- XMPI_Buoy fortran binding
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>
#include <blktype.h>

void 
xmpi_buoy_(char* nstr, int *ierr, int nlen)
{
  char *name;

  name = lam_F2C_string(nstr, nlen);
  if (name == 0) {
    lam_setfunc(BLKXMPIBUOY);
    *ierr = lam_errfunc(MPI_COMM_WORLD, lam_getfunc(),
			lam_mkerr(MPI_ERR_ARG, errno));
    return;
  }

  *ierr = XMPI_Buoy(name);
  free(name);
}
