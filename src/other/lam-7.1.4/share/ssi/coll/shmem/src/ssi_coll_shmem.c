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
 * $Id: ssi_coll_shmem.c,v 1.4 2004/03/09 16:53:08 adharurk Exp $
 *
 *	Function:	- SHMEM collective routines
 */

#include <string.h>

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-shmem-config.h>

#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-shmem.h>
#include <mpisys.h>
#include <blktype.h>

/*
 * Extern Functions
 */
extern const lam_ssi_coll_actions_1_1_0_t *
lam_ssi_coll_lam_basic_query(MPI_Comm comm, int *priority);

extern int lam_ssi_coll_lam_basic_end_query(MPI_Comm comm);

/*
 * Local functions
 */
static void free_lcd_mem(lam_ssi_coll_data_t*); 

/*
 * Local variables
 */
static int param_shmem;
static lam_ssi_coll_data_t *local_coll_data;
static lam_ssi_coll_actions_1_1_0_t lb_functions;
static lam_ssi_coll_actions_1_1_0_t shmem_functions = {

/* Per-communicator initialization and finalization functions */

  lam_ssi_coll_shmem_init,
  lam_ssi_coll_shmem_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  /* Allgather */
  lam_ssi_coll_shmem_allgather,
  /* Allgatherv -- use lam_basic */
  NULL,
  /* Allreduce -- use lam_basic */
  NULL,
  /* Alltoall */
  lam_ssi_coll_shmem_alltoall,
  /* Alltoallv -- use lam_basic */
  NULL,
  /* Alltoallw -- use lam_basic */
  NULL, 
  /* Barrier */
  lam_ssi_coll_shmem_barrier,
  /* Bcast */
  1,
  lam_ssi_coll_shmem_bcast,
  /* Exscan -- use lam_basic */ 
  NULL, 
  /* Gather */
  lam_ssi_coll_shmem_gather,
  /* Gatherv -- use lam_basic */
  NULL,
  /* Reduce */
  1,
  lam_ssi_coll_shmem_reduce,
  /* Reduce scatter: use lam_basic */
  NULL,
  /* Scan: use lam_basic */
  NULL,
  /* Scatter */
  lam_ssi_coll_shmem_scatter,
  /* Scatterv -- use lam_basic */
  NULL,
};


/* 
 * Open the shmem module -- basically just sets up some runtime
 * parameters
 */
int
lam_ssi_coll_shmem_open(OPT *ad)
{
  param_shmem = 
    lam_ssi_base_param_register_int("coll", "shmem", NULL,
				    NULL, 0);
  return 1;
}


/*
 * Initial query function that is invoked during MPI_INIT, allowing
 * this module to indicate what level of thread support it provides.
 */
int 
lam_ssi_coll_shmem_thread_query(int *thread_min, int *thread_max)
{
  *thread_min = MPI_THREAD_SINGLE;
  *thread_max = MPI_THREAD_MULTIPLE;
  
  return 0;
}

/*
 * Invoked when there's a new communicator that has been created.
 * Look at the communicator and decide which set of functions and
 * priority we want to return.
 *
 * This module is selected 
 */
