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
 *  support for POSIX Draft 10ish Pthreads for Linux as developed by
 *  Xavier LeRoy@inria.fr
 *
 */

#define MICO_CONF_IMR

#include <CORBA-SMALL.h>
#include <mico/impl.h>

//
// Global things (used/defined ONLY when HAVE_THREADS is defined)
//
#ifdef HAVE_THREADS  

static pthread_mutex_t __debug_mutex = PTHREAD_MUTEX_INITIALIZER;

void __mtdebug_init()
{
}

void __mtdebug_lock()
{
    pthread_mutex_lock( &__debug_mutex );
}

void __mtdebug_unlock()
{
    pthread_mutex_unlock( &__debug_mutex );
}

//
// No matter what the actual thread implementation is, _init ()
// takes care of initializing whatever has to be initialized.
//
void MICOMT::_init ()
{
  pth_init();
  if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
    __mtdebug_lock();
    MICO::Logger::Stream (MICO::Logger::Thread)
      << "MICOMT::_init()" << endl;
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
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "ThreadWrapper(): after calling _run()" << endl;
	__mtdebug_unlock();
    }
    return NULL;          // the "start_routine" must return a void *.
}

/*********************************** Mutex implementation **********************************/
//
//   lock() and unlock() are inlined for optimal speed
//
	
MICO_ULong
MICOMT::Mutex::trylock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Mutex::trylock ()" << endl;
	__mtdebug_unlock();
    }
    switch ( pth_mutex_acquire(&_mutex, TRUE, NULL) ) {
    case 0:      return MICOMT::Thread::NoError;
    case EINVAL: return MICOMT::Thread::InvalidPriority;
    case EBUSY:  return MICOMT::Thread::MutexAlreadyLocked;
    };
    return MICOMT::Thread::UnknownError;
}

/***************************** condition variable ************************************/

MICO_Boolean
MICOMT::CondVar::timedwait( Mutex &_mutex, MICO_ULong tmout )
{ 
    pth_event_t ev;
    static pth_key_t ev_key = PTH_KEY_INIT;

    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": CondVar::timedwait ()" << endl;
	__mtdebug_unlock();
    }

    ev = pth_event(PTH_EVENT_TIME|PTH_MODE_STATIC, &ev_key,
		   pth_time( 0, tmout ));
    if (!pth_cond_await( &_cond, &_mutex._mutex, ev))
        return FALSE;
    return TRUE;
}

/*********************** Semaphore implementation/emulation **********************************/

void
MICOMT::Semaphore::wait()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Semaphore::wait ()" << endl;
	__mtdebug_unlock();
    }
    //_cond.lock();
    //MICOMT::AutoLock __lock(_mutex);
    _mutex.lock();
    //pth_cleanup_push( reinterpret_cast<void (*)(void *)>(&&MICOMT::CondVar::unlock, this ));
    //pth_cleanup_push( reinterpret_cast<void (*)(void *)>(&&MICOMT::Mutex::unlock, this ));
    void (MICOMT::Mutex::*unlock_method)(void) = &MICOMT::Mutex::unlock;
    //pth_cleanup_push((_mutex.*unlock_method),0);
    while (count <= 0)
	_cond.wait(_mutex);
    
    --count;
    _mutex.unlock();
    //pth_cleanup_pop( 1 );
}

void
MICOMT::Semaphore::post()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Semaphore::post ()" << endl;
	__mtdebug_unlock();
    }
    //_cond.lock();
    _mutex.lock();
    if (count++ == 0)
	_cond.signal();
    //_cond.unlock();
    _mutex.unlock();
}

/********************* Thread implementation for GNU PthThreads ********************************/

MICOMT::Thread::Thread (MICOMT::Thread::DetachFlag detached) __NAME( :NamedObject () )
{
    __NAME(name ("<Un-Named Thread>"));
    _detached = detached;
 
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    <<"Thread::Thread (detached = " << (int)(char)detached << ")" << endl;
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
    return ( thread.id() == id() );
}

//
// set the priority of this thread to `new_priority'.
// let's avoid using this if at all possible, some pthreads impls don't
// support it fully or at all:
//
void
MICOMT::Thread::priority (MICO_Long new_priority)
{
  pth_attr_t __attr = pth_attr_of(this->id());
  pth_attr_set(__attr, PTH_ATTR_PRIO, new_priority);
  pth_attr_destroy(__attr);
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
