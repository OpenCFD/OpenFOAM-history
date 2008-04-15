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
 * $Id: lamonesided.c,v 1.13 2004/01/02 00:01:02 jsquyres Exp $
 *
 *	Function:	- one sided communication
 *
 *	Notes:	Since all pre-defined handles are given F77 integer (handles)
 *		at init time we can use the F77 handle as a means of passing
 *		datatypes and operations.
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <MPISYSF.h>
#include <mpitrace.h>
#include <rpisys.h>

/*
 * extra state for accumulate handler
 */
struct accstate {
	char		*acc_buffer;		/* tmp buf for incoming data */
	char		*acc_targbuf;		/* target buffer in window */
	MPI_Op		acc_op;			/* accumulate operation */
	MPI_Datatype	acc_dtype;		/* accumulate datatype */
	int		acc_count;		/* # elements to apply op on */
};

/*
 * private functions
 */
static int lam_accumulate(MPI_Win win, int rank);
static int lam_put(MPI_Win win, int rank);
static int lam_get(MPI_Win win, int rank);
static MPI_Datatype header2dtype(struct _osdhdr *hdr);
static int accumulate_handler(MPI_Request req);

/*
 * public variables
 */
MPI_Datatype		lam_osdhdr_type = MPI_DATATYPE_NULL;
						/* type for protocol header */
/*
 *	lam_init_onesided
 *
 *	Function:	- initialize for one-sided communication
 *	Returns:	- 0 or LAMERROR
 */
int
lam_init_onesided(void)
{
	struct _osdhdr	header;			/* 1-sided header packet */
	MPI_Datatype	types[2];
	MPI_Aint	disps[2];
	int		blklens[2];
	int		err;
/*
 * Create the MPI datatype used to transport the one sided header
 * packet.  It corresponds to C type struct _osdhdr.  This type should
 * have no holes and hence not require packing/unpacking unless for
 * byte order conversion.  
 */
#if LAM_SIZEOF_INT == 8
	err = MPI_Type_contiguous(9, MPI_INT, &lam_osdhdr_type);
	if (err != MPI_SUCCESS) {
		return(LAMERROR);
	}
#else
#if LAM_SIZEOF_LONG == 8	
	types[1] = MPI_LONG;
#elif LAM_SIZEOF_LONG_LONG == 8	
	types[1] = MPI_LONG_LONG_INT;
#endif

	err = MPI_Type_contiguous(8, MPI_INT, &types[0]);
	if (err != MPI_SUCCESS) {
		return(LAMERROR);
	}

	blklens[0] = 1;
	blklens[1] = 1;
	disps[0] = 0;
	disps[1] = (char *) &header.os_disp - (char *) &header.os_func;

	err = MPI_Type_struct(2, blklens, disps, types, &lam_osdhdr_type);
	if (err != MPI_SUCCESS) {
		return(LAMERROR);
	}

	err = MPI_Type_free(&types[0]);
	if (err != MPI_SUCCESS) {
		return(LAMERROR);
	}
#endif

	err = MPI_Type_commit(&lam_osdhdr_type);
	if (err != MPI_SUCCESS) {
		return(LAMERROR);
	}

	return(0);
}

/*
 *	lam_free_onesided
 *
 *	Function:	- cleanup one-sided communication
 */
void
lam_free_onesided(void)
{
	MPI_Type_free(&lam_osdhdr_type);
}

/*
 *	lam_osd_blkadv
 *
 *	Function:	- advance with blocking one-sided request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_blkadv(MPI_Win win)
{
	int		err;
/*
 * Activate the request attached to the window if it is not already
 * active and mark it as blocking.  
 */
	if (win->w_req->rq_state == LAM_RQSINIT) {
		if ((err = _mpi_req_start(win->w_req)) != MPI_SUCCESS) {
			return(err);
		}

		_mpi_req_blkset_m(win->w_req);
	}

	return(_mpi_req_advance());
}

