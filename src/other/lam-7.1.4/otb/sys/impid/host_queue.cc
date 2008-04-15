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
 * $Id: host_queue.cc,v 1.22 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- maintains outgoing IMPI_Packet queues for IMPI hosts
 *                      - can insert an IMPI_Packet to the end of a host's 
 *                        queue
 *                      - will also progress queues by sending packets to
 *                        hosts according to ackmark/hiwater flow control
 *                      - have to do some squirmy things here because the 
 *                        send_packet function may be interrupted by
 *                        intermittent reads (to ensure that we won't
 *                        block while reading from the socket)
 *                        
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

// IRIX MIPSpro 7.30 compilers require that <new> comes first... @#$@#$!!!
#if LAM_CXX_NOTHROW_NEW
#include <new>
#endif

#include <poll.h>
#include <stdio.h>

#include <iostream>
#include <list>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <packet_mgmt.h>
#include <laminternal.h>
#include <lamdebug-cc.h>

using std::list;
using std::cerr;
using std::endl;


//
// local typedef for our queue
//
// We use a list because we sometimes need to put a priority packet at
// the front of the queue.
// Queue contains packet_mgmt pointers.
//
typedef std::list<packet_mgmt*> host_packet_queue;


//
// private variables
//
static bool *sending_to = 0;
static struct pollfd sock;
static host_packet_queue* host_queue = 0;
static Debug debug(false);


//
// private functions
//
static int send_packet(IMPI_Packet* pk, char* buffer, IMPI_Uint4 host_rank,
		       bool& interrupted);


