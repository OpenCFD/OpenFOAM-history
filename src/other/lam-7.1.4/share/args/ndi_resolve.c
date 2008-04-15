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
 * $Id: ndi_resolve.c,v 6.7 2003/03/19 23:20:01 jsquyres Exp $
 *
 *	Function:	- node identifier resolution
 */

#include <stdlib.h>

#include <all_list.h>
#include <ndi.h>
#include <net.h>
#include <portable.h>
#include <typical.h>
#include <rreq.h>


/*
 * local functions
 */
static int
ndi_cast(LIST *nodelist, int4 type, int4 mask, int is_node, int *ixp);


/*
 *	ndi_resolve
 *
 *	Function:	- node identifier resolution
 *			- expands "special" node identifiers into
 *			  multiple specific node identifiers
 *	Accepts:	- node ID list
 *	Returns:	- resolved node ID list or null
 */
LIST *
ndi_resolve(LIST *nodelist)
{
	struct ndi	newnode;		/* new list element */	
	struct ndi *	p;			/* current list element */
	LIST *		newnodelist;		/* resolved node ID list */
	int4		index;			/* index counter */
/*
 * Initialize the new list.
 */
	newnodelist = al_init(sizeof(struct ndi), 0);
	if (newnodelist == 0) return(0);
/*
 * Resolve each entry in the given list.
 */
	index = 0;
	p = (struct ndi *) al_top(nodelist);

	while (p) {
		if (p->ndi_node == HOST2ALL) {
			if (ndi_cast(newnodelist, 0, NT_CAST, 1, &index)) {
				al_free(newnodelist);
				return(0);
			}
		} 
/*
 * The "C" notation has been redfined from its original OTB definition
 * -- it now means "one per CPU".
 */
		else if (p->ndi_node == HOST2COMPCPU) {
			if (ndi_cast(newnodelist, 0, NT_WASTE | NT_CAST, 0,
					&index)) {
				al_free(newnodelist);
				return(0);
			}
		} else if (p->ndi_node == HOST2COMPNODE) {
			if (ndi_cast(newnodelist, 0, NT_WASTE | NT_CAST, 1,
					&index)) {
				al_free(newnodelist);
				return(0);
			}
		} else {
			newnode.ndi_node = p->ndi_node;
			newnode.ndi_flags = p->ndi_flags;
			newnode.ndi_count = p->ndi_count;
			newnode.ndi_index = index++;

			if (al_append(newnodelist, &newnode) == 0) {
				al_free(newnodelist);
				return(0);
			}
		}

		p = (struct ndi *) al_next(nodelist, p);
	}

	return(newnodelist);
}


/*
 *	ndi_cast
 *
 *	Function:	- expands a cast of nodes
 *	Accepts:	- node type
 *			- node mask
 *			- whether CPUs or nodes
 *			- current index ptr
 *	Returns:	- 0 or ERROR
 */
static int
ndi_cast(LIST *nodelist, int4 type, int4 mask, int is_node, int *ixp)
{
	int4		nnodes;			/* node array size */
	int4		*pnodes;		/* node array */
	int		i, j;
	struct ndi	newnode;		/* new list element */	
	struct route	r;			/* route entry */

	nnodes = getntype(type, mask);
	if (nnodes < 0) return(LAMERROR);
	if (nnodes == 0) return(0);

	pnodes = (int4 *) malloc((unsigned) (nnodes * sizeof(int4)));
	if (pnodes == 0) return(LAMERROR);

	if (getnodes(pnodes, nnodes, type, mask)) {
	  free((char *) pnodes);
	  return(LAMERROR);
	}

	for (i = 0; i < nnodes; ++i) {
	  newnode.ndi_node = pnodes[i];
	  newnode.ndi_flags = 0;
	  newnode.ndi_count = 1;
	  newnode.ndi_index = (*ixp)++;
	  if (al_append(nodelist, &newnode) == 0) {
	    free((char *) pnodes);
	    return(LAMERROR);
	  }
	  if (!is_node) {
	    r.r_nodeid = pnodes[i];
	    getrent(&r);
	    for (j = 1; j < r.r_ncpus; j++) {
	      if (al_append(nodelist, &newnode) == 0) {
		free((char *) pnodes);
		return(LAMERROR);
	      }
	    }
	  }
	  
	}

	free((char *) pnodes);
	return(0);
}
