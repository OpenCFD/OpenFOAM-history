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
 * $Id: mpil_rgetname.c,v 6.6 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- get name for MPI_Requests
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>

/*
 *	MPIL_Request_get_name
 *
 *	Function:	- gets a string name on an MPI_Request
 *	Accepts:	- MPI_Request
 */
/*@

MPIL_Request_get_name - LAM/MPI-specific function to get a string name
on an MPI_Request

Input Parameters:
- req - MPI_Request (handle)

Return Value:

If a valid MPI_Request is passed to this function, a valid string will
be returned, even if no name was ever set on the request.  The string
that is returned is a pointer to internal storage and should not be
modified or freed.

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.seealso: MPIL_Request_set_name
@*/
char *
MPIL_Request_get_name(MPI_Request req)
{
  char *ret = "(Unnamed MPI_Request)";

  lam_initerr();
  lam_setfunc(BLKMPILREQUESTGETNAME);

  if (req == MPI_REQUEST_NULL) {
    lam_errfunc(MPI_COMM_WORLD,
		lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL));
    return "(ERROR IN MPIL_REQUEST_GET_NAME)";
  }

  if (req->rq_name != NULL)
    ret = req->rq_name;

  lam_resetfunc(BLKMPILREQUESTGETNAME);
  return ret;
}
