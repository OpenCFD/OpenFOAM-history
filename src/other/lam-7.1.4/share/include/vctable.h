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
 * $Id: vctable.h,v 6.4 2003/07/05 21:54:43 jsquyres Exp $
 * 
 *	Function:	- virtual circuits templates and constants
 */

#ifndef _VCTABLE
#define _VCTABLE

#include <portable.h>

/*
 * constants
 */
#define VCMAX		67		/* VC table size (prime #) */

/*
 * hashing macro
 */
#define _vchash(n,e,t)	(abs((n) + (e) + (t)) % VCMAX)

/*
 * templates
 */
struct vcdesc {
	int4		vc_node;	/* node identifier */
	int4		vc_event;	/* message event */
	int4		vc_type;	/* event type */
	int4		vc_path;	/* VC path */
};

#ifdef __cplusplus
extern "C" {
#endif

int _vcinsert(int4 node, int4 event, int4 type, int4 path);
struct vcdesc *_vcfind(int4 node, int4 event, int4 type);
int _vcdelete(int4 node, int4 event, int4 type);
void _vcfree(void);

#ifdef __cplusplus
}
#endif

#endif
