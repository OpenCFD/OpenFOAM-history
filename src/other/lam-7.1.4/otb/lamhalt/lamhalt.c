/*
 * Copyright (c) 2001-2005 The Trustees of Indiana University.  
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
 *	Function:	- kill an entire running LAM run time system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <lam_config.h>
#include <args.h>
#include <etc_misc.h>
#include <dl_inet.h>
#include <portable.h>
#include <net.h>
#include <events.h>
#include <priority.h>
#include <hreq.h>
#include <laminternal.h>
#include <etc_misc.h>
#include <lam_network.h>


/*
 * Local variables
 */
static int lamhalt_timeout = 15;


int
main(int argc, char **argv)
{
  int fl_debug = 0;
  int fl_verbose = 0;
  int fl_wait = 1;
  int localnode; 
  int *nodes;
  int i, save, nnodes;
  
  validopts("Hhvdi");
  if (do_args(&argc, argv)) {
    show_help("lamhalt", "usage", NULL);
    exit(errno);
  }
  
  if (opt_taken('h')) {
    show_help("lamhalt", "usage", NULL);
    exit(0);
  }

  /* Get flags  */

  fl_debug = opt_taken('d');
  fl_verbose = opt_taken('v') || fl_debug;
  if (opt_taken('i')) {
      fl_wait = 0;
  }
  
  /* Announce */

  if (!opt_taken('H'))
    lam_show_version(0);

  /* Attach to the local LAM daemon */

  if (kinit(PRLAMHALT)) {
    char hostname[MAXHOSTNAMELEN];
    gethostname(hostname, MAXHOSTNAMELEN);

    show_help(NULL, "no-lamd", "lamhalt", hostname, NULL);
    exit(LAM_EEXIT);
  }

  if (fl_verbose)
    printf("Shutting down LAM\n");

  /* Build array of node ID's, not including the localnode */

  localnode = getnodeid();
  nnodes = getnall();
  nodes = malloc(sizeof(int) * nnodes);
  if (nodes == NULL) {
    show_help(NULL, "system-call-fail", "malloc", NULL);
    kexit(1);
  }

  if (getnodes(nodes, nnodes, NT_CAST, 0) != 0) {
    show_help(NULL, "library-call-fail", "getnodes", NULL);
    kexit(1);
  }

  /* Now strip out the localnode */

  for (save = i = 0; i < nnodes; ++i)
    if (nodes[i] != localnode && nodes[i] != NOTNODEID)
      nodes[save++] = nodes[i];
  nnodes = save;

  /* Tell them all to die */

  rhreq(nodes, nnodes, lamhalt_timeout, 
	(fl_debug ? 1 : (fl_verbose ? 0 : -1)));

  /* Send one more message, telling local LAM daemon that it's a good
     day to die.  Do the localnode last so that it can be used to
     send/receive all the previous messages without dying.  */

  rhreq(&localnode, 1, lamhalt_timeout,
	(fl_debug ? 1 : (fl_verbose ? 0 : -1)));

  /* Sending these messages do *not* immediately cause the lamds to
     quit; they take 2-4 seconds to quit.  So unless the user invoked
     "-i" (immediate), have lamhalt sleep for 4 seconds before
     returning to give everyone a chance to die. */

  if (fl_wait) {
      int err;
      struct stat buf;
      char *sock = lam_get_sockname();
      struct timeval tv;

      if (fl_verbose) {
          printf("lamhalt: sleeping to wait for lamds to die\n");
      }

      /* Wait for the socket to disappear. */
      if (NULL != sock) {
          tv.tv_sec = 0;
          tv.tv_usec = 5;
          while (NULL != sock) {
              err = stat(sock, &buf);
              if (err != 0) {
                  break;
              }
              /* wait a little bit */
              err = select(0, NULL, NULL, NULL, &tv);
              if (err != 0) {
                  break;
              }
          }
      }
  } else {
      if (fl_verbose) {
          printf("lamhalt: exiting moments before lamds have died\n");
      }
  }

  /* Do not attempt to receive final ACK message */

  if (fl_debug)
    printf("lamhalt: local LAM daemon halted\n");
  if (fl_verbose)
    printf("LAM halted\n");

  /* That's all she wrote */

  return 0;
}
