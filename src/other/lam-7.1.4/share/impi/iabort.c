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
 *	$Id: iabort.c,v 1.3 2002/10/09 20:57:16 brbarret Exp $
 *
 *	Function:	- send an abort message to the LAM IMPI host
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <lamdebug.h>
#include <mpisys.h>


/*
 * private variables
 */
static int              fl_debug = 0;


int
IMPI_Abort(void)
{
  int impid_rank;

  MPI_Comm_size(impid_comm, &impid_rank);
  impid_rank--;
  if (MPI_Send((void *) 0, 0, MPI_BYTE, impid_rank, IMPI_ABORT_TAG, 
	       impid_comm) != MPI_SUCCESS)
    return (LAMERROR);
  DBUG("IMPI_ABORT_TAG message sent\n");

  return 0;
}

#endif /* LAM_WANT_IMPI */

