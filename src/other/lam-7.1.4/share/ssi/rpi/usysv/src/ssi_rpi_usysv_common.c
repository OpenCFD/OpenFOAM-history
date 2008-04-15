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
 * $Id: ssi_rpi_usysv_common.c,v 1.5 2003/07/11 19:48:19 jsquyres Exp $
 *
 *	Function:	- common shared memory transport low-level routines
 */

#include <lam_config.h>

#define SKIP_SHMEM_GLOBAL_INIT 1

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>

#ifdef PTHREAD_FREELOCK
#include <pthread.h>
#endif

#include <blktype.h>
#include <mpi.h>
#include <mpisys.h>
#include <rpisys.h>
#include <terror.h>
#include <typical.h>
#include <t_types.h>

#include <lam-ssi-rpi.h>
#include <rpi_usysv.h>
#include <usysv_low.h>
#include <usysv_common.h>

/*
 * We somewhat abuse the SSI RPI system here -- this RPI is tied to
 * the TCP RPI.  Hence, we know some of the variables and functions
 * that it exports.
 */
#include <tcp_low.h>


/*
 * external functions
 */
extern void lam_register_objects();


/*
 * private defines
 *
 * size taken up by postboxes and global lock 
 */
#define BOXTOTSIZE	(2*CACHELINESIZE + nshm*nshm*SHMBOXSIZE)


/*
 * global semaphores
 */
static struct sembuf shm_lock = { 2, -1, 0 };
static struct sembuf shm_unlock = { 2, 1, 0 };
static int semaphores;

#ifdef PTHREAD_FREELOCK
static pthread_mutex_t *freelock;
#endif

static union semun semctl_arg;

/*
 * free list data structures
 */
union header {
  struct {
    unsigned int ptr;		/* next block offset if on free list */
    unsigned int size;		/* size of this block */
  } s;
  char align[LAM_ALIGNMENT];	/* alignment */
};

typedef union header shm_header_t;


/*
 * private functions
 */
static int shm_req_send_body_box(struct lam_ssi_rpi_proc *, MPI_Request);
static int shm_req_send_body_pool(struct lam_ssi_rpi_proc *, MPI_Request);
static int shm_req_rcvd_2nd(struct lam_ssi_rpi_proc *, MPI_Request);
static int shm_req_done(struct lam_ssi_rpi_proc *, MPI_Request);

static void shminit(void *, unsigned int, int);
static void shmlock(void);
static void shmunlock(void);


/*
 * public variables
 */
char *lam_ssi_rpi_usysv_membase = 0;	/* base of shared memory */
struct lam_ssi_rpi_proc **lam_ssi_rpi_usysv_read = 0;	/* reading shm procs */
struct lam_ssi_rpi_proc **lam_ssi_rpi_usysv_write = 0;	/* writing shm procs */
int lam_ssi_rpi_usysv_nread;		/* # reading shm processes */
int lam_ssi_rpi_usysv_nwrite;		/* # writing shm processes */
int lam_ssi_rpi_usysv_nprocs = 0;	/* number of shm processes */
int lam_ssi_rpi_usysv_narray = 0;	/* shm read/write array sizes */
int lam_ssi_rpi_usysv_poll_yield = 1;	/* yield in poll loop */
int lam_ssi_rpi_usysv_poolsize = LAM_SHMPOOLSIZE; /* size of long msg pool */
int lam_ssi_rpi_usysv_maxalloc = LAM_SHMMAXALLOC; /* max allocation of shmem */
int lam_ssi_rpi_usysv_short = LAM_SHMSHORTMSGLEN; /* short message len */

int lam_ssi_rpi_usysv_param_poll_yield;
int lam_ssi_rpi_usysv_param_poolsize;
int lam_ssi_rpi_usysv_param_maxalloc;
int lam_ssi_rpi_usysv_param_short;


/*
 * private variables
 */
static unsigned int *freelist;
static shm_header_t *poolbase;


