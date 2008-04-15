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
 * $Id: bootsockio.c,v 6.12 2003/07/05 21:52:08 jsquyres Exp $
 *
 *	Function:	- OTB booting socket I/O functions
 *				- readsockint4(): read an int4 from a socket
 *				- writesockint4(): read an int4 to a socket
 *				- readcltcoord(): read client nodeid/port #
 *				- readcltnbr(): read client neighbour info
 *				- writecltcoord(): write client nodeid/port #
 *				- writecltnbr(): write client neighbour info
 */

#include <errno.h>
#include <portable.h>
#include <t_types.h>
#include <typical.h>
#include <sys/param.h>
#include <netdb.h>
#include <string.h>

#include <laminternal.h>
#include <net.h>
#include <etc_misc.h>


/*
 *	readsockint4
 *
 *	Function:	- read a 32-bit integer from a socket
 *			- handles byte ordering issue
 *	Accepts:	- socket
 *			- ptr to integer
 *	Returns:	- 0 or ERROR
 */
int
readsockint4(int sock, int4 *pdata)
{
	int4		data;
	int		ret;

	ret = mread(sock, (char *) &data, sizeof(int4));
	if (ret < (int)sizeof(int4)) {
		errno = (ret < 0) ? errno : EIO;
		return (ERROR);
	}

	*pdata = ttol(data);
	return(0);
}


/*
 *	writesockint4
 *
 *	Function:	- write a 32-bit integer to a socket
 *			- handles byte ordering issue
 *	Accepts:	- socket
 *			- integer
 *	Returns:	- 0 or ERROR
 */
int
writesockint4(int sock, int4 data)
{
	int		ret;

	data = ltot(data);
	ret = mwrite(sock, (char *) &data, sizeof(int4));
	if (ret < (int)sizeof(int4)) {
		errno = (ret < 0) ? errno : EIO;
		return (ERROR);
	}

	return(0);
}


/*
 *	readcltcoord
 *
 *	Function:	- read client STREAM and UDP port numbers
 *	Accepts:	- socket
 *			- ptr to node ID #
 *			- ptr to STREAM port #
 *			- ptr to UDP port #
 *	Returns:	- 0 or ERROR
 */
int
readcltcoord(int sock, int4 *pid, int4 *pstrport, int4 *pudpport)
{
	int4		status;

	if (readsockint4(sock, &status)) {
		return (ERROR);
	}

	if (status) {
		errno = status;
		return (ERROR);
	}

	if (readsockint4(sock, pid)) {
		return (ERROR);
	}

	if (readsockint4(sock, pstrport)) {
		return (ERROR);
	}

	if (readsockint4(sock, pudpport)) {
		return (ERROR);
	}

	return(0);
}


/*
 *	writecltcoord
 *
 *	Function:	- write client STREAM and UDP port numbers
 *			- called by the booted client
 *	Accepts:	- socket
 *			- node ID #
 *			- STREAM port #
 *			- UDP port #
 *	Returns:	- 0 or ERROR
 */
int
writecltcoord(int sock, int4 id, int4 strport, int4 udpport)
{
	if (writesockint4(sock, INT4_NIL)) {
		return (ERROR);
	}

	if (writesockint4(sock, id)) {
		return (ERROR);
	}

	if (writesockint4(sock, strport)) {
		return (ERROR);
	}

	if (writesockint4(sock, udpport)) {
		return (ERROR);
	}

	return(0);
}


/*
 *	readcltnbr
 *
 *	Function:	- read client neighbour <link index, hostaddr, port,
 *                                               ncpus>
 *	Accepts:	- socket
 *			- ptr link index
 *			- ptr host address (4 bytes, in network byte order)
 *			- ptr port number
 *			- ptr num cpus
 *	Returns:	- 0 or ERROR
 */
