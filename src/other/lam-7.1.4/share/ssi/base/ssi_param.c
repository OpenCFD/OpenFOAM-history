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
 * $Id: ssi_param.c,v 1.7 2004/03/06 19:59:29 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <lam-ssi.h>
#include <all_array.h>
#include <etc_misc.h>
#include <typical.h>


/*
 * public variables
 */
#define PUB(a) lam_ssi_base_##a
lam_array_t *PUB(params) = NULL;


/*
 * local variables
 */
static char *ssi_prefix = "LAM_MPI_SSI_";


/*
 * local functions
 */
static int param_register(char *type_name, char *module_name, char *param_name,
                          char *ssi_param_name,
                          lam_ssi_base_param_type_t type,
                          lam_ssi_base_param_storage_t *default_value);
static int param_lookup(int index, lam_ssi_base_param_storage_t *storage);
static int param_compare(const void *a, const void *b);
static void param_free(lam_ssi_base_param_t *p);


int
lam_ssi_base_param_register_int(char *type_name, char *module_name,
                                char *param_name, char *ssi_param_name, 
                                int default_value)
{
  lam_ssi_base_param_storage_t storage;

  storage.intval = default_value;
  return param_register(type_name, module_name, param_name, ssi_param_name,
                        LAM_SSI_BASE_PARAM_TYPE_INT, &storage);
}


int
lam_ssi_base_param_register_string(char *type_name, char *module_name,
                                   char *param_name, char *ssi_param_name,
                                   char *default_value)
{
  lam_ssi_base_param_storage_t storage;

  storage.stringval = default_value;
  return param_register(type_name, module_name, param_name, ssi_param_name,
                        LAM_SSI_BASE_PARAM_TYPE_STRING, &storage);
}


int 
lam_ssi_base_param_lookup_int(int index)
{
  lam_ssi_base_param_storage_t storage;
  
  if (param_lookup(index, &storage) == 0)
    return storage.intval;
  return LAMERROR;
}


char *
lam_ssi_base_param_lookup_string(int index)
{
  lam_ssi_base_param_storage_t storage;
  
  if (param_lookup(index, &storage) == 0)
    return storage.stringval;
  return NULL;
}


int
lam_ssi_base_param_find(char *type, char *module, char *param) 
{
  int i, size;
  lam_ssi_base_param_t *array;

  /* Check for bozo cases */

  if (PUB(params) == NULL)
    return LAMERROR;
  if (type == NULL || param == NULL)
    return LAMERROR;

  /* Loop through looking for a parameter of a given
     type/module/param */

  array = lam_arr_get(PUB(params));
  size = lam_arr_size(PUB(params));
  for (i = 0; i < size; ++i) {
    if (strcmp(type, array[i].lsbp_type_name) == 0 &&
        ((module == NULL && array[i].lsbp_module_name == NULL) ||
         (module != NULL && array[i].lsbp_module_name != NULL &&
          strcmp(module, array[i].lsbp_module_name) == 0)) &&
        strcmp(param, array[i].lsbp_param_name) == 0)
      return i;
  }

  /* Didn't find it */

  return LAMERROR;
}


int
lam_ssi_base_param_finalize(void)
{
  int i, size;
  lam_ssi_base_param_t *array;

  if (PUB(params) != NULL) {
    array = lam_arr_get(PUB(params));
    size = lam_arr_size(PUB(params));
    for (i = 0; i < size; ++i)
      param_free(&array[i]);

    lam_arr_free(PUB(params));
    PUB(params) = NULL;
  }

  return 0;
}


int
lam_ssi_base_param_set_string(char *param_name, char *value)
{
  char *tmp;
  int len;

  len = strlen(param_name) + strlen(ssi_prefix) + 32;
  if (value != NULL) {
    len += strlen(value);
    tmp = malloc(len);
    if (tmp == NULL)
      return LAMERROR;

    snprintf(tmp, len, "%s%s=%s\n", ssi_prefix, param_name, value);
  } else {
    tmp = malloc(len);
    if (tmp == NULL)
      return LAMERROR;

    snprintf(tmp, len, "%s%s=\n", ssi_prefix, param_name);
  }

  /* All done */

  putenv(tmp);
  return LAMERROR;
}

int
lam_ssi_base_param_set_int(char *param_name, int value)
{
  char *tmp;
  int len;

  len = strlen(param_name) + strlen(ssi_prefix) + 32;
  tmp = malloc(len);
  if (tmp == NULL)
    return LAMERROR;

  snprintf(tmp, len, "%s%s=%d\n", ssi_prefix, param_name, value);

  /* All done */

  putenv(tmp);
  return LAMERROR;
}

/*************************************************************************/

