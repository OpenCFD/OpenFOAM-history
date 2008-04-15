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
 * $Id: ssi_rpi_tcp_low.c,v 1.13 2003/10/11 19:15:38 brbarret Exp $
 *
 *	Function:	- TCP low-level routines
 */

#include <lam_config.h>

#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined(HAVE_NETINET_TCP_H) && HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#include <sys/uio.h>
#include <unistd.h>

#if LAM_HAVE_FIONBIO
#include <sys/ioctl.h>
#endif

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
#include <lamdebug.h>
#include <all_array.h>

#include <tcp_low.h>
#include <rpi_tcp.h>

/*
 * external functions
 */
extern void lam_commfault();

/*
 * private functions
 */
static int tcp_proc_read_body(struct lam_ssi_rpi_proc *ps);
static int tcp_proc_read_buffer(struct lam_ssi_rpi_proc *ps);
static int tcp_proc_read_extra(struct lam_ssi_rpi_proc *ps);
static int tcp_req_send_ack_long(struct lam_ssi_rpi_proc *ps,
				 MPI_Request req);
static int tcp_req_send_ack_only(struct lam_ssi_rpi_proc *ps,
				 MPI_Request req);
static int tcp_req_send_body(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int tcp_req_rcvd_2nd(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int tcp_req_rcvd_body_synch(struct lam_ssi_rpi_proc *ps,
				   MPI_Request req);
static int tcp_req_rcvd_ack_long(struct lam_ssi_rpi_proc *ps,
				 MPI_Request req);
static int tcp_req_done(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int tcp_match_adv(struct lam_ssi_rpi_proc *ps);
static int tcp_buffer(struct lam_ssi_rpi_proc *ps);
static int tcp_push_body(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int tcp_push_env(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int sread(int sock, char *buf, int nbytes);
static int swrite(int p, char *buf, int nbytes);
static int swritev(int sock, struct iovec *iov, int iovcnt);
static int sselect(int width, fd_set * readfds, fd_set * writefds,
		   fd_set * exceptfds, struct timeval *timeout);
static int setsockblk(int sock, int fl_block);
static void comminvalidate(int sock);
static int badsockerr(MPI_Comm comm);
static int sinksuck(int sock, int len);
static int waitforack(struct lam_ssi_rpi_proc *ps, int from, int tag,
		      MPI_Comm comm, int *len);

/*
 * public variables
 */
int lam_ssi_rpi_tcp_flblock;
int lam_ssi_rpi_tcp_haveadv;
int lam_ssi_rpi_tcp_nio;		/* # processes doing tcp io */
int lam_ssi_rpi_tcp_sockmax;		/* max. tcp io socket num. */
fd_set lam_ssi_rpi_tcp_read;		/* read sockets */
fd_set lam_ssi_rpi_tcp_write;		/* write sockets */
fd_set lam_ssi_rpi_tcp_except;		/* exception sockets */
fd_set lam_ssi_rpi_tcp_block;		/* blocked mode socket? */
fd_set lam_ssi_rpi_tcp_eoferr;		/* eof on socket is error? */
MPI_Request lam_ssi_rpi_tcp_lastreq;	/* last tcp request */
struct lam_ssi_rpi_proc *lam_ssi_rpi_tcp_smap[FD_SETSIZE];	/* maps socket fd to process */
int lam_ssi_rpi_tcp_short;              /* short message length */
lam_array_t *lam_ssi_rpi_tcp_procs;


/*
 * private variables
 */
static struct timeval zerotime = { 0, 0 };	/* zero timeval */
#define WANT_SEND_MESSAGES 0
#define WANT_RECV_MESSAGES 0

#if WANT_SEND_MESSAGES || WANT_RECV_MESSAGES
static int fl_debug = 0;
#endif


/*
 *	_tcp_advmultiple
 *
 *	Function:	- advance multiple tcp processes
 *	Accepts:	- request list
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_advmultiple(void)
{
  MPI_Request req;
  int sock;			/* socket descriptor */
  int nready;			/* # ready sockets */
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;

  memcpy((char *) &readfds, (char *) &lam_ssi_rpi_tcp_read, sizeof(fd_set));
  memcpy((char *) &writefds, (char *) &lam_ssi_rpi_tcp_write, sizeof(fd_set));
  memcpy((char *) &exceptfds, (char *) &lam_ssi_rpi_tcp_except, 
	 sizeof(fd_set));

  if (lam_ssi_rpi_tcp_flblock) {
    nready = sselect(lam_ssi_rpi_tcp_sockmax + 1, &readfds, &writefds,
		     &exceptfds, (struct timeval *) 0);
    if (nready <= 0)
      return (LAMERROR);
  } else {
    nready = sselect(lam_ssi_rpi_tcp_sockmax + 1, &readfds,
		     &writefds, &exceptfds, &zerotime);
    if (nready < 0)
      return (0);
  }
  /*
   * Loop through enabled sockets and advance on each one.
   */
  for (sock = 0; nready && sock <= lam_ssi_rpi_tcp_sockmax; sock++) {
    if (FD_ISSET(sock, &exceptfds)) {
      /*
       * Oops! An exception on the socket. Remove it from further
       * consideration.
       */
      --nready;
      comminvalidate(sock);

      if (FD_ISSET(sock, &readfds)) {
	--nready;
      }
      if (FD_ISSET(sock, &writefds)) {
	--nready;
      }

      continue;
    }
    if (FD_ISSET(sock, &readfds)) {
      --nready;
      if (setsockblk(sock, FALSE))
	return (LAMERROR);

      if (lam_ssi_rpi_tcp_smap[sock]->cp_readfn(lam_ssi_rpi_tcp_smap[sock]))
	return (LAMERROR);
    }

    if (FD_ISSET(sock, &writefds)) {
      --nready;
      if (setsockblk(sock, FALSE))
	return (LAMERROR);

      req = lam_ssi_rpi_tcp_smap[sock]->cp_wreq;
      if (req->rq_rpi->cq_adv(lam_ssi_rpi_tcp_smap[sock], req))
	return (LAMERROR);
    }
  }

  return (0);
}


/*
 *	_tcp_adv1
 *
 *	Function:	- advance single tcp process optimally
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_adv1(void)
{
  MPI_Request req;

  if (lam_ssi_rpi_comm_dead_m(lam_ssi_rpi_tcp_lastreq)) {
    lam_ssi_rpi_tcp_haveadv = 1;
    return 0;
  }

  if (setsockblk(lam_ssi_rpi_tcp_sockmax, lam_ssi_rpi_tcp_flblock))
    return (LAMERROR);

  if (lam_ssi_rpi_tcp_lastreq->rq_rpi->cq_state == C2CREAD) {
    /*
     * In blocking mode EOF is considered to be an error.
     */
    if (lam_ssi_rpi_tcp_flblock)
      FD_SET(lam_ssi_rpi_tcp_sockmax, &lam_ssi_rpi_tcp_eoferr);

    return (lam_ssi_rpi_tcp_smap[lam_ssi_rpi_tcp_sockmax]->cp_readfn(lam_ssi_rpi_tcp_smap[lam_ssi_rpi_tcp_sockmax]));
  } else {
    req = lam_ssi_rpi_tcp_smap[lam_ssi_rpi_tcp_sockmax]->cp_wreq;
    return (req->rq_rpi->cq_adv(lam_ssi_rpi_tcp_smap[lam_ssi_rpi_tcp_sockmax], 
				req));
  }
}


/*
 *	_tcp_proc_read_env
 *
 *	Function:	- read envelope from process
 *			- if full envelope read in then try to match with
 *			  and advance a receiving request
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_proc_read_env(struct lam_ssi_rpi_proc *ps)
{
  int nread;

  if ((nread = sread(ps->cp_sock, ps->cp_envbuf, ps->cp_nenvin)) <= 0)
    return (nread);

#if WANT_RECV_MESSAGES
  {
    struct lam_ssi_rpi_envl *foo = (struct lam_ssi_rpi_envl *) &ps->rp_net.cp_env;
    DBUG
      ("LAM C2C progression engine got env (pid %d): len %d tag %d rank %d cid %d\n",
       lam_getpid(), foo->ce_len, foo->ce_tag, foo->ce_rank, foo->ce_cid);
  }
#endif

  ps->cp_nenvin -= nread;
  if (ps->cp_nenvin == 0) {
    /*
     * Read complete envelope.  Reset to be ready for read of next envelope.
     */
    ps->cp_envbuf = (char *) &ps->cp_env;
    ps->cp_nenvin = sizeof(struct lam_ssi_rpi_envl);
    if (!lam_homog) {
      mttoli4((int4 *) & ps->cp_env,
	      sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));
    }
    return (tcp_match_adv(ps));
  } else {
    ps->cp_envbuf += nread;
    return (0);
  }
}


