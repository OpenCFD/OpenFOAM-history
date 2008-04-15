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
 * $Id: lamnodes.c,v 1.24 2003/02/04 17:51:30 jsquyres Exp $
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <memory.h>

#include <blktype.h>
#include <app_mgmt.h>
#include <net.h>
#include <events.h>
#include <rpisys.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <preq.h>
#include <typical.h>
#include <impi-defs.h>
#include <impi.h>
#include <impid.h>
#include <lamdebug.h>


/*
 * local functions
 */
static void setup_clients(void);
static void setup_hosts(void);
static void setup_procs(void);


/*
 * exported variables
 */
int start_rank = 0;              /* new rank of local LAM rank 0 */


/*
 * private varibles
 */
static int fl_debug = 0;             /* local run-time debug flag */



/*
 *	impi_setup_server_info
 *
 *	Function:	- send message to LAM processes in this job
 *                        with executive summary of the data recvd from
 *                        IMPI server
 *
 *	Returns:	- 0 for success, LAMERROR on failure
 */
int
impi_setup_server_info()
{
  int i, client, host;

  /* Setup hosts and clients array */
  
  setup_clients();
  setup_hosts();
  setup_procs();

  /* Determine the global hiwater and ackmark values, per IMPI 0.0
     errata */

  for (client = 0; client < nclients; client++)
    for (host = 0; host < lam_impi_clients[client].ic_nhosts; host++) {
      i = lam_impi_clients[client].ic_hostrank + host;
      impi_lam_ackmark = LAM_min(cdata[IMPI_H_ACKMARK_I][i],
				 impi_lam_ackmark);
      impi_lam_hiwater = LAM_min(cdata[IMPI_H_HIWATER_I][i],
				 impi_lam_hiwater);
    }
  
  return 0;
}


/*
 *	distrib_server_info
 *
 *	Function:	- send message to LAM processes in this job
 *                        with executive summary of the data recvd from
 *                        IMPI server
 *
 *	Returns:	- 0 for success, LAMERROR on failure
 */
int
impi_distrib_server_info(int err)
{
  int i, j, base;
  IMPI_server_info msg;

  /* Calculate the starting rank for the first local rank */

  DBUG("Calculating starting rank for this LAM\n");
  base = 0;
    
  /* Construct the message to send back */
  
  msg.nclients = nclients;
  msg.nhosts = nhosts;
  msg.nprocs = nprocs;

  if (err == 0 && cdata[IMPI_P_PID_I] != 0) {
    if (cdata[IMPI_C_NHOSTS_I] != 0 || cdata[IMPI_H_NPROCS_I] != 0)
      for (i = 0; i < my_client_rank; i++) {
	for (j = 0; j < cdata[IMPI_C_NHOSTS_I][i]; j++)
	  start_rank += cdata[IMPI_H_NPROCS_I][base + j];
	base += cdata[IMPI_C_NHOSTS_I][i];
      }
    else
      err = 1;
    DBUG("Got starting rank: %d\n", start_rank);
    msg.start_rank = start_rank;
    
    /* Calculate max packet data len as the min of the maxes sent to us */
    
    IMPI_Pk_maxdatalen = (IMPI_Uint4) ~0;
    for (i = 0; i < nclients; i++)
      IMPI_Pk_maxdatalen = LAM_min(IMPI_Pk_maxdatalen, 
				   cdata[IMPI_C_DATALEN_I][i]);
    msg.max_datalen = IMPI_Pk_maxdatalen;
    
    /* Calculate the max context ID of the minimums that were sent */
    /* Irrelevant now; IMPI just says that it can be 0 */
    
    msg.min_cid = 0;
    
    /* Calculate the min tag of the maximums that were sent */
    
    msg.max_tag = INT_MAX;
    if (cdata[IMPI_C_TAGUB_I] != 0) {
      for (i = 0; i < nclients; i++)
	msg.max_tag = (msg.max_tag < cdata[IMPI_C_TAGUB_I][i]) ? 
	  msg.max_tag : cdata[IMPI_C_TAGUB_I][i];
    } else
      err = 1;
  } else
    err = 1;

  /* Did we run into an error above, or get one passed in from the
     caller? */

  if (err)
    msg.nclients = msg.nhosts = msg.nprocs = msg.start_rank = msg.min_cid = -1;

  /* Send the message to all processes */

  for (i = 0; i < num_lamprocs; i++) {
    MPI_Send(&msg, sizeof(msg) / sizeof(int), MPI_INT, i, 
	     IMPI_SERVER_INFO_TAG, parent_intra);
    if (msg.nclients > 0) {
      MPI_Send(lam_impi_clients, sizeof(IMPI_client) / 4 * nclients, 
	       MPI_INT, i, IMPI_SERVER_INFO_TAG, parent_intra);
      MPI_Send(lam_impi_hosts, sizeof(IMPI_host) / 4 * nhosts, MPI_INT, i,
	       IMPI_SERVER_INFO_TAG, parent_intra);
      MPI_Send(lam_impi_procs, sizeof(IMPI_proc) / 4 * nprocs, MPI_INT, i,
	       IMPI_SERVER_INFO_TAG, parent_intra);
    }
    DBUG("Sent server info message to rank %d\n", i);
  }

  return 0;
}

