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
 * $Id: bcast.c,v 6.15 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- broadcast info to all nodes SIMD-style
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- root
 *			- communicator
 *	Returns:	- MPI_SUCCESS or an MPI error code
 */

#include <lam_config.h>

#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lam-ssi-rpi.h>
#include <lam-ssi-coll.h>


/*@

MPI_Bcast - Broadcasts a message from the process with rank "root" to
            all other processes of the group. 

Input/output Parameters:
+ buff - starting address of buffer (choice) 
. count - number of entries in buffer (integer) 
. datatype - data type of buffer (handle) 
. root - rank of broadcast root (integer) 
- comm - communicator (handle) 

.N IMPI_YES

Algorithm:  

For 4 or less ranks, a linear algorithm is used, where rank 0
loops over sending the message to each other rank.

If more than 4 ranks are involved, a tree-based algorithm is used to
send the messages from rank 0.

.N fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_COLL_NOT_IMPLEMENTED
.N MPI_ERR_COUNT
.N MPI_ERR_TYPE
.N MPI_ERR_BUFFER
.N MPI_ERR_ROOT

.N ACK
@*/
int MPI_Bcast(void *buff, int count, MPI_Datatype datatype, 
	      int root, MPI_Comm comm)
{
    int size;
    int err;
    struct _gps *p;
    lam_ssi_coll_bcast_fn_t func;

    lam_initerr();
    lam_setfunc(BLKMPIBCAST);
/*
 * Check for invalid arguments.
 */
    if (comm == MPI_COMM_NULL) {
      return(lam_errfunc(comm, BLKMPIBCAST, 
			 lam_mkerr(MPI_ERR_COMM, EINVAL)));
    }
    func = comm->c_ssi_coll.lsca_bcast;

    if (datatype == MPI_DATATYPE_NULL) {
      return(lam_errfunc(comm, BLKMPIBCAST, 
			 lam_mkerr(MPI_ERR_TYPE, EINVAL)));
    }

    if (count < 0) {
      return(lam_errfunc(comm, BLKMPIBCAST, 
			 lam_mkerr(MPI_ERR_COUNT, EINVAL)));
    }

    if (LAM_IS_INTRA(comm)) {
      MPI_Comm_size(comm, &size);
      if ((root >= size) || (root < 0)) {
	return(lam_errfunc(comm, BLKMPIBCAST, 
			   lam_mkerr(MPI_ERR_ROOT, EINVAL)));
      }
      /*
       * Set debugging parameters 
       */
      
      p = &(comm->c_group->g_procs[root]->p_gps);
      lam_setparam(BLKMPIBCAST,
		   root | (p->gps_grank << 16), 
		   (p->gps_node << 16) | p->gps_idx);
      /*
       * Check for zero count case.
       */
      if (count == 0 && comm->c_ssi_coll.lsca_bcast_optimization) {
	LAM_TRACE(lam_tr_cffend(BLKMPIBCAST, root, comm, datatype, count));
	lam_resetfunc(BLKMPIBCAST);
	return(MPI_SUCCESS);
      }

      /* If there's only one node, we're done */
      
      else if (size <= 1) {
	LAM_TRACE(lam_tr_cffend(BLKMPIBCAST, root, comm, datatype, count));
	lam_resetfunc(BLKMPIBCAST);
	return(MPI_SUCCESS);
      }
    } else {
      MPI_Comm_remote_size(comm, &size);
      if (!(((root < size) && (root >= 0)) 
	|| (root == MPI_ROOT) || (root == MPI_PROC_NULL))) {
	return(lam_errfunc(comm, BLKMPIBCAST, 
			   lam_mkerr(MPI_ERR_ROOT, EINVAL)));
      }
    } 

    if (func == NULL) {
      return(lam_errfunc(comm, BLKMPIBCAST,
			 lam_mkerr(MPI_ERR_OTHER, ENOT_IMPLEMENTED)));
    }

    LAM_TRACE(lam_tr_cffstart(BLKMPIBCAST));

    /* Call the coll SSI to actually perform the barrier */
    
    if ((err = func(buff, count, datatype, root, comm)) != MPI_SUCCESS)
      return(lam_errfunc(comm, BLKMPIBCAST, 
			 lam_mkerr(MPI_ERR_COMM, err)));

    /* All done */
    
    LAM_TRACE(lam_tr_cffend(BLKMPIBCAST, root, comm, datatype, count));
    lam_resetfunc(BLKMPIBCAST);
    return MPI_SUCCESS;
}
