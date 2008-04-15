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
 *	$Id: all_list.h,v 6.7 2003/11/06 18:22:48 jsquyres Exp $
 * 
 *	Function:	- constants and templates associated with all_list
 */

#ifndef _ALLLIST
#define _ALLLIST

#include <stdio.h>
#include "lam.h"

/*
 * templates
 */
struct al_head {
	struct al_head	*al_next;		/* next list element */
	struct al_head	*al_prev;		/* previous list element */
};

struct al_desc {
	int4		al_elemsize;		/* element size */
	int4		al_nelem;		/* # of elements */
	struct al_head	*al_ltop;		/* top of list ptr */
	struct al_head	*al_lbottom;		/* bottom of list ptr */
	struct al_head	*al_ftop;		/* top of free list ptr */
	int4		(*al_comp)();		/* comparison function */
};

typedef struct al_desc	LIST;

#define al_body(x) ((x) ? (((char *) (x)) + sizeof(struct al_head)) : NULL)

#define al_count(x) ((x) ? (x)->al_nelem : 0)
#define al_top(x) ((al_count(x) > 0) ? (void *) al_body((x)->al_ltop) : NULL)
#define al_bottom(x) ((al_count(x) > 0) ? (void *) al_body((x)->al_lbottom) : NULL)

#ifdef __cplusplus
extern "C" {
#endif

extern LIST		*al_init __ARGS((int4 elemsize, int4 (*comp)()));
extern void		*al_append __ARGS((LIST *ald, void *unew));
extern void		*al_find __ARGS((LIST *ald, void *key));
extern void		*al_insert __ARGS((LIST *ald, void *unew));
extern void		*al_update __ARGS((LIST *ald, void *old));
extern void		*al_next __ARGS((LIST *ald, void *old));
extern void		*al_prev __ARGS((LIST *ald, void *old));
extern int4		al_delete __ARGS((LIST *ald, void *old));
extern int4		al_free __ARGS((LIST *ald));

#ifdef __cplusplus
}
#endif

#endif
