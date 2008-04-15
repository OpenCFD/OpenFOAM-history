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
 * $Id: iinit.c,v 1.30 2003/05/08 22:39:53 jsquyres Exp $
 *
 *	Function:	- initializes an IMPID to run an IMPI job
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sfh.h>           

#include <kio.h>
#include <app_mgmt.h>
#include <events.h>
#include <rpisys.h>
#include <mpisys.h>
#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <lamdebug.h>
#include <mpi.h>
#include <lam-ssi-coll.h>


/*
 * Exported variables
 */
struct _proc *lam_impid_proc = 0;
MPI_Comm impid_comm = MPI_COMM_NULL; /* IMPI comm after merge */ 
MPI_Comm lam_impid_intercomm = MPI_COMM_NULL;
IMPI_Uint4 IMPI_Pk_maxdatalen = (IMPI_Uint4) ~0;
IMPI_server_info impi_server_info;
IMPI_client *lam_impi_clients = 0;
IMPI_host *lam_impi_hosts = 0;
IMPI_proc *lam_impi_procs = 0;


/*
 * private functions
 */
static int set_proc_array(struct _gps *procs, int *world_n, int start_rank, 
			  int *cid, int procs_n, int lam_procs_n, int is_impi, 
			  struct _gps *lam_procs);
static void set_proc(int i, struct _gps* g);


/*
 * private variables
 */
static int fl_debug = 0;
static int impid_rank = -1;


/*
 *	IMPI_init
 *
 *	Function:	- receive a mesage from the IMPI client daemon with
 *                        IMPI_server_info
 */
