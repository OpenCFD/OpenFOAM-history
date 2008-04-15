/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	Ohio Trollius
 *	Copyright 1996 The Ohio State University
 *	NJN
 *
 *	$Id: ssi_rpi_fault.c,v 1.2 2002/10/09 21:00:14 brbarret Exp $
 *
 *	Function:	- LAM-MPI fault tolerance
 */

#include <lam_config.h>

#include <all_list.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <lam-ssi-rpi.h>


/*
 *	lam_ssi_rpi_commfault
 *
 *	Function:	- invalidate communicators involving a dead node
 *	Accepts:	- dead node's id
 */
void
lam_ssi_rpi_commfault(int node)
{
  MPI_Comm *comm;		/* communicator */
  struct _proc **p;
  int i;

  comm = al_top(lam_comms);

  for (; comm; comm = al_next(lam_comms, comm)) {
/*
 * There is nothing to do if the local group is already dead.
 */
    if ((*comm)->c_flags & LAM_CLDEAD)
      continue;
/*
 * Check the local group.
 */
    p = (*comm)->c_group->g_procs;

    for (i = (*comm)->c_group->g_nprocs; i > 0; i--, p++) {

      if ((*p)->p_gps.gps_node == node) {
	(*comm)->c_flags |= LAM_CLDEAD;
	break;
      }
    }
/*
 * If it's an intercommunicator and the remote group is not dead, then
 * check the remote group.
 */
    if (LAM_IS_INTER(*comm) && !((*comm)->c_flags & LAM_CRDEAD)) {

      p = (*comm)->c_rgroup->g_procs;

      for (i = (*comm)->c_rgroup->g_nprocs; i > 0; i--, p++) {

	if ((*p)->p_gps.gps_node == node) {
	  (*comm)->c_flags |= LAM_CRDEAD;
	  break;
	}
      }
    }
  }
}
