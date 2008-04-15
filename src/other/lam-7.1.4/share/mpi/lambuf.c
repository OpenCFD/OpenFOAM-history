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
 * $Id: lambuf.c,v 6.7 2003/04/10 15:40:01 jsquyres Exp $
 *
 *	Function:	- buffer management functions
 */

#include <stdlib.h>
#include <errno.h>

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <t_types.h>
#include <terror.h>


/*
 * macros
 */
#define HDRSIZE		(sizeof(struct _bsndhdr))


/*
 * local variables
 */
static void		*userbuf = 0;
static int		bufsize = 0;
static struct _bsndhdr	*buftop = 0;
static struct _bsndhdr	*bufend = 0;
static struct _bsndhdr	*buffree = 0;


/*
 * local functions
 */
static void buf_append(struct _bsndhdr *pbuf);
static void buf_free(struct _bsndhdr *pchunk);
static void buf_unlink(struct _bsndhdr *pbuf);
static struct _bsndhdr *buf_alloc(int size);


/*
 *	lam_bufinitbsend_
 *
 *	Function:	- setup bsend buffer
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_bufinitbsend_(MPI_Request rq)
{
/*
 * Handle zero length messages.
 */
	if ((rq->rq_count == 0) || (rq->rq_dtype->dt_size == 0)) {
		rq->rq_packbuf = (char *) rq->rq_buf;
		rq->rq_packsize = 0;
		return(MPI_SUCCESS);
	}

	rq->rq_packsize = rq->rq_count * rq->rq_dtype->dt_size;
/*
 * Allocate a chunk of buffer for the message.
 */
	rq->rq_bsend = buf_alloc(rq->rq_packsize);
	if (rq->rq_bsend == 0) return(lam_mkerr(MPI_ERR_OTHER, ENOBUFS));

	rq->rq_bsend->bsh_req = rq;

	buf_append(rq->rq_bsend);
/*
 * Setup the request for a typical packing buffer.
 * The message will be packed in and out by _mpi_req_start() and
 * _mpi_req_end().  We should optimize with a memcpy() and Trollius
 * data conversion if the message is homogenous and contiguous.
 */
	rq->rq_packbuf = ((char *) rq->rq_bsend) + HDRSIZE;

	return(MPI_SUCCESS);
}


/*
 *	lam_bufinit
 *
 *	Function:	- setup send/recv message buffer
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_bufinit(MPI_Request rq)
{
	MPI_Datatype	dtype;			/* datatype */
	double		local_rep;		/* local data representation */
	double		net_rep;		/* net data representation */

	dtype = rq->rq_dtype;
/*
 * Handle zero length messages.
 */
	if ((rq->rq_count == 0) || (dtype->dt_size == 0)) {
		rq->rq_packbuf = (char *) rq->rq_buf;
		rq->rq_packsize = 0;
		return(MPI_SUCCESS);
	}

	local_rep = 1.1;
	ltotf8(&local_rep, &net_rep);
/*
 * If contiguous, use the caller's buffer.
 */
	rq->rq_packsize = rq->rq_count * dtype->dt_size;

	if ((dtype->dt_flags & LAM_DTNOPACK) &&
            ((dtype->dt_flags & LAM_DTNOXADJ) || rq->rq_count==1) &&
            ((local_rep == net_rep) || lam_homog)) {
          rq->rq_packbuf = (char *) rq->rq_buf;

          /* Check for bad buffer. */

          if (rq->rq_packbuf == 0)
            return(lam_mkerr(MPI_ERR_BUFFER, EINVAL));
	}

        /* Otherwise allocate a buffer. */

	else {
          rq->rq_packbuf = malloc((unsigned) rq->rq_packsize);
          if (rq->rq_packbuf == 0)
            return(lam_mkerr(MPI_ERR_OTHER, errno));
	}

	return(MPI_SUCCESS);
}


/*
 *	lam_buffreebsend_
 *
 *	Function:	- free bsend buffer
 *	Accepts:	- request
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_buffreebsend_(MPI_Request req)
{
/*
 * Handle zero length messages.
 */
	if ((req->rq_count == 0) || (req->rq_dtype->dt_size == 0)) {
		return(MPI_SUCCESS);
	}

	buf_unlink(req->rq_bsend);
	buf_free(req->rq_bsend);
	req->rq_packbuf = 0;

	return(MPI_SUCCESS);
}


/*
 *	lam_bufattach
 *
 *	Function:	- set the user buffer space
 *	Accepts:	- buffer
 *			- buffer size
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_bufattach(void *buf, int size)
{
	unsigned long	inc;			/* alignment increment */

	if (userbuf) {
		return(lam_mkerr(MPI_ERR_OTHER, EBUSY));
	}
/*
 * Determine round up required for alignment.
 */
	if ((inc = (unsigned long) buf % sizeof(void *)) != 0) {
		inc = sizeof(void *) - inc;
	}

	if ((size > 0) && ((unsigned long) size >= HDRSIZE + inc)) {

		userbuf = buf;
		bufsize = size;

		size -= (int) inc;
		size /= sizeof(void *);
		size *= sizeof(void *);

		buffree = (struct _bsndhdr *) ((char *) buf + inc);
		buffree->bsh_req = 0;
		buffree->bsh_size = size - HDRSIZE;
		buffree->bsh_prev = 0;
		buffree->bsh_next = 0;

		return(MPI_SUCCESS);
	}
	else {
		return(lam_mkerr(MPI_ERR_OTHER, EINVAL));
	}
}


