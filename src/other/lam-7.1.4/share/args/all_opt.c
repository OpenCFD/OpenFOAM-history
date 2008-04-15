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
 * $Id: all_opt.c,v 6.10 2003/03/22 15:31:55 jsquyres Exp $
 * 
 *	Function:	- generic option parsing package
 *			- uses malloc()
 */

#include <stdlib.h>
#include <string.h>

#include <all_opt.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <sfh.h>

/*
 * local functions
 */
static int addinst(struct ao_opt *opt, int nparam, char **params);
static int countparam(OPT *aod, struct ao_opt *opt, char **argv);
static int mutexchk(struct ao_opt *opt);
static int lam_strtonum(const char *str, int *pnum);
static struct ao_opt *findopt(OPT *desc, const char *optstr);


/*
 *	ao_init
 *
 *	Function:	- create an option descriptor
 *	Returns:	- ptr option desc. or NULL
 */
OPT *
ao_init(void)
{
	OPT		*aod;			/* ptr option desc. */

	aod = (OPT *) malloc(sizeof(OPT));

	if (aod) {
		aod->ao_ntaken = 0;
		aod->ao_opttop = 0;
		aod->ao_optend = 0;
		aod->ao_optcur = 0;

		aod->ao_flags = 0;

		aod->ao_argv0 = 0;

		aod->ao_unusedc = 0;
		aod->ao_unusedv = 0;

		aod->ao_argv_dup = 0;
                aod->ao_string_dups = 0;

		aod->ao_tailc = 0;
		aod->ao_tailv = 0;
	}

	return(aod);
}


/*
 *	ao_free
 *
 *	Function:	- destroy an option descriptor
 *	Accepts:	- ptr option desc.
 */
void
ao_free(OPT *aod)
{
	struct ao_inst	*pinst;			/* ptr instance */
	struct ao_opt	*p;			/* favourite pointer */
	int		i;			/* favourite index */

	if (aod == 0) return;
/*
 * Loop free'ing the options.
 */
	while ((p = aod->ao_opttop) != 0) {

		aod->ao_opttop = p->aoo_next;

		if ((pinst = p->aoo_insts) != 0) {
			for (i = 0; i < p->aoo_ninst; ++i, ++pinst) {
				free((char *) pinst->aoi_params);
			}

			free((char *) p->aoo_insts);
		}

		free((char *) p);
	}

	/* Free the argv0 copy */

	if (aod->ao_argv0)
	  free(aod->ao_argv0);

	/* If AOPRESERVE_ARGV was set, we need to free the argv and
           corresponding strings that were allocated in ao_parse(). */

	if (aod->ao_argv_dup != NULL) {
          for (i = 0; aod->ao_string_dups[i] != NULL; ++i)
            free(aod->ao_string_dups[i]);
          free(aod->ao_string_dups);
	  free(aod->ao_argv_dup);
        }

	free((char *) aod);
}


/*
 *	ao_ntaken
 *
 *	Function:	- return # taken options
 *	Accepts:	- ptr option descriptor
 *	Returns:	- # taken options
 */
int
ao_ntaken(OPT *aod)
{
	return((aod) ? aod->ao_ntaken : 0);
}


/*
 *	ao_taken
 *
 *	Function:	- check if option is taken
 *	Accepts:	- ptr option descriptor
 *			- option string
 *	Returns:	- TRUE or FALSE
 */
int
ao_taken(OPT *aod, const char *opt)
{
	struct ao_opt	*p;			/* favourite pointer */

	if (aod) {
		p = findopt(aod, opt);
		if (p && (p->aoo_flags & AOTAKEN)) return(TRUE);
	}

	return(FALSE);
}


/*
 *	ao_chosen
 *
 *	Function:	- find chosen option in mutex group
 *			- if not a mutex group, returns NULL
 *	Accepts:	- ptr option descriptor
 *			- an option string in mutex group
 *	Returns:	- chosen/taken option or NULL
 */
char *
ao_chosen(OPT *aod, const char *opt)
{
	struct ao_opt	*p;			/* favourite pointer */

	if (aod == 0) return((char *) 0);
/*
 * Loop locating a taken option in mutex group.
 */
	p = findopt(aod, opt);

	if (p->aoo_mutex == 0) p = 0;

	while (p) {
		if (p->aoo_flags & AOTAKEN) {
			aod->ao_optcur = p;
			break;
		}

		p = p->aoo_mutex;
		if (p == aod->ao_optcur) p = 0;
	}

	return((p) ? p->aoo_string : (char *) 0);
}


