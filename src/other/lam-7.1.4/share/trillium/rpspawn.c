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
 *	$Log: rpspawn.c,v $
 *	Revision 6.3  2002/10/09 21:00:48  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/26 21:50:02  kmeyer1
 *	
 *	added copyright
 *	
 *	Revision 6.1  1996/11/24 00:26:11  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  14:05:06  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  96/02/11  10:32:28  gdburns
 * Add process ID and index as output parameters.
 * 
 * Revision 5.2  94/08/22  14:13:37  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:58:14  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:46:22  gdburns
 * Ohio Release
 * 
 *	Function:	- spawns processes from one remote node to another
 *			- spawns kenya processes
 *	Accepts:	- source node ID
 *			- destination node ID
 *			- process select flags
 *			- process select value
 *			- arguments argv structure
 *			- process ID (out)
 *			- process index (out)
 *	Returns:	- 0 or ERROR
 */

#include <kio.h>
#include <portable.h>
#include <preq.h>
#include <terror.h>
#include <typical.h>

/*
 * local variables
 */
static struct pstate	ps;

int
rpspawn(srcnode, destnode, sflags, svalue, argv, pid, idx)

int4			srcnode;
int4			destnode;
int4			sflags;
int4			svalue;
char			**argv;
int4			*pid;
int4			*idx;

{
	int		n;		/* # selected processes */
/*
 * Determine the load module filename of the existing process.
 */
	n = rpstate(srcnode, sflags, svalue, &ps, 1);

	if (n < 0) {
		return(LAMERROR);
	} else if (n < 1) {
		errno = EIMPOSSIBLE;
		return(LAMERROR);
	}
/*
 * Load and execute the program on the destination node.
 */
	return(rploadgo(srcnode, destnode, ps.ps_loadpt,
			ps.ps_rtf & RTF_TRACE, argv, pid, idx));
}
