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
 * $Id: ssi_module_find.c,v 1.4 2003/12/04 19:51:51 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Ensure to get the right <ltdl.h> */ 
#include <lam_ltdl.h>

#include <lam-ssi.h>
#include <all_array.h>
#include <typical.h>


/*
 * Private types
 */
typedef enum module_status {
  UNVISITED,
  FAILED_TO_LOAD,
  CHECKING_CYCLE,
  LOADED,

  STATUS_MAX
} module_status_t;

typedef struct module_file {
  char *type;
  char *name;
  char *basename;
  char *filename;
  module_status_t status;
} module_file_t;

typedef struct ltfn_data_holder {
  char *type;
  char *name;
} ltfn_data_holder_t;


/*
 * Private functions
 */
static int find_modules(char *path, char *type, char *name,
                        lam_ssi_t *static_modules[]);
static int save_filename(const char *filename, lt_ptr data);
static int open_module(int index, lam_ssi_t **static_modules);
static int check_laminfo(int index, lam_ssi_t **static_modules,
                         lam_array_t **dependencies);
static int check_dependency(char *line, int index, lam_ssi_t **static_modules,
                            lam_array_t **dependencies);


/*
 * Private variables
 */
static const char *laminfo_suffix = ".laminfo";
static const char *key_dependency = "dependency=";
static const char module_template[] = "ssi_%s_";
static lam_array_t *found_modules = NULL;
static lam_array_t *found_files = NULL;


/*
 * Function to find as many modules of a given type as possible.  This
 * includes statically-linked in modules as well as opening up a
 * directory and looking for shared-library SSI modules of the
 * appropriate type (load them if available).
 *
 * Return one consolidated array of (lam_ssi_t*) pointing to all
 * available modules.
 */
int
lam_ssi_base_module_find(char *directory, char *type, 
                         lam_ssi_t *static_modules[], 
                         lam_ssi_t ***modules_out)
{
  int i, size;
  lam_ssi_t **array;

  /* Find all the modules that were statically linked in */

  found_modules = lam_arr_init(sizeof(lam_ssi_t *), NULL);
  for (i = 0; static_modules[i] != NULL; ++i)
    lam_arr_append(found_modules, &static_modules[i]);

  /* Find any available dynamic modules in the specified directory */

  find_modules(directory, type, NULL, static_modules);

  /* Allocate and copy the found module pointers into the output
     array */

  size = lam_arr_size(found_modules);
  array = (lam_ssi_t **) lam_arr_get(found_modules);
  *modules_out = malloc(sizeof(lam_ssi_t *) * (size + 1));
  if (*modules_out == NULL)
    return LAMERROR;
  for (i = 0; i < size; ++i)
    (*modules_out)[i] = array[i];
  (*modules_out)[i] = NULL;
  lam_arr_free(found_modules);
  found_modules = NULL;

  /* All done */

  return 0;
}


/*
 * Open up all directories in a given path and search for modules of
 * the specified type (and possibly of a given name).
 *
 * Note that we use our own path iteration functionality (vs. ltdl's
 * lt_dladdsearchdir() functionality) because we need to look at
 * companion .laminfo files in the same directory as the library to
 * generate dependencies, etc.  If we use the plain lt_dlopen()
 * functionality, we would not get the directory name of the file
 * finally opened in recursive dependency traversals.
 */
