// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 2004-2005 by The Mico Team
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

#ifndef __mico_poll_dispatcher_h__
#define __mico_poll_dispatcher_h__

namespace MICO {	
	
class PollDispatcher : public CORBA::Dispatcher {
    struct FileEvent {
        Event event;
        CORBA::Long fd;
        CORBA::DispatcherCallback *cb;
        CORBA::Boolean deleted;
        int pollidx;

        FileEvent () {}
        FileEvent (Event _ev, CORBA::Long _fd, CORBA::DispatcherCallback *_cb)
            : event(_ev), fd(_fd), cb(_cb), deleted(FALSE), pollidx(-1)
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

    CORBA::Boolean has_deleted_fevents;
    CORBA::Boolean must_rebuild_pollset;

    std::vector<struct pollfd> pollset;

    void lock ();
    void unlock ();
    CORBA::Boolean islocked () const;

    CORBA::Long gettime () const;
    void build_pollset  ();
    void update_tevents ();
    void handle_tevents ();
    void handle_fevents ();
    CORBA::Long sleeptime ();

    static CORBA::Boolean _isblocking;
public:
    PollDispatcher ();
    virtual ~PollDispatcher ();
    virtual void rd_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void wr_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void ex_event (CORBA::DispatcherCallback *, CORBA::Long fd);
    virtual void tm_event (CORBA::DispatcherCallback *, CORBA::ULong tmout);
    virtual void remove (CORBA::DispatcherCallback *, Event);
    virtual void move (CORBA::Dispatcher *);
    virtual void run (CORBA::Boolean infinite = TRUE);
    virtual CORBA::Boolean idle () const;

    virtual void block(CORBA::Boolean b);
    virtual CORBA::Boolean isblocking ();
};


class PollDispatcherFactory
    : public CORBA::DispatcherFactory
{
public:
    virtual
    ~PollDispatcherFactory()
    {}

    virtual CORBA::Dispatcher*
    create();
};

}

#endif // __mico_poll_dispatcher_h__

