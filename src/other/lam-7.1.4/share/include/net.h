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
 *      $Id: net.h,v 6.8 2003/03/19 19:16:22 jsquyres Exp $
 *
 *	Function:	- constants and structures associated with the
 *			  network subsystem
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#ifndef LAM_NET_H
#define LAM_NET_H

#include <lam_config.h>
#include <kreq.h>

/*
 * network constants
 */
#define	MAXNMSGLEN	8192		/* maximum network frame size */
#define	NOTNODEID	(-1)	/* not a valid node ID */
#define NOTLINKID       (-1)    /* not a valid logical link ID */
#define	NOTNODETYPE	(-1)	/* not a valid node type */
#define	LOCAL		(-2)	/* on the local node */
#define	ORIGIN		(getorigin())	/* should be user's node */

/*
 * max # links (ITB, OTB and generic)
 */
#define MAXITBNLINK	4
#define MAXOTBNLINK	256

#ifdef itb
#define MAXNLINK	MAXITBNLINK
#else
#define MAXNLINK	MAXOTBNLINK
#endif

/*
 * node types and masks
 */
#define NT_ITB		0x01		/* ITB, not-OTB */
#define NT_CAST		0x02		/* a multicast of nodes */
#define NT_WASTE	0x04		/* not fully used/wasted */
#define NT_DISK		0x08		/* node has a disk */
#define NT_TUBE		0x10		/* node has video unit */
#define NT_ALL		0x1F		/* match all bit-flags */
#define NT_JONES	0x20		/* node is my neighbour */
#define NT_BOOT		0x40		/* node to boot */
#define NT_ME		0x80		/* my local node */
#define NT_ORIGIN	0x100		/* origin booting node */
#define NT_ERROR	NOTNODETYPE	/* invalid node type */

/*
 * node type inquiry macros
 */
#define tisitb(x)	((x) & NT_ITB)		/* node is itb */
#define tisotb(x)	(!tisitb(x))		/* node is otb */
#define tiscast(x)	((x) & NT_CAST)		/* is a cast */
#define tiswaste(x)	((x) & NT_WASTE)	/* is a wasted node */
#define tiscomp(x)	(!tiswaste(x))		/* is a compute node */
#define tisdisk(x)	((x) & NT_DISK)		/* node has disk */
#define tistube(x)	((x) & NT_TUBE)		/* node has video */
#define tisbad(x)	((x) & NT_ERROR)	/* node type is invalid */

#define getnitb()	getntype(NT_ITB, NT_ITB | NT_CAST)	/* all itbs */
#define getnotb()	getntype(0, NT_ITB | NT_CAST)		/* all otbs */
#define getnall()	getntype(0, NT_CAST)			/* all nodes */
#define getncast()	getntype(NT_CAST, NT_CAST)		/* all casts */
#define getncomp()	getntype(0, NT_WASTE | NT_CAST)		/* all comps */
#define getnjones()	getntype(NT_JONES, NT_JONES)		/* all jones */

/*
 * getnodes wrappers to go with the getntype macros above
 */
#define getitbs(x,y)	getnodes((x), (y), NT_ITB, NT_ITB | NT_CAST) 
#define getotbs(x,y)	getnodes((x), (y), 0, NT_ITB | NT_CAST)
#define getall(x,y)	getnodes((x), (y), 0, NT_CAST)
#define getcasts(x,y)	getnodes((x), (y), NT_CAST, NT_CAST)
#define getcomps(x,y)	getnodes((x), (y), 0, NT_WASTE | NT_CAST)
#define getjones(x,y)	getnodes((x), (y), NT_JONES, NT_JONES)

/*
 * node casts (implies an alias and a source)
 */
#define HOST2ALL	(-3)	/* origin otb to all nodes */
#define HOST2ITB	(-4)	/* origin otb to all ITB nodes */
#define HOST2OTB        (-6)	/* origin otb to all OTB nodes */
#define HOST2COMPNODE	(-7)	/* origin otb to all compute nodes */
#define HOST2COMPCPU	(-8)	/* origin otb to all compute CPUs */

/*
 * network message header
 */
#define NHDSIZE		8		/* # of data words in header */

