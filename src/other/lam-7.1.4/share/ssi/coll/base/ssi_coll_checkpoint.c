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
 * $Id: ssi_coll_checkpoint.c,v 1.2 2003/08/21 21:54:05 ssankara Exp $
 *
 */

#include <lam_config.h>

#include <lam-ssi.h>
#include <lam-ssi-coll.h>


/*
 * Empty checkpoint / restart base functions that coll modules can use
 * if they don't need anything special during checkpoint / restart
 * functionality.
 */
int 
lam_ssi_coll_base_empty_checkpoint(MPI_Comm comm)
{
  return MPI_SUCCESS;
}


int 
lam_ssi_coll_base_empty_continue(MPI_Comm comm)
{
  return MPI_SUCCESS;
}


int
lam_ssi_coll_base_empty_restart(MPI_Comm comm)
{
  return MPI_SUCCESS;
}


int
lam_ssi_coll_base_empty_interrupt(void)
{
  return 0;
}