static int
find_modules(char *path, char *type, char *name, lam_ssi_t **static_modules)
{
  int i, ret = 0;
  ltfn_data_holder_t params;
  char *path_to_use, *dir, *end;
  int num_files;
  module_file_t *files;

  params.type = type;
  params.name = name;

  found_files = lam_arr_init(sizeof(module_file_t), NULL);

  if (lam_ssi_verbose > 40) {
    if (name == NULL)
      lam_debug(lam_ssi_did, " looking for all dynamic %s SSI modules",
                type, NULL);
    else
      lam_debug(lam_ssi_did, " looking for dynamic %s SSI module named \"%s\"",
                type, name, NULL);
  }

  /* If directory is NULL, iterate over the set of directories
     specified by the SSI param ssi_base_module_path.  If path is not
     NULL, then use that as the path. */

  if (path == NULL)
    path_to_use =
      lam_ssi_base_param_lookup_string(lam_ssi_base_module_path_param);
  else
    path_to_use = path;

  /* Iterate over all the files in the directories in the path and
     make a master array of all the matching filenames that we
     find. */

  dir = path_to_use;
  do {
    end = strchr(dir, ':');
    if (end != NULL)
      *end = '\0';
    if ((ret = lt_dlforeachfile(dir, save_filename, &params)) != 0)
      break;
    dir = end + 1;
  } while (end != NULL);

  /* Iterate through all the filenames that we found.  Since one
     module may [try to] call another to be loaded, only try to load
     the UNVISITED files.  Also, ignore the return code -- basically,
     give every file one chance to try to load.  If they load, great.
     If not, great. */

  num_files = lam_arr_size(found_files);
  files = lam_arr_get(found_files);

  for (i = 0; i < num_files; ++i) {
    if (files[i].status == UNVISITED)
      open_module(i, static_modules);
  }

  /* So now we have a final list of loaded modules.  We can free all
     the file information. */

  for (i = 0; i < num_files; ++i) {
    free(files[i].type);
    free(files[i].name);
    free(files[i].basename);
    free(files[i].filename);
  }
  lam_arr_free(found_files);
  found_files = NULL;

  /* All done */

  return ret;
}


/*
 * Given a filename, see if it appears to be of the proper filename
 * format.  If so, save it in the array so that we can process it
 * later.
 */
static int 
save_filename(const char *filename, lt_ptr data)
{
  int len, prefix_len, total_len;
  char *prefix;
  const char *basename;
  module_file_t module_file;
  ltfn_data_holder_t *params = (ltfn_data_holder_t *) data;

  /* Check to see if the file is named what we expect it to be
     named */

  len = sizeof(module_template) + strlen(params->type) + 32;
  if (params->name != NULL)
    len += strlen(params->name);
  prefix = malloc(len);
  snprintf(prefix, len, module_template, params->type);
  prefix_len = strlen(prefix);
  if (params->name != NULL)
    strcat(prefix, params->name);
  total_len = strlen(prefix);

  basename = strrchr(filename, '/');
  if (basename == NULL)
    basename = filename;
  else
    basename += 1;

  if (strncmp(basename, prefix, total_len) != 0) {
    free(prefix);
    return 0;
  }

  /* Save all the info and put it in the list of found modules */

  module_file.type = strdup(params->type);
  module_file.name = strdup(basename + prefix_len);
  module_file.basename = strdup(basename);
  module_file.filename = strdup(filename);
  module_file.status = UNVISITED;
  lam_arr_append(found_files, &module_file);

  /* All done */

  free(prefix);
  return 0;
}


/*
 * Open a module, chasing down its dependencies first, if possible.
 */
