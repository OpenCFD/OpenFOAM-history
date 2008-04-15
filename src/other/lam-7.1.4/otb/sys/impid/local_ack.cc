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
 * $Id: local_ack.cc,v 1.5 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- maintains SYNC ACKs that we expect to receive 
 *                        from the local LAM -- we've already sent a ping
 *                        to the local LAM containing the drqid; when they
 *                        reply with the drqid, we find the corresponding
 *                        syncack and queut it up to the remote IMPI host
 *                        
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <map>
#include <utility>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <hostack_mgmt.h>
#include <lamdebug-cc.h>

using std::map;
using std::pair;
using std::endl;


/*
 * local typedef for our map
 * Map indexed on IMPI_Uint8 (drqid)
 */
typedef map<IMPI_Uint8, pair<int, IMPI_Packet*> > local_ack_map;


/*
 * private variables
 */
static local_ack_map ack_map;
static Debug debug(false);


/*
 *	local_ack_init
 *
 *	Function:	- initialize local ack map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
local_ack_init()
{
  // Don't need to do anything yet

  return 0;
}


/*
 *	local_expect_ack
 *
 *	Function:	- puts in an IMPI_Packet to be [eventually] sent
 *                        out to a remote IMPI host 
 *                      - indexed on drqid
 *
 *	Accepts:	- drqid 
 *                      - ptr to IMPI_packet
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
local_expect_ack(IMPI_Uint8 drqid, int host, IMPI_Packet *pk)
{
  local_ack_map::iterator i;

  // If the drqid is in the map already, this is an error

  i = ack_map.find(drqid);

  //
  // ***** WARNING *****
  //
  // See the big warning in the local_req.cc.
  //

  if (i == ack_map.end()) {
    debug << "Registered local ACK drqid " << (int) drqid << endl;
    ack_map[drqid] = pair<int, IMPI_Packet*>(host, pk);
  } else
    return (LAMERROR);

  return 0;
}


/*
 *	local_process_ack
 *
 *	Function:	- find a pending drqid in the map
 *                      - if found, send the pending SYNCACK IMPI_Packet 
 *                      - indexed on drqid
 *
 *	Accepts:	- drqid 
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
void
local_process_ack(IMPI_Uint8 drqid)
{
  local_ack_map::iterator i;

  // If found, queue the packet up (don't delete it here; it will be
  // deleted when it is actually sent)

  debug << "Got drqid " << drqid << " from local LAM" << endl;
  i = ack_map.find(drqid);
  if (i != ack_map.end()) {
    debug << "IMPID sending SYNCACK packet back to remote host: drqid " 
	  << (*i).second.second->pk_drqid << endl;
    host_enque_packet((*i).second.second, 0, (*i).second.first);
    ack_map.erase(i); // complexity for removing by iterator is constant time
  }
}


/*
 *	local_ack_destroy
 *
 *	Function:	- destroy local ack map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
local_ack_destroy()
{
  local_ack_map::iterator i;

  for (i = ack_map.begin(); i != ack_map.end(); i++) {
    debug << "Deleting local_ack" << endl;
    delete ((*i).second.second);
  }
  ack_map.clear();

  return 0;
}


#endif
