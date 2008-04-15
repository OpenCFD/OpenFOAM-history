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
 * $Id: ssi_coll_shmem_util.c,v 1.2 2004/02/19 23:31:36 adharurk Exp $
 *
 *	Function:	- lam_basic collective routines
 */

#include <unistd.h>

#include <lam_config.h>
#if LAM_WANT_PROFILE
#define LAM_PROFILELIB 1
#endif
#include <lam-ssi-coll-lam-basic-config.h>

#include <lam-ssi-coll.h>
#include <lam-ssi-coll-lam-basic.h>
#include <mpisys.h>
#include <rpisys.h>

#include <args.h>
#include <dl_inet.h>
#include <net.h>
#include <rreq.h>
#include <typical.h>
#include <t_types.h>
#include <terror.h>
#include <all_list.h>

void 
setup_local_comm_for_intercomm(MPI_Comm intercomm) {
  
  /* This may seem little bit round way but it is very simple to understand
     The goal is to create a new intracommunicator for local group of
     processes in the intercommunicator. This happens during lam_basic thread
     query. The context id is decided using allreduce operator over the intra-
     communicator hence we first merge the intercommunicator and then split the
     new communicator using MPI_Comm_split(). Processes in local and remote
     group specifies same color.
     In future, this can be simplified
  */
  /* Find out the order in which the groups will be merged and then fix 
     my color, all the process in the same group must have same color so
     that intracommunicator can be formed */
  int color;
  MPI_Comm big_intracomm;
  MPI_Comm local_comm;
  lam_ssi_coll_lam_basic_data_t *lblcd;

  if (lam_proccmp(intercomm->c_group->g_procs[0],
		  intercomm->c_rgroup->g_procs[0]) < 0) {
    color = 0;
    /* This process is in the lower group */
  } else {
    /* This process in in the higher group */
    color = 1;
  }

  /* merge */
  MPI_Intercomm_merge(intercomm, 0, &big_intracomm);
  
  /* now split the big communicator using predetermined colors */
  MPI_Comm_split(big_intracomm, color, 0, &local_comm);

  /* Now I have got my local communicator. YEYEY, I can communicate locally
     . This new local communicator has selected appropriate collective 
     module too. No need of big_intracomm now */
  MPI_Comm_free(&big_intracomm);

  lblcd = (lam_ssi_coll_lam_basic_data_t*) 
    malloc(sizeof(lam_ssi_coll_lam_basic_data_t));
  lblcd->local_comm = local_comm;
  lblcd->is_low = color;

  /* Hang this little structure on the intercomm */
  intercomm->c_ssi_coll_lam_basic_data = lblcd;
}
