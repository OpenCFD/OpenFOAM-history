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
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	GDB
 *
 *	$Id: do_wait.ext.c,v 6.10 2003/10/11 19:15:34 brbarret Exp $
 *
 *	Function:	- waits for a packet, an ack or a timeout
 */

#include <lam_config.h>

#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <dl_inet.h>
#include <net.h>
#include <terror.h>
#include <typical.h>

/*
 * global functions
 */
void			do_wait_block();
void			do_wait_running();

/*
 * external functions
 */
extern void		do_ack();

/*
 * local functions
 */
static int		wait_select();

/*
 * local variables
 */
static int		to_count = 0;		/* preempted timeout periods */

/*
 *	do_wait_block
 *
 *	Function:	- blocks on message receive while servicing
 *			  interrupts
 *			- if no receive, services one interrupt
 *	Accepts:	- ack socket
 *			- packet descriptor
 *			- timeout function
 *			- timeout value
 */
void
do_wait_block(sd_ack, pnh_packet, timeout_fn, pto_actual)

int			sd_ack;
struct nmsg		*pnh_packet;
void			(*timeout_fn)();
struct timeval		*pto_actual;

{
	int		r;
	int		sd_kernel;	/* Trollius kernel socket desc. */
	fd_set		readfds;	/* select read-mask */

/*
 * Initiate a message receive.
 */
	sd_kernel = frecvfront(pnh_packet);
	if (sd_kernel < 0) lampanic("dlo_inet (frecvfront)");

	do {
		FD_ZERO(&readfds);
		FD_SET(sd_ack, &readfds);
		FD_SET(sd_kernel, &readfds);

		r = wait_select(LAM_max(sd_ack, sd_kernel) + 1, &readfds,
				pto_actual);

		if (r == 0) {
			(*timeout_fn)(&timeout_fn, &pto_actual);
		} else if (FD_ISSET(sd_ack, &readfds)) {
			do_ack(&timeout_fn, &pto_actual);
		}
	} while ((r == 0) || ! FD_ISSET(sd_kernel, &readfds));

	if (frecvback(pnh_packet)) lampanic("dlo_inet (frecvback)");
}

/*
 *	do_wait_running
 *
 *	Function:	- waits for one interrupt and handles it
 *	Accepts:	- ack socket
 *			- timeout function
 *			- timeout value
 */
void
do_wait_running(sd_ack, timeout_fn, pto_actual)

int			sd_ack;
void			(*timeout_fn)();
struct timeval		*pto_actual;

{
	int		r;
	fd_set		readfds;	/* select read-mask */

	FD_ZERO(&readfds);
	FD_SET(sd_ack, &readfds);

	r = wait_select(sd_ack + 1, &readfds, pto_actual);

	if (r == 0) {
		(*timeout_fn)(&timeout_fn, &pto_actual);
	} else {
		do_ack(&timeout_fn, &pto_actual);
	}
}

/*
 *	wait_select
 *
 *	Function:	- blocks on receive, if present
 *			- services one interrupt
 *	Accepts:	- mask width
 *			- read descriptor mask
 *			- timeout value
 *	Returns:	- # active read descriptors
 */
static int
wait_select(width, preadfds, pto_actual)

int			width;
fd_set			*preadfds;
struct timeval		*pto_actual;

{
	int		r;
	struct timeval  tmp;           /* because Linux changes it */

/*
 * Trigger timeout based on estimated period.
 */
	if ((pto_actual != 0) && (to_count > TO_DLO_ESTIMATE)) {
		to_count = 0;
		return(0);
	}

	do {
	  /* Linux changes the value of the struct timeval in select.
	   * Because of this, the timeout periods can shink over time
	   * unless we use a temporary timeval to protect us. 
	   */
	  if (pto_actual != 0) {
	    tmp.tv_sec = pto_actual->tv_sec;
	    tmp.tv_usec = pto_actual->tv_usec;
	  }

	  r = select(width, preadfds, (fd_set*) 0, (fd_set*) 0,
		     (pto_actual != 0) ? &tmp : (struct timeval *) 0);
	} while ((r < 0) && (EINTR == errno));

	if (r < 0) {
		lampanic("dlo_inet (select)");
	} else if (r == 0) {
		to_count = 0;
	}
/*
 * Increment the estimated timeout period.
 */
	else if (pto_actual != 0) {
		to_count++;
	}

	return(r);
}