/*
 *	host_queue_init
 *
 *	Function:	- initialize the queues
 *			- initialize the sending_to array
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int 
host_queue_init()
{
#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
  host_queue = new LAM_CXX_NOTHROW_NEW_ARG host_packet_queue[nhosts];
  if (host_queue == 0)
    return (LAMERROR);

  sending_to = new LAM_CXX_NOTHROW_NEW_ARG bool[nhosts];
  if (sending_to == 0) {
    delete[] host_queue;
    return (LAMERROR);
  }
#else
  host_queue = 0;
  sending_to = 0;
  
  try {
    host_queue = new host_packet_queue[nhosts];
    sending_to = new bool[nhosts];
  } catch(...) {
    if (host_queue != 0)
      delete[] host_queue;
    return (LAMERROR);
  }
#endif

  for (IMPI_Uint4 i = 0; i < nhosts; i++)
    sending_to[i] = false;

  return 0;
}


/*
 *	host_enque_packet
 *
 *	Function:	- enques a packet and tries to progress the queue
 *                      - takes destination rank (relative to MPI_COMM_WORLD
 *                        and figure out which host to send to
 *                      - if we short circuit the queue and we get an 
 *                        unexpected read in the send_packet, be sure to 
 *                        advance the queue, because the unexpected read 
 *                        may have put more packets on the queue
 *
 *	Accepts:	- pointer to an IMPI_Packet
 *                      - rank of host to send to
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_enque_packet(IMPI_Packet* pk, char* buffer, int host_rank) 
{
  int ret = 0;
  bool interrupted;

  // Special case: if queue is empty and the flow control allows it,
  // send the packet immediately

  debug << "host_enque_packet sending_to: " << sending_to[host_rank] << endl;
  if (!sending_to[host_rank] && host_queue[host_rank].empty() && 
      fc_can_send(&pk->pk_src, &pk->pk_dest)) {
    debug << "host_enque_packet short circuit to host rank " << host_rank 
	  << endl;
    ret = send_packet(pk, buffer, (IMPI_Uint4) host_rank, interrupted);

    if (pk != 0)
      delete pk;
    if (buffer != 0)
      delete[] buffer;

    // If we were interrupted, there may be more things on the queue
    // that demand attention

    if (interrupted)
      host_progress_queue(host_rank);
  } else {
    debug << "host_enqueue_packet no short circuit to host rank " 
	  << host_rank << endl;
    packet_mgmt *pmgmt = new packet_mgmt(buffer, buffer, true, pk);

    // Special case: if this is a PROTOACK, put it at the head of the
    // queue, 'cause it needs to go out ASAP.

    IMPI_Uint4 type = pk->pk_type;
    LAM_SWAP_ENDIAN(&pk->pk_type, sizeof(pk->pk_type));

    if (type == IMPI_PK_PROTOACK) {
      debug << "host_enque_first_packet: special priority PROTOACK" << endl;
      host_queue[host_rank].push_front(pmgmt);
    } else
      host_queue[host_rank].push_back(pmgmt);

    // A copy of pk is saved in pmgmt, so delete the one that they
    // gave us

    delete pk;

    ret = host_progress_queue(host_rank);
  }

  return ret;
}


/*
 *	host_enque_first_packet
 *
 *	Function:	- enques first packet on list
 *                      - takes destination rank (relative to MPI_COMM_WORLD
 *                        and figure out which host to send to
 *                      - removes first packet from list after it is enqued
 *                      - if we short circuit the queue and we get an 
 *                        unexpected read in the send_packet, be sure to 
 *                        advance the queue, because the unexpected read 
 *                        may have put more packets on the queue
 *
 *	Accepts:	- pointer to a list of packet wrappers
 *                      - MPI rank (relative to MCW) of destination
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_enque_first_packet(packet_mgmt_list* plist, int world_dest_rank) 
{
  bool interrupted;
  int ret = 0;
  IMPI_Uint4 host_rank = lam_impi_procs[world_dest_rank].ip_hostrank;

  // Take the first item off the list

  packet_mgmt* pmgmt = plist->front();
  plist->pop_front();

  // Special case: if queue is empty and the flow control allows it,
  // send the packet immediately

  IMPI_Packet *pk = pmgmt->get_packet();
  debug << "host_enque_first_packet sending_to: " << sending_to[host_rank] 
	<< endl;
  if (!sending_to[host_rank] && host_queue[host_rank].empty() && 
      fc_can_send(&pk->pk_src, &pk->pk_dest)) {
    debug << "host_enque_first_packet: short circuit send" << endl;
    ret = send_packet(pk, pmgmt->get_buffer(), host_rank, interrupted);
    delete pmgmt;

    // If we were interrupted, there may be more things on the queue
    // that demand attention

    if (interrupted)
      host_progress_queue(host_rank);
  } else {
    debug << "host_enque_first_packet: queued up first packet" << endl;
    host_queue[host_rank].push_back(pmgmt);
    ret = host_progress_queue(host_rank);
  }

  return ret;
}


/*
 *	host_enque_all_packets
 *
 *	Function:	- enques all packets in a list
 *                      - takes destination rank (relative to MPI_COMM_WORLD
 *                        and figure out which host to send to
 *                      - empties list upon completion, but does not destroy 
 *                        it (to be symmetrical with host_enqueue_first_packet)
 *                      - if we short circuit the queue and we get an 
 *                        unexpected read in the send_packet, be sure to 
 *                        advance the queue, because the unexpected read 
 *                        may have put more packets on the queue
 *
 *	Accepts:	- pointer to a list of packet wrappers
 *                      - MPI rank (relative to MCW) of destination
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_enque_all_packets(packet_mgmt_list* plist, int host_rank) 
{
  int ret;
  IMPI_Packet *pk;
  packet_mgmt *pmgmt;
  bool queued = false;
  bool interrupted;

  // Special case: if queue is empty and the flow control allows it,
  // send the packet immediately.  Otherwise, queue it up.

  debug << "host_enque_all_packets: have " << plist->size() 
	<< " packets to queue (empty: " << plist->empty() 
	<< ")" << endl;
  while (!plist->empty()) {
    pmgmt = plist->front();
    pk = pmgmt->get_packet();
    plist->pop_front();

    debug << "Checking for send policy: size " << plist->size() 
	  << " sending_to " << sending_to[host_rank] << endl;
    if (!sending_to[host_rank] && !queued && host_queue[host_rank].empty() && 
	fc_can_send(&pk->pk_src, &pk->pk_dest)) {
      debug << "host_enque_all_packets:: doing quick send b/c que's empty ("
	    << plist->size() << ")" << endl;
      ret = send_packet(pk, pmgmt->get_buffer(), (IMPI_Uint4) host_rank, 
			interrupted);

      delete pmgmt;
      if (ret != 0)
	break;

    } else {
      debug << "host_enque_all_packets:: queuing it up (" << plist->size() 
	    << ")" << endl;
      host_queue[host_rank].push_back(pmgmt);
      queued = true;
    }
  }
  debug << "Finished queing up all packets: size now " << plist->size()
	<< endl;

  // If we queued anything up or were interrupted while direct sending
  // a single packet, try to progress the queue

  if (queued || interrupted)
    ret = host_progress_queue(host_rank);

  return ret;
}


/*
 *	host_progress_queue
 *
 *	Function:	- tries to progress a host queue according to IMPI 
 *                        flow control rules
 *
 *	Accepts:	- host rank to progress
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_progress_queue(int host_rank)
{
  packet_mgmt *pmgmt;
  IMPI_Packet *pk;
  bool interrupted;

  debug << "progress_queue for host rank " << host_rank 
	<< " apparently has " << host_queue[host_rank].size() << " messages"
	<< endl;
  while (!sending_to[host_rank] && !(host_queue[host_rank].empty())) {
    debug << "progress_queue queue not empty: size " 
	  << host_queue[host_rank].size() << endl;
    pmgmt = host_queue[host_rank].front();
    pk = pmgmt->get_packet();

    // If flow control says we can send, send it and delete it from
    // the queue

    if (fc_can_send(&pk->pk_src, &pk->pk_dest)) {
      host_queue[host_rank].pop_front();
      if (send_packet(pk, pmgmt->get_buffer(), (IMPI_Uint4) host_rank, 
		      interrupted) != 0)
	return (LAMERROR);

      delete pmgmt;
    } else
      break;
  }

  debug << "progress_queue returning" << endl;
  return 0;
}


/*
 *	host_queue_destroy
 *
 *	Function:	- destroys the queues
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int 
host_queue_destroy()
{
  if (host_queue != 0) {
    debug << "IMPI deleting host queue: " << host_queue->size() 
	  << " elements still left in queue" << endl;
    delete[] host_queue;
#if LAM_IMPID_DEBUG
    host_queue = 0;
#endif
  }

  if (sending_to != 0) {
    debug << "IMPI deleting sending_to" << endl;
    delete[] sending_to;
#if LAM_IMPID_DEBUG
    sending_to = 0;
#endif
  }

  return 0;
}


/*
 *	send_packet
 *
 *	Function:	- write a packet and its data down a socket
 *                      - only called if fc_can_send() said we could send
 *
 *	Accepts:	- ptr to IMPI_Packet
 *                      - ptr to data buffer (or 0)
 *                      - host rank to send to
 *                      - ref to interrupted flag
 *      Returns:        - 0 for success, LAMERROR on error
 *                      - sets interrupted flag if we do an unexpected read
 */
