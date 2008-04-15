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
 * $Id: ssi_coll_smp.c,v 1.21 2004/03/02 03:43:38 jsquyres Exp $
 *
 *	Function:	- SMP collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-smp-config.h>

#include <string.h>

#include <etc_misc.h>
#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>
#include <mpisys.h>
#include <blktype.h>

/*
 * Extern Functions
 */
extern const lam_ssi_coll_actions_1_1_0_t  *lam_ssi_coll_lam_basic_query(MPI_Comm comm, int *priority);
extern int lam_ssi_coll_lam_basic_end_query(MPI_Comm comm);

/*
 * Local functions
 */
static int alloc_lcd_item(lam_ssi_coll_data_t *lcd, void *ptr, int size);
static void free_lcd(lam_ssi_coll_data_t *lcd);
static void free_lcd_mem(lam_ssi_coll_data_t *lcd);

/*
 * Local variables
 */
static lam_ssi_coll_actions_1_1_0_t smp_functions = {

  /* Per-communicator initialization and finalization functions */

  lam_ssi_coll_smp_init,
  lam_ssi_coll_smp_finalize,

  /* Checkpoint / restart functions */

  lam_ssi_coll_base_empty_checkpoint,
  lam_ssi_coll_base_empty_continue,
  lam_ssi_coll_base_empty_restart,
  lam_ssi_coll_base_empty_interrupt,

  /* Collective function pointers */

  /* Note that many of these will be replaced with their lam_basic
     counterparts in the C code, below.  They are replaced at run-time
     (vs. compile time) because lam_basic may decide to use different
     algorithms depending on the size of the communicator.  So we
     leave all that logic in lam_basic, and simply query lam_basic,
     then replace our NULL function pointers with the ones that
     lam_basic chose. */

  /* Allgather: use lam_basic (still needs to be wide-area
     optimized) */
  lam_ssi_coll_smp_allgather,
  /* Allgatherv: use lam_basic (still needs to be wide-area
     optimized) */
  lam_ssi_coll_smp_allgatherv,
  /* Allreduce */
  lam_ssi_coll_smp_allreduce,
  /* Alltoall -- use lam_basic */
  NULL,
  /* Alltoallv -- use lam_basic */
  NULL,
  /* Alltoallw -- use lam_basic */
  NULL, 
  /* Barrier */
  lam_ssi_coll_smp_barrier,
  /* Bcast */
  1,
  lam_ssi_coll_smp_bcast,
  /* exscan -- use lam_basic */ 
  NULL, 
  /* Gather -- use lam_basic */
  NULL,
  /* Gatherv -- use lam_basic */
  NULL,
  /* Reduce */
  1,
  lam_ssi_coll_smp_reduce,
  /* Reduce scatter: use lam_basic (still needs to be wide-area
     optimized) */
  lam_ssi_coll_smp_reduce_scatter,
  /* Scan: use lam_basic (still needs to be wide-area optimized) */
  lam_ssi_coll_smp_scan,
  /* Scatter -- use lam_basic */
  NULL,
  /* Scatterv -- use lam_basic */
  NULL,
};

/* Save the lam_basic collective function pointers between query and
   init.  This is only safe because LAM is not MPI_THREAD_MULTIPLE, 
   so there is no way a new communicator can magically "pop up"
   between the query and init calls */
static lam_ssi_coll_actions_1_1_0_t lb_functions;


/*
 * Initial query function that is invoked during MPI_INIT, allowing
 * this module to indicate what level of thread support it provides.
 */
int 
lam_ssi_coll_smp_thread_query(int *thread_min, int *thread_max)
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
 * Only allow this module to be selected if there is more than 1 node
 * in this communicator, and at least one node has more than one
 * process on it.
 */
