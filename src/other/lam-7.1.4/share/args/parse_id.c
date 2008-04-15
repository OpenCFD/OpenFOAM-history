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
 * $Id: parse_id.c,v 6.12 2003/03/19 19:15:36 jsquyres Exp $
 *
 *	Function:	- identifier parsing package
 *			- id_range() based in part on Trollius 2.0,
 *			  Copyright 1990 The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <rreq.h>
#include <terror.h>
#include <typical.h>
#include <args.h>

#define NEW		1
#define ONE		2
#define MANY		3

/*
 * local variables
 */
static LIST		*nodelist = 0;	/* node ID list */
static LIST		*proclist = 0;	/* process ID list */
static char		fmt_str[16];	/* more than 16 long ain't mnemonic! */

struct id {
	int4		id_value;	/* ID itself */
	int4		id_flags;	/* descriptive flags */
	int4		id_count;	/* # IDs actually represented */
	int4		id_index;	/* position in list (legacy) */
};

/*
 * local functions
 */
static int nid_cast(int4 type, int4 mask, int is_node, int *ixp);
static int id_range(char *s, int4 flags, int is_node, int *ixp, LIST *lp);
static void id_get (int4 *ixp, int4 *idp, int4* flp, LIST *lp);
static int resolve_cpu(int cpuid);

/*
 *	nid_parse
 *
 *	Functions:	- parses node identifier mnemonics
 *			- rewrites arg vector with non-identifier args
 *			- argc, argv are undefined in case of ERROR
 *	Accepts:	- command line arguments
 *	Returns:	- 0 or ERROR
 */
int
nid_parse(int *argc, char **argv)
{
	int		index = 0;		/* current index */
	int		othersc;		/* left over count */
	char		**othersv;		/* left over arguments */
	struct id	newid;			/* new list element */	
/*
 * Initialize the list.
 */
	if (nodelist == 0) {
		nodelist = al_init(sizeof(struct id), 0);
		if (nodelist == 0) return(LAMERROR);
	}
/*
 * Pass argument 0 through.
 */
	othersc = 1;
	othersv = argv++;
	++othersv;
	--*argc;
/*
 * Loop through all the arguments.
 */
	while (*argc) {
/*
 * Is it a generic node identifier?
 */
		if ((**argv == 'n') && isdigit((int) *(*argv + 1))) {

			if (id_range(*argv + 1, 0, 1, &index, nodelist))
					return(LAMERROR);
		}
/*
 * Is it a generic CPU identifier?
 */
		else if ((**argv == 'c') && isdigit((int) *((*argv + 1)))) {

			if (id_range(*argv + 1, 0, 0, &index, nodelist)) {
					return(LAMERROR);
			}
		}
/*
 * Is it the local host identifier (h)?
 */
		else if ((**argv == 'h') && (*(*argv + 1) == '\0')) {
			newid.id_value = LOCAL;
			newid.id_flags = NT_ME;
			newid.id_count = 1;
			newid.id_index = index++;

			if (al_append(nodelist, &newid) == 0) return(LAMERROR);
		}
/*
 * Is it the origin host identifier (o)?
 */
		else if ((**argv == 'o') && (*(*argv + 1) == '\0')) {
			newid.id_value = getorigin();
			newid.id_flags = NT_ORIGIN;
			newid.id_count = 1;
			newid.id_index = index++;

			if (al_append(nodelist, &newid) == 0) return(LAMERROR);
		}
/*
 * Is it a multicast destination?
 */
/*
 * The "C" notation has been redfined from its original OTB definition
 * -- it now means "one per CPU".
 */
		else if ((**argv == 'C') && (*(*argv + 1) == '\0')) {

			if (nid_cast(0, NT_WASTE | NT_CAST, 0, &index))
					return(LAMERROR);
		}

		else if ((**argv == 'N') && (*(*argv + 1) == '\0')) {

			if (nid_cast(0, NT_WASTE | NT_CAST, 1, &index)) 
			  return(LAMERROR);
		}
/*
 * Bump the non-nodeid pointer and counter.
 */
		else {
			*(othersv++) = *argv;
			++othersc;
		}

		--*argc;
		++argv;
	}

	*argc = othersc;
	return (0);
}

