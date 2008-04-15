/*
 * Copyright (c) 2001-2003 The Trustees of Indiana University.  
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
 * $Id: kinner.c,v 1.9 2003/05/21 19:16:15 jsquyres Exp $
 * 
 *	Function:	- simple message passing kernel
 *			- user level kernel requests
 */

#include <stdlib.h>
#include <string.h>

#include <lamdebug.h>
#include <kreq.h>
#include <terror.h>

/*
 * external variables
 */
extern struct kproc	*pready;	/* top of ready list */
extern struct kproc	*prun;		/* current running client */

/*
 * external functions
 */
extern void		kio_reply();	/* reply to client */
extern void		kpreempt();	/* preempt client */
extern int		transfer();	/* message from recver to sender */
extern struct kproc	*kpfind();	/* find kernel process */
extern double		ttime();

/*
 * global variables
 */
struct kproc		*pblock = 0;	/* top of blocked list */

/*
 * global functions
 */
void kpinsert(struct kproc **pptop, struct kproc *pnew);
void			kpdelete();
void			kqdoom();
void			kqsync();
void			ksnap();
int			ksync();

/*
 *	kqsync
 *
 *	Function:	- synchronizes two processses
 *	Accepts:	- client process
 *			- client request
 */
void
kqsync(pclient, pkq)

struct kproc		*pclient;
struct kreq		*pkq;

{
	struct kreply	reply;		/* kernel reply */
	struct kproc	*p;		/* favourite ptr */
	struct kproc	*pprev;		/* previous list entry ptr */
	int4		syncstate;	/* matching state */
	int		bsync;		/* sync'd yet? */
	double		t;

	lam_comm_debug_local_cond((1,
				   "kernel (%s): "
				   "pid=%d event=%x type=%x length=%d\n",
			(pkq->kq_req == KQSEND) ? "kqsend" :
			((pkq->kq_req == KQRECV) ? "kqrecv" : "kqsr"),
			pclient->kp_pid, pkq->kq_msg.k_event,
			pkq->kq_msg.k_type, pkq->kq_msg.k_length));
/*
 * Is there a signal pending?
 */
	LAM_ZERO_ME(reply);
	if (pclient->kp_signal) {
		reply.kr_reply = EINTR;
		reply.kr_signal = pclient->kp_signal;
		reply.kr_blktime = 0;
		pclient->kp_signal = 0;
		kio_reply(&reply, pclient->kp_port);
		return;
	}
/*
 * Look down the blocked list for a synchronizing process.
 */
	syncstate = (pkq->kq_req == KQRECV) ? KSSEND : KSRECV;
	bsync = 0;
	p = pblock;
	pprev = p;

	while ((p != 0) && !bsync) {
		bsync = (p->kp_state == syncstate) &&
				ksync_cmp(p, pkq->kq_msg);

		if (! bsync) {
			pprev = p;
			p = p->kp_next;
		}
	}
/*
 * If a synchronizing blocked process is found, insert it
 * into the ready list.  Transfer the message.
 */
	memcpy((char *) &pclient->kp_msg, (char *) &pkq->kq_msg,
			sizeof(struct kmsg));
/*
 * KPROBE is triggered by the blocked process.  KPROBE will only
 * be used on the second half of a KQSR and only by external processes.
 */
	if (bsync && (p->kp_msg.k_flags & KPROBE)) {
		reply.kr_signal = 0;
		reply.kr_reply = 0;
		reply.kr_event = pclient->kp_msg.k_event;
		reply.kr_type = pclient->kp_msg.k_type;
		reply.kr_length = (pclient->kp_msg.k_length <
				p->kp_msg.k_length) ?
				pclient->kp_msg.k_length : p->kp_msg.k_length;

		if (p->kp_msg.k_flags & KTRACE) {
			p->kp_blktime = ttime() - p->kp_blktime;
			reply.kr_blktime = p->kp_blktime;
		}

		kio_reply(&reply, p->kp_port);
		p->kp_state = KSREADY;
		kpdelete(&pblock, p, pprev);
		kpinsert(&pready, p);
		bsync = 0;
	}

