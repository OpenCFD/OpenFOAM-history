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
 * $Id: ssi_coll_smp_reduce_scatter.c,v 1.3 2003/10/22 23:20:25 adharurk Exp $
 *
 *	Function:	- SMP-aware collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-smp-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-smp.h>
#include <mpi.h>
#include <mpisys.h>
#include <blktype.h>


/*
 * Local functions
 */
static int assoc_reduce_scatter(void *sbuf, void *rbuf, int *rcounts,
				MPI_Datatype dtype, MPI_Op op,
				MPI_Comm comm);

static int nonassoc_reduce_scatter(void *sbuf, void *rbuf, int *rcounts,
				   MPI_Datatype dtype, MPI_Op op,
				   MPI_Comm comm);

/*
 *	reduce_scatter
 *
 *	Function:	- multi-algorithm reduce_scatter
 *	Accepts:	- same as MPI_Reduce_scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_smp_reduce_scatter(void *sbuf, void *rbuf, int *rcounts,
				MPI_Datatype dtype, MPI_Op op,
				MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  int size, i, mag_short = 1;
  
  /* If this communicator was marked to be associative, use the wide
     area optimal associative algorithm. */

  if (lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_ASSOCIATIVE) == 1 &&
      op->op_commute)
    return assoc_reduce_scatter(sbuf, rbuf, rcounts, dtype, op, comm);

  /* Otherwise, look at how many bytes will be transferred by each
     process to determine whether to use MagPIe algorithm for short
     messages or lam_basic algorithm */

  MPI_Comm_size(comm, &size);
  for (i = 0; i < size; ++i)
    if (rcounts[i] * dtype->dt_size > lcd->lcd_reduce_crossover) {
      mag_short = 0;
      break;
    }
  
  if (mag_short) 
    return nonassoc_reduce_scatter(sbuf, rbuf, rcounts, dtype, op, comm);
  else
    return lcd->lb_functions.lsca_reduce_scatter(sbuf, rbuf, rcounts, dtype, 
                                                 op, comm);
}


