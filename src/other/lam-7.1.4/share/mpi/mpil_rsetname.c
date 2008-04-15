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
 * $Id: mpil_rsetname.c,v 6.6 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- set name for MPI_Requests
 */

#include <string.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*
 *	MPIL_Request_set_name
 *
 *	Function:	- sets a string name on an MPI_Request
 *	Accepts:	- MPI_Request
 *			- string name
 */
/*@

MPIL_Request_set_name - LAM/MPI-specific function to set a string name
on an MPI_Request

Input Parameters:
+ req - MPI_Request (handle)
- name - Name

Notes:

The 'name' must be a null-terminated string.  It is copied into
internal storage during this call.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso: MPIL_Request_get_name
@*/
int
MPIL_Request_set_name(MPI_Request req, char *name)
{
  lam_initerr();
  lam_setfunc(BLKMPILREQUESTSETNAME);

  if (req == MPI_REQUEST_NULL)
    return(lam_errfunc(MPI_COMM_WORLD,
		       lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));

  if (name == NULL)
    return(lam_errfunc(MPI_COMM_WORLD,
		       lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));

  req->rq_name = strdup(name);
  
  lam_resetfunc(BLKMPILREQUESTSETNAME);
  return(MPI_SUCCESS);
}
