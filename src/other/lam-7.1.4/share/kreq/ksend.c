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
 *	Function:	- sends a message to another process
 *	Accepts:	- kernel message descriptor ptr
 *	Returns:	- 0 or ERROR
 */

#include <kio.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t	_kio;		/* kernel I/O block */

/*
 * external functions
 */
extern int		_cipc_ksend();	/* ITB/OTB msg send */
extern void		_ksig_follow();	/* check signals */

int
ksend(pkmsg)

struct kmsg		*pkmsg;		/* message desc. ptr */

{
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */
/*
 * Formulate the KQSEND request.
 */
	LAM_ZERO_ME(req);
	LAM_ZERO_ME(reply);
	req.kq_req = KQSEND;
	req.kq_index = _kio.ki_index;
	req.kq_msg.k_event = pkmsg->k_event;
	req.kq_msg.k_type = pkmsg->k_type;
	req.kq_msg.k_flags = pkmsg->k_flags;
	req.kq_msg.k_length = pkmsg->k_length;
	req.kq_msg.k_msg = pkmsg->k_msg;
/*
 * Loop through possible signal interruptions.
 */
	do {
		if (_cipc_ksend(&req, &reply)) return(LAMERROR);

		if (pkmsg->k_flags & KTRACE) {
			_kio.ki_blktime += reply.kr_blktime;
		}
/*
 * Do we need to follow a signal?
 */
		if (reply.kr_signal) {
			_kio.ki_signal |= reply.kr_signal;
			_ksig_follow();
		}

	} while ((reply.kr_reply == EINTR) &&
			((reply.kr_signal & _kio.ki_sigretry) ==
			reply.kr_signal));

	if (reply.kr_reply) {
		errno = reply.kr_reply;
		return(LAMERROR);
	}

	pkmsg->k_length = reply.kr_length;
	return(0);
}
