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
 * $Id: ssi_coll_shmem_reduce.c,v 1.3 2004/03/09 16:04:52 adharurk Exp $
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
 *	reduce
 *
 *	Function:	- shmem reduce algorithm
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */

#if 0
int 
lam_ssi_coll_shmem_reduce(void *sbuf, void *rbuf, int count,
			  MPI_Datatype dtype, MPI_Op op,
			  int root, MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  /* If this communicator was marked to be associative, use the wide
     area optimal associative algorithm. */

  if (lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_ASSOCIATIVE) == 1 &&
      op->op_commute)
    return lam_ssi_coll_shmem_assc_reduce(sbuf, rbuf, count, dtype, op, 
					  root, comm);
  else {
    if ((count * dtype->dt_size) < 
	lam_ssi_coll_base_get_param(comm, 
				    LAM_MPI_SSI_COLL_REDUCE_CROSSOVER))
      return lam_ssi_coll_lam_basic_reduce_lin(sbuf, rbuf, count, dtype, 
					       op, root, comm);
    else
      return lam_ssi_coll_lam_basic_reduce_log(sbuf, rbuf, count, dtype, 
					       op, root, comm);
  }
} 

#endif

/* Overall Algorithm -----
   Non-root processes copy their part into the shared memory and root
   waits till it gets signal from all the non-root processes. Root then 
   reduces the data using op and copies the result to rbuf. 
*/

/*
 *	reduce
 *
 *	Function:	- shmem reduce algorithm
 *	Accepts:	- same as MPI_Reduce()
 *	Returns:	- MPI_SUCCESS or error code
 */

int 
lam_ssi_coll_shmem_reduce(void *sbuf, void* rbuf, int count,
			  MPI_Datatype dtype, MPI_Op op,
			  int root, MPI_Comm comm) 
{
  int rank, segment_in_use, i, size;
  long int max_count_per_rank, copy_count_per_rank, single_segment_size;
  MPI_Aint extent;
  char *src, *dest, *start;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  MPI_Type_extent(dtype, &extent);
  single_segment_size = lcd->lcd_message_pool_size / lcd->lcd_num_segments;
  max_count_per_rank = single_segment_size / (size * extent);
  copy_count_per_rank = (count < max_count_per_rank)
    ? count : max_count_per_rank;

  if (count == 0) {
    segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
    lam_ssi_coll_shmem_signal_by_non_roots(comm, segment_in_use, root, rank);
    if (segment_in_use == lcd->lcd_num_segments - 1) 
      lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
  } else {
    if (rank == root) {
      if (size == 1) {
	if (lam_dtsndrcv(sbuf, count, dtype, rbuf, count, dtype, BLKMPIREDUCE,
			 comm) != MPI_SUCCESS)
	  return LAMERROR;
      } else {
	start = (char*) sbuf;
	dest = (char*) rbuf;
	while (count > 0) {
	  
	  segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	  
	  copy_count_per_rank = (count < max_count_per_rank)
	    ? count : max_count_per_rank;
	  
	  /* wait till all non-root processes have copied their data */
	  lam_ssi_coll_shmem_signal_by_non_roots(comm, segment_in_use, root, 
						 rank);
	  
	  /* Initialize the rbuf */
	  if (root == size -1) {
	    src = start;
	  } else {
	    src = lcd->lcd_message_segment[segment_in_use] +
	      ((size - 1) * extent * copy_count_per_rank);
	  }
	  
	  if (lam_bottom_dtrcv(src, copy_count_per_rank, dtype,
			       dest, copy_count_per_rank, dtype, BLKMPIREDUCE,
			       comm) != MPI_SUCCESS)
	    return LAMERROR;
	  
	  /* Apply the reduction operation */
	  for (i = size - 2; i >= 0; --i) {
	    if (i == root) {
	      src = start;
	    } else {
	      src = lcd->lcd_message_segment[segment_in_use]
		+ (i * extent * copy_count_per_rank);
	    }
	    if (op->op_flags & LAM_LANGF77) {
	      (op->op_func)
		(src, dest, &copy_count_per_rank, &dtype->dt_f77handle);
	    } else {
	      (op->op_func) (src, dest, &copy_count_per_rank, &dtype);
	    }
	  }
	  
	  if (segment_in_use == lcd->lcd_num_segments - 1)
	    lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	  
	  count -= copy_count_per_rank;
	  if (count <= 0) {
	    break;
	  } else {
	    dest += copy_count_per_rank * extent;
	    start += copy_count_per_rank * extent;
	  }
	} /* end while */
      } /* else size != 1 */
      
    } else {
      
      src = sbuf;
      while (count > 0) {
	segment_in_use = lam_ssi_coll_shmem_increment_segment_in_use(lcd);
	
	copy_count_per_rank = (count < max_count_per_rank)
	  ? count : max_count_per_rank;
	dest = lcd->lcd_message_segment[segment_in_use]
	  + (extent * rank * copy_count_per_rank);
	
	
	/* copy the data into proper place in shared segment */
	if (lam_bottom_dtsnd(src, copy_count_per_rank, dtype, dest,
			     copy_count_per_rank, dtype, BLKMPIREDUCE,
			     comm) != MPI_SUCCESS)
	  return LAMERROR;
	
	/* tell the root that it has copied the data and it can grab all
	   the data corresponding to non-root processes for reduce operation */
	lam_ssi_coll_shmem_signal_by_non_roots(comm, segment_in_use, root,
					       rank);
	
	if (segment_in_use == lcd->lcd_num_segments - 1)
	  lam_ssi_coll_shmem_comm_barrier(comm, lcd->lcd_num_segments, rank);
	
	count -= copy_count_per_rank;
	if (count <= 0) {
	  break;
	} else {
	  src += copy_count_per_rank * extent;
	}
      } /* end while */
    }
  }
  return MPI_SUCCESS;
}