/*
 *	lam_ssi_rpi_usysv_advance
 *
 *	Function:	- advance tcp and shm requests where possible
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_advance_common(void)
{
  MPI_Request req;
  double blkstart = 0.0;
  double loopstart = 0.0;
  int blksave;			/* save blocking state */
  int i;
/*
 * Save and set blocking state.
 */
  blksave = lam_ssi_rpi_tcp_flblock;
  lam_ssi_rpi_tcp_flblock &= ((lam_ssi_rpi_usysv_nread +
			   lam_ssi_rpi_usysv_nwrite == 0) ||
			  (lam_ssi_rpi_usysv_nread +
			   lam_ssi_rpi_usysv_nwrite +
			   lam_ssi_rpi_tcp_nio == 1));

  if ((_kio.ki_rtf & RTF_TRON) == RTF_TRON && blksave
      && !lam_ssi_rpi_tcp_flblock)
    loopstart = blkstart = ttime();
/*
 * Advance reading shared memory processes.
 */
  do {
    for (i = 0; i < lam_ssi_rpi_usysv_nread; i++) {
      lam_ssi_rpi_usysv_read[i]->cp_read = 0;
      if (lam_ssi_rpi_usysv_read[i]->cp_readfn(lam_ssi_rpi_usysv_read[i]))
	return (LAMERROR);
    }
/*
 * Advance writing shared memory processes.
 */
    for (i = 0; i < lam_ssi_rpi_usysv_nwrite; i++) {
      lam_ssi_rpi_usysv_write[i]->cp_write = 0;
      req = lam_ssi_rpi_usysv_write[i]->cp_wreq;
      if (req->rq_rpi->cq_adv(lam_ssi_rpi_usysv_write[i], req))
	return (LAMERROR);
    }
/*
 * Advance the TCP requests (if any).  In the case of a single TCP
 * request make sure that the request has not been completed and is
 * still A TCP request.  This can not be true when a receive from
 * MPI_ANY_SOURCE is completed by a send from a shared memory source.
 */
    if (lam_ssi_rpi_tcp_nio == 1
	&& lam_ssi_rpi_tcp_lastreq->rq_state != LAM_RQSDONE
	&& (lam_ssi_rpi_tcp_lastreq->rq_proc == 0
	    || lam_ssi_rpi_tcp_lastreq->rq_proc->p_rpi->cp_sock >= 0)) {
      if (lam_ssi_rpi_tcp_adv1())
	return (LAMERROR);
    } else if (lam_ssi_rpi_tcp_nio > 1) {
      if (lam_ssi_rpi_tcp_advmultiple())
	return (LAMERROR);
    }

    if (blksave && !lam_ssi_rpi_tcp_haveadv) {
      if (lam_ssi_rpi_usysv_poll_yield)
	lam_yield();
      LAM_TRACE(loopstart = ttime());
    }
  } while (blksave && !lam_ssi_rpi_tcp_haveadv);

  if ((_kio.ki_rtf & RTF_TRON) == RTF_TRON && blksave
      && !lam_ssi_rpi_tcp_flblock)
    _kio.ki_blktime += (loopstart - blkstart);

  lam_ssi_rpi_tcp_flblock = blksave;
  return (lam_ssi_rpi_tcp_haveadv);
}

/*
 *	_shmtcp_req_probe
 *
 *	Function:	- mark probe request as done
 *			- the matched incoming envelope/message is buffered
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_shmtcp_req_probe(struct lam_ssi_rpi_proc *ps,
				   MPI_Request req)
{
  struct lam_ssi_rpi_envl *env;

  if (ps->cp_sock >= 0) {
/*
 * TCP protocol takes care of it.
 */
    return (lam_ssi_rpi_tcp_req_probe(ps, req));
  }

  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  env = (struct lam_ssi_rpi_envl *) ps->cp_inbox;
  lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);

  return (lam_ssi_rpi_usysv_buffer(ps));
}


