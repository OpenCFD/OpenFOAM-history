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
 * $Id: client.c,v 1.31 2003/04/27 14:03:33 jsquyres Exp $
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <laminternal.h>
#include <args.h>
#include <portable.h>
#include <typical.h>
#include <mpisys.h>
#include <lamdebug.h>
#include <impi-defs.h>
#include <impi.h>
#include <impid.h>
#include <sfh.h>
#include <etc_misc.h>

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG 1

/*
 * exported variables
 */
IMPI_Int4 *cdata[IMPI_MAX_I];         /* return data from IMPI server */
IMPI_Uint4 nclients = 0;              /* number of IMPI clients */
IMPI_Uint4 nhosts = 0;                /* number of other IMPI hosts */
IMPI_Uint4 nprocs = 0;                /* total number of IMPI processes */


/*
 * private variables
 */
static int fl_debug = 0;
static int server_fd = -1;
static IMPI_Version version = { IMPI_MAJOR_VERSION , IMPI_MINOR_VERSION };


static int labels[] = {
  IMPI_NO_LABEL,
  
  /* Per-Client labels for the 'COLL' commands. */
  
  IMPI_C_VERSION,
  IMPI_C_NHOSTS,
  IMPI_C_NPROCS,
  IMPI_C_DATALEN,
  IMPI_C_TAGUB,
  IMPI_C_COLL_XSIZE,
  IMPI_C_COLL_MAXLINEAR,

  /* Per-Host labels for the 'COLL' commands. */
  
  IMPI_H_IPV6,
  IMPI_H_PORT,
  IMPI_H_NPROCS,
  IMPI_H_ACKMARK,
  IMPI_H_HIWATER,
  
  /* Per-Proc labels for the 'COLL' commands. */
  
  IMPI_P_IPV6,
  IMPI_P_PID,

  -1
};

/*
 * private functions 
 */
static int do_cmd_auth(void);
static int do_cmd_auth_none(IMPI_Server_auth *sauth);
static int do_cmd_auth_key(IMPI_Server_auth *sauth);
static int do_cmd_impi(void);
static int do_cmd_coll(IMPI_Uint4 ipaddress, int port);
static int do_coll_label(IMPI_Int4 label, void* data, int elsize,
			 int send_nelem, int recv_nelem, int unknown_len);


/*
 *	impi_server_connect
 *
 *	Function:	- connect to the IMPI server
 *
 *	Accepts:	- IP name/number (in a string) of server
 *			- port number of server
 *
 *	Returns:	- 0 on success, LAMERROR on failure
 */
int
impi_server_connect(char *server_name, int server_port)
{
  struct in_addr* server_node;
  struct hostent* server_struct;
  IMPI_Uint4 server_address;

  /* If a hostname was given, get its IP number */
  /* otherwise, assume a w.x.y.z form was given */

  if (!isdigit((int) server_name[0])) {
    DBUG("Looking up hostname: %s\n", server_name);
    server_struct = gethostbyname(server_name);
    if (server_struct == NULL) {
      char foo[1024];
      snprintf(foo, 1024, "LAM IMPI client: could not find hostname (%s)", 
	       server_name);
      lamfail(foo);
    }
    
    server_node = (struct in_addr*) server_struct->h_addr;
    server_address = server_node->s_addr;
  } else {
    DBUG("Got server addr %s\n", server_name);
    server_address = inet_addr(server_name);
  }

  /* Use the SFH code to open a socket to the server */

  server_fd = sfh_sock_open_clt_inet_stm((unsigned char*) &server_address, 
					 server_port);
  DBUG("Got socket: %d\n", server_fd);

  /* Make the socket close on exec. */

  if (server_fd != -1) {
    fcntl(server_fd, F_SETFD, 1);
    return 0;
  }

  return (LAMERROR);
}


/*
 *	impi_server_do_work
 *
 *	Function:	- does data exchange with IMPI server
 *
 *      Accepts:        - IP address of this machine
 *                      - port that we're listening on for IMPI clients
 *	Returns:	- 0 for success, != 0 on failure
 */
