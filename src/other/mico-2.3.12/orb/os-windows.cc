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

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mico/impl.h>
#include <mico/template_impl.h>
#include <mico/util.h>
#include <mico/os-net.h>
#include <mico/os-misc.h>


#ifdef _DEBUG
OSNet::WSA_Number_Text_struct
OSNet::WSA_Number_Text_table[WSA_Number_Text_table_n] = {
    { ENAMETOOLONG, "ENAMETOOLONG" },
    { ENOTEMPTY, "ENOTEMPTY" },
    { EWOULDBLOCK, "EWOULDBLOCK" },
    { HOST_NOT_FOUND, "HOST_NOT_FOUND" },
    { NO_ADDRESS, "NO_ADDRESS" },
    { NO_DATA, "NO_DATA" },
    { NO_RECOVERY, "NO_RECOVERY" },
    { TRY_AGAIN, "TRY_AGAIN" },
    { WSABASEERR, "WSABASEERR" },
    { WSAEACCES, "WSAEACCES" },
    { WSAEADDRINUSE, "WSAEADDRINUSE" },
    { WSAEADDRNOTAVAIL, "WSAEADDRNOTAVAIL" },
    { WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
    { WSAEALREADY, "WSAEALREADY" },
    { WSAEBADF, "WSAEBADF" },
    { WSAECANCELLED, "WSAECANCELLED" },
    { WSAECONNABORTED, "WSAECONNABORTED" },
    { WSAECONNREFUSED, "WSAECONNREFUSED" },
    { WSAECONNRESET, "WSAECONNRESET" },
    { WSAEDESTADDRREQ, "WSAEDESTADDRREQ" },
    { WSAEDISCON, "WSAEDISCON" },
    { WSAEDQUOT, "WSAEDQUOT" },
    { WSAEFAULT, "WSAEFAULT" },
    { WSAEHOSTDOWN, "WSAEHOSTDOWN" },
    { WSAEHOSTUNREACH, "WSAEHOSTUNREACH" },
    { WSAEINPROGRESS, "WSAEINPROGRESS" },
    { WSAEINTR, "WSAEINTR" },
    { WSAEINVAL, "WSAEINVAL" },
    { WSAEINVALIDPROCTABLE, "WSAEINVALIDPROCTABLE" },
    { WSAEINVALIDPROVIDER, "WSAEINVALIDPROVIDER" },
    { WSAEISCONN, "WSAEISCONN" },
    { WSAELOOP, "WSAELOOP" },
    { WSAEMFILE, "WSAEMFILE" },
    { WSAEMSGSIZE, "WSAEMSGSIZE" },
    { WSAENAMETOOLONG, "WSAENAMETOOLONG" },
    { WSAENETDOWN, "WSAENETDOWN" },
    { WSAENETRESET, "WSAENETRESET" },
    { WSAENETUNREACH, "WSAENETUNREACH" },
    { WSAENOBUFS, "WSAENOBUFS" },
    { WSAENOMORE, "WSAENOMORE" },
    { WSAENOPROTOOPT, "WSAENOPROTOOPT" },
    { WSAENOTCONN, "WSAENOTCONN" },
    { WSAENOTEMPTY, "WSAENOTEMPTY" },
    { WSAENOTSOCK, "WSAENOTSOCK" },
    { WSAEOPNOTSUPP, "WSAEOPNOTSUPP" },
    { WSAEPFNOSUPPORT, "WSAEPFNOSUPPORT" },
    { WSAEPROCLIM, "WSAEPROCLIM" },
    { WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT" },
    { WSAEPROTOTYPE, "WSAEPROTOTYPE" },
    { WSAEPROVIDERFAILEDINIT, "WSAEPROVIDERFAILEDINIT" },
    { WSAEREFUSED, "WSAEREFUSED" },
    { WSAEREMOTE, "WSAEREMOTE" },
    { WSAESHUTDOWN, "WSAESHUTDOWN" },
    { WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT" },
    { WSAESTALE, "WSAESTALE" },
    { WSAETIMEDOUT, "WSAETIMEDOUT" },
    { WSAETOOMANYREFS, "WSAETOOMANYREFS" },
    { WSAEUSERS, "WSAEUSERS" },
    { WSAEWOULDBLOCK, "WSAEWOULDBLOCK" },
    { WSAHOST_NOT_FOUND, "WSAHOST_NOT_FOUND" },
    { WSANOTINITIALISED, "WSANOTINITIALISED" },
    { WSANO_DATA, "WSANO_DATA" },
    { WSANO_RECOVERY, "WSANO_RECOVERY" },
    { WSASERVICE_NOT_FOUND, "WSASERVICE_NOT_FOUND" },
    { WSASYSCALLFAILURE, "WSASYSCALLFAILURE" },
    { WSASYSNOTREADY, "WSASYSNOTREADY" },
    { WSATRY_AGAIN, "WSATRY_AGAIN" },
    { WSATYPE_NOT_FOUND, "WSATYPE_NOT_FOUND" },
    { WSAVERNOTSUPPORTED, "WSAVERNOTSUPPORTED" },
    { WSA_E_CANCELLED, "WSA_E_CANCELLED" },
    { WSA_E_NO_MORE, "WSA_E_NO_MORE" },
    { WSA_FLAG_MULTIPOINT_C_LEAF, "WSA_FLAG_MULTIPOINT_C_LEAF" },
    { WSA_FLAG_MULTIPOINT_C_ROOT, "WSA_FLAG_MULTIPOINT_C_ROOT" },
    { WSA_FLAG_MULTIPOINT_D_LEAF, "WSA_FLAG_MULTIPOINT_D_LEAF" },
    { WSA_FLAG_MULTIPOINT_D_ROOT, "WSA_FLAG_MULTIPOINT_D_ROOT" },
    { WSA_FLAG_OVERLAPPED, "WSA_FLAG_OVERLAPPED" },
    { WSA_INFINITE, "WSA_INFINITE" },
    { WSA_INVALID_HANDLE, "WSA_INVALID_HANDLE" },
    { WSA_INVALID_PARAMETER, "WSA_INVALID_PARAMETER" },
    { WSA_IO_INCOMPLETE, "WSA_IO_INCOMPLETE" },
    { WSA_IO_PENDING, "WSA_IO_PENDING" },
    { WSA_MAXIMUM_WAIT_EVENTS, "WSA_MAXIMUM_WAIT_EVENTS" },
    { WSA_NOT_ENOUGH_MEMORY, "WSA_NOT_ENOUGH_MEMORY" },
    { WSA_OPERATION_ABORTED, "WSA_OPERATION_ABORTED" },
    { WSA_WAIT_EVENT_0, "WSA_WAIT_EVENT_0" },
    { WSA_WAIT_FAILED, "WSA_WAIT_FAILED" },
    { WSA_WAIT_IO_COMPLETION, "WSA_WAIT_IO_COMPLETION" },
    { WSA_WAIT_TIMEOUT, "WSA_WAIT_TIMEOUT" }
};
#endif
HANDLE OSMisc::SignalMutex=NULL;


/**************************** UnixProcess *****************************/


namespace MICO {

  MICO::UnixProcess::ListProcess UnixProcess::_procs;
  // List of Processhandles, except for first one, which
  // is a WakeUp Event for the Waiting Thread
  HANDLE MICO::UnixProcess::s_childprocs[OSWIN_MAXPROCS+1];
  DWORD MICO::UnixProcess::s_childpids[OSWIN_MAXPROCS+1];
  int MICO::UnixProcess::s_numofchildren=0;
  // Handle of the waiting thread
  HANDLE MICO::UnixProcess::s_waitthread=NULL;
  int MICO::UnixProcess::s_stop_waiting=FALSE;
  
};

MICO::UnixProcess::UnixProcess (const char *cmd, MICO::ProcessCallback *cb)
{
    WaitForSingleObject(OSMisc::SignalMutex,INFINITE);
    _exit_status = -1;
    _pid = 0;
    _detached = FALSE;
    _cb = cb;
    _procs.push_back (this);
    _args = cmd;
    
    hRequestExitEvent = NULL;
    
    ReleaseMutex(OSMisc::SignalMutex);  
}

MICO::UnixProcess::~UnixProcess ()
{
    WaitForSingleObject(OSMisc::SignalMutex,INFINITE);
    for (ListProcess::iterator pos = _procs.begin(); pos != _procs.end(); ++pos) {
	if (*pos == this) {
	    _procs.erase (pos);

            CloseHandle(hRequestExitEvent);
            ReleaseMutex(OSMisc::SignalMutex);      
	    if (!_detached && !exited()) {
		// terminate ();
            }
	    return;
	}
    }
    assert (0);
}



CORBA::Boolean
MICO::UnixProcess::run ()
{
#ifdef _POCKET_PC
	MICO_NOT_IMPLEMENTED;
	return 1;
#else
    STARTUPINFO StartupInfo;    
    PROCESS_INFORMATION processInfo;     	

    StartupInfo.cb = sizeof(StartupInfo);
    GetStartupInfo(&StartupInfo); 
    StartupInfo.lpReserved = NULL;
    StartupInfo.lpTitle = NULL;

    BOOL result = CreateProcess( NULL, (char*)_args.c_str(), NULL, NULL,
                                 FALSE, CREATE_NEW_CONSOLE /* 0 */ , NULL, NULL,
                                 &StartupInfo, &processInfo ); 

    CloseHandle(processInfo.hThread); // must be closed and we don't need it

    _hProcess= processInfo.hProcess;
    _pid=processInfo.dwProcessId;
           
    if ( result )
    {
        // Synchronization with waiting thread,
        WaitForSingleObject(OSMisc::SignalMutex,INFINITE);
        s_numofchildren++;
        // Insert new Process at the end
        s_childprocs[s_numofchildren]=_hProcess;
        s_childpids[s_numofchildren]=_pid;
        // Wake Up the thread
        SetEvent(s_childprocs[0]);

        
      if (hRequestExitEvent == NULL) { 
        char szExitEventName[] = "Process00000000";

        sprintf(szExitEventName, "Process%08X", _pid);
                hRequestExitEvent = CreateEvent(NULL, TRUE, FALSE, szExitEventName);
      }
      ReleaseMutex(OSMisc::SignalMutex);        
      return ((int)_pid);
    } else {
      return FALSE;
    }
#endif
}




void
MICO::UnixProcess::process_died(DWORD pid)
{
    int status;

	ListProcess::iterator i;
	for (i = _procs.begin(); i != _procs.end(); ++i) {
	    if (pid == (*i)->_pid)
        {
            DWORD exitcode;


            GetExitCodeProcess( (*i)->_hProcess, &exitcode);
            // cannot be set to exitcode which could be -1, if mains returns with -1
            (*i)->_exit_status = 0; // exitcode;
            // Avoid zombies
            CloseHandle((*i)->_hProcess);
            
            if ((*i)->_cb)
            {
                CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
                CORBA::Dispatcher *disp = orb->dispatcher();
                disp->remove (*i, CORBA::Dispatcher::Timer);
		 disp->tm_event (*i, 0);
            }
            
        } 
    }
}



void 
MICO::UnixProcess::_init() {
#ifdef _POCKET_PC
	MICO_NOT_IMPLEMENTED;
#else
  HANDLE hProcessExitEvent = NULL;
  char  szExitEventName[] = "Process00000000";
  HANDLE hExitThread = NULL;
  unsigned int ExitThreadID = 0;

  sprintf(szExitEventName, "Process%08X", GetCurrentProcessId());
  hProcessExitEvent = OpenEvent(SYNCHRONIZE, TRUE, szExitEventName);
  if (hProcessExitEvent != NULL) { // we have been started by MICO::UnixProcess::run()
    CloseHandle(hProcessExitEvent);
    // Start the thread waiting for process termination
#if !defined(__MINGW32__)
    hExitThread = (HANDLE)_beginthreadex(NULL, 0, ThreadExitFunc, NULL, 0, &ExitThreadID);
#else
    hExitThread = (HANDLE)_beginthread(ThreadExitFunc, 0, NULL);
#endif
    if (hExitThread != NULL) {
      CloseHandle(hExitThread);
    }
  }
  return;
#endif
}


void
MICO::UnixProcess::win32_process_init() {
#ifdef _POCKET_PC
	MICO_NOT_IMPLEMENTED;
#else
  unsigned int	threadid = 0;

  s_stop_waiting = FALSE;
  s_childprocs[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
  //XXX TODO kill the thread when shutting down
#ifndef __MINGW32__
  s_waitthread = (HANDLE)_beginthreadex(NULL, 0, wait_thread_func, NULL, 0, &threadid);
#else
  s_waitthread = (HANDLE)_beginthread(wait_thread_func, 0, NULL);
#endif
  if (s_waitthread != NULL) {
    CloseHandle(s_waitthread);
  }
  return;
#endif
}


#if !defined(__MINGW32__)
unsigned int
#else
void
#endif
__stdcall MICO::UnixProcess::ThreadExitFunc(VOID *arg) {
#ifdef _POCKET_PC
	MICO_NOT_IMPLEMENTED;
#else
  HANDLE hProcessExitEvent = NULL;
  char szExitEventName[] = "Process00000000";

  sprintf(szExitEventName, "Process%08X", GetCurrentProcessId());
  hProcessExitEvent = OpenEvent(SYNCHRONIZE, TRUE, szExitEventName);
  if (hProcessExitEvent != NULL) {
    WaitForSingleObject(hProcessExitEvent, INFINITE);
    ResetEvent(hProcessExitEvent);
  CORBA::ORB_var orb = CORBA::ORB_instance("mico-local-orb");
  orb->shutdown(FALSE);
  orb->perform_work();
  }
#endif
#if !defined(__MINGW32__)
  return 0;
#endif
}


#if !defined(__MINGW32__)
unsigned int
#else
void
#endif
__stdcall MICO::UnixProcess::wait_thread_func (VOID *arg)
{
    while(9*6)
    {
        DWORD rc = WaitForMultipleObjects (s_numofchildren+1, s_childprocs,
                                           FALSE,INFINITE);
        if (rc == WAIT_TIMEOUT)
            break;			// Exiting
//  Will be set when shutdown is requested        
//        if (s_stop_waiting)
//            break;
        
        rc -= WAIT_OBJECT_0;
        // if rc==0 it is a wakeup call to update
        // the list of child procs
        if (rc-- != 0)
        {
            // move last proc to the position of terminated proc
            // could be the same 

            WaitForSingleObject(OSMisc::SignalMutex,INFINITE);

            int pid=s_childpids[rc+1];

            s_childprocs[rc+1]=s_childprocs[s_numofchildren];
            s_childpids[rc+1]=s_childpids[s_numofchildren];
            s_numofchildren--;
            // Call the "signal_handler"
            (void)process_died (pid);
            
            ReleaseMutex(OSMisc::SignalMutex);

        }        
    }
#if !defined(__MINGW32__)
    return 0;
#endif
}

CORBA::Boolean
MICO::UnixProcess::exited ()
{
    return _pid == 0 || _exit_status >= 0;
}

CORBA::Boolean
MICO::UnixProcess::exit_status ()
{
    return _exit_status == 0;
}

void
    MICO::UnixProcess::terminate() {

    SetEvent(hRequestExitEvent);
    return;
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


MICO::UnixSharedLib::UnixSharedLib (const char *name)
{
    _name = name;
    _handle = ::LoadLibrary(name);
}

MICO::UnixSharedLib::~UnixSharedLib ()
{
    if (_handle != NULL)
    {
	::FreeLibrary((HINSTANCE)_handle);
    }
}

void *
MICO::UnixSharedLib::symbol (const char *sym)
{
    return ::GetProcAddress((HINSTANCE)_handle,sym);
}

const char *
MICO::UnixSharedLib::error ()
{
  LPVOID lpMsgBuf = NULL; //Here is the Pointer to Error String.
  ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		  FORMAT_MESSAGE_FROM_SYSTEM | 
		  FORMAT_MESSAGE_IGNORE_INSERTS,
		  NULL,
		  ::GetLastError(),
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		  (LPTSTR) &lpMsgBuf,
		  0,
		  NULL    
		  );
  const char *err = (const char*)lpMsgBuf;
  if (err)
    _error = err;
  ::LocalFree(lpMsgBuf);
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

