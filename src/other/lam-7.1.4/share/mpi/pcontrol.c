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
 * $Id: pcontrol.c,v 6.7 2002/12/11 19:15:13 jsquyres Exp $
 *
 *	Function:	- set profiling control level
 *			- NOP in MPI, up for profiling packages
 *	Accepts:	- level setting
 *			- varargs
 *	Returns:	- MPI_SUCCESS
 */

#include <lam_config.h>
#include <mpi.h>
#include <mpisys.h>

#if LAM_WANT_PROTOS

/*@

MPI_Pcontrol - Controls profiling

Input Parameters:
. level - Profiling level 

Notes:

This routine provides a common interface for profiling control.  The
interpretation of 'level' and any other arguments is left to the
profiling library.  LAM/MPI''s implementation of this function is a
no-op that always returns 'MPI_SUCCESS'.

.N fortran

.N ACK
@*/
int MPI_Pcontrol(int level, ...)
{
	return(MPI_SUCCESS);
}

#else

int
MPI_Pcontrol(level)
int			level;
{
	return(MPI_SUCCESS);
}

#endif
