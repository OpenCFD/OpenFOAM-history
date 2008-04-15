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
 *	$Id: kdetach.c,v 1.3 2002/10/09 20:57:47 brbarret Exp $
 * 
 *	Function:	- detaches calling process from the kernel
 *	Accepts:	- process scheduling priority
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>
#include <etc_misc.h>			  
#include <kio.h>
#include <kreq.h>
#include <terror.h>
#include <typical.h>

#include <unistd.h>

/*
 * external variables
 */
extern struct kio_t	_kio;			/* Kernel I/O block */

/*
 * external functions
 */
extern int		_cio_kreqfront();	/* make kernel request */

int
kdetach(pid)

int			pid;

{
	struct kreq	req;			/* kernel request */

	if (_kio.ki_pid != lam_getpid()) {
		errno = ENOTATTACHED;	
		return(ERROR);
	}

	LAM_ZERO_ME(req);
	req.kq_req = KQDETACH;
	req.kq_index = _kio.ki_index;
	req.kq_pid = (pid == 0) ? lam_getpid() : pid;

	if (_cio_kreqfront(&req) < 0) {
		return(ERROR);
	}

	if ((pid == 0) || (pid == lam_getpid())) {
		_kio.ki_rtf &= ~RTF_KERNEL;	
		_kio.ki_pid = -1;	
		_kio.ki_index = -1;
	}

	return(0);
}
