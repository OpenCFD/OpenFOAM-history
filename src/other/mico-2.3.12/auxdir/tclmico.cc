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
#include <mico/template_impl.h>
#include <mico/tclmico.h>

using namespace std;

#if TCL_MAJOR_VERSION > 7
#define TCL_FILE(fd) ((int)(fd))
#else
// XXX Tcl_File is never deleted ...
#define TCL_FILE(fd) Tcl_GetFile((ClientData)(fd), TCL_UNIX_FD)
#endif

using namespace std;

void
TclDispatcher::input_callback (ClientData _event, int mask)
{
    /*
     * Tcl allows only one handler to be installed for each file
     * handle. so we have to extract the file handle from the passend
     * event, traverse the list of file events and call each handler
     * that matches the file handle and 'mask'. we have to be
     * careful, because event handlers can remove and add new events,
     * possibly invalidating iterators.
     */
    FileEvent *event = (FileEvent *)_event;
    TclDispatcher *disp = event->disp;
    CORBA::Long handle = event->handle;

    set<FileEvent *, less<FileEvent *> > seen;
    while (42) {
	list<FileEvent *>::iterator i;
	for (i = disp->fevents.begin(); i != disp->fevents.end(); ++i) {
	    if ((*i)->handle != handle)
		continue;
	    Event ev = (*i)->ev;
	    if (!((ev == Read   && (mask & TCL_READABLE)) ||
		  (ev == Write  && (mask & TCL_WRITABLE)) ||
		  (ev == Except && (mask & TCL_EXCEPTION))))
		continue;
	    if (seen.count (*i))
		continue;
	    seen.insert (*i);
	    (*i)->cb->callback (disp, (*i)->ev);
	    /*
	     * callback may have removed or added events, which can
	     * invalidate the iterators. thraverse list again from begin.
	     * by recording called handlers in 'seen' we avoid calling
	     * handlers twice.
	     */
	    break;
	}
	if (i == disp->fevents.end())
	    break;
    }
}

void
TclDispatcher::timer_callback (ClientData _event)
{
    TimerEvent *event = (TimerEvent *)_event;
    TclDispatcher *disp = event->disp;

    list<TimerEvent *>::iterator i;
    for (i = disp->tevents.begin(); i != disp->tevents.end(); ++i) {
        if ((*i) == event) {
	  disp->tevents.erase(i);
	  break;
	}
    }
    event->cb->callback (disp, Timer);
    delete event;
}

TclDispatcher::TclDispatcher ()
{
}

TclDispatcher::~TclDispatcher ()
{
    list<FileEvent *>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i) {
	(*i)->cb->callback (this, Remove);
	delete *i;
    }

    list<TimerEvent *>::iterator j;
    for (j = tevents.begin(); j != tevents.end(); ++j) {
	(*j)->cb->callback (this, Remove);
	delete *j;
    }
}

int
TclDispatcher::tcl_mask (CORBA::Long handle)
{
    FileEvent *ev;
    return tcl_mask (handle, ev);
}

int
TclDispatcher::tcl_mask (CORBA::Long handle, FileEvent * &ev)
{
    ev = 0;
    int mask = 0;

    list<FileEvent *>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i) {
	if ((*i)->handle != handle)
	    continue;
	if (!ev)
	    ev = *i;
	switch ((*i)->ev) {
	case Read:
	    mask |= TCL_READABLE;
	    break;
	case Write:
	    mask |= TCL_WRITABLE;
	    break;
	case Except:
	    mask |= TCL_EXCEPTION;
	    break;
	}
    }
    return mask;
}

void
TclDispatcher::rd_event (CORBA::DispatcherCallback *cb, CORBA::Long fd)
{
    FileEvent *ev = new FileEvent (this, fd, cb, Read);
    fevents.push_back (ev);
    Tcl_CreateFileHandler (TCL_FILE (fd),
			   tcl_mask (fd), input_callback, (ClientData)ev);
}

void
TclDispatcher::wr_event (CORBA::DispatcherCallback *cb, CORBA::Long fd)
{
    FileEvent *ev = new FileEvent (this, fd, cb, Write);
    fevents.push_back (ev);
    Tcl_CreateFileHandler (TCL_FILE (fd),
			   tcl_mask (fd), input_callback, (ClientData)ev);
}

void
TclDispatcher::ex_event (CORBA::DispatcherCallback *cb, CORBA::Long fd)
{
    FileEvent *ev = new FileEvent (this, fd, cb, Except);
    fevents.push_back (ev);
    Tcl_CreateFileHandler (TCL_FILE (fd),
			   tcl_mask (fd), input_callback, (ClientData)ev);
}

void
TclDispatcher::tm_event (CORBA::DispatcherCallback *cb, CORBA::ULong tmout)
{
    TimerEvent *ev = new TimerEvent (this, 0, cb);
    tevents.push_back (ev);
    ev->token = Tcl_CreateTimerHandler (tmout, timer_callback, (ClientData)ev);
}

void
TclDispatcher::remove (CORBA::DispatcherCallback *cb, Event e)
{
    if (e == All || e == Timer) {
	list<TimerEvent *>::iterator i, next;
	for (i = tevents.begin(); i != tevents.end(); i = next) {
	    next = i;
	    ++next;
	    if ((*i)->cb == cb) {
		Tcl_DeleteTimerHandler ((*i)->token);
		delete *i;
		tevents.erase (i);
	    }
	}
    }
    if (e == All || e == Read || e == Write || e == Except) {
	list<FileEvent *>::iterator i, next;
	for (i = fevents.begin(); i != fevents.end(); i = next) {
	    next = i;
	    ++next;
	    if ((*i)->cb == cb && (e == All || e == (*i)->ev)) {
		CORBA::Long handle = (*i)->handle;
		delete *i;
		fevents.erase (i);
		FileEvent *next_event;
		int nmask = tcl_mask (handle, next_event);
		if (next_event) {
		    Tcl_CreateFileHandler (TCL_FILE (handle),
					   nmask, input_callback,
					   (ClientData)next_event);
		} else {
		    Tcl_DeleteFileHandler (TCL_FILE (handle));
		}
	    }
	}
    }
}

void
TclDispatcher::run (CORBA::Boolean infinite)
{
  do {
      Tcl_DoOneEvent (0);
  } while (infinite);
}

void
TclDispatcher::move (CORBA::Dispatcher *)
{
    assert (0);
}

CORBA::Boolean
TclDispatcher::idle () const
{
    return fevents.size() + tevents.size() == 0;
}

