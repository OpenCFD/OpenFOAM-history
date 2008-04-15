/*
 * Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
 * $Id: ssi_rpi_sysv_low.c,v 1.9 2004/01/02 00:16:58 jsquyres Exp $
 *
 *	Function:	- universal shared memory low-level routines
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>

#if defined(HAVE_SYS_SELECT_H) && HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <blktype.h>
#include <dl_inet.h>
#include <mpi.h>
#include <mpisys.h>
#include <net.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>

#include <lam-ssi-rpi.h>
#include <rpi_sysv.h>
#include <sysv_common.h>
#include <sysv_low.h>
#include <sysv_shmem.h>

/*
 * We somewhat abuse the SSI RPI system here -- this RPI is tied to
 * the TCP RPI.  Hence, we know some of the variables and functions
 * that it exports.
 */
#include <tcp_low.h>


/*
 * private functions
 */
static int waitforack(struct lam_ssi_rpi_proc *, int, int, MPI_Comm, int *);


/*
 *	_shm_proc_read_env
 *
 *	Function:	- read envelope from process
 *			- if full envelope read in then try to match with
 *			  and advance a receiving request
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_proc_read_env(struct lam_ssi_rpi_proc *ps)
{
  int lock;
  double starttime = 0.0;

  if (lam_ssi_rpi_tcp_flblock) {
    LAM_TRACE(starttime = ttime());
    if (lam_ssi_rpi_sysv_readlock(ps))
      return (LAMERROR);
    LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));
  } else {
    if ((lock = lam_ssi_rpi_sysv_readtrylock(ps)) < 0) {
      return (LAMERROR);
    } else if (lock == 1) {
      return (0);
    }
  }
  ps->cp_locked = 1;

  return (lam_ssi_rpi_sysv_match_adv(ps));
}

/*
 *	lam_ssi_rpi_sysv_proc_read_body_box
 *
 *	Function:	- read the body of an MPI message from process
 *			  via the postbox
 *			- this is only called when there is a receiving request
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_proc_read_body_box(struct lam_ssi_rpi_proc *ps)
{
  int len;
  int lock;

  if (ps->cp_locked) {
/*
 * Already locked, copy data minus the envelope from shared buffer to receiver.
 */
    len = LAM_min(ps->cp_nmsgin, lam_ssi_rpi_sysv_short);
    lam_memcpy(ps->cp_msgbuf, ps->cp_inbox + 1, len);

    if (lam_ssi_rpi_sysv_readunlock(ps))
      return (LAMERROR);

    ps->cp_msgbuf += len;
    ps->cp_nmsgin -= len;
    ps->cp_locked = 0;
  }

  if (lam_ssi_rpi_tcp_flblock) {
/*
 * Blocking case. Loop until all of the message has been read.
 */
    while (ps->cp_nmsgin) {
      if (lam_ssi_rpi_sysv_readlock(ps))
	return (LAMERROR);

      len = LAM_min(ps->cp_nmsgin, lam_ssi_rpi_sysv_short);
      lam_memcpy(ps->cp_msgbuf, ps->cp_inbox + 1, len);

      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);

      ps->cp_msgbuf += len;
      ps->cp_nmsgin -= len;
    }
  } else {
/*
 * Non-blocking case.  Loop until all ofthe message has been read or we
 * cannot get the lock in which case we just exit to try again another
 * day.  
 */
    while (ps->cp_nmsgin) {

      if ((lock = lam_ssi_rpi_sysv_readtrylock(ps)) < 0) {
	return (LAMERROR);
      } else if (lock == 1) {
	return (0);
      }

      len = LAM_min(ps->cp_nmsgin, lam_ssi_rpi_sysv_short);
      lam_memcpy(ps->cp_msgbuf, ps->cp_inbox + 1, len);

      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);

      ps->cp_msgbuf += len;
      ps->cp_nmsgin -= len;
    }
  }
/*
 * All of message has been read.  Advance the request receiving the message.
 */
  if (ps->cp_rreq->rq_rpi->cq_adv(ps, ps->cp_rreq)) {
    return (LAMERROR);
  }
/*
 * Set process up to read the next incoming envelope.
 */
  ps->cp_rreq = 0;
  ps->cp_readfn = lam_ssi_rpi_sysv_proc_read_env;
  return (0);
}

