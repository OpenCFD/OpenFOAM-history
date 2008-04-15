/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 2001 by The Vivian project
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <iomanip>

#include <mico/os-net.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/ltp.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

using namespace std;

/*******************************************************************************
*                             LTPTransport class                               *
*******************************************************************************/


/*******************************************************************************
* MICOLTP::LTPTransport::LTPTransport()
*
* Constructor, reserves memory for an internal buffer, creates a socket and sets
* initial values.
*
* input parameters:
*   CORBA::Long thefd             : file descriptor to a BT socket
*
* return value: guess what...
*******************************************************************************/

MICOLTP::LTPTransport::LTPTransport(CORBA::Long thefd) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::LTPTransport" << endl;
    cerr << "    internal buffer is    : " << BUF_SIZE << " bytes" << endl;
    cerr << "    LTP message buffer is : " << LTP_BUF << " bytes" << endl;
  #endif

  // init the internal buffer and its handling variables
  if ((buf_the_dragon = (char *)malloc(BUF_SIZE)) == NULL) {
    cerr << "    Buf died unexpectally :(" << endl;
    assert(0);
  }

  if ((ltp_buffer = (char *)malloc(LTP_BUF)) == NULL) {
    cerr << "    Error while allocating memory" << endl;
    assert(0);
  }

  buffer_empty = TRUE;
  scales = 0;

  // create the socket
  fd = thefd;
  if (fd < 0) {
    fd = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    assert(fd >= 0);
  }

  // set initial values for the socket, dispatchers and EOF flag
  OSNet::sock_block(fd, TRUE);
  is_blocking = TRUE;
  is_buffering = FALSE;
  rdisp = wdisp = 0;
  rcb = wcb = 0;
  ateof = FALSE;
}


/*******************************************************************************
* MICOLTP::LTPTransport::~LTPTransport()
*
* Destructor, frees a socket and removes all dispathers.
*
* input parameters:
*   none
*
* return value: right from the book...
*******************************************************************************/

MICOLTP::LTPTransport::~LTPTransport() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::~LTPTransport" << endl;
  #endif

  // remove dispatchers (in case they are set of course)
  if (rdisp && rcb) {
    rdisp->remove(this, CORBA::Dispatcher::Read);
    rdisp = 0;
    rcb->callback(this, CORBA::TransportCallback::Remove);
  }
  if (wdisp && wcb) {
    wdisp->remove(this, CORBA::Dispatcher::Write);
    wdisp = 0;
    wcb->callback(this, CORBA::TransportCallback::Remove);
  }

  // close the socket and free internal buffer
  OSNet::sock_close(fd);
  free(buf_the_dragon);
  free(ltp_buffer);
}


