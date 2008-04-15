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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <mico/os-net.h>
#include <mico/impl.h>
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__


using namespace std;

/************************** TCPTransport *****************************/

MICO::TCPTransport::~TCPTransport () {
    close();
}

CORBA::Boolean
MICO::TCPTransport::bind (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "inet"));
    InetAddress *ia = (InetAddress *)a;

    struct sockaddr_in sin = ia->sockaddr();
    CORBA::Long r = ::bind (fd, (socket_addr_t)&sin, sizeof (sin));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::TCPTransport::connect (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "inet"));
    InetAddress *ia = (InetAddress *)a;

    if (!ia->valid()) {
        err = "invalid address";
        return FALSE;
    }

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
MICO::TCPTransport::open (CORBA::Long thefd)
{
    assert (state == Closed);

    if (thefd < 0) {
	thefd = ::socket (PF_INET, SOCK_STREAM, 0);
	assert (thefd >= 0);
    } 
    OSNet::sock_ndelay (thefd, TRUE);

    SocketTransport::open( thefd );

    is_buffering = FALSE;

    // force blocking mode directly on fd
    is_blocking = FALSE;
    this->block(TRUE);
    state = Open;
}

void
MICO::TCPTransport::close ()
{
    if (state != Open) 
    	return;

    state = Closed;

    OSNet::sock_shutdown(fd);
    OSNet::sock_close (fd);

    SocketTransport::close();
}

CORBA::Long
MICO::TCPTransport::read (void *_b, CORBA::Long len)
{
    CORBA::Long todo = len;
    CORBA::Octet *b = (CORBA::Octet *)_b;

    while (todo > 0) {
	CORBA::Long r = OSNet::sock_read (fd, b, todo);
	if (r < 0) {
            OSNet::set_errno();
	    if (state != Open)
		return r;
	    if (errno == EINTR)
		continue;
            // Cygnus CDK sometimes returns errno 0 when read would block
	    if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN ||
                todo != len)
		break;
	    err = xstrerror (errno);
	    return r;
	} else if (r == 0) {
	    ateof = TRUE;
	    break;
	}
	b += r;
	todo -= r;
    }
    return len - todo;
}

CORBA::Long
MICO::TCPTransport::write (const void *_b, CORBA::Long len)
{
    CORBA::Long todo = len;
    CORBA::Octet *b = (CORBA::Octet *)_b;

    while (todo > 0) {
	CORBA::Long r = OSNet::sock_write (fd, b, todo);
	if (r < 0) {
            OSNet::set_errno();
	    if (state != Open)
		return r;
	    if (errno == EINTR)
		continue;
            // Cygnus CDK sometimes returns errno 0 when read would block
	    if (errno == 0 || errno == EWOULDBLOCK || errno == EAGAIN ||
                todo != len)
		break;
	    err = xstrerror (errno);
	    return r;
	} else if (r == 0) {
	    break;
	}
	b += r;
	todo -= r;
    }
    return len - todo;
}

const CORBA::Address *
MICO::TCPTransport::addr ()
{
    struct sockaddr_in sin;
    socket_size_t sz = sizeof (sin);
    CORBA::Long r = ::getsockname (fd, (socket_addr_t)&sin, &sz);
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return 0;
    }
    local_addr.sockaddr (sin);
    return &local_addr;
}

const CORBA::Address *
MICO::TCPTransport::peer ()
{
    struct sockaddr_in sin;
    socket_size_t sz = sizeof (sin);
    CORBA::Long r = ::getpeername (fd, (socket_addr_t)&sin, &sz);
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
        // XXX allow for peer() after disconnect ...
	//return 0;
    } else {
        peer_addr.sockaddr (sin);
    }
    return &peer_addr;
}

/************************ TCPTransportServer **************************/


MICO::TCPTransportServer::TCPTransportServer ()
{
    OSNet::sock_init();

    fd = ::socket (PF_INET, SOCK_STREAM, 0);
    assert (fd >= 0);

    // force blocking mode directly on fd
    is_blocking = FALSE;
    this->block(TRUE);

    OSNet::sock_reuse (fd, TRUE);
}

void
MICO::TCPTransportServer::listen ()
{
    if (!listening) {
	int r = ::listen (fd, 10);
	assert (r == 0);
	listening = TRUE;
    }
}

void
MICO::TCPTransportServer::close ()
{
#ifdef HAVE_THREADS
    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "MICO::TCPTransportServer::close ()" << endl;
    }
#endif // HAVE_THREADS
    remove_aselect();

    OSNet::sock_shutdown(fd);
    MICO_Long result = OSNet::sock_close (fd);
    assert (!result);

    fd = ::socket (PF_INET, SOCK_STREAM, 0);
    assert (fd >= 0);

    // force blocking mode directly on fd
    is_blocking = FALSE;
    this->block(TRUE);

    OSNet::sock_reuse (fd, TRUE);

    listening = FALSE;
}

CORBA::Boolean
MICO::TCPTransportServer::bind (const CORBA::Address *a)
{
    assert (!strcmp (a->proto(), "inet"));
    InetAddress *ia = (InetAddress *)a;

    struct sockaddr_in sin = ia->sockaddr();
    CORBA::Long r = ::bind (fd, (socket_addr_t)&sin, sizeof (sin));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

CORBA::Transport *
MICO::TCPTransportServer::accept ()
{
    TCPTransport *ret;
 
    listen ();

#if defined(HAVE_THREADS) && defined(HAVE_POLL_H)
    ::pollfd pfd;

    pfd.fd = fd;
    pfd.events = POLLIN | POLLOUT | POLLRDNORM | POLLRDBAND | POLLPRI | POLLHUP | POLLERR | POLLNVAL;

    if (poll (&pfd, 1, -1) < 0) {
	if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Transport)
		<< "TCPTransportServer::accept () return:" << errno << endl;
	}
	return 0;
    }
    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "TCPTransportServer::poll () return:" << pfd.revents << endl;
    }

#endif // HAVE_THREADS && HAVE_POLL_H

    CORBA::Long newfd = ::accept (fd, 0, 0);
    if (newfd < 0) {
        OSNet::set_errno();
	if (errno != EWOULDBLOCK && errno != EAGAIN)
	    err = xstrerror (errno);
	return 0;
    }
    ret = new TCPTransport ();
    ret->open( newfd );
    return ret;
}

const CORBA::Address *
MICO::TCPTransportServer::addr ()
{
    struct sockaddr_in sin;
    socket_size_t sz = sizeof (sin);
    CORBA::Long r = ::getsockname (fd, (socket_addr_t)&sin, &sz);
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return 0;
    }
    local_addr.sockaddr (sin);
    return &local_addr;
}

