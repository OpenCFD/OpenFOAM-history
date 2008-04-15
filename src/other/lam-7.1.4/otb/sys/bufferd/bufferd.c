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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: bufferd.c,v 1.2 2002/10/09 20:56:30 brbarret Exp $
 * 
 *	Function:	- buffer daemon
 *			- Bufferd receives all messages to be buffered
 *			  as well as control/monitor requests.
 */

#include <stdlib.h>
#include <string.h>

#include <lam_config.h>
#include <all_hash.h>
#include <all_list.h>
#include <bfreq.h>
#include <dl_inet.h>
#include <events.h>
#include <net.h>
#include <portable.h>
#include <priority.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>

typedef struct bfbuf	*bfb_t;

/*
 * local functions
 */
static void		bfchkspace();	/* check space limits */
static void		bfqlook();
static void		bfqprobe();
static void		bfqrecv();
static void		bfqstate();
static void		bfqsweep();
static void		bfqsweep1();
static void		bfqspace();
static void		bfrem_msg();	/* removes one buffered message */
static void		bfsweepev();	/* sweep one event */
static void		buffer();	/* buffer incoming msg */
static void		send_msg();	/* send remote message */
static void		seqfree();	/* release sequence # */
static int4		seqnext();	/* allocates next sequence # */

/*
 * local variables
 */
static int4		filter;		/* filter bufferd requests */
static int4		maxspace;	/* max amount of buffer space */
static int4		nspace;		/* amount of buffer space */

static char		msg[MAXNMSGLEN];/* incoming message body */

static uint4		*seq;		/* sequence bits */
static uint4		seqlen;		/* sequence array length in bytes */
static uint4		seqmax;		/* sequence array length in bits */
static int4		seqcurr;	/* current seq # */

static void		(*bfq[BFQMAX])() = {
				bfqsweep,
				bfqrecv,
				bfqprobe,
				bfqstate,
				bfqsweep,
				bfqsweep1,
				bfqspace,
				bfqlook,
			};		/* request functions */

static HASH		*bfhp;		/* buffered message hash table */
static struct nmsg	bfincoming;	/* incoming msg */

/*
 * global functions
 */
void			(*(bf_init()))();
void			(*(bufferd()))();

/*
 *	bf_init
 *
 *	Function:	- bufferd initialization
 */
void (*(
bf_init()))()

{
	int		i;
/*
 * Initialize lists.
 */
	bfhp = ah_init(BFNEV, sizeof(struct bfevent), NOTEVENT, 0);
/*
 * Initialize limits, filters and flags.
 */
	maxspace = BFDEFSPACE;
	nspace = 0;
	filter = DLCTL;

	if (maxspace >= MAXNMSGLEN) {
		filter = 0;
	}

	seqcurr = 0;
	seqlen = BFSEQLEN;
	seqmax = BFSEQLEN * 32;
	seq = (uint4 *) malloc(seqlen * sizeof(int4));
	if (seq == 0) lampanic("bufferd (malloc)");

	for (i = 0; i < BFSEQLEN; ++i) seq[i] = 0;
/*
 * Receive first request.
 */
	bfincoming.nh_event = EVBUFFERD;
	bfincoming.nh_type = filter;
	bfincoming.nh_flags = 0;
	bfincoming.nh_length = MAXNMSGLEN;
	bfincoming.nh_msg = msg;

	if (frecv(&bfincoming)) lampanic("bufferd (frecv)");

	return((void (*)()) bufferd);
}

/*
 *	bufferd
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(bufferd()))()

{
	struct bfreq	*req;		/* bufferd request */

	req = (struct bfreq *) bfincoming.nh_data;
	bfincoming.nh_msg = msg;
/*
 * Do we have a network or a remote service to perform?
 */
	if ((bfincoming.nh_event == EVBUFFERD) &&
			((bfincoming.nh_node == LOCAL) ||
			(bfincoming.nh_node == getnodeid()))) {

		if ((req->bfq_req >= 0) && (req->bfq_req < BFQMAX)) {
			(*bfq[req->bfq_req])(req);
		}
	} else {
/*
 * Fill in route information.
 */
		if (getroute(&bfincoming)) lampanic("bufferd (getroute)");
		buffer(&bfincoming);
	}

	bfchkspace();

	bfincoming.nh_event = EVBUFFERD;
	bfincoming.nh_type = filter;
	bfincoming.nh_flags = NOBUF;
	bfincoming.nh_length = MAXNMSGLEN;
	bfincoming.nh_msg = msg;

	if (frecv(&bfincoming)) lampanic("bufferd (frecv)");

	return((void (*)()) bufferd);
}

