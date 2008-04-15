// -*- c++ -*-
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
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __MICO_QT_H__
#define __MICO_QT_H__

#ifdef HAVE_QAPPLICATION_H
#include <qapplication.h>
#else // HAVE_QAPPLICATION_H
#include <qapp.h>
#endif // HAVE_QAPPLICATION_H

#ifdef HAVE_QSOCKETNOTIFIER_H
#include <qsocketnotifier.h>
#else // HAVE_QSOCKETNOTIFIER_H
#include <qsocknot.h>
#endif // HAVE_QSOCKETNOTIFIER_H

#include <qtimer.h>
#include <CORBA.h>

class QTDispatcher; // forward

class CApp
    : public QApplication
{
    Q_OBJECT
    QTDispatcher* qtd_;
    CORBA::Boolean finished_;
public:
    CApp(int argc, char *argv[]);
    ~CApp()
    {};

    QTDispatcher*
    get_dispatcher()
    { return qtd_; }

    CORBA::Boolean
    finished()
    { return finished_; }

public slots:
    void
    shutdown()
    { finished_ = TRUE; }
};

class FileEvent
    : public QObject
{
    Q_OBJECT
public:
    QTDispatcher* disp_;
    QSocketNotifier* xid_;
    CORBA::DispatcherCallback* cb_;
    CORBA::Dispatcher::Event ev_;

    FileEvent();
    FileEvent(QTDispatcher* disp, CORBA::DispatcherCallback* cb,
	      int fd,  QSocketNotifier::Type evq,
	      CORBA::Dispatcher::Event evc);
    ~FileEvent();
private slots:
   void
   activated(int /*fd*/);
};

class TimerEvent
    : public QObject
{
    Q_OBJECT
public:
    QTimer* xid_;
    CORBA::DispatcherCallback* cb_;
    QTDispatcher* disp_;

    TimerEvent();
    TimerEvent(QTDispatcher* disp, CORBA::DispatcherCallback* cb,
	       long tmout);
    ~TimerEvent();
private slots:
    void
    timeout();
};

class QTDispatcher
    : public CORBA::Dispatcher
{
    CApp* ctx_;

    std::list<FileEvent*> fevents_;
    std::list<TimerEvent*> tevents_;
    
public:
    QTDispatcher(CApp*);
    virtual
    ~QTDispatcher();

    virtual void
    rd_event(CORBA::DispatcherCallback*, CORBA::Long fd);

    virtual void
    wr_event(CORBA::DispatcherCallback*, CORBA::Long fd);

    virtual void
    ex_event(CORBA::DispatcherCallback*, CORBA::Long fd);

    virtual void
    tm_event(CORBA::DispatcherCallback*, CORBA::ULong tmout);

    virtual void
    remove(CORBA::DispatcherCallback*, Event);

    virtual void
    run(CORBA::Boolean infinite = TRUE);

    virtual void
    move(CORBA::Dispatcher*);

    virtual CORBA::Boolean
    idle() const;

    void
    remove(TimerEvent *);
};

#endif // __MICO_QT_H__
