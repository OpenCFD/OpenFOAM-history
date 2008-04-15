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
 * $Id: ssi_rpi_crtcp_low.c,v 1.40 2003/10/11 19:15:37 brbarret Exp $
 *
 *      Function:       - CRTCP low-level routines
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

#include <crtcp_low.h>
#include <rpi_crtcp.h>
#include <lam-ssi-rpi-crtcp.h>
#include <rpi_crtcp_bookmarks.h>
#include <lammpithreads.h>

#include <lam-ssi-cr.h>


/*
 * external functions
 */
extern void lam_commfault();

/*
 * private functions
 */
static int crtcp_proc_read_body(struct lam_ssi_rpi_proc *ps);
static int crtcp_proc_read_buffer(struct lam_ssi_rpi_proc *ps);
static int crtcp_proc_read_extra(struct lam_ssi_rpi_proc *ps);
static int crtcp_req_send_ack_long(struct lam_ssi_rpi_proc *ps,
                                 MPI_Request req);
static int crtcp_req_send_ack_only(struct lam_ssi_rpi_proc *ps,
                                 MPI_Request req);
static int crtcp_req_send_body(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int crtcp_req_rcvd_2nd(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int crtcp_req_rcvd_body_synch(struct lam_ssi_rpi_proc *ps,
                                   MPI_Request req);
static int crtcp_req_rcvd_ack_long(struct lam_ssi_rpi_proc *ps,
                                 MPI_Request req);
static int crtcp_req_done(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int crtcp_match_adv(struct lam_ssi_rpi_proc *ps);
static int crtcp_buffer(struct lam_ssi_rpi_proc *ps);
static int crtcp_push_body(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int crtcp_push_env(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int sread(struct lam_ssi_rpi_proc *ps, char **buf, int *nbytes, 
                 int maxbytes);
static int swrite(struct lam_ssi_rpi_proc *ps, char *buf, int nbytes);
static int swritev(struct lam_ssi_rpi_proc *ps, struct iovec *iov, int iovcnt);
static int sselect(int width, fd_set * readfds, fd_set * writefds,
                   fd_set * exceptfds, struct timeval *timeout);
static void comminvalidate(int sock);
#if CRTCP_DO_FAST
static int badsockerr(MPI_Comm comm);
static int sinksuck(struct lam_ssi_rpi_proc *ps, int len);
static int waitforack(struct lam_ssi_rpi_proc *ps, int from, int tag,
                      MPI_Comm comm, int *len);
#endif
static int enqueue_env(struct lam_ssi_rpi_proc *ps, MPI_Request req);
static int sread_app_suspend(struct lam_ssi_rpi_proc *ps);
static void swrite_app_suspend(struct lam_ssi_rpi_proc *ps);
static void sselect_app_suspend(void);


/*
 * public variables
 */
int PUB(flblock);
int PUB(haveadv);
int PUB(nio);                   /* # processes doing crtcp io */
int PUB(sockmax);               /* max. crtcp io socket num. */
fd_set PUB(read);               /* read sockets */
fd_set PUB(write);              /* write sockets */
fd_set PUB(except);             /* exception sockets */
fd_set PUB(block);              /* blocked mode socket? */
fd_set PUB(eoferr);             /* eof on socket is error? */
MPI_Request PUB(lastreq);       /* last crtcp request */
struct lam_ssi_rpi_proc *PUB(smap)[FD_SETSIZE]; /* maps socket fd to process */
unsigned int PUB(short) = LAM_CRTCPSHORTMSGLEN; /* short message length */
#if CRTCP_DO_FAST
int PUB(is_sread_in_fast) = 0;  /* are we in the middle of a _fastrecv? */
#endif

/*
 * external variables
 */
extern struct bookmark_payload_t *PUB(bookmarks);

/*
 * private variables
 */
static struct timeval zerotime = { 0, 0 };      /* zero timeval */

typedef struct long_ack_q_entry {
  struct lam_ssi_rpi_envl laq_env;
  struct lam_ssi_rpi_proc *laq_ps;
  struct long_ack_q_entry *laq_next;
} long_ack_q_entry_t;
static long_ack_q_entry_t *long_ack_q_head = NULL;
static long_ack_q_entry_t *long_ack_q_tail = NULL;

#if CRTCP_DO_FAST
static int is_fast = 0;
#endif


/*
 *      _crtcp_advmultiple
 *
 *      Function:       - advance multiple crtcp processes
 *      Accepts:        - request list
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_advmultiple(void)
{
  MPI_Request req;
  int sock;                     /* socket descriptor */
  int nready;                   /* # ready sockets */
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;

  memcpy((char *) &readfds, (char *) &PUB(read), 
         sizeof(fd_set));
  memcpy((char *) &writefds, (char *) &PUB(write), 
         sizeof(fd_set));
  memcpy((char *) &exceptfds, (char *) &PUB(except), 
         sizeof(fd_set));

  if (PUB(flblock)) {
    nready = sselect(PUB(sockmax) + 1, &readfds, &writefds,
                     &exceptfds, (struct timeval *) 0);
    /*
     * in blocking mode, when timeval is NULL, sselect returns 0 only when 
     * interrupted by the cr_handler
     */
    if (nready <= 0)
      return (nready);
  } else {
    nready = sselect(PUB(sockmax) + 1, &readfds,
                     &writefds, &exceptfds, &zerotime);
    if (nready < 0)
      return (nready);
  }
  /*
   * Loop through enabled sockets and advance on each one.
   */
  for (sock = 0; nready && sock <= PUB(sockmax); sock++) {
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
      if (lam_ssi_rpi_crtcp_setsockblk(sock, FALSE))
        return (LAMERROR);

      if (PUB(smap)[sock]->cp_readfn(PUB(smap)[sock]))
        return (LAMERROR);
    }

    if (FD_ISSET(sock, &writefds)) {
      --nready;
      if (lam_ssi_rpi_crtcp_setsockblk(sock, FALSE))
        return (LAMERROR);

      req = PUB(smap)[sock]->cp_wreq;
      if (req->rq_rpi->cq_adv(PUB(smap)[sock], req))
        return (LAMERROR);
    }
  }

  return (0);
}


/*
 *      _crtcp_adv1
 *
 *      Function:       - advance single crtcp process optimally
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_adv1(void)
{
  MPI_Request req;

  if (lam_ssi_rpi_comm_dead_m(PUB(lastreq))) {
    PUB(haveadv) = 1;
    return 0;
  }

  if (lam_ssi_rpi_crtcp_setsockblk(PUB(sockmax), PUB(flblock)))
    return (LAMERROR);

  if (PUB(lastreq)->rq_rpi->cq_state == C2CREAD) {
    /*
     * In blocking mode EOF is considered to be an error.
     */
    if (PUB(flblock))
      FD_SET(PUB(sockmax), &PUB(eoferr));

    return (PUB(smap)[PUB(sockmax)]->cp_readfn(PUB(smap)[PUB(sockmax)]));
  } else {
    req = PUB(smap)[PUB(sockmax)]->cp_wreq;
    return (req->rq_rpi->cq_adv(PUB(smap)[PUB(sockmax)], 
                                req));
  }
}


/*
 *      _crtcp_proc_read_env
 *
 *      Function:       - read envelope from process
 *                      - if full envelope read in then try to match with
 *                        and advance a receiving request
 *      Accepts:        - process
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_proc_read_env(struct lam_ssi_rpi_proc *ps)
{
  int nread;

  if ((nread = sread(ps, &ps->cp_envbuf, &ps->cp_nenvin, 0)) <= 0)
    return (nread);

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
    return (crtcp_match_adv(ps));
  } else {
    return (0);
  }
}


/*
 *      crtcp_proc_read_body
 *
 *      Function:       - read the body of an MPI message from a process
 *                      - this is only called when there is a receiving request
 *      Accepts:        - process
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_proc_read_body(struct lam_ssi_rpi_proc *ps)
{
  int nread;

  if ((nread = sread(ps, &ps->cp_msgbuf, &ps->cp_nmsgin, 0)) <= 0)
    return (nread);

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
      ps->cp_readfn = crtcp_proc_read_extra;
      return (crtcp_proc_read_extra(ps));
    } else
      ps->cp_readfn = lam_ssi_rpi_crtcp_proc_read_env;
  }

  return (0);
}


/*
 *      crtcp_proc_read_buffer
 *
 *      Function:       - read the body of an MPI message from a process
 *                      - this is called when we are buffering the message
 *      Accepts:        - process
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_proc_read_buffer(struct lam_ssi_rpi_proc *ps)
{
  int nread;

  if ((nread = sread(ps, &ps->cp_msgbuf, &ps->cp_nmsgin, 0)) <= 0)
    return (nread);

  if (ps->cp_nmsgin == 0) {
    /*
     * All of the message has been buffered. Set the process up to read the
     * next incoming envelope.  
     */
    ps->cp_readfn = lam_ssi_rpi_crtcp_proc_read_env;
    ps->cp_bmsg->cm_proc = 0;
  } 

  return (0);
}


