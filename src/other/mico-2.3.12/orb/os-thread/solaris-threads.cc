/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  OSThread: An abstract Thread class for MICO
 *  Copyright (C) 1999 Andy Kersting & Andreas Schultz
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
 *
 *  support for Solaris-Threads
 *
 */

#define MICO_CONF_IMR

#include <thread.h>
#include <synchb.h>
#include <CORBA-SMALL.h>

//
// Global things (used/defined ONLY when HAVE_THREADS is defined)
//
#ifdef HAVE_THREADS  

static mutex_t __debug_mutex;

void __mtdebug_init()
{
    mutex_init(&__debug_mutex, USYNC_THREAD, 0);
}

void __mtdebug_lock()
{
    mutex_lock( &__debug_mutex );
}

void __mtdebug_unlock()
{
    mutex_unlock( &__debug_mutex );
}

//
// No matter what the actual thread implementation is, _init ()
// takes care of initializing whatever has to be initialized.
//
static void MICOMT::_init (MICO_ULong _tpsize, MICO_ULong _tpincr) {
 
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "_init (_tpsize = " << _tpsize << ", _tpincr = "
	    << _tpincr << ")" << endl;
	__mtdebug_unlock();
    }
}

//
// Thread::ThreadWrapper is a static member function that
// pthread_create(...) (or the like) can call as its "thread start routine".
// Any object whose methods are to be invoked as a thread, must derive from
// the class "Thread".  Thread has a method, _run(),
// which starts the Thread (or a derivative) "in motion".
//
void *
MICOMT::Thread::ThreadWrapper( void *arg )
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "ThreadWrapper()" << endl;
	__mtdebug_unlock();
    }
    static_cast<Thread *>(arg)->_thr_startup( arg );
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock(); MICO::Logger::Stream (MICO::Logger::Thread)
	    << "ThreadWrapper(): after calling _run()" << endl;
	__mtdebug_unlock();
    }
    return NULL;          // the "start_routine" must return a void *.
}

//
// reader/writer implementation
//   xxlock() and unlock() are inlined for optimal speed
//
MICOMT::RWLock::RWLock() __NAME( : NamedObject () )
{
    __NAME(name ("<UnNamed RWLock>"));

    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "RWLock::RWLock ()" << endl;
	__mtdebug_unlock();
    }
    rwlock_init( &_rwlock, USYNC_THREAD, 0 );
}

MICOMT::RWLock::~RWLock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": RWLock::~RWLock ()" << endl;
	__mtdebug_unlock();
    }
    rwlock_destroy( &_rwlock );
}

//
// Semaphore implementation
//   post() and wait() are inlined for optimal speed
//
MICOMT::Semaphore::Semaphore( unsigned int val ) __NAME( : NamedObject () )
{
    __NAME(name ("<UnNamed Semaphore>"));

    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Semaphore::Semaphore (val = " << val << ")" << endl;
	__mtdebug_unlock();
    }
    assert( sema_init( &s, val, USYNC_THREAD, 0 ) == 0 ); 
}

MICOMT::Semaphore::~Semaphore()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Semaphore::~Semaphore ()" << endl;
	__mtdebug_unlock();
    }
    sema_destroy( &s );
}

//
// Mutex implementation
//   lock() and unlock() are inlined for optimal speed
//
MICOMT::Mutex::Mutex( MICO_Boolean locked, Attribute attr ) __NAME( :NamedObject () )
{
    __NAME(name ("<UnNamed Mutex>"));

    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "Mutex::Mutex (locked = " << locked << ", " 
	    << " attr = " << attr << ")" << endl;
	__mtdebug_unlock();
    }

    assert( mutex_init(&_mutex, USYNC_THREAD, 0) == 0 );
    if (locked) lock();
}

MICOMT::Mutex::~Mutex()
{ 
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Mutex::~Mutex ()" << endl;
	__mtdebug_unlock();
    }
    mutex_destroy(&_mutex);
}
	
MICO_ULong
MICOMT::Mutex::trylock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Mutex::trylock ()" << endl;
	__mtdebug_unlock();
    }
    switch (mutex_trylock (&_mutex)) {
    case 0:      return MICOMT::Thread::NoError;
    case EINVAL: return MICOMT::Thread::InvalidPriority;
    case EBUSY:  return MICOMT::Thread::MutexAlreadyLocked;
    };
    return MICOMT::Thread::UnknownError;
}

//
// condition variable
//
MICOMT::CondVar::CondVar()
{
    __NAME( name("<UnNamed CondVar>" ) );
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": ContVar::CondVar ()" << endl;
	__mtdebug_unlock();
    }

    cond_init(&_cond, USYNC_THREAD, 0);
}

MICOMT::CondVar::~CondVar()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": ContVar::~CondVar ()" << endl;
	__mtdebug_unlock();
    }

    cond_destroy(&_cond);
}


//
// Thread implementation for LinuxThreads
//
MICOMT::Thread::Thread (MICOMT::Thread::DetachFlag detached) __NAME( :NamedObject () )
{
    __NAME(name ("<Un-Named Thread>"));

    _detached = detached;
 
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    <<"Thread::Thread (detached = " << (int)(char)detached
	    << ")" << endl;
	__mtdebug_unlock();
    }
#ifdef _THR_CREATE_AND_BLOCK
    create_thread();
#endif
}
 
//
// Start the thread ala JAVA:
//
MICOMT::Thread::ErrorType 
MICOMT::Thread::start (void *arg)
{
    //FIXME: whats that for ?
    _arg = arg;
 
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Thread::start (void *arg)" << endl;
	__mtdebug_unlock();
    }
#ifdef _THR_CREATE_AND_BLOCK
    _ready.unlock();
    return MICOMT::Thread::NoError;
#else
    return create_thread();
#endif
}
 
//
// see if the thread given by ThreadID `id' is the same as this
// thread:
//
MICO_Boolean
MICOMT::Thread::operator == (const MICOMT::Thread &thread) const
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Thread::operator == ()" << endl;
	__mtdebug_unlock();
    }
    return (MICO_Boolean) (thread.id() == id() );
}

//
// set the priority of this thread to `new_priority'.
// let's avoid using this if at all possible, some pthreads impls don't
// support it fully or at all:
//
void
MICOMT::Thread::priority (MICO_Long new_priority) {
}

//
// destoying the object that runs our thread is not a good idea
// kill it first
//
MICOMT::Thread::~Thread ()
{
    // make sure the thread is really finished, before we remove it data structures 
    wait();
    _ready.unlock();
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Thread::~Thread()" << endl;
	__mtdebug_unlock();
    }
}


#endif // HAVE_THREADS
