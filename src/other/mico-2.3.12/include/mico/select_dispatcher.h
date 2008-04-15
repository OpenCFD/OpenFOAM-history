// -*- c++ -*-
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

#ifndef __mico_select_dispatcher_h__
#define __mico_select_dispatcher_h__

namespace MICO {	
	
class SelectDispatcher : public CORBA::Dispatcher {
    typedef fd_set FDSet;

    struct FileEvent {
	Event event;
	CORBA::Long fd;
	CORBA::DispatcherCallback *cb;
	CORBA::Boolean deleted;

	FileEvent () {}
	FileEvent (Event _ev, CORBA::Long _fd, CORBA::DispatcherCallback *_cb)
	    : event(_ev), fd(_fd), cb(_cb), deleted(FALSE)
	{}
    };
    struct TimerEvent {
	Event event;
	CORBA::Long delta;
	CORBA::DispatcherCallback *cb;

	TimerEvent () {}
	TimerEvent (Event _ev, CORBA::Long _delta,
		    CORBA::DispatcherCallback *_cb)
	    : event(_ev), delta(_delta), cb(_cb)
	{}
    };

    std::list<FileEvent> fevents;
    std::list<TimerEvent> tevents;

    CORBA::Long last_update;
    CORBA::Boolean init;
    CORBA::Long locked;
    CORBA::Boolean modified;
    FDSet curr_wset, curr_rset, curr_xset;
    CORBA::Long fd_max;

    void lock ();
    void unlock ();
    CORBA::Boolean islocked () const;

    CORBA::Long gettime () const;
    void update_tevents ();
    void handle_tevents ();
    void handle_fevents (FDSet &rset, FDSet &wset, FDSet &xset);
    void update_fevents ();
    void sleeptime (OSMisc::TimeVal &);

    static CORBA::Boolean _isblocking;
public:
    SelectDispatcher ();
    virtual ~SelectDispatcher ();
    virtual void rd_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void wr_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void ex_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void tm_event (CORBA::DispatcherCallback *, CORBA::ULong tmout);
    virtual void remove (CORBA::DispatcherCallback *, Event);
    virtual void move (CORBA::Dispatcher *);
    virtual void run (CORBA::Boolean infinite = TRUE);
    virtual CORBA::Boolean idle () const;

    virtual void block (CORBA::Boolean b);
    virtual CORBA::Boolean isblocking ();
};


class SelectDispatcherFactory
    : public CORBA::DispatcherFactory
{
public:
    virtual
    ~SelectDispatcherFactory()
    {}

    virtual CORBA::Dispatcher*
    create();
};

}

#endif // __mico_select_dispatcher_h__
