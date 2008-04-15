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
 * $Id: dlo_inet.c,v 6.13 2003/05/21 19:16:12 jsquyres Exp $
 *
 *	Function:	- datalink output program for OTB-OTB socket link
 */

#include <lam_config.h>
#include <sfh.h>

#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <lamdebug.h>
#include <dl_inet.h>
#include <events.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <terror.h>


/*
 * global variables
 */
struct doslot		doslots[DOSLOTCNT];	/* slot table */
struct dolink		*dolinks;		/* link table */
int			dol_cnt;		/* dol table length */
int			dos_nfull;		/* # window slots */
int			do_sd;			/* datalink socket */
int			do_fault;		/* watch for faulty links */

struct timeval		do_to_idle;		/* idle timeout period */
struct timeval		do_to_ack;		/* ack timeout period */


/*
 * local variables
 */
static int		nvclose;		/* value close ref. count */
static struct doslot	*pdos_empty;		/* current incoming slot */
static struct nmsg	nh_pkt;			/* incoming packet */
static struct nmsg	nhr;			/* reply message */
static char             packet[MAXNMSGLEN];     /* portion of route table 
						   to send */

/*
 * global functions
 */
void			(*(dlo_inet()))();
void			(*(do_init()))();
void			(*(do_init2()))();
void do_slotsweep(int4 link);
void do_outgoing(struct doslot *pdos, struct nmsg *pnh);
void do_sendframe(struct doslot *pdos);
struct doslot *do_slotempty(void);


/*
 * external functions
 */
extern void		_ltot_sysnmsg();
extern void		_ltot_usrnmsg();
extern void		(*(do_loadinit()))();
extern void		do_timeout_ack();
extern void		do_timeout_idle();
extern void		do_wait_block();
extern void		do_wait_running();


/*
 * local functions
 */
static void		request(struct nmsg *pnhq);


/*
 *	do_init
 *
 *	Function:	- dlo_inet initialization
 */
void (*(
do_init()))()
{
	int		n;
	struct doslot	*pdos;
/*
 * Create output link socket.
 */
	if ((do_sd = sfh_sock_open_clt_inet_dgm()) < 0)
			lampanic("dlo_inet (sfh_sock_open_clt_inet_dgm)");

        /* set socket to be close-on-exec */
        if (fcntl(do_sd, F_SETFD, 1) != 0)
            lampanic("dlo_inet: setting close-on-exec");

	nvclose = 0;
	dos_nfull = 0;
	getnodeid();
/*
 * Initialize the slot table.
 */
	for (n = DOSLOTCNT, pdos = doslots; n > 0; --n, ++pdos) {
		pdos->dos_link = NOTLINKID;
		pdos->dos_flags = 0;
	}
/*
 * Initialize the timeout periods.
 */
	do_to_idle.tv_sec = LAM_TO_DLO_IDLE;
	do_to_idle.tv_usec = 0;
	do_to_ack.tv_sec = LAM_TO_DLO_ACK / 1000000;
	do_to_ack.tv_usec = LAM_TO_DLO_ACK % 1000000;
/*
 * Initialize the output link table.
 */
	return(do_loadinit());
}


/*
 *	do_init2
 *
 *	Function:	- further dlo_inet initialization
 */
void (*(
do_init2()))()
{
/*
 * Attach to kenyad.
 */
	if (lpattach("dlo_inet")) lampanic("dlo_inet (lpattach)");
/*
 * Receive a packet or an ack or a timeout.
 */
	pdos_empty = do_slotempty();

	if (!pdos_empty) {
		errno = EFULL;
		lampanic("dlo_inet (do_slotempty)");
	}

	nh_pkt.nh_event = EVDL0;
	nh_pkt.nh_flags = 0;
	nh_pkt.nh_length = MAXNMSGLEN;
	nh_pkt.nh_type = 0;
	nh_pkt.nh_msg = pdos_empty->dos_frame.dlf_msg;

	if (do_fault) {
		do_wait_block(do_sd, &nh_pkt, do_timeout_idle, &do_to_idle);
	} else {
		do_wait_block(do_sd, &nh_pkt, (void (*)()) 0,
				(struct timeval *) 0);
	}

	return((void (*)()) dlo_inet);
}


/*
 *	dlo_inet
 *
 *	Function:	- dlo_inet mainline
 */
void (*(
dlo_inet()))()
{
	struct timeval	*pto;			/* updated timeout */
	void		(*pto_fn)();		/* updated timeout function */

	if (pdos_empty) {

		if ((nh_pkt.nh_event == EVDL0) &&
				((nh_pkt.nh_node == getnodeid()) ||
				(nh_pkt.nh_node == LOCAL))) {
			request(&nh_pkt);
		} else {
			do_outgoing(pdos_empty, &nh_pkt);
		}
	}
/*
 * Receive a packet or an ack or a timeout.
 */
	pdos_empty = do_slotempty();

	if (dos_nfull > 0) {
		pto = &do_to_ack;
		pto_fn = do_timeout_ack;
	} else if (do_fault) {
		pto = &do_to_idle;
		pto_fn = do_timeout_idle;
	} else {
		pto = 0;
		pto_fn = 0;
	}

	if (pdos_empty) {
		nh_pkt.nh_event = EVDL0;
		nh_pkt.nh_flags = 0;
		nh_pkt.nh_length = MAXNMSGLEN;
		nh_pkt.nh_type = (nvclose > 0) ? (DLCTL | DLI | DLSYS) : 0;
		nh_pkt.nh_msg = pdos_empty->dos_frame.dlf_msg;
		do_wait_block(do_sd, &nh_pkt, pto_fn, pto);
	} else {
		do_wait_running(do_sd, pto_fn, pto);
	}

	return((void (*)()) dlo_inet);
}


