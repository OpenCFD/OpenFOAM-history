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
 * $Id: ssi_coll_shmem_scatter.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
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
 *	scatter
 *
 *	Function:	- shared memory scatter
 *	Accepts:	- same as MPI_Scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int lam_ssi_coll_shmem_scatter(void *sbuf, int scount,
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
    lam_ssi_coll_shmem_signal_by_root(comm, segment_in_use, root,
				      rank);
    if (segment_in_use == lcd->lcd_num_segments - 1) 
      lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
  } else {
  
    if (rank == root) {
      
      /* copy my part */
      if (lam_dtsndrcv(((char*)sbuf) + (rank * scount * sextent), scount, 
		       sdtype, rbuf, rcount, rdtype, BLKMPISCATTER, comm) 
	  != MPI_SUCCESS)
	return LAMERROR;
      
      if (size != 1) {
	start = (char*) sbuf;
	while (rcount > 0) {
	  segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	  rcounts_per_rank_to_copy = (rcount < max_rcounts_per_rank)
	    ? rcount : max_rcounts_per_rank;
	  
	  src = start;
	  dest = lcd->lcd_message_segment[segment_in_use];
	  
	  if (rcounts_per_rank_to_copy == total_count) {
	    if (lam_bottom_dtsnd(src, rcounts_per_rank_to_copy * size, sdtype,
				 dest, rcounts_per_rank_to_copy * size, rdtype,
				 BLKMPISCATTER, comm) != MPI_SUCCESS)
	      return LAMERROR;
	  } else {
	    for (i = 0; i < size; ++i) {
	      if (i != root) {
		if (lam_bottom_dtsnd(src, rcounts_per_rank_to_copy, sdtype,
				     dest, rcounts_per_rank_to_copy, rdtype,
				     BLKMPISCATTER, comm) != MPI_SUCCESS)
		  return LAMERROR;
	      }
	      src +=  total_count * sextent;
	      dest += rcounts_per_rank_to_copy * rextent;
	    }
	  }
	  
	  /* tell non-root processes so that they can copy the data */
	  lam_ssi_coll_shmem_signal_by_root(comm, segment_in_use, root, rank);
	  
	  if (segment_in_use == lcd->lcd_num_segments - 1)
	    lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	  
	  rcount -= rcounts_per_rank_to_copy;
	  if (rcount <= 0) {
	    break;
	  } else {
	    start += rcounts_per_rank_to_copy * sextent;
	  }
	}
      }
    } else {
      
      dest = (char*) rbuf;
      while (rcount > 0) {
	segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	
	rcounts_per_rank_to_copy = (rcount < max_rcounts_per_rank)
	  ? rcount : max_rcounts_per_rank;
	
	/* wait till root finishes copying */
	lam_ssi_coll_shmem_signal_by_root(comm, segment_in_use, root, rank);
	
	src = lcd->lcd_message_segment[segment_in_use] +
	  (rcounts_per_rank_to_copy * rextent * rank);
	
	if (lam_bottom_dtrcv(src, rcounts_per_rank_to_copy, rdtype,
			     dest, rcounts_per_rank_to_copy, rdtype,
			     BLKMPISCATTER, comm) != MPI_SUCCESS)
	  return LAMERROR;
	
	if (segment_in_use == lcd->lcd_num_segments - 1)
	  lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	
	rcount -= rcounts_per_rank_to_copy;
	
	if (rcount <= 0) {
	  break;
	} else {
	  dest += rextent * rcounts_per_rank_to_copy;
	}
      }
    }
  }
  return MPI_SUCCESS;
}
