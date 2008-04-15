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
 * $Id: pathsearch.c,v 6.7 2003/07/05 21:52:21 jsquyres Exp $
 *
 *	Function:	- file searching
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <args.h>
#include <laminternal.h>
#include <sfh.h>

/*
 * PATH environment variable separator
 */
#ifdef WIN32
#define PATHENVSEP	';'
#else
#define PATHENVSEP	':'
#endif

static void		path_env_load(char *, int *, char ***);
static char		*path_access(char *, char *, int);
static char 		*list_env_get(char *, char **);

/*
 *	sfh_path_findv
 *
 *	Function:	- locates a file with certain permissions
 *			- environment variable can appear in the form
 *			  $variable at the start of a prefix path
 *			  and will be replaced by the environment value
 *			  if it is defined otherwise the whole prefix
 *			  is ignored
 *			- environment variable must be following by
 *			  a path delimiter or end-of-string
 *
 *	Accepts:	- file name
 *			- array of search directories
 *			- target permissions which must be satisfied
 *
 *	Returns:	- full pathname of located file or NULL
 */
char *
sfh_path_findv(char *fname, char **pathv, int mode, char **envv)
{
    char		*fullpath;	/* full pathname of search file */
    char		*delimit;	/* ptr to first delimiter in prefix */
    char		*env;		/* ptr to environment var */
    char		*pfix;		/* prefix directory */
    int			i;
/*
 * If absolute path is given, return it without searching.
 */
    if (*fname == STRDIR) {
	return(path_access(fname, "", mode));
    }
/*
 * Initialize.
 */
    fullpath = 0;
    i = 0;
/*
 * Consider each directory until the file is found.
 * Thus, the order of directories is important.
 */
    while (pathv[i] && !fullpath) {
/*
 * Replace environment variable at the head of the string.
 */
	if (*pathv[i] == '$') {
	    delimit = strchr(pathv[i], STRDIR);

	    if (delimit) {
		*delimit = '\0';
	    }

	    env = list_env_get(pathv[i]+1, envv);

	    if (delimit) {
		*delimit = STRDIR;
	    }

	    if (env) {

		if (!delimit) {
		    fullpath = path_access(fname, env, mode);
		} else {
		    pfix = malloc((unsigned) strlen(env) + strlen(delimit) + 1);
		    if (pfix == 0) return(0);

		    strcpy(pfix, env);
		    strcat(pfix, delimit);
		    fullpath = path_access(fname, pfix, mode);
		    free(pfix);
		}
	    }
	}

	else {
	    fullpath = path_access(fname, pathv[i], mode);
	}

	i++;
    }

    return(fullpath);
}

char *
sfh_path_find(char *fname, char **pathv, int mode)
{
    return(sfh_path_findv(fname, pathv, mode, 0));
}

/*
 *	sfh_path_env_findv
 *
 *	Function:	- locates a file with certain permissions
 *			  from the list of paths given by the $PATH
 *			  environment variable
 *			- replaces ./ of found path with working dir
 *	Accepts:	- file name
 *			- target permissions which must be satisfied
 *			- environment list
 *			- working directory
 *	Returns:	- full pathname of located file or NULL
 */
