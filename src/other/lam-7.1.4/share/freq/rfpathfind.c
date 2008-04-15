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
 *	$Id: rfpathfind.c,v 1.3 2003/03/11 00:46:08 jsquyres Exp $
 *
 *	Function:	- find a file in the $PATH on a remote node
 *	Accepts:	- bare file name
 *	Returns:	- absolute file name
 */

#include <lam_config.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <events.h>
#include <net.h>
#include <ksignal.h>
#include <freq.h>
#include <typical.h>
#include <etc_misc.h>


/*
 * Local variables
 */
static char buf[MAXNMSGLEN];


char *
lam_rfpathfind(const char *name, const char *cwd, int nodeid)
{
  struct nmsg nhead;
  struct freq *request;
  struct freply	*reply;
  int mask;

  request = (struct freq *) nhead.nh_data;
  reply = (struct freply *) nhead.nh_data;

  LAM_ZERO_ME(nhead);
  request->fq_src_node = getnodeid();
  request->fq_src_event = -lam_getpid();
  request->fq_req = FQPATHFIND;
  
  nhead.nh_node = nodeid;
  nhead.nh_event = EVFILED;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_length = strlen(name) + 1;
  if (cwd != NULL)
    nhead.nh_length += strlen(cwd) + 1;

  if (nhead.nh_length > MAXNMSGLEN) {
    errno = ENAMETOOLONG;
    return NULL;
  }

  nhead.nh_msg = (char *) buf;
  if (cwd != NULL) {
    snprintf(buf, MAXNMSGLEN, "%s%c%s", name, '\0', cwd);
  } else {
    snprintf(buf, MAXNMSGLEN, "%s%c%c", name, '\0', '\0');
  }
  buf[MAXNMSGLEN - 1] = '\0';

  mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));
  
  if (nsend(&nhead)) {
    ksigsetmask(mask);
    return NULL;
  }
  
  nhead.nh_event = -lam_getpid();
  nhead.nh_length = MAXNMSGLEN;
  nhead.nh_msg = buf;
  
  if (nrecv(&nhead)) {
    ksigsetmask(mask);
    return NULL;
  }
  
  ksigsetmask(mask);
  
  if (reply->fr_errno != 0) {
    errno = reply->fr_errno;
    return NULL;
  }

  /* Found it */

  return buf;
}
