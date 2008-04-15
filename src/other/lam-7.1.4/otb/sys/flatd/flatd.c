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
 * $Id: flatd.c,v 6.10 2003/05/21 19:16:14 jsquyres Exp $
 * 
 *	Function:	- OTB flat memory server
 *			- provides symbolic access to memory
 *			- "flattens" memory in a distributed system in
 *			  order to support broadcasting operations
 *			- memory blocks emulated as temporary files
 */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>

#include <events.h>
#include <flatreq.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <lamdebug.h>
#include <laminternal.h>
#include <etc_misc.h>

/*
 * global functions
 */
void			(*(fl_init()))();
void			(*(flatd()))();
void flwipe(void);
char *flforget(int4 tag);

/*
 * local functions
 */
static void		flqload(void);
static void		flqget(void);
static void		flqclean(void);

/*
 * static variables
 */
static int		flcount;	/* total # of created files */

static struct flreq	*request;	/* request from client */
static struct flreply	*reply;		/* reply to client */

static struct nmsg	nhq;		/* request desc. from client */
static struct nmsg	nhr;		/* reply desc. to client */

static char		buf[MAXNMSGLEN];/* input data buffer for request */
static struct flregion	fltable[FLMAX];	/* flat address table */

/*
 *	fl_init
 *
 *	Function:	- flatd initialization
 */
void (*(
fl_init()))()
{
	int		i;			/* index */

	flcount = 0;
	request = (struct flreq *) nhq.nh_data;
	reply = (struct flreply *) nhr.nh_data;

	nhr.nh_flags = NREEL;
	nhr.nh_type = 0;

	_lam_atexit(flwipe);
/*
 * Initialize table.
 */
	for (i = 0; i < FLMAX; ++i) {
		fltable[i].fl_addr = EMPTY;
	}
/*
 * Attach to kernel.
 */
	if (lpattach("flatd")) lampanic("flatd (lpattach)");
/*
 * Receive first request.
 */
	nhq.nh_event = EVFLATD;
	nhq.nh_type = 0;
	nhq.nh_flags = 0;
	nhq.nh_length = MAXNMSGLEN;
	nhq.nh_msg = buf;

	if (nrecv(&nhq)) lampanic("flatd (nrecv)");

	return((void (*)()) flatd);
}

/*
 *	flatd
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(flatd()))()
{
	nhr.nh_node = request->flq_src_node;
	nhr.nh_event = request->flq_src_event;

	switch(request->flq_req) {

		case FLQLOAD:
		flqload();
		break;

		case FLQGET:
		case FLQFORGET:
		flqget();
		break;

		case FLQCLEAN:
		flqclean();
		break;
	}

	nhq.nh_event = EVFLATD;
	nhq.nh_type = 0;
	nhq.nh_flags = 0;
	nhq.nh_length = MAXNMSGLEN;
	nhq.nh_msg = buf;

	if (nrecv(&nhq)) lampanic("flatd (nrecv)");

	return((void (*)()) flatd);
}

/*
 *	flqload
 *
 *	Function:	- loads a file and mark it with a tag
 *			- creates file in temporary directory
 */
