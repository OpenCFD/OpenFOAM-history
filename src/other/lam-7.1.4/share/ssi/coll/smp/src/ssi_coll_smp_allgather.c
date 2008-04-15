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
 * $Id: ssi_coll_smp_allgather.c,v 1.2 2003/11/07 19:17:05 adharurk Exp $
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
 *	allgather
 *
 *	Function:	- multi-algorithm allgather
 *	Accepts:	- same as MPI_Allgather()
 *	Returns:	- MPI_SUCCESS or error code
 */
int 
lam_ssi_coll_smp_allgather(void *sbuf, int scount,
			   MPI_Datatype sdtype, void *rbuf, int rcount,
			   MPI_Datatype rdtype, MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  int err;
  char *local_buffer = NULL, *local_origin = NULL;
  char *coord_buffer = NULL, *coord_origin = NULL;
  char *recv_ptr;

  /* determine the size of the comm */
  int total_size;

  MPI_Comm_size(comm, &total_size);
  
  /* if only one process in local comm then just copy the sent buffer */
  
  if (lcd->lcd_local_size == 1) {
    err = lam_dtbuffer(sdtype, scount, &local_buffer, &local_origin);
    err = lam_dtsndrcv(sbuf, scount, sdtype, local_origin, rcount, rdtype, 
		       BLKMPIALLGATHER, comm);
    recv_ptr = local_origin;
    
  } else {
    if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
      err = lam_dtbuffer(sdtype, scount * lcd->lcd_local_size, &local_buffer,
			 &local_origin);
      if (err != MPI_SUCCESS)
	return err;
      recv_ptr = local_origin;
    } else {
       recv_ptr = NULL;
    }
    
    /* do a local Gather */
   
    err = MPI_Gather(sbuf, scount, sdtype, recv_ptr, rcount, sdtype, 
		     lcd->lcd_local_roots[0], lcd->lcd_local_comm);
    if (err != MPI_SUCCESS) {
      return err;
    }
  }

  /* One of the coordinators will do a gatherv among coordinator comm. Let 
     this coordinator be rank 0 in coord_comm. Rank 0 in the coordinator comm 
     will always be the root ooordinator. Allocate space for recv buffer for 
     the root coordinator, */

  if (lcd->lcd_coord_comms[0] != MPI_COMM_NULL) {
    int coord_rank, coord_comm_size;
    int *local_sizes = NULL;
    int *global_disp = NULL;

    MPI_Comm_rank(lcd->lcd_coord_comms[0], &coord_rank);
    MPI_Comm_size(lcd->lcd_coord_comms[0], &coord_comm_size);

    if (coord_rank == 0) {
      int i, scan_size = 0;

      global_disp = (int*) malloc(sizeof(int) * coord_comm_size);
      local_sizes = (int*) malloc(sizeof(int) * coord_comm_size);
      if (global_disp == NULL || local_sizes == NULL)
	return ENOMEM;

      for (i = 0; i < coord_comm_size; ++i) {
	global_disp[i] = scan_size;
	local_sizes[i] = lcd->lcd_local_comm_size[i] * scount; 
	scan_size += local_sizes[i];
      }
      
      err = lam_dtbuffer(rdtype, rcount * total_size, &coord_buffer,
			 &coord_origin);
      if (err != MPI_SUCCESS) 
	return err;
    }
    
    /* Call to Gatherv */

    err = MPI_Gatherv(recv_ptr, scount * lcd->lcd_local_size, sdtype, 
		      coord_origin, local_sizes, global_disp, rdtype, 0,
		      lcd->lcd_coord_comms[0]);
    if (err != MPI_SUCCESS)
      return err;

    /* release the global_disp and local_sizes buffers */

    if (global_disp != NULL)
      free(global_disp);
    if (local_sizes != NULL)
      free(local_sizes);

    /* If I am the main coordinator then reoorganize the results. */
    
    if (coord_rank == 0) {
      MPI_Aint extent;
      int j;

      MPI_Type_extent(rdtype, &extent);
      for (j = 0; j < total_size; ++j) {
	
	/* Now copy scount elements from jth position of coord_origin to
	   global_ranks[j]th position in rbuf */

	err = lam_dtsndrcv(coord_origin + (extent * rcount * j), rcount,
			   rdtype, 
			   ((char *)rbuf) + 
			   (extent * rcount * lcd->lcd_coord_global_ranks[j]),
			   rcount, rdtype, BLKMPIALLGATHER, comm);
	if (err != MPI_SUCCESS) 
	  return err;
      }
    }

    /* Now the result is present in the rbuf corresponding to the root 
       coordinator. Broadcast the result to all the coordinators. */
    
    MPI_Bcast(rbuf, rcount * total_size, rdtype, 0,
	      lcd->lcd_coord_comms[0]);
    
  }

  /* Now each coordinator will have the result. Brroadcast the results locally.
     Little optimization that if the local_comm_size is equal to one then no
     need to broadcast the results. */
  
  if (lcd->lcd_local_size != 1) 
    MPI_Bcast(rbuf, rcount * total_size, rdtype, 
	      lcd->lcd_local_roots[0], lcd->lcd_local_comm);
  
  /* All done. Free all the buffers. */ 

  if (coord_buffer != NULL)
    free(coord_buffer);
  if (local_buffer != NULL)
    free(local_buffer);

  return MPI_SUCCESS;
}

