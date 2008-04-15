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
 * $Id: dance.c,v 1.27 2003/10/11 19:15:34 brbarret Exp $
 *
 *	Function:	- network functions for connecting up all
 *                        the IMPI clients
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#if defined(HAVE_NETINET_TCP_H) && HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#include <args.h>
#include <lamdebug.h>
#include <impi-defs.h>
#include <impi.h>
#include <impid.h>
#include <laminternal.h>
#include <sfh.h>
#include <etc_misc.h>

#define LAM_MAXHOSTNAMELEN 256


/*
 * exported variables 
 */
IMPI_Uint4 my_host_rank = 0xffff;    /* index of my IMPI host */


/*
 * local functions
 */
static void do_connect(int host);
static void do_accept(int host, int my_dance_fd);
static void check_remote(char *function, IMPI_Uint4 *addr);


/*
 * private variables 
 */
static int fl_debug = 0;
static IMPI_Uint4 my_ip_address = 0;
static char my_hostname[LAM_MAXHOSTNAMELEN];


/*
 *	impi_sprintf_ipaddr
 *
 *	Function:	- return an IP address in dot form in a string
 *                      - helpful for debugging
 *
 *      Returns:        - pointer to static buffer with result
 */
char *
impi_sprintf_ipaddr(IMPI_Uint4 addr)
{
  static char buffer[16];

#if LAM_WORDS_BIGENDIAN
  snprintf(buffer, 16, "%d.%d.%d.%d",
	   (int) ((addr & 0xff000000) >> 24),
	   (int) ((addr & 0x00ff0000) >> 16),
	   (int) ((addr & 0x0000ff00) >> 8),
	   (int) ((addr & 0x000000ff) >> 0));
#else
  snprintf(buffer, 16, "%d.%d.%d.%d",
	    (int) ((addr & 0x000000ff) >> 0),
	    (int) ((addr & 0x0000ff00) >> 8),
	    (int) ((addr & 0x00ff0000) >> 16),
	    (int) ((addr & 0xff000000) >> 24));
#endif

  return buffer;
}


/*
 *	impi_get_my_ipaddress
 *
 *	Function:	- return the IP address of this machine
 *                      - warn user if we get anything in 127.x.y.z
 *
 *      Returns:        - IP address of this machine
 */
IMPI_Uint4
impi_get_my_ipaddress()
{
  int loopback;
  struct in_addr* server_node;
  struct hostent* server_struct;
  const char *hostname;

  hostname = getenv("IMPI_HOST_NAME");
  if (hostname != NULL && strlen(hostname) > 0) {
    strncpy (my_hostname, hostname, LAM_MAXHOSTNAMELEN);
  } else {
    if (gethostname(my_hostname, LAM_MAXHOSTNAMELEN) != 0)
      lamfail("LAM IMPI client: gethostname failed");
  }
  if ((server_struct = gethostbyname(my_hostname)) == NULL)
    lamfail("LAM IMPI client: gethostbyname failed");

  server_node = (struct in_addr*) server_struct->h_addr;
  my_ip_address = server_node->s_addr;

  /* Check for 127.x.y.z */

#if LAM_WORDS_BIGENDIAN
  loopback = ((my_ip_address & 0xff000000) >> 24 == 0xff);
#else
  loopback = (my_ip_address & 0x000000ff) == 0xff;
#endif
  if (loopback) {
    char addr[128];

    snprintf(addr, 128, "%s", impi_sprintf_ipaddr(my_ip_address));
    show_help("impi-client", "possible-loopback", addr, my_hostname, NULL);
  }

  return my_ip_address;
}


/*
 *	impi_dance
 *
 *	Function:	- make/accept connections from other IMPI clients
 *                      - close my_dance_fd when done for security reasons
 *
 *	Returns:	- 0 for success, LAMERROR on failure
 */
