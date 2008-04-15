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
 *	$Id: impid.c,v 1.28 2003/02/12 23:27:38 jsquyres Exp $
 *
 *      Function:	- IMIPD is to call the startup to the IMPI server and
 *                        handle communication from IMPI world and LAM world
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <all_list.h>
#include <terror.h>
#include <typical.h>
#include <rpisys.h>
#include <mpisys.h>
#include <sfh.h>
#include <lamdebug.h>
#include <mpi.h>
#include <impi-defs.h>
#include <impi.h>
#include <impid.h>
#include <args.h>
#include <etc_misc.h>


/*
 * exported variables
 */
IMPI_Uint4 num_lamprocs = 0;  /* number of LAM procs */
int my_client_rank = -1;      /* our IMPI client rank */
int impi_lam_datalen = LAM_TCPSHORTMSGLEN; /* data packet len for LAM */
int impi_lam_hiwater = 20;    /* hiwater mark for LAM/this is a guess*/
int impi_lam_ackmark = 10;    /* ackmark for LAM/this is a guess */
int impi_lam_coll_xsize = -1; /* gets the default value of 1024 */
int impi_lam_coll_maxlinear = -1;
MPI_Comm parent_intra;


/*
 * private variables
 */
static int fl_debug = 0;             /* local run-time debug flag */
static int server_port = -1;         /* port where IMPI server listens */
static IMPI_Uint4 my_ipaddress = 0;  /* IP address of this machine */
static int my_dance_port = -1;       /* port we listen on for other clients */
static int my_dance_fd = -1;         /* socket fd to listen for IMPI clients */
static char *server_name = 0;        /* IP name of IMPI server */
static int need_distrib_info = 1;    /* mainly for abnormal exits */


/*
 * local functions
 */
static void parse_args(int argc, char* argv[]);



/*
 * main
 */
int
main(int argc, char *argv[])
{
  MPI_Comm       parent_inter;
  extern char *lam_ssi_coll_base_override;

  /* Parse the command line arguments */

  parse_args(argc, argv);
  DBUG("LAM IMPI client starting: client rank %d, server %s, port %d\n",
       my_client_rank, server_name, server_port);

  lam_ssi_coll_base_override = "lam_basic";
  MPI_Init(&argc, &argv);
  DBUG("Finished MPI_Init in impid\n");

  /* Break in the above-the-MPI-layer abstraction here, but it helps
     in terms of error reporting and whatnot. */

  gimpid = lam_myproc->p_gps;

  /* Get the whole intercomm that has the origional lam nodes and the
     impid. */

  MPI_Comm_get_parent(&parent_inter);

  /* Merge into an intracomm, and free the original
     intercommunicator. */

  MPI_Intercomm_merge(parent_inter, 1, &parent_intra);
#if 0
  /* Can't do this because MPI_Finalize automatically frees
     parent_inter (but still leaves us with memory leaks).  We'll have
     to find a better way...  */
  MPI_Comm_free(&parent_inter);
#endif
  
  MPI_Comm_size(parent_intra, (int*) &num_lamprocs);
  num_lamprocs--;

  /* Make a "dance" server port to listen for the other IMPI clients on */

  my_ipaddress = impi_get_my_ipaddress();
  my_dance_fd = sfh_sock_open_srv_inet_stm(&my_dance_port, -1);

  DBUG("IMPI client connecting to server...\n");
  if (impi_server_connect(server_name, server_port) != 0) {
    fprintf(stderr, "LAM IMPI client could not connect to the IMPI server\n");
    impi_bail(1);
  } else if (impi_server_do_work(my_ipaddress, my_dance_port) != 0) {
    fprintf(stderr, "LAM IMPI client got an error in the IMPI server information\n");
    impi_bail(1);
  }

  /* Setup the data structures for all the client/host/proc data */

  impi_setup_server_info();

  /* Connect up with the remote IMPI hosts */

  DBUG("IMPI client doing dance\n");
  if (impi_dance(my_dance_fd) != 0) {
    fprintf(stderr, "LAM IMPI host had problems connecting to other IMPI hosts\n");
    impi_bail(1);
  }
  close(my_dance_fd);
  my_dance_fd = -1;

  /* Distribute the data from the IMPI server to the LAM nodes.  Note
     we now do this *after* the dance so that the LAM user programs
     don't procede until after we are sure that we can connect up all
     the hosts.  Connecting the hosts while the procs are running can
     lead to some nasty race conditions if we need to abort. */

  DBUG("IMPI client distributing server info\n");
  impi_distrib_server_info(0);
  need_distrib_info = 0;

  /* Do great things */

  DBUG("IMPI client doing router\n");
  do_router();

  /* Shut down our connection with the IMPI server */

  DBUG("Shutting down IMPI LAM client\n");
  impi_server_disconnect();

  /* Acknowledge that we're done to our parents */

  MPI_Barrier(parent_intra);

  /* That's all, folks.  Free the parent_intra (this is collective; it
     also happens in IMPI_Finalize()) and the datatype(s) that we
     created for IMPI, and other random data items that we created. */

#if 0
  in_finalize = 1;
#endif

  MPI_Comm_free(&parent_intra);
  IMPI_Free_dtype();
  free(lam_impi_procs);
  free(lam_impi_hosts);
  free(lam_impi_clients);

  MPI_Finalize();

  return 0;
}


/*
 *	impi_bail
 *
 *	Function:	- called for abnormal termination
 *                      - performs cleanup, then exits
 *
 *	Accepts:	- error code
 */
void
impi_bail(int code)
{
  impi_server_bail(code);

  /* If our listening socket is still open, close it */

  if (my_dance_fd != -1) {
    close(my_dance_fd);
    my_dance_fd = -1;
  }

  /* If we haven't send messages back to the LAM nodes yet, set nprocs
     to -1 (a failured condition) and send them messages */

  if (need_distrib_info == 1) {
    impi_distrib_server_info(1);
    need_distrib_info = 0;
  }

#if 0
  if (in_finalize == 0) {
    DBUG("IMPI_Bail Calling comm_free on parent_intra\n");
    MPI_Comm_free(&parent_intra);
    IMPI_Free_dtype();
    DBUG("IMPI_Bail calling MPI_Finalize\n");
    MPI_Finalize();
  }
#else
  printf("LAM IMPI client and host aborting...\n");
  MPI_Abort(MPI_COMM_WORLD, code);
#endif

  exit(code);
}


/*
 *	parse_args
 *
 *	Function:	- parse command line arguments
 *
 *	Accepts:	- argc and argv
 */
static void
parse_args(int argc, char* argv[])
{
  char *p;

  /* Check to ensure that we got good command line args */

  if (argc != 3) {
    show_help("impid", "usage", argv[0], NULL);
    exit(1);
  }

  my_client_rank = atoi(argv[1]);
  server_name = argv[2];
  if ((p = strchr(server_name, ':')) == NULL) {
    show_help("impid", "usage", argv[0], NULL);
    exit(2);
  } else {
    *p = '\0';
    server_port = atoi(p + 1);
  }
  if (my_client_rank < 0 || my_client_rank > 31 || server_port < 0) {
    show_help("impid", "usage", argv[0], NULL);
    exit(3);
  }
}


#endif
