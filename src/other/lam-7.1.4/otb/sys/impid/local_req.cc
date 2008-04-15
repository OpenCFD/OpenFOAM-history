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
 * $Id: local_req.cc,v 1.18 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- maintains the MPI_Requests and associated data
 *                        for messages from IMPI hosts to the local LAM
 *                      - when msg comes from an IMPI host, do an MPI_Isend
 *                      - save the MPI_Request in an STL list
 *                      - do an MPI_Test loop; then prune completed requests 
 *                        out of the STL list
 *                      - when pruning, may need to queue up an SYNC ACK 
 *                        IMPI_Packet on the appropriate outgoing host queue
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <iostream>

#include <impi.h>
#include <req_mgmt.h>
#include <lamdebug-cc.h>

using std::list;
using std::cerr;
using std::endl;


/*
 * private variables
 */
typedef list<req_mgmt*> local_requests_list;

static local_requests_list req_list;
static Debug debug(false);


/*
 *	local_req_send
 *
 *	Function:	- send a message to a local LAM rank
 *                      - must calculate local LAM rank number from message
 *                        header (pk_src)
 *                      - do MPI_Isend to send message to LAM
 *                      - insert MPI_Request and ptr to SYNCACK packet in
 *                        list of pending requests to LAM
 *
 *      Accepts:        - ptr to IMPI_Packet header of message
 *                      - ptr to data buffer
 *                      - ptr to a SYNCACK IMPI_Packet that will be sent 
 *                        when this message is actually received by the 
 *                        LAM rank (or 0 if there is no syncack)
 *
 *      Returns:        - 0 on success, LAMERROR on failure
 */
int
local_req_send(IMPI_Packet* pk, char* buffer, MPI_Datatype type, 
	       IMPI_Packet* syncack)
{
  MPI_Request req;

  //
  // ***** WARNING *****
  //
  // Putting in the proc_resolver(&pk->pk_src) for the dest rank will
  // only work right now because the impid knows about the
  // communicator that it is being sent on, MPI_COMM_WORLD.  Once
  // collectives are put into IMPI, it is likely that the impid will
  // need to become aware of all the communicators in LAM (i.e.,
  // participate in all the communicator constructors).  Then we can
  // lookup ranks in arbitrary [local] communicators.
  //
  // The issue is for the LAM progression engine (at least the C2C
  // engine) -- an MPI_Ssend (or MPI_Issend) will send an ACK back
  // deep within the progression engine.  The destination rank will
  // put its rank number in the ACK (relative to the CID that it was
  // sent on), which the impid's progression engine will not
  // understand (since we don't know that the destination's rank is in
  // the target cid).
  //
  // We can currently forge it properly only because without
  // collectives, there is only one communicator: MPI_COMM_WORLD, so
  // the impid essentially can lookup the destination rank in the
  // target communicator.  This will change when the collectives are
  // put in and more communicators (within an IMPI job) are possible.

  int dest_rank = proc_resolver(&pk->pk_dest);
  MPI_Comm comm = comm_make(pk->pk_cid, pk->pk_lsrank, 
			    dest_rank, 
			    proc_resolver(&pk->pk_dest));
  
  // post the send to the lam node

  debug << "local_req_send: sending to LAM: src rank " << pk->pk_lsrank
	<< " dest rank " << dest_rank
	<< " dest world rank " << proc_resolver(&pk->pk_dest) 
	<< " msglen " << pk->pk_msglen 
	<< " tag " << pk->pk_tag 
	<< " cid " << (int) pk->pk_cid << endl;
  if (MPI_Isend(buffer, pk->pk_msglen, type, dest_rank, 
		pk->pk_tag, comm, &req) != MPI_SUCCESS) {
    cerr << "LAM IMPI host was not able to send to the local LAM" << endl;
    impi_bail(1);
  }
  
  // make request_mgmt with the req and the corresponding syncack and
  // insert it into the list

  req_mgmt *lrs = new req_mgmt(buffer, req, syncack, comm);
  req_list.push_back(lrs); 

  return 0;
}


/*
 *	local_test
 *
 *	Function:	- loop over the list of requests outstanding and 
 *                        do MPI_Test on each 
 *                      - if the send completed, enque the syncack pk
 *      Accepts:        - list of local requests
 *
 *      Returns:        - 0 on success, LAMERROR on failure
 */
int
local_test()
{
  // Short circuit optimization; if we have nothing to wait for, just
  // return

  if (req_list.empty())
    return 0;

  int flag;
  IMPI_Packet *pk;
  MPI_Status status;
  local_requests_list::iterator i, prev;
  static unsigned int last = 0;

  if (last != req_list.size()) {
    debug << "Checking " << req_list.size() << " pending MPI_Requests" << endl;
    last = req_list.size();
  }
  for (prev = req_list.end(), i = req_list.begin(); i != req_list.end(); i++) {
    if (prev != req_list.end())
      req_list.erase(prev);

    MPI_Test((*i)->get_req(), &flag, &status);
    if (flag) {
      debug << "A send to the local LAM has finished" << endl;

      // Error check

      if (status.MPI_ERROR != MPI_SUCCESS) {
	cerr << "LAM IMPI host discovered an error sending to the local LAM"
	     << endl;
	impi_bail(1);
      }

      // the send has completed so send the syncack to the host 

      pk = (*i)->get_packet();
      if (pk != 0) {
	debug << "Sending some SYNCACK back to MCW rank "
	      << proc_resolver(&pk->pk_dest) << endl;
	host_enque_packet(pk, (char *) 0,
			  lam_impi_procs[proc_resolver(&(pk->pk_dest))].ip_hostrank);
      }

      // Free the fake comm used for the isend, and the (req_mgmt*)

      comm_free((*i)->get_comm());
      delete (*i);

      // per STL reference guide pg. 144 erasing i++ is fine

      prev = i;
    } else
      prev = req_list.end();
  }
  if (prev != req_list.end())
    req_list.erase(prev);

  if (last != req_list.size()) {
    debug << req_list.size() << " MPI_Requests still pending" << endl;
    last = req_list.size();
  }

  return 0;
}

#endif
