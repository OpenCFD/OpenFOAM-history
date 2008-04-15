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
 *	$Id: loadd.c,v 6.7 2002/10/09 20:56:50 brbarret Exp $
 *
 *	Function	- load daemon
 *			- loads local file onto remote node
 *			- written like a state machine to accomodate
 *			  operation as an internal process
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <lamdebug.h>
#include <events.h>
#include <flatreq.h>
#include <ksignal.h>
#include <lreq.h>
#include <net.h>
#include <preq.h>
#include <priority.h>
#include <rreq.h>
#include <terror.h>
#include <sfh.h>

/*
 * global functions
 */
void			(*(l_init()))();

/*
 * local variables
 */
static struct lreq	*lq;		/* loadd request */
static struct lreply	*lr;		/* loadd reply */
static struct flreq	*flq;		/* flatd request */
static struct flreply	*flr;		/* flatd reply */
static char		*filebuf;	/* file memory buffer */
static char		fname[FNAMELEN];/* name of file to load */
static int		mask;		/* signal mask */
static int4		ldlength;	/* file size */
static struct nmsg	lqhead;		/* loadd request msg */
static struct nmsg	lrhead;		/* loadd reply msg */
static struct nmsg	flqhead;	/* flatd request msg */
static struct nmsg	flrhead;	/* flatd reply msg */

/*
 * local functions
 */
static void		(*(loadd()))();
static void		(*(lflreply()))();
static void		(*(lflsync()))();
static void		(*(lsendr()))();

/*
 *	l_init
 *
 *	Function:	- loadd initialization
 */
void (*(
l_init()))()

{
	lq = (struct lreq *) lqhead.nh_data;
	lr = (struct lreply *) lrhead.nh_data;
	flq = (struct flreq *) flqhead.nh_data;
	flr = (struct flreply *) flrhead.nh_data;
/*
 * Attach to kernel.
 */
	if (lpattach("loadd")) lampanic("loadd (lpattach)");
/*
 * Receive first request.
 */
	lqhead.nh_event = EVLOADD;
	lqhead.nh_type = 0;
	lqhead.nh_flags = 0;
	lqhead.nh_length = FNAMELEN;
	lqhead.nh_msg = fname;

	if (nrecv(&lqhead)) lampanic("loadd (nrecv)");

	return((void (*)()) loadd);
}

/*
 *	loadd
 *
 *	Function:	- services next loadd request
 */
static void (*(
loadd()))()

{
	struct stat	status;		/* store file status */
	int		fdesc;		/* local file descriptor */
	char		*fullname;	/* full path filename */

	lamlog("loadd: dest_node: %X src_node: %X file: %s\n",
	       lq->lq_dest_node, lq->lq_src_node, fname);
/*
 * Locate the file.
 */
	fullname = sfh_path_env_find(fname, R_OK | X_OK);

	if (fullname == 0) {
		lr->lr_reply = errno;
		lsendr();
		return((void (*)()) loadd);
	}
/*
 * Get file information.
 */
	if (stat(fullname, &status)) {
		lr->lr_reply = errno;
		lsendr();
		free(fullname);
		return((void (*)()) loadd);
	}

	if (status.st_size == 0) {
		lr->lr_reply = EINVAL;
		lsendr();
		free(fullname);
		return((void (*)()) loadd);
	}

	ldlength = status.st_size;
/*
 * Open file to be loaded.
 */
	if ((fdesc = open(fullname, O_RDONLY, 0600)) < 0) {
		lr->lr_reply = EINVAL;
		lsendr();
		free(fullname);
		return((void (*)()) loadd);
	}

	free(fullname);
/*
 * Allocate space for the file.
 */
	if ((filebuf = malloc((unsigned) ldlength)) == 0)
			lampanic("loadd (malloc)");
/*
 * Read in the file.
 */
	if (read(fdesc, filebuf, ldlength) < 0) lampanic("loadd (read)");
/*
 * Close the file.
 */
	if (close(fdesc)) lampanic("loadd (close)");
/*
 * Load file on destination node.
 */
	flq->flq_src_node = ((lq->lq_dest_node == LOCAL) ||
			(getrtype(lq->lq_dest_node) & NT_CAST)) ?
			lq->lq_dest_node : getnodeid();
	flq->flq_src_event = -kbgetpid();
	flq->flq_req = FLQLOAD;
	flq->flq_ldlength = ldlength;
	flq->flq_malength = ldlength;
	flq->flq_tag = lq->lq_tag;

	flqhead.nh_node = lq->lq_dest_node;
	flqhead.nh_event = EVFLATD;
	flqhead.nh_type = 0;
	flqhead.nh_flags = 0;
	flqhead.nh_length = 0;
	flqhead.nh_msg = 0;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&flqhead)) {

		if (errno == EBADNODE) {
			lr->lr_reply = errno;
			lsendr();
			free(filebuf);
			return((void (*)()) loadd);
		} else {
			lampanic("loadd (nsend)");
		}
	}

	flrhead.nh_event = -kbgetpid();
	flrhead.nh_type = 0;
	flrhead.nh_flags = 0;
	flrhead.nh_length = 0;
	flrhead.nh_msg = 0;
	
	if (nrecv(&flrhead)) lampanic("loadd (nrecv)");

	return((void (*)()) lflreply);
}

