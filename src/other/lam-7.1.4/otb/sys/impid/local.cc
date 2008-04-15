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
 * $Id: local.cc,v 1.16 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- all actions triggered by messages coming in from 
 *                        local LAM
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <stdio.h>
#include <errno.h>

#include <iostream>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <lamdebug-cc.h>

#include <mpi.h>

using std::cerr;
using std::endl;


/*
 * private variables
 */
static Debug debug(false);
static union {
  IMPI_Uint8 srqid;
  IMPI_Lamgiappe lamgiappe;
} impid_buffer;
enum { 
  ALL_INDEX,
  REQUEST_MAX 
};
static MPI_Request requests[REQUEST_MAX];


/*
 * local functions
 */
static int local_receive();


/*
 *	impi_local_init
 *
 *	Function:	- setup persistent receives from local LAM ranks
 */ 
int
impi_local_init()
{
  int i;

  debug << "Call the mk_impi_dtype for the IMPID for the IMPI_LAMGIAPPE" 
	<< endl;
  if (IMPI_Make_dtype()) {
    debug << "Failure in making the IMPI_LAMIAPPE dtype in the IMPID" << endl;
    return(LAMERROR);
  }

  for (i = 0; i < REQUEST_MAX; i++)
    requests[i] = MPI_REQUEST_NULL;

  /* Start the persistent receives  */

  /* JMS WARNING! This is actually somewhat shady.  We're taking
     advantage of the fact that the sender can send a shorter message
     than we are expecting.  i.e., this one receive can receive one of
     two types of messages: a since IMPI_Uint8 or a full
     IMPI_Lamgiappe.  The MPI datatype for IMPI_Lamgiappe was
     carefully constructed to have an IMPI_Uint8 at the top, so proper
     endian byte-swapping will occur if necessary.  i.e., a real
     IMPI_Lamgiappe will never have that IMPI_Uint8 at the top filled
     in -- it's padding, explicitly for the purpose of being able to
     use one datatype to receive two different kinds of messages.  

     Adding 8 more bytes to an IMPI_Lamgiappe is probably not a crime
     because the overall message passing time will likely be dominated
     by latency, not message length.  Plus, this is IMPI
     communication, so it's expected to suck. :-)  */

  MPI_Recv_init(&impid_buffer, 1, IMPI_LAMGIAPPE, MPI_ANY_SOURCE, 
		MPI_ANY_TAG, parent_intra, &requests[ALL_INDEX]);

  MPI_Startall(REQUEST_MAX, requests);
  return 0;
}


/*
 *	impi_local_recv
 *
 *	Function:	- see if any messages came in from local LAM ranks
 *                      - this func is called frequently, use static vars
 */ 
int
impi_local_recv()
{
  static int ret;
  static int index, flag;
  static MPI_Status status;
  bool want_restart = true;

  MPI_Testany(REQUEST_MAX, requests, &index, &flag, &status);
  if (!flag)
    return 0;

  debug << "*** IMPID received message from local LAM rank" << endl;
  
  /* Handle received request.  */

  if (status.MPI_ERROR != MPI_SUCCESS)
    debug << "IMPID Houston -- we have a problem" << endl;

  ret = 0;
  switch(status.MPI_TAG) {
  case IMPI_LAMGIAPPE_TAG:
    debug << "Getting message from local LAM rank" << endl;
    local_receive();
    break;

  case IMPI_SYNCACK_TAG:
    debug << "Getting SYNCACK back from LAM rank" << endl;
    local_process_ack(impid_buffer.srqid);
    break;

  case IMPI_FINALIZE_TAG:
    debug << "Got FINALIZE message from local LAM rank" << endl;
    impi_nfinal_msgs++;
    if (impi_nfinal_msgs > num_lamprocs) 
      want_restart = false;
    break;

  case IMPI_ABORT_TAG:
    debug << "Got ABORT message from local LAM rank" << endl;
    want_restart = false;
    impi_bail(1);
    break;

  default:
    debug << "Got unknown message from local LAM rank: tag " 
	  << status.MPI_TAG << endl;
    want_restart = false;
    break;
  }

  if (want_restart)
    MPI_Start(&requests[index]);

  return ret;
}


/*
 *	impi_local_destroy
 *
 *	Function:	- cancel the persistent requests, and free the 
 *                        Lamgiappe datatype
 */ 
int
impi_local_destroy()
{
  int i, flag;

  /*
   * Cancel the persistent receives 
   */
  debug << "Canceling impid MPI_Requests" << endl;
  for (i = 0; i < REQUEST_MAX; i++) {
    if (requests[i] != MPI_REQUEST_NULL) {
      MPI_Cancel(&requests[i]);
      MPI_Test(&requests[i], &flag, MPI_STATUS_IGNORE);
      MPI_Request_free(&requests[i]);
    }
  }
  debug << "Canceling impid MPI_Requests done" << endl;

  /*
   * Free the lamgiappe datatype
   */

  debug << "Freeing IMPI_LAMGIAPPE" << endl;
  if (IMPI_Free_dtype()) {
    debug << "Failure when freeing the IMPI_LAMIAPPE dtype in the IMPID" 
	  << endl;
    return(LAMERROR);
  }
  debug << "Freeing IMPI_LAMGIAPPE done" << endl;

  return 0;
}


