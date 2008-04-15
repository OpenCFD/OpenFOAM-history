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
 *  support for Draft 4 (DCE) threads
 *
 */

#define MICO_CONF_IMR

#include <pthread.h>
#include <CORBA-SMALL.h>

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
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "ThreadWrapper(): after calling _run()" << endl;
	__mtdebug_unlock();
    }
    return NULL;          // the "start_routine" must return a void *.
}

/******* Mutex implementation *************************************************************************/
//
//   lock() and unlock() are inlined for optimal speed
//
MICOMT::Mutex::Mutex( MICO_Boolean locked, Attribute attr ) __NAME( :NamedObject () )
{
    __NAME(name ("<UnNamed Mutex>"));

    pthread_mutexattr_t m_attr;

    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "Mutex::Mutex (locked = " << (int)locked << ", " 
	    << " attr = " << attr << ")" << endl;
	__mtdebug_unlock();
    }
    pthread_mutexattr_create( &m_attr);
    if ( attr != Normal ) {
	switch (attr) {
	case Recursive: {
	    pthread_mutexattr_setkind_np( &m_attr, MUTEX_RECURSIVE_NP );
	    break;
	}
	}
    }
    assert( pthread_mutex_init(&_mutex, m_attr) == 0 );
    pthread_mutexattr_delete( &m_attr);
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
    pthread_mutex_destroy(&_mutex);
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
    switch (pthread_mutex_trylock (&_mutex)) {
    case 0:      return MICOMT::Thread::NoError;
    case EINVAL: return MICOMT::Thread::InvalidPriority;
    case EBUSY:  return MICOMT::Thread::MutexAlreadyLocked;
    };
    return MICOMT::Thread::UnknownError;
}

/******** condition variable ***************************************************************************/

MICOMT::CondVar::CondVar()
{
    pthread_condattr_t  c_attr;

    __NAME( name ("<UnNamed CondVar>") );
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": ContVar::CondVar ()" << endl;
	__mtdebug_unlock();
    }

    pthread_condattr_create ( &c_attr );
    pthread_cond_init(&_cond, c_attr );
    pthread_condattr_delete ( &c_attr );
}

MICOMT::CondVar::~CondVar()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": CondVar::~CondVar ()" << endl;
	__mtdebug_unlock();
    }
    pthread_cond_destroy(&_cond);
}

/********* reader/writer implementation ***************************************************************/

MICOMT::RWLock::RWLock() __NAME( : NamedObject() )
{
    __NAME(name ("<UnNamed RWLock>"));
    pthread_mutexattr_t m_attr;
    pthread_condattr_t  c_attr;
    
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "RWLock::RWLock ()" << endl;
	__mtdebug_unlock();
    }
    pthread_mutexattr_create  ( &m_attr );
    pthread_condattr_create ( &c_attr );

    pthread_mutex_init( &lock, m_attr );
 
    pthread_cond_init( &readers, c_attr );
    pthread_cond_init( &writers, c_attr );
    
    state = waiters = 0;

    pthread_condattr_delete  (&c_attr);
    pthread_mutexattr_delete (&m_attr);
}

MICOMT::RWLock::~RWLock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": RWLock::~RWLock ()" << endl;
	__mtdebug_unlock();
    }
    pthread_mutex_destroy( &lock );
    pthread_cond_destroy( &readers );
    pthread_cond_destroy( &writers );
}

void MICOMT::RWLock::rdlock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": RWLock::rdlock ()" << endl;
	__mtdebug_unlock();
    }
    pthread_mutex_lock( &lock );
    pthread_cleanup_push( reinterpret_cast<void (*)(void *)>(pthread_mutex_unlock),
			  &lock );
    while ( state < 0 && waiters)
	pthread_cond_wait( &readers, &lock );
    state++;
    pthread_cleanup_pop( 1 );         
}

void MICOMT::RWLock::wrlock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": RWLock::wrlock ()" << endl;
	__mtdebug_unlock();
    }
    pthread_mutex_lock( &lock );
    waiters++;
    pthread_cleanup_push( reinterpret_cast<void (*)(void *)>(pthread_mutex_unlock),
			  &lock );
    while ( state < 0 && waiters)
	pthread_cond_wait( &writers, &lock );
    state = -1;
    pthread_cleanup_pop( 1 );
}

void MICOMT::RWLock::unlock()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": RWLock::unlock ()" << endl;
	__mtdebug_unlock();
    }
    pthread_mutex_lock( &lock );
    if (state  == -1) {                                     // a writer is releasing
	state = 0;                                          // mark as available
	if (waiters)                                        // other queued writers?
	    pthread_cond_signal( &writers );
	else
	    pthread_cond_broadcast( &readers );
    } else {
	if (--state == 0)                                   // No more readers
	    pthread_cond_signal( &writers );
    }
    pthread_mutex_unlock( &lock );
}

MICO_Boolean MICOMT::RWLock::trywrlock()
{
    MICO_Boolean status = false;
    pthread_mutex_lock( &lock );
    if (!state && !waiters) { // No readers, no writers && no writers queued
	state = -1;
	status = true;
    }
    pthread_mutex_unlock( &lock );
    return ( status );
}

/********* Semaphore implementation *******************************************************************/
//
//   post() and wait() are inlined for optimal speed
//
MICOMT::Semaphore::Semaphore( unsigned int val ) __NAME( : NamedObject () )
{
    __NAME(name ("<UnNamed Semaphore>"));

    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Semaphore::Semaphore (val = " << (int)val << ")" << endl;
	__mtdebug_unlock();
    }
    assert( sem_init( &s, 0, val) == 0 ); 
}

MICOMT::Semaphore::~Semaphore()
{
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME(<< name ()) << ": Semaphore::~Semaphore ()" << endl;
	__mtdebug_unlock();
    }
    sem_destroy( &s );
}


/********* Thread implementation for DCE threads ********************************************************/

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
    return (MICO_Boolean)pthread_equal (thread.id(), id() );
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