static void
flqload(void)
{
	int		fd;			/* load file descriptor */
	int		i;			/* favourite index */
	char		*buffer;		/* formatting buffer */
	
	nhr.nh_msg = (char *) 0;
	nhr.nh_length = 0;
/*
 * Search for an existing request.
 */
	for (i = 0; (i < FLMAX); ++i) {

		if ((fltable[i].fl_addr != EMPTY) &&
			(fltable[i].fl_tag == request->flq_tag) &&
			(fltable[i].fl_src_node == request->flq_src_node) &&
			(fltable[i].fl_src_event == request->flq_src_event)) {

			break;
		}
	}
/*
 * If it is a new request, we must create an entry and send an ACK.
 */
	if (i >= FLMAX) {

		for (i = 0; (i < FLMAX) && (fltable[i].fl_addr != EMPTY); ++i);

		if (i >= FLMAX) {
			reply->flr_reply = ENOFLDESCRIPTORS;
			if (nsend(&nhr)) lampanic("flatd (nsend)");
			return;
		}
/*
 * Make sure we can open the file before the user starts pumping data our way.
 */
		fltable[i].fl_tag = request->flq_tag;
		fltable[i].fl_src_node = request->flq_src_node;
		fltable[i].fl_src_event = request->flq_src_event;
		fltable[i].fl_byteswritten = 0;
		fltable[i].fl_status = 0;
/*
 * Create the temporary filename.
 */
		fltable[i].fl_addr = malloc(MAXPATHLEN);

		if (fltable[i].fl_addr == 0) {
			fltable[i].fl_addr = EMPTY;
			reply->flr_reply = errno;
			if (nsend(&nhr)) lampanic("flatd (nsend)");
			return;
		}

		if ((buffer = lam_get_tmpdir()) == 0) {
			free(fltable[i].fl_addr);
			fltable[i].fl_addr = EMPTY;
			reply->flr_reply = errno;
			if (nsend(&nhr)) lampanic("flatd (nsend)");
			return;
		}

		snprintf(fltable[i].fl_addr, MAXPATHLEN - 1, 
			 "%s/lam-flatd%d", buffer, flcount++);
		free(buffer);

		fd = open(fltable[i].fl_addr,
			  O_WRONLY | O_CREAT | O_APPEND, 0700);

		/* If the LAM session directory
		   (/tmp/lam-user@hostname) is on a networked
		   filesystem, O_APPEND may fail (e.g., Linux/NFS --
		   see open(2) on linux and search for "NFS").  So if
		   we failed and got errno == EIO, give it another
		   college try without the O_APPEND.  Be sure to
		   unlink it first, because Linux/NFS will create a
		   [broken] file entry in the directory, even if the
		   open failed. <sigh> */

		if (fd < 0 && errno == EIO) {
		  unlink(fltable[i].fl_addr);
		  fd = open(fltable[i].fl_addr, O_WRONLY | O_CREAT, 0700);
		}

		if (fd < 0) {
			reply->flr_reply = errno;
                        lamlog("flatd: flqload - failed to create file %s",
			       fltable[i].fl_addr);
			lamlog("flatd: flqload - errno %d", errno);

			/* On some stupid systems (e.g., Linux/NFS) a
                           file may be created even though the open
                           failed.  So be sure to unlink it (and
                           ignore if unlink fails).  <sigh> */

			unlink(fltable[i].fl_addr);
			fltable[i].fl_addr = EMPTY;
		} else {
			reply->flr_reply = 0;
			close(fd);
			lamlog("flatd: flqload - successfully created file %s",
			       fltable[i].fl_addr);
			lamlog("flatd: flqload - file descriptor %d", fd);
		}

		if (nsend(&nhr)) lampanic("flatd (nsend)");
		return;
	}
/*
 * If we get this far, then it was an existing request which must be continued.
 */
	if (fltable[i].fl_status == 0) {
		fd = open(fltable[i].fl_addr, O_WRONLY | O_APPEND, 0700);

		if (fd < 0) {
			reply->flr_reply = errno;
			fltable[i].fl_status = errno;
		}
/*
 * Write this chunk of data.
 */
		else if (write(fd, buf, nhq.nh_length) < nhq.nh_length) {
			fltable[i].fl_status = errno;
			close(fd);
			lamlog("flatd: flqload - calling unlink on \"%s\"", 
			       fltable[i].fl_addr);
			unlink(fltable[i].fl_addr);
		}
/*
 * Close even if this isn't the end of the file to conserve descriptors.
 */
		lamlog("flatd: flqload - successfully appended %d bytes to %s",
		       nhq.nh_length, fltable[i].fl_addr);
		close(fd);
	}
		
	fltable[i].fl_byteswritten += nhq.nh_length;

	if (fltable[i].fl_byteswritten == request->flq_ldlength) {
		reply->flr_reply = fltable[i].fl_status;
		if (nsend(&nhr)) lampanic("flatd (nsend)");
	}
}

/*
 *	flqget
 *
 *	Function:	- finds the address associated with a given tag
 *			- FLQFORGET also cleans out the fltable entry
 *			  associated with the tag
 */
static void
flqget(void)
{
	int		i;		/* favourite index */

	for (i = 0; (i < FLMAX) && ((fltable[i].fl_tag != request->flq_tag)
		|| (fltable[i].fl_addr == EMPTY)); ++i);

	if (i >= FLMAX) {
		reply->flr_reply = EBADTAG;
	} else {
		reply->flr_reply = 0;
		nhr.nh_msg = fltable[i].fl_addr;
		nhr.nh_length = FLPATHLEN;
	}

	if (nsend(&nhr)) lampanic("flatd (nsend)");

	if (request->flq_req == FLQFORGET) {
		free(fltable[i].fl_addr);
		fltable[i].fl_addr = EMPTY;
	}
}

/*
 *	flqclean
 *
 *	Function:	- removes tag and associated file
 */
static void
flqclean(void)
{
	int		i;

	for (i = 0; (i < FLMAX) && ((fltable[i].fl_tag != request->flq_tag)
			|| (fltable[i].fl_addr == EMPTY)); ++i);

	if (i >= FLMAX) {
		reply->flr_reply = EBADTAG;
	} else {
		reply->flr_reply = 0;
		lamlog("flatd: flqclean - calling unlink on \"%s\"", 
		       fltable[i].fl_addr);
		unlink(fltable[i].fl_addr);
		fltable[i].fl_addr = EMPTY;
	}

	if (nsend(&nhr)) lampanic("flatd (nsend)");
}

/*
 *	flforget
 *
 *	Function:	- internal process access to FLQFORGET service
 *	Accepts:	- flat tag
 *	Returns:	- local address
 */
char *
flforget(int4 tag)
{
	int		i;
	char		*temp;

	for (i = 0; (i < FLMAX) && ((fltable[i].fl_tag != tag)
			|| (fltable[i].fl_addr == EMPTY)); ++i);

	if (i >= FLMAX) {
		errno = EBADTAG;
		return(0);
	} else {
		temp = fltable[i].fl_addr;
		fltable[i].fl_addr = EMPTY;
		return(temp);
	}
}

/*
 *	flwipe
 *
 *	Function:	- clean up everything
 *			- should become a request someday
 */
void
flwipe(void)
{
	int		i;

	for (i = 0; i < FLMAX; ++i) {

		if (fltable[i].fl_addr != EMPTY) {
		  lamlog("flatd: flqwipe - calling unlink on \"%s\"", 
			 fltable[i].fl_addr);
		  unlink(fltable[i].fl_addr);
		  fltable[i].fl_addr = EMPTY;
		}
	}
}
