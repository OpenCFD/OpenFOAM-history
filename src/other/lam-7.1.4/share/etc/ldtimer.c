/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: ldtimer.c,v 1.3 2003/07/05 21:50:25 jsquyres Exp $
 *	Function:	- loads current value of local node's timer
 *			- just a stub now, to match ITB ldtimer()
 */

#include <lam_config.h>

#include <portable.h>
#include <etc_misc.h>

int4
ldtimer(void)
{
	return(INT4_NIL);
}
