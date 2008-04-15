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
 * $Id: local_long.cc,v 1.23 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- maintains partially received long messages 
 *                        from IMPI hosts
 *                      - indexed on pk_drqid (IMPI_Uint8)
 *                      - when a message is fully received, it is moved to 
 *                        local_req.cc to send to the local LAM
 *                        
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

// IRIX MIPSpro 7.30 compilers require that <new> comes first... @#$@#$!!!
#if LAM_CXX_NOTHROW_NEW
#include <new>
#endif
#include <iostream>
#include <map>

#include <mpi.h>
#include <laminternal.h>
#include <mpisys.h>
#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <longbuf_mgmt.h>
#include <lamdebug-cc.h>

#include <typical.h>

using std::map;
using std::cerr;
using std::endl;


/*
 * local typedef for our map
 * Map indexed on IMPI_Uint8 (drqid)
 * The map contains a longbuf_mgmt which is the packet, some meta info,
 *     and the buffer that is being filled 
 */
typedef map<IMPI_Uint8, longbuf_mgmt*> local_long_map;


/*
 * private variables
 */
static local_long_map long_map;
static Debug debug(false);


/*
 *	local_long_init
 *
 *	Function:	- initialize local long map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
local_long_init()
{
  // Don't need to do anything yet; just here for symmetry

  return 0;
}

/*
 *	local_expect_long
 *
 *	Function:	- get the drqid for this new long message
 *                      - call local_req_send to the lam node with the negative of
 *                        the tag 
 *                      - make the syncack for it and attach to the longbuf_mgmt
 *                      - put partially received buffer into map so that 
 *                        we can later receive the rest of it
 *                      - indexed on drqid
 *
 *	Accepts:	- ptr to IMPI_Packet header
 *                      - buffer with partially received data
 *                      - how many bytes already received (should always be 
 *                        IMPI_Pk_maxdatalen, but...)
 *                      - drqid to index on
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
local_expect_long(IMPI_Packet* pk, char* buffer, int host, IMPI_Uint4 received)
{
  local_long_map::iterator i;
   
  IMPI_Uint8 *pdrqid = new IMPI_Uint8(get_new_drqid());
  IMPI_Uint8 msglen = pk->pk_msglen;

  // if the drqid is in the map already, it's an error

  i = long_map.find(*pdrqid);

  if (i == long_map.end()) {
    pk->pk_drqid = *pdrqid;
#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
    longbuf_mgmt *lbm = new LAM_CXX_NOTHROW_NEW_ARG longbuf_mgmt(buffer, 
								 received, 
								 *pk);
    if (lbm == 0) {
      cerr << "LAM IMPI host not able to alloc long message placement holder" 
	   << endl;
      impi_bail(1);
    }
#else
    longbuf_mgmt *lbm;
    try {
      lbm = new longbuf_mgmt(buffer, received, *pk);
    } catch(...) {
      cerr << "LAM IMPI host not able to alloc long message placement holder" 
	   << endl;
      impi_bail(1);
    }
#endif

    IMPI_Packet *syncack = make_syncack(pk, *pdrqid);
    IMPI_Uint8 cid;

    // Post the "wait for ACK" before we send the ping.  When the ping
    // ACK is received from the destination LAM rank, the syncack will
    // be sent back to the remote IMPI host

    debug << "Waiting for long ACK from local LAM for drqid " 
	  << (int) *pdrqid << endl;
    local_expect_ack(*pdrqid, host, syncack);
    long_map[*pdrqid] = lbm;

    // send datasync "ping" to the lam node that a long msg is waiting

    cid = pk->pk_cid;
    pk->pk_cid = (IMPI_Uint8) lam_pt2impidatasync((int) cid);
    pk->pk_msglen = 1;
    debug << "Sending datasync drqid of " << *pdrqid << " to local LAM on cid "
	  << pk->pk_cid 
	  << endl;
    local_enqueue(pk, (char*) pdrqid, IMPI_TYPE_UINT8, 0, true, true);
    pk->pk_msglen = msglen;
    pk->pk_cid = cid;
    debug << "local_expect_long finished" << endl;
  }
  else
    return (LAMERROR);
  
  return 0;
}


/*
 *	local_long_midreceive
 *
 *	Function:	- locate the long msg in the local_long_map
 *                      - do a read on the host socket
 *                      - read the data directly into the msg buffer, 
 *                        appending onto what has already been read
 *                      - increment the bytes read by the packet length
 *                      - indexed on drqid
 *
 *	Accepts:	- drqid to index on
 *                      - host socket fd to read on
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
local_long_midreceive(IMPI_Uint8 drqid, int fd, IMPI_Uint4 pk_len)
{
  int recvd;
  local_long_map::iterator i;
  longbuf_mgmt *ret = 0;
  IMPI_Uint4 bytes_read = 0;
  IMPI_Uint8 rem_len;
  IMPI_Packet *pk;

  // Locate the longbuf_mgmt in the local_long_map

  i = long_map.find(drqid);
  if (i != long_map.end()) {
    ret = (*i).second;
    pk = ret->get_packet();
    recvd =  ret->get_received();
    rem_len = pk->pk_msglen - recvd;  // the size of the remainder of the msg

    if (pk_len > rem_len) {  // reading more than the buff's length
      cerr << "LAM IMPI tried to received a message that exceeded its pk_msglen of " 
	   << (unsigned int)  pk->pk_msglen  <<  " on socket  "  << fd  << endl;
      impi_bail(1);
    }
    if ((pk_len < IMPI_Pk_maxdatalen) && (pk_len != rem_len))
      cerr << "WARNING:  LAM IMPI receiving a middle packet of a long msg that's shorter than IMPI_Pk_maxdatalen "
	   << IMPI_Pk_maxdatalen << endl;

    // Now read in the data from the socket.

    bytes_read = mread(fd, ret->get_buffer() + recvd, pk_len);
    if (bytes_read != pk_len) {  
      cerr << "LAM IMPI got a error reading on socket  " << fd 
	   << endl;
      impi_bail(1);
    }
    ret->inc_received(bytes_read);
    if (ret->get_received() == pk->pk_msglen) {  // End of the long msg

      // send the long msg to lam and delete it from the map.
      // Ownership of the longbug_mgmt now passes to the local send
      // function, who will delete it later.

      local_enqueue_finish_long(ret);
      long_map.erase(i);
    }
  }
  else {
    cerr << "LAM IMPI host received an invalid drqid" << endl;
    return (LAMERROR);
  }
  return 0;
}



#endif