char *
sfh_path_env_findv(char *fname, int mode, char **envv, char *wrkdir)
{
    char		**dirv;		/* search directories */
    char		*fullpath;	/* full pathname */
    char		*path;		/* value of PATH */
    int			dirc;		/* # search directories */
    int			i;
    int                 found_dot = 0;

/*
 * Set the local search paths.
 */
    dirc = 0;
    dirv = 0;

    /*
     * If there is anything that looks like a path in the fname, put
     * the current working directory (if we have one) first.  Hack up
     * the path, because there is really no way to prepend to the argv
     * using the sfh_argv code.
     */
    if (strchr(fname, STRDIR) != NULL && wrkdir != NULL) {
      sfh_argv_add(&dirc, &dirv, wrkdir);
    }

    if ((path = list_env_get("PATH", envv))) {
        path_env_load(path, &dirc, &dirv);
    }
/*
 * Replace the "." path by the working directory.
 */
    for (i = 0; i < dirc; ++i) {
	if (strcmp(dirv[i], ".") == 0 && wrkdir) {
	    found_dot = 1;
	    free(dirv[i]);
	    dirv[i] = strdup(wrkdir);
	    if (dirv[i] == 0) return(0);
	}
    }

/*
 * If we didn't find "." in the path and we have a wrkdir, append
 * the wrkdir to the end of the path
 */
    if (!found_dot && wrkdir) {
	    sfh_argv_add(&dirc, &dirv, wrkdir);
    }

    fullpath = sfh_path_findv(fname, dirv, mode, envv);
    sfh_argv_free(dirv);
    return(fullpath);
}

char *
sfh_path_env_find(char *fname, int mode)
{
    char		*cwd;
    char		*r;

    cwd = getworkdir();
    r = sfh_path_env_findv(fname, mode, 0, cwd);
    if (cwd)
	free(cwd);

    return(r);
}

/*
 *	path_access
 *
 *	Function:	- forms a complete pathname and checks it for
 *			  existance and permissions
 *	Accepts:	- filename
 *			- path prefix
 *			- permissions which must be satisfied
 *	Returns:	- full pathname or NULL
 */
static char *
path_access(char *fname, char *path, int mode)
{
    char		*fullpath;	/* full pathname of search file */
/*
 * Allocate space for the full pathname.
 */
    fullpath = malloc((unsigned) strlen(path) + strlen(fname) + 2);
    if (fullpath == 0) return(0);

    if (strlen(path) > 0) {
	strcpy(fullpath, path);
	strcat(fullpath, STRSDIR);
	strcat(fullpath, fname);
    } else {
	strcpy(fullpath, fname);
    }
/*
 * Get status on the full path name to check for existance.
 * Then check the permissions.
 */
    if (access(fullpath, mode)) {
	free(fullpath);
	fullpath = 0;
    }

    return(fullpath);
}

/*
 *	path_env_load
 *
 *	Function:	- loads argument array with $PATH env var
 *	Accepts:	- argv
 *			- argc
 */
static void
path_env_load(char *path, int *pargc, char ***pargv)
{
    char		*p;		/* favourite pointer */
    char		saved;		/* saved character */

    if (path == 0) {
	*pargc =  0;
	return;
    }
/*
 * Loop through the paths (delimited by PATHENVSEP), adding each one to argv.
 */
    while (*path) {
/*
 * Locate the delimiter.
 */
	for (p = path; *p && (*p != PATHENVSEP); ++p);
/*
 * Add the path.
 */
	if (p != path) {
	    saved = *p;
	    *p = '\0';
	    sfh_argv_add(pargc, pargv, path);
	    *p = saved;
	    path = p;
	}
/*
 * Skip past the delimiter, if present.
 */
	if (*path) {
	    ++path;
	}
    }
}

/*
 *	list_env_get
 *
 *	Function:	- get value of variable in list or environment
 *			- looks in list first
 *	Accepts:	- variable
 *			- environment list (null terminated)
 *	Returns:	- pointer to variables value or NULL
 */
static char *
list_env_get(char *var, char **list)
{
    int			n;

    if (list) {
	n = strlen(var);

	while (*list) {
	    if (strncmp(var, *list, n) == 0 && (*list)[n] == '=') {
		return(*list + n+1);
	    }

	    list++;
	}
    }

    return(getenv(var));
}

#if 0
/* Down with backwards compatability! */
/*
 * backwards compatibility
 */
char *findpath(fname, pathv, mode) char *fname, **pathv; int mode;
{ return(sfh_path_find(fname, pathv, mode)); }

char *_path_env_find(fname, mode) char *fname; int mode;
{ return(sfh_path_env_find(fname, mode)); }
#endif
