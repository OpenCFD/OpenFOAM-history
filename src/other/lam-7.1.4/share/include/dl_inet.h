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
 * $Id: dl_inet.h,v 6.9 2003/03/25 00:34:01 jsquyres Exp $
 * 
 *	Function:	- dli/dlo_inet constants and templates
 */

#ifndef LAM_DL_INET
#define LAM_DL_INET

#include <sys/types.h>
#include <netinet/in.h>

#include <lam_config.h>
#include <portable.h>
#include <net.h>

/*
 * dli/dlo common constants
 */
#define TO_DLO_ESTIMATE	200		/* timeout estimate count trigger */

#define DLDEFPORT	5555		/* default dl_inet port */
#define DLMAXSEQ	0x40000000	/* max sequence # */
#define DLMINFRAMELEN	(sizeof(struct dlframe) - MAXNMSGLEN)
					/* header-only frame size */

/*
 * dlo constants
 */
#define DOSLOTCNT	8		/* max # window slots */
#define DOMAXPENDING	3		/* max # pending messages */
#define DOMAXSTALE	((int) (LAM_TO_DLO_IDLE / (LAM_TO_DLO_ACK / 1000000.0)))
					/* causes an alive message */
#ifndef DOMAXRESEND
#define DOMAXRESEND	300		/* max # resends per message */
#endif

/*
 * dlo slot flags
 */
#define DORESEND	1		/* resend-on-timeout flag */
#define DOPENDING	2		/* message pending */
#define DORESERVED	4		/* reserved for receive */

/*
 * sequence macros
 */
#define	seqinc(a) ((a + 1) % DLMAXSEQ)
#define	seqmin(a, b) (((b - a) > 0) ? (((b - a) > (DLMAXSEQ / 2)) ? b : a) : \
		(((a - b) > (DLMAXSEQ / 2)) ? a : b))

/*
 * dlo requests
 */
#define DOQSETLINK	0
#define DOQREMLINK	1
#define DOQGETLINKS	2
#define DOQVOPEN	3		/* open dlo valve */
#define DOQVCLOSE	4		/* close dlo valve */

/*
 * dli requests
 */
#define DIQSETLINK	0
#define DIQSANITY	1
#define DIQREMLINK	2

/*
 * dlo request template
 */
struct doreq {
	int4		doq_req;	/* request code */
	int4		doq_src_node;	/* client node */
	int4		doq_src_event;	/* client event */
	int4		doq_link;	/* output link */
	int4		doq_extra;	/* extra information */
};

/*
 * dlo reply
 */
struct doreply {
	int4		dor_reply;	/* reply code */
	int4		dor_nlinks;	/* # datalinks */
};

/*
 * dli request
 */
struct direq {
	int4		diq_req;	/* request code */
	int4		diq_src_node;	/* client node */
	int4		diq_src_event;	/* client event */
	int4		diq_link;
	int4		diq_type;
	int4		diq_ncpus;
};

/*
 * dli reply
 */
struct direply {
	int4		dir_reply;	/* reply code */
};

/*
 * output datalink
 */
struct dolink {
	int4		dol_link;	/* link number */
	int4		dol_seqgive;	/* next assignable sequence # */
	int4		dol_seqsend;	/* next sendable sequence # */
	int4		dol_npending;	/* # pending seq. # */
	int4		dol_idle;	/* idle timeout periods */
	struct sockaddr_in
			dol_addr;	/* dli socket address */
};

/*
 * extra information from startup protocols
 */
struct diinfo {
  int4 dii_num_cpus;
  int4 dii_node_type;
};

/*
 * input datalink
 */
struct dilink {
	int4		dil_link;	/* link number */
	int4		dil_seqrecv;	/* next recvable sequence # */
	struct sockaddr_in
			dil_addr;	/* dli socket address */
};

/*
 * network portion of struct nmsg
 */
struct dlheader {
	int4		dlh_node;
	int4		dlh_event;
	int4		dlh_type;
	int4		dlh_length;
	int4		dlh_flags;
	int4		dlh_data[NHDSIZE];
};

/*
 * datalink frame
 */
struct dlframe {
	int4		dlf_src_node;	/* message source node */
	int4		dlf_seqnum;	/* message sequence # */
	struct dlheader	dlf_msghead;	/* message header */
	char		dlf_msg[MAXNMSGLEN];
					/* message body */
};

struct doslot {
	int4		dos_link;	/* outgoing link */
	int4		dos_flags;	/* slot status flags */
	int4		dos_resend;	/* # resends */
	struct dlframe	dos_frame;	/* datalink frame */
};

struct dlack {
	int4		dla_seqnum;	/* ack sequence # */
	int4		dla_destid;	/* destination node */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		ldogetlinks __ARGS((struct dolink **pdol,int4 *pndol));
extern int		ldosetlink __ARGS((struct dolink *pdol));
extern int		ldoremlink __ARGS((int4 link));

#ifdef __cplusplus
}
#endif

#endif /* LAM_DL_INET */
