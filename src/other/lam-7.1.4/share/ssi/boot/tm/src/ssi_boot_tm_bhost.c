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
 * $Id: ssi_boot_tm_bhost.c,v 1.7 2003/04/08 00:04:14 jsquyres Exp $
 *
 *	Function:	- create host list for tm interface
 */


#include <lam_config.h>
#include <lam-ssi-boot-tm-config.h>

#include <tm.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boot.h>
#include <lamnet.h>
#include <net.h>
#include <typical.h>
#include <etc_misc.h>
#include <sfh.h>
#include <lam-ssi-boot-tm.h>
#include <ssi_boot_tm_lamnode.h>
#include <lamdebug.h>


/*
 * constants
 */
#define LNDBLOCK	10	       /* array allocation block */
#define TMINFOLEN       1000

static char* ssi_boot_tm_gethostname(tm_node_id node);

/*
 *	parse_host
 *
 *	Function:	- parse a host file
 *			- fill the network description
 *			- node IDs are [0, N-1]
 *	Accepts:	- input file stream
 *			- node description array (value returned)
 *			- array size (value returned)
 */
int
lam_ssi_boot_tm_bhostparse(struct lamnode **plamnet, int *pnlamnet)
{
  tm_node_id *tm_nodes;        /* TM nodes */
  int tm_nnodes;                /* number of TM nodes */
  int ret;
  int i, j, found;
  char *hostname;

  struct lamnode *lamnet = 0;
  int nlndalloc = 0;
  int nnodes = 0;
  
  /* Get the number of TM "nodes", which we will parse through.  While
     TM calls them nodes, they are actually VCPUs, which means that we
     may have two "nodes" that are the same node, in the traditional
     sense of the word.  Argh. */
  ret = tm_nodeinfo(&tm_nodes, &tm_nnodes);
  if (ret != TM_SUCCESS) {
    return LAMERROR;
  }

  /* Loop over each "node", which may only be a CPU */
  for (i = 0 ; i < tm_nnodes ; ++i) {
    /* get hostname for this node */
    hostname = ssi_boot_tm_gethostname(tm_nodes[i]);
    
    /* if the hostname is already used,  ++ the cpu count */
    for (found = j = 0 ; j < nnodes ; ++j) {
      if (strcmp(hostname, lamnet[j].lnd_hname) == 0) {
	lamnet[j].lnd_ncpus++;
	found = 1;
	break;
      }
    }
    if (found) {
      /* free the hostname, which was strdup'ed below, since we aren't
         going to stick it in the lamnet struct */
      free(hostname);
      continue;
    }

    /* allocate / expand node desc array */
    if (lamnet == 0) {
      lamnet = (struct lamnode *) malloc((unsigned)
					 LNDBLOCK *
					 sizeof(struct lamnode));
      if (lamnet == 0) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	return LAMERROR;
      }
      nlndalloc = LNDBLOCK;
    }

    if (nlndalloc <= nnodes) {
      nlndalloc += LNDBLOCK;
      lamnet = (struct lamnode *) realloc((char *) lamnet,
					  (unsigned) nlndalloc *
					  sizeof(struct lamnode));
      if (lamnet == 0) {
	show_help(NULL, "system-call-fail", "realloc", NULL);
	return LAMERROR;
      }
    }

    lamnet[nnodes].lnd_nodeid = nnodes;
    lamnet[nnodes].lnd_type = NT_BOOT;
    lamnet[nnodes].lnd_ncpus = 1;
    lamnet[nnodes].lnd_bootport = 0;
    lamnet[nnodes].lnd_hname = hostname;
    lamnet[nnodes].lnd_uname = NULL;
    lamnet[nnodes].lnd_keyval = NULL;

    /* add our special data */
    lamnet[nnodes].lnd_ssi = (struct lnd_ssi_boot_nodeinfo*)
      malloc(sizeof(struct lnd_ssi_boot_nodeinfo));
    if (lamnet[nnodes].lnd_ssi == NULL) {
	show_help(NULL, "system-call-fail", "malloc", NULL);
	return LAMERROR;
    }
    lamnet[nnodes].lnd_ssi->tm_node_num = tm_nodes[i];

    nnodes++;
  }

  /* clean up */
  free(tm_nodes);

  *plamnet = lamnet;
  *pnlamnet = nnodes;
    
  return 0;
}


/* Get the hostname from the info keys provided by TM.  This could
   probably be made more async, but who cares? */
static char*
ssi_boot_tm_gethostname(tm_node_id node)
{
  char *hostname = NULL;
  tm_event_t info_event;        /* Event for getting info */
  int tm_errno;                 /* TM's special errno */
  char buf[TMINFOLEN + 1];
  char **parse_buf;
  int ret;

  /* Get the info string */
  ret = tm_rescinfo(node, buf, TMINFOLEN, &info_event);
  buf[TMINFOLEN] = '\0'; /* just in case */
  if (ret != TM_SUCCESS) {
    return NULL;
  }
  
  ret = tm_poll(TM_NULL_EVENT, &info_event, 1, &tm_errno);
  if (ret != TM_SUCCESS) {
    return NULL;
  }

  /* we naw have a space-separated string array.  Break it up. */
  parse_buf = sfh_argv_break(buf, ' ');
  if (parse_buf == NULL) {
    return NULL;
  }

  /* Accoring to TM manpage, hostname is the second thing in the
   * list */
  hostname = strdup(parse_buf[1]);

  sfh_argv_free(parse_buf);

  return hostname;
}
