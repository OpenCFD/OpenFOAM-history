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
 *
 *      Software for Humanity
 *      This program is freely distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	$Id: kenyad.int.c,v 6.3 2002/10/09 20:56:47 brbarret Exp $
 * 
 *	Function:	- internal OTB process management server
 */

#include <fcntl.h>
#include <unistd.h>

#include <terror.h>

/*
 * external variables
 */
extern int	diedfd;
extern int	diesigfd;

/*
 *	chldpipe_init
 *
 *	Function:	- sigchld handling initialization
 */
void
chldpipe_init(void)
{
    int		fds[2];
/*
 * Set up pipe for handling SIGCHLDs. Make it close-on-exec.
 */
    if (pipe(fds))
	lampanic("kenyad (pipe)");

    if (fcntl(fds[0], F_SETFD, 1) == -1)
	lampanic("kenyad (fcntl)");

    if (fcntl(fds[1], F_SETFD, 1) == -1)
	lampanic("kenyad (fcntl)");

    diedfd = fds[0];
    diesigfd = fds[1];
}
