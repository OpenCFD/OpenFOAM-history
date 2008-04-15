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
 *	$Id: kenyad.ext.c,v 6.3 2002/10/09 20:56:42 brbarret Exp $
 * 
 *	Function:	- external OTB process management server
 */

/*
 *	chldpipe_init
 *
 *	Function:	- sigchld handling initialization
 */
void
chldpipe_init(void)
{
    return;
}

/*
 * stub for kio_intr
 */
void
kio_intr(int fd, void (*func)(), int bclear)
{
    return;
}