/*
 *      crtcp_proc_read_extra
 *
 *      Function:       - read truncated data from a process into a sink
 *      Accepts:        - process
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_proc_read_extra(struct lam_ssi_rpi_proc *ps)
{
  char sink[512];
  int nread;
  char *bogus;

  do {
    bogus = sink;
    nread = sread(ps, &bogus, &ps->cp_extra, sizeof(sink));
    if (nread <= 0)
      return (nread);
    ps->cp_extra -= nread;
  } while (ps->cp_extra > 0);
  /*
   * Set process up to read the next incoming envelope.
   */
  ps->cp_readfn = lam_ssi_rpi_crtcp_proc_read_env;
  return (0);
}


/*
 *      crtcp_req_send_ack_long
 *
 *      Function:       - send ack and prepare to receive message body
 *                      - long protocol
 *      Accepts:        - source process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_send_ack_long(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  /* Special case: if this is the app handler thread, we cannot do a
     write here (because all writes have been suspended while we are
     quiescing the network).  So queue up a copy of the envelope on
     the to_be_sent_after_quiesce queue, and then return -- just as if
     it had been sent. */

  if (LAM_SSI_CRMPI_BASE_HANDLER_STATE_RUNNING == 
      lam_ssi_crmpi_base_handler_state) {
    if (enqueue_env(ps, req))
      return(LAMERROR);
    return 0;
  }

  /* Otherwise, we're just the normal app coming through here sending
     an ACK for a long */
  
  if ((nbytes = crtcp_push_env(ps, req)) <= 0) {
    return (nbytes);
  }
  
  if (req->rq_rpi->cq_nenvout == 0) {
    /*
     * The acknowledgment has been sent.
     */
    PUB(haveadv) = 1;
    ps->cp_wreq = 0;
    /*
     * Prepare to receive the message body.
     */
    req->rq_rpi->cq_state = C2CREAD;
    req->rq_rpi->cq_env.ce_flags &= ~C2CACK;
    req->rq_rpi->cq_env.ce_flags |= C2C2ND;
    req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
    req->rq_rpi->cq_adv = crtcp_req_rcvd_2nd;
  }

  return (0);
}


/*
 *      crtcp_req_send_ack_only
 *
 *      Function:       - send an acknowledgment
 *      Accepts:        - source process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_send_ack_only(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  if ((nbytes = crtcp_push_env(ps, req)) <= 0) {
    return (nbytes);
  }

  if (req->rq_rpi->cq_nenvout == 0) {
    /*
     * The acknowledgment has been sent.
     */
    PUB(haveadv) = 1;
    ps->cp_wreq = 0;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (0);
}


/*
 *      crtcp_req_send_body
 *
 *      Function:       - send message body
 *      Accepts:        - destination process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_send_body(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;

  if ((nbytes = crtcp_push_body(ps, req)) <= 0) {
    return (nbytes);
  }

  if (req->rq_rpi->cq_nmsgout == 0) {
    /*
     * All of the message has been written.
     */
    PUB(haveadv) = 1;
    ps->cp_wreq = 0;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (0);
}


/*
 *      _crtcp_req_send_long
 *
 *      Function:       - send the first envelope of a long message and
 *                        prepare to receive the ack
 *      Accepts:        - destination process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_req_send_long(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes; 

  if ((nbytes = crtcp_push_env(ps, req)) <= 0) {
    return (nbytes);
  }

  req->rq_state = LAM_RQSACTIVE;

  if (req->rq_rpi->cq_nenvout == 0) {
    /*
     * Prepare to read long protocol ack.
     */
    PUB(haveadv) = 1;
    ps->cp_wreq = 0;
    req->rq_rpi->cq_state = C2CREAD;
    req->rq_rpi->cq_env.ce_flags |= C2CACK;
    req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
    req->rq_rpi->cq_adv = crtcp_req_rcvd_ack_long;
  }

  return (0);
}


