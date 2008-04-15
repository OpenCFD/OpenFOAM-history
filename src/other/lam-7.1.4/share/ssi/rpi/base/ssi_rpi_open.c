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
 * $Id: ssi_rpi_open.c,v 1.32 2003/12/04 19:54:53 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>
#include <stdlib.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <all_list.h>


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * module's public lam_ssi_rpi_t struct.
 */

#include "lam-ssi-rpi-modules.h"


/*
 * Global variables
 */
int lam_ssi_rpi_verbose = -1;
int lam_ssi_rpi_did = -1;
LIST *lam_ssi_rpi_base_opened = NULL;
const lam_ssi_t **lam_ssi_rpi_modules = NULL;


/*
 * Local variables
 */
static int param_verbose = -1;
static int param_rpi = -1;
static lam_debug_stream_info_t lds = {
  0, -1, -1, NULL, "rpi:", -1, -1, -1, -1, "ssi-rpi.txt"
};


/*
 * Local functions
 */
static int module_compare(const void *a, const void *b);
static int check_specific_module(OPT *aod, char *name);
static int check_all_modules(OPT *aod);


/*
 * This is step 1 of 3 in starting up the RPI.  Open up one or more
 * RPI modules and see who wants to be considered for selection.
 */
int
lam_ssi_rpi_base_open(OPT *aod)
{
  int i;
  char *rpi;

  /* Register SSI parameters */

  param_verbose = 
    lam_ssi_base_param_register_string("rpi", "base", "verbose",
                                       "rpi_verbose", NULL);
  param_rpi = lam_ssi_base_param_register_string("rpi", NULL, NULL, 
                                                 NULL, NULL);

  /* Setup verbosity for this kind */

  lam_ssi_base_set_verbose(param_verbose, &lds,
			   &lam_ssi_rpi_verbose, &lam_ssi_rpi_did);
  if (lam_ssi_rpi_verbose >= 1)
    lam_debug(lam_ssi_rpi_did, "open: verbosity:%d", lam_ssi_rpi_verbose);

  /* Find all available modules */

  lam_ssi_base_module_find(NULL, "rpi", 
                           (lam_ssi_t **) lam_ssi_rpi_static_modules, 
                           (lam_ssi_t ***) &lam_ssi_rpi_modules);

  /* Initialize the list */

  lam_ssi_rpi_base_opened = al_init(sizeof(lam_ssi_rpi_t*), module_compare);
  if (lam_ssi_rpi_base_opened == NULL) {
    show_help(NULL, "system-call-failed", "malloc", NULL);
    return LAMERROR;
  }

  /* See if a specific boot module was requested */
  
  rpi = lam_ssi_base_param_lookup_string(param_rpi);
  if (rpi != NULL && strlen(rpi) > 0) {
    i = check_specific_module(aod, rpi);
    free(rpi);
  }

  /* Nope -- a specific one was not selected.  Go choose one. */
  
  else
    i = check_all_modules(aod);
  
  if (i == LAMERROR) {
    al_free(lam_ssi_rpi_base_available);
    lam_ssi_rpi_base_available = NULL;
    return LAMERROR;
  }

  /* Now that that's all done, setup unexpected message buffering */

  if (lam_ssi_rpi_cbuf_init() != 0) {
    al_free(lam_ssi_rpi_base_available);
    lam_ssi_rpi_base_available = NULL;
    return LAMERROR;
  }

  return 0;
}


/*
 * Compare two elements in the LIST
 */
static int 
module_compare(const void *a, const void *b)
{
  const lam_ssi_rpi_t **aa = (const lam_ssi_rpi_t **) a;
  const lam_ssi_rpi_t **bb = (const lam_ssi_rpi_t **) b;

  return (*aa - *bb);
}


/*
 * A specific module was selected on the command line.  If a module by
 * that name is found, call its open function (if it exists).  If it
 * opens successfully, or if the open function does not exist, save it
 * in the "opened" list for later selection/initialization.
 * that name is found, call its open (if it exists) 
 */
