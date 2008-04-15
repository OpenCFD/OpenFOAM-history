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
 * $Id: all_hash.h,v 6.5 2002/10/09 20:57:22 brbarret Exp $
 * 
 *	Function:	- generic hash table templates and constants
 *			- for both the static and dynamic versions
 */

#ifndef _ALLHASH
#define _ALLHASH

#include <lam_config.h>
#include <portable.h>

/*
 * flags defining the modes of operation
 */
#define AHLRU		((int4) 1)	/* LRU counters are used */
#define AHNOINIT	((int4) 2)	/* hash table not initialized */

/* 
 * structure definitions
 */
struct ah_desc {			/* hash table descriptor */
	int4		ah_maxnelem;	/* maximum nbr. elements */
	int4		ah_nelem;	/* current nbr. elements */
	int4		ah_elemsize;	/* size of element */
	int4		ah_nullkey;	/* null hash key value */
	int4		ah_mode;	/* mode of operation */
	int4		*ah_lru;	/* table of LRU counters */
	void		*ah_table;	/* ptr to the hash table */
	int		(*ah_cmp)();	/* comparison function */
};

/*
 * type definitions
 */
typedef struct ah_desc	HASH;
typedef struct ah_desc	SHASH;

/*
 * prototypes
 */

#ifdef __cplusplus
extern "C" {
#endif

extern HASH		*ah_init __ARGS((int4 size, int4 elemsize,
				int4 nullkey, int4 mode));
extern SHASH		*ahs_init __ARGS((int4 size, int4 esize,
				int4 nullkey, int4 mode, void *htbl,
				int4 *lru, SHASH *ahsd));
extern int		ah_delete __ARGS((HASH *ahd, int4 key));
extern int		ah_delete_elm __ARGS((HASH *ahd, void *elem));
extern int		ah_expand __ARGS((HASH *ahd, int4 newsize));
extern int		ah_insert __ARGS((HASH *ahd, void *elem));
extern int		ah_kick __ARGS((HASH *ahd, void *elem));
extern void		ah_free __ARGS((HASH *ahd));
extern void		ah_setcmp __ARGS((HASH *ahd, int (*cmp)()));
extern void		*ah_find __ARGS((HASH *ahd, int4 key));
extern void		*ah_find_elm __ARGS((HASH *ahd, void *elem));
extern void		*ah_next __ARGS((HASH *ahd, void *elem));

#ifdef __cplusplus
}
#endif

/*
 * function macros
 */
#define ah_count(ahd)		((ahd)->ah_nelem)
#define ah_size(ahd)		((ahd)->ah_maxnelem)
#define ah_top(ahd)		(ah_next(ahd, (void *) 0))

#define ahs_find(ahd, x)	ah_find(ahd, x)
#define ahs_find_elm(ahd, x)	ah_find_elm(ahd, x)
#define ahs_delete(ahd, x)	ah_delete(ahd, x)
#define ahs_delete_elm(ahd, x)	ah_delete_elm(ahd, x)
#define ahs_insert(ahd, x)	ah_insert(ahd, x)
#define ahs_kick(ahd, x)	ah_kick(ahd, x)
#define ahs_setcmp(ahd, x)	ah_setcmp(ahd, x)
#define ahs_count(ahd)		ah_count(ahd)
#define ahs_size(ahd)		ah_size(ahd)
#define ahs_top(ahd)		ah_top(ahd)
#define ahs_next(ahd, x)	ah_next(ahd, x)

#endif
