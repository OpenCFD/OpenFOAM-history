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
 *	$Id: fqwrite.c,v 6.3 2002/10/09 20:56:36 brbarret Exp $
 * 
 *	Function:	- writes data to an open file descriptor
 *			- length limited to MAXNMSGLEN
 *			- must be aware of Cubix multi mode
 *	Accepts:	- filed request
 */

#include <errno.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern char		fbuf[MAXNMSGLEN];
					/* body of filed request */

/*
 * external functions
 */
extern struct fdesc	*fdfind();	/* find a filed descriptor */
extern void		fsendr();	/* general purpose file reply */
extern void		fdactivate();	/* make sure tfd is active */
extern int4		fmywrite();

void
fqwrite(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target descriptor */
	int4		ret;		/* bytes written by mywrite() */
	int		err;		/* errno */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				EBADF, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}

	f->f_src_node = fq->fq_src_node;
	f->f_src_event = fq->fq_src_event;
/*
 * Ensure that the file is activated.
 */
	fdactivate(f);
	ret = fmywrite(f->f_fd, fbuf, fq->fq_length);

	if (ret != ERROR) {
		f->f_flow += ret;
		f->f_ptr += ret;
		err = 0;
	}

	else {
		err = errno;
	}

	if (! (f->f_tflags & LAM_O_1WAY)) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				err, ret, INT4_NIL, INT4_NIL);
	}
}