/*
 *	lam_ssi_rpi_usysv_buffered_adv
 *
 *	Function:	- determine protocol for receive request matching
 *			  a buffered envelope/message and act upon it
 *	Accepts:	- request
 *			- buffered envelope/message
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_buffered_adv(MPI_Request req,
			       struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_envl *env;	/* matching incoming env. */

  env = &msg->cm_env;

  if (req->rq_type == LAM_RQIPROBE) {
/*
 * The request is a probe.  Set the status and leave the envelope buffered.
 */
    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag,
				env->ce_len);
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;

    return (0);
  }

  if (env->ce_len > req->rq_packsize) {
    req->rq_flags |= LAM_RQFTRUNC;
    env->ce_len = req->rq_packsize;
  }
  lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, env->ce_len);

  if (env->ce_flags & C2CLONG) {
/*
 * Matched a long protocol envelope.  Copy the partially buffered body and
 * reply with an ack.
 */
    req->rq_state = LAM_RQSACTIVE;
    req->rq_rpi->cq_state = C2CWRITE;
    req->rq_rpi->cq_env.ce_flags |= (C2CACK | C2CLONG);

    if (env->ce_len > lam_ssi_rpi_usysv_short) {
      lam_memcpy(req->rq_packbuf, msg->cm_buf, 
			      lam_ssi_rpi_usysv_short);
      req->rq_rpi->cq_env.ce_len = env->ce_len;
      req->rq_rpi->cq_adv = lam_ssi_rpi_usysv_req_send_ack_long;
    } else {
      if (env->ce_len > 0)
	lam_memcpy(req->rq_packbuf, msg->cm_buf, env->ce_len);

      req->rq_rpi->cq_adv = lam_ssi_rpi_usysv_req_send_ack_only;
      req->rq_rpi->cq_env.ce_len = 0;
    }

    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;
    req->rq_rpi->cq_nenvout = sizeof(struct lam_ssi_rpi_envl);
  } else {
/*
 * Matched a short protocol envelope.  Copy the message (if any) from
 * the buffer and advance the request.  
 */
    if (env->ce_len)
      lam_memcpy(req->rq_packbuf, msg->cm_buf, env->ce_len);
    if (env->ce_flags & C2CSSEND) {
      req->rq_state = LAM_RQSACTIVE;
      if (lam_ssi_rpi_usysv_req_rcvd_body_synch(0, req))
	return (LAMERROR);
    } else {
      req->rq_state = LAM_RQSDONE;
      lam_rq_nactv--;
    }
  }
/*
 * Discard the buffered message.
 */
  lam_ssi_rpi_cbuf_delete(msg);

  return (0);
}

/*
 *	shm_match_adv
 *
 *	Function:	- match env read from process with a read request
 *			  and advance the matched request
 *			- if no match is found then the env/msg is buffered
 *	Accepts:	- envelope's source process
 */
int
lam_ssi_rpi_usysv_match_adv(struct lam_ssi_rpi_proc *ps)
{
  MPI_Request req;		/* request */
  struct lam_ssi_rpi_envl *env;	/* envelope */
/*
 * There cannot be any matching recvs after a matching probe because
 * probes are blocking.	 Thus we may return upon the first match
 * (buffering the envelope in the case of a probe) and maintain the
 * invariant "no requests in the list match buffered envelopes".  This
 * means once a request is in the list after being checked against
 * buffered envelopes it need never again be checked against any
 * buffered envelopes.
 */
  env = (struct lam_ssi_rpi_envl *) ps->cp_inbox;

  for (req = ps->cp_mreq; req; req = req->rq_next)
    if ((req->rq_state != LAM_RQSDONE)
	&& (req->rq_rpi->cq_state == C2CREAD)
	&& (!lam_ssi_rpi_envl_cmp(env, &req->rq_rpi->cq_env)))
      return (req->rq_rpi->cq_adv(ps, req));

  return (lam_ssi_rpi_usysv_buffer(ps));
}

