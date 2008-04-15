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
 *	$Log: fqlseek.c,v $
 *	Revision 6.3  2002/10/09 20:56:35  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:29  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:41:20  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:30:57  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  96/02/01  21:08:09  gdburns
 * Remove cubix support into a separate library.
 * 
 * Revision 5.2  94/08/22  13:47:53  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  10:41:54  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:26:30  gdburns
 * Ohio Release
 * 
 *	Function:	- FQLSEEK request
 */

#include "filed.h"
#include <freq.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

/*
 * external functions
 */
struct fdesc		*fdfind();	/* find a filed descriptor */
void			fsendr();	/* general purpose file reply */
void			fdactivate();	/* make sure tfd is active */
int4			fmylseek();	/* portable int4 lseek() */

/*
 *	fqlseek
 *
 *	Function:	- handles simple client lseek
 *	Accepts:	- ptr to filed request
 */
void
fqlseek(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target descriptor */
	int4		ret;		/* return value of lseek() */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, (int4) ERROR, INT4_NIL, INT4_NIL);
	} else {
		fdactivate(f);
		f->f_src_node = fq->fq_src_node;
		f->f_src_event = fq->fq_src_event;
		ret = fmylseek(f->f_fd, fq->fq_offset, fq->fq_whence);

		if (ret != ERROR) {
			errno = 0;
			f->f_ptr = ret;
		}

		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, ret, INT4_NIL, INT4_NIL);
	}
}