static int 
assoc_reduce_scatter(void *sbuf, void *rbuf, int *rcounts,
		     MPI_Datatype dtype, MPI_Op op,
		     MPI_Comm comm)
{
  int err, rank;
  char *local_buffer = NULL, *local_origin = NULL;
  char *coord_recv_buffer = NULL, *coord_recv_origin = NULL;
  char *recv_ptr;
  int i;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;

  int total_size, total_count = 0, local_count = 0;
  int *local_send_counts = NULL, *local_disp = NULL;
 
  MPI_Comm_size(comm, &total_size);
  MPI_Comm_rank(comm, &rank);

  /* determine the total count */
  for (i = 0; i < total_size; ++i) {
    if (rcounts[i] < 0)
      return EINVAL;
    total_count += rcounts[i]; 
  }

  /* determine the local count (total count in local_comm). Required for 
     coordinator scatterv. */

  if (lcd->lcd_coord_comms[0] != NULL)
    for (i = 0; i < lcd->lcd_local_size; ++i)
      local_count += rcounts[lcd->lcd_coord_global_ranks[i]];

  /* if only one process in local comm then just copy the sent buffer */
 
  if (lcd->lcd_local_size == 1) {
    err = lam_dtbuffer(dtype, total_count, &local_buffer, &local_origin);
    if (err != MPI_SUCCESS)
      return err;
    err = lam_dtsndrcv(sbuf, total_count, dtype, local_origin, total_count,
		       dtype, BLKMPIREDUCESCATTER, comm);
    if (err != MPI_SUCCESS)
      return err;
    recv_ptr = local_origin;
 
  } else {
    if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
      err = lam_dtbuffer(dtype, total_count, &local_buffer, &local_origin);
      if (err != MPI_SUCCESS)
        return err;
      recv_ptr = local_origin;
    } else {
      recv_ptr = NULL;
    }
 
    /* do a local reduce */
    
    err = MPI_Reduce(sbuf, recv_ptr, total_count, dtype, op,
                     lcd->lcd_local_roots[0], lcd->lcd_local_comm);
    if (err != MPI_SUCCESS) 
      return err;
  }
 
  /* One of the coordinators will do a reduce among coordinator comm. Let
     this root coordinator be rank 0 in coord_comm. */
 
  if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
    int *coord_send_counts = NULL, *global_disp = NULL;
    char *coord_buffer = NULL, *coord_origin = NULL;
    char *temp_buffer = NULL, *temp_origin = NULL;
    int coord_rank, coord_comm_size;
    int scan_size = 0;
    int i, j, k;

    MPI_Comm_rank(lcd->lcd_coord_comms[0], &coord_rank);
    MPI_Comm_size(lcd->lcd_coord_comms[0], &coord_comm_size);

    if (coord_rank == 0) {
      err = lam_dtbuffer(dtype, total_count, &coord_buffer,
			 &coord_origin);
      if (err != MPI_SUCCESS)
	return err;
    }

    /* Call to Reduce */
    
    err = MPI_Reduce(recv_ptr, coord_origin, total_count, dtype, op, 0, 
		     lcd->lcd_coord_comms[0]);
    if (err != MPI_SUCCESS)
      return err;
    
    /* If I am the main coordinator then reoorganize the results. */
    
    if (coord_rank == 0) {
      MPI_Aint extent;
      int filled_size = 0;

      /* Allocate space for the dummy array. */
      err = lam_dtbuffer(dtype, total_count, &temp_buffer,
                         &temp_origin);
      if (err != MPI_SUCCESS)
        return err;

      /* Reorganize the result which is ready for scatterv. */

      MPI_Type_extent(dtype, &extent);
      for (j = 0; j < total_size; ++j) {
	int prev_count = 0;
	for (k = 0; k < lcd->lcd_coord_global_ranks[j]; ++k) 
	  prev_count += rcounts[k];
        err = lam_dtsndrcv(coord_origin + (extent * prev_count), 
			   rcounts[lcd->lcd_coord_global_ranks[j]],
                           dtype, temp_origin + (extent * filled_size),
                           rcounts[lcd->lcd_coord_global_ranks[j]],
			   dtype, BLKMPIREDUCESCATTER, comm);
        if (err != MPI_SUCCESS)
	  return err;
	filled_size += rcounts[lcd->lcd_coord_global_ranks[j]];
      }
      
      /* Form sendcounts and global_disp arrays required for scatterv 
	 operatioon. */
      
      coord_send_counts = (int *) malloc(sizeof(int) * coord_comm_size);
      global_disp = (int *) malloc(sizeof(int) * coord_comm_size);
      if (coord_send_counts == NULL || global_disp == NULL)
	return ENOMEM;

      for (i = 0; i < coord_comm_size; ++i) {
	coord_send_counts[i] = 0;
      }
      k = 0;
      for (i = 0; i < coord_comm_size; ++i) {
	global_disp[i] = scan_size;
	for (j = 0; j < lcd->lcd_local_comm_size[i]; ++j) {
	  coord_send_counts[i] += rcounts[lcd->lcd_coord_global_ranks[k]]; 
	  k++;
	}
	scan_size += coord_send_counts[i];
      }
      
    }

    /* allocate buffer for all the coordinators for recieving the results
       scatttered by the root coordinator */
 
    err = lam_dtbuffer(dtype, local_count, &coord_recv_buffer,
                       &coord_recv_origin);
    if (err != MPI_SUCCESS)
      return err;

    /* call scatterv */
    
    err = MPI_Scatterv(temp_origin, coord_send_counts, global_disp, dtype,
		       coord_recv_origin, local_count, dtype, 0,
		       lcd->lcd_coord_comms[0]);
    
    if (err != MPI_SUCCESS)
      return err;

    if (coord_send_counts != NULL)
      free(coord_send_counts);
    if (global_disp != NULL)
      free(global_disp);

    /* Since I am coordinator prepare data structure for local scatterv */
       
    local_send_counts = (int*) malloc(sizeof(int) * lcd->lcd_local_size);
    local_disp = (int*) malloc(sizeof(int) * lcd->lcd_local_size);
    if (local_send_counts == NULL || local_disp == NULL)
      return ENOMEM;

    scan_size = 0;
    for (i = 0; i < lcd->lcd_local_size; ++i) {
      local_disp[i] = scan_size;
      local_send_counts[i] = rcounts[lcd->lcd_coord_global_ranks[i]];
      scan_size += local_send_counts[i];
    }

    /* Free ccoord_buffer and temp_buffer which are not required */

    if (coord_buffer != NULL)
      free(coord_buffer);
    if (temp_buffer != NULL)
      free(temp_buffer);
      
  }
    
  /* Now a local scatterv */

  if (lcd->lcd_local_size != 1) {
    err = MPI_Scatterv(coord_recv_origin, local_send_counts, local_disp,
		       dtype, rbuf, rcounts[rank], dtype, 0, 
		       lcd->lcd_local_comm);
    if (err != MPI_SUCCESS)
      return err;
  } else {
    err = lam_dtsndrcv(coord_recv_origin, rcounts[rank], dtype, rbuf,
		       rcounts[rank], dtype, BLKMPIREDUCESCATTER, comm);
    if (err != MPI_SUCCESS)
      return err;
  }
  
  /* All done free all the buffers */
  
  if (local_send_counts != NULL)
    free(local_send_counts);
  if (local_disp != NULL)
    free(local_disp);
    
  if (local_buffer != NULL)
    free(local_buffer);
  if (coord_recv_buffer != NULL)
    free(coord_recv_buffer);
  
  return MPI_SUCCESS;
}

