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
 *	Software for Humanity
 *	RBD
 *
 *	This program is freely distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	$Id: all_squeue.c,v 6.3 2002/10/09 20:57:01 brbarret Exp $
 * 
 *	Function:	- generic queue management code
 *			- fully static version
 *				- aqs_init(): create a queue
 *			- other functions macro'ed to the dynamic version
 */

#include <errno.h>

#include <all_queue.h>
#include <portable.h>

/*
 *	aqs_init
 *
 *	Function:	- creates a static queue
 *	Accepts:	- size of queue
 *			- size of queue element
 *			- queue array
 *			- static queue descriptor
 *	Returns:	- static queue descriptor or NULL
 */
SQUEUE *
aqs_init(size, elemsize, queue, aqsd)

int4			size;
int4			elemsize;
void			*queue;
SQUEUE			*aqsd;

{
	if ((size <= 0) || (elemsize <= 0)) {
		errno = EINVAL;
		return((SQUEUE *) 0);
	}
/*
 * Initialize the queue descriptor.
 */
	aqsd->aq_maxnelem = size;
	aqsd->aq_elemsize = elemsize;
	aqsd->aq_nelem = 0;
	aqsd->aq_first = 0;
	aqsd->aq_last = 0;
	aqsd->aq_queue = queue;

	return(aqsd);
}
