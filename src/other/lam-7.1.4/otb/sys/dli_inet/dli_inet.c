/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: dli_inet.c,v 6.15 2003/03/25 00:39:28 jsquyres Exp $
 *
 *	Function:	- datalink input program for OTB-OTB link
 *			- reads network frames from a socket and forwards them
 *			- works either in stand-alone mode or coordinates
 *			  with booting agent to determine OTB neighbours
 *			- handles dlo_inet initialization
 *			- handles router initialization
 *
 *	Usage:		- -H <agentaddr> -P <agentport> -n <nodeid> -o <origin>
 *			- -p <port> <host> <link> ... -n <nodeid> -o <origin>
 */

#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <all_opt.h>
#include <lamdebug.h>
#include <dl_inet.h>
#include <events.h>
#include <ksignal.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <rreq.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>


/*
 * external functions
 */
extern int		di_bootagent();
extern int		di_bootcmd();
extern void		_ttol_sysnmsg();
extern void		_ttol_usrnmsg();
extern void		di_wait();


/*
 * local functions
 */
static int		initdlo(void);
static void		di_ack(int4);
static void		diqsetlink(struct nmsg *);
static void		diqremlink(struct nmsg *);
static void             initrouter(struct diinfo *pdiinfo, int4 mynode);
static void		getmessage(void);


/*
 * local variables
 */
static int		dli_sd;		/* dli input socket */
static int		ndil;		/* # neighbour links */
static LAM_RECVFROM_LEN_T srclen;	/* length of source address */
static struct dlframe	dlf_incoming;	/* incoming frame */
static struct sockaddr_in
			srcaddr;	/* source address */
struct dilink		*dilinks;	/* links to neighbours */
struct diinfo           *diinfo = 0;


/*
 * global functions
 */
void			(*(di_inet()))();
void			(*(di_init(int argc, char* argv[])))();
void			di_forward(void);


/*
 * global (to lam daemons) options
 */
extern OPT* lam_daemon_optd;


/*
 *	di_init
 *
 *	Function:	- initialization
 */
void (*(
di_init(int argc, char *argv[])))()
{
        int4 mynode;

	--argc;
	++argv;
/*
 * Boot from agent.  Get agent address and port from command line.
 */
	if (ao_taken(lam_daemon_optd, "H")) {
		dli_sd = di_bootagent(&dilinks, &diinfo, &ndil, &mynode, 
                                      argc, argv);
	} else {
		dli_sd = di_bootcmd(&dilinks, &diinfo, &ndil, &mynode,
                                    argc, argv);
	}
/*
 * Build and send route information to router.
 */
	initrouter(diinfo, mynode);
/*
 * Send the link info to dlo.
 */
	if (initdlo()) lampanic("dli_inet (initdlo)");
/*
 * Attach to kenyad.
 */
	if (lpattach("dli_inet")) lampanic("dli_inet (lpattach)");

	return((void (*)()) di_inet);
}


/*
 *	di_inet
 *
 *	Function:	- mainline
 *			- forwards message and waits for next frame
 */
void (*(
di_inet()))()
{
/*
 * Wait for the next network frame.
 */
	di_wait(dli_sd);
	return((void (*)()) di_inet);
}


/*
 *	di_forward
 *
 *	Function:	- reads and forwards network message
 */
