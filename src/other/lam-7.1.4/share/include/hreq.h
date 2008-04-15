/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 *	Function:	- defines constants and structures for use
 *			  with the halt daemon
 */

#ifndef _HREQ_H_
#define _HREQ_H_

#include <lam_config.h>
#include <portable.h>
#include <net.h>

struct hreq {
  int4 hq_node;  /* source node */
  int4 hq_event; /* ack/reply event */
  int4 hq_index; /* reply index */
  int4 hq_pid; /* client PID */
};

typedef enum {
  LAM_HALT_PING = 1,
  LAM_HALT_DIE
} halt_types;


/*
 * Function to shut down a set of LAM daemons
 */

int rhreq(int *nodes, int nnodes, int timeout, int verbose);


#endif
