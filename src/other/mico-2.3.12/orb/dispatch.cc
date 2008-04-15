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
#ifndef _WIN32
#include <string.h>
#endif
#include <errno.h>
#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/os-misc.h>
#include <mico/os-net.h>

#endif // FAST_PCH


using namespace std;

/**************************** misc dtors *******************************/


CORBA::Dispatcher::~Dispatcher ()
{
}

CORBA::DispatcherCallback::~DispatcherCallback ()
{
}


/****************************** Timeout *********************************/


CORBA::Timeout::Timeout (Dispatcher *d, Long tm)
    : _ready (tm == 0), _disp (d), _have_tmout (FALSE)
{
    if (tm > 0) {
	_have_tmout = TRUE;
	_disp->tm_event (this, tm);
    }
}

CORBA::Timeout::~Timeout ()
{
    if (_have_tmout)
	_disp->remove (this, Dispatcher::Timer);
}

void
CORBA::Timeout::callback (Dispatcher *d, Event ev)
{
    switch (ev) {
    case CORBA::Dispatcher::Timer:
	_have_tmout = FALSE;
        _ready = TRUE;
	break;

    case CORBA::Dispatcher::Moved:
	_disp = d;
	break;
    default:
	break;
    }
}


/************************** SelectDispatcher *****************************/

class SignalBlocker {
    OSMisc::SigState _sigs;
    bool _blocked;
public:
    void block ()
    {
	if (!_blocked) {
	    _blocked = true;
	    OSMisc::block_sigs (_sigs);
	}
    }
    void unblock ()
    {
	if (_blocked) {
	    _blocked = false;
	    OSMisc::restore_sigs (_sigs);
	}
    }
    SignalBlocker ()
    {
	_blocked = false;
	block();
    }
    ~SignalBlocker ()
    {
	unblock();
    }
};

CORBA::Boolean MICO::SelectDispatcher::_isblocking = FALSE;

MICO::SelectDispatcher::SelectDispatcher ()
    : last_update (0), init (TRUE), locked (0), modified (FALSE)
{
    FD_ZERO (&curr_wset);
    FD_ZERO (&curr_rset);
    FD_ZERO (&curr_xset);
    fd_max = 0;
}

MICO::SelectDispatcher::~SelectDispatcher ()
{
    list<FileEvent>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i)
	(*i).cb->callback (this, Remove);

    list<TimerEvent>::iterator j;
    for (j = tevents.begin(); j != tevents.end(); ++j)
	(*j).cb->callback (this, Remove);
}

void 
MICO::SelectDispatcher::block (CORBA::Boolean b)
{
  _isblocking = b;
}

CORBA::Boolean 
MICO::SelectDispatcher::isblocking ()
{
  return _isblocking;
}

CORBA::Long
MICO::SelectDispatcher::gettime () const
{
  OSMisc::TimeVal ct = OSMisc::gettime();
  return ct.tv_sec*1000+ct.tv_usec/1000;
}

void
MICO::SelectDispatcher::sleeptime (OSMisc::TimeVal &tm)
{
    if (tevents.size() == 0) {
	// wdh: changed sleeptime to 1 second
 	tm.tv_sec =  1; //10; 
	tm.tv_usec = 0;
	return;
    }
    update_tevents();
    CORBA::Long t = (tevents.front().delta > 0 ? tevents.front().delta : 0);

    tm.tv_sec = t / 1000L;
    tm.tv_usec = (t % 1000L) * 1000L;
}

void
MICO::SelectDispatcher::update_fevents ()
{
    modified = TRUE;

    FD_ZERO (&curr_rset);
    FD_ZERO (&curr_wset);
    FD_ZERO (&curr_xset);
    fd_max = 0;

    list<FileEvent>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i) {
	if (!(*i).deleted) {
	    switch ((*i).event) {
	    case Read:
		FD_SET ((*i).fd, &curr_rset);
		break;
	    case Write:
		FD_SET ((*i).fd, &curr_wset);
		break;
	    case Except:
		FD_SET ((*i).fd, &curr_xset);
		break;
	    default:
		assert (0);
	    }
            if ((*i).fd > fd_max)
                fd_max = (*i).fd;
	}
    }
}

void
MICO::SelectDispatcher::update_tevents ()
{
    CORBA::Long curr = gettime();
    if (init || tevents.size() == 0 || curr - last_update < 0) {
	last_update = curr;
	init = FALSE;
    } else {
	tevents.front().delta -= (curr - last_update);
	last_update = curr;
    }
}

