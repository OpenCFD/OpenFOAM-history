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
 * $Id: lam-ssi-rpi.h,v 1.9 2003/11/15 14:54:50 jsquyres Exp $
 *
 *	Function:	- SSI RPI interface
 */

#ifndef LAM_SSI_RPI_H
#define LAM_SSI_RPI_H

#include <lam_config.h>

#include <string.h>

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <all_list.h>
#include <lam-ssi.h>


/*
 * Types for each function
 * Top-level functions
 */

typedef int (*lam_ssi_rpi_query_fn_t)(int *priority, int *thread_min, 
                                      int *thread_max);

/*
 * Action functions
 */

typedef int (*lam_ssi_rpi_addprocs_fn_t)(struct _proc **procs, int nprocs);
typedef int (*lam_ssi_rpi_advance_fn_t)(MPI_Request, int);
typedef int (*lam_ssi_rpi_alloc_mem_fn_t)(MPI_Aint size, MPI_Info info,
					  void *baseptr);
typedef int (*lam_ssi_rpi_build_fn_t)(MPI_Request);
typedef int (*lam_ssi_rpi_checkpoint_fn_t)(void);
typedef int (*lam_ssi_rpi_continue_fn_t)(void);
typedef int (*lam_ssi_rpi_destroy_fn_t)(MPI_Request);
typedef int (*lam_ssi_rpi_fastrecv_fn_t)(char *, int, MPI_Datatype, int,
					 int *, MPI_Comm, MPI_Status *, 
					 int *);
typedef int (*lam_ssi_rpi_fastsend_fn_t)(char *, int, MPI_Datatype, int,
					 int, MPI_Comm);
typedef int (*lam_ssi_rpi_finalize_fn_t)(struct _proc *);
typedef int (*lam_ssi_rpi_free_mem_fn_t)(void *baseptr);
typedef const struct lam_ssi_rpi_actions_1_0_0 *
  (*lam_ssi_rpi_init_1_0_0_fn_t)(struct _proc **procs, int nprocs,
                                 int *max_tag, int *max_cid);
typedef const struct lam_ssi_rpi_actions_1_1_0 *
  (*lam_ssi_rpi_init_1_1_0_fn_t)(struct _proc **procs, int nprocs,
                                 int *max_tag, int *max_cid);
typedef int (*lam_ssi_rpi_interrupt_fn_t)(void);
typedef int (*lam_ssi_rpi_iprobe_fn_t)(MPI_Request);
typedef int (*lam_ssi_rpi_restart_fn_t)(void);
typedef int (*lam_ssi_rpi_start_fn_t)(MPI_Request, MPI_Request);


/*
 * Struct used to pass RPI SSI information from the each SSI instance
 * back to the SSI glue.
 */

typedef struct lam_ssi_rpi_1_0_0 {
  lam_ssi_1_0_0_t lsr_meta_info;

  /* RPI API function pointers */

  lam_ssi_rpi_query_fn_t lsr_query;

  lam_ssi_rpi_init_1_0_0_fn_t lsr_init;
} lam_ssi_rpi_1_0_0_t;


typedef struct lam_ssi_rpi_actions_1_0_0 {

  /* rpi API function pointers */

  lam_ssi_rpi_addprocs_fn_t lsra_addprocs;
  lam_ssi_rpi_finalize_fn_t lsra_finalize;

  lam_ssi_rpi_build_fn_t lsra_build;
  lam_ssi_rpi_start_fn_t lsra_start;
  lam_ssi_rpi_advance_fn_t lsra_advance;
  lam_ssi_rpi_destroy_fn_t lsra_destroy;

  lam_ssi_rpi_iprobe_fn_t lsra_iprobe;

  lam_ssi_rpi_fastrecv_fn_t lsra_fastrecv;
  lam_ssi_rpi_fastsend_fn_t lsra_fastsend;

  lam_ssi_rpi_alloc_mem_fn_t lsra_alloc_mem;
  lam_ssi_rpi_free_mem_fn_t lsra_free_mem;

  lam_ssi_rpi_checkpoint_fn_t lsra_checkpoint;
  lam_ssi_rpi_continue_fn_t lsra_continue;
  lam_ssi_rpi_restart_fn_t lsra_restart;

  /* Flags */

  int lsra_tv_queue_support;
} lam_ssi_rpi_actions_1_0_0_t;


/*
 * Ver 1.1.0
 * Changes:
 *
 * - New type for lsr_init (different return type; return new 1_1_0
 *   actions struct potiner)
 * - New lsra_interrupt action function
 */

typedef struct lam_ssi_rpi_1_1_0 {
  lam_ssi_1_0_0_t lsr_meta_info;

  /* RPI API function pointers */

  lam_ssi_rpi_query_fn_t lsr_query;

  lam_ssi_rpi_init_1_1_0_fn_t lsr_init;
} lam_ssi_rpi_1_1_0_t;

