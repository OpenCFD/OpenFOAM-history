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
 *	RBD/GDB
 *
 *	$Id: rtdinit_lam.c,v 1.2 2002/10/09 20:56:51 brbarret Exp $
 * 
 *	Function:	- route table initializer
 */

#include <all_hash.h>
#include <net.h>
#include <rreq.h>
#include <terror.h>

/*
 * external functions
 */
extern int4		next_prime();

int4
rtdinit(pprtd)

HASH			**pprtd;

{
	int4		nroutes;	/* size of route table */
/*
 * Build the route table.
 */
	nroutes = next_prime(RNRENT);

	if ((*pprtd = ah_init(nroutes, (int4) sizeof(struct route),
			(int4) NOTNODEID, INT4_NIL)) == 0)
			lampanic("router (ah_init)");

	return(RDUMB);
}
