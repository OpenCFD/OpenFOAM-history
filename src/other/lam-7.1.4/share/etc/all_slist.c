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
 * $Id: all_slist.c,v 6.5 2003/07/05 21:47:33 jsquyres Exp $
 * 
 *	Function:	- general purpose static allocated list package
 *			- space supplied by user
 *			- doubly linked
 *			- The dl_head structure is prepended to
 *			  the list element given by the user.
 *			  The user does not see dl_head.
 *			  Thus, we often have to shift back and
 *			  forth between the user element pointer
 *			  and the dl_head pointer, which is used to
 *			  traverse the list.
 *
 *			- The user must supply a function
 *			  that compares two list elements and returns
 *			  -1 if the first element is less than the second,
 *			  0 if they are equal, and
 *			  1 if the first element is greater than the second.
 *			  Lists are maintained in increasing element order.
 *
 *			- functions that operate on a list:
 *
 *			  als_init - Allocate and initialize a new list.
 *			  als_delete - Delete a list element.
 *			  als_insert - Insert a list element.
 *			  als_append - Insert an element at the end of a list.
 *			  als_free - Blow away a list.
 *			  als_find - Find a list element.
 *			  als_next - Find the next list element.
 *			  als_prev - Find the previous list element.
 *			  als_count - Count the elements in a list.
 */

#include <string.h>

#include <all_slist.h>
#include <terror.h>
#include <typical.h>

/*
 *	als_init
 *
 *	Function:	- Initialize a new list descriptor.
 *			- The free list is singly linked.
 *	Accepts:	- size of user element
 *			- comparison function
 *			- ptr to list space
 *			- ptr to list descriptor space
 *	Returns:	- ptr to new list descriptor
 */
LIST *
als_init(int4 elemsize, int4 (*comp)(), int4 nlist, struct al_head *plist, 
         LIST *ald)
{
	char		*curr;		/* current list element ptr */
	char		*next;		/* next list element ptr */
	char		*prev;		/* previous list element ptr */
	int4		n;		/* favourite counter */
/*
 * Initialize list descriptor.
 */
	ald->al_elemsize = elemsize;
	ald->al_comp = comp;
	ald->al_ltop = 0;
	ald->al_lbottom = 0;
	ald->al_ftop = plist;
/*
 * Initialize the free list.
 */
	curr = (char *) plist;
	prev = curr;

	for (n = 0; n < nlist; ++n) {
		next = curr + elemsize + sizeof(struct al_head);
		((struct al_head *) curr)->al_next = (struct al_head *) next;
		prev = curr;
		curr = next;
	}
		
	((struct al_head *) prev)->al_next = 0;
	return(ald);
}

/*
 *	als_delete
 *
 *	Function:	- Delete an element from a list.
 *			- Place old element in free list.
 *	Accepts:	- list descriptor ptr
 *			- ptr to existing element
 *	Returns:	- 0 or ERROR
 */
int4
als_delete(SLIST *ald, void *old)
{
	struct al_head	*curr;
	struct al_head	*prev;

	curr = ald->al_ltop;
	prev = 0;

	while ((curr != 0) && (al_body(curr) != (char *) old)) {
		prev = curr;
		curr = curr->al_next;
	}

	if (curr == 0) {
		errno = EDELETE;
		return(LAMERROR);
	}

	if (curr == ald->al_ltop) {
		ald->al_ltop = curr->al_next;
	} else {
		prev->al_next = curr->al_next;
	}

	if (curr == ald->al_lbottom) {
		ald->al_lbottom = prev;
	} else {
		curr->al_next->al_prev = prev;
	}
/*
 * Insert old element into the free list.
 */
	curr->al_next = ald->al_ftop;
	ald->al_ftop = curr;
	return(0);
}

/*
 *	als_insert
 *
 *	Function:	- Insert an element into a list.
 *			- The user's new element is copied
 *			  into a new element.
 *			- A new element is taken from the free list.
 *	Accepts:	- list descriptor ptr
 *			- new element ptr
 *	Returns:	- new list element ptr
 */
void *
als_insert(LIST *ald, void *unew)
{
	struct al_head	*curr;
	struct al_head	*prev;
	struct al_head	*new_head;		/* new internal list element */
	char		*new_char;		/* user portion of list
						   element */
/*
 * Obtain an element from the free list.
 */
	if (ald->al_ftop == 0) {
		errno = EFULL;
		return((void *) 0);
	}

	new_head = ald->al_ftop;
	ald->al_ftop = new_head->al_next;

	new_char = al_body(new_head);
	memcpy(new_char, (char *) unew, ald->al_elemsize);
	curr = ald->al_ltop;
	prev = 0;

	while ((curr != 0) &&
		(((*(ald->al_comp))(new_char, al_body(curr))) >= 0)) {

		prev = curr;
		curr = curr->al_next;
	}

	new_head->al_next = curr;
	new_head->al_prev = prev;

	if (curr == ald->al_ltop) {
		ald->al_ltop = new_head;
	} else {
		prev->al_next = new_head;
	}

	if (curr == 0) {
		ald->al_lbottom = new_head;
	} else {
		curr->al_prev = new_head;
	}

	return((void *) new_char);
}

/*
 *	als_append
 *
 *	Function:	- Append a new element to a list.
 *			- Element is added at the end of the list.
 *			- The user's new element is copied
 *			  into a new element.
 *	Accepts:	- list descriptor ptr
 *			- new element ptr
 *	Returns:	- new list element ptr
 */
void *
als_append(LIST *ald, void *unew)
{
	struct al_head	*new_head;		/* new internal list element */
	char		*new_char;		/* user portion of list
						   element */
/*
 * Obtain an element from the free list.
 */
	if (ald->al_ftop == 0) {
		errno = EFULL;
		return((void *) 0);
	}

	new_head = ald->al_ftop;
	ald->al_ftop = new_head->al_next;

	new_char = al_body(new_head);
	memcpy(new_char, (char *) unew, ald->al_elemsize);
	
	new_head->al_next = 0;
	new_head->al_prev = ald->al_lbottom;

	if (ald->al_lbottom != 0) {
		ald->al_lbottom->al_next = new_head;
	} else {
		ald->al_ltop = new_head;
	}

	ald->al_lbottom = new_head;
	return((void *) new_char);
}
