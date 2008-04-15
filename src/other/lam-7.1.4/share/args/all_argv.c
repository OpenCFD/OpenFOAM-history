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
 * $Id: all_argv.c,v 6.10 2003/07/05 21:43:46 jsquyres Exp $
 *
 *	Function:	- argument vector library
 */

#include <stdlib.h>
#include <string.h>

#include <lam.h>
#include <etc_misc.h>
#include <sfh.h>

#define ARGSIZE		128


/*
 *	sfh_argv_add
 *
 *	Function:	- extends an argv with a new arg
 *	Accepts:	- argc (value returned)
 *			- argv (value returned)
 *			- new arg
 *	Returns:	- 0 or -1 (error)
 */
int
sfh_argv_add(int *argc, char ***argv, char *arg)
{
/*
 * Create new argv.
 */
	if (*argv == 0) {
		*argv = (char **) malloc(2 * sizeof(char *));
		if (*argv == 0) return(-1);
		(*argv)[0] = 0;
		(*argv)[1] = 0;
	}
/*
 * Extend existing argv.
 */
	else {
		*argv = (char **) realloc((char *) *argv,
				(unsigned) (*argc + 2) * sizeof(char *));
		if (*argv == 0) return(-1);
	}

	(*argv)[*argc] = (char*) malloc((unsigned) strlen(arg) + 1);
	if ((*argv)[*argc] == 0) return(-1);

	strcpy((*argv)[*argc], arg);
	*argc = *argc + 1;
	(*argv)[*argc] = 0;
	return(0);
}

/*
 *	sfh_argv_free
 *
 *	Function:	- frees memory associated with argv
 *	Accepts:	- argv
 */
void
sfh_argv_free(char **argv)
{
	char		**p;

	if (argv == 0) return;

	for (p = argv; *p; ++p) {
		free(*p);
	}

	free((char *) argv);
}

/*
 *	sfh_argv_break
 * 
 *	Function:	- breaks single string into argv
 *			- allocates space for argv structure
 *			  which must be freed by the caller
 *	Accepts:	- argument string
 *			- arg delimiter within string
 *	Returns:	- argv or NULL (error)
 */
char **
sfh_argv_break(char *args, int delim)
{
	char		arg[ARGSIZE];	/* single argument */
	char		**argv = 0;	/* argument structure ptr */
	char            *p;		/* walk through args */
	char		*argtemp;	/* temporary single argument */
	int		argc = 0;	/* # arguments */
	unint		arglen;		/* single argument length */

	while (*args) {
		p = args;
		arglen = 0;

		while ((*p != '\0') && (*p != delim)) {
			p++;
			arglen++;
		}
/*
 * zero length argument, skip
 */
		if (args == p) {
			p++;
		}
/*
 * tail argument, add straight from the original string
 */
		else if (*p == '\0') {

			if (sfh_argv_add(&argc, &argv, args)) return(0);
		}
/*
 * long argument, malloc buffer, copy and add
 */
		else if (arglen > (ARGSIZE - 1)) {
			argtemp = (char*) malloc((unsigned) (arglen + 1));
			if (argtemp == 0) return(0);

			lam_strncpy(argtemp, args, arglen);
			argtemp[arglen] = '\0';

			if (sfh_argv_add(&argc, &argv, argtemp)) {
				free(argtemp);
				return(0);
			}

			free(argtemp);
		}
/*
 * short argument, copy to buffer and add
 */
		else {
			lam_strncpy(arg, args, arglen);
			arg[arglen] = '\0';

			if (sfh_argv_add(&argc, &argv, arg)) return(0);
		}

		args = p;
	}

	return(argv);
}

/*
 *	sfh_argv_count
 * 
 *	Function:	- count the # of entries in argv
 *	Accepts:	- argv
 *	Returns:	- # of entries
 */
int
sfh_argv_count(char **argv)
{
	char		**p;
	int		i;

	if (argv == 0) return(0);

	for (i = 0, p = argv; *p; i++, p++);

	return(i);
}

/*
 *	sfh_argv_glue
 * 
 *	Function:	- glues argv into single string
 *			- delimiter and max string length specified
 *			- allocates space for string which must be
 *			  freed by the caller
 *	Accepts:	- argv
 *			- arg delimiter within string
 *			- max string length (or 0 meaning full argv)
 *	Returns:	- string or NULL (error)
 */
char *
sfh_argv_glue(char **argv, int delim, unint maxlen)
{
	char		**p;		/* favorite pointer */
	char		*pp;		/* another pointer */
	char		*str;		/* glued string */
	unint		str_len = 0;	/* string length */
	unint		i;		/* favorite index */
/*
 * Find the total string length in argv including delimiters.
 * The last delimiter is replaced by the NULL character.
 */
	for (p = argv; *p; p++) {
		str_len += strlen(*p) + 1;
	}

	if ((maxlen > 0) && (maxlen + 1 < str_len)) {
		str_len = maxlen + 1;
	}
/*
 * Allocate the string.
 */
	if ((str = (char*) malloc((unsigned) str_len)) == 0) return(0);
/*
 * Loop filling in the string.
 */
	str[--str_len] = '\0';
	p = argv;
	pp = *p;

	for (i = 0; i < str_len; i++) {

		if (*pp == '\0') {
/*
 * End of a string, fill in a delimiter and go to the next string.
 */
			str[i] = (char) delim;
			p++;
			pp = *p;
		} else {
			str[i] = *pp++;
		}
	}

	return(str);
}

