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
 *      $Id: ereq.h,v 6.5 2002/10/09 20:57:23 brbarret Exp $
 *	Function:	- defines constants and structures for use
 *			  with the echo daemon
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell Research
 *			  Foundation
 */

#ifndef _EREQ
#define _EREQ

#include <lam_config.h>
#include <portable.h>
#include <net.h>

struct ereq {
	int4		eq_node;	/* source node */
	int4		eq_event;	/* source event */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int4		recho __ARGS((int4 node, const char *sm,
						char *dm, int4 len));

#ifdef __cplusplus
}
#endif

#endif
