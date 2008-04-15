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
 * $Id: lamapps.c,v 6.3 2003/04/10 19:59:56 jsquyres Exp $
 *
 *	Function:	- helper functions to the spawn routines
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <etc_misc.h>
#include <mpisys.h>
#include <rpisys.h>
#include <net.h>
#include <mpi_apps.h>
#include <lam-ssi-rpi.h>


/*
 * Local functions
 */

static int qsort_proc_cmp(const void *a, const void *b);


/*
 *	lam_exchange_endian_info
 *
 *	Function:	- exchange endian information
 *	Accepts:	- size of world
 *			- initial process world
 *			- MPI process world (out)
 *			- string prefix to print in error message
 *	Returns:	- 0 or LAMERROR
 */
int
lam_exchange_endian_info(struct _proc **procs, int numprocs)
{
  int i, j;
  int *ip;
  struct _proc **myprocs;
  struct _proc *proc;
  struct nmsg inmsg, outmsg;

  /* Go through all the procs of the newly-spawned processes, and
     determine if each proc is the same endian as me.  Not very
     efficient (should use some kind of tree thing, probably), but
     it's ok [for now] because we're in MPI_COMM_SPAWN.  (Re)Set
     lam_homog to be 1 if everyone is the same endian, 0 otherwise.  */

  LAM_ZERO_ME(inmsg);
  LAM_ZERO_ME(outmsg);

  lam_homog = 1;
  outmsg.nh_flags = 0;
  outmsg.nh_length = sizeof(int);
  outmsg.nh_data[0] = lam_homog;
  ip = malloc(sizeof(int));
  if (ip == NULL)
    return LAMERROR;
  ip[0] = 1;
  outmsg.nh_msg = (char*) ip;
  
  inmsg.nh_flags = 0;
  inmsg.nh_length = sizeof(int);
  ip = malloc(sizeof(int));
  if (ip == NULL) {
    free(outmsg.nh_msg);
    return LAMERROR;
  }
  inmsg.nh_msg = (char*) ip;

  /* Sort the list of procs so that every uses the same order */

  myprocs = malloc(sizeof(struct _proc *) * numprocs);
  if (myprocs == NULL) {
    free(outmsg.nh_msg);
    free(inmsg.nh_msg);
    return LAMERROR;
  }
  memcpy(myprocs, procs, sizeof(struct _proc *) * numprocs);
  qsort(myprocs, numprocs, sizeof(struct _proc *), qsort_proc_cmp);

  /* Do the message passing */
  
  for (i = 0; i < numprocs; ++i) {
    proc = myprocs[i];

    /* If this proc is me, skip it */

    if (proc == lam_myproc)
      continue;

    /* If the proc is on my node, then I know it's the same endian as
       me, and I don't need to send/receive any messages */

    else if (proc->p_gps.gps_node == lam_myproc->p_gps.gps_node) {
      proc->p_mode |= LAM_PHOMOG;
      continue;
    }

    /* Otherwise, it's a proc on a different node -- send/receive the
       messages and figure out if it's the same endian as me.  We may
       actually end up sending to multiple procs on the same node
       (i.e., redudant messages, since endian is by node, not by
       proc), but for the moment, that's fine.  :-)  */

    else {

      /* Send the outgoing message */
      
      _m2l_fillsys(lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
		   proc->p_gps.gps_node, proc->p_gps.gps_idx, 
		   0, ((lam_myproc->p_gps.gps_node & 0xFFFF00) >> 8), &outmsg);
      if (nsend(&outmsg))
	return LAMERROR;
      
      /* Receive the incoming message */
      
      _m2l_fillsys(proc->p_gps.gps_node, proc->p_gps.gps_idx, 
		   lam_myproc->p_gps.gps_node, lam_myproc->p_gps.gps_idx,
		   0, ((proc->p_gps.gps_node & 0xFFFF00) >> 8), &inmsg);
      if (nrecv(&inmsg))
	return LAMERROR;
      
      /* Did we get an (int) 1 from the other side?  If so, our
	 endians match */

      if (ip[0] == 1)
	proc->p_mode |= LAM_PHOMOG;
      else
	lam_homog = 0;
    }
  }

  /* Now we need to go reconcile those LAM_PHOMOG flags.  This is an
     n^2 algorithm (yeah, I know...), but it's no big deal in this
     case (we're still in MPI_COM_SPAWN*, after all). */

  for (i = 0; i < numprocs; ++i) {
    proc = myprocs[i];
    for (j = 0; j < numprocs; ++j) {
      if (lam_proccmp(proc, procs[j]) == 0) {
	procs[j]->p_mode = proc->p_mode;
	j = numprocs;
      }
    }    
  }

  free(myprocs);
  free(outmsg.nh_msg);
  free(inmsg.nh_msg);

  return 0;
}


/*
 *	qsort_proc_cmp
 *
 *	Function:	- be the qsort compare function for struct _procs
 *	Accepts:	- two struct _proc pointers
 * Returns: - 0 if 0, -1 if a < b, or 1 if a > b */
static int
qsort_proc_cmp(const void *a, const void *b)
{
  return lam_proccmp(*((struct _proc **) a), *((struct _proc **) b));
}

