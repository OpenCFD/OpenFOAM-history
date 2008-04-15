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
 * $Id: xcoloron.c,v 6.4 2002/12/11 19:15:15 jsquyres Exp $
 *
 *	Function:	- enable a color in the XMPI trace stream
 */

#include <blktype.h>
#include <mpisys.h>

/*
 *	XMPI_Coloron
 *
 *	Function:	- drop a buoy in the trace stream
 *	Accepts:	- buoy name
 */
/*@

XMPI_Coloron - LAM/MPI-specific function to enable a color in the XMPI
trace stream.

Input Parameter:
+ red - red value (integer)
. green - green value (integer)
- blue - blue value (integer)

Notes:

The LAM implementation of MPI is integrated with the XMPI run/debug
viewer.  It can generate tracefiles and on-the-fly trace streams
suitable for display in XMPI.  

A new functionality in XMPI is the ability to enable and disable
select colors in the trace stream.  LAM/MPI allows this functionality
with the 'XMPI_Coloron' and 'XMPI_Coloroff' functions.  

'XMPI_Coloron' is called with 'red', 'green', and 'blue' parameters.
Each value may be from 0 to 255.  The resulting RGB composite will
become activated for that rank at that point in time.  Enabling and
disabling colors is a local action; the calls will return
immediately.  The color will be activated or deactivated on the
timeline corresponding to the rank that invoked 'XMPI_Coloron' /
'XMPI_Colorff' in the XMPI trace window.

Only one color is active at a time.  However, 'XMPI_Coloron' may be
invoked multiple times to build a stack of colors.  'XMPI_Coloroff'
will pop the last color off the stack and make the previous color
active.

If this function is invoked and tracing is not active, the color is
ignored.

There is no profiling version of this function.

This is a LAM/MPI-specific function and is intended mainly for use
with XMPI.  If this function is used, it should be used in conjunction
with the 'LAM_MPI' C preprocessor macro

.vb
  #if LAM_MPI
     XMPI_Coloron(255, 255, 0);
  #endif
.ve

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
.N MPI_ERR_INTERN

.seealso: XMPI_Buoy, XMPI_Coloroff
@*/
int 
XMPI_Coloron(int red, int green, int blue)
{
  lam_initerr();
  lam_setfunc(BLKXMPICOLORON);

  if (red < 0 || red > 255 || green < 0 || green > 255 || 
      blue < 0 || blue > 255)
    return(lam_errfunc(MPI_COMM_WORLD,
		       lam_getfunc(), lam_mkerr(MPI_ERR_ARG, EINVAL)));

  /* Enable the color in the trace stream */

  if (LAM_TRACE_TOP())
    if (lam_tr_coloron(red, green, blue, MPI_Wtime()) != 0)
      return(lam_errfunc(MPI_COMM_WORLD,
			 lam_getfunc(), lam_mkerr(MPI_ERR_INTERN, EINVAL)));

  lam_resetfunc(BLKXMPICOLORON);
  return MPI_SUCCESS;
}
