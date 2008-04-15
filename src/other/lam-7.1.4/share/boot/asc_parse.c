/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * Id: asc_parse.c,v 6.33 2004/01/09 18:03:34 jsquyres Exp $
 *
 *	Function:	- application schema parsing
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include <all_list.h>
#include <all_opt.h>
#include <args.h>
#include <app_schema.h>
#include <lam.h>
#include <ndi.h>
#include <sfh.h>
#include <lamdebug.h>

/*
 * local functions
 */
static int		parseline(LIST *, char *, int, char **);
static int		add_var(int *, char ***, char *);

/*
 *	asc_parse
 *
 *	Function:	- parse an application schema
 *			- returns line # in case of parsing error
 *	Accepts:	- app. schema file 
 *			- ptr line # (returned value)
 *	Returns:	- app. schema descriptor or NULL
 */
LIST *
asc_parse(const char *appfile, int *plineno, char **env)
{
	int		fd;			/* file desc. */
	int		nbytes;			/* # bytes read */
	int		bufsize;		/* buffer size */
	char		*filebuf;		/* file buffer */
	struct stat	fileinfo;		/* file information */
	LIST		*appd;			/* app. schema desc. */
/*
 * Get file info.
 */
	if (stat(appfile, &fileinfo)) return((LIST *) 0);

	bufsize = (int) fileinfo.st_size;
	if (bufsize <= 0) return((LIST *) 0);
/*
 * Allocate a file buffer.
 */
	filebuf = malloc((unsigned) bufsize);
	if (filebuf == 0) return((LIST *) 0);
/*
 * Open and read the file.
 */
	fd = open(appfile, O_RDONLY, 0);
	if (fd < 0) {
		free(filebuf);
		return((LIST *) 0);
	}

	nbytes = read(fd, filebuf, bufsize);
	close(fd);

	if (nbytes < bufsize) {
		if (nbytes >= 0) errno = EIO;
		free(filebuf);
		return((LIST *) 0);
	}
/*
 * Parse the app. schema buffer.
 */
	appd = asc_bufparse(filebuf, bufsize, plineno, env);

	free(filebuf);
	return(appd);
}

/*
 *	asc_bufparse
 *
 *	Function:	- parse an application schema from a buffer
 *			- returns line # in case of parsing error
 *	Accepts:	- buffer containing the app. schema
 *			- buffer size
 *			- ptr line # (returned value)
 *	Returns:	- app. schema descriptor or NULL
 */
LIST *
asc_bufparse(char *appbuf, int bufsize, int *pline, char **env)
{
	int		linesize;		/* size of parsed line */
	int		fl_comment;		/* comment mode flag */
	int		i;			/* favourite index */
	char		*p;			/* favourite pointer */
	LIST		*appd;			/* app. schema desc. */
/*
 * File must be pure ASCII.
 */
	*pline = 0;

	for (i = 0, p = appbuf; i < bufsize; ++i, ++p) {

		if ( ! isascii(*p)) {
			errno = ENOEXEC;
			return(0);
		}
	}
	
	appd = al_init(sizeof(struct aschema), (int (*)()) 0);
	if (appd == 0) return(0);
/*
 * Loop over the app. schema lines.
 */
	*pline = 1;
	fl_comment = 0;

	for ( ; bufsize > 0; ++appbuf, --bufsize) {
/*
 * Handle newline characters.
 */
		if (*appbuf == '\n') {
			++(*pline);
			fl_comment = 0;
		}
/*
 * Skip leading spaces and comments.
 */
		else if (isspace((int) *appbuf) || fl_comment) {
		}
/*
 * Skip comments.
 */
		else if (*appbuf == '#') {
			fl_comment = 1;
/*
 * Reject scripts.
 */
			if (*pline == 1 && bufsize > 2 && *(appbuf+1) == '!') {
				return(0);
			}
		}
/*
 * Parse the line.
 */
		else {
			linesize = parseline(appd, appbuf, bufsize, env);

			if (linesize < 0) {
				asc_free(appd);
				appd = 0;
				bufsize = 0;
			} else if (--linesize >= 0) {
				bufsize -= linesize;
				appbuf += linesize;
			}
		}
	}

	return(appd);
}

/*
 *	parseline
 *
 *	Function:	- parse a single app. schema line
 *			- update the app. schema desc.
 *	Accepts:	- ptr app. schema desc.
 *			- line buffer
 *			- buffer size
 *			- top level environment (if any)
 *	Returns:	- parsed line size or LAMERROR
 */
