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
 * $Id: ssi_coll_shmem_gather.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
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

/* Overall Algoritm --
 */

/*
 *	gather
 *
 *	Function:	- shared memory gather
 *	Accepts:	- same as MPI_Gather()
 *	Returns:	- MPI_SUCCESS or error code
 */

int lam_ssi_coll_shmem_gather(void *sbuf, int scount,
			      MPI_Datatype sdtype, void *rbuf,
			      int rcount, MPI_Datatype rdtype,
			      int root, MPI_Comm comm)
{
  int rank, segment_in_use, i, size, total_count;
  long int max_rcounts_per_rank, rcounts_per_rank_to_copy, single_segment_size;
  MPI_Aint sextent, rextent;
  char *src, *dest, *start;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  MPI_Type_extent(sdtype, &sextent);
  MPI_Type_extent(rdtype, &rextent);
  single_segment_size = lcd->lcd_message_pool_size / lcd->lcd_num_segments;

  /* First determine, how many bytes (each rank) can reside in the shared
     memory segment */
  max_rcounts_per_rank = single_segment_size / (size * rextent);
  total_count = scount;
  
  if (rcount == 0) {
    segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
    lam_ssi_coll_shmem_signal_by_non_roots(comm, segment_in_use, root,
					   rank);
    if (segment_in_use == lcd->lcd_num_segments - 1) 
      lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
  } else {
    if (rank == root) {
      if (size != 1) {
	start = (char*) rbuf;
	
	while (rcount > 0) {
	  segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	  rcounts_per_rank_to_copy = (rcount < max_rcounts_per_rank)
	    ? rcount : max_rcounts_per_rank;
	  
	  src = lcd->lcd_message_segment[segment_in_use];
	  dest = start;
	  
	  /* wait till non-root processes copy the data */
	  lam_ssi_coll_shmem_signal_by_non_roots(comm, segment_in_use, root,
						 rank);
	  
	  if (rcounts_per_rank_to_copy == total_count) {
	    if (lam_bottom_dtrcv(src, rcounts_per_rank_to_copy * size, rdtype,
				 dest, rcounts_per_rank_to_copy * size, rdtype,
				 BLKMPIGATHER, comm) != MPI_SUCCESS)
	      return LAMERROR;
	  } else {
	    for (i = 0; i < size; ++i) {
	      if (i != root) {
		if (lam_bottom_dtrcv(src, rcounts_per_rank_to_copy, rdtype,
				     dest, rcounts_per_rank_to_copy, rdtype,
				     BLKMPIGATHER, comm) != MPI_SUCCESS)
		  return LAMERROR;
	      }
	      src +=  rcounts_per_rank_to_copy * rextent;
	      dest += total_count * rextent;
	    }
	  }
	  
	  if (segment_in_use == lcd->lcd_num_segments - 1) 
	    lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	  
	  rcount -= rcounts_per_rank_to_copy;
	  if (rcount <= 0) {
	    break;
	  } else {
	    start += rcounts_per_rank_to_copy * rextent;
	  }
	}
      }
      
      /* copy my part */
      if (lam_dtsndrcv(sbuf, total_count, sdtype, 
		       (char*) rbuf + (rextent * rank * total_count),
		       total_count, rdtype, BLKMPIGATHER, comm) != MPI_SUCCESS)
	return LAMERROR;
      
    } else {
      
      src = (char*) sbuf;
      while (rcount > 0) {
	segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	
	rcounts_per_rank_to_copy = (rcount < max_rcounts_per_rank)
	  ? rcount : max_rcounts_per_rank;

	dest = lcd->lcd_message_segment[segment_in_use] +
	  (rcounts_per_rank_to_copy * rextent * rank);
	
	if (lam_bottom_dtsnd(src, rcounts_per_rank_to_copy, sdtype,
			     dest, rcounts_per_rank_to_copy, rdtype,
			     BLKMPIGATHER, comm) != MPI_SUCCESS)
	  return LAMERROR;
	
	/* tell root that they have finished copying the data */
	lam_ssi_coll_shmem_signal_by_non_roots(comm, segment_in_use, root, 
					       rank);
	
	if (segment_in_use == lcd->lcd_num_segments - 1)
	  lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	
	rcount -= rcounts_per_rank_to_copy;
	
	if (rcount <= 0) {
	  break;
	} else {
	  src  += sextent * rcounts_per_rank_to_copy;
	}
      }
    }
  }
  return MPI_SUCCESS;
}






