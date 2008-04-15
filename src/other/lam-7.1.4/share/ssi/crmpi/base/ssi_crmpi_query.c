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
 * $Id: ssi_crmpi_query.c,v 1.5 2003/12/04 19:54:51 jsquyres Exp $
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
 * Global variables
 */
LIST *lam_ssi_crmpi_base_available = NULL;


/*
 * Scan down the list of successfully opened modules and query each of
 * them (the opened list will be one or more modules.  If the user
 * requested a specific module, it will be the only module in the
 * opened list).  Create and populate the available list of all
 * modules who indicate that they want to be considered for selection.
 * Close all modules who do not want to be considered for selection,
 * and destroy the opened list.
 */
int
lam_ssi_crmpi_base_query(void)
{
#if LAM_HAVE_THREADS
  int priority, thread_min, thread_max;
  const lam_ssi_t *ls;
  void *p;
  lam_ssi_crmpi_t *module;
  lam_ssi_module_t entry;

  /* If there's nothing to open, then don't do anything */

  if (lam_ssi_crmpi_base_opened == NULL)
    return 0;

  /* Initialize the list */

  lam_ssi_crmpi_base_available = 
    al_init(sizeof(lam_ssi_module_t), lam_ssi_base_module_compare);
  if (lam_ssi_crmpi_base_available == NULL) {
    show_help(NULL, "system-call-failed", "malloc", NULL);
    return LAMERROR;
  }

  /* The list of modules that we should check has already been
     established in lam_ssi_crmpi_base_opened. */

  for (p = al_top(lam_ssi_crmpi_base_opened); p != NULL;
       p = al_next(lam_ssi_crmpi_base_opened, p)) {
    module = *((lam_ssi_crmpi_t **) p);
    ls = &(module->lscrm_meta_info);
    if (lam_ssi_cr_verbose > 0)
      lam_debug(lam_ssi_cr_did, "query: querying cr module %s", 
                ls->ssi_module_name);

    /* This module has already been successfully opened.  So now query
       it. */

    if (module->lscrm_query(&priority, &thread_min, &thread_max) == 1) {
      if (lam_ssi_cr_verbose >= 10)
        lam_debug(lam_ssi_cr_did, 
                  "query: cr module available: %s, priority: %d", 
                  ls->ssi_module_name, priority);

      /* This module wants to be considered */

      entry.lsm_priority = priority;
      entry.lsm_thread_min = thread_min;
      entry.lsm_thread_max = thread_max;
      entry.lsm_module = (lam_ssi_t*) module;
      al_insert(lam_ssi_crmpi_base_available, &entry);
    } else {

      /* The module does not want to be considered.  Close it. */

      if (lam_ssi_cr_verbose >= 10)
        lam_debug(lam_ssi_cr_did, "query: cr module not available: %s",
                  ls->ssi_module_name);
      if (ls->ssi_close_module != NULL)
        ls->ssi_close_module();
      lam_ssi_base_module_registry_unuse((lam_ssi_t *) ls);
    }
  }

  /* The opened list is now no longer useful and we can free it */

  al_free(lam_ssi_crmpi_base_opened);
  lam_ssi_crmpi_base_opened = NULL;
#else
  /* If we do not have thread support, then simply set the available
     list to NULL.  The opened list should already be NULL, so we
     don't need to free it. */

  if (lam_ssi_cr_verbose >= 10)
    lam_debug(lam_ssi_cr_did, "query: no thread support, no cr modules");
  lam_ssi_crmpi_base_available = NULL;
#endif

  /* All done */

  return 0;
}
