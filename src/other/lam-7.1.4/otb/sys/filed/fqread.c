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
 *	$Log: fqread.c,v $
 *	Revision 6.3  2002/10/09 20:56:36  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *
 *	Revision 6.2  1999/06/12 17:11:29  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:41:23  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:30:59  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  96/02/01  21:08:26  gdburns
 * Remove cubix support into a separate library.
 * 
 * Revision 5.2  94/08/22  13:47:55  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  10:41:57  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:26:31  gdburns
 * Ohio Release
 * 
 *	Function:	- FQREAD request
 *			- exits daemon in case of non-user error
 *			- read operations longer than a network message
 *			  are done in packets
 *	Accepts:	- filed request
 */

#include <errno.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

/*
 * external functions
 */
struct fdesc		*fdfind();	/* find a filed descriptor */
void			fsendr();	/* general purpose file reply */
void			fdactivate();	/* make sure tfd is active */
int4			fmyread();	/* portable read() */

/*
 * global variables
 */
extern char		fbuf[];		/* input data from read */

void
fqread(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* target filed descriptor */
	int4		ret;		/* length returned by myread */
	int4		len;		/* read packet length */
	int4		err;		/* consistent errno */

	
	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				EBADF, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}

	f->f_src_node = fq->fq_src_node;
	f->f_src_event = fq->fq_src_event;

	fdactivate(f);
/*
 * We want to perform at least one read operation even if the
 * length is 0.  Multiple reads are performed if the length requested
 * exceeds the network message size.  The client will be able to
 * detect, based on the reply->fr_ret, which is the last packet.
 */
	do {
		len = (fq->fq_length > MAXNMSGLEN) ? MAXNMSGLEN :
				fq->fq_length;
		ret = fmyread(f->f_fd, fbuf, len);

		if (ret == ERROR) {
			fq->fq_length = 0;
			err = errno;
			len = 0;
		} else {
			err = 0;
			f->f_ptr += ret;
			f->f_flow += ret;

			if (ret < len) {
				fq->fq_length = 0;
				len = ret;
			} else {
				fq->fq_length -= len;
			}
		}

		fsendr(fq->fq_src_node, fq->fq_src_event, err, ret,
				len, f->f_tflags & LAM_O_DMSG);
		
	} while (fq->fq_length > 0);
}