/*
 *      crtcp_req_send_short
 *
 *      Function:       - send a short protocol message
 *      Accepts:        - destination process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_req_send_short(struct lam_ssi_rpi_proc *ps,
                               MPI_Request req)
{
  int nbytes;
  
  if ((nbytes = crtcp_push_body(ps, req)) <= 0) {
    return (nbytes);
  }
  
  req->rq_state = LAM_RQSACTIVE;

  if (req->rq_rpi->cq_nenvout == 0 && req->rq_rpi->cq_nmsgout == 0) {
    PUB(haveadv) = 1;
    ps->cp_wreq = 0;
    req->rq_state = LAM_RQSDONE;
    lam_rq_nactv--;
  }

  return (0);
}


/*
 *      crtcp_req_send_synch
 *
 *      Function:       - send short synchronous protocol message body
 *                        and prepare to read the ack
 *      Accepts:        - destination process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_req_send_synch(struct lam_ssi_rpi_proc *ps,
                               MPI_Request req)
{
  int nbytes;

  if ((nbytes = crtcp_push_body(ps, req)) <= 0) {
    return (nbytes);
  }
  /*
   * The message has been sent.  Prepare to read the acknowledgment.
   */
  req->rq_state = LAM_RQSACTIVE;

  if (req->rq_rpi->cq_nenvout == 0 && req->rq_rpi->cq_nmsgout == 0) {
    PUB(haveadv) = 1;
    ps->cp_wreq = 0;
    req->rq_rpi->cq_state = C2CREAD;
    req->rq_rpi->cq_env.ce_flags |= C2CACK;
    req->rq_rpi->cq_env.ce_rank = req->rq_rpi->cq_peer;
    req->rq_rpi->cq_adv = crtcp_req_done;
  }

  return (0);
}


/*
 *      crtcp_req_rcvd_2nd
 *
 *      Function:       - read the body of a long protocol message
 *      Accepts:        - source process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_rcvd_2nd(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  ps->cp_rreq = req;
  ps->cp_msgbuf = req->rq_packbuf;
  ps->cp_nmsgin = ps->cp_env.ce_len;
  ps->cp_readfn = crtcp_proc_read_body;
  req->rq_rpi->cq_adv = crtcp_req_done;

  return (crtcp_proc_read_body(ps));
}


/*
 *      crtcp_req_done
 *
 *      Function:       - mark request as done
 *      Accepts:        - source process (ignored)
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_done(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  PUB(haveadv) = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  return (0);
}


/*
 *      crtcp_req_rcvd_body_synch
 *
 *      Function:       - prepare to send an ack
 *                      - short synchronous protocol
 *      Accepts:        - source process (ignored)
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_rcvd_body_synch(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  PUB(haveadv) = 1;
  req->rq_rpi->cq_state = C2CWRITE;
  req->rq_rpi->cq_env.ce_flags |= C2CACK;
  req->rq_rpi->cq_env.ce_rank = req->rq_comm->c_group->g_myrank;

  /* These are not used; zero them out for purify clean */

  LAM_ZERO_ME(req->rq_rpi->cq_env.ce_len);
  LAM_ZERO_ME(req->rq_rpi->cq_env.ce_seq);

  req->rq_rpi->cq_adv = crtcp_req_send_ack_only;
  lam_set_out_envelope_m(req->rq_rpi);

  return (0);
}


/*
 *      crtcp_req_rcvd_ack_long
 *
 *      Function:       - long protocol transition from reading ack to
 *                        done (if receiver wants 0 bytes) or sending
 *                        requested # of bytes
 *      Accepts:        - destination process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_req_rcvd_ack_long(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  PUB(haveadv) = 1;
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
    req->rq_rpi->cq_adv = crtcp_req_send_body;
    lam_set_out_envelope_m(req->rq_rpi);
  }

  return (0);
}


/*
 *      _crtcp_req_probe
 *
 *      Function:       - mark probe request as done
 *                      - the matched incoming envelope/message is buffered
 *      Accepts:        - source process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_req_probe(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  PUB(haveadv) = 1;
  req->rq_state = LAM_RQSDONE;
  lam_rq_nactv--;

  lam_ssi_rpi_fill_mpi_status(req, ps->cp_env.ce_rank,
                              ps->cp_env.ce_tag, ps->cp_env.ce_len);

  return (crtcp_buffer(ps));
}


/*
 *      crtcp_req_recv
 *
 *      Function:       - determine protocol for receive request on
 *                        matched incoming envelope and act upon it
 *                      - this is called for the first packet only
 *      Accepts:        - source process
 *                      - request
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_req_recv(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  struct lam_ssi_rpi_envl *env; /* the envelope */
  int extra;                    /* # bytes to go into sink */

  req->rq_state = LAM_RQSACTIVE;
  env = &ps->cp_env;
  lam_ssi_rpi_crtcp_fill_wildcards(req, env);

  if (env->ce_flags & C2CLONG) {
    /*
     * Got a long protocol envelope.
     * Check for message length mismatch, set status and reply with an ack.
     */
    PUB(haveadv) = 1;
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
      (env->ce_len > 0) ? crtcp_req_send_ack_long : crtcp_req_send_ack_only;
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
      PUB(haveadv) = 1;
      if (env->ce_flags & C2CSSEND) {
        if (crtcp_req_rcvd_body_synch((struct lam_ssi_rpi_proc *) 0, req)) {
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
      ps->cp_readfn = crtcp_proc_read_body;
      req->rq_rpi->cq_adv = (env->ce_flags & C2CSSEND) ?
        crtcp_req_rcvd_body_synch : crtcp_req_done;

      return (crtcp_proc_read_body(ps));
    }
  }

  return (0);
}


