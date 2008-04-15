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
 *	$Id: flatreq.h,v 6.8 2002/10/09 20:57:23 brbarret Exp $
 *
 *	Function:	- constants and structures associated
 *			  with the flat server
 */

#ifndef _FLATREQ_H
#define _FLATREQ_H

#include "lam_config.h"
#include "net.h"
#include "portable.h"
#include "preq.h"

/*
 * constants
 */
#define FLPATHLEN	96			/* load file path length */
#define FLMAX		256
#define	EMPTY		((char *) 0)

/*
 * request codes
 */
#define FLQLOAD		1
#define FLQGET		2
#define FLQFORGET	3
#define FLQDONE		4
#define FLQCLEAN	5

/*
 * request template
 */
struct flreq {
	int4		flq_src_node;
	int4		flq_src_event;
	int4		flq_req;
	int4		flq_parm1;
#define flq_tag		flq_parm1
#define flq_pid		flq_parm1
	int4		flq_parm2;
#define flq_malength	flq_parm2
#define flq_dseg	flq_parm2
	int4		flq_parm3;
#define flq_ldlength	flq_parm3
};

/*
 * reply template
 */
struct flreply {
	int4		flr_reply;
};

/*
 * flat table entry
 */
struct flregion {
	int4		fl_tag;			/* identifying tag */
	int4		fl_status;		/* status flag */
	int4		fl_src_event;		/* sender's -pid */
	int4		fl_src_node;		/* sender's nodeid */
	int4		fl_byteswritten;	/* how many so far */
	char		*fl_addr;		/* flatted file name */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		rflat __ARGS((int4, char *, int4, int4, int4));
extern int		rflatv __ARGS((int4, char **, struct penv *,
    					int4 *, int4 *));
extern int		rflclean __ARGS((int4, int4));
extern char		*rget __ARGS((int4, int4));
extern char             *rforget __ARGS((int4, int4));

#ifdef __cplusplus
}
#endif

#endif /* _FLATREQ_H */
