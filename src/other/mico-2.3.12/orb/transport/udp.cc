/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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
#include <mico/os-net.h>
#include <mico/impl.h>
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__


/************************** UDPTransport *****************************/

#define UDP_MAGIC_CREQ ((char *)"CREQ-EjAQBgNVBAcTCUZyYW5rZnVyd")
#define UDP_MAGIC_CREP ((char *)"CREP-EjAQBgNVBAcTCUZyYW5rZnVyd")
#define UDP_MAGIC_SIZE 30

CORBA::Boolean
MICO::UDPTransport::bind (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "inet-dgram"));
    InetAddress *ia = (InetAddress *)a;

    // remember the bind address, since some systems will
    // return a different address from getsockname() when
    // we bind to a broadcast address
    *addr_sin = ia->sockaddr();
    is_bound = TRUE;

    CORBA::Long r = ::bind (fd, (socket_addr_t)addr_sin, sizeof (*addr_sin));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::UDPTransport::bind_server (const CORBA::Address *a)
{
    // server sockets can reuse port numbers
    OSNet::sock_reuse (fd, TRUE);
    return bind (a);
}

CORBA::Long
MICO::UDPTransport::collect_replies (CORBA::Long tmout)
{
    CORBA::Buffer b;
    CORBA::Long replies = 0;
    while (42) {
        int r;
#ifdef HAVE_POLL_H
        if (fd < FD_SETSIZE) {
#endif // HAVE_POLL_H
            fd_set rset;
            struct timeval tm;

            FD_ZERO (&rset);
            FD_SET (fd, &rset);

            tm.tv_sec  = tmout / 1000L;
            tm.tv_usec = (tmout % 1000L) * 1000L;

            r = ::select (fd+1, (select_addr_t)&rset, 0, 0, &tm);
#ifdef HAVE_POLL_H
        } else {
            struct pollfd ps;
            ps.fd       = fd;
            ps.events   = POLLIN | POLLERR | POLLHUP;

            r = ::poll(&ps, 1, tmout);
        }
#endif // HAVE_POLL_H
	if (r == 0) {
	    // timeout
	    break;
	}
	if (r < 0) {
            OSNet::set_errno();
	    if (errno == EINTR || errno == EAGAIN)
		continue;
	    assert (0);
	}
	r = read_dgram (b);
	if (r == 0)
	    continue;
	if (r != UDP_MAGIC_SIZE)
	    return 0;
	if (strncmp ((char *)b.buffer(), UDP_MAGIC_CREP, UDP_MAGIC_SIZE))
	    assert (0);
	++replies;
    }
    return replies;
}

CORBA::Boolean
MICO::UDPTransport::connect (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "inet-dgram"));
    InetAddress *ia = (InetAddress *)a;

    // do not really connect here, otherwise we dont get replies
    // if peer_addr is a broadcast or multicast address
    *peer_sin = ia->sockaddr();
    is_connected = TRUE;

    CORBA::Long retries = 5;
    while (--retries >= 0) {
	if (write (UDP_MAGIC_CREQ, UDP_MAGIC_SIZE) != UDP_MAGIC_SIZE)
	    assert (0);
	if (collect_replies (300 /*ms*/) > 0)
	    break;
    }
    if (retries < 0) {
	err = "timeout";
	is_connected = FALSE;
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::UDPTransport::connect_server (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "inet-dgram"));
    InetAddress *ia = (InetAddress *)a;

    struct sockaddr_in sin = ia->sockaddr();
    CORBA::Long r = ::connect (fd, (socket_addr_t)&sin, sizeof (sin));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

void
MICO::UDPTransport::open (CORBA::Long thefd)
{
    assert (state == Closed);

    if (thefd < 0) {
	thefd = ::socket (PF_INET, SOCK_DGRAM, 0);
	assert (thefd >= 0);
    }
    SocketTransport::open( thefd );
    OSNet::sock_block (thefd, TRUE);
    OSNet::sock_broadcast (thefd, TRUE);
    OSNet::sock_reuse(thefd, TRUE);

    is_blocking = TRUE;
    is_established = FALSE;
    is_connected = FALSE;
    is_bound = FALSE;
    peer_sin = new sockaddr_in;
    addr_sin = new sockaddr_in;

    state = Open;
}

