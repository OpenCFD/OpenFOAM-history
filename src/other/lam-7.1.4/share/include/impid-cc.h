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
 * $Id: impid-cc.h,v 6.19 2003/02/04 00:24:17 jsquyres Exp $
 *
 *	Function:	- IMPI client daemon header file for C++ functions
 */

#ifndef _LAM_IMPID_CC_H_
#define _LAM_IMPID_CC_H_

#ifndef __cplusplus

Error: This should only be included by C++ source files

#endif

/*
 * Turn on and off debugging wiping in classes
 */
#define LAM_IMPID_DEBUG 0


#include <impid.h>
#include <packet_mgmt.h>

class hostack_mgmt;
class longbuf_mgmt;

/*
 * comm.cc
 */
MPI_Comm comm_make(IMPI_Uint8 cid, int src_rank, int dest_rank, 
		   int dest_world_rank);
int comm_free(MPI_Comm comm);


/*
 * flow_control.cc
 */
int fc_init();
bool fc_can_send(IMPI_Proc* psrc, IMPI_Proc* pdest);
void fc_rcvd_packet(IMPI_Packet* pk);
void fc_rcvd_protoack(IMPI_Packet* pk);
void fc_sent_packet(IMPI_Packet* pk);
int fc_destroy();

/*
 * host.cc
 */
IMPI_Packet* make_syncack(IMPI_Packet* pk, IMPI_Uint8 drqid);


/*
 * host_ack.cc
 */
int host_ack_init();
int host_expect_ssend_ack(IMPI_Uint8 srqid, int local_world_rank, 
			  int dest_rank, IMPI_Packet* pk);
hostack_mgmt* host_find_ssend_ack(IMPI_Uint8 srqid);
int host_ack_destroy();


/*
 * host_long.cc
 */
int host_long_init();
int host_expect_long_ack(IMPI_Uint8 srqid, packet_mgmt_list* plist);
packet_mgmt_list* host_find_long_ack(IMPI_Uint8 srqid, bool wantErase);
int host_long_destroy();


/*
 * host_queue.cc
 */
int host_queue_init();
int host_enque_packet(IMPI_Packet* pk, char* buffer, int host_rank);
int host_enque_first_packet(packet_mgmt_list* plist, int world_dest_rank);
int host_enque_all_packets(packet_mgmt_list* plist, int host_rank);
int host_progress_queue(int host_rank);
int host_queue_destroy();


/* 
 * local.cc
 */
int impi_local_init();
int impi_local_destroy();
int impi_local_recv();


/*
 * local_ack.cc
 */
int local_ack_init();
int local_expect_ack(IMPI_Uint8 drqid, int host, IMPI_Packet *pk);
void local_process_ack(IMPI_Uint8 drqid);
int local_ack_destroy();


/*
 * local_long.cc
 */
int local_expect_long(IMPI_Packet* pk, char* buffer, int host, 
		      IMPI_Uint4 received);
int local_long_midreceive(IMPI_Uint8 drqid, int fd, IMPI_Uint4 pk_len);


/*
 * local_queue.cc
 */
int local_enqueue(IMPI_Packet* pk, char* buffer, MPI_Datatype type,
		  IMPI_Packet* syncack, bool is_datasync_ping, bool is_long);
int local_enqueue_finish_long(longbuf_mgmt* longbuf);


/*
 * local_req.cc
 */
int local_req_send(IMPI_Packet* pk, char* buffer, MPI_Datatype type,
		   IMPI_Packet* syncack);
int local_test();

/*
 * packet_mgmt.cc
 */
packet_mgmt_list* make_packet_mgmt_list(IMPI_Lamgiappe* lamgiappe, 
					char* buffer);
void set_packet_drqid(packet_mgmt_list* plist, IMPI_Uint8 drqid);

/*
 * procs.cc
 */
int proc_resolver_init();
int proc_resolver(IMPI_Proc* p);
int proc_resolver_destroy();


#endif
