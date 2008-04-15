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
 *	NJN
 *
 *	$Id: iod.ext.c,v 6.9 2003/10/11 19:15:34 brbarret Exp $
 *
 *	Function:	- external IO daemon
 */

#include <lam_config.h>

#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <typical.h>
#include <terror.h>

/*
 * public functions
 */
void			(*(iod()))();
void			io_accept();
void			io_register();
void			io_deregister();

/*
 * external functions
 */
extern void		io_forward();
extern void		io_new();

/*
 * private variables
 */
static fd_set		allfds;			/* all sockets to listen on */
static int		fd_max;			/* max. socket listening to */
static int		fd_io = -1;		/* socket clients connect on */

/*
 *	io_accept
 *
 *	Function:	- prepare to accept client connections
 *	Accepts:	- IO server file descriptor
 */
void
io_accept(fd)

int			fd;

{
	FD_ZERO(&allfds);
	FD_SET(fd, &allfds);
	fd_io = fd;
	fd_max = fd;
}

/*
 *	io_register
 *
 *	Function:	- register a new client connection
 *			- iod will now read and forward client data 
 *	Accepts:	- client file descriptor
 */
void
io_register(fd)

int			fd;

{
	FD_SET(fd, &allfds);
	if (fd > fd_max) {
		fd_max = fd;
	}
}

/*
 *	io_deregister
 *
 *	Function:	- deregister a client connection
 *			- iod will no longer read and forward client data
 *	Accepts:	- client file descriptor
 */
void
io_deregister(fd)

int			fd;

{
	FD_CLR(fd, &allfds);
}

/*
 *	iod
 *
 *	Function:	- server loop
 */
void (*(
iod()))()

{
	fd_set		readfds;
	int		nready;
	int		fd;

	memcpy((char *) &readfds, (char *) &allfds, sizeof(fd_set));

	while (((nready = select(fd_max + 1, &readfds, (fd_set *) 0,
				(fd_set *) 0, (struct timeval *) 0)) < 0)
		&& (errno == EINTR));

	if (nready < 0) lampanic("iod (select)");

	if (FD_ISSET(fd_io, &readfds)) {
/*
 * New client wants to connect.
 */
		FD_CLR(fd_io, &readfds);
		nready--;

		io_new(fd_io);
	}
/*
 * Forward data for all clients.
 */
	for (fd = 0; nready && fd <= fd_max; fd++) {
		if (FD_ISSET(fd, &readfds)) {
			nready--;
			io_forward(fd);
		}
	}

	return((void (*)()) iod);
}