/*
 *      _crtcp_buffered_adv
 *
 *      Function:       - determine protocol for receive request matching
 *                        a buffered envelope/message and act upon it
 *      Accepts:        - request
 *                      - buffered envelope/message
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_buffered_adv(MPI_Request req,
                             struct lam_ssi_rpi_cbuf_msg *msg)
{
  struct lam_ssi_rpi_envl *env; /* matching incoming env. */
  int len;                      /* message length */
  int extra;                    /* # bytes to go into sink */
  int nread;                    /* # bytes read */

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
      (env->ce_len > 0) ? crtcp_req_send_ack_long : crtcp_req_send_ack_only;
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
        if (crtcp_req_rcvd_body_synch((struct lam_ssi_rpi_proc *) 0, req))
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
        msg->cm_proc->p_rpi->cp_readfn = crtcp_proc_read_body;
        req->rq_state = LAM_RQSACTIVE;
        req->rq_rpi->cq_adv = (env->ce_flags & C2CSSEND) ?
          crtcp_req_rcvd_body_synch : crtcp_req_done;
      } else {
        /*
         * The whole message has been read. Complete the request and set the
         * process read state to read the remaining bytes into the sink.
         */
        if (env->ce_flags & C2CSSEND) {
          req->rq_state = LAM_RQSACTIVE;
          if (crtcp_req_rcvd_body_synch((struct lam_ssi_rpi_proc *) 0, req)) {
            return (LAMERROR);
          }
        } else {
          req->rq_state = LAM_RQSDONE;
          lam_rq_nactv--;
        }
        msg->cm_proc->p_rpi->cp_extra = extra - (nread - len);
        msg->cm_proc->p_rpi->cp_readfn = crtcp_proc_read_extra;
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
 *      crtcp_match_adv
 *
 *      Function:       - match an envelope read from a process with a read
 *                        request if possible and advance the matched request
 *                      - if no match is found then the env/msg is buffered
 *      Accepts:        - envelope's source process
 */
static int
crtcp_match_adv(struct lam_ssi_rpi_proc *ps)
{
  MPI_Request req;              /* request */
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

  return (crtcp_buffer(ps));
}


/*
 *      crtcp_buffer
 *
 *      Function:       - buffer incoming envelope/message
 *                      - there is never any data to be read to the
 *                        data sink when buffering
 *      Accepts:        - process envelope came in from
 *      Returns:        - 0 or LAMERROR
 */
static int
crtcp_buffer(struct lam_ssi_rpi_proc *ps)
{
  struct lam_ssi_rpi_cbuf_msg msg;      /* buffer list entry */

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
    ps->cp_readfn = crtcp_proc_read_buffer;
    ps->cp_bmsg = lam_ssi_rpi_cbuf_append(&msg);

    if (!ps->cp_bmsg)
      return (LAMERROR);

    return (crtcp_proc_read_buffer(ps));
  } else {
    msg.cm_buf = 0;
    msg.cm_proc = 0;
    return (lam_ssi_rpi_cbuf_append(&msg) ? 0 : LAMERROR);
  }
}


/*
 *      crtcp_push_body
 *
 *      Function:       - push request envelope and message body down the pike
 *      Accepts:        - process
 *                      - request
 *      Returns:        - # bytes written or LAMERROR
 */
static int
crtcp_push_body(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  struct iovec iov[2];          /* IO vector */
  int nbytes;                   /* # bytes written */
  int msgbytes;                 /* # bytes msg body written */

  if (req->rq_rpi->cq_nenvout > 0 && req->rq_rpi->cq_nmsgout > 0) {
    iov[0].iov_base = (caddr_t) req->rq_rpi->cq_envbuf;
    iov[0].iov_len = req->rq_rpi->cq_nenvout;

    iov[1].iov_base = (caddr_t) req->rq_rpi->cq_msgbuf;
    iov[1].iov_len = req->rq_rpi->cq_nmsgout;

    nbytes = swritev(ps, iov, 2);

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
    nbytes = swrite(ps, req->rq_rpi->cq_msgbuf, req->rq_rpi->cq_nmsgout);
    if (nbytes > 0) {
      req->rq_rpi->cq_msgbuf += nbytes;
      req->rq_rpi->cq_nmsgout -= nbytes;
    }
  } else {
    nbytes = swrite(ps, req->rq_rpi->cq_envbuf, req->rq_rpi->cq_nenvout);
    if (nbytes > 0) {
      req->rq_rpi->cq_envbuf += nbytes;
      req->rq_rpi->cq_nenvout -= nbytes;
    }
  }

  return (nbytes);
}


/*
 *      crtcp_push_env
 *
 *      Function:       - push request envelope down the pike
 *      Accepts:        - process
 *                      - request
 *      Returns:        - # bytes written or LAMERROR
 */
static int
crtcp_push_env(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  int nbytes;                   /* # bytes written */

  nbytes = swrite(ps, req->rq_rpi->cq_envbuf, req->rq_rpi->cq_nenvout);
  if (nbytes > 0) {
    req->rq_rpi->cq_envbuf += nbytes;
    req->rq_rpi->cq_nenvout -= nbytes;
  }

  return (nbytes);
}


/*
 *      sread
 *
 *      Function:       - atomic socket read()
 *      Returns:        - # bytes read or LAMERROR
 */