/*
 *	tcp_proc_read_body
 *
 *	Function:	- read the body of an MPI message from a process
 *			- this is only called when there is a receiving request
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_proc_read_body(struct lam_ssi_rpi_proc *ps)
{
  int nread;

  if ((nread = sread(ps->cp_sock, ps->cp_msgbuf, ps->cp_nmsgin)) <= 0)
    return (nread);

  ps->cp_nmsgin -= nread;
  if (ps->cp_nmsgin == 0) {
    /*
     * All of the message (not including truncated data) has been
     * read. Advance the request receiving the message.  
     */
    if (ps->cp_rreq->rq_rpi->cq_adv(ps, ps->cp_rreq))
      return (LAMERROR);

    /*
     * If there is extra truncated data to read, read it otherwise set
     * process up to read the next incoming envelope.
     */
    ps->cp_rreq = 0;
    if (ps->cp_extra > 0) {
      ps->cp_readfn = tcp_proc_read_extra;
      return (tcp_proc_read_extra(ps));
    } else
      ps->cp_readfn = lam_ssi_rpi_tcp_proc_read_env;
  } else {
    /*
     * There is still more message data to be read.
     */
    ps->cp_msgbuf += nread;
  }

  return (0);
}


/*
 *	tcp_proc_read_buffer
 *
 *	Function:	- read the body of an MPI message from a process
 *			- this is called when we are buffering the message
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_proc_read_buffer(struct lam_ssi_rpi_proc *ps)
{
  int nread;

  if ((nread = sread(ps->cp_sock, ps->cp_msgbuf, ps->cp_nmsgin)) <= 0)
    return (nread);

  ps->cp_nmsgin -= nread;

  if (ps->cp_nmsgin == 0) {
    /*
     * All of the message has been buffered. Set the process up to read the
     * next incoming envelope.  
     */
    ps->cp_readfn = lam_ssi_rpi_tcp_proc_read_env;
    ps->cp_bmsg->cm_proc = 0;
  } else {
    ps->cp_msgbuf += nread;
  }

  return (0);
}


/*
 *	tcp_proc_read_extra
 *
 *	Function:	- read truncated data from a process into a sink
 *	Accepts:	- process
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_proc_read_extra(struct lam_ssi_rpi_proc *ps)
{
  char sink[512];		/* data sink */
  int nread;

  do {
    nread = sread(ps->cp_sock, sink, LAM_min(ps->cp_extra, 512));
    if (nread <= 0) {
      return (nread);
    }
    ps->cp_extra -= nread;
  } while (ps->cp_extra > 0);
  /*
   * Set process up to read the next incoming envelope.
   */
  ps->cp_readfn = lam_ssi_rpi_tcp_proc_read_env;
  return (0);
}


/*
 *	tcp_req_send_ack_long
 *
 *	Function:	- send ack and prepare to receive message body
 *			- long protocol
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_send_ack_long(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  if ((nbytes = tcp_push_env(ps, req)) <= 0) {
    return (nbytes);
  }

  if (req->rq_rpi->cq_nenvout == 0) {
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
    req->rq_rpi->cq_adv = tcp_req_rcvd_2nd;
  }

  return (0);
}


/*
 *	tcp_req_send_ack_only
 *
 *	Function:	- send an acknowledgment
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_send_ack_only(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  if ((nbytes = tcp_push_env(ps, req)) <= 0) {
    return (nbytes);
  }

  if (req->rq_rpi->cq_nenvout == 0) {
    /*
     * The acknowledgment has been sent.
     */
    lam_ssi_rpi_tcp_haveadv = 1;
    ps->cp_wreq = 0;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (0);
}


/*
 *	tcp_req_send_body
 *
 *	Function:	- send message body
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_send_body(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  if ((nbytes = tcp_push_body(ps, req)) <= 0) {
    return (nbytes);
  }

  if (req->rq_rpi->cq_nmsgout == 0) {
    /*
     * All of the message has been written.
     */
    lam_ssi_rpi_tcp_haveadv = 1;
    ps->cp_wreq = 0;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (0);
}


/*
 *	_tcp_req_send_long
 *
 *	Function:	- send the first envelope of a long message and
 *			  prepare to receive the ack
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_req_send_long(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  if ((nbytes = tcp_push_env(ps, req)) <= 0) {
    return (nbytes);
  }

  req->rq_state = LAM_RQSACTIVE;

  if (req->rq_rpi->cq_nenvout == 0) {
    /*
     * Prepare to read long protocol ack.
     */
    lam_ssi_rpi_tcp_haveadv = 1;
    ps->cp_wreq = 0;
    req->rq_rpi->cq_state = C2CREAD;
    req->rq_rpi->cq_env.ce_flags |= C2CACK;
    req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
    req->rq_rpi->cq_adv = tcp_req_rcvd_ack_long;
  }

  return (0);
}