struct nmsg {
	int4	 nh_dl_event;		/* datalink output event from router */
	int4	 nh_dl_link;		/* datalink output link from router */
	int4	 nh_node;		/* destination process's node */
	int4	 nh_event;		/* destination process's event */
	int4	 nh_type;		/* destination process's type */
	int4	 nh_length;		/* how much data to transmit */
	int4	 nh_flags;		/* network flags */
#define NOBUF		0x10		/* don't do buffering at all */
#define N2ND		0x40		/* packets >= 2 */
#define DTBO		0x100		/* data/msg in Trollius byte order */
#define DDLI		0x200		/* categorize as datalink input pkt */
#define NREEL		0x400		/* multireel */
#define NMORE		0x1000		/* more packets coming */
#define DRAWDATA	0x10000		/* nh_data contains raw data */
#define DINT4DATA	0x20000		/* nh_data contains 4-byte integers */
#define DFLT4DATA	0x40000		/* nh_data contains 4-byte floats */
#define DFLT8DATA	0x80000		/* nh_data contains 8-byte floats */
#define DMSKDATA	0xf0000		/* mask nh_data flags */
#define DRAWMSG		0x100000	/* nh_msg contains raw data */
#define DINT4MSG	0x200000	/* nh_msg contains 4-byte integers */
#define DFLT4MSG	0x400000	/* nh_msg contains 4-byte floats */
#define DFLT8MSG	0x800000	/* nh_msg contains 8-byte floats */
#define DINT2MSG	0x1000000	/* nh_msg contains 2-byte integers */
#define DMSKMSG		0x1f00000	/* mask nh_msg flags */
	int4	 nh_data[NHDSIZE];	/* builtin data field */
	char	*nh_msg;		/* points to contiguous data */
};

/*
 * datalink message type filters
 */
#define DLALL		0		/* every message */
#define DLUSR		1		/* user messages */
#define DLSYS		2		/* system messages */
#define DLI		4		/* datalink input messages */
#define DLCTL		8		/* datalink control messages */

/*
 * network frame
 */
struct netframe {
	struct nmsg	nf_head;
	char		nf_msg[MAXNMSGLEN];
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		drecv __ARGS((struct nmsg *nhead));
extern int		dsend __ARGS((struct nmsg *nhead));
extern int		dsfr __ARGS((struct nmsg *nhs, struct nmsg *nhr));
extern int		dsfrfront __ARGS((struct nmsg *nhs, struct nmsg *nhr));
extern int		dsfrback __ARGS((struct nmsg *nhs));
extern int		dtry_recv __ARGS((struct nmsg *nhead));
extern int		dtry_send __ARGS((struct nmsg *nhead));
extern int		frecv __ARGS((struct nmsg *nhead));
extern int		frecvfront __ARGS((struct nmsg *nhead));
extern int		frecvback __ARGS((struct nmsg *nhead));
extern int		getnodes __ARGS((int4 *p, int4 n, int4 tp, int4 msk));
extern int		nprobe __ARGS((struct nmsg *nhead));
extern int		nrecv __ARGS((struct nmsg *nhead));
extern int		nsend __ARGS((struct nmsg *nhead));
extern int		ntry_recv __ARGS((struct nmsg *nhead));
extern int		ntry_send __ARGS((struct nmsg *nhead));
extern int		precv __ARGS((struct nmsg *nhead));
extern int		precvclose __ARGS((struct nmsg *nhead));
extern int		precvopen __ARGS((struct nmsg *nhead));
extern int		psend __ARGS((struct nmsg *nhead));
extern int		psendclose __ARGS((struct nmsg *nhead));
extern int		psendopen __ARGS((struct nmsg *nhead));
extern int		trecv __ARGS((struct nmsg *nhead));
extern int		tsend __ARGS((struct nmsg *nhead));
extern int4		getnodeid __ARGS((void));
extern int4		getnodetype __ARGS((void));
extern int4		getntype __ARGS((int4 type, int4 mask));
extern int4		getorigin __ARGS((void));

/*
 * kitchen sink functions
 */
extern double		ttime __ARGS((void));

#ifdef __cplusplus
}
#endif

#endif /* LAM_NET_H */
