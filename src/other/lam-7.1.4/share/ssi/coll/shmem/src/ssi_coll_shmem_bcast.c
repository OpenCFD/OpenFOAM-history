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
 * $Id: ssi_coll_shmem_bcast.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
 *
 *	Function:	- shmem collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-shmem-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-shmem.h>
#include <mpi.h>
#include <mpisys.h>
#include <blktype.h>


/*
 *	bcast
 *
 *	Function:	- shared memory bcast
 *	Accepts:	- same as MPI_Bcast()
 *	Returns:	- MPI_SUCCESS or error code
 */

/* Overall Algorithm -----
   Root copies message stripe into shared memory and then signals non-root
   processes. Non-root processes wait till they get signal from the root and 
   then copy the appropriate portion of the shared memory to their process
   buffer. A barrier is used only when the processes reach to the last 
   segment. This ensures that a process can start using [0 .. NUM_SEGMENTS-1]
   segments at a stretch.
*/

int
lam_ssi_coll_shmem_bcast(void *buff, int count, MPI_Datatype datatype,
                         int root, MPI_Comm comm)
{
  int rank, segment_in_use, size;
  long int counts_to_copy, max_count, single_segment_size;
  MPI_Aint extent;
  char *src, *dest;

  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  MPI_Type_extent(datatype, &extent);
  single_segment_size = lcd->lcd_message_pool_size / lcd->lcd_num_segments;
  max_count = single_segment_size / extent;

  if (count == 0) {
    segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
    lam_ssi_coll_shmem_signal_by_root(comm, segment_in_use, root, rank);
    if (segment_in_use == lcd->lcd_num_segments - 1) 
      lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
  } else {

    /* Switch between the two segments to broadcast the messages */
    if (rank == root) {
      
      src = (char*) buff;
      while (count > 0) {
	segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	
	counts_to_copy = (count < max_count) ? count : max_count;
	dest = lcd->lcd_message_segment[segment_in_use];
	
	if (lam_bottom_dtsnd(src, counts_to_copy, datatype, dest, 
			     counts_to_copy, datatype, BLKMPIBCAST, comm) 
	    != MPI_SUCCESS)
	  return LAMERROR;
	
	count -= counts_to_copy;
	
	/* Tell other processes that I have copied the data */
	lam_ssi_coll_shmem_signal_by_root(comm, segment_in_use, root, rank);
	
	if (segment_in_use == lcd->lcd_num_segments - 1) 
	  lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	
	/* If entire message fits into the buffer then exit */
	if (count <= 0)
	  break;
	else
	  src += counts_to_copy * extent;
	
      }
    } else {
      
      dest = (char*) buff;
      /* non root processses */
      while (count > 0) {
	segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	
	counts_to_copy = (count < max_count) ? count : max_count;
	src = lcd->lcd_message_segment[segment_in_use];
	
	/* wait till you get message from the root */
	lam_ssi_coll_shmem_signal_by_root(comm, segment_in_use, root, rank);
	
	if (lam_bottom_dtrcv(src, counts_to_copy, datatype, dest,
			     counts_to_copy, datatype, BLKMPIBCAST, comm) 
	    != MPI_SUCCESS)
	  return LAMERROR;
	
	if (segment_in_use == lcd->lcd_num_segments - 1)
	  lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	
	count -= counts_to_copy;
	if (count <= 0) {
	  break;
	} else {
	  dest += counts_to_copy * extent;
	}
	
      }
    }
  }
  return MPI_SUCCESS;
}
