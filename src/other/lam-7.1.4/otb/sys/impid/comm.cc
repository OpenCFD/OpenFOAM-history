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
 * $Id: comm.cc,v 1.16 2003/05/08 22:33:23 jsquyres Exp $
 *
 *	Function:	- make and destroy "fake" communicators
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <unistd.h>

#include <iostream>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <hostack_mgmt.h>
#include <lamdebug-cc.h>
extern "C" {
#include <lam-ssi-coll.h>
}

#include <mpisys.h>
#include <rpisys.h>

using std::endl;


/*
 * private variables
 */
static Debug debug(false);


/*
 *	comm_make
 *
 *	Function:	- make a "fake" communicator
 *
 *      Accepts:        - context ID
 *                      - rank of who the IMPID is supposed to be
 *                        impersonating (relative to cid)
 *                      - rank of the destination (relative to cid)
 *                      - rank of other process in full MPI_COMM_WORLD
 *      Returns:        - new communicator containing us as rank 0, 
 *                        them as rank 1
 */
MPI_Comm
comm_make(IMPI_Uint8 cid, int src_rank, int dest_rank, int dest_world_rank)
{
  MPI_Comm comm = 0;
  int start_rank, i;
  MPI_Group newgroup;
  struct _proc *dest_proc;
  int size = LAM_max(src_rank, dest_rank) + 1;
  IMPI_Uint8 local_id;

  // Make a new group.  Really disgusting, but the TCP RPI identifies
  // messages as who they say they are, not from what socket they came
  // from.  So fake out our communicator and say that our rank in the
  // group is actually the source rank that the message should be
  // coming from.

  newgroup = (MPI_Group) malloc(sizeof(struct _group) + 
				(sizeof(struct _proc*) * size));
  newgroup->g_nprocs = size;
  newgroup->g_myrank = src_rank;
  newgroup->g_refcount = 1;
  newgroup->g_f77handle = 0;
  newgroup->g_procs = (struct _proc**) ((char*) newgroup + 
					sizeof(struct _group));

  // Put my proc in the g_procs slot of all the ranks so that when
  // MPI_Group_free comes through when this comm gets freed, it has
  // something to decrement the refcount for.

  for (i = 0; i < size; i++)
    if (i != dest_rank) {
      newgroup->g_procs[i] = lam_myproc;
      lam_myproc->p_refcount++;
    }

  // We need to find the proc of dest rank.  Take advantage of the
  // fact that our group of LAM procs was carefully constructed; proc
  // 0 is us, the rest are (in order) the procs of the source
  // MPI_COMM_WORLD of our local LAM

  start_rank = 
    lam_impi_hosts[lam_impi_procs[dest_world_rank].ip_hostrank].ih_procrank;
  dest_proc = parent_intra->c_group->g_procs[dest_world_rank - start_rank];
  debug << "comm_make: Got dest_world_rank " << dest_world_rank 
	<< " calc local_rank " << dest_world_rank - start_rank << endl;
  newgroup->g_procs[dest_rank] = dest_proc;
  dest_proc->p_refcount++;

  // Make the new communicator

  lam_comm_new((int) cid, newgroup, MPI_GROUP_NULL, LAM_CIMPI, &comm);
  lam_ssi_coll_base_init_comm(comm);  

  // A second CID is necessary because it will be possible to have
  // multiple "fake" communicators with the same *real* CID -- so we
  // need to be able to tell them apart.

  local_id = get_new_localcid();
  memcpy(comm->c_reserved, &local_id, sizeof(local_id));
  comm->c_errhdl->eh_refcount++;

  // Insert into the global list of communicators

  al_insert(lam_comms, &comm);
	
  return comm;
}


/*
 *	comm_free
 *
 *	Function:	- free a fake communicator that was created with 
 *                        make_comm
 *
 *      Accepts:        - communicator to free
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
int
comm_free(MPI_Comm comm)
{
  IMPI_Uint8 local_cid;

  /* Release the local CID */

  memcpy(&local_cid, comm->c_reserved, sizeof(local_cid));
  release_localcid(local_cid);

  int ret = MPI_Comm_free(&comm);
  if (ret == MPI_SUCCESS)
    return 0;
  return ret;
}

#endif