static int
parseline(LIST *appd, char *buf, int bufsize, char **env)
{
  int		lsize;			/* line size */
  int i;
  char          *cmdline;               /* command line */
  char          *progname;              /* program name for this line */
  char          *p;                     /* favorite pointer */
  int           orig_cmdc;              /* Original cmdc / cmdv */
  char **       orig_cmdv;              /* from buf */
  int		cmdc;			/* argv/argc faking */
  char **	cmdv;			
  int           tmp_cmdc;               /* tmp cmd line info */
  char **	tmp_cmdv;
  struct apparg * procargv;		/* process argv */
  struct appenv * procenv;		/* process env */
  struct aschema proc;			/* process list entry */
  OPT *ad;                              /* Options for app schema */

  /* prep the proc description */
  LAM_ZERO_ME(proc);
  proc.asc_errno = 0;
  proc.asc_proc_cnt = -1;
  proc.asc_srcnode = -1;
  proc.asc_node = -1;

  /* 
   * Set up the options.  Do early for asc_compat()
   */
  ad = ao_init();
  ao_setopt(ad, "c", 0, 1, AOINT);
  ao_setopt(ad, "np", "c", 1, AOINT);
  ao_setopt(ad, "s", 0, 1, 0);
  ao_setopt(ad, "x", 0, 1, 0);
  ao_setopt(ad, "wd", 0, 1, 0);

  /* Copy the line to a temporary buffer. */
  p = strchr(buf, '\n');
  lsize = (p == 0) ? bufsize : (int) (p - buf);

  /*
   * Allocate one extra char for terminator and 2 extra so we can put
   * a fake argv[0] at the start and then use the usual parsing
   * routines.  
   */
  cmdline = malloc((unsigned) lsize + 1 + 2);
  if (cmdline == 0) {
    ao_free(ad);
    return(LAMERROR);
  }

  /*
   * Insert the fake argv[0] and then copy the line.
   */
  strcpy(cmdline, "x ");
  memcpy(cmdline + 2, buf, lsize);
  cmdline[lsize + 2] = '\0';

  /*
   * Split it into a command line argv.  Don't free to the blody end,
   * as all the code used believes that we are using the argv/argc
   * from main, which means they expect it to be there until end of
   * time.  Many moons wasted on this. 
   */
  orig_cmdv = sfh_argv_break_quoted(cmdline, ' ', "\\'\"");
  free(cmdline);
  if (orig_cmdv == 0) {
    ao_free(ad);
    return(LAMERROR);
  }
  orig_cmdc = sfh_argv_count(orig_cmdv);
  cmdv = orig_cmdv;
  cmdc = orig_cmdc;

  /*
   * Put in -- form
   */
  if (asc_compat(&cmdc, &cmdv, ad)) {
    ao_free(ad);
    sfh_argv_free(orig_cmdv);
    return(LAMERROR);
  }

  /*
   * Parse the options.  We used to do this by hand, which seemed
   * crazy just to avoid pulling in all_opt.  Maybe back when this
   * thing was actually running on a multicomputer...  
   */
  if (ao_parse(ad, &cmdc, cmdv)) {
    if (orig_cmdv != cmdv) {
      sfh_argv_free(orig_cmdv);
    }
    sfh_argv_free(cmdv);
    ao_free(ad);
    return LAMERROR;
  }

  /*
   * Parse the node identifiers.  Would be easier if ndi_parse removed
   * from the array like everyone else... 
   */
  proc.asc_nodelist = ndi_parse(cmdc, cmdv, &tmp_cmdv);
  if (proc.asc_nodelist == 0) {
    if (orig_cmdv != cmdv) {
      sfh_argv_free(orig_cmdv);
    }
    sfh_argv_free(cmdv);
    return(LAMERROR);
  }

  /* Fix up cmdv and cmdc - from here on, cmdv and orig_cmdv are different*/
  if (orig_cmdv != cmdv) {
#if 0  /* BWB: Fix me later */
    sfh_argv_free(cmdv);
#endif
  }
  cmdv = tmp_cmdv;
  cmdc = sfh_argv_count(cmdv);
  tmp_cmdv = 0;

  /*
   * We should be left with the fake command name and the single
   * program name.  Copy name into progname
   */
  if (sfh_argv_count(cmdv) != 2) {
    sfh_argv_free(orig_cmdv);
    sfh_argv_free(cmdv);
    errno = EUSAGE;
    return(LAMERROR);
  }
  progname = strdup(cmdv[1]);
#if 0 /* BWB: Fix me later */
  sfh_argv_free(cmdv);
#endif
  cmdc = 0;
  cmdv = 0;
  tmp_cmdc = 0;
  tmp_cmdv = 0;

  /*
   * Start filling in proc structure.
   */
  if (ao_taken(ad, "s")) {
    proc.asc_srcnode = ndi_parse1(ao_param(ad, "s", 0, 0));
    if ((proc.asc_srcnode < 0)
	&& (proc.asc_srcnode != LOCAL)) {
      ao_free(ad);
      sfh_argv_free(orig_cmdv);
      return(LAMERROR);
    }
  }

  if (ao_taken(ad, "c")) {
    ao_intparam(ad, "c", 0, 0, &(proc.asc_proc_cnt));
  } else if (ao_taken(ad, "np")) {
    ao_intparam(ad, "np", 0, 0, &(proc.asc_proc_cnt));
  }

  /*
   * Build the process's argv structure.
   */
  procargv = (struct apparg *) malloc(sizeof(struct apparg));
  if (procargv == 0) {
    ao_free(ad);
    sfh_argv_free(orig_cmdv);
    return(LAMERROR);
  }

  procargv->apa_argc = 0;
  procargv->apa_argv = 0;
  procargv->apa_refcount = 1;

  /* build argument list */
  if (ao_tail(ad, &tmp_cmdc, &tmp_cmdv)) {
    ao_free(ad);
    sfh_argv_free(orig_cmdv);
  }

  sfh_argv_add(&(procargv->apa_argc), &(procargv->apa_argv), progname);
  for (i = 0 ; i < tmp_cmdc ; ++i) {
    sfh_argv_add(&(procargv->apa_argc), &(procargv->apa_argv), tmp_cmdv[i]);
  }
  /* No need to free tmp_cmdv, as it is just a pointer into orig_cmdv */
  tmp_cmdv = 0;
  tmp_cmdc = 0;
  free(progname);

  /* degenerate case - but it would be bad not to double check */
  if (procargv->apa_argv == 0) {
    ao_free(ad);
    sfh_argv_free(orig_cmdv);
    free(procargv);
    return(LAMERROR);
  }

  proc.asc_args = procargv;

  /*
   * Build the process's env structure.
   */
  procenv = (struct appenv *) malloc(sizeof(struct appenv));
  if (procenv == 0) {
    ao_free(ad);
    sfh_argv_free(orig_cmdv);
    sfh_argv_free(procargv->apa_argv);
    free(procargv);
    return(LAMERROR);
  }

  procenv->ape_refcount = 1;
  procenv->ape_envv = 0;
  procenv->ape_wrkdir = 0;
  
  if (env) {
    if ((procenv->ape_envv = sfh_argv_dup(env)) == 0) {
      ao_free(ad);
      sfh_argv_free(orig_cmdv);
      sfh_argv_free(procargv->apa_argv);
      free(procargv);
      free(procenv);
      return(LAMERROR);
    }
  }

  if (ao_taken(ad, "x")) {
    /*
     * Add env vars specified by -x options on this line.
     */
    if (asc_environment(0, ao_param(ad, "x", 0, 0), &procenv->ape_envv)) {
      ao_free(ad);
      sfh_argv_free(orig_cmdv);
      sfh_argv_free(procargv->apa_argv);
      free(procargv);
      sfh_argv_free(procenv->ape_envv);
      free(procenv);
    }
  }

  if (ao_taken(ad, "wd")) {
    procenv->ape_wrkdir = strdup(ao_param(ad, "wd", 0, 0));
    if (procenv->ape_wrkdir == 0) {
      ao_free(ad);
      sfh_argv_free(orig_cmdv);
      sfh_argv_free(procargv->apa_argv);
      free(procargv);
      sfh_argv_free(procenv->ape_envv);
      free(procenv);
    }
  }

  proc.asc_env = procenv;

  /*
   * Add ourselves to the list
   */
  if (al_append(appd, &proc) == 0) {
    sfh_argv_free(procargv->apa_argv);
    free(procargv);
    if (procenv->ape_wrkdir) {
      free(procenv->ape_wrkdir);
    }
    sfh_argv_free(procenv->ape_envv);
    free(procenv);
    return(LAMERROR);
  }

  /*
   * all done... Clean up
   */
  ao_free(ad);
#if 0 /* BWB: Fix me later */
  sfh_argv_free(orig_cmdv);
#endif

  return(lsize);
}

