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
 *	Copyright 1994 The Ohio State University
 *	GDB
 *
 *	$Id: kdoom.c,v 6.6 2002/10/09 20:57:47 brbarret Exp $
 *	
 *	Function:	- delivers a signal to a process
 *	Accepts:	- process identifier (0 means self)
 *			- signal number
 *	Returns:	- 0 or ERROR
 */

#include <kio.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>

/*
 * external variables
 */
extern struct kio_t	_kio;			/* kernel I/O block */

/*
 * external functions
 */
extern int		_cipc_kreq();		/* make kernel request */
extern void		_ksig_follow();		/* check signals */

int
kdoom(pid, signal)

int			pid;
int			signal;

{
	struct kreq	req;			/* kernel request */
	struct kreply	reply;			/* kernel reply */

	if (signal < 1) {
		if (kill(pid, -signal)) {
			return(ERROR);
		}
	} else {
/*
 * Check the signal number bounds.
 */
		if (signal > TNSIG) {
			errno = EINVAL;
			return(ERROR);
		}
/*
 * Formulate the KQDOOM kernel request.
 */
		LAM_ZERO_ME(req);
		LAM_ZERO_ME(reply);
		req.kq_req = KQDOOM;
		req.kq_index = _kio.ki_index;
		req.kq_pid = pid;
		req.kq_signal = sigmask(signal);

		if (_cipc_kreq(&req, &reply)) {
			return(ERROR);
		}
/*
 * Check signals.
 */
#if 0
		/* This seems to be redundant: the reply.kr_signal is
                   checked (and potentially followed) in _cipc_kreq().
                   No need to check/follow it again here.  I'm leaving
                   the code and this comment here just to be explicit,
                   and to document where the [LAM] signal is actually
                   processed. */
		if (reply.kr_signal) {
			_kio.ki_signal |= reply.kr_signal;
			_ksig_follow();
		}
#endif

		if (reply.kr_reply) {
			errno = reply.kr_reply;
			return(ERROR);
		}
	}

	return(0);
}
