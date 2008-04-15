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
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: fqstate.c,v 6.3 2002/10/09 20:56:36 brbarret Exp $
 * 
 *	Function:	- FQSTATE request
 *			- a monitoring request with no associated file
 *			  operation
 */

#include <string.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <t_types.h>
#include <terror.h>

/*
 * external variables
 */
extern char		fbuf[];		/* general purpose message buffer */
extern struct fdesc	*fdtop;		/* top of descriptor list */

/*
 * external functions
 */
void			fsendr();	/* general purpose file reply */

/*
 *	fqstate
 *
 *	Function:	- sends back every file descriptor
 *	Accepts:	- filed request
 *			- assumes MAXNMSGLEN > sizeof(struct fdesc)
 */
void
fqstate(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target descriptor */
	struct fstate	*p;		/* ptr to desc state */
	int4		n;		/* total space used in buf */

	p = (struct fstate *) fbuf;
	n = 0;

	for (f = fdtop; f != FDNULL; f = f->f_next) {
/*
 * If buffer is almost full, send it back to the client and reset.
 */
		if ((n + sizeof(struct fstate)) > MAXNMSGLEN) {
			fsendr(fq->fq_src_node, fq->fq_src_event,
					0, INT4_NIL, n, INT4_NIL);
			p = (struct fstate *) fbuf;
			n = 0;
		}
/*
 * Add to the buffer.
 */
		else {
			p->fs_tfd = ltot(f->f_tfd);
			p->fs_tflags = ltot(f->f_tflags);
			p->fs_flow = ltot(f->f_flow);
			p->fs_count = ltot(f->f_count);
			p->fs_src_node = ltot(f->f_src_node);
			p->fs_src_event = ltot(f->f_src_event);
			strcpy(p->fs_name, f->f_name);
			p += 1;
			n += sizeof(struct fstate);
		}
	}

	fsendr(fq->fq_src_node, fq->fq_src_event, EEOF, INT4_NIL, n, 0);
}