/*
 *	lam_bufdetach
 *
 *	Function:	- detach and return user buffer
 *			- block until all requests are done
 *	Accepts:	- ptr buffer
 *			- ptr buffer size
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_bufdetach(void *pbuf, int *psize)
{
	int		err;			/* error code */
	struct _bsndhdr	*p;			/* favourite pointer */
/*
 * Block until all bsend requests are done.
 * Since all bsend requests are orphans, they will be cleaned up
 * by _mpi_req_advance().
 */
	while (buftop) {
/*
 * Mark all bsend requests as blocking and advance the system.
 */
		_mpi_req_blkclr();

		for (p = buftop; p; p = p->bsh_next) {
			_mpi_req_blkset(p->bsh_req);
		}

		err = _mpi_req_advance();
		if (err != MPI_SUCCESS) return(err);
	}
/*
 * Return the buffer to the user.
 */
	*((void **) pbuf) = userbuf;
	*psize = bufsize;

	userbuf = 0;
	bufsize = 0;
	buftop = bufend = buffree = 0;
 
	return(MPI_SUCCESS);
}


/*
 *	buf_alloc
 *
 *	Function:	- allocate a chunk of buffer
 *			- sizes are forced to multiple of pointer size
 *	Accepts:	- buffer size
 *	Returns:	- ptr allocated buffer header or NULL
 */
static struct _bsndhdr *
buf_alloc(int size)
{
	struct _bsndhdr	*pchunk;		/* ptr allocated chunk */
	struct _bsndhdr	*pcur;			/* ptr current block */
	struct _bsndhdr	*pprev;			/* ptr previous block */
/*
 * Round up the size to a multiple of pointer size.
 */
	size = (size + sizeof(void *) - 1) / sizeof(void *);
	size *= sizeof(void *);
/*
 * Loop searching for a big-enough free chunk.
 */
	for (pcur = buffree, pprev = 0; pcur; pcur = pcur->bsh_next) {
		if (pcur->bsh_size >= size) break;
		pprev = pcur;
	}

	if (pcur == 0) return((struct _bsndhdr *) 0);
/*
 * Take a slice from the free chunk.
 */
	if ((pcur->bsh_size - size) >= (2 * HDRSIZE)) {
		pchunk = (struct _bsndhdr *)
				(((char *) pcur) + pcur->bsh_size - size);
		pchunk->bsh_size = size;
		pcur->bsh_size -= size + HDRSIZE;
	}
/*
 * Chunk not worth slicing, take it all.
 */
	else {
		pchunk = pcur;

		if (pprev == 0) {
			buffree = pcur->bsh_next;
		} else {
			pprev->bsh_next = pcur->bsh_next;
		}
	}

	pchunk->bsh_req = 0;
	pchunk->bsh_next = 0;
	pchunk->bsh_prev = 0;

	return(pchunk);
}


/*
 *	buf_free
 *
 *	Function:	- free a chunk of buffer
 *			- add it to the sorted free list
 *			- coalesce with up/down neighbours if possible
 *	Accepts:	- ptr chunk
 */
static void
buf_free(struct _bsndhdr *pchunk)
{
	struct _bsndhdr	*pcur;			/* ptr current chunk */
	struct _bsndhdr	*pprev;			/* ptr previous chunk */
/*
 * Locate the chunk's place in the free list.
 */
	for (pcur = buffree, pprev = 0; pcur; pcur = pcur->bsh_next) {
		if (((long) pcur) > ((long) pchunk)) break;
		pprev = pcur;
	}
/*
 * Link the chunk in the list.
 */
	if (pprev == 0) {
		buffree = pchunk;
	} else {
		pprev->bsh_next = pchunk;
	}

	pchunk->bsh_next = pcur;
/*
 * Coalesce with previous if possible.
 */
	if (pprev) {
		if ( ((long) pchunk) ==
			(((long) pprev) + HDRSIZE + pprev->bsh_size) ) {

			pprev->bsh_next = pchunk->bsh_next;
			pprev->bsh_size += HDRSIZE + pchunk->bsh_size;
			pchunk = pprev;
		}
	}
/*
 * Coalesce with next if possible.
 */
	if (pcur) {
		if (((long) pcur) ==
			(((long) pchunk) + HDRSIZE + pchunk->bsh_size) ) {

			pchunk->bsh_next = pcur->bsh_next;
			pchunk->bsh_size += HDRSIZE + pcur->bsh_size;
		}
	}
}


/*
 *	buf_append
 *
 *	Function:	- append buffer to list of requests
 *	Accepts:	- ptr buffer
 */
static void
buf_append(struct _bsndhdr *pbuf)
{
	pbuf->bsh_prev = bufend;
	pbuf->bsh_next = 0;

	if (bufend == 0) buftop = pbuf;
	else bufend->bsh_next = pbuf;

	bufend = pbuf;
}


/*
 *	buf_unlink
 *
 *	Function:	- unlink buffer from list of requests
 *	Accepts:	- ptr buffer entry
 */
static void
buf_unlink(struct _bsndhdr *pbuf)
{
	struct _bsndhdr	*pprev;			/* ptr previous buffer */
	struct _bsndhdr	*pnext;			/* ptr next buffer */

	pprev = pbuf->bsh_prev;
	pnext = pbuf->bsh_next;

	if (pprev == 0) buftop = pnext;
	else pprev->bsh_next = pnext;

	if (pnext) pnext->bsh_prev = pprev;
	else bufend = pprev;
}