static int 
open_module(int index, lam_ssi_t **static_modules)
{
  int i, len, *deps;
  lt_dlhandle module_handle;
  lam_ssi_t *module_struct;
  char *struct_name;
  module_file_t *files = (module_file_t *) lam_arr_get(found_files);
  module_file_t *file = &(files[index]);
  lam_array_t *dependencies = NULL;

  if (lam_ssi_verbose > 40) {
    lam_debug(lam_ssi_did, " examining dyanmic %s SSI module \"%s\"",
              file->type, file->name, NULL);
    lam_debug(lam_ssi_did, " %s", file->filename, NULL);
  }

  /* Was this module already loaded (e.g., via dependency)? */

  if (file->status == LOADED) {
    if (lam_ssi_verbose > 40)
      lam_debug(lam_ssi_did, " already loaded (ignored)", NULL);
    return 0;
  }

  /* Ensure that this module is not already statically loaded.  It's
     an error if it's already loaded because we're evaluating this
     file -- not this module.  Hence, returning LAMERROR indicates
     that the *file* failed to load, not the module. */

  for (i = 0; static_modules[i] != NULL; ++i) {
    if (strcmp(static_modules[i]->ssi_kind_name, file->type) == 0 &&
        strcmp(static_modules[i]->ssi_module_name, file->name) == 0) {
      if (lam_ssi_verbose > 40)
        lam_debug(lam_ssi_did, " already loaded (ignored)", NULL);
      file->status = FAILED_TO_LOAD;
      return LAMERROR;
    }
  }

  /* Look at see if this module has any dependencies.  If so, load
     them.  If we can't load them, then this module must also fail to
     load. */

  if (check_laminfo(index, static_modules, &dependencies) != 0) {
    file->status = FAILED_TO_LOAD;
    return LAMERROR;
  }

  /* Now try to load the module */

  module_handle = lt_dlopenext(file->filename);
  if (module_handle == NULL) {
    if (lam_ssi_verbose > 40)
      lam_debug(lam_ssi_did, " unable to open: %s (ignored)", 
                lt_dlerror(), NULL);
    file->status = FAILED_TO_LOAD;
    if (dependencies != NULL)
      lam_arr_free(dependencies);
    return LAMERROR;
  }

  /* Successfully opened the module; now find the public struct.
     Malloc out enough space for it. */

  len = strlen(file->type) + strlen(file->name) + 32;
  struct_name = malloc(len);
  if (struct_name == NULL) {
    lt_dlclose(module_handle);
    file->status = FAILED_TO_LOAD;
    if (dependencies != NULL)
      lam_arr_free(dependencies);
    return LAMERROR;
  }
  snprintf(struct_name, len, "lam_ssi_%s_%s_module", file->type,
           file->name);

  module_struct = lt_dlsym(module_handle, struct_name);
  if (module_struct == NULL) {
    if (lam_ssi_verbose > 40)
      lam_debug(lam_ssi_did, " \"%s\" does not appear to be a valid "
                "%s SSI dynamic module (ignored)", 
                file->basename, file->type, NULL);
    free(struct_name);
    lt_dlclose(module_handle);
    file->status = FAILED_TO_LOAD;
    if (dependencies != NULL)
      lam_arr_free(dependencies);
    return LAMERROR;
  }

  /* We found the public struct.  Save it, and register this module to
     be closed later. */

  lam_arr_append(found_modules, &module_struct);
  lam_ssi_base_module_register(file->type, module_handle, 
                               module_struct);

  /* Now that that's all done, link all the dependencies in to this
     module's registry entry */

  if (dependencies != NULL) {
    deps = lam_arr_get(dependencies);
    for (i = lam_arr_size(dependencies) - 1; i >= 0; --i)
      lam_ssi_base_module_registry_link(file->type, file->name,
                                        files[deps[i]].type,
                                        files[deps[i]].name);
    lam_arr_free(dependencies);
  }

  if (lam_ssi_verbose > 40)
    lam_debug(lam_ssi_did, " opened dynamic %s SSI module \"%s\"",
              file->type, file->name, NULL);
  file->status = LOADED;
    
  /* All done */
    
  free(struct_name);
  return 0;
}


/*
 * For a given filename, see if there exists a filename.laminfo, which
 * lists dependencies that must be loaded before this module is
 * loaded.  If we find this file, try to load those modules first.
 *
 * Detect dependency cycles and error out.
 */
static int 
check_laminfo(int index, lam_ssi_t **static_modules, 
              lam_array_t **dependencies)
{
  int len;
  FILE *fp;
  char *depname;
  char buffer[BUFSIZ], *p;
  module_file_t *files = lam_arr_get(found_files);
  module_file_t *file = &(files[index]);

  /* Form the filename */

  len = strlen(file->filename) + strlen(laminfo_suffix) + 16;
  depname = malloc(len);
  if (depname == NULL)
    return LAMERROR;
  snprintf(depname, len, "%s%s", file->filename, laminfo_suffix);

  /* Try to open the file.  If there's no file, return success (i.e.,
     there are no dependencies). */

  if ((fp = fopen(depname, "r")) == NULL) {
    free(depname);
    return 0;
  }

  /* Otherwise, loop reading the lines in the file and trying to load
     them.  Return failure upon the first module that fails to
     load. */

  if (lam_ssi_verbose > 40)
    lam_debug(lam_ssi_did, " opening laminfo file: %s", depname, NULL);
  while (fgets(buffer, BUFSIZ, fp) != NULL) {

    /* Perl chomp */

    buffer[BUFSIZ - 1] = '\0';
    len = strlen(buffer);
    if (buffer[len - 1] == '\n')
      buffer[len - 1] = '\0';

    /* Ignore emtpy lines and lines beginning with "#" or "//" */

    for (p = buffer; p != '\0'; ++p)
      if (!isspace((int) *p))
        break;

    if (*p == '\0')
      continue;
    else if (*p == '#' || (*p == '/' && *(p + 1) == '/'))
      continue;

    /* Is it a dependency? */

    else if (strncasecmp(p, key_dependency, strlen(key_dependency)) == 0) {
      if (check_dependency(p + strlen(key_dependency), index, 
                           static_modules, dependencies) == LAMERROR) {
        fclose(fp);
        free(depname);

        /* We can leave any successfully loaded dependencies; we might
           need them again later.  But free the dependency list for
           this module, because since [at least] one of them didn't
           load, we have to pretend like all of them didn't load and
           disallow loading this module.  So free the dependency
           list. */

        if (*dependencies != NULL)
          lam_arr_free(*dependencies);
        return LAMERROR;
      }
    }
  }
  if (lam_ssi_verbose > 40)
    lam_debug(lam_ssi_did, " laminfo file closed (%s)", file->basename, NULL);

  /* All done -- all depenencies satisfied */

  fclose(fp);
  free(depname);
  return 0;
}


