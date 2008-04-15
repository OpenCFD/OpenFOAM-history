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
 * $Id: lpattach.c,v 1.4 2003/05/31 22:28:52 jsquyres Exp $
 *
 *	Function:	- registers with kenyad
 *	Accepts:	- name of program
 *	Returns:	- 0 or ERROR
 */

#include <lam_config.h>

#include <string.h>

#include <events.h>
#include <ksignal.h>
#include <kio.h>
#include <net.h>
#include <portable.h>
#include <preq.h>
#include <t_types.h>
#include <typical.h>
#include <terror.h>
#include <etc_misc.h>

/*
 * external variables
 */
extern struct kio_t	_kio;		/* Kernel Input/Output */

/*
 * external functions
 */
extern int4		kpgetpid();

int
lpattach(name)

CONST char		*name;

{
	struct nmsg	nhead;		/* network message */
	struct preq	request;	/* kenya request */
	struct preply	reply;		/* kenya reply */
	int		mask;

	LAM_ZERO_ME(request);
	LAM_ZERO_ME(nhead);

	request.pq_src_node = ltot(LOCAL);
	request.pq_src_event = ltot((int4) -kbgetpid());
	request.pq_req = ltot(PQATTACH);
	request.pq_pid = ltot((int4) kbgetpid());
	request.pq_rtf = ltot(_kio.ki_rtf);

	if (name == 0) {
		request.pq_name[0] = '\0';
	} else {
		lam_strncpy(request.pq_name, name, PSMAXNAME);
		request.pq_name[PSMAXNAME - 1] = '\0';
	}

	nhead.nh_node = LOCAL;
	nhead.nh_event = EVKENYAD;
	nhead.nh_type = 0;
	nhead.nh_flags = 0;
	nhead.nh_length = sizeof(request);
	nhead.nh_msg = (char *) &request;
	mask = ksigblock(sigmask(SIGUDIE) | sigmask(SIGARREST));

	if (nsend(&nhead)) {
		ksigsetmask(mask);
		return(ERROR);
	}

	nhead.nh_event = -kbgetpid();
	nhead.nh_flags = 0;
	nhead.nh_length = sizeof(reply);
	nhead.nh_msg = (char *) &reply;

/*
 *	if (nrecv(&nhead)) {
 *		ksigsetmask(mask);
 *		return(ERROR);
 *	}
 */

	ksigsetmask(mask);

/*
 *	if (reply.pr_reply) {
 *		errno = ttol(reply.pr_reply);
 *		return(ERROR);
 *	} else {
 */
		_kio.ki_rtf |= RTF_KENYA_ATTACH;
		return(0);
/*
 *	}
 */
}
