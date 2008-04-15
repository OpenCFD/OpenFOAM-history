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
 * $Id: ssi_boot_server.c,v 1.15 2004/03/06 19:52:54 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <args.h>
#include <etc_misc.h>
#include <lamdebug.h>
#include <sfh.h>
#include <lam-ssi-boot.h>


/*
 * Global variables
 */
int lam_ssi_boot_base_server_port = -1;


/*
 * Local variables
 */
static int server_fd = -1;


/*
 * Local functions
 */
static struct lamnode *do_promiscuous_accept(struct lamnode *nodes, 
					     int nnodes);
static struct lamnode *do_expected_accept(struct lamnode *nodes, int nnodes);


/*
 * Open a server socket connection
 */
int 
lam_ssi_boot_base_open_srv_connection(struct lamnode *nodes, int nnodes)
{
  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "base:server: opening server TCP socket");

  if ((server_fd = 
       sfh_sock_open_srv_inet_stm(&lam_ssi_boot_base_server_port, 
				  nnodes)) == -1) {
    show_help("boot", "server-socket-fail", NULL);
    return -1;
  }

  /* Make the socket close-on-exec */

  if (fcntl(server_fd, F_SETFD, 1) == -1) {
    show_help(NULL, "system-call-fail", "fcntl (set close-on-exec)",  NULL);
    return -1;
  }

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "base:server: opened port %d", 
	      lam_ssi_boot_base_server_port);

  return 0;
}


/*
 * Get a callback from a newly-invoked lamd
 */
int 
lam_ssi_boot_base_receive_lamd_info(struct lamnode *nodes, int nnodes)
{
  struct lamnode *node;
  int want_promisc;

  /* See if we want to accept promiscuous connections.  NOTE: THIS
     WILL NEED TO BE CHANGED WHEN WE DO THREADED/TREE-BASED BOOT
     ALGORITHMS! */

  want_promisc = 
    lam_ssi_base_param_lookup_int(lam_ssi_boot_base_param_promisc);

  /* Accept a connection from the new host. */

  if (want_promisc == 1)
    node = do_promiscuous_accept(nodes, nnodes);
  else
    node = do_expected_accept(nodes, nnodes);

  if (lam_ssi_boot_verbose >= 1) {
    if (node != NULL)
      lam_debug(lam_ssi_boot_did, 
		"base:server: remote lamd is at %s:%d", 
		inet_ntoa(node->lnd_addr.sin_addr), 
                ntohs(node->lnd_addr.sin_port));
    else
      lam_debug(lam_ssi_boot_did, 
		"base:server: failed to connect to remote lamd!");
  }

  /* Return 0 on success, 1 on failure */
  
  return (node == NULL);
}


/*
 * Accept from anyone, even if they're not in my list of lamnodes.
 * Rely on the boot protocol to identify them for me.
 */
static struct lamnode *
do_promiscuous_accept(struct lamnode *nodes, int nnodes)
{
  int i;
  int dlport, bootport, num_cpus;
  int boot_fd;
  struct sockaddr_in sa;
  int optlen = sizeof(sa);
  struct lamnode *node;
  int node_id;

  /* Announce the promiscuous algorithm */

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "base:server: waiting for anyone to connect");

  LAM_ZERO_ME(sa);
  while (1) {
    boot_fd = sfh_sock_accept_peer_tmout(server_fd, LAM_TO_BOOT, 
					 (struct sockaddr *) &sa, &optlen);
    if (boot_fd < 0) {
      show_help("boot", "server-side-timeout", NULL);
      return NULL;
    }
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "base:server: got connection from %s",
                inet_ntoa(sa.sin_addr));
    
    /* Read the new host port numbers: connecting IP address, TCP for
       the broadcast in send_universe_info() (below), and UDP for
       general lamd dli operations. */
    
    if (readcltcoord(boot_fd, &node_id, &bootport, &dlport) != 0 ||
        readsockint4(boot_fd, &num_cpus) != 0) {
      show_help("boot", "read-clt-coord", NULL);
      return NULL;
    }
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, 
		"base:server: client identified themselves as n%d", node_id);
    
    /* Find the IP address who just connected to me in the list of
       lamnodes that we're expecting. */
    
    for (i = 0; i < nnodes; ++i)
      if (nodes[i].lnd_nodeid == node_id)
	break;
    if (i < nnodes) {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, 
		  "base:server: this connection is expected (n%d)", node_id);
      node = &nodes[i];
      break;
    } else {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, 
		  "base:server: unexpected connection; dropping");
      close(boot_fd);
    }
  }

  node->lnd_bootport = bootport;
  node->lnd_addr.sin_port = htons((unsigned short) dlport);
  if (num_cpus != -1)
    node->lnd_ncpus = num_cpus;

  /* Close the host connection. */

  if (close(boot_fd) != 0) {
    show_help("boot", "close-socket-fail", NULL);
    return NULL;
  }

  return node;
}


/*
 * Accept from someone who is in my list of lamnodes
 */
