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
 *	$Id: imk_dtype.c,v 1.9 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	Make a datatype for IMPI meta messages
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
#include <lamdebug.h>
#include <impi-defs.h>
#include <impi.h>


/*
 * Exported variables
 */
MPI_Datatype IMPI_LAMGIAPPE = MPI_DATATYPE_NULL;
MPI_Datatype IMPI_TYPE_UINT8 = MPI_DATATYPE_NULL;


/*
 * private variables
 */
static int fl_debug = 0;


int 
IMPI_Make_dtype() 
{
  int lengths[2];
  MPI_Aint disps[2];
  MPI_Datatype oldtypes[2];
  IMPI_Lamgiappe lamgiappe;

  /* Make a datatype for the IMPI_Uint8 type */

  if (sizeof(unsigned int) == 8)
    IMPI_TYPE_UINT8 = MPI_UNSIGNED;
  else if (sizeof(unsigned long) == 8)
    IMPI_TYPE_UINT8 = MPI_UNSIGNED_LONG;
  else if (sizeof(unsigned long long) == 8)
    IMPI_TYPE_UINT8 = MPI_UNSIGNED_LONG_LONG;
  else
    return (LAMERROR);

  /* Make the datatype for IMPI_Lamgiappe */

  DBUG("Making the IMPI_Lamgiappe datatype\n");

  lengths[0] = 1;
  lengths[1] = (sizeof(IMPI_Lamgiappe) - sizeof(IMPI_Uint8)) / sizeof(int);
  disps[0] = ((char*) &lamgiappe.srqid) - ((char*) &lamgiappe);
  disps[1] = ((char*) &lamgiappe.local_src_rank) - ((char*) &lamgiappe);
  oldtypes[0] = IMPI_TYPE_UINT8;
  oldtypes[1] = MPI_INT;

  MPI_Type_create_struct(2, lengths, disps, oldtypes, &IMPI_LAMGIAPPE);
  MPI_Type_commit(&IMPI_LAMGIAPPE);

  return MPI_SUCCESS;
}

#endif