/*
 *	sfh_argv_len
 * 
 *	Function:	- count the bytes consumed by an argv
 *	Accepts:	- argv
 *	Returns:	- length
 */
unint
sfh_argv_len(char **argv)
{
	char		**p;
	unint		length;		/* argv length */

	if (argv == 0) return(0);

	length = sizeof(char *);

	for (p = argv; *p; p++) {
		length += strlen(*p) + 1 + sizeof(char *);
	}

	return(length);
}

/*
 *	sfh_argv_dup
 *
 *	Function:	- create a duplicate copy of an argv
 *	Accepts:	- argv
 *	Returns:	- duplicate argv or NULL
 */
char **
sfh_argv_dup(char **argv)
{
	char		**dupv = 0;	/* duplicate argv */
	int		dupc = 0;	/* dupv count */

	if (argv == 0) return(0);

	while (*argv != 0) {

		if (sfh_argv_add(&dupc, &dupv, *argv)) {
			sfh_argv_free(dupv);
			return(0);
		}

		argv++;
	}

	return(dupv);
}

#define Q 		quotes[0]
#define SQ 		quotes[1]
#define DQ 		quotes[2]

static void
unquote(char *unquoted, char *arg, char *end, char quotes[])
{
	char  		*p;
	int         	insq = 0;
	int		indq = 0;

	p = arg;
	while (p < end) {
		if (*p == SQ) {
			if (insq) {
				insq = 0;
				p++;
			} else if (indq) {
				*unquoted++ = *p++;
			} else {
				insq = 1;
				p++;
			}
		} else if (*p == DQ) {
			if (insq) {
				*unquoted++ = *p++;
			} else if (indq) {
				indq = 0;
				p++;
			} else {
				indq = 1;
				p++;
			}
		} else if (*p == Q) {
			if (*(p+1) == Q || *(p+1) == DQ) {
				*unquoted++ = *(p+1);
				p += 2;
			} else {
				*unquoted++ = *p++;
			}
		} else {
			*unquoted++ = *p++;
		}
	}

	*unquoted = 0;
}

/*
 *	sfh_argv_break_quoted
 * 
 *	Function:	- breaks single possibly quoted string into argv
 *			- allocates space for argv structure
 *			  which must be freed by the caller
 *	Accepts:	- argument string
 *			- arg delimiter within string
 *			- array of quotes (quote, single quote, double quote)
 *	Returns:	- argv or NULL (error)
 */
char **
sfh_argv_break_quoted(char *args, int delim, char quotes[])
{
	char		arg[ARGSIZE];	/* single argument */
	char		**argv = 0;	/* argument structure ptr */
	char	        *p;		/* walk through args */
	char		*argtemp;	/* temporary single argument */
	int		argc = 0;	/* # arguments */
	unint		arglen;		/* single argument length */
	int             insq;		/* inside single quotes */
	int		indq;		/* inside double quotes */
 
	while (*args) {
		p = args;
		arglen = 0;
		indq = insq = 0;

		while (*p) {
			if (*p == SQ) {
				if (insq) {
					insq = 0;
					p++;
				} else if (indq) {
					arglen++;
					p++;
				} else {
					insq = 1;
					p++;
				}
			} else if (*p == DQ) {
				if (insq) {
					arglen++;
					p++;
				} else if (indq) {
					indq = 0;
					p++;
				} else {
					indq = 1;
					p++;
				}
			} else if (*p == delim) {
				if (insq || indq) {
					arglen++;
					p++;
				} else {
					break;
				}
			} else if (*p == Q) {
				if (*(p+1) == Q || *(p+1) == DQ) {
					arglen++;
					p += 2;
				} else {
					arglen++;
					p++;
				}
			} else {
				arglen++;
				p++;
			}
		}
/*
 * zero length argument, skip
 */
		if (args == p) {
			p++;
		}
/*
 * long argument, malloc buffer, copy and add
 */
		else if (arglen > (ARGSIZE - 1)) {
			argtemp = (char*) malloc((unsigned) (arglen + 1));
			if (argtemp == 0) return(0);

			unquote(argtemp, args, p, quotes);
			if (sfh_argv_add(&argc, &argv, argtemp)) {
				free(argtemp);
				return(0);
			}

			free(argtemp);
		}
/*
 * short argument, copy to buffer and add
 */
		else {
			unquote(arg, args, p, quotes); 
			if (sfh_argv_add(&argc, &argv, arg)) return(0);
		}

		args = p;
	}

	return(argv);
}

/*
 *	sfh_argv_quote
 * 
 *	Function:	- quote string
 *			- allocates space for quoted string
 *			  which must be freed by the caller
 *	Accepts:	- string
 *			- array of quotes (quote, single quote, double quote)
 *	Returns:	- quoted string or NULL (error)
 */
char *
sfh_argv_quote(char *s, char quotes[])
{
	char        	*qs;
	int         	len = strlen(s);
	int         	nq = 0;
	int         	i, j;

	/* find number of \ quotes needed */
	for (i = 0; i < len; i++) {
		if (s[i] == DQ || s[i] == Q) {
			nq++;
		}
	}

	qs = (char*) malloc(len+nq+3);
	qs[0] = DQ;

	for (j = 1, i = 0; i < len; i++) {
		if (s[i] == DQ) {
			qs[j++] = Q;
			qs[j++] = DQ;
		} else if (s[i] == Q) {
			qs[j++] = Q;
			qs[j++] = Q;
		} else {
			qs[j++] = s[i];
		}
	}

	qs[j++] = DQ;
	qs[j] = 0;

	return(qs);
}
