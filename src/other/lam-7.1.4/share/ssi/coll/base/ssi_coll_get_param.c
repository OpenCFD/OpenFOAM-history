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
 * $Id: ssi_coll_get_param.c,v 1.3 2004/03/09 16:49:58 adharurk Exp $
 *
 */

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>
#include <mpi.h>
#include <mpisys.h>


int
lam_ssi_coll_base_get_param(MPI_Comm comm, int keyval)
{
  void **val;
  int ret, flag;

  /* First get the value that's in the global variable (which
     initially had a default value, but may have been overridden by an
     environment variable at run-time) */

  switch (keyval) {
  case LAM_MPI_SSI_COLL_CROSSOVER:
    ret = lam_ssi_coll_base_crossover;
    break;

  case LAM_MPI_SSI_COLL_ASSOCIATIVE:
    ret = lam_ssi_coll_base_associative;
    break;

  case LAM_MPI_SSI_COLL_REDUCE_CROSSOVER:
    ret = lam_ssi_coll_base_reduce_crossover;
    break;
    
  case LAM_MPI_SSI_COLL_SHMEM_MESSAGE_POOL_SIZE:
    ret = lam_ssi_coll_base_shmem_message_pool_size;
    break;
    
  case LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS:
    ret = lam_ssi_coll_base_shmem_num_segments;
    break;

  default:
    return LAMERROR;
  }

  /* Now check and see there's another override on the corresponding
     attribute on this communicator.  If it's there, it will overwrite
     the value of ret.  If it's not there, ret will be left alone. */

  MPI_Comm_get_attr(comm, keyval, &val, &flag);
  if (flag == 1)
    ret = (int) *val;

  /* All done */

  return ret;
}