/*
 *	tcp_req_send_short
 *
 *	Function:	- send a short protocol message
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_req_send_short(struct lam_ssi_rpi_proc *ps,
			       MPI_Request req)
{
  int nbytes;

  if ((nbytes = tcp_push_body(ps, req)) <= 0) {
    return (nbytes);
  }

  req->rq_state = LAM_RQSACTIVE;

  if (req->rq_rpi->cq_nenvout == 0 && req->rq_rpi->cq_nmsgout == 0) {
    lam_ssi_rpi_tcp_haveadv = 1;
    ps->cp_wreq = 0;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (0);
}


/*
 *	tcp_req_send_synch
 *
 *	Function:	- send short synchronous protocol message body
 *			  and prepare to read the ack
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_req_send_synch(struct lam_ssi_rpi_proc *ps,
			       MPI_Request req)
{
  int nbytes;

  if ((nbytes = tcp_push_body(ps, req)) <= 0) {
    return (nbytes);
  }
  /*
   * The message has been sent.  Prepare to read the acknowledgment.
   */
  req->rq_state = LAM_RQSACTIVE;

  if (req->rq_rpi->cq_nenvout == 0 && req->rq_rpi->cq_nmsgout == 0) {
    lam_ssi_rpi_tcp_haveadv = 1;
    ps->cp_wreq = 0;
    req->rq_rpi->cq_state = C2CREAD;
    req->rq_rpi->cq_env.ce_flags |= C2CACK;
    req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
    req->rq_rpi->cq_adv = tcp_req_done;
  }

  return (0);
}


/*
 *	tcp_req_rcvd_2nd
 *
 *	Function:	- read the body of a long protocol message
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_rcvd_2nd(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  ps->cp_rreq = req;
  ps->cp_msgbuf = req->rq_packbuf;
  ps->cp_nmsgin = ps->cp_env.ce_len;
  ps->cp_readfn = tcp_proc_read_body;
  req->rq_rpi->cq_adv = tcp_req_done;

  return (tcp_proc_read_body(ps));
}


/*
 *	tcp_req_done
 *
 *	Function:	- mark request as done
 *	Accepts:	- source process (ignored)
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_done(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}


/*
 *	tcp_req_rcvd_body_synch
 *
 *	Function:	- prepare to send an ack
 *			- short synchronous protocol
 *	Accepts:	- source process (ignored)
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_rcvd_body_synch(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_rpi->cq_state = C2CWRITE;
  req->rq_rpi->cq_env.ce_flags |= C2CACK;
  req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;

  /* These are not used; zero them out for purify clean */

  LAM_ZERO_ME(req->rq_rpi->cq_env.ce_len);
  LAM_ZERO_ME(req->rq_rpi->cq_env.ce_seq);

  req->rq_rpi->cq_adv = tcp_req_send_ack_only;
  lam_set_out_envelope_m(req->rq_rpi);

  return (0);
}


/*
 *	tcp_req_rcvd_ack_long
 *
 *	Function:	- long protocol transition from reading ack to
 *			  done (if receiver wants 0 bytes) or sending
 *			  requested # of bytes
 *	Accepts:	- destination process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_req_rcvd_ack_long(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  lam_ssi_rpi_tcp_haveadv = 1;
  /*
   * Set message length to minimum of what sender and receiver specified.
   */
  if (req->rq_packsize < ps->cp_env.ce_len) {
    req->rq_rpi->cq_nmsgout = req->rq_packsize;
  } else {
    req->rq_rpi->cq_nmsgout = ps->cp_env.ce_len;
  }

  if (req->rq_rpi->cq_nmsgout == 0) {
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  } else {
    req->rq_rpi->cq_state = C2CWRITE;
    req->rq_rpi->cq_env.ce_len = req->rq_rpi->cq_nmsgout;
    req->rq_rpi->cq_env.ce_flags &= ~C2CACK;
    req->rq_rpi->cq_env.ce_flags |= C2C2ND;
    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;
    req->rq_rpi->cq_adv = tcp_req_send_body;
    lam_set_out_envelope_m(req->rq_rpi);
  }

  return (0);
}


/*
 *	_tcp_req_probe
 *
 *	Function:	- mark probe request as done
 *			- the matched incoming envelope/message is buffered
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_req_probe(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  lam_ssi_rpi_tcp_haveadv = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  lam_ssi_rpi_fill_mpi_status(req, ps->cp_env.ce_rank,
			      ps->cp_env.ce_tag, ps->cp_env.ce_len);

  return (tcp_buffer(ps));
}


/*
 *	tcp_req_recv
 *
 *	Function:	- determine protocol for receive request on
 *			  matched incoming envelope and act upon it
 *			- this is called for the first packet only
 *	Accepts:	- source process
 *			- request
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_req_recv(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  struct lam_ssi_rpi_envl *env;	/* the envelope */
  int extra;			/* # bytes to go into sink */

  req->rq_state = LAM_RQSACTIVE;
  env = &ps->cp_env;
  lam_ssi_rpi_tcp_fill_wildcards(req, env);

  if (env->ce_flags & C2CLONG) {
    /*
     * Got a long protocol envelope.
     * Check for message length mismatch, set status and reply with an ack.
     */
    lam_ssi_rpi_tcp_haveadv = 1;
    if (env->ce_len > req->rq_packsize) {
      req->rq_flags |= LAM_RQFTRUNC;
      env->ce_len = req->rq_packsize;
    }

    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag,
				env->ce_len);

    req->rq_rpi->cq_state = C2CWRITE;
    req->rq_rpi->cq_env.ce_flags |= C2CACK;
    req->rq_rpi->cq_env.ce_len = env->ce_len;
    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;

    /* The sequence number is not examined on a C2C long ACK, but
     * it gives an RUI in memory checking debuggers. */

    LAM_ZERO_ME(req->rq_rpi->cq_env.ce_seq);

    req->rq_rpi->cq_adv =
      (env->ce_len > 0) ? tcp_req_send_ack_long : tcp_req_send_ack_only;
    lam_set_out_envelope_m(req->rq_rpi);
  } else {
    /*
     * Got a short protocol envelope.
     * Check for length mismatch and set what still must be read in.
     */
    if (env->ce_len <= req->rq_packsize) {
      extra = 0;
    } else {
      extra = env->ce_len - req->rq_packsize;
      env->ce_len = req->rq_packsize;
      req->rq_flags |= LAM_RQFTRUNC;
    }

    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag,
				env->ce_len);

    if (env->ce_len == 0) {
      /*
       * Zero length message. Send ack if matched a synchronous send
       * otherwise complete the request.
       */
      lam_ssi_rpi_tcp_haveadv = 1;
      if (env->ce_flags & C2CSSEND) {
	if (tcp_req_rcvd_body_synch((struct lam_ssi_rpi_proc *) 0, req)) {
	  return (LAMERROR);
	}
      } else {
	req->rq_state = LAM_RQSDONE;
	lam_rq_nactv--;
      }
    } else {
      /*
       * Read the message body.
       */
      ps->cp_rreq = req;
      ps->cp_nmsgin = env->ce_len;
      ps->cp_extra = extra;
      ps->cp_msgbuf = req->rq_packbuf;
      ps->cp_readfn = tcp_proc_read_body;
      req->rq_rpi->cq_adv = (env->ce_flags & C2CSSEND) ?
	tcp_req_rcvd_body_synch : tcp_req_done;

      return (tcp_proc_read_body(ps));
    }
  }

  return (0);
}