void
MICO::SelectDispatcher::handle_tevents ()
{
    SignalBlocker __sb;

    if (tevents.size() == 0)
	return;

    update_tevents ();
    while (tevents.size() > 0 && tevents.front().delta <= 0) {
	TimerEvent t = tevents.front();
	tevents.pop_front();
	if (tevents.size() > 0)
	    tevents.front().delta += t.delta;

	__sb.unblock();
	t.cb->callback (this, t.event);
	__sb.block();

	update_tevents ();
    }
}

void
MICO::SelectDispatcher::handle_fevents (FDSet &rset, FDSet &wset, FDSet &xset)
{
    // already signal safe because of lock()/unlock()

    lock ();
    list<FileEvent>::iterator i;

    for (i = fevents.begin(); i != fevents.end(); ++i) {
	if (!(*i).deleted) {
	    switch ((*i).event) {
	    case Read:
		if (FD_ISSET ((*i).fd, &rset))
		    (*i).cb->callback (this, Read);
		break;
	    case Write:
		if (FD_ISSET ((*i).fd, &wset))
		    (*i).cb->callback (this, Write);
		break;
	    case Except:
		if (FD_ISSET ((*i).fd, &xset))
		    (*i).cb->callback (this, Except);
		break;
	    default:
		assert (0);
	    }
	}
    }

    unlock ();
}

void
MICO::SelectDispatcher::lock ()
{
    if (!locked)
	modified = FALSE;
    ++locked;
}

void
MICO::SelectDispatcher::unlock ()
{
    if (--locked > 0)
	return;
    assert (locked == 0);

    if (modified) {
	list<FileEvent>::iterator i;
	bool again;

	do {
	  again = false;
	  for (i = fevents.begin(); i != fevents.end(); i++) {
	    if ((*i).deleted) {
	      fevents.erase (i);
	      again = true;
	      break;
	    }
	  }
	}
	while (again);
    }
}

CORBA::Boolean
MICO::SelectDispatcher::islocked () const
{
    return locked > 0;
}

void
MICO::SelectDispatcher::rd_event (CORBA::DispatcherCallback *cb,
				  CORBA::Long fd)
{
    SignalBlocker __sb;

    fevents.push_back (FileEvent (Read, fd, cb));
    update_fevents ();
}

void
MICO::SelectDispatcher::wr_event (CORBA::DispatcherCallback *cb,
				  CORBA::Long fd)
{
    SignalBlocker __sb;

    fevents.push_back (FileEvent (Write, fd, cb));
    update_fevents ();
}

void
MICO::SelectDispatcher::ex_event (CORBA::DispatcherCallback *cb,
				  CORBA::Long fd)
{
    SignalBlocker __sb;

    fevents.push_back (FileEvent (Except, fd, cb));
    update_fevents ();
}

void
MICO::SelectDispatcher::tm_event (CORBA::DispatcherCallback *cb,
				  CORBA::ULong tmout)
{
    SignalBlocker __sb;

    assert ((CORBA::Long)tmout >= 0);
    TimerEvent t (Timer, tmout, cb);

    update_tevents ();
    list<TimerEvent>::iterator i;
    for (i = tevents.begin(); i != tevents.end(); ++i) {
	if ((*i).delta <= t.delta) {
	    t.delta -= (*i).delta;
	} else {
	    (*i).delta -= t.delta;
	    break;
	}
    }
    tevents.insert (i, t);
}

void
MICO::SelectDispatcher::remove (CORBA::DispatcherCallback *cb, Event e)
{
    SignalBlocker __sb;

    if (e == All || e == Timer) {
	list<TimerEvent>::iterator i, next;
	bool again;

	do {
	  again = false;
	  for (i = tevents.begin(); i != tevents.end(); i++) {
	    next = i;
	    ++next;
	    if ((*i).cb == cb) {
	      CORBA::Long delta = (*i).delta;
	      if (next != tevents.end())
		(*next).delta += delta;
	      tevents.erase (i);
	      again = true;
	      break;
	    }
	  }
	}
	while (again);
    }
    if (e == All || e == Read || e == Write || e == Except) {
	list<FileEvent>::iterator i;
	bool again;

	do {
	  again = false;
	  for (i = fevents.begin(); i != fevents.end(); i++) {
	    if ((*i).cb == cb && (e == All || (*i).event == e)) {
		if (islocked()) {
		    (*i).deleted = TRUE;
		} else {
		    fevents.erase (i);
		    again = true;
		    break;
		}
	    }
	  }
	}
	while (again);

	update_fevents ();
    }
}

