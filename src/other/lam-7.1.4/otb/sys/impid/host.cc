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
 * $Id: host.cc,v 1.34 2003/02/04 17:51:30 jsquyres Exp $
 *
 *	Function:	- all actions triggered by messages coming in from 
 *                        IMPI hosts
 */

#include <lam_config.h>

#if LAM_WANT_IMPI

// IRIX MIPSpro 7.30 compilers require that <new> comes first... @#$@#$!!!
#if LAM_CXX_NOTHROW_NEW
#include <new>
#endif

#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <set>

#include <impi-defs.h>
#include <impi.h>
#include <impid-cc.h>
#include <hostack_mgmt.h>
#include <lamdebug-cc.h>
#include <mpisys.h>
#include <packet_mgmt.h>

#include <laminternal.h>

using std::set;
using std::cerr;
using std::endl;
typedef set<int>  host_pkfini_rcvd_set;


/*
 * private variables
 */
static host_pkfini_rcvd_set pkfini_set;
static Debug debug(false);


/*
 * local functions
 */
static void process_data(IMPI_Packet* pk, int src_host);
static void process_datasync(IMPI_Packet* pk, int src_host);
static void process_protoack(IMPI_Packet* pk, int src_host);
static void process_syncack(IMPI_Uint8 srqid, IMPI_Uint8 drqid, 
			    int host_dest);
static void process_cancel(IMPI_Packet pk);
static void process_cancelyes();
static void process_cancelno();
static void process_fini(IMPI_host *h, struct pollfd *poll_fd);

static char* read_packet_data(IMPI_Uint4 len, IMPI_Uint8 msglen, int fd, 
			      IMPI_Uint4* bytes_read);


/*
 *	impi_host_init
 *
 *	Function:	- setup IMPI host routing communications
 *                      - create a list for each host so that we can 
 *                        queue packets when we are over the highwater mark
 *                      - create array of fd's that can be polled
 *      Returns:        - array of fd's suitable for polling
 */
int *
impi_host_init()
{
  IMPI_Uint4 i, j;
  int *fds;

  debug << "In impi_host_init" << endl;

  // Scan along all hosts, if they are non-local (i.e., not me), save
  // their fd

#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
  fds = new LAM_CXX_NOTHROW_NEW_ARG int [nhosts - 1];
  if (fds == 0) {
    cerr << "LAM IMPI host could not allocate array of ints" << endl; 
    impi_bail(1);
  }
#else
  try {
    fds = new int [nhosts - 1];
  } catch(...) {
    cerr << "LAM IMPI host could not allocate array of ints" << endl; 
    impi_bail(1);
  }
#endif
  for (j = i = 0; i < nhosts; i++)
    if (i != my_host_rank) {
      debug << "Gonna poll fd " << 
	lam_impi_hosts[i].ih_fd << " as index " << j << endl;
      fds[j] = lam_impi_hosts[i].ih_fd;
      j++;
    }

  debug << "Leaving impi_host_init" << endl;

  return fds;
}


/*
 *	impi_host_recv
 *
 *	Function:	- receive a packet from an IMPI host
 *                      - process the packet accordingly
 *
 *      Accepts:        - rank of host that is ready to be read from
 *                      - ptr to fd that will be used for polling in the 
 *                        main router (we'll fill this with -1 if we close
 *                        the socket)
 *      Returns:        - 0 on success, LAMERROR otherwise
 */
