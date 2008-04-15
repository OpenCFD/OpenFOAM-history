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
 * $Id: ssi_boot_client.c,v 1.10 2003/12/19 15:20:25 jsquyres Exp $
 *
 */

#include <lam_config.h>

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <args.h>
#include <sfh.h>
#include <lam_network.h>
#include <etc_misc.h>
#include <lam-ssi-boot.h>


/*
 * local functions
 */
static void reply2agent(int errcode, char *abort_msg);
static void gethaddr(char *token, unsigned char *haddr);


/*
 * local variables
 */
static int boot_fd = -1;
static int boot_port = -1;
static int agent_port = -1;
static unsigned char agent_haddr[4] = { 0, 0, 0, 0 };


/*
 * Extract command line information and then send lamd information
 * back to the booting agent.
 */
int
lam_ssi_boot_base_send_lamd_info(OPT *aod, int dli_port)
{
  unsigned char agent_haddr[4] = { 0, 0, 0, 0 };

  /* Invoke the function to do the work */

  return lam_ssi_boot_base_send_lamd_info_args(aod, agent_haddr, -1, -1, 
                                               dli_port, -1);
}


/*
 * Connect back up to the invoking agent and send information about
 * the lamd that was just booted.
 */
int
lam_ssi_boot_base_send_lamd_info_args(OPT *aod, unsigned char agent_haddr[4],
                                      int agent_port, int node_id, 
                                      int dli_port, int num_cpus)
{
  int agent_fd;

  /* If we didn't get valid params, then look on the command line (in
     aod) */

  if (agent_haddr[0] == 0 && agent_haddr[1] == 0 &&
      agent_haddr[2] == 0 && agent_haddr[3] == 0) {
    if (lam_ssi_boot_verbose >= 5)
      lam_debug(lam_ssi_boot_did,
                "send_lamd: getting agent haddr from command line");
    if (ao_taken(aod, "H"))
      gethaddr(ao_param(aod, "H", 0, 0), agent_haddr);
    else {
      show_help("ssi-boot", "base-startup-proto-missing-args", NULL);
      errno = EUSAGE;
      return 1;
    }
  }

  if (agent_port == -1) {
    if (lam_ssi_boot_verbose >= 5)
      lam_debug(lam_ssi_boot_did,
                "send_lamd: getting agent port from command line");
    if (ao_taken(aod, "P")) {
      ao_intparam(aod, "P", 0, 0, &agent_port);
    } else {
      show_help("ssi-boot", "base-startup-proto-missing-args", NULL);
      errno = EUSAGE;
      return 1;
    }
  }
   
  if (node_id == -1) {
    node_id = lam_ssi_boot_base_get_nodeid(aod);
  }

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, 
              "send_lamd: connecting to %d.%d.%d.%d:%d, node id %d",
              agent_haddr[0], agent_haddr[1], agent_haddr[2], agent_haddr[3], 
              agent_port, node_id);

  if (agent_haddr[0] == '\0' || agent_port == -1 || node_id == -1) {
    show_help("ssi-boot", "base-startup-proto-missing-args", NULL);
    reply2agent(errno, 
		"ssi_boot_send_lamd_info: missing cmd line args");
  }

  /* Establish a server socket to receive link information */

  boot_port = 0;
  boot_fd = sfh_sock_open_srv_inet_stm(&boot_port, -1);
  if (boot_fd < 0)
    reply2agent(errno, 
		"ssi_boot_send_lamd_info: sfh_sock_open_srv_inet_stm failed");

  /* Connect to the boot agent (blocking) */

  agent_fd = sfh_sock_open_clt_inet_stm(agent_haddr, agent_port);
  if (agent_fd < 0)
    lampanic("ssi_boot_send_lamd_info: sfh_sock_open_clt_inet_stm failed");

  /* Send my coordinates to the boot agent. */

  if (lam_ssi_boot_verbose >= 5)
    lam_debug(lam_ssi_boot_did, "send_lamd: sending dli_port %d", dli_port);
  if (writecltcoord(agent_fd, node_id, (int4) boot_port, (int4) dli_port))
    lampanic("ssi_boot_send_lamd_info: writecltcoord failed");
  if (writesockint4(agent_fd, num_cpus) != 0)
    lampanic("ssi_boot_send_lamd_info: writesockint(num_cpus) failed");

  /* All done */

  if (close(agent_fd))
    lampanic("ssi_boot_send_lamd_info: close(agent_fd) failed");

  return 0;
}