/*
 *	pid_parse
 *
 *	Functions:	- parses process identifier mnemonics
 *			- rewrites arg vector with non-identifier args
 *			- argc, argv are undefined in case of ERROR
 *	Accepts:	- command line arguments
 *	Returns:	- 0 or ERROR
 */
int
pid_parse(int *argc, char **argv)
{
	int		index = 0;		/* current index */
	int		othersc;		/* left over count */
	char		**othersv;		/* left over arguments */
/*
 * Initialize the list.
 */
	if (proclist == 0) {
		proclist = al_init(sizeof(struct id), 0);
		if (proclist == 0) return(LAMERROR);
	}
/*
 * Pass argument 0 through.
 */
	othersc = 1;
	othersv = argv++;
	++othersv;
	--*argc;
/*
 * Loop through all the arguments.
 */
	while (*argc) {
/*
 * Is it a process identifier?
 */
		if ((**argv == 'p') && isdigit((int) *(*argv + 1))) {

			if (id_range(*argv + 1, SELECT_PID, 1, &index, 
				     proclist))
					return(LAMERROR);
		}
/*
 * Is it an index identifier?
 */
		else if (((**argv == '%') || (**argv == 'i')) &&
				isdigit((int) *(argv[0] + 1))) {

			if (id_range(*argv + 1, SELECT_INDEX, 1, &index,
					proclist)) return(LAMERROR);
		}
/*
 * Bump the non-id pointer and counter.
 */
		else {
			*(othersv++) = *argv;
			++othersc;
		}

		--*argc;
		++argv;
	}

	*argc = othersc;
	return (0);
}

/*
 *	nid_free
 *
 *	Function:	- frees the node ID list
 */
void
nid_free()
{
	if (!nodelist) return;

	al_free(nodelist);
	nodelist = 0;
}

/*
 *	pid_free
 *
 *	Function:	- frees the process ID list
 */
void
pid_free()
{
	if (!proclist) return;

	al_free(proclist);
	proclist = 0;
}

/*
 *	nid_get
 *
 *	Function:	- gets next node identifier
 *	Accepts:	- value returned index ptr
 *			  (-1 is returned if empty list)
 *			- value returned identifier ptr
 *			- value returned flags ptr
 */
void
nid_get (int4 *ixp, int4 *idp, int4 *flp)
{
	id_get(ixp, idp, flp, nodelist);
}

/*
 *	pid_get
 *
 *	Function:	- gets next process identifier
 *	Accepts:	- value returned index ptr
 *			  (-1 is returned if empty list)
 *			- value returned identifier ptr
 *			- value returned flags ptr
 */
void
pid_get (int4 *ixp, int4 *idp, int4 *flp)
{
	id_get(ixp, idp, flp, proclist);
}

/*
 *	nid_count
 *
 *	Returns:	- count of current node identifier
 */
int4
nid_count()
{
	struct id	*p;

	if (nodelist == 0) {
		return(0);
	} else {
		p = (struct id *) al_top(nodelist);
		return(p ? p->id_count : 0);
	}
}

/*
 *	nid_fmt
 *
 *	Function:	- translates a node identifier into a mnemonic string
 *	Accepts:	- node identifier
 *	Returns:	- mnemonic string
 */
char *
nid_fmt(int4 node)
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
	} else if (node == getorigin()) {
		sprintf(fmt_str, "n%d (o)", node);
	} else {
		sprintf(fmt_str, "n%d", node);
	}

	return(fmt_str);
}

/*
 *	nid_parse1
 *
 *	Function:	- parses one node identifier
 *	Accepts:	- mnemonic string
 *	Returns:	- node identifier
 */