/*
 *	do_outgoing
 *
 *	Function:	- handles outgoing dlo packet
 *	Accepts:	- output slot ptr
 *			- output packet ptr
 */
void
do_outgoing(struct doslot *pdos, struct nmsg *pnh)
{
	struct dlheader	*pdlh;			/* dl_inet header ptr */
	struct dolink	*pdol;			/* output link ptr */
/*
 * Drop packets with invalid links.
 */
	if ((pnh->nh_dl_link < 0) || (pnh->nh_dl_link >= dol_cnt)) {
		return;
	}

	if (dolinks[pnh->nh_dl_link].dol_link == NOTLINKID) {
		return;
	}
/*
 * Don't forward the KTRY, DDLI or KTRACE flags.
 */
	pnh->nh_flags &= ~(KTRY | DDLI | KTRACE);
/*
 * Determine outgoing link.  Fill in slot.
 */
	dos_nfull += 1;
	pdos->dos_link = pnh->nh_dl_link;
	pdos->dos_resend = 0;
	pdol = dolinks + pdos->dos_link;
	pdol->dol_idle = 0;

	lam_comm_debug_dlo_cond((1,
				 "nd=%d ev=%d, tp=X%X, ln=%d, sq=%d, pn=%d\n",
				 pnh->nh_node, 
				 pnh->nh_event, pnh->nh_type,
				 pnh->nh_length, pdol->dol_seqgive,
				 pdol->dol_npending));
/*
 * Convert packet to Trollius byte order.
 */
	if ((pnh->nh_flags & DTBO) == 0) {
		pnh->nh_flags |= DTBO;
		_ltot_usrnmsg(pnh);
	}

	_ltot_sysnmsg(pnh);
/*
 * Copy the Trollius header to its dl_inet header.
 */
	pdlh = &(pdos->dos_frame.dlf_msghead);
	pdlh->dlh_node = pnh->nh_node;
	pdlh->dlh_event = pnh->nh_event;
	pdlh->dlh_type = pnh->nh_type;
	pdlh->dlh_flags = pnh->nh_flags;
	pdlh->dlh_length = pnh->nh_length;

	memcpy((char *) pdlh->dlh_data, (char *) pnh->nh_data,
			NHDSIZE * sizeof(int4));
/*
 * Set the message sequence # and advance the link sequence #.
 */
	pdos->dos_frame.dlf_src_node = getnodeid();
	pdos->dos_frame.dlf_seqnum = pdol->dol_seqgive;
	pdol->dol_seqgive = seqinc(pdol->dol_seqgive);
/*
 * Convert frame envelope to Trollius byte order.
 */
	ltoti4(&(pdos->dos_frame.dlf_src_node),
			&(pdos->dos_frame.dlf_src_node));
	ltoti4(&(pdos->dos_frame.dlf_seqnum), &(pdos->dos_frame.dlf_seqnum));
/*
 * Send the frame.
 */
	if (pdol->dol_npending < DOMAXPENDING) {
		pdol->dol_npending++;
		pdos->dos_flags = DOPENDING;
		pdol->dol_seqsend = seqinc(pdol->dol_seqsend);
		do_sendframe(pdos);
	} else {
		pdos->dos_flags = 0;
	}
}


/*
 *	do_slotempty
 *
 *	Function:	- finds an empty slot
 *	Returns:	- slot ptr
 */
struct doslot *
do_slotempty(void)
{
	int		n;
	struct doslot	*pdos;
/*
 * Find a slot that is empty and not reserved (due to frecvfront()).
 */
	for (n = DOSLOTCNT, pdos = doslots; (n > 0) &&
			((pdos->dos_link != NOTLINKID) ||
			(pdos->dos_flags & DORESERVED)); --n, ++pdos);
	if (n <= 0) return(0);

	pdos->dos_flags = DORESERVED;
	return(pdos);
}


/*
 *	do_sendframe
 *
 *	Function:	- sends message to remote dli_inet
 *	Accepts:	- slot ptr
 */