/*
 *	_shm_proc_read_body_pool
 *
 *	Function:	- read the body of an MPI message from a process
 *			  via the shared pool
 *			- this is only called when there is a receiving request
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_proc_read_body_pool(struct lam_ssi_rpi_proc *ps)
{
  int lock;
  int len;
  char *buf = 0;

  if (ps->cp_locked) {
/*
 * Already locked, copy data minus the envelope from shared pool to receiver.
 */
    buf = lam_ssi_rpi_sysv_membase + ps->cp_inbox->pb_header.bh_bufoff;
    len = LAM_min(ps->cp_nmsgin, ps->cp_insize);
    lam_memcpy(ps->cp_msgbuf, buf, len);

    if (lam_ssi_rpi_sysv_readunlock(ps))
      return (LAMERROR);

    ps->cp_msgbuf += len;
    ps->cp_nmsgin -= len;
    ps->cp_locked = 0;
  }

  if (lam_ssi_rpi_tcp_flblock) {
/*
 * Blocking case. Loop until all of message has been read.
 */
    while (ps->cp_nmsgin) {
      if (lam_ssi_rpi_sysv_readlock(ps))
	return (LAMERROR);

      buf = lam_ssi_rpi_sysv_membase + ps->cp_inbox->pb_header.bh_bufoff;
      len = LAM_min(ps->cp_nmsgin, ps->cp_insize);
      lam_memcpy(ps->cp_msgbuf, buf, len);

      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);

      ps->cp_msgbuf += len;
      ps->cp_nmsgin -= len;
    }
  } else {
/*
 * Non-blocking case.  Loop until all of message has been read or we cannot
 * get the lock in which case we just exit to try again another day.
 */
    while (ps->cp_nmsgin) {
      if ((lock = lam_ssi_rpi_sysv_readtrylock(ps)) < 0) {
	return (LAMERROR);
      } else if (lock == 1) {
	return (0);
      }

      buf = lam_ssi_rpi_sysv_membase + ps->cp_inbox->pb_header.bh_bufoff;
      len = LAM_min(ps->cp_nmsgin, ps->cp_insize);
      lam_memcpy(ps->cp_msgbuf, buf, len);

      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);

      ps->cp_msgbuf += len;
      ps->cp_nmsgin -= len;
    }
  }
/*
 * All of message has been read.  Free shared area and advance the
 * request receiving the message.  
 */
  if (buf)
    lam_ssi_rpi_sysv_shfree(buf);

  if (ps->cp_rreq->rq_rpi->cq_adv(ps, ps->cp_rreq)) {
    return (LAMERROR);
  }
/*
 * Set process up to read the next incoming envelope.
 */
  ps->cp_rreq = 0;
  ps->cp_readfn = lam_ssi_rpi_sysv_proc_read_env;
  return (0);
}

/*
 *	_shm_req_done_synch
 *
 *	Function:	- mark synchronous send request as done
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_req_done_synch(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  if (lam_ssi_rpi_sysv_readunlock(ps))
    return (LAMERROR);
  ps->cp_locked = 0;

  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}

/*
 *	_shm_req_rcvd_long_ack
 *
 *	Function:	- long protocol transition from reading ack to
 *			  done (if receiver wants 0 bytes) or sending
 *			  requested # of bytes
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_req_rcvd_long_ack(struct lam_ssi_rpi_proc *ps, 
				   MPI_Request req)
{
  int len;

  len = ps->cp_inbox->pb_header.bh_env.ce_len;

  if (lam_ssi_rpi_sysv_readunlock(ps))
    return (LAMERROR);
  ps->cp_locked = 0;

  lam_ssi_rpi_tcp_haveadv = 1;
/*
 * If the receiver truncated the message we may already be done.  The
 * length field of the incoming ack packet specifies the amount of data
 * the receiver expected in total.  We have already send a short packets
 * worth.  
 */
  len -= lam_ssi_rpi_sysv_short;

  if (len <= 0) {
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  } else {
    req->rq_rpi->cq_state = C2CWRITE;
    req->rq_rpi->cq_env.ce_len = len;
    req->rq_rpi->cq_env.ce_flags &= ~C2CACK;
    req->rq_rpi->cq_env.ce_flags |= C2C2ND;
    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;
    req->rq_rpi->cq_adv = lam_ssi_rpi_sysv_req_send_body_first;
    req->rq_rpi->cq_nenvout = sizeof(struct lam_ssi_rpi_envl);
    req->rq_rpi->cq_nmsgout = len;
  }

  return (0);
}