/*
 *	lam_osd_push
 *
 *	Function:	- push progress engine
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_push(void)
{
/*
 * This is always done non-blocking.
 */
	_mpi_req_blkclr_m();
	return(_mpi_req_advance());
}

/*
 *	lam_osd_complete
 *
 *	Function:	- complete window's outstanding one-sided requests
 *	Accepts:	- window
 *			- request type
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_complete(MPI_Win win, int reqtype)
{
	MPI_Request	*reqs;
	MPI_Request	p;
	int		err;
	int		nreqs;
	int		cid;
	int		i;

	cid = win->w_comm->c_contextid;

	for (nreqs = 0, p = lam_rq_top; p; p = p->rq_next) {
		if ((p->rq_marks & reqtype) && (p->rq_cid == cid)) {
			nreqs++;
		}
	}

	if (nreqs > 0) {
		reqs = (MPI_Request *) malloc(nreqs * sizeof(MPI_Request));
		if (reqs == 0) {
			return(lam_mkerr(MPI_ERR_OTHER, errno));
		}

		for (i = 0, p = lam_rq_top; p; p = p->rq_next) {
			if ((p->rq_marks & reqtype)
					&& (p->rq_cid == cid)) {
				reqs[i++] = p;
				p->rq_flags &= ~LAM_RQFORPHAN;
			}
		}

		/* Must use LAM_MPI_C_STATUSES_IGNORE as MPISYSF #undefs
		   MPI_STATUSES_IGNORE due to name clash */
		err = MPI_Waitall(nreqs, reqs, LAM_MPI_C_STATUSES_IGNORE);
		if (err != MPI_SUCCESS) return(err);

		free((char *) reqs);
	}

	return(MPI_SUCCESS);
}

/*
 *	lam_osd_fillheader
 *
 *	Function:	- fill a one sided protocol header
 *	Accepts:	- one-sided function being performed
 *			- accumulation operation (if any)
 *			- target displacement
 *			- target count
 *			- target datatype
 *			- header to fill (out)
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_fillheader(int func, int op, MPI_Aint disp, int count, 
		   MPI_Datatype dtype, struct _osdhdr *header)
{
	MPI_Datatype t = MPI_DATATYPE_NULL;
/*
 * Check if datatype supported.  Currently we only support basic types and
 * vectors or contigs of basic types.
 */
	if (dtype) {
		t = dtype;
		while (t->dt_flags & LAM_DTISDUP)
			t = (MPI_Datatype)t->dt_uargs;

		if (!(t->dt_format == LAM_DTBASIC
			|| ((t->dt_format == LAM_DTVECTOR
				|| t->dt_format == LAM_DTCONTIG)
				&& t->dt_dtype->dt_format == LAM_DTBASIC)))
			return(MPI_ERR_TYPENOTSUP);
	}
/*
 * Fill in information.
 */
	header->os_func = func;
	header->os_disp = (int8) disp;
	header->os_op = op;
	if (func == LAM_OSD_ACCUMULATE) {
		header->os_op = op;
	}

	if (dtype) {
		header->os_count = count;
		header->os_format = t->dt_format;

		if (t->dt_format == LAM_DTBASIC) {
			header->os_type = t->dt_f77handle;
		} else {
			t = t->dt_dtype;
			while (t->dt_flags & LAM_DTISDUP)
				t = (MPI_Datatype)t->dt_uargs;

			header->os_type = t->dt_f77handle;
			header->os_dcount = dtype->dt_count;
			if (dtype->dt_format == LAM_DTVECTOR) {
				header->os_blklen = dtype->dt_length;
				header->os_stride = dtype->dt_stride;
			}
		}
	}

	return(MPI_SUCCESS);
}

