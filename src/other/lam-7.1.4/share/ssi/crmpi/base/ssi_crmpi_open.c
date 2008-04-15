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
 * $Id: ssi_crmpi_open.c,v 1.18 2003/12/04 19:54:51 jsquyres Exp $
 *
 */

#include <stdio.h>
#include <string.h>

#include <lam_config.h>
#include <etc_misc.h>
#include <lamdebug.h>

#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-cr.h>


/*
 * The following file was created by configure.  It contains extern
 * statements and the definition of an array of pointers to each
 * module's public lam_ssi_crmpi_t struct.
 */

#include "lam-ssi-crmpi-modules.h"


/*
 * Global variables
 */
lam_ssi_crmpi_t lam_ssi_crmpi_base_module;
lam_ssi_crmpi_actions_t lam_ssi_crmpi;
LIST *lam_ssi_crmpi_base_opened = NULL;
volatile lam_ssi_crmpi_base_handler_state_t
    lam_ssi_crmpi_base_handler_state = LAM_SSI_CRMPI_BASE_HANDLER_STATE_IDLE;
const lam_ssi_t **lam_ssi_crmpi_modules = NULL;


/*
 * Local variables
 */
static int param_verbose;
static int param_cr;
#if LAM_HAVE_THREADS
static lam_debug_stream_info_t lds = {
  0, 0, 0, NULL, "crmpi:", 0, 1, 1, 0, "ssi-crmpi.txt"
};
#endif


/*
 * Local functions
 */
#if LAM_HAVE_THREADS
static int module_compare(const void *a, const void *b);
static int check_all_modules(OPT *aod);
static int check_specific_module(OPT *aod, char *);
#endif


int
lam_ssi_crmpi_base_open(OPT *aod)
{
  int i;
  char *cr;

  /* Register SSI parameters */

  param_verbose = lam_ssi_base_param_register_string("cr", "base", "verbose",
                                                     "cr_verbose", NULL);
  param_cr = lam_ssi_base_param_register_string("cr", "base", "module", 
                                                "cr", NULL);

#if LAM_HAVE_THREADS
  /* The CR modules must have thread support in order to run */
  /* Setup verbosity for this kind */

  lam_ssi_base_set_verbose(param_verbose, &lds,
			   &lam_ssi_cr_verbose, &lam_ssi_cr_did);
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "open: opening");

  /* Find all available modules */

  lam_ssi_base_module_find(NULL, "crmpi", 
                           (lam_ssi_t **) lam_ssi_crmpi_static_modules, 
                           (lam_ssi_t ***) &lam_ssi_crmpi_modules);

  /* Initialize the list */

  lam_ssi_crmpi_base_opened = al_init(sizeof(lam_ssi_crmpi_t*), 
                                      module_compare);
  if (lam_ssi_crmpi_base_opened == NULL) {
    show_help(NULL, "system-call-failed", "malloc", NULL);
    return LAMERROR;
  }

  /* check env overrides -- was a specific module requested? */

  cr = lam_ssi_base_param_lookup_string(param_cr);
  if (cr != NULL && strlen(cr) > 0) {
    i = check_specific_module(aod, cr);
    free(cr);
  }

  /* else select one, based on priority */

  else
    i = check_all_modules(aod);

  if (LAMERROR == i) {
    al_free(lam_ssi_crmpi_base_opened);
    lam_ssi_crmpi_base_opened = NULL;
    return LAMERROR;
  }
#else

  /* If we do not have thread support, then simply set the opened
     list to NULL */

  lam_ssi_crmpi_base_opened = NULL;
  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "open: no thread support, no cr modules");

  /* If the user requested a specific module, it's an error (because
     we cannot possibly use that module, since there is no thread
     support). */

  cr = lam_ssi_base_param_lookup_string(param_cr);
  if (cr != NULL) {
    if (strcmp(cr, "none") == 0) {
      show_help("cr-ssi", "specific-module-no-threads", cr, NULL);
      return LAMERROR;
    }
    free(cr);
  }
#endif

  /* All done */

  return 0;
}


#if LAM_HAVE_THREADS
/*
 * Compare two elements in the LIST
 */