static int
sread(struct lam_ssi_rpi_proc *ps, char **buf, int *nbytes, int maxbytes)
{
  int nread = 0;                /* # of bytes read */
  int r;                        /* read syscall return val */
  int num;
  double starttime = 0.0;       /* time blocking starts */

#if 0
  /* if is_fast is true, set PUB(is_sread_in_fast). */
  if (is_fast) {
    PUB(is_sread_in_fast) = 1;
    if (PUB(verbose) >= 1100)
      lam_debug(PUB(did), "sread: in _fast");
  }
#endif

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(ps->cp_sock, &PUB(block))) {
    starttime = ttime();
  }

  do {
    num = (maxbytes > 0) ? LAM_min(maxbytes, *nbytes) : *nbytes;
    r = read(ps->cp_sock, *buf, num);

    if (r < 0) {
      if (errno == EAGAIN) {
        break;
      } else if (errno != EINTR) {
        comminvalidate(ps->cp_sock);
        return (0);
      } else if (LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING 
                 == lam_ssi_crmpi_base_handler_state) {
        if (0 != sread_app_suspend(ps)) {
          /* 
           * we reach this point only if the sread was called from the
           * state-machine. sread_app_suspend always returns 0 if we are in _fast.
           */
          return 0;
        }
      }
    } else if ((0 == r) && (*nbytes > 0)) {     /* eof */
      if (FD_ISSET(ps->cp_sock, &PUB(eoferr))) {
        comminvalidate(ps->cp_sock);
      }
      return 0;
    } else {
      ps->cp_in_nbytes += r;
      nread += r;
      *buf += r;
      *nbytes -= r;
#if 0
      if (PUB(is_sread_in_fast) && 
          PUB(read_and_yield_back) && 
          (ps->cp_in_nbytes == 
           bookmarks[ps->cp_proc->p_gps.gps_grank].bp_out_nbytes)) {
        /*
         * in _fast, and all the messages have been quiesced. so yield back to
         * the cr_handler so that it can proceed.
         */
        if (PUB(verbose) >=40) {
          lam_debug(PUB(did), 
		    "_fast yielding to handler after quiescing from %d",
		    ps->cp_proc->p_gps.gps_grank);
        }
        lam_ssi_rpi_crtcp_yield_to_cr_handler();
      }
#endif
    }
  } while (*nbytes > 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(ps->cp_sock, &PUB(block))) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  FD_CLR(ps->cp_sock, &PUB(eoferr));

#if 0
  /* reset PUB(is_sread_in_fast), if it is set. */
  if (PUB(is_sread_in_fast))
    PUB(is_sread_in_fast) = 0;
#endif

  return (nread);
}

/*
 *      swrite
 *
 *      Function:       - atomic socket write()
 *      Returns:        - # bytes written or 0
 */
static int
swrite(struct lam_ssi_rpi_proc *ps, char *buf, int nbytes)
{
  int nwritten = 0;             /* # of bytes written */
  int r;                        /* write syscall return val */
  double starttime = 0.0;       /* time blocking starts */


  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(ps->cp_sock, &PUB(block))) {
    starttime = ttime();
  }

  do {
    r = write(ps->cp_sock, buf, nbytes);

    if (r < 0) {
      if (errno == EAGAIN) {
        break;
      } else if (errno != EINTR) {
        comminvalidate(ps->cp_sock);
        nwritten = 0;
        break;
      } else if (LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING 
                 == lam_ssi_crmpi_base_handler_state) {
        swrite_app_suspend(ps);
        /* 
         * Don't need to do anything special upon return from a
         * checkpoint -- writes will never progress during a quiesce 
         */
      }
    } else if ((r == 0) && (nbytes > 0)) {      /* eof */
      comminvalidate(ps->cp_sock);
      nwritten = 0;
      break;
    } else {
      ps->cp_out_nbytes += r;
      nwritten += r;
      buf += r;
      nbytes -= r;
    }
  } while (nbytes > 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(ps->cp_sock, &PUB(block))) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  return (nwritten);
}


/*
 *      swritev
 *
 *      Function:       - atomic socket writev()
 *      Returns:        - #bytes written or 0
 */
static int
swritev(struct lam_ssi_rpi_proc *ps, struct iovec *iov, int iovcnt)
{
  int nwritten = 0;             /* # of bytes written */
  int r;                        /* writev syscall return val */
  int savelen = 0;              /* save full length */
  char *savebase = 0;           /* save original base ptr */
  double starttime = 0.0;       /* time blocking starts */

  if (iovcnt > 0) {
    savelen = iov->iov_len;
    savebase = iov->iov_base;
  }

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON)
      && FD_ISSET(ps->cp_sock, &PUB(block))) {
    starttime = ttime();
  }

  do {
    r = writev(ps->cp_sock, iov, iovcnt);

    if (r < 0) {
      if (errno == EAGAIN) { 
        break;
      } else if (errno != EINTR) {
        iov->iov_len = savelen;
        iov->iov_base = savebase;
        comminvalidate(ps->cp_sock);
        nwritten = 0;
        break;
      } else if (LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING 
                 == lam_ssi_crmpi_base_handler_state) {
        swrite_app_suspend(ps);
        /* 
         * Don't need to do anything special upon return from a
         * checkpoint -- writes will never progress during a quiesce 
         */
      }
    } else if (r == 0) {               /* eof */
      iov->iov_len = savelen;
      iov->iov_base = savebase;

      while ((iovcnt > 0) && (iov->iov_len == 0)) {
        iov++;
        iovcnt--;
      }

      if (iovcnt > 0) {
        comminvalidate(ps->cp_sock);
        nwritten = 0;
      }

      break;
    } else {
      ps->cp_out_nbytes += r;
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
      && FD_ISSET(ps->cp_sock, &PUB(block))) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  return (nwritten);
}


/*
 *      sselect
 *
 *      Function:       - atomic socket select()
 *      Returns:        - # ready descriptors
 *                        or LAMERROR
 */
static int
sselect(int width, fd_set * readfds, fd_set * writefds, fd_set * exceptfds,
        struct timeval *timeout)
{
  int nready;                   /* number ready descriptors */
  double starttime = 0.0;       /* time blocking starts */

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON) && (timeout == 0)) {
    starttime = ttime();
  }

  do {
    nready = select(width, readfds, writefds, exceptfds, timeout);

    if (nready < 0) {
      if(errno != EINTR) {
        return (LAMERROR);
      } else if (LAM_SSI_CRMPI_BASE_HANDLER_STATE_WAITING 
                 == lam_ssi_crmpi_base_handler_state) {
        sselect_app_suspend();
        /* return 0 so the advance() can get us to the proper state */
        return(0);
      }
    }
  } while (nready < 0);

  if (((_kio.ki_rtf & RTF_TRON) == RTF_TRON) && (timeout == 0)) {
    _kio.ki_blktime += (ttime() - starttime);
  }

  return (nready);
}


/*
 *      lam_ssi_rpi_crtcp_setsockblk
 *
 *      Function:       - set socket to block/non-block mode
 *      Accepts:        - socket descriptor
 *                      - blocking flag
 *      Returns:        - 0 or LAMERROR
 */
