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
 *	$Id: do_load.c,v 6.4 2002/10/09 20:56:33 brbarret Exp $
 * 
 *	Function:	- UDP Internet datalink output
 *			- loads output link information
 */

#include <stdlib.h>

#include <dl_inet.h>
#include <events.h>
#include <kreq.h>
#include <portable.h>
#include <terror.h>

/*
 * external variables
 */
extern struct dolink	*dolinks;	/* link table */
extern int		dol_cnt;	/* dol table length */
extern int		do_fault;	/* watch for faulty links */

/*
 * local variables
 */
static int4		config[3];	/* configuration parameters */

/*
 * global functions
 */
void			(*(do_loadinit()))();
void			(*(do_loadlinks()))();

/*
 * external functions
 */
extern void		(*(do_init2()))();

/*
 *	do_loadinit
 *
 *	Function:	- initializes the link table
 *			- receives neighbour link info from dli
 *			- blocks on a special event until info is received
 */
void (*(
do_loadinit()))()

{
	struct kmsg	khead;		/* kernel message desc. */

	khead.k_event = EVDLOINIT;
	khead.k_type = 0;
	khead.k_flags = 0;
	khead.k_length = sizeof(config);
	khead.k_msg = (char *) config;
/*
 * Receive the length of the link array.
 */
	if (krecv(&khead)) lampanic("dlo_inet (krecv)");

	return((void (*)()) do_loadlinks);
}

/*
 *	do_loadlinks
 *
 *	Function:	- loads link information from dli
 */
void (*(
do_loadlinks()))()

{
	struct kmsg	khead;			/* kernel message desc. */

	dol_cnt = (int) config[0];
	do_fault = (int) config[2];

	khead.k_event = EVDLOINIT;
	khead.k_type = 0;
	khead.k_flags = 0;
	khead.k_length = dol_cnt * sizeof(struct dolink);

	dolinks = (struct dolink *) malloc((unsigned) khead.k_length);
	if (!dolinks) lampanic("dlo_inet (malloc)");

	khead.k_msg = (char *) dolinks;
/*
 * Receive the neighbour link information.
 */
	if (krecv(&khead)) lampanic("dlo_inet (krecv)");

	return(do_init2());
}