int
IMPI_Init(int *world_n, int *cid, struct _gps **worlds)
{
  int i, rank, start_rank = 0;
  char *rank_string, *server_string, *spawn_argv[30], *cmd = 0;
  int orig_procs_n, procs_n, errcode, ret, spawn_count = 0;
  MPI_Comm new_comm, cw_copy;
  MPI_Status status;
  struct _gps *orig_procs;
  struct nmsg msg;

  /* Need to make it possible to call collectives on MPI_COMM_WORLD.
     Set the override so that only lam_basic will be queried / chosen.
     This override will affect all new communicators (even the ones
     created by spawn and intercomm merge) until we reset it at the
     end of this function.

     The reason for this is that we *cannot* invoke the normal
     coll_base_init_comm() function here, because that will trigger
     all the modules' query functions, some of which may be a) doing
     communication, and b) looking for attributes that don't exist
     yet.  Remember that MPI_COMM_WORLD is going to be freed and
     re-created soon, anyway. */

  lam_ssi_coll_base_override = "lam_basic";
  lam_ssi_coll_base_init_comm(MPI_COMM_WORLD);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
/*
 * mpirun will send the IMPI arguments to rank 0
 */
  if (rank == 0) {
    LAM_ZERO_ME(msg);
    msg.nh_event = (-getpid()) & 0xBFFFFFFF;
    msg.nh_type = 4;
    msg.nh_length = 1024;
    msg.nh_flags = 0;
    msg.nh_msg = (char *) malloc(msg.nh_length);

    DBUG("IMPI_Init rank 0 waiting for args from mpirun (1024 -- %d)\n",
	 msg.nh_length);
    nrecv(&msg);
    rank_string = malloc(msg.nh_length);
    server_string = malloc(msg.nh_length);

    sscanf((char *) msg.nh_msg, "%s %s", server_string, rank_string);
    DBUG("Got impid args: %s %s\n", server_string, rank_string);
    free((char *) msg.nh_msg);

    cmd = "impid";
    spawn_argv[spawn_count++] = rank_string;
    spawn_argv[spawn_count++] = server_string;  
    spawn_argv[spawn_count++] = NULL;

    DBUG("Spawning: %s ", cmd);
    for (i = 0; i < spawn_count; i++)
      if (spawn_argv[i] != NULL)
	DBUG("%s ", spawn_argv[i]);
    DBUG("\n");
  } else
    spawn_argv[0] = NULL;

  /* Spawn the impid */

  ret = MPI_Comm_spawn(cmd, spawn_argv, 1, MPI_INFO_NULL, 0,
		       MPI_COMM_WORLD, &new_comm, &errcode); 
  MPI_Comm_set_name(new_comm, "IMPID intercommunicator");
  DBUG("Done with comm spawn\n");
  if (ret != MPI_SUCCESS) {
    DBUG("Didn't like comm spawn\n");
    return LAMERROR;
  }
  if (rank == 0)
    for (i = 0; i < spawn_count; i++)
      if (spawn_argv[i] != NULL)
	free(spawn_argv[i]);

  /* Get the gps of the impid which should be the only one in the
   * remote group. This will be used as the proxy entry in the new
   * mpi_comm_world that is expanded to represent the impi world.
   * Also, since the impid is technically not in MPI_COMM_WORLD, we
   * don't ever want it to be matched with a LAMGPS_FULLCMP, so set
   * its g_rank to -1. */

  lam_impid_proc = new_comm->c_rgroup->g_procs[0];
  lam_impid_proc->p_gps.gps_grank = -1;
  gimpid = lam_impid_proc->p_gps;

  /* Merge into an intracomm, and free the original intercommunicator */

  DBUG("About to intercomm merge\n");
  MPI_Intercomm_merge(new_comm, 0, &impid_comm);
  DBUG("Did intercomm merge\n");
  MPI_Comm_set_name(impid_comm, "IMPID intracommunicator");

  /* We cannot immediately free new_comm because it is still
     referenced as the parent communicator in the impid.  Hence, we
     stash it away until IMPI_Finalize. */

  lam_impid_intercomm = new_comm;

  /* The impid_comm will be a special communicator for meta messages
     and data on to and from the impid -- we don't it have to have a
     shadow communicator.  This is used as a sentinal value elsewhere;
     we don't want the impid_comm to be considered an "IMPI"
     communicator. */

  MPI_Comm_free(&impid_comm->c_shadow);

  /* Contribute our PID to the IMPI daemon's array of all the PID's.
     We could have the impid examine the GPS structures in the correct
     MPI_Comm, but it is nicer to have the impid be as pure an MPI
     application as possible (and not dip below the MPI layer of
     abstraction). */

  MPI_Comm_size(impid_comm, &impid_rank);
  impid_rank--;
  rank = getpid();
  DBUG("About to MPI_Gather\n");
  MPI_Gather(&rank, 1, MPI_INT, 0, 0, MPI_INT, impid_rank, impid_comm);
  DBUG("Did MPI_Gather\n");

  /* Now wait for the response from the IMPI client daemon with all
   * the information from IMPI startup (chapter 2, IMPI std).  The
   * impid's rank is the last one so it's size - 1.  */
  
  DBUG("About to MPI_Recv\n");
  MPI_Recv(&impi_server_info, sizeof(impi_server_info) / sizeof(int), 
	   MPI_INT, impid_rank, IMPI_SERVER_INFO_TAG, impid_comm, &status);
  
  DBUG("Got message from IMPI client daemon: total size %d, start rank %d\n", 
       impi_server_info.nprocs, impi_server_info.start_rank);

  /* Was there an error? */
 
  if (impi_server_info.nclients < 0) {
    /* The IMPID will print out an appropriate error message already;
       we don't really want to print out any more message here, so we
       should just die silently. */
    DBUG("Honk if you love silence\n");
    kexit(1);
  }

  /* Receive the other supplemental information */

  lam_impi_clients = malloc(sizeof(IMPI_client) * impi_server_info.nclients);
  if (lam_impi_clients == 0) {
    return LAMERROR;
  }
  lam_impi_hosts = malloc(sizeof(IMPI_host) * impi_server_info.nhosts);
  if (lam_impi_hosts == 0) {
    free(lam_impi_clients);
    return LAMERROR;
  }
  lam_impi_procs = malloc(sizeof(IMPI_proc) * impi_server_info.nprocs);
  if (lam_impi_procs == 0) {
    free(lam_impi_hosts);
    free(lam_impi_clients);
    return LAMERROR;
  }
  DBUG("Receiving: %lu ints of clients, %lu ints of hosts, and %lu ints of procs\n",
       sizeof(IMPI_client) / 4 * impi_server_info.nclients,
       sizeof(IMPI_host) / 4 * impi_server_info.nhosts,
       sizeof(IMPI_proc) / 4 * impi_server_info.nprocs);

  MPI_Recv(lam_impi_clients, 
	   sizeof(IMPI_client) / 4 * impi_server_info.nclients,
	   MPI_INT, impid_rank, IMPI_SERVER_INFO_TAG, impid_comm, &status);
  MPI_Recv(lam_impi_hosts, 
	   sizeof(IMPI_host) / 4 * impi_server_info.nhosts,
	   MPI_INT, impid_rank, IMPI_SERVER_INFO_TAG, impid_comm, &status);
  MPI_Recv(lam_impi_procs, 
	   sizeof(IMPI_proc) / 4 * impi_server_info.nprocs,
	   MPI_INT, impid_rank, IMPI_SERVER_INFO_TAG, impid_comm, &status);

  IMPI_Pk_maxdatalen = impi_server_info.max_datalen;
  start_rank = impi_server_info.start_rank;

  /* This is the right value for the number of original lam nodes b/c
   * IMPI jobs will never come from an MPI_Comm_spawn.  Need to malloc
   * out somewhere to hold the original proc array and copy the info
   * in to hold for later. 
   */
  orig_procs_n = *world_n;  
  orig_procs = (struct _gps *)
    malloc((unsigned) orig_procs_n * sizeof(struct _gps));
  memcpy(orig_procs, *worlds, sizeof(struct _gps) * orig_procs_n);
  free(*worlds);

  DBUG("IN IMPI_Init, the size of the world before get_proc is %d\n", 
       *world_n);
  *world_n = impi_server_info.nprocs;
  DBUG("IN IMPI_Init, the size of the world from IMPI server is %d\n", 
       *world_n);

  /* Malloc a new proc_array to hold the procs_n sent to us by the
     server -- returns the new world_n and procs_n */

  if (lam_get_proc_array(worlds, world_n, cid, &procs_n, 1) == LAMERROR)
    return LAMERROR;
  DBUG("IN IMPI_Init, the size of the world after get_proc is %d\n", *world_n);

  if (set_proc_array(*worlds, world_n, start_rank, cid, procs_n, 
		     orig_procs_n, 1, orig_procs) == LAMERROR)
    return LAMERROR;
  free(orig_procs);

  /* free the original MCW */

  DBUG("Freeing MPI_COMM_WORLD\n");
  MPI_Group_free(&MPI_COMM_WORLD->c_group);
  MPI_COMM_WORLD->c_errhdl->eh_refcount--;
  cw_copy = MPI_COMM_WORLD;
  DBUG("Calling al_delete\n");
  al_delete(lam_comms, al_find(lam_comms, &cw_copy));

  /* Note that the override for coll modules is still in place; we
     need lam_basic to initally be chosen here -- we'll reset it to
     impi below.  This is because the impi coll module's query()
     function uses LAM_IS_IMPI(comm), which, when it is called in the
     normal process of things, will return false (because the
     additional IMPI procs have not yet been added yet).  Rather than
     changing that whole complicated sequence of events, just reset
     the coll module a few lines later in this file. :-) */

  DBUG("Redo the mpi comm world and comm self\n");
  if (lam_init_comm_world(*worlds, *world_n)) {
    free(*worlds);
    return LAMERROR;
  }

  /* IMPI mandates that MPI_COMM_WORLD's CID is 0 */

  MPI_COMM_WORLD->c_contextid = 0; 

  /* Check for bozo case where there is only one IMPI client, and
     therefore no shadow communicator */

  if (MPI_COMM_WORLD->c_shadow != NULL)
    MPI_COMM_WORLD->c_shadow->c_contextid = 1;

  /* Now that it's all setup, reset the coll module to be impi */

  lam_ssi_coll_base_override = "impi";
  lam_ssi_coll_base_init_comm(MPI_COMM_WORLD);

  /* Random note: IMPI attributes on MPI_COMM_WORLD will be added
     later */

  DBUG("Finished *** redoing *** MCW\n");
  
  if (IMPI_Make_dtype()) {
    free(*worlds);
    return LAMERROR;
  }

  /* All done */

  lam_ssi_coll_base_override = NULL;
  DBUG("Finished IMPI_init\n"); 
  return 0;
}