int
impi_server_do_work(IMPI_Uint4 ipaddress, int dance_port)
{
  return !(do_cmd_auth() == 0 &&
	   do_cmd_impi() == 0 &&
	   do_cmd_coll(ipaddress, dance_port) == 0);
}


/*
 *	impi_server_disconnect
 *
 *	Function:	- Send the FINI command, close the socket
 */
void
impi_server_disconnect()
{
  IMPI_Cmd command;

  /* Send the FINI */

  command.cmd = IMPI_CMD_FINI;
  command.len = 0;

  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  mwrite(server_fd, (const char*) &command, sizeof(command));

  /* That's all, folks! */

  impi_server_bail(0);
}


/*
 *	server_bail
 *
 *	Function:	- [usually] called for abnormal termination
 *                      - performs cleanup
 *
 *	Accepts:	- error code
 */
void
impi_server_bail(int code)
{
  /* If we've got sockets open, close them */

  if (server_fd != -1) 
    close(server_fd);
  server_fd = -1;
}


/*
 *	do_cmd_auth
 *
 *	Function:	- perform authentication with IMPI server
 *
 *	Returns:	- 0 for success, LAMERROR on failure
 */
static int
do_cmd_auth()
{
  IMPI_Client_auth cauth;
  IMPI_Server_auth sauth;
  IMPI_Cmd command;

  command.cmd = IMPI_CMD_AUTH;
  command.len = sizeof(IMPI_Client_auth);
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  LAM_SWAP_ENDIAN(&command.len, sizeof(command.len));
  if (mwrite(server_fd, (const char*) &command, sizeof(command)) != 
      sizeof(command))
    return (LAMERROR);

  /* Fill up IMPI_Client_auth */

  cauth.auth_mask = 0;
  if (getenv("IMPI_AUTH_NONE") != 0)
    cauth.auth_mask |= 1;
  if (getenv("IMPI_AUTH_KEY"))
    cauth.auth_mask |= 2;
  if (cauth.auth_mask == 0)
    show_help("impi-client", "no-auth", NULL);
  LAM_SWAP_ENDIAN(&cauth.auth_mask, sizeof(cauth.auth_mask));
  if (mwrite(server_fd, (const char*) &cauth, sizeof(cauth)) != sizeof(cauth))
    return (LAMERROR);

  /* Wait for server response telling us which to do */

  if (mread(server_fd, (char*) &sauth, sizeof(sauth)) != sizeof(sauth))
    return (LAMERROR);
  LAM_SWAP_ENDIAN(&sauth.which, sizeof(sauth.which));
  LAM_SWAP_ENDIAN(&sauth.len, sizeof(sauth.len));
  switch (sauth.which) {
  case IMPI_AUTH_NONE:
    return do_cmd_auth_none(&sauth);

  case IMPI_AUTH_KEY:
    return do_cmd_auth_key(&sauth);

  default:
    lamfail("Server wants unrecognized authentication type.");
    break;
  }

  return (LAMERROR);
}

static int 
do_cmd_auth_none(IMPI_Server_auth *sauth)
{
  fprintf(stderr, "WARNING: IMPI server requested IMPI_AUTH_NONE authentication protocol\n");

  return 0;
}

static int 
do_cmd_auth_key(IMPI_Server_auth *sauth)
{
  IMPI_Auth_key auth;

  /* Send out our key */
  /* It has already been verified that the IMPI_AUTH_KEY environment */
  /* variable exists */

  char *key = getenv("IMPI_AUTH_KEY");
  if (key != 0)
    DBUG("Got IMPI_AUTH_KEY\n");
  DBUG("I got the auth key of: %s\n", key);

#if LAM_HAVE_ATOLL
  auth.key = atoll(key);
#elif LAM_HAVE_STRTOLL
  auth.key = strtoll(key, (char**) 0, 10);
#else

Error: We do not seem to have atoll() or stroll().  Do not know if any
hardware/OS/compiler will fit this description, so we have not
bothered to code up a replacement stoll() or stroll().  

#endif
  DBUG("Got auth key: %lld\n", auth.key);

  DBUG("Sending auth key to server\n");
  LAM_SWAP_ENDIAN(&auth.key, sizeof(auth.key));
  if (mwrite(server_fd, (const char*) &auth, sizeof(auth)) != sizeof(auth))
    return (LAMERROR);

  DBUG("Done snending key\n");
  return 0;
}


