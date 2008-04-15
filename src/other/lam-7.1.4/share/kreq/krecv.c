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
 *	$Id: krecv.c,v 6.5 2002/10/29 02:11:39 brbarret Exp $
 * 
 *	Function:	- kernel message receive
 */

#include <errno.h>

#include <kio.h>
#include <kreq.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t	_kio;			/* kernel I/O block */

/*
 * external functions
 */
extern int		_cipc_krecvback();	/* ITB/OTB msg recv */
extern int		_cipc_krecvfront();	/* ITB/OTB msg recv */
extern void		_ksig_follow();		/* check signals */

/*
 *	krecv
 *
 *	Function:	- receives a message from another process
 *			- retries kernel request after signal interruption
 *			  or transfer timeout
 *	Accepts:	- kernel message descriptor ptr
 *	Returns:	- 0 or ERROR
 */
int
krecv(struct kmsg *pkmsg)
{
	int4		r;
/*
 * Loop through possible signal interruptions.
 */
	do {
		if (krecvfront(pkmsg) < 0) {
			return(LAMERROR);
		}

		r = krecvback(pkmsg);

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
 *	krecvfront
 *
 *	Function:	- request portion of kernel message receive
 *	Accepts:	- kernel message descriptor ptr
 *	Returns:	- kernel-client socket descriptor or ERROR
 */
int
krecvfront(struct kmsg *pkmsg)
{
	struct kreq	req;		/* kernel request */
/*
 * Formulate the KQRECV request.
 */
	LAM_ZERO_ME(req);
	req.kq_req = KQRECV;
	req.kq_index = _kio.ki_index;
	req.kq_msg.k_event = pkmsg->k_event;
	req.kq_msg.k_type = pkmsg->k_type;
	req.kq_msg.k_flags = pkmsg->k_flags;
	req.kq_msg.k_length = pkmsg->k_length;
	req.kq_msg.k_msg = pkmsg->k_msg;

	return(_cipc_krecvfront(&req));
}

/*
 *	krecvback
 *
 *	Function:	- reply portion of kernel message receive
 *	Accepts:	- kernel message descriptor ptr
 *	Returns:	- signal if request was interrupted
 *			- 0 or ERROR or signal
 */
int4
krecvback(struct kmsg *pkmsg)
{
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */

	req.kq_msg.k_flags = pkmsg->k_flags;
	req.kq_msg.k_msg = pkmsg->k_msg;

	if (_cipc_krecvback(&req, &reply)) {
		return(LAMERROR);
	}

	if (pkmsg->k_flags & KTRACE) {
		_kio.ki_blktime += reply.kr_blktime;
	}

	if (reply.kr_signal) {
		_kio.ki_signal |= reply.kr_signal;
		_ksig_follow();

		if (reply.kr_reply == EINTR) {
			errno = EINTR;
			return(reply.kr_signal);
		}
	}

	if (reply.kr_reply) {
		errno = reply.kr_reply;
		return(LAMERROR);
	}

	pkmsg->k_type = reply.kr_type;
	pkmsg->k_length = reply.kr_length;
	return(0);
}