/*******************************************************************************
* void MICOLTP::LTPTransport::rselect()
*
* Selects a read dispatcher.
*
* input parameters:
*   CORBA::Dispatcher *disp       : new dispatcher to attach to
*   CORBA::TransportCallback *cb  : new transport callback to attach to
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::rselect(CORBA::Dispatcher *disp,
  CORBA::TransportCallback *cb) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::rselect" << endl;
  #endif

  // remove an existing dispatcher (if any)
  if (rcb && rdisp) {
    rdisp->remove(this, CORBA::Dispatcher::Read);
    rdisp = 0;
    rcb = 0;
  }

  // attach to a new dispatcher
  if (cb) {
    disp->rd_event(this, fd);
    rdisp = disp;
    rcb = cb;
  }
}


/*******************************************************************************
* void MICOLTP::LTPTransport::wselect()
*
* Selects a write dispatcher.
*
* input parameters:
*   CORBA::Dispatcher *disp       : new dispatcher to attach to
*   CORBA::TransportCallback *cb  : new transport callback to attach to
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::wselect(CORBA::Dispatcher *disp,
  CORBA::TransportCallback *cb) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::wselect" << endl;
  #endif

  // remove an existing dispatcher (if any)
  if (wcb && wdisp) {
    wdisp->remove(this, CORBA::Dispatcher::Write);
    wdisp = 0;
    wcb = 0;
  }

  // attach to a new dispatcher
  if (cb) {
    disp->wr_event(this, fd);
    wdisp = disp;
    wcb = cb;
  }
}


/*******************************************************************************
* void MICOLTP::LTPTransport::callback()
*
* Handles different callback events.
*
* input parameters:
*   CORBA::Dispatcher *disp       : new dispatcher to attach to
*   CORBA::Dispatcher::Event ev   : event to handle
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::callback(CORBA::Dispatcher *disp,
  CORBA::Dispatcher::Event ev) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::callback" << endl;
  #endif

  // switch to a correct callback action depending on the event received
  switch (ev) {
    case CORBA::Dispatcher::Read:
      assert (rcb);
      rcb->callback(this, CORBA::TransportCallback::Read);
      break;
    case CORBA::Dispatcher::Write:
      assert (wcb);
      wcb->callback(this, CORBA::TransportCallback::Write);
      break;
    case CORBA::Dispatcher::Remove:
      wdisp = rdisp = 0;
      wcb = rcb = 0;
      break;
    case CORBA::Dispatcher::Moved:
      wdisp = rdisp = disp;
      break;
    default:
      assert(0);
  }
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransport::bind()
*
* Binds a socket.
*
* input parameters:
*   CORBA::Address *a             : address that the socket is binded to
*
* return value: TRUE if success, otherwise FALSE
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::bind(const CORBA::Address *a) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::bind" << endl;
  #endif

  struct sockaddr_l2 sin;

  // check that the address *a has correct proto
  assert(!strcmp(a->proto(), "bt-l2cap"));
  LTPAddress *ia = (LTPAddress *)a;

  // get the address into sin struct and bind the socket
  ia->getsockaddr(&sin);
  CORBA::Long r = ::bind(fd, (socket_addr_t)&sin, sizeof(sin));
  if (r < 0) {
    OSNet::set_errno();
    err = xstrerror(errno);
    return FALSE;
  }

  return TRUE;
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransport::connect()
*
* Connects to a peer.
*
* input parameters:
*   CORBA::Address *a             : an address to connect to
*
* return value: TRUE if success, otherwise FALSE
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::connect(const CORBA::Address *a) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::connect" << endl;
  #endif

  struct sockaddr_l2 sin;

  // check that the address in *a has a correct proto
  assert(!strcmp(a->proto(), "bt-l2cap"));
  LTPAddress *ia = (LTPAddress *)a;

  // get host address
  ia->getsockaddr(&sin);

  // connect
  CORBA::Long r = ::connect(fd, (socket_addr_t)&sin, sizeof(sin));
  if (r < 0) {
    OSNet::set_errno();
    err = xstrerror(errno);
    return FALSE;
  }

  return TRUE;
}

/*******************************************************************************
*  void MICOLTP::LTPTransport::open()
*
*
*
* input parameters:
*   CORBA::Long fd                : the socket descriptor to open
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::open(CORBA::Long) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::open" << endl;
  #endif

  /*
   * I have put this as a no-op.  It seems that MICO's transport
   * interfaces have changed quite a bit since this class was
   * implemented.  I hope someday to bring these classes up to speed.
   */
}