/*
 *	_shmtcp_req_recv
 *
 *	Function:	- determine protocol for receive request on
 *			  matched incoming envelope and act upon it
 *			- this is called for the first packet only
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_tcp_req_recv(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  struct lam_ssi_rpi_envl * env;			/* the envelope */
  int len;

  if (ps->cp_sock >= 0) {
/*
 * TCP protocol takes care of it.
 */
    return (lam_ssi_rpi_tcp_req_recv(ps, req));
  }

  env = (struct lam_ssi_rpi_envl *) ps->cp_inbox;
  lam_ssi_rpi_sysv_fill_wildcards(req, env);
  lam_ssi_rpi_tcp_haveadv = 1;
/*
 * Check for message length mismatch and set status.
 */
  if (env->ce_len > req->rq_packsize) {
    req->rq_flags |= LAM_RQFTRUNC;
    env->ce_len = req->rq_packsize;
  }
  lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);

  if (env->ce_flags & C2CLONG) {
/*
 * Got a long protocol envelope. Copy data part into the receive buffer
 * and reply with an ack.  
 */
    len = env->ce_len;
    lam_memcpy(req->rq_packbuf, ps->cp_inbox + 1, LAM_min(len,
						      lam_ssi_rpi_sysv_short));
/*
 * Done with data transfer, unlock the postbox.
 */
    if (lam_ssi_rpi_sysv_readunlock(ps))
      return (LAMERROR);
    ps->cp_locked = 0;

    req->rq_state = LAM_RQSACTIVE;
    req->rq_rpi->cq_state = C2CWRITE;
    req->rq_rpi->cq_env.ce_flags |= (C2CACK | C2CLONG);
    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;
    req->rq_rpi->cq_nenvout = sizeof(struct lam_ssi_rpi_envl);
    if (len > lam_ssi_rpi_sysv_short) {
      req->rq_rpi->cq_env.ce_len = len;
      req->rq_rpi->cq_adv = lam_ssi_rpi_sysv_req_send_ack_long;
    } else {
      req->rq_rpi->cq_env.ce_len = 0;
      req->rq_rpi->cq_adv = lam_ssi_rpi_sysv_req_send_ack_only;
    }
  } else {
/*
 * Got a short protocol envelope.  Copy the message from postbox.
 */
    if (env->ce_len > 0) {
      lam_memcpy(req->rq_packbuf, ps->cp_inbox + 1, env->ce_len);
    }
/*
 * Send ack if matched a synchronous send otherwise complete the request.
 */
    if (env->ce_flags & C2CSSEND) {
      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);
      ps->cp_locked = 0;

      req->rq_state = LAM_RQSACTIVE;
      if (lam_ssi_rpi_sysv_req_rcvd_body_synch(0, req)) {
	return (LAMERROR);
      }
    } else {
      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);
      ps->cp_locked = 0;

      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    }
  }

  return (0);
}

/*
 *	_shm_buffer
 *
 *	Function:	- buffer incoming envelope/message
 *			- there is never any data to be read to the
 *			  data sink when buffering
 *	Accepts:	- process envelope came in from
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_buffer(struct lam_ssi_rpi_proc *ps)
{
  struct lam_ssi_rpi_cbuf_msg msg;		/* buffer list entry */
  int len;

  /* In 64 bit mode, this struct may have extra padding */

  LAM_ZERO_ME(msg);
  msg.cm_env = ps->cp_inbox->pb_header.bh_env;
  len = LAM_min(msg.cm_env.ce_len, lam_ssi_rpi_sysv_short);

  if (len > 0) {
/*
 * Set up the buffer for the message body and copy into it.
 */
    msg.cm_buf = (char *) malloc(len);
    if (msg.cm_buf == 0)
      return (LAMERROR);

    lam_memcpy(msg.cm_buf, ps->cp_inbox + 1, len);
  } else {
    msg.cm_buf = 0;
  }

  if (lam_ssi_rpi_sysv_readunlock(ps))
    return (LAMERROR);
  ps->cp_locked = 0;

  msg.cm_proc = 0;
  msg.cm_req = 0;
  msg.cm_dont_delete = 0;
  return (lam_ssi_rpi_cbuf_append(&msg) ? 0 : LAMERROR);
}


