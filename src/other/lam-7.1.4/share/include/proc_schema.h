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
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *	$Log: proc_schema.h,v $
 *	Revision 1.2  2002/10/09 20:57:25  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 1.1  2000/02/12 00:14:19  jsquyres
 *	Moved share/boot/proc_schema.a to share/include/proc_schema.h 'cause
 *	multiple places include it.
 *	
 *	Revision 6.2  1999/05/23 19:28:25  kmeyer1
 *	added copyrights
 *	
 *	Revision 6.1  1996/11/24 00:44:59  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:41:36  gdburns
 * Ohio Release
 * 
 *	Function:	- internal representation of a process schema
 */

#ifndef _PROC_SCHEMA
#define _PROC_SCHEMA

#include <portable.h>

#define PSC_MAXLINE	256
#define PSC_DELAY	1

/*
 * process schema list entry
 */
struct psc {
	char		**psc_argv;	/* argv */
	int4		psc_argc;	/* argc */
	int4		psc_delay;	/* delay (in seconds) after startup */
	int4		psc_flags;	/* info bits */
};

#endif
