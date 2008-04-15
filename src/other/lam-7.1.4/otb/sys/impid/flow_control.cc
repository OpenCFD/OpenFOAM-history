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
 * $Id: flow_control.cc,v 1.19 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- IMPI flow control rules
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

#include <string.h>

#include <iostream>
#include <vector>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <lamdebug-cc.h>

using std::vector;
using std::endl;


/*
 * private data
 */
typedef vector < vector < int > > count_t;
static count_t *count = 0;
static Debug debug(false);


/*
 *	fc_init
 *
 *	Function:	- initialize flow control data
 *
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
int
fc_init()
{
  IMPI_Uint4 i;
  vector<int> temp;

  // Set all the counts to zero

  for (i = 0; i < nprocs; i++)
    temp.push_back(0);

  count = new count_t;
  for (i = 0; i < nprocs; i++)
    count->push_back(temp);

  return 0;
}


/*
 *	fc_can_send
 *
 *	Function:	- determines if flow control allows to send to a 
 *                        given host
 *
 *	Accepts:	- host rank
 *      Returns:        - true if can immediately send, false if cannot
 */
bool
fc_can_send(IMPI_Proc* psrc, IMPI_Proc *pdest)
{
  debug << "fc_can_send" << endl;
  int src = proc_resolver(psrc);
  int dest = proc_resolver(pdest);

  // If both src and dest are < 0, then we're sending an IMPI_PK_FINI,
  // and no source proc or dest proc was specified.  Hence, just let
  // it through.
  
  if (src < 0 && dest < 0) {
    debug << "fc_can_send: shortcut FINI" << endl;
    return true;
  }

  // However, if *one* of the src/dest is < 0, we've got an error.
  // Return false and hope for the best...

  if (src < 0 || dest < 0) {
    debug << "fc_can_send: shortcut BADNESS: " << src << " " << dest << endl;
    return false;
  }

  debug << "fc_can_send checking: src " << src << " dest: " << dest << ": count is  "
     << (*count)[src][dest] << " ?<? " 
     << impi_lam_hiwater
     << endl;
  return (bool) ((*count)[src][dest] < impi_lam_hiwater);
}


/*
 *	fc_rcvd_packet
 *
 *	Function:	- records the receipt of a packet from a host
 *                      - if PROTOACK necessary, send it
 *
 *	Accepts:	- host rank
 */
void 
fc_rcvd_packet(IMPI_Packet* pk)
{
  if (pk->pk_type == IMPI_PK_PROTOACK || pk->pk_type == IMPI_PK_FINI)
    return;

  int src = proc_resolver(&pk->pk_src);
  int dest = proc_resolver(&pk->pk_dest);

  debug << "fc_rcvd_packet -- checking if we need a PROTOACK: "
	<< (*count)[src][dest] + 1 << " ?>? " 
	<< impi_lam_ackmark << endl;
  if (++(*count)[src][dest] >= impi_lam_ackmark) {

    // Fill in packet fields (re-used the make_syncack routine so that
    // we fill in all the fields)

    IMPI_Packet *ack = make_syncack(pk, 0);
    ack->pk_type = IMPI_PK_PROTOACK;
    ack->pk_srqid = 0;

    LAM_SWAP_ENDIAN(&ack->pk_type, sizeof(ack->pk_type));

    // Reset the recvd counter.  This may seem weird, but we have to
    // do it *before* we queue up the PROTOACK to go, since the
    // queue/write may get interrupted, and it might be possible to
    // get here again, and therefore double decrement the counter
    // (which is a Bad Thing).

    (*count)[src][dest] -= impi_lam_ackmark;
    debug << "Send a protoack so now the count on [ " << src << " ] [ "
	  << dest << " ] is " << (*count)[src][dest] << endl;

    // Queue it up to go

    debug << "fc_rcvd_packet -- queueing up a PROTOACK for src "
	  << src << " / host " 
	  << lam_impi_procs[src].ip_hostrank << " :: dest " << dest
	  << " / host " << lam_impi_procs[dest].ip_hostrank << endl;
    host_enque_packet(ack, 0, lam_impi_procs[src].ip_hostrank);
  }
}


/*
 *	fc_rcvd_protoack
 *
 *	Function:	- records the receipt of a PROTOACK between a 
 *                        process pair
 *
 *	Accepts:	- host rank
 */
void 
fc_rcvd_protoack(IMPI_Packet* pk)
{
  int src = proc_resolver(&pk->pk_src);
  int dest = proc_resolver(&pk->pk_dest);

  (*count)[dest][src] -= impi_lam_ackmark;
  debug << "Recved a protoack so now the count on [ " << dest << " ] [ "
	<< src << " ] is " << (*count)[dest][src] << endl;

  // Can we progress the queue now?

  if ((*count)[dest][src] < impi_lam_hiwater) {
    debug<< "Progressing the que b/c we got a protoack" << endl;
    host_progress_queue(lam_impi_procs[src].ip_hostrank);
    debug << "After progressing the queue, the count is " 
	  << (*count)[dest][src] << endl;
  }
}


/*
 *	fc_sent_packet
 *
 *	Function:	- records a send between a process pair
 *
 *	Accepts:	- host rank
 */
void 
fc_sent_packet(IMPI_Packet* pk)
{
  /* Must re/unswap endian to get the type back -- it's already been
     set for network order.  JMS This is ripe for
     optimization... later */

  IMPI_Uint4 type = pk->pk_type;
  LAM_SWAP_ENDIAN(&type, sizeof(type));

  if (type == IMPI_PK_PROTOACK || type == IMPI_PK_FINI)
    return;

  if (type == IMPI_PK_DATA)
    debug << "fc_sent_packet: just sent IMPI_PK_DATA" << endl;
  else if (type == IMPI_PK_DATASYNC)
    debug << "fc_sent_packet: just sent IMPI_PK_DATASYNC" << endl;

  int src = proc_resolver(&pk->pk_src);
  int dest = proc_resolver(&pk->pk_dest);

  (*count)[src][dest]++;
  debug << "fc_sent_packet: incrementing count from " << src << " to " 
	<< dest << "so now the count is " << (*count)[src][dest] << endl;
}


/*
 *	fc_destroy
 *
 *	Function:	- cleans up flow control data
 *
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
int
fc_destroy()
{
  if (count != 0) {
    count->clear();
    delete count;
#if LAM_IMPID_DEBUG
    count = 0;
#endif
  }

  return 0;
}


#endif
