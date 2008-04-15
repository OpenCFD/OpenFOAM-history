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
 * $Id: ssi_module_registry.c,v 1.5 2003/12/04 20:40:36 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Ensure to get the right <ltdl.h> */ 
#include <lam_ltdl.h>

#include <lam-ssi.h>
#include <all_array.h>
#include <typical.h>
#include <etc_misc.h>


/*
 * Global variables
 */
int lam_ssi_base_module_path_param = -1;


/*
 * Private types
 */
typedef struct module_registry_item {
  char *mri_type;
  lt_dlhandle mri_dlhandle;
  lam_ssi_t *mri_module_struct;
  int mri_refcount;
  lam_array_t *mri_dependencies;
} module_registry_item_t;


/*
 * Private variables
 */
static lam_array_t *modules = NULL;


/*
 * Private functions
 */
static int find_module(const char *type, const char *name);
static int link_index(int src_index, int depend_index);
static void unuse_index(int index);


/*
 * Initialize the registry
 */
int
lam_ssi_base_module_registry_init(void)
{
  if (modules == NULL)
    modules = lam_arr_init(sizeof(module_registry_item_t *), NULL);
  if (modules == NULL)
    return LAMERROR;

  if (lt_dlinit() != 0)
    return LAMERROR;

  lam_ssi_base_module_path_param = 
    lam_ssi_base_param_register_string("base", NULL, "module_path",
                                       NULL, LAM_PKGLIBDIR);
  return 0;
}


/*
 * Add a newly-opened dyanmic module to the registry of open modules.
 * The module's type, handle, and public struct are saved.
 */
int
lam_ssi_base_module_register(char *type, lt_dlhandle module_handle, 
                             lam_ssi_t *module_struct)
{
  int size, i;
  module_registry_item_t *new_item, **m = lam_arr_get(modules);

  new_item = malloc(sizeof(module_registry_item_t));
  if (new_item == NULL)
    return LAMERROR;

  new_item->mri_type = strdup(type);
  new_item->mri_dlhandle = module_handle;
  new_item->mri_module_struct = module_struct;
  new_item->mri_refcount = 1;
  new_item->mri_dependencies = lam_arr_init(sizeof(int), NULL);

  /* See if there's any open slots */

  size = lam_arr_size(modules);
  for (i = 0; i < size; ++i) {
    if (m[i] == NULL) {
      m[i] = new_item;
      new_item = NULL;
    }
  }

  /* If we didn't find a slot, append it */

  if (new_item != NULL) {
    lam_arr_append(modules, &new_item);
  }

  return 0;
}


/*
 * Create a dependency from one module entry to another
 */
int 
lam_ssi_base_module_registry_link(const char *src_type, 
                                  const char *src_name,
                                  const char *depend_type,
                                  const char *depend_name)
{
  return link_index(find_module(src_type, src_name),
                    find_module(depend_type, depend_name));
}


/*
 * If it's in the array, close a specified module and remove it from
 * the array.
 */
void
lam_ssi_base_module_registry_unuse(lam_ssi_t *module)
{
  unuse_index(find_module(module->ssi_kind_name, module->ssi_module_name));
}


/*
 * Finalize the registry -- close everything that's still open.
 */
int
lam_ssi_base_module_registry_finalize(void)
{
  int non_null, i, changed = 1;
  module_registry_item_t **m = lam_arr_get(modules);

  if (modules != NULL) {

    /* Have to be slightly careful about this because of dependencies,
       particularly on OS's where it matters (i.e., closing a module
       that is depended on by other modules actually causes missing
       symbols because the OS actually does unload it from memory!),
       such as OS X.

       So instead of just blindly closing everything, we have iterate
       over the array of open modules unuse everything with a refcount
       of 1 -- skip anything with a refcount of more than 1.  Repeat
       this procedure until either we have nothing open or we made one
       full pass and no refcounts went to one (which is technically an
       error). */

    do {
      for (changed = non_null = 0, i = lam_arr_size(modules)- 1; i >= 0; --i) {
        if (m[i] != NULL && m[i]->mri_refcount == 1) {
          unuse_index(i);
          changed = 1;
        }
        if (m[i] != NULL) {
          ++non_null;
        }
      }
    } while (non_null > 0 && changed == 1);
    lam_arr_free(modules);
    modules = NULL;
  }

  lt_dlexit();

  return 0;
}


static int
find_module(const char *type, const char *name)
{
  int i, len = lam_arr_size(modules);
  module_registry_item_t **m = lam_arr_get(modules);

  for (i = 0; i < len; ++i) {
    if (m[i] != NULL && strcmp(m[i]->mri_type, type) == 0 && 
        strcmp(m[i]->mri_module_struct->ssi_module_name, name) == 0)
      return i;
  }

  /* Not found */

  return LAMERROR;
}


static int
link_index(int src_index, int depend_index)
{
  module_registry_item_t **m;

  if (src_index == LAMERROR || depend_index == LAMERROR)
    return LAMERROR;

  m = lam_arr_get(modules);

  /* Add the depend module index to the source's dependency list */

  lam_arr_append(m[src_index]->mri_dependencies, &depend_index);

  /* Increment the refcount in the dependency */

  ++m[depend_index]->mri_refcount;

  return 0;
}


static void
unuse_index(int index)
{
  int i, *deps;
  module_registry_item_t **m, *item;

  if (index == LAMERROR)
    return;

  m = lam_arr_get(modules);
  item = m[index];
  --item->mri_refcount;
  if (item->mri_refcount <= 0) {
    lt_dlclose(item->mri_dlhandle);

    /* Mark this slot as now empty */

    m[index] = NULL;

    /* Now go unuse/close (at a minimum: decrement the refcount) any
       dependencies of this module */

    deps = lam_arr_get(item->mri_dependencies);
    for (i = lam_arr_size(item->mri_dependencies) - 1; i >= 0; --i) {
      --m[deps[i]]->mri_refcount;

      /* Special case: if we decrease someone to a refcount of 0
         (i.e., no one else is using it -- we only had it loaded
         because of dependencies), then go unuse it right now. */

      if (m[deps[i]]->mri_refcount <= 0) {
        unuse_index(deps[i]);
      }
    }

    /* It should be obvious, but I'll state it anyway because it bit
       me during debugging: after the dlclose(), the ssi pointer is no
       longer valid because it has [potentially] been unloaded from
       memory.  So don't try to use it.  :-) */

    lam_arr_free(item->mri_dependencies);
    free(item->mri_type);
    free(item);

  }
}
