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
 * $Id: packet_mgmt.cc,v 1.14 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- makes long and short packets to place on host queues
 */

#include <lam_config.h>
#if LAM_WANT_IMPI

#include <iostream>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <packet_mgmt.h>



/*
 *	make_packet_mgmt_list
 *
 *	Function:	- makes a list of IMPI packets (wrapped in 
 *                        packet_mgmts)
 *                      - sets pk_type properly of resulting packets 
 *                        (depending if msg is short or long and if ssend)
 *                      - sets pk_srqid
 *
 *	Accepts:	- IMPI_Lamgiappe pointer "ping" message from local 
 *                        LAM node with meta info about message
 *                      - pointer to buffer
 *      Returns:        - STL list of packet_mgmts
 */
packet_mgmt_list*
make_packet_mgmt_list(IMPI_Lamgiappe* lamgiappe, char* buffer)
{
  IMPI_Uint8 size = (IMPI_Uint8) lamgiappe->length;
  IMPI_Uint8 count = (IMPI_Uint8) 0;
  IMPI_Uint8 srqid = (IMPI_Uint8) 0;
  packet_mgmt *pmgmt;
  bool wantDelete = false;
  bool first = true;
  IMPI_Packet pk;

  // If we have a long or synchronous message, we need to get a new
  // srqid for it

  if (size > IMPI_Pk_maxdatalen || lamgiappe->is_ssend == 1)
    srqid = get_new_srqid();

  // For zero-length messages, we still need to have a single
  // IMPI_Packet
  
  packet_mgmt_list *ret = new packet_mgmt_list;
  do {

    // Fill a IMPI_Packet with all the values

    pk.pk_type = IMPI_PK_DATA;
    if (first && (lamgiappe->is_ssend == 1 || 
		  ((IMPI_Uint4) lamgiappe->length) > IMPI_Pk_maxdatalen)) 
      pk.pk_type = IMPI_PK_DATASYNC;

    pk.pk_len = (IMPI_Uint4) (size > IMPI_Pk_maxdatalen) ?
      IMPI_Pk_maxdatalen : size;
    memcpy(pk.pk_src.p_hostid, 
	   lam_impi_procs[lamgiappe->world_src_rank].ip_ipv6,
	   sizeof(pk.pk_src.p_hostid));
    pk.pk_src.p_pid = lam_impi_procs[lamgiappe->world_src_rank].ip_pid;
    memcpy(pk.pk_dest.p_hostid, 
	   lam_impi_procs[lamgiappe->world_dest_rank].ip_ipv6,
	   sizeof(pk.pk_dest.p_hostid));
    pk.pk_dest.p_pid = lam_impi_procs[lamgiappe->world_dest_rank].ip_pid;

    pk.pk_srqid = srqid;
    pk.pk_drqid = (IMPI_Uint8) 0;
    pk.pk_msglen = (IMPI_Uint8) lamgiappe->length;
    pk.pk_lsrank = (IMPI_Int4) lamgiappe->world_src_rank;
    pk.pk_tag = (IMPI_Int4) lamgiappe->tag;
    pk.pk_cid = (IMPI_Uint8) lamgiappe->cid;
    pk.pk_seqnum = get_new_seqnum();

    pk.pk_count = (IMPI_Int8) 0;
    pk.pk_dtype = (IMPI_Uint8) 0;
    pk.pk_reserved = (IMPI_Uint8) 0;

    LAM_SWAP_ENDIAN(&pk.pk_type, sizeof(IMPI_Uint4));
    LAM_SWAP_ENDIAN(&pk.pk_len, sizeof(IMPI_Uint4));
    LAM_SWAP_ENDIAN(&pk.pk_srqid, sizeof(IMPI_Uint8));
    LAM_SWAP_ENDIAN(&pk.pk_msglen, sizeof(IMPI_Uint8));
    LAM_SWAP_ENDIAN(&pk.pk_lsrank, sizeof(IMPI_Int4));
    LAM_SWAP_ENDIAN(&pk.pk_tag, sizeof(IMPI_Int4));
    LAM_SWAP_ENDIAN(&pk.pk_cid, sizeof(IMPI_Uint8));
    LAM_SWAP_ENDIAN(&pk.pk_seqnum, sizeof(IMPI_Uint8));

    // Now make the packet wrapper.  It saves a *copy* of the
    // IMPI_Packet, so we can just send the address of this one so
    // that we don't have to worry about deleting it.

    if (size <= IMPI_Pk_maxdatalen)
      wantDelete = true;
    pmgmt = new packet_mgmt(buffer, buffer + count, wantDelete, &pk);

    // Put the packet wrapper in the list

    ret->push_back(pmgmt);

    // Update the numbers

    size -= (size > IMPI_Pk_maxdatalen) ? IMPI_Pk_maxdatalen : size;
    count += IMPI_Pk_maxdatalen;
    first = false;
  } while (size > (IMPI_Uint8) 0);

  return ret;
}


/*
 *	set_packet_drqid
 *
 *	Function:	- traverse a packet_mgmt_list and set the drqid of 
 *                        all the items
 *
 *	Accepts:	- list of packets
 *                      - drqid
 *      Returns:        - STL list of packet_mgmts
 */
void
set_packet_drqid(packet_mgmt_list* plist, IMPI_Uint8 drqid)
{
  packet_mgmt_list::iterator i;
  IMPI_Uint8 adjusted = drqid;
  LAM_SWAP_ENDIAN(&adjusted, sizeof(IMPI_Uint8));

  for (i = plist->begin(); i != plist->end(); i++)
    (*i)->get_packet()->pk_drqid = adjusted;
}

#endif
