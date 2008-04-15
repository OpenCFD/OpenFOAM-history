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
 * $Id: ssi_coll_query.c,v 1.4 2003/12/04 19:54:47 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <all_list.h>
#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <typical.h>


/*
 * Global variables
 */
LIST *lam_ssi_coll_base_available = NULL;


/*
 * Private functions
 */
static int thread_query(lam_ssi_t *module, lam_ssi_module_t *entry);
static int thread_query_1_0_0(lam_ssi_t *module, lam_ssi_module_t *entry);
static int thread_query_1_1_0(lam_ssi_t *module, lam_ssi_module_t *entry);


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
lam_ssi_coll_base_query(void)
{
  int found;
  void *p;
  lam_ssi_module_t entry;

  /* Initialize the list */

  lam_ssi_coll_base_available = 
    al_init(sizeof(lam_ssi_module_t), lam_ssi_base_module_compare);
  if (lam_ssi_coll_base_available == NULL) {
    show_help(NULL, "system-call-failed", "malloc", NULL);
    return LAMERROR;
  }

  /* In 64 bit mode, this struct can have empty padding */

  LAM_ZERO_ME(entry);

  /* The list of modules that we should check has already been
     established in lam_ssi_coll_base_opened. */

  for (found = 0, p = al_top(lam_ssi_coll_base_opened); p != NULL;
       p = al_next(lam_ssi_coll_base_opened, p)) {
    entry.lsm_module = *((lam_ssi_t **) p);

    /* Call a subroutine to do the work, because the module may
       represent different versions of the coll SSI struct. */

    if (thread_query(entry.lsm_module, &entry) == 0) {

      /* Save the results in the list.  The priority isn't relevant,
         because selection is decided at communicator-constructor
         time.  But we save the thread_min and thread_max arguments
         (set in the thread_query() function) so that the initial selection
         algorithm can negotiate the overall thread level for this
         process. */

      entry.lsm_priority = 0;
      al_insert(lam_ssi_coll_base_available, &entry);
      found = 1;
    } else {

      /* If the module doesn't want to run, then close it.  It's
         already had its close() method invoked; now close it out of
         the dynamic registry (if it's there). */

      lam_ssi_base_module_registry_unuse((lam_ssi_t *) entry.lsm_module);
    }
  }

  /* The opened list is now no longer useful and we can free it */

  al_free(lam_ssi_coll_base_opened);
  lam_ssi_coll_base_opened = NULL;

  /* If we have no collective modules available, it's an error.
     Thanks for playing! */

  if (found == 0) {
    al_free(lam_ssi_coll_base_available);
    lam_ssi_coll_base_available = NULL;

    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, "query: no collectives available!");
    show_help("ssi-coll", "none-available", NULL);
    return LAMERROR;
  }

  /* All done */

  return 0;
}


static int 
thread_query(lam_ssi_t *ls, lam_ssi_module_t *entry)
{
  int ret;

  if (lam_ssi_coll_verbose > 10)
    lam_debug(lam_ssi_coll_did, "query: querying coll module %s", 
              ls->ssi_module_name);

  /* This module has already been successfully opened.  So now query
     it. */

  if (ls->ssi_kind_major_version == 1 &&
      ls->ssi_kind_minor_version == 0 &&
      ls->ssi_kind_release_version == 0)
    ret = thread_query_1_0_0(ls, entry);
  else if (ls->ssi_kind_major_version == 1 &&
           ls->ssi_kind_minor_version == 1 &&
           ls->ssi_kind_release_version == 0)
    ret = thread_query_1_1_0(ls, entry);

  /* Query done -- look at the return value to see what happened */

  if (ret != 0) {
    if (lam_ssi_coll_verbose > 10)
      lam_debug(lam_ssi_coll_did, "query: coll module %s is not available", 
                ls->ssi_module_name);
    if (ls->ssi_close_module != NULL)
      ls->ssi_close_module();
  } else {
    if (lam_ssi_coll_verbose >= 10)
      lam_debug(lam_ssi_coll_did, "query: coll module %s available", 
                ls->ssi_module_name);
  }

  return ret;
}


static int 
thread_query_1_0_0(lam_ssi_t *module, lam_ssi_module_t *entry)
{
  lam_ssi_coll_1_0_0_t *coll = (lam_ssi_coll_1_0_0_t *) module;

  return coll->lsc_thread_query(&(entry->lsm_thread_min), 
                                &(entry->lsm_thread_max));
}


static int 
thread_query_1_1_0(lam_ssi_t *module, lam_ssi_module_t *entry)
{
  lam_ssi_coll_1_1_0_t *coll = (lam_ssi_coll_1_1_0_t *) module;

  return coll->lsc_thread_query(&(entry->lsm_thread_min), 
                                &(entry->lsm_thread_max));
}

