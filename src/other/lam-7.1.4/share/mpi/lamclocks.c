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
 * $Id: lamclocks.c,v 6.6 2003/11/02 21:13:10 brbarret Exp $
 *
 *	Function:	- synchronize clocks for tracing
 */

#include <lam_config.h>

#include <stdlib.h>
#include <string.h>

#include <typical.h>
#include <net.h>
#include <app_mgmt.h>
#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <mpitrace.h>
#include <rpisys.h>

/*
 * local definitions
 */
#define	NSYNCS		10			/* # of synch pingpongs to do */

/*
 * global functions
 */
int			lam_clocksync(int4 nworld, struct _gps *wgps, float8 *skew);

/*
 * local functions
 */
static int		sync_leaders(int me, int n, int *leaders, 
                                     struct _gps *wgps, float8 *skew);
static int		sync_master(struct _gps *master, struct _gps *slave, int iter);
static int		sync_slave(struct _gps *master, struct _gps *slave, 
                                   float8 *skew, int iter);
static void		sync_fill(struct _gps *src, struct _gps *dest, 
                                  struct nmsg *head);

/*
 *	lam_clocksync
 *
 *	Function:	- world clock synchronizaton
 *	Accepts:	- number of processes in world
 *			- world GPS
 *			- skew (out)
 *	Returns:	- 0 or LAMERROR
 */
int 
lam_clocksync(int4 nworld, struct _gps *wgps, float8 *skew)
{
	char	        *msg;			/* ptr message data */
	struct nmsg	nhead;			/* network message header */
	struct _gps	*p;			/* favourite pointer */
	int4		*leaders;		/* array of leader indices */
	int		nleaders;		/* number of leaders */
	int4		mynode;			/* local node */
	int		done;			/* flag */
	int		i;
	int		j;
	
	*skew = 0.0;
	leaders = (int4 *) malloc(nworld * sizeof(int4));
	if (leaders == 0) return(LAMERROR);
/*
 * Make the list of leaders on which the world is running.
 */
	leaders[0] = 0;
	nleaders = 1;

	for (i = 1, p = wgps + 1; i < nworld; i++, p++) {

		for (j = 0, done = 0; j < nleaders && !done; j++) {
			done = (wgps[leaders[j]].gps_node == p->gps_node);
		}
		if (!done) {
			leaders[nleaders++] = i;
		}
	}
/*
 * Find the leader process on the local node.
 */
	mynode = getnodeid();

	for (i = 0, p = wgps; i < nworld && p->gps_node != mynode; i++, p++)
		;

	if (i >= nworld) return(LAMERROR);
/*
 * Set up local skew message header.
 */
	nhead.nh_flags = DFLT8DATA;
	nhead.nh_length = 0;
	msg = (char *) (&nhead.nh_data[0]);
/*
 * If I am the leader on the local node enter the clock synchronization
 * ping-pong and then send the result to the other processes on the
 * local node.  Otherwise I wait for the skew to be sent from the
 * leader.
 */
	if (p->gps_pid == lam_myproc->p_gps.gps_pid) {

		if (sync_leaders(i, nleaders, leaders, wgps, skew)) {
			return(LAMERROR);
		}

		memcpy(msg, (char *) skew, sizeof(float8));

		for (i++, p++; i < nworld; i++, p++) {
			if (p->gps_node == mynode) {
				sync_fill(&lam_myproc->p_gps, p, &nhead);
				if (nsend(&nhead)) return(LAMERROR);
			}
		}
	} else {
		sync_fill(p, &lam_myproc->p_gps, &nhead);
		if (nrecv(&nhead)) return(LAMERROR);

		memcpy((char *) skew, msg, sizeof(float8));
	}

	return(0);
}

/*
 *	sync_leaders
 *
 *	Function:	- synchronize clock on leaders
 *	Accepts:	- my index in leaders array
 *			- number of leaders
 *			- array of leaders
 *			- world GPS
 *			- clock skew (out)
 *	Returns:	- 0 or LAMERROR
 */
static int
sync_leaders(int me, int n, int *leaders, struct _gps *wgps, float8 *skew)
{
	int		i;			/* favourite index */

	if (me == 0) {
/*
 * I am the master leader.
 */
		*skew = 0.0;
		
		for (i = 1; i < n; i++) {
			if (sync_master(wgps, wgps + leaders[i], NSYNCS))
				return(LAMERROR);
		}
	} else {
/*
 * I am a slave leader.
 */
		if (sync_slave(wgps, wgps + me, skew, NSYNCS))
			return(LAMERROR);
	}

	return(0);
}

