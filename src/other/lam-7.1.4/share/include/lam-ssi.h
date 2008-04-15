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
 * $Id: lam-ssi.h,v 1.12 2004/03/06 19:59:29 jsquyres Exp $
 *
 *	Function: - SSI interface -- the glue that holds it all
 *	together
 */

#ifndef LAM_SSI_H
#define LAM_SSI_H

#include <lam_config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Ensure to get the right <ltdl.h> */ 
#include <lam_ltdl.h>

#include <lamdebug.h>
#include <all_opt.h>
#include <all_array.h>
#include <mpi.h>


/*
 * Public variables
 */

extern int lam_ssi_verbose;
extern int lam_ssi_did;
extern int lam_ssi_base_module_path_param;


/*
 * Types for each function
 */

typedef int (*lam_ssi_open_module_fn_t)(OPT *aod);
typedef int (*lam_ssi_close_module_fn_t)(void);
typedef int (*lam_ssi_mpi_init_callback_fn_t)(void);


/*
 * Struct of meta data necessary in every SSI module, regardless of
 * its kind.
 */

#define LAM_MPI_SSI_BASE_MAX_KIND_NAME_LEN 32
#define LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN 64

typedef struct lam_ssi_1_0_0 {

  /* Integer version numbers indicating which SSI API version this
     module conforms to. */

  int ssi_major_version;
  int ssi_minor_version;
  int ssi_release_version;

  /* Information about the kind */

  char ssi_kind_name[LAM_MPI_SSI_BASE_MAX_KIND_NAME_LEN];
  int ssi_kind_major_version;
  int ssi_kind_minor_version;
  int ssi_kind_release_version;

  /* Information about the module itself */

  char ssi_module_name[LAM_MPI_SSI_BASE_MAX_MODULE_NAME_LEN];
  int ssi_module_major_version;
  int ssi_module_minor_version;
  int ssi_module_release_version;

  /* Functions for opening and closing the module */

  lam_ssi_open_module_fn_t ssi_open_module;
  lam_ssi_close_module_fn_t ssi_close_module;
} lam_ssi_1_0_0_t;


/*
 * Set the default type to use version 1.0.0 of the SSI struct 
 */

typedef lam_ssi_1_0_0_t lam_ssi_t;


/*
 * Structure for making priority lists of modules
 */

typedef struct lam_ssi_module {
  int lsm_priority;
  int lsm_thread_min, lsm_thread_max;
  lam_ssi_t *lsm_module;
} lam_ssi_module_t;


/*
 * Types for SSI parameters
 */

typedef enum {
  LAM_SSI_BASE_PARAM_TYPE_INT,
  LAM_SSI_BASE_PARAM_TYPE_STRING,

  LAM_SSI_BASE_PARAM_TYPE_MAX
} lam_ssi_base_param_type_t;
typedef union {
  int intval;
  char *stringval;
} lam_ssi_base_param_storage_t;
#define LAM_SSI_BASE_PARAM_INFO ((void*) -1)
typedef struct {
  lam_ssi_base_param_type_t lsbp_type;
  char *lsbp_type_name;
  char *lsbp_module_name;
  char *lsbp_param_name;
  char *lsbp_full_name;

  int lsbp_keyval;
  char *lsbp_env_var_name;

  lam_ssi_base_param_storage_t lsbp_default_value;
} lam_ssi_base_param_t;


/*
 * Variable holding the array of registered SSI parameters
 */
extern lam_array_t *lam_ssi_base_params;


/*
 * Global functions for SSI
 */

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  int lam_ssi_base_ao_setup(OPT *ad);
  int lam_ssi_base_ao_process_args(OPT *ad);
  int lam_ssi_base_ao_process_arg(char *kind, char *arg);

  int lam_ssi_base_close(void);
  int lam_ssi_base_open(OPT *aod);

  int lam_ssi_base_module_check(char *name, char *module, int is_default);
  int lam_ssi_base_module_compare(lam_ssi_module_t *a, lam_ssi_module_t *b);
  int lam_ssi_base_module_find(char *directory, char *type, 
                               lam_ssi_t *static_modules[], 
                               lam_ssi_t ***modules_out);
  int lam_ssi_base_module_register(char *type, lt_dlhandle module_handle, 
                                   lam_ssi_t *module_struct);
  int lam_ssi_base_module_registry_init(void);
  int lam_ssi_base_module_registry_finalize(void);
  int lam_ssi_base_module_registry_link(const char *src_type, 
                                        const char *src_name,
                                        const char *depend_type,
                                        const char *depend_name);
  void lam_ssi_base_module_registry_unuse(lam_ssi_t *module);
  int lam_ssi_base_module_registry_use(const char *type, const char *name);

  int lam_ssi_base_mpi_init_callback(lam_ssi_mpi_init_callback_fn_t func);
  int lam_ssi_base_mpi_init_callbacks_invoke(void);
  int lam_ssi_base_mpi_module_select(int requested);
  struct in_addr lam_ssi_base_hostmap(struct in_addr *addr, char *keyname);
  void lam_ssi_base_hostmap_finalize(void);
  int lam_ssi_base_set_verbose(int index, lam_debug_stream_info_t *lds,
                               int *level, int *did);

  int lam_ssi_base_param_register_int(char *type_name, char *module_name,
                                      char *param_name, char *ssi_param_name,
                                      int default_value);
  int lam_ssi_base_param_register_string(char *type_name, char *module_name,
                                         char *param_name, 
                                         char *ssi_param_name,
                                         char *default_value);
  int lam_ssi_base_param_lookup_int(int index);
  char *lam_ssi_base_param_lookup_string(int index);
  int lam_ssi_base_param_find(char *type, char *module, char *param);
  int lam_ssi_base_param_finalize(void);
  int lam_ssi_base_param_set_string(char *param_name, char *value);
  int lam_ssi_base_param_set_int(char *param_name, int value);

  int lam_ssi_base_param_kv_associate(int index, int keyval);
  int lam_ssi_base_param_kv_lookup_int(int index, MPI_Comm comm);
  char *lam_ssi_base_param_kv_lookup_string(int index, MPI_Comm comm);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif /* LAM_SSI_H */