const lam_ssi_coll_actions_1_1_0_t *
lam_ssi_coll_shmem_query(MPI_Comm comm, int *priority)
{
  int i;
  int size, rank;
  int shmem_size, control_size, array_size;
  int invalid_shmemid = -1;
  int invalid_semid = -1;
  int shared_ids[2];
  int bogus_priority;
  int success, all_success;
  int *successes;
  lam_ssi_coll_data_t * lcd = NULL;

  /* See if shmem module is enabled - proceed only if enabled */

  if (lam_ssi_base_param_lookup_int(param_shmem) != 1) {
    return NULL;
  }

  if (LAM_IS_INTER(comm))
    return NULL;

  /* Find out whether all the processes in the comm are on the same node */
  
  MPI_Comm_size(comm, &size);
  for (i = 0; i < size - 1; ++i)
    if ((comm->c_group->g_procs[i]->p_gps.gps_node) != 
	(comm->c_group->g_procs[i+1]->p_gps.gps_node))
      return NULL;

  if (size == 1) {
    /* For MPI_COMM_SELF goto lam_basic only */
    return NULL;
  }

  success = 1;
  /* Ok now we satisfy the condition, check if enogh shared memory can be
     allocated for this communicator. First allocate space for the local
     per communicator structure. Lowest ranked process will create the 
     area and send the shmid to rest of the processes. All other processes will
     see if they can attach to it. If yes then success otherwise they will
     return NULL
  */

  lcd = (lam_ssi_coll_data_t *) malloc(sizeof(lam_ssi_coll_data_t));
  if (lcd == NULL)
    success = 0;

  lcd->lcd_message_pool_size = 
    lam_ssi_coll_base_get_param(comm, 
				LAM_MPI_SSI_COLL_SHMEM_MESSAGE_POOL_SIZE);
  lcd->lcd_num_segments = 
    lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS);

  /* Initialize shared pointers to NULL and then allocate memory for them */
  lcd->lcd_barrier = NULL;
  lcd->lcd_collective_track = NULL;
  lcd->lcd_message_segment = NULL;
  lcd->lcd_barrier = (volatile int**) 
    malloc(sizeof(int*) * (lcd->lcd_num_segments + 1));
  lcd->lcd_collective_track = (volatile int**) 
    malloc(sizeof(int*) * (lcd->lcd_num_segments + 1));
  lcd->lcd_message_segment = (char**) 
    malloc(sizeof(char*) * lcd->lcd_num_segments);

  if (lcd->lcd_barrier == NULL || lcd->lcd_collective_track == NULL
      || lcd->lcd_message_segment == NULL)
    success = 0;

  MPI_Comm_rank(comm, &rank);
  array_size = size * (2 * lcd->lcd_num_segments + 2) * CACHELINESIZE;
  control_size = array_size;
  shmem_size =  control_size + lcd->lcd_message_pool_size;

  if (rank == 0) {
    if (((lam_ssi_coll_shmem_create_area(shmem_size, 
					 &(lcd->lcd_shared_segment_base),
					 &(lcd->lcd_shmemid), 
					 &(lcd->lcd_semid), size))
	== LAMERROR) 
	|| ((successes = (int*) malloc(size * sizeof(int))) == NULL)) {
      free_lcd_mem(lcd);
      shared_ids[0] = invalid_shmemid;
      shared_ids[1] = invalid_semid;
      for (i = 1; i < size; ++i) { 
	MPI_Send(shared_ids, 2, MPI_INT, i, 1, comm);
      }
      return NULL;
    }

    /* Send Information to other ranks */
    shared_ids[0] = lcd->lcd_shmemid;
    shared_ids[1] = lcd->lcd_semid;
    for (i = 1; i < size; ++i) { 
	MPI_Send(shared_ids, 2, MPI_INT, i, 1, comm);
    }
    
  } else {
    
    /* Recieve the shmemid from rank 0 and see if I can attach to the shared
       memory segment */
    MPI_Recv(shared_ids, 2, MPI_INT, 0, 1, comm, MPI_STATUS_IGNORE);
    lcd->lcd_shmemid = shared_ids[0];
    lcd->lcd_semid = shared_ids[1];

    if ((lcd->lcd_shmemid == -1) || (lcd->lcd_semid == -1))
      return NULL;

    /* Attach the shared memory segment */
    if ((lam_ssi_coll_shmem_attach_area(lcd->lcd_shmemid, lcd->lcd_semid,
				       &(lcd->lcd_shared_segment_base)))
	== LAMERROR) {
      free_lcd_mem(lcd);
      success = 0;
    } 
  }

  *priority = 50;

  /* Get lam_basic function pointers so that we can use them */
  lb_functions = *(lam_ssi_coll_lam_basic_query(comm, &bogus_priority));
  /* End the query */
  
  if (lam_ssi_coll_lam_basic_end_query(comm) == LAMERROR) {
    free_lcd_mem(lcd);
    success = 0;
  }
  

  lb_functions.lsca_init = lam_ssi_coll_shmem_init;
  lb_functions.lsca_finalize = lam_ssi_coll_shmem_finalize;
  /* Allreduce to confirm that all the processes have successfully attached
     the shared memory buffer */
  
  lb_functions.lsca_gather(&success, 1, MPI_INT, successes, 1, MPI_INT,
			   0, comm);
  if (rank == 0) {
    for (i = 0; i < size; ++i) {
      if (!successes[i])
	break;
    }
    if (i == size) {
      all_success = 1;
    } else {
      all_success = 0;
    }
    free(successes);
  }
  
  lb_functions.lsca_bcast(&all_success, 1, MPI_INT, 0, comm);
  
  if (all_success == 1) {
    lcd->lcd_shmem_size = shmem_size;
    lcd->lcd_control_segment_size = control_size;
    lcd->lcd_num_procs = size;
    local_coll_data = lcd;
    return &lb_functions;
  } else {
    free_lcd_mem(lcd);
    return NULL;
  }
}

