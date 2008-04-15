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
 *	$Id: ihelpers.c,v 1.6 2002/10/09 20:57:17 brbarret Exp $
 *
 *	Function:	- utility functions for the IMPI collectives
 *                      - NOTE: some of these functions lack
 *                        effenciency in order to be clear ('cause this
 *                        is confusing stuff).  Since we're talking
 *                        about computations that are so small, the loss
 *                        of effenciency was guaged to be irrelevant.
 *                      - the data structures are very organized, but 
 *                        hellishly confusing.  Not for the meek.
 *                      - the majority of these functions are
 *                        described in section 4.2 of the IMPI standard.
 */


#include <lam_config.h>

#if LAM_WANT_IMPI

#include <stdio.h>
#include <string.h>

#include <app_mgmt.h>
#include <rpisys.h>
#include <mpisys.h>
#include <impi-defs.h> /* This file supplied by IMPI server package */
#include <impi.h>
#include <lamdebug.h>
#include <mpi.h>


/* 
 * private functions
 */

static void clear_flags(void);

/*
 *	IMPI_Is_master()
 *
 *	Function:	- determine if rank is a master process
 *	Accepts:	- rank
 *                      - IMPI communicator
 *	Returns:	- true or false
 */
int 
IMPI_Is_master(int rank, MPI_Comm comm)
{
  if (IMPI_Master_num(rank, comm) == -1)
    return FALSE;
  else
    return TRUE;
}

/*
 *	IMPI_Num_masters()
 *
 *	Function:	- return number of master processes in the comm
 *	Accepts:	- IMPI communicator
 *	Returns:	- number of master processes
 */
int
IMPI_Num_masters(MPI_Comm comm)
{
  int  i, j, comm_size; 

  int rank;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &comm_size);
  
  /* Find the clients that are in this comm and mark their flag */

  clear_flags();
  for (i = 0; i < comm_size; i++)
    lam_impi_clients[lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[i]->p_gps.gps_grank].ip_hostrank].ih_clientrank].ic_flag = 1;

  for (j = i = 0; i < impi_server_info.nclients; i++)
    if (lam_impi_clients[i].ic_flag == 1)
      j++;

  return j;
}

/*
 *	IMPI_Master_rank()
 *
 *	Function:	- return rank in comm of master number master_num
 *	Accepts:	- master number
 *                      - IMPI communicator
 *	Returns:	- rank in communicator
 */
int 
IMPI_Master_rank(int master_num, MPI_Comm comm)
{
  int  i, j, comm_size; 

  MPI_Comm_size(comm, &comm_size);

  /* Find the clients that are in this comm and mark their flag */

  clear_flags();
  for (i = 0; i < comm_size; i++) 
    lam_impi_clients[lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[i]->p_gps.gps_grank].ip_hostrank].ih_clientrank].ic_flag = 1;

  for (j = i = 0; i < impi_server_info.nclients && j < master_num; i++)
    if (lam_impi_clients[i].ic_flag == 1)
      j++;
  
  for (i = 0; i < comm_size; i++) 
    if (lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[i]->p_gps.gps_grank].ip_hostrank].ih_clientrank == j)
      return i;
  
  return -1;
}

/*
 *	IMPI_Master_num()
 *
 *	Function:	- if process rank in comm is a master process, 
 *                        return its master number, else return -1
 *	Accepts:	- rank
 *                      - IMPI communicator
 *	Returns:	- master number or -1
 */
int
IMPI_Master_num(int rank, MPI_Comm comm)
{
  int i;
  
  for (i = 0; i < impi_server_info.nclients; i++)
    if (IMPI_Master_rank(i, comm) == rank)
      return i;

  return -1;
}

/*
 *	IMPI_Are_local()
 *
 *	Function:	- determine if rank1 and rank2 are local to each 
 *                        other in comm
 *	Accepts:	- rank1
 *                      - rank2
 *                      - IMPI communicator
 *	Returns:	- true or false
 */
int 
IMPI_Are_local(int rank1, int rank2, MPI_Comm comm)
{
  if (IMPI_Local_master_rank(rank1, comm) ==
      IMPI_Local_master_rank(rank2, comm))
    return TRUE;
  
  return FALSE;
}

/*
 *	IMPI_Local_master_rank()
 *
 *	Function:	- returns rank in comm of the master process local 
 *                        to process rank 
 *	Accepts:	- rank
 *                      - IMPI communicator
 *	Returns:	- rank of local master process
 */
int 
IMPI_Local_master_rank(int rank, MPI_Comm comm)
{
  int i;

  /* First, get my client number */

  int world_rank = comm->c_group->g_procs[rank]->p_gps.gps_grank;
  int my_client = lam_impi_hosts[lam_impi_procs[world_rank].ip_hostrank].ih_clientrank;

  /* Now traverse all the ranks, and look for the first rank with the
     same client number as me (which might be me!). */

  for (i = 0; i < comm->c_group->g_nprocs; i++) {
    world_rank = comm->c_group->g_procs[i]->p_gps.gps_grank;
    if (lam_impi_hosts[lam_impi_procs[world_rank].ip_hostrank].ih_clientrank ==
	my_client)
      return i;
  }

  return LAMERROR;
}