/*
 *	_tcp_buffered_adv
 *
 *	Function:	- determine protocol for receive request matching
 *			  a buffered envelope/message and act upon it
 *	Accepts:	- request
 *			- buffered envelope/message
 *	Returns:	- 0 or LAMERROR
 */
int
lam_ssi_rpi_tcp_buffered_adv(MPI_Request req,
			     struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_envl *env;	/* matching incoming env. */
  int len;			/* message length */
  int extra;			/* # bytes to go into sink */
  int nread;			/* # bytes read */

  env = &msg->cm_env;
  if (req->rq_type == LAM_RQIPROBE) {
    /*
     * The request is a probe. Set the status and leave the envelope buffered.
     */
    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag,
				env->ce_len);
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;

    return (0);
  }
  /*
   * Special case of a synchronous send with the sender being the same as
   * the receiver.  Copy the message data directly from the sender's
   * buffer and both requests are done.  
   */
  if (msg->cm_req) {
    if (env->ce_len > req->rq_packsize) {
      req->rq_flags |= LAM_RQFTRUNC;
      env->ce_len = req->rq_packsize;
    }

    if (env->ce_len > 0)
      lam_memcpy(req->rq_packbuf, msg->cm_buf, env->ce_len);

    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag,
				env->ce_len);
    req->rq_state = LAM_RQSDONE;
    msg->cm_req->rq_state = LAM_RQSDONE;
    lam_rq_nactv -= 2;
  } else if (env->ce_flags & C2CLONG) {
    /*
     * Matched a long protocol envelope.
     * Check for message length mismatch, set status and reply with an ack.
     */
    if (env->ce_len > req->rq_packsize) {
      req->rq_flags |= LAM_RQFTRUNC;
      env->ce_len = req->rq_packsize;
    }

    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag,
				env->ce_len);
    req->rq_state = LAM_RQSACTIVE;
    req->rq_rpi->cq_state = C2CWRITE;
    req->rq_rpi->cq_env.ce_flags |= C2CACK;
    req->rq_rpi->cq_env.ce_len = env->ce_len;
    req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;

    /* The sequence number is not examined on a C2C long ACK, but
     * it gives an RUI in memory checking debuggers. */

    LAM_ZERO_ME(req->rq_rpi->cq_env.ce_seq);

    req->rq_rpi->cq_adv =
      (env->ce_len > 0) ? tcp_req_send_ack_long : tcp_req_send_ack_only;
    lam_set_out_envelope_m(req->rq_rpi);
  } else {
    /*
     * Matched a short protocol envelope. In this case there may be a message
     * body which may or not yet be completely read in.
     *
     * Check for length mismatch and set what still must be read in.
     */
    if (env->ce_len <= req->rq_packsize) {
      len = env->ce_len;
      extra = 0;
    } else {
      len = req->rq_packsize;
      extra = env->ce_len - req->rq_packsize;
      req->rq_flags |= LAM_RQFTRUNC;
    }
    lam_ssi_rpi_fill_mpi_status(req, env->ce_rank, env->ce_tag, len);

    if (msg->cm_proc == 0) {
      /*
       * The message has been completely buffered since there is no process
       * associated with it. Copy the message from the buffer and advance the
       * request.
       */
      if (len)
	lam_memcpy(req->rq_packbuf, msg->cm_buf, len);

      if (env->ce_flags & C2CSSEND) {
	req->rq_state = LAM_RQSACTIVE;
	if (tcp_req_rcvd_body_synch((struct lam_ssi_rpi_proc *) 0, req))
	  return (LAMERROR);
      } else {
	req->rq_state = LAM_RQSDONE;
	lam_rq_nactv--;
      }
    } else {
      /*
       * There are still bytes to be read from the sender.
       * Copy into receiver's buffer what has been read so far and then
       * set the process up to read in the rest.
       */
      nread = env->ce_len - msg->cm_proc->p_rpi->cp_nmsgin;
      if (len)
	lam_memcpy(req->rq_packbuf, msg->cm_buf, LAM_min(nread, len));

      if (nread < len) {
	/*
	 * There is still message body to be read. Change the process
	 * read state which was to read everything into the system
	 * buffer. This was because there was no matching receive and
	 * the receive size was unknown.
	 */
	msg->cm_proc->p_rpi->cp_nmsgin = len - nread;
	msg->cm_proc->p_rpi->cp_extra = extra;
	msg->cm_proc->p_rpi->cp_msgbuf = req->rq_packbuf + nread;
	msg->cm_proc->p_rpi->cp_rreq = req;
	msg->cm_proc->p_rpi->cp_readfn = tcp_proc_read_body;
	req->rq_state = LAM_RQSACTIVE;
	req->rq_rpi->cq_adv = (env->ce_flags & C2CSSEND) ?
	  tcp_req_rcvd_body_synch : tcp_req_done;
      } else {
	/*
	 * The whole message has been read. Complete the request and set the
	 * process read state to read the remaining bytes into the sink.
	 */
	if (env->ce_flags & C2CSSEND) {
	  req->rq_state = LAM_RQSACTIVE;
	  if (tcp_req_rcvd_body_synch((struct lam_ssi_rpi_proc *) 0, req)) {
	    return (LAMERROR);
	  }
	} else {
	  req->rq_state = LAM_RQSDONE;
	  lam_rq_nactv--;
	}
	msg->cm_proc->p_rpi->cp_extra = extra - (nread - len);
	msg->cm_proc->p_rpi->cp_readfn = tcp_proc_read_extra;
      }
    }
  }
  /*
   * Discard the buffered message.
   */
  lam_ssi_rpi_cbuf_delete(msg);

  return (0);
}


/*
 *	tcp_match_adv
 *
 *	Function:	- match an envelope read from a process with a read
 *			  request if possible and advance the matched request
 *			- if no match is found then the env/msg is buffered
 *	Accepts:	- envelope's source process
 */