void
di_forward(void)
{
	struct nmsg	incoming;	/* network message from link */
	struct dlheader	*head;		/* datalink header */
	struct dilink	*p;
	struct direq	*pdiq;		/* dli request */
	int4		srcnode;	/* message source nodeid */
	int4		seqnum;		/* message sequence # */
/*
 * Datalink input is ready, read the incoming message.
 */
	getmessage();
/*
 * Copy the header to a Trollius header and convert it to local byte order.
 */
	head = &(dlf_incoming.dlf_msghead);
	incoming.nh_dl_event = 0;
	incoming.nh_dl_link = 0;
	incoming.nh_node = head->dlh_node;
	incoming.nh_event = head->dlh_event;
	incoming.nh_type = head->dlh_type;
	incoming.nh_length = head->dlh_length;
	incoming.nh_flags = head->dlh_flags;

	memcpy((char *) incoming.nh_data, (char *) head->dlh_data,
			NHDSIZE * sizeof(int4));
	_ttol_sysnmsg(&incoming);

	incoming.nh_flags |= DDLI;
	incoming.nh_msg = dlf_incoming.dlf_msg;
/*
 * Check the message sequence number and ACK back if necessary.
 */
	ttoli4(&(dlf_incoming.dlf_src_node), &srcnode);
	ttoli4(&(dlf_incoming.dlf_seqnum), &seqnum);
/*
 * Check the source node validity.
 */
	if ((srcnode >= ndil) || (srcnode < 0)) {
		return;
	} else if (dilinks[srcnode].dil_link == NOTLINKID) {
		return;
	}

	p = dilinks + srcnode;
/*
 * If we received expected number, ACK it and wait for the next number.
 */
	if (seqnum == p->dil_seqrecv) {
	  lam_comm_debug_dli_cond((1,
				   "ev=%d, tp=X%X, ln=%d, nd=%d, "
				   "sq=%d, acked\n",
				   incoming.nh_event, incoming.nh_type,
				   incoming.nh_length, srcnode, seqnum));
		di_ack(seqnum);
		p->dil_seqrecv = seqinc(p->dil_seqrecv);
	} else {
/*
 * Do not forward the message.
 * If it is a duplicate message, ACK it.
 * Otherwise it is a future message so just drop it.
 */
		if (seqnum == seqmin(p->dil_seqrecv, seqnum)) {
			di_ack(seqnum);
			lam_comm_debug_dli_cond((1,
						 "ev=%d, tp=X%X, ln=%d, "
						 "nd=%d, sq=%d, duplicate\n",
						 incoming.nh_event, 
						 incoming.nh_type,
						 incoming.nh_length, 
						 srcnode, seqnum));
		} else {
		  lam_comm_debug_dli_cond((1,
					   "ev=%d, tp=X%X, ln=%d, nd=%d, "
					   "sq=%d, future\n",
					   incoming.nh_event, 
					   incoming.nh_type,
					   incoming.nh_length, 
					   srcnode, seqnum));
		}

		return;
	}
/*
 * Convert data pouch and message to local byte order if necessary.
 * Do it for messages that are local or going to the cast daemon.
 */
	if ((incoming.nh_node == LOCAL) || (incoming.nh_node == getnodeid()) ||
			tiscast(getrtype(incoming.nh_node))) {
		incoming.nh_flags &= ~DTBO;
		_ttol_usrnmsg(&incoming);
	}
/*
 * Forward the message.
 */
	if (incoming.nh_flags & NOBUF) {
		if (getroute(&incoming)) lampanic("dli_inet (getroute)");
	} else {
		incoming.nh_dl_event = EVBUFFERD;
	}

	if ((incoming.nh_event == EVDLI) && ((incoming.nh_node == LOCAL) ||
			(incoming.nh_node == getnodeid()))) {
		pdiq = (struct direq *) incoming.nh_data;

		if (pdiq->diq_req == DIQSETLINK) {
			diqsetlink(&incoming);
		} else if (pdiq->diq_req == DIQREMLINK) {
			diqremlink(&incoming);
		}
	} else {
		if (dsend(&incoming)) lampanic("dli_inet (dsend)");
	}
}


/*
 *	getmessage
 *
 *	Function:	- reads a network frame from a UDP socket
 *			- exits in case of peer shutdown
 */
static void
getmessage(void)
{
	int		r;

	srclen = sizeof(srcaddr);

	while (((r = recvfrom(dli_sd, (char *) &dlf_incoming,
			sizeof(struct dlframe), 0,
			(struct sockaddr *) &srcaddr,
			&srclen)) < 0) && (errno == EINTR || 
					   errno == ECHILD ||
					   errno == EAGAIN));

	if (r < 0) {
		lampanic("dli_inet (recvfrom)");
	} else if (r < (int)DLMINFRAMELEN) {
		errno = EINVAL;
		lampanic("dli_inet (recvfrom): invalid network frame");
	}
}


/*
 *	di_ack
 *
 *	Function:	- sends numbered-ACK to dlo on source node
 *	Accepts:	- frame sequence #
 */
