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
 *	$Id: filed.c,v 6.7 2003/06/30 05:19:30 brbarret Exp $
 * 
 *	Function:	- filed mainline 
 *			- remote daemon
 *			- a leaner, meaner fighting machine
 *			- You, too, can maintain this program.
 *
 *			- Filed is organized into four parts:
 *			  1) filed - the request/reply mainline
 *			  2) fq* - the guts of the requests
 *			  3) f* - data structure modules
 *			  4) fface - the native file interface
 *
 *			- Filed provides the standard Unix I/O
 *			  operations as a remote service to Trollius
 *			  clients.  Additional requests, specific to
 *			  Trollius, control and monitor filed functions.
 *
 *			- There are actually three types of file descriptors
 *			  in the filed world.  First, there is the user file
 *			  descriptor, ufd, which resides wholly in the client
 *			  and has the look and feel of a Unix file descriptor.
 *			  The ufd has a field which contains the Trollius
 *			  file descriptor handle.  It is passed to filed
 *			  from the client, so that filed can identify the
 *			  target Trollius file descritor, tfd.  Before an
 *			  operation is carried out on a file, the tfd must
 *			  have an associated native OS (eg, Unix) file
 *			  descriptor open, and this is referred to as the fd.
 *			  Thus, an active file has an associated open fd,
 *			  while an inactive file does not.
 *
 *			- The word handle means the identifying number of
 *			  the descriptor while the word descriptors refers
 *			  to the whole data structure associated with an open
 *			  file.
 *
 *			- Filed does not ensure that the client that called for
 *			  a file open is the same client that calls for later
 *			  operations.  Since the client does not manage the
 *			  tfd handle (he only sees the ufd), this should not
 *			  be a problem.
 */

#include <string.h>

#include <events.h>
#include "filed.h"
#include <freq.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <typical.h>


/*
 * external functions
 */
extern void		fqopen();
extern void		fqclose();
extern void		fqread();
extern void		fqwrite();
extern void		fqlseek();
extern void		fqstate();
extern void		fqchdir();
extern void		fqgetwd();
extern void		fqrmfd();
extern void		fqaccess();
extern void		fqstat();
extern void		fqfstat();
extern void		fqtrunc();
extern void		fqftrunc();
extern void		fqdup();
extern void		fqmkdir();
extern void		fqsystem();
extern void		fqrename(struct freq* fq);
extern void		fqrmdir();
extern void		fqunlink();
extern void		fqincr();
extern void		fqpathfind();
extern void		fqf77read();
extern void		fqf77back();
extern void		fqopenfd();
extern void		fdstdio();	/* set up 0, 1 and 2 */
extern int		fmyinit();	/* machine dependent initialization */


/*
 * local variables
 */
static struct nmsg	incoming;	/* incoming filed request */

static void		(*(fqfunc[FQMAX]))() = {
			fqopen,
			fqclose,
			fqread,
			fqwrite,
			fqlseek,
			fqstate,
			fqrmfd,
			fqincr,
			fqaccess,
			fqmkdir,
			fqunlink,
			fqsystem,
			fqchdir,
			fqrmdir,
			fqgetwd,
			fqstat,
			fqfstat,
			fqdup,
			fqtrunc,
			fqftrunc,
			fqopenfd,
			fqpathfind,
			fqrename,
			fqf77read,
			fqf77back,
			};
/*
 * global variables
 */
char			fbuf[MAXNMSGLEN];


/*
 * global functions
 */
void			(*(f_init()))();
void			(*(filed()))();


/*
 *	f_init
 *
 *	Function:	- filed initialization
 */
void 
(*(f_init()))()
{
        memset((void*) fbuf, -1, MAXNMSGLEN);
	if (fmyinit()) lampanic("filed (fmyinit)");
/*
 * Attach to kernel.
 */
	if (lpattach("filed")) lampanic("filed (lpattach)");
/*
 * Set up standard I/O descriptors.
 */
	fdstdio();
/*
 * Receive first request.
 */
	incoming.nh_event = EVFILED;
	incoming.nh_type = 0;
	incoming.nh_flags = 0;
	incoming.nh_msg = fbuf;
	incoming.nh_length = sizeof(fbuf);

	if (nrecv(&incoming)) lampanic("filed (nrecv)");

	return((void (*)()) filed);
}

/*
 *	filed
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(filed()))()
{
	struct freq	*fq;		/* filed request details */

	fq = (struct freq *) incoming.nh_data;
/*
 * Process the request.
 * Individual request functions handle replies, if any.
 */
	if ((fq->fq_req < FQMAX) && (fq->fq_req >= 0)) {
		(*(fqfunc[fq->fq_req]))(fq);
	}
/*
 * Receive next request.
 */
	incoming.nh_event = EVFILED;
	incoming.nh_flags = 0;
	incoming.nh_msg = fbuf;
	incoming.nh_type = 0;
	incoming.nh_length = sizeof(fbuf);

	if (nrecv(&incoming)) lampanic("filed (nrecv)");

	return((void (*)()) filed);
}


/*
 *	fsendr
 *
 *	Function:	- sends reply back to client
 *			- copies parameters into the outgoing (reply) message
 *			  descriptor and calls nsend()
 *
 *	Accepts:	- client node
 *			- client event
 *			- reply error number (will be set as client errno)
 *			- reply return value (will be return to client caller)
 *			- length of buffer to return
 *			- message flags
 */
void
fsendr(int4 node, int4 event, int err, int4 ret, int4 length, int4 flags)
{
	struct nmsg	outgoing;	/* outgoing filed reply */
	struct freply	*fr;		/* filed reply details */

	LAM_ZERO_ME(outgoing);
	outgoing.nh_node = node;
	outgoing.nh_event = event;
	outgoing.nh_type = 0;
	outgoing.nh_flags = flags;
	outgoing.nh_length = length;
	outgoing.nh_msg = fbuf;

	fr = (struct freply *) outgoing.nh_data;
	fr->fr_errno = (int4) err;
	fr->fr_ret = ret;

	if (nsend(&outgoing)) lampanic("filed (nsend)");
}
