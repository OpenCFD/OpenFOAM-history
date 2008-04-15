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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <string.h>
#include <errno.h>
#include <mico/os-net.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

/**************************** misc dtors *******************************/


CORBA::TransportCallback::~TransportCallback ()
{
}

CORBA::TransportServer::~TransportServer ()
{
}

CORBA::TransportServerCallback::~TransportServerCallback ()
{
}


/*************************** Transport *******************************/


CORBA::Transport::~Transport ()
{
}

CORBA::Long
CORBA::Transport::read (Buffer &b, Long len)
{
    b.resize (len);
    Long r = read (b.buffer()+b.wpos(), len);
    if (r > 0)
	b.wseek_rel (r);
    return r;
}

CORBA::Long
CORBA::Transport::write (Buffer &b, Long len, Boolean eat)
{
    assert (len >= 0);
    assert ((CORBA::ULong)len <= b.length());
    Long r = write (b.buffer()+b.rpos(), len);
    if (r > 0 && eat)
	b.rseek_rel (r);
    return r;
}

void
CORBA::Transport::buffering (CORBA::Boolean)
{
}

CORBA::Boolean
CORBA::Transport::isbuffering ()
{
    return FALSE;
}

CORBA::Principal_ptr
CORBA::Transport::get_principal ()
{
    return new CORBA::Principal (this);
}

/*********************** SocketTransport *****************************/

MICO::SocketTransport::SocketTransport ()
{
    state = Closed;
    fd = -1;
    rdisp = wdisp = 0;
    rcb = wcb = 0;
}

MICO::SocketTransport::~SocketTransport ()
{
    //FIXME: shouldn't the Remove callback be done by close ?
    //       it appears that the use of rcb::Remove etc. is inconsistent
    //                                   (on the other hand - maybe i am :-)
    if (rdisp && rcb) {
	rdisp->remove (this, CORBA::Dispatcher::Read);
	rdisp = 0;
	rcb->callback (this, CORBA::TransportCallback::Remove);
    }
    if (wdisp && wcb) {
	wdisp->remove (this, CORBA::Dispatcher::Write);
	wdisp = 0;
	wcb->callback (this, CORBA::TransportCallback::Remove);
    }
    close();
}

//void
//MICO::SocketTransport::setown( MICOMT::Thread::ThreadID id )
//{
//    OSNet::sock_setown(fd, id );
//}

void
MICO::SocketTransport::open (CORBA::Long thefd)
{
    fd = thefd;

    is_blocking = TRUE;
    is_buffering = FALSE;
    ateof = FALSE;
}

void
MICO::SocketTransport::close ()
{
    state = Closed;
    fd = -1;

    if (rdisp && rcb)
	rdisp->remove (this, CORBA::Dispatcher::Read);
    
    rdisp = 0;
    rcb = 0;
    if (wdisp && wcb)
	wdisp->remove (this, CORBA::Dispatcher::Write);
    
    wdisp = 0;
    wcb = 0;
}

void
MICO::SocketTransport::rselect (CORBA::Dispatcher *disp,
			     CORBA::TransportCallback *cb)
{
    if (rcb && rdisp) {
	rdisp->remove (this, CORBA::Dispatcher::Read);
	rdisp = 0;
	rcb = 0;
    }
    if (cb) {
	disp->rd_event (this, fd);
	rdisp = disp;
	rcb = cb;
    }
}

void
MICO::SocketTransport::wselect (CORBA::Dispatcher *disp,
			     CORBA::TransportCallback *cb)
{
    if (wcb && wdisp) {
	wdisp->remove (this, CORBA::Dispatcher::Write);
	wdisp = 0;
	wcb = 0;
    }
    if (cb) {
	disp->wr_event (this, fd);
	wdisp = disp;
	wcb = cb;
    }
}

void
MICO::SocketTransport::callback (CORBA::Dispatcher *disp,
			      CORBA::Dispatcher::Event ev)
{
    switch (ev) {
    case CORBA::Dispatcher::Read:
	assert (rcb);
	rcb->callback (this, CORBA::TransportCallback::Read);
	break;
    case CORBA::Dispatcher::Write:
	assert (wcb);
	wcb->callback (this, CORBA::TransportCallback::Write);
	break;
    case CORBA::Dispatcher::Remove:
	wdisp = rdisp = 0;
	wcb = rcb = 0;
	break;
    case CORBA::Dispatcher::Moved:
        wdisp = rdisp = disp;
        break;
    default:
	assert (0);
    }
}

void
MICO::SocketTransport::block (CORBA::Boolean doblock)
{
    if (!!is_blocking != !!doblock) {
	is_blocking = doblock;
	OSNet::sock_block (fd, doblock);
    }
}

CORBA::Boolean
MICO::SocketTransport::isblocking ()
{
    return is_blocking;
}

