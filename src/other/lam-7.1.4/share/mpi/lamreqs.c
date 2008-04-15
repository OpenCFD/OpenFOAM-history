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
 * $Id: lamreqs.c,v 6.28 2004/03/02 03:43:29 jsquyres Exp $
 *
 *	Function:	- requests management functions
 */

#include <lam_config.h>

#include <stdlib.h>

#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <lam-ssi-rpi.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#if LAM_WANT_IMPI
#include <impi.h>
#endif
#include <lamdebug.h>
#include <etc_misc.h>


/*
 * external variables
 */
extern struct _proc     *lam_impid_proc;


/*
 * global variables
 */
int			lam_rq_flblock = 0;	/* some block reqs? */
int			lam_rq_nreqs = 0;	/* # requests in list */
int			lam_rq_nactv = 0;	/* # active requests in list */
int			lam_seqnum = 0;		/* message sequence number */
MPI_Request lam_rq_top = MPI_REQUEST_NULL;	/* top list of requests */
MPI_Request lam_rq_bottom = MPI_REQUEST_NULL;	/* bottom list of requests */


/*
 * local variables
 */
static int req_mpid = -1;


/*
 *	_mpi_req_build
 * 
 *	Function:	- builds an MPI request from scratch
 *	Accepts:	- buffer/count/datatype/peer-rank/tag/comm.
 *			- request type
 *			- request (in or out)
 *	Returns: 	- MPI_SUCCESS or error code
 */
int
_mpi_req_build(void *buf, int count, MPI_Datatype dtype, int rank,
	       int tag, MPI_Comm comm, int reqtype, MPI_Request *preq)
{
	MPI_Request	req;			/* request */
	int		err;			/* error code */
/*
 * Check common arguments.
 */
	if (count < 0) {
		return(lam_mkerr(MPI_ERR_COUNT, EINVAL));
	}

	if (dtype == MPI_DATATYPE_NULL || (!dtype->dt_commit)) {
		return(lam_mkerr(MPI_ERR_TYPE, EINVAL));
	}

	if (comm == MPI_COMM_NULL) {
		return(lam_mkerr(MPI_ERR_COMM, EINVAL));
	}

	if ((rank != MPI_PROC_NULL) && (rank != MPI_ANY_SOURCE)) {
		if (LAM_IS_INTER(comm)) {
			if ((rank < 0) || (rank >= comm->c_rgroup->g_nprocs)) {
				return(lam_mkerr(MPI_ERR_RANK, EINVAL));
			}
		} else {
			if ((rank < 0) || (rank >= comm->c_group->g_nprocs)) {
				return(lam_mkerr(MPI_ERR_RANK, EINVAL));
			}
		}
	}
/*
 * Allocate a request if needed.
 */
	if (*preq == MPI_REQUEST_NULL) {
          if (req_mpid == -1)
            req_mpid = lam_mp_init(sizeof(struct _req), NULL, NULL,
                                   LAM_DEFAULT_MP_POOL_SIZE,
                                   LAM_DEFAULT_MP_INCR_SIZE, 1);
          *preq = (MPI_Request) lam_mp_malloc(req_mpid);
          if (*preq == NULL) 
            return(lam_mkerr(MPI_ERR_OTHER, errno));
          (*preq)->rq_marks = LAM_RQFDYNREQ;
	} else {
		(*preq)->rq_marks = 0;
	}

	req = *preq;
/*
 * Handle special case of peer MPI_PROC_NULL.
 */
	req->rq_name = NULL;
	if (rank == MPI_PROC_NULL) {
		req->rq_state = LAM_RQSINIT;
		req->rq_rank = MPI_PROC_NULL;
		req->rq_hdlr = NULL;
		req->rq_flags = 0;
		req->rq_comm = MPI_COMM_NULL;
		req->rq_shadow = MPI_REQUEST_NULL;
		return(MPI_SUCCESS);
	}
/*
 * Fill the request.
 */
	req->rq_type = reqtype;
	req->rq_state = LAM_RQSINIT;
	req->rq_flags = 0;
	req->rq_next = MPI_REQUEST_NULL;
	req->rq_hdlr = NULL;
	req->rq_shadow = MPI_REQUEST_NULL;
	req->rq_extra = NULL;
	req->rq_f77handle = -1;
/*
 * Save the original communication parameters.
 */
	req->rq_buf = buf;
	req->rq_count = count;
	req->rq_dtype = dtype;
	req->rq_rank = rank;
	req->rq_tag = tag;
	req->rq_comm = comm;
	req->rq_cid = comm->c_contextid;
/*
 * Note additional usage of the communicator and datatype.
 */
	++(comm->c_refcount);
	++(dtype->dt_refcount);
/*
 * Set peer process.
 */
	if (rank == MPI_ANY_SOURCE) {
		req->rq_proc = NULL;
	} else {
		req->rq_proc = ((LAM_IS_INTER(comm)) ?
				comm->c_rgroup->g_procs[rank] :
				comm->c_group->g_procs[rank]);

#if LAM_WANT_IMPI
/*
 * If this is an IMPI job, and the impid_comm has been established,
 * if this is a proxy send to the impid, reset the proc to point to
 * the one, true proc to the impid (not the proxy).
 */
		if (lam_impid_proc != NULL) {
		  if (LAM_GPSCMP(&req->rq_proc->p_gps, 
				 &lam_impid_proc->p_gps) == 0) {
		    req->rq_proc = lam_impid_proc;
		  }
		}
#endif
	}
/*
 * Establish a buffer, if needed, for the request.
 * Buffered sends need new buffers on each request start.
 * Probes don't have buffers.  Caller should have passed buf==0.
 * Fake requests don't communicate anything.
 */
	if (reqtype == LAM_RQIFAKE) {
		req->rq_packbuf = (char *) req->rq_buf;
		req->rq_packsize = 0;
	} else if (reqtype == LAM_RQIPROBE) {
		req->rq_buf = NULL;
		req->rq_packbuf = (char *) req->rq_buf;
		req->rq_packsize = 0;
	} else if ((reqtype != LAM_RQIBSEND)) {
		if ((count == 0) || (dtype->dt_size == 0)) {
			req->rq_packbuf = (char *) req->rq_buf;
			req->rq_packsize = 0;
		} else {
			err = lam_bufinit(req);
			if (err != MPI_SUCCESS) 
			  return(err);
		}
	}
/*
 * Categorize the direction of the communication.
 */
	if ((reqtype == LAM_RQIRECV) || (reqtype == LAM_RQIPROBE)) {
		req->rq_marks |= LAM_RQFDEST;
	} else {
		req->rq_marks |= LAM_RQFSOURCE;
	}

	req->rq_rpi = NULL;
	if (RPI_BUILD(req)) {
		err = lam_mkerr(MPI_ERR_INTERN, errno);
		if (err != MPI_SUCCESS) 
		  return(err);
	}

	return(MPI_SUCCESS);
}

