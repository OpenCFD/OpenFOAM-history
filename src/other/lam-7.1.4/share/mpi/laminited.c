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
 * $Id: laminited.c,v 6.10 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- test if MPI has been initialized/finalized
 */

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/param.h>

#include <lam_config.h>
#include <mpi.h>
#include <mpisys.h>
#include <etc_misc.h>
#include <terror.h>
#include <laminternal.h>
#include <lam_network.h>

/*
 * Local variables
 */
static char host[MAXHOSTNAMELEN];
static char pid[32];

/*
 * global variables
 */
int 			lam_flinit = 0;
int 			lam_flfinal = 0;

/*
 *	lam_inited
 *
 *	Function:	- check if MPI has been initialized
 *	Returns:	- TRUE or FALSE
 */
int
lam_inited()

{
/*
 * Using _kio_ki_pid works when a process is forked.
 *
 * return(_kio.ki_pid == getpid());
 */
  return(lam_flinit);
}

/*
 *	lam_finalized
 *
 *	Function:	- check if MPI has been finalized
 *	Returns:	- TRUE or FALSE
 */
int
lam_finalized()

{
  return(lam_flfinal);
}

/*
 *	lam_initerr
 *
 *	Function:	- if MPI not initialized, abort as an error
 */
void
lam_initerr()

{
  /* If we get an error here, it's no use calling kexit() because we
     will either not be attached to the local lamd yet, or we will
     have already detached.  Hence, calling kexit() to register the
     return status is useless.  :-( */

  if (lam_flinit == 0 && lam_flfinal == 0) {
    lam_gethostname(host, MAXHOSTNAMELEN);
    LAMSetLastError(EMPINOINIT);
    snprintf(pid, 32, "%d", lam_getpid());
    show_help("ALL", "uninitialized", host, pid, NULL);
    exit(LAM_EEXIT);
  } else if (lam_flfinal == 1) {
    char *rank = getenv("LAMRANK");
    if (rank == NULL)
      rank = "(unknown)";

    lam_gethostname(host, MAXHOSTNAMELEN);
    LAMSetLastError(EMPIALREADYFINALIZE);
    snprintf(pid, 32, "%d", lam_getpid());
    show_help("ALL", "already-finalized", host, pid, rank, NULL);
    exit(LAM_EEXIT);
  }
}