/*
 *	do_cmd_impi
 *
 *	Function:	- do CMD_IMPI with server
 *
 *	Returns:	- 0 for success, LAMERROR on failure
 */
static int
do_cmd_impi()
{
  IMPI_Cmd command;
  IMPI_Impi impi;

  /* Send my rank */

  DBUG("Rank %d: Sending IMPI_CMD_IMPI to server\n", my_client_rank);
  command.cmd = IMPI_CMD_IMPI;
  command.len = sizeof(impi);
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  LAM_SWAP_ENDIAN(&command.len, sizeof(command.len));
  if (mwrite(server_fd, (const char*) &command, sizeof(command)) != 
      sizeof(command))
    return (LAMERROR);

  impi.rank = my_client_rank;
  LAM_SWAP_ENDIAN(&impi.rank, sizeof(impi.rank));
  if (mwrite(server_fd, (const char*) &impi, sizeof(impi)) != sizeof(impi))
    return (LAMERROR);

  /* Get the overall size */

  if (mread(server_fd, (char*) &command, sizeof(command)) != sizeof(command))
    return (LAMERROR);
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  if (command.cmd != IMPI_CMD_IMPI) {
    char temp[1024];
    sprintf(temp, "Got unexpected command -- expected IMPI_CMD_IMPI, got 0x%x\n", command.cmd);
    lamfail(temp);
  }
  DBUG("Reading the num_clients from the server\n");
  if (mread(server_fd, (char*) &impi, sizeof(impi)) != sizeof(impi))
    return (LAMERROR);
  LAM_SWAP_ENDIAN(&impi.size, sizeof(impi.size));
  nclients = impi.size;
  DBUG("Got 0x%x clients back from server\n", nclients);
  if (my_client_rank >= nclients) {
    DBUG("Oops -- it appears that my client rank is too large (%d >= %d)\n",
	 my_client_rank, nclients);
    return (LAMERROR);
  }

  return 0;
}


/*
 *	do_cmd_coll
 *
 *	Function:	- do data exchange with IMPI server
 *                      _ save all the collated data in cdata[]
 *                      _ specifically for IMPI version 0
 *
 *      Accepts:        - IP address of this machine
 *                      - port that we're listening on for IMPI clients
 *
 *	Returns:	- 0 for success, LAMERROR on failure
 */
