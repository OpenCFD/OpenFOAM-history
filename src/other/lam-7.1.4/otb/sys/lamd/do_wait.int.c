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
 *	GDB
 *
 *	$Id: do_wait.int.c,v 6.7 2002/10/09 20:56:47 brbarret Exp $
 */

#include <net.h>
#include <typical.h>
#include <terror.h>

/*
 * global functions
 */
void			do_wait_block();
void			do_wait_running();

/*
 * local functions
 */
static void		ack_handler();
static void		to_handler();

/*
 * external functions
 */
extern void		do_ack();
extern void		kio_intr();
extern int		kio_to();

/*
 * local variables
 */
static void		*pto_actual_sv;
static void		(*timeout_fn_sv)();

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
void			*pto_actual;

{
	timeout_fn_sv = timeout_fn;
	pto_actual_sv = pto_actual;

	if (frecv(pnh_packet)) lampanic("lamd (frecv)");

	if (kio_to(pto_actual, pto_actual ? to_handler : (void (*)()) 0))
			lampanic("lamd (kio_to)");

	kio_intr(sd_ack, ack_handler, FALSE);
}

/*
 *	do_wait_running
 *
 *	Function:	- waits for one interrupt and handles it
 *	Accepts:	- ack socket
 *			- timeout function
 *			- timeout value
 *	Returns:	- packet received?
 */
void
do_wait_running(sd_ack, timeout_fn, pto_actual)

int			sd_ack;
void			(*timeout_fn)();
void			*pto_actual;

{
	timeout_fn_sv = timeout_fn;
	pto_actual_sv = pto_actual;

	if (kio_to(pto_actual, pto_actual ? to_handler : (void (*)()) 0))
			lampanic("lamd (kio_to)");

	kio_intr(sd_ack, ack_handler, FALSE);
}

/*
 *	to_handler
 *
 *	Function:	- calls the current timeout handler
 *			- possibly updates the current timeout handler
 *			  and current timeout interval
 */
static void
to_handler()

{
	(*timeout_fn_sv)(&timeout_fn_sv, &pto_actual_sv);

	if (kio_to(pto_actual_sv, pto_actual_sv ? to_handler : (void (*)()) 0))
			lampanic("lamd (kio_to)");
}

/*
 *	ack_handler
 *
 *	Function:	- calls the ack handler
 *			- possibly updates the current timeout handler
 *			  and current timeout interval
 */
static void
ack_handler()

{
	do_ack(&timeout_fn_sv, &pto_actual_sv);

	if (kio_to(pto_actual_sv, pto_actual_sv ? to_handler : (void (*)()) 0))
			lampanic("lamd (kio_to)");
}
