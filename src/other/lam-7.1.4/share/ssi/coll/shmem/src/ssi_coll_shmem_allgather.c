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
 * $Id: ssi_coll_shmem_allgather.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
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

/* Overall Algorithm --
   All ranks place their data in the shared memory, wait till all have
   coped the data and then copy all the shared memory area into their process
   buffer. Simple barrier will guarantee that the process wait for all
   ranks. */

/*
 *	allgather
 *
 *	Function:	- shared memory allgather
 *	Accepts:	- same as MPI_Allgather()
 *	Returns:	- MPI_SUCCESS or error code
 */

int
lam_ssi_coll_shmem_allgather(void *sbuf, int scount,
			     MPI_Datatype sdtype, void *rbuf,
			     int rcount, MPI_Datatype rdtype,
			     MPI_Comm comm)
{
  int rank, segment_in_use, i, size, total_count;
  long int max_rcounts_per_rank, rcounts_per_rank_to_copy, single_segment_size;
  MPI_Aint sextent, rextent;
  char *src, *dest, *src1, *dest1, *dest2;
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

  if (scount == 0) {
    segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
    lam_ssi_coll_shmem_comm_barrier(comm, segment_in_use, rank);
  } else if (size != 1) {
    src = (char*) sbuf;
    dest2 = (char*) rbuf;
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
      
      /* wait till all the processes finish copying into shared buffer */
      lam_ssi_coll_shmem_comm_barrier(comm, segment_in_use, rank);

      /* Now copy to the process buffer at proper offset */
      src1 = lcd->lcd_message_segment[segment_in_use];
      dest1 = dest2;
      for (i = 0; i < size; ++i) {
	if (lam_bottom_dtrcv(src1, rcounts_per_rank_to_copy, rdtype,
			     dest1, rcounts_per_rank_to_copy, rdtype,
			     BLKMPIGATHER, comm) != MPI_SUCCESS)
	  return LAMERROR;
	src1 += rcounts_per_rank_to_copy * rextent;
	dest1 += total_count * rextent;
      }
      
      rcount -= rcounts_per_rank_to_copy;

      if (rcount <= 0) {
	break;
      } else {
	src  += sextent * rcounts_per_rank_to_copy;
	dest2 += rcounts_per_rank_to_copy * rextent;
      }
    }
  } else {
    if (lam_dtsndrcv(sbuf, scount, sdtype, rbuf, rcount, rdtype, 
		     BLKMPIALLGATHER, comm) != MPI_SUCCESS)
      return LAMERROR;
  }

  return MPI_SUCCESS;
}






