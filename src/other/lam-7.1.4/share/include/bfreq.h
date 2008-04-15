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
 *	Revision 6.2  1999/03/17 14:19:39  jsquyres
 *
 *	Revision 6.1  1996/11/23 21:54:46  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:53:17  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.2  96/02/26  20:48:46  gdburns
 * Quadruple buffer space limit.
 * 
 * Revision 5.2.1.1  96/02/18  01:41:44  gdburns
 * Remove process state.
 * 
 * Revision 5.2  94/08/22  14:00:25  gdburns
 * Ohio Release
 * 
 * Revision 5.1.1.2  94/08/16  16:28:55  raja
 * Fixed rbfprocstate() prototype.
 * 
 * Revision 5.1.1.1  94/05/25  10:31:51  gdburns
 * Use struct bftask for process status.
 * 
 * Revision 5.1  94/05/18  12:44:16  gdburns
 * Ohio Release
 * 
 * Revision 2.3.1.1  94/05/18  11:00:24  gdburns
 * Upgrade SQL to full MPI.
 * 
 * Revision 2.3  94/04/22  12:48:39  gdburns
 * Ohio Release
 * 
 *	Function:	- bufferd constants and templates
 */

#ifndef _BFREQ
#define _BFREQ

#include <all_list.h>
#include <net.h>
#include <portable.h>

/*
 * bufferd request codes
 */
#define BFQWIPE		0
#define BFQRECV		1
#define	BFQPROBE	2
#define BFQSTATE	3
#define BFQSWEEP	4
#define BFQSWEEP1	5
#define	BFQSPACE	6
#define	BFQLOOK		7
#define	BFQMAX		8

/*
 * general bufferd constants
 */
#define BFDEFSPACE	0x800000
#define BFSMAX		20
#define BFNEV		67
#define BFNOTSEQ	-1
#define BFSEQLEN	256

/*
 * synchronization comparisons
 */
#define bfq_bfb_cmp(q, b) ((q->bfq_flags & KSYNCSQL) ? \
		ksync_type_sql(q->bfq_type, b->bfb_head.nh_type) : \
		ksync_type(q->bfq_type, b->bfb_head.nh_type))

#define bfk_nh_cmp(k, n) (((n)->nh_flags & KSYNCSQL) ? \
		(ksync_event_sql((n)->nh_event, (k)->bfk_event) && \
		ksync_type_sql((n)->nh_type, (k)->bfk_type)) : \
		(ksync_event((n)->nh_event, (k)->bfk_event) && \
		ksync_type((n)->nh_type, (k)->bfk_type)))

/*
 * message synchronization point
 */
struct bfsync {
	int4		bfk_event;
	int4		bfk_type;
};

/*
 * bufferd request
 */
struct bfreq {
	int4		bfq_src_node;
	int4		bfq_src_event;
	int4		bfq_req;
	int4		bfq_maxspace;
	int4		bfq_event;
#define bfq_seq		bfq_event
	int4		bfq_type;
	int4		bfq_maxtbl;
	int4		bfq_flags;
};

/*
 * bufferd reply
 */
struct bfreply {
	int4		bfr_reply;
	int4		bfr_type;
	int4		bfr_maxspace;
	int4		bfr_length;
	int4		bfr_flags;
	int4		bfr_nspace;
#define bfr_data0	bfr_nspace
	int4		bfr_nbuf;
#define bfr_data1	bfr_nbuf
	int4		bfr_index;
};

/*
 * message buffer status
 */
struct bfstatus {
	int4		bfs_node;	/* destination node */
	int4		bfs_event;	/* destination event */
	int4		bfs_type;	/* destination type */
	int4		bfs_length;	/* message length */
	int4		bfs_flags;	/* message flags */
	int4		bfs_seq;	/* sequence number */
	int4		bfs_data[NHDSIZE];
};

/*
 * bufferd parameters
 */
struct bfparms {
	int4		bfp_maxspace;
	int4		bfp_nspace;
};

/*
 * buffered event
 */
struct bfevent {
	int4		bfe_event;	/* event key */
	LIST		*bfe_nmsglp;	/* message list */
};

/*
 * buffered message
 */
struct bfbuf {
	struct nmsg	bfb_head;	/* message header */
	int4		bfb_seq;	/* sequence # */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		bfiprobe __ARGS((struct nmsg *nhead));
extern int		bfiselect __ARGS((struct bfsync *pbfk, int nbfk,
				int4 flags, int *index));
extern int		bfrecv __ARGS((struct nmsg *nhead));
extern int		bfrecvback __ARGS((struct nmsg *nhead));
extern int		bfrecvfront __ARGS((struct nmsg *nhead));
extern int		bfselect __ARGS((struct bfsync *pbfk, int nbfk,
				int4 flags, int *index));

extern int		rbflook __ARGS((int4 node, struct nmsg *pnh_look,
				int4 seq));
extern int		rbfparms __ARGS((int4 node, struct bfparms *parms));
extern int		rbfstate __ARGS((int4 node, struct bfstatus *tbl,
				int tblsize, struct bfparms *parms));
extern int		rbfsweep __ARGS((int4 node));
extern int		rbfsweep1 __ARGS((int4 node, int4 event));
extern int		rbfwipe __ARGS((int4 node));

#ifdef __cplusplus
}
#endif

#endif /* _BFREQ */