/*******************************************************************************
* void MICOLTP::LTPTransport::close()
*
* Closes the socket and removes (read & write)dispatchers.
*
* input parameters:
*   none
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::close() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::close" << endl;
  #endif

  // close the socket and recreate it
  OSNet::sock_close (fd);
  fd = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
  assert(fd >= 0);

  // remove dispacthers (if any)
  if (rdisp && rcb) rdisp->remove(this, CORBA::Dispatcher::Read);
  if (wdisp && wcb) wdisp->remove(this, CORBA::Dispatcher::Write);

  // set initial values the socket, dispathers and EOF flag
  is_blocking = TRUE;
  is_buffering = FALSE;
  rdisp = wdisp = 0;
  rcb = wcb = 0;
  ateof = FALSE;
}


/*******************************************************************************
* void MICOLTP::LTPTransport::block()
*
* Tells the transport to block / unblock.
*
* input parameters:
*   CORBA::Boolean doblock        : TRUE = block, FALSE = unblock
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::block(CORBA::Boolean doblock) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::block" << endl;
  #endif

  if (!!is_blocking != !!doblock) {
    is_blocking = doblock;
    OSNet::sock_block (fd, doblock);
  }
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransport::isblocking()
*
* Checks if the transport is blocking.
*
* input parameters:
*   none
*
* return value: TRUE if the transport is blocking
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::isblocking() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::isblocking" << endl;
  #endif

  return is_blocking;
}


/*******************************************************************************
* void MICOLTP::LTPTransport::buffering()
*
* Tells the transport to perform / not to perform buffering.
*
* input parameters:
*   CORBA::Boolean dobuffering    : TRUE = buffer, FALSE = do not buffer
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransport::buffering(CORBA::Boolean dobuffering) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::buffering" << endl;
  #endif

  if (!!is_buffering != !!dobuffering) {
    is_buffering = dobuffering;
  }
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransport::isbuffering()
*
* Checks is a socket is buffering.
*
* input parameters:
*   none
*
* return value: TRUE if the socket is buffering
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::isbuffering() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::isbuffering" << endl;
  #endif

  return is_buffering;
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransport::isreadable()
*
* Checks is a socket is readable.
*
* input parameters:
*   none
*
* return value: TRUE if the socket is readable
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::isreadable() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::isreadable" << endl;
  #endif

  fd_set rset;
  struct timeval tm;

  // clear the set and add transport's file descriptor to it
  FD_ZERO (&rset);
  FD_SET (fd, &rset);

  // set timeout values
  tm.tv_sec = 0;
  tm.tv_usec = 0;

  // test
  int r = ::select (fd+1, (select_addr_t)&rset, 0, 0, &tm);
  return r > 0;
}


/*******************************************************************************
* CORBA::Long MICOLTP::LTPTransport::read()
*
* Returns local address.
*
* input parameters:
*   const void *_b                : buffer to read into
*   CORBA::Long len               : the length of the buffer
*
* return value: the number of bytes read
*******************************************************************************/

