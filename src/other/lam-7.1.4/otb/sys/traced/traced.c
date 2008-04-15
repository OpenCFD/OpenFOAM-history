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
 *      Ohio Trollius
 *      Copyright 1997 The Ohio State University
 *      GDB
 *
 *      $Id: traced.c,v 1.2 2002/10/09 20:56:52 brbarret Exp $
 *
 *	Function:	- trace daemon
 *			- manages local trace data
 *			- transport mechanism
 *			- prefix code is tr
 */

#include <stdlib.h>
#include <string.h>

#include <lam_config.h>
#include <events.h>
#include <net.h>
#include <preq.h>
#include <priority.h>
#include <terror.h>
#include <trreq.h>

/*
 * global functions
 */
void			(*(tr_init()))();
void			(*(traced()))();
void			trsendr();	/* send traced reply */

/*
 * external functions
 */
extern void		trbinit();	/* initialize trace structure */
extern void		trqstore();	/* store trace data */
extern void		trqget();	/* unload trace data */
extern void		trqsweep();	/* remove all traces */
extern void		trqudie();	/* remove pid related traces */
extern void		trqwipe();	/* reset traced */
extern void		trqnamepub();	/* publish name */
extern void		trqnamedel();	/* delete name */
extern void		trqnameget();	/* get name */
extern void		trqcleanobjs();	/* cleanup objects */

/*
 * global variables
 */
char			trqmsg[MAXNMSGLEN];
					/* trace buffer */

/*
 * local variables
 */
static void		(*trqfunc[TRQMAX])() = {
				trqstore,
				trqget,
				trqget,
				trqsweep,
				trqudie,
				trqwipe,
				trqnamepub,
				trqnamedel,
				trqnameget,
				trqcleanobjs,
			};

static struct nmsg	trqhead;	/* request message */

/*
 *	tr_init
 *
 *	Function:	- traced initialization
 */
void (*(
tr_init()))()

{
	if (lpattach("traced")) lampanic("traced (lpattach)");
/*
 * Initialize trace module.
 */
	trbinit();
/*
 * Receive first request.
 */
	LAM_ZERO_ME(trqhead);
	trqhead.nh_event = EVTRACED;
	trqhead.nh_type = 0;
	trqhead.nh_flags = 0;
	trqhead.nh_length = MAXNMSGLEN;
	trqhead.nh_msg = trqmsg;

	if (nrecv(&trqhead)) lampanic("traced (nrecv)");

	return((void (*)()) traced);
}

/*
 *	traced
 *
 *	Function:	- server loop
 *			- replies message & receives next request
 */
void
(*(traced()))()

{
	struct trreq	*ptrq;			/* incoming request */

	ptrq = (struct trreq *) trqhead.nh_data;

	if ((ptrq->trq_req < 0) || (ptrq->trq_req >= TRQMAX)) {
		errno = EBADREQUEST;
		lampanic("traced");
	}

	(*trqfunc[ptrq->trq_req])(ptrq);

	trqhead.nh_event = EVTRACED;
	trqhead.nh_type = 0;
	trqhead.nh_flags = 0;
	trqhead.nh_length = MAXNMSGLEN;
	trqhead.nh_msg = trqmsg;

	if (nrecv(&trqhead)) lampanic("traced (nrecv)");

	return((void (*)()) traced);
}

/*
 *	trsendr
 *
 *	Function:	- sends a traced reply to a client
 *
 *	Accepts:	- ptr to traced request
 *			- ptr to traced reply
 *			- reply msg
 *			- length of reply message
 */
void
trsendr(ptrq, ptrr, msg, len)

struct trreq		*ptrq;
struct trreply		*ptrr;
char			*msg;
int4			len;

{
	struct nmsg	nhead;		/* reply descriptor */

	LAM_ZERO_ME(nhead);
	memcpy((char *) nhead.nh_data, (char *) ptrr,
			sizeof(struct trreply));

	nhead.nh_node = ptrq->trq_src_node;
	nhead.nh_event = ptrq->trq_src_event;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = len;
	nhead.nh_msg = msg;

	if (nsend(&nhead)) lampanic("traced (nsend)");
}
