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
 * $Id: versiond.c,v 1.5 2003/09/27 05:16:16 brbarret Exp $
 * 
 *	Function:	- version server
 *			- receives a message and sends back the LAM 
 *                        version number
 *			- used to ensure all LAM componoents agree 
 *                        version-wise
 */

#include <lam_config.h>

#include <string.h>

#include <events.h>
#include <vreq.h>
#include <net.h>
#include <preq.h>
#include <terror.h>


/*
 * local variables
 */
static char msg[MAXNMSGLEN];
static int str_len;
static struct nmsg nhead;

/*
 * global functions
 */
void (*(v_init()))();
void (*(versiond()))();

/*
 *	v_init
 *
 *	Function:	- versiond initialization
 */
void (*(
v_init()))()
{
  str_len = strlen(LAM_VERSION) + 1;
  memcpy(msg, LAM_VERSION, str_len);
  msg[str_len + 1] = '\0';

  /* Attach to kernel */

  if (lpattach("versiondd"))
    lampanic("versiond (lpattach)");

  /* Receive first request */

  LAM_ZERO_ME(nhead);
  nhead.nh_event = EVVERSIOND;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_length = 0;
  nhead.nh_msg = 0;

  if (nrecv(&nhead))
    lampanic("versiond (nrecv)");

  return((void (*)()) versiond);
}

/*
 *	versiond
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(versiond()))()
{
  struct vreq *request;
  struct vreply *reply;
  
  request = (struct vreq *) nhead.nh_data;
  reply = (struct vreply *) nhead.nh_data;

  /* Reply to the request. */

  nhead.nh_node = request->vq_node;
  nhead.nh_event = request->vq_event;
  nhead.nh_flags = 0;
  nhead.nh_length = str_len;
  nhead.nh_msg = msg;

  reply->vrp_major = LAM_MAJOR_VERSION;
  reply->vrp_minor = LAM_MINOR_VERSION;
  reply->vrp_release = LAM_RELEASE_VERSION;
  reply->vrp_alpha = LAM_ALPHA_VERSION;
  reply->vrp_beta = LAM_BETA_VERSION;
  
  if (nsend(&nhead))
    lampanic("versiond (nsend)");

  /* Receive a request. */

  nhead.nh_event = EVVERSIOND;
  nhead.nh_type = 0;
  nhead.nh_flags = 0;
  nhead.nh_length = 0;
  nhead.nh_msg = 0;

  if (nrecv(&nhead))
    lampanic("versiond (nrecv)");

  return((void (*)()) versiond);
}
