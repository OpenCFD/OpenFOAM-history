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
 *	$Id: do_intr.c,v 6.10 2002/10/09 20:56:32 brbarret Exp $
 * 
 *	Function:	- UDP Internet datalink output
 *			- ack and timeout interrupt handlers
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <lamdebug.h>
#include <dl_inet.h>
#include <events.h>
#include <net.h>
#include <portable.h>
#include <t_types.h>
#include <terror.h>

/*
 * external variables
 */
extern struct doslot	doslots[DOSLOTCNT];	/* slot table */
extern struct dolink	*dolinks;		/* link table */
extern int		dol_cnt;		/* dol table length */
extern int		dos_nfull;		/* # window slots */
extern int		do_sd;			/* datalink socket */
extern int		do_fault;		/* watch for faulty links */

extern struct timeval	do_to_idle;		/* idle timeout period */
extern struct timeval	do_to_ack;		/* ack timeout period */

/*
 * local variables
 */
static int		ndol_stale = 0;		/* # stale links */
static struct doslot	*pdos_sort[DOSLOTCNT];	/* sorted slot array */

/*
 * global functions
 */
void			do_ack();
void			do_timeout_ack();
void			do_timeout_idle();

/*
 * external functions
 */
extern void		do_outgoing();
extern void		do_sendframe();
extern void		do_slotsweep();
extern struct doslot	*do_slotempty();

/*
 * local functions
 */
static void		linkdie();
static void		linksanity();

/*
 *	do_ack
 *
 *	Function:	- handles a message ACK
 *	Accepts:	- timeout handler (value returned)
 *			- timeout (value returned)
 */
void
do_ack(ptimeout_fn, ppto)

void			(**ptimeout_fn)();
struct timeval		**ppto;

{
	int		n, r;
	LAM_RECVFROM_LEN_T srclen;		/* source address length */
	int4		seqnum;		/* message sequence # */
	struct doslot	*pdos;		/* slot ptr */
	struct dolink	*pdol;		/* output link ptr */
	struct dlack	ackdest;	/* ack from remote dli */
	struct sockaddr srcaddr;
/*
 * Receive the ACK.
 */
	srclen = sizeof(srcaddr);

	LAM_ZERO_ME(ackdest);
	while (((r = recvfrom(do_sd, (char *) &ackdest,
			sizeof(struct dlack), 0, &srcaddr,
			&srclen)) < 0) && (errno == EINTR || errno == ECHILD));

	if (do_fault && (r < 0) && (errno == ECONNREFUSED)) {
	  /* On linux, recvfrom can return connection refused, which
	     implies something went bad.  Since we aren't going to get
	     our ACK, might as well abort and let someone else figure
	     it out (if we get the ack again, we end up in do_ack
	     again.  If not, the link is probably dead and the
	     heartbeat will save us. */
	  return;
	}

	if (r < 0) lampanic("dlo_inet (recvfrom)");

	ttoli4(&ackdest.dla_seqnum, &ackdest.dla_seqnum);
	ttoli4(&ackdest.dla_destid, &ackdest.dla_destid);
/*
 * Locate the ACK'ed message.
 */
	for (pdos = doslots, n = DOSLOTCNT; n > 0; --n, ++pdos) {

		if (pdos->dos_link == NOTLINKID) continue;

		ttoli4(&(pdos->dos_frame.dlf_seqnum), &seqnum);

		if ((pdos->dos_link == ackdest.dla_destid) &&
				(seqnum == ackdest.dla_seqnum)) {
			break;
		}
	}

	if (n == 0) {
		lam_comm_debug_dlo_cond((1,
					 "ack nd=%d, sq=%d, duplicate\n",
				ackdest.dla_destid, ackdest.dla_seqnum));
		return;
	}
/*
 * Update the link seq. # information.
 * Free the message slot.
 */
	pdol = dolinks + pdos->dos_link;
	pdos->dos_link = NOTLINKID;
	pdos->dos_flags = 0;
	pdol->dol_npending -= 1;
	dos_nfull -= 1;
/*
 * Is there a pending packet for this link that can be sent?
 */
	if (pdol->dol_seqgive == pdol->dol_seqsend) {
		lam_comm_debug_dlo_cond((1,
					 "ack nd=%d, sq=%d, none holding\n",
					 ackdest.dla_destid, 
					 ackdest.dla_seqnum));
	} else {
/*
 * Find the next packet to send.
 */
		for (pdos = doslots, n = DOSLOTCNT; n > 0; --n, ++pdos) {
			if (pdos->dos_link == NOTLINKID) continue;

			ttoli4(&(pdos->dos_frame.dlf_seqnum), &seqnum);

			if ((pdos->dos_link == ackdest.dla_destid) &&
					(seqnum == pdol->dol_seqsend)) {
				break;
			}
		}

		pdol->dol_npending += 1;
		pdos->dos_flags |= DOPENDING;
		do_sendframe(pdos);
		lam_comm_debug_dlo_cond((1, 
					 "ack nd=%d, sq=%d, sent sq=%d\n",
					 ackdest.dla_destid, 
					 ackdest.dla_seqnum,
					 pdol->dol_seqsend));
		pdol->dol_seqsend = seqinc(pdol->dol_seqsend);
	}