/*
 *	lam_ssi_rpi_usysv_req_send_ack_long
 *
 *	Function:	- send ack and prepare to receive message body
 *			- long protocol
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_send_ack_long(struct lam_ssi_rpi_proc *ps,
				    MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_env(ps, req)) <= 0)
    return (done);
/*
 * The acknowledgment has been sent.
 */
  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
/*
 * Prepare to receive the message body.
 */
  req->rq_rpi->cq_state = C2CREAD;
  req->rq_rpi->cq_env.ce_flags &= ~C2CACK;
  req->rq_rpi->cq_env.ce_flags |= C2C2ND;
  req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
  req->rq_rpi->cq_adv = shm_req_rcvd_2nd;

  return (0);
}

/*
 *	lam_ssi_rpi_usysv_req_send_ack_only
 *
 *	Function:	- send an acknowledgment
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_send_ack_only(struct lam_ssi_rpi_proc *ps,
				    MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_env(ps, req)) <= 0)
    return (done);
/*
 * The acknowledgment has been sent.
 */
  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}

/*
 *	lam_ssi_rpi_usysv_req_rcvd_body_synch
 *
 *	Function:	- synchronous protocol transition from reading
 *			  message body to sending ack
 *	Accepts:	- source process (ignored)
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_rcvd_body_synch(struct lam_ssi_rpi_proc *ps,
				      MPI_Request req)
{
  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_rpi->cq_state = C2CWRITE;
  req->rq_rpi->cq_env.ce_flags |= C2CACK;
  req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;
  req->rq_rpi->cq_adv = lam_ssi_rpi_usysv_req_send_ack_only;

  return (0);
}

/*
 *	lam_ssi_rpi_usysv_req_send_body_first
 *
 *	Function:	- first send of long message body
 *			- we decide here whether to use the postbox or
 *			  shared pool
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_send_body_first(struct lam_ssi_rpi_proc *ps,
				      MPI_Request req)
{
  int done;
  unsigned int nbytes;
  int bufoffset;
/*
 * We use the postbox if the number of message bytes left to send is
 * smaller than or equal to the postbox size.  We may also have to use
 * it if we cannot get a piece of the shared pool.  
 */
  nbytes = req->rq_rpi->cq_env.ce_len;
  if (nbytes > lam_ssi_rpi_usysv_short &&
      lam_ssi_rpi_usysv_shmalloc(&nbytes, &bufoffset)) {
    req->rq_rpi->cq_bufoff = bufoffset;
    req->rq_rpi->cq_bufsize = nbytes;
    req->rq_rpi->cq_adv = shm_req_send_body_pool;

    if ((done = lam_ssi_rpi_usysv_push_body_pool(ps, req)) <= 0)
      return (done);
  } else {
    req->rq_rpi->cq_env.ce_flags |= C2CBOX;
    req->rq_rpi->cq_adv = shm_req_send_body_box;

    if ((done = lam_ssi_rpi_usysv_push_body_box(ps, req)) <= 0)
      return (done);
  }
/*
 * All of message has been written.
 */
  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}


/*
 *	shm_req_send_body_box
 *
 *	Function:	- send message body via the postbox
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
shm_req_send_body_box(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_body_box(ps, req)) <= 0) {
    return (done);
  }
/*
 * All of the message has been written.
 */
  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;
  return (0);
}

/*
 *	shm_req_send_body_pool
 *
 *	Function:	- send message body via the shared pool
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
shm_req_send_body_pool(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_body_pool(ps, req)) <= 0) {
    return (done);
  }
/*
 * All of the message has been written.
 */
  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}

/*
 *	lam_ssi_rpi_usysv_req_send_long
 *
 *	Function:	- send the first envelope of a long message and
 *			  prepare to receive the ack
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_send_long(struct lam_ssi_rpi_proc *ps,
				MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_body_box(ps, req)) <= 0)
    return (done);
/*
 * Prepare to read long protocol ack.
 */
  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_rpi->cq_state = C2CREAD;
  req->rq_rpi->cq_env.ce_flags |= C2CACK;
  req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
  req->rq_rpi->cq_adv = lam_ssi_rpi_usysv_req_rcvd_long_ack;

  return (0);
}