const lam_ssi_coll_actions_1_1_0_t *
lam_ssi_coll_smp_query(MPI_Comm comm, int *priority)
{
  int i;
  int size;
  int *nodes;
  int max_node;
  int node;
  int last_node;
  int bogus_priority;
  int have_multi_nodes;
  int have_multi_proc_node;

  /* Double check that this is not an intercommunicator.  This is
     important because even though LAM doesn't [yet] support
     collectives on intercommunicators, the init phase (below) invokes
     sub-collectives during the setup.  So even if no collectives are
     ever invoked by the user program, we still do some internally.
     And that's bad (let's not forget that this can happen for
     MPI_COMM_PARENT).  So just disallow this communicator from being
     selected for intercommunicators.  lam_basic will likely be
     chosen, which is ok, because it doesn't invoke any
     sub-collectives during init.  If the user invokes a collective on
     an intercommunicator, it will result in a run-time error, but
     that's ok.  That's their fault, not ours.  ;-) */

  if (LAM_IS_INTER(comm))
    return NULL;

  /* Find the min node number */

  MPI_Comm_size(comm, &size);
  max_node = 0;
  for (i = 0; i < size; ++i)
    max_node = LAM_max(max_node, comm->c_group->g_procs[i]->p_gps.gps_node);
  ++max_node;

  /* Alloc and zero out an array of counters to count how many procs
     on each node.  This is somewhat space-inefficient, since we
     simply alloc out enough counters for [0, max_node_number], but
     it's easier than making dense node counter packing.  Plus, this
     is during communicator intialization, so effenciency isn't
     super-crucial.  And we'll be freeing this array right away
     anyway, so it's no big deal. */

  nodes = malloc(max_node * sizeof(int));
  if (nodes == NULL)
    return NULL;
  memset(nodes, 0, max_node * sizeof(int));

  /* WARNING: This currently assumes intra-communicators.  Although
     inter-communicators probably fit this model quite well (and could
     probably benefit from N-level algorithms), the code will have to
     be resturctured a bit before being suitable for
     inter-communicators. */

  /* Add up how many procs are on each node.  Check as we're going
     along if we have the Right Conditions. */

  for (i = have_multi_nodes = have_multi_proc_node = 0, last_node = -1;
       i < size && !(have_multi_nodes && have_multi_proc_node); ++i) {
    node = comm->c_group->g_procs[i]->p_gps.gps_node;

    if (last_node != node && last_node != -1)
      have_multi_nodes = 1;
    last_node = node;

    ++nodes[node];
    if (nodes[node] > 1)
      have_multi_proc_node = 1;
  }
  free(nodes);

  /* If we didn't meet the condition, bail */

  if (!(have_multi_nodes && have_multi_proc_node))
    return NULL;

  /* Ok, so we meet the conditions.  We can now say "yes, please
     consider me for selection." */

  *priority = 50;

  /* Return API functions from the lam_basic module so that we can
     invoke MPI_COMM_SPLIT in the init function, below.  Be sure to
     override the initi/finalize functions, though, so that the
     functions in this file are called (instead of the lam_basic
     init/finalize functions). */

  lb_functions = *(lam_ssi_coll_lam_basic_query(comm, &bogus_priority));
  /* End the query */
  if (lam_ssi_coll_lam_basic_end_query(comm) == LAMERROR) {
    return NULL;
  }
  lb_functions.lsca_init = lam_ssi_coll_smp_init;
  lb_functions.lsca_finalize = lam_ssi_coll_smp_finalize;
  return &lb_functions;
}

int
lam_ssi_coll_smp_end_query(MPI_Comm comm) {
  /* Nothing to do right now because we don't allocate any structure during
     smp query */
  
  return 0;
}

/*
 * When this module has been selected, it's all about creating and
 * initializing the module-specific data that hangs off the
 * communicator.
 */
