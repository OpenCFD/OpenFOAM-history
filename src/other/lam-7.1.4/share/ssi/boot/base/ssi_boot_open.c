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
 * $Id: ssi_boot_open.c,v 1.29 2003/12/04 19:54:45 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include <typical.h>
#include <net.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-boot.h>
#include <all_opt.h>
#include <all_list.h>


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * module's public lam_ssi_boot_t struct.
 */

#include "lam-ssi-boot-modules.h"


/*
 * Global variables
 */
lam_ssi_boot_actions_t lam_ssi_boot;
lam_ssi_boot_t lam_ssi_boot_base_module;
int lam_ssi_boot_verbose = -1;
int lam_ssi_boot_did = -1;
OPT *lam_ssi_boot_optd = NULL;
LIST *lam_ssi_boot_base_opened = NULL;
int lam_ssi_boot_base_param_promisc;
int lam_ssi_boot_base_param_window_size;
const lam_ssi_t **lam_ssi_boot_modules = NULL;


/*
 * local variables
 */
static int verbose_param_index = -1;
static int boot_param_index = -1;
static lam_debug_stream_info_t lds = {
  0, -1, -1, NULL, "boot:", -1, -1, -1, -1, "ssi-boot.txt"
};


/*
 * Local functions
 */
static int module_compare(const void *a, const void *b);
static int check_specific_module(OPT *aod, char *name);
static int check_all_modules(OPT *aod);


/*
 * Figure out which boot SSI module to use.  Also determine verbosity
 * level.
 *
 *  Verbosity level:
 *   < 0 - only print things if you really, really have to
 *   = 0 - normal, errors and warnings only, behavior
 *   > 0 - increased verbosity.  Higher the number, the more to print
 */
int
lam_ssi_boot_open(OPT *aod)
{
  int i;
  char *boot;

  lam_ssi_boot_optd = aod;

  /* Register SSI parameter */

  verbose_param_index = 
    lam_ssi_base_param_register_string("boot", "base", "verbose",
                                       "boot_verbose", NULL);
  boot_param_index = 
    lam_ssi_base_param_register_string("boot", NULL, NULL, NULL, NULL);
  lam_ssi_boot_base_param_promisc =
    lam_ssi_base_param_register_int("boot", "base", "promisc", NULL, 
                                    LAM_WANT_BOOT_PROMISC_CONNECTIONS);
  lam_ssi_boot_base_param_window_size =
    lam_ssi_base_param_register_int("boot", "base", "window_size", NULL, 5);

  /* Setup verbosity for this kind */

  lam_ssi_base_set_verbose(verbose_param_index, &lds,
			   &lam_ssi_boot_verbose, &lam_ssi_boot_did);
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "open: opening");
  
  /* Find all available modules */

  lam_ssi_base_module_find(NULL, "boot", 
                           (lam_ssi_t **) lam_ssi_boot_static_modules, 
                           (lam_ssi_t ***) &lam_ssi_boot_modules);

  /* Initialize the list */

  lam_ssi_boot_base_opened = al_init(sizeof(lam_ssi_boot_t*), module_compare);
  if (lam_ssi_boot_base_opened == NULL) {
    show_help(NULL, "system-call-failed", "malloc", NULL);
    return LAMERROR;
  }

  /* See if a specific boot module was requested */
  
  boot = lam_ssi_base_param_lookup_string(boot_param_index);
  if (boot != NULL && strlen(boot) > 0) {
    i = check_specific_module(aod, boot);
    free(boot);
  }

  /* Nope -- a specific one was not selected.  Go choose one. */
  
  else
    i = check_all_modules(aod);
  
  if (i == LAMERROR)
    return LAMERROR;
  
  /* All done */

  return 0;
}


/*
 * Compare two elements in the LIST
 */
static int 
module_compare(const void *a, const void *b)
{
  const lam_ssi_boot_t **aa = (const lam_ssi_boot_t **) a;
  const lam_ssi_boot_t **bb = (const lam_ssi_boot_t **) b;

  return (*aa - *bb);
}


/*
 * A specific module was selected on the command line.  If a module by
 * that name is found, call its open function (if it exists).  If it
 * opens successfully, or if the open function does not exist, save it
 * in the "opened" list for later selection/initialization.
 */
