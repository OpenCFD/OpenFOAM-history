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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: kiproc.c,v 6.4 2002/10/09 20:56:45 brbarret Exp $
 *
 *	Function:	- stub for stand-alone kernel
 */

#include <portable.h>
#include <unistd.h>

/*
 *	kboot
 *
 *	Function:	- a stub for the external-only kernel
 */
void
kboot()

{
}

/*
 *	kbgetpid
 *
 *	Function:	- a wrapper for getpid() for external procs
 */
int4
kbgetpid()

{
	return(getpid());
}
