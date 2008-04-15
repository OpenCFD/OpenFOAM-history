/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_boot_client.c,v 1.10 2003/12/19 15:20:25 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <args.h>
#include <etc_misc.h>
#include <lam-ssi-boot.h>


int
lam_ssi_boot_base_get_nodeid(OPT *aod)
{
  int node_id;

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, 
              "send_lamd: getting node ID from command line");
  if (ao_taken(aod, "n")) {
    ao_intparam(aod, "n", 0, 0, &node_id);
  } else {
    show_help("ssi-boot", "base-startup-proto-missing-args", NULL);
    errno = EUSAGE;
    return 1;
  }

  return node_id;
}
