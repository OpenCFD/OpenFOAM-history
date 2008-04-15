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
 *
 *	$Log: _tiob.c,v $
 *	Revision 6.3  2002/10/09 21:00:53  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/05/27 20:07:04  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/24 00:35:47  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  14:06:24  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  14:17:29  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:58:42  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:46:49  gdburns
 * Ohio Release
 * 
 * Revision 2.2  93/05/10  16:36:11  raja
 * pre-release to 2.3
 * 
 *	We are just putting our stamp on this for housekeeping reasons.
 */

/*
 * This file is originally from dLibs 1.2 by Dale Schumacher et al.
 * It was modified for Trollius by MB, 88/12.
 * It was modified again for portability by GDB, 91/08.
 * And yet again by JRV for LAM, 93/05.
 */

/*
 * (MB) This file only holds the _iob array,
 * plus added flag _ioexit that is set when we call atkexit().
 */

/*
 * Look out:	we added _TIODEV to the flags for stdin/stdout/stderr:
 * 		and also _TIOREMOTE to indicate a pass through for rf*();
 */

#include <tstdio.h>

/*
 * global variables
 */
int			_ioexit = 0;

/*
 * stream buffers
 */
TFILE			_tiob[_TNFILE] = {

/*		{cnt, ptr, base, flag, file, bsiz, ch}  */

/* tstdin */
{0, NULL, NULL, (_TIOREAD | _TIOLBF | _TIODEV | _TIOREMOTE), 0,  0,  '\0'},

/* tstdout */
{0, NULL, NULL, (_TIOWRT  | _TIOLBF | _TIODEV | _TIOREMOTE), 1,  0,  '\0'},

/* tstderr */
{0, NULL, NULL, (_TIOWRT  | _TIONBF | _TIODEV | _TIOREMOTE), 2,  0,  '\0'},

/* 3 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 4 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 5 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 6 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 7 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 8 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 9 */		{0, NULL, NULL, 0, 0, 0, '\0'},
/* 10 */	{0, NULL, NULL, 0, 0, 0, '\0'},
/* 11 */	{0, NULL, NULL, 0, 0, 0, '\0'},
/* 12 */	{0, NULL, NULL, 0, 0, 0, '\0'},
/* 13 */	{0, NULL, NULL, 0, 0, 0, '\0'},
/* 14 */	{0, NULL, NULL, 0, 0, 0, '\0'},
/* 15 */	{0, NULL, NULL, 0, 0, 0, '\0'},
};
