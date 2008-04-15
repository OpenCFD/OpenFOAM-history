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
 * $Id: lamnbarrier.c,v 6.4 2003/02/05 20:20:51 ssankara Exp $
 *
 */

#include <lam_config.h>

#include <etc_misc.h>
#include <mpisys.h>
#include <rpisys.h>

/*
 * private functions
 */
static int nbarrier_lin(struct _proc **procs, int nprocs);
static int nbarrier_log(struct _proc **procs, int nprocs);
static void setup_nmsgs(struct nmsg *local, struct nmsg *remote);


/*
 * nsend/nrecv based barrier
 */
int
lam_nbarrier(struct _proc **procs, int nprocs)
{
  if (nprocs <= LAM_COLLMAXLIN) {
    return(nbarrier_lin(procs, nprocs));
  } else {
    return(nbarrier_log(procs, nprocs));
  }
}
    

/* linear algorithm */
static int
nbarrier_lin(struct _proc **procs, int nprocs)
{
  struct nmsg local, remote;
  int i;

  /* Setup nmsg's */
  setup_nmsgs(&local, &remote);

  /* All non-root send & receive zero-length message. */
  if (procs[0] != lam_myproc) {
    remote.nh_node = procs[0]->p_gps.gps_node;
    remote.nh_event = (-procs[0]->p_gps.gps_pid) & 0xBFFFFFFF;

    if (nsend(&remote) != 0) {
      return LAMERROR;
    }
    
    if (nrecv(&local) != 0) {
      return LAMERROR;
    }
  }

  /* The root collects and broadcasts the messages. */
  else {
    for (i = 1; i < nprocs; ++i) {
      if (nrecv(&local) != 0) {
        return LAMERROR;
      }
    }
    
    for (i = 1; i < nprocs; ++i) {
      remote.nh_node = (procs[i])->p_gps.gps_node;
      remote.nh_event = (-(procs[i])->p_gps.gps_pid) & 0xBFFFFFFF;

      if (nsend(&remote) != 0) {
        return LAMERROR;
      }
    }
  }
  
  return 0;
}


/* binomial tree algorithm */
static int
nbarrier_log(struct _proc **procs, int nprocs)
{
  int dim;
  int hibit;
  int mask;
  int rank = -1;
  int i;
  int nchildren = 0;
  struct nmsg local, remote;

  for (i = 0; i < nprocs; ++i) {
    if (procs[i] == lam_myproc) {
      rank = i;
      break;
    }
  }
  
  if (rank == -1) 
    return LAMERROR;

  dim = lam_cubedim(nprocs);
  hibit = lam_hibit(rank, dim);
  --dim;

  /* determine the number of children */
  for (i = dim, mask = 1 << i; i > hibit; --i, mask >>= 1)
    if ((rank | mask) < nprocs)
      ++nchildren;
  
  /* Setup nmsg's */
  setup_nmsgs(&local, &remote);

  /* Receive from all children */
  for (i = 0; i < nchildren; ++i) {
    if (nrecv(&local) != 0) {
      return LAMERROR;
    }
  }

  /* Send to and receive from parent */
  if (rank > 0) {
    remote.nh_node = procs[rank & ~(1 << hibit)]->p_gps.gps_node;
    remote.nh_event = (-procs[rank & ~(1 << hibit)]->p_gps.gps_pid) 
                      & 0xBFFFFFFF;
    if (nsend(&remote) != 0) {
      return LAMERROR;
    }
    
    if (nrecv(&local) != 0) {
      return LAMERROR;
    }
  }
    
  /* Send to all children */
  for (i = hibit + 1, mask = 1 << i; i <= dim; ++i, mask <<= 1) {
    if ((rank | mask) < nprocs) {
      remote.nh_node = procs[rank | mask]->p_gps.gps_node;
      remote.nh_event = (-procs[rank | mask]->p_gps.gps_pid) & 0xBFFFFFFF;
      if (nsend(&remote) != 0) {
        return LAMERROR;
      }
    }
  }

  return 0;  
}


static void 
setup_nmsgs(struct nmsg *local, struct nmsg *remote)
{
  LAM_ZERO_ME(*local);
  local->nh_type = 8;
  local->nh_flags = 0;
  local->nh_length = 0;
  local->nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  
  LAM_ZERO_ME(*remote);
  remote->nh_type = 8;
  remote->nh_flags = 0;
  remote->nh_length = 0;

  return;
}
