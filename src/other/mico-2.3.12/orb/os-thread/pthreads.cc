/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
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

#include <pthread.h>
#include <CORBA-SMALL.h>


using namespace std;

//
// Global things (used/defined ONLY when HAVE_THREADS is defined)
//
#ifdef HAVE_THREADS  

static pthread_mutex_t __debug_mutex = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t __debug_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/*!
 * \ingroup micomt
 *
 * This function initializes the MICOMT debug module
 */
void __mtdebug_init()
{ 
}

/*!
 * \ingroup micomt
 *
 * This method locks the debug mutex.
 */
void __mtdebug_lock()
{
    int result = pthread_mutex_lock(&__debug_mutex);
    assert (!result);
}

/*!
 * \ingroup micomt
 *
 * This method unlocks the debug mutex.
 */
void __mtdebug_unlock()
{
    int result = pthread_mutex_unlock(&__debug_mutex);
    assert (!result);
}

//
// No matter what the actual thread implementation is, _init ()
// takes care of initializing whatever has to be initialized.
//
/*!
 * \ingroup micomt
 *
 * This method will perform any initialization of the threading
 * subsystem. For pthread, there is none.
 */
void MICOMT::_init ()
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "MICOMT::_init()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
}

//
// Thread::ThreadWrapper is a static member function that
// pthread_create(...) (or the like) can call as its "thread start routine".
// Any object whose methods are to be invoked as a thread, must derive from
// the class "Thread".  Thread has a method, _run(),
// which starts the Thread (or a derivative) "in motion".
//
/*!
 * \param arg		The arguments to the thread context
 * \return		A return value or pointer.
 *
 * The ThreadWrapper method is passed to pthread_create() when
 * a new thread is created. The argument is always a pointer to
 * the thread object. This method calls _thr_startup on the thread.
 */
void*
MICOMT::Thread::ThreadWrapper(void* arg)
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "ThreadWrapper()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    static_cast<Thread*>(arg)->_thr_startup(arg);
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "ThreadWrapper(): after calling _run()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    return NULL;          // the "start_routine" must return a void *.
}


void
MICOMT::Thread::_thr_startup(void *arg)
{
#ifdef _THR_CREATE_AND_BLOCK
    _ready.lock();
#endif // _THR_CREATE_AND_BLOCK
    try {
        this->_run(arg);
    }
    catch (const CORBA::Exception& ex) {
        __mtdebug_lock();
        cerr << "thread: " << this->id() << " uncaught MICO exception: ";
        ex._print (cerr);
        cerr << endl;
        ex._print_stack_trace (cerr);
        __mtdebug_unlock();
        //throw;
    }
    catch (...) {
        __mtdebug_lock();
        cerr << "thread: " << this->id() << " uncaught unknown exception"
             << endl;
        __mtdebug_unlock();
    }
#ifdef _THR_CREATE_AND_BLOCK
    _ready.unlock();
#endif // _THR_CREATE_AND_BLOCK
}


//
// reader/writer implementation
//   xxlock() and unlock() are inlined for optimal speed
//
/*!
 * The constructor initializes a pthread_rwlock.
 */
MICOMT::RWLock::RWLock()
#ifdef DEBUG_NAMES
    : NamedObject()
#endif // DEBUG_NAMES
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "RWLock::RWLock ()" << endl;
	__mtdebug_unlock();
    }
    __NAME (name ("<UnNamed RWLock>"));
#endif // MTDEBUG
    int result = pthread_rwlock_init(&_rwlock, NULL);
    assert(!result);
}

/*!
 * The destructor frees the resources used by the lock.
 */
MICOMT::RWLock::~RWLock()
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME (<< name ())
		    << ": RWLock::~RWLock ()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    int result = pthread_rwlock_destroy(&_rwlock);
    assert(!result);
}


#if defined(HAVE_SEMAPHORE_H) && !(defined(__APPLE__) && defined(__MACH__))

// the OS provides POSIX semaphore implementation
// in semaphore.h header file

// kcg: in the case of MacOS X 10.2 (Darwin 6.8) we have an issue with
// POSIX semaphore initialization, so we use MICO's own semaphore
// implementation on this platform

