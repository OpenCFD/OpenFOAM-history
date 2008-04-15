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
 * $Id: ndi_parse.c,v 6.11 2003/07/05 21:44:33 jsquyres Exp $
 *
 *	Function:	- node identifier parsing package
 *			- id_range() based in part on Trollius 2.0,
 *			  Copyright 1990 The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <lam_config.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <args.h>
#include <ndi.h>
#include <rreq.h>
#include <lam.h>
#include <sfh.h>

#define NEW		1
#define ONE		2
#define MANY		3


/*
 * local functions
 */
static int id_range(char *s, int4 flags, int *ixp, int is_node, LIST *lp);
static int resolve_cpu(int cpuid);


/*
 *	ndi_parse
 *
 *	Functions:	- parses node identifier mnemonics
 *	Accepts:	- command line count
 *			- command line vector
 *			- leftover vector (returned)
 *	Returns:	- node list or null
 */
LIST *
ndi_parse(int argc, char **argv, char ***othersv)
{
	int		index = 0;		/* current index */
	int		othersc;		/* leftover count */
	struct ndi	newndi;			/* new list element */	
	LIST *		nodelist;		/* node ID list */
/*
 * Initialize the list.
 */
	nodelist = al_init(sizeof(struct ndi), 0);
	if (nodelist == 0) return(0);
/*
 * Pass argument 0 through.
 */
	if (othersv) {
		othersc = 0;
		*othersv = 0;

		if (sfh_argv_add(&othersc, othersv, argv[0])) {
			al_free(nodelist);
			return(0);
		}
	}

	argc--;
	argv++;
/*
 * Loop through all the arguments.
 */
	while (argc) {
/*
 * Is it a generic node identifier?
 */
		if ((**argv == 'n') && isdigit((int) *((*argv + 1)))) {
			if (id_range(*argv + 1, 0, &index, 1, nodelist)) {
				al_free(nodelist);
				if (othersv) sfh_argv_free(*othersv);
				return(0);
			}
		}
/*
 * Is it a generic CPU identifier?
 */
		else if ((**argv == 'c') && isdigit((int) *((*argv + 1)))) {
			if (id_range(*argv + 1, 0, &index, 0, nodelist)) {
				al_free(nodelist);
				if (othersv) sfh_argv_free(*othersv);
				return(0);
			}
		}
/*
 * Is it the local host identifier (h)?
 */
		else if ((**argv == 'h') && (*(*argv + 1) == '\0')) {
			newndi.ndi_node = LOCAL;
			newndi.ndi_flags = NT_ME;
			newndi.ndi_count = 1;
			newndi.ndi_index = index++;

			if (al_append(nodelist, &newndi) == 0) {
				al_free(nodelist);
				if (othersv) sfh_argv_free(*othersv);
				return(0);
			}
		}
/*
 * Is it a multicast destination?
 */
/*
 * The "C" notation has been redfined from its original OTB definition
 * -- it now means "one per CPU".
 */
		else if ((**argv == 'C') && (*(*argv + 1) == '\0')) {
			newndi.ndi_node = HOST2COMPCPU;
			newndi.ndi_flags = NT_CAST | NT_WASTE;
			newndi.ndi_count = -1;
			newndi.ndi_index = index++;

			if (al_append(nodelist, &newndi) == 0) {
				al_free(nodelist);
				if (othersv) sfh_argv_free(*othersv);
				return(0);
			}
		}

		else if ((**argv == 'N') && (*(*argv + 1) == '\0')) {
		        newndi.ndi_node = HOST2COMPNODE;
			newndi.ndi_flags = NT_CAST | NT_WASTE;
			newndi.ndi_count = -1;
			newndi.ndi_index = index++;

			if (al_append(nodelist, &newndi) == 0) {
				al_free(nodelist);
				if (othersv) sfh_argv_free(*othersv);
				return(0);
			}
		}
/*
 * Bump the non-nodeid pointer and counter.
 */
		else if (othersv) {

			if (sfh_argv_add(&othersc, othersv, argv[0])) {
				al_free(nodelist);
				sfh_argv_free(*othersv);
				return(0);
			}
		}

		argc--;
		argv++;
	}

	return(nodelist);
}


/*
 *	ndi_fmt
 *
 *	Function:	- translates a node identifier into a mnemonic string
 *	Accepts:	- node identifier
 *			- string buffer
 */
void
ndi_fmt(int4 node, char *fmt_str)
{
	if (node == NOTNODEID) {
		strcpy(fmt_str, "?");
	} else if (node == LOCAL) {
		strcpy(fmt_str, "local");
	} else if (node == HOST2COMPNODE) {
		strcpy(fmt_str, "N");
/*
 * The "C" notation has been redfined from its original OTB definition
 * -- it now means "one per CPU".
 */
	} else if (node == HOST2COMPCPU) {
		strcpy(fmt_str, "C");
	} else {
		sprintf(fmt_str, "n%d", node);
	}
}