/*
 *	lam_osd_send
 *
 *	Function:	- send and forget a 1-sided MPI message
 *	Accepts:	- MPI send args
 *			- origin flag
 *			- dynamic data flag
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_send(void *buffer, int count, MPI_Datatype dtype, int dest, 
	     int tag, MPI_Comm comm, int origin, int dynamic)
{
	MPI_Request	req = MPI_REQUEST_NULL;
	int		savecid = comm->c_contextid;
	int		err;

	lam_mpi_mkpt(comm);

	err = _mpi_req_build(buffer, count, dtype,
			     dest, tag, comm, LAM_RQISEND, &req);
	if (err != MPI_SUCCESS) return(err);

	if (dynamic) {
		req->rq_marks |= LAM_RQFDYNBUF;
	}

	if ((err = _mpi_req_start(req)) != MPI_SUCCESS) {
		return(err);
	}

	if (req->rq_state == LAM_RQSDONE) {
		if ((err = _mpi_req_end(req)) != MPI_SUCCESS) {
			return(err);
		}

		if ((err = _mpi_req_destroy(&req)) != MPI_SUCCESS) {
			return(err);
		}
	} else {
		req->rq_flags |= LAM_RQFORPHAN;
		req->rq_marks |= (origin) ? LAM_RQFOSORIG : LAM_RQFOSTARG;
#if LAM_WANT_DEBUG
		MPIL_Request_set_name(req, "Onesided send request");
#endif
		_mpi_req_add_m(req);
	}

	comm->c_contextid = savecid;
	return(MPI_SUCCESS);
}

/*
 *	lam_osd_recv
 *
 *	Function:	- recv and forget a 1-sided MPI message
 *	Accepts:	- MPI recv args
 *			- origin flag
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_recv(void *buffer, int count, MPI_Datatype dtype, int src, 
	     int tag, MPI_Comm comm, int origin)
{
	MPI_Request	req = MPI_REQUEST_NULL;
	int		savecid = comm->c_contextid;
	int		err;

	lam_mpi_mkpt(comm);

	err = _mpi_req_build(buffer, count, dtype,
			src, tag, comm, LAM_RQIRECV, &req);
	if (err != MPI_SUCCESS) return(err);

	if ((err = _mpi_req_start(req)) != MPI_SUCCESS) {
		return(err);
	}

	if (req->rq_state == LAM_RQSDONE) {
		if ((err = _mpi_req_end(req)) != MPI_SUCCESS) {
			return(err);
		}

		if ((err = _mpi_req_destroy(&req)) != MPI_SUCCESS) {
			return(err);
		}
	} else {
		req->rq_flags |= LAM_RQFORPHAN;
		req->rq_marks |= (origin) ? LAM_RQFOSORIG : LAM_RQFOSTARG;
		_mpi_req_add_m(req);
	}

	comm->c_contextid = savecid;
	return(MPI_SUCCESS);
}

/*
 *	lam_osd_handler
 *
 *	Function:	- handler for 1-sided protocol
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_osd_handler(MPI_Request req)
{
	MPI_Win		win;			/* communication window */
	int		rank;			/* rank of origin */
	int		err;

	rank = req->rq_status.MPI_SOURCE;
	win = (MPI_Win) req->rq_extra;

	switch (win->w_header.os_func) {

	case LAM_OSD_ACCUMULATE:
		if (!((win->w_flags & LAM_WFEXPEPOCH)
				|| win->w_pstate[rank] & LAM_WFPOSTED)) {

			if (win->w_flags & LAM_WFNOOUT) {
				return(lam_mkerr(MPI_ERR_EPOCH, EINVAL));
			} else {
				win->w_flags |= LAM_WFACCEPOCH | LAM_WFEXPEPOCH;
			}
		}

		if ((err = lam_accumulate(win, rank)) != MPI_SUCCESS) {
			return(err);
		}
		break;

	case LAM_OSD_GET:
		if (!((win->w_flags & LAM_WFEXPEPOCH)
				|| win->w_pstate[rank] & LAM_WFPOSTED)) {

			if (win->w_flags & LAM_WFNOOUT) {
				return(lam_mkerr(MPI_ERR_EPOCH, EINVAL));
			} else {
				win->w_flags |= LAM_WFACCEPOCH | LAM_WFEXPEPOCH;
			}
		}

		if ((err = lam_get(win, rank)) != MPI_SUCCESS) {
			return(err);
		}
		break;

	case LAM_OSD_PUT:
		if (!((win->w_flags & LAM_WFEXPEPOCH)
				|| win->w_pstate[rank] & LAM_WFPOSTED)) {

			if (win->w_flags & LAM_WFNOOUT) {
				return(lam_mkerr(MPI_ERR_EPOCH, EINVAL));
			} else {
				win->w_flags |= LAM_WFACCEPOCH | LAM_WFEXPEPOCH;
			}
		}

		
		if ((err = lam_put(win, rank)) != MPI_SUCCESS) {
			return(err);
		}
		break;

	case LAM_OSD_COMPLETE:
		win->w_ncomplete--;
		break;

	case LAM_OSD_FENCE:
		win->w_nfence--;
		break;

	default:
		return(lam_mkerr(MPI_ERR_INTERN, EINVAL));
	}

	req->rq_state = LAM_RQSINIT;
	return(MPI_SUCCESS);
}

