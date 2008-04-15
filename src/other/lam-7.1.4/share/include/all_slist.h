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
 * $Id: all_slist.h,v 6.5 2003/07/05 21:52:56 jsquyres Exp $
 * 
 *	Function:	- constants and templates associated with all_slist
 */

#ifndef _ALLSLIST
#define _ALLSLIST

#include <all_list.h>

typedef struct al_desc	SLIST;

#define als_top(x) ((void *) al_body(x->al_ltop))
#define als_bottom(x) ((void *) al_body(x->al_lbottom))
#define als_find al_find
#define als_next al_next
#define als_prev al_prev
#define als_count al_count

#ifdef __cplusplus
extern "C" {
#endif

extern SLIST		*als_init __ARGS((int4 elemsize, int4 (*comp)(),
				int4 nlist, struct al_head *list, SLIST *ald));
extern void		*als_append __ARGS((SLIST *ald, void *unew));
extern void		*als_insert __ARGS((SLIST *ald, void *unew));
extern int4		als_delete __ARGS((SLIST *ald, void *old));

#ifdef __cplusplus
}
#endif

#endif
