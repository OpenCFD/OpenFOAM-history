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
 * $Id: cdisconnect.c,v 6.8 2003/08/22 02:33:18 brbarret Exp $
 *
 *	Function:	- completes all pending communication on
 *			  a communicator and frees it
 *	Accepts:	- communicator
 *	Returns:	- MPI_SUCCESS or error code
 */

#include <errno.h>
#include <stdlib.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*@
   MPI_Comm_disconnect - Disconnect a client and server

Input Parameter:

. comm - communicator

.N fortran

.N IMPI_MPI2

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER

.seealso MPI_Close_port(3), MPI_Comm_accept(3), MPI_Comm_connect(3), MPI_Open_port(3)

.N WEB
@*/
int
MPI_Comm_disconnect(MPI_Comm *comm)
{
	MPI_Request 	*reqs;			/* array of pending requests */
	struct _req	*r;			/* request */
	int4		ccid;			/* collective context ID */
	int		npend = 0;		/* number of pending requests */
	int		err;			/* error return */
	int		i;
	
	lam_initerr();
	lam_setfunc(BLKMPICOMMDISCONNECT);

	LAM_TRACE(lam_tr_cffstart(BLKMPICOMMDISCONNECT));
/*
 * Find number of pending requests on the communicator.
 */
	ccid = lam_mpi_pt2coll((*comm)->c_contextid);
	
	for (r = lam_rq_top; r; r = r->rq_next) {
		if (r->rq_state != LAM_RQSDONE && (r->rq_cid == ccid
				|| r->rq_cid == (*comm)->c_contextid)) {
			npend++;
		}
	}
/*
 * Create array of pending requests to wait on. (but only if we
 * actually have requests to wait on)
 */
        if (npend > 0) {
          reqs = (MPI_Request *) malloc((unsigned) npend * 
                                        sizeof(MPI_Request));
          if (reqs == 0) {
            return(lam_errfunc(*comm, BLKMPICOMMDISCONNECT,
                               lam_mkerr(MPI_ERR_NO_MEM, errno)));
          }

          for (i = 0, r = lam_rq_top; r; r = r->rq_next) {
            if (r->rq_state != LAM_RQSDONE && (r->rq_cid == ccid ||
                                               r->rq_cid == 
                                               (*comm)->c_contextid)) {
              reqs[i++] = r;
            }
          }

          err = MPI_Waitall(npend, reqs, MPI_STATUSES_IGNORE);

          free((char *) reqs);
          if (err) {
            return(lam_errfunc(*comm, BLKMPICOMMDISCONNECT, err));
          }
        }
	
	LAM_TRACE(lam_tr_cffend(BLKMPICOMMDISCONNECT, -1, *comm, 0, 0));

	if ((err = MPI_Comm_free(comm)) != MPI_SUCCESS) {
		return(lam_errfunc(MPI_COMM_WORLD, BLKMPICOMMDISCONNECT, err));
	}
	*comm = MPI_COMM_NULL;

        lam_resetfunc(BLKMPICOMMDISCONNECT);
	return(MPI_SUCCESS);
}