static int
check_specific_module(OPT *aod, char *name)
{
  int i;
  const lam_ssi_t *ls;
  
  if (lam_ssi_boot_verbose > 10)
    lam_debug(lam_ssi_boot_did, "open: looking for boot module named %s", 
              name);
  
  for (i = 0; lam_ssi_boot_modules[i] != NULL; ++i) {
    ls = lam_ssi_boot_modules[i];
    if (strcmp(name, ls->ssi_module_name) == 0) {
      if (lam_ssi_boot_verbose > 10)
	lam_debug(lam_ssi_boot_did, "open: opening boot module %s", 
                  ls->ssi_module_name);
      if (ls->ssi_open_module == NULL ||
	  (ls->ssi_open_module != NULL &&
	   ls->ssi_open_module(aod) == 1)) {

        /* If the open function returns happiness, save this module
           for later querying. */

        if (lam_ssi_boot_verbose > 10)
          lam_debug(lam_ssi_boot_did, "open: opened boot module %s", 
                    ls->ssi_module_name);
        al_insert(lam_ssi_boot_base_opened, &lam_ssi_boot_modules[i]);

        /* If this module didn't add a priority for itself, add one
           with a default priority of 0. */

        if (lam_ssi_base_param_find("boot", (char*) ls->ssi_module_name,
                                    "priority") == LAMERROR)
          lam_ssi_base_param_register_int("boot", (char*) ls->ssi_module_name, 
                                          "priority", NULL, 0);
        break;
      }

      /* Otherwise, this module's init function did not want to run.
	 Abort. */

      else {
        if (lam_ssi_boot_verbose > 10)
          lam_debug(lam_ssi_boot_did, "open: boot module %s did not open",
                    ls->ssi_module_name);
	show_help("boot-ssi", "selected-module-unavailable", name);
        lam_ssi_base_module_registry_unuse((lam_ssi_t*) ls);
	return LAMERROR;
      }
    }
  }

  /* Did we find one? */

  if (lam_ssi_boot_modules[i] == NULL) {
    show_help("ssi", "module-not-found", "boot", name, NULL);
    return LAMERROR;
  }

  return 0;
}


/*
 * No specific module was selected on the command line.  Call open all
 * the available modules (if it exists).  If each module opens
 * successfully, or if its open function does not exist, save it in
 * the "opened" list for later selection/initialization.
 *
 * Return LAMERROR if there are no modules available.
 */
static int
check_all_modules(OPT *aod)
{
  int i, found;
  const lam_ssi_t *ls;

  /* Call the open function in every module and see if they want to
     run */

  for (found = i = 0; lam_ssi_boot_modules[i] != NULL; ++i) {
    ls = lam_ssi_boot_modules[i];
    if (lam_ssi_boot_verbose > 0)
      lam_debug(lam_ssi_boot_did, "open: opening boot module %s", 
                ls->ssi_module_name);

    if (ls->ssi_open_module == NULL ||
	(ls->ssi_open_module != NULL &&
	 ls->ssi_open_module(aod) == 1)) {
      
      /* If the open function returns happiness, save this module
         for later querying. */
      
      if (lam_ssi_boot_verbose > 10)
        lam_debug(lam_ssi_boot_did, "open: opened boot module %s", 
                  ls->ssi_module_name);
      al_insert(lam_ssi_boot_base_opened, &lam_ssi_boot_modules[i]);
      found = 1;

      /* If this module didn't add a priority for itself, add one with
         a default priority of 0. */

      if (lam_ssi_base_param_find("boot", (char*) ls->ssi_module_name,
                                  "priority") == LAMERROR)
        lam_ssi_base_param_register_int("boot", (char*) ls->ssi_module_name, 
                                        "priority", NULL, 0);
    } else {
      if (lam_ssi_boot_verbose >= 10)
        lam_debug(lam_ssi_boot_did, "open: boot moduled did not open: %s",
                  ls->ssi_module_name);
        lam_ssi_base_module_registry_unuse((lam_ssi_t*) ls);
    }
  }

  /* If there were no modules available, it's an error. */

  if (found == 0) {
    if (lam_ssi_boot_verbose > 10)
      lam_debug(lam_ssi_boot_did,
                "open: no boot moduless available top be opened!");
    show_help("ssi-boot", "none-available", NULL);
    return LAMERROR;
  }

  /* All done */

  return 0;
}