/*
 *	buffer
 *
 *	Function:	- buffers incoming message
 *			- the bread and butter of bufferd
 *	Accepts:	- message descriptor
 */
static void
buffer(bfhead)

struct nmsg		*bfhead;

{
	struct bfevent	newbfevent;		/* new buffered event */
	struct bfevent	*pbfe;		/* buffered event ptr */
	struct bfbuf	wrap;		/* wrap up buffered msg */
	LIST		*nmsglp;	/* net message list */
	char		*temp;		/* a temporary */
/*
 * Don't forward the KTRY, DDLI or KTRACE flags.
 */
	bfhead->nh_flags &= ~(KTRY | DDLI | KTRACE);
/*
 * Check for a blocked receiver.
 */
	if (dtry_send(bfhead)) {

		if (errno != EWOULDBLOCK) {
			lampanic("bufferd (dtry_send)");
		}
	} else {
		return;
	}
/*
 * Buffer the message.
 */
	pbfe = ah_find(bfhp, bfhead->nh_dl_event);

	if (!pbfe) {
		newbfevent.bfe_event = bfhead->nh_dl_event;
		newbfevent.bfe_nmsglp = al_init(sizeof(struct bfbuf), (int (*)()) 0);

		if (ah_insert(bfhp, &newbfevent)) {
			ah_expand(bfhp, 3 * ah_count(bfhp));
			ah_insert(bfhp, &newbfevent);
		}

		nmsglp = newbfevent.bfe_nmsglp;
	} else {
		nmsglp = pbfe->bfe_nmsglp;
	}
/*
 * Buffer the message.
 */
	nspace += sizeof(struct nmsg);
	temp = bfhead->nh_msg;

	if (bfhead->nh_length) {
		bfhead->nh_msg = malloc((unsigned) bfhead->nh_length);
		if (! bfhead->nh_msg) lampanic("bufferd (malloc)");

		memcpy(bfhead->nh_msg, temp, bfhead->nh_length);
		nspace += bfhead->nh_length;
	}

	memcpy((char *) &wrap.bfb_head, (char *) bfhead, sizeof(struct nmsg));
	wrap.bfb_seq = BFNOTSEQ;

	if (al_append(nmsglp, &wrap) == 0) lampanic("bufferd (al_append)");
	bfhead->nh_msg = temp;
}

/*
 *	bfqsweep
 *
 *	Function:	- frees and loses all message list entries
 *			- resets message sequence number
 */
static void
bfqsweep()

{
	struct bfevent	*p;
	struct bfevent	*pnext;			/* next element */
	int		i;

	p = ah_top(bfhp);

	while (p) {
		pnext = ah_next(bfhp, p);
		bfsweepev(p);
		p = pnext;
	}

	seqcurr = 0;
	seqlen = BFSEQLEN;
	seqmax = BFSEQLEN * 32;
	seq = (uint4 *) realloc((char *) seq, (unsigned) (seqlen *
			sizeof(int4)));
	if (seq == 0) lampanic("bufferd (realloc)");

	for (i = 0; i < BFSEQLEN; ++i) seq[i] = 0;
}

/*
 *	bfqsweep1
 *
 *	Function:	- sweeps one event
 */
static void
bfqsweep1(req)

struct bfreq		*req;

{
	struct bfevent	*p;			/* favourite ptr */

	p = ah_find(bfhp, req->bfq_event);

	if (p) {
		bfsweepev(p);
	}
}

/*
 *	bfsweepev
 *
 *	Function:	- frees all messages of one event
 *	Accepts:	- buffered event
 */
static void
bfsweepev(pbfe)

struct bfevent		*pbfe;

{
	struct bfbuf	*pbfb;
	int4		n;

	n = al_count(pbfe->bfe_nmsglp);

	for (; n > 0; --n) {
		pbfb = (bfb_t) al_top(pbfe->bfe_nmsglp);
		bfrem_msg(pbfb, pbfe);
	}
}

/*
 *	bfqstate
 *
 *	Function:	- returns state information on all buffered messages
 *			- returns general bufferd information
 *	Accepts:	- bufferd request
 */
static void
bfqstate(req)

struct bfreq		*req;

