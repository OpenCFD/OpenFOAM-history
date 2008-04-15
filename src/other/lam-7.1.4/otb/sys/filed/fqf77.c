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
 *	$Log: fqf77.c,v $
 *	Revision 6.5  2003/09/27 05:16:16  brbarret
 *	* Removed the already deprecated panic() function in favor of the already
 *	  existing lampanic().  Some versions of OS X include a panic() in a
 *	  header file that gets pulled in with some of the standard headers
 *
 *	Revision 6.4  2002/10/09 20:56:35  brbarret
 *	* Update tree to match the latest, greatest, license for LAM/MPI.  And this
 *	  one is even done properly :)
 *	
 *	Revision 6.3  1999/08/27 21:51:55  jsquyres
 *	Fixes to make -Wall warnings go away
 *	
 *	Revision 6.2  1999/06/12 17:11:29  kmeyer1
 *	added copyright
 *	
 *	Revision 6.1  1996/11/23 18:41:18  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:30:56  gdburns
 * Ohio Release
 * 
 * Revision 5.2.1.1  95/11/20  13:30:04  gdburns
 * Do not use NULL.
 * 
 * Revision 5.2  94/08/22  13:47:52  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  10:41:53  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:26:29  gdburns
 * Ohio Release
 * 
 *	Function:	- F77 specific requests
 *			- works on files assumed to be in an F77 record format
 *			- generally does not detect bad record format, but
 *			  will try not to die solely due to bad record format
 *			- much more efficient to do these operations here
 *			  than on a remote client
 *			- does not handle direct access files, sequential
 *			  access only
 *			- unformatted files end records with newline ('\n')
 *			- formatted files head and trail record with length
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "filed.h"
#include <freq.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

#define NCARD		(80)

/*
 * external variables
 */
extern char		fbuf[];		/* input data from read */

/*
 * external functions
 */
struct fdesc		*fdfind();	/* find a filed descriptor by tfd */
void			fdactivate();	/* ensure fd is open */
void			fsendr();	/* general purpose file reply */
int4			fmyread();	/* read() */
int4			fmylseek();	/* lseek() */

/*
 *	fqf77back
 *
 *	Function:	- moves file ptr to start of previous record
 *			- record format based on file format, either
 *			  "formatted" or "unformatted"
 *			- assumes we have a sequential F77 file
 *	Accepts:	- ptr to filed request
 */
void
fqf77back(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to target file descriptor */
	int4		nback;		/* # bytes to jump back */
	int4		nscan;		/* # bytes scanned */
	int4		ret;		/* return value of f77back */
	char		*p;		/* scanning byte ptr */
	int		nfound;		/* # newlines found */
	int		err = 0;	/* error code for f77back */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
			errno, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}

	f->f_src_node = fq->fq_src_node;
	f->f_src_event = fq->fq_src_event;
	fdactivate(f);
/*
 * If file is formatted then seek backwards for two newlines ('\n').
 */
	if (fq->fq_fmt == FMT) {
		nfound = 0;
		err = 0;
		ret = 0;
/*
 * Continue until we find two newlines or we hit the start of the file.
 */
		while ((nfound < 2) && (f->f_ptr > 0)) {

			nback = (f->f_ptr < NCARD) ? f->f_ptr : NCARD;
			nscan = 0;
/*
 * Back up a reasonable distance.
 */
			if ((f->f_ptr = fmylseek(f->f_fd, -nback,
					SEEK_CUR)) < 0) {
				lampanic("filed (fmylseek)");
			}
/*
 * Read that amount into our multi-purpose buffer.
 */
			if (fmyread(f->f_fd, fbuf, nback) < nback) {
				lampanic("filed (fmyread)");
			}

			f->f_ptr += nback;
/*
 * Scan it backwards for newlines.
 */
			p = fbuf + nback;

			while ((nback > 0) && (nfound < 2)) {
				p--; nback--; nscan++;

				if (*p == '\n') {
					nfound++;
				}
			}
/*
 * Bump forward past the second newline, if found.
 */
			if (nfound >= 2) {
				nback++;
				nscan--;
			}
/*
 * Back up past the scanned portion.
 */
			if ((f->f_ptr = fmylseek(f->f_fd, -nscan, SEEK_CUR))
					< 0) {
				lampanic("filed (fmylseek)");
			}
		}
	}
/*
 * Otherwise, read the length of the previous record and then
 * seek backwards that amount.
 */
	else if ((f->f_ptr > 0) && (f->f_ptr < 4)) {
		err = EBADREC;
		ret = ERROR;
	}
/*
 * We're already at the head of the file - essentially a NO-OP.
 */
	else if (f->f_ptr == 0) {
		err = 0;
		ret = 0;
	}
/*
 * It's an unformatted file.
 */
	else {
/*
 * Back up in position to read the trailing byte count of the last record.
 */
		if ((f->f_ptr = fmylseek(f->f_fd, -4, SEEK_CUR)) < 0) {
			lampanic("filed (fmylseek)");
		}
/*
 * Read the byte count of the last record.
 */
		if (fmyread(f->f_fd, (char *) &nback, 4) < 4) {
			lampanic("filed (fmyread)");
		}

		f->f_ptr += 4;
		nback += 2 * 4;
/*
 * Back up to the beginning of the last record.
 */
		if ((ret = fmylseek(f->f_fd, -nback, SEEK_CUR)) < 0) {
			lampanic("filed (fmylseek)");
		}

		if (ret < 0) {

			if (errno == EINVAL) {
				err = EBADREC;
			} else {
				lampanic("filed (fmylseek)");
			}
		} else {
			err = 0;
			f->f_ptr = ret;
			ret = 0;
		}
	}

	fsendr(fq->fq_src_node, fq->fq_src_event, err, ret,
			INT4_NIL, INT4_NIL);
}