/*
 *	ndi_parse1
 *
 *	Function:	- parses one node identifier
 *	Accepts:	- mnemonic string
 *	Returns:	- node identifier
 */
int4
ndi_parse1(char *s)
{
	int4		node;

	if (strcmp(s, "h") == 0) {
		node = LOCAL;
	} else if (strcmp(s, "local") == 0) {
		node = LOCAL;
	} else if (strcmp(s, "N") == 0) {
		node = HOST2COMPNODE;
/*
 * The "C" notation has been redfined from its original OTB definition
 * -- it now means "one per CPU".
 */
	} else if (strcmp(s, "C") == 0) {
		node = HOST2COMPCPU;
	}

	else if (sscanf(s, " n%d", &node) == 1) {
	}

	else if (sscanf(s, " c%d", &node) == 1) {
	}

	else {
		node = NOTNODEID;
	}

	return(node);
}


/*
 *	ndi_usage
 *
 *	Function:	- prints a nodespec usage message
 */
void
ndi_usage(void)
{
	printf("\nNodes:		n<list>, eg., n0-3,5,0xa,12-15\n");
	printf("\nCPUs:			c<list>, eg., c0-3,5,0xa,12-15\n");
}


/*
 *	id_range
 *
 *	Function:	- parses the ID range format
 *			- example: 1,3-5,7
 *	Accepts:	- range string
 *			- flags to use with every value found
 *			- current index ptr
 *			- flag specifying nodes or CPUs
 *			- list descriptor ptr
 *	Returns:	- 0 or ERROR
 */
static int
id_range(char *s, int4 flags, int *ixp, int is_node, LIST *lp)
{
	int		c;			/* current character */
	int4		id;			/* current identifier */
	int4		oldid = 0;		/* previous identifier */
	int		mode;			/* parsing state */
	struct ndi	newndi;			/* new list entry */
	char		*t;			/* bumped up s */

	mode = NEW;

	while ((c = *s) != '\0') {

		if (c == ',') {

			if (mode == NEW) {
				errno = EBADIDSPEC;
				return(LAMERROR);
			}

			++s;
			mode = NEW;
		}

		else if (c == '-') {

			if (mode != ONE) {
				errno = EBADIDSPEC;
				return(LAMERROR);
			}

			++s;
			mode = MANY;
		}

		else {
			id = strtol(s, &t, 0);

			if ((id == 0) && (s == t)) {
				errno = EBADIDSPEC;
				return(LAMERROR);
			}

			s = t;

			if (mode == NEW) {
				if (is_node)
				  newndi.ndi_node = id;
				else
				  newndi.ndi_node = resolve_cpu(id);
				newndi.ndi_flags = flags;
				newndi.ndi_count = 1;
				newndi.ndi_index = (*ixp)++;

				if (al_append(lp, &newndi) == 0) 
				  return(LAMERROR);

				oldid = id;
				mode = ONE;
			}

			else if (mode == MANY) {

				if (id < oldid) {
					errno = EBADIDSPEC;
					return(LAMERROR);
				}

				while (++oldid <= id) {
					if (is_node)
					  newndi.ndi_node = oldid;
					else
					  newndi.ndi_node = resolve_cpu(oldid);
					newndi.ndi_flags = flags;
					newndi.ndi_count = 1;
					newndi.ndi_index = (*ixp)++;

					if (al_append(lp, &newndi) == 0)
							return(LAMERROR);
				}
			}

			else {
				errno = EIMPOSSIBLE;
				return(LAMERROR);
			}
		}
	} /* while() */

	return (0);
}


/*
 *	resolve_cpu
 *
 *	Function:	- resolves a CPU id to a node ID
 *	Accepts:	- cpu ID
 *	Returns:	- nid or LAMERROR
 */
static int 
resolve_cpu(int cpuid)
{
  int i, ncpus;
  int nnodes;
  int *nodes;
  struct route *r;

/*  
 * Find out how many CPUs we have
 */
  nnodes = getntype(0, NT_CAST);
  nodes = (int *) malloc(sizeof(int) * nnodes);
  if (nodes == NULL)
    return (LAMERROR);

  r = (struct route*) malloc(sizeof(struct route) * nnodes);
  if (r == NULL)
    return (LAMERROR);
  getnodes(nodes, nnodes, 0, NT_CAST);
  for (ncpus = i = 0; i < nnodes; i++) {
    r[i].r_nodeid = nodes[i];
    getrent(&r[i]);
    ncpus += r[i].r_ncpus;
  }

  if (cpuid < 0 || cpuid >= ncpus) {
    free(r);
    free(nodes);
    return (LAMERROR);
  }
/*  
 * Map the CPU id to an nid
 */
  for (ncpus = i = 0; i < nnodes; i++) {
    if (cpuid < (ncpus + r[i].r_ncpus))
      break;
    ncpus += r[i].r_ncpus;
  }

  free(r);
  free(nodes);

  return i;
}
