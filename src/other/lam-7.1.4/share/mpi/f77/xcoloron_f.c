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
 *	$Id: xcoloron_f.c,v 1.2 2002/10/09 20:59:11 brbarret Exp $
 *
 *	Function:	- XMPI_Coloron fortran binding
 */

#include <lam_config.h>

#include <mpi.h>
#include <MPISYSF.h>
#include <mpisys.h>

void 
xmpi_coloron_(int *red, int *green, int *blue, int *ierr)
{
  *ierr = XMPI_Coloron(*red, *green, *blue);
}
