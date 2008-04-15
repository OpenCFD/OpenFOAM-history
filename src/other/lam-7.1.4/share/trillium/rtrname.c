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
 *	Copyright 1997 The Ohio State University
 *	NJN
 *
 *	$Id: rtrname.c,v 6.8 2002/10/09 21:00:48 brbarret Exp $
 *
 *	Function:	- traced name publishing requests
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <etc_misc.h>
#include <events.h>
#include <lam_ksignal.h>
#include <net.h>
#include <portable.h>
#include <terror.h>
#include <trreq.h>
#include <typical.h>

/*
 *	lam_rtrnamepub
 *
 *	Function:	- publish a name
 *	Accepts:	- node to publish on
 *			- ID to publish under
 *			- name
 *			- data associated with name
 *	Returns:	- 0 or LAMERROR
 */
int
lam_rtrnamepub(node, id, name, data)

int4			node;
int4			id;
char			*name;
char			*data;

{
	struct nmsg	nhead;			/* network message desc. */
	struct trreq  	*request;		/* traced request */
	struct trreply	*reply;			/* traced reply */
	char		*buf;			/* name and data buffer */
	int4		len;			/* length of name + data */
	int4		mask;			/* signal mask */
/*
 * The name and data are stored contiguously, each terminated with a
 * null character.  
 */
	len = strlen(name) + strlen(data) + 2;
	if (len > MAXNMSGLEN) {
		errno = ENAMETOOLONG;
		return(LAMERROR);
	}

	if ((buf = (char *) malloc(len)) == 0) {
		return(LAMERROR);
	}

	strcpy(buf,  name);
	strcpy(buf + strlen(name) + 1, data);

	request = (struct trreq *) nhead.nh_data;
	request->trq_src_node = (node == LOCAL) ? node : getnodeid();
	request->trq_src_event = -lam_getpid();
	request->trq_req = TRQNAMEPUB;
 	request->trq_pid = id;
	request->trq_length = len;
	
	nhead.nh_node = node;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = len;
	nhead.nh_msg = buf;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}
	free(buf);
/*
 * Receive traced reply.
 */
	nhead.nh_event = -lam_getpid();
	nhead.nh_type = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}
	ksigsetmask(mask);

	reply = (struct trreply *) nhead.nh_data;
	if (reply->trr_errno) {
		errno = reply->trr_errno;
		return(LAMERROR);
	}

	return(0);
}

/*
 *	lam_rtrnamedel
 *
 *	Function:	- delete a published name
 *	Accepts:	- node published on
 *			- name
 *	Returns:	- 0 or LAMERROR
 */
int
lam_rtrnamedel(node, name)

int4			node;
char			*name;

{
	struct nmsg	nhead;			/* network message desc. */
	struct trreq  	*request;		/* traced request */
	struct trreply	*reply;			/* traced reply */
	int4		mask;			/* signal mask */

	request = (struct trreq *) nhead.nh_data;
	request->trq_src_node = (node == LOCAL) ? node : getnodeid();
	request->trq_src_event = -lam_getpid();
	request->trq_req = TRQNAMEDEL;
	
	nhead.nh_node = node;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = strlen(name) + 1;
	nhead.nh_msg = name;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}
/*
 * Receive traced reply.
 */
	nhead.nh_event = -lam_getpid();
	nhead.nh_type = 0;
	nhead.nh_length = 0;
	nhead.nh_msg = 0;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}
	ksigsetmask(mask);

	reply = (struct trreply *) nhead.nh_data;
	if (reply->trr_errno) {
		errno = reply->trr_errno;
		return(LAMERROR);
	}

	return(0);
}

/*
 *	lam_rtrnameget
 *
 *	Function:	- get data associated with published name
 *			- space is allocated for the returned data
 *	Accepts:	- node name published on
 *			- name
 *			- data associated with name (returned)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_rtrnameget(node, name, data)

int4			node;
char			*name;
char			**data;

{
	struct nmsg	nhead;			/* network message desc. */
	struct trreq  	*request;		/* traced request */
	struct trreply	*reply;			/* traced reply */
	char		buf[MAXNMSGLEN];	/* reply data buffer */
	int4		mask;			/* signal mask */

	request = (struct trreq *) nhead.nh_data;
	request->trq_src_node = (node == LOCAL) ? node : getnodeid();
	request->trq_src_event = -lam_getpid();
	request->trq_req = TRQNAMEGET;
	
	nhead.nh_node = node;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = strlen(name) + 1;
	nhead.nh_msg = name;

	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}
/*
 * Receive traced reply.
 */
	nhead.nh_event = -lam_getpid();
	nhead.nh_type = 0;
	nhead.nh_length = MAXNMSGLEN;
	nhead.nh_msg = buf;

	if (nrecv(&nhead)) {
		ksigsetmask(mask);
		return(LAMERROR);
	}
	ksigsetmask(mask);

	reply = (struct trreply *) nhead.nh_data;
	if (reply->trr_errno) {
		errno = reply->trr_errno;
		return(LAMERROR);
	}

	if ((*data = (char *) malloc(reply->trr_length)) == 0) {
		return(LAMERROR);
	}

	strcpy(*data, buf);
	return(0);
}

/*
 *	lam_rtrnamesweep
 *
 *	Function:	- sweep away names
 *			- names are selected by node and ID
 *	Accepts:	- node
 *			- ID
 *	Returns:	- 0 or LAMERROR
 */
int
lam_rtrnamesweep(node, id)

int4			node;
int4			id;

{
	struct nmsg	nhead;			/* network message desc. */
	struct trreq  	*request;		/* traced request */

	request = (struct trreq *) nhead.nh_data;
	request->trq_req = TRQSWEEP;
 	request->trq_pid = id;
	request->trq_lnum = TRNAMELIST;

	LAM_ZERO_ME(nhead);
	nhead.nh_node = node;
	nhead.nh_event = EVTRACED;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = 0;

	if (nsend(&nhead)) {
		return(LAMERROR);
	}

	return(0);
}
