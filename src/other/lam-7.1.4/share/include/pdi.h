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
 *	$Id: pdi.h,v 6.5 2002/10/09 20:57:25 brbarret Exp $
 * 
 *	Function:	- process identifier parsing
 */

#ifndef _PDI_H
#define _PDI_H

#include <lam_config.h>
#include <all_list.h>
#include <portable.h>


/*
 * node identifier list entry
 */
struct pdi {
	int4		pdi_node;	/* ID itself */
	int4		pdi_flags;	/* descriptive flags */
	int4		pdi_count;	/* # IDs actually represented */
	int4		pdi_index;	/* position in list (legacy) */
};

#ifdef __cplusplus
extern "C" {
#endif

extern LIST *	pdi_parse __ARGS((int argc, char ** argv, char *** othersv));

#ifdef __cplusplus
}
#endif

#endif	/* _PDI_H */