	if (ndol_stale > 0) {
		linksanity();
	}
/*
 * Change timeout if there are no occupied slots.
 */
	if (dos_nfull <= 0) {
		lam_comm_debug_dlo_cond((1, "ack, quiet link\n"));

		if (do_fault) {
			*ptimeout_fn = do_timeout_idle;
			*ppto = &do_to_idle;
		} else {
			*ptimeout_fn = 0;
			*ppto = 0;
		}
	}
}

/*
 *	do_timeout_ack
 *
 *	Function:	- timeout handler
 *	Accepts:	- timeout handler (value returned)
 *			- timeout (value returned)
 */
void
do_timeout_ack(ptimeout_fn, ppto)

void			(**ptimeout_fn)();
struct timeval		**ppto;

{
	int		i, j, n;
	int		nresend;	/* # frames to resend */
	struct doslot	*pdos;
/*
 * Resend all pending old frames and mark all pending slots as old.
 * Resend frames in increasing sequence number order.
 */
	nresend = 0;
	lam_comm_debug_dlo_cond((0, "timeout ack\n"));

	for (pdos = doslots, n = DOSLOTCNT; n > 0; ++pdos, --n) {

		if (pdos->dos_link == NOTLINKID) continue;

		if (pdos->dos_flags & DORESEND) {
			pdos_sort[nresend++] = pdos;
			ttoli4(&(pdos->dos_frame.dlf_seqnum),
					&(pdos->dos_frame.dlf_seqnum));
		}

		if (pdos->dos_flags & DOPENDING) {
			pdos->dos_flags |= DORESEND;
		}
	}
/*
 * Sort slots by frame sequence number.
 */
	for (i = 0; i < nresend - 1; ++i) {

		for (j = i + 1; j < nresend; ++j) {

			if (pdos_sort[i]->dos_frame.dlf_seqnum >
					pdos_sort[j]->dos_frame.dlf_seqnum) {
				pdos = pdos_sort[i];
				pdos_sort[i] = pdos_sort[j];
				pdos_sort[j] = pdos;
			}
		}
	}
/*
 * Resend frames in increasing sequence number order.
 */
	for (i = 0; i < nresend; ++i) {
		pdos = pdos_sort[i];

		if (pdos->dos_link == NOTLINKID) continue;

		if (do_fault && (pdos->dos_resend > DOMAXRESEND)) {
			lam_comm_debug_dlo_cond((0, "timeout ack, "
						 "dead link nd=%d\n",
						 pdos->dos_link));
			linkdie(pdos->dos_link);
		}

		else {
			ltoti4(&(pdos->dos_frame.dlf_seqnum),
					&(pdos->dos_frame.dlf_seqnum));

		lam_comm_debug_dlo_cond((0, "timeout ack, resend nd=%d, "
					 "sq=%d, #resend=%d\n",
					 pdos->dos_link, 
					 pdos->dos_frame.dlf_seqnum,
					 pdos->dos_resend));
			do_sendframe(pdos);
			pdos->dos_resend++;
		}
	}

	if (!do_fault) return;
/*
 * Check all idle links.
 */
	for (i = 0; i < dol_cnt; ++i) {

		if (dolinks[i].dol_link == NOTLINKID) continue;

		if ((dolinks[i].dol_npending == 0) && (i != getnodeid())) {
			dolinks[i].dol_idle++;

			if (dolinks[i].dol_idle == DOMAXSTALE) {
			  lam_comm_debug_dlo_cond((0,
						   "timeout ack, "
						   "nd=%d stale\n",
						   i));
				ndol_stale++;
			}
		}
	}

	if (ndol_stale > 0) {
		linksanity();
	}
}

/*
 *	do_timeout_idle
 *
 *	Function:	- idle period timeout handler
 *	Accepts:	- timeout handler (value returned)
 *			- timeout (value returned)
 */
