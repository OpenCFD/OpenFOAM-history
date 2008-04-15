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
 * $Id: ssi_rpi_crtcp_bookmarks_alltoall.c,v 1.9 2003/04/02 02:42:02 ssankara Exp $
 *
 */


#include <string.h>
#include <unistd.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <lam-ssi-rpi-crtcp.h>
#include <rpi_crtcp_proc.h>
#include <rpi_crtcp_bookmarks.h>


/*
 * private variables
 */
static int myindex = -1;


/*
 * global functions
 */
int PUB(bookmark_exchange_alltoall)(bookmark_payload_t *my_bookmarks);


/*
 * private functions
 */
static int send_bookmarks_alltoall(struct _proc *p);
static int receive_bookmarks_alltoall(bookmark_payload_t *bookmarks);


/*
 * lam_ssi_rpi_crtcp_bookmark_exchange_alltoall
 * 
 * Function:	    - use an all-to-all algorithm to send/receive bookmarks.
 * Returns:	    - 0 or LAMERROR
 */
int 
PUB(bookmark_exchange_alltoall)(bookmark_payload_t *my_bookmarks)
{
  int i, j;

  /* 
   * send out the out_nbytes count to each process.  receive the count from all
   * the processes and for each process, advance the incoming requests till the
   * in_nbytes count matches the one sent by that process.  
   */
  for(i = 0; i < PUB(nprocs); ++i)
    if (PUB(procs)[i] == lam_myproc) {
      myindex = i;
      break;
    }

  if (-1 == myindex)
    return LAMERROR;

  
  for (i = PUB(nprocs) - myindex - 1, j = 0; j < PUB(nprocs); 
       ++j, i = (i + 1) % PUB(nprocs)) {
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did), "bookmark exchange iter %d", i);
    if (i != myindex) {
      if (myindex > i) {
        if (send_bookmarks_alltoall(PUB(procs)[i]) != 0 || 
		receive_bookmarks_alltoall(my_bookmarks) != 0)
          return LAMERROR;
      } else {
        if (receive_bookmarks_alltoall(my_bookmarks) != 0 || 
		send_bookmarks_alltoall(PUB(procs)[i]) != 0)
          return LAMERROR;
      }
    }
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did), "bookmark exchange iter %d done", i);

    /* mark the socket as 'not connected' */
    PUB(procs)[i]->p_mode &= ~LAM_PRPICONNECT;
  }
  return 0;
}

/*
 * send_bookmarks_alltoall
 *
 * Function:	    - send a bookmark to a specific process
 *
 * Returns:	    - 0 or LAMERROR
 */
static int
send_bookmarks_alltoall(struct _proc *p)
{
  struct nmsg nhead;
  bookmark_payload_t *bp;

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did),
	      "Quiesce: index %d sending bookmark %d to rank %d",
              myindex, 
	      p->p_rpi->cp_out_nbytes,
	      p->p_gps.gps_grank);

  nhead.nh_node = p->p_gps.gps_node;
  nhead.nh_event = (-p->p_gps.gps_pid) & 0xBFFFFFFF;
  nhead.nh_type = 32;
  nhead.nh_flags = DINT4DATA;
  nhead.nh_length = 0;

  bp = (bookmark_payload_t *) nhead.nh_data;
  bp->bp_proc_index = myindex;
  bp->bp_out_nbytes = p->p_rpi->cp_out_nbytes;

  if (nsend(&nhead)) {
    if (PUB(verbose) >= 20)
      lam_debug(PUB(did), "Quiesce: nsend failed!!");
    return(LAMERROR);
  }

  return(0);
}

/*
 * receive_bookmarks_alltoall
 *
 * Function:	    - receive a bookmark from anyone, and use the sender's 
 *		      grank that's in the message to save the bookmark in 
 *		      the Right place in the array.
 * 
 * Returns:	    - 0 or LAMERROR
 */
static int
receive_bookmarks_alltoall(bookmark_payload_t *bookmarks)
{
  struct nmsg nhead;
  bookmark_payload_t *bp;

  nhead.nh_event = (-lam_getpid()) & 0xBFFFFFFF;
  nhead.nh_type = 32;
  nhead.nh_flags = DINT4DATA;
  nhead.nh_length = 0;
  bp = (bookmark_payload_t *) nhead.nh_data;

  if (nrecv(&nhead)) {
    if (PUB(verbose) >= 20)
      lam_debug(PUB(did), "Quiesce: nrecv failed!");
    return(LAMERROR);
  }

  /* Find who we just received that bookmark from, and save it in the
     array */

  if (PUB(verbose) >= 20)
    lam_debug(PUB(did), "just got a bookmark from index %d: %d",
	      bp->bp_proc_index, bp->bp_out_nbytes);
  
  bookmarks[bp->bp_proc_index] = *bp;

  return 0;
}

