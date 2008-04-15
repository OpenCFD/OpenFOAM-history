/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  multi-thread dispatcher
 *  Copyright (C) 1999 Andreas Schultz                                 
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
 *
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
#ifndef _WIN32
#include <string.h>
#endif
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>

#endif // FAST_PCH


using namespace std;

/*!
 * The constructor for the dispatcher sets the the
 * operation type to Operation::ORB.
 */
MICO::MTDispatcher::MTDispatcher()
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "MICO::MTDispatcher::MTDispatcher()" << endl;
    }
    this->info().set_op_id( MICO::Operation::ORB );
}


/*!
 * \param op		The dispatcher to copy
 * The copy constructor copies the specified
 * dispatcher and its contents to the new
 * object.
 */
MICO::MTDispatcher::MTDispatcher( const MICO::MTDispatcher &op )
    : PassiveOperation( op )
{

    this->info().set_op_id( MICO::Operation::ORB );
}

/*!
 * \return A pointer to the new dispatcher (as an operation).
 * This method makes a new dispatcher with the same contents.
 *
 * \todo
 * Is this method really necessary? why not just (in the program)
 * call new MICO::MTDispatcher( obj ). That should be sufficent.
 */
MICO::Operation *
MICO::MTDispatcher::copy() const
{

    return new MICO::MTDispatcher( *this );
}

/*!
 * \param msg		The message to process
 * Process a message and dispatch it to somewhere else. If the
 * event is a KillConn, then the connection is deleted. If the
 * event is a CloseConn, the event is pushed to the callback
 * object.
 */
void
MICO::MTDispatcher::process( MICO::msg_type * msg )
{

    MICO::ORBMsg *_msg = (MICO::ORBMsg *)msg->data();

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "MTDispatcher::process" << endl;
    }

    switch (_msg->ev) {
    case MICO::ORBMsg::KillConn:
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "  ORBMsg::KillConn" << endl;
	}
	assert( _msg->conn->state() == MICOMT::StateRefCnt::Terminated );
	delete _msg->conn;
	break;

    case MICO::ORBMsg::CloseConn:
  	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "  ORBMsg::CloseConn" << endl;
	}
	_msg->cb->callback (_msg->conn, GIOPConnCallback::Closed);
	break;

    default: 
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< _msg->ev << endl;
	}
    }

    delete _msg;
    delete msg;
}