int
lam_ssi_rpi_crtcp_setsockblk(int sock, int fl_block)
{
  if (fl_block) {
    /*
     * Switch socket to blocking if needed.
     */
    if (!FD_ISSET(sock, &PUB(block))) {

#if LAM_HAVE_FIONBIO
      int flag = 0;
      if (ioctl(sock, FIONBIO, &flag) == -1)
        return (LAMERROR);
#else
      if (fcntl(sock, F_SETFL, 0) == -1)
        return (LAMERROR);
#endif

      FD_SET(sock, &PUB(block));
    }
  } else {
    /*
     * Switch socket to non-blocking if needed.
     */
    if (FD_ISSET(sock, &PUB(block))) {

#if LAM_HAVE_FIONBIO
      int flag = 1;
      if (ioctl(sock, FIONBIO, &flag) == -1)
        return (LAMERROR);
#else
      if (fcntl(sock, F_SETFL, O_NONBLOCK))
        return (LAMERROR);
#endif

      FD_CLR(sock, &PUB(block));
    }
  }

  return (0);
}


/*
 *      comminvalidate
 *
 *      Function:       - invalidate communicators with processes on the node
 *                        a bad socket is connected to
 *      Accepts:        - bad socket descriptor
 */
static void
comminvalidate(int sock)
{
  int i;

  FD_CLR(sock, &PUB(read));
  FD_CLR(sock, &PUB(write));
  FD_CLR(sock, &PUB(except));

  for (i = 0; i < PUB(nprocs); ++i) {
    if ((PUB(procs)[i])->p_rpi->cp_sock == sock) {
      lam_ssi_rpi_commfault((PUB(procs)[i])->p_gps.gps_node);

      if ((PUB(procs)[i])->p_rpi->cp_rreq)
        (void) lam_ssi_rpi_comm_dead_m((PUB(procs)[i])->p_rpi->cp_rreq);

      if ((PUB(procs)[i])->p_rpi->cp_wreq)
        (void) lam_ssi_rpi_comm_dead_m((PUB(procs)[i])->p_rpi->cp_wreq);
      PUB(haveadv) = 1;
    }
  }
}


/*
 * The routines below implement the progress engine short circuit for
 * blocking send/receive requests when there are NO outstanding
 * requests.  Yes they are long, verbose and duplicate code found in
 * other functions but they are written this way for speed.
 */