/*
 *	asc_free
 *
 *	Function:	- free an application schema descriptor
 *	Accepts:	- app. schema desc.
 */
void
asc_free(LIST *appd)
{
	struct aschema	*pp;			/* ptr process entry */
	struct apparg	*pa;			/* ptr process args */
	struct appenv	*pe;			/* ptr process env */

	if (appd == 0) return;

	for (pp = (struct aschema *) al_top(appd); pp;
			pp = (struct aschema *) al_next(appd, pp)) {

		if (pp->asc_nodelist) {
			al_free(pp->asc_nodelist);
		}

		pa = pp->asc_args;
		if (--pa->apa_refcount == 0) {
			sfh_argv_free(pa->apa_argv);
			free((char *) pa);
		}

		pe = pp->asc_env;
		if (--pe->ape_refcount == 0) {
		  sfh_argv_free(pe->ape_envv);
		  if (pe->ape_wrkdir) {
		    free(pe->ape_wrkdir);
		  }
		  free((char *) pe);
		}
	}

	al_free(appd);
}


/*
 *	asc_compat
 *
 *	Function:	- convert argv into "--" style argv
 *	Accepts:	- argument count (inout)
 *			- argument vector (inout)
 *                      - options descriptor (to parse argv)
 *	Returns:	- 0 (success) or -1 (error)
 *
 *      Notes: - If we have to modify the argv array, we reallocate a
 *               new one, since uargv may be a pointer to the argv 
 *               array for main sitting on the stack and modifying 
 *               that would be bad.  This also means that we can't
 *               free the old argv, so you should if appropriate.
 */
