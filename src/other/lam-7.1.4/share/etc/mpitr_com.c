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
 * $Id: mpitr_com.c,v 6.5 2003/07/05 21:50:25 jsquyres Exp $
 * 
 *	Function:	- MPI communicator trace access functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <app_mgmt.h>
#include <mpitrace.h>
#include <portable.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>


/*
 * external functions
 */
extern int		lam_coll2pt();
extern int		lam_rtrget();

/*
 *	mpitr_comget
 *
 *	Function:	- get MPI communicator trace
 *			- specified by context ID and member process
 *	Accepts:	- process node
 *			- process pid
 *			- context ID
 *			- ptr trace communicator buffer (returned value)
 *	Returns:	- 0 or ERROR
 */
int
mpitr_comget(int4 nodeid, int4 pid, int4 contextid, char **ptrcom)
{
	struct trcid	*ptrc;		/* ptr cid trace */
	int		trlen;		/* trace buffer length */
	int		len;		/* buffer length */
	char		*p;		/* favourite pointer */
	char		*trbuf;		/* communicator traces buffer */
	
	*ptrcom = 0;
	contextid = lam_coll2pt(contextid);
/*
 * Get the trace communicator representation.
 */
	trlen = lam_rtrget(nodeid, (int4) TRCOMM, pid, &trbuf);
	if (trlen < 0) {
		return(LAMERROR);
	}
/*
 * Locate the communicator buffer with given contextid.
 */
	p = trbuf;

	while (trlen > 0) {

		ptrc = (struct trcid *) (p + sizeof(struct trsrc));
		mttoli4((int4 *) ptrc, sizeof(struct trcid) / sizeof(int4));

		len = sizeof(struct trsrc) + sizeof(struct trcid)
			+ (sizeof(struct _gps)
				* (ptrc->trc_nlg + ptrc->trc_nrg));

		if (ptrc->trc_cid == contextid) {

			len -= sizeof(struct trsrc);
			*ptrcom = malloc((unsigned) len);
			if (*ptrcom == 0) {
				free(trbuf);
				return(LAMERROR);
			}
			mttoli4((int4 *) (p + sizeof(struct trsrc)
						+ sizeof(struct trcid)),
				(len - sizeof(struct trcid)) / sizeof(int4));
				memcpy(*ptrcom, (char *) ptrc, len);

			free(trbuf);
			return(0);
		}

		trlen -= len;
		p += len;
	}

	free(trbuf);
	errno = EINVAL;
	return(LAMERROR);
}

/*
 *	mpitr_comprint
 *
 *	Function:	- print MPI communicator trace
 *	Accepts:	- context ID
 *			- trace communicator buffer
 *			- flag, on means show in GPS form
 */
void
mpitr_comprint(int cid, char *trcom, int showgps)
{
	int		collective;	/* is it a collective comm? */
	int4		lsize;		/* size of 1st group */
	int4		rsize;		/* size of 2nd group */
	struct _gps	*p;		/* ptr to group members */
	struct trcid	*t;		/* ptr cid trace */

	collective = (cid < 0);
	cid = lam_coll2pt(cid);
	
	t = (struct trcid *) trcom;

	if (cid == 0) {
		printf("INTRACOMM:\tMPI_COMM_WORLD");
	}
	else if (cid == 1) {
		printf("INTRACOMM:\tMPI_COMM_SELF");
		return;
	} else {
		printf("%s:\t", (t->trc_nrg == 0) ? "INTRACOMM" : "INTERCOMM");
		printf("<%d>", cid);
	}
	if (collective) printf("*");
	printf("\n");
		
	lsize = t->trc_nlg;
	rsize = t->trc_nrg;

	printf("%s%d\n",((rsize == 0) ? "SIZE:\t\t" : "LOCAL SIZE:\t"), lsize);
/*
 * Print global ranks of local processes.
 */
	p = (struct _gps *) (t+1);
	printf("%s", (rsize == 0) ? "GROUP:\t\t" : "LOCAL GROUP:\t");
	for (; lsize > 0; --lsize, ++p) {
		if (showgps) {
			printf("n%d,i%d ", p->gps_node, p->gps_idx);
		} else {
			printf("%d ", p->gps_grank);
		}
	}

	printf("\n");
/*
 * Print global ranks of remote processes.
 */
	if (rsize > 0) {
		printf("REMOTE GROUP:\t");

		for (; rsize > 0; --rsize, ++p) {
			if (showgps) {
				printf("n%d,i%d ", p->gps_node, p->gps_idx);
			} else {
				printf("%d ", p->gps_grank);
			}
		}
		printf("\n");
	}
}
