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
 * $Id: xbuoy.c,v 6.3 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- drop a buoy in the XMPI trace stream
 */

#include <blktype.h>
#include <mpisys.h>

/*
 *	XMPI_Buoy
 *
 *	Function:	- drop a buoy in the trace stream
 *	Accepts:	- buoy name
 */
/*@

XMPI_Buoy - LAM/MPI-specific function to drop a buoy in the XMPI trace
stream.

Input Parameter:
. name - buoy name (string)

Notes:

The LAM implementation of MPI is integrated with the XMPI run/debug
viewer.  It can generate tracefiles and on-the-fly trace streams
suitable for display in XMPI.  

A new functionality in XMPI is the ability to view "buoys" in the
trace stream that can be helpful in marking particular sections of
code.  LAM/MPI allows the dropping of buoys with the 'XMPI_Buoy'
function.  Calling 'XMPI_Buoy' with a string will drop a bouy of that
name (if tracing is enabled) in the trace stream.  Dropping a buoy is
a local action; the call will return immediately.  The buoy is dropped
in the trace stream of the local rank, and will appear on the
corresponding rank timeline in XMPI.

The name can be a maximum of 32 characters long; if it is longer, it
will be truncated.

If this function is invoked and tracing is not active, the buoy is
ignored.

There is no profiling version of this function.

This is a LAM/MPI-specific function and is intended mainly for use
with XMPI.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     XMPI_Buoy("my function begins");
     // ... message passing code
     XMPI_Buoy("my function ends");
  #endif
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INTERN

.seealso: XMPI_Coloron, XMPI_Coloroff
@*/
int 
XMPI_Buoy(const char* name)
{
  lam_initerr();
  lam_setfunc(BLKXMPIBUOY);

  if (name == 0)
    return(lam_errfunc(MPI_COMM_WORLD,
		       lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));

  /* Drop the trace in the stream */

  if (LAM_TRACE_TOP())
    if (lam_tr_buoy(name, MPI_Wtime()) != 0)
      return(lam_errfunc(MPI_COMM_WORLD,
			 lam_getfunc(), lam_mkerr(MPI_ERR_INTERN, EINVAL)));

  lam_resetfunc(BLKXMPIBUOY);
  return MPI_SUCCESS;
}