/*
 *	_shm_push_body_box
 *
 *	Function:	- push request envelope and message body down the pike
 *			  via the postbox
 *	Accepts:	- process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_push_body_box(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int lock;
  double starttime = 0.0;
  int len;

  if (lam_ssi_rpi_tcp_flblock) {
/*
 * Blocking case.
 */
    if (req->rq_rpi->cq_nenvout > 0) {
      LAM_TRACE(starttime = ttime());
      if (lam_ssi_rpi_sysv_writelock(ps))
	return (LAMERROR);
      LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

      ps->cp_outbox->pb_header.bh_env = req->rq_rpi->cq_env;

      if (req->rq_rpi->cq_nmsgout > 0) {
	len = LAM_min(req->rq_rpi->cq_nmsgout, lam_ssi_rpi_sysv_short);
	lam_memcpy(ps->cp_outbox + 1, req->rq_rpi->cq_msgbuf, len);
	req->rq_rpi->cq_msgbuf += len;
	req->rq_rpi->cq_nmsgout -= len;
      }

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_nenvout = 0;
    }

    while (req->rq_rpi->cq_nmsgout > 0) {
      if (lam_ssi_rpi_sysv_writelock(ps))
	return (LAMERROR);

      len = LAM_min(req->rq_rpi->cq_nmsgout, lam_ssi_rpi_sysv_short);
      lam_memcpy(ps->cp_outbox + 1, req->rq_rpi->cq_msgbuf, len);

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_msgbuf += len;
      req->rq_rpi->cq_nmsgout -= len;
    }
  } else {
/*
 * Non-blocking case.  Exit to try again another day as soon as we fail
 * to obtain the lock.
 */
    if (req->rq_rpi->cq_nenvout > 0) {
      if ((lock = lam_ssi_rpi_sysv_writetrylock(ps)) < 0) {
	return (LAMERROR);
      } else if (lock == 1) {
	return (0);
      }

      ps->cp_outbox->pb_header.bh_env = req->rq_rpi->cq_env;

      if (req->rq_rpi->cq_nmsgout > 0) {
	len = LAM_min(req->rq_rpi->cq_nmsgout, lam_ssi_rpi_sysv_short);
	lam_memcpy(ps->cp_outbox + 1, req->rq_rpi->cq_msgbuf, len);
	req->rq_rpi->cq_msgbuf += len;
	req->rq_rpi->cq_nmsgout -= len;
      }

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_nenvout = 0;
    }

    while (req->rq_rpi->cq_nmsgout > 0) {
      if ((lock = lam_ssi_rpi_sysv_writetrylock(ps)) < 0) {
	return (LAMERROR);
      } else if (lock == 1) {
	return (0);
      }

      len = LAM_min(req->rq_rpi->cq_nmsgout, lam_ssi_rpi_sysv_short);
      lam_memcpy(ps->cp_outbox + 1, req->rq_rpi->cq_msgbuf, len);

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_msgbuf += len;
      req->rq_rpi->cq_nmsgout -= len;
    }
  }

  return (1);
}

