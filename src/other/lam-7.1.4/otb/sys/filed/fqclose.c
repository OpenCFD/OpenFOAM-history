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
 *	Copyright 1995 The Ohio State University
 *	GDB
 *
 *	$Log: fqclose.c,v $
 *	Revision 6.3  2002/10/09 20:56:35  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:28  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:41:16  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:30:55  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  96/02/01  21:08:52  gdburns
 * Remove cubix support into a separate library.
 * 
 * Revision 5.2  94/08/22  13:47:50  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  10:41:51  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:26:28  gdburns
 * Ohio Release
 * 
 *	Function:	- FQCLOSE request
 *			- two possibilites are:
 *			  1) reused file - decrement the count and reply
 *			  2) simple file - close it and reply
 */

#include "filed.h"
#include <freq.h>
#include <terror.h>
#include <typical.h>

/*
 * external functions
 */
extern struct fdesc	*fdfind();	/* find a filed descriptor */
extern void		fdcut();	/* free filed descriptor */
extern void		fsendr();	/* general purpose file reply */
extern int4		fmyclose();	/* portable close interface */

/*
 *	fqclose
 *
 *	Function:	- handles simple client close
 *	Accepts:	- ptr to filed request
 *			- ptr to filed reply
 */
void
fqclose(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target descriptor */
	int4		ret;		/* result of close() */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, (int4) ERROR, INT4_NIL, INT4_NIL);
	}
/*
 * reused file
 */
	else if (f->f_count > 1) {
		f->f_count -= 1;
		f->f_src_node = fq->fq_src_node;
		f->f_src_event = fq->fq_src_event;
		fsendr(fq->fq_src_node, fq->fq_src_event,
				0, INT4_NIL, INT4_NIL, INT4_NIL);
	}
/*
 * plain file
 */
	else {
		if (f->f_tfd <= 2) {
			ret = 0;
		} else {
			ret = (f->f_tflags & FACTIVE) ? fmyclose(f->f_fd) : 0;
		}

		if (ret == 0) errno = 0;
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, ret, INT4_NIL, INT4_NIL);
		fdcut(f);
	}
}
