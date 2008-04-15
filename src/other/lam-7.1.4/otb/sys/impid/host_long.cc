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
 * $Id: host_long.cc,v 1.8 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- holds the remaining packets of a long message
 *                      - indexed on the pk_srqid (IMPI_Uint8)
 *                      - first packet of each long message has already 
 *                        been sent
 *                      - when a SYNC ACK is found in this table, we fill 
 *                        in the pk_drqid and place the remaining packets 
 *                        on the destination host queue
 *
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <map>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <lamdebug-cc.h>

using std::map;
using std::endl;


/*
 * local typedef for our map
 * Map indexed on IMPI_Uint8 (srqid)
 * Map contains a pointer to a list of IMPI packet wrappers
 */
typedef map<IMPI_Uint8, packet_mgmt_list*> host_long_map;


/*
 * private variables
 */
static host_long_map long_map;
static Debug debug(false);


/*
 *	host_long_init
 *
 *	Function:	- initialize host long map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_long_init()
{
  // Don't need to do anything yet

  return 0;
}

/*
 *	host_expect_long_ack
 *
 *	Function:	- puts a list of packets in the map as expecting an 
 *                        long ack from a remote IMPI host
 *                      - indexed on srqid
 *
 *	Accepts:	- srqid 
 *                      - list of packets
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_expect_long_ack(IMPI_Uint8 srqid, packet_mgmt_list* plist)
{
  host_long_map::iterator i;

  // If the srqid is in the map already, this is an error

  debug << "Trying to register for long ACK from a host for srqid " 
	<< (int) srqid << endl;
  i = long_map.find(srqid);
  if (i == long_map.end())
    long_map[srqid] = plist;
  else
    return (LAMERROR);

  return 0;
}


/*
 *	host_find_long_ack
 *
 *	Function:	- find a pending SYNCACK in the map
 *                      - if found, return the rest of packets for a 
 *                        long message that need to be sent, and delete 
 *                        the srqid from the map
 *                      - indexed on srqid
 *
 *	Accepts:	- srqid 
 *      Returns:        - ptr to list of packets if found, 0 if not found
 */
packet_mgmt_list*
host_find_long_ack(IMPI_Uint8 srqid, bool wantErase)
{
  host_long_map::iterator i;
  packet_mgmt_list *ret = 0;

  // If found, return the list, and erase from the map

  i = long_map.find(srqid);
  if (i != long_map.end()) {
    ret = (*i).second;
    if (wantErase)
      long_map.erase(i); // complexity for removing by iterator is constant
  }

  return ret;
}


/*
 *	host_long_destrong
 *
 *	Function:	- destroy host long map
 *
 *      Returns:        - 0 for success, LAMERROR on error
 */
int
host_long_destroy()
{
  host_long_map::iterator i;

  for (i = long_map.begin(); i != long_map.end(); i++) {
    debug << "Deleting host_long" << endl;
    delete (*i).second;
  }

  long_map.clear();

  return 0;
}


#endif
