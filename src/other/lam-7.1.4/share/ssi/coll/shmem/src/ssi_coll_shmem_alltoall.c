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
 * $Id: ssi_coll_shmem_alltoall.c,v 1.2 2004/03/09 16:04:52 adharurk Exp $
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
   Each process starts with copying data into shared memory and calculates
   the rank from which it is supposed to recieve the data in the present loop
   After copying data into shared memory, all call barrier and then copy the
   appropriate data from shared memory to process buffer. In each loop it is
   ensured that all the processes take part in copying to achieve maximum
   parallelism. This was possible because of inherent parallel nature of the 
   alltoall operation.
*/

/*
 *      alltoall
 *
 *      Function:       - MPI_Alltoall using shared memory
 *      Accepts:        - same as MPI_Alltoall()
 *      Returns:        - MPI_SUCCESS or an MPI error code
 */
int
lam_ssi_coll_shmem_alltoall(void *sbuf, int scount,
			    MPI_Datatype sdtype, void *rbuf,
			    int rcount, MPI_Datatype rdtype,
			    MPI_Comm comm)
{
  int rank, segment_in_use, j, size, total_count, recieve_from_rank, count;
  long int max_rcounts_per_rank, rcounts_per_rank_to_copy, single_segment_size;
  MPI_Aint sextent, rextent;
  char *src, *dest, *src1, *dest1;
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
  count = 1;

  if (scount == 0) {
    segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
    lam_ssi_coll_shmem_comm_barrier(comm, segment_in_use, rank);
  } else {
    if (lam_dtsndrcv(((char*)sbuf) + (rank * sextent * scount), scount, sdtype,
		     ((char*)rbuf) + (rank * rextent * rcount), rcount, 
		     rdtype, BLKMPIALLTOALL, comm) != MPI_SUCCESS)
      return LAMERROR;
    for (j = (rank + 1) % size; j != rank; j = (j + 1) % size, ++count) {
      
      recieve_from_rank = ((rank - count) < 0) 
	? (rank - count + size) : (rank - count);
      
      /* copy the data into shared buffer and then do a barrier. After a 
	 barrier copy the data to proper place. For each iteration do not 
	 return till all scount is sent completely. Initialize src location in 
	 sbuf and dest1 location in rbuf */
      src = (char*) sbuf + (sextent * j * total_count);
      dest1 = (char*) rbuf + (rextent * recieve_from_rank * total_count);
      rcount = total_count;
      
      while (rcount > 0) {
	segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	
	rcounts_per_rank_to_copy = (rcount < max_rcounts_per_rank)
	  ? rcount : max_rcounts_per_rank;
	
	dest = lcd->lcd_message_segment[segment_in_use] +
	  (rcounts_per_rank_to_copy * rextent * rank);
	
	if (lam_bottom_dtsnd(src, rcounts_per_rank_to_copy, sdtype,
			     dest, rcounts_per_rank_to_copy, rdtype,
			     BLKMPIALLTOALL, comm) != MPI_SUCCESS)
	  return LAMERROR;
	
	/* wait till all the processes finish copying into shared buffer */
	lam_ssi_coll_shmem_comm_barrier(comm, segment_in_use, rank);
	
	/* Now copy to the process buffer at proper offset */
	src1 = lcd->lcd_message_segment[segment_in_use] + 
	  (rcounts_per_rank_to_copy * rextent * recieve_from_rank);
	if (lam_bottom_dtrcv(src1, rcounts_per_rank_to_copy, rdtype,
			     dest1, rcounts_per_rank_to_copy, rdtype,
			     BLKMPIALLTOALL, comm) != MPI_SUCCESS)
	  return LAMERROR;
	
	rcount -= rcounts_per_rank_to_copy;
	
	if (rcount <= 0) {
	  break;
	} else {
	  dest1 += rcounts_per_rank_to_copy * rextent;
	  src += rcounts_per_rank_to_copy * sextent;
	}
      }
    }
  }
  return MPI_SUCCESS;
}
