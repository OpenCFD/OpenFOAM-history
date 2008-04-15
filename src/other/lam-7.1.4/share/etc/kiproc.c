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
 * $Id: kiproc.c,v 1.3 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- stub for stand-alone kernel
 */

#include <lam_config.h>

#include <unistd.h>

#include <portable.h>
#include <kreq.h>


/*
 *	kboot
 *
 *	Function:	- a stub for the external-only kernel
 */
void
kboot(int *argc, char **argv)
{
}

/*
 *	kbgetpid
 *
 *	Function:	- a wrapper for getpid() for external procs
 */
int4
kbgetpid(void)
{
	return(getpid());
}