static void
di_ack(int4 seqnum)
{
	struct dlack	ack2src;	/* ack to source node */
	int		r;

	ack2src.dla_destid = getnodeid();
	ack2src.dla_seqnum = seqnum;
	ltoti4(&ack2src.dla_seqnum, &ack2src.dla_seqnum);
	ltoti4(&ack2src.dla_destid, &ack2src.dla_destid);

	while (((r = sendto(dli_sd, (char *) &ack2src,
			sizeof(struct dlack), 0, (struct sockaddr *) &srcaddr,
			sizeof(srcaddr))) < 0)
			&& (errno == EINTR || errno == ECHILD));
/*
 * On the DEC Alpha sendto (on a blocking mode socket!) can return with an
 * ENOBUFS error.  We workaround this by ignoring the error and letting
 * the upper level protocol take care of retransmitting the ack.
 */
	if ((r < 0) && (errno != ENOBUFS)) lampanic("dli_inet (sendto)");
}


/*
 *	initdlo
 *
 *	Function:	- initialize the dlo
 *			- send the links information
 */
static int
initdlo(void)
{
	int		i;
	int4		config[3];	/* link parameters */
	struct kmsg	khead;		/* kernel message desc. */
	struct dolink	*pdol;		/* output link table */

	config[0] = ndil;
	config[1] = 0;			/* was -O */
	config[2] = ao_taken(lam_daemon_optd, "x");

	khead.k_event = EVDLOINIT;
	khead.k_type = 0;
	khead.k_flags = 0;
	khead.k_length = sizeof(config);
	khead.k_msg = (char *) config;

	if (ksend(&khead)) return(LAMERROR);

	khead.k_length = sizeof(struct dolink) * ndil;
	pdol = (struct dolink *) malloc((unsigned) khead.k_length);
	if (pdol == 0) lampanic("dli_inet (malloc)");

	for (i = 0; i < ndil; ++i) {
		pdol[i].dol_link = dilinks[i].dil_link;
		pdol[i].dol_addr = dilinks[i].dil_addr;
		pdol[i].dol_seqgive = 0;
		pdol[i].dol_seqsend = 0;
		pdol[i].dol_idle = 0;
		pdol[i].dol_npending = 0;
	}

	khead.k_msg = (char *) pdol;

	if (ksend(&khead)) return(LAMERROR);

	return(0);
}


/*
 *	initrouter
 *
 *	Function:	- sends route information to router
 *			- LAM specific
 *			- assumes all nodes are neighbours
 *	Accepts:	- array (of length ndil) specifying how many CPUs
 *                      - on each node
 */
static void
initrouter(struct diinfo *pdiinfo, int4 mynodeid)
{
	int4		i;		/* favourite index */
	int4		nnodes;		/* # nodes */
	struct route	*proutes;	/* route array */
	struct route	*p;		/* favourite pointer */
	struct dilink	*pdil;		/* link ptr */
	struct diinfo	*pdii;		/* link ptr */
	int4		origin;		/* origin node ID */
/*
 * Determine and origin node ID
 */
	if (ao_taken(lam_daemon_optd, "o")) {
		ao_intparam(lam_daemon_optd, "o", 0, 0, &origin);
	} else {
		origin = 0;
	}
/*
 * Count the nodes.
 */
	for (nnodes = 0, i = 0; i < ndil; ++i) {

		if (dilinks[i].dil_link != NOTLINKID) {
			nnodes++;
		}
	}
/*
 * Allocate a route array.
 */
	proutes = (struct route *) malloc((unsigned) nnodes *
			sizeof(struct route));
	if (proutes == 0) lampanic("dli_inet (malloc)");
/*
 * Fill the route array.
 */
	p = proutes;

	for (pdii = diinfo, pdil = dilinks, i = 0; i < ndil; 
	     ++i, ++pdii, ++pdil) {

		if (pdil->dil_link == NOTLINKID) 
		  continue;

		p->r_nodeid = pdil->dil_link;
		p->r_nodetype = pdii->dii_node_type;
		p->r_ncpus = pdii->dii_num_cpus;
		p->r_link = pdil->dil_link;

		if (p->r_nodeid == mynodeid) {
			p->r_nodetype |= NT_ME;
			p->r_event = NOTEVENT;
		} else {
			p->r_nodetype &= ~NT_ME;
			p->r_event = EVDL0;
		}

		p->r_event2 = p->r_event;
		p->r_link2 = p->r_link;

		if (p->r_nodeid == origin) {
			p->r_nodetype |= NT_ORIGIN;
		}

		++p;
	}

	if (rrsetrents(LOCAL, proutes, nnodes))
			lampanic("dli_inet (rrsetrents)");

	free((char *) proutes);

	if (rrsetsmart(LOCAL, RSMART)) lampanic("dli_inet (rrsetsmart)");
}


