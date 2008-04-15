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
 * $Id: lam_gethostby.c,v 1.5 2003/07/17 18:02:58 jsquyres Exp $
 * 
 *	Function:	- Wrapper for gethostby* for machines where
 *                        the functions don't work (Scyld)
 */

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/param.h>

#include <arpa/inet.h>

#include <lam_config.h>
#include <laminternal.h>
#include <lam_network.h>

#if LAM_HAVE_BPROC && LAM_HAVE_BPROC3_API
#include <sys/bproc.h>

static struct hostent h_byname;
static struct hostent h_byaddr;

static char h_byname_name[MAXHOSTNAMELEN + 1];
static char h_byaddr_name[MAXHOSTNAMELEN + 1];

static char *h_byname_list[2];
static char *h_byaddr_list[2];

static char h_byname_addr[4];
static char h_byaddr_addr[4];

#endif /* if LAM_HAVE_BPROC && LAM_HAVE_BPROC3_API */


struct hostent *
lam_gethostbyname(const char *name)
{
#if LAM_HAVE_BPROC && LAM_HAVE_BPROC3_API
  int len;
  int node;
  struct sockaddr_in s;
  int s_len;
  char *tmp;
  struct hostent *name_resolver;

  /* Special case for "localhost", mainly for the default
     lam-bhost.def */

  node = bproc_getnodebyname(name);
  if (node <= BPROC_NODE_NONE) {
    if (strcmp(name, "localhost") == 0)
      node = bproc_currnode();
    else {
      name_resolver = gethostbyname(name);
      if (name_resolver == NULL)
	return NULL;

      memcpy(&s.sin_addr, name_resolver->h_addr_list[0], 4);
      s.sin_family = AF_INET;
      node = bproc_nodenumber((struct sockaddr *) &s, sizeof(s));
    }
  } 

  /* Set up the hostname */
  
  /* sizeof(int) * 8 will always be bigger than the decimal
     representation of anything stored in an int. */
  
  len = sizeof(char) * (sizeof(int) * 8 + 1);
  tmp = malloc(len);
  if (tmp == 0)
    return NULL;
  
  snprintf(tmp, len, "%d", node);
  
  if (MAXHOSTNAMELEN <= strlen(tmp)) {
    free(tmp);
    return NULL;
  }

  strcpy(h_byname_name, tmp);
  free(tmp);

  /* Set up the addr */

  s_len = sizeof(s);
  if (bproc_nodeaddr(node, (struct sockaddr*) &s, &s_len))
    return 0;

  memcpy(h_byname_addr, &s.sin_addr, 4);
  h_byname_list[0] = h_byname_addr;
  h_byname_list[1] = (char*) 0;

  h_byname.h_name = h_byname_name;
  h_byname.h_aliases = 0;
  h_byname.h_addrtype = AF_INET;
  h_byname.h_length = 4;
  h_byname.h_addr_list = h_byname_list;

  return &h_byname;
#else
  return gethostbyname(name);
#endif
}


struct hostent *
lam_gethostbyaddr(const char *addr, int len, int type)
{
#if LAM_HAVE_BPROC && LAM_HAVE_BPROC3_API

  int node;
  struct sockaddr_in s;
  int s_len;
  char *tmp;

  /* This is just silly, but the only solution I can find... */
  memcpy(&s.sin_addr, addr, 4);
  s.sin_family = AF_INET;

  node = bproc_nodenumber((struct sockaddr*) &s, sizeof(s));
  if (node == BPROC_NODE_NONE)
    return 0;

  /* Set up the hostname */

  /* I'm a sucker, special case the master.  ONLY because it will work
     with bproc_getnodebyaddr() */
  if (node == BPROC_NODE_MASTER) {
    tmp = strdup("master");
  } else {
    tmp = malloc(sizeof(char) * (sizeof(int) * 8 + 1));
    if (tmp == 0)
      return 0;

    sprintf(tmp, "%d", node);

    if (MAXHOSTNAMELEN <= strlen(tmp)) {
      free(tmp);
      return 0;
    }
  }

  strcpy(h_byaddr_name, tmp);
  free(tmp);

  /* Set up the addr */

  s_len = sizeof(s);
  if (bproc_nodeaddr(node, (struct sockaddr*) &s, &s_len))
    return 0;

  memcpy(h_byaddr_addr, &s.sin_addr, 4);
  h_byaddr_list[0] = h_byaddr_addr;
  h_byaddr_list[1] = (char*) 0;

  h_byaddr.h_name = h_byaddr_name;
  h_byaddr.h_aliases = 0;
  h_byaddr.h_addrtype = AF_INET;
  h_byaddr.h_length = 4;
  h_byaddr.h_addr_list = h_byaddr_list;

  return &h_byaddr;
#else
  return gethostbyaddr(addr, len, type);
#endif
}

