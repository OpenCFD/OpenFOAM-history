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
 *	GDB
 *
 *	$Id: nprobe.c,v 6.4 2002/10/09 20:59:50 brbarret Exp $
 * 
 *	Function:	- checks for the existance of messages with
 *			  specific events/types on the local bufferd
 *			- cannot bypass buffers due to lack of probe
 *			  capability in the kernel
 *			- receives information on whether such a 
 *			  message exists and its characteristics
 *
 *	Accepts:	- network message
 *
 *	Returns:	- 1 if message exists, 0 if not
 *			- ERROR in case of error (-1)
 *			- entries in nhead are updated: 
 *			  type, length, flags, data[0], data[1]
 */

#include <bfreq.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

int
nprobe(pnh)

struct	nmsg		*pnh;

{
	if (pnh->nh_flags & NOBUF) {
		errno = ESERVICE;
		return(ERROR);
	}

	return(bfiprobe(pnh));
}