/*
 *	lam_ssi_rpi_usysv_req_send_short
 *
 *	Function:	- send a short protocol message
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_send_short(struct lam_ssi_rpi_proc *ps,
				 MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_body_box(ps, req)) <= 0)
    return (done);

  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}


/*
 *	lam_ssi_rpi_usysv_req_send_synch
 *
 *	Function:	- send short synchronous protocol message body
 *			  and prepare to read the ack
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_req_send_synch(struct lam_ssi_rpi_proc *ps,
				 MPI_Request req)
{
  int done;

  if ((done = lam_ssi_rpi_usysv_push_body_box(ps, req)) <= 0)
    return (done);

  lam_ssi_rpi_tcp_haveadv = 1;
  ps->cp_wreq = 0;
  req->rq_rpi->cq_state = C2CREAD;
  req->rq_rpi->cq_env.ce_flags |= C2CACK;
  req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
  req->rq_rpi->cq_adv = lam_ssi_rpi_usysv_req_done_synch;

  return (0);
}

/*
 *	shm_req_rcvd_2nd
 *
 *	Function:	- read the body of a long protocol message
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
shm_req_rcvd_2nd(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  ps->cp_rreq = req;
  ps->cp_msgbuf = req->rq_packbuf + lam_ssi_rpi_usysv_short;
  ps->cp_nmsgin = ps->cp_inbox->pb_header.bh_env.ce_len;
  req->rq_rpi->cq_adv = shm_req_done;
/*
 * We may be being sent to via the postbox or the global pool.
 */
  if (ps->cp_inbox->pb_header.bh_env.ce_flags & C2CBOX) {
    ps->cp_readfn = lam_ssi_rpi_usysv_proc_read_body_box;
    return (lam_ssi_rpi_usysv_proc_read_body_box(ps));
  } else {
    ps->cp_readfn = lam_ssi_rpi_usysv_proc_read_body_pool;
    ps->cp_insize = ps->cp_inbox->pb_header.bh_size;
    return (lam_ssi_rpi_usysv_proc_read_body_pool(ps));
  }
}

/*
 *	shm_req_done
 *
 *	Function:	- mark request as done
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
shm_req_done(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}

/*
 *	shminit
 *
 *	Function:	- initialize shared pool
 *	Accepts:	- base of pool
 *			- size of pool (bytes)
 *			- first to initialize?
 */
static void
shminit(void *memory, unsigned int nbytes, int first)
{
  int nunits;

  nunits = nbytes >> LAM_LOG_ALIGN;

  freelist = memory;
  poolbase = memory;

  *freelist = 1;

  if (first) {
    (poolbase + 1)->s.ptr = 1;
    (poolbase + 1)->s.size = nunits - 1;
  }
}


/*
 *	lam_ssi_rpi_usysv_shmalloc
 *
 *	Function:	- allocate memory from shared area
 *	Accepts:	- number of bytes required/obtained (inout)
 *			- offset of allocated memory (out)
 *	Returns:	- 1 => allocated some or all required
 *			  0 => nothing allocated
 */