{
	struct nmsg	nhead;		/* replies */
	struct nmsg	*p = 0;
	struct bfbuf	*pbfb = 0;	/* buffered msg */
	struct bfstatus	*pbfs = 0;	/* ptr to buffer state */
	struct bfreply	*reply;		/* bufferd reply */
	struct bfevent	*pev;		/* current event */
	int4		i;
	int4		n;		/* favourite counter */
	int4		nbf;		/* total number buffers */
	int4		pktsize;	/* current packet size */
	LIST		*templp;	/* temporary message list */

	LAM_ZERO_ME(nhead);
	nhead.nh_node = req->bfq_src_node;
	nhead.nh_event = req->bfq_src_event;
	nhead.nh_type = 0;
	nhead.nh_flags = DINT4MSG;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	nbf = 0;
	pev = ah_top(bfhp);
	templp = al_init(sizeof(struct nmsg), (int (*)()) 0);

	while (pev) {
		nbf += al_count(pev->bfe_nmsglp);
		pev = ah_next(bfhp, pev);
	}

	n = LAM_min(nbf, req->bfq_maxtbl);

	reply = (struct bfreply *) nhead.nh_data;
	reply->bfr_reply = 0;
	reply->bfr_maxspace = maxspace;
	reply->bfr_nspace = nspace;
	reply->bfr_nbuf = nbf;
/*
 * Send reply to client.
 */
	al_append(templp, &nhead);
/*
 * Send all buffered message headers to the client.
 */
	pev = ah_top(bfhp);
	pktsize = 0;

	if (pev) {
		pbfb = (bfb_t) al_top(pev->bfe_nmsglp);
		p = &pbfb->bfb_head;
		pbfs = (struct bfstatus *) msg;
	}

	for (i = 0; i < n; ++i) {
/*
 * Label unlabelled packets with an unused sequence number.
 */
		if (pbfb->bfb_seq == BFNOTSEQ) {
			pbfb->bfb_seq = seqnext();
		}

		if (pktsize > (MAXNMSGLEN - sizeof(struct bfstatus))) {
			nhead.nh_length = pktsize;
			nhead.nh_msg = malloc((unsigned) pktsize);
			if (!nhead.nh_msg) lampanic("bufferd (malloc)");

			memcpy(nhead.nh_msg, msg, pktsize);
			al_append(templp, &nhead);
			pktsize = 0;
			pbfs = (struct bfstatus *) msg;
		}

		pbfs->bfs_node = p->nh_node;
		pbfs->bfs_event = p->nh_event;
		pbfs->bfs_type = p->nh_type;
		pbfs->bfs_length = p->nh_length;
		pbfs->bfs_flags = p->nh_flags;
		pbfs->bfs_seq = pbfb->bfb_seq;
		memcpy((char *) pbfs->bfs_data, (char *) p->nh_data,
				sizeof(p->nh_data));
		pbfs++;
		pktsize += sizeof(struct bfstatus);

		pbfb = (bfb_t) al_next(pev->bfe_nmsglp, pbfb);

		if (!pbfb) {
			pev = ah_next(bfhp, pev);

			if (pev) {
				pbfb = (bfb_t) al_top(pev->bfe_nmsglp);
				p = &pbfb->bfb_head;
			}
		} else {
			p = &pbfb->bfb_head;
		}
	}

	p = (struct nmsg *) al_top(templp);

	while (p) {
		send_msg(p);
		if (p->nh_msg) free(p->nh_msg);
		p = (struct nmsg *) al_next(templp, p);
	}

	al_free(templp);

	if (pktsize > 0) {
		nhead.nh_length = pktsize;
		nhead.nh_msg = msg;
		send_msg(&nhead);
	}
}

/*
 *	bfqprobe
 *
 *	Function:	- checks for the existance of messages with a given
 *			  event/type
 *			- sends a reply message with that message's
 *			  type, length, flags, data[0] and data[1]
 *	Accepts:	- bufferd request
 */
static void
bfqprobe(req)

struct bfreq		*req;

{
	struct nmsg	nhr;		/* reply msg */
	struct nmsg	*pnh;		/* sync'd msg desc. */
	struct bfbuf	*p;		/* current buffered message */
	struct bfevent	*pbfe;		/* matched event */
	struct bfreply	*reply;		/* bufferd reply */
	struct bfsync	*syncany;	/* synchronization points */
	int		nsyncany;	/* # synchronization points */
	int		i;

	LAM_ZERO_ME(nhr);
	nhr.nh_dl_event = req->bfq_src_event;
	nhr.nh_node = LOCAL;
	nhr.nh_event = req->bfq_src_event;
	nhr.nh_type = 0;
	nhr.nh_flags = 0;
	nhr.nh_length = 0;
	nhr.nh_msg = 0;
	reply = (struct bfreply *) nhr.nh_data;
	reply->bfr_reply = 0;
	reply->bfr_index = -1;
	syncany = (struct bfsync *) bfincoming.nh_msg;

