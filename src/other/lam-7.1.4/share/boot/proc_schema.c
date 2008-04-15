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
 * $Id: proc_schema.c,v 6.8 2003/07/05 21:45:40 jsquyres Exp $
 * 
 *	Function:	- process schema operations
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <all_list.h>
#include <args.h>
#include <proc_schema.h>
#include <typical.h>
#include <etc_misc.h>
#include <sfh.h>
#include <boot.h>

/*
 * private functions
 */
static void var_substitute(struct psc *ppsc, char *var, char **argv_var);

/*
 * private variables
 */
static char line[PSC_MAXLINE];
static char *seps = " 	,=\n";

/*
 *	psc_parse
 *
 *	Function:	- parses process schema
 *	Accepts:	- open file stream
 *			- process schema list (out)
 *			- substitution variables
 */
int
psc_parse(FILE *fp, LIST **list_psc, char **argv_var)
{
	char		*r;
	char		*token;
	struct psc	psc_new;

	*list_psc = al_init(sizeof(struct psc), (int (*)()) 0);
	if (*list_psc == 0) return(LAMERROR);

	r = fgets(line, PSC_MAXLINE, fp);

	while (r) {

		if (*line == '#') {
			r = fgets(line, PSC_MAXLINE, fp);
			continue;
		}

		token = strtok(line, seps);

		if (token == 0) {
			r = fgets(line, PSC_MAXLINE, fp);
			continue;
		}

		psc_new.psc_argc = 0;
		psc_new.psc_argv = 0;
		psc_new.psc_delay = 0;
		psc_new.psc_flags = 0;

		while (token) {

			if (! strcmp(token, "$delay")) {
				psc_new.psc_delay = PSC_DELAY;
			} else if (*token == '$') {
				var_substitute(&psc_new, token, argv_var);
			} else {
                          sfh_argv_add(&psc_new.psc_argc, &psc_new.psc_argv,
						token);
			}

			token = strtok((char *) 0, seps);
		}

		if (psc_new.psc_argc != 0) {
			al_append(*list_psc, &psc_new);
		}

		r = fgets(line, PSC_MAXLINE, fp);
	}

	return(0);
}

/*
 *	psc_find
 *
 *	Function:	- expands each argv[0] to full pathname
 *	Accepts:	- process schema list
 *	Returns:	- 0 or ptr to program that can't be located
 */
struct psc *
psc_find(LIST *list_psc)
{
	char		*full;		/* full pathname */
	struct psc	*p;

	if (list_psc == 0) return(0);
/*
 * Loop through all the programs in the parsed process schema.
 */
	for (p = al_top(list_psc); p; p = al_next(list_psc, p)) {

		if (p->psc_argv == 0) continue;

		full = sfh_path_env_find(p->psc_argv[0], R_OK | X_OK);

		if (! full) {
			return(p);
		} else {
			p->psc_argv[0] = full;
		}
	}

	return(0);
}

/* 	
 *	psc_fill
 *
 *	Function:	- appends prefix to each argv[0] to generate full path 
 *			  name and checks for permissions
 *	Accepts:	- process schema list and prefix
 *	Returns:	- 0 or ptr to program that can't be located
 */
struct psc *
psc_fill(LIST *list_psc, char *prefix)
{

	char		*full;		/* full pathname */
	struct psc	*p;

	if (list_psc == 0) return(0);
/*
 * Loop through all the programs in the parsed process schema.
 */
	for (p = al_top(list_psc); p; p = al_next(list_psc, p)) {

		if (p->psc_argv == 0) continue;

	 	full = (char *) malloc((strlen(prefix) 
					+ strlen(p->psc_argv[0]) + 1)
				       * sizeof(char));
		
		if (full == NULL) 
			return (0);
		
		strcpy(full, prefix);
		strcat(full, p->psc_argv[0]);

		if (access(full, R_OK | X_OK)) {
			free(full);
			full = 0;
		}

		if (! full) {
			return(p);
		} else {
			p->psc_argv[0] = full;
		}
	}

	return(0);


}

/*
 *	cnfexec
 *
 *	Function:	- forks, execs and waits for a subordinate program
 *	Accepts:	- argument vector, argv[0] is program
 *	Returns:	- status code or ERROR
 */
int
cnfexec(char *argv[])
{
	return(_lam_few(argv));
}

/*
 *	var_substitute
 *
 *	Function:	- searches a variable in the substitution vector
 *			- if found, inserts its contents into process schema
 *	Accepts:	- process schema list entry
 *			- substitution variable
 *			- substitution variable vector
 */
static void
var_substitute(struct psc *ppsc, char *var, char **argv_var)
{
  char **p;
  char **args;
  
  if (argv_var == 0) 
    return;
  
  for (p = argv_var; *p; p++)
    if (!strncmp(*p, var, strlen(var))) 
      break;

  /* We can't nest the use of strtok here */
  if (*p) {
    args = sfh_argv_break(*p, ' ');
    for (p = args + 1; *p != NULL; p++) {
      sfh_argv_add(&ppsc->psc_argc, &ppsc->psc_argv, *p);
    }
    free(args);
  }
}