void
do_timeout_idle(ptimeout_fn, ppto)

void			(**ptimeout_fn)();
struct timeval		**ppto;

{
	int		i;

	for (i = 0; i < dol_cnt; ++i) {

		if (dolinks[i].dol_link == NOTLINKID) continue;

		if (dolinks[i].dol_link != getnodeid()) {
			dolinks[i].dol_idle += DOMAXSTALE;
/*
 * Ensure that we do not count the same link as being stale more than once.
 */
			if ((dolinks[i].dol_idle - DOMAXSTALE) < DOMAXSTALE) {
				ndol_stale++;
			}
		}
	}

	lam_comm_debug_dlo_cond((0, "timeout idle\n"));
	linksanity();

	if (dos_nfull > 0) {
		*ptimeout_fn = do_timeout_ack;
		*ppto = &do_to_ack;
	}
}

/*
 *	linksanity
 *
 *	Function:	- sends a sanity packet to neighbour nodes
 *			  connected by stale links
 */
static void
linksanity()

{
	struct doslot	*pdos;			/* empty slot */
	struct dolink	*pdol;			/* most stale link */
	struct direq	*pdiq;			/* dli request */
	int4		maxidle = 0;
	int		i;
	struct nmsg	nhq;			/* request message */

	pdos = do_slotempty();

	while (pdos && (ndol_stale > 0)) {
/*
 * Find the most stale link.
 */
		pdol = 0;
		maxidle = 0;

		for (i = 0; i < dol_cnt; ++i) {

			if (dolinks[i].dol_link == NOTLINKID) continue;
			if (dolinks[i].dol_link == getnodeid()) continue;

			if (dolinks[i].dol_idle > maxidle) {
				maxidle = dolinks[i].dol_idle;
				pdol = &dolinks[i];
			}
		}

		ndol_stale--;
/*
 * Check for inconsistency error.
 */
		if (maxidle < DOMAXSTALE) continue;

		nhq.nh_dl_event = EVDL0;
		nhq.nh_dl_link = pdol->dol_link;
		nhq.nh_node = pdol->dol_link;
		nhq.nh_event = EVDLI;
		nhq.nh_type = 0;
		nhq.nh_flags = 0;
		nhq.nh_length = 0;
		nhq.nh_msg = 0;

		pdiq = (struct direq *) nhq.nh_data;
		pdiq->diq_req = DIQSANITY;

		do_outgoing(pdos, &nhq);
		pdos = do_slotempty();
	}

	if (pdos) {
		pdos->dos_flags = 0;
	}
}

/*
 *	linkdie
 *
 *	Function:	- sends a link kill request to partner dli
 *			- starts the process of removing a node
 *	Accepts:	- link
 */
static void
linkdie(link)

int4			link;

{
	struct nmsg	nhq;			/* request message */
	struct direq	*pdiq;			/* dli request */

	if (dolinks[link].dol_link == NOTLINKID) return;
/*
 * Free all slots containing frames bound for the dead node.
 */
	do_slotsweep(link);
	dolinks[link].dol_link = NOTLINKID;
/*
 * Send a link kill request to my partner dli on the loopback link.
 */
	nhq.nh_dl_event = EVDL0;
	nhq.nh_dl_link = getnodeid();
	nhq.nh_node = getnodeid();
	nhq.nh_event = EVDLI;
	nhq.nh_type = 0;
	nhq.nh_flags = NOBUF;
	nhq.nh_length = 0;
	nhq.nh_msg = 0;

	pdiq = (struct direq *) nhq.nh_data;
	pdiq->diq_req = DIQREMLINK;
	pdiq->diq_src_node = NOTNODEID;
	pdiq->diq_link = link;
/*
 * We know that at least one slot is available because we just freed
 * at least one (the one that caused this to be called).
 */
	do_outgoing(do_slotempty(), &nhq);
}

/*
 *	do_linksweep
 *
 *	Function:	- frees all slots allocated for given link
 *	Accepts:	- link
 */
void
do_linksweep(link)

int4			link;

{
	int		n;
/*
 * Free all slots containing frames bound for the dead node.
 */
	for (n = DOSLOTCNT - 1; n >= 0; --n) {

		if (doslots[n].dos_link == NOTLINKID) continue;

		if (doslots[n].dos_link == link) {
			doslots[n].dos_link = NOTLINKID;
			dos_nfull--;
		}
	}

	dolinks[link].dol_link = NOTLINKID;
}
