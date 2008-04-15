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
 * $Id: rpi_ib_send_queue.h,v 1.1 2003/11/11 18:39:49 vsahay Exp $
 */

#ifndef LAM_SSI_RPI_IB_SEND_QUEUE_H
#define LAM_SSI_RPI_IB_SEND_QUEUE_H

#include <mpisys.h>
#include <rpisys.h>

#include <rpi_ib_reqlist.h>


/*
 * public variables
 * (only so we can use a #define, below -- it's not *really* public)
 */
extern struct lam_ssi_rpi_ib_reqlist_t lam_ssi_rpi_ib_send_queue;
extern struct lam_ssi_rpi_ib_reqlist_t lam_ssi_rpi_ib_ack_queue;


/*
 * public functions
 */

int lam_ssi_rpi_ib_send_queue_init(void);
int lam_ssi_rpi_ib_send_queue_advance(void);

int lam_ssi_rpi_ib_send_queue_add(MPI_Request req);
int lam_ssi_rpi_ib_ack_queue_add(MPI_Request req);

/*
 * This is such a quickie that it doesn't need to be a function -- but
 * we keep the abstraction (oh for inline functions...)
 */

#define LAM_SSI_RPI_IB_SEND_QUEUE_ADD(req) \
    lam_ssi_rpi_ib_send_queue_add((req))

#define LAM_SSI_RPI_IB_SEND_QUEUE_EMPTY() \
    (lam_ssi_rpi_ib_send_queue.size == 0)

#define LAM_SSI_RPI_IB_ACK_QUEUE_ADD(req) \
    lam_ssi_rpi_ib_ack_queue_add((req))

#define LAM_SSI_RPI_IB_ACK_QUEUE_EMPTY() \
    (lam_ssi_rpi_ib_ack_queue.size == 0)


#endif /* LAM_SSI_RPI_IB_SEND_QUEUE_H */