//
// Semaphore implementation
//   post() and wait() are inlined for optimal speed
//
/*!
 * The constructor initializes the semaphore.
 *
 * \param val The initial value of the semaphore
 */
MICOMT::Semaphore::Semaphore(unsigned int val)
#ifdef DEBUG_NAMES
    : NamedObject()
#endif // DEBUG_NAMES
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Semaphore::Semaphore (val = " << val << ")" << endl;
	__mtdebug_unlock();
    }
    __NAME (name ("<UnNamed Semaphore>"));
#endif // MTDEBUG
    int result = sem_init(&_s, 0, val);
    assert(result == 0); 
}

/*!
 * The destructor frees the resources used by the semaphore.
 */
MICOMT::Semaphore::~Semaphore()
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME (<< name ())
		    << ": Semaphore::~Semaphore ()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    sem_destroy(&_s);
}

#else // HAVE_SEMAPHORE_H && !(__APPLE__ && __MACH__)

// the OS doesn't provide POSIX semaphore implementation
// in semaphore.h header file so we will use own
// implementation based on mutex and condition variable

void
MICOMT::Semaphore::wait()
{
    MICOMT::AutoLock t_lock(_mutex);
    while(_counter == 0)
	_condition.wait();
    _counter--;
}


MICOMT::Semaphore::ErrorType
MICOMT::Semaphore::trylock()
{
    MICOMT::AutoLock T_lock(_mutex);
    if (_counter == 0)
	return MICOMT::Semaphore::TryAgain;
    return MICOMT::Semaphore::NoError;
}


void
MICOMT::Semaphore::post()
{
    {
	MICOMT::AutoLock t_lock(_mutex);
	_counter++;
    }

    _condition.signal();
}
    
#endif // HAVE_SEMAPHORE_H && !(__APPLE__ && __MACH__)


//
// Mutex implementation
//   lock() and unlock() are inlined for optimal speed
//
/*!
 * The constructor initializes the mutex with the specified
 * attribute and locks it, if specified.
 *
 * \param locked  If true, the mutex is created locked
 * \param attr  The type of mutex (default is normal).
 */
MICOMT::Mutex::Mutex(MICO_Boolean locked, Attribute attr)
#ifdef DEBUG_NAMES
    : NamedObject()
#endif // DEBUG_NAMES
{
    int result;
    pthread_mutexattr_t m_attr;
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "Mutex::Mutex (initially locked = "
	    << ((locked == FALSE) ? "FALSE" : "TRUE")
	    << ", type = "
	    << ((attr == Normal) ? "Normal" : "Recursive")
	    << ")" << endl;
	__mtdebug_unlock();
    }
    __NAME (name ("<UnNamed Mutex>"));
#endif // MTDEBUG

#ifdef SOLARIS_MUTEX
    _id = 0;
    _cnt = 0;
    _rec = 0;
#endif
    result = pthread_mutexattr_init(&m_attr);
    assert(!result);
    if (attr != Normal) {
	switch (attr) {
	case Recursive:
#ifdef SOLARIS_MUTEX
	    _rec = 1;
#else // SOLARIS_MUTEX
            result = pthread_mutexattr_settype(&m_attr,
					       PTHREAD_MUTEX_RECURSIVE);
	    assert (!result);
#endif // SOLARIS_MUTEX
	    break;
	default:
	    break;
	}
    }
    result = pthread_mutex_init(&_mutex, &m_attr);
    assert(!result);
    result = pthread_mutexattr_destroy(&m_attr); 
    assert(!result);
    if (locked)
	this->lock();
}

/*!
 * The destructor frees resources used by the mutex.
 */
MICOMT::Mutex::~Mutex()
{ 
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME (<< name ())
		    << ": Mutex::~Mutex ()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
#ifdef SOLARIS_MUTEX
    assert(_cnt==0);
#endif
    int result = pthread_mutex_destroy(&_mutex);
    assert(!result);
}
	
//
// condition variable
//
/*!
 * The constructor initializes the condition variable.
 */
