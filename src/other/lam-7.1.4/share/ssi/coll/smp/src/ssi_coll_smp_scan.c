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
 * $Id: ssi_coll_smp_scan.c,v 1.1 2003/10/22 23:17:44 adharurk Exp $
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
 *	scan
 *
 *	Function:	- multi-algorithm scan
 *	Accepts:	- same as MPI_Scan()
 *	Returns:	- MPI_SUCCESS or error code
 */

static int lam_ssi_coll_smp_ass_scan(void *sbuf, void *rbuf, int count,
				     MPI_Datatype dtype, MPI_Op op, 
				     MPI_Comm comm);


static int lam_ssi_coll_smp_nonass_scan(void *sbuf, void *rbuf, int count,
					MPI_Datatype dtype, MPI_Op op,
					MPI_Comm comm);

int
lam_ssi_coll_smp_scan(void *sbuf, void *rbuf, int count,
                      MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  
  if (lam_ssi_coll_base_get_param(comm, LAM_MPI_SSI_COLL_ASSOCIATIVE) == 1)
    return lam_ssi_coll_smp_ass_scan(sbuf, rbuf, count, dtype, op, comm);
 
  /* Otherwise, look at how many bytes will be transferred by each
     process to determine whether to use the lam_basic algorithm, or
     the short MagPIe algorithm */
 
  if ((count * dtype->dt_size) < lcd->lcd_reduce_crossover)
    return lam_ssi_coll_smp_nonass_scan(sbuf, rbuf, count, dtype, op, comm);
  else
    return lcd->lb_functions.lsca_scan(sbuf, rbuf, count, dtype, op, comm);
}  

