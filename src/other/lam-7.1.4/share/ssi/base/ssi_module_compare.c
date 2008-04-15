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
 * $Id: ssi_module_compare.c,v 1.1 2003/04/03 18:24:13 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <string.h>

#include <lam-ssi.h>


/*
 * Function for comparing two lam_ssi_module_t structs so that we can
 * build prioritized LIST's of them.  This assumed that the types of
 * the modules are the same.  Sort first by priority, second by module
 * name, third by module version.
 *
 * Note that we acutally want a *reverse* ordering here -- the al_*
 * functions will put "smaller" items at the head, and "larger" items
 * at the tail.  Since we want the highest priority at the head, it
 * may help the gentle reader to consider this an inverse comparison.
 * :-)
 */
int
lam_ssi_base_module_compare(lam_ssi_module_t *a, lam_ssi_module_t *b)
{
  int val;

  /* First, compare the priorties */

  if (a->lsm_priority > b->lsm_priority)
    return -1;
  else if (a->lsm_priority < b->lsm_priority)
    return 1;
  else {
    lam_ssi_t *aa = a->lsm_module;
    lam_ssi_t *bb = b->lsm_module;

    /* The priorities were equal, so compare the names */

    val = strncmp(aa->ssi_module_name, bb->ssi_module_name,
                  LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN);
    if (val != 0)
      return -val;

    /* The names were equal, so compare the versions */

    if (aa->ssi_module_major_version > bb->ssi_module_major_version)
      return -1;
    else if (aa->ssi_module_major_version < bb->ssi_module_major_version)
      return 1;

    else if (aa->ssi_module_minor_version > bb->ssi_module_minor_version)
      return -1;
    else if (aa->ssi_module_minor_version < bb->ssi_module_minor_version)
      return 1;

    else if (aa->ssi_module_release_version > bb->ssi_module_release_version)
      return -1;
    else if (aa->ssi_module_release_version < bb->ssi_module_release_version)
      return 1;
  }

  /* They're equal */

  return 0;
}
