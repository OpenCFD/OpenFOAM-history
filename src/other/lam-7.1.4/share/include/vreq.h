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
 *      $Id: vreq.h,v 1.3 2003/03/18 04:03:34 jsquyres Exp $
 *	Function:	- defines constants and structures for use
 *			  with the version daemon
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell Research
 *			  Foundation
 */

#ifndef LAM_LAMD_VREQ
#define LAM_LAMD_VREQ

#include <lam_config.h>
#include <portable.h>
#include <net.h>


/*
 * Send in the source node and event so that the versiond can send
 * back reply
 */

struct vreq {
  int4 vq_node;
  int4 vq_event;
};

struct vreply {
  int4 vrp_major;
  int4 vrp_minor;
  int4 vrp_release;
  int4 vrp_alpha;
  int4 vrp_beta;
};

#ifdef __cplusplus
extern "C" {
#endif

  extern int4 rversion(char **version_string, int *major, int *minor,
		       int *release, int *alpha, int *beta);

#ifdef __cplusplus
}
#endif

#endif