MICOMT::CondVar::CondVar(MICOMT::Mutex* mut)
    : _mutex(mut)
{
#ifdef MTDEBUG
    __NAME( name ("<UnNamed CondVar>") );
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME (<< name ())
		    << ": ContVar::CondVar ()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    int result = pthread_cond_init(&_cond, NULL);
    assert(!result);
}

/*!
 * The destructor frees the resources used by the condition variable.
 */
MICOMT::CondVar::~CondVar()
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    __NAME (<< name ())
		    << ": ContVar::~CondVar ()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    int result = pthread_cond_destroy(&_cond);
    assert (!result);
}


//
// Thread implementation for LinuxThreads
//
/*!
 * The constructor initializes the thread. The thread can
 * be created attached or detached.
 *
 * \param detached  Whether the thread is created attached or detached
 */
MICOMT::Thread::Thread(MICOMT::Thread::DetachFlag detached)
#ifdef DEBUG_NAMES
    : NamedObject()
#endif // DEBUG_NAMES
{
    _detached = detached;
#ifdef MTDEBUG
    __NAME (name ("<Un-Named Thread>"));
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    <<"Thread::Thread (detached = " << (int)(char)detached << ")" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
#ifdef _THR_CREATE_AND_BLOCK
    _start_error = this->create_thread();
#endif
}
 
//
// destoying the object that runs our thread is not a good idea
// kill it first
//
/*!
 * The destructor waits for the thread to exit before
 * cleaning up resources.
 */
MICOMT::Thread::~Thread ()
{
    // make sure the thread is really finished, before we remove it data structures 
    this->wait();
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Thread::~Thread()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
}

//
// Start the thread ala JAVA:
//
/*!
 * This method starts the thread.
 *
 * \param arg  Parameters to the thread.
 */
MICOMT::Thread::ErrorType 
MICOMT::Thread::start(void* arg)
{
    //FIXME: whats that for ?
    _arg = arg;
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Thread::start (void *arg)" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
#ifdef _THR_CREATE_AND_BLOCK
    _ready.unlock();
    return this->_start_error;
#else // _THR_CREATE_AND_BLOCK
    return this->create_thread();
#endif // _THR_CREATE_AND_BLOCK
}
 
//
// see if the thread given by ThreadID `id' is the same as this
// thread:
//
/*!
 * \param thread  The thread to compare against
 * \return  True if the threads are the same, false otherwise
 */
MICO_Boolean
MICOMT::Thread::operator==(const MICOMT::Thread &thread) const
{
#ifdef MTDEBUG
    if (MICO::Logger::IsLogged(MICO::Logger::Thread)) {
	__mtdebug_lock();
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Thread::operator == ()" << endl;
	__mtdebug_unlock();
    }
#endif // MTDEBUG
    return (MICO_Boolean)pthread_equal(thread.id(), this->id());
}

//
// set the priority of this thread to `new_priority'.
// let's avoid using this if at all possible, some pthreads impls don't
// support it fully or at all:
//
/*!
 * \param new_priority	The new priority for the thread.
 * Set a new priority for the thread.
 *
 * \bug
 * This method is not implemented.
 *
 * \todo
 * Implement this function??
 */
void
MICOMT::Thread::priority(MICO_Long new_priority)
{
}

#if defined(_WIN32) && defined(NEED_THREADID_OPERATORS)
// kcg: we need to define comparison operator for recent (from snap-2004-11-22 up)
// POSIX Win32 threading, since pthread_t is no longer a pointer or long int type
// Note that the better solution would be to investigate pthread_equal function
// usage
bool
operator==(MICOMT::Thread::ThreadID first, MICOMT::Thread::ThreadID second)
{
	return (first.x == second.x) && (first.p == second.p);
}

bool
operator!=(MICOMT::Thread::ThreadID first, MICOMT::Thread::ThreadID second)
{
	return (first.x != second.x) || (first.p != second.p);
}

ostream&
operator<<(ostream& out, MICOMT::Thread::ThreadID id)
{
	out << (void*)id.p << "(" << id.x << ")";
	return out;
}
#endif // _WIN32 && NEED_THREADID_OPERATORS

#endif // HAVE_THREADS
