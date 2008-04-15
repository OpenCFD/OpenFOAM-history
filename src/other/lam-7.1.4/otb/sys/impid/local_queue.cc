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
 * $Id: local_queue.cc,v 1.3 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function: - queues up MPI messages to send to the local LAM
 *	rank.  If there are messages ahead of the message being queued
 *	(including long messages that have not finished being received
 *	from the remote host yet), queue it up for later sending.
 *	This is on a destination/cid/tag basis.  i.e., if the
 *	beginning of a long message for local rank 2, cid 3, tag 4
 *	arrives, and then a short message arrives for local rank 2,
 *	cid 17, tag 45, this short message will be allowed to pass.
 *	If, however, another message (short or long) arrives for local
 *	rank 2, cid 3, tag 4 before the first message is released from
 *	the queue, that message will not be sent until the first one
 *	completes (we don't even allow the ping to go if it's a long
 *	message).
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <iostream>
#include <map>
#include <list>

#include <impi.h>
#include <longbuf_mgmt.h>
#include <lamdebug-cc.h>
#include <impid-cc.h>

using std::list;
using std::map;
using std::endl;


/*
 * private variables
 */
struct entry {
  // For quick reference, put the drqid in the top level

  IMPI_Uint8 drqid;

  IMPI_Packet pk;
  char* buffer;
  MPI_Datatype type;
  IMPI_Packet *syncack;

  bool is_datasync_ping;
  bool ping_sent;
  bool is_long;
  longbuf_mgmt* longbuf;
};
typedef list<entry*> local_list_t;
typedef map<IMPI_Uint8, local_list_t> tag_map_t;
typedef map<IMPI_Uint8, tag_map_t> cid_map_t;
typedef map<int, cid_map_t> dest_map_t;
typedef map<int, dest_map_t> src_map_t;

static src_map_t local_queue;
static Debug debug(false);


/*
 * private functions
 */
static void progress_queue(local_list_t& q);


/*
 *	local_enqueue
 *
 *	Function:	- queue a data message to a local LAM rank
 *                      - must calculate local LAM rank number from message
 *                        header (pk_src)
 *                      - if no other message is in the middle of
 *                      being received (i.e., a long message that
 *                      started to be received before this message)
 *                      and not yet sent to the local LAM rank, call
 *                      local_req_send to actually send this message.
 *                      - if a long message that arrived before this
 *                      data message has not yet been completely
 *                      received (and therefore sent to the local LAM
 *                      rank), queue this message up so that we
 *                      preserve MPI's message ordering guarantee.
 *      Accepts:        - ptr to IMPI_Packet header of message
 *                      - ptr to data buffer
 *                      - ptr to a SYNCACK IMPI_Packet that will be sent 
 *                        when this message is actually received by the 
 *                        LAM rank (or 0 if there is no syncack)
 *                      - boolean indicating whether it is a ping or a
 *                        data message
 *      Returns:        - 0 on success, LAMERROR on failure
 */
int
local_enqueue(IMPI_Packet* pk, char* buffer, MPI_Datatype type, 
	      IMPI_Packet* syncack, bool is_datasync_ping, bool is_long)
{
  struct entry *e = 0;
  int src_rank = pk->pk_lsrank;
  int dest_rank = proc_resolver(&pk->pk_dest);
  int tag = (int) pk->pk_tag;
  int cid = (int) pk->pk_cid;

  // Since we can send oob messages on related CIDs, we'll just use
  // the base CID here.

  cid = (cid / 3) * 3;
  local_list_t &q = local_queue[src_rank][dest_rank][cid][tag];

  // First check to see if the queue for this rank/cid/tag is empty.
  // If it is, we can just send the message immediately.

  if (q.empty()) {
    debug << "local_enqueue: queue is empty, sending immediaty" << endl;
    int ret = local_req_send(pk, buffer, type, syncack);
    if (ret != 0)
      return ret;

    // If we just sent a ping for a long message, put it in the queue
    // and mark it as "waiting for the rest of the message to arrive
    // from the remote host" (although it's *really* waiting for the
    // ACK from the local LAM -- the rest of the message won't arrive
    // until that happens first :-)

    if (is_datasync_ping && is_long) {
      debug << "local_enqueue: this was a datasync ping, enqueued (drqid " 
	<< pk->pk_drqid << ", src " << src_rank << " dest " << dest_rank 
	    << " tag " << tag << " cid " << cid << ")" << endl;
      e = new entry;
      e->ping_sent = true;
    }
  }

  // Otherwise, there was something in the queue already.  Hence, this
  // needs to be queued up at the end.  SIDENOTE: local_enqueue() will
  // not be called with the remainder of a long message
  // (local_enqueue_finish_long()) will be -- hence, we *always* put
  // entries at the end of the queue if the queue is not empty.

  else {
    debug << "local_enqueue: queueing up the message (drqid " 
	  << pk->pk_drqid << ", src " << src_rank << " dest " << dest_rank 
	  << " tag " << tag << " cid " << cid << " msglen " 
	  << pk->pk_msglen << ")" << endl;
    e = new entry;
    e->ping_sent = false;
  }

  // Save a bit of typing, 'cause the two cases where we enqueue save
  // the same info except for the ping_sent field.

  if (e != 0) {
    debug << "local_enqeue: queueing up entry: drqid " << pk->pk_drqid << endl;
    e->drqid = pk->pk_drqid;
    e->pk = *pk;
    e->buffer = buffer;
    e->type = type;
    e->syncack = syncack;
    e->is_datasync_ping = is_datasync_ping;
    e->is_long = is_long;
    e->longbuf = 0;
    q.push_back(e);
  }

  return 0;
}