void
MICO::SelectDispatcher::run (CORBA::Boolean infinite)
{
    FDSet rset, wset, xset;
    OSMisc::TimeVal tm;

    do {
	{
	    SignalBlocker __sb;

	    rset = curr_rset;
	    wset = curr_wset;
	    xset = curr_xset;
	    sleeptime (tm);
	}

	int r = ::select (fd_max+1,
			  (select_addr_t)&rset,
			  (select_addr_t)&wset,
			  (select_addr_t)&xset,
			  &tm);
	assert (r >= 0 || errno == EINTR || errno == EAGAIN ||
                errno == EWOULDBLOCK);

	if (r > 0)
	    handle_fevents (rset, wset, xset);
	handle_tevents ();
    } while (infinite);
}

void
MICO::SelectDispatcher::move (CORBA::Dispatcher *disp)
{
    SignalBlocker __sb;

    assert (!islocked());

    list<FileEvent>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i) {
	switch ((*i).event) {
	case Read:
            (*i).cb->callback (disp, CORBA::Dispatcher::Moved);
	    disp->rd_event ((*i).cb, (*i).fd);
	    break;
	case Write:
            (*i).cb->callback (disp, CORBA::Dispatcher::Moved);
	    disp->wr_event ((*i).cb, (*i).fd);
	    break;
	case Except:
            (*i).cb->callback (disp, CORBA::Dispatcher::Moved);
	    disp->ex_event ((*i).cb, (*i).fd);
	    break;
	default:
	    break;
	}
    }
    fevents.erase (fevents.begin(), fevents.end());
    update_fevents ();

    update_tevents ();
    CORBA::Long tmout = 0;
    list<TimerEvent>::iterator j;
    for (j = tevents.begin(); j != tevents.end(); ++j) {
	tmout += (*j).delta;
	if (tmout < 0)
	    tmout = 0;
        (*j).cb->callback (disp, CORBA::Dispatcher::Moved);
	disp->tm_event ((*j).cb, tmout);
    }
    tevents.erase (tevents.begin(), tevents.end());
}

CORBA::Boolean
MICO::SelectDispatcher::idle () const
{
  SignalBlocker __sb;
  FDSet rset, wset, xset;
  OSMisc::TimeVal tm;

  /*
   * Any pending file events?
   */

  if (fevents.size() > 0) {
    rset = curr_rset;
    wset = curr_wset;
    xset = curr_xset;
    tm.tv_sec = 0;
    tm.tv_usec = 0;

    int r = ::select (fd_max+1,
		      (select_addr_t)&rset,
		      (select_addr_t)&wset,
		      (select_addr_t)&xset,
		      &tm);
    assert (r >= 0 || errno == EINTR || errno == EAGAIN ||
            errno == EWOULDBLOCK);

    if (r > 0) {
      return FALSE;
    }
  }

  /*
   * No? Then what about pending timer events?
   */

  if (tevents.size()) {
    // Discard const for update_tevents()
    ((SelectDispatcher *) this)->update_tevents ();
    if (tevents.front().delta <= 0) {
      return FALSE;
    }
  }

  /*
   * Then we're idle ...
   */

  return TRUE;
}

//
// SelectDispatcherFactory
//

CORBA::Dispatcher*
MICO::SelectDispatcherFactory::create()
{
    return new SelectDispatcher;
}

#ifdef HAVE_POLL_H

/************************** PollDispatcher *****************************/

CORBA::Boolean MICO::PollDispatcher::_isblocking = FALSE;

MICO::PollDispatcher::PollDispatcher ()
:   last_update(0)
,   init(TRUE)
,   locked(0)
,   has_deleted_fevents(FALSE)
,   must_rebuild_pollset(TRUE)
{
}

MICO::PollDispatcher::~PollDispatcher ()
{
    list<FileEvent>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i)
        (*i).cb->callback (this, Remove);

    list<TimerEvent>::iterator j;
    for (j = tevents.begin(); j != tevents.end(); ++j)
        (*j).cb->callback (this, Remove);
}

void 
MICO::PollDispatcher::block (CORBA::Boolean b)
{
    _isblocking = b;
}

CORBA::Boolean 
MICO::PollDispatcher::isblocking ()
{
    return _isblocking;
}

CORBA::Long
MICO::PollDispatcher::gettime () const
{
    OSMisc::TimeVal ct = OSMisc::gettime();
    return ct.tv_sec*1000+ct.tv_usec/1000;
}

