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
 *	Copyright 1996 The Ohio State University
 *	NJN
 *
 *	$Id: iod.int.c,v 6.6 2002/10/09 20:56:47 brbarret Exp $
 *
 *	Function:	- internal IO daemon
 */

#include <typical.h>

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
extern void		kio_intr();
extern void		kio_intr_clr();
extern void		io_forward();
extern void		io_new();

/*
 *	iod
 *
 *	Function:	- server loop
 */
void (*(
iod()))()

{
	return((void (*)()) iod);
}

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
	kio_intr(fd, io_new, FALSE);
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
	kio_intr(fd, io_forward, FALSE);
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
	kio_intr_clr(fd);
}