static int
nonassoc_reduce_scatter(void *sbuf, void *rbuf, int *rcounts,
			MPI_Datatype dtype, MPI_Op op,
			MPI_Comm comm)
{

  int i, err, rank, size;
  MPI_Aint extent;
  char *local_buffer = NULL, *local_origin = NULL;
  char *inbuf;
  int total_count = 0, offset = 0;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  for (i = 0; i < size; ++i) {
    total_count += rcounts[i];
  }
  
  /* For short messages, give everyone the data and do the reduction
     locally.  Allocate a buffer to allgather into. */
 
  err = lam_dtbuffer(dtype, total_count * size, &local_buffer, &local_origin);
  if (err != MPI_SUCCESS)
    return err;
 
  /* Do the allgather */

  err = MPI_Allgather(sbuf, total_count, dtype, local_origin, total_count,
		      dtype, comm);
  if (err != MPI_SUCCESS)
    return err;
  
  /* Now perform the reduction locally */
  /* Initialize the receive buffer. */
 
  MPI_Type_extent(dtype, &extent);
  for (i = 0; i < rank; ++i)
    offset += rcounts[i];
  err = lam_dtsndrcv(local_origin + (total_count * extent * (size - 1))
		     + (offset * extent),
                     rcounts[rank], dtype, rbuf, rcounts[rank], dtype,
		     BLKMPIREDUCESCATTER, comm);
  if (err != MPI_SUCCESS) {
    free(local_origin);
    return err;
  }

  for (i = size - 2; i >= 0; --i) {
 
    /* Call reduction function. */
 
    inbuf = local_origin + (total_count * extent * i) + (offset * extent);
    if (op->op_flags & LAM_LANGF77) {
      (op->op_func)(inbuf, rbuf, &(rcounts[rank]), &dtype->dt_f77handle);
    } else {
      (op->op_func)(inbuf, rbuf, &(rcounts[rank]), &dtype);
    }
  }
 
  /* Free temporary buffer */
 
  if (local_buffer != NULL)
    free(local_buffer);
 
  /* All done */
 
  return MPI_SUCCESS;
}
  
