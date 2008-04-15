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
 *	JRV
 *
 *	$Id: iomap.c,v 6.4 2002/10/09 21:00:53 brbarret Exp $
 * 
 *      Function:	- mux the basic I/O calls to either Unix or Trollius
 *	Accepts:	- a flag, then same as Unix/POSIX calls, in all cases
 *	Returns:	- same as the Unix/POSIX calls, in all cases
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <laminternal.h>
#include <freq.h>
#include <portable.h>
#include <tstdio.h>
#include <kio.h>

/*
 * global variables
 */
extern struct kio_t	_kio;

/*
 *	lam_iomap_lseek
 *
 *	Function:	- maps to lseek or rflseek, based on file flags.
 *	Accepts:	- file flag
 *			- same as Unix/POSIX
 *	Returns:	- same as Unix/POSIX
 */
off_t
lam_iomap_lseek(flag, fd, offset, whence)

unsigned int		flag;
int			fd;
off_t			offset;
int			whence;

{
	if (flag & _TIOREMOTE) {
		return((off_t) lam_rflseek(fd, (int4) offset, whence));
	}
	else {
		return(lseek(fd, offset, whence));
	}
}

/*
 *	lam_iomap_open
 *
 *	Function:	- maps to open or rfopen, based on file flags
 *	Accepts:	- file flag
 *			- same as Unix/POSIX
 *	Returns:	- same as Unix/POSIX
 */
int
lam_iomap_open(flag, fname, iomode, perm)

unsigned int		flag;
char			*fname;
int			iomode;
int			perm;

{
	if (flag & _TIOREMOTE) {
		return((int) lam_rfopen(fname, iomode, perm));
	}
	else {
		return(open(fname, iomode, perm));
	}
}

/*
 *	lam_iomap_close
 *
 *	Function:	- maps to close or rfclose, based on file flags
 *	Accepts:	- file flag
 *			- same as Unix/POSIX
 *	Returns:	- same as Unix/POSIX
 */
int
lam_iomap_close(flag, fd)

unsigned int		flag;
int			fd;

{
	if (flag & _TIOREMOTE) {
		return((int) lam_rfclose(fd));
	}
	else {
		return(close(fd));
	}
}

/*
 *	lam_iomap_read
 *
 *	Function:	- maps to read or rfread, based on file flags
 *	Accepts:	- file flag
 *			- same as Unix/POSIX
 *	Returns:	- same as Unix/POSIX
 */
int
lam_iomap_read(flag, fd, buf, nbyte)

unsigned int		flag;
int			fd;
char			*buf;
int			nbyte;

{
	if (flag & _TIOREMOTE) {
		return((int) lam_rfread(fd, buf, nbyte));
	}
	else {
		return(read(fd, buf, nbyte));
	}
}

/*
 *	lam_iomap_write
 *
 *	Function:	- maps to write or rfwrite, based on file flags
 *	Accepts:	- file flag
 *			- same as Unix/POSIX
 *	Returns:	- same as Unix/POSIX
 */
int
lam_iomap_write(flag, fd, buf, nbyte)

unsigned int		flag;
int			fd;
CONST char		*buf;
int			nbyte;

{
	if (flag & _TIOREMOTE) {
		return((int) lam_rfwrite(fd, buf, nbyte));
	}
	else {
		return(write(fd, buf, nbyte));
	}
}