static int
check_specific_module(OPT *aod, char *name)
{
  int i;
  const lam_ssi_t *ls;

  if (lam_ssi_rpi_verbose > 10)
    lam_debug(lam_ssi_rpi_did, "open: looking for rpi module named %s", name);
  
  for (i = 0; lam_ssi_rpi_modules[i] != NULL; ++i) {
    ls = lam_ssi_rpi_modules[i];
    if (strcmp(name, ls->ssi_module_name) == 0) {
      if (lam_ssi_rpi_verbose > 10)
	lam_debug(lam_ssi_rpi_did, "open: opening rpi module %s", 
                  ls->ssi_module_name);
      if (ls->ssi_open_module == NULL ||
	  (ls->ssi_open_module != NULL &&
	   ls->ssi_open_module(aod) == 1)) {

        /* If the open function returns happiness, save this module
           for later querying. */

        if (lam_ssi_rpi_verbose > 10)
          lam_debug(lam_ssi_rpi_did, "open: opened rpi module %s", 
                    ls->ssi_module_name);
        al_insert(lam_ssi_rpi_base_opened, &lam_ssi_rpi_modules[i]);

        /* If this module didn't add a priority for itself, add one
           with a default priority of 0. */
        
        if (lam_ssi_base_param_find("rpi", (char*) ls->ssi_module_name,
                                    "priority") == LAMERROR)
          lam_ssi_base_param_register_int("rpi", (char*) ls->ssi_module_name, 
                                          "priority", NULL, 0);
        break;
      }

      /* Otherwise, this module's init function did not want to run.
	 Abort. */

      else {
        if (lam_ssi_rpi_verbose > 10)
          lam_debug(lam_ssi_rpi_did, "open: rpi module %s did not open",
                    ls->ssi_module_name);
	show_help("rpi-ssi", "selected-module-unavailable", name, NULL);
        lam_ssi_base_module_registry_unuse((lam_ssi_t*) ls);
	return LAMERROR;
      }
    }
  }

  /* Did we find one? */

  if (lam_ssi_rpi_modules[i] == NULL) {
    show_help("ssi", "module-not-found", "rpi", name, NULL);
    return LAMERROR;
  }

  /* All done */

  return 0;
}


/*
 * Call open on all the available modules (if any exist).  If open
 * returns happiness (or doesn't exist), save it in the "opened" list
 * for later selection/initialization.
 *
 * Return LAMERROR if there are no modules available.
 */
static int
check_all_modules(OPT *aod)
{
  int i, found;
  const lam_ssi_t *ls;

  /* Call the open/query functions in every module and see if they
     want to run */

  for (found = i = 0; lam_ssi_rpi_modules[i] != NULL; ++i) {
    ls = lam_ssi_rpi_modules[i];
    if (lam_ssi_rpi_verbose > 0)
      lam_debug(lam_ssi_rpi_did, "open: opening rpi module %s", 
                ls->ssi_module_name);
    if (ls->ssi_open_module == NULL ||
	(ls->ssi_open_module != NULL &&
	 ls->ssi_open_module(aod) == 1)) {
      
      /* If the open function returns happiness, save this module
         for later querying. */
      
      if (lam_ssi_rpi_verbose > 10)
        lam_debug(lam_ssi_rpi_did, "open: opened rpi module %s", 
                  ls->ssi_module_name);
      al_insert(lam_ssi_rpi_base_opened, &lam_ssi_rpi_modules[i]);
      found = 1;

      /* If this module didn't add a priority for itself, add one with
         a default priority of 0. */
      
      if (lam_ssi_base_param_find("rpi", (char*) ls->ssi_module_name,
                                  "priority") == LAMERROR)
        lam_ssi_base_param_register_int("rpi", (char*) ls->ssi_module_name, 
                                        "priority", NULL, 0);
    } else {
      if (lam_ssi_rpi_verbose > 10)
        lam_debug(lam_ssi_rpi_did, "open: rpi module %s did not open",
                  ls->ssi_module_name);
      lam_ssi_base_module_registry_unuse((lam_ssi_t*) ls);
    }
  }

  /* If there were no modules available, it's an error. */

  if (found == 0) {
    if (lam_ssi_rpi_verbose > 10)
      lam_debug(lam_ssi_rpi_did, 
                "open: no rpi moduless available to be opened!");
    show_help("ssi-rpi", "none-available", NULL);
    return LAMERROR;
  }

  /* All done */

  return 0;
}
