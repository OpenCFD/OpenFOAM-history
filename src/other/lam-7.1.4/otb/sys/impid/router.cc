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
 * $Id: router.cc,v 1.18 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- network functions for connecting up all
 *                        the IMPI clients
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>

#if LAM_CXX_NOTHROW_NEW
#include <new>
#endif
#include <iostream>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <lamdebug.h>
#include <mpisys.h>
#include <blktype.h>

using std::cerr;
using std::endl;


/*
 * Exported variables
 */
IMPI_Uint4 impi_nfinal_msgs = 0;      /* how many MPI_FINALIZE msgs recvd */
IMPI_Uint4 impi_npkfini_msgs = 0;     /* how many PK_FINI IMPI packets recvd */
IMPI_Uint4 impi_sent_pkfini_msgs = 0; /* whether we've sent our PK_FINI msgs */


/*
 * private variables
 */
static int fl_debug = 0;
static int default_timeout = 50;  /* default poll timeout, in ms */


/*
 *	do_router
 *
 *	Function:	- perform routing between local LAM and rest of IMPI
 *                      - wait for num_lamprocs FINALIZE messages, then return
 *
 */
void
do_router()
{
  struct pollfd *fds;
  IMPI_Uint4 i;
  int ret;
  int *host_fds;
  short revents;
  int timeout = default_timeout;
  char *toverride;

  /* Fake out LAM so that it thinks we're an internal library routine */

  lam_setfunc(BLKIMPIDAEMON);

  /* See if we want to override the default timeout */

  if ((toverride = getenv("LAM_IMPID_SPIN_TIMEOUT")) != NULL) {
    timeout = atoi(toverride);
    if (timeout <= 0)
      timeout = default_timeout;
  }

  /* Setup local LAM communications */

  impi_local_init();

  /* Setup host communciations; get an array of fd's back */

  host_fds = impi_host_init();

  /* Setup host queues, flow control, etc. */

  fc_init();
  host_ack_init();
  host_long_init();
  host_queue_init();
  local_ack_init();
  proc_resolver_init();

  /* Setup poll fd array with the fd's that we just got back.  We have
     nhosts - 1 fd's from impi_host_init because we don't get an fd
     for ourselves, and we should have the same number of fd's back in
     lam_fds that we sent out for IMPI_C_NHOSTS.  Make a big array of
     them all, put the lam_fds in the appropriate spot in the middle
     of the host_fds, so that they are all in host/proc order. The
     situation isn't completely nice, because if we're in C2C LAM
     mode, LAM will have multiple fd's for this host.  Hence, the
     array will be the fd's of all the other IMPI hosts plus fds of
     individual LAM procs.  */

#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
  fds = new LAM_CXX_NOTHROW_NEW_ARG struct pollfd[nhosts];
  if (fds == 0) {
    cerr << "LAM IMPI host cannot allocate fd poll structures" << endl;
    impi_bail(1);
  }
#else
  try {
    fds = new struct pollfd[nhosts];
  } catch(...) {
    cerr << "LAM IMPI host cannot allocate fd poll structures" << endl;
    impi_bail(1);
  }
#endif
  for (i = 0; i < nhosts; i++) {
    fds[i].events = POLLIN;
    fds[i].revents = (short) 0;
    if (i < my_host_rank) {
      fds[i].fd = host_fds[i];
      DBUG("Setting fd %d (1): %d\n", i, host_fds[i]);
    } else if (i > my_host_rank) {
      fds[i].fd = host_fds[i - 1];
      DBUG("Setting fd %d (2): %d (index %d)\n", i, 
	   host_fds[i - 1], i - 1);
    } else {
      fds[i].events = (short) 0;
      fds[i].fd = -1;
      DBUG("Setting fd %d (3): -1\n", i);
    }
  }
  delete[] host_fds;

  /* Do normal functions, wait for num_lamprocs FINALIZE msgs and
     (nhosts - 1) PK_FINI messages (we don't need to receive one from
     ourselves). */

  impi_nfinal_msgs = 0;
  impi_npkfini_msgs = 0;
  DBUG("*** IMPID starting router poll loop: %d fds\n", nhosts);

  while (impi_nfinal_msgs < num_lamprocs || impi_npkfini_msgs < (nhosts - 1)) {
    ret = poll(fds, nhosts, timeout);
    if (ret < 0) {
      if (errno != EINTR) {
	cerr <<"LAM IMPI client daemon had a problem with poll().  Goodbye." 
	     << endl;
	impi_bail(1);
      }
    } else if (ret > 0) {

      /* Once we return from poll, we have to check and see which fd's
         are ready for reading */

      DBUG("*** IMPID Got %d messages!  Checking fds...\n", ret);
      for (i = 0; i < nhosts; i++) {
	ret = 0;
	revents = fds[i].revents;

	DBUG("Revents for fd %d: 0x%x fd value %d \n", i, fds[i].revents, 
	     fds[i].fd);

	/* If there's a good message, call the appropriate read routine */

	if ((revents & POLLIN) && fds[i].fd != -1) {
	  DBUG("Got a message from host rank %d!!! -- fd %d\n", i, fds[i].fd);
	  ret = impi_host_recv(i, &fds[i]);
	} 

	/* If there was an error, bail */

	else if (i != my_host_rank && 
		 (revents & POLLHUP) || (revents & POLLERR)) {
	  cerr << "LAM IMPI host: host " << i 
	       << " has closed its connection" << endl;
	  ret = 1;
	} else if (revents > 0)
	  DBUG("Unknown event 0x%x on host %d\n", revents, i);

	/* Was there an error? */

	if (ret != 0) {
	  cerr <<"Error on IMPI host " << i 
	       << ".  Closing connection (good luck)." << endl;
	  close(fds[i].fd);
	  fds[i].fd = -1;
	}

	if (fds[i].fd != -1)
	  fds[i].events = (short) POLLIN;
	else
	  fds[i].events = (short) 0;
	fds[i].revents = (short) 0;
      }
    } 

    /* Now check for LAM events if we haven't received all the
       finalize messages yet :-( */

    if (impi_nfinal_msgs < num_lamprocs) {
      /* LAM side hasn't finished yet */
      if (impi_local_recv() < 0) {
	cerr <<"IMPI LAM host: Bad things appear to have happened with LAM.  Goodbye." << endl;
	impi_bail(1);
      }
      if (local_test() < 0) {
	cerr <<"IMPI LAM host: An error has occured in checking for recv's going to LAM." << endl;
	impi_bail(1);
      }
    }

    if (impi_nfinal_msgs >= num_lamprocs)
      timeout = -1;
    
    /* Check and see if we just received the last IMPI_FINALIZE
       message from the LAM clients */

    if (impi_nfinal_msgs >= num_lamprocs && !impi_sent_pkfini_msgs)
      if (impi_host_send_pkfini_msgs(fds) != 0) {
        cerr <<"IMPI LAM host: Bad things appear to have happened when trying to shut down." << endl;
        impi_bail(1);
      }
  }

  /* When we drop out, it means that we have gotten num_lamprocs
     MPI_FINALIZE messages.  But it's possible that some of the host
     sockets are still open; recall that we only closed them when we
     received an IMPI_PK_FINI packet, but only if we had already sent
     out *our* IMPI_PK_FINI's already.  */
  for (i = 0; i < nhosts; i++)
    if (fds[i].fd != -1) {
      DBUG(" Shuting down the socket for fd number %d\n", fds[i].fd );
      close(fds[i].fd);
    }

  /* Tear down other structures, free other data */

  proc_resolver_destroy();
  local_ack_destroy();
  host_queue_destroy();
  host_long_destroy();
  host_ack_destroy();
  fc_destroy();
  
  impi_local_destroy();

  delete[] fds;

  lam_resetfunc(BLKIMPIDAEMON);

  DBUG("IMPID DONE -- DROPPING OUT OF ROUTER\n");
}

#endif
