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
 * $Id: getversion.c,v 6.5 2002/12/11 19:15:11 jsquyres Exp $
 *
 *	Function:	- MPI library version
 *	Accepts:	- version (out)
 *			- sub version (out)
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>


/*@
   MPI_Get_version - obtain the version number of MPI that LAM/MPI supports

Output Parameters:
+ version - version number
- subversion - subversion number

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG

.N WEB
@*/
int
MPI_Get_version(int *version, int *subversion)
{
  /* MPI_Get_version can be called before MPI_INIT and after
   * MPI_FINALIZE, so it can not use the normal error handling
   * mechanism.  Always return (no abort).  Use the same error code
   * encoding that lam_mkerr uses.  Can't use lam_mkerr because it
   * looks at lam_topfunc, which we can't properly set. 
   */
  if (version == 0 || subversion == 0) {
    int errcode;
    errcode = ((EINVAL & 0x7FFF) << 9) | (BLKMPIGETVERSION & 0x1FF);
    errcode = (errcode << 8) | (MPI_ERR_ARG & 0xFF);
    return errcode;
  }

  *version = MPI_VERSION;
  *subversion = MPI_SUBVERSION;
	
  return(MPI_SUCCESS);
}