int
impi_host_recv(int src_host, struct pollfd *poll_fd)
{
  IMPI_host *h = &lam_impi_hosts[src_host];
  int src_rank;
  IMPI_Packet pk;
  int size, ret = 0;

  // Read in the packet header 

  debug << "Impi_host_recv -- reading packet header from host " 
	<< src_host << " (fd " << h->ih_fd << ") size " << sizeof(pk) << endl;
  size = mread(h->ih_fd, (char*) &pk, sizeof(pk));
  debug << "READ: sizeof(pk) = " << sizeof(pk) << ", read " << size << endl;
  if (size != sizeof(pk)) {
    cerr << "LAM IMPI client: Unexpected truncation of socket read (IMPI_Packet header)" << endl;
    return (LAMERROR);
  }

  // Properly endianize select fields from the packet header.  For
  // example, endianize the tags, but not the IMPI_Procs, because the
  // procs are stored in the lam_impi_* structs in network order for
  // speed of comparison and whatnot.

  // JMS: We should probably move these into the specific message
  // types, because not all fields are used for all message types, but
  // for now, I'm concerned with getting this working, not
  // optimization... 

  LAM_SWAP_ENDIAN(&pk.pk_type, sizeof(IMPI_Uint4));
  LAM_SWAP_ENDIAN(&pk.pk_len, sizeof(IMPI_Uint4));
  LAM_SWAP_ENDIAN(&pk.pk_srqid, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&pk.pk_drqid, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&pk.pk_msglen, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&pk.pk_lsrank, sizeof(IMPI_Int4));
  LAM_SWAP_ENDIAN(&pk.pk_tag, sizeof(IMPI_Int4));
  LAM_SWAP_ENDIAN(&pk.pk_cid, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&pk.pk_seqnum, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&pk.pk_count, sizeof(IMPI_Int8));
  LAM_SWAP_ENDIAN(&pk.pk_dtype, sizeof(IMPI_Uint8));

  // We know the *host* that it came from, now figure out the *rank*
  // that it came from.  This does *not* apply to IMPI_PK_FINI
  // messages, since they come from the host itself (i.e., they do not
  // come from a rank). 
  
  src_rank = proc_resolver(&pk.pk_src);
  debug << "Got source MCW rank: " << src_rank << " / host " 
	<< src_host << " (pid " << (int) pk.pk_src.p_pid << ")" << endl;
  if (pk.pk_type != IMPI_PK_FINI && src_rank == -1) {
    cerr << "IMPI LAM client: Message not able to be reverse identified -- this is bad." << endl;
    return (LAMERROR);
  }
  
  // What kind of message was it? 

  switch (pk.pk_type) {
  case IMPI_PK_DATA:
    debug << "Got IMPI_PK_DATA, srqid " << pk.pk_srqid << endl;
    
    process_data(&pk, src_host);
    break;

  case IMPI_PK_DATASYNC:
    debug << "Got IMPI_PK_DATASYNC, srqid " << pk.pk_srqid << endl;

    process_datasync(&pk, src_host);
    break;

  case IMPI_PK_PROTOACK:
    debug << "Got IMPI_PK_PROTOACK" << endl;

    process_protoack(&pk, src_host);
    break;

  case IMPI_PK_SYNCACK:
    debug << "Got IMPI_PK_SYNCACK" << endl;

    process_syncack(pk.pk_srqid, pk.pk_drqid, src_host);
    break;
    
  case IMPI_PK_CANCEL:
    debug << "Got IMPI_PK_CANCEL" << endl;
    process_cancel(pk);
    break;
    
  case IMPI_PK_CANCELYES:
    debug << "Got IMPI_PK_CANCELYES" << endl;

    process_cancelyes();
    break;
    
  case IMPI_PK_CANCELNO:
    debug << "Got IMPI_PK_CANCELNO" << endl;

    process_cancelno();
    break;
    
  case IMPI_PK_FINI:
    debug << "Got IMPI_PK_FINI" << endl;

    process_fini(h, poll_fd);
    break;
    
  default:
    debug << "IMPI host has no idea what kind of packet this is: "
	  << pk.pk_type << endl;
  }

  // If it wasn't a PROTOACK or FINI, increment our received counter.
  // Send back a PROTOACK if necessary. 

  if (pk.pk_type != IMPI_PK_PROTOACK && pk.pk_type != IMPI_PK_FINI)
    fc_rcvd_packet(&pk);

  return ret;
}


/*
 *	impi_host_send_pkfini_msgs
 *
 *	Function:	- Send IMPI_PK_FINI messages to all other hosts
 *      
 *      Accepts:        - fd's for the hosts so they can be set to -1 
 *                        when closed
 */
