// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

#ifndef __mico_transport_udp_h__
#define __mico_transport_udp_h__

namespace MICO {

class UDPTransport : public SocketTransport {

    InetAddress local_addr, peer_addr;
    CORBA::Buffer dgram;
    CORBA::Boolean is_established;
    CORBA::Boolean is_connected;
    CORBA::Boolean is_bound;
    struct sockaddr_in *peer_sin, *addr_sin;

    CORBA::Long collect_replies (CORBA::Long tmout);
public:
    UDPTransport()
    {}

    CORBA::Boolean bind (const CORBA::Address *);
    CORBA::Boolean connect (const CORBA::Address *);

    void open(CORBA::Long fd = -1);
    void close ();
    
    CORBA::Long read (void *, CORBA::Long len);
    CORBA::Long write (const void *, CORBA::Long len);
    
    const CORBA::Address *addr ();
    const CORBA::Address *peer ();
    
    CORBA::Boolean bind_server (const CORBA::Address *);
    CORBA::Boolean connect_server (const CORBA::Address *);
    CORBA::Long read_dgram (CORBA::Buffer &buf);
};


class UDPTransportServer : public SocketTransportServer {

    InetAddress local_addr;
    CORBA::Boolean is_bound;
    struct sockaddr_in *addr_sin;

    void __clean_up();
public:
    UDPTransportServer ();
    
    CORBA::Boolean bind (const CORBA::Address *);
    void close ();
    
    CORBA::Transport *accept ();
    const CORBA::Address *addr ();
    
};

}

#endif // __mico_transport_udp_h__
