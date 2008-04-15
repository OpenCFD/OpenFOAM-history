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

#ifndef __mico_x11_h__
#define __mico_x11_h__

#include <X11/Intrinsic.h>

class X11Dispatcher : public CORBA::Dispatcher {
    XtAppContext ctx;

    struct FileEvent {
	XtInputId xid;
	CORBA::DispatcherCallback *cb;
	Event ev;

	FileEvent () {}
	FileEvent (XtInputId _xid, CORBA::DispatcherCallback *_cb, Event _ev)
	    : xid (_xid), cb (_cb), ev (_ev)
	{}
    };
    struct TimerEvent {
	XtIntervalId xid;
	CORBA::DispatcherCallback *cb;

	TimerEvent () {}
	TimerEvent (XtIntervalId _xid, CORBA::DispatcherCallback *_cb)
	    : xid (_xid), cb (_cb)
	{}
    };
    std::list<FileEvent> fevents;
    std::list<TimerEvent> tevents;

    static void input_callback (XtPointer, int*, XtInputId *);
    static void timer_callback (XtPointer, XtIntervalId*);
public:
    X11Dispatcher (XtAppContext);
    virtual ~X11Dispatcher ();

    virtual void rd_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void wr_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void ex_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void tm_event (CORBA::DispatcherCallback *, CORBA::ULong tmout);
    virtual void remove (CORBA::DispatcherCallback *, Event);
    virtual void run (CORBA::Boolean infinite = TRUE);
    virtual void move (CORBA::Dispatcher *);
    virtual CORBA::Boolean idle () const;
};

#endif // __mico_x11_h__