/*
 *	accumulate
 *
 *	Function:	- target side of an accumulate
 *	Accepts:	- window
 *			- rank of origin
 *	Returns:	- MPI_SUCCESS or error ode
 */
static int
lam_accumulate(MPI_Win win, int rank)
{
	MPI_Request	req = MPI_REQUEST_NULL;
	MPI_Datatype	dtype;
	MPI_Op		op;
	struct accstate	*extra;			/* extra state for handler */
	char		*targbuf;		/* target buffer */
	char		*buffer;		/* incoming msg buffer */
	char		*origin;		/* origin of data in buffer */
	int		savecid;
	int		err;

	dtype = header2dtype(&win->w_header);
	op = GETHDL(win->w_header.os_op);

	if (op == MPI_REPLACE) {
/*
 * Just do a background receive of the data from the origin.
 */
		buffer = (char *)
			win->w_base + win->w_header.os_disp * win->w_disp_unit;

		return(lam_osd_recv(buffer, win->w_header.os_count, dtype,
				rank, LAM_OSD_ACCUMULATE, win->w_comm, FALSE));
	}
/*
 * Receive data into a temporary buffer and then apply the accumulation
 * operation.  
 */
	err = lam_dtbuffer(dtype, win->w_header.os_count, &buffer, &origin);
	if (err != MPI_SUCCESS) return(err);

	savecid = win->w_comm->c_contextid;
	lam_mpi_mkpt(win->w_comm);

	err = _mpi_req_build(origin, win->w_header.os_count, dtype, rank,
			LAM_OSD_ACCUMULATE, win->w_comm, LAM_RQIRECV, &req);
	if (err != MPI_SUCCESS) return(err);

	if ((err = _mpi_req_start(req)) != MPI_SUCCESS) {
		return(err);
	}

	targbuf = (char *)
		win->w_base + win->w_header.os_disp * win->w_disp_unit;

	if (req->rq_state == LAM_RQSDONE) {
/*
 * We have the data so apply the accumulation operation and destroy the
 * request.  
 */
		if ((err = _mpi_req_end(req)) != MPI_SUCCESS) {
			return(err);
		}

		(op->op_func)(origin, targbuf, &win->w_header.os_count, &dtype);

		if ((err = _mpi_req_destroy(&req)) != MPI_SUCCESS) {
			return(err);
		}

		free(buffer);
	} else {
/*
 * The data has not arrived yet so set a handler to apply the
 * accumulation operation once the data is received.  
 */
		extra = (struct accstate *) malloc(sizeof(struct accstate));
		if (extra == 0) {
			return(lam_mkerr(MPI_ERR_OTHER, errno));
		}

		extra->acc_buffer = buffer;
		extra->acc_targbuf = targbuf;
		extra->acc_op = op;
		extra->acc_dtype = dtype;
		extra->acc_count = win->w_header.os_count;

		req->rq_hdlr = accumulate_handler;
		req->rq_extra = extra;
		req->rq_flags |= LAM_RQFORPHAN;
		req->rq_marks |= LAM_RQFOSTARG;
		_mpi_req_add_m(req);
	}

	win->w_comm->c_contextid = savecid;
	return(MPI_SUCCESS);
}