/*
 *	ao_ninsts
 *
 *	Function:	- get # instances for taken option
 *	Accepts:	- ptr option descriptor
 *			- option string
 *	Returns:	- # instances
 */
int
ao_ninsts(OPT *aod, const char *opt)
{
	struct ao_opt	*p;			/* favourite pointer */

	p = (aod) ? findopt(aod, opt) : (struct ao_opt *) 0;

	return((p && (p->aoo_ninst > 0)) ? p->aoo_ninst : 0);
}


/*
 *	ao_nparams
 *
 *	Function:	- get # parameters for taken option/instance
 *	Accepts:	- ptr option descriptor
 *			- option string
 *			- instance number
 *	Returns:	- # parameters
 */
int
ao_nparams(OPT *aod, const char *opt, int inst)
{
	struct ao_opt	*p;			/* favourite pointer */
	int		ret = 0;		/* returned value */

	if (aod) {
		p = findopt(aod, opt);

		if (p && (p->aoo_flags & AOTAKEN) && (p->aoo_insts) &&
				(inst >= 0) && (inst < p->aoo_ninst)) {
			ret = p->aoo_insts[inst].aoi_nparam;
		}
	}

	return(ret);
}


/*
 *	ao_param
 *
 *	Function:	- return a given string parameter for an option
 *	Accepts:	- ptr option descriptor
 *			- option string
 *			- instance number
 *			- parameter index
 *	Returns:	- parameter string or NULL
 */
char *
ao_param(OPT *aod, const char *opt, int inst, int idx)
{
	struct ao_opt	*p = 0;			/* favourite pointer */
	struct ao_inst	*pinst;			/* ptr instance */
	char		*param = 0;		/* returned parameter */

	if (aod) p = findopt(aod, opt);

	if (p && (p->aoo_flags & AOTAKEN) && !(p->aoo_flags & AOINT) &&
		(p->aoo_insts) && (inst >= 0) && (inst < p->aoo_ninst)) {

		pinst = p->aoo_insts + inst;

		if ((idx >= 0) && (idx < pinst->aoi_nparam)) {
			param = ((char **) pinst->aoi_params)[idx];
		}
	}

	return(param);
}


/*
 *	ao_intparam
 *
 *	Function:	- return a given integer parameter for an option
 *	Accepts:	- ptr option descriptor
 *			- option string
 *			- instance number
 *			- parameter index
 *			- ptr parameter (returned value)
 *	Returns:	- 0 or LAMERROR
 */
int
ao_intparam(OPT *aod, const char *opt, int inst, int idx, int *pparam)
{
	struct ao_opt	*p = 0;			/* favourite pointer */
	struct ao_inst	*pinst;			/* ptr instance */
	int		ret = LAMERROR;		/* returned value */

	if (aod) p = findopt(aod, opt);

	if (p && (p->aoo_flags & AOTAKEN) && (p->aoo_flags & AOINT) &&
		(p->aoo_insts) && (inst >= 0) && (inst < p->aoo_ninst)) {

		pinst = p->aoo_insts + inst;

		if ((idx >= 0) && (idx < pinst->aoi_nparam)) {
			*pparam = ((int *) pinst->aoi_params)[idx];
			ret = 0;
		}
	}

	return(ret);

}


/*
 *	ao_tail
 *
 *	Function:	- return the tail argc/argv
 *	Accepts:	- ptr option descriptor
 *			- ptr tail argc (returned value)
 *			- ptr tail argv (returned value)
 *	Returns:	- 0 or LAMERROR
 */
int
ao_tail(OPT *aod, int *tailc, char ***tailv)
{
	int		ret = LAMERROR;		/* returned value */

	if (aod) {
		*tailc = aod->ao_tailc;
		*tailv = aod->ao_tailv;
		ret = 0;
	}

	return(ret);
}


/*
 *	ao_unused
 *
 *	Function:	- return the unused argc/argv
 *	Accepts:	- ptr option descriptor
 *			- ptr unused argc (returned value)
 *			- ptr unused argv (returned value)
 *	Returns:	- 0 or LAMERROR
 */
int
ao_unused(OPT *aod, int *unusedc, char ***unusedv)
{
  int ret = LAMERROR;

  if (aod) {
    *unusedc = aod->ao_unusedc;
    *unusedv = aod->ao_unusedv;
    ret = 0;
  }

  return ret;
}


/*
 *	ao_setflags
 *
 *	Function:	- return the unused argc/argv
 *	Accepts:	- ptr option descriptor
 *			- flags
 *	Returns:	- 0 or LAMERROR
 */
