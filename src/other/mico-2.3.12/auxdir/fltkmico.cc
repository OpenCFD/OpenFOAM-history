/*
 *  Fltk support for MICO
 *  Copyright (C) 1999 Jacques Tremblay
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
#include <stdio.h>
#include <stdlib.h>

#include <CORBA.h>
#include <mico/template_impl.h>

#include "mico/fltkmico.h"
#include "FL/fl_ask.H"

void
FL_MicoDispatcher::input_callback(int fd, void *_event) {
  FileEvent *event = (FileEvent *)_event;

  event->cb->callback (event->disp, event->ev);
}

void
FL_MicoDispatcher::timer_callback(void * _event) {
  TimerEvent *event = (TimerEvent *)_event;
  FL_MicoDispatcher *disp = event->disp;

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

FL_MicoDispatcher::FL_MicoDispatcher(int shut) {
  shutdownOnLastWindowClose = shut;
  isShuttingDown = 0;
}

FL_MicoDispatcher::~FL_MicoDispatcher() {
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

void
FL_MicoDispatcher::rd_event(CORBA::DispatcherCallback *cb, CORBA::Long fd) {
  FileEvent *ev = new FileEvent (this, cb, Read, int(fd));

  Fl::add_fd (fd, FL_READ, input_callback, ev);
  fevents.push_back (ev);
}

void
FL_MicoDispatcher::wr_event(CORBA::DispatcherCallback *cb, CORBA::Long fd) {
  FileEvent *ev = new FileEvent (this, cb, Write, int(fd));

  Fl::add_fd  (fd, FL_WRITE, input_callback, ev);
  fevents.push_back (ev);
}

void
FL_MicoDispatcher::ex_event(CORBA::DispatcherCallback *cb, CORBA::Long fd) {
  FileEvent *ev = new FileEvent (this, cb, Except, int(fd));

  Fl::add_fd (fd, FL_EXCEPT, input_callback, ev);
  fevents.push_back (ev);
}

void
FL_MicoDispatcher::tm_event(CORBA::DispatcherCallback *cb, CORBA::ULong tmout) {
  TimerEvent *ev = new TimerEvent (this, cb, tmout);

  // tmout is in msec and fltk uses sec
  Fl::add_timeout (tmout/1000.0, timer_callback, ev);

  tevents.push_back (ev);
}

void
FL_MicoDispatcher::remove(CORBA::DispatcherCallback *cb, Event e) {
  if (e == All || e == Timer) {
    list<TimerEvent *>::iterator i, next;
    for (i = tevents.begin(); i != tevents.end(); i = next) {
      next = i;
      ++next;
      if ((*i)->cb == cb) {
	Fl::remove_timeout (timer_callback, (*i));
	delete *i;
	tevents.erase (i);
      }
    }
  }

  int fd = -1;
  if (e == All || e == Read || e == Write || e == Except) {
    list<FileEvent *>::iterator i, next;
    for (i = fevents.begin(); i != fevents.end(); i = next) {
      next = i;
      ++next;
      if ((*i)->cb == cb && (e == All || e == (*i)->ev)) {
	Fl::remove_fd ((*i)->fd);
	fd = (*i)->fd;
	delete *i;
	fevents.erase (i);
      }
    }

    // fltk removes every callback... so we have to add
    // the one it wrongly removed...
    if (fd != -1 &&(e == Read || e == Write || e == Except)) {
      for (i = fevents.begin(); i != fevents.end(); i = next) {
	next = i;
	++next;
	if (fd == (*i)->fd && e != (*i)->ev) {
	  Fl::add_fd  (fd, (*i)->ev, input_callback, (*i));
	}
      }
    }
  }
}

void
FL_MicoDispatcher::run(CORBA::Boolean infinite) {
  do {
    if (!shutdownOnLastWindowClose) {
      Fl::wait(0.001); 
    } else {
      if (!Fl::wait()) {
	if (!isShuttingDown) {
	  shutdown();
	} else {
	  Fl::wait(0.001);
	}
      }
    } 
  } while (infinite);
}

CORBA::Boolean
FL_MicoDispatcher::idle() const {
  return fevents.size() + tevents.size() == 0;
}


void
FL_MicoDispatcher::shutdown() {
  fl_alert("There are no more window shown.\nThe ORB will shut down.");
  isShuttingDown = 1;
  CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb", 0);
  if (orb) {
    orb->shutdown(1);
  }
}

void
FL_MicoDispatcher::move(CORBA::Dispatcher *disp) {
  assert(0);

  // It does not work....
/*
  while(fevents.size()) {
    list<FileEvent *>::iterator i = fevents.begin();

    if (*i) {
      FileEvent f = **i;
      remove((*i)->cb, (*i)->ev);
      switch (f.ev) {
      case Read:
	rd_event(f.cb, f.ev);
	break;
      case Write:
	wr_event(f.cb, f.ev);
	break;
      case Except:
	ex_event(f.cb, f.ev);
	break;
      }
    }
  }
  while (tevents.size()) {
    list<TimerEvent *>::iterator i = tevents.begin();

    // This is not quite ok because it does not
    // take into account the time elapsed since
    // the timer event was put in place.
    if (*i) {
      TimerEvent t = **i;
      remove((*i)->cb, Timer);
      tm_event(t.cb, t.tmout);
    }
  }
*/

}
