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
 *
 *
 *	$Id: app_mgmt.h,v 6.9 2002/11/03 17:03:11 ssankara Exp $
 *
 *	Function:	- running application management
 */

#ifndef _APP_MGMT_H
#define _APP_MGMT_H

#include "lam.h"

/*
 * LAM constants & macros
 */
#define LAM_GPSCMP(p1, p2)	   (((p1)->gps_node < (p2)->gps_node) ? -1 : \
				   (((p1)->gps_node > (p2)->gps_node) ? 1 : \
				   (((p1)->gps_idx < (p2)->gps_idx) ? -1 : \
				   (((p1)->gps_idx > (p2)->gps_idx) ? 1 : \
				   (((p1)->gps_pid < (p2)->gps_pid) ? -1 : \
				   (((p1)->gps_pid > (p2)->gps_pid) ? 1 : 0))))))

#define LAM_FULL_GPSCMP(p1, p2)    (((p1)->gps_node < (p2)->gps_node) ? -1 : \
                                   (((p1)->gps_node > (p2)->gps_node) ? 1 : \
                                   (((p1)->gps_idx < (p2)->gps_idx) ? -1 : \
                                   (((p1)->gps_idx > (p2)->gps_idx) ? 1 : \
                                   (((p1)->gps_pid < (p2)->gps_pid) ? -1 : \
                                   (((p1)->gps_pid > (p2)->gps_pid) ? 1 : \
                                   (((p1)->gps_grank > (p2)->gps_grank) ? -1 : \
                                   (((p1)->gps_grank < (p2)->gps_grank) ? 1 : 0))))))))

/*
 * Global Positioning System for running processes
 */
struct _gps {
	int4		gps_node;		/* node ID */
	int4		gps_pid;		/* process ID */
	int4		gps_idx;		/* process index */
	int4		gps_grank;		/* glob. rank in loc. world */
};

#ifdef __cplusplus
extern "C" {
#endif

extern int		app_doom __ARGS((int app_n, struct _gps *app, 
					 int sig));
extern int		app_sendprocs __ARGS((int np, struct _gps *w));

#ifdef __cplusplus
}
#endif

#endif	/* _APP_MGMT_H */