int
ao_setflags(OPT *aod, int flags)
{
  int ret = LAMERROR;

  if (aod) {
    aod->ao_flags = flags;
    ret = 0;
  }

  return ret;
}


/*
 *	ao_argv0
 *
 *	Function:	- return a copy of the original argv[0]
 *	Accepts:	- ptr option descriptor
 *	Returns:	- ptr to copy of original argv[0] or NULL
 */
char *
ao_argv0(OPT *aod)
{
  return (aod) ? aod->ao_argv0 : NULL;
}


/*
 *	ao_setopt
 *
 *	Function:	- define a valid option
 *			- option "#" is a special case
 *	Accepts:	- ptr option descriptor
 *			- option string
 *			- mutex option (or NULL)
 *			- # parameters (or AOVARNUM)
 *			- option flags
 *	Returns:	- 0 or LAMERROR
 */
int
ao_setopt(OPT *aod, const char *opt, const char *mutex, int nparams, int flags)
{
	struct ao_opt	*p;			/* favourite pointer */
	struct ao_opt	*mtx;			/* ptr mutex option */
 
	if (aod == 0) return(LAMERROR);
/*
 * If option is "#", force AOINT flag and 1 parameter.
 */
	if (strcmp(opt, "#") == 0) {
		flags |= (flags & AODOARGS) ? 0 : AOINT;
		nparams = 1;
	}
/*
 * If new option create it.
 */
	p = findopt(aod, opt);

	if (p == 0) {
		p = (struct ao_opt *) malloc((unsigned)
				sizeof(struct ao_opt) + strlen(opt) + 1);
		if (p == 0) return(LAMERROR);

		p->aoo_string = ((char *) p) + sizeof(struct ao_opt);
		strcpy(p->aoo_string, opt);

		if (aod->ao_opttop == 0) {
			aod->ao_opttop = p;
		} else {
			aod->ao_optend->aoo_next = p;
		}

		aod->ao_optend = p;
		p->aoo_next = 0;
	}
/*
 * Otherwise, better not have mutex or instances previously defined.
 */
	else if (p->aoo_insts || p->aoo_mutex) {
		return(LAMERROR);
	}
/*
 * Fill the option information.
 */
	p->aoo_flags = flags & ~AOTAKEN;
	p->aoo_nmaxparam = nparams;
	p->aoo_ninst = 0;
	p->aoo_insts = 0;
	p->aoo_mutex = 0;

	if (mutex) {
		mtx = findopt(aod, mutex);
		if (mtx == 0) return(LAMERROR);

		p->aoo_mutex = (mtx->aoo_mutex) ? mtx->aoo_mutex : mtx;
		mtx->aoo_mutex = p;
	}

	aod->ao_optcur = p;

	return(0);
}


/*
 *	ao_setopt1
 *
 *	Function:	- defines several one-letter options
 *			- calls ao_setopt in a loop
 *	Accepts:	- ptr option descriptor
 *			- option string
 *			- mutex option (or NULL)
 *			- # parameters (or AOVARNUM)
 *			- option flags
 *	Returns:	- 0 or LAMERROR
 */
int
ao_setopt1(OPT *aod, const char *opt, const char *mutex, int nparams,
	   int flags)
{
	const char	*p;
	char		opt1[2];

	opt1[1] = '\0';

	for (p = opt; *p; p++) {
		opt1[0] = *p;

		if (ao_setopt(aod, opt1, mutex, nparams, flags)) {
			return(LAMERROR);
		}
	}

	return(0);
}


/*
 *	ao_parse
 *
 *	Function:	- parse the given argc/argv
 *			- remove parsed options from argc/argv
 *			- args after -- are parsed as the tail
 *			- special option "#" matches -number option
 *			- for pure one-letter options, spaces not needed
 *	Accepts:	- ptr option descriptor
 *			- ptr cmd line argc
 *			- cmd line argv
 *	Returns:	- 0 or LAMERROR
 */