/*
 * A DEPENDENCY key was found in the laminfo file.  Chase it down: see
 * if we've already got such a module loaded, or go try to load it if
 * it's not already loaded.
 */
static int
check_dependency(char *line, int index, lam_ssi_t **static_modules,
                 lam_array_t **dependencies)
{
  int i, happiness;
  char buffer[BUFSIZ];
  char *type, *name;
  int num_files = lam_arr_size(found_files);
  module_file_t *files = lam_arr_get(found_files);
  module_file_t *file = &(files[index]);

  /* Ensure that this was a valid dependency statement */

  type = line;
  name = strchr(line, ':');
  if (name == NULL)
    return LAMERROR;
  *name = '\0';
  ++name;

  /* Form the name of the module to compare to */

  if (strlen(type) + strlen(name) + 32 >= BUFSIZ) {
    file->status = FAILED_TO_LOAD;
    return LAMERROR;
  }
  snprintf(buffer, BUFSIZ, module_template, type);
  strcat(buffer, name);

  /* Traverse down the list of files that we have, and see if we can
     find it */

  file->status = CHECKING_CYCLE;
  for (happiness = i = 0; i < num_files; ++i) {

    /* Compare the name to the basename */

    if (strcmp(files[i].basename, buffer) != 0)
      continue;

    /* Catch the bozo dependency on itself */

    else if (i == index) {
      if (lam_ssi_verbose > 40)
        lam_debug(lam_ssi_did,
                  " module depends on itself (ignored dependency)", NULL);
      happiness = 1;
      break;
    }

    /* If it's loaded, great -- we're done (no need to check that
       dependency sub-tree) */

    else if (files[i].status == LOADED) {
      if (lam_ssi_verbose > 40)
        lam_debug(lam_ssi_did, " dependency has already been loaded (%s)",
                  files[i].basename, NULL);
      happiness = 1;
      break;
    }

    /* If it's specifically not loaded (i.e., there was some kind of
       error when we tried to load it), then we cannot meet the
       dependencies. */

    else if (files[i].status == FAILED_TO_LOAD) {
      if (lam_ssi_verbose > 40)
        lam_debug(lam_ssi_did, " dependency previously failed to load (%s)",
                  files[i].basename, NULL);
      break;
    }

    /* If we hit a cycle, return badness */

    else if (files[i].status == CHECKING_CYCLE) {
      if (lam_ssi_verbose > 40)
        lam_debug(lam_ssi_did, " found cycle! (%s)",
                  files[i].basename, NULL);
      break;
    }

    /* Otherwise, this dependency has not been looked at yet.  Go try
       to load it. */

    else if (files[i].status == UNVISITED) {
      if (lam_ssi_verbose > 40)
        lam_debug(lam_ssi_did, " loading dependency (%s)",
                  files[i].basename, NULL);
      if (open_module(i, static_modules) == 0) {
        happiness = 1;
      } else {
        if (lam_ssi_verbose > 40)
          lam_debug(lam_ssi_did, " dependency failed to load (%s)",
                    files[i].basename, NULL);
      }
      break;
    }
  }

  /* Did we find the dependency? */

  if (happiness != 1) {
    file->status = FAILED_TO_LOAD;
    return LAMERROR;
  }


  /* The dependency loaded properly.  Increment its refcount so that
     it doesn't get unloaded before we get unloaded. */
  
  if (*dependencies == NULL)
    *dependencies = lam_arr_init(sizeof(int), NULL);
  lam_arr_append(*dependencies, &i);
  
  /* All done -- all depenencies satisfied */

  return 0;
}