int 
lam_ssi_coll_smp_ass_scan(void *sbuf, void *rbuf, int count,
			  MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{

  int rank, size, err;
  lam_ssi_coll_data_t *lcd = comm->c_ssi_coll_data;
  char *local_buffer = NULL, *local_origin = NULL;
  char *coord_buffer = NULL, *coord_origin = NULL;
  
  MPI_Group comm_group, local_group;
  int iamcoordinator, scan_comm_rank;
  int  maxrank, local_rank;
  int i;
  
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  /* First job is to create a new communicator for performing scan. 
     Relatively higher ranked processes will act as coordinators in this
     new communicator */
  
  /* Broadcast the global_ranks array within local communicator so that each
     process can determine whether it is a coordinator or not */

  MPI_Bcast(lcd->lcd_coord_global_ranks, lcd->lcd_size, MPI_INT, 0,
	    lcd->lcd_local_comm);

  /* Find out maximum rank in local comm */
  maxrank = -1;
  for (i = 0; i < lcd->lcd_local_size ; ++i)
    if (lcd->lcd_coord_global_ranks[i] > maxrank) {
      maxrank = lcd->lcd_coord_global_ranks[i];
    }

  iamcoordinator = (maxrank == rank);

  MPI_Comm_split(comm, iamcoordinator ? 1 : MPI_UNDEFINED, 0, 
		 &(lcd->lcd_scan_comm));
  
  /* Do a local scan first */
  
  err = lam_dtbuffer(dtype, count, &local_buffer, &local_origin);
  if (err != MPI_SUCCESS)
    return err;

  err = MPI_Scan(sbuf, local_origin, count, dtype, op, lcd->lcd_local_comm);
  if (err != MPI_SUCCESS)
    return err;

  /* Allocate the buffer for all processes, that will contain partial result
     reduced by their coordinator */
  
  err = lam_dtbuffer(dtype, count, &coord_buffer, &coord_origin);
  if (err != MPI_SUCCESS)
    return err;

  /* Now do a exclusive scan among the coordinators */

  if (lcd->lcd_scan_comm != MPI_COMM_NULL) {

    MPI_Comm_rank(lcd->lcd_scan_comm, &scan_comm_rank);
    err = MPI_Exscan(local_origin, coord_origin, count, dtype, op, 
		     lcd->lcd_scan_comm);
    if (err != MPI_SUCCESS)
      return err;

  }

  /* partial result will be in coord_origin and local result is in the 
     local_origin. Boradcast partial results in local comm. Processes should
     know the local rank of their coordinator in scan_comm */ 

  MPI_Comm_group(comm, &comm_group);
  MPI_Comm_group(lcd->lcd_local_comm, &local_group);
  MPI_Group_translate_ranks(comm_group, 1, &maxrank,
                            local_group, &local_rank);

  /* Coordinator: Broadcast the rank in scan comm */

  err = MPI_Bcast(&scan_comm_rank, 1, MPI_INT, local_rank,
		  lcd->lcd_local_comm); 
  if (err != MPI_SUCCESS)
    return err;

  /* No need to do anything for the processes that belong to the 
     coordinator 0 in scan_comm */
  
  if (scan_comm_rank != 0) {
    if (lcd->lcd_local_size != 1) {
      err = MPI_Bcast(coord_origin, count, dtype, local_rank,
		      lcd->lcd_local_comm);
      if (err != MPI_SUCCESS)
	return err;
    }
    
    /* Now each process has scanned the result in local_origin and rest
       partial result in coord_origin. Reduce both the results and transfer 
       them to rbuf */

    if (op->op_commute) {
      if (op->op_flags & LAM_LANGF77) {
	(op->op_func)
	  (local_origin, coord_origin, &count, &dtype->dt_f77handle);
      } else {
	(op->op_func) (local_origin, coord_origin, &count, &dtype);
      }
      err = lam_dtsndrcv(coord_origin, count, dtype, rbuf,
			 count, dtype, BLKMPIEXSCAN, comm);

    } else {
      if (op->op_flags & LAM_LANGF77) {
	(op->op_func)
	  (coord_origin, local_origin, &count, &dtype->dt_f77handle);
      } else {
	(op->op_func) (coord_origin, local_origin, &count, &dtype);
      }
      err = lam_dtsndrcv(local_origin, count, dtype, rbuf,
			 count, dtype, BLKMPIEXSCAN, comm);
      
    }
  } else {
    err = lam_dtsndrcv(local_origin, count, dtype, rbuf,
		       count, dtype, BLKMPIEXSCAN, comm);
  }

  if (err != MPI_SUCCESS)
    return err;
  
  /* All Done */

  if (local_buffer != NULL)
    free(local_buffer);
  if (coord_buffer != NULL)
    free(coord_buffer);
  if (lcd->lcd_scan_comm != MPI_COMM_NULL)
    MPI_Comm_free(&lcd->lcd_scan_comm);
  if (comm_group != MPI_GROUP_NULL)
    MPI_Group_free(&comm_group);
  if (local_group != MPI_GROUP_NULL)
    MPI_Group_free(&local_group);
  
  return MPI_SUCCESS;
}

int
lam_ssi_coll_smp_nonass_scan(void *sbuf, void *rbuf, int count,
			     MPI_Datatype dtype, MPI_Op op, MPI_Comm comm)
{
  int i, err, rank, size;
  MPI_Aint extent;
  char *local_buffer = NULL, *local_origin;
  char *inbuf;
 
  /* For short messages, give everyone the data and do the reduction
     locally.  Allocate a buffer to allgather into. */
 
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  err = lam_dtbuffer(dtype, count * size, &local_buffer, &local_origin);
  if (err != MPI_SUCCESS)
    return err;

  /* Do the allgather */
 
  err = MPI_Allgather(sbuf, count, dtype, local_origin, count, dtype, comm);
  if (err != MPI_SUCCESS)
    return err;
 
  /* Now perform the reduction locally */
  /* Initialize the receive buffer. */
 
  MPI_Type_extent(dtype, &extent);

  err = lam_dtsndrcv(local_origin + (extent * count * rank),
                     count, dtype, rbuf, count, dtype, BLKMPIREDUCE, comm);
  if (err != MPI_SUCCESS) {
    free(local_origin);
    return err;
  }
 
  for (i = rank - 1; i >= 0; --i) {
 
    /* Call reduction function. */
 
    inbuf = local_origin + (extent * count * i);
    if (op->op_flags & LAM_LANGF77) {
      (op->op_func)(inbuf, rbuf, &count, &dtype->dt_f77handle);
    } else {
      (op->op_func)(inbuf, rbuf, &count, &dtype);
    }
  }
 
  /* Free temporary buffer */
 
  if (local_buffer != NULL)
    free(local_buffer);
 
  /* All done */
  
  return MPI_SUCCESS;

}