int
impi_dance(int my_dance_fd)
{
  int i, fd;
#define WANT_SETSOCKOPT 0
#if WANT_SETSOCKOPT
  int flag;
#endif
  int bufsize;
  unsigned char *address;
  IMPI_Uint4 *addr;

  /* Find my host rank */

  for (my_host_rank = i = 0; i < my_client_rank; i++)
    my_host_rank += cdata[IMPI_C_NHOSTS_I][i];
  DBUG("My host rank is %d\n", my_host_rank);

  /* Sanity check -- check public vs. private network IP addresses.
     This may actually save aggrivation for some users! */

  for (i = 0; i < nhosts; i++) {
    if (i == my_host_rank)
      continue;

    address = &lam_impi_hosts[i].ih_ipv6[12];
    addr = (IMPI_Uint4 *) address;
    if (i < my_host_rank) {
      check_remote("connect", addr);
    } else if (i > my_host_rank) {
      check_remote("accept", addr);
    }
  }

  /* Higher ranked host connects, lower ranked host accepts */
  /* Get *all* sockets first */

  for (i = 0; i < nhosts; i++) {
    if (i == my_host_rank)
      continue;

    DBUG("Establishing comms with IMPI rank %d\n", i);
    if (i < my_host_rank) {
      DBUG("Doing CONNECT\n");
      do_connect(i);
    } else if (i > my_host_rank) {
      DBUG("Doing ACCEPT\n");
      do_accept(i, my_dance_fd);
    }
  }

  for (i = 0; i < nhosts; i++) {
    if (i == my_host_rank)
      continue;

    /* Ensure that we really got a connection */

    DBUG("Checking for socket...\n");
    if (lam_impi_hosts[i].ih_fd == -1) {
      address = &lam_impi_hosts[i].ih_ipv6[12];
      addr = (IMPI_Uint4 *) address;
      if (i < my_host_rank)
	fprintf(stderr, "LAM IMPI host: Could not connect() to remote IMPI host %d (%s)\n",
		i, impi_sprintf_ipaddr(*addr));
      else
	fprintf(stderr, "LAM IMPI host: Could not accept() from remote IMPI host %d (%s)\n",
		i, impi_sprintf_ipaddr(*addr));
      return LAMERROR;
    }
    DBUG("Got happy socket!\n");

    /* Set sockets in non-blocking mode and set the send and receive
       buffer sizes.  */

    bufsize = (impi_lam_datalen + sizeof(IMPI_Packet)) * 2;
    fd = lam_impi_hosts[i].ih_fd;
#if WANT_SETSOCKOPT
    printf("Setting socket options on fd %d\n", fd);
    if (fcntl(fd, F_SETFL, O_NONBLOCK)) {
      printf("No go on the O_NONBLOCK\n");
      return LAMERROR;
    }
    
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, 
		   (char *) &flag, sizeof(flag))) {
      printf("No go on the TCP_NDELAY\n");
      return LAMERROR;
    }
#endif
    
    DBUG("Setting host socket send buffer size\n");
    if (sfh_sock_set_buf_size(fd, SFH_INET, SO_SNDBUF, bufsize) &&
	sfh_sock_set_buf_size(fd, SFH_INET, SO_SNDBUF, bufsize / 2)) {
      fprintf(stderr, "LAM IMPI client: Could not set the socket send buffer size to %d or %d bytes\n", bufsize, bufsize / 2);
      perror("setsockopt");
      return LAMERROR;
    }
    
    DBUG("Setting host socket receive buffer size\n");
    if (sfh_sock_set_buf_size(fd, SFH_INET, SO_RCVBUF, bufsize) &&
	sfh_sock_set_buf_size(fd, SFH_INET, SO_RCVBUF, bufsize / 2)) {
      fprintf(stderr, "LAM IMPI client: Could not set the socket receive buffer size to %d or %d bytes\n", bufsize, bufsize / 2);
      perror("setsockopt");
      return LAMERROR;
    }
    
    DBUG("Done setting socket sizes\n");
  }

  return 0;
}


/*
 *	do_connect
 *
 *	Function:	- connect to remote IMPI client
 *
 *      Accepts:        - rank of IMPI client to connect to
 */
static void
do_connect(int host)
{
  unsigned char *address = &lam_impi_hosts[host].ih_ipv6[12];
  int fd, port = lam_impi_hosts[host].ih_port;
  IMPI_Uint4 endian_hrank = my_host_rank;

  /* Use the SFH code to open a socket to the server */

  IMPI_Uint4 *addr = (IMPI_Uint4 *) address;
  DBUG("Opening socket to %s, port %d\n", impi_sprintf_ipaddr(*addr), port);
  fd = sfh_sock_open_clt_inet_stm(address, port);
  DBUG("Got socket for host %d: %d\n", host, fd);

  /* Make the socket close on exec. */

  if (fd != -1)
    fcntl(fd, F_SETFD, 1);

  /* Send my index down the socket, per IMPI standard */

  DBUG("Sending my host index: %d\n", my_host_rank);
  LAM_SWAP_ENDIAN(&endian_hrank, sizeof(endian_hrank));
  mwrite(fd, (const char*) &endian_hrank, sizeof(IMPI_Uint4));
  lam_impi_hosts[host].ih_fd = fd;
}


/*
 *	do_accept
 *
 *	Function:	- accept connection from any other IMPI client
 *
 */
