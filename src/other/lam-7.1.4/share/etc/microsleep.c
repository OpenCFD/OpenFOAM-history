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
 *
 * $Id: microsleep.c,v 6.9 2003/10/11 19:15:35 brbarret Exp $
 * 
 *	Function:	- higher precision sleep (microseconds)
 *			- uses select() timeout value
 *			- does not restart if interrupted
 */

#include <lam_config.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <laminternal.h>


int
microsleep(unsigned int	usec)
{
	struct timeval	timeout;

	timeout.tv_sec = usec / 1000000;
	timeout.tv_usec = usec % 1000000;

	while ((select(0, (fd_set *) 0, (fd_set *) 0,
			(fd_set *) 0, &timeout) < 0) && (errno == EINTR));

	return(0);
}
