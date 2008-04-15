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
 *      $Id: trreq.h,v 6.5 2002/10/09 20:57:26 brbarret Exp $
 * 
 *	Function:	- constants and templates associated with traced
 */

#ifndef _TRREQ
#define _TRREQ

#include <lam_config.h>
#include <all_list.h>
#include <portable.h>

/*
 * traced requests
 */
#define	TRQSTORE	0		/* store traces */
#define	TRQGET		1		/* get traces */
#define	TRQFORGET	2		/* get and remove traces */
#define	TRQSWEEP	3		/* remove all traces */
#define	TRQUDIE		4		/* remove pid related traces */
#define	TRQWIPE		5		/* reset traced */
#define	TRQNAMEPUB	6		/* publish name */
#define	TRQNAMEDEL	7		/* delete name */
#define	TRQNAMEGET	8		/* get name */
#define	TRQCLEANOBJS	9		/* cleanup objects */
#define TRQMAX          10		/* # requests */

/*
 * misc constants
 */
#define TRSMAX		100		/* # state descs in trstate */
#define TRHASHSIZE	100		/* initial size, trace hash tbl */ 
#define TRMAXSPACE	0x200000	/* total trace space limit */
#define TRANYLIST	-1		/* matches any trace list */
#define TRANYPID	-1		/* matches any pid */
#define TRNAMELIST	-99		/* name publishing list */
#define TRDEFFNAME	"def.lamtr"	/* default trace output file */

/*
 * traced request
 */
struct trreq {
	int4		trq_src_node;	/* client's nodeid */
	int4		trq_src_event;	/* client's reply event	*/
	int4		trq_req;	/* traced request code */
	int4		trq_pid;	/* client pid */
	int4		trq_length;	/* trace length */
	int4		trq_lnum;	/* list number */
};

/*
 * traced reply
 */
struct trreply {
	int4		trr_errno;	/* error code */
	int4		trr_length;	/* length of selected trace data */
};

/*
 * trace buffer
 */
struct trbuffer {
	int4		trb_length;	/* buffer length */
	int4		trb_pid;	/* buffer pid */
	char		*trb_buffer;	/* buffer ptr */
};

/*
 * trace list
 */
struct trblist {
	int4		trl_lnum;	/* list number */
	LIST		*trl_ld;	/* list descriptor */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		lam_rtrstore __ARGS((int4 node, int4 lnum, int4 pid,
					const char *bf, int4 len));
extern int		lam_rtrget __ARGS((int4 node, int4 lnum,
					int4 pid, char **buffer));
extern int		lam_rtrforget __ARGS((int4 node, int4 lnum,
					int4 pid, char **buffer));
extern int		lam_rtrfget __ARGS((int4 node, int4 lnum,
					int4 pid, int fd));
extern int		lam_rtrfforget __ARGS((int4 node, int4 lnum,
					int4 pid, int fd));
extern int		lam_rtrsweep __ARGS((int4 node, int4 lnum));
extern int		lam_rtrudie __ARGS((int4 node, int4 pid));
extern int		lam_rtrwipe __ARGS((int4 node));
extern int		lam_rtrnamepub __ARGS((int4 node, int4 id, char *name,
					char *data));
extern int		lam_rtrnamedel __ARGS((int4 node, char *name));
extern int		lam_rtrnameget __ARGS((int4 node, char *name,
					char **data));
extern int		lam_rtrnamesweep __ARGS((int4 node, int4 id));
extern int		lam_rtrcleanobjs __ARGS((int4 node));

#ifdef __cplusplus
}
#endif

#endif