/*
 *	diqsetlink
 *
 *	Function:	- adds a new link
 *	Accepts:	- message header
 */
static void
diqsetlink(struct nmsg *pnhq)
{
	struct direq	*pdiq;
	struct direply	*pdir;
	struct dilink	*pdil;		/* link ptr */
	int4		i;
	int4		ilink;
	struct nmsg	nhr;
	struct route	newroute;
	struct dolink	newdol;

	pdiq = (struct direq *) pnhq->nh_data;
	pdil = (struct dilink *) pnhq->nh_msg;
	pdil->dil_link = ttol(pdil->dil_link);
	ilink = pdil->dil_link;

	if (ilink >= ndil) {
		dilinks = (struct dilink *) realloc((char *) dilinks,
				(unsigned) (ilink + 1) *
				sizeof(struct dilink)); 
		if (dilinks == 0) 
                  lampanic("dli_inet (realloc)");

		for (i = ndil; i < ilink; ++i) {
			dilinks[i].dil_link = NOTLINKID;
		}

		ndil = ilink + 1;
	}

	memcpy((char *) &dilinks[ilink], pnhq->nh_msg, sizeof(struct dilink));

	dilinks[ilink].dil_seqrecv = 0;
/*
 * Inform router of the new link.
 */
	newroute.r_nodeid = ilink;
	newroute.r_nodetype = NT_JONES | pdiq->diq_type;
	newroute.r_link = ilink;
	newroute.r_event = EVDL0;
	newroute.r_link2 = ilink;
	newroute.r_event2 = EVDL0;
	newroute.r_ncpus = pdiq->diq_ncpus;

	if (rrsetrents(LOCAL, &newroute, 1))
          lampanic("dli_inet (rrsetrents)");
/*
 * Inform dlo of the new link.
 */
	newdol.dol_link = pdil->dil_link;
	newdol.dol_addr = pdil->dil_addr;
	newdol.dol_seqgive = 0;
	newdol.dol_seqsend = 0;
	newdol.dol_idle = 0;
	newdol.dol_npending = 0;

	if (ldosetlink(&newdol)) 
          lampanic("dli_inet (ldosetlink)");

	pdir = (struct direply *) nhr.nh_data;
	pdir->dir_reply = 0;

	nhr.nh_node = pdiq->diq_src_node;
	nhr.nh_event = pdiq->diq_src_event;
	nhr.nh_type = 0;
	nhr.nh_flags = NOBUF;
	nhr.nh_length = 0;
	nhr.nh_msg = 0;

	if (nsend(&nhr)) 
          lampanic("dli_inet (nsend)");
}


/*
 *	diqremlink
 *
 *	Function:	- removes an existing link
 *			- signals local processes
 *	Accepts:	- message header
 */
static void
diqremlink(struct nmsg* pnhq)
{
	struct nmsg	nhr;
	struct direq	*pdiq;
	struct direply	*pdir;

	pdiq = (struct direq *) pnhq->nh_data;
	dilinks[pdiq->diq_link].dil_link = NOTLINKID;

	if (rremrent(LOCAL, pdiq->diq_link)) lampanic("dli_inet (rremrent)");
	if (ldoremlink(pdiq->diq_link)) lampanic("dli_inet (ldoremlink)");

	while (dilinks[ndil - 1].dil_link == NOTLINKID) {
		ndil--;
	}
/*
 * Signal local processes.
 */
	if (rpdoom(LOCAL, SELECT_APPL, 0, SIGSHRINK))
			lampanic("dli_inet (rpdoom)");

	if (pdiq->diq_src_node != NOTNODEID) {
		pdir = (struct direply *) nhr.nh_data;
		pdir->dir_reply = 0;

		nhr.nh_node = pdiq->diq_src_node;
		nhr.nh_event = pdiq->diq_src_event;
		nhr.nh_type = 0;
		nhr.nh_flags = NOBUF;
		nhr.nh_length = 0;
		nhr.nh_msg = 0;

		if (nsend(&nhr)) lampanic("dli_inet (nsend)");
	}
}