/*
 *	_mpi_req_start
 *
 *	Function:	- resets the request for a new communication
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
_mpi_req_start(MPI_Request req)
{
	int		err;
	MPI_Request	cur;

/*
 * Ensure to start the request and all of its shadows
 */
	for (cur = req; cur != MPI_REQUEST_NULL; cur = cur->rq_shadow) {
	  cur->rq_flags = 0;
/*
 * Even though only receive functions have to return valid infomation
 * in the status, we might as well be social and fill in "bad" values
 * for the status so that if someone does a non-blocking send and
 * tries to look at the status returned by MPI_Test* or MPI_Wait*,
 * they will get sentinenal values (and avoid read-from-unitialized
 * errors).  Real values will be filled in for receives elsewhere in
 * the RPI.
 */
	  cur->rq_status.MPI_SOURCE = MPI_UNDEFINED;
	  cur->rq_status.MPI_TAG = MPI_UNDEFINED;
	  cur->rq_status.MPI_ERROR = MPI_SUCCESS;
/*
 * Handle special case of peer MPI_PROC_NULL.
 */
	  if (cur->rq_rank == MPI_PROC_NULL) {
		cur->rq_state = LAM_RQSDONE;
		return(MPI_SUCCESS);
	  }

	  lam_rq_nactv++;
	  cur->rq_state = LAM_RQSSTART;
/*
 * Set the sequence number.
 */
	  if (cur->rq_marks & LAM_RQFSOURCE) {
		cur->rq_seq = lam_seqnum++;
		cur->rq_status.MPI_SOURCE = MPI_PROC_NULL;
/*
 * Establish a buffer for a bsend.
 */
		if (cur->rq_type == LAM_RQIBSEND) {
			err = lam_bufinitbsend_(cur);
			if (err != MPI_SUCCESS) 
			  return(err);
			cur->rq_flags |= LAM_RQFORPHAN;
		}
/*
 * Pack the message, if necessary.
 */
		if (cur->rq_packbuf != (char *) cur->rq_buf) {

			if (lam_pack((char *) cur->rq_buf, cur->rq_count,
					cur->rq_dtype, cur->rq_packbuf,
					cur->rq_packsize) < 0) {
				return(lam_mkerr(MPI_ERR_INTERN,
						errno));
			}
		}
	}
/*
 * Reset peer process in the wildcard receive case since for a restarted
 * persistent request it will have been set to the source process when it
 * last completed.
 */
	  else if (cur->rq_rank == MPI_ANY_SOURCE) {
		cur->rq_proc = NULL;
	  }

	  if (RPI_START(lam_rq_top, cur)) {
		return(lam_mkerr(MPI_ERR_INTERN, errno));
	  }
	}

	return(MPI_SUCCESS);
}

