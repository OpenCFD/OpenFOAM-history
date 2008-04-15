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
 * $Id: ssi_coll_smp_util.c,v 1.11 2003/10/23 11:22:31 jsquyres Exp $
 *
 *	Function:	- SMP collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-smp-config.h>

#include <etc_misc.h>
#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>
#include <mpisys.h>
#include <rpisys.h>


/*
 * Setup global ranks for one ccordinator. Each coordinator calls this function
 * so that global ranks of the processes in the local communicator are filled
 * into the lcd_coord_global_ranks array.
 */
 
int
lam_ssi_coll_smp_set_global_ranks(MPI_Comm comm, MPI_Group g2, int coord_rank)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  struct _proc **p1, **p2;
  MPI_Group g1;
  int i, j, k = -1;

  MPI_Comm_group(comm, &g1);
 
  if (g1 == MPI_GROUP_NULL || g2 == MPI_GROUP_NULL) 
    return LAMERROR;
 
  p1 = g1->g_procs;
  p2 = g2->g_procs;

  /* loop through processes in local comm */
  
  for (i = 0; i < g2->g_nprocs; ++i, ++p2) {

    /* loop through processes in global comm if match is found then store
       its global rank in the array */
    
    for (j = k + 1; j < g1->g_nprocs; ++j, ++p1)
      if (*p1 == *p2) {
        lcd->lcd_coord_global_ranks[i] = j;
	++p1;
	k = j;
        break;
      }
  }

  MPI_Group_free(&g1);
  return 0;
}

/* Checks if the smp scan algorithm can be applied for the provided
   communicator. Basically checks for consecutive ranks in the array */
int
lam_ssi_coll_smp_check_for_scan(int *ranks, int coord_size, int *sizes)
{
  int i, j, count = 0;
  
  for (i = 0; i < coord_size; ++i) {
    for (j = 0; j < sizes[i] - 1; ++j) {
      if (ranks[count + 1] != ranks[count] + 1)
	return 0;
      count++;
    }
    count++;
  }
  return 1;
}

/*
 * Setup the rooted coordinator arrays on the module-specific data
 * hanging off the communicator.  Happily, this does not need to be
 * thread safe (and hence, it's ok to only have one copy of this data
 * struct hanging off the communicator) because MPI says that only one
 * thread can be invoking a collective on a given communicator at a
 * time.
 */
int 
lam_ssi_coll_smp_set_root(MPI_Comm comm, int root)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  int local_rank, coord_rank;
  int am_coordinator;
  MPI_Group comm_group, local_group, coord_group;
  char name[MPI_MAX_OBJECT_NAME];

  /* See if we've already computed for this root.  If so, we can
     return immediately */

  if (lcd->lcd_local_roots[root] != MPI_UNDEFINED)
    return 0;

  /* Nope -- we haven't calculated for this root yet. */
  /* Translate the rank of the outter root into my local comm */

  MPI_Comm_group(comm, &comm_group);
  MPI_Comm_group(lcd->lcd_local_comm, &local_group);
  MPI_Group_translate_ranks(comm_group, 1, &root,
			    local_group, &local_rank);

  /* If the translated rank was in my local comm, then the translated
     rank is root in my local comm. */

  if (local_rank != MPI_UNDEFINED)
    lcd->lcd_local_roots[root] = local_rank;

  /* Otherwise, the outter root was not in my local comm.  So
     arbitrarily select local rank 0 to be the root. */

  else
    lcd->lcd_local_roots[root] = 0;

  /* So am I a coordinator? */

  MPI_Comm_rank(lcd->lcd_local_comm, &local_rank);
  am_coordinator = (local_rank == lcd->lcd_local_roots[root]);

  /* Make the coordinator communicator */

  MPI_Comm_split(comm, am_coordinator ? 1 : MPI_UNDEFINED,
		 0, &(lcd->lcd_coord_comms[root]));

  /* If I am the coordinator and called when root = 0, store the global ranks
     (in comm) corresponding to the local processes. */ 
     
  if ((am_coordinator) && (root == 0)) {
    int rank_coord, err;
    MPI_Comm_rank(lcd->lcd_coord_comms[root], &rank_coord);
    err = lam_ssi_coll_smp_set_global_ranks(comm, local_group, rank_coord);
    if (err != MPI_SUCCESS) 
      return err;
  }

  if (lcd->lcd_coord_comms[root] != MPI_COMM_NULL) {
    int rank, rank2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(lcd->lcd_coord_comms[root], &rank2);

    lcd->lcd_coord_comms[root]->c_flags |= LAM_CHIDDEN;

    snprintf(name, MPI_MAX_OBJECT_NAME - 1, 
             "SSI:coll:smp:coord comm for CID %d", comm->c_contextid);
    name[MPI_MAX_OBJECT_NAME - 1] = '\0';
    MPI_Comm_set_name(lcd->lcd_local_comm, name);
  }

  /* Translate the outter rank into the coordinator communicator; that
     is the root of the coordinator communicator */


  if (am_coordinator) {
    MPI_Comm_group(lcd->lcd_coord_comms[root], &coord_group);
    MPI_Group_translate_ranks(comm_group, 1, &root, 
			      coord_group, &coord_rank);
    lcd->lcd_coord_roots[root] = coord_rank;
    
    MPI_Group_free(&coord_group);
  }

  /* All done */

  MPI_Group_free(&comm_group);
  MPI_Group_free(&local_group);
  return 0;
}