int
lam_ssi_coll_smp_init(MPI_Comm comm, 
		      const lam_ssi_coll_actions_1_1_0_t **new_actions)
{
  int i;
  lam_ssi_coll_data_t *lcd;
  char name[MPI_MAX_OBJECT_NAME];
  
  /* Note: Some of the loops and logic in this function could probably
     be combined for greater efficiency.  However, in the interest of
     clarity (because this is already extremely confusing!), they have
     been kept separated into distinct loops. */

  /* Build up the data structure to hang off the communicator that
     holds basic information about this communicator */
  
  comm->c_ssi_coll_data = NULL;
  lcd = malloc(sizeof(lam_ssi_coll_data_t));
  if (lcd == NULL)
    return LAMERROR;

  /* save the lam basic functions, and get them out of new_actions */
  lcd->lb_functions = lb_functions;

  /* Allocate arrays for the local data structure */

  MPI_Comm_size(comm, &lcd->lcd_size);
  if (alloc_lcd_item(lcd, &lcd->lcd_local_roots, 
		     sizeof(int) * lcd->lcd_size) == LAMERROR ||
      alloc_lcd_item(lcd, &lcd->lcd_coord_roots,
		     sizeof(int) * lcd->lcd_size) == LAMERROR ||
      alloc_lcd_item(lcd, &lcd->lcd_coord_comms,
		     sizeof(MPI_Comm) * lcd->lcd_size) == LAMERROR)
    return LAMERROR;
  
  /* Initialize the various arrays that need initializing */
  
  for (i = 0; i < lcd->lcd_size; ++i) {
    lcd->lcd_local_roots[i] = MPI_UNDEFINED;
    lcd->lcd_coord_comms[i] = MPI_COMM_NULL;
    lcd->lcd_coord_roots[i] = MPI_UNDEFINED;
  }
  
  lcd->lcd_scan_comm = MPI_COMM_NULL;

  /* Set default values */

  lcd->lcd_crossover = 
    lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_CROSSOVER);
  lcd->lcd_reduce_crossover = 
    lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_REDUCE_CROSSOVER);

  /* Hang lcd off the communicator so that we can make collective
     calls, below */

  comm->c_ssi_coll_data = lcd;

  /* Make communicator for my local ranks */

  MPI_Comm_split(comm, getnodeid(), 0, &(lcd->lcd_local_comm));
  lcd->lcd_local_comm->c_flags |= LAM_CHIDDEN;
  snprintf(name, MPI_MAX_OBJECT_NAME - 1, 
           "SSI:coll:smp:local comm for CID %d", comm->c_contextid);
  name[MPI_MAX_OBJECT_NAME - 1] = '\0';
  MPI_Comm_set_name(lcd->lcd_local_comm, name);
  MPI_Comm_size(lcd->lcd_local_comm, &(lcd->lcd_local_size));

  /* allocate space for lcd_coord_global_ranks array which will be used in
     set_global_ranks function. */

  if (alloc_lcd_item(lcd, &lcd->lcd_coord_global_ranks,
                     sizeof(int) * lcd->lcd_size) == LAMERROR)
    return LAMERROR;

  /* Initialize the gloabl_ranks array. */
 
  for (i = 0; i < lcd->lcd_size; ++i) 
    lcd->lcd_coord_global_ranks[i] = MPI_UNDEFINED;

  /* Optimization: setup the common case -- where root == 0 (same as
     unrooted) */
  
  lam_ssi_coll_smp_set_root(comm, 0);

  /* Now we know that we have local comm and coord_comm set up properly. If
     my rank in coord_comms[0] is 0 then allocate space for 
     lcd_local_comm_size. */

  lcd->lcd_local_comm_size = NULL;

  if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
    int *global_disp = NULL;
    int *temp_array_for_holding_global_ranks = NULL;
    int coord_rank, coord_size;
    
    MPI_Comm_rank(lcd->lcd_coord_comms[0], &coord_rank);
    MPI_Comm_size(lcd->lcd_coord_comms[0], &coord_size); 
    
    if (coord_rank == 0) {
      /* I will be main coordinator */
      if ((temp_array_for_holding_global_ranks =
	   (int*) malloc(lcd->lcd_size * sizeof(int))) == NULL)
	return LAMERROR;
      for (i = 0; i < lcd->lcd_size; ++i) 
	temp_array_for_holding_global_ranks[i] = MPI_UNDEFINED;

      if (alloc_lcd_item(lcd, &lcd->lcd_local_comm_size,
			 sizeof(int) * coord_size) == LAMERROR)
	return LAMERROR;
      for (i = 0; i < coord_size; ++i)
	lcd->lcd_local_comm_size[i] = MPI_UNDEFINED;
    } 
    
    /* Now we have allocated sufficient space for required data structure. 
       Do a gather with coordinator 0 as root so that coordinator 0 will know
       everything about processes controlled by rest of the coordinators. This
       operation occurs among coordinators during init and does not harm the
       completion time for collective operations. */
    
    MPI_Gather(&lcd->lcd_local_size, 1, MPI_INT, lcd->lcd_local_comm_size, 1,
	       MPI_INT, 0, lcd->lcd_coord_comms[0]);

    if (coord_rank == 0) {
      /* form global positioning array. */

      global_disp = (int*) malloc(sizeof(int) * coord_size);
      if (global_disp == NULL)
	return ENOMEM;

      global_disp[0] = 0;
      for (i = 1; i < coord_size; i++) 
	global_disp[i] = lcd->lcd_local_comm_size[i - 1] + global_disp[i - 1];
      
    }

    /* Gatherv to collect global ranks of all the processes. */

    MPI_Gatherv(lcd->lcd_coord_global_ranks, lcd->lcd_local_size, 
		MPI_INT, temp_array_for_holding_global_ranks, 
		lcd->lcd_local_comm_size, global_disp, MPI_INT, 0, 
		lcd->lcd_coord_comms[0]);
    
    /* Now copy the temporary array to lcd_coord_global_ranks and destroy the
       temporary array which was used for MPI_Gatherv */
    if (coord_rank == 0) {
      for (i = 0; i < lcd->lcd_size; ++i) 
	lcd->lcd_coord_global_ranks[i] = 
	  temp_array_for_holding_global_ranks[i];
      free(temp_array_for_holding_global_ranks);
    }

    /* Free global_disp array */
    if (global_disp != NULL)
      free(global_disp);

    /* Following part is specifically required for scan operation. Determine
       if the smp scan can be applied to the comm. If yes broadcast the result
       to all other coordinators. */
    
    if (coord_rank == 0) {
      lcd->lcd_is_scan_applicable = 
        lam_ssi_coll_smp_check_for_scan(lcd->lcd_coord_global_ranks,
                                        coord_size, lcd->lcd_local_comm_size);
    }

    MPI_Bcast(&(lcd->lcd_is_scan_applicable), 1, MPI_INT, 0, 
		lcd->lcd_coord_comms[0]);

  }

  /* Broadcast the flag for scan in local comm so that each process knows 
     which algorithm to apply */

  MPI_Bcast(&lcd->lcd_is_scan_applicable, 1, MPI_INT, 0, 
	    lcd->lcd_local_comm);
   
  /* There are two categories of functions that we steal from
     lam_basic: those that are already wide-area optimal, and those
     that we ran out of time to wide-area optimize before
     release. :-\ */
  /* Allready wide-area optimized */

  if (!lcd->lcd_is_scan_applicable)
    smp_functions.lsca_scan = lcd->lb_functions.lsca_scan;

  /* Now that that's all setup, assign the smp functions to this
     communicator */

  *new_actions = &smp_functions;

  /* Announce */

  if (lam_ssi_coll_verbose >= 10) {
    lam_debug(lam_ssi_coll_did, "smp: init communicator %s", comm->c_name);
    lam_debug(lam_ssi_coll_did, "smp: crossover: %d", lcd->lcd_crossover);
    lam_debug(lam_ssi_coll_did, "smp: reduce crossover: %d", 
              lcd->lcd_reduce_crossover);
  }

  /* All done */

  lam_debug(lam_ssi_coll_did, "smp: done initializing %s", comm->c_name);
  return 0;
}


