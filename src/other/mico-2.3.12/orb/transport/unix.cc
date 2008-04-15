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


/************************** UnixTransport ****************************/


#ifdef HAVE_SYS_UN_H
CORBA::Boolean
MICO::UnixTransport::bind (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "unix"));
    UnixAddress *ua = (UnixAddress *)a;

    // XXX should do that after the socket is destroyed ...
    ::unlink (ua->filename());

    struct sockaddr_un una = ua->sockaddr();
    CORBA::Long r = ::bind (fd, (socket_addr_t)&una, sizeof (una));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

CORBA::Boolean
MICO::UnixTransport::connect (const CORBA::Address *a)
{
    assert (state == Open);
    assert (!strcmp (a->proto(), "unix"));
    UnixAddress *ua = (UnixAddress *)a;

    struct sockaddr_un una = ua->sockaddr();
    CORBA::Long r = ::connect (fd, (socket_addr_t)&una, sizeof (una));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

void
MICO::UnixTransport::open (CORBA::Long thefd)
{
    assert (state == Closed);

    if (thefd < 0) {
	thefd = ::socket (PF_UNIX, SOCK_STREAM, 0);
	assert (thefd >= 0);
    }
    OSNet::sock_block (thefd, TRUE);

    SocketTransport::open( thefd );

    is_blocking = TRUE;
    state = Open;
}

void
MICO::UnixTransport::close ()
{
    if (state != Open) 
	return;

    state = Closed;

    OSNet::sock_shutdown(fd);
    OSNet::sock_close (fd);

    SocketTransport::close();
}

CORBA::Long
MICO::UnixTransport::read (void *_b, CORBA::Long len)
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
MICO::UnixTransport::write (const void *_b, CORBA::Long len)
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
MICO::UnixTransport::addr ()
{
    struct sockaddr_un una;
    socket_size_t sz = sizeof (una);
    CORBA::Long r = ::getsockname (fd, (socket_addr_t)&una, &sz);
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return 0;
    }
    local_addr.sockaddr (una);
    return &local_addr;
}

const CORBA::Address *
MICO::UnixTransport::peer ()
{
    struct sockaddr_un una;
    socket_size_t sz = sizeof (una);
    CORBA::Long r = ::getpeername (fd, (socket_addr_t)&una, &sz);
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
        // allow for peer() after disconnect ...
	//return 0;
    } else {
        peer_addr.sockaddr (una);
    }
    return &peer_addr;
}

/************************ UnixTransportServer **************************/


MICO::UnixTransportServer::UnixTransportServer ()
{
    fd = ::socket (PF_UNIX, SOCK_STREAM, 0);
    is_blocking = TRUE;
    assert (fd >= 0);
}

void
MICO::UnixTransportServer::listen ()
{
    if (!listening) {
	int r = ::listen (fd, 10);
	assert (r == 0);
	listening = TRUE;
    }
}

CORBA::Boolean
MICO::UnixTransportServer::bind (const CORBA::Address *a)
{
    assert (!strcmp (a->proto(), "unix"));
    UnixAddress *ua = (UnixAddress *)a;

    // XXX should do that after the socket is destroyed ...
    ::unlink (ua->filename());

    struct sockaddr_un una = ua->sockaddr();
    CORBA::Long r = ::bind (fd, (socket_addr_t)&una, sizeof (una));
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return FALSE;
    }
    return TRUE;
}

void
MICO::UnixTransportServer::close ()
{
    OSNet::sock_close (fd);
    fd = ::socket (PF_UNIX, SOCK_STREAM, 0);
    is_blocking = TRUE;
    assert (fd >= 0);

    listening = FALSE;
    remove_aselect();
}

CORBA::Transport *
MICO::UnixTransportServer::accept ()
{
    UnixTransport *ret;

    listen ();
    CORBA::Long newfd = ::accept (fd, 0, 0);
    if (newfd < 0) {
        OSNet::set_errno();
	if (errno != EWOULDBLOCK && errno != EAGAIN)
	    err = xstrerror (errno);
	return 0;
    }
    ret = new UnixTransport ();
    ret->open( newfd );
    return ret;
}

const CORBA::Address *
MICO::UnixTransportServer::addr ()
{
    struct sockaddr_un una;
    socket_size_t sz = sizeof (una);
    CORBA::Long r = ::getsockname (fd, (socket_addr_t)&una, &sz);
    if (r < 0) {
        OSNet::set_errno();
	err = xstrerror (errno);
	return 0;
    }
    local_addr.sockaddr (una);
    return &local_addr;
}

#endif // HAVE_SYS_UN_H
