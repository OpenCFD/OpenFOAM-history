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
 * $Id: lamshrink.c,v 6.19 2003/02/27 03:01:11 jsquyres Exp $
 * 
 *	Function:	- removes a node from an existing LAM session
 */

#include <lam_config.h>

#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <netdb.h>

#include <args.h>
#include <hreq.h>
#include <etc_misc.h>
#include <priority.h>
#include <lamdebug.h>
#include <preq.h>
#include <dl_inet.h>
#include <events.h>
#include <hreq.h>


/*
 * local variables
 */
static int fl_verbose;
static int fl_debug;
static const int lamshrink_timeout = 15;


/*
 * main
 */
int
main(int argc, char *argv[])
{
  int delay;			/* delay in seconds */
  int flags;			/* node flags */
  int index;			/* node index */
  int4 i;
  int4 nodetarget;		/* node to be removed */
  int4 nnodes;			/* # neighbour nodes */
  int4 *pnodes;			/* neighbour nodes */
  struct direq *pdiq;		/* dli request */
  struct direply *pdir;		/* dli reply */
  struct nmsg nh;

  /* Ensure that we are not root */

#ifndef LAM_ALLOW_RUN_AS_ROOT
  if (getuid() == 0 || geteuid() == 0) {
    show_help(NULL, "deny-root", NULL);
    exit(EACCES);
  }
#endif

  validopts("dhv");
  followed("w");

  if (do_args(&argc, argv)) {
    show_help("lamshrink", "usage", NULL);
    exit(errno);
  }

  if (opt_taken('h')) {
    show_help("lamshrink", "usage", NULL);
    exit(0);
  }

  /* Set the flags. */

  fl_debug = opt_taken('d');
  fl_verbose = opt_taken('v');

  /* Initialize LAM.*/

  if (kinit(PRCMD)) {
    char hostname[MAXHOSTNAMELEN];
    gethostname(hostname, MAXHOSTNAMELEN);

    show_help(NULL, "no-lamd", "lamshrink", hostname, NULL);
    exit(errno);
  }

  if (nid_parse(&argc, argv) != 0 || ((errno = (argc == 1) ? 0 : EUSAGE))) {
    show_help("lamshrink", "usage", NULL);
    kexit(errno);
  }

  /* Determine the target node.  Allow only one node. */

  nid_get(&index, &nodetarget, &flags);
  if (index < 0) {
    show_help("lamshrink", "usage", NULL);
    kexit(EUSAGE);
  }

  nid_get(&index, &nodetarget, &flags);
  if (index != 0) {
    show_help("lamshrink", "usage", NULL);
    kexit(EUSAGE);
  }

  /* Obtain all IDs. */

  nnodes = getnall();
  if (nnodes < 0)
    lamfail("lamshrink (getnall)");

  pnodes = (int4 *) malloc((unsigned) nnodes * sizeof(int4));
  if (pnodes == 0)
    lamfail("lamshrink (malloc)");

  if (getall(pnodes, nnodes))
    lamfail("lamshrink (getall)");

  /* Send a warning signal to all application processes on the target
     node. */

  if (opt_taken('w')) {
    intparam('w', &delay);
    VERBOSE("signal doomed node ...\n");

    if (rpdoom(nodetarget, SELECT_APPL, 0, SIGFUSE))
      lamfail("lamshrink (rpdoom)");

    VERBOSE("pause ...\n");
    sleep(delay);
  }

  /* Kill the target LAM daemon */

  i = nodetarget;
  rhreq(&i, 1, lamshrink_timeout, 
	(fl_debug ? 1 : (fl_verbose ? 0 : -1)));

  /* Invoke a node remove procedure on all nodes other than the node
     being removed.  */

  LAM_ZERO_ME(nh);
  pdiq = (struct direq *) nh.nh_data;
  pdir = (struct direply *) nh.nh_data;
  nh.nh_flags = NOBUF;
  nh.nh_msg = 0;

  if (fl_verbose)
    nodespin_init("update");

  for (i = 0; i < nnodes; ++i) {
    if (pnodes[i] == nodetarget)
      continue;

    if (fl_verbose)
      nodespin_next(i);

    pdiq->diq_req = DIQREMLINK;
    pdiq->diq_src_node = (getnodeid() == nodetarget) ? NOTNODEID :
      getnodeid();
    pdiq->diq_src_event = -lam_getpid();
    pdiq->diq_link = nodetarget;
    nh.nh_node = pnodes[i];
    nh.nh_event = EVDLI;
    nh.nh_type = 0;
    nh.nh_length = 0;

    if (getroute(&nh))
      lamfail("lamshrink (getroute)");

    /* Re-route the local node. */

    if (nh.nh_dl_event == nh.nh_event) {
      nh.nh_dl_event = EVDL0;
      nh.nh_dl_link = getnodeid();
    }

    if (dsend(&nh))
      lamfail("lamshrink (dsend)");

    if (getnodeid() != nodetarget) {
      nh.nh_event = -lam_getpid();
      nh.nh_length = 0;

      if (drecv(&nh))
	lamfail("lamshrink (nrecv)");

      if (pdir->dir_reply != 0) {
	errno = pdir->dir_reply;
	lamfail("lamshrink (DIQREMLINK)");
      }
    }
  }

  if (fl_verbose)
    nodespin_end();

  /* All done */

  free((char *) pnodes);
  return (0);
}