/*
 *	sync_master
 *
 *	Function:	- perform clock sync from master side
 *	Accepts:	- master GPS
 *			- slave GPS
 *			- number of iterations
 *	Returns:	- 0 or LAMERROR
 */
static int
sync_master(struct _gps *master, struct _gps *slave, int iter)
{
	float8 		local1;
	float8 		local2;
	float8		remote;
	float8		skew;
	char	        *imsg;
	char	        *omsg;
	struct nmsg	inmsg;
	struct nmsg	outmsg;
	int		i;
/*
 * Set up outgoing message header.
 */
	outmsg.nh_flags = DFLT8DATA;
	outmsg.nh_length = 0;
	sync_fill(master, slave, &outmsg);
	omsg = (char *) (&outmsg.nh_data[0]);
/*
 * Set up incoming message header.
 */
	inmsg.nh_flags = DFLT8DATA;
	inmsg.nh_length = 0;
	sync_fill(slave, master, &inmsg);
	imsg = (char *) (&inmsg.nh_data[0]);
/*
 * Priming.
 */
	if (nsend(&outmsg)) return(LAMERROR);
	if (nrecv(&inmsg)) return(LAMERROR);
/*
 * Ping-pong clock times.
 */
	skew = 0.0;

	for (i = 0; i < iter; i++) {
		local1 = (float8) ttime();
	
		if (nsend(&outmsg)) return(LAMERROR);
	
		if (nrecv(&inmsg)) return(LAMERROR);

		local2 = (float8) ttime();
		memcpy((char *) &remote, imsg, sizeof(float8));

		skew += (local1 + local2) / 2.0  - remote;
	}
/*
 * Calculate and send delta.
 */
	skew /= (float8) iter;
	memcpy(omsg, (char *) &skew, sizeof(float8));

	if (nsend(&outmsg)) return(LAMERROR);

	return(0);
}

/*
 *	sync_slave
 *
 *	Function:	- perform clock sync from slave side
 *	Accepts:	- master GPS
 *			- slave GPS
 *			- clock skew (out)
 *			- number of iterations
 *	Returns:	- 0 or LAMERROR
 */
static int
sync_slave(struct _gps *master, struct _gps *slave, float8 *skew, int iter)
{
	float8		local;
	char	        *imsg;
	char	        *omsg;
	struct nmsg	inmsg;
	struct nmsg	outmsg;
	int		i;
/*
 * Set up outgoing message header.
 */
	outmsg.nh_flags = DFLT8DATA;
	outmsg.nh_length = 0;
	sync_fill(slave, master, &outmsg);
	omsg = (char *) (&outmsg.nh_data[0]);
/*
 * Set up incoming message header.
 */
	inmsg.nh_flags = DFLT8DATA;
	inmsg.nh_length = 0;
	sync_fill(master, slave, &inmsg);
	imsg = (char *) (&inmsg.nh_data[0]);
/*
 * Priming.
 */
	if (nrecv(&inmsg)) return(LAMERROR);
	if (nsend(&outmsg)) return(LAMERROR);
/*
 * Ping-pong clock times.
 */
	for (i = 0; i < iter; i++) {

		if (nrecv(&inmsg)) return(LAMERROR);

		local = (float8) ttime();
		memcpy(omsg, (char *) &local, sizeof(float8));

		if (nsend(&outmsg)) return(LAMERROR);

	}

	if (nrecv(&inmsg)) return(LAMERROR);

	memcpy((char *) skew, imsg, sizeof(float8));
	
	return(0);
}

/*
 *	
 *
 *	Function:	- fill in network message synch for clock messages
 *	Accepts:	- source process
 *			- destination process
 *			- network message header (filled)
 */
static void
sync_fill(struct _gps *src, struct _gps *dest, struct nmsg *head)
{
/*
 * Use comm world cid zero and MPI_Init tag.
 */
	_m2l_fillsys(src->gps_node, src->gps_idx, dest->gps_node,
		dest->gps_idx, 0, BLKMPIINIT, head);
}
