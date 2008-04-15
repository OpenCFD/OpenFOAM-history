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
 * $Id: trdrain_mpi.c,v 6.4 2003/07/05 21:50:25 jsquyres Exp $
 *
 *	Function:	- unloads trace data for one MPI world
 *	Accepts:	- trace file desc. opened for append
 *			- GPS array of world
 *			- GPS array length
 *			- get/forget flag
 *			- delay period in secs after flush signal
 */

#include <unistd.h>

#include <lam_ksignal.h>
#include <app_mgmt.h>
#include <mpitrace.h>
#include <preq.h>
#include <trreq.h>
#include <typical.h>


int
trdrain_mpi(int fd, struct _gps *world, int nworld, int keep, int flush_delay)
{
	int		i;
	int		(*drain)();

	drain = keep ? lam_rtrfget : lam_rtrfforget;
/*
 * Signal everybody to flush in-process trace buffers to the daemon.
 */
	if (flush_delay >= 0) {

		for (i = 0; i < nworld; i++) {
			rpdoom(world[i].gps_node, SELECT_PID,
					world[i].gps_pid, LAM_SIGTRACE);
		}

		if (flush_delay > 0) {
			sleep((unsigned) flush_delay);
		}
	}
/*
 * Drain the init/on/off traces.
 */
	for (i = 0; i < nworld; i++) {

		if ((*drain)(world[i].gps_node, TRONOFF, world[i].gps_pid,
				fd) < 0) return(LAMERROR);
	}
/*
 * Drain the communicator traces.
 */
	for (i = 0; i < nworld; i++) {

		if ((*drain)(world[i].gps_node, TRCOMM, world[i].gps_pid,
				fd) < 0) return(LAMERROR);
	}
/*
 * Drain the datatype traces.
 */
	for (i = 0; i < nworld; i++) {

		if ((*drain)(world[i].gps_node, TRDTYPE, world[i].gps_pid,
				fd) < 0) return(LAMERROR);
	}
/*
 * Drain the runtime traces.
 */
	for (i = 0; i < nworld; i++) {

		if ((*drain)(world[i].gps_node, TRRUNTIME, world[i].gps_pid,
				fd) < 0) return(LAMERROR);
	}

	return(0);
}