/*
 *	local_enqueue_finish_long
 *
 *	Function:	
 *                      - called with the data for a long message,
 *                        invoked when we receive the last IMPI_Packet
 *                        of a long message from a remote host
 *                      - i.e., the ping has already been queued (and
 *                        possibly sent already)
 *                      - this function finds the ping in the queue
 *                        and attaches the longbuf_mgmt to it.
 *                      - if the entry is at the head of the queue (in
 *                      which case the ping will have been sent
 *                      already), send the rest of the message (use
 *                      MPI_Send, 'cause we know that the local LAM
 *                      rank is already waiting to receivie it
 *                      (otherwise we wouldn't have sent the SYNCACK
 *                      to the remote IMPI host and gotten the rest of
 *                      the long message).  Also advance the queue
 *                      after that.
 *      Accepts:        - ptr to longbuf_mgmt
 *      Returns:        - 0 on success, LAMERROR on failure
 */
int 
local_enqueue_finish_long(longbuf_mgmt* longbuf)
{
  int ret = 0;
  IMPI_Packet *pk = longbuf->get_packet();
  int src_rank = pk->pk_lsrank;
  int dest_rank = proc_resolver(&pk->pk_dest);
  int tag = (int) pk->pk_tag;
  int cid = (int) pk->pk_cid;

  // Since we can send oob messages on related CIDs, we'll just use
  // the base CID here.

  cid = (cid / 3) * 3;
  local_list_t &q = local_queue[src_rank][dest_rank][cid][tag];
  local_list_t::iterator i;
  IMPI_Uint8 drqid = longbuf->get_packet()->pk_drqid;

  debug << "local_enqueue_finish_long: got rest of long message (drqid " 
	<< drqid << ", src " << src_rank << " dest " << dest_rank 
	<< " tag " << tag << " cid " << cid << ")" << endl;
  debug << "local_enqueue_finish_log: queue.empty(): " << q.empty() << endl;
  for (i = q.begin(); i != q.end(); ++i)
    if ((*i)->drqid == drqid)
      break;

  if (i == q.end()) {
    debug << "local_enqeue_finish_long didn't find the drqid in the list!" 
	  << endl;
    return LAMERROR;
  }

  // We found the entry in the list, so attach the longbuf to it

  (*i)->longbuf = longbuf;

  // Are we at the head of the queue?  If so, send it immediately,
  // dequeue this entry, and then try to progress the rest of the
  // queue.

  if (i == q.begin()) {
    debug << "local_enqueue_finish_long: long is at head of queue, sending" 
	  << endl;
    struct entry *e = (*i);
    IMPI_Packet *pk = longbuf->get_packet(); 

    //
    // ***** WARNING *****
    //
    // See the big warning in the local_req.cc.
    //
    
    int src_rank = (int) pk->pk_lsrank;
    int dest_rank = proc_resolver(&pk->pk_dest);
    MPI_Comm comm = comm_make(pk->pk_cid, src_rank,
			      dest_rank, 
			      proc_resolver(&(pk->pk_dest)));
    
    debug << "send_long()>>>>>> the pk_tag is " << (int) pk->pk_tag 
	  << " the pk_lsrank is " << src_rank
	  << " dest_rank is " << dest_rank
	  << " about to do an MPI_Send wiht the rest of the msg" << endl;
    debug << "send_long()>>>>>> proc_resolver of pk_src is " 
	  << proc_resolver(&pk->pk_src)
	  << " of pk_dest is " << proc_resolver(&pk->pk_dest) << endl;
    
    int ret = MPI_Send(longbuf->get_buffer(), longbuf->get_received(), 
		       MPI_BYTE, dest_rank, pk->pk_tag, comm); 
    debug << "send_long()>>>>>> MPI_Send is done" << endl;
    
    comm_free(comm);

    // All done; dequeue the front entry and try to progress the queue

    delete e;
    q.erase(q.begin());
    delete longbuf;
    if (ret == 0)
      progress_queue(q);
  }
  debug << "local_enque_finish_long: done" << endl;

  return ret;
}


//
// progress_queue
//
// This function is invoked with the assumption that it is acceptable
// to send the first entry in the queue to the local LAM rank.
//
static void
progress_queue(local_list_t& q)
{
  local_list_t::iterator i, j;

  // Loop through the entire queue

  debug << "local_progress_queue: progressing queue" << endl;
  for (i = q.begin(); i != q.end(); ) {
    debug << "local_progress_queue: got entry -- drqid \n" 
	  << (*i)->drqid << endl;
    local_req_send(&(*i)->pk, (*i)->buffer, (*i)->type, (*i)->syncack);

    // Was this a ping for a long message?  If so, we're done -- we
    // have to wait for the LAM rank to send an ACK, which will
    // trigger the remote host to send us the rest of the message, at
    // which point we'll call local_enqueue_finish_long() and finish
    // sending this long message.

    if ((*i)->is_datasync_ping && (*i)->is_long) {
      debug << "local_progress_queue: got a datasync ping; stopping" << endl;
      (*i)->ping_sent = true;
      break;
    }

    // Otherwise, erase this entry from the queue and keep going

    j = i;
    delete (*j);
    ++i;
    q.erase(j);
  }
  debug << "local_progress_queue: done" << endl;
}

#endif
