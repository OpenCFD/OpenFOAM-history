/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
 *	$Id: ifinalize.c,v 1.12 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	- finalize an IMPI job
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sfh.h>           

#include <kio.h>
#include <app_mgmt.h>
#include <events.h>
#include <rpisys.h>
#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <lamdebug.h>
#include <mpisys.h>
#include <all_hash.h>


/*
 * local variables
 */
static int              fl_debug = 0;


int
IMPI_Finalize(void)
{
  int impid_rank;

  /* IMPI-mandated barrier on MPI_COMM_WORLD */

  DBUG("*** Invoking IMPI finalize barrier\n");
  fflush(stdout);
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS)
    return (LAMERROR);
  DBUG("*** Finished with IMPI finalize barrier\n");
  fflush(stdout);

  /* Send a message to the IMPI client daemon.  It can be zero bytes,
     since we're sending a specifc nh_type. */

  DBUG("Sending IMPI_TYPE_MPI_FINALIZE message to IMPI impid\n"); 
  fflush(stdout);

  MPI_Comm_size(impid_comm, &impid_rank);
  impid_rank--;

  DBUG("Sending...\n");
  fflush(stdout);
  if (MPI_Send((void *) 0, 0, MPI_BYTE, impid_rank, IMPI_FINALIZE_TAG, 
	       impid_comm) != MPI_SUCCESS)
    return (LAMERROR);
  DBUG("IMPI_TYPE_FINALIZE message sent\n");
  fflush(stdout);

  /* Wait for the impid to ack this message -- we don't want to abort
     the impid before it has shut down in an orderly way (which is
     possible, particularly on uniprocessor machines, especially if
     someone does "mpirun -client .....; lamclean".  Yes Virginia,
     someone *will* do this! */

  MPI_Barrier(impid_comm);

  /* Now free the impid_comm intracommunicator.  It's collective, so
     we have to do this in the impid as well. */

  DBUG("Freeing impid_comm\n");
  fflush(stdout);
  if (MPI_Comm_free(&impid_comm) != MPI_SUCCESS)
    return (LAMERROR);

  /* Free the shadow comm for the mpi_comm_world that we expanded to
     include the impi hosts */

  if (LAM_IS_IMPI(MPI_COMM_WORLD)) {
    lam_rmcid(MPI_COMM_WORLD->c_shadow->c_contextid);
    free((char *) MPI_COMM_WORLD->c_shadow->c_group);
    ah_free(MPI_COMM_WORLD->c_shadow->c_keys);
    free((char *) MPI_COMM_WORLD->c_shadow);
    MPI_COMM_WORLD->c_shadow = 0;
  }

  /* Free the IMPI_LAMGIAPPE datatype */

  if (IMPI_Free_dtype() != 0)
    return (LAMERROR);

  /* It is now safe to free the intercommunicator that we made with
     MPI_Comm_spawn to launch the impid. */

  if (lam_impid_intercomm != MPI_COMM_NULL)
    lam_comm_free(lam_impid_intercomm);

  /* Free the supplemental data that we got from the impid at the
     beginning of time */

  if (lam_impi_clients != 0)
    free(lam_impi_clients);
  if (lam_impi_hosts != 0)
    free(lam_impi_hosts);
  if (lam_impi_procs != 0)
    free(lam_impi_procs);
  
  DBUG("IMPI_Finalize done\n");
  fflush(stdout);
  return 0;
}

#endif /* LAM_WANT_IMPI */

