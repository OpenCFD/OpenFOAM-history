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
 *	GDB/RBD
 *
 *	$Id: mrw.c,v 6.7 2002/10/09 20:57:03 brbarret Exp $
 * 
 *	Function:	- atomic reads and writes
 *			- continues through partial transfers and interrupts
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <laminternal.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

/*
 *	mread
 *
 *	Function:	- atomic read()
 *	Returns:	- #bytes read or ERROR
 */
int4
mread(int fd, char *buf, int4 nbytes)
{
	int		nread;		/* # of bytes read */
	int		r;

	nread = 0;

	do {
		r = read(fd, buf, nbytes);

		if (r < 0) {

			if (errno != EINTR) {
				return(ERROR);
			}
		} else if ((r == 0) && (nbytes > 0)) {	/* eof */
			errno = EEOF;
			return((int4) nread);
		} else {
			nread += r;
			buf += r;
			nbytes -= r;
		}
	} while (nbytes > 0);

	return((int4) nread);
}

/*
 *	mreadv
 *
 *	Function:	- atomic readv()
 *	Returns:	- #bytes read or ERROR
 */
int4
mreadv(int fd, struct iovec *iov, int iovcnt)
{
	int		nread;		/* # of bytes read */
	int		r;
	int		savelen = 0;	/* save full length */
	char		*savebase = 0;	/* save original base ptr */

	nread = 0;

	if (iovcnt > 0) {
		savelen = iov->iov_len;
		savebase = iov->iov_base;
	}

	do {
		r = readv(fd, iov, iovcnt);

		if (r < 0) {

			if (errno != EINTR) {
				iov->iov_len = savelen;
				iov->iov_base = savebase;
				return(LAMERROR);
			}
		} else if (r == 0) {	/* eof */
			iov->iov_len = savelen;
			iov->iov_base = savebase;

			while ((iovcnt > 0) && (iov->iov_len == 0)) {
				iov++;
				iovcnt--;
			}

			if (iovcnt > 0) {
				errno = EEOF;
			}

			return((int4) nread);
		} else {
			nread += r;

			while (r > 0) {

				if (r >= iov->iov_len) {
					r -= iov->iov_len;
					iov->iov_len = savelen;
					iov->iov_base = savebase;
					iov++;
					iovcnt--;

					if (iovcnt > 0) {
						savelen = iov->iov_len;
						savebase = iov->iov_base;
					}
				} else {
					iov->iov_len -= r;
					iov->iov_base =
						(char *) iov->iov_base + r;
					r = 0;
				}
			}

			while ((iovcnt > 0) && (iov->iov_len == 0)) {
				iov++;
				iovcnt--;
				if (iovcnt > 0) {
				  savelen = iov->iov_len;
				  savebase = iov->iov_base;
				}
			}
		}
	} while (iovcnt > 0);

	return((int4) nread);
}

/*
 *	mwrite
 *
 *	Function:	- atomic write()
 *	Returns:	- #bytes written or ERROR
 */
int4
mwrite(int fd, CONST char *buf, int nbytes)
{
	int		nwritten;	/* # of bytes written */
	int		r;

	nwritten = 0;

	do {
		r = write(fd, buf, nbytes);

		if (r < 0) {

			if (errno != EINTR) {
				return(ERROR);
			}
		} else if ((r == 0) && (nbytes > 0)) {	/* eof */
			errno = EEOF;
			return((int4) nwritten);
		} else {
			nwritten += r;
			buf += r;
			nbytes -= r;
		}
	} while (nbytes > 0);

	return((int4) nwritten);
}

/*
 *	mwritev
 *
 *	Function:	- atomic writev()
 *	Returns:	- #bytes written or ERROR
 */
int4
mwritev(int fd, struct iovec *iov, int iovcnt)
{
	int		nwritten;	/* # of bytes written */
	int		r;
	int		savelen = 0;	/* save full length */
	char		*savebase = 0;	/* save original base ptr */

	nwritten = 0;

	if (iovcnt > 0) {
		savelen = iov->iov_len;
		savebase = iov->iov_base;
	}

	do {
		r = writev(fd, iov, iovcnt);

		if (r < 0) {

			if (errno != EINTR) {
				iov->iov_len = savelen;
				iov->iov_base = savebase;
				return(LAMERROR);
			}
		} else if (r == 0) {	/* eof */
			iov->iov_len = savelen;
			iov->iov_base = savebase;

			while ((iovcnt > 0) && (iov->iov_len == 0)) {
				iov++;
				iovcnt--;
			}

			if (iovcnt > 0) {
				errno = EEOF;
			}

			return((int4) nwritten);
		} else {
			nwritten += r;

			while (r > 0) {

				if (r >= iov->iov_len) {
					r -= iov->iov_len;
					iov->iov_len = savelen;
					iov->iov_base = savebase;
					iov++;
					iovcnt--;

					if (iovcnt > 0) {
						savelen = iov->iov_len;
						savebase = iov->iov_base;
					}
				} else {
					iov->iov_len -= r;
					iov->iov_base =
						(char *) iov->iov_base + r;
					r = 0;
				}
			}

			while ((iovcnt > 0) && (iov->iov_len == 0)) {
				iov++;
				iovcnt--;
				if (iovcnt > 0) {
				  savelen = iov->iov_len;
				  savebase = iov->iov_base;
				}
			}
		}
	} while (iovcnt > 0);

	return((int4) nwritten);
}
