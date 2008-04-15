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
 * $Id: rversion.c,v 1.7 2003/03/23 22:21:27 jsquyres Exp $
 *
 *	Function:	- gets the version of the lamd
 *	Accepts:	- pointer to version string to return to caller
 *                      - pointers to integers for version components
 *	Returns:	- 0 if our version matches, LAMERROR otherwise
 */

#include <lam_config.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <etc_misc.h>
#include <vreq.h>
#include <events.h>
#include <net.h>
#include <portable.h>
#include <typical.h>


/*
 * private variables
 */
static char cached_string[MAXNMSGLEN];
static int cached_major = -1;
static int cached_minor = -1;
static int cached_release = -1;
static int cached_alpha = -1;
static int cached_beta = -1;


/*
 * local functions
 */
static int get_lamd_version(void);


int
rversion(char **version_string, int *major, int *minor, int *release,
	 int *alpha, int *beta)
{
  /* Check params */

  if (version_string == NULL && major == NULL && minor == NULL &&
      release == NULL && alpha == NULL && beta == NULL) {
    errno = EINVAL;
    return LAMERROR;
  }

  /* If we haven't yet queried the lamd for its version, do so */

  if (cached_major == -1)
    if (get_lamd_version() != 0)
      return LAMERROR;

  /* Fill in the desired parameters */

  if (version_string != NULL)
    *version_string = cached_string;
  if (major != NULL)
    *major = cached_major;
  if (minor != NULL)
    *minor = cached_minor;
  if (release != NULL)
    *release = cached_release;
  if (alpha != NULL)
    *alpha = cached_alpha;
  if (beta != NULL)
    *beta = cached_beta;

  /* Return 0 if we're the same version, LAMERROR otherwise */

  if (strcmp(cached_string, LAM_VERSION) != 0) {
    errno = EFAULT;
    return LAMERROR;
  }
  
  return 0;
}


static int
get_lamd_version(void)
{
  struct nmsg nhead;
  struct vreq *request;
  struct vreply *reply;

  /* Fill in versiond request. */

  LAM_ZERO_ME(nhead);
  request = (struct vreq *) nhead.nh_data;
  request->vq_event = -lam_getpid();
  request->vq_node = getnodeid();
  nhead.nh_node = getnodeid();
  nhead.nh_length = 0;
  nhead.nh_event = EVVERSIOND;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_msg = 0;

  /* Send request to the version daemon */

  if (nsend(&nhead))
    return LAMERROR;
  
  nhead.nh_event = -lam_getpid();
  nhead.nh_msg = cached_string;
  nhead.nh_length = MAXNMSGLEN;

  /* Received version message */

  memset(cached_string, 0, MAXNMSGLEN);
  if (nrecv(&nhead))
    return LAMERROR;

  /* What did we get? */

  reply = (struct vreply *) nhead.nh_data;
  cached_major = reply->vrp_major;
  cached_minor = reply->vrp_minor;
  cached_release = reply->vrp_release;
  cached_alpha = reply->vrp_alpha;
  cached_beta = reply->vrp_beta;

  return 0;
}