int
asc_compat(int *uargc, char ***uargv, OPT *aod)
{
    int			argc = *uargc;
    char		**argv = *uargv;
    int 		newargc = 0;		/* new argument count */
    char		**newargv = 0;		/* new argument vector */
    int			i,j;
    int                 nparams;

    /*
     * If there is already a "--" or a "-c" option then there is
     * nothing to do.  
     */
    for (i = 1; i < argc; i++) {
	if (strcmp(argv[i], "--") == 0) {
	    return(0);
	}
    }

    /*
     * Add the command name to the new command line.
     */
    if (sfh_argv_add(&newargc, &newargv, argv[0])) {
	return(-1);
    }

    /*
     * iterate over arguments until something not in aod or a node
     * specifier is found.  Assume that is the binary or app schema
     * name.
     */
    for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-' && 
	  strlen(argv[i]) > 1 && 
	  ao_exam_isopt(aod, argv[i] + 1)) {
	/*
	 * It's an option 
	 */
	ao_exam_nparams(aod, argv[i] + 1, &nparams);

	/* add the option */
	if (sfh_argv_add(&newargc, &newargv, argv[i]))
	  return -1;

	/* Eat the params to the option.  If this is a variable, eat
	 * until the next thing that could be an option (starts with
	 * -, which handles the -- case).  This isn't perfect, but
	 * then options parsing shouldn't require an AI to figure out
	 * what the user wanted. */
	if (nparams != AOVARNUM) {
	  for (j = 0 ; j < nparams ; ++j) {
	    /* bounds checking is good */
	    if (i + 1 >= argc) 
	      return -1;

	    if (sfh_argv_add(&newargc, &newargv, argv[i + 1]))
	      return -1;
	    ++i;
	  }
	} else {
	  /* Note: This case is not well tested, as it is not
	   * currently used anywhere in the LAM/MPI tree.  But it
	   * might be some day! */
	  while (i + 1 < argc && (argv[i + 1])[0] != '-') {
	    if (sfh_argv_add(&newargc, &newargv, argv[i + 1]))
	      return -1;
	    ++i;
	  }
	}

      } else if (ndi_parse1(argv[i]) != NOTNODEID) {
	/*
	 * It's a node identifier.  
	 */
	if (sfh_argv_add(&newargc, &newargv, argv[i])) {
	  return(-1);
	}
      } else {
	/*
	 * Must now have the binary or application schema name.
	 * Append it to the new argument vector and then if there are
	 * trailing arguments add a "--" option.
	 */
	    if (sfh_argv_add(&newargc, &newargv, argv[i])) {
		return(-1);
	    }	

	    if (i + 1 < argc) {
		if (sfh_argv_add(&newargc, &newargv, "--")) {
		    return(-1);
		}
	    }

	    i++;
	    break;
	}
    }
