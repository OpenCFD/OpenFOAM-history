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
 *	Function:	- OTB kenyad
 *			- assembles status of processes
 *			- checks for a match on each used process descriptor
 *			- builds state and sends reply for each descriptor
 *			  that matches true
 *			- if no process is matched and a pid is supplied,
 *			  attempts to acquire state from the kernel
 *			- based on Trollius 2.0 Copyright 1990
 *			  The Ohio State University and Cornell
 *			  Research Foundation
 */

#include <string.h>

#include <events.h>
#include <kio.h>
#include <kreq.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <terror.h>
#include <typical.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct pprocess	ptable[];	/* kenya process table */
extern struct preq	pq;		/* kenya request */

/*
 * external functions
 */
extern void		pdetachindex();
extern void		psendr();
extern int		pmatch();

/*
 * local variables
 */
static struct pstate	reply_state;	/* kenya state reply */

void
pqstate()

{
	int		i;		/* favourite index */
	int		found_match;	/* did anything match? */
	struct pprocess	*pstate;	/* target process */

	reply_state.ps_reply = ltot(0);
	found_match = FALSE;

	if (pq.pq_maxproc <= 0) {
		reply_state.ps_reply = ltot(EINVAL);
		psendr((char *) &reply_state, sizeof(reply_state), 0);
		return;
	}

	for (i = 0; (i < PMAX) && (pq.pq_maxproc > 0); ++i) {
		pstate = ptable + i;

		if (!pmatch(pstate, pq.pq_sel_method, pq.pq_sel_value))
				continue;

		found_match = TRUE;
		pq.pq_maxproc--;
/*
 * Fill in the kenya info.
 */
		reply_state.ps_pid = ltot(pstate->p_pid);
		reply_state.ps_rtf = ltot(pstate->p_rtf);
		reply_state.ps_nodeid = ltot(pstate->p_nodeid);
		reply_state.ps_event = ltot(pstate->p_event);
/*
 * Fill in process index.
 */
		reply_state.ps_index = ltot(i + 1);
/*
 * Fill in argv[0], usually the program name, if it exists.
 */
		if (pstate->p_argc != 0) {
			lam_strncpy(reply_state.ps_name, *(pstate->p_argv),
					PSMAXNAME);
		} else {
			lam_strncpy(reply_state.ps_name, "<unknown>", 
                                    PSMAXNAME);
		}

		reply_state.ps_name[PSMAXNAME - 1] = '\0';
		lam_strncpy(reply_state.ps_loadpt, pstate->p_loadpt, 
                            PSMAXNAME);
		reply_state.ps_loadpt[PSMAXNAME - 1] = '\0';
/*
 * Fill in the kernel info.
 */
		if (kstate(pstate->p_pid, &reply_state.ps_kernel)) {
/*
 * Is the process attached to kenyad, but not yet a known kernel process?
 */
			if ((pstate->p_rtf & RTF_KENYA_ATTACH) &&
			    !(pstate->p_rtf & RTF_KERNEL)) {
				reply_state.ps_kernel.ks_state = ltot(KSFREE);
				reply_state.ps_kernel.ks_event = ltot(0);
				reply_state.ps_kernel.ks_type = ltot(0);
				reply_state.ps_kernel.ks_priority = ltot(0);
				reply_state.ps_kernel.ks_signal = ltot(0);
				psendr((char *) &reply_state,
						sizeof(reply_state), 0);
			} else {
				pdetachindex(i);
				found_match = FALSE;
			}
		} else {
			mltoti4(&reply_state.ps_kernel,
					sizeof(reply_state.ps_kernel) /
					sizeof(int4));
			psendr((char *) &reply_state, sizeof(reply_state), 0);
		}
	}
/*
 * No matching kenya process was found.  Try to build kernel state only
 * if a pid is supplied (because the pid is the only key the kernel will take).
 */
	if ((!found_match) && (pq.pq_sel_method & SELECT_PID) &&
			!kstate(pq.pq_sel_value, &reply_state.ps_kernel)) {
		pq.pq_maxproc--;
/*
 * Fill in the non-existant kenya info.
 */
		reply_state.ps_pid = ltot(pq.pq_sel_value);
		reply_state.ps_rtf = ltot(0);
		reply_state.ps_index = ltot(0);
		reply_state.ps_nodeid = ltot(NOTNODEID);
		reply_state.ps_event = ltot(NOTEVENT);

		lam_strncpy(reply_state.ps_name, "<unknown>", PSMAXNAME);
		reply_state.ps_name[PSMAXNAME - 1] = '\0';
		reply_state.ps_loadpt[0] = '\0';

		mltoti4(&reply_state.ps_kernel, sizeof(reply_state.ps_kernel) /
				sizeof(int4));
/*
 * Send a state reply back to the client.
 */
		psendr((char *) &reply_state, sizeof(reply_state), 0);
	}
/*
 * If client is expecting more, send an EEOF.
 */
	if (pq.pq_maxproc > 0) {
		reply_state.ps_reply = ltot(EEOF);
		psendr((char *) &reply_state, sizeof(reply_state), 0);
	}
}
