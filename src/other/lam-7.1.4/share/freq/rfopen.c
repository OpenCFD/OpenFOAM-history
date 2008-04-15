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
 *	$Id: rfopen.c,v 6.5 2002/10/09 20:57:14 brbarret Exp $
 *
 *	Function:	- opens a file using remote filed
 *			- server nodeid may be specified by prefixing
 *			  filename with <nodeid>: as in 100:temp
 *	Accepts:	- file name
 *			- open flags
 *			- mode for newly created files
 *	Returns:	- remote file descriptor
 */

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <events.h>
#include <freq.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct fclient	_ufd[FUMAX];	/* client file descriptor table */

/*
 * external functions
 */
extern char		*_fnparse();	/* parse nodeid from file name */

int
lam_rfopen(name, flags, mode)

CONST char		*name;
int4			flags;
int4			mode;

{
	struct nmsg	nhead;		/* network message desc. */
	struct freq	*request;	/* filed request */
	struct freply	*reply;		/* filed reply */
	int		mask;		/* signal mask */
	int		fd;		/* new file descriptor (ufd) */
/*
 * Find slot in client fd table.
 */
	for (fd = 3; (fd < FUMAX) && (_ufd[fd].fu_tflags != 0); ++fd);

	if (fd >= FUMAX) {
		errno = ENFILE;
		return(LAMERROR);
	}
/*
 * Determine server node ID.
 */
	name = _fnparse(name, &nhead.nh_node);
		
	request = (struct freq *) nhead.nh_data;
	reply = (struct freply *) nhead.nh_data;

	request->fq_src_node = getnodeid();
	request->fq_src_event = -lam_getpid();
	request->fq_req = FQOPEN;
	request->fq_mode = mode;
	request->fq_flags = flags;

	nhead.nh_event = EVFILED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = strlen(name) + 1;

	if (nhead.nh_length > MAXNMSGLEN) {
		errno = ENAMETOOLONG;
		return(LAMERROR);
	}

	nhead.nh_msg = (char *) name;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	nhead.nh_event = -lam_getpid();
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}

	ksigsetmask(mask);

	if (reply->fr_errno != 0) {
		errno = reply->fr_errno;
		return(LAMERROR);
	}

	else {
		_ufd[fd].fu_tflags = flags;
		_ufd[fd].fu_node = nhead.nh_node;
		_ufd[fd].fu_tfd = reply->fr_ret;
		return(fd);
	}
}

/*
 * backwards compatibility
 */
int rfopen(name, flags, mode) CONST char *name; int4 flags, mode;
	{ return(lam_rfopen(name, flags, mode)); }
