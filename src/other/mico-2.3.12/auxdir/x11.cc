/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
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
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 */

#include <CORBA-SMALL.h>
#include <mico/x11.h>
#include <mico/template_impl.h>


using namespace std;

void
X11Dispatcher::input_callback (XtPointer _disp, int *fd, XtInputId *xid)
{
    X11Dispatcher *disp = (X11Dispatcher *)_disp;

    list<FileEvent>::iterator i;
    for (i = disp->fevents.begin(); i != disp->fevents.end(); ++i) {
	if (*xid == (*i).xid) {
	    (*i).cb->callback (disp, (*i).ev);
	    break;
	}
    }
}

void
X11Dispatcher::timer_callback (XtPointer _disp, XtIntervalId *xid)
{
    X11Dispatcher *disp = (X11Dispatcher *)_disp;

    list<TimerEvent>::iterator i;
    for (i = disp->tevents.begin(); i != disp->tevents.end(); ++i) {
	if (*xid == (*i).xid) {
	    CORBA::DispatcherCallback *cb = (*i).cb;
	    disp->tevents.erase (i);
	    cb->callback (disp, Timer);
	    break;
	}
    }
}

X11Dispatcher::X11Dispatcher (XtAppContext _ctx)
    : ctx (_ctx)
{
}

X11Dispatcher::~X11Dispatcher ()
{
    list<FileEvent>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i)
	(*i).cb->callback (this, Remove);

    list<TimerEvent>::iterator j;
    for (j = tevents.begin(); j != tevents.end(); ++j)
	(*j).cb->callback (this, Remove);
}

void
X11Dispatcher::rd_event (CORBA::DispatcherCallback *cb, CORBA::Long fd)
{
    XtInputId xid = XtAppAddInput (ctx, fd,
				   (XtPointer)XtInputReadMask,
				   input_callback,
				   (XtPointer)this);
    fevents.push_back (FileEvent (xid, cb, Read));
}

void
X11Dispatcher::wr_event (CORBA::DispatcherCallback *cb, CORBA::Long fd)
{
    XtInputId xid = XtAppAddInput (ctx, fd,
				   (XtPointer)XtInputWriteMask,
				   input_callback,
				   (XtPointer)this);
    fevents.push_back (FileEvent (xid, cb, Write));
}

void
X11Dispatcher::ex_event (CORBA::DispatcherCallback *cb, CORBA::Long fd)
{
    XtInputId xid = XtAppAddInput (ctx, fd,
				   (XtPointer)XtInputExceptMask,
				   input_callback,
				   (XtPointer)this);
    fevents.push_back (FileEvent (xid, cb, Except));
}

void
X11Dispatcher::tm_event (CORBA::DispatcherCallback *cb, CORBA::ULong tmout)
{
    XtIntervalId xid = XtAppAddTimeOut (ctx, tmout, timer_callback,
					(XtPointer)this);
    tevents.push_back (TimerEvent (xid, cb));
}

void
X11Dispatcher::remove (CORBA::DispatcherCallback *cb, Event e)
{
    if (e == All || e == Timer) {
	list<TimerEvent>::iterator i, next;
	for (i = tevents.begin(); i != tevents.end(); i = next) {
	    next = i;
	    ++next;
	    if ((*i).cb == cb) {
		XtRemoveTimeOut ((*i).xid);
		tevents.erase (i);
	    }
	}
    }
    if (e == All || e == Read || e == Write || e == Except) {
	list<FileEvent>::iterator i, next;
	for (i = fevents.begin(); i != fevents.end(); i = next) {
	    next = i;
	    ++next;
	    if ((*i).cb == cb && (e == All || e == (*i).ev)) {
		XtRemoveInput ((*i).xid);
		fevents.erase (i);
	    }
	}
    }
}

void
X11Dispatcher::run (CORBA::Boolean infinite)
{
    do {
	XtAppProcessEvent (ctx, XtIMAll);
    } while (infinite);
}

void
X11Dispatcher::move (CORBA::Dispatcher *)
{
    assert (0);
}

CORBA::Boolean
X11Dispatcher::idle () const
{
    return fevents.size() + tevents.size() == 0;
}