/*
 *	_shm_push_body_pool
 *
 *	Function:	- push request envelope and message body down the pike
 *			  via the shared pool
 *	Accepts:	- process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_push_body_pool(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int lock;
  double starttime = 0.0;
  int len;
  int size;			/* size of shared pool buffer */
  char *buf;			/* shared pool buffer */

  size = req->rq_rpi->cq_bufsize;
  buf = lam_ssi_rpi_sysv_membase + req->rq_rpi->cq_bufoff;

  if (lam_ssi_rpi_tcp_flblock) {
/*
 * Blocking case.
 */
    if (req->rq_rpi->cq_nenvout > 0) {
      LAM_TRACE(starttime = ttime());
      if (lam_ssi_rpi_sysv_writelock(ps))
	return (LAMERROR);
      LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

      ps->cp_outbox->pb_header.bh_env = req->rq_rpi->cq_env;

      if (req->rq_rpi->cq_nmsgout > 0) {
	len = LAM_min(req->rq_rpi->cq_nmsgout, size);
	lam_memcpy(buf, req->rq_rpi->cq_msgbuf, len);
	ps->cp_outbox->pb_header.bh_size = size;
	ps->cp_outbox->pb_header.bh_bufoff = req->rq_rpi->cq_bufoff;
	req->rq_rpi->cq_msgbuf += len;
	req->rq_rpi->cq_nmsgout -= len;
      }

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_nenvout = 0;
    }

    while (req->rq_rpi->cq_nmsgout > 0) {
      len = LAM_min(req->rq_rpi->cq_nmsgout, size);

      if (lam_ssi_rpi_sysv_writelock(ps))
	return (LAMERROR);

      lam_memcpy(buf, req->rq_rpi->cq_msgbuf, len);
      ps->cp_outbox->pb_header.bh_size = size;
      ps->cp_outbox->pb_header.bh_bufoff = req->rq_rpi->cq_bufoff;

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_msgbuf += len;
      req->rq_rpi->cq_nmsgout -= len;
    }
  } else {
/*
 * Non-blocking case.  Exit to try again another day as soon as we fail
 * to obtain the lock.
 */
    if (req->rq_rpi->cq_nenvout > 0) {
      if ((lock = lam_ssi_rpi_sysv_writetrylock(ps)) < 0) {
	return (LAMERROR);
      } else if (lock == 1) {
	return (0);
      }

      ps->cp_outbox->pb_header.bh_env = req->rq_rpi->cq_env;
      req->rq_rpi->cq_nenvout = 0;

      if (req->rq_rpi->cq_nmsgout > 0) {
	len = LAM_min(req->rq_rpi->cq_nmsgout, size);
	lam_memcpy(buf, req->rq_rpi->cq_msgbuf, len);
	ps->cp_outbox->pb_header.bh_size = size;
	ps->cp_outbox->pb_header.bh_bufoff = req->rq_rpi->cq_bufoff;
	req->rq_rpi->cq_msgbuf += len;
	req->rq_rpi->cq_nmsgout -= len;
      }

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);
    }

    while (req->rq_rpi->cq_nmsgout > 0) {
      if ((lock = lam_ssi_rpi_sysv_writetrylock(ps)) < 0) {
	return (LAMERROR);
      } else if (lock == 1) {
	return (0);
      }

      len = LAM_min(req->rq_rpi->cq_nmsgout, size);
      lam_memcpy(buf, req->rq_rpi->cq_msgbuf, len);
      ps->cp_outbox->pb_header.bh_size = size;
      ps->cp_outbox->pb_header.bh_bufoff = req->rq_rpi->cq_bufoff;

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      req->rq_rpi->cq_msgbuf += len;
      req->rq_rpi->cq_nmsgout -= len;
    }
  }

  return (1);
}

/*
 *	_shm_push_env
 *
 *	Function:	- push request envelope down the pike
 *	Accepts:	- process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_sysv_push_env(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int lock;
  double starttime = 0.0;

  if (lam_ssi_rpi_tcp_flblock) {
    LAM_TRACE(starttime = ttime());
    if (lam_ssi_rpi_sysv_writelock(ps))
      return (LAMERROR);
    LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));
  } else if ((lock = lam_ssi_rpi_sysv_writetrylock(ps)) < 0) {
    return (LAMERROR);
  } else if (lock == 1) {
    return (0);
  }
/*
 * Got the lock.
 */
  ps->cp_outbox->pb_header.bh_env = req->rq_rpi->cq_env;

  if (lam_ssi_rpi_sysv_writeunlock(ps))
    return (LAMERROR);
  return (1);
}


/*
 * The routines below implement the progress engine short circuit for
 * blocking send/receive requests when there are NO outstanding
 * requests.  Yes they are long, verbose and duplicate code found in
 * other functions but they are written this way for speed.
 */

/*
 *	_shm_fastsend
 *
 *	Function:	- fast message send
 *			- does not create requests and does not use
 *			  the normal advance mechanism
 *	Accepts:	- packed buffer
 *			- size of packed data
 *			- destination process
 *			- destination rank
 *			- message tag
 *			- message communicator
 *	Returns:	- MPI_SUCCESS
 */
int
lam_ssi_rpi_sysv_low_fastsend(char *packbuf, int packsize, 
			      struct lam_ssi_rpi_proc *ps,
			      int dest, int tag, MPI_Comm comm)
{
  char *buf;
  int len;
  int offset;			/* offset into shared pool */
  unsigned int size;
  double starttime = 0.0;

/*
 * Wait until outgoing postbox is free.
 */
  LAM_TRACE(starttime = ttime());
  if (lam_ssi_rpi_sysv_writelock(ps))
    return (LAMERROR);
  LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));
