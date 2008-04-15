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
 * $Id: lam-ssi-cr.h,v 1.8 2004/03/06 21:17:31 jsquyres Exp $
 *
 * Function:	- SSI CR header file.
 */

#ifndef LAM_SSI_CR_H
#define LAM_SSI_CR_H

#include <lam_config.h>

#include <mpi.h>
#include <app_mgmt.h>
#include <all_list.h>
#include <lam-ssi.h>


/**************************************************************************
 * Common to crlam and crmpi
 **************************************************************************/

/*
 * Global variables
 */
extern int lam_ssi_cr_verbose;
extern int lam_ssi_cr_did;


/**************************************************************************
 * Specific to crlam
 **************************************************************************/

  /*
   * Global functions for SSI overall CRLAM open and close
   */

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  int lam_ssi_crlam_base_checkpoint(struct _gps *, int);
  int lam_ssi_crlam_base_close(void);
  int lam_ssi_crlam_base_continue(void);
  int lam_ssi_crlam_base_create_restart_argv(char **, OPT *);
  int lam_ssi_crlam_base_open(OPT *, char *);
  int lam_ssi_crlam_base_restart(char *, char *);
  int lam_ssi_crlam_base_do_exec(char *, char *);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif


/*
 * Types for each function
 */

typedef int (*lam_ssi_crlam_checkpoint_fn_t)(void);
typedef int (*lam_ssi_crlam_continue_fn_t)(void);
typedef void (*lam_ssi_crlam_disable_checkpoint_fn_t)(void);
typedef void (*lam_ssi_crlam_enable_checkpoint_fn_t)(void);
typedef int (*lam_ssi_crlam_finalize_fn_t)(void);
typedef int (*lam_ssi_crlam_init_fn_t)(char *, char **, OPT *, 
                                       struct _gps *, int);
typedef int (*lam_ssi_crlam_restart_fn_t)(char *, char *);

typedef int (*lam_ssi_crlam_lamcheckpoint_fn_t)(pid_t);
typedef int (*lam_ssi_crlam_lamrestart_fn_t)(char *);

typedef const struct lam_ssi_crlam_actions_1_1_0 * 
  (*lam_ssi_crlam_query_fn_t)(int *);


/*
 * Struct used to pass CRLAM SSI information from the each SSI instance
 * back to the SSI glue.
 */

typedef struct lam_ssi_crlam_1_1_0 {
  lam_ssi_1_0_0_t lscrl_meta_info;

  /* CRLAM API function pointers */

  lam_ssi_crlam_query_fn_t lscrl_query;

} lam_ssi_crlam_1_1_0_t;


typedef struct lam_ssi_crlam_actions_1_1_0 {

  /* CRLAM API functions pointers */

  lam_ssi_crlam_checkpoint_fn_t lscrla_checkpoint;
  lam_ssi_crlam_continue_fn_t lscrla_continue;
  lam_ssi_crlam_disable_checkpoint_fn_t lscrla_disable_checkpoint;
  lam_ssi_crlam_enable_checkpoint_fn_t lscrla_enable_checkpoint;
  lam_ssi_crlam_finalize_fn_t lscrla_finalize;
  lam_ssi_crlam_init_fn_t lscrla_init;
  lam_ssi_crlam_restart_fn_t lscrla_restart;

  /* To be invoked by lamcheckpoint and lamrestart, respectively */

  lam_ssi_crlam_lamcheckpoint_fn_t lscrla_lamcheckpoint;
  lam_ssi_crlam_lamrestart_fn_t lscrla_lamrestart;
} lam_ssi_crlam_actions_1_1_0_t;

/*
 * Set the default type to use version 1.1.0 of the SSI CRLAM struct 
 */

typedef lam_ssi_crlam_1_1_0_t lam_ssi_crlam_t;
typedef lam_ssi_crlam_actions_1_1_0_t lam_ssi_crlam_actions_t;


/*
 * Global instance of array of pointers to lam_ssi_crlam_t.  Will
 * effectively be filled in by configure.
 */

extern const lam_ssi_t **lam_ssi_crlam_modules;


/*
 * Global variables
 */
extern lam_ssi_crlam_actions_t lam_ssi_crlam;
extern lam_ssi_crlam_t lam_ssi_crlam_base_module;


/**************************************************************************
 * Specific to crmpi
 **************************************************************************/

/*
 * Global functions for SSI overall CRMPI open and close
 */

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  void lam_ssi_crmpi_base_app_suspend(void);
  int lam_ssi_crmpi_base_checkpoint(void);
  int lam_ssi_crmpi_base_close(void);
  int lam_ssi_crmpi_base_continue(void);
  void lam_ssi_crmpi_base_finalize_lock(void);
  void lam_ssi_crmpi_base_init_lock(void);
  int lam_ssi_crmpi_base_lock_mpi(void);
  int lam_ssi_crmpi_base_open(OPT *args);
  int lam_ssi_crmpi_base_query(void);                               
  int lam_ssi_crmpi_base_release_mpi(void);
  int lam_ssi_crmpi_base_restart(void);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

/*
 * Types for each function
 */

typedef int (*lam_ssi_crmpi_finalize_fn_t)(void);

typedef const struct lam_ssi_crmpi_actions_1_0_0 * 
  (*lam_ssi_crmpi_init_fn_t)(void);
	
typedef void (*lam_ssi_crmpi_app_suspend_fn_t)(void);
typedef int (*lam_ssi_crmpi_query_fn_t)(int *priority, 
                                        int *thread_min, int *thread_max);


/*
 * Struct used to pass CRMPI SSI information from the each SSI instance
 * back to the SSI glue.
 */
typedef struct lam_ssi_crmpi_1_0_0 {
  lam_ssi_1_0_0_t lscrm_meta_info;

  /* CRMPI API function pointers */

  lam_ssi_crmpi_query_fn_t lscrm_query;
  lam_ssi_crmpi_init_fn_t lscrm_init;
} lam_ssi_crmpi_1_0_0_t;


typedef struct lam_ssi_crmpi_actions_1_0_0 {

  /* CRMPI API functions pointers */

  lam_ssi_crmpi_finalize_fn_t lscrma_finalize;
  lam_ssi_crmpi_app_suspend_fn_t lscrma_app_suspend;

} lam_ssi_crmpi_actions_1_0_0_t;
  
/*
 * Set the default type to use version 1.0.0 of the SSI CRMPI struct 
 */

typedef lam_ssi_crmpi_1_0_0_t lam_ssi_crmpi_t;
typedef lam_ssi_crmpi_actions_1_0_0_t lam_ssi_crmpi_actions_t;


/*
 * Global instance of array of pointers to lam_ssi_crmpi_t.  Will
 * effectively be filled in by configure.
 */

extern const lam_ssi_t **lam_ssi_crmpi_modules;


/*
 * Global variables
 */
typedef enum {
  LAM_SSI_CRMPI_BASE_HANDLER_STATE_IDLE,
  LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING,
  LAM_SSI_CRMPI_BASE_HANDLER_STATE_RUNNING,

  LAM_SSI_CRMPI_BASE_STATE_MAX
} lam_ssi_crmpi_base_handler_state_t; 
extern volatile lam_ssi_crmpi_base_handler_state_t
                lam_ssi_crmpi_base_handler_state; 

extern lam_ssi_crmpi_actions_t lam_ssi_crmpi;
extern lam_ssi_crmpi_t lam_ssi_crmpi_base_module;
extern LIST *lam_ssi_crmpi_base_opened;
extern LIST *lam_ssi_crmpi_base_available;


#endif /* LAM_SSI_CR_H */