int
impi_host_send_pkfini_msgs(struct pollfd *h_fds)
{
  IMPI_Uint4 host_rank = 0;
  IMPI_Packet pk, *p;

  // If we haven't sent them already, do so 

  if (!impi_sent_pkfini_msgs) {
    // Per IMPI standard, this does not affect protocol ACK counts 

    LAM_ZERO_ME(pk);
    pk.pk_type = IMPI_PK_FINI;
    
    // These are all probably not necessary, but we'll fill them in
    // anyway with some sentinel values; should help with debugging.
    
    memset(pk.pk_src.p_hostid, 0, sizeof(pk.pk_src.p_hostid));
    pk.pk_src.p_pid = (IMPI_Int8) -1;
    memset(pk.pk_dest.p_hostid, 0, sizeof(pk.pk_dest.p_hostid));
    pk.pk_dest.p_pid = (IMPI_Int8) -1;
    
    pk.pk_len = sizeof(pk);
    pk.pk_srqid = 0;
    pk.pk_drqid = 0;
    pk.pk_msglen = sizeof(pk);
    pk.pk_lsrank = -1;
    pk.pk_tag = -1;
    pk.pk_cid = 0;
    pk.pk_seqnum = get_new_seqnum();
    
    pk.pk_count = -1;
    pk.pk_dtype = 0;
    pk.pk_reserved = 0;
    
    LAM_SWAP_ENDIAN(&pk.pk_type, sizeof(IMPI_Uint4));
    LAM_SWAP_ENDIAN(&pk.pk_len, sizeof(IMPI_Uint4));
    LAM_SWAP_ENDIAN(&pk.pk_msglen, sizeof(IMPI_Uint8));
    LAM_SWAP_ENDIAN(&pk.pk_seqnum, sizeof(IMPI_Uint8));

    for (host_rank = 0; host_rank < nhosts; host_rank++) {
      if (host_rank != my_host_rank) {
	// Call host_enque_packet but it is send right away b/c the
        // queue will be empty, and if we have rcvd the pk_fini from
	// that host close the socket

	debug << "HOST_ENQUE_PACKET !!!!!! for host rank " << host_rank 
	      << endl;

	// Must give host_enque_packet a pointer that it can delete
	// when the packet actually gets sent (which will be
	// immediately, in this case)

#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
	p = new LAM_CXX_NOTHROW_NEW_ARG IMPI_Packet;
	if (p == 0) {
	  cerr << "LAM IMPI host could not allocate IMPI_Packet (impi_host_send_pkfini_msgs)" << endl;
	  impi_bail(1);
	}
#else
	try {
	  p = new IMPI_Packet;
	} catch(...) {
	  cerr << "LAM IMPI host could not allocate IMPI_Packet (impi_host_send_pkfini_msgs)" << endl;
	  impi_bail(1);
	}
#endif
	memcpy(p, &pk, sizeof(IMPI_Packet));

	host_enque_packet(p, (char *) 0, host_rank);
	if (pkfini_set.find(lam_impi_hosts[host_rank].ih_fd) != 
	    pkfini_set.end()) {
	  debug << "CLOSING THE %&@&#@ SOCKET ON HOST " << host_rank 
		<< " the fd is " << lam_impi_hosts[host_rank].ih_fd << endl;
	  close(lam_impi_hosts[host_rank].ih_fd);
	  h_fds[host_rank].fd = -1;
	  h_fds[host_rank].events = 0;
	}
      }
    }
    impi_sent_pkfini_msgs = 1;
  }
  return 0;
}


/*
 *	process_data
 *
 *	Function:	- process an IMPI_PK_DATA packet
 *
 *      Accepts:        - ptr to packet that we just received
 *                      - rank of host that it came from
 */
