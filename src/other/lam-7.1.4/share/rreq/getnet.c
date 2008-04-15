/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 *	Function:	- obtains basic network information
 */

#include <kio.h>
#include <net.h>
#include <rreq.h>
#include <etc_misc.h>


/*
 * external variables
 */
extern struct kio_t	_kio;		/* kernel interface */

/*
 *	getnodeid
 *
 *	Returns:	- local node identifier
 */
int4
getnodeid(void)
{
	if (_kio.ki_pid != lam_getpid()) {
	 	_kio.ki_nodeid = NOTNODEID;
	}

	if (_kio.ki_nodeid == NOTNODEID) {
		getnodes(&_kio.ki_nodeid, 1, NT_ME, NT_ME);
	}

	return(_kio.ki_nodeid);
}

/*
 *	getorigin
 *
 *	Returns:	- origin node identifier
 */
int4
getorigin(void)
{
	if (_kio.ki_pid != lam_getpid()) {
	 	_kio.ki_origin = NOTNODEID;
	}

	if (_kio.ki_origin == NOTNODEID) {
		getnodes(&_kio.ki_origin, 1, NT_ORIGIN, NT_ORIGIN);
	}

	return(_kio.ki_origin);
}

/*
 *	getnodetype
 *
 *	Returns:	- local node type
 */
int4
getnodetype(void)
{
	return(getrtype(getnodeid()));
}
