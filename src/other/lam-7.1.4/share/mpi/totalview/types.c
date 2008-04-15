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
 *	Function:	- load type definitions used in TotalView DLL.
 */

#include <lam_config.h>

#include <stdio.h>

#include <all_list.h>
#include <app_mgmt.h>
#include <rpisys.h>
#include <mpisys.h>
#include <lam-ssi-rpi.h>

/* We need to load the type definitions in TotalView of the structures
   we use in DLL.  To do that we must declare variables of those types
   in a file and compile that file with -g.  These variables are
   declared volatile so that compiler does not optimize them out.*/

volatile struct _req *lam_dummy_req = NULL;
volatile struct _comm *lam_dummy_comm = NULL;
volatile struct _group *lam_dummy_group = NULL;
volatile struct _proc *lam_dummy_proc = NULL;
volatile struct _gps *lam_dummy_gps = NULL;
volatile struct ah_desc *lam_dummy_ah_desc = NULL;
volatile struct al_desc *lam_dummy_al_desc = NULL;
volatile struct al_head *lam_dummy_al_head = NULL;
volatile struct lam_ssi_rpi_cbuf_msg *lam_dummy_msg = NULL;
volatile struct lam_ssi_rpi_cbuf_cid *lam_dummy_cid = NULL;
volatile struct lam_ssi_rpi_envl *lam_dummy_envl = NULL;


void * 
lam_tv_load_type_defs(void)
{
  static void *dummy[11];
  
  /* Referencing the above variables needed for loading type
     definitions in TotalView so that compiler does not optimize them
     out. */
  
  dummy[0] = &lam_dummy_req; 
  dummy[1] = &lam_dummy_comm;
  dummy[2] = &lam_dummy_group; 
  dummy[3] = &lam_dummy_proc; 
  dummy[4] = &lam_dummy_gps;
  dummy[5] = &lam_dummy_ah_desc; 
  dummy[6] = &lam_dummy_al_desc; 
  dummy[7] = &lam_dummy_al_head; 
  dummy[8] = &lam_dummy_msg; 
  dummy[9] = &lam_dummy_cid;
  dummy[10] = &lam_dummy_envl;
  
  return dummy;
}
