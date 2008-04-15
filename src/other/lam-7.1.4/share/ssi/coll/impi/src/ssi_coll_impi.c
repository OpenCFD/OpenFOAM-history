/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#include <lam-ssi-coll-impi-config.h>

#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-impi.h>
#include <mpisys.h>


/*
 * Lograthmic set of collective algorithms for the lamd RPI (ugh!)
 * (same disclaimers as above)
 */
static const lam_ssi_coll_actions_1_1_0_t actions = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_impi_init,
  lam_ssi_coll_impi_finalize,

  /* Checkpoint/continue/restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  NULL,
  NULL,
  lam_ssi_coll_lam_basic_allreduce,
  NULL,
  NULL,
  NULL, /* alltoallw not implemented */
  lam_ssi_coll_impi_barrier,
  0,
  lam_ssi_coll_impi_bcast,
  NULL, /* exscan not implemented */
  NULL,
  NULL,
  0,
  lam_ssi_coll_impi_reduce,
  NULL,
  NULL,
  NULL,
  NULL
};


/*
 * Initial query function that is invoked during MPI_INIT, allowing
 * this module to indicate what level of thread support it provides.
 */
int 
lam_ssi_coll_impi_thread_query(int *thread_min, int *thread_max)
{
  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_MULTIPLE;
  
  return 0;
}


/*
 * Invoked when there's a new communicator that has been created.
 * Look at the communicator and decide which set of functions and
 * priority we want to return.
 */
const lam_ssi_coll_actions_1_1_0_t *
lam_ssi_coll_impi_query(MPI_Comm comm, int *priority)
{
#if LAM_WANT_IMPI
  /* Should this module run? */

  if (!LAM_IS_IMPI(comm)) {
    *priority = -1;
    return NULL;
  }

  /* If this is an IMPI communicator, then this module *must* be
     selected */

  *priority = 100;
  return &actions;
#else

  /* If we're not compiling with IMPI support, then hard-wire this
     module to always return 0 */

  *priority = -1;
  return NULL;
#endif
}


int
lam_ssi_coll_impi_end_query(MPI_Comm comm) {
  return 0;
}


int
lam_ssi_coll_impi_init(MPI_Comm comm,
		       const lam_ssi_coll_actions_1_1_0_t **new_actions)
{
  /* Nothing to init on the communicator */

  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "impi: init communicator %s", comm->c_name);
  return 0;
}


int
lam_ssi_coll_impi_finalize(MPI_Comm comm)
{
  /* Nothing to finalize on the communicator */

  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "impi: finalize communicator %s",
	      comm->c_name);
  return 0;
}
