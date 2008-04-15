/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: di_bootagent.c,v 6.23 2003/11/15 13:17:15 jsquyres Exp $
 *
 *	Function:	- obtains link information from boot agent
 *	Accepts:	- link array (value returned)
 *			- link array size (value returned)
 */

#include <lam_config.h>

#include <netdb.h>		       /* Solaris MAXHOSTNAMELEN */
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <sfh.h>
#include <dl_inet.h>
#include <laminternal.h>
#include <terror.h>
#include <typical.h>
#include <lam_network.h>
#include <etc_misc.h>
#include <all_opt.h>

#include <lam-ssi-boot.h>


/*
 * External variables.  
 *
 */
extern OPT *lam_daemon_optd;


/*
 *	di_bootagent
 *
 *	Function:	- obtains link information from boot agent
 *	Accepts:	- link array (value returned)
 *			- num CPU array (value returned)
 *			- link array size (value returned)
 *	Returns:	- datalink input socket
 */
int
di_bootagent(struct dilink **pdilinks, struct diinfo **pdiinfo, int4 *pndil, 
	     int4 *mynode, int argc, char* argv[])
{
  int4 i;
  int dli_port = 0;
  int dli_sd;
  struct lamnode *universe;
  int universe_size;

  /* Create the inter-node communication socket. */

  if ((dli_sd = sfh_sock_open_srv_inet_dgm(&dli_port)) < 0)
    lampanic("dli_inet (sfh_sock_open_srv_inet_dgm)");
  
  /* set socket to be close-on-exec */
  if (fcntl(dli_sd, F_SETFD, 1) != 0)
      lampanic("dli_inet: setting close-on-exec");

  if (sfh_sock_set_buf_size(dli_sd, SFH_INET, SO_RCVBUF,
			    sizeof(struct dlframe) * DOMAXPENDING))
    lampanic("dli_inet (sfh_sock_set_buf_size)");

  /* Open the boot SSI */

  if (lam_ssi_base_open(lam_daemon_optd) != 0 ||
      lam_ssi_boot_open(lam_daemon_optd) != 0 ||
      lam_ssi_boot_select(LAM_SSI_BOOT_LOCATION_LEAF) != 0)
    lampanic("di_bootagent: lam_ssi_boot_open failed");

  /* Ask the boot module what our node ID is */

  *mynode = lam_ssi_boot.lsba_get_nodeid(lam_daemon_optd);

  /* Call back to the booting agent and send it our info. */

  if (lam_ssi_boot.lsba_send_lamd_info(lam_daemon_optd, dli_port) != 0)
    lampanic("dli_inet (lam_ssi_boot_send_lamd_info)");

  /* Wait for the universe info broadcast back from the booting
   * agent */

  if (lam_ssi_boot.lsba_receive_universe_info(&universe, &universe_size) != 0)
    lampanic("dli_inet (lam_ssi_boot_receive_universe_info)");
  *pndil = universe_size;

  /* Close the boot SSI */

  if (lam_ssi_boot_close() != 0)
    lampanic("di_bootagent: lam_ssi_boot_close failed");

  /* Create the links array. */

  *pdilinks = (struct dilink *) malloc((unsigned) universe_size *
				       sizeof(struct dilink));
  *pdiinfo = (struct diinfo *) malloc((unsigned) *pndil * 
				      sizeof(struct diinfo));
  if (*pdilinks == NULL || *pdiinfo == NULL)
    lampanic("dli_inet (malloc)");

  /* Init all entries to invalid link. */

  for (i = 0; i < *pndil; ++i) {
    (*pdilinks)[i].dil_link = NOTLINKID;
    (*pdilinks)[i].dil_seqrecv = 0;
  }

  /* Copy the results back over from the universe that we got back
     from receive_universe_info() */

  for (i = 0; i < *pndil; ++i) {
    (*pdilinks)[i].dil_link = universe[i].lnd_nodeid;
    (*pdilinks)[i].dil_addr = universe[i].lnd_addr;
    (*pdiinfo)[i].dii_num_cpus = universe[i].lnd_ncpus;
    (*pdiinfo)[i].dii_node_type = universe[i].lnd_type;
    lamlog("Link %d: node: %d, cpus: %d, type: %d, ip: %s, port %d\n",
	   i, (*pdilinks)[i].dil_link,  (*pdiinfo)[i].dii_num_cpus,
	   (*pdiinfo)[i].dii_node_type, 
	   inet_ntoa((*pdilinks)[i].dil_addr.sin_addr),
           ntohs((*pdilinks)[i].dil_addr.sin_port));
  }
  free(universe);

  return (dli_sd);
}
