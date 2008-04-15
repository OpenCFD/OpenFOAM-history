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
 * $Id: ssi_boot_alg_linear_windowed.c,v 1.4 2003/07/11 19:33:43 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#include <sfh.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>


/*
 * local variables
 */
static const char *prefix = "base:linear_windowed:";


/*
 * Common utility function for boot SSI modules.
 *
 * Boot using a linear algorithm.
 */
int
lam_ssi_boot_base_alg_linear_windowed(struct lamnode *nodes, int nnodes, 
                                      int want_startup_protocol,
                                      lam_ssi_boot_proc_t which,
                                      int *num_started)
{
  int i, num_left, num_booted, num_to_boot;
  int ret = 0;
  int wsize;
  int failed = 0;

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "%s starting", prefix);

  /* If we're not doing the startup protocol, just fall through to
     normal linear, because there's no point in a windowed
     algorithm. */

  if (want_startup_protocol == 0) {
    if (lam_ssi_boot_verbose >= 1) {
      lam_debug(lam_ssi_boot_did, "%s no startup protocol", prefix);
      lam_debug(lam_ssi_boot_did, "%s invoking linear", prefix);
    }
    return lam_ssi_boot_base_alg_linear(nodes, nnodes, 0, which, num_started);
  }

  /* See if there is a user override of the window size */

  *num_started = 0;
  wsize = lam_ssi_base_param_lookup_int(lam_ssi_boot_base_param_window_size);
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "%s window size: %d", prefix, wsize);

  /* Open a server listening channel */

  if (lam_ssi_boot.lsba_open_srv_connection(nodes, wsize) != 0)
    return 1;

  /* Main algorithm */
  /* This is a windowed algorithm.  The main idea is to boot all the
     nodes, but not wait for callbacks from them until we've got
     (wsize) pending lamds.  Specifically, don't start waiting for
     lamd info until we have launched the first (wsize) lamds. */

  num_left = nnodes;
  for (num_to_boot = num_booted = i = 0; i < nnodes; ++i) {

    /* Skip invalid nodes */

    if (nodes[i].lnd_nodeid == NOTNODEID) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did,
                  "%s skipping n%d; invalid node", prefix, i);
      continue;
    }

    /* Only boot notes with type NT_BOOT */

    else if ((nodes[i].lnd_type & NT_BOOT) == 0) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did,
		  "%s skipping n%d (%s); not bootable",
		  prefix, i, nodes[i].lnd_hname);
      continue;
    }
    ++num_to_boot;

    /* Start the process */

    if (lam_ssi_boot_verbose >= 0)
      lam_debug(lam_ssi_boot_did, "%s booting n%d (%s)", prefix,
		i, nodes[i].lnd_hname);

    if ((ret = lam_ssi_boot.lsba_start_rte_proc(&nodes[i], which)) != 0) {
      lam_debug(lam_ssi_boot_did,
		"%s Failed to boot n%d (%s)\n", prefix,
		i, nodes[i].lnd_hname);
      failed = 1;
      break;
    }

    /* If we've booted wsize processes, wait for a connection from any
       newly-booted program and receive its lamd info. */

    ++num_booted;
    if (num_booted >= wsize) {
      --num_left;
      if ((ret = lam_ssi_boot.lsba_receive_lamd_info(nodes, nnodes)) != 0) {
        failed = 1;
        break;
      }
    }

    /* Nope, this one successfully started */

    ++(*num_started);
  }
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "%s finished launching", prefix);

  /* Finish receiving all the lamd information (i.e., drain the
     window) */

  if (failed == 0) {
    for (i = 0; i < num_left; ++i) {
      if ((ret = lam_ssi_boot.lsba_receive_lamd_info(nodes, nnodes)) != 0) {
        failed = 1;
        break;
      }
    }
  }

  /* Close the server listening channel and broadcast out the universe
     info. */

  if (lam_ssi_boot.lsba_close_srv_connection() != 0)
    ret = 1;

  /* Only finish the startup protocol if the above stuff was
     successful */

  if (failed == 0) {
    for (i = 0; i < nnodes; ++i) {

      /* Skip nodes that are invalid or already booted */

      if ((nodes[i].lnd_nodeid == NOTNODEID) ||
	  (nodes[i].lnd_type & NT_BOOT) == 0)
	continue;

      if (lam_ssi_boot.lsba_send_universe_info(nodes, nnodes, i) != 0)
	ret = 1;

      /* This one successfully booted */

      nodes[i].lnd_type &= ~NT_BOOT;
    }
  }

  /* All done */

  if (lam_ssi_boot_verbose >= 1) {
    if (ret == 0)
      lam_debug(lam_ssi_boot_did, "%s finished", prefix);
    else
      lam_debug(lam_ssi_boot_did, "%s aborted!", prefix);
  }

  return ret;
}
