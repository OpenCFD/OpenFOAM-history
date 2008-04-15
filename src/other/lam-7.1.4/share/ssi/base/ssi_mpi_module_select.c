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
 * $Id: ssi_mpi_module_select.c,v 1.3 2003/06/05 05:34:18 brbarret Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <errno.h>

#include <mpi.h>
#include <lammpithreads.h>
#include <typical.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <all_list.h>
#include <lam-ssi.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-cr.h>


/*
 * Local functions
 */
static int try_level(int requested, LIST *rpi, LIST *coll, LIST *crmpi);
static LIST *list_copy(LIST *l);
static void trim(int level, LIST *list);
static int make_mask(int minval, int maxval);


/*
 * Local variables
 */
enum { no_error, no_rpi_avail, no_coll_avail };
static int my_errno = no_error;


/*
 * Function for taking lists of all available MPI modules and
 * selecting the modules that will be used.  Must consider both
 * priority (in some kind's cases) as well as the thread level that
 * the modules reported that theycould run at.
 *
 * Discard any modules that cannot run at the requested thead level.
 *
 * Note that the incoming LISTs are already in priority order.
 */
int 
lam_ssi_base_mpi_module_select(int requested)
{
  int i;
  LIST *rpi_list, *coll_list, *crmpi_list;

  for (i = requested; i >= MPI_THREAD_SINGLE; --i) {
    rpi_list = list_copy(lam_ssi_rpi_base_available);
    coll_list = list_copy(lam_ssi_coll_base_available);
    crmpi_list = list_copy(lam_ssi_crmpi_base_available);

    /* Try a specific thread level */

    my_errno = no_error;
    if (try_level(i, rpi_list, coll_list, crmpi_list) == 0) {

      /* If it succeeded, save the lists that try_level() munged */

      if (lam_ssi_rpi_base_available != NULL) {
	al_free(lam_ssi_rpi_base_available);
      }
      if (lam_ssi_coll_base_available != NULL) {
	al_free(lam_ssi_coll_base_available);
      }
      if (lam_ssi_crmpi_base_available != NULL) {
	al_free(lam_ssi_crmpi_base_available);
      }

      lam_ssi_rpi_base_available = rpi_list;
      lam_ssi_coll_base_available = coll_list;
      lam_ssi_crmpi_base_available = crmpi_list;

      break;
    }

    /* If it didn't succeed, free the lists that try_level() munged */

    if (rpi_list != NULL) {
      al_free(rpi_list);
    }
    if (coll_list != NULL) {
      al_free(coll_list);
    }
    if (crmpi_list != NULL) {
      al_free(crmpi_list);
    }
  }

  /* Failure! */

  if (i < MPI_THREAD_SINGLE) {
    if (my_errno == no_rpi_avail)
      show_help("ssi-rpi", "none-available", NULL);
    else if (my_errno == no_coll_avail)
      show_help("ssi-coll", "none-available", NULL);
    errno = ENOENT;
    return LAMERROR;
  }

  /* All done */

  return 0;
}


/*
 * Copy a LAM LIST
 */
static LIST *
list_copy(LIST *l)
{
  LIST *newlist;
  lam_ssi_module_t *entry;

  newlist = al_init(sizeof(lam_ssi_module_t), lam_ssi_base_module_compare);
  if (newlist == NULL)
    return NULL;

  for (entry = al_top(l); entry != NULL; entry = al_next(l, entry))
    al_append(newlist, entry);

  /* All done */

  return newlist;
}


/*
 * Function for taking lists of all available MPI modules and
 * selecting the modules that will be used.  Must consider both
 * priority (in some kind's cases) as well as the thread level that
 * the modules reported that theycould run at.
 *
 * Discard any modules that cannot run at the requested thead level.
 *
 * Note that the incoming LISTs are already in priority order.
 */
static int 
try_level(int requested, LIST *rpi_list, LIST *coll_list, LIST *crmpi_list)
{
  lam_ssi_module_t *rpi, *coll, *cr;
  int i, rpi_mask, mask, running_mask;

  /* First, trim off entries that are < requested */

  trim(requested, rpi_list);
  trim(requested, coll_list);
  trim(requested, crmpi_list);

  /* For the moment, do a simplified thread level calculation.  

     The first RPI on the list is the one with the highest priority,
     so that's the one that we'll use.

     Discard any coll and cr modules that cannot operate within that
     RPI's thread level constraints.  Find the minimum thread level
     common to all those who remain, and use that. */

  rpi = al_top(rpi_list);
  if (rpi == NULL) {
    my_errno = no_rpi_avail;
    return LAMERROR;
  }
  running_mask = rpi_mask = 
    make_mask(rpi->lsm_thread_min, rpi->lsm_thread_max);

  /* Delete any coll modules that cannot operate in this thread
     range */

  for (coll = al_top(coll_list); coll != NULL; 
       coll = al_next(coll_list, coll)) {
    mask = make_mask(coll->lsm_thread_min, coll->lsm_thread_max);
    if ((rpi_mask & mask) == 0) {

      /* This coll module cannot operate in the thread levels
         specified by the winning RPI.  Delete it from
         consideration. */

      al_delete(coll_list, coll);
    }
    running_mask &= mask;
  }

  /* If we have no coll modules left, this is an error.  Doh! */

  if (al_top(coll_list) == NULL) {
    my_errno = no_coll_avail;
    return LAMERROR;
  }

  /* Find all cr modules that can operate in this thread range */

  if (crmpi_list != NULL && 
      al_top(crmpi_list) != NULL) {
    for (cr = al_top(crmpi_list); cr != NULL; 
         cr = al_next(crmpi_list, cr)) {
      mask = make_mask(cr->lsm_thread_min, cr->lsm_thread_max);
      if ((rpi_mask & mask) == 0) {
        
        /* This cr module cannot operate in the thread levels
           specified by the winning RPI.  Delete it from
           consideration. */
        
        al_delete(crmpi_list, cr);
      }
      running_mask &= mask;
    }
  }

  /* If we have no cr modules, it's not an error.  So don't bother to
     check. */
  /* Determine the final thread level */

  for (i = LAM_max(MPI_THREAD_SINGLE, requested); 
       i <= MPI_THREAD_MULTIPLE; ++i) {
    if ((running_mask & (1 << (i + 1))) != 0) {
      lam_mpi_thread_level = i;
      break;
    }
  }

  /* All done */

  return 0;
}


/*
 * Iterate through a list of lam_ssi_module_t items and delete those
 * items who do not have a thread_min high enough
 */
static void
trim(int level, LIST *list)
{
  lam_ssi_module_t *lsm, *next;

  for (lsm = al_top(list); lsm != NULL; lsm = next) {
    next = al_next(list, lsm);

    if (lsm->lsm_thread_max < level)
      al_delete(list, lsm);
  }
}


static int 
make_mask(int minval, int maxval)
{
  int i, mask;

  mask = 0;
  for (i = minval; i <= maxval; ++i)
    mask |= (1 << (i + 1));

  return mask;
}
