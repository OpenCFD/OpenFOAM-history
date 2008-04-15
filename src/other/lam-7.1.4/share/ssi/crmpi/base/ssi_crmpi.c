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
 * $Id: ssi_crmpi.c,v 1.6 2004/03/02 03:43:44 jsquyres Exp $
 *
 */

#include <lam-ssi-cr.h>
#include <lam-ssi-rpi.h>


int 
lam_ssi_crmpi_base_checkpoint(void)
{
  MPI_Comm *comm;

  /* Just turn around and call the appropriate checkpoint functions in
     all the other modules that are checkpoint-aware. Right now, those
     are rpi and coll.  We don't have to check and see if the modules
     are checkpointable -- we wouldn't be here if they weren't. */

  if (RPI_CHECKPOINT() < 0)
    return LAMERROR;

  for (comm = (MPI_Comm *) al_top(lam_comms); comm != NULL; 
       comm = al_next(lam_comms, comm))
    if ((*comm)->c_ssi_coll.lsca_checkpoint(*comm) < 0)
      return LAMERROR;

  return 0;
}


int
lam_ssi_crmpi_base_continue(void)
{
  MPI_Comm *comm;

  /* Just turn around and call the appropriate checkpoint functions in
     all the other modules that are checkpoint-aware. Right now, those
     are rpi and coll.  We don't have to check and see if the modules
     are checkpointable -- we wouldn't be here if they weren't. */

  if (RPI_CONTINUE() < 0)
    return LAMERROR;

  for (comm = (MPI_Comm *) al_top(lam_comms); comm != NULL; 
       comm = al_next(lam_comms, comm))
    if ((*comm)->c_ssi_coll.lsca_continue(*comm) < 0)
      return LAMERROR;

  return 0;
}


int
lam_ssi_crmpi_base_restart(void)
{
  MPI_Comm *comm;

  /* First up, reopen all debug streams. */

  lam_debug_reopen_all();

  /* Just turn around and call the appropriate checkpoint functions in
     all the other modules that are checkpoint-aware. Right now, those
     are rpi and coll.  We don't have to check and see if the modules
     are checkpointable -- we wouldn't be here if they weren't. */

  if (RPI_RESTART() < 0)
    return LAMERROR;

  for (comm = (MPI_Comm *) al_top(lam_comms); comm != NULL; 
       comm = al_next(lam_comms, comm))
    if ((*comm)->c_ssi_coll.lsca_restart(*comm) < 0)
      return LAMERROR;

  return 0;
}