	if (bfincoming.nh_length == 0) {
	    nsyncany = 1;
	    syncany[0].bfk_event = req->bfq_event;
	    syncany[0].bfk_type = req->bfq_type;
	} else {
	    nsyncany = (bfincoming.nh_length / sizeof(int4)) / 2;
	}

	for (i = 0; (i < nsyncany) && (reply->bfr_index == -1); ++i) {
	    pbfe = ah_find(bfhp, syncany[i].bfk_event);
	    if (!pbfe) continue;

	    p = (bfb_t) al_top(pbfe->bfe_nmsglp);

	    while (p && (reply->bfr_index == -1)) {
		req->bfq_type = syncany[i].bfk_type;

		if (bfq_bfb_cmp(req, p)) {
		    reply->bfr_index = i;
		    pnh = &p->bfb_head;
		    reply->bfr_type = pnh->nh_type;
		    reply->bfr_length = (pnh->nh_flags & NMORE) ?
			    pnh->nh_data[0] : pnh->nh_length;
		    reply->bfr_flags = pnh->nh_flags;
		    reply->bfr_data0 = pnh->nh_data[0];
		    reply->bfr_data1 = pnh->nh_data[1];
		}

		p = (bfb_t) al_next(pbfe->bfe_nmsglp, p);
	    }
	}

	if (dtry_send(&nhr)) {

	    if (errno != EWOULDBLOCK) {
		lampanic("bufferd (dtry_send)");
	    }
	}
}

/*
 *	bfqrecv
 *
 *	Function:	- forwards a buffered message
 *	Accepts:	- bufferd request
 */
static void
bfqrecv(req)

struct bfreq		*req;

{
	struct bfbuf	*p;		/* current buffered message */
	struct bfevent	*pbfe;		/* matched event */
	struct bfsync	*syncany;	/* synchronization points */
	int		nsyncany;	/* # synchronization points */
	int		i;

	syncany = (struct bfsync *) bfincoming.nh_msg;

	if (bfincoming.nh_length == 0) {
	    nsyncany = 1;
	    syncany[0].bfk_event = req->bfq_event;
	    syncany[0].bfk_type = req->bfq_type;
	} else {
	    nsyncany = (bfincoming.nh_length / sizeof(int4)) / 2;
	}

	for (i = 0; i < nsyncany; ++i) {
	    pbfe = ah_find(bfhp, syncany[i].bfk_event);
	    if (!pbfe) continue;

	    p = (bfb_t) al_top(pbfe->bfe_nmsglp);

	    while (p) {
		req->bfq_type = syncany[i].bfk_type;

		if (bfq_bfb_cmp(req, p)) {

		    if (dtry_send(&p->bfb_head)) {

			if (errno != EWOULDBLOCK) {
			    lampanic("bufferd (dtry_send)");
			}
		    } else {
			bfrem_msg(p, pbfe);
		    }

		    return;
		}

		p = (bfb_t) al_next(pbfe->bfe_nmsglp, p);
	    }
	}
}

/*
 *	bfqlook
 *
 *	Function:	- forwards a copy of a buffered packet
 *			  with a given sequence #
 */
static void
bfqlook()

{
	struct bfbuf	*pbfb;		/* buffered packet */
	struct bfevent	*pbfe;		/* buffered event */
	struct bfreq	*pbfq;		/* bufferd request */
	struct bfreply	*pbfr;		/* bufferd reply */
	struct nmsg	*pnh;		/* matching buffered packet */
	struct nmsg	nhr;		/* reply message */

	pbfq = (struct bfreq *) bfincoming.nh_data;
	nhr.nh_node = pbfq->bfq_src_node;
	nhr.nh_event = pbfq->bfq_src_event;
	nhr.nh_type = 0;
	nhr.nh_flags = 0;
	nhr.nh_length = 0;
	nhr.nh_msg = 0;

	pbfr = (struct bfreply *) nhr.nh_data;
/*
 * Look for a matching sequence # among buffered messages.
 */
	pbfe = ah_top(bfhp);
	pnh = 0;
	pbfr->bfr_reply = EWOULDBLOCK;

	while (!pnh && pbfe) {
		pbfb = (bfb_t) al_top(pbfe->bfe_nmsglp);

		while (!pnh && pbfb) {

			if (pbfb->bfb_seq == pbfq->bfq_seq) {
				pnh = &(pbfb->bfb_head);
				pbfr->bfr_reply = 0;
				pbfr->bfr_type = pnh->nh_type;
			}

			pbfb = al_next(pbfe->bfe_nmsglp, pbfb);
		}

		pbfe = ah_next(bfhp, pbfe);
	}

	send_msg(&nhr);

	if (pnh) {
		memcpy((char *) nhr.nh_data, (char *) pnh->nh_data,
				NHDSIZE * sizeof(int4));
		nhr.nh_flags = pnh->nh_flags & (DMSKDATA | DMSKMSG);
		nhr.nh_length = pnh->nh_length;
		nhr.nh_msg = pnh->nh_msg;
		send_msg(&nhr);
	}
}

