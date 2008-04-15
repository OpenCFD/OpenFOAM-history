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
 *	$Log: di_wait.int.c,v $
 *	Revision 6.6  2002/10/09 20:56:47  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.5  2000/02/08 10:29:33  jsquyres
 *	Duh.  Remove the mail headers before trying to compile...
 *	
 *	Revision 6.4  2000/02/08 10:28:37  jsquyres
 *	First take at automake-izing LAM/MPI.  Cross your fingers!
 *	
 *	Revision 6.2  1999/06/12 17:11:35  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:45:11  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:32:10  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  96/02/26  20:58:38  gdburns
 * Don't check return value of void kio_intr().
 * 
 * Revision 5.2  94/08/22  13:51:13  gdburns
 * Ohio Release
 * 
 * Revision 5.1.1.1  94/08/18  11:11:41  gdburns
 * overhaul for new kernel
 * 
 * Revision 5.1  94/05/18  10:42:45  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:28:00  gdburns
 * Ohio Release
 * 
 *	Function:	- waits for network frame message
 *			- registers interrupt function and returns
 *			  to kernel in an 'R' state
 *	Accepts:	- Trollius interrupt number
 *			- socket descriptor
 */

#include <typical.h>

/*
 * external functions
 */
extern int		kio_intr();
extern void		di_forward();

void
di_wait(sockd)

int			sockd;

{
	kio_intr(sockd, di_forward, TRUE);
}
