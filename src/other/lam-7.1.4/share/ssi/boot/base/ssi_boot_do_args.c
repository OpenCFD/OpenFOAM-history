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
 * $Id: ssi_boot_do_args.c,v 1.9 2003/05/08 19:06:32 brbarret Exp $
 *
 */

#include <lam_config.h>
#include <lam-ssi-boot.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <args.h>
#include <etc_misc.h>
#include <laminternal.h>
#include <sfh.h>
#include <lamdebug.h>


/*
 * Args common to all things we might start
 */
int
lam_ssi_boot_do_common_args(OPT *aod, int *argc, char ***argv)
{
  /* Verbose and debug flags */
  
  if (ao_taken(aod, "d"))
    sfh_argv_add(argc, argv, "-d");
  if (ao_taken(aod, "v"))
    sfh_argv_add(argc, argv, "-v");

  return 0;
}


/*
 * build the inet_topo string used by hboot and friends
 */
char *
lam_ssi_boot_build_inet_topo(struct lamnode *dest_node, 
			     struct lamnode origin_node,
			     int origin)
{
  char *buf;
  char *addr;
  char address_buffer[BUFSIZ];
  unsigned char *p;

  buf = (char *) malloc(BUFSIZ);
  if (buf == NULL) {
    return buf;
  }

  /* Suggestion/modified patch from Tim Mattox (KLAT group) to pass
     around host *names* instead of IP addresses.  Necessary for
     non-uniform networks (e.g., KLAT flat networks) where IP addr
     lookup for name "foo" may result in different answers on
     different nodes.  Hence, if the "-l" option was given to
     lamboot, use the name.  Otherwise, make a string with the
     address and use that. */

  if (ao_taken(lam_ssi_boot_optd, "l")) {
    addr = origin_node.lnd_hname;
  } else {
    p = (unsigned char *) &(origin_node.lnd_addr.sin_addr);
    snprintf(address_buffer, BUFSIZ, "%u.%u.%u.%u", 
	     p[0], p[1], p[2], p[3]);
    addr = address_buffer;
  }
    
  snprintf(buf, BUFSIZ, "%s-H %s -P %d -n %d -o %d",
	   ao_taken(lam_ssi_boot_optd, "x") ? "-x " : "",
	   addr,
	   lam_ssi_boot_base_server_port,
	   dest_node->lnd_nodeid,
	   origin);

  return buf;
}
