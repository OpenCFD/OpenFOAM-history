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
 * $Id: lamattr.c,v 6.10 2003/11/23 15:02:24 jsquyres Exp $
 *
 *	Function:	- manage table of attribute keys
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <all_hash.h>
#include <mpi.h>
#include <mpisys.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

#define DEFATTRSIZE	8
#define DEFKEYSIZE	11			/* prime number */


/*
 * static variables
 */
static int		attrcount = 0;
static int		attrsize = 0;
static struct _attrkey	*attrs = 0;
#if LAM_SIZEOF_VOID_P > LAM_SIZEOF_INT
static int		intpos = -1;
#endif

/*
 *	lam_mkkey
 *
 *	Function:	- create a new attribute key
 *	Returns:	- key or ERROR
 */
int
lam_mkkey(void)
{
	struct _attrkey	*p;
	int		i;
/*
 * Allocate initial attribute array if needed.
 * Assign first position.
 */
	if (attrsize == 0) {
		i = DEFATTRSIZE * sizeof(struct _attrkey);
		attrs = (struct _attrkey *) malloc((unsigned) i);
		if (attrs == 0) {
			return(ERROR);
		}

		memset((char *) attrs, 0, i);
		attrsize = DEFATTRSIZE;
		p = attrs;
		i = 0;
	}
/*
 * Expand attribute array if needed.
 * Assign first position in the expanded side.
 */
	else if (attrsize == attrcount) {
		i = attrsize * sizeof(struct _attrkey);
		p = (struct _attrkey *) realloc((char *)attrs, (unsigned) i+i);
		if (p == 0) {
			return(ERROR);
		}

		attrs = p;
		p += attrsize;
		memset((char *) p, 0, i);
		i = attrsize;
		attrsize += attrsize;
	}
/*
 * Find an empty slot and assign it.
 */
	else {
		for (i = 0, p = attrs; i < attrsize; ++i, ++p) {
			if (p->ak_refcount == 0) {
				break;
			}
		}

		if (i == attrsize) {
			errno = EIMPOSSIBLE;
			return(ERROR);
		}
	}
/*
 * Reserve the slot.
 */
	++attrcount;
	p->ak_refcount = 1;
	return(i);
}


/*
 *	lam_getattr
 *
 *	Function:	- get ptr to attribute
 *	Accepts:	- attribute key
 *	Returns:	- ptr to attribute or NULL
 */
struct _attrkey *
lam_getattr(int key)
{
	struct _attrkey	*p;

	errno = EINVAL;

	if ((key >= 0) && (key < attrsize)) {
		p = attrs + key;
		if (p->ak_refcount == 0) {
			p = 0;
		}
	} else {
		p = 0;
	}

	return(p);
}


/*
 *	lam_freekey
 *
 *	Function:	- free attribute key
 *	Accepts:	- attribute key
 *	Accepts:	- attribute key type
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_freekey(int key, int keytype)
{
	struct _attrkey	*p;

	p = lam_getattr(key);
	if (p == 0 || (p->ak_flags & LAM_PREDEF)
			|| (p->ak_refcount <= 0) || !(p->ak_flags & keytype)) {
		return(lam_mkerr(MPI_ERR_KEYVAL, EINVAL));
	}

	if (--(p->ak_refcount) == 0) {
		--attrcount;
	}

	return(MPI_SUCCESS);
}


/*
 *	lam_putkey
 *
 *	Function:	- insert a key into a set of keys
 *	Accepts:	- set of keys
 *			- key
 *	Returns:	- 0 or ERROR
 */
int
lam_putkey(HASH **keys, int key)
{
	struct _attr	elem;
	struct _attrkey	*p;
/*
 * Check if key is valid.
 */
	p = lam_getattr(key);
	if (p == 0) {
		return(ERROR);
	}
/*
 * Allocate the key hash table if needed.
 */
	if (*keys == 0) {
		*keys = ah_init(DEFKEYSIZE, (int4) sizeof(struct _attr),
					(int4) MPI_KEYVAL_INVALID, INT4_NIL);
		if (*keys == 0) {
			return(ERROR);
		}
	}
/*
 * Expand the key hash table if needed.
 */
	else if (ah_count(*keys) == ah_size(*keys)) {
		if (ah_expand(*keys,
				next_prime(1 + ah_size(*keys)))) {
			return(ERROR);
		}
	}
/*
 * Store new key in hash table.  
 */
	LAM_ZERO_ME(elem);
	elem.a_key = key;
	elem.a_value = (void*) 0;

	if (ah_insert(*keys, &elem)) {
		return(ERROR);
	}

	if (p->ak_refcount > 0) {
		++(p->ak_refcount);
	}

	return(0);
}


/*
 *	lam_getkey
 *
 *	Function:	- get key entry in set of keys
 *	Accepts:	- set of keys
 *			- key
 *	Returns:	- ptr to entry or NULL
 */
struct _attr *
lam_getkey(HASH *keys, int key)
{
	struct _attr	*p;

	if (keys) {
		p = (struct _attr *) ah_find(keys, key);
	} else {
		p = 0;
	}

	return(p);
}


/*
 *	lam_delkey
 *
 *	Function:	- delete key from set
 *	Accepts:	- set of keys
 *			- key
 *	Returns:	- 0 or ERROR
 */
int
lam_delkey(HASH *keys, int key)
{
	return(ah_delete(keys, key));
}


/*
 *	lam_nukekeys
 *
 *	Function:	- deallocate all keys (cleanup)
 */
void
lam_nukekeys(void)
{
	if (attrs) {
		free((char *) attrs);
		attrcount = attrsize = 0;
		attrs = 0;
	}
}


/*
 *	lam_keyval_create
 *
 *	Function:	- create an attribute key
 *	Accepts:	- copy function
 *			- delete function
 *			- key value (out)
 *			- extra state info
 *			- type of key
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_keyval_create(lam_void_fn_t cpyfunc, lam_void_fn_t delfunc, int *pkey, 
                  void *extra, int keytype)
{
	int		key;
	struct _attrkey	*p;
/*
 * Check the arguments.
 */
	if (pkey == 0) {
		return(lam_mkerr(MPI_ERR_ARG, EINVAL));
	}
/*
 * Create the key.
 */
	if ((key = lam_mkkey()) < 0) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	*pkey = key;
	p = lam_getattr(key);
	if (p == 0) {
		return(lam_mkerr(MPI_ERR_OTHER, errno));
	}
/*
 * Fill the attribute information.
 */
	p->ak_copy = cpyfunc;
	p->ak_del = delfunc;
	p->ak_extra = extra;
	p->ak_refcount = 1;
	p->ak_flags = keytype;

	return(MPI_SUCCESS);
}


/*
 *	lam_int_pos
 *
 *	Function:	- where sizeof(void*) > sizeof(int)
 *	Returns:	- returns position, if (void*) is looked at as 
 *			  (int**), of where lower (sizeof(int)) bits are
 */
int
lam_int_pos(void)
{
#if LAM_SIZEOF_VOID_P <= LAM_SIZEOF_INT
  return 0;
#else
  void *v = (void*) 1;
  int *i = (int*) &v;

  if (intpos != -1)
    return intpos;

  for (intpos = 0; intpos < (sizeof(void*) / sizeof(int)); intpos++)
    if (i[intpos] == 1)
      break;

  return intpos;
#endif
}