CORBA::Long MICOLTP::LTPTransport::read(void *_b, CORBA::Long len) {
  CORBA::Long bytes_read = 0;
  CORBA::Long r;
  CORBA::Boolean blocking = isblocking();
  uint16_t msg_size;

  #ifdef LTP_DEBUG
    cerr << "LTPTransport::read" << endl;
    cerr << "    bytes asked     : " << len << endl;

    // read from the socket if internal buffer is empty
    if (buffer_empty) {
      block(FALSE);

      while(1) {
	r = OSNet::sock_read(fd, ltp_buffer, LTP_BUF);
	cerr << "    bytes read      : " << r << endl;

	// nothing to read
	if (r == 0) {
	  cerr << "    EOF reached so return 0" << endl;
	  ateof = TRUE;
	  block(blocking);
	  return 0;
	}

	// error while reading
	else if (r < 0) {
	  cerr << "    an error occurred (harmless one)";
	  OSNet::set_errno();
	  if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN) {
	    cerr << " so return 0" << endl;
	    block(blocking);
	    return 0;
	  }
	  err = xstrerror(errno);
	  cerr << endl << "    error number is : " << r << endl;
	  block(blocking);
	  return r;
	}

	memcpy(&msg_size, ltp_buffer + 1, sizeof(uint16_t));
	cerr << "    message size    : " << msg_size << endl;
	cerr << "    type            : ";

	switch (ltp_buffer[0]) {
	case LTP_SEG_NONE:
          cerr << "LTP_SEG_NONE" << endl;
          break;

        case LTP_SEG_FIRST:
          cerr << "LTP_SEG_FIRST" << endl;
	  block(TRUE);
          break;

        case LTP_SEG_MIDDLE:
          cerr << "LTP_SEG_MIDDLE" << endl;
          break;

        case LTP_SEG_LAST:
          cerr << "LTP_SEG_LAST" << endl;
	  block(FALSE);
          break;

        default:
          cerr << "    What the hell are we doing here???" << endl;
          assert(0);
	}

	memcpy(buf_the_dragon + bytes_read, ltp_buffer + LTP_HDR_SIZE, msg_size);
	bytes_read += msg_size;
	cerr << "    bytes_read: " << bytes_read;

	// print message data
	cerr << "\n    Data was:" << endl << "     ";
	for (int i = 1;i <= msg_size + LTP_HDR_SIZE;i++) {
	  if (ltp_buffer[i-1] < 0x10) cerr << " 0" << hex << (unsigned short)ltp_buffer[i-1];
	  else cerr << " " << hex << (unsigned short)ltp_buffer[i-1];
	  if (!(i%19)) cerr << "\n    ";
	}
	cerr << dec << endl << endl;

	if ((ltp_buffer[0] == LTP_SEG_NONE) || (ltp_buffer[0] == LTP_SEG_LAST)) {
	  cerr << "    breaking out" << endl;
	  break;
	}
      }
      
      cout << "    copying data" << endl;
      // copy requested amount of data from internal to given buffer
      memcpy(_b, buf_the_dragon, len);
      scales = bytes_read;
      tail = len;
      if (tail == scales) buffer_empty = TRUE;
      else buffer_empty = FALSE;
    }

    // there was some data left in the internal buffer
    else {
      memcpy(_b, buf_the_dragon + tail, len);
      tail += len;
      if (tail == scales) buffer_empty = TRUE;
    }

    // by default we did succesfully read what was requested
    cerr << "    return value    : " << len << endl;
    block(blocking);
    return len;
  
  #else
    // read from the socket if internal buffer is empty
    if (buffer_empty) {
      block(FALSE);

      while(1) {
	r = OSNet::sock_read(fd, ltp_buffer, LTP_BUF);

	// nothing to read
	if (r == 0) {
	  ateof = TRUE;
	  block(blocking);
	  return 0;
	}

	// error while reading
	else if (r < 0) {
	  OSNet::set_errno();
	  block(blocking);
	  if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN) return 0;
	  err = xstrerror(errno);
	  return r;
	}

	if (ltp_buffer[0] == LTP_SEG_FIRST) block(TRUE);
	else if (ltp_buffer[0] == LTP_SEG_LAST) block(FALSE);

	memcpy(&msg_size, ltp_buffer + 1, sizeof(uint16_t));
	memcpy(buf_the_dragon + bytes_read, ltp_buffer + LTP_HDR_SIZE, msg_size);
	bytes_read += msg_size;
	if ((ltp_buffer[0] == LTP_SEG_NONE) || (ltp_buffer[0] == LTP_SEG_LAST)) break;
      }
    
      // copy requested amount of data from internal to given buffer
      memcpy(_b, buf_the_dragon, len);
      scales = bytes_read;
      tail = len;
      if (tail == scales) buffer_empty = TRUE;
      else buffer_empty = FALSE;
    }
    
    // there was some data left in the internal buffer
    else {
      memcpy(_b, buf_the_dragon + tail, len);
      tail += len;
      if (tail == scales) buffer_empty = TRUE;
    }

    // by default we did succesfully read what was requested
    block(blocking);
    return len;
  #endif
}


/*******************************************************************************
* CORBA::Long MICOLTP::LTPTransport::write()
*
* Returns local address.
*
* input parameters:
*   const void *_b                : buffer to write
*   CORBA::Long len               : the length of the buffer
*
* return value: the number of bytes written
*******************************************************************************/