void
MICO::UDPTransport::close ()
{
    if (state != Open) 
	return;

    state = Closed;
    OSNet::sock_shutdown(fd);
    OSNet::sock_close (fd);

    SocketTransport::close();

    delete peer_sin;
    delete addr_sin;
}

CORBA::Long
MICO::UDPTransport::read_dgram (CORBA::Buffer &buf)
{
    // XXX somehow get real dgram size ...
    CORBA::ULong dgram_size = 10000;
    buf.reset (dgram_size);
	
    while (42) {
	CORBA::Long r = OSNet::sock_read (fd, buf.buffer(), dgram_size);
	if (r < 0) {
            OSNet::set_errno();
	    if (state != Open)
		return r;
	    if (errno == EINTR)
		continue;
	    // Cygnus CDK sometimes returns errno 0 when read would block
	    if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN)
		return 0;
	    err = xstrerror (errno);
	    return r;
	}
	buf.wseek_rel (r);
	return r;
    }
    // notreached
    return 0;
}

CORBA::Long
MICO::UDPTransport::read (void *b, CORBA::Long len)
{
    assert(len > 0);
    if (dgram.length() == 0) {
	while (42) {
	    CORBA::Long r = read_dgram (dgram);
	    if (r <= 0)
		return r;
	    if (is_established || r != UDP_MAGIC_SIZE)
		break;
	    if (!strncmp((char*)dgram.buffer(),UDP_MAGIC_CREP,UDP_MAGIC_SIZE))
		// ignore late connect replies
		continue;
	    if (!strncmp((char*)dgram.buffer(),UDP_MAGIC_CREQ,UDP_MAGIC_SIZE)){
		// peer resent connect request, but reply was lost
		if (write (UDP_MAGIC_CREP, UDP_MAGIC_SIZE) != UDP_MAGIC_SIZE)
		    assert (0);
		continue;
	    }
	    break;
	}
	is_established = TRUE;
    }
    // here we can cast because of assert(len > 0) above
    if (((CORBA::ULong)len) > dgram.length())
	len = dgram.length();
    if (!dgram.get (b, len))
	assert (0);

    return len;
}

CORBA::Long
MICO::UDPTransport::write (const void *_b, CORBA::Long len)
{
    CORBA::Octet *b = (CORBA::Octet *)_b;

    while (42) {
	CORBA::Long r;
	if (is_connected) {
	    r = OSNet::sock_write_to (fd, b, len,
				      (struct sockaddr *)peer_sin,
				      sizeof (*peer_sin));
	} else {
	    r = OSNet::sock_write (fd, b, len);
	}
	if (r < 0) {
            OSNet::set_errno();
	    if (state != Open)
		return r;
	    if (errno == EINTR)
		continue;
            // Cygnus CDK sometimes returns errno 0 when read would block
	    if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN)
		return 0;
	    err = xstrerror (errno);
	}
	return r;
    }
    // notreached
    return 0;
}

const CORBA::Address *
MICO::UDPTransport::addr ()
{
    struct sockaddr_in sin;
    socket_size_t sz = sizeof (sin);
    CORBA::Long r = ::getsockname (fd, (socket_addr_t)&sin, &sz);
    if (r < 0) {
        OSNet::set_errno();
        err = xstrerror (errno);
        return 0;
    }
    local_addr.family (MICO::InetAddress::DGRAM);
    if (is_bound) {
        local_addr.sockaddr (*addr_sin);
        local_addr.port (ntohs (sin.sin_port));
    } else {
        local_addr.sockaddr (sin);
    }
    return &local_addr;
}

