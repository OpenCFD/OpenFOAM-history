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
 * $Id: pdi_parse.c,v 6.9 2003/07/05 21:44:46 jsquyres Exp $
 *
 *	Function:	- node identifier parsing package
 *			- id_range() based in part on Trollius 2.0,
 *			  Copyright 1990 The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <args.h>
#include <net.h>
#include <pdi.h>
#include <portable.h>
#include <preq.h>
#include <terror.h>
#include <typical.h>
#include <sfh.h>

#define NEW		1
#define ONE		2
#define MANY		3

/*
 * local functions
 */
static int id_range(char *s, int4 flags, int *ixp, LIST *lp);

/*
 *	pdi_parse
 *
 *	Functions:	- parses process identifier mnemonics
 *	Accepts:	- command line count
 *			- command line vector
 *			- leftover vector (returned)
 *	Returns:	- process list or null
 */
LIST *
pdi_parse(int argc, char **argv, char ***othersv)
{
	int		index = 0;		/* current index */
	int		othersc;		/* leftover count */
	LIST *		proclist;		/* node ID list */
/*
 * Initialize the list.
 */
	proclist = al_init(sizeof(struct pdi), 0);
	if (proclist == 0) return(0);
/*
 * Pass argument 0 through.
 */
	if (othersv) {
		othersc = 0;
		*othersv = 0;

		if (sfh_argv_add(&othersc, othersv, argv[0])) {
			al_free(proclist);
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
		if ((**argv == 'p') && isdigit((int) *(*argv + 1))) {

			if (id_range(*argv + 1, SELECT_PID, &index, proclist)) {
				al_free(proclist);
				sfh_argv_free(*othersv);
				return(0);
			}
		}
/*
 * Is it an index identifier?
 */
		else if (((**argv == '%') || (**argv == 'i')) &&
				isdigit((int) *(*argv + 1))) {

			if (id_range(*argv + 1,
					SELECT_INDEX, &index, proclist)) {
				al_free(proclist);
				sfh_argv_free(*othersv);
				return(0);
			}
		}
/*
 * Bump the non-id pointer and counter.
 */
		else if (othersv) {

			if (sfh_argv_add(&othersc, othersv, argv[0])) {
				al_free(proclist);
				sfh_argv_free(*othersv);
				return(0);
			}
		}

		argc--;
		argv++;
	}

	return(proclist);
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
id_range(char *s, int4 flags, int *ixp, LIST *lp)
{
	int		c;			/* current character */
	int4		id;			/* current identifier */
	int4		oldid = 0;		/* previous identifier */
	int		mode;			/* parsing state */
	struct pdi	newpdi;			/* new list entry */
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
				newpdi.pdi_node = id;
				newpdi.pdi_flags = flags;
				newpdi.pdi_count = 1;
				newpdi.pdi_index = (*ixp)++;

				if (al_append(lp, &newpdi) == 0) return(LAMERROR);

				oldid = id;
				mode = ONE;
			}

			else if (mode == MANY) {

				if (id < oldid) {
					errno = EBADIDSPEC;
					return(LAMERROR);
				}

				while (++oldid <= id) {
					newpdi.pdi_node = oldid;
					newpdi.pdi_flags = flags;
					newpdi.pdi_count = 1;
					newpdi.pdi_index = (*ixp)++;

					if (al_append(lp, &newpdi) == 0)
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