static void
process_data(IMPI_Packet* pk, int src_host)
{
  IMPI_Uint4 size;
  char *buffer;

  // Is this a standalone short message? 

  if (pk->pk_len == pk->pk_msglen) {
    debug << "process_data: short message" 
	  << " len is " << (int) pk->pk_msglen 
	  << " local src rank " << (int) pk->pk_lsrank 
	  << " tag " << (int) pk->pk_tag
	  << " cid " << (int) pk->pk_cid << endl;

    // Read the data into a buffer
    
    buffer = read_packet_data(pk->pk_len, pk->pk_msglen, 
			      lam_impi_hosts[src_host].ih_fd, &size);

    // Pass message on to local_req.cc to isend it to LAM

    if (((int) pk->pk_len) == 4)
      debug << "process_data: first int - " << ((int*) buffer)[0] << endl;
    debug << "process_data: calling local_req_send" << endl;
    local_enqueue(pk, buffer, MPI_BYTE, 0, false, false);
  }

  // No, it is the middle of a long message

  else {
    debug << "process_data: middle of long message" << endl;

    // Look this puppy up in local_long_map.cc and read directly into
    // the buffer that has already been allocated

    local_long_midreceive(pk->pk_drqid, lam_impi_hosts[src_host].ih_fd, 
			  pk->pk_len);
  }

  debug << "process_data: done" << endl;
}


/*
 *	process_datasync
 *
 *	Function:	- process an IMPI_PK_DATASYNC packet
 *
 *      Accepts:        - ptr to packet that we just received
 *                      - rank of host that it came from
 */
static void
process_datasync(IMPI_Packet* pk, int src_host)
{
  char *buffer;
  IMPI_Uint4 received;
  IMPI_Packet *syncack;
  IMPI_Uint8 msglen, cid;

  // Read in the packet message data

  buffer = read_packet_data(pk->pk_len, pk->pk_msglen, 
			    lam_impi_hosts[src_host].ih_fd, &received);

  // Is the message short?

  debug << "process_datasync: Received DATASYNC: len = " 
	<< (int) pk->pk_len << " msglen = "
	<< (int) pk->pk_msglen << endl;
  if (pk->pk_len == pk->pk_msglen) {
    debug << "process_datasync: got short synchronous" << endl;

    // Pass message on to local_req.cc to isend it to LAM.  Need to
    // make 3 requests: send the ping (which must go first), send the
    // actual data, and receive the ACK back from the local LAM (at
    // which point we can then send the syncack back).

    IMPI_Uint8 *pdrqid = new IMPI_Uint8(get_new_drqid());

    // Wait for the ACK from the destination LAM rank before sending
    // off the syncack back to the remote IMPI host

    syncack = make_syncack(pk, (IMPI_Uint8) 0);
    debug << "process_datasync: queued up ACK expectation/SYNCACK" << endl;
    local_expect_ack(*pdrqid, src_host, syncack);

    msglen = pk->pk_msglen;
    cid = pk->pk_cid;

    pk->pk_cid = (IMPI_Uint8) lam_pt2impidatasync((int) cid);
    pk->pk_msglen = 1;
    debug << "process_datasync: sending ping to local lam, cid " 
	  << pk->pk_cid << " tag " << pk->pk_tag << endl;
    local_enqueue(pk, (char*) pdrqid, IMPI_TYPE_UINT8, 0, true, false);
    pk->pk_msglen = msglen;
    pk->pk_cid = cid;

    debug << "process_datasync: sending data to local lam, cid " 
	  << pk->pk_cid << " tag " << pk->pk_tag << endl;
    local_enqueue(pk, buffer, MPI_BYTE, 0, false, false);
  } 

  // No, the message is long; this packet is the first

  else {
    debug << "process_datasync: got long" << endl;
    local_expect_long(pk, buffer, src_host, (IMPI_Uint4) received);
    debug << "process_datasync: done" << endl;
  }
}


/*
 *	process_protoack
 *
 *	Function:	- process an IMPI_PK_PROTOACK packet
 */
static void
process_protoack(IMPI_Packet* pk, int src_host)
{
  fc_rcvd_protoack(pk);
}


/*
 *	process_syncack
 *
 *	Function:	- process an IMPI_PK_SYNCACK packet
 *
 *      Accepts:        - srqid
 *                      - drqid
 *                      - host rank of destination
 */
