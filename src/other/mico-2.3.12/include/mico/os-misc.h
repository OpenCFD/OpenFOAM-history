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

#ifndef __mico_os_misc_h__
#define __mico_os_misc_h__

#if defined(_WIN32) && !defined(__MINGW32__)

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <io.h>
#include <process.h>

#ifdef __BORLANDC__
#define _timeb timeb
#define _ftime ftime
#define _getpid getpid
#endif


struct OSMisc {
    static void _init()
    {
        SignalMutex= CreateMutex (NULL, FALSE, NULL);
    }
    typedef __int64 longlong;
    typedef unsigned __int64 ulonglong;
    static HANDLE SignalMutex;

#if 0    
    struct TimeVal {
      MICO_Long tv_sec;
      MICO_Long tv_usec;
    };
#else
    typedef struct timeval TimeVal;
#endif

    static MICO_ULong timestamp ()
    {
        TimeVal _ts = gettime();
	return (_ts.tv_sec * 1000L) + (_ts.tv_usec / 1000L);
    }

    static TimeVal gettime ()
    {
      struct _timeb timebuffer;
      ::_ftime( &timebuffer );
      TimeVal ct;
      ct.tv_sec  = timebuffer.time;
      ct.tv_usec = timebuffer.millitm * 1000;
      return ct;
    }

    enum AccessMode {
	ACCESS_READ = 0,
	ACCESS_WRITE = 0,
	ACCESS_EXEC = 0
    };

    static MICO_Long access (const char *file, AccessMode md)
    {
	return ::_access (file, md);
    }

    static MICO_Long getpid ()
    {
	return ::_getpid();
    }

    static FILE *popen (const char *cmd, const char *mode)
    {
	return ::_popen (cmd, mode);
    }

    static void pclose (FILE *fp)
    {
	::_pclose (fp);
    }

    typedef short SigState;

    static void block_sigs (SigState &)
    {
        WaitForSingleObject( SignalMutex, INFINITE );
    }

    static void restore_sigs (SigState &)
    {
        ReleaseMutex( SignalMutex);
    }
};

#elif defined(_WIN32) && defined (__MINGW32__)

#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <io.h>
#include <process.h>


struct OSMisc {
    static void _init()
    {
        SignalMutex= CreateMutex (NULL, FALSE, NULL);
    }
    typedef MICO_LongLong longlong;
    typedef MICO_ULongLong ulonglong;
    static HANDLE SignalMutex;

    typedef struct timeval TimeVal;

    static TimeVal gettime ()
    {
      struct _timeb timebuffer;
      ::_ftime( &timebuffer );
      TimeVal ct;
      ct.tv_sec  = timebuffer.time;
      ct.tv_usec = timebuffer.millitm * 1000;
      return ct;
    }

    enum AccessMode {
	ACCESS_READ = 0,
	ACCESS_WRITE = 0,
	ACCESS_EXEC = 0
    };

    static MICO_Long access (const char *file, AccessMode md)
    {
	return ::_access (file, md);
    }

    static MICO_Long getpid ()
    {
	return ::_getpid();
    }

    static FILE *popen (const char *cmd, const char *mode)
    {
	return ::_popen (cmd, mode);
    }

    static void pclose (FILE *fp)
    {
	::_pclose (fp);
    }

    typedef short SigState;

    static void block_sigs (SigState &)
    {
        WaitForSingleObject( SignalMutex, INFINITE );
    }

    static void restore_sigs (SigState &)
    {
        ReleaseMutex( SignalMutex);
    }
};

#elif defined(_POCKET_PC)

//#include <winbase.h>
#include <winsock.h>
#if 0
#include <stdio.h>
#include <sys/timeb.h>
#include <io.h>
#include <process.h>
#endif


struct OSMisc {
    static void _init()
    {
        SignalMutex= CreateMutex (NULL, FALSE, NULL);
    }
    typedef MICO_LongLong longlong;
    typedef MICO_ULongLong ulonglong;
    static HANDLE SignalMutex;

    typedef struct timeval TimeVal;

#if 0
// _ftime() unsupported
    static TimeVal gettime ()
    {
      struct timeb timebuffer;
      ::_ftime( &timebuffer );
      TimeVal ct;
      ct.tv_sec  = timebuffer.time;
      ct.tv_usec = timebuffer.millitm * 1000;
      return ct;
    }
#endif

		static TimeVal gettime ()
		{
			TimeVal tv;
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			MICO_NOT_IMPLEMENTED;
			return tv;
		}
#if 0
// unsupported

    enum AccessMode {
	ACCESS_READ = 0,
	ACCESS_WRITE = 0,
	ACCESS_EXEC = 0
    };

    static MICO_Long access (const char *file, AccessMode md)
    {
	return ::_access (file, md);
    }
#endif

    static MICO_Long getpid ()
    {
	return 4711;  // L'eau de Cologne   :-)
    }

#if 0
    static FILE *popen (const char *cmd, const char *mode)
    {
	return ::_popen (cmd, mode);
    }

    static void pclose (FILE *fp)
    {
	::_pclose (fp);
    }
#endif

    typedef short SigState;

    static void block_sigs (SigState &)
    {
        WaitForSingleObject( SignalMutex, INFINITE );
    }

    static void restore_sigs (SigState &)
    {
        ReleaseMutex( SignalMutex);
    }
};

#else // _POCKET_PC

#include <mico/config.h>

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

#ifdef __CYGWIN32__
extern "C" int gettimeofday (struct timeval *, struct timezone *);
#endif

struct OSMisc {
    typedef long long longlong;
    typedef unsigned long long ulonglong;

    static void _init()
    {
    }

    typedef struct timeval TimeVal;

    static TimeVal gettime ()
    {
      struct timeval tv;
#ifdef HAVE_GETTIMEOFDAY_TWOARGS
      gettimeofday (&tv, NULL);
#else
      gettimeofday (&tv);
#endif
      return tv;
    }

    static MICO_ULong timestamp ()
    {
        TimeVal _ts = gettime();
	return (_ts.tv_sec * 1000L) + (_ts.tv_usec / 1000L);
    }

    enum AccessMode {
	ACCESS_READ = R_OK,
	ACCESS_WRITE = W_OK,
	ACCESS_EXEC = X_OK
    };

    static MICO_Long access (const char *file, AccessMode md)
    {
	return ::access (file, md);
    }

    static MICO_Long getpid ()
    {
	return ::getpid();
    }

    static FILE *popen (const char *cmd, const char *mode)
    {
	return ::popen (cmd, mode);
    }

    static void pclose (FILE *fp)
    {
	::pclose (fp);
    }

    typedef sigset_t SigState;

    static void block_sigs (SigState &osigs)
    {
        SigState sigs;
        sigemptyset (&sigs);
        // XXX ...
        sigaddset (&sigs, SIGCHLD);
#ifdef HAVE_PTHREADS
	pthread_sigmask (SIG_BLOCK, &sigs, &osigs);
#else
	sigprocmask (SIG_BLOCK, &sigs, &osigs);
#endif // HAVE_PTHREADS
    }

    static void restore_sigs (SigState &osigs)
    {
#ifdef HAVE_PTHREADS
	pthread_sigmask (SIG_SETMASK, &osigs, NULL);
#else
	sigprocmask (SIG_SETMASK, &osigs, NULL);
#endif // HAVE_PTHREADS
    }
};

#endif

#endif // __mico_os_misc_h__
