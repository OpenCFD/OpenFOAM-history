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
 *	Function:	- activate the haltd on a set of nodes
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <hreq.h>
#include <typical.h>
#include <portable.h>
#include <sfh.h>
#include <lam_network.h>
#include <events.h>
#include <etc_misc.h>
#include <terror.h>
#include <dl_inet.h>


/*
 * Local variables
 */
static int4 nlinks = 0;
static struct dolink *links = NULL;


/*
 * Local functions
 */
static char *resolve(int nodeid);


/*
 * Activate the haltd on a set of nodes.
 *
 * Return number of nodes that were actually killed, or LAMERROR if
 * nothing happened.
 */
int
rhreq(int *nodes, int nnodes, int timeout, int verbose)
{
  struct nmsg sendmsg, recvmsg;
  struct hreq *request = (struct hreq*) sendmsg.nh_data;
  int i;
  int received;
  time_t end;
  int nresolves = 0;
  char **resolves = NULL;

  /* Setup the halt ping messages.  The first message just pings the
     remote daemon (to which it will respond with an ACK).  The second
     message will have the lamd actually kill itself.  Need to break
     this into two parts so that the dying LAM daemon will be
     guaranteed to be able to send us the ACK.  */

  LAM_ZERO_ME(sendmsg);
  sendmsg.nh_event = EVHALTD;
  sendmsg.nh_type = LAM_HALT_PING;
  sendmsg.nh_length = 0;
  sendmsg.nh_flags = 0;
  sendmsg.nh_msg = NULL;
  request->hq_node = getnodeid();
  request->hq_event = (-lam_getpid()) & 0xBFFFFFFF;
  request->hq_pid = lam_getpid();

  /* Setup the resolver */

  if (ldogetlinks(&links, &nlinks))
    lamfail("hreq (ldogetlinks)");

  /* Resolve all node IDs to string hostnames / IP addresses */

  for (i = 0; i < nnodes; ++i)
    sfh_argv_add(&nresolves, &resolves, resolve(nodes[i]));

  /* Send shutdown notices to all LAM daemons in the array */

  for (i = 0; i < nnodes; ++i) {
    sendmsg.nh_node = nodes[i];
    request->hq_index = i;
    if (verbose >= 1)
      printf("hreq: sending HALT_PING to n%d (%s)\n", nodes[i], resolves[i]);
    if (nsend(&sendmsg))
      lamfail("rhreq (nsend)");
  }
    
  /* Wait for ACKs.  Once we get an ACK, send the actual death message */

  LAM_ZERO_ME(recvmsg);
  recvmsg.nh_event = request->hq_event;
  request = (struct hreq *) recvmsg.nh_data;
  sendmsg.nh_type = LAM_HALT_DIE;
  if (verbose >= 0 && nnodes > 1)
    printf("hreq: waiting for HALT ACKs from remote LAM daemons\n");

  /* Wait for ACKs.  If we go timeout seconds without getting an ACK,
     print an error message. */

  for (i = 0; i < nnodes; ++i) {
    recvmsg.nh_type = 0;
    recvmsg.nh_length = 0;
    recvmsg.nh_flags = 0;

    /* Wait only timeout seconds between receipt of ACKs.  The current
       LAM infrastructure currently only allows this by spinning.  Oh
       well. */

    received = 0;
    end = time(NULL) + timeout;
    while (time(NULL) < end) {
      if (ntry_recv(&recvmsg) == 0) {
	received = 1;
	break;
      }
    }

    /* Print pretty debug messages */

    if (verbose >= 0) {
      if (received == 0)
	printf("hreq: timeout without receiving enough ACKs\n");
      else
	printf("hreq: received HALT_ACK from n%d (%s)\n", 
	       request->hq_node, resolves[request->hq_index]);
    }

    /* If we didn't get an ACK in the timeout, bust out */

    if (received == 0)
      break;

    /* Record that we got this node's ACK, and send back the final
       "please kill yourself now" message. */

    if (verbose >= 1)
      printf("hreq: sending HALT_DIE to n%d (%s)\n", 
	     nodes[request->hq_index], resolves[request->hq_index]);
    sendmsg.nh_node = request->hq_node;
    nodes[request->hq_index] = -(nodes[request->hq_index] + 1);
    if (nsend(&sendmsg))
      lamfail("hreq (nsend)");
  }

  /* Did we get ACKs from all the nodes? */

  if (i < nnodes) {
    char *str;
    char dummy[1024];
    int my_argc = 0;
    char **my_argv = NULL;

    for (i = 0; i < nnodes; ++i) {
      if (nodes[i] >= 0) {
	snprintf(dummy, 1024, "    %s (n%d)", resolves[i], i);
	sfh_argv_add(&my_argc, &my_argv, dummy);
      }
    }
    str = sfh_argv_glue(my_argv, '\n', 0);
    show_help("rhreq", "timeout", str, NULL);
    free(str);
    sfh_argv_free(my_argv);
  }

  /* All done */

  sfh_argv_free(resolves);
  free(links);
  return i;
}


/*
 * Resolve a LAM nX designation into a string hostname or IP address
 */
static char *
resolve(int nodeid)
{
  struct hostent *hent;
  static char name[1024];

  hent = lam_gethostbyaddr((char *)&links[nodeid].dol_addr.sin_addr,
			   sizeof(struct in_addr), AF_INET);
  if (hent)
    lam_strncpy(name, hent->h_name, sizeof(name));
  else
    sprintf(name, "%s", inet_ntoa(links[nodeid].dol_addr.sin_addr));

  return name;
}
