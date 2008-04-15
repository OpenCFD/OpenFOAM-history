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

#ifndef __mico_transport_impl_h__
#define __mico_transport_impl_h__

namespace MICO {

class SocketTransport: public CORBA::Transport,
		       public CORBA::DispatcherCallback {
protected:
    CORBA::Dispatcher *rdisp, *wdisp;
    CORBA::TransportCallback *rcb, *wcb;
    CORBA::Long fd;
    CORBA::Boolean ateof;
    std::string err;
    State  state;

    CORBA::Boolean is_blocking;
    CORBA::Boolean is_buffering;

public:
    SocketTransport ();
    virtual ~SocketTransport ();

    virtual void open (CORBA::Long fd = -1);
    virtual void close ();

    virtual void rselect (CORBA::Dispatcher *, CORBA::TransportCallback *);
    virtual void wselect (CORBA::Dispatcher *, CORBA::TransportCallback *);
    virtual void callback (CORBA::Dispatcher *, CORBA::Dispatcher::Event);
 
    virtual void block (CORBA::Boolean doblock = TRUE);
    virtual CORBA::Boolean isblocking ();
    virtual void buffering (CORBA::Boolean dobuffering = TRUE);
    virtual CORBA::Boolean isbuffering ();
    virtual CORBA::Boolean isreadable ();

    virtual CORBA::Boolean eof () const;
    virtual CORBA::Boolean bad () const;
    virtual std::string errormsg () const;

// #ifdef HAVE_THREADS
//     void setown( MICOMT::Thread::ThreadID );
// #endif
};

class SocketTransportServer: public CORBA::TransportServer,
#ifdef HAVE_THREADS
			     public MICO::Operation,
#endif
			     public CORBA::DispatcherCallback {
protected:
    CORBA::Dispatcher *adisp;
    CORBA::TransportServerCallback *acb;
    CORBA::Long fd;
    std::string err;

    CORBA::Boolean listening;
    CORBA::Boolean is_blocking;

    void remove_aselect() {
	if (adisp && acb)
	    adisp->remove (this, CORBA::Dispatcher::Read);
	
	adisp = 0;
	acb = 0;
    };
    virtual void __clean_up();

public:
    SocketTransportServer ();
    virtual ~SocketTransportServer ();

    void aselect (CORBA::Dispatcher *,
    		  CORBA::TransportServerCallback *);
    void callback (CORBA::Dispatcher *, CORBA::Dispatcher::Event);

    CORBA::Boolean isblocking () const;
    void block (CORBA::Boolean doblock = TRUE);

    CORBA::Boolean bad () const;
    std::string errormsg () const;

    virtual void listen ();

#ifdef HAVE_THREADS
    virtual void _run();
    virtual Operation *copy () const;

    void create_thread();
    void start();
#endif

};

}
#endif // __mico_transport_impl_h__