CORBA::Long MICOLTP::LTPTransport::write(const void *_b, CORBA::Long len) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::write" << endl;
  #endif

  CORBA::Long todo = len;
  CORBA::Octet *b = (CORBA::Octet *)_b;
  CORBA::Long written = 0;
  CORBA::Long r;
  uint16_t msg_size = 0;

  // write all data in the buffer
  while (todo > 0) {

    while (written < len) {
      if (len <= (LTP_BUF - LTP_HDR_SIZE)) {
        ltp_buffer[0] = LTP_SEG_NONE;
        msg_size = len;
      } else if ((len - written) <= (LTP_BUF - LTP_HDR_SIZE)) {
        ltp_buffer[0] = LTP_SEG_LAST;
        msg_size = len - written;
      } else if (written == 0) {
        ltp_buffer[0] = LTP_SEG_FIRST;
        msg_size = LTP_BUF - LTP_HDR_SIZE;
      } else {
        ltp_buffer[0] = LTP_SEG_MIDDLE;
        msg_size = LTP_BUF - LTP_HDR_SIZE;
      }

      memcpy(ltp_buffer + 1, &msg_size, sizeof(uint16_t));
      memcpy(ltp_buffer + LTP_HDR_SIZE,  b + written, msg_size);
      r = OSNet::sock_write(fd, ltp_buffer, (msg_size + LTP_HDR_SIZE));

      written += msg_size;

      #ifdef LTP_DEBUG
        // print message data
        cerr << "    written   : " << written << endl; 
        cerr << "    length    : " << msg_size << endl;
        cerr << "    data was  :" << endl << "     ";
        for (int i = 1;i <= r;i++) {
	  if (ltp_buffer[i-1] < 0x10) cerr << " 0" << hex << (unsigned short)ltp_buffer[i-1];
	  else cerr << " " << hex << (unsigned short)ltp_buffer[i-1];
	  if (!(i%19)) cerr << "\n    ";
        }
		
        cerr << dec << endl << endl;
      #endif
    }

    // error while writing
    if (r < 0) {
      OSNet::set_errno();
      if (errno == EINTR) continue;
      err = xstrerror(errno);
      return r;
    }

    // all written
    else if (r == 0) {
      break;
    }

    b += written;
    todo -= written;
  }

  // return the number of bytes written
  return len - todo;
}


/*******************************************************************************
* CORBA::Address* MICOLTP::LTPTransport::addr()
*
* Returns local address.
*
* input parameters:
*   none
*
* return value: pointer to an address containing the local address
*******************************************************************************/

const CORBA::Address* MICOLTP::LTPTransport::addr() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::addr" << endl;
  #endif

  struct sockaddr_l2 sin;
  socket_size_t sz = sizeof(sin);

  CORBA::Long r = ::getsockname(fd, (socket_addr_t)&sin, &sz);
  if (r < 0) {
    OSNet::set_errno();
    err = xstrerror(errno);
    return 0;
  }

  local_addr.sockaddr(&sin);
  return &local_addr;
}


/*******************************************************************************
* CORBA::Address* LTPTransport::peer()
*
* Gets the name of the peer connected to the transport.
*
* input parameters:
*   none
*
* return value: struct containing the name of the connected peer
*******************************************************************************/

const CORBA::Address* MICOLTP::LTPTransport::peer() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::peer" << endl;
  #endif

  struct sockaddr_l2 sin;
  socket_size_t sz = sizeof(sin);

  CORBA::Long r = ::getpeername(fd, (socket_addr_t)&sin, &sz);
  if (r < 0) {
    OSNet::set_errno();
    err = xstrerror(errno);
  }
  else {
    peer_addr.sockaddr(&sin);
  }

  return &peer_addr;
}


/*******************************************************************************
* CORBA::Boolean LTPTransport::bad()
*
* Returns true if an error message is set.
*
* input parameters:
*   none
*
* return value: TRUE if an error message is set
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::bad() const {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::bad" << endl;
  #endif

  return err.length() > 0;
}


/*******************************************************************************
* string MICOLTP::LTPTransport::eof()
*
* Checks if the EOF of current transport has been reached.
*
* input parameters:
*   none
*
* return value: TRUE if EOF, otherwise FALSE
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransport::eof() const {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::eof" << endl;
    cerr << "    EOF: " << (bool)ateof << endl;
  #endif

  return ateof;
}


/*******************************************************************************
* string MICOLTP::LTPTransport::errormsg()
*
* Returns the last error message.
*
* input parameters:
*   none
*
* return value: string containing an error message
*******************************************************************************/

