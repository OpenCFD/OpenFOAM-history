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
 * $Id: rpi_gm_recv_events.h,v 1.2 2002/10/09 21:00:21 brbarret Exp $
 */

#ifndef LAM_SSI_RPI_GM_RECV_EVENTS_H
#define LAM_SSI_RPI_GM_RECV_EVENTS_H

#include <rpisys.h>

#include <gm.h>


/*
 * public functions
 */

int lam_ssi_rpi_gm_recv_event(gm_recv_event_t *event, int fast);

#endif /* LAM_SSI_RPI_GM_RECV_EVENTS_H */
