/*
 *  Qt support for MICO
 *  Copyright (C) 1997 Lars Doelle
 *
 *  Rewritten by Karel Gardas, <kgardas@objectsecurity.com>
 *  Copyright (C) 2002 ObjectSecurity Ltd.
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

#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <mico/template_impl.h>
#include <mico/qtmico.h>
#include <algorithm>


using namespace std;

//
// CApp
//
CApp::CApp(int argc, char *argv[])
    : QApplication (argc,argv)
{
    qtd_ = new QTDispatcher (this);
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdown()));
    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(shutdown()));
    finished_ = FALSE;
}

//
// FileEvent
//
FileEvent::~FileEvent()
{
    delete xid_;
}

FileEvent::FileEvent()
{
}

FileEvent::FileEvent
(QTDispatcher* disp,
 CORBA::DispatcherCallback* cb,
 int fd, 
 QSocketNotifier::Type evq,
 CORBA::Dispatcher::Event evc)
    : disp_(disp), xid_(new QSocketNotifier(fd, evq)), cb_(cb), ev_(evc)
{
    QObject::connect(xid_, SIGNAL (activated(int)),
		     this, SLOT (activated(int)));
}

void
FileEvent::activated(int /*fd*/) 
{
    cb_->callback(disp_, ev_);
}
//
// TimerEvent
//
TimerEvent::~TimerEvent()
{
    delete xid_;
}

TimerEvent::TimerEvent()
{
}

TimerEvent::TimerEvent
(QTDispatcher* disp,
 CORBA::DispatcherCallback* cb,
 long tmout)
    : xid_(new QTimer()), cb_(cb), disp_(disp)
{
    QObject::connect(xid_, SIGNAL (timeout()), this, SLOT (timeout()));
    xid_->start(tmout, TRUE);
}

void
TimerEvent::timeout()
{
    disp_->remove(this);
    cb_->callback(disp_, CORBA::Dispatcher::Timer);
    delete this;
}
//
// create & destroy
//
QTDispatcher::QTDispatcher(CApp* ctx)
    : ctx_(ctx)
{
}

QTDispatcher::~QTDispatcher()
{
    list<FileEvent*>::iterator i;
    for (i = fevents_.begin(); i != fevents_.end(); ++i) {
	(*i)->cb_->callback(this, Remove);
	delete *i;
    }

    list<TimerEvent*>::iterator j;
    for (j = tevents_.begin(); j != tevents_.end(); ++j) {
	(*j)->cb_->callback(this, Remove);
	delete *j;
    }
}
//
// adding events
//
void
QTDispatcher::rd_event(CORBA::DispatcherCallback* cb, CORBA::Long fd)
{
    fevents_.push_back(new FileEvent(this, cb, fd, QSocketNotifier::Read,
				     CORBA::Dispatcher::Read));
}

void
QTDispatcher::wr_event(CORBA::DispatcherCallback* cb, CORBA::Long fd)
{
    fevents_.push_back(new FileEvent(this, cb, fd, QSocketNotifier::Write,
				     CORBA::Dispatcher::Write));
}

void
QTDispatcher::ex_event(CORBA::DispatcherCallback* cb, CORBA::Long fd)
{
    fevents_.push_back(new FileEvent(this, cb, fd, QSocketNotifier::Exception,
				    CORBA::Dispatcher::Except));
}

void
QTDispatcher::tm_event(CORBA::DispatcherCallback* cb, CORBA::ULong tmout)
{
    // both Qt and MICO timeouts are millisecs
    tevents_.push_back(new TimerEvent(this, cb, tmout));
}

void
QTDispatcher::remove(CORBA::DispatcherCallback* cb, CORBA::Dispatcher::Event e)
{
    if (e == All || e == Timer) {
	list<TimerEvent*>::iterator i, next;
	for (i = tevents_.begin(); i != tevents_.end(); i = next) {
	    next = i;
	    ++next;
	    if ((*i)->cb_ == cb) {
		delete *i;
		tevents_.erase (i);
	    }
	}
    }
    if (e == All || e == Read || e == Write || e == Except) {
	list<FileEvent*>::iterator i, next;
	for (i = fevents_.begin(); i != fevents_.end(); i = next) {
	    next = i;
	    ++next;
	    if ((*i)->cb_ == cb && (e == All || e == (*i)->ev_)) {
		delete *i;
		fevents_.erase(i);
	    }
	}
    }
}

void
QTDispatcher::remove(TimerEvent *t)
{
    list<TimerEvent*>::iterator i
	= find(tevents_.begin(), tevents_.end(), t);
    assert(i != tevents_.end());
    tevents_.erase(i);
}
//
// misc
//
void
QTDispatcher::run(CORBA::Boolean infinite)
{
    do {
	ctx_->processOneEvent();
    } while (infinite);
}

void
QTDispatcher::move(CORBA::Dispatcher*)
{
    assert (0);
}

CORBA::Boolean
QTDispatcher::idle() const
{
    return fevents_.size() + tevents_.size() == 0;
}

#include "qtmico.moc"