void
do_sendframe(struct doslot *pdos)
{
	int		r;
	int4		length;			/* frame length */
	struct dolink	*pdol;			/* output link */

	pdol = dolinks + pdos->dos_link;
/*
 * Send the message to the remote dli_inet.
 */
	ttoli4(&(pdos->dos_frame.dlf_msghead.dlh_length), &length);
	length += DLMINFRAMELEN;

	while (((r = sendto(do_sd, (char *) &(pdos->dos_frame),
		(int) length, 0, (struct sockaddr *) &(pdol->dol_addr),
		sizeof(struct sockaddr_in))) < 0)
	    	&& (errno == EINTR || errno == ECHILD));
/*
 * On the DEC Alpha sendto (on a blocking mode socket!) can return with an
 * ENOBUFS error.  We workaround this by ignoring this error and letting
 * the upper level protocol take care of retransmitting the ack.
 */
	if (r < 0) {
		if (errno != ENOBUFS) lampanic("dlo_inet (sendto)");
	} else if (r != length) {
		errno = EMSGLEN;
		lampanic("dlo_inet (sendto)");
	}
}


/*
 *	request
 *
 *	Function:	- handles dlo request
 *	Accepts:	- request message
 */
static void
request(struct nmsg *pnhq)
{
	struct doreq	*pdoq;
	struct doreply	*pdor;
	struct dolink	*pdol;
	int		i;
	int		eps;
	int		seg;

	pdoq = (struct doreq *) pnhq->nh_data;
/*
 * Open the user message valve.
 */
	if (pdoq->doq_req == DOQVOPEN) {

		if (nvclose > 1) {
			nvclose -= 1;
			lam_comm_debug_dlo_cond((0, 
						 "valve open, still shut\n"));
		} else {
			nvclose = 0;
			lam_comm_debug_dlo_cond((0, "valve opened\n"));
		}
	}
/*
 * Close the user message valve.
 */
	else if (pdoq->doq_req == DOQVCLOSE) {
		lam_comm_debug_dlo_cond((0, "valve shut\n"));
		nvclose += 1;
	}
/*
 * Provide the array of link information.
 */
	else if (pdoq->doq_req == DOQGETLINKS) {
	        seg = pdoq->doq_extra;
	        LAM_ZERO_ME(nhr);
		LAM_ZERO_ME(packet);

		pdor = (struct doreply *) nhr.nh_data;
		pdor->dor_reply = 0;
		pdor->dor_nlinks = dol_cnt;

		/* Protocol recoded 24 June 2001.  We now only send
		   one segment back at a time, where one segment is as
		   many entries will fit <MAXNMSGLEN (so that we don't
		   have to packetize the result).  See extensive note
		   in share/nreq/ldolinks.c. */

		nhr.nh_node = LOCAL;
		nhr.nh_event = pdoq->doq_src_event;
		nhr.nh_type = 0;
		nhr.nh_flags = 0;
		nhr.nh_length = MAXNMSGLEN;
		nhr.nh_msg = packet;

		eps = MAXNMSGLEN / sizeof(struct dolink);

		if (dol_cnt - (seg * eps) > eps)
		  i = eps * sizeof(struct dolink);
		else
		  i = (dol_cnt - (seg * eps)) * sizeof(struct dolink);
		memcpy(packet, dolinks + (seg * eps), i);

		if (nsend(&nhr)) 
		  lampanic("dlo_inet (nsend)");
	}
/*
 * Add a link.  
 */
	else if (pdoq->doq_req == DOQSETLINK) {
		pdol = (struct dolink *) pdos_empty->dos_frame.dlf_msg;

		if (pdol->dol_link >= dol_cnt) {
			dolinks = (struct dolink *) realloc((char *) dolinks,
					(unsigned) (pdol->dol_link + 1) *
					sizeof(struct dolink));
			if (dolinks == 0) lampanic("dlo_inet (malloc)");

			for (i = dol_cnt; i < pdol->dol_link; ++i) {
				dolinks[i].dol_link = NOTNODEID;
			}

			dol_cnt = pdol->dol_link + 1;
		}

		pdol = &dolinks[pdol->dol_link];
		memcpy((char *) pdol, pdos_empty->dos_frame.dlf_msg,
				sizeof(struct dolink));
		pdol->dol_seqsend = 0;
		pdol->dol_seqgive = 0;
		pdol->dol_npending = 0;
		pdol->dol_idle = 0;
	}
/*
 * Remove a link.
 */
	else if (pdoq->doq_req == DOQREMLINK) {
		do_slotsweep(pdoq->doq_link);
		dolinks[pdoq->doq_link].dol_link = NOTLINKID;

		while (dolinks[dol_cnt - 1].dol_link == NOTLINKID) {
			dol_cnt--;
		}
	}

	else {
		errno = EBADREQUEST;
		lampanic("dlo_inet");
	}
/*
 * Give back the empty slot.
 */
	pdos_empty->dos_flags = 0;
}


/*
 *	do_slotsweep
 *
 *	Function:	- frees all slots allocated for given link
 *	Accepts:	- link
 */
void
do_slotsweep(int4 link)
{
	int		n;
/*
 * Free all slots containing frames bound for the dead node.
 */
	for (n = DOSLOTCNT - 1; n >= 0; --n) {

		if (doslots[n].dos_link == NOTLINKID) continue;

		if (doslots[n].dos_link == link) {
			doslots[n].dos_link = NOTLINKID;
			doslots[n].dos_flags = 0;
			dos_nfull--;
		}
	}

	dolinks[link].dol_link = NOTLINKID;
}
