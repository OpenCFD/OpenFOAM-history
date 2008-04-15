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
 *	$Id: all_queue.c,v 6.3 2002/10/09 20:57:00 brbarret Exp $
 * 
 *	Function:	- generic queue management code
 *			- fully dynamic version
 *				- aq_init(): create a queue
 *				- aq_free(): free a queue
 *				- aq_insert(): insert element
 *				- aq_shove(): insert element by force
 *				- aq_delete(): delete element
 *				- aq_find(): read element
 *				- aq_expand(): increase queue size
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <all_queue.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>

/*
 *	aq_init
 *
 *	Function:	- create a queue
 *	Accepts:	- size of queue
 *			- size of queue element
 *	Returns:	- ptr to queue descriptor or NULL
 */
QUEUE *
aq_init(size, elemsize)

int4			size;
int4			elemsize;

{
	QUEUE		*aqd;
/*
 * Allocate the queue descriptor.
 */
	if ((aqd = (QUEUE *) malloc(sizeof(QUEUE))) == 0) {
		return((QUEUE *) 0);
	}

	aqd->aq_maxnelem = size;
	aqd->aq_elemsize = elemsize;
	aqd->aq_nelem = 0;
	aqd->aq_first = 0;
	aqd->aq_last = 0;
/*
 * Allocate the queue array.
 */
	aqd->aq_queue = (void *) malloc((uint4) size * elemsize);
	if (aqd->aq_queue == 0) {
		free((char *) aqd);
		return((QUEUE *) 0);
	}

	return(aqd);
}

/*
 *	aq_free
 *
 *	Function:	- free a queue
 *			- queue must have been created with aq_init()
 *	Accepts:	- ptr to queue descriptor
 */
void
aq_free(aqd)

QUEUE			*aqd;

{
	if (aqd) {
		if (aqd->aq_queue) {
			free((char *) aqd->aq_queue);
		}
		free((char *) aqd);
	}
}

/*
 *	aq_insert
 *
 *	Function:	- insert an element at the end of the queue
 *	Accepts:	- ptr to queue descriptor
 *			- ptr to the element
 *	Returns:	- 0 or LAMERROR
 */
int
aq_insert(aqd, elem)

QUEUE			*aqd;
void			*elem;

{
	char		*p;		/* favorite pointer */
/*
 * Check if the queue is full.
 */
	if (aqd->aq_nelem >= aqd->aq_maxnelem) {
		errno = EFULL;
		return(LAMERROR);
	}
/*
 * Enqueue the new element.
 */
	aqd->aq_nelem++;
	p = ((char *) aqd->aq_queue) + (aqd->aq_last * aqd->aq_elemsize);
	memcpy(p, (char *) elem, aqd->aq_elemsize);
	aqd->aq_last = (aqd->aq_last + 1) % aqd->aq_maxnelem;

	return(0);
}

/*
 *	aq_shove
 *
 *	Function:	- insert an element at the end of the queue
 *			- if the queue is full double its size
 *	Accepts:	- ptr to queue descriptor
 *			- ptr to the element
 *	Returns:	- 0 or LAMERROR
 */
int
aq_shove(aqd, elem)

QUEUE			*aqd;
void			*elem;

{
	if (aq_insert(aqd, elem)) {
/*
 * The queue is full, expand it by doubling its size.
 */
		if (aq_expand(aqd, 2 * aqd->aq_maxnelem)) {
			return(LAMERROR);
		}
/*
 * Insert the element.
 */
		if (aq_insert(aqd, elem)) {
			errno = EIMPOSSIBLE;
			return(LAMERROR);
		}
	}

	return(0);
}

/*
 *	aq_delete
 *
 *	Function:	- delete an element from the top of the queue
 *	Accepts:	- ptr to queue descriptor
 *	Returns:	- 0 or LAMERROR
 */
int
aq_delete(aqd)

QUEUE			*aqd;

{
/*
 * Check if the queue if empty.
 */
	if (aqd->aq_nelem <= 0) {
		errno = EDELETE;
		return(LAMERROR);
	}
/*
 * Dequeue an element.
 */
	aqd->aq_nelem--;
	aqd->aq_first = (aqd->aq_first + 1) % aqd->aq_maxnelem;

	return(0);
}

/*
 *	aq_find
 *
 *	Function:	- locate the first element in the queue
 *	Accepts:	- ptr to queue descriptor
 *	Returns:	- ptr to element or NULL
 */
void *
aq_find(aqd)

QUEUE			*aqd;

{
/*
 * Check if the queue is empty.
 */
	if (aqd->aq_nelem <= 0) {
		return((void *) 0);
	}
	else {
		return((void *) (((char *) aqd->aq_queue) +
			(aqd->aq_first * aqd->aq_elemsize)));
	} 
}

/*
 *	aq_expand
 *
 *	Function:	- expand the size of the queue
 *			- new size must be bigger than old size
 *	Accepts:	- ptr to queue descriptor
 *			- new queue size
 *	Returns:	- 0 or LAMERROR
 */
int
aq_expand(aqd, newsize)

QUEUE			*aqd;
int4			newsize;

{
	void		*new_q;		/* new queue array */
	int4		size;

	if (newsize < aqd->aq_maxnelem) {
		errno = EINVAL;
		return(LAMERROR);
	}
	else if (newsize == aqd->aq_maxnelem) {
		return(0);
	}
/*
 * Create a new queue array of a bigger size.
 */
	new_q = (void *) malloc((uint4) newsize * aqd->aq_elemsize);
	if (new_q == 0) {
		return(LAMERROR);
	}

	if (aqd->aq_nelem == 0) {
/*
 * Do nothing, no elements to copy.
 */
	}
	else if (aqd->aq_first + aqd->aq_nelem <= aqd->aq_maxnelem) {
/*
 * The current queue does not wrap around the array boundary.
 * A single memory copy is enough.
 */
		memcpy((char *) new_q, ((char *) aqd->aq_queue) +
			(aqd->aq_first * aqd->aq_elemsize),
			(aqd->aq_nelem * aqd->aq_elemsize));
	}
	else {
/*
 * The current queue wraps around the array boundary.
 * Two memory copies are required for the two separate chunks.
 */
		size = (aqd->aq_maxnelem - aqd->aq_first) * aqd->aq_elemsize;
		memcpy((char *) new_q, ((char *) aqd->aq_queue) +
				(aqd->aq_first * aqd->aq_elemsize), size);
		memcpy((char *) new_q + size, (char *) aqd->aq_queue,
				aqd->aq_last * aqd->aq_elemsize);
	}
/*
 * Finish the clean up of the queue descriptor.
 */
	if (aqd->aq_queue) {
		free((char *) aqd->aq_queue);
	}

	aqd->aq_queue = new_q;
	aqd->aq_first = 0;
	aqd->aq_last = aqd->aq_nelem;
	aqd->aq_maxnelem = newsize;

	return(0);
}
