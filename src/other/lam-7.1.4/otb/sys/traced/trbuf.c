/*
 * Copyright (c) 2001-2006 The Trustees of Indiana University.  
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
 *	Function:	- functions that manipulate the trace buffer
 *			  data structure
 */

#include <stdlib.h>
#include <string.h>

#include <all_hash.h>
#include <all_list.h>
#include <lamdebug.h>
#include <portable.h>
#include <terror.h>
#include <trreq.h>

/*
 * external variables
 */
extern char		trqmsg[];

/*
 * private variables
 */
static HASH		*trbhd;
static int4		totalspace;
static LIST		*namelist;

/*
 * external functions
 */
extern void		trsendr();
extern void		lam_cleanup_objects();

/*
 * private functions
 */
static int		name_cmp();

/*
 *	trbinit
 *
 *	Function:	- initializes trace buffer hash table
 *			  and creates name publishing trace list
 */
void
trbinit()

{
	struct trblist	newl;
	
	totalspace = 0;
	trbhd = ah_init(TRHASHSIZE, sizeof(struct trblist), -1, 0);
	if (trbhd == 0) lampanic("traced (ah_init)");

	namelist = al_init(sizeof(struct trbuffer), name_cmp);
	if (namelist == 0) lampanic("traced (al_init)");

	newl.trl_lnum = TRNAMELIST;
	newl.trl_ld = namelist;

	if (ah_insert(trbhd, &newl)) lampanic("traced (ah_insert)");
}

/*
 *	trqstore
 *
 *	Function:	- stores trace data from a client
 *			- no reply message
 *	Accepts:	- client pid
 */
void
trqstore(ptrq)

struct trreq		*ptrq;

{
	struct trblist	newl;
	struct trblist	*ptrl;
	struct trbuffer	newb;
	struct trbuffer	*ptrb;
	LIST		*ld;

	lamlog_cond(("traced: store: length=%d, list=%d, pid=%d\n", 
		     ptrq->trq_length, ptrq->trq_lnum, ptrq->trq_pid));
	if (ptrq->trq_length <= 0) return;
/*
 * Find the requested trace list.
 */
	ptrl = ah_find(trbhd, ptrq->trq_lnum);

	if (ptrl == 0) {
		LAM_ZERO_ME(newl);
		newl.trl_lnum = ptrq->trq_lnum;

		if (ah_insert(trbhd, &newl)) {

			if (errno != EFULL) {
				lampanic("traced (ah_insert)");
			}

			if (totalspace < TRMAXSPACE) {

				if (ah_expand(trbhd, ah_size(trbhd) +
						TRHASHSIZE))
						lampanic("traced (ah_expand)");

				if (ah_insert(trbhd, &newl))
						lampanic("traced (ah_insert)");
			} else {
				return;
			}
		}

		ptrl = ah_find(trbhd, ptrq->trq_lnum);
		ptrl->trl_ld = al_init(sizeof(struct trbuffer), (int (*)()) 0);
		if (ptrl->trl_ld == 0) lampanic("traced (al_init)");
	}

	ld = ptrl->trl_ld;
/*
 * Delete old traces.
 */
	if ((totalspace >= TRMAXSPACE) && (al_count(ld) > 0) &&
			(ptrq->trq_lnum >= 0)) {
		ptrb = al_top(ld);
		if (ptrb == 0) lampanic("traced (al_top)");

		lamlog_cond(("traced: store: deleting old traces, "
			     "length=%d, pid=%d\n",
			     ptrb->trb_length, ptrb->trb_pid));

		totalspace -= ptrb->trb_length;
		free(ptrb->trb_buffer);
		al_delete(ld, ptrb);
	}
/*
 * Allocate space for the traces.
 */
	newb.trb_pid = ptrq->trq_pid;
	newb.trb_length = ptrq->trq_length;

	newb.trb_buffer = malloc((unsigned) ptrq->trq_length);
	if (newb.trb_buffer == 0) lampanic("traced (malloc)");

	totalspace += newb.trb_length;
	memcpy(newb.trb_buffer, trqmsg, ptrq->trq_length);