int4
nid_parse1(char *s)
{
	int4		node;

	if (strcmp(s, "h") == 0) {
		node = LOCAL;
	} else if (strcmp(s, "o") == 0) {
		node = getorigin();
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
 *	nid_usage
 *
 *	Function:	- prints a nodespec usage message
 */
void
nid_usage()
{
	printf("\nNodes:		n<list>, eg., n0-3,5,0xa,12-15\n");
	printf("\nCPUs:			c<list>, eg., c0-3,5,0xa,12-15\n");
}

/*
 *	nid_cast
 *
 *	Function:	- establish a cast of nodes
 *			- LAM version
 *			- Trollius version registers with castd
 *	Accepts:	- node type
 *			- node mask
 *			- whether CPUs or nodes
 *			- current index ptr
 *	Returns:	- 0 or ERROR
 */
static int
nid_cast(int4 type, int4 mask, int is_node, int *ixp)
{
	int4		nnodes;			/* node array size */
	int4		*pnodes;		/* node array */
	int		i, j;
	struct id	newid;			/* new list element */	
	struct route	r;			/* route entry */

	nnodes = getntype(type, mask);
	if (nnodes < 0) return(LAMERROR);
	if (nnodes == 0) return(0);

	pnodes = (int4 *) malloc((unsigned) (nnodes * sizeof(int4)));
	if (pnodes == 0) return(LAMERROR);

	if (getnodes(pnodes, nnodes, type, mask)) return(LAMERROR);

	for (i = 0; i < nnodes; ++i) {
		newid.id_value = pnodes[i];
		newid.id_flags = 0;
		newid.id_count = 1;
		newid.id_index = (*ixp)++;
		if (al_append(nodelist, &newid) == 0) 
		  return(LAMERROR);
		if (!is_node) {
		  r.r_nodeid = pnodes[i];
		  getrent(&r);
		  for (j = 1; j < r.r_ncpus; j++) {
		    newid.id_index = (*ixp)++;
		    if (al_append(nodelist, &newid) == 0) 
		      return(LAMERROR);
		  }
		}
	}

	return(0);
}

/*
 *	id_get
 *
 *	Function:	- gets next identifier from given list
 *			- rotates it to bottom of list
 *	Accepts:	- value returned index ptr
 *			  (-1 is returned if empty list)
 *			- value returned identifier ptr
 *			- value returned flags ptr
 *			- list descriptor ptr
 */
static void
id_get (int4 *ixp, int4 *idp, int4* flp, LIST *lp)
{
	struct id	*p;
	struct id	newid;

	if (nodelist == 0) {
		*ixp = -1;
	} else {
		p = (struct id *) al_top(lp);

		if (p == 0) {
			*ixp = -1;
		} else {
			*ixp = p->id_index;
			*idp = p->id_value;
			*flp = p->id_flags;

			memcpy((char *) &newid, (char *) p, sizeof(newid));
			al_delete(lp, p);
			al_append(lp, &newid);
		}
	}
}

/*
 *	id_range
 *
 *	Function:	- parses the ID range format
 *			- example: 1,3-5,7
 *	Accepts:	- range string
 *			- flags to use with every value found
 *			- current index ptr
 *			- list descriptor ptr
 *	Returns:	- 0 or ERROR
 */
static int 
id_range(char *s, int4 flags, int is_node, int *ixp, LIST *lp)
{
	int		c;			/* current character */
	int4		id;			/* current identifier */
	int4		oldid = 0;		/* previous identifier */
	int		mode;			/* parsing state */
	struct id	newid;			/* new list entry */

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

			if (xtoi(&s, &id)) {

				if (errno == EBADASCIINUMB) {
					errno = EBADIDSPEC;
				}

				return(LAMERROR);
			}

			if (mode == NEW) {
				if (is_node)
				  newid.id_value = id;
				else
				  newid.id_value = resolve_cpu(id);
				newid.id_flags = flags;
				newid.id_count = 1;
				newid.id_index = (*ixp)++;

				if (al_append(lp, &newid) == 0) 
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
					  newid.id_value = oldid;
					else
					  newid.id_value = resolve_cpu(oldid);
					newid.id_flags = flags;
					newid.id_count = 1;
					newid.id_index = (*ixp)++;

					if (al_append(lp, &newid) == 0)
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

#if 0
/*
 * backwards compatibility
 */
/* Down with internal backwards compatability... */
int parse_nodes(int *ac, char **av) { return(nid_parse(ac, av)); }
int parse_procs(ac, av) int *ac; char **av; { return(pid_parse(ac, av)); }
void getnode(ixp, idp, flp) int4 *ixp, *idp, *flp; { nid_get(ixp, idp, flp); }
void getproc(ixp, idp, flp) int4 *ixp, *idp, *flp; { pid_get(ixp, idp, flp); }
char *mnemonic(node) int4 node; { return(nid_fmt(node)); }
int mnnodeid(s) char *s; { return(nid_parse1(s)); }
void mnusage() { nid_usage(); }
#endif
