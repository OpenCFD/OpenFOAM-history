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
 *	Revision 6.2  1999/03/17 14:19:38  jsquyres
 *	
 *	Revision 6.1  1996/11/23 21:54:39  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:53:05  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  14:00:22  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:44:13  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:48:37  gdburns
 * Ohio Release
 * 
 * Revision 2.2.1.4  94/01/08  16:34:05  raja
 * Remove const from prototypes for now.
 * 
 * Revision 2.2.1.3  93/12/11  17:02:38  raja
 * Use const in prototypes.
 * 
 * Revision 2.2.1.2  93/09/15  14:59:11  raja
 * Declare ANSI C and C++ function prototypes.
 * 
 * Revision 2.2.1.1  92/05/24  16:03:24  raja
 * Make structure declaration preceed its typedef usage to suit ULS.
 * 
 * Revision 2.2  92/04/30  14:44:51  trillium
 * Ohio Release
 * 
 * Revision 2.1.1.1  92/01/19  22:01:51  raja
 * Modified functions to fit new standard interface.
 * 
 * Revision 2.1  91/03/20  11:34:37  gdburns
 * Ohio Release
 * 
 *	Function:	- generic queue templates and constants
 *			- for both the static and dynamic versions
 */

#ifndef _ALLQUEUE
#define _ALLQUEUE

#include <portable.h>

/*
 * structure definitions
 */
struct aq_desc {			/* queue descriptor */
	int4		aq_maxnelem;	/* maximum nbr. elements */
	int4		aq_nelem;	/* current nbr. elements */
	int4		aq_elemsize;	/* size of element */
	int4		aq_first;	/* index to first element */
	int4		aq_last;	/* index to last element */
	void		*aq_queue;	/* ptr to the queue array */
};

/*
 * type definitions
 */
typedef struct aq_desc	QUEUE;
typedef struct aq_desc	SQUEUE;

#ifdef __cplusplus
extern "C" {
#endif

extern QUEUE		*aq_init __ARGS((int4 size, int4 elemsize));
extern SQUEUE		*aqs_init __ARGS((int4 size, int4 elemsize,
				void *queue, SQUEUE *aqsd));
extern int		aq_delete __ARGS((QUEUE *aqd));
extern int		aq_expand __ARGS((QUEUE *aqd, int4 newsize));
extern int		aq_insert __ARGS((QUEUE *aqd, void *elem));
extern int		aq_shove __ARGS((QUEUE *aqd, void *elem));
extern void		aq_free __ARGS((QUEUE *aqd));
extern void		*aq_find __ARGS((QUEUE *aqd));
 
#ifdef __cplusplus
}
#endif

/*
 * function macros
 */
#define aq_count(aqd)		((aqd)->aq_nelem)
#define aq_size(aqd)		((aqd)->aq_maxnelem)

#define aqs_find(aqd)		aq_find(aqd)
#define aqs_delete(aqd)		aq_delete(aqd)
#define aqs_insert(aqd, x)	aq_insert(aqd, x)
#define aqs_count(aqd)		aq_count(aqd)
#define aqs_size(aqd)		aq_size(aqd)

#endif