int
ao_parse(OPT *aod, int *pcmdc, char **cmdv)
{
	int		i;
	int		num;			/* parsed number */
	int		fl_onelet;		/* one-letter options? */
	int		argc;			/* argc for parsing */
	char		optstr[2];		/* 1-let opt. string */
	char		**argv;			/* argv for parsing */
	char		*str;			/* string parsed */
	struct ao_opt	*popt;			/* ptr option */

	if (aod == 0) {
		errno = EINVAL;
		return(LAMERROR);
	}
/*
 * Check if all options are one-letter.
 */
	fl_onelet = 1;

	for (popt = aod->ao_opttop; popt; popt = popt->aoo_next) {

		if (strlen(popt->aoo_string) > 1) {
			fl_onelet = 0;
			break;
		}
	}

	/* If AOPRESERVE_ARGV is set, copy argc/argv into ao_unused*,
	   and operate from there */

	if (aod->ao_flags & AOPRESERVE_ARGV) {
	  aod->ao_unusedc = *pcmdc;
	  aod->ao_unusedv = aod->ao_argv_dup = sfh_argv_dup(cmdv);

          /* Remember that sfh_argv_dup() mallocs two major kinds of
             things: the argv vector itself (i.e., char **), and a set
             of strings that are held in the argv vector (i.e., a
             bunch of (char*)'s).  We've taken care of freeing the
             argv vector by holding its valie in ao_argv_dup (which is
             freed in ao_free()).  But the individual strings must
             also be freed when no longer used.

             This is a problem because it violates one of the original
             assumptions of the ao_parse code: in the parsing
             algorithm below, when an option is reconized, its string
             is simply discarded, assuming that it was an argument to
             main(), and therefore we don't need to free it.  However,
             with AOPRESERVE_ARGV, this string now *is* specifically
             malloc'ed (and therefore must be freed when it is no
             longer being used).  There are two options on how to keep
             track of all of these strings so that they can be freed
             properly:

             1. Implement a new type that holds the string and a flag
                indicating whether the string needs to be freed or
                not.  Instead of having an argv of (char*), now have
                an argv of this new type.  Whenever an element in this
                argv is discarded, examine the flag and see if the
                string needs to be freed.

             2. Save all malloc'ed strings when they are initially
                strdup'ed.  At the end of time, free all of them. 

             The second approach is slightly more wasteful in that the
             strings will not be freed as they are discarded by the
             parsing algorithm, but it still frees them all at the end
             of time.  And it's dramatically simpler and less
             error-prone to implement.  Hence, that's what we'll
             do. 

             Since the vector pointed to by ao_argv_dup will shrink as
             options are removed from it, we need another vector that
             simply holds pointers to all the strings that were dup'ed
             by sfh_argv_dup (above) -- and doesn't dup them again.
             Hence, we have ao_string_dups, which is a
             manually-malloc'ed vector that we preserve all the string
             pointers in so that they can be freed in ao_free(). */

          aod->ao_string_dups = malloc(sizeof(char**) * (*pcmdc + 1));
          for (i = 0; i < *pcmdc; ++i)
            aod->ao_string_dups[i] = aod->ao_argv_dup[i];
          aod->ao_string_dups[i] = NULL;

	  pcmdc = NULL;
	  cmdv = NULL;
	}

	/* If pcmdc/cmdv are NULL, use the unusedc/unusedv (e.g., if
	   this is a re-parse) */

	if (pcmdc == NULL && cmdv == NULL) {
	  pcmdc = &aod->ao_unusedc;
	  cmdv = aod->ao_unusedv;
	}

	/* Save argv[0] if we haven't done so already */

	if (aod->ao_argv0 == NULL && cmdv != NULL && cmdv[0] != NULL)
	  aod->ao_argv0 = strdup(cmdv[0]);

/*
 * Skip argv[0].
 */
	argc = *pcmdc - 1;
	argv = cmdv + 1;

	*pcmdc = 1;
/*
 * Loop parsing.
 */
	for ( ; argc > 0; --argc, ++argv) {

		str = *argv;
/*
 * If not an option, pass it through.
 */
		if (*str != '-') {
			cmdv[(*pcmdc)++] = str;
		}
/*
 * If option is "--" we found the tail.
 */
		else if (strcmp(str, "--") == 0) {
			aod->ao_tailc = argc - 1;
			aod->ao_tailv = argv + 1;
			break;
		}
/*
 * If a number, assume '#' option.
 */
		else if (lam_strtonum(++str, &num) == 0) {

			popt = findopt(aod, "#");
			if ((popt == 0) || (mutexchk(popt))) {
				errno = EUSAGE;
				return(LAMERROR);
			}

			if ((popt->aoo_flags & AOTAKEN) == 0) {
				popt->aoo_flags |= AOTAKEN;
				++(aod->ao_ntaken);
			}

			if (addinst(popt, 1, &str)) {
				return(LAMERROR);
			}
		}
/*
 * Handle as a one-letter option.
 * Following letters may be valid options or a parameter.
 */
		else if (fl_onelet && (str[1] != '\0')) {

			while (*str) {

				optstr[0] = *str;
				optstr[1] = '\0';

				popt = findopt(aod, optstr);

				if ((popt == 0) || (mutexchk(popt))) {
					errno = EUSAGE;
					return(LAMERROR);
				}

				if ((popt->aoo_flags & AOTAKEN) == 0) {
					popt->aoo_flags |= AOTAKEN;
					++(aod->ao_ntaken);
				}
/*
 * If option takes no parameters, consider next option letter.
 */
				if (popt->aoo_nmaxparam == 0) {
					++str;
				}
/*
 * If option can be followed by one argument and we're not at
 * the end-of-string, treat the rest-of-string as a parameter.
 */
				else if ((popt->aoo_nmaxparam == 1) &&
						(str[1] != '\0')) {

					++str;
					if (addinst(popt, 1, &str)) {
						return(LAMERROR);
					}

					str += strlen(str);
				}
/*
 * Otherwise handle parameters as usual.
 */
				else {
					num = countparam(aod, popt, argv + 1);
					if (num < 0) return(LAMERROR);

					if (addinst(popt, num, argv + 1)) {
						return(LAMERROR);
					}

					argv += num;
					argc -= num;
					break;
				}
			}
		}
/*
 * Handle as a regular option.
 */
		else {
			popt = findopt(aod, str);

			if ((popt == 0) || (mutexchk(popt))) {
				errno = EUSAGE;
				return(LAMERROR);
			}

			if ((popt->aoo_flags & AOTAKEN) == 0) {
				popt->aoo_flags |= AOTAKEN;
				++(aod->ao_ntaken);
			}
/*
 * Add parameters if any.
 */
			num = countparam(aod, popt, argv + 1);
			if (num < 0) return(LAMERROR);

			if (addinst(popt, num, argv + 1)) {
				return(LAMERROR);
			}

			argv += num;
			argc -= num;
		}
	}

	cmdv[*pcmdc] = 0;
/*
 * Check the "must" take options.
 */
	for (popt = aod->ao_opttop; popt; popt = popt->aoo_next) {

		if ((popt->aoo_flags & AOMUST) &&
			((popt->aoo_flags & AOTAKEN) == 0)) {
/*
 * If mutex, check that one of them is taken.
 */
			str = 0;
			if (popt->aoo_mutex) {
				str = ao_chosen(aod, popt->aoo_string);
			}
/*
 * If no mutex taken or not a mutex case, it's an error.
 */
			if (str == 0) {
				errno = EUSAGE;
				return(LAMERROR);
			}
		}
	}

	return(0);
}