	if (bsync) {
		pclient->kp_blktime = 0;
		t = p->kp_blktime;

		if (p->kp_msg.k_flags & KTRACE) {

			if (p->kp_flags & KMSG2) {
				p->kp_blktime = 0;
			} else {
				p->kp_blktime = ttime() - p->kp_blktime;
			}
		}

		if (pkq->kq_req == KQRECV) {
			if (transfer(p, pclient)) return;
		} else {
			if (transfer(pclient, p)) return;
		}

		if (p->kp_flags & KMSG2) {
			p->kp_state = KSRECV;
			p->kp_flags &= ~KMSG2;
			p->kp_blktime = t;
			memcpy((char *) &p->kp_msg, (char *) &p->kp_msg2,
					sizeof(struct kmsg));
		} else {
			p->kp_state = KSREADY;
			kpdelete(&pblock, p, pprev);
			kpinsert(&pready, p);
		}

		if (pkq->kq_req == KQSR) {
			pclient->kp_state = KSRECV;
			kpdelete(&pready, pclient, (struct kproc *) 0);
			kpinsert(&pblock, pclient);
			memcpy((char *) &pclient->kp_msg,
					(char *) &pkq->kq_msg2,
					sizeof(struct kmsg));
			memcpy((char *) pclient->kp_fyi, (char *) pkq->kq_fyi,
					sizeof(pkq->kq_fyi));

			if (pclient->kp_msg.k_flags & KTRACE) {
				pclient->kp_blktime = ttime();
			}
		} else {
			pclient->kp_state = KSREADY;
		}
	}
/*
 * The client does not synchronize.
 */
	else if (pkq->kq_msg.k_flags & KTRY) {
		reply.kr_signal = 0;
		reply.kr_reply = EWOULDBLOCK;
		reply.kr_blktime = 0;

		if (!(pclient->kp_flags & KIPROC)) {
			kio_reply(&reply, pclient->kp_port);
		}
	} else {
		kpdelete(&pready, pclient, (struct kproc *) 0);
		kpinsert(&pblock, pclient);

		pclient->kp_state = (pkq->kq_req == KQRECV) ?
				KSRECV : KSSEND;
		memcpy((char *) pclient->kp_fyi, (char *) pkq->kq_fyi,
				sizeof(pkq->kq_fyi));

		if (pkq->kq_req == KQSR) {
			pclient->kp_flags |= KMSG2;
			memcpy((char *) &pclient->kp_msg2,
					(char *) &pkq->kq_msg2,
					sizeof(struct kmsg));
		}

		if (pclient->kp_msg.k_flags & KTRACE) {
			pclient->kp_blktime = ttime();
		}
	}
}

/*
 *	kqstate
 *
 *	Function:	- replies state of given process identifier
 *			- assumes client will be scheduled immediately
 *	Accepts:	- client process
 *			- client request
 */
void
kqstate(pclient, pkq)

struct kproc		*pclient;
struct kreq		*pkq;

{
	struct kreply	reply;		/* kernel reply */

	ksnap(pkq, &reply);
	kio_reply(&reply, pclient->kp_port);
}

/*
 *	kqdoom
 *
 *	Function:	- delivers a signal to a process
 *	Accepts:	- client process
 *			- client request
 */
void
kqdoom(pclient, pkq)

struct kproc		*pclient;
struct kreq		*pkq;

{
	struct kproc	*pdoom;		/* signalled process */
	struct kreply	reply;		/* kernel reply */

	LAM_ZERO_ME(reply);
	lamlog_cond(("kernel (kqdoom): pid=%d target=%d signal=%d\n", 
		     pclient->kp_pid,
		     pkq->kq_pid, pkq->kq_signal));

	if ((pdoom = kpfind(pkq->kq_pid)) == 0) {
		reply.kr_reply = ENOTPROCESS;
		reply.kr_signal = 0;
	} else if (pdoom->kp_flags & KIPROC) {
		reply.kr_reply = ESERVICE;
		reply.kr_signal = 0;
	} else if (pdoom == pclient) {
		reply.kr_reply = 0;
		reply.kr_signal = pkq->kq_signal;
	} else {

		if (pdoom->kp_state == KSREADY) {
			pdoom->kp_signal |= pkq->kq_signal;
			kpreempt(pdoom);
		} else {
			reply.kr_reply = EINTR;
			reply.kr_signal = pkq->kq_signal;
			kpdelete(&pblock, pdoom, (struct kproc *) 0);
			kpinsert(&pready, pdoom);
			pdoom->kp_state = KSREADY;

			if (pdoom->kp_msg.k_flags & KTRACE) {
				pdoom->kp_blktime = ttime() -
						pdoom->kp_blktime;
				reply.kr_blktime = pdoom->kp_blktime;
			}

			kio_reply(&reply, pdoom->kp_port);
			reply.kr_signal = 0;
		}

		reply.kr_reply = 0;
		reply.kr_signal = 0;
	}

	if (!(pclient->kp_flags & KIPROC)) {
		kio_reply(&reply, pclient->kp_port);
	}
}

