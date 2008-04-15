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
 *	RBD/NJN
 *
 *	$Id: mpitr_dtype.c,v 6.4 2002/10/09 20:57:03 brbarret Exp $
 * 
 *	Function:	- MPI datatype trace access functions
 */

#include <stdlib.h>
#include <string.h>

#include <mpitrace.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>

/*
 * global functions
 */
int			mpitr_dtypeget();

/*
 * external functions
 */
extern int		lam_rtrget();

/*
 *	mpitr_dtypeget
 *
 *	Function:	- get datatype trace
 *	Accepts:	- process node
 *			- process pid
 *			- datatype label
 *			- ptr trace datatype buffer (returned value)
 *	Returns:	- 0 or LAMERROR
 */
int
mpitr_dtypeget(nodeid, pid, dtlabel, ptrdtype)

int4			nodeid;
int4			pid;
int4			dtlabel;
char			**ptrdtype;

{
	int		trlen;			/* trace buffer length */
	char		*p;			/* favourite pointer */
	char		*trbuf;			/* datatype trace buffer */
	struct trdtype	*ptrdt = 0;		/* ptr into datatype trace */
	
	*ptrdtype = 0;

	if (dtlabel < 0) {
		errno = EINVAL;
		return(LAMERROR);
	}

	if (dtlabel <= TRDTMAX) {
/*
 * Allocate datatype description.
 */
		trlen = 2 * sizeof(struct trdtype);

		*ptrdtype = malloc((unsigned) trlen);
		if (*ptrdtype == 0) return(LAMERROR);

		ptrdt = (struct trdtype *) *ptrdtype;
		ptrdt->trd_dtype = dtlabel;
		ptrdt->trd_length = trlen;
		
		ptrdt++;
		ptrdt->trd_dtype = dtlabel;
		ptrdt->trd_count = 1;

		return(0);
	}
/*
 * It's a constructed datatype, get its trace representation.
 */
	trlen = lam_rtrget(nodeid, (int4) TRDTYPE, pid, &trbuf);
	if (trlen < 0) return(LAMERROR);
/*
 * Locate the datatype buffer with given number.
 */
	*ptrdtype = 0;
	p = trbuf;

	while (trlen > 0) {
		ptrdt = (struct trdtype *) (p + sizeof(struct trsrc));
		mttoli4((int4 *) ptrdt, sizeof(struct trdtype) / sizeof(int4));
		
		if (ptrdt->trd_dtype == dtlabel) {
			*ptrdtype = (char *) ptrdt;
			break;
		}

		trlen -= (ptrdt->trd_length + sizeof(struct trsrc));
		p += (ptrdt->trd_length + sizeof(struct trsrc));
	}

	if (*ptrdtype == 0) {
		free((char *) trbuf);
		errno = EINVAL;
		return(LAMERROR);
	}
/*
 * Copy datatype record.
 */
	*ptrdtype = malloc((unsigned) (ptrdt->trd_length));
	if (*ptrdtype == 0) {
		free((char *) trbuf);
		return(LAMERROR);
	}

	mttoli4((int4 *) (((char *) ptrdt) + sizeof(struct trdtype)),
		(ptrdt->trd_length - sizeof(struct trdtype)) / sizeof(int4));

	memcpy(*ptrdtype, (char *) ptrdt, ptrdt->trd_length);
	free((char *) trbuf);

	return(0);
}
