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
 *	$Id: pqdoom.c,v 6.6 2002/10/09 20:56:43 brbarret Exp $
 *
 *	Function:	- kenyad
 *			- delivers a signal to local processes
 */

#include <kio.h>
#include <preq.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>

/*
 * public functions
 */
void			pqdoom();
void			pdoom();

/*
 * external variables
 */
extern struct pprocess	ptable[];	/* kenya process table */
extern struct preq	pq;		/* kenya request */
extern struct preply	pr;		/* kenya reply */

/*
 * external functions
 */
extern void		lam_cleanup_objects();
extern void		pdetachindex();
extern void		psendr();
extern int		pmatch();

/*
 *	pqdoom
 *
 *	Function:	- signal delivery server
 */
void
pqdoom()

{
	pdoom(&pq, &pr);
/*
 * Send reply of doom operation back to client.
 */
	psendr((char *) &pr, sizeof(pr), 0);
}

/*
 *	pdoom
 *
 *	Function:	- delivers signals to matching processes
 *			- also called internally in lamd
 *			- checks for a match on each used process descriptor
 *			- signals each matched process using kdoom()
 *			- if no process is matched and a pid is supplied,
 *			  attempts to doom process with the kernel only
 */
void
pdoom(request, reply)

struct preq		*request;
struct preply		*reply;

{
	int		i;
	int		found_match;	/* did anything match? */

	found_match = FALSE;
	reply->pr_reply = 0;

	for (i = 0; (i < PMAX) && !pr.pr_reply; ++i) {

		if (!pmatch(ptable + i, request->pq_sel_method,
				request->pq_sel_value)) continue;

		found_match = TRUE;
/*
 * Signal process.
 */
		if (kdoom(ptable[i].p_pid, request->pq_signal)) {

			if (errno != ESERVICE) {

				if (!(ptable[i].p_rtf & RTF_KENYA_CHILD) &&
				    !(ptable[i].p_rtf & RTF_KENYA_ATTACH)) {
					pdetachindex(i);
					found_match = FALSE;
				} else {
					reply->pr_reply = ltot(errno);
				}
			}
		}
	}
/*
 * Mop up any persistent RPI state when killing all user processes.
 */
	if (request->pq_sel_method == SELECT_APPL
			&& request->pq_signal == SIGUDIE) {
		lam_cleanup_objects();
	}

	if (!found_match && ((request->pq_sel_method == SELECT_PID) ||
			(request->pq_sel_method == SELECT_INDEX))) {
		reply->pr_reply = ltot(ENOTPROCESS);
	}
}