static int
do_cmd_coll(IMPI_Uint4 my_ipaddress, int my_dance_port)
{
  int i, j;
  IMPI_Int4 data;
  IMPI_Cmd command;
  char *env;
  char *buffer;
  unsigned int *ibuffer;
  IMPI_Uint8 *i8buffer;
  unsigned char temp[1024];
  char temp2[1024];
  int my_rank;
  
  /* Send/receive all the labels */
  /* do_coll_label will alloc space for the return data */
  /* Client labels */
  /* IMPI_C_VERSION label */

  LAM_SWAP_ENDIAN(&version.major, sizeof(version.major));
  LAM_SWAP_ENDIAN(&version.minor, sizeof(version.minor));
  DBUG(" &&& the size of version is %d\n", sizeof(version));
  DBUG("IMPI_C_VERSION\n");
  if (do_coll_label(IMPI_C_VERSION, &version, sizeof(version), 1, 
		    nclients, 1) == LAMERROR)
    return LAMERROR;

  /* IMPI_C_NHOSTS label */

  data = 1;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_C_NHOSTS\n");
  if (do_coll_label(IMPI_C_NHOSTS, &data, 4, 1, nclients, 0) == LAMERROR)
    return LAMERROR;

  /* IMPI_C_NPROCS label */

  data = num_lamprocs;
  DBUG("The number of local hosts is %d\n", num_lamprocs);
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_C_VERSION\n");
  if (do_coll_label(IMPI_C_NPROCS, &data, 4, 1, nclients, 0) == LAMERROR)
      return LAMERROR;

  /* IMPI_C_DATALEN label */

  data = impi_lam_datalen;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_C_DATALEN\n");
  if (do_coll_label(IMPI_C_DATALEN, &data, 4, 1, nclients, 0) == LAMERROR)
    return LAMERROR;

  /* IMPI_C_TAGUB label */

  data = lam_mpi_max_tag;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_C_TAGUB\n");
  if (do_coll_label(IMPI_C_TAGUB, &data, 4, 1, nclients, 0) == LAMERROR)
    return LAMERROR;

  /* IMPI_C_COLL_XSIZE label */

  data = impi_lam_coll_xsize;
  if ((env = getenv("LAM_IMPI_COLL_XSIZE")) != NULL)
    data = stoi((unsigned char*) env);
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_C_COLL_XSIZE\n");
  if (do_coll_label(IMPI_C_COLL_XSIZE, &data, 4, 1, nclients, 0) == LAMERROR)
    return LAMERROR;

  /* Check the return from coll_xsize -- everyone must have sent the
     same size.  Replace -1's with the default size before
     comparing. */

  j = IMPI_C_COLL_XSIZE_I;
  for (i = 0; i < nclients; i++) {
    if (cdata[j][i] == -1)
      cdata[j][i] = IMPI_COLL_XSIZE_DEFAULT;

    if (i > 0 && cdata[j][i] != cdata[j][i - 1])
      break;
  }
  if (i < nclients) {
    fprintf(stderr, "LAM IMPI client: did not receive same IMPI_COLL_XSIZE from all clients\n");
    for (i = 0; i < nclients; i++) {
      fprintf(stderr, "IMPI client %d: %d", i, cdata[j][i]);
      if (cdata[j][i] == IMPI_COLL_XSIZE_DEFAULT)
	fprintf(stderr, " (IMPI %d.%d default value)", IMPI_MAJOR_VERSION,
		IMPI_MINOR_VERSION);
      fprintf(stderr, "\n");
    }
    impi_bail(1);
  }

  /* IMPI_C_COLL_MAXLINEAR label */

  data = impi_lam_coll_maxlinear;
  if ((env = getenv("LAM_IMPI_COLL_MAXLINEAR")) != NULL)
    data = stoi((unsigned char*) env);
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_C_MAXLINEAR\n");
  if (do_coll_label(IMPI_C_COLL_MAXLINEAR, &data, 4, 1, 
		    nclients, 0) == LAMERROR)
    return LAMERROR;

  /* Check the return from coll_maxlinear -- everyone must have sent
     the same size.  Replace -1's with the default size before
     comparing. */

  j = IMPI_C_COLL_MAXLINEAR_I;
  for (i = 0; i < nclients; i++) {
    DBUG("Checking slot %d, %d of %d\n", j, i, nclients);
    if (cdata[j][i] == -1)
      cdata[j][i] = IMPI_COLL_MAXLINEAR_DEFAULT;

    if (i > 0 && cdata[j][i] != 
	cdata[j][i - 1])
      break;
  }
  DBUG("Done checking\n");
  if (i < nclients) {
    fprintf(stderr, "LAM IMPI client: did not receive same IMPI_COLL_MAXLINEAR from all clients\n");
    for (i = 0; i < nclients; i++) {
      fprintf(stderr, "IMPI client %d: %d", i, cdata[j][i]);
      if (cdata[j][i] == IMPI_COLL_MAXLINEAR_DEFAULT)
	fprintf(stderr, " (IMPI %d.%d default value)", IMPI_MAJOR_VERSION,
		IMPI_MINOR_VERSION);
      fprintf(stderr, "\n");
    }
    impi_bail(1);
  }

  /* Host labels */
  /* Make a dummy buffer completely filled with IPv6 versions of 
     the adddress of this node, and send it */

  nhosts = 0;
  for (i = 0; i < nclients; i++)
    nhosts += cdata[IMPI_C_NHOSTS_I][i];
  buffer = calloc(1, 16 * nhosts);
  memcpy(buffer + 12, &my_ipaddress, 4);
  DBUG("Got my ipaddress: %s\n", impi_sprintf_ipaddr(my_ipaddress));
  DBUG("IMPI_H_IPV6\n");
  if (do_coll_label(IMPI_H_IPV6, buffer, 16, 1, nhosts, 0) == LAMERROR)
    return LAMERROR;

  /* Note that there is no need to un-swap endian the contents (even
     though do_coll_label will automatically LAM_SWAP_ENDIAN the
     received data), 'cause LAM_SWAP_ENDIAN for 128 bit quantities is
     a noop -- see endian.c. */

  free(buffer);

  /* We're already waiting on my_dance_port.  The rest of the
     host labele are straightforward */

  /* IMPI_H_PORT label */

  data = my_dance_port;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_H_PORT\n");
  if (do_coll_label(IMPI_H_PORT, &data, 4, 1, nhosts, 0) == LAMERROR)
    return LAMERROR;

  /* IMPI_H_NPROCS label */

  data = num_lamprocs;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("*** SENDING NUM_LAMPROCS: %d -- %d\n", num_lamprocs, data);
  DBUG("IMPI_H_NPROCS\n");
  if (do_coll_label(IMPI_H_NPROCS, &data, 4, 1, nhosts, 0) == LAMERROR)
    return LAMERROR;
  for (i = 0; i < nhosts; i++)
    DBUG("*** GOT IMPI_H_NPROCS: (index %d) host %d, %d\n", IMPI_H_NPROCS_I, 
	 i, cdata[IMPI_H_NPROCS_I][i]);

  /* IMPI_H_ACKMARK label */

  data = impi_lam_ackmark;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_H_ACKMARK\n");
  if (do_coll_label(IMPI_H_ACKMARK, &data, 4, 1, nhosts, 0) == LAMERROR)
    return LAMERROR;

  /* IMPI_H_HIWATER label */

  data = impi_lam_hiwater;
  LAM_SWAP_ENDIAN(&data, sizeof(data));
  DBUG("IMPI_H_HIWATER\n");
  if (do_coll_label(IMPI_H_HIWATER, &data, 4, 1, nhosts, 0) == LAMERROR)
    return LAMERROR;

  /* Process labels */
  /* Again, make a dummy buffer completely filled with IPv6 versions of
     the address of this node, and send it */
  /* IMPI_P_IPV6 label */

  nprocs = 0;
  for (i = 0; i < nhosts; i++)
    nprocs += cdata[IMPI_H_NPROCS_I][i];
  buffer = calloc(1, 16 * num_lamprocs);
  for (i = 0; i < num_lamprocs; i++)
    memcpy(buffer + 12 + (i * 16), &my_ipaddress, 4);
  DBUG("IMPI_P_IPV6\n");
  if (do_coll_label(IMPI_P_IPV6, buffer, 16, num_lamprocs, 
		    nprocs, 0) == LAMERROR)
    return LAMERROR;

  /* Note that there is no need to un-swap endian the contents (even
     though do_coll_label will automatically LAM_SWAP_ENDIAN the
     received data), 'cause LAM_SWAP_ENDIAN for 128 bit quantities is
     a noop -- see endian.c. */

  /* Now take the same buffer (there is more than enough space) and
     fill it with the PID's of the LAM node processes.  Then copy them
     to IMPI_In8 holding spaces to do the label. */
  /* IMPI_P_PID label */

  ibuffer = (unsigned int*) buffer;
  i8buffer = (IMPI_Uint8*) temp;

  MPI_Comm_rank(parent_intra, &my_rank);
  MPI_Gather(&i, 1, MPI_INT, ibuffer, 1, MPI_INT, my_rank, parent_intra);
  for (i = 0; i < num_lamprocs; i++) {
    DBUG("IMPID got PID: (normal) %d / (swapped) ", ibuffer[i]);

    i8buffer[i] = (IMPI_Uint8) ibuffer[i];
    LAM_SWAP_ENDIAN(&i8buffer[i], sizeof(i8buffer[i]));

    for (j = 0; j < 4; j++)
      DBUG("*%02x", ((char*) &ibuffer[i])[j]);
    DBUG(" (8byte) %lld ", i8buffer[i]);
    for (j = 0; j < 8; j++)
      DBUG("*%02x", temp[i * 8 + j]);
    DBUG("\n");
  }
  DBUG("IMPI_P_PID\n");
  if (do_coll_label(IMPI_P_PID, temp, 8, num_lamprocs, 
		    nprocs, 0) == LAMERROR) {
    free(buffer);
    return LAMERROR;
  }
  free(buffer);

  /* It is far, far easier (and more efficient) to keep the data in
     network order rather than to keep it in native order and swap it
     every time we want to access it.  i.e., the *value* of the
     IMPI_Pid is never checked -- only bytewise compares are made.
     Hence, since do_coll_label swapped it for us, swap it back. */

  for (i = 0; i < nprocs; i++) {
    LAM_SWAP_ENDIAN(&cdata[IMPI_P_PID_I][(i * 2)], sizeof(IMPI_Int8));

    DBUG("IMPID GOT PID BACK FROM SERVER: %lld / ", 
	 *((IMPI_Int8*) &cdata[IMPI_P_PID_I][(i * 2)]));
    for (j = 0; j < 8; j++)
      DBUG("%02x", (unsigned char) ((unsigned char*) 
				    &cdata[IMPI_P_PID_I][i * 2])[j]);
    DBUG("\n");
  }

  /* Send the DONE */

  DBUG("Sending IMPI_CMD_DONE\n");
  command.cmd = IMPI_CMD_DONE;
  command.len = 0;
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  if (mwrite(server_fd, (const char*) &command, sizeof(command)) != 
      sizeof(command))
    return (LAMERROR);
  DBUG("Finished writing IMPI_CMD_DONE\n");
  if (mread(server_fd, (char*) &command, sizeof(command)) != 
      sizeof(command))
    return (LAMERROR);
    DBUG("Finished reading IMPI_CMD_DONE\n");
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  LAM_SWAP_ENDIAN(&command.len, sizeof(command.len));
  if (command.cmd != IMPI_CMD_DONE) {
    errno = EAGAIN;
    snprintf(temp2, 1024, "LAM IMPI client: expecting 0x%x (IMPI_CMD_DONE) from server, got 0x%x\n", IMPI_CMD_DONE, command.cmd);
    lamfail(temp2);
  }
  if (command.len != 0) {
    DBUG("Got IMPI_CMD_DONE from server: expected payload : 0\n");
    DBUG("                             :   actual payload : %d\n",
	 command.len);
    DBUG("Consuming faulty payload from server.\n");
    buffer = malloc(command.len);
    if (mread(server_fd, buffer, command.len) != command.len)
      return (LAMERROR);
    free(buffer);
  }
  DBUG("Received IMPI_CMD_DONE from server. Ready to do great things.\n");

  return 0;
}