/*
 *	put
 *
 *	Function:	- target side of a put
 *	Accepts:	- window
 *			- rank of origin
 *	Returns:	- MPI_SUCCESS or error ode
 */
static int
lam_put(MPI_Win win, int rank)
{
	char		*buffer;
	MPI_Datatype	dtype;

	buffer = (char *)
		win->w_base + win->w_header.os_disp * win->w_disp_unit;

	dtype = header2dtype(&win->w_header);
/*
 * Start background receiving of the data from the origin.
 */
	return(lam_osd_recv(buffer, win->w_header.os_count, dtype,
			rank, LAM_OSD_PUT, win->w_comm, FALSE));
}


/*
 *	get
 *
 *	Function:	- target side of a get
 *	Accepts:	- window
 *			- rank of origin
 *	Returns:	- MPI_SUCCESS or error ode
 */
static int
lam_get(MPI_Win win, int rank)
{
	char		*buffer;
	MPI_Datatype	dtype;

	buffer = (char *)
		win->w_base + win->w_header.os_disp * win->w_disp_unit;

	dtype = header2dtype(&win->w_header);
/*
 * Start background sending of the data to the origin.
 */
	return(lam_osd_send(buffer, win->w_header.os_count, dtype,
			rank, LAM_OSD_GET, win->w_comm, FALSE, FALSE));
}

/*
 *	accumulate_handler
 *
 *	Function:	- handler for accumulate operation
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
static int
accumulate_handler(MPI_Request req)
{
	struct accstate	*extra;
/*
 * Apply the accumulation operation.  The request is destroyed by the
 * progression engine.
 */
	extra = req->rq_extra;

	(extra->acc_op->op_func)(req->rq_buf, extra->acc_targbuf,
				&extra->acc_count, &extra->acc_dtype);

	free(extra->acc_buffer);
	free((char *) extra);

	return(MPI_SUCCESS);
}

/*
 *	header2dtype
 *
 *	Function:	- get datatype from information in header
 *	Accepts:	- one sided protocol header
 *	Returns:	- datatype or MPI_DATATYPE_NULL (error)
 */
static MPI_Datatype
header2dtype(struct _osdhdr *hdr)
{
	MPI_Datatype	dtype;
	int		err;

	if (hdr->os_format == LAM_DTBASIC) {
		dtype = GETHDL(hdr->os_type);
	}
	else if (hdr->os_format == LAM_DTCONTIG) {
/*
 * Since we only deal with basic types as the subtype we can treat this
 * as a receive of (count * block length) elements of the basic type.
 */
		hdr->os_count *= hdr->os_blklen;
		dtype = GETHDL(hdr->os_type);
	}
	else {
		err = MPI_Type_vector(hdr->os_dcount, hdr->os_blklen,
				hdr->os_stride, GETHDL(hdr->os_type), &dtype);

		if (err != MPI_SUCCESS) {
			dtype = MPI_DATATYPE_NULL;
		} else {
/*
 * MPI_Type_commit() is not called directly to avoid tracing.  The
 * reference count is set to zero and will be incremented to one when
 * creating the background request and then upon completion of the
 * request will go back to zero and the datatype will be deallocated.  
 */
			dtype->dt_commit = 1;
			dtype->dt_refcount = 0;
		}
	}

	return(dtype);
}
