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
 *	$Id: couter.c,v 6.4 2002/10/09 20:56:47 brbarret Exp $
 * 
 *	Function:	- OTB internal process client protocols
 */

#include <stdlib.h>
#include <string.h>

#include <all_queue.h>
#include <kreq.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kproc	*prun;		/* current running client */

/*
 * external functions
 */
extern void		kio_cleanup();	/* free resources */
extern void		ksnap();	/* kernel state request */
extern int		ksync();	/* look ahead at sync */

/*
 *	_cipc_ksend
 *
 *	Function:	- internal kernel message send
 *			- enqueues request in process descriptor
 *			- copies the message body
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0
 */
int
_cipc_ksend(preq, preply)

struct kreq		*preq;
struct kreply		*preply;

{
	char		*p;
	char		**ppmsg;	/* ptr to msg body ptr */
/*
 * Check for non-blocking service.
 */
	if (preq->kq_msg.k_flags & KTRY) {

		if (! ksync(&preq->kq_msg, KQSEND)) {
/*
 * Enqueue the kernel request even though we know it is a NOP,
 * so that KPROBE will be satisfied.
 */
			if (aq_shove(prun->kp_kqueue, preq))
					lampanic("lamd (aq_shove)");
			errno = EWOULDBLOCK;
			return(LAMERROR);
		}
	}
/*
 * Copy network message header.
 */
	if (preq->kq_msg.k_flags & KNMSG) {
		p = malloc(sizeof(struct nmsg));
		if (!p) lampanic("lamd (malloc)");

		memcpy(p, preq->kq_msg.k_msg, sizeof(struct nmsg));
		preq->kq_msg.k_msg = p;
		ppmsg = &(((struct nmsg *) p)->nh_msg);
	} else {
		ppmsg = &(preq->kq_msg.k_msg);
	}
/*
 * Copy message body.
 */
	if (preq->kq_msg.k_length > 0) {
		p = malloc((unsigned) preq->kq_msg.k_length);
		if (!p) lampanic("lamd (malloc)");

		memcpy(p, *ppmsg, preq->kq_msg.k_length);
		*ppmsg = p;
	}
/*
 * Enqueue the kernel request.
 */
	if (aq_shove(prun->kp_kqueue, preq)) lampanic("lamd (aq_shove)");
/*
 * Fake the reply.
 */
	preply->kr_reply = 0;
	preply->kr_signal = 0;
	preply->kr_length = preq->kq_msg.k_length;
	preply->kr_type = 0;
	return(0);
}

/*
 *	_cipc_krecvfront
 *
 *	Function:	- internal kernel message receive
 *			- enqueues request in process descriptor
 *	Accepts:	- kernel request ptr
 *	Returns:	- 0
 */
int
_cipc_krecvfront(preq)

struct kreq		*preq;

{
	if (aq_shove(prun->kp_kqueue, preq)) lampanic("lamd (aq_shove)");
	return(0);
}

/*
 *	_cipc_krecvback
 *
 *	Function:	- internal kernel message receive
 *			- builds fake reply
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0
 */
int
_cipc_krecvback(preq, preply)

struct kreq		*preq;
struct kreply		*preply;

{
	preply->kr_reply = 0;
	preply->kr_signal = 0;
	preply->kr_length = 0;
	preply->kr_type = 0;
	return(0);
}

/*
 *	_cipc_ksrfront
 *
 *	Function:	- internal kernel message send/receive
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0
 */
int
_cipc_ksrfront(preq, preply)

struct kreq		*preq;
struct kreply		*preply;

{
	return(_cipc_ksend(preq, preply));
}

/*
 *	_cipc_ksrback
 *
 *	Function:	- internal kernel message send/receive
 *			- builds fake reply
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 */
int
_cipc_ksrback(preq, preply)

struct kreq		*preq;
struct kreply		*preply;

{
	preply->kr_reply = 0;
	preply->kr_signal = 0;
	preply->kr_length = 0;
	preply->kr_type = 0;
	return(0);
}

/*
 *	_cipc_req
 *
 *	Function:	- internal kernel request
 *			- enqueues request in process descriptor
 *	Accepts:	- kernel request ptr
 *			- kernel reply ptr
 *	Returns:	- 0
 */
int
_cipc_kreq(preq, preply)

struct kreq		*preq;
struct kreply		*preply;

{
	if (preq->kq_req == KQSTATE) {
		ksnap(preq, preply);
	} else {
/*
 * Enqueue the kernel request.
 */
		if (aq_shove(prun->kp_kqueue, preq))
				lampanic("lamd (aq_shove)");
/*
 * Fake the reply.
 */
		preply->kr_reply = 0;
	}

	preply->kr_signal = 0;
	return(0);
}

/*
 *	_cipc_cleanup
 *
 *	Function:	- frees internal client and kernel resources
 */
void
_cipc_cleanup()

{
	kio_cleanup();
}