	if (al_append(ld, &newb) == 0) lampanic("traced (al_append)");
}

/*
 *	trqget
 *
 *	Function:	- unloads trace data
 *			- if TRQGET, then maintain trace data
 *			- if TRQFORGET, then free trace data
 *			- sends trace buffers for given list and pid
 *			- list TRANYLIST means all lists
 *			- pid TRANYPID means all processes
 *	Accepts:	- traced request ptr
 */
void
trqget(ptrq)

struct trreq		*ptrq;

{
	struct trreply	reply;
	struct trbuffer	*next;
	struct trbuffer	*ptrb;
	struct trblist	*ptrl;

	reply.trr_errno = 0;
	reply.trr_length = 0;
/*
 * Calculate total length of selected traces.
 */
	ptrl = (ptrq->trq_lnum == TRANYLIST) ? ah_top(trbhd) :
			ah_find(trbhd, ptrq->trq_lnum);

	while (ptrl) {
		ptrb = al_top(ptrl->trl_ld);

		while (ptrb) {

			if ((ptrq->trq_pid == ptrb->trb_pid) ||
					(ptrq->trq_pid == TRANYPID)) {
				reply.trr_length += ptrb->trb_length;
			}

			ptrb = al_next(ptrl->trl_ld, ptrb);
		}

		ptrl = (ptrq->trq_lnum == TRANYLIST) ?
				ah_next(trbhd, ptrl) : NULL;
	}

	trsendr(ptrq, &reply, (char *) 0, 0);
/*
 * Send the selected traces.
 */
	ptrl = (ptrq->trq_lnum == TRANYLIST) ? ah_top(trbhd) :
			ah_find(trbhd, ptrq->trq_lnum);

	while (ptrl) {
		ptrb = al_top(ptrl->trl_ld);

		while (ptrb) {
			next = al_next(ptrl->trl_ld, ptrb);

			if ((ptrq->trq_pid == ptrb->trb_pid) ||
					(ptrq->trq_pid == TRANYPID)) {
				trsendr(ptrq, &reply, ptrb->trb_buffer,
						ptrb->trb_length);

				if (ptrq->trq_req == TRQFORGET) {
					totalspace -= ptrb->trb_length;
					free(ptrb->trb_buffer);
					al_delete(ptrl->trl_ld, ptrb);
				}
			}

			ptrb = next;
		}

		ptrl = (ptrq->trq_lnum == TRANYLIST) ?
				ah_next(trbhd, ptrl) : NULL;
	}
}

/*
 *	trqsweep
 *
 *	Function:	- sweeps out trace data
 *	Accepts:	- traced request ptr
 */
void
trqsweep(ptrq)

struct trreq		*ptrq;

{
	struct trblist	*ptrl;
	struct trbuffer	*ptrb;
	struct trbuffer	*next;

	ptrl = (ptrq->trq_lnum == TRANYLIST) ? ah_top(trbhd) :
			ah_find(trbhd, ptrq->trq_lnum);

	while (ptrl) {
		ptrb = al_top(ptrl->trl_ld);

		while (ptrb) {
			next = al_next(ptrl->trl_ld, ptrb);

			if ((ptrq->trq_pid == ptrb->trb_pid) ||
					(ptrq->trq_pid == TRANYPID)) {
				free(ptrb->trb_buffer);
				totalspace -= ptrb->trb_length;
				al_delete(ptrl->trl_ld, ptrb);
			}

			ptrb = next;
		}

		ptrl = (ptrq->trq_lnum == TRANYLIST) ?
				ah_next(trbhd, ptrl) : NULL;
	}
}

/*
 *	trqudie
 *
 *	Function:	- removes all traces related to a pid
 *	Accepts:	- traced request ptr
 */
void
trqudie(ptrq)

struct trreq		*ptrq;

{
	ptrq->trq_lnum = TRANYLIST;
	trqsweep(ptrq);
}

/*
 *	trqwipe
 *
 *	Function:	- completely resets state of traced
 *	Accepts:	- traced request ptr
 */
void
trqwipe(ptrq)

struct trreq		*ptrq;

{
	struct trblist	*ptrl;
	
