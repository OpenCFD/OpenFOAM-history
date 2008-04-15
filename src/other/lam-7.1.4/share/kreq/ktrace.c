/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ktrace.c,v 6.9 2004/02/04 03:06:21 jsquyres Exp $
 * 
 *	Function:	- runtime trace package
 *			- an internal package, hence does not check or
 *			  return many error conditions
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <kio.h>
#include <ktrace.h>
#include <net.h>
#include <mpitrace.h>
#include <portable.h>
#include <trreq.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct kio_t	_kio;			/* Kernel Input/Output block */

/*
 * local functions
 */
static void		tralloc(int);		/* allocate trace buffers */

/*
 * global functions
 */
void			lam_trexit();		/* trace cleanup */
void			lam_trinit();		/* initialize traces */
void			lam_trunload(int);	/* unload traces to traced */

/*
 *	lam_trinit
 *
 *	Function:	- initializes trace system
 */
void
lam_trinit()
{
	int		i;			/* favourite index */
/*
 * Initialize trace buffer header array.
 * Its size limits the total size of trace data per process
 * since each individual trace buffer is MAXNMSGLEN long.
 */
	_kio.ki_tid = lam_getpid();
	_kio.ki_trcurr = 0;
	_kio.ki_trhead = (struct trhead *) malloc((unsigned)
		(TRHMAX * sizeof(struct trhead)));

	if (! _kio.ki_trhead) {
		kexit(errno);
	}

	for (i = 0; i < TRHMAX; ++i) {
		_kio.ki_trhead[i].trh_length = 0;
		_kio.ki_trhead[i].trh_buf = 0;
	}
/*
 * Vector SIGTRACE to unload trace data to the local daemon.
 * Register a cleanup function.
 */
	ksignal(SIGTRACE, lam_trunload);
	atkexit(lam_trexit, (char *) 0);
/*
 * Allocate the first few trace buffers.
 */
	tralloc(0);
}

/*
 *	tralloc
 *
 *	Functions:	- allocates memory for the next few trace buffers
 *			- next TRHQUAN buffers are allocated
 *			- assumes TRHQUAN evenly divides TRHMAX
 *	Accepts:	- index of first trace buffer ptr
 */
static void
tralloc(int idx)
{
	int		i;
	int		imax;
	char		*p;

	p = malloc((unsigned) (MAXNMSGLEN * TRHQUAN));
	if (! p) kexit(errno);

	imax = idx + TRHQUAN;

	for (i = idx; i < imax; ++i) {
		_kio.ki_trhead[i].trh_buf = p;
		p += MAXNMSGLEN;
	}
}

/*
 *	lam_trunload
 *
 *	Function:	- unloads used trace buffers to local trace daemon
 */
void
lam_trunload(int sig)
{
	int		i;

	for (i = 0; i < TRHMAX; ++i) {

		if (_kio.ki_trhead[i].trh_length > 0) {

			if (lam_rtrstore(LOCAL, TRRUNTIME, _kio.ki_tid,
					_kio.ki_trhead[i].trh_buf,
					_kio.ki_trhead[i].trh_length) < 0) {
				kexit(errno);
			}

			_kio.ki_trhead[i].trh_length = 0;
		}
	}

	_kio.ki_trcurr = 0;
}

/*
 *	lam_trpoint
 *
 *	Function:	- determines where the next trace can be stored
 *			- calls tralloc, if necessary
 *			- may permanently turn off tracing if all buffers fill
 *	Accepts:	- size of next trace
 *	Returns:	- ptr to next trace location
 */
char *
lam_trpoint(int size)
{
	struct trhead	*p;

	p = _kio.ki_trhead + _kio.ki_trcurr;
/*
 * If the current trace buffer is filled, move to the next one.
 */
	if ((p->trh_length + size) > MAXNMSGLEN) {
/*
 * Flush the trace buffers if we've used up the last one.
 */
		if (_kio.ki_trcurr == (TRHMAX - 1)) {
			lam_trunload(0);
			p = _kio.ki_trhead + _kio.ki_trcurr;
		} else {
			_kio.ki_trcurr += 1;
			p++;
/*
 * If the next trace buffer is unallocated, allocate it plus a few more.
 */
			if (p->trh_buf == 0) {
				tralloc(_kio.ki_trcurr);
			}
		}
	}
/*
 * Increment the trace buffer length before passing the address to be stored.
 */
	p->trh_length += size;
	return(p->trh_buf + (p->trh_length - size));
}

/*
 *	lam_trexit
 *
 *	Function:	- cleanup trace system prior to process exit
 */
void
lam_trexit()
{
	int		i;
/*
 * Unload all traces to the local daemon.
 */
	lam_trunload(0);
/*
 * Free all trace buffers.
 */
	for (i = 0; i < TRHMAX; i += TRHQUAN) {

		if (_kio.ki_trhead[i].trh_buf != 0) {
			free(_kio.ki_trhead[i].trh_buf);
		}
	}
/*
 * Free the trace header array.
 */
	free((char *) _kio.ki_trhead);
}