/*
 *	lflreply
 *
 *	Function:	- responds to reply from flatd
 *			- replies to loadd client if flatd error
 *			- ships file to flatd if ok
 */
static void (*(
lflreply()))()

{
	if (flr->flr_reply) {
		ksigsetmask(mask);
		lr->lr_reply = flr->flr_reply;
		return((void (*)()) lsendr);
	}
/*
 * Load file buffer to destination node.
 */
	flqhead.nh_msg = filebuf;

	while (ldlength > 0) {
		flqhead.nh_length = (ldlength > MAXNMSGLEN) ?
				MAXNMSGLEN : ldlength;

		if (nsend(&flqhead)) lampanic("loadd (nsend)");

		ldlength -= flqhead.nh_length;
		flqhead.nh_msg += flqhead.nh_length;
	}
/*
 * Receive a reply that ensures that all data has been fully loaded.
 */
	flrhead.nh_event = -kbgetpid();
	flrhead.nh_type = 0;
	flrhead.nh_flags = 0;
	flrhead.nh_length = 0;
	flrhead.nh_msg = 0;

	if (nrecv(&flrhead)) lampanic("loadd (nrecv)");

	return((void (*)()) lflsync);
}

/*
 *	lflsync
 *
 *	Function:	- receives "all done" message from flatd
 *			- replies to loadd client
 *	Returns:	- next function to execute
 */
static void (*(
lflsync()))()

{
	ksigsetmask(mask);
	lr->lr_reply = flr->flr_reply;
	free(filebuf);
	lsendr();
	return((void (*)()) loadd);
}

/*
 *	lsendr
 *
 *	Function:	- sends a reply back to the client
 *	Returns:	- next function to execute
 */
static void (*(
lsendr()))()

{
	lrhead.nh_node = lq->lq_src_node;
	lrhead.nh_event = lq->lq_src_event;
	lrhead.nh_type = 0;
	lrhead.nh_flags = 0;
	lrhead.nh_length = 0;
	lrhead.nh_msg = 0;

	if (nsend(&lrhead)) lampanic("loadd (nsend)");
/*
 * Receive next request.
 */
	lqhead.nh_event = EVLOADD;
	lqhead.nh_type = 0;
	lqhead.nh_flags = 0;
	lqhead.nh_length = FNAMELEN;
	lqhead.nh_msg = fname;

	if (nrecv(&lqhead)) lampanic("loadd (nrecv)");

	return((void (*)()) loadd);
}
