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
 *	Copyright 1994 The Ohio State University
 *	GDB
 *
 *	$Log: kexit.c,v $
 *	Revision 6.3  2002/10/09 20:56:47  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:36  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:45:15  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:32:12  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  13:51:14  gdburns
 * Ohio Release
 * 
 * Revision 5.1.1.1  94/08/18  11:11:44  gdburns
 * overhaul for new kernel
 * 
 * Revision 5.1  94/05/18  10:42:45  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:28:17  gdburns
 * Ohio Release
 * 
 * Revision 2.2  94/02/16  14:22:54  gdburns
 * pre-release to 2.3
 * 
 *	Function:	- exits internal process
 *	Accepts:	- return code
 */

#include <kreq.h>

/*
 * external functions
 */
extern int		_cipc_kreq();		/* make kernel request */
extern int		kbgetpid();

void
kexit(status)

int			status;

{
	struct kreq	req;			/* kernel request */
	struct kreply	reply;			/* kernel reply */
/*
 * Formulate the KQDETACH kernel request.
 */
	req.kq_req = KQDETACH;
	req.kq_pid = kbgetpid();
	_cipc_kreq(&req, &reply);
}
