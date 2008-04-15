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
 *	$Id: rfrename.c,v 1.1 2003/06/30 05:19:30 brbarret Exp $
 *
 *	Function:	- renames a file using remote filed
 *			- server nodeid may be specified by prefixing
 *			  filename with <nodeid>: as in 100:temp
 *                      - prefix must match in from and to
 *	Accepts:	- source file name
 *			- target file name
 *	Returns:	- 0 on success, non-zero otherwise
 */

#include <string.h>

#include "etc_misc.h"
#include "events.h"
#include "freq.h"
#include "ksignal.h"
#include "terror.h"


/*
 * external functions
 */
extern char		*_fnparse();	/* parse nodeid from file name */


int
lam_rfrename(CONST char *rffrom, const char *rfto)
{
  char *from_name;
  char *to_name;
  int4 from_node;
  int4 to_node;
  int mask; /* signal mask */

  struct nmsg	nhead;		/* network message desc. */
  struct freq	*request;	/* filed request */
  struct freply	*reply;		/* filed reply */
  struct fqrename data;

  from_name = _fnparse(rffrom, &from_node);
  to_name = _fnparse(rfto, &to_node);

  if (from_node != to_node) {
    errno = EBADREQUEST;
    return LAMERROR;
  }

  request = (struct freq *) nhead.nh_data;
  reply = (struct freply *) nhead.nh_data;

  nhead.nh_node = from_node;
  nhead.nh_event = EVFILED;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_length = sizeof(struct fqrename);

  request->fq_src_node = getnodeid();
  request->fq_src_event = -lam_getpid();
  request->fq_req = FQRENAME;
  request->fq_mode = 0;
  request->fq_flags = 0;

  lam_strncpy(data.source, from_name, MAXPATHLEN);
  lam_strncpy(data.dest, to_name, MAXPATHLEN);

  nhead.nh_msg = (char*) &data;

  mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

  if (nsend(&nhead)) {
    ksigsetmask(mask);
    return(LAMERROR);
  }

  nhead.nh_event = -lam_getpid();
  nhead.nh_length = 0;
  nhead.nh_msg = 0;

  if (nrecv(&nhead)) {
    ksigsetmask(mask);
    return(LAMERROR);
  }

  ksigsetmask(mask);

  if (reply->fr_errno != 0) {
    errno = reply->fr_errno;
    return LAMERROR;
  }

  return 0;
}