static int
set_proc_array(struct _gps *gpss, int *world_n, int start_rank,
	       int *cid, int gpss_n, int lam_gpss_n, int is_impi, 
	       struct _gps *lam_gpss)
{
  struct _proc  *proc;                  /* favourite pointer */
  int           i;

  /* Insert the original lam gpss array in the new enlarged impi proc
   * array in their correct rank spot */
 
  DBUG("The lam_gpss_n is SET_PROC is %d\n", lam_gpss_n);

  memcpy(gpss + start_rank, lam_gpss, sizeof(struct _gps) * lam_gpss_n);
  for (i = 0; i < lam_gpss_n; i++)
    gpss[start_rank + i].gps_grank = i + start_rank;
  for (proc = lam_topproc(); proc != NULL; proc = lam_nextproc())
    if (proc != lam_impid_proc) {
      DBUG("Resetting rank's %d proc\n", proc->p_gps.gps_grank + start_rank);
      proc->p_gps.gps_grank += start_rank;
    }
  
/* If this is an IMPI job, we need to shift the parents down to the
 * end of the gpss list -- there may be some ranks between the last
 * local LAM rank and the end of the list (where the parents should
 * go). This is added for future compatability -- if we can ever
 * mpi_comm_spawn the impi jobs.
 */
  DBUG("Doing some IMPI checking -- shift parents down?\n");
  if (gpss_n != start_rank + *world_n + _kio.ki_parent) {
    int pstart = start_rank + *world_n;
    int pslot_start = gpss_n - _kio.ki_parent - 1;
    for (i = _kio.ki_parent - 1; i >= 0; i--) {
      DBUG("Move parent %d to slot %d\n", pstart + 1, pslot_start + i);
      gpss[pslot_start + i] = gpss[pstart + i];
    }
  }