static int 
send_packet(IMPI_Packet* pk, char* buffer, IMPI_Uint4 host_rank, 
	    bool& interrupted)
{
  int i, err;
  Debug debug(false);

  // First check to see if we can read off the socket.  This helps
  // avoid deadlock.

  sending_to[host_rank] = true;
  sock.fd = lam_impi_hosts[host_rank].ih_fd;
  sock.events = POLLIN;

  // Need to put an upper limit here so that our packet eventually
  // will get out.  Put it at the ackmark for this proc pair.

  interrupted = false;
  for (i = 0, err = 1; err == 1 && i < impi_lam_ackmark; i++) {
    sock.revents = 0;
    err = poll(&sock, 1, 0);
    if (err < 0 && errno != EINTR) {
      cerr << "LAM IMPI host unexpectedly failed to poll host " << host_rank 
	   << "'s socket" << endl;
      impi_bail(1);
    }
    else if (err == 1) {
      debug << "*** IMPID doing unexpected read" << endl;
      interrupted = true;
      impi_host_recv(host_rank, &sock);
      debug << "*** IMPID back from unexpected read" << endl;
    }
  }

  // Now that we've assured that the socket should be writable, read
  // off the socket, try to send the packet

  ::debug << "IMPID progressing host queue for host rank " << host_rank
	  << " -- sending packet" << endl;
  if (host_rank >= 0 && host_rank < nhosts) {
    if (pk != 0) {

      /* Must re/unswap endian to get the length back -- it's already
         been set for network order.  JMS This is ripe for
         optimization... later */

      IMPI_Uint4 len = pk->pk_len;
      LAM_SWAP_ENDIAN(&len, sizeof(len));

      debug << "send_packet about to mwrite to host " << host_rank << endl;
      if (mwrite(lam_impi_hosts[host_rank].ih_fd, (char*) pk, sizeof(*pk)) != 
	  sizeof(*pk)) {
	perror("socket write");
	cerr << "LAM IMPI host was not able to send an entire IMPI_Packet down a socket to host "
	     << host_rank << endl;
	impi_bail(1);
      }

      // Send the data buffer if we have one

      if (buffer != 0 && len > 0) {
	debug << "send_packet about to mwrite data" << endl;
	if (((IMPI_Uint4) mwrite(lam_impi_hosts[host_rank].ih_fd, buffer, len))
	     != len) {
	  cerr << "LAM IMPI host was not able to send an entire data message down a socket to host " 
	       << host_rank << endl;
	  impi_bail(1);
	}
      }
      
      // Tell the flow control that we sent a packet

      debug << "send_packet tell fc we sent a packet" << endl;
      fc_sent_packet(pk);
    }
  }

  debug << "send_packet returning" << endl;
  sending_to[host_rank] = false;
  return 0;
}

#endif
