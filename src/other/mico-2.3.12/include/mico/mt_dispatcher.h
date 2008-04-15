/* -*- mode: c++; c-basic-offset: 4; -*-
 *
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


#ifndef __MT_DISPATCHER_H__
#define __MT_DISPATCHER_H__

namespace MICO
{

    class GIOPConn;
    struct GIOPConnCallback;

    /*!
     * \ingroup orb
     * The ORBMsg structure defines a structured event for
     * orbs. An ORB event can either kill a connection or
     * close a connection.
     */
    struct ORBMsg
    {
	/*!
	 * The event type of an ORB message.
	 */
	enum Event {
	    KillConn,		//!< Message to kill the connection
	    CloseConn		//!< Message to gracefully close the connection
	};

	ORBMsg(GIOPConn *_conn,
	       GIOPConnCallback *_cb,
	       Event _ev) :
	    conn(_conn),
	    cb(_cb),
	    ev(_ev)
	{};

	GIOPConn         *conn;		//!< The connection
	GIOPConnCallback *cb;		//!< The callback object
	Event            ev;		//!< The event type
    };

    /*!
     * \ingroup orb
     * The MTDispatcher class dispatches events to the ORB.
     */
    class MTDispatcher: public PassiveOperation {
    public:
	MTDispatcher();
	MTDispatcher( const MICO::MTDispatcher & );

	void process( msg_type * msg );
	virtual Operation *copy() const;
    };

}

#endif // __MT_DISPATCHER_H__