int
lam_ssi_rpi_usysv_shmalloc(unsigned int *nbytes, int *offset)
{
  shm_header_t *p;
  shm_header_t *prev;
  unsigned int nunits;

  shmlock();

  if (*freelist == 0) {
    shmunlock();
    return (0);
  }
/*
 * Round allocation request down to maximum allowed.
 */
  nunits = (*nbytes + sizeof(shm_header_t) - 1) >> LAM_LOG_ALIGN;

  if (nunits > lam_ssi_rpi_usysv_maxalloc) {
    nunits = lam_ssi_rpi_usysv_maxalloc;
    *nbytes = lam_ssi_rpi_usysv_maxalloc << LAM_LOG_ALIGN;
  }
/*
 * One extra unit for the header.
 */
  nunits++;
/*
 * Look for large enough free block.
 */
  prev = poolbase + *freelist;
  for (p = poolbase + prev->s.ptr;; prev = p, p = poolbase + p->s.ptr) {
    if (p->s.size >= nunits) {
      if (p->s.size == nunits) {
	if (p == poolbase + p->s.ptr) {
	  prev = poolbase;
	} else {
	  prev->s.ptr = p->s.ptr;
	}
      } else {
	p->s.size -= nunits;
	p += p->s.size;
	p->s.size = nunits;
      }

      *freelist = prev - poolbase;
      *offset = (char *) (p + 1) - lam_ssi_rpi_usysv_membase;
      break;
    }
/*
 * Wrapped around the free list.  No fit found.
 */
    if (p == poolbase + *freelist) {
      shmunlock();
      return (0);
    }
  }

  shmunlock();
  return (1);
}


/*
 *	lam_ssi_rpi_usysv_shfree
 *
 *	Function:	- free up a block of the shared area
 *	Accepts:	- address of data part of block
 */
void
lam_ssi_rpi_usysv_shfree(void *block)
{
  shm_header_t *head;		/* block's header */
  shm_header_t *p;

  if (block == 0)
    return;

  shmlock();

  head = (shm_header_t *) block - 1;

  if (*freelist) {
/*
 * There are already free region(s) in the shared memory region.
 */
    for (p = poolbase + *freelist;
	 !(head > p && head < poolbase + p->s.ptr);
	 p = poolbase + p->s.ptr) {
      if (p >= poolbase + p->s.ptr
	  && (head > p || head < poolbase + p->s.ptr))
/*
 * Freed block at start of end of arena.
 */
	break;
    }
/*
 * Integrate block in list.
 */
    *freelist = p - poolbase;

    if (head + head->s.size == poolbase + p->s.ptr) {
/*
 * Join to upper neighbour.
 */
      if (p->s.ptr == *freelist) {
	*freelist = head - poolbase;
      }

      if (poolbase + p->s.ptr == p) {
	head->s.ptr = head - poolbase;
      } else {
	head->s.ptr = (poolbase + p->s.ptr)->s.ptr;
      }

      head->s.size += (poolbase + p->s.ptr)->s.size;
    } else {
      head->s.ptr = p->s.ptr;
    }

    if (p + p->s.size == head) {
/*
 * Join to lower neighbour.
 */
      p->s.size += head->s.size;
      p->s.ptr = head->s.ptr;
    } else
      p->s.ptr = head - poolbase;
  } else {
/*
 * There wasn't a free shared memory region before.
 */
    head->s.ptr = head - poolbase;
    *freelist = head - poolbase;
  }

  shmunlock();
}

/*
 *	lam_ssi_rpi_usysv_create_area
 *
 *	Function:	- create shared memory area
 *	Accepts:	- number of processes sharing area
 *			- process (in-out)
 *			- LAM msg to fill with shared info (out)
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_create_area(int nshm, struct lam_ssi_rpi_proc *ps,
			      struct nmsg *msg)
{
  char objs[2][32];
  int shmid;
  int semid;
  int shmsize;

#ifdef PTHREAD_FREELOCK
  pthread_mutexattr_t mattr;
#endif

/*
 * Create the shared memory segment.
 */
  shmsize = BOXTOTSIZE + lam_ssi_rpi_usysv_poolsize;

  shmid = shmget(IPC_PRIVATE, shmsize, 0600 | IPC_CREAT);
  if (shmid < 0) {
    errno = ESHMCREATE;
    return (LAMERROR);
  }

  lam_ssi_rpi_usysv_membase = (char *) shmat(shmid, (char *) 0, 0);
  if (lam_ssi_rpi_usysv_membase == (char *) -1) {
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    return (LAMERROR);
  }

  ps->cp_shm = shmid;

  memset(lam_ssi_rpi_usysv_membase, 0, BOXTOTSIZE);

