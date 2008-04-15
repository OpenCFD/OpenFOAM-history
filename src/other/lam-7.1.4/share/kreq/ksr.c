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
 *	$Id: ksr.c,v 6.6 2002/10/09 20:57:47 brbarret Exp $
 * 
 *	Function:	- atomic send/recv
 *			- sends a message and then receives a message
 *			  without becoming ready in between
 *	Accepts:	- kernel send message descriptor ptr
 *			- recv message ptr
 *	Returns:	- 0 or ERROR
 */

#include <errno.h>

#include <kio.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>
#include <laminternal.h>

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
ksr(pkmsg, pkmsg2)

struct kmsg		*pkmsg;		/* send message desc. ptr */
struct kmsg		*pkmsg2;	/* recv message desc. ptr */

{
	int4		r;
	int4		sv_length;

	sv_length = pkmsg->k_length;
/*
 * Loop through possible signal interruptions on the back end.
 */
	do {
		pkmsg->k_length = sv_length;

		if (ksrfront(pkmsg, pkmsg2) < 0) {
			return(LAMERROR);
		}

		r = ksrback(pkmsg2);

	} while ((r > 0) && ((r & _kio.ki_sigretry) == r));

	if (r > 0) {
		errno = EINTR;
		return(LAMERROR);
	} else if (r < 0) {
		return(LAMERROR);
	}

	return(0);
}

/*
 *	ksrfront
 *
 *	Function:	- front portion of kernel message send/receive
 *	Accepts:	- kernel send message descriptor ptr
 *			- recv message ptr
 *	Returns:	- kernel-client socket descriptor or ERROR
 */
int
ksrfront(pkmsg, pkmsg2)

struct kmsg		*pkmsg;		/* send message desc. ptr */
struct kmsg		*pkmsg2;	/* recv message desc. ptr */

{
	int		r;
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */

/*
 * Formulate the KQSR request.
 */
	LAM_ZERO_ME(req);
	LAM_ZERO_ME(reply);
	req.kq_req = KQSR;
	req.kq_index = _kio.ki_index;
	req.kq_msg.k_event = pkmsg->k_event;
	req.kq_msg.k_type = pkmsg->k_type;
	req.kq_msg.k_flags = pkmsg->k_flags;
	req.kq_msg.k_length = pkmsg->k_length;
	req.kq_msg.k_msg = pkmsg->k_msg;

	req.kq_msg2.k_event = pkmsg2->k_event;
	req.kq_msg2.k_type = pkmsg2->k_type;
	req.kq_msg2.k_flags = pkmsg2->k_flags;
	req.kq_msg2.k_length = pkmsg2->k_length;
	req.kq_msg2.k_msg = pkmsg2->k_msg;
/*
 * Loop through possible signal interruptions.
 */
	do {
		if ((r = _cipc_ksrfront(&req, &reply)) < 0) {
			return(LAMERROR);
		}

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
	return(r);
}

/*
 *	ksrback
 *
 *	Function:	- back portion of kernel message send/receive
 *	Accepts:	- kernel message descriptor ptr
 *	Returns:	- signal if request was interrupted
 *			- 0 or ERROR or signal
 */
int4
ksrback(pkmsg2)

struct kmsg		*pkmsg2;

{
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */

	req.kq_msg2.k_flags = pkmsg2->k_flags;
	req.kq_msg2.k_msg = pkmsg2->k_msg;

	if (_cipc_ksrback(&req, &reply)) {
		return(LAMERROR);
	}

	if (pkmsg2->k_flags & KTRACE) {
		_kio.ki_blktime += reply.kr_blktime;
	}
/*
 * Do we need to follow a signal?
 */
	if (reply.kr_signal) {
		_kio.ki_signal |= reply.kr_signal;
		_ksig_follow();

		if (reply.kr_reply == EINTR) {
			return(reply.kr_signal);
		}
	}

	if (reply.kr_reply) {
		errno = reply.kr_reply;
		return(LAMERROR);
	}

	if (pkmsg2->k_flags & KPROBE) {
		pkmsg2->k_event = reply.kr_event;
	}

	pkmsg2->k_type = reply.kr_type;
	pkmsg2->k_length = reply.kr_length;
	return(0);
}
