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
 * $Id: lamnet.c,v 6.13 2004/02/16 22:20:52 jsquyres Exp $
 * 
 *	Function:	- LAM network functions
 */

#include <lam_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#ifndef SIOCGIFCONF
#include <sys/sockio.h>
#endif
#include <sys/time.h>

#if defined(__APPLE__) && defined(_LP64)
/* Apple engineering suggested using options align=power as a
   workaround for a bug in OS X 10.4 (Tiger) that prevented ioctl(...,
   SIOCGIFCONF, ...) from working properly in 64 bit mode on Power PC.
   It turns out that the underlying issue is the size of struct
   ifconf, which the kernel expects to be 12 and natural 64 bit
   alignment would make 16.  The same bug appears in 64 bit mode on
   Intel macs, but align=power is a no-op there, so instead, use the
   pack pragma to instruct the compiler to pack on 4 byte words, which
   has the same effect as align=power for our needs and works on both
   Intel and Power PC Macs. */
#pragma pack(push,4)
#endif
#include <net/if.h>
#if defined(__APPLE__) && defined(_LP64)
#pragma pack(pop)
#endif

#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <laminternal.h>
#include <lamnet.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <lam_network.h>
#include <etc_misc.h>

#define DEFAULT_NUMBER_INTERFACES 10
#define MAX_IFCONF_SIZE 10 * 1024 * 1024

/*
 * global functions
 */
int			lamnet_findhosts();
int			lamnet_findorig();
int			lamnet_dups();


/*
 * local functions
 */
static int getifaddr(uint4 **ifaddr);


/*
 *	lamnet_findhosts
 *
 *	Function:	- gets the network address of each host
 *			- "localhost" is changed to the local hostname
 *	Accepts:	- node description array
 *			- array size
 *			- bad host index (out)
 */
int
lamnet_findhosts(struct lamnode *lamnet, int nlamnet, int *badhost)
{
  int i;
  char buffer[16];

  if (badhost != NULL) 
    *badhost = -1;
  
  /* Find INET addresses for each node */
  
  for (i = 0; i < nlamnet; ++i) {

    /* Skip empty entries */

    if (lamnet[i].lnd_nodeid == NOTNODEID)
      continue;
    
    /* Get the host network address  */

    if (getinetaddr(lamnet[i].lnd_hname,
		    (unsigned char *) &lamnet[i].lnd_addr.sin_addr) != 0) {
      if (badhost != NULL) 
	*badhost = i;
      
      if (errno == EBADHOST)
	show_help("boot", "resolve-hostname", lamnet[*badhost].lnd_hname,
		  NULL);
      else {
	snprintf(buffer, 16, "%d", errno);
	show_help("boot", "resolve-unknown", buffer, NULL);
      }
      return(LAMERROR);
    }

    /* Nope; it resolved properly, so save it */

    lamnet[i].lnd_addr.sin_family = AF_INET;
  }

  return 0;
}


/*
 *	lamnet_findorig
 *
 *	Function:	- finds the origin host node
 *			- origin is the machine this code is running on
 *			- calls lamnet_findhosts() if necessary
 *			- do matching on the network addresses
 *			- make sure each host is unique (no duplicates)
 *	Accepts:	- network description array
 *			- array size
 *	Returns:	- index of origin or LAMERROR
 */
int
lamnet_findorig(struct lamnode *lamnet, int nlamnet)
{
	int		i, j;
	int		nifaddr;	/* # of interface addresses */
	uint4		*ifaddr;

	nifaddr = getifaddr(&ifaddr);
	if (nifaddr < 0) 
          return(LAMERROR);

/*
 * Loop over the host entries, comparing their addresses to
 * our local host interface addresses.
 */
	for (i = 0; i < nlamnet; ++i) {

          /* Skip empty entries */

          if (lamnet[i].lnd_nodeid == NOTNODEID)
            continue;

	  if (inet_netof(lamnet[i].lnd_addr.sin_addr) == 127) {
            free(ifaddr);
            return(i);
	  }

          for (j = 0; j < nifaddr; ++j) {
            if (ifaddr[j] == lamnet[i].lnd_addr.sin_addr.s_addr) {
              free(ifaddr);
              return(i);
            }
          }
	}

        free(ifaddr);
	errno = EINVAL;
	return(LAMERROR);
}