/*
 * ao_exam_isopt
 *
 * Function:
 *  Determine whether opt has been added to the options descriptor or
 *  not.
 * Accepts:
 *  aod - options descriptor
 *  opt - option in question
 * Returns:
 *  1        - opt has been added
 *  0        - opt has not bee added
 */
int
ao_exam_isopt(OPT *aod, const char *opt)
{
  struct ao_opt	*p;

  if (aod) {
    p = findopt(aod, opt);
    if (p)
      return 1;
  }

  return 0;
} 


/*
 * ao_exam_nparams
 *
 * Function:
 *  Find number of expected (NOT TAKEN) params for a given option
 *
 * Accepts:
 *  aod - options descriptor
 *  opt - option in question
 * Output:
 *  nparams - number of params (May be AOVARNUM)
 * Returns:
 *  0        - success
 *  LAMERROR - internal accounting error (aka option not found)
 */
int
ao_exam_nparams(OPT *aod, const char *opt, int *nparams)
{
  struct ao_opt *p;

  if (aod) {
    p = findopt(aod, opt);

    if (p) {
      *nparams = p->aoo_nmaxparam;
      return 0;
    }
  }

  return LAMERROR;
}


/*
 *	findopt
 *
 *	Function:	- locate given option
 *	Accepts:	- ptr option descriptor
 *			- option string
 *	Returns:	- ptr option or NULL
 */
static struct ao_opt *
findopt(OPT *desc, const char *optstr)
{
	struct ao_opt	*p;			/* favourite pointer */
/*
 * Check if current option matches.
 */
	p = desc->ao_optcur;

	if (p && (strcmp(p->aoo_string, optstr) == 0)) return(p);
/*
 * Otherwise search for a match.
 */
	for (p = desc->ao_opttop; p; p = p->aoo_next) {

		if (strcmp(p->aoo_string, optstr) == 0) {
			desc->ao_optcur = p;
			break;
		}
	}

	if (p == 0) errno = EINVAL;

	return(p);
}


