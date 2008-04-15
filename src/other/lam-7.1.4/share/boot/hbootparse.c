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
 * $Id: hbootparse.c,v 6.7 2004/01/02 00:00:06 jsquyres Exp $
 * 
 *	Function:	- vanilla boot config parser
 */

#include <lam_config.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <all_list.h>
#include <all_opt.h>
#include <args.h>
#include <boot.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <proc_schema.h>
#include <sfh.h>

extern char **environ;

static char *conffile;
static char **av_psc_path;
static int ac_psc_path;

/*
 * local functions
 */
static void hboot_setdefaults(OPT *ad);

static void setvars(int *ac, char ***av, char *var, char *opt);


/*
 * INPUT:
 *  inet_topo  - string for inet_topo (may be NULL)
 *  rtr_topo   - string for rtr_topo (may be NULL - usually NULL)
 * OUTPUT:
 *  proc_list  - list of struct psc's  - must be al_free'ed later
 * RETURNS:
 *  0          - success
 *  LAMERROR   - badness has occurred
 */
int
hbootparse(int debug, OPT *ad, 
	   char *inet_topo, char *rtr_topo, LIST **proc_list)
{
  FILE *conf_fp;
  int ac_topo = 0;	  /* # topology variable overrides */
  char **av_topo;         /* topology variable overrides */
  LIST *list_psc;
  struct psc *p;
  int prefix_len;
  int suffix_len;
  int pathprefix_len;
  int fl_prefix;
  char *opt_prefix;
  char *opt_suffix;
  char *opt_pathprefix;
  char *prefix;
  char *suffix;
  char *pathprefix = NULL;

  hboot_setdefaults(ad);
  fl_prefix = ao_taken(ad, "prefix");

  /* Locate the conf file if prefix not provided */
  if (!fl_prefix) {
    conffile = sfh_path_findv(conffile, av_psc_path, R_OK, environ);
    if (conffile == NULL) {
      fprintf(stderr, "base: cannot find process schema %s: ",
	      conffile);
      perror("");
      return LAMERROR;
    }
  }
 
  /* Open the conf file. */
  conf_fp = fopen(conffile, "r");
  if (conf_fp == NULL) {
    fprintf(stderr, "base: cannot open %s: ", conffile);
    perror("");
    return LAMERROR;
  }
  
  /* Handle command line override of variables. */
  ac_topo = 0;
  av_topo = 0;

  if (inet_topo != NULL)
    setvars(&ac_topo, &av_topo, "$inet_topo", inet_topo);

  if (rtr_topo != NULL)
    setvars(&ac_topo, &av_topo, "$rtr_topo", rtr_topo);

  if (ao_taken(ad, "sessionprefix")) {
    opt_prefix = ao_param(ad, "sessionprefix", 0, 0);
    prefix_len = 24 + strlen(opt_prefix);
    prefix = (char*) malloc(sizeof(char) * prefix_len);
    snprintf(prefix, prefix_len, "-sessionprefix %s", opt_prefix);
    setvars(&ac_topo, &av_topo, "$session_prefix", prefix);
    free(prefix);
  } else {
    setvars(&ac_topo, &av_topo, "$session_prefix", "");
  }

  if (ao_taken(ad, "sessionsuffix")) {
    opt_suffix = ao_param(ad, "sessionsuffix", 0, 0);
    suffix_len = 24 + strlen(opt_suffix);
    suffix = (char*) malloc(sizeof(char) * suffix_len);
    snprintf(suffix, suffix_len, "-sessionsuffix %s", opt_suffix);
    setvars(&ac_topo, &av_topo, "$session_suffix", suffix);
    free(suffix);
  } else {
    setvars(&ac_topo, &av_topo, "$session_suffix", "");
  }

  if (ao_taken(ad, "d")) {
    setvars(&ac_topo, &av_topo, "$debug", "-d");
  } else {
    setvars(&ac_topo, &av_topo, "$debug", "");
  }

  if (fl_prefix) {
    opt_pathprefix = ao_param(ad, "prefix", 0, 0);

    /* make path for psc_fill to find lamd */
    pathprefix_len = strlen(opt_pathprefix) + strlen("/bin/") + 1;
    pathprefix = (char *) malloc (pathprefix_len * sizeof(char));
    if (pathprefix == NULL) {
      show_help(NULL, "lib-call-fail", "malloc", NULL);
      return LAMERROR;
    }
    snprintf(pathprefix, pathprefix_len, "%s/bin/", opt_pathprefix);
  }

  /* Parse the conf file. */
  if (psc_parse(conf_fp, proc_list, av_topo)) {
    show_help("hboot", "psc-parse", conffile, NULL);
    return LAMERROR;
  }

  list_psc = *proc_list;

  sfh_argv_free(av_topo);
  fclose(conf_fp);

  if (al_count(list_psc) == 0) {
    show_help("hboot", "nothing-to-do", conffile, NULL);
    return LAMERROR;
  }

  /* Find programs with sufficient permissions (adds full length...)  */

  if (fl_prefix) {
    p = psc_fill(list_psc, pathprefix);
    free(pathprefix);
  }  
  else
    p = psc_find(list_psc);

  if (p) {
    show_help("hboot", "cant-find-executables", p->psc_argv[0]);
    exit(errno);
  }

  proc_list = &list_psc;
  
  return 0;
}


/*
 *	setdefaults
 *
 *	Function:	- sets default files and paths
 */
static void
hboot_setdefaults(OPT *ad)
{
  /* prefix paths */

  ac_psc_path = 0;
  av_psc_path = 0;
  sfh_argv_add(&ac_psc_path, &av_psc_path, "");
  sfh_argv_add(&ac_psc_path, &av_psc_path, "$LAMHOME/etc");
  sfh_argv_add(&ac_psc_path, &av_psc_path, "$TROLLIUSHOME/etc");
  sfh_argv_add(&ac_psc_path, &av_psc_path, LAM_SYSCONFDIR);

  /* files */

  if (ao_taken(ad, "c")) {
    conffile = ao_param(ad, "c", 0, 0);
  } else {
    conffile = DEFFCONFIGH;
  }
}


/*
 *	setvars
 *
 *	Function:	- handles command line override of variables
 *			- vars: $inet_topo, $rtr_topo
 */
static void
setvars(int *ac, char ***av, char *var, char *opt)
{
  char *override;

  override = malloc((unsigned) (strlen(var) + strlen(opt) + 2));
  if (override == 0) {
    show_help(NULL, "lib-call-fail", "malloc", NULL);
    exit(errno);
  }

  strcpy(override, var);
  strcat(override, " ");
  strcat(override, opt);

  if (sfh_argv_add(ac, av, override)) {
    show_help(NULL, "lib-call-fail", "sfh_argv_add", NULL);
    exit(errno);
  }

  free(override);
}
