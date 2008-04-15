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
 * $Id: ssi_coll_lam_basic_reduce_scatter.c,v 1.2 2003/05/28 00:16:28 jsquyres Exp $
 *
 *	Function:	- Basic collective routines
 */

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpi.h>
#include <blktype.h>
#include <mpisys.h>


/*
 *	reduce_scatter
 *
 *	Function:	- reduce then scatter
 *	Accepts:	- same as MPI_Reduce_scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_reduce_scatter(void *sbuf, void *rbuf, int *rcounts,
				      MPI_Datatype dtype, MPI_Op op,
				      MPI_Comm comm)
{
  int i;
  int err;
  int rank;
  int size;
  int count;
  int *disps = 0;
  char *buffer = 0;
  char *origin = 0;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  /* Initialize reduce & scatterv info at the root (rank 0). */

  for (i = 0, count = 0; i < size; ++i) {
    if (rcounts[i] < 0) {
      return EINVAL;
    }
    count += rcounts[i];
  }

  if (rank == 0) {
    disps = (int *) malloc((unsigned) size * sizeof(int));
    if (disps == 0) {
      free((char *) disps);
      return errno;
    }

    err = lam_dtbuffer(dtype, count, &buffer, &origin);
    if (err != MPI_SUCCESS) {
      free((char *) disps);
      return err;
    }

    disps[0] = 0;
    for (i = 0; i < (size - 1); ++i)
      disps[i + 1] = disps[i] + rcounts[i];
  }

  /* reduction */

  err = MPI_Reduce(sbuf, origin, count, dtype, op, 0, comm);
  if (err != MPI_SUCCESS) {
    if (disps)
      free((char *) disps);
    if (buffer)
      free(buffer);
    return err;
  }

  /* scatter */

  err = MPI_Scatterv(origin, rcounts, disps, dtype,
		     rbuf, rcounts[rank], dtype, 0, comm);
  if (disps)
    free((char *) disps);
  if (buffer)
    free(buffer);
  return err;
}

/*
 *	intercomm reduce_scatter
 *
 *	Function:	- intercomm reduce then scatter
 *	Accepts:	- same as MPI_Reduce_scatter()
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_coll_lam_basic_reduce_scatter_inter(void *sbuf, void *rbuf, 
					    int *rcounts,
					    MPI_Datatype dtype, MPI_Op op,
					    MPI_Comm intercomm)
{
  int local_rank, local_size, remote_size, total_count;
  int i, root;
  int* disps;
  char* local_buffer = NULL, *local_origin = NULL;
  MPI_Comm local_comm;
  int is_low;

  local_comm = intercomm->c_ssi_coll_lam_basic_data->local_comm;
  is_low = intercomm->c_ssi_coll_lam_basic_data->is_low;

  MPI_Comm_size(intercomm, &local_size);
  MPI_Comm_rank(intercomm, &local_rank);
  MPI_Comm_remote_size(intercomm, &remote_size);
  
  /* Get total count so that I can allocate that much space for Reduce and 
     subsequent scatterv */
  total_count = 0;
  for (i = 0; i < local_size; ++i)
    total_count += rcounts[i];
  
  if (local_rank == 0) {
    /* Both the leaders will allocate space for a temprary buffer */
    if ((lam_dtbuffer(dtype, total_count, &local_buffer,
		      &local_origin)) != MPI_SUCCESS)
      return LAMERROR;
    
    /* Form displacement array for scatterv operation */
    disps = (int*) malloc(local_size * sizeof(int));
    total_count = 0;
    for (i = 0; i < local_size; ++i) {
      disps[i] = total_count;
      total_count += rcounts[i];
    }
  }

  /* Now do a Intercommunicator Reduce */
  if (is_low) {
    /* lower group */
    root = (local_rank == 0) ? MPI_ROOT : MPI_PROC_NULL;
    if ((MPI_Reduce(sbuf, local_origin, total_count, dtype, op, root,
		    intercomm)) != MPI_SUCCESS)
      return LAMERROR;

    root = 0;
    if ((MPI_Reduce(sbuf, local_origin, total_count, dtype, op, root,
		    intercomm)) != MPI_SUCCESS)
      return LAMERROR;

  } else {
    /* higher group */
    root = 0;
    if ((MPI_Reduce(sbuf, local_origin, total_count, dtype, op, root,
		    intercomm)) != MPI_SUCCESS)
      return LAMERROR;

    root = (local_rank == 0) ? MPI_ROOT : MPI_PROC_NULL;
    if ((MPI_Reduce(sbuf, local_origin, total_count, dtype, op, root,
		    intercomm)) != MPI_SUCCESS)
      return LAMERROR;
  }

  /* Now All do a Scatterv */
  if ((MPI_Scatterv(local_origin, rcounts, disps, dtype, rbuf, 
		    rcounts[local_rank], dtype, 0, local_comm)) 
      != MPI_SUCCESS)
    return LAMERROR;
  
  if (local_rank == 0) {
    /* free the buffers */
    free(disps);
    free(local_origin);
  }

  return MPI_SUCCESS;
}