/*
 *	kpinsert
 *
 *	Function:	- inserts a process descriptor into a list
 *			- list sorted by priority
 *	Accepts:	- top of list ptr ptr
 *			- new element ptr
 */
void
kpinsert(struct kproc **pptop, struct kproc *pnew)
{
	struct kproc	*pcurr;		/* current chase pointer */
	struct kproc	*pprev;		/* previous chase pointer */

	pcurr = pprev = *pptop;

	while (pcurr && (pnew->kp_priority <= pcurr->kp_priority)) {
	  /* Make sure that we are not adding an existing process back
             into the list.  Since we put at the end of the priority
             setting, we can be sure that we will see the process (if
             it is here) before we exit this while loop */
	  if (pnew == pcurr) {
	    lamlog("Process %d (state %d) trying to add to process list twice",
		   pnew->kp_pid, pnew->kp_state);
	    return;
	  }
		pprev = pcurr;
		pcurr = pcurr->kp_next;
	}

	pnew->kp_next = pcurr;

	if (pcurr == *pptop) {
		*pptop = pnew;
	} else {
		pprev->kp_next = pnew;
	}
}

/*
 *	kpdelete
 *
 *	Function:	- deletes a process descriptor from a list
 *			- O(1) deletion if previous element ptr is
 *			  provided (non-null), else linear search
 *	Accepts:	- top of list ptr ptr
 *			- old element ptr
 *			- previous element (to old) ptr
 */
void
kpdelete(pptop, pold, pprev)

struct kproc		**pptop;
struct kproc		*pold;
struct kproc		*pprev;

{
	struct kproc	*pcurr;		/* current chase pointer */
/*
 * If previous ptr is supplied, use it, otherwise find element in list.
 */
	if (!pprev) {
		pcurr = pprev = *pptop;

		while (pcurr && (pcurr != pold)) {
			pprev = pcurr;
			pcurr = pcurr->kp_next;
		}

		if (!pcurr) {
			return;
		}
	}

	if (*pptop == pold) {
		*pptop = pold->kp_next;
	} else {
		pprev->kp_next = pold->kp_next;
	}
}

/*
 *	ksync
 *
 *	Function:	- will message sync?
 *			- direct access for internal processes
 *			- The correct answer is guaranteed only for
 *			  the first such call after a process dispatch.
 *	Accepts:	- kernel message ptr
 *			- KQSEND or KQRECV
 *	Returns:	- 0 or 1
 */
int
ksync(pkmsg, req)

struct kmsg		*pkmsg;
int			req;

{
	struct kproc	*p;
	int4		syncstate;	/* matching state */
	int		bsync;		/* sync'd yet? */
/*
 * Try to make ksync() return the right answer more often than
 * just the guaranteed case.
 */
	if (pkmsg->k_event == prun->kp_msg.k_event) {
		return(0);
	}

	memcpy((char *) &prun->kp_msg, (char *) pkmsg, sizeof(struct kmsg));

	syncstate = (req == KQRECV) ? KSSEND : KSRECV;
	bsync = 0;
	p = pblock;

	while ((p != 0) && !bsync) {
		bsync = (p->kp_state == syncstate) &&
				ksync_cmp(p, *pkmsg);

		if (!bsync) {
			p = p->kp_next;
		}
	}
/*
 * Cancel a synchronization if it matches a KPROBE.
 */
	if (bsync && (p->kp_msg.k_flags & KPROBE)) {
		bsync = 0;
	}

	return(bsync);
}

/*
 *	ksnap
 *
 *	Function:	- process status
 *			- direct access for internal processes
 *	Accepts:	- client process
 *			- request
 *			- reply
 */
void
ksnap(pkq, pkr)

struct kreq		*pkq;
struct kreply		*pkr;

{
	struct kproc	*pstate;	/* monitored process */

	lamlog_cond(("kernel (kqstate): pid=%d target=%d\n", prun->kp_pid,
			pkq->kq_pid));

	if ((pstate = kpfind(pkq->kq_pid)) == 0) {
		pkr->kr_reply = ENOTPROCESS;
	} else {
		pkr->kr_reply = 0;
		pkr->kr_state = pstate->kp_state;
		pkr->kr_priority = pstate->kp_priority;
		pkr->kr_sigs = pstate->kp_signal;
		pkr->kr_event = pstate->kp_msg.k_event;
		pkr->kr_type = pstate->kp_msg.k_type;
		pkr->kr_length = pstate->kp_msg.k_length;
		memcpy((char *) pkr->kr_fyi, (char *) pstate->kp_fyi,
				sizeof(pstate->kp_fyi));
	}

	pkr->kr_signal = 0;
}
