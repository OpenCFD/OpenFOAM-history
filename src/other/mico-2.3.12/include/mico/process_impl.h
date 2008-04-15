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

#ifndef __mico_process_impl_h__
#define __mico_process_impl_h__

#if defined (_WIN32) || defined (_POCKET_PC)
#include <winbase.h>
#endif

namespace MICO {

class UnixProcess : public MICO::Process, public CORBA::DispatcherCallback {

    CORBA::Long _exit_status;
    CORBA::Boolean _detached;
    MICO::ProcessCallback *_cb;
    std::string _args;
#if defined (_WIN32) || defined (_POCKET_PC)
#define OSWIN_MAXPROCS 128
    HANDLE _hProcess;
    static HANDLE MICO::UnixProcess::s_childprocs[OSWIN_MAXPROCS+1];
    static DWORD s_childpids[OSWIN_MAXPROCS+1];
    static int s_numofchildren;
    static void process_died(DWORD pid);
#if !defined(__MINGW32__)
    static unsigned int __stdcall wait_thread_func (VOID *arg);
    static unsigned int __stdcall ThreadExitFunc(VOID *arg);
#else
    static void __stdcall wait_thread_func (VOID *arg);
    static void __stdcall ThreadExitFunc(VOID *arg);
#endif
    static HANDLE s_waitthread;
    static int s_stop_waiting;

    HANDLE hRequestExitEvent;
    
    DWORD _pid;
#else
    CORBA::Long _pid;
#endif
    typedef std::list<UnixProcess *> ListProcess;
    static ListProcess _procs;

    static void signal_handler (int sig);
public:
#if defined (_WIN32) || defined (_POCKET_PC)
    static void _init();
    static void win32_process_init();
#endif
    UnixProcess (const char *cmd, MICO::ProcessCallback * = 0);
    virtual ~UnixProcess ();

    virtual CORBA::Boolean run ();
    virtual CORBA::Boolean exited ();
    virtual CORBA::Boolean exit_status ();
    virtual void terminate ();
    virtual void detach ();
    virtual operator CORBA::Boolean ();

    virtual void callback (CORBA::Dispatcher *,
			   CORBA::DispatcherCallback::Event);
};

}

#endif // __mico_process_impl_h__
