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
 *	$Id: kattach.c,v 1.4 2002/10/09 20:57:47 brbarret Exp $
 * 
 *	Function:	- attaches calling process to the kernel
 *	Accepts:	- process scheduling priority
 *	Returns:	- 0 or ERROR
 */
			  
#include <lam_config.h>

#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <kio.h>
#include <kreq.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct kio_t	_kio;			/* Kernel I/O block */

/*
 * external functions
 */
extern int		_cio_kreq();		/* make kernel request */
extern void		_ksig_follow();		/* follow signals */

int
kattach(priority)

int			priority;

{
	struct kreq	req;		/* kernel request */
	struct kreply	reply;		/* kernel reply */
	sigset_t	newset;		/* new signal set */
	sigset_t	oldset;		/* old signal set */

	LAM_ZERO_ME(req);
	LAM_ZERO_ME(reply);
	req.kq_req = KQATTACH;
	req.kq_index = _kio.ki_index;
	req.kq_pid = lam_getpid();
	req.kq_priority = priority;
/*
 * Send the request to the kernel and receive a reply.
 */
	sigemptyset(&newset);
	sigaddset(&newset, LAM_SIGUSR);
	sigprocmask(SIG_BLOCK, &newset, &oldset);

	if (_cio_kreq(&req, &reply)) {
		sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);
		return(LAMERROR);
	}

	if (reply.kr_reply) {
		sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);
		errno = reply.kr_reply;
		return(LAMERROR);
	}

	_kio.ki_index = reply.kr_index;
	_kio.ki_rtf |= RTF_KERNEL;	
	_kio.ki_pid = lam_getpid();	
	_kio.ki_priority = priority;
	sigprocmask(SIG_SETMASK, &oldset, (sigset_t *) 0);

	if (reply.kr_signal) {
		_kio.ki_signal |= reply.kr_signal;
		_ksig_follow();
	}

	return(0);
}