/*
 * Copy data and envelope into the outgoing postbox.
 */
  ps->cp_outbox->pb_header.bh_env.ce_len = packsize;
  ps->cp_outbox->pb_header.bh_env.ce_tag = tag;
  ps->cp_outbox->pb_header.bh_env.ce_rank = comm->c_group->g_myrank;
  ps->cp_outbox->pb_header.bh_env.ce_cid = comm->c_contextid;
  ps->cp_outbox->pb_header.bh_env.ce_seq = lam_seqnum++;

  if (packsize <= lam_ssi_rpi_sysv_short) {
/*
 * Short protocol.
 */
    ps->cp_outbox->pb_header.bh_env.ce_flags = 0;
    if (packsize > 0) {
      lam_memcpy(ps->cp_outbox + 1, packbuf, packsize);
    }

    if (lam_ssi_rpi_sysv_writeunlock(ps))
      return (LAMERROR);
  } else {
/*
 * Long protocol.  Send first packet.
 */
    ps->cp_outbox->pb_header.bh_env.ce_flags = C2CLONG;
    lam_memcpy(ps->cp_outbox + 1, packbuf, lam_ssi_rpi_sysv_short);

    if (lam_ssi_rpi_sysv_writeunlock(ps))
      return (LAMERROR);

    packbuf += lam_ssi_rpi_sysv_short;
/*
 * Wait for the acknowledgment.
 */
    if (waitforack(ps, dest, tag, comm, &packsize)) {
      return (LAMERROR);
    }

    packsize -= lam_ssi_rpi_sysv_short;
    if (packsize <= 0) {
      return (MPI_SUCCESS);
    }
/*
 * Send the message body first packet.
 */
    offset = 0;
    size = packsize;

    if (lam_ssi_rpi_sysv_writelock(ps))
      return (LAMERROR);

    if (packsize > lam_ssi_rpi_sysv_short && 
	lam_ssi_rpi_sysv_shmalloc(&size, &offset)) {
      ps->cp_outbox->pb_header.bh_env.ce_flags = C2C2ND;
      ps->cp_outbox->pb_header.bh_bufoff = offset;
      buf = lam_ssi_rpi_sysv_membase + offset;
    } else {
      ps->cp_outbox->pb_header.bh_env.ce_flags = C2CBOX | C2C2ND;
      buf = (char *) (ps->cp_outbox + 1);
      size = lam_ssi_rpi_sysv_short;
    }

    len = LAM_min(packsize, size);
    lam_memcpy(buf, packbuf, len);
    ps->cp_outbox->pb_header.bh_size = size;
    ps->cp_outbox->pb_header.bh_env.ce_len = packsize;

    if (lam_ssi_rpi_sysv_writeunlock(ps))
      return (LAMERROR);

    packbuf += len;
    packsize -= len;
/*
 * Continue sending packets until the whole message has been sent.
 */
    while (packsize > 0) {
      len = LAM_min(packsize, size);

      if (lam_ssi_rpi_sysv_writelock(ps))
	return (LAMERROR);

      lam_memcpy(buf, packbuf, len);

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);

      packbuf += len;
      packsize -= len;
    }
  }

  return (MPI_SUCCESS);
}

/*
 *	_shm_fastrecv
 *
 *	Function:	- fast message receive
 *			- does not create requests and does not use
 *			  the normal advance mechanism
 *	Accepts:	- buffer to receive packed data
 *			- size of buffer (inout)
 *			- source process
 *			- source rank
 *			- message tag (inout)
 *			- message communicator
 *			- message sequence number (out)
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_sysv_low_fastrecv(char *packbuf, int *packsize, 
			      struct lam_ssi_rpi_proc *ps, int src,
			      int *tag, MPI_Comm comm, int *seqnum)
{
  double starttime = 0.0;
  int len;
  int msglen;
  struct lam_ssi_rpi_envl env;
  struct lam_ssi_rpi_envl *inenv;	/* proc's incoming envelope */
  struct lam_ssi_rpi_cbuf_msg *bmsg;	/* buffered message */
  char *buf;
  int size;
  int synch;			/* synchronous send? */
  int err = MPI_SUCCESS;

  env.ce_flags = 0;
  env.ce_rank = src;
  env.ce_tag = *tag;
  env.ce_cid = comm->c_contextid;