CORBA::Long
MICO::PollDispatcher::sleeptime ()
{
    if (tevents.empty()) 
        return 1000;

    update_tevents();
    return (tevents.front().delta > 0 ? tevents.front().delta : 0);
}

void
MICO::PollDispatcher::build_pollset ()
{
    if (!must_rebuild_pollset)
        return;

    // printf("rebuild\n");
    pollset.clear();

    list<FileEvent>::iterator cur;
    list<FileEvent>::iterator end = fevents.end();
    for (cur = fevents.begin(); cur != end; ++cur) {
        if ((*cur).deleted) 
            continue;

        struct pollfd pollevent;

        pollevent.fd = (*cur).fd;

        switch ((*cur).event) { 
            // XXX: events richtig? 
            case Read:
                pollevent.events = POLLIN | POLLERR | POLLHUP;
                break;
            case Write:
                pollevent.events = POLLOUT | POLLERR;
                break;
            case Except:
                pollevent.events = POLLPRI;
                break;
            default:
                assert (0);
        }
        
        (*cur).pollidx = pollset.size();
        pollset.push_back(pollevent);
    }

    must_rebuild_pollset = FALSE;
}

void
MICO::PollDispatcher::update_tevents ()
{
    CORBA::Long curr = gettime();
    if (init || tevents.empty() || curr - last_update < 0) {
        last_update = curr;
        init = FALSE;
    } else {
        tevents.front().delta -= (curr - last_update);
        last_update = curr;
    }
}

void
MICO::PollDispatcher::handle_tevents ()
{
    SignalBlocker __sb;

    if (tevents.empty())
        return;

    update_tevents ();
    while (!tevents.empty() && tevents.front().delta <= 0) {
        TimerEvent t = tevents.front();
        tevents.pop_front();
        if (!tevents.empty())
            tevents.front().delta += t.delta;

        __sb.unblock();
        t.cb->callback (this, t.event);
        __sb.block();

        update_tevents ();
    }
}

void
MICO::PollDispatcher::handle_fevents()
{
    lock ();

    list<FileEvent>::iterator i;

    for (i = fevents.begin(); i != fevents.end(); ++i) {

        if ((*i).deleted) 
            continue;

        if ((*i).pollidx == -1)
            continue;
        
        switch ((*i).event) {
            case Read:
                if (pollset[(*i).pollidx].revents & (POLLIN | POLLERR | POLLHUP))
                    (*i).cb->callback (this, Read);
                break;
            case Write:
                if (pollset[(*i).pollidx].revents & (POLLOUT | POLLERR))
                    (*i).cb->callback (this, Write);
                break;
            case Except:
                if (pollset[(*i).pollidx].revents & (POLLPRI))
                    (*i).cb->callback (this, Except);
                break;
            default:
                assert (0);
        }
    }

    unlock ();
}

void
MICO::PollDispatcher::lock ()
{
    ++locked;
    // printf("lock, now %d\n", locked);
}

void
MICO::PollDispatcher::unlock ()
{
    // printf("unlock, now %d\n", locked - 1);
    if (--locked > 0)
        return;
    assert (locked == 0);

    if (has_deleted_fevents) {
        list<FileEvent>::iterator i = fevents.begin();
        while (i != fevents.end()) {
            if ((*i).deleted) {
                i = fevents.erase(i);
            } else {
                ++i;
            }
        }
        has_deleted_fevents = FALSE;
    }
}

CORBA::Boolean
MICO::PollDispatcher::islocked () const
{
    return locked > 0;
}

void
MICO::PollDispatcher::rd_event (CORBA::DispatcherCallback *cb,
                CORBA::Long fd)
{
    SignalBlocker __sb;

    fevents.push_back (FileEvent (Read, fd, cb));
    must_rebuild_pollset = TRUE;
}

void
MICO::PollDispatcher::wr_event (CORBA::DispatcherCallback *cb,
                CORBA::Long fd)
{
    SignalBlocker __sb;

    fevents.push_back (FileEvent (Write, fd, cb));
    must_rebuild_pollset = TRUE;
}

void
MICO::PollDispatcher::ex_event (CORBA::DispatcherCallback *cb,
                CORBA::Long fd)
{
    SignalBlocker __sb;

    fevents.push_back (FileEvent (Except, fd, cb));
    must_rebuild_pollset = TRUE;
}

