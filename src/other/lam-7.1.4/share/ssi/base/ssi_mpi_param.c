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
 * $Id: ssi_mpi_param.c,v 1.1 2003/07/11 19:31:33 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>
#include <lam-ssi.h>
#include <all_array.h>
#include <typical.h>


/*
 * local functions
 */
static int param_lookup(int index, MPI_Comm comm,
                        lam_ssi_base_param_storage_t *storage);


/*
 * Associate a keyval with a registered SSI parameter.  This has to be
 * a second, distinct function call from registering the parameter
 * because this call needs to be in libmpi (because this is
 * MPI-specific functionality), whereas the register call is in liblam
 * (because it applies to all LAM processes).
 */
int
lam_ssi_base_param_kv_associate(int index, int keyval)
{
  int size;
  lam_ssi_base_param_t *array;

  if (lam_ssi_base_params == NULL)
    return LAMERROR;

  array = lam_arr_get(lam_ssi_base_params);
  size = lam_arr_size(lam_ssi_base_params);
  if (index < 0 || index >= size)
    return LAMERROR;
  array[index].lsbp_keyval = keyval;

  return 0;
}


int 
lam_ssi_base_param_kv_lookup_int(int index, MPI_Comm comm)
{
  lam_ssi_base_param_storage_t storage;
  
  if (param_lookup(index, comm, &storage) == 0)
    return storage.intval;
  return LAMERROR;
}


char *
lam_ssi_base_param_kv_lookup_string(int index, MPI_Comm comm)
{
  lam_ssi_base_param_storage_t storage;
  
  if (param_lookup(index, comm, &storage) == 0)
    return storage.stringval;
  return NULL;
}


/*************************************************************************/

/*
 * DO NOT MODIFY THIS FUNCTION WITHOUT ALSO MODIFYING ssi_param.c!
 *
 * This function is essentially of the private function param_lookup()
 * in ssi_param.c.  The big difference is that ssi_param.c is in
 * liblam, and this is in libmpi.  So here, we can look for keyval SSI
 * attributes as well -- and since we need MPI_Comm_get_attr() to do
 * that, we cannot do that in liblam.
 *
 * So if this function ever changes, ssi_param.c likely must also be
 * changed.
 */
static int
param_lookup(int index, MPI_Comm comm, lam_ssi_base_param_storage_t *storage)
{
  int found;
  void **attrp;
  char *env;
  lam_ssi_base_param_t *p;

  /* Lookup the index and see if it's valid */

  if (lam_ssi_base_params == NULL)
    return LAMERROR;
  if (lam_arr_size(lam_ssi_base_params) < index)
    return LAMERROR;
  p = ((lam_ssi_base_param_t*) lam_arr_get(lam_ssi_base_params)) + index;


  /* If we have a valid keyval, look that up first */

  if (p->lsbp_keyval != MPI_KEYVAL_INVALID && comm != MPI_COMM_NULL) {
    MPI_Comm_get_attr(comm, p->lsbp_keyval, &attrp, &found);
    if (found == 1) {
      if (p->lsbp_type == LAM_SSI_BASE_PARAM_TYPE_INT)
        storage->intval = (int) *attrp;
      else if (p->lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING)
        storage->stringval = strdup(*attrp);
      else
        return LAMERROR;

      return 0;
    }
  }

  /* We either don't have a keyval or didn't find it.  So look in the
     environment. */

  if ((env = getenv(p->lsbp_env_var_name)) != NULL) {
    if (p->lsbp_type == LAM_SSI_BASE_PARAM_TYPE_INT)
      storage->intval = atoi(env);
    else if (p->lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING)
      storage->stringval = strdup(env);
    else
      return LAMERROR;

    return 0;
  }

  /* Didn't find it; use the default value. */

  switch (p->lsbp_type) {
  case LAM_SSI_BASE_PARAM_TYPE_INT:
    storage->intval = p->lsbp_default_value.intval;
    break;

  case LAM_SSI_BASE_PARAM_TYPE_STRING:
    storage->stringval = p->lsbp_default_value.stringval;
    break;

  default:
    return LAMERROR;
  }

  /* All done */

  return 0;
}
