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
 * $Id: vctable.c,v 6.6 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- manipulate the table of virtual circuits
 *
 *			- _vcinsert(): insert a new virtual circuit
 *			- _vcfind(): locate a virtual circuit entry
 *			- _vcdelete(): delete a virtual circuit entry
 */

#include <portable.h>
#include <net.h>
#include <terror.h>
#include <typical.h>
#include <vctable.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * local variables
 */
static int		f_init = 1;		/* initialization flag */
static struct vcdesc	vctable[VCMAX];		/* VC hash table */

/*
 *	_vcinsert
 *
 *	Function:	- insert a virtual circuit in the table
 *			- initialize the table if necessary
 *			- does not allow duplicate entries
 *	Accepts:	- node ID
 *			- event
 *			- type
 *			- VC path
 *	Returns:	- 0 or ERROR
 */
int
_vcinsert(int4 node, int4 event, int4 type, int4 path)
{
	int4		i, j;		/* favourite indices */
	struct vcdesc	*p;		/* favourite pointer */
/*
 * Initialize the table if first time function is called.
 */
	if (f_init) {
		f_init = 0;
		for (i = 0, p = vctable; i < VCMAX; ++i, ++p) {
			p->vc_node = NOTNODEID;
		}
	}

	if (node == NOTNODEID) {
		errno = EBADNODE;
		return(ERROR);
	}
/*
 * Locate an empty slot in the hash table.
 * Duplicate entries are not allowed.
 */
	i = _vchash(node, event, type);
	p = vctable + i;

	for (j = 0; j < VCMAX; ++j) {
		if (p->vc_node == NOTNODEID) {
/*
 * Found empty slot.
 */
			break;
		}
		else if ((p->vc_node == node) &&
			(p->vc_event == event) && (p->vc_type == type)) {
/*
 * Found duplicate entry.
 */
			errno = EINVAL;
			return(ERROR);
		}

		if (++i == VCMAX) {
			i = 0;
			p = vctable;
		} else {
			++p;
		}
	}
/*
 * If table is full, return an error.
 */
	if (j == VCMAX) {
		errno = EFULL;
		return(ERROR);
	}
/*
 * Fill the empty slot.
 */
	p->vc_node = node;
	p->vc_event = event;
	p->vc_type = type;
	p->vc_path = path;

	return(0);
}

/*
 *	_vcfind
 *
 *	Function:	- locate a virtual circuit in the table
 *	Accepts:	- node ID
 *			- event
 *			- type
 *	Returns:	- ptr to VC entry or NULL
 */
struct vcdesc *
_vcfind(int4 node, int4 event, int4 type)
{
	int4		i, j;		/* favourite indices */
	struct vcdesc	*p;		/* favourite pointer */

	if (node == NOTNODEID) {
		errno = EBADNODE;
		return((struct vcdesc *) 0);
	}
/*
 * Locate the entry in the hash table.
 */
	i = _vchash(node, event, type);
	p = vctable + i;

	for (j = 0; j < VCMAX; ++j) {
		if ((p->vc_node == node) &&
			(p->vc_event == event) && (p->vc_type == type)) {
/*
 * The entry is found.
 */
			return(p);
		}

		if (++i == VCMAX) {
			i = 0;
			p = vctable;
		} else {
			++p;
		}
	}
/*
 * The entry does not exist.
 */
	errno = EINVAL;
	return((struct vcdesc *) 0);
}

/*
 *	_vcdelete
 *
 *	Function:	- delete a virtual circuit from the table
 *	Accepts:	- node ID
 *			- event
 *			- type
 *	Returns:	- 0 or ERROR
 */
int
_vcdelete(int4 node, int4 event, int4 type)
{
	struct vcdesc	*p;		/* favourite pointer */
/*
 * Locate the correct VC entry in the table
 */
	p = _vcfind(node, event, type);
	if (p == 0) {
		return(ERROR);
	}
/*
 * Erase the entry.
 */
	p->vc_node = NOTNODEID;

	return(0);
}

/*
 *	_vcfree
 *
 *	Function:	- free all virtual circuits
 */
void
_vcfree(void)
{
	int4		i;		/* favourite index */
	struct vcdesc	*p;		/* favourite pointer */
/*
 * Free all V.C. entries.
 */
	f_init = 1;

	for (i = 0, p = vctable; i < VCMAX; ++i, ++p) {
		p->vc_node = NOTNODEID;
		close(p->vc_path);
	}

}
