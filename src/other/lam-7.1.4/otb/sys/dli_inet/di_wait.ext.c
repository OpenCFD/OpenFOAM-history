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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: di_wait.ext.c,v 6.8 2003/10/11 19:15:33 brbarret Exp $
 *
 *	Function:	- block waiting for network frame message
 *	Accepts:	- socket descriptor
 */

#include <lam_config.h>

#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <kreq.h>
#include <terror.h>

/*
 * external functions 
 */
extern void		di_forward();

void
di_wait(sockd)

int			sockd;

{
	fd_set		readmask;		/* select read-mask */
/*
 * Block for datalink input.
 */
	FD_ZERO(&readmask);
	FD_SET(sockd, &readmask);

	if (select(sockd + 1, &readmask, (fd_set *) 0, (fd_set *) 0,
			(struct timeval *) 0) < 0) {
		terror("dli_inet (select)");
		kexit(errno);
	}

	di_forward();
}
