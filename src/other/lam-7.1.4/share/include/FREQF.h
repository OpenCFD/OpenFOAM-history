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
 *      LAM	
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *	$Log: FREQF.h,v $
 *	Revision 6.2  2002/10/09 20:57:21  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.1  1999/09/02 01:34:37  jsquyres
 *	Changes to make LAM NT/Cygwin friendly
 *	- Renamed MPISYS.h, NET.h, and FREQ.h to be MPISYSF.h, NETF.h, and FREQF.h,
 *	  respectively, because NT filesystems do not have case-sensitive names,
 *	  and we already have <mpisys.h>, <net.h>, and <freq.h>.
 *	
 *	Revision 6.2  1999/05/18 22:12:58  kmeyer1
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
 *	Revision 6.1  1996/11/23 21:54:31  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:52:52  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  95/10/18  15:44:17  gdburns
 * Remove Cubix and backwards compatibility.
 * 
 * Revision 5.2  94/08/22  14:00:19  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:44:10  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:48:34  gdburns
 * Ohio Release
 * 
 *	Function:	- open flags, suitable for inclusion into F77 programs
 */

#ifndef _FREQF
#define _FREQF

#define	LAM_O_RDONLY		(1)
#define	LAM_O_WRONLY		(2)
#define	LAM_O_RDWR		(4)
#define	LAM_O_APPEND		(16)
#define	LAM_O_CREAT		(32)
#define	LAM_O_TRUNC		(64)
#define	LAM_O_EXCL		(128)
#define	LAM_O_LOCK		(65536)
#define	LAM_O_REUSE		(131072)
#define	LAM_O_DRAW		(0)
#define	LAM_O_DINT4		(2097152)
#define	LAM_O_DFLT4		(4194304)
#define	LAM_O_DFLT8		(8388608)
#define	LAM_O_1WAY		(16777216)

#endif /* _FREQF */
