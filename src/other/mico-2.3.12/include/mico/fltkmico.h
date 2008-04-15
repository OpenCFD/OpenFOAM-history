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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef _FL_MICO_DISPATCHER_HEADER_
#define _FL_MICO_DISPATCHER_HEADER_

#include <FL/Fl.H>
#include <CORBA.h>

class FL_MicoDispatcher : public CORBA::Dispatcher {

  struct FileEvent {
    FL_MicoDispatcher *disp;
    CORBA::DispatcherCallback *cb;
    Event ev;
    int fd;
    
    FileEvent (FL_MicoDispatcher *_disp, CORBA::DispatcherCallback *_cb, Event _ev, int _fd)
      : disp (_disp), cb (_cb), ev (_ev), fd(_fd) {}
    
  private:
    FileEvent () {}
  };
  
  struct TimerEvent {
    FL_MicoDispatcher *disp;
    CORBA::DispatcherCallback *cb;
    CORBA::ULong tmout;

    TimerEvent (FL_MicoDispatcher *_disp, CORBA::DispatcherCallback *_cb, CORBA::ULong _tmout)
      : disp (_disp), cb (_cb), tmout(_tmout) {}

  private:
    TimerEvent () {}
  };
  
  list<FileEvent *> fevents;
  list<TimerEvent *> tevents;

  static void input_callback (int, void *);
  static void timer_callback (void *);
  
protected:
  int isShuttingDown;
  int shutdownOnLastWindowClose;

public:
  FL_MicoDispatcher(int shutdown = 1);
  virtual ~FL_MicoDispatcher();

  virtual void rd_event (CORBA::DispatcherCallback *, CORBA::Long fd);
  virtual void wr_event (CORBA::DispatcherCallback *, CORBA::Long fd);
  virtual void ex_event (CORBA::DispatcherCallback *, CORBA::Long fd);
  virtual void tm_event (CORBA::DispatcherCallback *, CORBA::ULong tmout);
  virtual void remove (CORBA::DispatcherCallback *, Event);
  virtual void run (CORBA::Boolean infinite = TRUE);
  virtual void move (CORBA::Dispatcher *);
  virtual CORBA::Boolean idle () const;

  virtual void shutdown(); // Called when Fl::wait() returns 0 i.e. no more window are shown
};

#endif  // _FL_MICO_DISPATCHER_HEADER_