/*
 *	do_coll_label
 *
 *	Function:	- send a set of label data to server
 *                      - receive back full set of data for that label
 *                        from label, place into cdata[]
 *      Arguments:      - label of data to send
 *                      - pointer to ALREADY ENDIAN ADJUSTED data to send
 *                      - size of each element
 *                      - number of elements to send
 *                      - number of elements to receive
 *                      - whether we know the receive length or not
 *
 */
static int
do_coll_label(IMPI_Int4 label, void* data, int elsize,
	      int send_nelem, int recv_nelem, int unknown_len)
{
  IMPI_Cmd command;
  IMPI_Coll coll;
  IMPI_Chdr chdr;
  char *buffer;
  int i, j;
  int send_size = elsize * send_nelem;
  int recv_size = elsize * recv_nelem;
  char temp[1024];

  DBUG("Sending COLL label 0x%x to server\n", label);
  command.cmd = IMPI_CMD_COLL;
  command.len = sizeof(coll) + send_size;
  DBUG("Sending IMPI_CMD_COLL\n");
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  LAM_SWAP_ENDIAN(&command.len, sizeof(command.len));
  if (mwrite(server_fd, (const char*) &command, sizeof(command)) != 
      sizeof(command))
    return (LAMERROR);

  coll.label = label;
  LAM_SWAP_ENDIAN(&coll.label, sizeof(coll.label));
  DBUG("Sending IMPI_Coll label\n");
  if (mwrite(server_fd, (const char*) &coll, sizeof(coll)) != sizeof(coll))
    return (LAMERROR);

  /* Send out the data */
  /* Must rely on caller to set endianess of data */

  DBUG("Sending data of size %d\n", send_size);
  if (mwrite(server_fd, data, send_size) != send_size)
    return (LAMERROR);

  /* Get the response back from the server, should be IMPI_CMD_COLL */
  /* Must rely on caller to fix endianess */

  DBUG("Reading server's IMPI_Cmd response\n");
  if (mread(server_fd, (char*) &command, sizeof(command)) != sizeof(command))
    return (LAMERROR);
  LAM_SWAP_ENDIAN(&command.cmd, sizeof(command.cmd));
  LAM_SWAP_ENDIAN(&command.len, sizeof(command.len));
  if (command.cmd != IMPI_CMD_COLL) {
    errno = EAGAIN;
    snprintf(temp, 1024, "LAM IMPI client: expecting IMPI_CMD_COLL from server, got 0x%x",
	    command.cmd); 
    lamfail(temp);
  }

  /* Alloc out space for the return data and read it */
  /* Should get the label and bit mask first, followed by a packed set
     of the data -- use the bit mask to unpack the data */

  DBUG("Reading server's IMPI_Chdr response\n");
  if (mread(server_fd, (char*) &chdr, sizeof(chdr)) != sizeof(chdr))
    return (LAMERROR);
  LAM_SWAP_ENDIAN(&chdr.label, sizeof(chdr.label));
  if (chdr.label != label) {
    errno = EAGAIN;
    snprintf(temp, 1024, "LAM IMPI client: expecing COLL label 0x%x from server,got 0x%x", label, command.cmd);
    lamfail(temp);
  }
  DBUG("Got right label back from server -- reading %d bytes from server\n",
       command.len - 8);
  buffer = malloc(command.len);
  if (mread(server_fd, buffer, command.len - 8) != command.len - 8)
    return (LAMERROR);

  /* Unpack the data */
  /* Simplified for IMPI version 0 because all clients must have
     returned data for all labels -- hence, the bitmask is always
     (2^nclients) - 1 -- so no unpacking is necessary */
  /* Not receiving straight into the buffer, because a) it's going to
     be small, b) this is not intended to be high performance, and c)
     future versions of IMPI may require actual unpacking */

  if ((command.len - 8 != recv_size) && (!unknown_len)) {
    DBUG("Uh oh -- (command.len - 8) != recv_size :: %d != %d\n", 
	 command.len - 8, recv_size);
  }
  for (i = 0; labels[i] != -1; i++)
    if (labels[i] == label)
      break;
  if (labels[i] == -1) 
    i = 0;
  if (unknown_len)
    recv_size = command.len - 8;
  DBUG("SAving in label %d, recv_size %d, elsize %d\n", i, recv_size, elsize);

  /* Malloc out the buffer, copy in the received data, and swap the
     endian.  */

  for (j = 0; j < recv_size / elsize; j++) {
    LAM_SWAP_ENDIAN(buffer + j * elsize, elsize);
  }
  cdata[i] = malloc(recv_size);
  DBUG("Got memory location for lable %d: 0x%x\n", i, (unsigned int) cdata[i]);
  memcpy(cdata[i], buffer, recv_size);
  free(buffer);

  return 0;
}

#endif /* LAM_WANT_IMPI */