void
MICO::SocketTransport::buffering (CORBA::Boolean dobuffering)
{
    if (!!is_buffering != !!dobuffering) {
	is_buffering = dobuffering;
	OSNet::sock_ndelay (fd, !dobuffering);
    }
}

CORBA::Boolean
MICO::SocketTransport::isbuffering ()
{
    return is_buffering;
}

CORBA::Boolean
MICO::SocketTransport::isreadable ()
{
#ifdef HAVE_POLL_H
  if (fd < FD_SETSIZE) {
#endif // HAVE_POLL_H
    fd_set rset;
    struct timeval tm;

    FD_ZERO (&rset);
    FD_SET (fd, &rset);

    tm.tv_sec = 0;
    tm.tv_usec = 0;

    int r = ::select (fd+1, (select_addr_t)&rset, 0, 0, &tm);
    return r > 0;
#ifdef HAVE_POLL_H
  } else {
    struct pollfd ps;
    ps.fd       = fd;
    ps.events   = POLLIN | POLLERR | POLLHUP;

    int r = ::poll(&ps, 1, 0);
    return r > 0;
  }
#endif // HAVE_POLL_H
}

CORBA::Boolean
MICO::SocketTransport::bad () const
{
    return err.length() > 0;
}

CORBA::Boolean
MICO::SocketTransport::eof () const
{
    return ateof;
}

string
MICO::SocketTransport::errormsg () const
{
    return err;
}

/******************** SocketTransportServer **************************/

MICO::SocketTransportServer::SocketTransportServer ()
{
#ifdef HAVE_THREADS
    __NAME( name( "SocketTransportServer" ) );
#endif // HAVE_THREADS
    listening = FALSE;

    adisp = 0;
    acb = 0;
}

void
MICO::SocketTransportServer::__clean_up () { }

MICO::SocketTransportServer::~SocketTransportServer ()
{
    if (adisp && acb) {
	adisp->remove (this, CORBA::Dispatcher::Read);
	adisp = 0;
	acb->callback (this, CORBA::TransportServerCallback::Remove);
    }
    OSNet::sock_shutdown(fd);
    OSNet::sock_close (fd);
#ifdef HAVE_THREADS
    this->finalize_shutdown();
#endif // HAVE_THREADS
}

void
MICO::SocketTransportServer::aselect (CORBA::Dispatcher *disp,
				      CORBA::TransportServerCallback *cb)
{
    if (acb && adisp) {
	adisp->remove (this, CORBA::Dispatcher::Read);
	adisp = 0;
	acb = 0;
    }
    if (cb) {
	listen ();
	disp->rd_event (this, fd);
	adisp = disp;
	acb = cb;
    }
}

void
MICO::SocketTransportServer::callback (CORBA::Dispatcher *disp,
				       CORBA::Dispatcher::Event ev)
{
    switch (ev) {
    case CORBA::Dispatcher::Read:
	assert (acb);
	acb->callback (this, CORBA::TransportServerCallback::Accept);
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

CORBA::Boolean
MICO::SocketTransportServer::isblocking () const
{ 
  return is_blocking;
}

void
MICO::SocketTransportServer::block (CORBA::Boolean doblock)
{
    if (!!is_blocking != !!doblock) {
	is_blocking = doblock;
	OSNet::sock_block (fd, doblock);
    }
}


CORBA::Boolean
MICO::SocketTransportServer::bad () const
{
    return err.length() > 0;
}

string
MICO::SocketTransportServer::errormsg () const
{
    return err;
}

void
MICO::SocketTransportServer::listen () { }

#ifdef HAVE_THREADS

MICO::Operation *
MICO::SocketTransportServer::copy() const {

    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "OP *MICO::SocketTransportServer::copy()" << endl;
    }
    return NULL;
} 

void
MICO::SocketTransportServer::_run() {

    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "void MICO::SocketTransportServer::_run()" << endl;
    }
    while ( acb ) {
	acb->callback( this, CORBA::TransportServerCallback::Accept );
    };

    // Thread()->deregisterOperation( this );
    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "after void MICO::SocketTransportServer::_run()" << endl;
    }
}

void
MICO::SocketTransportServer::create_thread() {

    // create a new thread
    // there is no limit on how much TransportServer we can have
    //   each on runs in its own thread !!

    WorkerThread *kt = MICO::MTManager::thread_pool_manager()->get_thread_pool( MICO::Operation::Accept ).get_idle_thread();
    kt->register_operation( this );
    kt->mark_busy();
}

void
MICO::SocketTransportServer::start() {

    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "void MICO::SocketTransportServer::start() -before" << endl;
    }
    //    Thread().start();
    this->thread()->post_state_change();
    if (MICO::Logger::IsLogged (MICO::Logger::Transport)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Transport)
	    << "void MICO::SocketTransportServer::start() -after" << endl;
    }
}

#endif // HAVE_THREADS
