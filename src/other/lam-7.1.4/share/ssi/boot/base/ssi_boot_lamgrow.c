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
 * $Id: ssi_boot_lamgrow.c,v 1.7 2004/02/04 03:06:21 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>



#include <lam-ssi-boot.h>
#include <rreq.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <dl_inet.h>


/*
 * Common utility function for boot SSI modules.
 *
 * Make an array of lamode's based on the universe that already exists
 */
char *
lam_ssi_boot_base_lamgrow(char *hostname, struct lamnode **nodes,
                          int *nnodes, int *origin)
{
  int i;
  int nlinks;
  int newnodeid = *origin;
  struct lamnode *n;
  struct route r;
  struct dolink *links;
  struct hostent *h;
  char hostip[60];
  

  /* Obtain neighbour link information from local dlo. */

  if (ldogetlinks(&links, &nlinks)) {
    show_help(NULL, "lib-call-fail", "ldogetlinks", NULL);
    kexit(1);
  }

  /* Special case: if nlinks == 1 and links[0] == 127.0.0.1, bail */

  if (nlinks == 1 && strcmp("127.0.0.1", 
                            inet_ntoa(links[0].dol_addr.sin_addr)) == 0) {
    show_help("lamgrow", "localhost-universe", NULL);
    kexit(1);
  }

  /* If a specific new node ID was not requested, then make it one
     beyond the last link */

  if (newnodeid == -1)
    newnodeid = nlinks;

  /* Sanity check */

  if ((newnodeid < 0) || ((newnodeid < nlinks) &&
			  (links[newnodeid].dol_link != NOTLINKID))) {
    show_help("lamgrow", "bad-node-id", NULL);
    kexit(1);
  }
  if (lam_ssi_boot_verbose > 0)
    lam_debug(lam_ssi_boot_did, " adding node n%d", newnodeid);

  /* Allocate node description array. */

  if (newnodeid > (nlinks - 1))
    *nnodes = newnodeid + 1;
  else
    *nnodes = nlinks;
  n = *nodes = malloc(*nnodes * sizeof(struct lamnode));
  if (*nnodes == 0) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    kexit(errno);
  }

  /* VPS: Get the IP address of the new node */
    
  if ((h = gethostbyname(hostname)) == NULL) {
    show_help("lamgrow", "unknown-host", hostname);
    kexit(1);
  }
  
  strcpy(hostip, inet_ntoa(*((struct in_addr *)h->h_addr)));

  /* Fill it in with all the other nodes that already exist.  Mark
     sure to mark invalid nodes with NOTNODEID. */

  for (i = 0; i < nlinks; ++i) {
    n[i].lnd_uname = 0;
    n[i].lnd_ssi = 0;
    n[i].lnd_keyval = 0;
    if (links[i].dol_link == NOTLINKID) {
      n[i].lnd_nodeid = NOTNODEID;
      n[i].lnd_type = 0;
      n[i].lnd_hname = 0;
      if (lam_ssi_boot_verbose >= 10)
        lam_debug(lam_ssi_boot_did, " found empty node n%d", i);
    }
    else {
      r.r_nodeid = i;
      getrent(&r);

      n[i].lnd_nodeid = i;
      n[i].lnd_type = r.r_nodetype;
      n[i].lnd_addr = links[i].dol_addr;
      n[i].lnd_ncpus = r.r_ncpus;
      n[i].lnd_hname = malloc(32);
      strcpy(n[i].lnd_hname, inet_ntoa(links[i].dol_addr.sin_addr));

      /* VPS: Check if the node is already in the lamnodes */
      
      if (strcmp(n[i].lnd_hname, hostip) == 0) {
	show_help("lamgrow", "host-existing", hostname);
	kexit(1);
      }
      
      if (lam_ssi_boot_verbose >= 10)
        lam_debug(lam_ssi_boot_did, " found existing n%d: %s, cpu=%d",
                  i, n[i].lnd_hname, r.r_ncpus);
    }
  }

  /* Fill in any "empty buffer" entries with NOTNODEID.  e.g., if we
     had a universe of size X, and the user specified "-n X+3" on the
     command line, then there would be 2 empty slots. */

  for (i = nlinks; i < *nnodes; ++i) {
    n[i].lnd_nodeid = NOTNODEID;
    n[i].lnd_type = 0;
    if (lam_ssi_boot_verbose >= 10)
      lam_debug(lam_ssi_boot_did, " creating empty node n%d", i);
  }

  /* Fill in some flags */

  n[getnodeid()].lnd_type |= NT_ME;
  if (getorigin() != NOTNODEID)
    n[getorigin()].lnd_type |= NT_ORIGIN;

  /* Fill in the info for this node.  Note that putting hostname in
     here means that it will be free()'ed later. */

  n[newnodeid].lnd_nodeid = newnodeid;
  n[newnodeid].lnd_type = NT_BOOT;
  n[newnodeid].lnd_hname = hostname;
  n[newnodeid].lnd_ssi = NULL;
  n[newnodeid].lnd_keyval = 0;
  n[newnodeid].lnd_uname = 0;
  n[newnodeid].lnd_ncpus = 1;
  n[newnodeid].lnd_bootport = 0;
  if (lam_ssi_boot_verbose >= 10)
    lam_debug(lam_ssi_boot_did, " filled n%d data", newnodeid);

  /* All done */

  free(links);
  return 0;
}