static struct lamnode *
do_expected_accept(struct lamnode *nodes, int nnodes)
{
  int i;
  int dlport, num_cpus;
  int boot_fd;
  struct sockaddr_in sa;
  int optlen = sizeof(sa);
  struct lamnode *node;
  int node_id;

  /* Announce that this is the expected algorithm */

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, 
	      "base:server: expecting connection from finite list");

  LAM_ZERO_ME(sa);
  while (1) {
    boot_fd = sfh_sock_accept_peer_tmout(server_fd, LAM_TO_BOOT, 
					 (struct sockaddr *) &sa, &optlen);
    if (boot_fd < 0) {
      show_help("boot", "server-side-timeout", NULL);
      return NULL;
    }
    if (lam_ssi_boot_verbose >= 1)
      lam_debug(lam_ssi_boot_did, "base:server: got connection from %s",
		inet_ntoa(sa.sin_addr));
    
    /* Find the IP address who just connected to me in the list of
       lamnodes that we're expecting. */
    
    for (i = 0; i < nnodes; ++i)
      if (memcmp(&(sa.sin_addr), &(nodes[i].lnd_addr.sin_addr), 
		 sizeof(sa.sin_addr)) == 0)
	break;
    if (i < nnodes) {
      node = &nodes[i];
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, 
		  "base:server: this connection is expected (n%d)",
		  node->lnd_nodeid);
      break;
    } else {
      if (lam_ssi_boot_verbose >= 1)
	lam_debug(lam_ssi_boot_did, 
		  "base:server: unexpected connection; dropping");
      close(boot_fd);
    }
  }

  /* Read the new host port numbers: TCP for the broadcast in
     send_universe_info() (below), and UDP for general lamd dli
     operations. */

  if (readcltcoord(boot_fd, &node_id, &node->lnd_bootport, &dlport) != 0 ||
      readsockint4(boot_fd, &num_cpus) != 0) {
    show_help("boot", "read-clt-coord", NULL);
    return NULL;
  }
  node->lnd_addr.sin_port = htons((unsigned short) dlport);
  if (num_cpus != -1)
    node->lnd_ncpus = num_cpus;

  /* Close the host connection. */

  if (close(boot_fd) != 0) {
    show_help("boot", "close-socket-fail", NULL);
    return NULL;
  }

  return node;
}


/*
 * Close the server socket.
 */
int 
lam_ssi_boot_base_close_srv_connection(void)
{
  if (server_fd != -1) {
    if (lam_ssi_boot_verbose >= 5)
      lam_debug(lam_ssi_boot_did, "base:server: closing server socket");

    close(server_fd);
  }
  server_fd = -1;
  lam_ssi_boot_base_server_port = -1;

  return 0;
}


/*
 * Connect to a newly-booted lam daemon and send it information about
 * all of its peers.
 */
int 
lam_ssi_boot_base_send_universe_info(struct lamnode *nodes, int nnodes, 
				     int which)
{
  int i, ret;
  int boot_fd;
  char ip_addr[32];
  char ip_port[32];
  int fl_localname = ao_taken(lam_ssi_boot_optd, "l");
  unsigned char *ucp;

  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, 
	      "base:server: connecting to lamd at %s:%d",
	      inet_ntoa(nodes[which].lnd_addr.sin_addr),
	      nodes[which].lnd_bootport);

  boot_fd = 
    sfh_sock_open_clt_inet_stm((unsigned char *) 
			       &(nodes[which].lnd_addr.sin_addr),
			       nodes[which].lnd_bootport);
  if (boot_fd < 0) {
    snprintf(ip_addr, sizeof(ip_addr), "%s", 
	     inet_ntoa(nodes[which].lnd_addr.sin_addr));
    snprintf(ip_port, sizeof(ip_port), "%d", nodes[which].lnd_bootport);
    show_help("boot", "open-client-socket-fail", ip_addr, 
	      ip_port, NULL);
    return 1;
  }
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, "base:server: connected");

  /* Send it the number of links */

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did,
	      "base:server: sending number of links (%d)", nnodes);
  if (writesockint4(boot_fd, (int4) nnodes)) {
    show_help("boot", "socket-write-fail", NULL);
    return 1;
  }

  /* Loop sending info on all the links */

  for (i = 0; i < nnodes; ++i) {
    
    /* Suggestion/modified patch from Tim Mattox (KLAT
       group) to pass around host *names* instead of IP
       addresses.  See comment above for explanation. */
    
    if (fl_localname)
      ucp = (unsigned char*) nodes[i].lnd_hname;
    else
      ucp = (unsigned char*) &(nodes[i].lnd_addr.sin_addr);
    
    if (lam_ssi_boot_verbose >= 5) {
      if (nodes[i].lnd_nodeid != NOTNODEID)
        lam_debug(lam_ssi_boot_did, "base:server: sending info: n%d (%s)", 
                  i, nodes[i].lnd_hname);
      else
        lam_debug(lam_ssi_boot_did, 
                  "base:server: sending info: n%d (invalid node)", i);
    }
    if (nodes[i].lnd_nodeid != NOTNODEID)
      ret = writecltnbr(boot_fd,
                        i, fl_localname, ucp,
                        (int) ntohs(nodes[i].lnd_addr.sin_port),
                        nodes[i].lnd_type & (~NT_BOOT), 
                        nodes[i].lnd_ncpus);
    else
      ret = writecltnbr(boot_fd,
                        NOTLINKID, fl_localname, NULL,
                        0, 0, 0);
    if (ret != 0) {
      show_help("boot", "socket-write-fail", NULL);
      return 1;
    }
  }
  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "base:server: finished sending");

  /* Close the socket */
  
  if (close(boot_fd)) {
    show_help("boot", "close-socket-fail", NULL);
    return 1;
  }
  if (lam_ssi_boot_verbose >= 1)
    lam_debug(lam_ssi_boot_did, 
	      "base:server: disconnected from %s:%d",
	      inet_ntoa(nodes[which].lnd_addr.sin_addr),
	      nodes[which].lnd_bootport);

  /* All done */

  return 0;
}


