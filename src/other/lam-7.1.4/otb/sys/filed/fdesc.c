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
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: fdesc.c,v 6.5 2002/10/09 20:56:35 brbarret Exp $
 * 
 *	Function:	- helper routines for managing Trollius file descriptors
 *			- Active descriptors are always at the top of the
 *			  list sorted in LRU order.
 *			- Inactive descriptors are always at the bottom of
 *			  the list in no particular order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include "filed.h"
#include <freq.h>
#include <terror.h>
#include <typical.h>

/*
 * global variables
 */
struct fdesc		*fdtop = FDNULL;/* top of descriptor list */
struct fdesc		*fdbot = FDNULL;/* bottom of descriptor list */

/*
 * external functions
 */
extern int4		fmyclose();
extern int4		fmylseek();
extern int4		fmyopen();

/*
 * local variables
 */
static int4		tfd = 0;	/* now serving descriptor number ... */

/*
 * local functions
 */
static void		fdappend();
static void		fddelete();
static void		fdprepend();

/*
 *	fdnew
 *
 *	Function:	- allocates a new file descriptor
 *	Returns:	- ptr to new file descriptor
 */
struct fdesc *
fdnew()

{
	struct fdesc	*newfd;		/* ptr new file descriptor */

	newfd = (struct fdesc *) malloc((unsigned int) sizeof(struct fdesc));
	if (newfd == FDNULL) lampanic("filed (malloc)");
/*
 * Since the descriptor starts out inactive, append it to the end of fdtop.
 */
	newfd->f_tfd = tfd++;
	fdappend(newfd);
	return(newfd);
}

/*
 *	fdcut
 *
 *	Function:	- removes old file descriptor
 *	Accepts:	- ptr to old file descriptor
 */
void
fdcut(old)

struct fdesc		*old;

{
	fddelete(old);
	free(old->f_name);
	free((char *) old);
}

/*
 *	fdfind
 *
 *	Function:	- finds descriptor in list using handle (tfd)
 *			  as the search key
 *	Accepts:	- file handle
 *	Returns:	- ptr to found file descriptor
 */
struct fdesc *
fdfind(tfd)

int4			tfd;

{
	struct fdesc	*curr;			/* search ptr */

	curr = fdtop;

	while ((curr != FDNULL) && (curr->f_tfd != tfd)) {
		curr = curr->f_next;
	}

	return(curr);
}

/*
 *	fdfindname
 *
 *	Function:	- finds descriptor in list using file name
 *			  and open flags as the search keys
 *	Accepts:	- file name
 *			- matching flags
 *	Returns:	- ptr to found file descriptor
 */
struct fdesc *
fdfindname(name, tflags)

char			*name;
int4			tflags;

{
	struct fdesc	*curr;			/* search ptr */
	int4		lock;			/* LAM_O_LOCK bit */

	curr = fdtop;
/*
 * FACTIVE is not considered.
 * LAM_O_LOCK is considered only if the caller asks for it.
 */
	tflags &= ~FACTIVE;
	lock = (tflags & LAM_O_LOCK) | ~LAM_O_LOCK;

	while ((curr != FDNULL) && ((strcmp(name, curr->f_name) != 0) ||
			(((curr->f_tflags & ~FACTIVE) & lock) != tflags))) {
		curr = curr->f_next;
	}

	return(curr);
}

/*
 *	fdactivate
 *
 *	Function:	- activates a file descriptor
 *			- If the file is active (Unix fd is open),
 *			  there is nothing to do.
 *			  Otherwise the file must be reopened and the
 *			  file pointer set to its former location.
 *			  The number of open Unix fds is limited; if the
 *			  filed limit is reached, a previously active file
 *			  must be de-activated (close Unix fd).
 *			  The de-activated fd is chosen on a LRU basis.
 *			- exits on error
 *
 *	Accepts:	- ptr to file descriptor
 */
void
fdactivate(f)

struct fdesc		*f;

{
	int		nactive;	/* # currently active descs */
	struct fdesc	*p;		/* favourite ptr */