/*
 *	lam_strtonum
 *
 *	Function:	- convert string to number if possible
 *			- handles decimal/octal/hexadecimal
 *			- uses strtol()
 *	Accepts:	- string
 *			- ptr number (returned value)
 *	Returns:	- 0 or LAMERROR
 */
static int
lam_strtonum(const char *str, int *pnum)
{
	char		*endstr;		/* end of parsed string */

	*pnum = (int) strtol(str, &endstr, 0);

	if (*endstr) {
		errno = EBADASCIINUMB;
		return(LAMERROR);
	}

	return(0);
}


/*
 *	mutexchk
 *
 *	Function:	- check mutex validity
 *			- if any other taken, it's an error
 *	Accepts:	- ptr option
 *	Returns:	- 0 or LAMERROR
 */
static int
mutexchk(struct ao_opt *opt)
{
	struct ao_opt	*p;			/* favourite pointer */

	for (p = opt->aoo_mutex; p && (p != opt); p = p->aoo_mutex) {
		if (p->aoo_flags & AOTAKEN) return(LAMERROR);
	}

	return(0);
}


/*
 *	countparam
 *
 *	Function:	- get # parameters following option
 *			- loop counting till 1st valid option
 *	Accepts:	- ptr option descriptor
 *			- ptr option
 *			- ptr remaining argv
 *	Returns:	- # parameters or LAMERROR
 */
static int
countparam(OPT *aod, struct ao_opt *opt, char **argv)
{
	int		n;			/* counter */
	int		num;			/* junk number */
	char		*string;		/* arg. string */
/*
 * Loop counting till first valid option or end-of args.
 */
	for (n = 0; *argv && (n != opt->aoo_nmaxparam); ++n, ++argv) {

		string = *argv;
		if (*string != '-') continue;

		if (strcmp(string, "--") == 0) break;

		if ((lam_strtonum(string + 1, &num) == 0) &&
				(findopt(aod, "#") != 0)) break;

		if (findopt(aod, string + 1)) break;
	}
/*
 * If expected fixed number not matching, it's an error.
 */
	if ((opt->aoo_nmaxparam > 0) && (n != opt->aoo_nmaxparam)) {
		errno = EUSAGE;
		return(LAMERROR);
	}

	return(n);
}


/*
 *	addinst
 *
 *	Function:	- add instance parameters to option
 *			- convert integer parameters if needed
 *	Accepts:	- ptr option
 *			- # parameters to add
 *			- parameter argv
 *	Returns:	- 0 or LAMERROR
 */
static int
addinst(struct ao_opt *opt, int nparam, char **params)
{
	int		fl_int;			/* integer parameters */
	int		num;			/* parsed number */
	unsigned	size;			/* used for malloc() */
	struct ao_inst	*pinst;			/* ptr instance */
	char		*parmbuf;		/* tmp param buffer */

	if (nparam == 0) {
		++(opt->aoo_ninst);
		return(0);
	}

	fl_int = opt->aoo_flags & AOINT;
/*
 * Allocate the parameter buffer.
 */
	size = (fl_int) ? sizeof(int) : sizeof(char *);

	parmbuf = (char*) malloc(size * nparam);

	if (parmbuf == 0) return(LAMERROR);
/*
 * Allocate or expand instance array.
 */
	size = sizeof(struct ao_inst);

	if (opt->aoo_insts == 0) {
		pinst = (struct ao_inst *) malloc(size);
	} else {
		pinst = (struct ao_inst *) realloc((char *) opt->aoo_insts,
						(opt->aoo_ninst + 1) * size);
	}

	if (pinst == 0) {
		free((char *) parmbuf);
		return(LAMERROR);
	}

	opt->aoo_insts = pinst;
	pinst += (opt->aoo_ninst)++;
/*
 * Add the parameters, converting integers if needed.
 */
	size = (fl_int) ? sizeof(int) : sizeof(char *);

	pinst->aoi_nparam = nparam;
	pinst->aoi_params = (void *) parmbuf;

	for (; nparam > 0; --nparam, ++params, parmbuf += size) {

		if (fl_int) {
			if (lam_strtonum(*params, &num)) {
				errno = EUSAGE;
				return(LAMERROR);
			}

			*((int *) parmbuf) = num;
		}
		else {
			*((char **) parmbuf) = *params;
		}
	}

	return(0);
}
