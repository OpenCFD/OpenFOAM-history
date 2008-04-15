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
 *	$Id: fqopen.c,v 6.3 2002/10/09 20:56:35 brbarret Exp $
 * 
 *	Function:	- FQOPEN request
 *			- exits daemon in case of non-user error
 *			- activates file after opening
 *			- open flags specific to Trollius:
 *			  LAM_O_REUSE - reuse existing descriptor with same
 *			  name
 *			  LAM_O_LOCK  - lock file permanently active
 *			  LAM_O_1WAY  - no reply sent on NOCBX write
 *			  LAM_O_DINT4 - data are integers
 *			  LAM_O_DFLT4 - data are floats
 *			  LAM_O_DFLT8 - data are floats
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <events.h>
#include "filed.h"
#include <freq.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern char		fbuf[];		/* multi purpose buffer */

/*
 * external functions
 */
extern struct fdesc	*fdfindname();	/* find a filed descriptor by name */
extern struct fdesc	*fdnew();	/* allocate a new descriptor */
extern void		fsendr();	/* general purpose file reply */
extern void		fdactivate();	/* make sure tfd is active */
extern void		fdcut();	/* clean and free descriptor */
extern char		*ttyname();
extern int		fdlock();	/* make descriptor permanently active */
extern int4		fmyclose();
extern int4		fmylseek();
extern int4		fmyopen();

/*
 * global functions
 */
void			fqopen();
struct fdesc		*fdinit();

/*
 *	fqopen
 *
 *	Function:	- handles simple client opens
 *	Accepts:	- ptr to filed request
 */
void
fqopen(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target descriptor */
	int4		fd;		/* initial open descriptor */
/*
 * Don't allow "/dev/" files to be opened for "read".
 */
	if ((strncmp(fbuf, "/dev/", 5) == 0) &&
			((fq->fq_flags & LAM_O_RDWR) ||
			((fq->fq_flags & LAM_O_WRONLY) != LAM_O_WRONLY)) ) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				EINVAL, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}
/*
 * Is this a REUSE situation?
 */
	if (fq->fq_flags & LAM_O_REUSE) {
		fq->fq_flags &= ~LAM_O_REUSE;
		f = fdfindname(fbuf, fq->fq_flags);
/*
 * This is the first open.  Remove LAM_O_REUSE and recall do_open().
 */
		if (f == FDNULL) {
			fqopen(fq);
		}
/*
 * This is a subsequent open.  Bump the reuse counter.  Note the client.
 */
		else {
			f->f_count += 1;
			f->f_src_node = fq->fq_src_node;
			f->f_src_event = fq->fq_src_event;
			fsendr(fq->fq_src_node, fq->fq_src_event,
					0, f->f_tfd, INT4_NIL, INT4_NIL);
		}
	}
/*
 * This is a simple open.
 */
	else {
/*
 * Open the file first to detect errors.
 */
		fd = fmyopen(fbuf, fq->fq_flags & ~LAM_O_FILED, fq->fq_mode);

		if (fd != ERROR) {
/*
 * Determine the pointer within the file.
 * Close it, initialize the descriptor and let fdactivate decide active status.
 */
			f = fdinit(fq->fq_flags);
			f->f_ptr = fmylseek(fd, INT4_NIL, SEEK_CUR);
			fmyclose(fd);
			f->f_count = 1;
			f->f_src_node = fq->fq_src_node;
			f->f_src_event = fq->fq_src_event;
			fdactivate(f);
/*
 * Lock the file active if requested.
 */
			if ((fq->fq_flags & LAM_O_LOCK) &&
						(fdlock(f) == ERROR)) {
				fd = ERROR;
			} else {
				errno = 0;
				fd = f->f_tfd;
			}
		}

		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, fd, INT4_NIL, INT4_NIL);
	}
}

/*
 *	fdinit
 *
 *	Function:	- initializes a new file descriptor
 *			- New descriptors cannot be active or locked.
 *			- sets as many fields as possible
 *			- file name comes from I/O buffer
 *	Accepts:	- access flags
 *	Returns:	- ptr to new descriptor
 */
struct fdesc *
fdinit(tflags)

int4			tflags;		/* Trollius and Unix flags */

{
	struct fdesc	*f;		/* ptr to target descriptor */

	f = fdnew();
	f->f_fd = -1;
	f->f_ptr = 0;
	f->f_count = 0;
	f->f_src_node = NOTNODEID;
	f->f_src_event = NOTEVENT;
	f->f_flow = 0;
	f->f_tflags = tflags & ~(LAM_O_LOCK | FACTIVE);
/*
 * Isolate the Unix flags.
 */
	f->f_flags = tflags & ~LAM_O_FILED;
/*
 * The effect of LAM_O_CREAT and LAM_O_TRUNC will be achieved in
 * the test open().  We don't want them when activating files.
 */
	f->f_flags &= ~(LAM_O_TRUNC | LAM_O_CREAT);
/*
 * Copy the filename.
 */
	f->f_name = malloc((unsigned) (strlen(fbuf) + 1));
	if (f->f_name == 0) lampanic("filed (malloc)");
	
	strcpy(f->f_name, fbuf);
	return(f);
}

/*
 *	fdstdio
 *
 *	Function:	- initializes the standard I/O descriptors,
 *			  stdin (0), stdout (1) and stderr (2)
 *			- stdio are locked and active; therefore it
 *			  is assumed that ACTIVEMAX >= 4
 *			- should never be closed
 */
void
fdstdio()

{
	struct fdesc	*f;		/* ptr to new target descriptor */
/*
 * stdin
 */
	strcpy(fbuf, "/dev/null");

	f = fdinit(LAM_O_RDONLY);
	f->f_tflags |= FLOCK | FACTIVE;

	f->f_fd = fmyopen(fbuf, LAM_O_RDONLY, 0);
	if (f->f_fd < 0) lampanic("filed (/dev/null)");

	f->f_src_node = NOTNODEID;
	f->f_src_event = NOTEVENT;
/*
 * stdout
 */
	if (getnodeid() == getorigin()) {

		if (isatty(1)) {
			strcpy(fbuf, ttyname(1));
		} else {
			strcpy(fbuf, "/dev/tty");
		}
	} else {
		strcpy(fbuf, "/dev/null");
	}

	f = fdinit(LAM_O_WRONLY);

	if (getnodeid() == getorigin()) {
		f->f_fd = 1;
	} else {
		f->f_fd = fmyopen(fbuf, LAM_O_WRONLY, 0);
		if (f->f_fd < 0) lampanic("filed (/dev/null)");
	}

	f->f_tflags |= FLOCK | FACTIVE;
	f->f_src_node = NOTNODEID;
	f->f_src_event = NOTEVENT;
/*
 * stderr
 */
	if (getnodeid() == getorigin()) {

		if (isatty(2)) {
			strcpy(fbuf, ttyname(2));
		} else {
			strcpy(fbuf, "/dev/tty");
		}
	} else {
		strcpy(fbuf, "/dev/null");
	}

	f = fdinit(LAM_O_RDWR);

	if (getnodeid() == getorigin()) {
		f->f_fd = 2;
	} else {
		f->f_fd = fmyopen(fbuf, LAM_O_RDWR, 0);
		if (f->f_fd < 0) lampanic("filed (/dev/null)");
	}

	f->f_tflags |= FLOCK | FACTIVE;
	f->f_src_node = NOTNODEID;
	f->f_src_event = NOTEVENT;
}