  /*
   * Now we need to fill in the rest of the "impi world" slots on either side
   * of the local LAM entires in gpss with copies of the impid gps so
   * that things get routed appropriately
   */
  DBUG("Filling in other gpss\n");
  DBUG("The start_rank is SET_PROC is %d\n", start_rank); 
  for (i = 0; i < start_rank; i++) {
    set_proc(i, &gpss[i]);
  }
  for (i = start_rank + lam_gpss_n; i < *world_n; i++) {
    set_proc(i, &gpss[i]);
  }

  DBUG("Next phase of IMPI styartup complete\n"); 
  return 0;
}


/*
 * Zero out the rest of the proc; we only use this proxy proc as a
 * sentinel value anyway -- it gets intercepted in
 * lamreqs.c:_mpi_req_build() and replaced with a pointer to the
 * "real" impid proc (the LAM progression engine was not designed to
 * have concurrent accesses to the same underlying proc through
 * proxies).
 */
static void
set_proc(int i, struct _gps* g)
{
  struct _proc *p;

  *g = gimpid;
  (*g).gps_grank = i;

  p = lam_procadd(g);
  DBUG("Adding in IMPID proxy for rank %d: %p (lam_impid_proc %p)\n", 
       i, p, lam_impid_proc);

  /* Have to do a little more mucking around here since we joined the
     game late */

  p->p_ger_nsnd = 0;
  p->p_mode = 0;
  memset(&p->p_rpi, 0, sizeof(p->p_rpi));
}


#endif /* LAM_WANT_IMPI */

