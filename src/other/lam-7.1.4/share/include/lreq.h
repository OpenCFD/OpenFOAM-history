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
 *	Revision 6.2  1999/03/17 14:19:41  jsquyres
 *	
 *	Revision 6.1  1996/11/23 21:55:10  nevin
 *	Ohio Release
 *	
 * Revision 6.0  96/02/29  13:53:48  gdburns
 * Ohio Release
 * 
 * Revision 5.2  94/08/22  14:00:38  gdburns
 * Ohio Release
 * 
 * Revision 5.1  94/05/18  12:44:29  gdburns
 * Ohio Release
 * 
 * Revision 2.3  94/04/22  12:48:56  gdburns
 * Ohio Release
 * 
 * Revision 2.2.1.1  94/02/16  15:28:00  gdburns
 * Convert to internal process.
 * Give general facelift.
 * 
 * Revision 2.2  92/04/30  14:45:50  trillium
 * Ohio Release
 * 
 * Revision 2.1  91/03/20  11:34:56  gdburns
 * Ohio Release
 *
 *	Function:	- loadd constants and templates
 */

#ifndef _LREQ
#define _LREQ

#include <lam_config.h>
#include <portable.h>

/*
 * maximum filename size
 */
#define FNAMELEN	160

/*
 * loadd request codes
 */
#define LQLOAD		1

/*
 * loadd request 
 */
struct lreq {
	int4	lq_req;			/* loadd request code */
	int4	lq_src_node;		/* client node */
	int4	lq_src_event;		/* client event */
        int4	lq_dest_node;		/* file's node destination */
	int4	lq_tag;			/* flatd tag */
};

/*
 * loadd reply
 */
struct lreply {
        int4	lr_reply;
};

#ifdef __cplusplus
extern "C" {
#endif

extern int4		rload __ARGS((int4 src_node, int4 dest_node,
					int4 tag, const char *filename));

#ifdef __cplusplus
}
#endif

#endif