static int 
param_register(char *type_name, char *module_name, char *param_name,
               char *ssi_param_name,
               lam_ssi_base_param_type_t type,
               lam_ssi_base_param_storage_t *default_value)
{
  int i, len;
  lam_ssi_base_param_t param, *array;

  /* Initialize the array if it has never been initialized */
  
  if (PUB(params) == NULL) {
    PUB(params) = lam_arr_init(sizeof(lam_ssi_base_param_t), param_compare);
    if (PUB(params) == NULL)
      return LAMERROR;
  }

  /* Create a parameter entry.  If a keyval is to be used, it will be
     registered elsewhere.  We simply assign -1 here. */

  param.lsbp_type = type;
  param.lsbp_keyval = -1;

  param.lsbp_type_name = strdup(type_name);
  if (param.lsbp_type_name == NULL)
    return LAMERROR;
  if (module_name != NULL) {
    param.lsbp_module_name = strdup(module_name);
    if (param.lsbp_module_name == NULL) {
      free(param.lsbp_type_name);
      return LAMERROR;
    }
  } else
    param.lsbp_module_name = NULL;
  if (param_name != NULL) {
    param.lsbp_param_name = strdup(param_name);
    if (param.lsbp_param_name == NULL) {
      free(param.lsbp_type_name);
      free(param.lsbp_module_name);
      return LAMERROR;
    }
  } else
    param.lsbp_param_name = NULL;

  /* The full parameter name may have been specified by the caller.
     If it was, use that (only for backwards compatability).
     Otherwise, derive it from the type, module, and parameter
     name. */

  param.lsbp_env_var_name = NULL;
  if (ssi_param_name != LAM_SSI_BASE_PARAM_INFO && ssi_param_name != NULL) {
    param.lsbp_full_name = strdup(ssi_param_name);
  } else {
    len = 16 + strlen(type_name);

    if (module_name != NULL)
      len += strlen(module_name);
    if (param_name != NULL)
      len += strlen(param_name);

    param.lsbp_full_name = malloc(len);
    if (param.lsbp_full_name == NULL) {
      free(param.lsbp_type_name);
      free(param.lsbp_module_name);
      free(param.lsbp_param_name);
      return LAMERROR;
    }
    strncpy(param.lsbp_full_name, type_name, len);

    if (module_name != NULL) {
      strcat(param.lsbp_full_name, "_");
      strcat(param.lsbp_full_name, module_name);
    }
    if (param_name != NULL) {
      strcat(param.lsbp_full_name, "_");
      strcat(param.lsbp_full_name, param_name);
    }
  }

  /* If ssi_param_name isn't LAM_SSI_BASE_PARAM_INFO, then it's a
     lookup-able value.  So assign the environment variable name as
     well. */

  if (ssi_param_name != LAM_SSI_BASE_PARAM_INFO) {
    len = strlen(param.lsbp_full_name) + strlen(ssi_prefix) + 16;
    param.lsbp_env_var_name = malloc(len);
    if (param.lsbp_env_var_name == NULL) {
      free(param.lsbp_full_name);
      free(param.lsbp_type_name);
      free(param.lsbp_module_name);
      free(param.lsbp_param_name);
      return LAMERROR;
    }
    snprintf(param.lsbp_env_var_name, len, "%s%s", ssi_prefix, 
             param.lsbp_full_name);
  }

  /* Figure out the default value */

  if (default_value != NULL) {
    if (param.lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING &&
        default_value->stringval != NULL)
      param.lsbp_default_value.stringval = strdup(default_value->stringval);
    else
      param.lsbp_default_value = *default_value;
  } else
    memset(&param.lsbp_default_value, 0, sizeof(param.lsbp_default_value));

  /* See if this entry is already in the Array */

  len = lam_arr_size(PUB(params));
  array = lam_arr_get(PUB(params));
  for (i = 0; i < len; ++i)
    if (param_compare(&param, &array[i]) == 0) {

      /* Copy in the new default value to the old entry */

      if (array[i].lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING &&
          array[i].lsbp_default_value.stringval != NULL)
        free(array[i].lsbp_default_value.stringval);
      if (param.lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING &&
          param.lsbp_default_value.stringval != NULL)
        array[i].lsbp_default_value.stringval =
          strdup(param.lsbp_default_value.stringval);

      param_free(&param);
      return i;
    }

  /* Add it to the array */

  if (lam_arr_append(PUB(params), &param) != 0)
    return LAMERROR;
  return lam_arr_size(PUB(params)) - 1;
}


/*
 * DO NOT MODIFY THIS FUNCTION WITHOUT ALSO MODIFYING ssi_mpi_param.c!
 *
 * This function appears in liblam.  Because of unix linker semantics,
 * it's simply easier to essentially duplicate this function in libmpi
 * because in libmpi, we need to lookup on a keyval before looking in
 * the environment.  The logic is simpler if we just duplicate/alter
 * the code in ssi_mpi_param.c rather than try to make this a) public,
 * and b) more general (to accomodate looking up keyvals while not
 * linking to MPI_Comm_get_attr() in libmpi).
 */
static int
param_lookup(int index, lam_ssi_base_param_storage_t *storage)
{
  char *env;
  lam_ssi_base_param_t *p;

  /* Lookup the index and see if it's valid */

  if (PUB(params) == NULL)
    return LAMERROR;
  if (lam_arr_size(PUB(params)) < index)
    return LAMERROR;
  p = ((lam_ssi_base_param_t*) lam_arr_get(PUB(params))) + index;

  /* We either don't have a keyval or didn't find it.  So look in the
     environment. */

  if (p->lsbp_env_var_name != NULL &&
      (env = getenv(p->lsbp_env_var_name)) != NULL) {
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


static int 
param_compare(const void *a, const void *b)
{
  const lam_ssi_base_param_t *aa = (const lam_ssi_base_param_t*) a;
  const lam_ssi_base_param_t *bb = (const lam_ssi_base_param_t*) b;

  return strcmp(aa->lsbp_full_name, bb->lsbp_full_name);
}


static void 
param_free(lam_ssi_base_param_t *p)
{
  if (p->lsbp_type_name != NULL)
    free(p->lsbp_type_name);
  if (p->lsbp_module_name != NULL)
    free(p->lsbp_module_name);
  if (p->lsbp_param_name != NULL)
    free(p->lsbp_param_name);
  if (p->lsbp_env_var_name != NULL)
    free(p->lsbp_env_var_name);
  if (p->lsbp_full_name != NULL)
    free(p->lsbp_full_name);
  if (p->lsbp_type == LAM_SSI_BASE_PARAM_TYPE_STRING &&
      p->lsbp_default_value.stringval != NULL)
    free(p->lsbp_default_value.stringval);
}
