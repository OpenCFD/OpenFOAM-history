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
 * $Id: lamcid.c,v 6.17 2003/08/22 20:55:16 jsquyres Exp $
 *
 *	Function:	- manage context identifiers
 *			- trivial version
 */

#include <lam_config.h>

#include <typical.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <string.h>
#include <errno.h>


/*
 * local variables
 */
static int map_size = -1;
static unsigned char *cid_map = NULL;
static unsigned char *empty_map = NULL;


/*
 * local functions
 */
static int find_unused(unsigned char *map);
static void rmcid(int cid);


/*
 * init_cid
 *
 * Initialize the CID map
 */
int
lam_init_cid(void)
{
  map_size = lam_mpi_max_cid / sizeof(unsigned char);
  cid_map = malloc(map_size);
  if (cid_map == NULL)
    return LAMERROR;
  empty_map = malloc(map_size);
  if (empty_map == NULL) {
    free(cid_map);
    return LAMERROR;
  }

  memset(cid_map, 0, map_size);
  memset(empty_map, 0, map_size);

  return 0;
}


/*
 * find_unused
 *
 * Scan a map and find an unused CID.
 */
static int
find_unused(unsigned char *map)
{
  int i, byte, bit, step;

  /* Scan down the combined bitmap to find an open CID */

#if LAM_WANT_IMPI
  step = 3;
#else
  step = 1;
#endif
  for (i = 0; i < map_size * 8; i += step) {
    byte = (i / (sizeof(char) * 8));
    bit = (1 << (i % (sizeof(char) * 8)));

    if ((map[byte] & bit) == 0)
      return i;
  }

  /* We didn't find out.  Doh! */

  return LAMERROR;
}


/*
 *	lam_getcid
 *
 *	Function:	- get next available context ID
 *	Returns:	- context ID or LAMERROR
 */
int
lam_getcid(void)
{
  return find_unused(cid_map);
}


/*
 * Agree on a new CID for a new communicator.  Some of LAM's RPI's
 * only support a small number of CIDs (%@#$@#$ lamd RPI...), so we
 * have to be as efficient with CIDs as possible.  Hence, we tradeoff
 * communication efficiency for CID allocation efficiency.
 * Additionally, with most commodity networks these days (e.g.,
 * 10/100Mbps), the knee bend in the latency curve typically occurs
 * around 1k.  So with 4096 CIDs, that's only 512 bytes, so it's just
 * as cheap to send 512 bytes as it is to send 1 byte.  Hence, doing
 * an Allreduce of 512 bytes is about as expensive as reduce/bcast of
 * a single int.
 *
 * Unlike for intracommunicators, callers will always contribute when
 * they invoke this function.  This will change when we implement the
 * intercommunicator version of MPI_COMM_SPLIT, but that hasn't
 * happened yet.
 * 
 * This function will be called by intercommunicators and during intercomm
 * create intercomm merge and intercomm dup. 
 *
 * For intercomm create -- we use old algorith. Reduce then BIT_OR then
 * bcast. 
 *
 * For intercomm merge and intercomm dup we, use following scheme though.
 * The scheme that we use is as follows
 * - Each rank sends its map to remote leader
 * - Local leaders do a reduce on the maps recieved from remote rank.s This 
 *   they have reduced result of the remote maps
 * - Both the local leaders then exchange each others map and do a Bitwise
 *   OR
 * - Now the leaders has the FINAL RESULT which they simply broadcast to the
 *   remote processes.
 *  
 * WHOLE SCHEME IS BASED ON SEND AND RECV SO THAT THIS PROCEDURE IS NOT AT
 * ALL DEPENDENT ON ANY OF THE COLLECIVE ALGORITHMS
 */