const CORBA::Address *
MICO::UDPTransport::peer()
{
    if (is_connected) {
	peer_addr.family (MICO::InetAddress::DGRAM);
        peer_addr.sockaddr (*peer_sin);
    } else {
	struct sockaddr_in sin;
	socket_size_t sz = sizeof (sin);
	CORBA::Long r = ::getpeername (fd, (socket_addr_t)&sin, &sz);
	if (r < 0) {
            OSNet::set_errno();
	    err = xstrerror (errno);
	    return 0;
	} else {
	    peer_addr.family (MICO::InetAddress::DGRAM);
	    peer_addr.sockaddr (sin);
	}
    }
    return &peer_addr;
}

/************************ UDPTransportServer **************************/

MICO::UDPTransportServer::UDPTransportServer ()
{
    OSNet::sock_init();

    fd = ::socket (PF_INET, SOCK_DGRAM, 0);
    assert (fd >= 0);

    OSNet::sock_reuse (fd, TRUE);

    is_blocking = TRUE;
    is_bound = FALSE;

    addr_sin = new sockaddr_in;
}

void
MICO::UDPTransportServer::__clean_up ()
{
    delete addr_sin;
}

CORBA::Boolean
MICO::UDPTransportServer::bind (const CORBA::Address *a)
{
    assert (!strcmp (a->proto(), "inet-dgram"));
    InetAddress *ia = (InetAddress *)a;

    // remember the bind address, since some systems will
    // return a different address from getsockname() when
    // we bind to a broadcast address
    *addr_sin = ia->sockaddr();
    is_bound = TRUE;

    CORBA::Long r = ::bind (fd, (socket_addr_t)addr_sin, sizeof (*addr_sin));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

void
MICO::UDPTransportServer::close ()
{
    OSNet::sock_close (fd);
    fd = ::socket (PF_INET, SOCK_DGRAM, 0);
    assert (fd >= 0);

    OSNet::sock_reuse (fd, TRUE);

    is_blocking = TRUE;
    is_bound = FALSE;

    remove_aselect();
}

CORBA::Transport *
MICO::UDPTransportServer::accept ()
{
    struct sockaddr_in sin;
    CORBA::Octet b[UDP_MAGIC_SIZE+1];
    CORBA::Long r = OSNet::sock_read_from (fd, b, UDP_MAGIC_SIZE+1,
					   (struct sockaddr *)&sin,
					   sizeof (sin));
    if (r < 0) {
        OSNet::set_errno();
	if (errno == EINTR)
	    return 0;
	// Cygnus CDK sometimes returns errno 0 when read would block
	if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN)
	    return 0;
	err = xstrerror (errno);
	return 0;
    } else if (r == 0) {
	return 0;
    }

    // we will also receive data broadcasts/multicasts, ignore them
    // if they are not connect requests ...
    if (r != UDP_MAGIC_SIZE)
	return 0;
    if (strncmp ((char *)b, UDP_MAGIC_CREQ, UDP_MAGIC_SIZE))
	return 0;

    UDPTransport *t = new UDPTransport;
    t->open();
    if (!t->bind_server (this->addr()))
	assert (0);

    InetAddress raddr (sin, InetAddress::DGRAM);
    if (!t->connect_server (&raddr))
	assert (0);

    if (t->write (UDP_MAGIC_CREP, UDP_MAGIC_SIZE) != UDP_MAGIC_SIZE)
	assert (0);

    return t;
}

const CORBA::Address *
MICO::UDPTransportServer::addr ()
{
    struct sockaddr_in sin;
    socket_size_t sz = sizeof (sin);
    CORBA::Long r = ::getsockname (fd, (socket_addr_t)&sin, &sz);
    if (r < 0) {
        OSNet::set_errno();
        err = xstrerror (errno);
        return 0;
    }
    local_addr.family (MICO::InetAddress::DGRAM);
    if (is_bound) {
	local_addr.sockaddr(*addr_sin);
        local_addr.port (ntohs (sin.sin_port));
    } else {
        local_addr.sockaddr (sin);
    }
    return &local_addr;
}