#ifdef PTHREAD_FREELOCK
  assert(2 * CACHELINESIZE >= sizeof(pthread_mutex_t));
  freelock = (pthread_mutex_t *) lam_ssi_rpi_usysv_membase;
  pthread_mutexattr_init(&mattr);
  pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(freelock, &mattr);
#endif

  shminit(lam_ssi_rpi_usysv_membase + BOXTOTSIZE,
	  lam_ssi_rpi_usysv_poolsize, TRUE);
/*
 * Create the semaphores.
 */
  if ((semid = semget(IPC_PRIVATE, 3, 0600 | IPC_CREAT)) < 0) {
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    errno = ESEMCREATE;
    return (LAMERROR);
  }
/*
 * Register id's for cleanup.
 */
  sprintf(objs[0], "%d", shmid);
  sprintf(objs[1], "%d", semid);
  lam_register_objects(2, 'm', objs[0], 's', objs[1]);
/*
 * Initilize semaphore.
 */
  ps->cp_sem = semid;
  semaphores = semid;

  semctl_arg.val = 1;
  if (semctl(semid, 0, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
  if (semctl(semid, 2, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
  semctl_arg.val = nshm;
  if (semctl(semid, 1, SETVAL, LAM_SEMUN_VAL(semctl_arg)) < 0)
    return (LAMERROR);
/*
 * Set information to pass to other processes.
 */
  msg->nh_data[1] = (int4) shmid;
  msg->nh_data[2] = (int4) semid;

  return (0);
}


/*
 *	lam_ssi_rpi_usysv_attach_area
 *
 *	Function:	- attach to shared memory area
 *	Accepts:	- number of processes sharing area
 *			- process (in-out)
 *			- LAM msg containing shared info
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_usysv_attach_area(int nshm, struct lam_ssi_rpi_proc *ps,
			      struct nmsg *msg)
{
  ps->cp_shm = (int) msg->nh_data[1];
  ps->cp_sem = (int) msg->nh_data[2];
  semaphores = ps->cp_sem;
/*
 * Attach the shared memory segment and mark it for removal.  It will be
 * automatically removed once all processes exit.
 */
  lam_ssi_rpi_usysv_membase = (char *) shmat(ps->cp_shm, (char *) 0, 0);
  if (lam_ssi_rpi_usysv_membase == (char *) -1) {
    shmctl(ps->cp_shm, IPC_RMID, (struct shmid_ds *) 0);
    semctl_arg.val = 0;
    semctl(ps->cp_sem, 0, IPC_RMID, semctl_arg);
    return (LAMERROR);
  }

#ifdef PTHREAD_FREELOCK
  freelock = (pthread_mutex_t *) lam_ssi_rpi_usysv_membase;
#endif

  shminit(lam_ssi_rpi_usysv_membase + BOXTOTSIZE,
	  lam_ssi_rpi_usysv_poolsize, FALSE);

  return (0);
}


#ifdef PTHREAD_FREELOCK
/*
 *	shmlock
 *
 *	Function:	- lock the shared memory area
 */
static void
shmlock(void)
{
  pthread_mutex_lock(freelock);
}


/*
 *	shmunlock
 *
 *	Function:	- unlock the shared memory area
 */
static void
shmunlock(void)
{
  pthread_mutex_unlock(freelock);
}

#else /* use USYSV semaphore */

/*
 *	shmlock
 *
 *	Function:	- lock the shared memory area
 */
static void
shmlock(void)
{
  do {
    if (semop(semaphores, &shm_lock, 1) == 0) {
      return;
    } else if (errno != EINTR) {
      lam_err_comm(MPI_COMM_NULL, MPI_ERR_OTHER, errno,
		   "locking shared memory area");
    }
  } while (1);
}


/*
 *	shmunlock
 *
 *	Function:	- unlock the shared memory area
 */
static void
shmunlock(void)
{
  semop(semaphores, &shm_unlock, 1);
}
#endif
