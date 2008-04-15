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
 * $Id: host_ack.cc,v 1.8 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- maintains SYNC ACKs that we expect to receive 
 *                        from IMPI hosts that require the ACK to a local
 *                        LAM host to complete an MPI_Ssend or MPI_Issend
 *                      - indexed on pk_srqid (IMPI_Uint8)
 *                      - when a SYNC ACK is found in this table, we must
 *                        send back ACK to LAM to complete 
 *                        MPI_Ssend/MPI_Issend
 *                      - possible for a message both be in here and 
 *                        host_long.c if it is a long synchronous send
 *                        
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <map>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <hostack_mgmt.h>

/*
 * local typedef for our map
 * Map indexed on IMPI_Uint8 (srqid)
 * Map contains a rank of a local LAM process (int) relative to 
 *   MPI_COMM_WORLD/impid_comm
 */
using std::map;
typedef map<IMPI_Uint8, hostack_mgmt*> host_ssend_ack_map;


/*
 * private variables
 */
static host_ssend_ack_map ack_map;


/*
 *	host_ack_init
 *
 *	Function:	- initialize host ack map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_ack_init()
{
  // Don't need to do anything yet

  return 0;
}


/*
 *	host_expect_ssend_ack
 *
 *	Function:	- puts a local LAM rank in the map as expecting an 
 *                        MPI_Ssend/MPI_Issend ack from a remote IMPI host
 *                      - indexed on srqid
 *
 *	Accepts:	- srqid 
 *                      - local rank (relative to MCW)
 *                      - dest rank (relative to CID that msg was sent on)
 *                      - ptr to IMPI_packet
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_expect_ssend_ack(IMPI_Uint8 srqid, int local_world_rank, 
		      int dest_rank, IMPI_Packet *pk)
{
  host_ssend_ack_map::iterator i;

  // If the srqid is in the map already, this is an error

  i = ack_map.find(srqid);

  //
  // ***** WARNING *****
  //
  // See the big warning in the local_req.cc.
  //

  if (i == ack_map.end())
    ack_map[srqid] = new hostack_mgmt(local_world_rank, local_world_rank,
				      dest_rank, pk);
  else
    return (LAMERROR);

  return 0;
}


/*
 *	host_find_ssend_ack
 *
 *	Function:	- find a pending SYNCACK in the map
 *                      - if found, return the rank and delete the srqid 
 *                        from the map
 *                      - indexed on srqid
 *
 *	Accepts:	- srqid 
 *      Returns:        - local rank from map, or -1 if not found
 */
hostack_mgmt*
host_find_ssend_ack(IMPI_Uint8 srqid)
{
  host_ssend_ack_map::iterator i;
  hostack_mgmt *ret = 0;

  // If found, return the list, and erase from the map

  i = ack_map.find(srqid);
  if (i != ack_map.end()) {
    ret = (*i).second;
    ack_map.erase(i); // complexity for removing by iterator is constant time
  }

  return ret;
}


/*
 *	host_ack_destroy
 *
 *	Function:	- destroy host ack map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_ack_destroy()
{
  host_ssend_ack_map::iterator i;

  for (i = ack_map.begin(); i != ack_map.end(); i++) {
    delete ((*i).second)->get_packet();
    delete ((*i).second);
  }
  ack_map.clear();

  return 0;
}


#endif
