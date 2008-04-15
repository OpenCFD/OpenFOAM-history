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
 *	$Id: all_list.c,v 6.9 2003/06/05 05:34:18 brbarret Exp $
 * 
 *	Function:	- general purpose dynamic list package
 *			- assumes a fast malloc()
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
 *			  al_init - Allocate and initialize a new list.
 *			  al_delete - Delete a list element.
 *			  al_insert - Insert a list element.
 *			  al_update - Update a list element.
 *			  al_append - Insert an element at the end of a list.
 *			  al_free - Blow away a list.
 *			  al_find - Find a list element.
 *			  al_next - Find the next list element.
 *			  al_prev - Find the previous list element.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <lam.h>


/*
 *	al_init
 *
 *	Function:	- Initialize a new list descriptor.
 *	Accepts:	- size of user element
 *			- comparison function
 *	Returns:	- ptr to new list descriptor
 */
LIST *
al_init(int4 elemsize, int4 (*comp)())
{
	LIST		*ald;			/* new al descriptor */

	ald = (LIST *) malloc(sizeof(LIST));

	if (ald == 0) {
		return((LIST *) 0);
	}

	ald->al_elemsize = elemsize;
	ald->al_nelem = 0;
	ald->al_ltop = 0;
	ald->al_lbottom = 0;
	ald->al_comp = comp;

	return(ald);
}


/*
 *	al_delete
 *
 *	Function:	- Delete an element from a list.
 *	Accepts:	- list descriptor ptr
 *			- ptr to existing element
 *	Returns:	- 0 or ERROR
 */
int4
al_delete(LIST *ald, void *old)
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
		return(ERROR);
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

	free((char *) old - sizeof(struct al_head));
	ald->al_nelem--;
	return(0);
}


/*
 *	al_insert
 *
 *	Function:	- Insert an element into a list.
 *			- The user's new element is copied
 *			  into space allocated internally for a
 *			  new list element.
 *	Accepts:	- list descriptor ptr
 *			- new element ptr
 *	Returns:	- new list element ptr
 */
void *
al_insert(LIST *ald, void *unew)
{
	struct al_head	*curr;
	struct al_head	*prev;
	struct al_head	*new_head;		/* new internal list element */
	char		*new_char;		/* user portion of list
						   element */

	new_head = (struct al_head *) malloc((unsigned)
			(ald->al_elemsize + sizeof(struct al_head)));

	if (new_head == 0) {
		return((void *) 0);
	}

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

	ald->al_nelem++;
	return((void *) new_char);
}


/*
 *	al_append
 *
 *	Function:	- Append a new element to a list.
 *			- Element is added at the end of the list.
 *			- A new list element is allocated internally
 *			  and the user supplied element is copied,
 *			  as with al_insert().
 *	Accepts:	- list descriptor ptr
 *			- new element ptr
 *	Returns:	- new list element ptr
 */
void *
al_append(LIST *ald, void *unew)
{
	struct al_head	*new_head;		/* new internal list element */
	char		*new_char;		/* user portion of list
						   element */

	new_head = (struct al_head *) malloc((unsigned)
			(ald->al_elemsize + sizeof(struct al_head)));

	if (new_head == 0) {
		return((void *) 0);
	}

	new_char = al_body(new_head);
	memcpy(new_char, unew, ald->al_elemsize);
	
	new_head->al_next = 0;
	new_head->al_prev = ald->al_lbottom;

	if (ald->al_lbottom != 0) {
		ald->al_lbottom->al_next = new_head;
	} else {
		ald->al_ltop = new_head;
	}

	ald->al_lbottom = new_head;
	ald->al_nelem++;
	return((void *) new_char);
}


/*
 *	al_free
 *
 *	Function:	- Free all list elements and list descriptor.
 *			- Effectively, blow away the list.
 *	Accepts:	- list descriptor ptr
 *	Returns:	- 0
 */
int4
al_free(LIST *ald)
{
	struct al_head	*curr;
	struct al_head	*next;

	/* be nice to people passing in NULL */
	if (ald == NULL) {
	  return 0;
	}

	curr = ald->al_ltop;

	while (curr != 0) {
		next = curr->al_next;
		free((char *) curr);
		curr = next;
	}

	free((char *) ald);
	return(0);
}


/*
 *	al_find
 *
 *	Function:	- Find a list element that matches the
 *			  user supplied element's key.
 *			- The user supplied comparison function
 *			  is employed to determine equality.
 *	Accepts:	- list descriptor ptr
 *			- search key element ptr
 *	Returns:	- found list element ptr
 */
void *
al_find(LIST *ald, void *key)
{
	struct al_head	*curr;

	curr = ald->al_ltop;

	while ((curr != 0) && ((*(ald->al_comp))(key, al_body(curr)))) {
		curr = curr->al_next;
	}

	return(curr ? (void *) al_body(curr) : (void *) 0);
}


/*
 *	al_next
 *
 *	Function:	- find the next element in a list
 *	Accepts:	- list descriptor ptr
 *			- existing list element ptr
 *	Returns:	- next list element ptr
 */
void *
al_next(LIST *ald, void *old)
{
	struct al_head	*curr;

	curr = (struct al_head *) ((char *) old - sizeof(struct al_head));
	return(curr->al_next ? (void *) al_body(curr->al_next) : (void *) 0);
}


/*
 *	al_prev
 *
 *	Function:	- find the previous element in a list
 *	Accepts:	- list descriptor ptr
 *			- existing list element ptr
 *	Returns:	- previous list element ptr
 */
void *
al_prev(LIST *ald, void *old)
{
	struct al_head	*curr;

	curr = (struct al_head *) ((char *) old - sizeof(struct al_head));
	return((void *) al_body(curr->al_prev));
}
