/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 *	Function:	- simple filed requests
 *	Accepts:	- filed request
 */

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>

/*
 * external variables
 */
extern char		fbuf[];		/* multi-purpose I/O buffer */
extern struct fdesc	*fdtop;		/* top of descriptor list */

/*
 * global functions
 */
void			fqaccess();
void			fqchdir();
void			fqdup();
void			fqfstat();
void			fqftrunc();
void			fqgetwd();
void			fqincr();
void			fqmkdir();
void			fqrmdir();
void			fqrmfd();
void			fqstat();
void			fqtrunc();
void			fqunlink();
void fqrename(struct freq* fq);

/*
 * external functions
 */
extern int4		fmychdir();
extern int4		fmyaccess();
extern int4		fmyrename(CONST char *from, CONST char *to);
extern int4		fmymkdir();
extern int4		fmyrmdir();
extern int4		fmyunlink();
extern int4		fmyclose();
extern int4		fmyfstat();
extern int4		fmystat();
extern int4		fmysystem();
extern int4		fmyftrunc();
extern int4		fmytrunc();
extern char		*fmygetwd();
extern void		fdcut();	/* clean & remove descriptor */
extern void		fsendr();	/* to client */
extern void		fdactivate();	/* make descriptor active */
extern struct fdesc	*fdfind();	/* find descriptor by tfd */
extern struct fdesc	*fdinit();	/* create & initialize desc. */

/*
 *	fqftrunc
 *
 *	Function:	- sets an open file to a specified length
 */
void
fqftrunc(fq)

struct freq		*fq;

{
	int4		ret;		/* result of myftrunc() */
	struct fdesc	*f;		/* target descriptor */
	int4		*plen;		/* ptr desired length */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				EBADF, (int4) LAMERROR, INT4_NIL, INT4_NIL);
	} else {
		plen = (int4 *) fbuf;
		fdactivate(f);
		ret = fmyftrunc(f->f_fd, (int4) ttol(*plen));
		fsendr(fq->fq_src_node, fq->fq_src_event, (ret == LAMERROR) ?
				errno : 0, ret, INT4_NIL, INT4_NIL);
	}
}

/*
 *	fqtrunc
 *
 *	Function:	- set a named file to a specified length
 */
void
fqtrunc(fq)

struct freq		*fq;

{
	int4		ret;		/* result of fmytrunc() */

	ret = fmytrunc(fbuf, fq->fq_trunclen);
	fsendr(fq->fq_src_node, fq->fq_src_event, (ret == LAMERROR) ?
			errno : 0, ret, INT4_NIL, INT4_NIL);
}

/*
 *	fqfstat
 *
 *	Function:	- get status of a file descriptor
 */
void
fqfstat(fq)

struct freq		*fq;

{
	int4		ret;		/* result of stat() */
	struct fdesc	*f;		/* target descriptor */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				EBADF, (int4) LAMERROR, INT4_NIL, INT4_NIL);
	} else {
		fdactivate(f);
		ret = fmyfstat(f->f_fd, fbuf);
		fsendr(fq->fq_src_node, fq->fq_src_event, (ret == LAMERROR) ?
				errno : 0, ret, sizeof(struct stat), DINT4MSG);
	}
}

/*
 *	fqstat
 *
 *	Function:	- get status of a named file
 */
void
fqstat(fq)

struct freq		*fq;

{
	int4		ret;		/* result of stat() */

	ret = fmystat(fbuf, fbuf);
	fsendr(fq->fq_src_node, fq->fq_src_event, (ret == LAMERROR) ?
			errno : 0, ret, sizeof(struct stat), DINT4MSG);
}

/*
 *	fqunlink
 *
 *	Function:	- removes a file
 */
void
fqunlink(fq)

struct freq		*fq;

{
	int4		ret;		/* result of unlink() */

	ret = fmyunlink(fbuf);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			(ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
			INT4_NIL);
}

/*
 * fqrename
 *
 * Function: - rename a file
 */
