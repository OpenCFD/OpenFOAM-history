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
 * $Id: ssi_boot_alg_linear.c,v 1.5 2003/03/24 03:13:17 jsquyres Exp $
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
 * Boot using a linear algorithm.
 */
int
lam_ssi_boot_base_alg_linear(struct lamnode *nodes, int nnodes, 
			     int want_startup_protocol,
			     lam_ssi_boot_proc_t which,
			     int *num_started)
{
  int i;
  int ret = 0;

  *num_started = 0;
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "base:linear: starting");

  /* If we want the startup protocol, open a server listening channel */

  if (want_startup_protocol == 1 && 
      lam_ssi_boot.lsba_open_srv_connection(nodes, nnodes) != 0)
    return 1;

  /* Main algorithm */

  for (i = 0; i < nnodes; ++i) {

    /* Skip invalid nodes */

    if (nodes[i].lnd_nodeid == NOTNODEID) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did,
		  "base:linear: skipping n%d; invalid node", i);
      continue;
    }

    /* Only boot notes with type NT_BOOT */

    else if ((nodes[i].lnd_type & NT_BOOT) == 0) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did,
		  "base:linear: skipping n%d (%s); not bootable",
		  i, nodes[i].lnd_hname);
      continue;
    }

    /* Start the process */

    if (lam_ssi_boot_verbose >= 0)
      lam_debug(lam_ssi_boot_did, "base:linear: booting n%d (%s)",
		i, nodes[i].lnd_hname);

    if ((ret = lam_ssi_boot.lsba_start_rte_proc(&nodes[i], which)) != 0) {
      lam_debug(lam_ssi_boot_did,
		"base:linear: Failed to boot n%d (%s)\n",
		i, nodes[i].lnd_hname);
      break;
    }

    /* If we want startup protocol, wait for a connection from the
       newly-booted program, and receive its lamd info. */

    if (want_startup_protocol == 1)
      if ((ret = lam_ssi_boot.lsba_receive_lamd_info(&nodes[i], 1)) != 0)
	break;

    /* Nope, this one successfully started */

    ++(*num_started);
  }

  /* If we want the startup protocol, close the server listening
     channel and broadcast out the universe info. */

  if (want_startup_protocol == 1)
    if (lam_ssi_boot.lsba_close_srv_connection() != 0)
      ret = 1;

  /* Only finish the startup protocol if the above stuff was
     successful */

  if (i >= nnodes && want_startup_protocol == 1) {
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

  if (lam_ssi_boot_verbose >= 0) {
    if (ret == 0)
      lam_debug(lam_ssi_boot_did, "base:linear: finished");
    else
      lam_debug(lam_ssi_boot_did, "base:linear: aborted!");
  }

  return ret;
}
