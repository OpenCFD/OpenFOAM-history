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
 * $Id: deallocate.c,v 1.3 2003/07/05 21:45:17 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <lamnet.h>
#include <boot.h>


/*
 * Common utility function for boot SSI modules.
 *
 * Deallocate nodes and free the malloc'ed reosurces
 */
void
lam_deallocate_nodes(struct lamnode **nodes, int *nnodes)
{
  int i;
  struct lamnode *n;
  ELEM *listelem;

  /* Free the struct lamnodes */
  
  if (nodes != NULL && *nodes != NULL) {
    n = *nodes;
    for (i = 0; i < *nnodes; ++i) {
      if (n[i].lnd_nodeid != NOTNODEID && n[i].lnd_hname != NULL)
	free(n[i].lnd_hname);
      if (n[i].lnd_ssi != NULL) {
	free(n[i].lnd_ssi);
      }
      if (n[i].lnd_uname != NULL) {
	free(n[i].lnd_uname);
      }
      if (n[i].lnd_keyval != NULL) {
	for (listelem = al_top(n[i].lnd_keyval); listelem != NULL;
	     listelem = al_next(n[i].lnd_keyval, listelem)) {
	  if (listelem->key != NULL)
	    free(listelem->key);
	  
	  if (listelem->value != NULL)
	    free(listelem->value);
	}
	al_free(n[i].lnd_keyval);
      }
    }
    free(*nodes);
    *nodes = NULL;
    *nnodes = 0;
  }
}

