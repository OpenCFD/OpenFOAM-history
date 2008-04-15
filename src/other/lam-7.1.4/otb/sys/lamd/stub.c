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
 *	$Log: stub.c,v $
 *	Revision 6.3  2002/10/09 20:56:47  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:37  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:45:23  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:32:15  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  95/11/21  14:12:56  gdburns
 * Return zero in integer functions.
 * 
 * Revision 5.2  94/08/22  13:51:17  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  10:42:50  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:28:21  gdburns
 * Ohio Release
 * 
 *	Function:	- stubs for internal processes
 */

void
_ksig_follow() {}

int
ksigblock() { return(0); }

int
ksigsetmask() { return(0); }