int
readcltnbr(int sock, int4 *plink, unsigned char *phostaddr, int4 *pport,
	   int4 *pnode_type, int4 *pnum_cpus)
{
  int ret;
  int name_len;

  if (readsockint4(sock, plink))
    return (ERROR);

  /* If the link info is for an invalid node, nothing else is sent */

  if (*plink == NOTNODEID) {
    *pport = -1;
    *pnode_type = -1;
    *pnum_cpus = -1;
    return 0;
  }

  if (readsockint4(sock, pport))
    return (ERROR);

  if (readsockint4(sock, pnode_type))
    return (ERROR);

  if (readsockint4(sock, pnum_cpus))
    return (ERROR);

  /* Suggestion/modified patch from Tim Mattox (KLAT group) to pass
     around host *names* instead of IP addresses.  Necessary for
     non-uniform networks (e.g., KLAT flat networks) where IP addr
     lookup for name "foo" may result in different answers on
     different nodes.  If name_len > 0, then read that many chars from
     the remote side and then do a lookup on that name to get the
     address.  Otherwise, just read the raw address from the other
     side. */

  if (readsockint4(sock, &name_len))
    return (ERROR);
  if (name_len < 0) {
    errno = EINVAL;
    return (ERROR);
  } else if (name_len > MAXHOSTNAMELEN) {
    errno = ENAMETOOLONG;
    return (ERROR);
  }
  if (name_len > 0) {
    char buf[MAXHOSTNAMELEN + 1];
    ret = mread(sock, buf, name_len);
    if (ret < name_len) {
      errno = (ret < 0) ? errno : EIO;
      return (ERROR);
    }
    buf[name_len] = '\0';
    if (getinetaddr(buf, phostaddr)) 
      return (ERROR);
  } else {
    ret = mread(sock, (char *) phostaddr, 4);
    if (ret < 4) {
      errno = (ret < 0) ? errno : EIO;
      return (ERROR);
    }
  }

  return 0;
}

/*
 *	writecltnbr
 *
 *	Function:	- write client neighbour <link index, hostaddr, port, 
 *                                                ncpus>
 *			- called by the booting server
 *	Accepts:	- socket
 *			- link index
 *			- host address (4 bytes, in network byte order)
 *			- port number
 *			- num cpus
 *	Returns:	- 0 or ERROR
 */
int
writecltnbr(int sock, int4 link, int is_hostname, 
	    unsigned char *hostaddr_or_name, int4 port, 
	    int4 node_type, int4 num_cpus)
{
  int ret;
  int4 name_len;

  if (writesockint4(sock, link))
    return (ERROR);

  /* If the link is an invalid node, nothing else is sent */

  if (link == NOTNODEID)
    return 0;

  if (writesockint4(sock, port))
    return (ERROR);
  
  if (writesockint4(sock, node_type))
    return (ERROR);

  if (writesockint4(sock, num_cpus))
    return (ERROR);

  /* Suggestion/modified patch from Tim Mattox (KLAT group) to pass
     around host *names* instead of IP addresses.  See comment above
     for explanation. */
  /* Note that it is acceptable to have NULL for the hostaddr_or_name
     entry; this may happen if we are passing information about
     invalid nodes (e.g., nodes that have died). */

  name_len = is_hostname ? strlen((char*) hostaddr_or_name) : 0;

  /* Send the name_len, even if it's too long, so that both sides will
     abort cleanly. */

  if (writesockint4(sock, name_len)) 
    return(ERROR);
  
  if (name_len > MAXHOSTNAMELEN) {
    errno = ENAMETOOLONG;
    return (ERROR);
  } else if (name_len < 0) {
    errno = EINVAL;
    return (ERROR);
  }
  
  /* If it's an address, hardwire the length to send to 4 */

  if (!is_hostname)
    name_len = 4;
  ret = mwrite(sock, (char *) hostaddr_or_name, name_len);
  if (ret < name_len) {
    errno = (ret < 0) ? errno : EIO;
    return LAMERROR; 
  }

  return 0;
}