/*
 *      setup_clients
 *
 *	Function:	- setup clients array
 */
static void
setup_clients()
{
  int base, client;

  DBUG("Setting up clients array\n");
  lam_impi_clients = malloc(sizeof(IMPI_client) * nclients);
  for (base = client = 0; client < nclients; client++) {
    DBUG("Setting up client %d\n", client);
    LAM_ZERO_ME(lam_impi_clients[client]);

    lam_impi_clients[client].ic_version = cdata[IMPI_C_VERSION_I][client];
    lam_impi_clients[client].ic_nhosts = cdata[IMPI_C_NHOSTS_I][client];
    lam_impi_clients[client].ic_hostrank = base;
    lam_impi_clients[client].ic_datalen = 
      LAM_min(cdata[IMPI_C_DATALEN_I][client], impi_lam_datalen);
    lam_impi_clients[client].ic_flag = 0;

    DBUG("Client %d: version %d, nhosts %d, hostrank %d, datalen %d\n",
	 client,
	 lam_impi_clients[client].ic_version,
	 lam_impi_clients[client].ic_nhosts,
	 lam_impi_clients[client].ic_hostrank,
	 lam_impi_clients[client].ic_datalen);

    base += lam_impi_clients[client].ic_nhosts;
  }
}


/*
 *      setup_hosts
 *
 *	Function:	- setup hosts array
 */
static void
setup_hosts()
{
  int base, client, host, rank;

  /* Malloc out space for the hosts array, and fill it up.  Just for
     kicks, we fill in the entry for this host as well. */

  DBUG("Setting up hosts array\n");
  lam_impi_hosts = malloc(sizeof(IMPI_host) * nhosts);
  for (rank = base = client = 0; client < nclients; client++) {
    for (host = base; 
	 host < base + lam_impi_clients[client].ic_nhosts; host++) {
      DBUG("Setting up host %d\n", host);
      LAM_ZERO_ME(lam_impi_hosts[host]);

      lam_impi_hosts[host].ih_clientrank = client;
      lam_impi_hosts[host].ih_fd = -1;
      lam_impi_hosts[host].ih_port = cdata[IMPI_H_PORT_I][host];
      lam_impi_hosts[host].ih_nprocs = cdata[IMPI_H_NPROCS_I][host];
      lam_impi_hosts[host].ih_procrank = rank;

      memcpy(&lam_impi_hosts[host].ih_ipv6[0], 
	     &cdata[IMPI_H_IPV6_I][host * 4], 
	     sizeof(lam_impi_hosts[host].ih_ipv6));
      
      DBUG("Setup host: clientrank %d, nprocs %d, procrank %d\n",
	   lam_impi_hosts[host].ih_clientrank,
	   lam_impi_hosts[host].ih_nprocs,
	   lam_impi_hosts[host].ih_procrank);

      rank += lam_impi_hosts[host].ih_nprocs;
    }      

    base += lam_impi_clients[client].ic_nhosts;
  }
}


/*
 *      setup_procs
 *
 *	Function:	- setup procs array
 */
static void
setup_procs()
{
  int base, host, proc;

  DBUG("Setting up procs array\n");
  lam_impi_procs = malloc(sizeof(IMPI_proc) * nprocs);
  for (base = host = 0; host < nhosts; host++) {
    DBUG("Host %d: have %d procs\n", host, lam_impi_hosts[host].ih_nprocs);
    for (proc = base; 
	 proc < base + lam_impi_hosts[host].ih_nprocs; proc++) {
      DBUG("Setting up proc %d\n", proc);
      LAM_ZERO_ME(lam_impi_procs[proc]);

      lam_impi_procs[proc].ip_hostrank = host;
      /* cdata is an (IMPI_Int4*), so use proc * 2 to access the right
         (IMPI_Int8) pid */
      memcpy(&lam_impi_procs[proc].ip_pid,
	     &cdata[IMPI_P_PID_I][proc * 2], 
	     sizeof(lam_impi_procs[proc].ip_pid));
      /* cdata is an (IMPI_Int4*), so use proc * 4 to access the right
         64-bit (IMPI_Int16, if it existed) host ID */
      memcpy(&lam_impi_procs[proc].ip_ipv6[0], 
	     &cdata[IMPI_P_IPV6_I][proc * 4], 
	     sizeof(lam_impi_procs[proc].ip_ipv6));

      /* Wow. */

      DBUG("Setup proc: hostrank %d, pid %d\n", 
	   lam_impi_procs[proc].ip_hostrank,
	   *((int*) (((char*) &lam_impi_procs[proc].ip_pid) + 4)));
    }
    base += lam_impi_hosts[host].ih_nprocs;
  }
}


#endif