/*
 *	_mpi_req_destroy
 *
 *	Function:	- destroys a request
 *	Accepts:	- request ptr
 *	Returns:	- MPI_SUCCESS or error code
 */
int
_mpi_req_destroy(MPI_Request *preq)
{
	MPI_Request	req;
	int		err;
	int		ret;

	ret = MPI_SUCCESS;
	req = *preq;
	if (req->rq_name != NULL)
	  free(req->rq_name);
/*
 * Handle special case of peer MPI_PROC_NULL.
 */
	if (req->rq_rank == MPI_PROC_NULL) {
		if (req->rq_marks & LAM_RQFDYNREQ) {
			free((char *) req);
			*preq = MPI_REQUEST_NULL;
		}

		return(MPI_SUCCESS);
	}
/*
 * Free communicator and datatype.
 */
	if (--(req->rq_comm->c_refcount) == 0) {

	  /* Tell the coll SSI module that this comm is now gone */

	  if ((err = 
	       lam_ssi_coll_base_finalize_comm(req->rq_comm)) != 0)
	    ret = err;

		err = lam_comm_free(req->rq_comm);
		if (err != MPI_SUCCESS) ret = err;
	}
			
	if (--(req->rq_dtype->dt_refcount) == 0) {
		err = lam_type_free(req->rq_dtype);
		if (err != MPI_SUCCESS) ret = err;
	}
/*
 * Free the packing buffer (if dynamic).
 * Bsend buffers are different beasts that are freed on request end.
 * Fake requests do not communicate and don't have a packing buffer.
 */
	if (((req->rq_type != LAM_RQIBSEND) && (req->rq_type != LAM_RQIFAKE) &&
			(req->rq_packbuf != (char *) req->rq_buf))
			|| (req->rq_marks & LAM_RQFDYNBUF)) {
		free(req->rq_packbuf);
	}
/*
 * Remove the fortran handle if the user called MPI_Request_f2c on
 * this handle
 */
	if (req->rq_f77handle >= 0)
	  lam_F_free_hdl(req->rq_f77handle);
/*
 * Clear request and free it.
 */
	if (RPI_DESTROY(req)) {
	  ret = lam_mkerr(MPI_ERR_INTERN, errno);
	}
	if (req->rq_marks & LAM_RQFDYNREQ) {
          lam_mp_free(req_mpid, req);
          *preq = MPI_REQUEST_NULL;
	}

	return(ret);
}

/*
 *	_mpi_req_add
 *
 *	Function:	- add request to bottom of list
 *	Accepts:	- request
 */
void
_mpi_req_add(MPI_Request req)
{
/*
 * Ensure to add all the shadows, too.  Do this in a reverse-list
 * order, so that lamgiappes get sent out first, etc.
 */
	if (req->rq_shadow != MPI_REQUEST_NULL)
	  _mpi_req_add(req->rq_shadow);
	if (lam_rq_top == MPI_REQUEST_NULL) {
		lam_rq_top = req;
	} else {
		lam_rq_bottom->rq_next = req;
	}

	lam_rq_bottom = req;
	req->rq_next = MPI_REQUEST_NULL;

	++lam_rq_nreqs;
}

/*
 *	_mpi_req_rem
 *
 *	Function:	- removes a request from the list
 *	Accepts:	- request
 */