/*
 * Check for match with buffered message.
 */
  if ((bmsg = lam_ssi_rpi_cbuf_find(&env))) {
    if (bmsg->cm_env.ce_len > *packsize) {
      err = lam_mkerr(MPI_ERR_TRUNCATE, 0);
    } else {
      *packsize = bmsg->cm_env.ce_len;
    }

    *tag = bmsg->cm_env.ce_tag;
    *seqnum = bmsg->cm_env.ce_seq;
    msglen = *packsize;

    len = LAM_min(msglen, lam_ssi_rpi_sysv_short);
    if (len) {
      lam_memcpy(packbuf, bmsg->cm_buf, len);
    }

    if (bmsg->cm_env.ce_flags & C2CLONG) {
      lam_ssi_rpi_cbuf_delete(bmsg);
      msglen -= len;
      packbuf += len;
      goto longmsg;
    } else if (bmsg->cm_env.ce_flags & C2CSSEND) {
      lam_ssi_rpi_cbuf_delete(bmsg);
      goto synchmsg;
    } else {
      lam_ssi_rpi_cbuf_delete(bmsg);
    }

    return (err);
  }

  inenv = &ps->cp_inbox->pb_header.bh_env;

  while (1) {
    LAM_TRACE(starttime = ttime());
    if (lam_ssi_rpi_sysv_readlock(ps))
      return (LAMERROR);
    LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

    if (lam_ssi_rpi_envl_cmp(inenv, &env))
      lam_ssi_rpi_sysv_buffer(ps);
    else      
      break;
  }
/*
 * Check for message length mismatch.
 */
  if (inenv->ce_len > *packsize)
    err = lam_mkerr(MPI_ERR_TRUNCATE, 0);
  else
    *packsize = inenv->ce_len;

  *tag = inenv->ce_tag;
  *seqnum = inenv->ce_seq;
  msglen = *packsize;

  if (inenv->ce_flags & C2CLONG) {
/*
 * Got a long protocol envelope. Copy data part into the receive buffer
 * and reply with an ack.  
 */
    len = LAM_min(msglen, lam_ssi_rpi_sysv_short);
    lam_memcpy(packbuf, ps->cp_inbox + 1, len);
/*
 * Done with data transfer, unlock the postbox.
 */
    if (lam_ssi_rpi_sysv_readunlock(ps))
      return (LAMERROR);

    msglen -= len;
    packbuf += len;
/*
 * Send an acknowledgment.
 */
  longmsg:
    LAM_TRACE(starttime = ttime());
    if (lam_ssi_rpi_sysv_writelock(ps))
      return (LAMERROR);
    LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

    ps->cp_outbox->pb_header.bh_env.ce_tag = *tag;
    ps->cp_outbox->pb_header.bh_env.ce_rank = comm->c_group->g_myrank;
    ps->cp_outbox->pb_header.bh_env.ce_cid = comm->c_contextid;
    ps->cp_outbox->pb_header.bh_env.ce_flags = C2CACK | C2CLONG;
    ps->cp_outbox->pb_header.bh_env.ce_len = *packsize;

    if (lam_ssi_rpi_sysv_writeunlock(ps))
      return (LAMERROR);

    if (msglen) {
/*
 * Receive the first packet of the message body.
 */
      env.ce_tag = *tag;
      env.ce_flags = C2C2ND;
      inenv = &ps->cp_inbox->pb_header.bh_env;

      while (1) {
	LAM_TRACE(starttime = ttime());
	if (lam_ssi_rpi_sysv_readlock(ps))
	  return (LAMERROR);
	LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

	if (lam_ssi_rpi_envl_cmp(inenv, &env))
	  lam_ssi_rpi_sysv_buffer(ps);
	else
	  break;
      }
/*
 * Determine the transfer method.
 */
      if (inenv->ce_flags & C2CBOX) {
	buf = (char *) (ps->cp_inbox + 1);
	size = lam_ssi_rpi_sysv_short;
      } else {
	buf = lam_ssi_rpi_sysv_membase + ps->cp_inbox->pb_header.bh_bufoff;
	size = ps->cp_inbox->pb_header.bh_size;
      }

      len = LAM_min(msglen, size);
      lam_memcpy(packbuf, buf, len);

      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);

      msglen -= len;
      packbuf += len;
/*
 * Receive the rest of the message body.
 */
      while (msglen > 0) {
	len = LAM_min(msglen, size);

	LAM_TRACE(starttime = ttime());
	if (lam_ssi_rpi_sysv_readlock(ps))
	  return (LAMERROR);
	LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

	lam_memcpy(packbuf, buf, len);

	if (lam_ssi_rpi_sysv_readunlock(ps))
	  return (LAMERROR);

	msglen -= len;
	packbuf += len;
      }

      if (buf != (char *) (ps->cp_inbox + 1))
	lam_ssi_rpi_sysv_shfree(buf);
    }
  } else {
/*
 * Got a short protocol envelope.  Check for a synchronous send and copy
 * the message from the postbox.  
 */
    synch = inenv->ce_flags & C2CSSEND;

    if (*packsize > 0) {
      lam_memcpy(packbuf, ps->cp_inbox + 1, *packsize);
    }

    if (lam_ssi_rpi_sysv_readunlock(ps))
      return (LAMERROR);

    if (synch) {
/*
 * Send a synchronous send acknowledgment.
 */
    synchmsg:
      LAM_TRACE(starttime = ttime());
      if (lam_ssi_rpi_sysv_writelock(ps))
	return (LAMERROR);
      LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

      ps->cp_outbox->pb_header.bh_env.ce_tag = *tag;
      ps->cp_outbox->pb_header.bh_env.ce_rank = comm->c_group->g_myrank;
      ps->cp_outbox->pb_header.bh_env.ce_cid = comm->c_contextid;
      ps->cp_outbox->pb_header.bh_env.ce_flags = C2CACK;

      if (lam_ssi_rpi_sysv_writeunlock(ps))
	return (LAMERROR);
    }
  }

  return (err);
}