/*
 * Wait for a callback from the booting agent, and receive the list of
 * universe information.
 */
int
lam_ssi_boot_base_receive_universe_info(struct lamnode **universe,
					int *universe_size)
{
  int i;
  int agent_fd;
  struct sockaddr_in sa;
  int optlen = sizeof(sa);
  int4 link;
  int4 port;
  int4 node_type;
  int4 ncpus;
  unsigned char haddr[4];

  /* Accept a connection from the boot agent (blocking). */
  
  agent_fd = sfh_sock_accept_peer_tmout(boot_fd, -1, 
					(struct sockaddr *) &sa, &optlen);
  if (agent_fd < 0) 
    lampanic("ssi_boot_receive_universe_info (sfh_sock_accept_peer_tmout)");

  /* Read the number of links. */

  if (readsockint4(agent_fd, universe_size))
    lampanic("ssi_boot_receive_universe_info (readsockint4)");

  /* Allocate the array */

  *universe = malloc(sizeof(struct lamnode) * *universe_size);
  if (*universe == NULL)
    lampanic("ssi_boot_receive_universe_info (malloc)");

  /* Loop over reading them all in */

  lam_comm_debug_dli_cond((1, "receiving %d univ nodes", *universe_size));
  for (i = 0; i < *universe_size; ++i) {
    if (readcltnbr(agent_fd, &link, haddr, &port, &node_type, &ncpus) != 0)
      lampanic("ssi_boot_receive_universe_info (readcltnbr)");
    (*universe)[i].lnd_nodeid = link;
    if (link != NOTNODEID) {
      (*universe)[i].lnd_type = node_type;
      (*universe)[i].lnd_ncpus = ncpus;
      sfh_sock_fill_inet_addr(haddr, (int) port, &((*universe)[i].lnd_addr));
    }
  }
  
  if (close(agent_fd) != 0 || close(boot_fd) != 0) 
    lampanic("ssi_boot_receive_universe_info (close)");
  
  return 0;
}


/*
 *	gethaddr
 *
 *	Function:	- parses Internet address from string
 *			- can be a host name or a dotted-decimal number.
 *	Accepts:	- string
 *			- ptr Internet address
 */
static void
gethaddr(char *token, unsigned char *haddr)
{
  unsigned int a0, a1, a2, a3;

  /* Assume the address is given in dotted-decimal format. If not, try
     it as an alpha-numeric hostname. */

  if (sscanf(token, "%u.%u.%u.%u", &a0, &a1, &a2, &a3) == 4) {
    haddr[0] = a0;
    haddr[1] = a1;
    haddr[2] = a2;
    haddr[3] = a3;
  }

  /* Address is in hostname format. */

  else {
    if (getinetaddr(token, haddr))
      lampanic("ssi_boot_send_lamd_info: getinetaddr failed");
  }
}


/*
 *	reply2agent
 *
 *	Function:	- sends reply error code to boot agent
 *	Accepts:	- error code
 */
static void
reply2agent(int errcode, char *abort_msg)
{
  int agent_fd;			/* connection to agent */

  /* Connect to the boot agent (blocking).*/

  agent_fd = sfh_sock_open_clt_inet_stm(agent_haddr, agent_port);
  if (agent_fd < 0)
    lampanic("ssi_boot_send_lamd_info: sfh_sock_open_clt_inet_stm failed");

  if (writesockint4(agent_fd, (int4) errcode))
    lampanic("ssi_boot_send_lamd_info: writesockint4 failed)");

  if (close(agent_fd))
    lampanic("ssi_boot_send_lamd_info: close failed");

  /* Mad race to kill me ! */

  lampanic(abort_msg);
}
