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
 *	Ohio Trollius
 *	Copyright 1996 The Ohio State University
 *	GDB
 *
 *	$Id: ldolinks.c,v 6.9 2002/10/09 21:00:46 brbarret Exp $
 *
 *	Function:	- client functions for dlo link services
 */

#include <lam_config.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <dl_inet.h>
#include <etc_misc.h>
#include <events.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 *	ldogetlinks
 *
 *	Function:	- gets link information from dlo_inet
 *	Accepts:	- # links (value returned)
 *			- link array, malloc'd and filled (value returned)
 */
int
ldogetlinks(struct dolink **ppdol, int4 *pndol)
{
	struct doreq	*pdoq;		/* dlo request */
	struct doreply	*pdor;		/* dlo reply */
	struct nmsg	request;	/* request message */
	struct nmsg	reply;		/* reply message */
	int		eps;		/* # entries per segment */
	int		num;		/* # entries left to recv */
	int		seg;		/* segment number */
	int		len;		/* segment number */
	char		packet[MAXNMSGLEN]; /* segment number */

	LAM_ZERO_ME(request);
	LAM_ZERO_ME(reply);

	/* Recoded the protocol 24 June 2001.  When the routing table
	   in the lamd is very large (i.e., over MAXNMSGLEN -- this
	   can happen when you lamboot a large number of hosts), it
	   will need to be sent in multipe packets.  However, the
	   nsend in the lamd currently doesn't packetize because of
	   the severe hand-threading limitations.  Hence, the router
	   can essentially only send back *one* response -- which may
	   be less than the entire table.  So the protocol has changed
	   to ask for a single "segment" of the router table, where
	   each segment is as many entries will fit in <MAXNMSGLEN.
	   The response from the router will be that segment, as well
	   as the total number of segments in the table so that
	   ldogetlinks will know how many segments to ask for.  So we
	   start by asking for the 0th segment of the router table.
	   If the reply comes back that there are more segments, we
	   loop over asking for the rest.  A further change was
	   necessitated by the fact that having two nsends in a single
	   iteration seems to cause problems in the lamd message
	   passing engine (I suspect that the lamd One True Receive
	   Request gets overwritten during the first nsend, causing
	   Badness for the second nsend).  So change the protocol to
	   only one nsend -- make it MAXNMSGLEN length long, and pack
	   the requested segment into it, as well as the total number
	   of segments.  This seems to work.  */

	pdoq = (struct doreq *) request.nh_data;
	pdoq->doq_req = DOQGETLINKS;
	pdoq->doq_src_event = -lam_getpid();

	/* Initialize num to 0, even though we'll set it properly
	   during the first iteration of the do loop below.  But gcc
	   doesn't doesn't seem to realize this and gives a warning.
	   So initialize it to 0 just to ditch the warning.  <sigh> */

	num = seg = 0;

	request.nh_node = LOCAL;
	request.nh_event = EVDL0;
	request.nh_type = DLCTL;
	request.nh_flags = NOBUF;
	request.nh_length = 0;
	request.nh_msg = 0;

	pdor = (struct doreply *) reply.nh_data;
	reply.nh_node = LOCAL;
	reply.nh_event = pdoq->doq_src_event;
	reply.nh_type = 0;
	reply.nh_flags = 0;
	reply.nh_length = MAXNMSGLEN;
	reply.nh_msg = packet;

	eps = MAXNMSGLEN / sizeof(struct dolink);

	/* Loop over requesting / receiving until we receive the whole
           table */

	do {
	  pdoq->doq_extra = seg;

	  /* Send the request */

	  if (nsend(&request)) 
	    return(LAMERROR);

	  /* Receive the size */

	  if (nrecv(&reply)) 
	    return(LAMERROR);
	  if (pdor->dor_reply != 0) {
	    errno = pdor->dor_reply;
	    return(LAMERROR);
	  }

	  /* If this is the first segment, we need to allocate space
             for the returned table */

	  if (seg == 0) {
	    num = *pndol = pdor->dor_nlinks;

	    *ppdol = (struct dolink *) malloc((unsigned) num * 
					      sizeof(struct dolink));
	    if (*ppdol == 0) 
	      return(LAMERROR);
	  }

	  /* Calculate the length that we'll receive -- either a full
             segment, or, if this is the last semgment, something less
             than a full segment */

	  if (num > eps)
	    len = eps * sizeof(struct dolink);
	  else
	    len = num * sizeof(struct dolink);
	  
	  /* memcpy the segment into the right place */

	  memcpy(*ppdol + (*pndol - num), packet, len);
	  num -= eps;
	  ++seg;
	} while (num > 0);

	return 0;
}

/*
 *	ldosetlink
 *
 *	Function:	- sets link information in dlo_inet
 *	Accepts:	- link information
 */
int
ldosetlink(struct dolink *pdol)
{
	struct doreq	*pdoq;		/* dlo request */
	struct nmsg	nhq;		/* request message */

	pdoq = (struct doreq *) nhq.nh_data;
	pdoq->doq_req = DOQSETLINK;

	nhq.nh_node = LOCAL;
	nhq.nh_event = EVDL0;
	nhq.nh_type = DLCTL;
	nhq.nh_flags = NOBUF;
	nhq.nh_length = sizeof(struct dolink);
	nhq.nh_msg = (char *) pdol;

	return(nsend(&nhq));
}

/*
 *	ldoremlink
 *
 *	Function:	- removes link information in dlo_inet
 *	Accepts:	- link number
 */
int
ldoremlink(int4 link)
{
	struct doreq	*pdoq;		/* dlo request */
	struct nmsg	nhq;		/* request message */

	pdoq = (struct doreq *) nhq.nh_data;
	pdoq->doq_req = DOQREMLINK;
	pdoq->doq_link = link;

	nhq.nh_node = LOCAL;
	nhq.nh_event = EVDL0;
	nhq.nh_type = DLCTL;
	nhq.nh_flags = NOBUF;
	nhq.nh_length = 0;
	nhq.nh_msg = 0;

	return(nsend(&nhq));
}
