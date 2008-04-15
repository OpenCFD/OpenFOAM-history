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
 * $Id: ssi_coll_smp_allgatherv.c,v 1.1 2003/10/22 23:17:03 adharurk Exp $
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
 *	allgatherv
 *
 *	Function:	- multi-algorithm allgather
 *	Accepts:	- same as MPI_Allgatherv()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_smp_allgatherv(void *sbuf, int scount,
			    MPI_Datatype sdtype, void * rbuf,
			    int *rcounts, int *disps,
			    MPI_Datatype rdtype,
			    MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  int err, rank;
  char *local_buffer = NULL, *local_origin = NULL;
  char *coord_buffer = NULL, *coord_origin = NULL;
  char * temp_buffer = NULL, *temp_origin = NULL;
  char *recv_ptr;

  int total_size, i, j, k;
  int total_count = 0, local_count = 0, prev_local_count = 0;
  MPI_Aint extent;

  MPI_Comm_size(comm, &total_size);
  MPI_Comm_rank(comm, &rank);
  MPI_Type_extent(rdtype, &extent);
  
  /* if only one process in local comm then just copy the sent buffer */
  
  /* determine the total count */
  for (i = 0; i < total_size; ++i) {
    if (rcounts[i] < 0)
      return EINVAL;
    total_count += rcounts[i];
  }
 
  /* determine the local count (total count in local_comm). Required for
     coordinator scatterv. */
 
  if (lcd->lcd_coord_comms[0] != NULL)
    for (i = 0; i < lcd->lcd_local_size; ++i) {
      local_count += rcounts[lcd->lcd_coord_global_ranks[i]];
    }

  if (lcd->lcd_local_size == 1) {
    err = lam_dtbuffer(sdtype, scount, &local_buffer, &local_origin);
    if (err != MPI_SUCCESS)
      return err;
    err = lam_dtsndrcv(sbuf, scount, sdtype, local_origin, scount, sdtype, 
		       BLKMPIALLGATHERV, comm);
    if (err != MPI_SUCCESS)
      return err;
    recv_ptr = local_origin;
    
  } else {
    int *recv_local_counts = NULL;
    int *global_disp = NULL;

    if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
      err = lam_dtbuffer(sdtype, local_count, &local_buffer,
			 &local_origin);
      if (err != MPI_SUCCESS)
	return err;
      recv_ptr = local_origin;
    } else {
       recv_ptr = NULL;
    }

    /* prepare for gatherv */

    if (lcd->lcd_coord_comms[0] != NULL) {
      int scan_size = 0;
      recv_local_counts = (int *) malloc(sizeof(int) * lcd->lcd_local_size); 
      global_disp = (int *) malloc(sizeof(int) * lcd->lcd_local_size); 
      if (recv_local_counts == NULL || global_disp == NULL)
	return ENOMEM;

      for (i = 0; i < lcd->lcd_local_size; ++i) {
	global_disp[i] = scan_size;
	recv_local_counts[i] = rcounts[lcd->lcd_coord_global_ranks[i]];
	scan_size += recv_local_counts[i];
      }
    } 

    err = MPI_Gatherv(sbuf, scount, sdtype, recv_ptr, recv_local_counts,
		      global_disp, sdtype, lcd->lcd_local_roots[0],
		      lcd->lcd_local_comm);
    if (err != MPI_SUCCESS)
      return err;

    if (recv_local_counts != NULL)
      free(recv_local_counts);
    if (global_disp != NULL)
      free(global_disp);
  }

  /*
    Allocate space for temporary buffer required for each process. This 
    buffer will have reordered array without considering displacement. 
    Each process then copy temporary buffer to rbuf considering displacement
  */

  err = lam_dtbuffer(rdtype, total_count, &temp_buffer,
		     &temp_origin);
  if (err != MPI_SUCCESS)
    return err;

  /* One of the coordinators will do a gatherv among coordinator comm. Let 
     this coordinator be rank 0 in coord_comm. Rank 0 in the coordinator comm 
     will always be the root ooordinator. Allocate space for recv buffer for 
     the root coordinator, */

  if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
    int coord_rank, coord_comm_size;
    int *recv_coord_counts = NULL;
    int *global_disp = NULL;

    MPI_Comm_rank(lcd->lcd_coord_comms[0], &coord_rank);
    MPI_Comm_size(lcd->lcd_coord_comms[0], &coord_comm_size);

    if (coord_rank == 0) {
      int scan_size = 0;

      global_disp = (int*) malloc(sizeof(int) * coord_comm_size);
      recv_coord_counts = (int*) malloc(sizeof(int) * coord_comm_size);
      if (global_disp == NULL || recv_coord_counts == NULL)
	return ENOMEM;
      k = 0;
      for (i = 0; i < coord_comm_size; ++i) {
	global_disp[i] = scan_size;
	recv_coord_counts[i] = 0;
	for (j = 0; j < lcd->lcd_local_comm_size[i]; j++) {
	  recv_coord_counts[i] += rcounts[lcd->lcd_coord_global_ranks[k]]; 
	  k++;
	}
	scan_size += recv_coord_counts[i];
      }
      
      err = lam_dtbuffer(rdtype, total_count, &coord_buffer,
			 &coord_origin);
      if (err != MPI_SUCCESS) 
	return err;
    }
    
    /* Call to Gatherv */

    err = MPI_Gatherv(recv_ptr, local_count, sdtype, 
		      coord_origin, recv_coord_counts, global_disp, rdtype, 0,
		      lcd->lcd_coord_comms[0]);
    if (err != MPI_SUCCESS)
      return err;

    /* release the global_disp and local_sizes buffers */

    if (global_disp != NULL)
      free(global_disp);
    if (recv_coord_counts != NULL)
      free(recv_coord_counts);


    /* If I am the main coordinator then reoorganize the results considering 
       the displacement. */
    
    if (coord_rank == 0) {
      int prev_gather_count = 0, prev_reorder_count;
      for (j = 0; j < total_size; ++j) {
	prev_reorder_count = 0;
	for (k = 0; k < lcd->lcd_coord_global_ranks[j]; ++k)
	  prev_reorder_count += rcounts[k];

	/* Now copy rcount[global_ranks[j]] elements from prev_count th
	   position of coord_origin to prev_reorder_count th position in 
	   rbuf */
	
	err = lam_dtsndrcv(coord_origin + (extent * prev_gather_count), 
			   rcounts[lcd->lcd_coord_global_ranks[j]],
			   rdtype, 
			   temp_origin + 
			   (extent * prev_reorder_count),
			   rcounts[lcd->lcd_coord_global_ranks[j]], rdtype,
			   BLKMPIALLGATHERV, comm);
	if (err != MPI_SUCCESS) 
	  return err;
	prev_gather_count += rcounts[lcd->lcd_coord_global_ranks[j]];
      }
    }

    /* Now the result is present in the rbuf corresponding to the root 
       coordinator. Broadcast the result to all the coordinators. */
    
    MPI_Bcast(temp_origin, total_count, rdtype, 0,
	      lcd->lcd_coord_comms[0]);
    
  }

  /* Now each coordinator will have the reordered result. Brroadcast the 
     results locally. Little optimization that if the local_comm_size is
     equal to one then no need to broadcast the results. */
  
  if (lcd->lcd_local_size != 1) 
    MPI_Bcast(temp_origin, total_count, rdtype, 
	      lcd->lcd_local_roots[0], lcd->lcd_local_comm);

  /* Now each process will copy the reordered array to rbuf considering
     the displacement */

  for (i = 0; i < total_size; ++i) {
    err = lam_dtsndrcv(temp_origin + (extent * prev_local_count),
		       rcounts[i], rdtype,
		       ((char*)rbuf) + (extent * disps[i]), rcounts[i], rdtype,
		       BLKMPIALLGATHERV, comm);
    if (err != MPI_SUCCESS)
      return err;
    prev_local_count += rcounts[i];
  }

  /* All done. Free all the buffers. */ 

  if (coord_buffer != NULL)
    free(coord_buffer);
  if (local_buffer != NULL)
    free(local_buffer);
  if (temp_buffer != NULL)
    free(temp_buffer);
    
  return MPI_SUCCESS;
}