static int
tcp_match_adv(struct lam_ssi_rpi_proc *ps)
{
  MPI_Request req;		/* request */
  /*
   * There cannot be any matching recvs after a matching probe because
   * probes are blocking. Thus we may return upon the first match
   * (buffering the envelope in the case of a probe) and maintain the
   * invariant "no requests in the list match buffered
   * envelopes". This means once a request is in the list after being
   * checked against buffered envelopes it need never again be checked
   * against any buffered envelopes.
   */
  for (req = ps->cp_mreq; req; req = req->rq_next) {

    if ((req->rq_state != LAM_RQSDONE)
	&& (req->rq_rpi->cq_state == C2CREAD)
	&& (!lam_ssi_rpi_envl_cmp(&ps->cp_env, &req->rq_rpi->cq_env))) {

      return (req->rq_rpi->cq_adv(ps, req));
    }
  }

  return (tcp_buffer(ps));
}


/*
 *	tcp_buffer
 *
 *	Function:	- buffer incoming envelope/message
 *			- there is never any data to be read to the
 *			  data sink when buffering
 *	Accepts:	- process envelope came in from
 *	Returns:	- 0 or LAMERROR
 */
static int
tcp_buffer(struct lam_ssi_rpi_proc *ps)
{
  struct lam_ssi_rpi_cbuf_msg msg;	/* buffer list entry */

  /* In 64 bit mode, this struct may have extra padding */

  LAM_ZERO_ME(msg);
  msg.cm_env = ps->cp_env;
  msg.cm_req = 0;
  msg.cm_dont_delete = 0;

  if (ps->cp_env.ce_len > 0 && !(ps->cp_env.ce_flags & C2CLONG)) {
    /*
     * Set up the buffer for the message body and read as much as possible.
     */
    msg.cm_buf = (char *) malloc(ps->cp_env.ce_len);
    if (msg.cm_buf == 0)
      return (LAMERROR);

    msg.cm_proc = ps->cp_proc;
    ps->cp_nmsgin = ps->cp_env.ce_len;
    ps->cp_msgbuf = msg.cm_buf;
    ps->cp_readfn = tcp_proc_read_buffer;
    ps->cp_bmsg = lam_ssi_rpi_cbuf_append(&msg);

    if (!ps->cp_bmsg)
      return (LAMERROR);

    return (tcp_proc_read_buffer(ps));
  } else {
    msg.cm_buf = 0;
    msg.cm_proc = 0;
    return (lam_ssi_rpi_cbuf_append(&msg) ? 0 : LAMERROR);
  }
}


/*
 *	tcp_push_body
 *
 *	Function:	- push request envelope and message body down the pike
 *	Accepts:	- process
 *			- request
 *	Returns:	- # bytes written or LAMERROR
 */
static int
tcp_push_body(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  struct iovec iov[2];		/* IO vector */
  int nbytes;			/* # bytes written */
  int msgbytes;			/* # bytes msg body written */

  if (req->rq_rpi->cq_nenvout > 0 && req->rq_rpi->cq_nmsgout > 0) {

#if WANT_SEND_MESSAGES
    {
      struct lam_ssi_rpi_envl *foo =
	(struct lam_ssi_rpi_envl *) req->rq_rpi->cq_envbuf;
      DBUG
	("LAM C2C progression engine SHORT sent env (pid %d): len %d tag %d rank %d cid %d (iovec)\n",
	 lam_getpid(), foo->ce_len, foo->ce_tag, foo->ce_rank, foo->ce_cid);
    }
#endif

    iov[0].iov_base = (caddr_t) req->rq_rpi->cq_envbuf;
    iov[0].iov_len = req->rq_rpi->cq_nenvout;

    iov[1].iov_base = (caddr_t) req->rq_rpi->cq_msgbuf;
    iov[1].iov_len = req->rq_rpi->cq_nmsgout;

    nbytes = swritev(ps->cp_sock, iov, 2);

    if (nbytes > 0) {
      if (nbytes >= req->rq_rpi->cq_nenvout) {
	msgbytes = nbytes - req->rq_rpi->cq_nenvout;
	req->rq_rpi->cq_nenvout = 0;
	req->rq_rpi->cq_msgbuf += msgbytes;
	req->rq_rpi->cq_nmsgout -= msgbytes;
      } else {
	req->rq_rpi->cq_envbuf += nbytes;
	req->rq_rpi->cq_nenvout -= nbytes;
      }
    }
  } else if (req->rq_rpi->cq_nmsgout > 0) {

#if WANT_SEND_MESSAGES
    DBUG
      ("LAM C2C progression engine SHORT sent env (pid %d): don't reall y know what\n");
#endif

    nbytes = swrite(ps->cp_sock,
		    req->rq_rpi->cq_msgbuf, req->rq_rpi->cq_nmsgout);

    if (nbytes > 0) {
      req->rq_rpi->cq_msgbuf += nbytes;
      req->rq_rpi->cq_nmsgout -= nbytes;
    }
  } else {

#if WANT_SEND_MESSAGES
    {
      struct lam_ssi_rpi_envl *foo =
	(struct lam_ssi_rpi_envl *) req->rq_rpi->cq_envbuf;
      DBUG
	("LAM C2C progression engine SHORT sent env (pid %d): len %d tag %d rank %d cid %d\n",
	 lam_getpid(), foo->ce_len, foo->ce_tag, foo->ce_rank, foo->ce_cid);
    }
#endif

    nbytes = swrite(ps->cp_sock,
		    req->rq_rpi->cq_envbuf, req->rq_rpi->cq_nenvout);

    if (nbytes > 0) {
      req->rq_rpi->cq_envbuf += nbytes;
      req->rq_rpi->cq_nenvout -= nbytes;
    }
  }

  return (nbytes);
}


/*
 *	tcp_push_env
 *
 *	Function:	- push request envelope down the pike
 *	Accepts:	- process
 *			- request
 *	Returns:	- # bytes written or LAMERROR
 */
static int
tcp_push_env(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;			/* # bytes written */

  nbytes = swrite(ps->cp_sock, req->rq_rpi->cq_envbuf,
		  req->rq_rpi->cq_nenvout);

#if WANT_SEND_MESSAGES
  {
    struct lam_ssi_rpi_envl *foo =
      (struct lam_ssi_rpi_envl *) req->rq_rpi->cq_envbuf;
    DBUG
      ("LAM C2C progression engine sent env (pid %d): len %d tag %d rank %d cid %d\n",
       lam_getpid(), foo->ce_len, foo->ce_tag, foo->ce_rank, foo->ce_cid);
  }
#endif

  if (nbytes > 0) {
    req->rq_rpi->cq_envbuf += nbytes;
    req->rq_rpi->cq_nenvout -= nbytes;
  }

  return (nbytes);
}


/*
 *	sread
 *
 *	Function:	- atomic socket read()
 *	Returns:	- # bytes read or LAMERROR
 */
