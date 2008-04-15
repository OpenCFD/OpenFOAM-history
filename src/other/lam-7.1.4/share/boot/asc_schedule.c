/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	Ohio Trollius
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: asc_schedule.c,v 6.6 2002/10/09 20:56:58 brbarret Exp $
 *
 *	Function:	- schedules an application schema
 *			- LAM specific
 *			- generates a new application list with one
 *			  process, one node entries
 *			- four scheduling cases:
 *			  1) "foo -c # <nodes>"  # procs on this list of nodes
 *			  2) "foo -c #"          # procs using all nodes
 *			  3) "foo <nodes>"       1 proc on each of these nodes
 *			  4) "foo"               1 proc on every node
 *
 *	Accepts:	- parsed application schema
 *	Returns:	- expanded, precise application schema or NULL
 */

#include <all_list.h>
#include <app_schema.h>
#include <ndi.h>

LIST *
asc_schedule(applist)

LIST *			applist;

{
	LIST *		newapplist;		/* scheduled app schema */
	LIST *		newnodelist;		/* explicit node ids */
	LIST *		parsenodelist;		/* parsed nodes or default */
	struct aschema	newproc;		/* precise app process */
	struct aschema *p;			/* current app process */
	struct ndi *	node;			/* current node ID */
	char *		nodev[3];		/* default node spec */
	int		n;
/*
 * Expand each entry in the parsed schema.
 */
	p = (struct aschema *) al_top(applist);
	newapplist = al_init(sizeof(struct aschema), 0);

	while (p) {
/*
 * Absent node information is replaced by all nodes.
 */
		if (al_count(p->asc_nodelist) == 0) {
			nodev[0] = "cmd";
			nodev[1] = "C";
			nodev[2] = 0;

			parsenodelist = ndi_parse(2, nodev, 0);

			if (parsenodelist == 0) {
				al_free(newapplist);
				return(0);
			}
		} else {
			parsenodelist = p->asc_nodelist;
		}
/*
 * Expand the node list into plain node identifiers.
 */
		newnodelist = ndi_resolve(parsenodelist);

		if (al_count(p->asc_nodelist) == 0) {
			al_free(parsenodelist);
		}

		if (newnodelist == 0) {
			al_free(newapplist);
			return(0);
		}
/*
 * An absent process count means one process on each node.
 */
		n = (p->asc_proc_cnt < 1) ? al_count(newnodelist) :
				p->asc_proc_cnt;
/*
 * Generate a one process, one node application schema.
 */
		node = (struct ndi *) al_top(newnodelist);
		newproc.asc_errno = 0;
		newproc.asc_proc_cnt = 1;
		newproc.asc_args = p->asc_args;
		newproc.asc_env = p->asc_env;
		newproc.asc_nodelist = 0;

		while (n > 0) {
			newproc.asc_node = node->ndi_node;
			newproc.asc_srcnode = (p->asc_srcnode == -1) ?
					node->ndi_node : p->asc_srcnode;
			newproc.asc_args->apa_refcount += 1;
			newproc.asc_env->ape_refcount += 1;

			if (al_append(newapplist, &newproc) == 0) {
				al_free(newapplist);
				al_free(newnodelist);
				return(0);
			}

			node = (struct ndi *) al_next(newnodelist, node);

			if (node == 0) {
				node = (struct ndi *) al_top(newnodelist);
			}

			n--;
		}

		al_free(newnodelist);
		p = al_next(applist, p);
	}

	return(newapplist);
}