/*
 * Append user program arguments to the new argument vector.
 */
    for (; i < argc; i++) {
	if (sfh_argv_add(&newargc, &newargv, argv[i])) {
	    return(-1);
	}
    }

    *uargc = newargc;
    *uargv = newargv;

    return(0);
}

/*
 *	asc_environment
 *
 *	Function:	- build job environment
 *	Accepts:	- export all LAM_MPI_* vars?
 *			- explicit list of variables to export
 *			- environment (inout)
 *	Returns:	- 0 or LAMERROR
 */
int
asc_environment(int exportme, char *varlist, char ***env)
{
	extern char	**environ;		/* current environment */
	char		**p;
	char		*v;
	int		nenv;
        char bad_string[] = "LAM_MPI_SESSION_PREFIX=";

	nenv = sfh_argv_count(*env);

	if (exportme)
/*
 * Export all environment variables of form LAM_MPI_* or LAM_IMPI_* or 
 * IMPI_*.  Special case: do *not* export LAM_MPI_SESSION_PREFIX.
 */
		for (p = environ; p && *p; p++) {
		  if (strncmp(*p, "LAM_MPI_", 8) == 0 &&
                      strncmp(*p, bad_string, sizeof(bad_string) - 1) != 0) {
		    if (sfh_argv_add(&nenv, env, *p))
		      return LAMERROR;
		  }
#if LAM_WANT_IMPI
		  else if (strncmp(*p, "LAM_IMPI_", 8) == 0) {
		    if (sfh_argv_add(&nenv, env, *p))
		      return LAMERROR;
		  }
		  else if (strncmp(*p, "IMPI_", 5) == 0) {
		    if (sfh_argv_add(&nenv, env, *p))
		      return LAMERROR;
		  }
#endif
/*
 * Also export the PBS_ENVIRONMENT and PBS_JOBID vars, if they exist.
 * This allows us to use the right unix socket names on the other side.
 */
		  else if (strncmp(*p, "PBS_ENVIRONMENT=", 16) == 0) {
		    if (sfh_argv_add(&nenv, env, *p))
		      return LAMERROR;
		  }
		  else if (strncmp(*p, "PBS_JOBID=", 10) == 0) {
		    if (sfh_argv_add(&nenv, env, *p))
		      return LAMERROR;
		  }
		}
/*
 * Export explicitly specified variables.
 */
	if (varlist) {
		v = varlist + strlen(varlist);

		while (v != varlist) {
			/* search backwards for , */
			for ( ; *v != ',' && v != varlist; v--) ;

			if (v != varlist) {
				if (*(v-1) != '\\') {
					/* it's not escaped so we have a var */
					if (add_var(&nenv, env, v+1))
						return(LAMERROR);
					*v = 0;
				}
			
				v--;
			}
		}

		/* add first var */
		if (add_var(&nenv, env, varlist))
			return(LAMERROR);
	}

	return 0;
}

/*
 *	add_var
 *
 *	Function:	- add variable to environment vector
 *	Accepts:	- length of environment vector (inout)
 *			- environment vector (inout)
 *			- variable
 *	Returns:	- 0 or LAMERROR
 */
static int
add_var(int *nenv, char ***env, char *var)
{
	char		*val;
	char		*v;
	int		len;
	int		i, j;

	len = strlen(var);
	if (len == 0)
		return 0;
/*
 * Copy removing any escapes.
 */
	v = malloc(len+1);

	for (i = 0, j = 0; i < len; i++) {
		if (var[i] != '\\' || var[i+1] != ',') {
			v[j++] = var[i];
		} else {
			v[j++] = ',';
			i++;
		}
	}
	v[j] = 0;
/*
 * If there is an equals the variable's value has been given.
 */
	if (strchr(v, '=')) {
		if (sfh_argv_add(nenv, env, v))
			return LAMERROR;
	} else {
/*
 * Get value from the current environment.
 */
		if ((val = getenv(v))) {
			if ((var = malloc(strlen(v) + strlen(val) + 2)) == 0)
				return LAMERROR;
			strcpy(var, v);
			strcat(var, "=");
			strcat(var, val);

			if (sfh_argv_add(nenv, env, var))
				return LAMERROR;
			free(var);
		}
	}

	free(v);
	return 0;
}
