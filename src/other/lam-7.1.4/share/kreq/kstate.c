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
 *	$Id: kstate.c,v 6.5 2002/10/09 20:57:47 brbarret Exp $
 * 
 *	Function:	- obtains process status from kernel
 *	Accepts:	- process ID
 *			- kernel status structure ptr
 *	Returns:	- 0 or ERROR
 */

#include <errno.h>
#include <string.h>

#include <kio.h>
#include <kreq.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t	_kio;		/* kernel I/O block */

/*
 * external functions
 */
extern int		_cipc_kreq();	/* make kernel request */

int
kstate(pid, pstate)

int			pid;
struct kstatus		*pstate;

{
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */
/*
 * Formulate the KQSTATE kernel request.
 */
	req.kq_req = KQSTATE;
	req.kq_index = _kio.ki_index;
	req.kq_pid = pid;

	if (_cipc_kreq(&req, &reply)) {
		return(LAMERROR);
	}

	if (reply.kr_reply) {
		errno = reply.kr_reply;
		return(LAMERROR);
	}

	pstate->ks_state = reply.kr_state;
	pstate->ks_priority = reply.kr_priority; 
	pstate->ks_event = reply.kr_event;
	pstate->ks_type = reply.kr_type;
	pstate->ks_length = reply.kr_length;
	pstate->ks_signal = reply.kr_sigs;
	memcpy((char *) pstate->ks_fyi, (char *) reply.kr_fyi,
			sizeof(reply.kr_fyi));

	return(0);
}