string MICOLTP::LTPTransport::errormsg() const {
  #ifdef LTP_DEBUG
    cerr << "LTPTransport::LTPTransportServer" << endl;
  #endif

  return err;
}

/*******************************************************************************
*                          LTPTransportServer class                            *
*******************************************************************************/


/*******************************************************************************
* MICOLTP::LTPTransportServer::LTPTransportServer()
*
* Constructor, initializes a socket and dispatchers.
*
* input parameters:
*   none
*
* return value: Check from the "Learn C++ in 21 days" :-)
*******************************************************************************/

MICOLTP::LTPTransportServer::LTPTransportServer() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::LTPTransportServer" << endl;
  #endif

  // create a socket
  OSNet::sock_init();
  fd = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
  assert (fd >= 0);

  // set initial values
  is_blocking = TRUE;
  OSNet::sock_reuse(fd, TRUE);
  listening = FALSE;
  adisp = 0;
  acb = 0;
}


/*******************************************************************************
* MICOLTP::LTPTransportServer::~LTPTransportServer()
*
* Destructor, removes dispatchers and closes a socket.
*
* input parameters:
*   none
*
* return value: figure out by yourself...
*******************************************************************************/

MICOLTP::LTPTransportServer::~LTPTransportServer() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::~LTPTransportServer" << endl;
  #endif

  // remove both read and write dispatchers
  if (adisp && acb) {
    adisp->remove (this, CORBA::Dispatcher::Read);
    adisp = 0;
    acb->callback (this, CORBA::TransportServerCallback::Remove);
  }

  OSNet::sock_close (fd);
}


/*******************************************************************************
* void MICOLTP::LTPTransportServer::listen()
*
* Listens for a connection.
*
* input parameters:
*   none
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransportServer::listen() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::listen" << endl;
  #endif

  if (!listening) {
//    int r = ::listen(fd, 10);
//    assert(r == 0);
    ::listen(fd, 10);
    listening = TRUE;
  }
}


/*******************************************************************************
* void MICOLTP::LTPTransportServer::aselect()
*
* Selects a dispatcher.
*
* input parameters:
*   CORBA::Dispatcher *disp             :
*   CORBA::TransportServerCallback *cb  :
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransportServer::aselect(CORBA::Dispatcher *disp,
  CORBA::TransportServerCallback *cb) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::aselect" << endl;
  #endif

  if (acb && adisp) {
    adisp->remove(this, CORBA::Dispatcher::Read);
    adisp = 0;
    acb = 0;
  }

  if (cb) {
    listen();
    disp->rd_event(this, fd);
    adisp = disp;
    acb = cb;
  }
}


/*******************************************************************************
* void MICOLTP::LTPTransportServer::callback()
*
* Handles different callback events.
*
* input parameters:
*   CORBA::Dispatcher *disp       : new dispatcher to attach to
*   CORBA::Dispatcher::Event ev   : event to handle
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransportServer::callback(CORBA::Dispatcher *disp,
  CORBA::Dispatcher::Event ev) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::callback" << endl;
  #endif

  switch (ev) {
    case CORBA::Dispatcher::Read:
      assert (acb);
      acb->callback(this, CORBA::TransportServerCallback::Accept);
      break;
    case CORBA::Dispatcher::Remove:
      acb = 0;
      adisp = 0;
      break;
    case CORBA::Dispatcher::Moved:
      adisp = disp;
      break;
    default:
      assert (0);
  }
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransportServer::bind()
*
* Binds a socket.
*
* input parameters:
*   CORBA::Address *a             : address that the socket is binded to
*
* return value: TRUE if success, otherwise FALSE
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransportServer::bind (const CORBA::Address *a) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::bind" << endl;
  #endif

  assert(!strcmp (a->proto(), "bt-l2cap"));
  LTPAddress *ia = (LTPAddress *)a;

  struct sockaddr_l2 sin;
  ia->getsockaddr(&sin);

  #ifdef LTP_DEBUG
    string bt_host = ia->bthost();
    CORBA::UShort psm = ia->psm();
    cerr << "    BD_ADDR : " << bt_host << endl;
    cerr << "    PSM     : " << psm << endl;
  #endif

  // bind the socket
  CORBA::Long r = ::bind(fd, (socket_addr_t)&sin, sizeof(sin));
  if (r < 0) {
    OSNet::set_errno();
    err = xstrerror(errno);
    return FALSE;
  }

  return TRUE;
}


/*******************************************************************************
* void MICOLTP::LTPTransportServer::close()
*
* Closes the socket and removes (read)dispatcher.
*
* input parameters:
*   none
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransportServer::close() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::close" << endl;
  #endif

  // close the socket and reuse it
  OSNet::sock_close (fd);
  fd = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
  assert(fd >= 0);

  is_blocking = TRUE;
  OSNet::sock_reuse (fd, TRUE);

  // remove the dispatcher
  if (adisp && acb) adisp->remove(this, CORBA::Dispatcher::Read);

  // set initial values
  listening = FALSE;
  adisp = 0;
  acb = 0;
}


/*******************************************************************************
* void MICOLTP::LTPTransportServer::block()
*
* Tells the server to block / unblock.
*
* input parameters:
*   CORBA::Boolean doblock        : TRUE = block, FALSE = unblock
*
* return value: none
*******************************************************************************/

