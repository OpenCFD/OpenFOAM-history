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
 * $Id: ssi_boot_alg_thread.c,v 1.2 2003/02/08 17:50:23 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sfh.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>


/*
 * Common utility function for boot SSI modules.
 *
 * Boot using a threaded algorithm.
 */
int
lam_ssi_boot_base_alg_thread(struct lamnode *nodes, int nnodes, 
			     int want_startup_protocol,
			     lam_ssi_boot_proc_t which,
			     int *num_started)
{
  *num_started = 0;
  show_help("ssi-boot", "alg-not-implemented", "thread", NULL);

  /* JMS: Notes for future implementation:

     - Will need to address "toronto" issue of promiscuous
       connections.  Specifically, using an environment variable to
       specify the run-time behavior of promiscuous connections may
       not be suitable everywhere (e.g., rsh when remote agents are
       used).  Perhaps use a command line argument?

     */

  return 1;
}
