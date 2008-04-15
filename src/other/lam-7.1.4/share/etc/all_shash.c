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
 *	$Id: all_shash.c,v 6.3 2002/10/09 20:57:01 brbarret Exp $
 * 
 *	Function:	- generic hash table management code
 *			- fully static version
 *				- ahs_init(): create a hash table
 *			- other functions  macro'ed to the dynamic version
 *			- the record used must have an int4 key as
 *			  its first field
 */

#include <errno.h>

#include <all_hash.h>
#include <portable.h>

/*
 *	ahs_init
 *
 *	Function:	- create a static hash table
 *
 *	Accepts:	- size of hash table
 *			- size of hash table element
 *			- value of the null hash key
 *			- operation mode
 *			- hash table array
 *			- LRU counters array
 *			- ptr to static hash table descriptor
 *
 *	Returns:	- ptr to static hash table descriptor or NULL
 */
SHASH * 
ahs_init(size, elemsize, nullkey, mode, htable, lru, ahsd)

int4			size;
int4			elemsize;
int4			nullkey;
int4			mode;
void			*htable;
int4			*lru;
SHASH			*ahsd;

{
	int4		i;		/* favorite counter */
	int4		*p;		/* favorite pointer */
/* 
 * Handle the trivial cases.
 * The element must be big enough to hold the int4 key.
 */
	if ( (size <= 0) || (elemsize < sizeof(int4)) ) {
		errno = EINVAL;
		return(0);
	}
/*
 * Initialize the static hash table descriptor.
 */
	ahsd->ah_maxnelem = size;
	ahsd->ah_elemsize = elemsize;
	ahsd->ah_nelem = 0;
	ahsd->ah_nullkey = nullkey;
	ahsd->ah_mode = mode;
	ahsd->ah_table = htable;

	if ((mode & AHLRU) == AHLRU) {
		ahsd->ah_lru = lru;
	} else {
		ahsd->ah_lru = (int4 *) 0;
	}
/*
 * Initialize the hash table if AHNOINIT is not specified.
 * Reset all LRU counters if AHLRU is specified.
 */
	if ((mode & AHNOINIT) != AHNOINIT) {

		for (i = 0, p = (int4 *) ahsd->ah_table; i < size; i++) {
			*p = nullkey;
			p = (int4 *) ((char *) p + elemsize);
		}
	}

	if ((mode & AHLRU) == AHLRU) {

		for (i = 0; i < size; i++) {
			ahsd->ah_lru[i] = 0;
		}
	}

	return(ahsd);
}
