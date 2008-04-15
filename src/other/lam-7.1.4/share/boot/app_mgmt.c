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
 * $Id: app_mgmt.c,v 6.8 2003/04/10 17:47:01 jsquyres Exp $
 *
 *	Function:	- application execution and management
 */

#include <app_mgmt.h>
#include <lam_ksignal.h>
#include <net.h>
#include <preq.h>
#include <typical.h>


/*
 *	app_sendprocs
 *
 *	Function:	- send GPS array to all processes in the array
 *	Accepts:	- number of processes in GPS array
 *			- GPS array
 *	Returns:	- 0 or LAMERROR
 */
int
app_sendprocs(int nprocs, struct _gps *pgps)
{
	struct nmsg	nhead;			/* network msg header */
	struct _gps	*p;			/* ptr GPS entry */
	int		i;			/* favourite index */
/*
 * Set up the message.
 */
        LAM_ZERO_ME(nhead);
	nhead.nh_type = 0;
	nhead.nh_flags = DINT4MSG;
	nhead.nh_msg = (char *) pgps;
	nhead.nh_length = nprocs * sizeof(struct _gps);
/*
 * Loop sending to each process.
 */
	for (i = 0, p = pgps; i < nprocs; i++, p++) {
		nhead.nh_node = p->gps_node;
		nhead.nh_event = (-p->gps_pid) & 0xBFFFFFFF;

		if (nsend(&nhead)) return(LAMERROR);
	}
	
	return(0);
}


/*
 *	app_doom
 *
 *	Function:	- kill running application processes
 *	Accepts:	- GPS array length
 *			- application GPS array
 *	Returns:	- 0 or ERROR
 */
int
app_doom(int app_n, struct _gps *app, int sig)
{
	int		i;

	for (i = 0; i < app_n; i++) {
	    	if (app[i].gps_pid == 0) {
			break;
	    	}

		rpdoom(app[i].gps_node, SELECT_PID, app[i].gps_pid, sig);
	}

	return(0);
}