static int
sread(int sock, char *buf, int nbytes)
{
  int nread = 0;		/* # of bytes read */
  int r;			/* read syscall return val */
  double starttime = 0.0;	/* time blocking starts */

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {
    starttime = ttime();
  }

  do {
    r = read(sock, buf, nbytes);

    if (r < 0) {
      if (errno == EAGAIN) {
	break;
      }
      if (errno != EINTR) {
	comminvalidate(sock);
	return (0);
      }
    } else if ((r == 0) && (nbytes > 0)) {	/* eof */
      if (FD_ISSET(sock, &lam_ssi_rpi_tcp_eoferr)) {
	comminvalidate(sock);
      }
      return (0);
    } else {
      nread += r;
      buf += r;
      nbytes -= r;
    }
  } while (nbytes > 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  FD_CLR(sock, &lam_ssi_rpi_tcp_eoferr);
  return (nread);
}


/*
 *	swrite
 *
 *	Function:	- atomic socket write()
 *	Returns:	- # bytes written or 0
 */
static int
swrite(int sock, char *buf, int nbytes)
{
  int nwritten = 0;		/* # of bytes written */
  int r;			/* write syscall return val */
  double starttime = 0.0;	/* time blocking starts */

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {
    starttime = ttime();
  }

  do {
    r = write(sock, buf, nbytes);

    if (r < 0) {
      if (errno == EAGAIN) {
	break;
      }
      if (errno != EINTR) {
	comminvalidate(sock);
	nwritten = 0;
	break;
      }
    } else if ((r == 0) && (nbytes > 0)) {	/* eof */
      comminvalidate(sock);
      nwritten = 0;
      break;
    } else {
      nwritten += r;
      buf += r;
      nbytes -= r;
    }
  } while (nbytes > 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  return (nwritten);
}


/*
 *	swritev
 *
 *	Function:	- atomic socket writev()
 *	Returns:	- #bytes written or 0
 */
static int
swritev(int sock, struct iovec *iov, int iovcnt)
{
  int nwritten = 0;		/* # of bytes written */
  int r;			/* writev syscall return val */
  int savelen = 0;		/* save full length */
  char *savebase = 0;		/* save original base ptr */
  double starttime = 0.0;	/* time blocking starts */

  if (iovcnt > 0) {
    savelen = iov->iov_len;
    savebase = iov->iov_base;
  }

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {
    starttime = ttime();
  }

  do {
    r = writev(sock, iov, iovcnt);

    if (r < 0) {
      if (errno == EAGAIN) {
	break;
      }
      if (errno != EINTR) {
	iov->iov_len = savelen;
	iov->iov_base = savebase;
	comminvalidate(sock);
	nwritten = 0;
	break;
      }
    } else if (r == 0) {	       /* eof */
      iov->iov_len = savelen;
      iov->iov_base = savebase;

      while ((iovcnt > 0) && (iov->iov_len == 0)) {
	iov++;
	iovcnt--;
      }

      if (iovcnt > 0) {
	comminvalidate(sock);
	nwritten = 0;
      }

      break;
    } else {
      nwritten += r;

      while (r > 0) {

	if (r >= iov->iov_len) {
	  r -= iov->iov_len;
	  iov->iov_len = savelen;
	  iov->iov_base = savebase;
	  iov++;
	  iovcnt--;

	  if (iovcnt > 0) {
	    savelen = iov->iov_len;
	    savebase = iov->iov_base;
	  }
	} else {
	  iov->iov_len -= r;
	  iov->iov_base = (char *) iov->iov_base + r;
	  r = 0;
	}
      }
    }
  } while (iovcnt > 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  return (nwritten);
}


/*
 *	sselect
 *
 *	Function:	- atomic socket select()
 *	Returns:	- # ready descriptors or LAMERROR
 */
static int
sselect(int width, fd_set * readfds, fd_set * writefds, fd_set * exceptfds,
	struct timeval *timeout)
{
  int nready;			/* number ready descriptors */
  double starttime = 0.0;	/* time blocking starts */

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON) && (timeout == 0)) {
    starttime = ttime();
  }

  do {
    nready = select(width, readfds, writefds, exceptfds, timeout);

    if (nready < 0 && errno != EINTR)
      return (LAMERROR);
  } while (nready < 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON) && (timeout == 0)) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  return (nready);
}


/*
 *	setsockblk
 *
 *	Function:	- set socket to block/non-block mode
 *	Accepts:	- socket descriptor
 *			- blocking flag
 *	Returns:	- 0 or LAMERROR
 */