/*
 *	IMPI_Local_master_num()
 *
 *	Function:	- returns the master number of the master process 
 *                        local to process rank in comm
 *	Accepts:	- rank
 *                      - IMPI communicator
 *	Returns:	- master number local to rank
 */
int 
IMPI_Local_master_num(int rank, MPI_Comm comm)
{
  int i, num = -1;

  /* Iterate from 0 to the target rank.  When we find a new master (by
     checking the ic_flag), increment num. */

  clear_flags();
  for (i = 0; i <= rank; i++)
    if (lam_impi_clients[lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[i]->p_gps.gps_grank].ip_hostrank].ih_clientrank].ic_flag == 0) {
      num++;
      lam_impi_clients[lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[i]->p_gps.gps_grank].ip_hostrank].ih_clientrank].ic_flag = 1;
    }

  return num;
}

/*
 *	IMPI_Num_local_to_master()
 *
 *	Function:	- returns number of processes in comm local to
 *                        master process number master_num
 *	Accepts:	- master number
 *                      - IMPI communicator 
 *	Returns:	- number of procceses local to that master
 */
int 
IMPI_Num_local_to_master(int master_num, MPI_Comm comm)
{
  int host, num = 0;
  int host_startrank;
  int master_rank = IMPI_Master_rank(master_num, comm);
  IMPI_client *client = &lam_impi_clients[lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[master_rank]->p_gps.gps_grank].ip_hostrank].ih_clientrank];

  host_startrank = client->ic_hostrank;
  for (host = 0; host < client->ic_nhosts; host++)
    num += lam_impi_hosts[host + host_startrank].ih_nprocs;

  return num;
}

/*
 *	IMPI_Num_local_to_rank()
 *
 *	Function:	- returns number of processes in comm local to rank
 *	Accepts:	- rank
 *                      - IMPI communicator 
 *	Returns:	- number of procceses local to that rank
 */
int
IMPI_Num_local_to_rank(int rank, MPI_Comm comm)
{
  int host, num = 0;
  int host_startrank;
  IMPI_client *client = &lam_impi_clients[lam_impi_hosts[lam_impi_procs[comm->c_group->g_procs[rank]->p_gps.gps_grank].ip_hostrank].ih_clientrank];

  host_startrank = client->ic_hostrank;
  for (host = 0; host < client->ic_nhosts; host++)
    num += lam_impi_hosts[host + host_startrank].ih_nprocs;

  return num;
}


/*
 *	IMPI_Rank_impi2shadow()
 *
 *	Function:	- convert a rank in an IMPI communicator to its 
 *                        corresponding rank in the shadow communicator
 *	Accepts:	- global rank
 *                      - IMPI communicator
 *	Returns:	- rank in shadow communicator, or LAMERROR if rank 
 *                        is not in this shadow communicator (i.e., it is
 *                        not local to this host)
 */
int 
IMPI_Rank_impi2shadow(int rank, MPI_Comm comm)
{
  int i;
  struct _gps *pgps = &comm->c_group->g_procs[rank]->p_gps;
  int nprocs = comm->c_shadow->c_group->g_nprocs;
  MPI_Group group = comm->c_shadow->c_group;
  
  for (i = 0; i < nprocs; i++)
    if (LAM_FULL_GPSCMP(pgps, &(group->g_procs[i]->p_gps)) == 0)
      return i;

  return LAMERROR;
}


/*
 *	IMPI_Rank_shadow2impi()
 *
 *	Function:	- convert a rank in an IMPI shadow communicator to its 
 *                        corresponding rank in the main communicator
 *	Accepts:	- shadow rank
 *                      - IMPI communicator
 *	Returns:	- rank in main communicator
 */
int 
IMPI_Rank_shadow2impi(int rank, MPI_Comm comm)
{
  int i;
  struct _gps *pgps = &comm->c_shadow->c_group->g_procs[rank]->p_gps;
  int nprocs = comm->c_group->g_nprocs;
  MPI_Group group = comm->c_group;
  
  for (i = 0; i < nprocs; i++)
    if (LAM_FULL_GPSCMP(pgps, &(group->g_procs[i]->p_gps)) == 0)
      return i;

  return LAMERROR;
}


/*
 *	clear_flags()
 *
 *	Function:	- 
 *	Accepts:	- 
 *	Returns:	- void 
 */
static void
clear_flags(void)
{
  int i;

  for (i = 0; i < impi_server_info.nclients; i++) 
    lam_impi_clients[i].ic_flag = 0;
}


#endif