static int 
module_compare(const void *a, const void *b)
{
  const lam_ssi_crmpi_t **aa = (const lam_ssi_crmpi_t **) a;
  const lam_ssi_crmpi_t **bb = (const lam_ssi_crmpi_t **) b;

  return (*aa - *bb);
}


/*
 * A specific module was selected on the command line.  If a module by
 * that name is found, call its open function (if it exists).  If that
 * returns happiness, add it to the list of available crmpi modules.
 *
 * If the specific module is not available, it is an error (since it
 * was specifically requested).
 */
static int 
check_specific_module(OPT *aod, char *name)
{
  int i;
  const lam_ssi_t *ls;

  if (!strcmp(name, "none")) {
    if (lam_ssi_cr_verbose >= 0)
      lam_debug(lam_ssi_cr_did, "open: module \"none\" explicitly requested. "
                "Disabling cr support");
    return 0;
  }

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "open: looking for cr module named %s", name);

  for (i = 0; lam_ssi_crmpi_modules[i] != NULL; ++i) {
    ls = lam_ssi_crmpi_modules[i];
    if (0 == strcmp(name, ls->ssi_module_name)) {
      if (lam_ssi_cr_verbose > 10)
        lam_debug(lam_ssi_cr_did, "open: opening cr module %s", 
                  ls->ssi_module_name);
      if (ls->ssi_open_module == NULL ||
          (ls->ssi_open_module != NULL &&
           1 == ls->ssi_open_module(aod))) {

        /* If the open function returns happiness, save this module
           for later querying. */

        if (lam_ssi_cr_verbose > 10)
          lam_debug(lam_ssi_cr_did, "open: opened cr module %s",
                    ls->ssi_module_name);
        al_insert(lam_ssi_crmpi_base_opened, &lam_ssi_crmpi_modules[i]);

        /* If this module didn't add a priority for itself, add one
           with a default priority of 0. */
        
        if (lam_ssi_base_param_find("cr", (char*) ls->ssi_module_name,
                                    "priority") == LAMERROR)
          lam_ssi_base_param_register_int("cr", (char*) ls->ssi_module_name, 
                                          "priority", NULL, 0);
        break;
      }

      /* Otherwise this module's open function didn't want to
         run. Abort. */

      else {
        if (lam_ssi_cr_verbose > 10)
          lam_debug(lam_ssi_cr_did, "open: cr module %s did not open",
                    ls->ssi_module_name);

        show_help("cr-ssi", "selected-module-unavailable", name);
        lam_ssi_base_module_registry_unuse((lam_ssi_t *) ls);
        return LAMERROR;
      }
    }
  }

  /* Did we find one ? */

  if (NULL == lam_ssi_crmpi_modules[i]) {
    show_help("cr-ssi", "module-not-found", name, NULL);
    return LAMERROR;
  }

  return 0;
}


/*
 * Call open on all the available modules (if any exist).  If open
 * returns happiness (or doesn't exist), add the module to the list of
 * available crmpi modules.
 *
 * It is not an error if there are no CR modules available.
 */
static int
check_all_modules(OPT *aod)
{
  int i;
  const lam_ssi_t *ls;

  for (i = 0; lam_ssi_crmpi_modules[i] != NULL; ++i) {
    ls = lam_ssi_crmpi_modules[i];
    if (lam_ssi_cr_verbose > 0)
      lam_debug(lam_ssi_cr_did, "open: opening cr module %s", 
                ls->ssi_module_name);
    if (ls->ssi_open_module == NULL ||
        (ls->ssi_open_module != NULL &&
         1 == ls->ssi_open_module(aod))) {

      /* If the open function returns happiness, save this module for
         later querying. */

      if (lam_ssi_cr_verbose > 10)
        lam_debug(lam_ssi_cr_did, "open: opened cr module %s",
                  ls->ssi_module_name);
      al_insert(lam_ssi_crmpi_base_opened, &lam_ssi_crmpi_modules[i]);
    } else {
      if (lam_ssi_cr_verbose >= 10)
        lam_debug(lam_ssi_cr_did, "open: cr module %s did not open",
                  ls->ssi_module_name);
      lam_ssi_base_module_registry_unuse((lam_ssi_t *) ls);
    }
  }

  /* It doesn't matter if there were no modules found to be available.
     We always return happiness. */

  return 0;
}
#endif