static int
setsockblk(int sock, int fl_block)
{
  if (fl_block) {
    /*
     * Switch socket to blocking if needed.
     */
    if (!FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {

#if LAM_HAVE_FIONBIO
      int flag = 0;
      if (ioctl(sock, FIONBIO, &flag) == -1)
	return (LAMERROR);
#else
      if (fcntl(sock, F_SETFL, 0) == -1)
	return (LAMERROR);
#endif

      FD_SET(sock, &lam_ssi_rpi_tcp_block);
    }
  } else {
    /*
     * Switch socket to non-blocking if needed.
     */
    if (FD_ISSET(sock, &lam_ssi_rpi_tcp_block)) {

#if LAM_HAVE_FIONBIO
      int flag = 1;
      if (ioctl(sock, FIONBIO, &flag) == -1)
	return (LAMERROR);
#else
      if (fcntl(sock, F_SETFL, O_NONBLOCK))
	return (LAMERROR);
#endif

      FD_CLR(sock, &lam_ssi_rpi_tcp_block);
    }
  }

  return 0;
}


/*
 *	comminvalidate
 *
 *	Function:	- invalidate communicators with processes on the node
 *			  a bad socket is connected to
 *	Accepts:	- bad socket descriptor
 */
static void
comminvalidate(int sock)
{
  int i;
  struct _proc *p, **array;

  FD_CLR(sock, &lam_ssi_rpi_tcp_read);
  FD_CLR(sock, &lam_ssi_rpi_tcp_write);
  FD_CLR(sock, &lam_ssi_rpi_tcp_except);

  array = lam_arr_get(PUB(procs));
  for (i = 0; i < lam_arr_size(PUB(procs)); ++i) {
    p = array[i];
    if (p->p_rpi->cp_sock == sock) {
      lam_ssi_rpi_commfault(p->p_gps.gps_node);

      if (p->p_rpi->cp_rreq)
	(void) lam_ssi_rpi_comm_dead_m(p->p_rpi->cp_rreq);

      if (p->p_rpi->cp_wreq)
	(void) lam_ssi_rpi_comm_dead_m(p->p_rpi->cp_wreq);
      lam_ssi_rpi_tcp_haveadv = 1;
    }
  }
}


/*
 * The routines below implement the progress engine short circuit for
 * blocking send/receive requests when there are NO outstanding
 * requests.  Yes they are long, verbose and duplicate code found in
 * other functions but they are written this way for speed.
 */

/*
 *	lam_ssi_rpi_tcp_low_fastsend
 *
 *	Function:	- fast blocking send to a process
 *			- short circuits the usual request progression
 *			  mechanism and only be called if there are NO
 *			  active requests
 *	Accepts:	- buffer to send
 *			- size of buffer
 *			- destination process
 *			- message tag
 *			- message communicator
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_tcp_low_fastsend(char *buf, int size, struct lam_ssi_rpi_proc *ps,
			     int dest, int tag, MPI_Comm comm)
{
  struct iovec iov[2];		/* IO vector */
  struct lam_ssi_rpi_envl env;
  int err;
  /*
   * All communication is done in a blocking manner.  Hence EOF is
   * considered to be an error.  
   */
  if (setsockblk(ps->cp_sock, TRUE)) {
    return (LAMERROR);
  }
  /*
   * Set up message envelope.
   */
  env.ce_len = size;
  env.ce_tag = tag;
  env.ce_rank = comm->c_group->g_myrank;
  env.ce_cid = comm->c_contextid;
  env.ce_seq = lam_seqnum++;

#if WANT_SEND_MESSAGES
  {
    DBUG
      ("LAM C2C progression engine FAST sent env (pid %d): len %d tag %d rank %d cid %d\n",
       lam_getpid(), env.ce_len, env.ce_tag, env.ce_rank, env.ce_cid);
  }
#endif

  if (size <= lam_ssi_rpi_tcp_short) {
    /*
     * Short message.  Send envelope and message body.
     */
    env.ce_flags = 0;

    if (!lam_homog) {
      mltoti4((int4 *) & env,
	      sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));
    }

    if (size == 0) {
      if (swrite
	  (ps->cp_sock, (char *) &env,
	   sizeof(struct lam_ssi_rpi_envl)) <= 0) {
	return (badsockerr(comm));
      }
    } else {
      iov[0].iov_base = (caddr_t) & env;
      iov[0].iov_len = sizeof(struct lam_ssi_rpi_envl);

      iov[1].iov_base = (caddr_t) buf;
      iov[1].iov_len = size;

      if (swritev(ps->cp_sock, iov, 2) <= 0) {
	return (badsockerr(comm));
      }
    }
  } else {
    /*
     * Long message.  Send the envelope.
     */
    env.ce_flags = C2CLONG;

    if (!lam_homog) {
      mltoti4((int4 *) & env,
	      sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));
    }

    if (swrite(ps->cp_sock, (char *) &env, sizeof(struct lam_ssi_rpi_envl))
	<= 0) {
      return (badsockerr(comm));
    }
    /*
     * Wait for the acknowledgment.
     */
    if ((err = waitforack(ps, dest, tag, comm, &size))) {
      return (err);
    }

    if (size == 0) {
      return (MPI_SUCCESS);
    }
    /*
     * Send the message body.
     */
    if (lam_homog) {
      env.ce_len = size;
      env.ce_flags = C2C2ND;
    } else {
      env.ce_len = ltot(size);
      env.ce_flags = ltot(C2C2ND);
    }

    iov[0].iov_base = (caddr_t) & env;
    iov[0].iov_len = sizeof(struct lam_ssi_rpi_envl);

    iov[1].iov_base = (caddr_t) buf;
    iov[1].iov_len = size;

    if (swritev(ps->cp_sock, iov, 2) <= 0) {
      return (badsockerr(comm));
    }
  }

  return (MPI_SUCCESS);
}


/*
 *	lam_ssi_rpi_tcp_low_fastrecv
 *
 *	Function:	- fast blocking receive from a process
 *			- short circuits the usual request progression
 *			  mechanism and only be called if there are NO
 *			  active requests
 *	Accepts:	- buffer to receive into
 *			- size of buffer (inout)
 *			- source process
 *			- message tag (inout)
 *			- message communicator
 *			- message sequence number (out)
 *	Returns:	- MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_tcp_low_fastrecv(char *buf, int *size, 
			     struct lam_ssi_rpi_proc *ps,
			     int src, int *tag, MPI_Comm comm, int *seqnum)
{
  struct lam_ssi_rpi_envl env;	/* holds an envelope */
  struct lam_ssi_rpi_envl *inenv;	/* proc's incoming envelope */
  struct lam_ssi_rpi_cbuf_msg *bmsg;	/* buffered message */
  int msglen;
  int synch;			/* synchronous send? */
  int err;
  /*
   * All communication is done in a blocking manner and EOF is considered
   * to be an error.  
   */
  if (setsockblk(ps->cp_sock, TRUE))
    return (LAMERROR);

  FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_eoferr);
  /*
   * If currently busy doing a buffered receive on the socket complete it.  
   */
  if (ps->cp_readfn == tcp_proc_read_buffer) {
    if ((err = ps->cp_readfn(ps)) < 0)
      return (badsockerr(comm));
    if (err == 0 && ps->cp_nmsgin != 0)
      return (badsockerr(comm));
  }

  err = MPI_SUCCESS;
  /*
   * Set an envelope with the required synchronization for matching against
   * incoming envelopes.
   */
  LAM_ZERO_ME(env);
  env.ce_flags = 0;
  env.ce_rank = src;
  env.ce_tag = *tag;
  env.ce_cid = comm->c_contextid;

  inenv = (struct lam_ssi_rpi_envl *) &ps->cp_env;
  /*
   * Check for match with buffered message.
   */
  if ((bmsg = lam_ssi_rpi_cbuf_find(&env))) {
    if (bmsg->cm_env.ce_len > *size)
      err = lam_mkerr(MPI_ERR_TRUNCATE, 0);
    else
      *size = bmsg->cm_env.ce_len;

    *tag = bmsg->cm_env.ce_tag;
    *seqnum = bmsg->cm_env.ce_seq;
    msglen = *size;

    if (bmsg->cm_env.ce_flags & C2CLONG) {
      lam_ssi_rpi_cbuf_delete(bmsg);
      goto longmsg;
    } else {
      if (msglen)
	lam_memcpy(buf, bmsg->cm_buf, msglen);

      if (bmsg->cm_env.ce_flags & C2CSSEND) {
	lam_ssi_rpi_cbuf_delete(bmsg);
	goto synchmsg;
      } else
	lam_ssi_rpi_cbuf_delete(bmsg);

      return (err);
    }
  }
  /*
   * Block waiting for next incoming envelope.
   */
  while (1) {
    if (lam_ger && ps->cp_proc->p_num_buf_env >= MPI_GER)
      return (lam_mkerr(MPI_ERR_INTERN, EGERFLOW));

    FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_eoferr);
    if (sread(ps->cp_sock, (char *) inenv, sizeof(struct lam_ssi_rpi_envl))
	<= 0)
      return (badsockerr(comm));

#if WANT_RECV_MESSAGES
    {
      DBUG
	("LAM C2C progression engine FAST recv env (pid %d): len %d tag %d rank %d cid %d\n",
	 lam_getpid(), env.ce_len, env.ce_tag, env.ce_rank, env.ce_cid);
    }