/*
 *	fqf77read
 *
 *	Function:	- reads a record from a F77 file
 *			- reads up to a newline if file is formatted
 *			- reads the length and then the body of the record
 *			  if file is unformatted
 *			- assumes we have a sequential F77 file
 *	Accepts:	- ptr to filed request
 */
void
fqf77read(fq)

struct freq		*fq;

{
	struct fdesc	*f;		/* ptr to old target descriptor */
	int4		nread;		/* #bytes actually read */
	int4		nret;		/* #bytes replied to client */
	int4		nrec;		/* #bytes in full record */
	int4		npkt;		/* #bytes in one reply packet */
	int4		r;		/* result of mylseek */
	int4		n;		/* countdown of nret */
	char		*prec;		/* maximum record ptr */
	char		*p;		/* moving buffer ptr */
	char		*phole;		/* safe place to read & trash bytes */
	int		bfound;		/* newline found? */

	f = fdfind(fq->fq_tfd);

	if (f == FDNULL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}

	if (fq->fq_length <= INT4_NIL) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				EINVAL, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}

	f->f_src_node = fq->fq_src_node;
	f->f_src_event = fq->fq_src_event;
	fdactivate(f);
/*
 * Allocate a buffer large enough to hold the maximum record
 * size indicated by the client.
 */
	if (fq->fq_length < MAXNMSGLEN) {
		prec = fbuf;
	} else {
		prec = malloc((unsigned) fq->fq_length);
		if (prec == 0) lampanic("filed (malloc)");
	}

	nread = fmyread(f->f_fd, prec, fq->fq_length);

	if (nread < 0) {
		fsendr(fq->fq_src_node, fq->fq_src_event,
				errno, (int4) ERROR, INT4_NIL, INT4_NIL);
		return;
	}

	f->f_ptr += nread;
/*
 * If file is formatted then read up to next newline ('\n').
 */
	if (fq->fq_fmt == FMT) {
		bfound = FALSE;
		nrec = 0;
		nret = 0;
		p = prec;
/*
 * Scan for a newline.
 */
		while ((nread > 0) && !bfound) {
			nrec++;
			nret++;
			nread--;
			bfound = (*p == '\n');
			p++;
		}
/*
 * Usually, read less than the maximum record and will
 * have to backup over the unused portion.
 */
		if (nread > 0) {
			f->f_ptr = fmylseek(f->f_fd, -nread, SEEK_CUR);

			if (f->f_ptr < 0) {
				lampanic("filed (fmylseek)");
			}
/*
 * But if the maximum record is scanned without a newline,
 * move forward to the next record and determine the
 * size of the full current record.
 */
		} else if (!bfound) {
			phole = malloc((unsigned) NCARD);
			if (phole == 0) lampanic("filed (malloc)");
/*
 * Read reasonable sized chunks from the file and continue scanning
 * for the newline until it is found or the end of the file is reached.
 */
			do {
				nread = fmyread(f->f_fd, phole, NCARD);

				if (nread < 0) {
					lampanic("filed (fmyread)");
				} else if (nread == 0) {
					bfound = TRUE;
				} else {
/*
 * Scan current chunk for a newline.
 */
					p = phole;

					while ((nread > 0) && !bfound) {
						nrec++;
						nread--;
						bfound = (*p == '\n');
						p++;
					}
/*
 * Backup over unused portion of current chunk.
 */
					if (nread > 0) {
						f->f_ptr = fmylseek(f->f_fd,
							-nread, SEEK_CUR);

						if (f->f_ptr < 0) {
						    lampanic("filed (fmylseek)");
						}
					}
				}
			} while (!bfound);

			free(phole);
		}
	}
/*
 * The file is unformatted and length is found at the head and
 * tail of the record.
 */
	else {
		if (nread < 4) {
			fsendr(fq->fq_src_node, fq->fq_src_event, EBADREC,
					(int4) ERROR, INT4_NIL, INT4_NIL);
			return;
		}
/*
 * Total length of record accounts for head and tail record lengths.
 */
		nrec = *((int4 *) prec) + (2 * 4);
		nret = (nrec > nread) ? nread : nrec;
/*
 * Move file ptr to the end of the record, forward or backward.
 */
		r = fmylseek(f->f_fd, nrec - nread, SEEK_CUR);

		if (r < INT4_NIL) {

			if (errno == EINVAL) {
				fsendr(fq->fq_src_node, fq->fq_src_event,
						EBADREC, (int4) ERROR,
						INT4_NIL, INT4_NIL);
				return;
			} else {
				lampanic("filed (fmylseek)");
			}
		} else {
			f->f_ptr = r;
		}
	}

	n = nret;
	p = prec;

	do {
		npkt = (n > MAXNMSGLEN) ? MAXNMSGLEN : n;
		memcpy(fbuf, p, npkt);
		fsendr(fq->fq_src_node, fq->fq_src_event,
				0, nret, npkt, f->f_tflags & LAM_O_DMSG);
		n -= npkt;
		p += npkt;
	} while (n > 0);

	if (prec != fbuf) {
		free(prec);
	}
}