/*
 *	bfqspace
 *
 *	Function:	- sets space parameters
 *	Accepts:	- bufferd request
 */
static void
bfqspace(req)

struct bfreq		*req;

{
	struct nmsg	nhead;
	struct bfreply	*reply;
	char		*p;

	LAM_ZERO_ME(nhead);
	nhead.nh_node = req->bfq_src_node;
	nhead.nh_event = req->bfq_src_event;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	reply = (struct bfreply *) nhead.nh_data;
	reply->bfr_reply = 0;
/*
 * Attempt to malloc the additional space requirement.
 */
	if (req->bfq_maxspace > nspace) {
		p = malloc(req->bfq_maxspace - nspace);

		if (p == 0) {
			reply->bfr_reply = ENOMEM;
		} else {
			free(p);
		}
	}

	if (reply->bfr_reply == 0) {
		maxspace = req->bfq_maxspace;
	}

	send_msg(&nhead);
}

/*
 *	bfchkspace
 *
 *	Function:	- checks total amount of buffer space versus maximum
 *			- shuts off to all messages if maxspace is exceeded
 */
static void
bfchkspace()

{
	if (maxspace < (nspace + MAXNMSGLEN + sizeof(struct nmsg))) {
		filter = DLCTL;
	} else {
		filter = 0;
	}
}

/*
 *	send_msg
 *
 *	Function:	- sends a message to (possibly) offnode client
 *	Accepts:	- network message desc. ptr
 */
static void
send_msg(outhead)

struct nmsg		*outhead;

{
	if ((outhead->nh_node == LOCAL) ||
			(outhead->nh_node == getnodeid())) {
		outhead->nh_dl_event = outhead->nh_event;
	} else {
		outhead->nh_dl_event = EVBFORWARD;
	}

	buffer(outhead);
}

/*
 *	bfrem_msg
 *
 *	Function:	- removes one buffered message
 *	Accepts:	- buffered message ptr
 *			- event ptr
 */
static void
bfrem_msg(pbfb, pbfe)

struct bfbuf		*pbfb;
struct bfevent		*pbfe;

{
	if (pbfb->bfb_head.nh_length) {
		free(pbfb->bfb_head.nh_msg);
		nspace -= pbfb->bfb_head.nh_length;
	}

	seqfree(pbfb->bfb_seq);
	nspace -= sizeof(struct nmsg);
	al_delete(pbfe->bfe_nmsglp, pbfb);

	if (al_count(pbfe->bfe_nmsglp) == 0) {
		al_free(pbfe->bfe_nmsglp);
		ah_delete(bfhp, pbfe->bfe_event);
	}
}

/*
 *	seqnext
 *
 *	Returns:	- next available sequence #
 */
static int4
seqnext()

{
	uint4		i;

	for (i = 0; (i < seqmax) &&
			((seq[seqcurr / 32] >> (seqcurr % 32)) & 1); ++i) {
		seqcurr = (seqcurr + 1) % seqmax;
	}

	if (i >= seqmax) {
		seqcurr = seqmax;
		seqlen += BFSEQLEN;
		seqmax = seqlen * 32;
		seq = (uint4 *) realloc((char *) seq, (unsigned) (seqlen *
				sizeof(int4)));
		if (seq == 0) lampanic("bufferd (realloc)");
	}

	seq[seqcurr / 32] |= 1 << (seqcurr % 32);
	return(seqcurr++);
}

/*
 *	seqfree
 *
 *	Function:	- frees sequence number
 *	Accepts:	- sequence number
 */
static void
seqfree(curr)

int4			curr;

{
	if (curr != BFNOTSEQ) {
		seq[curr / 32] &= ~(1 << (curr % 32));
	}
}