int
lam_ssi_coll_shmem_end_query(MPI_Comm comm) {
  lam_ssi_coll_data_t *lcd;
  
  /* Free the shmem-coll-specific data on the communicator */
  if (comm->c_ssi_coll_data == NULL) {
    /* The module was not selected so we need to free the shared memory. Get 
       the details from locally cached pointer */
    lcd = local_coll_data;
  }
  else {
    lcd = comm->c_ssi_coll_data;
  }
    
  if (lam_ssi_coll_shmem_free_area(lcd->lcd_shmemid, lcd->lcd_semid, 
				   lcd->lcd_shared_segment_base) == LAMERROR)
    return LAMERROR;
  free_lcd_mem(lcd);
  return 0;
}

/*
 * When this module has been selected, it's all about creating and
 * initializing the module-specific data that hangs off the
 * communicator.
 */
int
lam_ssi_coll_shmem_init(MPI_Comm comm, 
			const lam_ssi_coll_actions_1_1_0_t **new_actions)
{
  int i;
  lam_ssi_coll_data_t *lcd;

  /* Initialize all the pointers that we need for implementing shared 
     memory collectives */

  comm->c_ssi_coll_data = local_coll_data;
  lcd = comm->c_ssi_coll_data;
  lcd->lcd_segment_in_use = 0;

  /* Detrmine the number of cpus */
  lcd->lcd_num_cpus = lam_ssi_coll_shmem_get_num_cpu();
  
  /* Set pointers to the control and message area */
  lcd->lcd_shared_control_base = lcd->lcd_shared_segment_base;
  lcd->lcd_shared_message_pool_base = lcd->lcd_shared_segment_base
    + lcd->lcd_control_segment_size;

  /* Initialize rest of the pointers that access shared memory. Aligning to the
   cacheline */
  
  lcd->lcd_barrier[0] = (int*) (lcd->lcd_shared_control_base);
  lcd->lcd_collective_track[0] = (int*) 
    ((char*)lcd->lcd_barrier[0] + lcd->lcd_num_procs* CACHELINESIZE);

  for (i = 1; i < lcd->lcd_num_segments + 1; ++i) {
    lcd->lcd_barrier[i] = (int*) 
      ((char*)lcd->lcd_collective_track[i - 1] + 
       lcd->lcd_num_procs* CACHELINESIZE);
    lcd->lcd_collective_track[i] = (int*) 
      ((char*)lcd->lcd_barrier[i] + lcd->lcd_num_procs* CACHELINESIZE);
  }
		
  for (i = 0; i < lcd->lcd_num_segments; ++i) {
    lcd->lcd_message_segment[i] = lcd->lcd_shared_message_pool_base +
      i * (lcd->lcd_message_pool_size / lcd->lcd_num_segments);
  }

  /* Now that that's all setup, assign the shmem functions to this
     communicator */
  *new_actions = &shmem_functions;

  /* Announce */
  if (lam_ssi_coll_verbose >= 10) {
    lam_debug(lam_ssi_coll_did, "shmem: init communicator %s", comm->c_name);
  }

  /* All done */

  lam_debug(lam_ssi_coll_did, "shmem: done initializing %s", comm->c_name);
  return 0;
}

int
lam_ssi_coll_shmem_finalize(MPI_Comm comm)
{
  /* Announce */
  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "shmem: finalize communicator %s",
	      comm->c_name);

  if (lam_ssi_coll_shmem_end_query(comm) == LAMERROR) 
    return LAMERROR;

  return 0;
}

void
free_lcd_mem(lam_ssi_coll_data_t *lcd) {
  if (lcd != NULL) {
    if (lcd->lcd_barrier != NULL)
      free(lcd->lcd_barrier);
    if (lcd->lcd_collective_track != NULL)
      free(lcd->lcd_collective_track);
    if (lcd->lcd_message_segment != NULL)
      free(lcd->lcd_message_segment);
    free(lcd);
  }
  lcd = NULL;
  /* All freed */
}