int
lam_ssi_coll_smp_finalize(MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  /* Announce */

  if (lam_ssi_coll_verbose >= 10)
    lam_debug(lam_ssi_coll_did, "smp: finalize communicator %s",
	      comm->c_name);

  /* Free the smp-coll-specific data on the communicator */
  if (lam_ssi_coll_smp_end_query(comm) == LAMERROR)
    return LAMERROR;

  free_lcd(lcd);

  return 0;
}


/*
 * Wrapper around allocating a single item in the lcd.  If we fail,
 * free everything already allocated in the lcd and return an error.
 */
static int 
alloc_lcd_item(lam_ssi_coll_data_t *lcd, void *ptr, int size)
{
  char **p = (char**) ptr;

  *p = malloc(size);
  if (*p == NULL) {
    free_lcd_mem(lcd);

    return LAMERROR;
  }

  return 0;
}


/*
 * Free all the communicators and memory associated with the lcd
 */
static void 
free_lcd(lam_ssi_coll_data_t *lcd)
{
  int i;

  /* Free any communicators that we might have */

  MPI_Comm_free(&(lcd->lcd_local_comm));
  for (i = 0; i < lcd->lcd_size; ++i)
    if (lcd->lcd_coord_comms[i] != MPI_COMM_NULL)
      MPI_Comm_free(&lcd->lcd_coord_comms[i]);

  /* Now free all the memory */

  free_lcd_mem(lcd);
}


/*
 * Free all the memory associated with the lcd
 */
static void 
free_lcd_mem(lam_ssi_coll_data_t *lcd)
{
  if (lcd->lcd_local_roots != NULL)
    free(lcd->lcd_local_roots);

  if (lcd->lcd_coord_comms != NULL)
    free(lcd->lcd_coord_comms);
  if (lcd->lcd_coord_roots != NULL)
    free(lcd->lcd_coord_roots);
  
  if (lcd->lcd_local_comm_size != NULL) 
    free(lcd->lcd_local_comm_size);
  if (lcd->lcd_coord_global_ranks != NULL) 
    free(lcd->lcd_coord_global_ranks);
  
  free(lcd);
  
}