void
fqrename(struct freq* fq)
{
  int4 ret;
  struct fqrename data;
  
  memcpy(&data, fbuf, sizeof(struct fqrename));

  ret = fmyrename(data.source, data.dest);
  fsendr(fq->fq_src_node, fq->fq_src_event,
	 (ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
	 INT4_NIL);
}


/*
 *	fqrmdir
 *
 *	Function:	- removes a directory
 */
void
fqrmdir(fq)

struct freq		*fq;

{
	int4		ret;		/* result of rmdir() */

	ret = fmyrmdir(fbuf);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			(ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
			INT4_NIL);
}

/*
 *	fqsystem
 *
 *	Function:	- issues a system command
 */
void
fqsystem(fq)

struct freq		*fq;

{
	int4		ret;		/* result of system() */

	ret = fmysystem(fbuf);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			(ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
			INT4_NIL);
}

/*
 *	fqmkdir
 *
 *	Function:	- makes a new directory
 */
void
fqmkdir(fq)

struct freq		*fq;

{
	int4		ret;		/* result of mkdir() */

	ret = fmymkdir(fbuf, fq->fq_accmode);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			(ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
			INT4_NIL);
}

/*
 *	fqaccess
 *
 *	Function:	- checks accessibility of a file
 */
void
fqaccess(fq)

struct freq		*fq;

{
	int4		ret;		/* result of access() */

	ret = fmyaccess(fbuf, fq->fq_accmode);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			(ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
			INT4_NIL);
}

/*
 *	fqchdir
 *
 *	Function:	- changes working directory
 */
void
fqchdir(fq)

struct freq		*fq;

{
	int4		ret;		/* result of chdir() */

	ret = fmychdir(fbuf);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			(ret == LAMERROR) ? errno : 0, ret, INT4_NIL,
			INT4_NIL);
}

/*
 *	fqgetwd
 *
 *	Function:	- gets working directory
 */
void
fqgetwd(fq)

struct freq		*fq;

{
	fmygetwd(fbuf);
	fsendr(fq->fq_src_node, fq->fq_src_event,
			0, INT4_NIL, (int4) strlen(fbuf) + 1, INT4_NIL);
}

/*
 *	fqrmfd
 *
 *	Function:	- blows away file descriptor
 *			- NOTFD indicates that all descriptors should
 *			  be blown away
 */
void
fqrmfd(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to old target descriptor */
	struct fdesc	*g;		/* ptr to next target descriptor */

	if (fq->fq_tfd == NOTFD) {

		for (f = fdtop; f; f = g) {
/*
 * We do not want to close stdio.
 */
			g = f->f_next;
			if (f->f_tfd > 2) {

				if (f->f_tflags & FACTIVE) {
					fmyclose(f->f_fd);
				}

				fdcut(f);
			}
		}
	}

	else {
		f = fdfind(fq->fq_tfd);

		if (f == FDNULL) {
			fsendr(fq->fq_src_node, fq->fq_src_event, errno,
					(int4) LAMERROR, INT4_NIL, INT4_NIL);
			return;
		}

		else {

			if (f->f_tflags & FACTIVE) {
				fmyclose(f->f_fd);
			}

			fdcut(f);
		}
	}

	fsendr(fq->fq_src_node, fq->fq_src_event,
			0, INT4_NIL, INT4_NIL, INT4_NIL);
}

/*
 *	fqdup
 *
 *	Function:	- duplicates file descriptor
 *			- inherits old descriptors file ptr but
 *			  flow is reset to 0
 *			- new descriptor starts inactive
 */
void
fqdup(fq)

struct freq		*fq;

{
	struct fdesc	*of;		/* ptr to old target descriptor */
	struct fdesc	*nf;		/* ptr to new target descriptor */

	of = fdfind(fq->fq_tfd);

	if (of == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, (int4) LAMERROR, INT4_NIL, INT4_NIL);
		return;
	}

	strcpy(fbuf, of->f_name);
	nf = fdinit(of->f_tflags);
	nf->f_ptr = of->f_ptr;
	nf->f_count = of->f_count;
	nf->f_src_node = of->f_src_node = fq->fq_src_node;
	nf->f_src_event = of->f_src_event = fq->fq_src_event;

	fsendr(fq->fq_src_node, fq->fq_src_event,
			0, nf->f_tfd, INT4_NIL, INT4_NIL);
}

/*
 *	fqincr
 *
 *	Function:	- increments the usage count of a file descriptor
 *			- used by loadgo to set the usage count of
 *			  redirected stdio files to match the number of
 *			  created processes - when all processes die,
 *			  usage count falls to 0 and file is removed
 *			- should be used whenever a tfd is given to another
 *			  process without a reuse open operation
 */
void
fqincr(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target descriptor */
	int4		*incr;		/* increment amount */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, (int4) LAMERROR, INT4_NIL, INT4_NIL);
		return;
	}

	incr = (int4 *) fbuf;
	f->f_count += ttol(*incr);
	fsendr(fq->fq_src_node, fq->fq_src_event, 0, INT4_NIL,
			INT4_NIL, INT4_NIL);
}