	if (f->f_tflags & FACTIVE) {

		if (f != fdtop) {
			fddelete(f);
			fdprepend(f);
		}

		return;
	}
/*
 * Count the number of active files.
 */
	nactive = 0;

	for (p = fdtop; (p != FDNULL) && (p->f_tflags & FACTIVE);
			p = p->f_next) {
		nactive++;
	}
/*
 * Check if we need to de-activate (close) the LRU descriptor to make
 * way for the newly activated file.
 */
	if (nactive >= ACTIVEMAX) {
/*
 * Look backwards thru the list to find the least recently used
 * descriptor that is not locked.
 */
		p = (p == FDNULL) ? fdbot : p->f_prev;

		for (; p->f_tflags & LAM_O_LOCK; p = p->f_prev);

		p->f_tflags &= ~FACTIVE;
		fddelete(p);
		fdappend(p);

		if (fmyclose(p->f_fd)) lampanic("filed (fmyclose)");
	}
/*
 * Open the newly activated file and seek to the current file pointer
 * in order to resume operatons at the right place.
 */
	f->f_fd = fmyopen(f->f_name, f->f_flags, 0);
	if (f->f_fd < 0) lampanic("filed (fmyopen)");

	if (!isatty(f->f_fd)) {
		if (fmylseek(f->f_fd, f->f_ptr, SEEK_SET) < 0) {
			lampanic("filed (fmylseek)");
		}
	}

	f->f_tflags |= FACTIVE;
	fddelete(f);
	fdprepend(f);
}

/*
 *	fdlock
 *
 *	Function:	- locks a descriptor permanently active
 *			- At most, we can tolerate ACTIVEMAX - 1
 *			  locked descriptors.  One slot must be
 *			  left open so the cache concept doesn't seize up.
 *	Accepts:	- ptr to file descriptor
 *	Returns:	- 0 or ERROR
 */
int
fdlock(f)

struct fdesc		*f;

{
	int		nlock;		/* # currently locked descs */
	struct fdesc	*p;		/* favourite ptr */

	if (f->f_tflags & FLOCK) {
		return(0);
	}

	fdactivate(f);
	nlock = 0;

	for (p = fdtop; p != FDNULL; p = p->f_next) {

		if (p->f_tflags & FLOCK) {
			++nlock;
		}
	}

	if (nlock >= (ACTIVEMAX - 1)) {
		errno = ENFILE;
		return(ERROR);
	}

	f->f_tflags |= FLOCK;
	return(0);
}

/*
 *	fddelete
 *
 *	Function:	- deletes existing descriptor from list
 *	Accepts:	- ptr to existing descriptor
 */
static void
fddelete(old)

struct fdesc		*old;

{
	if (old == fdtop) {
		fdtop = old->f_next;
	} else {
		old->f_prev->f_next = old->f_next;
	}

	if (old == fdbot) {
		fdbot = old->f_prev;
	} else {
		old->f_next->f_prev = old->f_prev;
	}
}

/*
 *	fdappend
 *
 *	Function:	- appends a descriptor to the end of the list
 *	Accepts:	- new descriptor
 */
static void
fdappend(newfd)

struct fdesc		*newfd;

{
	if (fdbot == FDNULL) {
		fdtop = newfd;
		newfd->f_prev = FDNULL;
	} else {
		fdbot->f_next = newfd;
		newfd->f_prev = fdbot;
	}

	newfd->f_next = FDNULL;
	fdbot = newfd;
}

/*
 *	fdprepend
 *
 *	Function:	- prepends a descriptor to the front of the list
 *	Accepts:	- new descriptor
 */
static void
fdprepend(newfd)

struct fdesc		*newfd;

{
	if (fdtop == FDNULL) {
		fdbot = newfd;
		newfd->f_next = FDNULL;
	} else {
		fdtop->f_prev = newfd;
		newfd->f_next = fdtop;
	}

	newfd->f_prev = FDNULL;
	fdtop = newfd;
}
