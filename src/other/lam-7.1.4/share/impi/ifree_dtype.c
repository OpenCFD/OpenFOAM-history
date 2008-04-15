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
 *	$Id: ifree_dtype.c,v 1.8 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	Free a datatype for IMPI meta messages
 *                      
 */

#include <lam_config.h>
#if LAM_WANT_IMPI
#include <stdio.h>
#include <errno.h>

#include <app_mgmt.h>
#include <net.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <kreq.h>
#include <events.h>
#include <impi-defs.h>
#include <impi.h>


int 
IMPI_Free_dtype(void)
{
  if (IMPI_LAMGIAPPE != MPI_DATATYPE_NULL)
    MPI_Type_free(&IMPI_LAMGIAPPE);

  IMPI_TYPE_UINT8 = MPI_DATATYPE_NULL;

  return 0;
}

#endif