void MICOLTP::LTPTransportServer::block(CORBA::Boolean doblock) {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::block" << endl;
  #endif

  if (!!is_blocking != !!doblock) {
    is_blocking = doblock;
    OSNet::sock_block(fd, doblock);
  }
}


/*******************************************************************************
* CORBA::Boolean MICOLTP::LTPTransportServer::isblocking()
*
* Checks if the server is blocking.
*
* input parameters:
*   none
*
* return value: TRUE if the server is blocking
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransportServer::isblocking() const {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::isblocking" << endl;
  #endif

  return is_blocking;
}


/*******************************************************************************
* CORBA::Transport* MICOLTP::LTPTransportServer::accept()
*
* Accepts a new connection.
*
* input parameters:
*   none
*
* return value: new LTPTransport object if success, otherwise returns 0
*******************************************************************************/

CORBA::Transport* MICOLTP::LTPTransportServer::accept() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::accept" << endl;
  #endif

  listen();
  CORBA::Long newfd = ::accept(fd, 0, 0);
  if (newfd < 0) {
    OSNet::set_errno();
    if (errno != EWOULDBLOCK && errno != EAGAIN) err = xstrerror(errno);
    return 0;
  }

  return new LTPTransport(newfd);
}


/*******************************************************************************
* CORBA::Address* MICOLTP::LTPTransportServer::addr()
*
* Returns local address.
*
* input parameters:
*   none
*
* return value: pointer to a struct containing a local address
*******************************************************************************/

const CORBA::Address* MICOLTP::LTPTransportServer::addr() {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::addr" << endl;
  #endif

  struct sockaddr_l2 sin;
  socket_size_t sz = sizeof(sin);

  CORBA::Long r = ::getsockname(fd, (socket_addr_t)&sin, &sz);
  if (r < 0) {
    OSNet::set_errno();
    err = xstrerror(errno);
    return 0;
  }

  local_addr.sockaddr(&sin);

  return &local_addr;
}


/*******************************************************************************
* CORBA::Boolean LTPTransportServer::bad()
*
* Returns true if an error message is set.
*
* input parameters:
*   none
*
* return value: TRUE if an error message is set
*******************************************************************************/

CORBA::Boolean MICOLTP::LTPTransportServer::bad() const {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::bad" << endl;
  #endif

  return err.length() > 0;
}


/*******************************************************************************
* string MICOLTP::LTPTransportServer::errormsg()
*
* Returns the last error message.
*
* input parameters:
*   none
*
* return value: string containing an error message
*******************************************************************************/

string MICOLTP::LTPTransportServer::errormsg() const {
  #ifdef LTP_DEBUG
    cerr << "LTPTransportServer::errormsg" << endl;
  #endif

  return err;
}