#if CRTCP_DO_FAST
/*
 *      lam_ssi_rpi_crtcp_low_fastsend
 *
 *      Function:       - fast blocking send to a process
 *                      - short circuits the usual request progression
 *                        mechanism and only be called if there are NO
 *                        active requests
 *      Accepts:        - buffer to send
 *                      - size of buffer
 *                      - destination process
 *                      - message tag
 *                      - message communicator
 *      Returns:        - MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_crtcp_low_fastsend(char *buf, int size, struct lam_ssi_rpi_proc *ps,
                               int dest, int tag, MPI_Comm comm)
{
  struct iovec iov[2];          /* IO vector */
  struct lam_ssi_rpi_envl env;
  int err, nbytes;

  /*
   * set is_fast when we enter this function, so that if the cr_handler
   * interrupts sread(), it will in turn set PUB(is_sread_in_fast), so that the
   * cr_handler knows that it needs to call fast, instead of trying to advance
   * the state machine. 
   */
  is_fast = 1;

  /*
   * All communication is done in a blocking manner.  Hence EOF is
   * considered to be an error.  
   */
  if (lam_ssi_rpi_crtcp_setsockblk(ps->cp_sock, TRUE)) {
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

  if (size <= PUB(short)) {
    /*
     * Short message.  Send envelope and message body.
     */
    env.ce_flags = 0;

    if (!lam_homog) {
      mltoti4((int4 *) & env,
              sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));
    }

    if (size == 0) {
      if ((nbytes = swrite(ps, (char *) &env,
                          sizeof(struct lam_ssi_rpi_envl))) <= 0) {
        return (badsockerr(comm));
      }
    } else {
      iov[0].iov_base = (caddr_t) & env;
      iov[0].iov_len = sizeof(struct lam_ssi_rpi_envl);

      iov[1].iov_base = (caddr_t) buf;
      iov[1].iov_len = size;

      if ((nbytes = swritev(ps, iov, 2)) <= 0) {
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

    if ((nbytes = swrite(ps, (char *) &env, 
                         sizeof(struct lam_ssi_rpi_envl))) <= 0) {
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

    if ((nbytes = swritev(ps, iov, 2)) <= 0) {
      return (badsockerr(comm));
    }
  }

  /* reset is_fast befre returning */
  is_fast = 0;

  return (MPI_SUCCESS);
}


/*
 *      lam_ssi_rpi_crtcp_low_fastrecv
 *
 *      Function:       - fast blocking receive from a process
 *                      - short circuits the usual request progression
 *                        mechanism and only be called if there are NO
 *                        active requests
 *      Accepts:        - buffer to receive into
 *                      - size of buffer (inout)
 *                      - source process
 *                      - message tag (inout)
 *                      - message communicator
 *                      - message sequence number (out)
 *      Returns:        - MPI_SUCCESS or error code
 */
int
lam_ssi_rpi_crtcp_low_fastrecv(char *buf, int *size, 
                             struct lam_ssi_rpi_proc *ps,
                             int src, int *tag, MPI_Comm comm, int *seqnum)
{
  struct lam_ssi_rpi_envl env;      /* holds an envelope */
  struct lam_ssi_rpi_envl *inenv;   /* proc's incoming envelope */
  struct lam_ssi_rpi_cbuf_msg *bmsg;/* buffered message */
  int msglen;
  int synch;                        /* synchronous send? */
  int err, nbytes;
  int tmplen;
  char *tmpbuf;

  if (PUB(verbose) >= 1100)
    lam_debug(PUB(did), "Inside _fastrecv");
  /* 
   * set is_fast when we enter this function, so that sread() will in turn set 
   * PUB(is_sread_in_fast). So, if the cr_handler insterrupts sread(), it knows 
   * that it needs to call quiesce_advance_fast(). 
   */
  is_fast = 1;

  /*
   * All communication is done in a blocking manner and EOF is considered
   * to be an error.  
   */
  if (lam_ssi_rpi_crtcp_setsockblk(ps->cp_sock, TRUE))
    return (LAMERROR);

  FD_SET(ps->cp_sock, &PUB(eoferr));
  /*
   * If currently busy doing a buffered receive on the socket complete it.  
   */
  if (ps->cp_readfn == crtcp_proc_read_buffer) {
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

    FD_SET(ps->cp_sock, &PUB(eoferr));
    tmplen = sizeof(struct lam_ssi_rpi_envl);
    tmpbuf = (char *) inenv;
    if ((nbytes = sread(ps, &tmpbuf, &tmplen, 0)) <= 0) {
      return (badsockerr(comm));
    } 

    if (!lam_homog)
      mttoli4((int4 *) inenv,
              sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

    if (lam_ssi_rpi_envl_cmp(inenv, &env)) {
      if (crtcp_buffer(ps))
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

    if (swrite(ps, (char *) &env, sizeof(struct lam_ssi_rpi_envl)) <= 0)
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

        FD_SET(ps->cp_sock, &PUB(eoferr));
        tmplen = sizeof(struct lam_ssi_rpi_envl);
        tmpbuf = (char *) inenv;
        if ((nbytes = sread(ps, &tmpbuf, &tmplen, 0)) <= 0) {
          return (badsockerr(comm));
        } 

        if (!lam_homog)
          mttoli4((int4 *) inenv,
                  sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

        if (lam_ssi_rpi_envl_cmp(inenv, &env)) {
          if (crtcp_buffer(ps))
            return (badsockerr(comm));
        } else
          break;
      }
      /*
       * Receive the message body.
       */
      FD_SET(ps->cp_sock, &PUB(eoferr));
      if ((nbytes = sread(ps, &buf, &msglen, 0)) <= 0) {
        return (badsockerr(comm));
      }
    }
  } else {
    /*
     * Got a short protocol envelope.  Check for a synchronous send and
     * read the message body.
     */
    synch = inenv->ce_flags & C2CSSEND;

    if (*size > 0) {
      FD_SET(ps->cp_sock, &PUB(eoferr));
      if ((nbytes = sread(ps, &buf, &msglen, 0)) <= 0) {
        return (badsockerr(comm));
      } 
    }
    if (inenv->ce_len > *size) {
      if (sinksuck(ps, inenv->ce_len - *size) <= 0)
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

      if (swrite(ps, (char *) &env, sizeof(struct lam_ssi_rpi_envl)) <= 0)
        return (badsockerr(comm));
    }
  }

  /* 
   * If the cr_handler is waiting for the app_thread to yield after quiescing
   * the _fast messages, and bookmarks still don't match, then there must be
   * some unmatched message(s) on the wire that need to be buffered. So, we
   * yield to the cr_handler, so that it can buffer it in the proc.
   */
  if (PUB(read_and_yield_back) && 
      (ps->cp_in_nbytes != 
       bookmarks[ps->cp_proc->p_gps.gps_grank].bp_out_nbytes)) {
    if (PUB(verbose) >=40) {
      lam_debug(PUB(did), 
		"_fastrecv from %d complete, but bookmarks don't match." 
		"yielding to handler to buffer the pending messages...",
		ps->cp_proc->p_gps.gps_grank);
    }
    lam_ssi_rpi_crtcp_yield_to_cr_handler();
  }

  /* reset is_fast before we return */
  is_fast = 0;
  
  return (err);
}
#endif


/*
 *      lam_ssi_rpi_crtcp_fill_wildcards
 *
 *      Function:       - replace wildcards in request with matched values
 *                        and fill in the sequence number
 *      Accepts:        - request
 *                      - matched envelope
 */
void
lam_ssi_rpi_crtcp_fill_wildcards(MPI_Request req, struct lam_ssi_rpi_envl *env)
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


#if CRTCP_DO_FAST
/*
 *      waitforack
 *
 *      Function:       - wait for a long message acknowledgment
 *      Accepts:        - process sending the ack
 *                      - rank of process sending the ack
 *                      - ack tag
 *                      - ack communicator
 *                      - length ack'er will receive (out)
 *      Returns:        - MPI_SUCCESS or error code
 */
static int
waitforack(struct lam_ssi_rpi_proc *ps, int from, int tag, MPI_Comm comm,
           int *len)
{
  struct lam_ssi_rpi_envl env;
  struct lam_ssi_rpi_envl *inenv;
  int err, nbytes;
  int tmplen;
  char *tmpbuf;
  
  /*
   * If currently busy doing a buffered receive on the socket complete it.  
   */
  if (ps->cp_readfn == crtcp_proc_read_buffer) {
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

    FD_SET(ps->cp_sock, &PUB(eoferr));
    tmplen = sizeof(struct lam_ssi_rpi_envl);
    tmpbuf = (char *) inenv;
    if ((nbytes = sread(ps, &tmpbuf, &tmplen, 0)) <= 0) {
      return (badsockerr(comm));
    }
    if (!lam_homog)
      mttoli4((int4 *) inenv,
              sizeof(struct lam_ssi_rpi_envl) / sizeof(int4));

    if (lam_ssi_rpi_envl_cmp(inenv, &env)) {
      if (crtcp_buffer(ps))
        return (badsockerr(comm));
    } else
      break;
  }

  *len = inenv->ce_len;
  return (MPI_SUCCESS);
}


/*
 *      badsockerr
 *
 *      Function:       - get bad socket error code 
 *      Accepts:        - communicator socket is on
 *      Returns:        - error code
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
 *      sinksuck
 *
 *      Function:       - read truncated data from a socket into a sink
 *      Accepts:        - process to read from 
 *                      - length of data to read
 *      Returns:        - 1 (success) or 0 (eof) or -1 (error)
 */
static int
sinksuck(struct lam_ssi_rpi_proc *ps, int len)
{
  char sink[512];               /* data sink */
  int nread;
  char *bogus;

  do {
    bogus = sink;
    nread = sread(ps, &bogus, &len, sizeof(sink));
    if (nread <= 0)
      return (nread);
  } while (len > 0);

  return (1);
}
#endif


#if 0
/*
 * lam_ssi_rpi_crtcp_yield_to_cr_handler
 *
 * Function:            - yield to cr_thread when a checkpoint request comes
 *                        in. this function returns only at restart/continue 
 *                        time.
 */
void
lam_ssi_rpi_crtcp_yield_to_cr_handler(void)
{
  /*
   * yield to the cr_handler thread, and resume execution after the cr_handler
   * releases the lam_cr_lock.
   *
   * In _fast, we have to handle the case where the cr_thread might yield to the
   * app_thread to complete an unfinished read and then return control to the 
   * cr_thread again. 
   * So, to ensure strict alternation in execution between the app_thread and 
   * the cr_thread using pthread_mutex, we have to use three mutices.
   */
  lam_mutex_unlock(&lam_cr_mutex);
  lam_mutex_lock(&lam_yield_to_cr_mutex);
  lam_mutex_unlock(&lam_yield_to_app_mutex);
  lam_mutex_lock(&lam_cr_mutex);
  lam_mutex_unlock(&lam_yield_to_cr_mutex);
  lam_mutex_lock(&lam_yield_to_app_mutex);
}
#endif


/*
 * Queue up an ACK for a long message; to be sent when the message
 * resume from a checkpoint.
 */
static int
enqueue_env(struct lam_ssi_rpi_proc *ps, MPI_Request req)
{
  long_ack_q_entry_t *entry;

  if (PUB(verbose) >= 40)
    lam_debug(PUB(did), "Enqueueing long ACK to be sent at RESTART");
  entry = malloc(sizeof(*entry));
  if (NULL == entry) {
    return LAMERROR;
  }
  entry->laq_env = req->rq_rpi->cq_env;
  entry->laq_ps = ps;
  entry->laq_next = NULL;

  /* Is the list empty? */

  if (NULL == long_ack_q_head) {
    long_ack_q_head = long_ack_q_tail = entry;
  } 

  /* No -- it's not empty; add this entry to the tail */

  else {
    long_ack_q_tail->laq_next = entry;
    long_ack_q_tail = entry;
  }
  return 0;
}


/*
 * Send all the ACKs that were queued before the checkpoint (during
 * the quiesce)
 */
int 
lam_ssi_rpi_crtcp_send_long_ack_queue(void)
{
  long_ack_q_entry_t *entry;
  struct _req req_store;
  struct lam_ssi_rpi_req rpi_req;
  MPI_Request req = &req_store;
  int nbytes;

  /* Setup a dummy request that contains the envelope */
  
  req_store.rq_rpi = &rpi_req;
  rpi_req.cq_envbuf = (char*) &rpi_req.cq_env;
  rpi_req.cq_nenvout = sizeof(rpi_req.cq_env);

  /* Loop over all the queued ACKs, call push_env with the dummy request */

  for (entry = long_ack_q_head; entry != NULL; entry = entry->laq_next) {
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did), 
		"Sending queued long ACK envelope at continue/restart."); 
    rpi_req.cq_env = entry->laq_env;
    while ((nbytes = crtcp_push_env(entry->laq_ps, req)) >= 0)
      continue;

    if (nbytes < 0)
      return LAMERROR;
  }

  return 0;
}

/*
 * Yield from sread to the checkpoint/restart handler
 * Returns non-zero if the sread should be aborted.
 */
static int
sread_app_suspend(struct lam_ssi_rpi_proc *ps)
{
  unsigned int in_nbytes = ps->cp_in_nbytes;

  if (PUB(verbose) >= 40)
    lam_debug(PUB(did),
	      "sread yielding to handler: "
	      "current bookmark rank %d = %u",
	      ps->cp_proc->p_gps.gps_grank, ps->cp_in_nbytes);

  /* yield to cr and return */
  lam_ssi_crmpi.lscrma_app_suspend();

#if 0
  if (PUB(is_sread_in_fast)) {
    /*
     * we're in _fast. So two cases arise:
     * - the cr_handler has yielded to us to receive messages that are on the 
     *   wire, and then return control to it. 
     * - no messages need to be quiesced.
     * In both cases, we just allow sread() to go about business as usual. 
     */
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did), "_fast sread: returned from yield.");
  } else {
#endif
    /* 
     * we are in the state machine. 
     *
     * If we come back from the checkpoint and my bookmark to this process has
     * not changed, then we can continue (because the handler thread did not do
     * anything with this process).  Otherwise, the handler thread came through
     * and received some bytes from this process, and we need to exit out of
     * this state back up to the top-level advance() in order get back into the
     * correct state. 
     */
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did),
		"sread back from handler: "
		"current bookmark rank %d = %u",
		ps->cp_proc->p_gps.gps_grank,
		ps->cp_in_nbytes);

    if (ps->cp_in_nbytes != in_nbytes) {
      if (PUB(verbose) >= 40)
        lam_debug(PUB(did), "sread: something changed -- returning");
      return 1;
    }
    if (PUB(verbose) >= 40)
      lam_debug(PUB(did), "sread: nothing changed -- continuing");
