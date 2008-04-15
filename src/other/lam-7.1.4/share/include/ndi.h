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
 *	$Id: ndi.h,v 6.5 2002/10/09 20:57:25 brbarret Exp $
 * 
 *	Function:	- node identifier parsing
 */

#ifndef _NDI_H
#define _NDI_H

#include <lam_config.h>
#include "all_list.h"
#include "lam.h"

/*
 * node identifier list entry
 */
struct ndi {
	int4		ndi_node;	/* ID itself */
	int4		ndi_flags;	/* descriptive flags */
	int4		ndi_count;	/* # IDs actually represented */
	int4		ndi_index;	/* position in list (legacy) */
};

#ifdef __cplusplus
extern "C" {
#endif

extern LIST *	ndi_parse __ARGS((int argc, char ** argv, char *** othersv));
extern LIST *	ndi_resolve __ARGS((LIST * nodelist));
extern void	ndi_fmt __ARGS((int4 node, char * fmt_str));
extern int4	ndi_parse1 __ARGS((char * s));
extern void	ndi_usage __ARGS((void));

#ifdef __cplusplus
}
#endif

#endif	/* _NDI_H */