#endif

    if (!lam_homog)
      mttoli4((int4 *) inenv,
	      sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

    if (lam_ssi_rpi_envl_cmp(inenv, &env)) {
      if (tcp_buffer(ps))
	return (badsockerr(comm));
    } else
      break;
  }
  /*
   * Check for message length mismatch.
   */
  if (inenv->ce_len > *size)
    err = lam_mkerr(MPI_ERR_TRUNCATE, 0);
  else
    *size = inenv->ce_len;

  *tag = inenv->ce_tag;
  *seqnum = inenv->ce_seq;
  msglen = *size;

  if (inenv->ce_flags & C2CLONG) {
    /*
     * Got a long protocol envelope. Send an acknowledgment.  
     */
  longmsg:
    env.ce_tag = *tag;
    env.ce_rank = comm->c_group->g_myrank;
    env.ce_flags = C2CACK | C2CLONG;
    env.ce_len = msglen;

    if (!lam_homog)
      mltoti4((int4 *) & env,
	      sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

    if (swrite(ps->cp_sock, (char *) &env, sizeof(struct lam_ssi_rpi_envl))
	<= 0)
      return (badsockerr(comm));

    if (msglen) {
      /*
       * Receive the envelope at the start of the message body.
       */
      env.ce_flags = C2C2ND;
      env.ce_rank = src;
      env.ce_tag = *tag;
      env.ce_cid = comm->c_contextid;

      while (1) {
	if (lam_ger && ps->cp_proc->p_num_buf_env >= MPI_GER) {
	  return (lam_mkerr(MPI_ERR_INTERN, EGERFLOW));
	}

	FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_eoferr);
	if (sread(ps->cp_sock, (char *) inenv,
		  sizeof(struct lam_ssi_rpi_envl)) <= 0)
	  return (badsockerr(comm));

	if (!lam_homog)
	  mttoli4((int4 *) inenv,
		  sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

	if (lam_ssi_rpi_envl_cmp(inenv, &env)) {
	  if (tcp_buffer(ps))
	    return (badsockerr(comm));
	} else
	  break;
      }
      /*
       * Receive the message body.
       */
      FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_eoferr);
      if (sread(ps->cp_sock, buf, msglen) <= 0)
	return (badsockerr(comm));
    }
  } else {
    /*
     * Got a short protocol envelope.  Check for a synchronous send and
     * read the message body.
     */
    synch = inenv->ce_flags & C2CSSEND;

    if (*size > 0) {
      FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_eoferr);
      if (sread(ps->cp_sock, buf, *size) <= 0)
	return (badsockerr(comm));
    }

    if (inenv->ce_len > *size) {
      if (sinksuck(ps->cp_sock, inenv->ce_len - *size) <= 0)
	return (badsockerr(comm));
    }

    if (synch) {
      /*
       * Send a synchronous send acknowledgment.
       */
    synchmsg:
      env.ce_tag = *tag;
      env.ce_rank = comm->c_group->g_myrank;
      env.ce_flags = C2CACK;

      if (!lam_homog)
	mltoti4((int4 *) & env,
		sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

      if (swrite(ps->cp_sock, (char *) &env,
		 sizeof(struct lam_ssi_rpi_envl)) <= 0)
	return (badsockerr(comm));
    }
  }

  return (err);
}


/*
 *	lam_ssi_rpi_tcp_fill_wildcards
 *
 *	Function:	- replace wildcards in request with matched values
 *			  and fill in the sequence number
 *	Accepts:	- request
 *			- matched envelope
 */
void
lam_ssi_rpi_tcp_fill_wildcards(MPI_Request req, struct lam_ssi_rpi_envl *env)
{
  struct _group	*g;
		
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


/*
 *	waitforack
 *
 *	Function:	- wait for a long message acknowledgment
 *	Accepts:	- process sending the ack
 *			- rank of process sending the ack
 *			- ack tag
 *			- ack communicator
 *			- length ack'er will receive (out)
 *	Returns:	- MPI_SUCCESS or error code
 */
static int
waitforack(struct lam_ssi_rpi_proc *ps, int from, int tag, MPI_Comm comm,
	   int *len)
{
  struct lam_ssi_rpi_envl env;
  struct lam_ssi_rpi_envl *inenv;
  int err;
  /*
   * If currently busy doing a buffered receive on the socket complete it.  
   */
  if (ps->cp_readfn == tcp_proc_read_buffer) {
    if ((err = ps->cp_readfn(ps)) < 0)
      return (badsockerr(comm));
    if (err == 0 && ps->cp_nmsgin != 0)
      return (badsockerr(comm));
  }
  /*
   * Set up an envelope to match against incoming envelopes.
   */
  env.ce_flags = C2CACK;
  env.ce_rank = from;
  env.ce_tag = tag;
  env.ce_cid = comm->c_contextid;

  inenv = (struct lam_ssi_rpi_envl *) &ps->cp_env;
  /*
   * Loop receiving envelopes (+ messages) until the required
   * acknowledgment is received.  Non-matching envelopes (+ messages) are
   * buffered.  
   */
  while (1) {
    if (lam_ger && ps->cp_proc->p_num_buf_env >= MPI_GER)
      return (lam_mkerr(MPI_ERR_INTERN, EGERFLOW));

    FD_SET(ps->cp_sock, &lam_ssi_rpi_tcp_eoferr);
    if (sread(ps->cp_sock, (char *) inenv, 
	      sizeof(struct lam_ssi_rpi_envl)) <= 0)
      return (badsockerr(comm));

    if (!lam_homog)
      mttoli4((int4 *) inenv,
	      sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

    if (lam_ssi_rpi_envl_cmp(inenv, &env)) {
      if (tcp_buffer(ps))
	return (badsockerr(comm));
    } else
      break;
  }

  *len = inenv->ce_len;
  return (MPI_SUCCESS);
}


/*
 *	badsockerr
 *
 *	Function:	- get bad socket error code 
 *	Accepts:	- communicator socket is on
 *	Returns:	- error code
 */
static int
badsockerr(MPI_Comm comm)
{
  int err;

  if (comm->c_flags & LAM_CLDEAD)
    err = lam_mkerr(MPI_ERR_LOCALDEAD, 0);
  else
    err = lam_mkerr(MPI_ERR_REMOTEDEAD, 0);

  return (err);
}


/*
 *	sinksuck
 *
 *	Function:	- read truncated data from a socket into a sink
 *	Accepts:	- socket
 *			- length of data to read
 *	Returns:	- 1 (success) or 0 (eof) or -1 (error)
 */
static int
sinksuck(int sock, int len)
{
  char sink[512];		/* data sink */
  int nread;

  do {
    nread = sread(sock, sink, LAM_min(len, 512));
    if (nread <= 0)
      return (nread);

    len -= nread;
  } while (len > 0);

  return (1);
}