void
_mpi_req_rem(MPI_Request req)
{
	MPI_Request	p, p2;		/* favourite pointers */

/*
 * Locate the request in the list.
 */
	for (p = lam_rq_top, p2 = MPI_REQUEST_NULL; 
             p != MPI_REQUEST_NULL && (p != req); p2 = p, p = p->rq_next);
	if (p == MPI_REQUEST_NULL) 
	  return;
/*
 * Unlink the request from the list.
 */
	if (p == lam_rq_top) {
		lam_rq_top = p->rq_next;
	} else {
		p2->rq_next = p->rq_next;
	}

	if (p == lam_rq_bottom) lam_rq_bottom = p2;

	--lam_rq_nreqs;
}

/*
 *	_mpi_req_advance
 *
 *	Function:	- advances state of requests
 *	Returns:	- MPI_SUCCESS or error code
 */
int
_mpi_req_advance(void)
{
	MPI_Request	p;
	MPI_Request	p2;
	MPI_Request	prev;			/* ptr previous req. */
	int		err;			/* error code */
	int		r;
/*
 * If there are no requests, we are done.
 */
	if (lam_rq_nreqs < 1) 
	  return(MPI_SUCCESS);
/*
 * If there are blocking requests, we continue advancing until one
 * of the blocking requests reaches the done state.
 */
	if (lam_rq_flblock) {

	    do {
/*
 * Advance the request list.  Blocking is permitted.
 */
		if (RPI_ADVANCE(lam_rq_top, TRUE) < 0) {
			return(lam_mkerr(MPI_ERR_INTERN, errno));
		}
/*
 * Search the request list for a completed blocking request.
 * Optimization:  Look at the last request first.
 * Execute handlers for completed requests which have them.
 */
		if (lam_rq_bottom->rq_state == LAM_RQSDONE) {

		    p = lam_rq_bottom;
		    if (p->rq_flags & LAM_RQFBLOCK) {
		      break;
		    }

		    if (p->rq_hdlr && 
			(p->rq_flags & LAM_RQFHDLDONE) == 0) {
			err = _mpi_req_end(p);
			if (err != MPI_SUCCESS) 
			  return(err);
/*			
 * Only invoke handlers if the request was not canceled
 */
			if ((p->rq_flags & LAM_RQFCANCEL) == 0) {
			  err = (*p->rq_hdlr)(p);
			  if (err != MPI_SUCCESS) 
			    return(err);
			}
			p->rq_flags |= LAM_RQFHDLDONE;

			if (p->rq_flags & LAM_RQFORPHAN) {
			    _mpi_req_rem(p);

			    err = _mpi_req_destroy(&p);
			    if (err != MPI_SUCCESS) 
			      return(err);
			}
		    }
		}

		p = lam_rq_top;

		while (p != lam_rq_bottom) {

		    if (p->rq_state == LAM_RQSDONE) {
			if (p->rq_flags & LAM_RQFBLOCK) {
			  break;
			}

			if (p->rq_hdlr && 
			    (p->rq_flags & LAM_RQFHDLDONE) == 0) {
			    err = _mpi_req_end(p);
			    if (err != MPI_SUCCESS) 
			      return(err);
/*			
 * Only invoke handlers if the request was not canceled
 */
			    if ((p->rq_flags & LAM_RQFCANCEL) == 0) {
			      err = (*p->rq_hdlr)(p);
			      if (err != MPI_SUCCESS) 
				return(err);
			    }
			    p->rq_flags |= LAM_RQFHDLDONE;

			    if (p->rq_flags & LAM_RQFORPHAN) {
				p2 = p;
				p = p->rq_next;
				_mpi_req_rem_m(p2);

				err = _mpi_req_destroy(&p2);
				if (err != MPI_SUCCESS) 
				  return(err);

				continue;
			    }
			}
		    }
		    p = p->rq_next;
		}
	    } while (p == lam_rq_bottom);
	}
/*
 * If there are no blocking requests, we continue advancing until
 * no progress is made.
 */
	else {

	    do {
/* 
 * Advance the request list.  Blocking is not permitted.
 */
	      r = RPI_ADVANCE(lam_rq_top, FALSE);
	      if (r < 0) 
		return(lam_mkerr(MPI_ERR_INTERN, errno));
	    } while (r == 1);
	}
/*
 * Execute handlers for any done requests which have them.
 * Free all requests that are both orphaned and done.
 * Clearly, all these request list loops could be merged and optimized,
 * although it will never be trivial because you don't know what or how
 * many requests _rpi_advance() completed.  This keeps RPI implementations
 * simpler and we bear the pain here - a command decision.
 */
	for (p = lam_rq_top, prev = MPI_REQUEST_NULL; p; 
		p = (prev) ? prev->rq_next : lam_rq_top) {

	    if (p->rq_state == LAM_RQSDONE) {

		if (p->rq_hdlr && (p->rq_flags & LAM_RQFHDLDONE) == 0) {
		    err = _mpi_req_end(p);
		    if (err != MPI_SUCCESS) 
		      return(err);

/*			
 * Only invoke handlers if the request was not canceled
 */
		    if ((p->rq_flags & LAM_RQFCANCEL) == 0) {
		      err = (*p->rq_hdlr)(p);
		      if (err != MPI_SUCCESS) 
			return(err);
		    }
		    p->rq_flags |= LAM_RQFHDLDONE;
		}

		if (p->rq_flags & LAM_RQFORPHAN) {
		    _mpi_req_rem(p);

		    if (!p->rq_hdlr) {
			err = _mpi_req_end(p);
			if (err != MPI_SUCCESS) 
			  return(err);
		    }
		    
		    err = _mpi_req_destroy(&p);
		    if (err != MPI_SUCCESS) 
		      return(err);
		} else {
		    prev = p;
		}
	    } else {
		prev = p;
	    }
	}

	return(MPI_SUCCESS);
}