/*
 *	local_receive
 *
 *	Function:	- receive a message from the local LAM
 *                      - process it and queue up the first packet to go
 *
 *      Returns:        - 0 on success, LAMERROR on failure
 */
static int
local_receive()
{
  MPI_Status local_status;
  IMPI_Packet *pk;
  IMPI_Lamgiappe lam = impid_buffer.lamgiappe;

  // Snarf the relevant data from the packet into easier-to-read
  // variable names 

  int from = lam.local_src_rank;
  int length = lam.length;
  packet_mgmt_list *packets;

  // Do an MPI_Recv to get the user's message 

  char *buffer = new char[length];
  debug << "impid receiving message of length " << length 
	<< " from MCW rank " << lam.world_src_rank 
	<< " / impid_comm rank " << from << endl;

  if (MPI_Recv(buffer, length, MPI_BYTE, from, 
	       IMPI_MESSAGE_TAG, parent_intra, &local_status) != MPI_SUCCESS) {
    cerr << "LAM IMPI host failed to receive a message from LAM properly -- this is bad." << endl;
    impi_bail(1);
  }
  debug << "impid received message for world source rank " 
	<< lam.world_dest_rank << endl;

  // Make a list of packets from the message that we just got

  packets = make_packet_mgmt_list(&lam, buffer);
  IMPI_Uint8 srqid = packets->front()->get_packet()->pk_srqid;
  LAM_SWAP_ENDIAN(&srqid, sizeof(srqid));
  if (packets == 0) {
    cerr << "LAM IMPI host could not packetize a message; memory error?" 
	 << endl;
    impi_bail(1);
  }

  // Queue up the ssend/long message ACK's, if necessary 

  const bool is_long = (packets->size() > 1);
  if (is_long) {
    debug << "Registering IMPID to receive long ack: src "
	  << lam.world_src_rank << " dest " << lam.dest_rank
	  << endl;
    /* save last (n-1) packets in host_long.cc map by srqid */
    if (host_expect_long_ack(srqid, packets) != 0) {
      cerr << "LAM IMPI host failed to register an expected long message ACK from rank " 
	   << lam.world_dest_rank << endl;
      impi_bail(1);
    }
  } 

  // save in host_ack.cc map by srqid

  if (lam.is_ssend) {
    pk = packets->front()->get_packet();
    debug << "Registering IMPID to receive ssend ack: src "
	  << lam.world_src_rank << " dest " << lam.dest_rank
	  << endl;
    if (host_expect_ssend_ack(srqid, lam.world_src_rank, 
			      lam.dest_rank, pk) != 0) {
      cerr << "LAM IMPI host was not able to register an expected synchronous send ACK from rank " 
	   << lam.world_dest_rank << " (srqid " << srqid << ")" << endl;
      impi_bail(1);
    }
  }

  // queue up first packet in host queue

  debug << "Message is supposed to be on cid " << (int) lam.cid << endl;
  if (host_enque_first_packet(packets, lam.world_dest_rank) != 0) {
    cerr << "LAM IMPI host failed to enque a packet for rank "
	 << lam.world_dest_rank << endl;
    impi_bail(1);
  }
  debug << "First packet of message queued up: srqid " << (int) srqid
	<< endl;

  // We have to ensure that "packets" hasn't been deleted from deep
  // within host_enque_first_packet.  Check out this whacked out
  // condition that can happen...
  //
  // When one calls host_enque_first_packet, it will try to progress
  // the queue.  The queue may get an unexpected read that triggers
  // something to get put on the end of the queue.  If the queue goes
  // far enough and the first of this long message goes out, and then
  // we get a SYNCACK back for this long message, it will trigger
  // putting the remaining packets on the queue and deleting the list
  // containing them (which is held in the "packets" variable here).
  //
  // Hence, we have to see if the srqid has already been deleted from
  // the map.  If so, don't use packets anymore.  If not, delete it if
  // it's empty.  Or, if the message is short (i.e., no ack), we can
  // delete it, 'cause we've already queued the one packet that was on
  // the list.

  debug << "Checking delete of list: " << packets->empty() << endl;
  if ((host_find_long_ack(srqid, false) != 0 && packets->empty()) ||
      !is_long) {

    // It was a single packet, so host_enque_first_packet() already
    // emptied the list 

    debug << "Deleting packets list" << endl;
    delete packets;
  }

  // All done

  return 0;
}

#endif