#if 0
  } 
#endif

  return 0;
}

/*
 * Yield from swrite/swritev to the checkpoint/restart handler
 */
static void
swrite_app_suspend(struct lam_ssi_rpi_proc *ps)
{
  int sock_block;

  if (PUB(verbose) >= 40)
    lam_debug(PUB(did), 
	      "swrite/swritev yielding to handler: "
	      "current bookmark rank %d = %u",
	      ps->cp_proc->p_gps.gps_grank,
	      ps->cp_out_nbytes);

  /* Save the socket blocking/non-blocking state before yield, and
   * restore it after we return from yield. We do this because the C/R
   * handler might have modified the blocking state of the socket to
   * advance some read requests during quiesce.
   */
  sock_block = FD_ISSET(ps->cp_sock, &PUB(block));
          
  /* yield to cr and return */
  lam_ssi_crmpi.lscrma_app_suspend();

  /*
   * We need to force _setsockblk() to do the right thing. Since it
   * checks PUB(block), we need to set it appropriately
   */
  if (sock_block)
    FD_CLR(ps->cp_sock, &PUB(block));
  else
    FD_SET(ps->cp_sock, &PUB(block));
        
  lam_ssi_rpi_crtcp_setsockblk(ps->cp_sock, sock_block);

  if (PUB(verbose) >= 40)
    lam_debug(PUB(did), "swrite/swritev back from yield to handler");
          
}

/*
 * Yield from sselect to the checkpoint/restart handler
 */
static void
sselect_app_suspend(void)
{
  if (PUB(verbose) >= 40)
    lam_debug(PUB(did), "sselect yielding to handler");

  /* yield to cr and return */
  lam_ssi_crmpi.lscrma_app_suspend();

  if (PUB(verbose) >= 40)
    lam_debug(PUB(did), "sselect back from yield to handler");
}