/*
 *	_mpi_req_blkclr
 *
 *	Function:	- clear requests block flag
 */
void
_mpi_req_blkclr(void)
{
	MPI_Request	p;			/* favourite pointer */

	for (p = lam_rq_top; p; p = p->rq_next) {
		p->rq_flags &= ~LAM_RQFBLOCK;
	}

	lam_rq_flblock = 0;
}

/*
 *	_mpi_req_blkset
 *
 *	Function:	- set request as blocking
 *	Accepts:	- request
 */
void
_mpi_req_blkset(MPI_Request req)
{
	req->rq_flags |= LAM_RQFBLOCK;
	lam_rq_flblock = 1;
}

/*
 *	_mpi_req_getn
 *
 *	Function:	- get the # requests
 *	Returns:	- # requests
 */
int
_mpi_req_getn(void)
{
	return(lam_rq_nreqs);
}

/*
 *	_mpi_req_get
 *
 *	Function:	- get requests
 *	Accepts:	- # array entries
 *			- requests array (returned values)
 */
void
_mpi_req_get(int n, MPI_Request *reqs)
{
	MPI_Request	p;			/* favourite pointer */
	int		i;			/* favourite index */

	for (i = 0, p = lam_rq_top; p && (i < n); ++i, p = p->rq_next) {
		reqs[i] = p;
	}

	for (; i < n; ++i) reqs[i] = MPI_REQUEST_NULL;
}

/*
 *	_mpi_req_end
 *
 *	Function:	- finishes a request that is done
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
_mpi_req_end(MPI_Request req)
{
    int			err;
/*
 * Handle special case of peer MPI_PROC_NULL.
 * These requests cannot be cancelled as they are either inactive or
 * done.
 */
    if (req->rq_rank == MPI_PROC_NULL) {
	req->rq_status.MPI_SOURCE = MPI_PROC_NULL;
	req->rq_status.MPI_TAG = MPI_ANY_TAG;
	req->rq_status.st_length = 0;
	return(MPI_SUCCESS);
    }
/*
 * No communication occurred if the receive request was cancelled.
 */
    if ((req->rq_type == LAM_RQIRECV)
	    && !(req->rq_flags & LAM_RQFCANCEL)
	    && (req->rq_status.MPI_ERROR == MPI_SUCCESS)) {
/*
 * Unpack the message, if necessary.
 */
	if (req->rq_packbuf != (char *) req->rq_buf) {

	    if (lam_unpack((char *) req->rq_packbuf, req->rq_status.st_length,
		    (char *) req->rq_buf, req->rq_count, req->rq_dtype) < 0) {
		return(lam_mkerr(MPI_ERR_INTERN, errno));
	    }
	}
/*
 * Check for error truncate.
 */
	if (req->rq_flags & LAM_RQFTRUNC) {
	    req->rq_status.MPI_ERROR = lam_mkerr(MPI_ERR_TRUNCATE, EIO);
	}
    }
/*
 * Free the message buffer from the attached buffer.
 */
    else if (req->rq_type == LAM_RQIBSEND) {
	err = lam_buffreebsend_(req);
	if (err != MPI_SUCCESS) return(err);
    }

    return(MPI_SUCCESS);
}
