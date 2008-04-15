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
 * $Id: lamreceive.c,v 1.5 2002/12/11 19:15:12 jsquyres Exp $
 *
 *	Function:	- internal version of receives
 */

#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>


/*
 *	lam_recv
 *
 *	Function:	- internal receive
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- source
 *			- tag
 *			- communicator
 *			- status (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */
int
lam_recv(void *buf, int count, MPI_Datatype dtype, int src, int tag,
	 MPI_Comm comm, MPI_Status *stat)
{
	int		err;
	struct _req	req_storage;
	MPI_Request	req;

	LAM_ZERO_ME(req_storage);
	req = &req_storage;
	err = _mpi_req_build(buf, count, dtype, 
			     src, tag, comm, LAM_RQIRECV, &req);
	if (err != MPI_SUCCESS) return(err);

	req->rq_marks |= (LAM_RQFBLKTYPE | LAM_RQFMAND);

	err = _mpi_req_start(req);
	if (err != MPI_SUCCESS) return(err);

	_mpi_req_add_m(req);
	_mpi_req_blkclr_m();
	_mpi_req_blkset_m(req);

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) return(err);
	
	_mpi_req_rem_m(req);

	err = _mpi_req_end(req);
	if (err != MPI_SUCCESS) return(err);

	*stat = req->rq_status;

	err = _mpi_req_destroy(&req);
	if (err != MPI_SUCCESS) return(err);

	if (stat->MPI_ERROR != MPI_SUCCESS) {
		return(stat->MPI_ERROR);
	}

	return(MPI_SUCCESS);
}


/*
 *	lam_irecv
 *
 *	Function:	- internal immediate receive
 *	Accepts:	- buffer
 *			- count
 *			- datatype
 *			- source
 *			- tag
 *			- communicator
 *			- request (out)
 *	Returns: 	- MPI_SUCCESS or error code
 */
int
lam_irecv(void *buf, int count, MPI_Datatype dtype, int src, int tag,
	  MPI_Comm comm, MPI_Request *req)
{
        int err;

	*req = MPI_REQUEST_NULL;
	err = _mpi_req_build(buf, count, dtype, src, tag, comm,
			LAM_RQIRECV, req);
	if (err != MPI_SUCCESS) return(err);

	(*req)->rq_marks |= LAM_RQFMAND;

	err = _mpi_req_start(*req);
	if (err != MPI_SUCCESS) return(err);

	_mpi_req_add(*req);
	_mpi_req_blkclr();

	err = _mpi_req_advance();
	if (err != MPI_SUCCESS) return(err);

	return(MPI_SUCCESS);
}