/*
 *	getifaddr
 *
 *	Function:	- obtains all up interface addresses for local host
 *	Accepts:	- INET address array (returned addresses)
 *	Returns:	- number of addresses or LAMERROR
 */
static int
getifaddr(uint4 **ifaddr)
{
	int		sock;
	int		length;
        int             lastlen;
	int		num;
	int		rem;
	char		*p;
	struct ifconf	config;
	struct ifreq	req, *preq;
	struct sockaddr_in *psin;
        int ifc_len;
        int successful_locate = 0;

/*
 * Create an Internet socket.
 */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) 
          return LAMERROR;
/*
 * Get network interface configuration.
 */
        /*
         * Some notes on the behavior of ioctl(..., SIOCGIFCONF,...)
         * when not enough space is allocated for all the entries.
         *
         * - Solaris returns -1, errno EINVAL if there is not enough
         *   space 
         * - OS X returns 0, sets .ifc_len to the space used by the
         *   by the entries that did fit.
         * - Linux returns 0, sets .ifc_len to the space required to
         *   hold all the entries (although it only writes what will
         *   fit in the buffer of .ifc_len passed to the function).
         * - FreeBSD returns 0, sets .ifc_len to 0.
         *
         * Everyone else seems to do one of the four.
         */
        lastlen = 0;
        ifc_len = sizeof(req) * DEFAULT_NUMBER_INTERFACES;
        do {
          config.ifc_len = ifc_len;
          config.ifc_req = malloc(ifc_len);
          if (config.ifc_req == NULL) {
            close(sock);
            return LAMERROR;
          }
          if (ioctl(sock, SIOCGIFCONF, &config) < 0) {
            /* if we got an einval, we probably don't have enough
               space.  so we'll fall down and try to expand our
               space */
	    if (errno != EINVAL && lastlen != 0) {
	      close(sock);
	      return LAMERROR;
	    } 
          } else {
            /* if config.ifc_len is 0, we obviously aren't done.  try
               again... */
	    if (config.ifc_len == lastlen && config.ifc_len > 0) {
	      /* we didn't expand.  we're done */
              successful_locate = 1;
	      break;
	    }
	    lastlen = config.ifc_len;
	  }

          /* Yes, we overflowed (or had an EINVAL on the ioctl).
          .  Loop back around and try again */
          free(config.ifc_req);
          ifc_len *= 2;
        } while (ifc_len < MAX_IFCONF_SIZE);
        if (0 == successful_locate) {
            fprintf(stderr, "lamnet_findorig: Unable to find network interfaces.\n");
            return LAMERROR;
        }

        /* Setup indexing */

	p = (char *) config.ifc_req;
	rem = config.ifc_len;
	num = 0;

        /* Malloc space for the return */

        *ifaddr = malloc(sizeof(uint4) * (rem / sizeof(req)));
        if (ifaddr == NULL) {
          close(sock);
          free(config.ifc_buf);
          return LAMERROR;
        }

        /* Loop through all interfaces. */

	while (rem > 0) {
		preq = (struct ifreq *) p;
#if LAM_HAVE_SA_LEN
		length = sizeof(struct sockaddr);

		if (preq->ifr_addr.sa_len > length) {
			length = preq->ifr_addr.sa_len;
		}

		length += sizeof(preq->ifr_name);
#else
		length = sizeof(struct ifreq);
#endif

		rem -= length;
		p += length;

		if (preq->ifr_addr.sa_family != AF_INET) 
                  continue;

		memcpy(req.ifr_name, preq->ifr_name, sizeof(req.ifr_name));

		if (ioctl(sock, SIOCGIFFLAGS, &req)) {
			close(sock);
                        free(ifaddr);
                        *ifaddr = NULL;
                        free(config.ifc_buf);
			return LAMERROR;
		}

		if (req.ifr_flags & IFF_UP) {
			psin = (struct sockaddr_in *) &(preq->ifr_addr);
			(*ifaddr)[num] = psin->sin_addr.s_addr;
			num++;
		}
	}

	close(sock);
        free(config.ifc_buf);
	return(num);
}