static void
process_syncack(IMPI_Uint8 srqid, IMPI_Uint8 drqid, int host_dest)
{
  packet_mgmt_list *plist;
  hostack_mgmt *hm;
  int tag;

  // Check to see if this requires a message sent back to LAM to ACK a
  // ssend/issend

  if ((hm = host_find_ssend_ack(srqid)) != 0) {
    IMPI_Packet *pk = hm->get_packet();
    IMPI_Uint8 cid;
    tag = pk->pk_tag;

    //
    // ***** WARNING *****
    //
    // See the big warning in the local_req.cc.
    //

    // Note that we use the *shadow* communicator's CID to send the
    // ping.  This avoids tag/cid conflicts with the real message that
    // the user has posted.

    int src_rank = hm->get_dest_rank();
    int dest_rank = hm->get_src_rank();
    debug << "process_syncack: send ACK back to LAM from rank " 
	  << src_rank
	  << " to rank " << dest_rank
	  << " with tag " << tag
	  << " on cid " << ((int) pk->pk_cid) + 1 << endl;
    cid = lam_pt2impisyncack(pk->pk_cid);
    MPI_Comm comm = comm_make(cid, src_rank,
			      dest_rank, 
			      proc_resolver(&pk->pk_src));

    // Make a bogus drqid to send.  The actual value doesn't matter
    // here because the LAM rank will not reply.

    IMPI_Uint8 bogus = (IMPI_Uint8) 0;
    MPI_Send(&bogus, 1, IMPI_TYPE_UINT8, dest_rank, tag, comm);
    comm_free(comm);

    // We own the memory now, so be sure to delete it

    delete hm;
    debug << "process_syncack: ACK sent back to LAM" << endl;
  }

  // Check to see if we need to send the remaining IMPI packets of a
  // long message

  if ((plist = host_find_long_ack(srqid, true)) != 0) {
    debug << "process_syncack: sending remaining packets of long message srqid " 
	  << (int) srqid << endl;

    // Put the drqid on all the packets in the plist 

    set_packet_drqid(plist, drqid);

    // Enque all the packet to the host that we received it from 

    host_enque_all_packets(plist, host_dest);
    debug << "process_syncack: deleting plist for srqid " << (int) srqid 
	  << " -- plist empty: " << plist->empty() << endl;
    delete plist;
  }
}


/*
 *	process_cancel
 *
 *	Function:	- process an IMPI_PK_CANCEL packet
 *      Returns:        - impi_pk_cancelno
 *                        since LAM does not have a send cancel 
 *                        implemented and due to the nature of the
 *                        impid, the window of opportunity to cancel 
 *                        a packet is too small to actually destroy the pk
 */
static void
process_cancel(IMPI_Packet pk)
{
  int host_rank = lam_impi_procs[proc_resolver(&(pk.pk_src))].ip_hostrank;
  IMPI_Packet* ret;

  ret = make_syncack(&pk, pk.pk_drqid);
  ret->pk_type  = IMPI_PK_CANCELNO;
  LAM_SWAP_ENDIAN(&ret->pk_type, sizeof(ret->pk_type));
  host_enque_packet(ret, (char *) 0, host_rank);
}


/*
 *	process_cancelyes
 *
 *	Function:	- process an IMPI_PK_CANCELYES packet
 */
static void
process_cancelyes()
{
  // Right now, we don't ask for remote cancels, so this function will
  // not get called 
}


/*
 *	process_cancelno
 *
 *	Function:	- process an IMPI_PK_CANCELNO packet
 */
static void
process_cancelno()
{
  // Right now, we don't ask for remote cancels, so this function will
  // not get called
}


/*
 *	process_fini
 *
 *	Function:	- process an IMPI_PK_FINI packet
 */
static void 
process_fini(IMPI_host *h, struct pollfd *poll_fd)
{
  // Insert the host fd of the pk_fini's that have been rcvd in a list
  // so the host socket can be shut down when all the mpi_finalize_tag
  // messages come in from the lam side
  
  debug << "Got PK_FINI from a HOST " << endl;
  pkfini_set.insert(h->ih_fd);
  impi_npkfini_msgs++;

  // Have we already sent out our IMPI_PK_FINI messages?  If so, then
  // it's time to close the socket to this host, because we just got
  // the corresponding ACK back from this host.
  
  if (impi_sent_pkfini_msgs) {
    debug << "Got PK_FINI, and I've already sent my PK_FINI's, so close the socket"
	  << endl;
    close(h->ih_fd);
    poll_fd->fd = h->ih_fd = -1;
    poll_fd->events = 0;
  }
}