/*
 *	waitforack
 *
 *	Function:	- wait for a long message acknowledgment
 *	Accepts:	- process sending the ack
 *			- rank of process sending the ack
 *			- ack tag
 *			- ack communicator
 *			- length ack'er will receive (out)
 * 	Returns:	- 0 or LAMERROR
 */
static int
waitforack(struct lam_ssi_rpi_proc *ps, int from, int tag, MPI_Comm comm, 
	   int *len)
{
  struct lam_ssi_rpi_envl env;
  double starttime = 0.0;

  env.ce_flags = C2CACK;
  env.ce_rank = from;
  env.ce_tag = tag;
  env.ce_cid = comm->c_contextid;

  while (1) {
    LAM_TRACE(starttime = ttime());
    if (lam_ssi_rpi_sysv_readlock(ps))
      return (LAMERROR);
    LAM_TRACE(_kio.ki_blktime += (ttime() - starttime));

    if (lam_ssi_rpi_envl_cmp(&ps->cp_inbox->pb_header.bh_env, &env)) {
      lam_ssi_rpi_sysv_buffer(ps);
    } else {
      *len = ps->cp_inbox->pb_header.bh_env.ce_len;
      if (lam_ssi_rpi_sysv_readunlock(ps))
	return (LAMERROR);
      break;
    }
  }

  return (0);
}


/*
 *	lam_ssi_rpi_sysv_fill_wildcards
 *
 *	Function:	- replace wildcards in request with matched values
 *			  and fill in the sequence number
 *	Accepts:	- request
 *			- matched envelope
 */
void
lam_ssi_rpi_sysv_fill_wildcards(MPI_Request req,
				struct lam_ssi_rpi_envl *env)
{
  struct _group *g;

  req->rq_seq = env->ce_seq;

  if (req->rq_rpi->cq_env.ce_tag == MPI_ANY_TAG)
    req->rq_rpi->cq_env.ce_tag = env->ce_tag;

  if (req->rq_rpi->cq_peer == MPI_ANY_SOURCE) {
    req->rq_rpi->cq_peer = env->ce_rank;
    req->rq_rpi->cq_env.ce_rank = env->ce_rank;

    g = (LAM_IS_INTER(req->rq_comm)) ?
      req->rq_comm->c_rgroup : req->rq_comm->c_group;

    req->rq_proc = g->g_procs[req->rq_rpi->cq_peer];
  }
}