typedef struct lam_ssi_rpi_actions_1_1_0 {

  /* RPI API function pointers */

  lam_ssi_rpi_addprocs_fn_t lsra_addprocs;
  lam_ssi_rpi_finalize_fn_t lsra_finalize;

  lam_ssi_rpi_build_fn_t lsra_build;
  lam_ssi_rpi_start_fn_t lsra_start;
  lam_ssi_rpi_advance_fn_t lsra_advance;
  lam_ssi_rpi_destroy_fn_t lsra_destroy;

  lam_ssi_rpi_iprobe_fn_t lsra_iprobe;

  lam_ssi_rpi_fastrecv_fn_t lsra_fastrecv;
  lam_ssi_rpi_fastsend_fn_t lsra_fastsend;

  lam_ssi_rpi_alloc_mem_fn_t lsra_alloc_mem;
  lam_ssi_rpi_free_mem_fn_t lsra_free_mem;

  lam_ssi_rpi_interrupt_fn_t lsra_interrupt;
  lam_ssi_rpi_checkpoint_fn_t lsra_checkpoint;
  lam_ssi_rpi_continue_fn_t lsra_continue;
  lam_ssi_rpi_restart_fn_t lsra_restart;

  /* Flags */

  int lsra_tv_queue_support;
} lam_ssi_rpi_actions_1_1_0_t;


/*
 * Set the default type to use version 1.1.0 of the SSI RPI struct 
 */

typedef lam_ssi_rpi_1_1_0_t lam_ssi_rpi_t;
typedef lam_ssi_rpi_actions_1_1_0_t lam_ssi_rpi_actions_t;


/*
 * Global functions for SSI overall RPI open and close
 */

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  int lam_ssi_rpi_base_close(void);
  int lam_ssi_rpi_base_open(OPT *aod);
  int lam_ssi_rpi_base_query(void);
  int lam_ssi_rpi_base_init(void);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif


/*
 * Public variables
 */

extern int lam_ssi_rpi_verbose;
extern int lam_ssi_rpi_did;
extern LIST *lam_ssi_rpi_base_opened;
extern LIST *lam_ssi_rpi_base_available;


/*
 * Global instance of array of pointers to lam_ssi_rpi_t.  Will
 * effectively be filled in by configure.
 */

extern const lam_ssi_t **lam_ssi_rpi_modules;


/*
 * Macros for access to RPI functions to allow single-RPI and
 * multi-RPI development in the same tree.  When we go multi-RPI, some
 * of these may turn into loops (e.g., addprocs) or dispatches to
 * "glue" functions to decide which RPI to invoke the member function
 * on (e.g., allocmem).
 */

/* Single-RPI definitions -- pretty simple */

extern lam_ssi_rpi_actions_t lam_ssi_rpi;
extern lam_ssi_rpi_t lam_ssi_rpi_base_module;

#define RPI_ADDPROCS(new_procs, num_new_procs) \
  lam_ssi_rpi.lsra_addprocs((new_procs), (num_new_procs))
#define RPI_ADVANCE(request_top, fl_block) \
  lam_ssi_rpi.lsra_advance((request_top), (fl_block))
#define RPI_ALLOC_MEM(size, info, baseptr) \
  lam_ssi_rpi.lsra_alloc_mem((size), (info), (baseptr))
#define RPI_BUILD(req) \
  lam_ssi_rpi.lsra_build((req))
#define RPI_CHECKPOINT() \
  lam_ssi_rpi.lsra_checkpoint()
#define RPI_CONTINUE() \
  lam_ssi_rpi.lsra_continue()
#define RPI_DESTROY(req) \
  lam_ssi_rpi.lsra_destroy((req))
#define RPI_FASTRECV(buf, count, datatype, dest, tag, comm, status, sequence) \
  lam_ssi_rpi.lsra_fastrecv((buf), (count), (datatype), (dest), (tag), \
                       (comm), (status), (sequence))
#define RPI_FASTSEND(buf, count, datatype, dest, tag, comm) \
  lam_ssi_rpi.lsra_fastsend((buf), (count), (datatype), (dest), (tag), (comm))
#define RPI_FINALIZE_PROC(proc) \
  lam_ssi_rpi.lsra_finalize((proc))
#define RPI_FINALIZE_ALLPROCS() \
  lam_ssi_rpi.lsra_finalize(0)
#define RPI_FREE_MEM(baseptr) \
  lam_ssi_rpi.lsra_free_mem((baseptr))
#define RPI_HAS_CHECKPOINT() \
  (lam_ssi_rpi.lsra_checkpoint != NULL && \
   lam_ssi_rpi.lsra_continue != NULL && \
   lam_ssi_rpi.lsra_restart != NULL)
#define RPI_HAS_FASTRECV(comm, source) \
  (lam_ssi_rpi.lsra_fastrecv != NULL)
#define RPI_HAS_FASTSEND(comm, dest) \
  (lam_ssi_rpi.lsra_fastsend != NULL)
#define RPI_INTERRUPT() \
  lam_ssi_rpi.lsra_interrupt()
#define RPI_IPROBE(req) \
  lam_ssi_rpi.lsra_iprobe((req))
#define RPI_IS_LAMD() \
  (strcmp(lam_ssi_rpi_base_module.lsr_meta_info.ssi_module_name, "lamd") == 0)
#define RPI_RESTART() \
  lam_ssi_rpi.lsra_restart()
#define RPI_START(top_request, current_request) \
  lam_ssi_rpi.lsra_start((top_request), (current_request))
#define RPI_HAS_TOTALVIEW_QUEUE_SUPPORT() \
  (lam_ssi_rpi.lsra_tv_queue_support == 1)
#define RPI_DISABLE_FAST() { \
    lam_ssi_rpi.lsra_fastrecv = NULL; \
    lam_ssi_rpi.lsra_fastsend = NULL; \
  }

#endif /* LAM_SSI_RPI_H */
