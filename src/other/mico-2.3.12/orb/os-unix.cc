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

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#ifdef HAVE_DL_H
#include <dl.h>
#endif
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/os-math.h>

#endif // FAST_PCH


using namespace std;

/****************************** OSMath ********************************/


MICO_Double OSMath::_infinity = 0;
MICO_LongDouble OSMath::_long_infinity = 0;
MICO_Double OSMath::_notanumber = 0;
MICO_LongDouble OSMath::_long_notanumber = 0;


/**************************** UnixProcess *****************************/


namespace MICO {
  MICO::UnixProcess::ListProcess UnixProcess::_procs;
}

void
MICO::UnixProcess::signal_handler (int sig)
{
    int status;
    while (42) {
	CORBA::Long pid = ::waitpid (-1, &status, WNOHANG);
        if (pid < 0 && errno == EINTR)
            continue;
	if (pid <= 0)
	    break;

	ListProcess::iterator i;
	for (i = _procs.begin(); i != _procs.end(); ++i) {
	    if (pid == (*i)->_pid) {
		if (WIFEXITED ((status))) {
		    (*i)->_exit_status = WEXITSTATUS ((status));
		} else {
		    (*i)->_exit_status = 1000;
		}
		if ((*i)->_cb) {
		    CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
		    CORBA::Dispatcher *disp = orb->dispatcher();
		    disp->remove (*i, CORBA::Dispatcher::Timer);
		    disp->tm_event (*i, 0);
		}
		break;
	    }
	}
    }
    ::signal (SIGCHLD, signal_handler);
}

MICO::UnixProcess::UnixProcess (const char *cmd, MICO::ProcessCallback *cb)
{
    _exit_status = -1;
    _pid = 0;
    _detached = FALSE;
    _cb = cb;
    _procs.push_back (this);
    _args = cmd;
}

MICO::UnixProcess::~UnixProcess ()
{
    for (ListProcess::iterator pos = _procs.begin(); pos != _procs.end(); ++pos) {
	if (*pos == this) {
	    _procs.erase (pos);
	    if (!_detached && !exited())
		terminate ();
	    return;
	}
    }
    assert (0);
}

CORBA::Boolean
MICO::UnixProcess::run ()
{
    ::signal (SIGCHLD, signal_handler);

    _pid = ::fork ();
    if (_pid == 0) {
	string command;
#ifndef __CYGWIN32__
	// with Cygwin32 using "exec" doesnt work for some strange reason
	command = "exec ";
#endif
	command += _args;
	::execl ("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
	exit (1);
    }
    return _pid > 0;
}

CORBA::Boolean
MICO::UnixProcess::exited ()
{
    return _pid <= 0 || _exit_status >= 0;
}

CORBA::Boolean
MICO::UnixProcess::exit_status ()
{
    return _exit_status == 0;
}

void
MICO::UnixProcess::terminate ()
{
    assert (_pid > 0);
    ::kill (_pid, SIGTERM);
}

void
MICO::UnixProcess::detach ()
{
    _detached = TRUE;
    _cb = 0;
}

MICO::UnixProcess::operator CORBA::Boolean ()
{
    return _pid > 0;
}

void
MICO::UnixProcess::callback (CORBA::Dispatcher *disp,
			     CORBA::DispatcherCallback::Event ev)
{
    if (ev == CORBA::Dispatcher::Timer && _cb)
	_cb->callback (this, MICO::ProcessCallback::Exited);
}


/*************************** UnixSharedLib ****************************/


#if defined(HAVE_DLOPEN) && defined(HAVE_DYNAMIC)

#ifndef RTLD_NOW
#define RTLD_NOW 1
#endif

#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL 0
#endif

MICO::UnixSharedLib::UnixSharedLib (const char *name)
{
    _name = name;
    _handle = ::dlopen (name, RTLD_NOW|RTLD_GLOBAL);
}

MICO::UnixSharedLib::~UnixSharedLib ()
{
    if (_handle)
	::dlclose (_handle);
}

void *
MICO::UnixSharedLib::symbol (const char *sym)
{
    assert (_handle);
    return ::dlsym (_handle, (char *)sym);
}

const char *
MICO::UnixSharedLib::error ()
{
    const char *err = ::dlerror ();
    if (err)
	_error = err;
    return _error.c_str();
}

MICO::UnixSharedLib::operator CORBA::Boolean ()
{
    return !!_handle;
}

const char *
MICO::UnixSharedLib::name ()
{
    return _name.c_str();
}

#elif defined(HAVE_SHL_LOAD) && defined(HAVE_DYNAMIC)

MICO::UnixSharedLib::UnixSharedLib (const char *name)
{
    _name = name;
    _handle = ::shl_load (name, BIND_IMMEDIATE, 0L);
}

MICO::UnixSharedLib::~UnixSharedLib ()
{
    if (_handle)
	::shl_unload ((shl_t)_handle);
}

void *
MICO::UnixSharedLib::symbol (const char *sym)
{
    assert (_handle);

    void *value;
    if (::shl_findsym ((shl_t *)&_handle, (char *)sym, TYPE_PROCEDURE, 
		       &value) < 0) {
	string _sym = "_";
	_sym += sym;
	if (::shl_findsym ((shl_t *)&_handle, (char *)_sym.c_str(),
			   TYPE_PROCEDURE, &value) < 0)
	    return 0;
    }
    return value;
}

const char *
MICO::UnixSharedLib::error ()
{
    _error = strerror (errno);
    return _error.c_str();
}

MICO::UnixSharedLib::operator CORBA::Boolean ()
{
    return !!_handle;
}

const char *
MICO::UnixSharedLib::name ()
{
    return _name.c_str();
}

#else

MICO::UnixSharedLib::UnixSharedLib (const char *name)
{
    _name = name;
    _handle = 0;
}

MICO::UnixSharedLib::~UnixSharedLib ()
{
}

void *
MICO::UnixSharedLib::symbol (const char *sym)
{
    return 0;
}

const char *
MICO::UnixSharedLib::error ()
{
    return "no shlib support";
}

MICO::UnixSharedLib::operator CORBA::Boolean ()
{
    return FALSE;
}

const char *
MICO::UnixSharedLib::name ()
{
    return _name.c_str();
}

#endif