void
MICO::PollDispatcher::tm_event (CORBA::DispatcherCallback *cb,
        CORBA::ULong tmout)
{
    SignalBlocker __sb;

    assert ((CORBA::Long)tmout >= 0);
    TimerEvent t (Timer, tmout, cb);

    update_tevents ();
    list<TimerEvent>::iterator i;
    for (i = tevents.begin(); i != tevents.end(); ++i) {
        if ((*i).delta <= t.delta) {
            t.delta -= (*i).delta;
        } else {
            (*i).delta -= t.delta;
            break;
        }
    }
    tevents.insert (i, t);
}

void
MICO::PollDispatcher::remove (CORBA::DispatcherCallback *cb, Event e)
{
    SignalBlocker __sb;

    if (e == All || e == Timer) {
        list<TimerEvent>::iterator i = tevents.begin();
        while (i != tevents.end()) {
            list<TimerEvent>::iterator next = i;
            ++next;
            if ((*i).cb == cb) {
                CORBA::Long delta = (*i).delta;
                if (next != tevents.end())
                    (*next).delta += delta;
                tevents.erase (i);
            }
            i = next;
        }
    }
    if (e == All || e == Read || e == Write || e == Except) {
        list<FileEvent>::iterator i = fevents.begin();

        while (i != fevents.end()) {
            if ((*i).cb == cb && (e == All || (*i).event == e)) {
                if (islocked()) {
                    (*i).deleted = TRUE;
                    has_deleted_fevents = TRUE;
                    ++i;
                } else {
                    i = fevents.erase(i);
                }
                must_rebuild_pollset = TRUE;
            } else {
                ++i;
            }
        }
    }
}

void
MICO::PollDispatcher::run (CORBA::Boolean infinite)
{
    do {
        build_pollset();
        // printf("pollsetsize = %d\n", pollset.size());

        int r = ::poll(&pollset[0], pollset.size(), sleeptime());
        assert (r >= 0 || errno == EINTR || errno == EAGAIN ||
                errno == EWOULDBLOCK);

        if (r > 0)
            handle_fevents();
        handle_tevents ();
    } while (infinite);

    if (islocked())
        must_rebuild_pollset = TRUE;
}

void
MICO::PollDispatcher::move (CORBA::Dispatcher *disp)
{
    SignalBlocker __sb;

    assert (!islocked());

    list<FileEvent>::iterator i;
    for (i = fevents.begin(); i != fevents.end(); ++i) {
        switch ((*i).event) {
            case Read:
                (*i).cb->callback (disp, CORBA::Dispatcher::Moved);
                disp->rd_event ((*i).cb, (*i).fd);
                break;
            case Write:
                (*i).cb->callback (disp, CORBA::Dispatcher::Moved);
                disp->wr_event ((*i).cb, (*i).fd);
                break;
            case Except:
                (*i).cb->callback (disp, CORBA::Dispatcher::Moved);
                disp->ex_event ((*i).cb, (*i).fd);
                break;
            default:
                break;
        }
    }
    fevents.erase (fevents.begin(), fevents.end());
    must_rebuild_pollset = TRUE;

    update_tevents ();
    CORBA::Long tmout = 0;
    list<TimerEvent>::iterator j;
    for (j = tevents.begin(); j != tevents.end(); ++j) {
        tmout += (*j).delta;
        if (tmout < 0)
            tmout = 0;
        (*j).cb->callback (disp, CORBA::Dispatcher::Moved);
        disp->tm_event ((*j).cb, tmout);
    }
    tevents.erase (tevents.begin(), tevents.end());
}

CORBA::Boolean
MICO::PollDispatcher::idle () const
{
    SignalBlocker __sb;

    /*
     * Any pending file events?
     */
    // printf("idle()\n");

    if (fevents.size() > 0) {
        ((PollDispatcher *) this)->build_pollset();
        
        int r = ::poll((struct pollfd *)&pollset[0], pollset.size(), 0);
        assert (r >= 0 || errno == EINTR || errno == EAGAIN ||
                errno == EWOULDBLOCK);

        if (r > 0) {
            return FALSE;
        }
    }

    /*
     * No? Then what about pending timer events?
     */

    if (tevents.size()) {
        // Discard const for update_tevents()
        ((PollDispatcher *) this)->update_tevents ();
        if (tevents.front().delta <= 0) {
            return FALSE;
        }
    }

    /*
     * Then we're idle ...
     */

    return TRUE;
}

//
// PollDispatcherFactory
//

CORBA::Dispatcher*
MICO::PollDispatcherFactory::create()
{
    return new PollDispatcher;
}

#endif // HAVE_POLL_H
