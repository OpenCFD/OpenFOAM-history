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
 * $Id: do_args.c,v 6.5 2003/07/05 21:43:59 jsquyres Exp $
 * 
 *	Function:	- do_args(3) backward compatibility
 *			- uses all_opt package
 */

#include <stdlib.h>

#include <all_opt.h>
#include <args.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <args.h>

/*
 * global variables
 */
int lam_argtailc = 0;
char **lam_argtailv = 0;
OPT *lam_args_optd = 0;


/*
 * local functions
 */
static int init(void);


/*
 *	validopts
 *
 *	Function:	- declare valid options
 *	Accepts:	- string of options
 */
void
validopts(CONST char *str)
{
	char		opt[2];

	if (str == 0) return;
	if (init()) return;
/*
 * Loop declaring the options.
 */
	for (opt[1] = '\0'; *str; ++str) {
		*opt = (*str == '0') ? '#' : *str;
		if (ao_setopt(lam_args_optd, opt, (char *) 0, 0, AODOARGS)) 
		  return;
	}
}


/*
 *	exclusive
 *
 *	Function:	- declare mutex option group
 *	Accepts:	- string of options
 */
void
exclusive(CONST char *str)
{
	char		opt[2];
	char		first[2];

	if (str == 0) return;
	if (init()) return;
/*
 * Declare first option.
 */
	first[0] = *str++; first[1] = '\0';
	if (ao_setopt(lam_args_optd, first, (char *) 0, 0, AODOARGS)) return;
/*
 * Declare the rest.
 */
	for (opt[1] = '\0'; *str; ++str) {
		*opt = *str;
		if (ao_setopt(lam_args_optd, opt, first, 0, AODOARGS)) return;
	}
}


/*
 *	followed
 *
 *	Function:	- declare followed options
 *	Accepts:	- string of options
 */
void
followed(CONST char *str)
{
	char		opt[2];

	if (str == 0) return;
	if (init()) return;
/*
 * Declare the options.
 */
	for (opt[1] = '\0'; *str; ++str) {
		*opt = *str;
		if (ao_setopt(lam_args_optd, opt, (char *) 0, 1, AODOARGS)) 
		  return;
	}
}


/*
 *	twoletter
 *
 *	Function:	- declare two-letter option group
 *	Accepts:	- string of options
 */
void
twoletter(CONST char *str)
{
	char		opt[3];

	if (str == 0) return;
	if (init()) return;
/*
 * Declare two-letter options.
 */
	opt[0] = *str++; opt[2] = '\0';

	if (*str == ':') ++str;

	for ( ; *str; ++str) {
		opt[1] = *str;
		if (ao_setopt(lam_args_optd, opt, (char *) 0, 0, AODOARGS)) 
		  return;
	}
}


/*
 *	opt_taken
 *
 *	Function:	- check if option taken
 *	Accepts:	- letter
 *	Returns:	- TRUE or FALSE
 */
int
opt_taken(int let)
{
	char		opt[2];

	opt[0] = (let == '0') ? '#' : let & 0xFF;
	opt[1] = '\0';

	return(ao_taken(lam_args_optd, opt));
}


/*
 *	opt_chosen
 *
 *	Function:	- get mutex chosen option
 *	Accepts:	- a mutex option
 *	Returns:	- chosen option
 */
int
opt_chosen(int let)
{
	char		opt[2];
	char		*chosen;

	opt[0] = let & 0xFF; opt[1] = '\0';

	chosen = ao_chosen(lam_args_optd, opt);

	return((chosen) ? *chosen : LAMERROR);
}


/*
 *	do_args
 *
 *	Function:	- parse arguments
 *	Accepts:	- ptr argc
 *			- argv
 *	Returns:	- 0 or LAMERROR
 */
int
do_args(int *argc, char **argv)
{
	if (lam_args_optd == 0) return(LAMERROR);

	if (ao_parse(lam_args_optd, argc, argv)) return(LAMERROR);

	lam_argtailc = lam_args_optd->ao_tailc;
	lam_argtailv = lam_args_optd->ao_tailv;

	return(0);
}


/*
 *	getparam
 *
 *	Function:	- get followed option parameter
 *	Accepts:	- taken option
 *	Returns:	- parameter or NULL
 */
char *
getparam(int let)
{
	char		opt[2];

	opt[0] = (let == '0') ? '#' : let & 0xFF;
	opt[1] = '\0';

	return(ao_param(lam_args_optd, opt, 0, 0));
}


/*
 *	intparam
 *
 *	Function:	- get followed option integer parameter
 *	Accepts:	- taken option
 *			- ptr integer (returned value)
 *	Returns:	- 0 or LAMERROR
 */
int
intparam(int let, int *pint)
{
	char		*param;

	param = getparam(let);
	if (param == 0) return(LAMERROR);

	return(xtoi(&param, pint));
}


/*
 *	get_2nd_let
 *
 *	Function:	- get 2nd letter of next taken two-letter option
 *			- resets state of two-letter options along the way
 *			  to handle the stateful design of the call
 *			- messy, not opaque in order to keep all_args nice
 *	Accepts:	- first letter of two-letter option
 *	Returns:	- 2nd letter or LAMERROR
 */
int
get_2nd_let(int let)
{
	struct ao_opt	*p;
	

	for (p = lam_args_optd->ao_opttop; p; p = p->aoo_next) {

		if ((p->aoo_string[0] == let) && (p->aoo_flags & AOTAKEN)) {
			p->aoo_flags &= ~AOTAKEN;
			return(p->aoo_string[1]);
		}
	}

	return(LAMERROR);
}


/*
 *	reset_args
 *
 *	Function:	- free the arguments
 */
void
reset_args(void)
{
	if (lam_args_optd) {
		ao_free(lam_args_optd);
		lam_args_optd = 0;
	}

	lam_argtailc = 0;
	lam_argtailv = 0;
}


/*
 *	xtoi
 *
 *	Function:	- convert string to integer (base 10 or 16) 
 *			- uses strtol()
 *	Accepts:	- ptr string
 *			- ptr integer (returned value)
 *	Returns:	- 0 or LAMERROR
 */
int
xtoi(char **pstr, int *pint)
{
	char		*str;

	if ((str = *pstr) == 0) {
		errno = EUSAGE;
		return(LAMERROR);
	}

	*pint = (int) strtol(str, pstr, 0);

	if ((*pint == 0) && (str == *pstr)) {
		errno = EBADASCIINUMB;
		return(LAMERROR);
	}

	return(0);
}


/*
 *	init
 *
 *	Function:	- create option descriptor if needed
 *	Returns:	- 0 or LAMERROR
 */
static int
init(void)
{
	if (lam_args_optd == 0) {
		lam_args_optd = ao_init();
	}

	return((lam_args_optd) ? 0 : LAMERROR);
}