static void
do_accept(int host, int my_dance_fd)
{
  int fd;
  LAM_SOCK_OPTLEN_T client_len;
  struct sockaddr* client_address_ptr;
  struct sockaddr_in client_inet_address;
  IMPI_Uint4 addr, data;
  char remote_addr[128];
  char *remote_name = "<no name found>";
  struct hostent *h;

  client_len = sizeof(client_inet_address);
  client_address_ptr = (struct sockaddr*) &client_inet_address;

  /* Hmm.  Solaris bcheck complains if we don't clear out 
     client_inet_address first */

  memset(client_address_ptr, 0, sizeof(client_inet_address));
  
  /* Do the accept */

  while (1) {
    DBUG("Doing accept\n");
    fd = accept(my_dance_fd, client_address_ptr, &client_len);
    if (fd == -1)
      lamfail("accept() failed from IMPI client");
    DBUG("Accept succeeded\n");

    /* JMS.  Hmm.  I'm still not crazy about the [lack of] security of
       this!! */
    /* Per IMPI standard, we have to read 32 bits (the host's rank).
       Just to be safe, we check what they sent with what they should
       have sent. */
    
    mread(fd, (char*) &data, sizeof(IMPI_Uint4));
    LAM_SWAP_ENDIAN(&data, sizeof(data));
    DBUG("READ: Read index of %d\n", data);
    
    /* Find out who it is, and put the fd in the right spot(s) in the
       procs array.  If we dont' know who it is, close the socket and
       wait for another -- we don't accept outsiders. */
    
    DBUG("Got client from %s\n", impi_sprintf_ipaddr(addr));
    addr = client_inet_address.sin_addr.s_addr;
    for (host = 0; host < nhosts; host++) {
      if (memcmp(&lam_impi_hosts[host].ih_ipv6[12], &addr, 4) == 0) {
	DBUG("Found client %s as IMPI host %d\n", impi_sprintf_ipaddr(addr), 
	   host);
	
	/* Check to be sure that their index and my index are consistent */
	
	if (host != data) {
	  char temp[1024];
	  snprintf(temp, 1024, "Remote IMPI host (my index %d) has a different index number for itself (%d)", host, data);
	  lamfail(temp);
	}
	
	lam_impi_hosts[host].ih_fd = fd;
	return;
      }
    }
    close(fd);

    snprintf(remote_addr, 128, "%s", impi_sprintf_ipaddr(addr));
    h = gethostbyaddr(remote_addr, client_len, AF_INET);
    if (h != NULL)
      remote_name = h->h_name;
    show_help("impi-client", "unexpected-connection", 
	      impi_sprintf_ipaddr(addr),
	      remote_name, my_hostname, NULL);
  }
}


static void
check_remote(char *function, IMPI_Uint4 *addr)
{
  int me_192 = 0, me_10 = 0;
  int addr_192 = 0, addr_10 = 0;
  if (my_ip_address == 0)
    impi_get_my_ipaddress();

  /* Check for 192.168.x.y */

#if LAM_WORDS_BIGENDIAN
  me_192 = ((my_ip_address & 0xc0a80000) >> 16 == 0xc0a8);
  me_10 = ((my_ip_address & 0x0a000000) >> 24 == 0x0a);
  addr_192 = ((*addr & 0xc0a80000) >> 16 == 0xc0a8);
  addr_10 = ((*addr & 0x0a000000) >> 24 == 0x0a);
#else
  me_192 = ((my_ip_address & 0x0000a8c0) == 0xa8c0);
  me_10 = ((my_ip_address & 0x0000000a) == 0x0a);
  addr_192 = ((*addr & 0x0000a8c0) == 0xa8c0);
  addr_10 = ((*addr & 0x0000000a) == 0x0a);
#endif
  if ((me_192 && addr_192) || (me_10 && addr_10))
    return;
  else if ((me_192 || me_10) && (addr_192 || addr_10)) {
    char me_str[128];
    char addr_str[128];

    snprintf(me_str, 128, "%s", impi_sprintf_ipaddr(my_ip_address));
    snprintf(addr_str, 128, "%s", impi_sprintf_ipaddr(*addr));
    show_help("impi-client", "different-private-nets", me_str, my_hostname, 
	      addr_str, function, NULL);
  } else if (me_192 + me_10 == 1) {
    char me_str[128];
    char addr_str[128];

    printf("LOCAL PRIVATE NET\n");
    snprintf(me_str, 128, "%s", impi_sprintf_ipaddr(my_ip_address));
    snprintf(addr_str, 128, "%s", impi_sprintf_ipaddr(*addr));
    show_help("impi-client", "local-private-net", me_str, my_hostname, 
	      addr_str, function, NULL);
  } else if (addr_192 + addr_10 == 1) {
    char me_str[128];
    char addr_str[128];

    snprintf(me_str, 128, "%s", impi_sprintf_ipaddr(my_ip_address));
    snprintf(addr_str, 128, "%s", impi_sprintf_ipaddr(*addr));
    show_help("impi-client", "remote-private-net", me_str, my_hostname, 
	      addr_str, function, NULL);
  }
}

#endif