	ptrq->trq_lnum = TRANYLIST;
	ptrq->trq_pid = TRANYPID;
	trqsweep(ptrq);
	ptrl = ah_top(trbhd);

	while ((ptrl = ah_top(trbhd))) {
		al_free(ptrl->trl_ld);
		ah_delete(trbhd, ptrl->trl_lnum);
	}
}

/*
 *	trqnamepub
 *
 *	Function:	- publishes a name
 *	Accepts:	- traced request ptr
 */
void
trqnamepub(ptrq)

struct trreq		*ptrq;

{
	struct trbuffer	newb;			/* new trace buffer */
	struct trreply	reply;			/* reply to client */

	LAM_ZERO_ME(reply);
	reply.trr_errno = 0;
	reply.trr_length = 0;

	newb.trb_buffer = trqmsg;
/*
 * Check for already published name.
 */
	if (al_find(namelist, &newb)) {
		reply.trr_errno = EPUBLISHED;
		trsendr(ptrq, &reply, (char *) 0, 0);
		return;
	}
/*
 * Add the name to the list of published names.
 */
	newb.trb_pid = ptrq->trq_pid;
	newb.trb_length = ptrq->trq_length;

	newb.trb_buffer = malloc((unsigned) ptrq->trq_length);
	if (newb.trb_buffer == 0) lampanic("traced (malloc)");

	totalspace += newb.trb_length;
	memcpy(newb.trb_buffer, trqmsg, ptrq->trq_length);

	if (al_append(namelist, &newb) == 0) lampanic("traced (al_append)");

	trsendr(ptrq, &reply, (char *) 0, 0);
}

/*
 *	trqnamedel
 *
 *	Function:	- deletes a name
 *	Accepts:	- traced request ptr
 */
void
trqnamedel(ptrq)

struct trreq		*ptrq;

{
	struct trbuffer	bsearch;		/* search buffer */
	struct trbuffer	*bfound;		/* found buffer */
	struct trreply	reply;			/* reply to client */

	reply.trr_errno = 0;
	reply.trr_length = 0;

	bsearch.trb_buffer = trqmsg;
/*
 * Find the name.
 */
	if ((bfound = al_find(namelist, &bsearch)) == 0) {
		reply.trr_errno = ENOTPUBLISHED;
		trsendr(ptrq, &reply, (char *) 0, 0);
		return;
	}
/*
 * Delete the name from the list of published names.
 */
	totalspace -= bfound->trb_length;
	free(bfound->trb_buffer);
	al_delete(namelist, bfound);
	trsendr(ptrq, &reply, (char *) 0, 0);
}	

/*
 *	trqnameget
 *
 *	Function:	- gets data associated with a published name
 *	Accepts:	- traced request ptr
 */
void
trqnameget(ptrq)

struct trreq		*ptrq;

{
	struct trbuffer	bsearch;		/* search buffer */
	struct trbuffer	*bfound;		/* found buffer */
	struct trreply	reply;			/* reply to client */
	char		*data;			/* data associated with name */

	reply.trr_errno = 0;

	bsearch.trb_buffer = trqmsg;
/*
 * Find the name.
 */
	if ((bfound = al_find(namelist, &bsearch)) == 0) {
		reply.trr_errno = ENOTPUBLISHED;
		trsendr(ptrq, &reply, (char *) 0, 0);
		return;
	}

	data = bfound->trb_buffer + strlen(bfound->trb_buffer) + 1;

	reply.trr_length = strlen(data) + 1;
	trsendr(ptrq, &reply, data, reply.trr_length);
}

/*
 *	trqcleanobs
 *
 *	Function:	- clean up registered objects
 *	Accepts:	- traced request ptr
 */
void
trqcleanobjs(ptrq)

struct trreq		*ptrq;

{
	lam_cleanup_objects();
}

/*
 *	name_cmp
 *
 *	Function:	- compare two names
 *	Accepts:	- ptr to two entries
 *	Returns:	- string comparison of the names
 */
static int
name_cmp(n1, n2)

struct trbuffer		*n1, *n2;

{
	return(strcmp(n1->trb_buffer, n2->trb_buffer));
}