/*
 *	read_packet_data
 *
 *	Function:	- malloc out buffer to read data
 *                      - read in data from a socket
 *
 *      Accepts:        - number of bytes to read
 *                      - size of target buffer
 *                      - file descriptor of socket to read
 *                      - ptr to number of bytes actually read
 *      Returns:        - ptr to new buffer
 */
static char*
read_packet_data(IMPI_Uint4 len, IMPI_Uint8 msglen, int fd, 
		 IMPI_Uint4 *bytes_read)
{
  char *buffer;

  // Malloc space for the receive buffer 

  debug << "read_packet_data: about to alloc " << (int) msglen 
     << " bytes" << endl;
#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
  buffer = new LAM_CXX_NOTHROW_NEW_ARG char[msglen];

  if (buffer == 0) {
    cerr << "LAM IMPI host could not malloc a buffer of " 
	 << (unsigned int) msglen << " bytes" << endl;
    impi_bail(1);
  }
#else
  try {
    buffer = new char[msglen];
  } catch(...) {
    cerr << "LAM IMPI host could not malloc a buffer of " 
	 << (unsigned int) msglen << " bytes" << endl;
    impi_bail(1);
  }
#endif

  // Now read in the data from the socket.  For short msgs, this will
  // be it.  For long messages, this will be the first set of data. 

  debug << "read_packet_data: mread" << endl;  
  if ((*bytes_read = mread(fd, buffer, (int4) len)) != len) {
    cerr << "LAM IMPI got a truncated message from socket " << fd 
	 << " expected " << (unsigned int) len << ", got " 
	 << *bytes_read << " bytes" << endl;
    impi_bail(1);
  }

  debug << "read_packet_data: all done" << endl;  
  return buffer;
}
  

/*
 *	make_syncack
 *
 *	Function:	- make an IMPI_PK_SYNCACK packet
 *                      - fill in a bunch of the fields
 *
 *      Accepts:        - ptr to IMPI_Packet to copy initial values from
 *                      - drqid
 *      Returns:        - ptr to new IMPI_Packet
 */
IMPI_Packet*
make_syncack(IMPI_Packet* pk, IMPI_Uint8 drqid)
{
#if LAM_CXX_NOTHROW_NEW || !LAM_CXX_EXCEPTIONS
  IMPI_Packet *ret = new LAM_CXX_NOTHROW_NEW_ARG IMPI_Packet;

  if (ret == 0) {
    cerr << "LAM IMPI host could not malloc an IMPI_Packet" << endl;
    impi_bail(1);
  }
#else
  IMPI_Packet *ret;
  try {
    ret = new IMPI_Packet;
  } catch(...) {
    cerr << "LAM IMPI host could not malloc an IMPI_Packet" << endl;
    impi_bail(1);
  }
#endif
  
  ret->pk_type  = IMPI_PK_SYNCACK;
  ret->pk_len   = sizeof(*pk);
  ret->pk_src   = pk->pk_dest;
  ret->pk_dest  = pk->pk_src;
  ret->pk_srqid = pk->pk_srqid;
  ret->pk_drqid = drqid;
  ret->pk_msglen = sizeof(*pk);

  // These fields are irrelevant, but filled in just to be safe

  ret->pk_lsrank = -1;
  ret->pk_tag = -1;
  ret->pk_cid = 73;
  ret->pk_seqnum = get_new_seqnum();
  
  ret->pk_count = -1;
  ret->pk_dtype = 0;
  ret->pk_reserved = 0;
  
  LAM_SWAP_ENDIAN(&ret->pk_type, sizeof(IMPI_Uint4));
  LAM_SWAP_ENDIAN(&ret->pk_len, sizeof(IMPI_Uint4));
  LAM_SWAP_ENDIAN(&ret->pk_srqid, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&ret->pk_drqid, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&ret->pk_msglen, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&ret->pk_seqnum, sizeof(IMPI_Uint8));
  LAM_SWAP_ENDIAN(&ret->pk_count, sizeof(IMPI_Int8));
  LAM_SWAP_ENDIAN(&ret->pk_dtype, sizeof(IMPI_Uint8));

  return ret;
}

#endif