int 
lam_coll_alloc_inter_cid(MPI_Comm lcomm, int lleader,
			 MPI_Comm pcomm, int pleader,
			 int ptag, int blk, int *new_cid)
{
  int i, j, err;
  MPI_Request req;
  unsigned char *recv_map;
  unsigned char *remote_map;
  int rsize, lower;

  /* Allocate some temporary space */
  recv_map = malloc(map_size * 2);
  if (recv_map == NULL)
    return lam_errfunc(lcomm, blk, lam_mkerr(MPI_ERR_NO_MEM, ENOMEM));
  remote_map = recv_map + map_size;

  if (blk != BLKMPIICOMMCREATE) {
    /* ALGORITHM FOR INTERCOMM MERGE AND INTERCOMM DUP. THIS ALGORITHM WILL
       BE USED WHENEVER WE HAVE INTERCOMM READY. lcomm and pcomm, both should
       be valid and equal intercommunicators */

    lower = (lam_proccmp(lcomm->c_group->g_procs[0],
			 lcomm->c_rgroup->g_procs[0]) < 0) ? 1 : 0;
    
    if (lcomm->c_group->g_myrank == lleader) {
      
      if (lower) {
	
	/* If I am the leader then recieve maps from all the remote processes
	   and simultaneously reduce the maps */
	err = MPI_Send(cid_map, map_size, MPI_BYTE, pleader, blk, lcomm);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
	err = MPI_Recv(remote_map, map_size, MPI_BYTE, pleader, blk, lcomm,
		       MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
      } else {
	err = MPI_Recv(remote_map, map_size, MPI_BYTE, pleader, blk, lcomm,
		       MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
	err = MPI_Send(cid_map, map_size, MPI_BYTE, pleader, blk, lcomm);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
      }
      
      /* Loop recieving maps for non-leaders */
      MPI_Comm_remote_size(lcomm, &rsize);
      for (i = 0; i < rsize; ++i) {
	if (i != pleader)
	  err = MPI_Recv(recv_map, map_size, MPI_BYTE, i, blk, lcomm,
			 MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
	for (j = 0; j < map_size; ++j)
	  remote_map[j] = remote_map[j] | recv_map[j];
      }
      
      
      /* Leaders again do the send_recvs of this remote map so that they
	 can find out the resulting map */
      
      if (lower) {
      /* If I am the leader then recieve maps from all the remote processes
	 and simultaneously reduce the maps */
	err = MPI_Send(remote_map, map_size, MPI_BYTE, pleader, blk, lcomm);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
	err = MPI_Recv(recv_map, map_size, MPI_BYTE, pleader, blk, lcomm,
		       MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
      } else {
	err = MPI_Recv(recv_map, map_size, MPI_BYTE, pleader, blk, lcomm,
		       MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
	err = MPI_Send(remote_map, map_size, MPI_BYTE, pleader, blk, lcomm);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
      }
      
      /* I have remote_map and recv_map which is actually reduction of maps
	 in my local group */
      for (j = 0; j < map_size; ++j)
	recv_map[j] = remote_map[j] | recv_map[j];
      
      /* This recv_map is the FINAL RESULT of all reductions. Broadcast this
	 to all the remote processes */
      for (i = 0; i < rsize; ++i) {
	if (i != pleader) 
	  err = MPI_Send(recv_map, map_size, MPI_BYTE, i, blk, lcomm);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
      }
      
    } else {
      
      /* Non-leaders just send the maps to remote leader */
      err = MPI_Send(cid_map, map_size, MPI_BYTE, pleader, blk, lcomm);
      if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
      }
      
      /* Recieve the resulting map */
      err = MPI_Recv(recv_map, map_size, MPI_BYTE, pleader, blk, lcomm,
		     MPI_STATUS_IGNORE);
      if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
      }
    }
  } else {
    /* Algorithm for INTERCOMM CREATE */
      err = MPI_Reduce(cid_map, recv_map, map_size, MPI_BYTE, MPI_BOR, 
		       lleader, lcomm);
      if (err != MPI_SUCCESS) {
	free(recv_map);
	return err;
      }

      /* If I'm the local leader, sendrecv with the remote leader */
      
      if (lcomm->c_group->g_myrank == lleader) {
	err = lam_irecv(remote_map, map_size, MPI_BYTE, pleader, ptag, 
			pcomm, &req);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}
	
	err = lam_send(recv_map, map_size, MPI_BYTE, pleader, ptag,
		       pcomm, LAM_RQISEND);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}

	err = MPI_Wait(&req, MPI_STATUS_IGNORE);
	if (err != MPI_SUCCESS) {
	  free(recv_map);
	  return err;
	}

	/* Merge the two maps */
	
	for (i = 0; i < map_size; ++i)
	  recv_map[i] |= remote_map[i];
      }

      /* Now do a local broadcast of the map */
      
      err = MPI_Bcast(recv_map, map_size, MPI_BYTE, lleader, lcomm);
      if (err != MPI_SUCCESS) {
	free(recv_map);
	return err;
      }
  }
   
  /* Scan down the combined bitmap to find an open CID  and DONE */
    
  *new_cid = find_unused(recv_map);
  free(recv_map);
  if (*new_cid < 0) 
    return lam_errfunc(lcomm, blk, lam_mkerr(MPI_ERR_INTERN, EFULL));

  return MPI_SUCCESS;
}


/*
 * Agree on a new CID for a new communicator.  Same rationale as for
 * intercommunicators, above.
 *
 * Callers may or may not contribute.  For example, the caller may
 * have specified MPI_UNDEFINED for the color in MPI_COMM_SPLIT, and
 * although they still have to take part in the collective, they
 * should not contribute to the new CID value.
 */
int 
lam_coll_alloc_intra_cid(MPI_Comm comm, int contribute, int blk, int *new_cid)
{
  int err;
  unsigned char *buf;
  unsigned char *recv_map;

  if (contribute == 1)
    buf = cid_map;
  else
    buf = empty_map;

  /* Allocate space */

  recv_map = malloc(map_size);
  if (recv_map == NULL)
    return lam_errfunc(comm, blk, lam_mkerr(MPI_ERR_NO_MEM, ENOMEM));

  /* Do an allreduce over a bitwise-OR to find CIDs that everyone has
     available */

  err = MPI_Allreduce(buf, recv_map, map_size, MPI_BYTE, MPI_BOR, comm);
  if (err != MPI_SUCCESS) {
    free(recv_map);
    return err;
  }

  /* Scan down the combined bitmap to find an open CID */

  if (contribute == 1)
    *new_cid = find_unused(recv_map);
  else
    *new_cid = -1;
  free(recv_map);
  if (contribute == 1 && *new_cid < 0)
    return lam_errfunc(comm, blk, lam_mkerr(MPI_ERR_INTERN, EFULL));

  return MPI_SUCCESS;
}


/*
 *	lam_setcid
 *
 *	Function:	- set highest used context ID
 *      Accepts:        - context ID 
 */
void
lam_setcid(int cid)
{
  int byte, bit;
  
  byte = cid / 8;
  bit = (1 << (cid % 8));
  cid_map[byte] |= bit;

#if LAM_WANT_IMPI
  /* Shadow/IMPI collective communicator */
  cid++;
  byte = cid / 8;
  bit = (1 << (cid % 8));
  cid_map[byte] |= bit;

  /* Datasync (positive)/Syancack (negative) communicator */
  cid++;
  byte = cid / 8;
  bit = (1 << (cid % 8));
  cid_map[byte] |= bit;

#endif
}


/*
 *	lam_rmcid
 *
 *	Function:	- deallocate (remove) a context ID
 *	Accepts:	- context ID
 */
void
lam_rmcid(int cid)
{
  rmcid(cid);
#if LAM_WANT_IMPI
  /* Free the next 3 communicators as well */
  rmcid(cid + 1);
  rmcid(cid + 2);
#endif
}


/*
 *	rmcid
 *
 *	Function:	- deallocate (remove) a context ID
 *	Accepts:	- context ID
 */
static void
rmcid(int cid)
{
  int byte, bit;
  
  byte = cid / 8;
  bit = (1 << (cid % 8));

  cid_map[byte] &= ~bit;
}


/*
 *	lam_nukecids
 *
 *	Function:	- deallocate all context IDs (cleanup)
 */
void
lam_nukecids(void)
{
  if (cid_map != NULL)
    free(cid_map);
  if (empty_map != NULL)
    free(empty_map);

  cid_map = NULL;
  empty_map = NULL;
}
