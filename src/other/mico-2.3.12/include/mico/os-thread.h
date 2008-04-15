/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  os-thread wrapper
 *  Copyright (C) 1999 Andreas Schultz                                 
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


#ifndef __OS_THREAD_H__
#define __OS_THREAD_H__

#if defined(_WIN32) && !defined(__MINGW32__)
#include <mico/config-win32.h>
#else
#include <mico/config.h>
#endif
	

/*!
 * \defgroup micomt	Threading Support
 *
 * Definitions in this group provide thread support
 * for MICO.
 */

/*!
 * \namespace MICOMT
 * \ingroup micomt
 *
 * The MICOMT namespace contains classes and types that provide
 * a complete multithreaded development environment for the MICO
 * ORB and applications.
 */

//
// Define HAVE_THREADS if any type of thread implementations are defined.
// supported "implementations" are:
//
// HAVE_PTHREADS        - POSIX Draft 10 PThreads [Single UNIX Spec V.2 & X/Open Interfaces]
//                        (i.e. HP/UX 11, Linux PThreads (Xavier LeRoy@inria.fr))
// HAVE_DCE_THREADS     - POSIX Draft 4 (DCE) Threads (i.e. HP/UX 10)
// HAVE_WIN_THREADS     - M$ Windows threads or whatever M$ calls them...
// HAVE_SOLARIS_THREADS - SUN Solaris Threads.
// HAVE_OSPACE_THREADS  - Object Space(r) Thread<ToolKit>(tm) via D10/D4/Linux
//                                                                              

#if defined (HAVE_PTHREADS) || defined (HAVE_DCE_THREADS) ||                         \
    defined (HAVE_SOLARIS_THREADS) || defined (HAVE_PTH_THREADS) ||                 \
    defined (HAVE_WIN_THREADS)

    #ifndef HAVE_THREADS
    #define HAVE_THREADS      // def'd as a convenience
    #endif // HAVE_THREADS

#endif // HAVE_THREADS

#include <mico/mtdebug.h>

#ifdef HAVE_THREADS

//
// OS dependant includes
//
#if defined(HAVE_PTH_THREADS) && defined(HAVE_PTH_H)
#  include <pth.h>
#endif
#if defined(HAVE_PTHREADS) || defined(HAVE_DCETHREADS)
#  ifdef HAVE_PTHREAD_H
#    include <pthread.h>
#  endif 
#  ifdef HAVE_SEMAPHORE_H
#    include <semaphore.h>
#  endif 
#  ifdef HAVE_SCHED_H
#    include <sched.h>
#  endif 
#endif
#if defined(HAVE_SOLARIS_THREADS)
#  ifdef HAVE_THREAD_H
#    include <thread.h>
#  endif 
#  ifdef HAVE_SYNCH_H
#    include <synch.h>
#  endif 
#  ifdef HAVE_SEMAPHORE_H
#    include <semaphore.h>
#  endif 
#endif

// thread startup mode
#define _THR_CREATE_AND_BLOCK
//#define _THR_DEFERED_CREATE

namespace MICOMT {

#ifdef HAVE_PTHREADS
#include <mico/os-thread/pthreads.h>
#endif // HAVE_PTHREADS

#ifdef HAVE_DCE_THREADS
#include <mico/os-thread/dcethreads.h>
#endif // HAVE_DCE_THREADS

#ifdef HAVE_PTH_THREADS
#include <mico/os-thread/pththreads.h>
#endif // HAVE_PTH_THREADS

#ifdef HAVE_WIN_THREADS
#error not yet
#endif // HAVE_WIN_THREADS

#ifdef HAVE_SOLARIS_THREADS
#include <mico/os-thread/solaris-threads.h>
#endif // HAVE_SOLARIS_THREADS

    //
    // auto locks
    //

    /*!
     * \ingroup micomt
     * The AutoLock class is better way to work with Mutex class, so
     * instead of:
     * \code
     * {
     *   my_mutex.lock();
     *   // do something
     *   my_mutex.unock();
     * }
     * \endcode
     * the user can use:
     * \code
     * {
     *   AutoLock l(my_mutex);
     *   // do something
     * }
     * \endcode
     * The main advantage here is that it'll automatically unlock
     * mutex even in case of throwing exception
     */
    class AutoLock
    {
	Mutex& _m;
    public:
	//! \name Constructor/Destructor
	//@{
	/*!
	 * \param m  The mutex to lock
	 * The constructor will lock the supplied mutex.
	 */
	AutoLock(Mutex& m)
	    : _m(m)
	{
	    m.lock();
	};

	/*!
	 * The destructor unlocks the mutex.
	 */
	~AutoLock()
	{
	    _m.unlock();
	};
	//@}
    };


    /*!
     * \ingroup micomt
     * The AutoRDLock is usefull for reader-writer locks in the same
     * way as AutoLock class is for mutexes, but this class will obtain
     * reader lock.
     */
    class AutoRDLock
    {
	RWLock& _l;
    public:
	/*!
	 * \name Constructor/Destructor
	 */
	//@{
	/*!
	 * \param l  The lock to use
	 * The constructor will obtain reader lock
	 */
	AutoRDLock(RWLock& l)
	    : _l(l)
	{
	    l.rdlock();
	};

	/*!
	 * The destructor frees obtained read lock
	 */
	~AutoRDLock()
	{
	    _l.unlock();
	};
	//@}
    };


    /*!
     * \ingroup micomt
     * The AutoWRLock is usefull for reader-writer locks in the same
     * way as AutoLock class for mutexes, but this class will obtain
     * writer lock
     */
    class AutoWRLock
    {
	RWLock& _l;
    public:
	//! \name Constructor/Destructor
	//@{
	/*!
	 * \param l  The lock to use
	 * The constructor will obtain writer lock
	 */
	AutoWRLock(RWLock& l)
	    : _l(l)
	{
	    l.wrlock();
	};

	/*!
	 * The destructor frees obtained write lock
	 */
	~AutoWRLock()
	{
	    _l.unlock();
	};
	//@}
    };


    //
    // lock able classes
    //

    /*!
     * \ingroup micomt
     * The Locked class can be used to make a class lockable
     * by inheriting from both T and RWLock.
     *
     * \param T  The class to make lockable
     *
     * Example: (Queue is queue of integers)
     * \code
     * Locked<Queue> queue_;
     * ...
     * {
     *   AutoLock l(queue_);
     *   queue_.push(10);
     * }
     * \endcode
     */
    template <class T>
    class Locked: public T, public Mutex
    {
    public:
	Locked()
	{}
	Locked(MICO_Boolean locked, Attribute rec)
	    : Mutex(locked, rec)
	{}
    };


    /*!
     * \ingroup micomt
     * The RWLocked class can be used to make a class lockable
     * by inheriting from both T and RWLock.
     *
     * \param T		The class type to make lockable.
     */
    template <class T>
    class RWLocked: public T, public RWLock
    {
    };


    //
    // state controlled reference counting
    //

    /*!
     * \ingroup micomt
     * The StateRefCnt class is used to control stated reference counting for
     * objects used by more than one thread.
     *
     * Rules:
     * \li active_ref() will only succed if the object is "Active"
     * \li active_deref() will wake threads waiting to change the objects state
     *   if the ref count is 0
     * \li a StateChange will only succed after ref count is 0
     */
    class StateRefCnt {
    public:
	/*!
	 * \enum ExecState
	 * The ExecState defines the state of the reference count.
	 */
	enum ExecState {
	    Init,				//!< Initialized
	    Active,				//!< Active
	    InitShutdown,			//!< Shutdown started
	    Shutdown,			        //!< Shut down
	    Terminated			        //!< Terminated
	};

    protected:
	ExecState  _current_state;		//!< Current state
	ExecState  _new_state;		        //!< The new state
	MICO_ULong _activerefs;		        //!< Active references
	Mutex      _mutex;			//!< Lock for the ref counter
	CondVar    _cond;			//!< Condition var fof the counter

    public:
	//! \name Constructor/Destructor
	//@{
	/*!
	 * Default constructor.
	 */
	StateRefCnt()
	    : _current_state(Init), _new_state(Init), _activerefs(0),
	      _cond(&_mutex)
	{};

	/*!
	 * Destructor.
	 */
	~StateRefCnt()
	{};
	//@}

	/*!
	 * Increment the active reference count if the state is
	 * active.
	 *
	 * \return  False if the object is not active.
	 */
	MICO_Boolean
	active_ref()
	{
	    AutoLock l(_mutex);

	    if ((_current_state != _new_state) && (_current_state != Active)) {
		return FALSE;
	    }
	    _activerefs++;
	    return TRUE;
	};

	/*!
	 * Decrement the active reference counts. If the count reaches
	 * 0, the condition variable is broadcasted on.
	 */
	void
	active_deref()
	{
	    AutoLock l(_mutex);

	    _activerefs--;
	    if ( (_activerefs == 0) && (_new_state != Active) )
		_cond.broadcast();
	};

	/*!
	 * This function changes the current state to the requested
	 * state.
	 *
	 * \param s  The requested state
	 * \return  True on success, false on failure
	 */
	MICO_Boolean
	state_change(ExecState s)
	{
	    AutoLock l(_mutex);

	    if ( s <= _current_state ) {
		return FALSE;
	    }
	    _new_state = s;
	    //wait until the state change has become effective
	    while (_activerefs != 0) {
		_cond.wait();
	    }
	    if ( _new_state <= _current_state )
		return FALSE;

	    _current_state = _new_state;
	    return TRUE;
	};

	/*!
	 * Get the current state.
	 *
	 * \return  The current state
	 */
	ExecState
	state() const
	{ return _current_state; };

	/*!
	 * This method puts the state into InitShutdown if
	 * it is Active.
	 *
	 * \return  True on success, false on failure.
	 */
	MICO_Boolean
	init_shutdown()
	{
	    AutoLock l(_mutex);

	    if (_new_state != Active ) {
		return FALSE;
	    }
	    _current_state = _new_state = InitShutdown;
	    return TRUE;
	};
    };
}

#if defined(_WIN32) && defined(NEED_THREADID_OPERATORS)
// kcg: we need to define comparison operator for recent (from snap-2004-11-22 up)
// POSIX Win32 threading, since pthread_t is no longer a pointer or long int type
// Note that the better solution would be to investigate pthread_equal function
// usage
bool
operator==(MICOMT::Thread::ThreadID first, MICOMT::Thread::ThreadID second);

bool
operator!=(MICOMT::Thread::ThreadID first, MICOMT::Thread::ThreadID second);

std::ostream&
operator<<(std::ostream& out, MICOMT::Thread::ThreadID id);
#endif // _WIN32 && NEED_THREADID_OPERATORS

#else // HAVE_THREADS
//
// The user should use following code for making code better portable
// between single and multi threaded environment i.e. the user can use
// Mutex, CondVar, Semaphore etc. classes even while building
// on single-threaded environment - so w/o any #ifdef HAVE_THREADS #endif
//
// Following code is w/o doxygen documentation because doxyfile defines
// HAVE_PTHREADS and so it's not used for generating of documentation
//
namespace MICOMT {

    class Mutex
#ifdef DEBUG_NAMES
	: public NamedObject
#endif // DEBUG_NAMES
    {
    public:
	enum Attribute
	{
	    Normal,
	    Recursive
	};

	Mutex(MICO_Boolean locked = FALSE, Attribute attr = MICOMT::Mutex::Normal)
	{};
	~Mutex()
	{};
    
	MICO_ULong
	trylock()
	{ return 1; };

	void
	lock()
	{};

	void
	unlock()
	{};
    };


    class CondVar
#ifdef DEBUG_NAMES
	: public NamedObject
#endif // DEBUG_NAMES
    {
    public:
	CondVar(Mutex*)
	{};
	~CondVar()
	{};

	MICO_Boolean
	wait()
	{ return 1; };

	MICO_Boolean
	timedwait(MICO_ULong)
	{ return 1; };

	void
	broadcast()
	{};

	void
	signal()
	{};
    };


    class RWLock
#ifdef DEBUG_NAMES
	: public NamedObject
#endif // DEBUG_NAMES
    {
    public:
	RWLock()
	{};
	~RWLock()
	{};

	void
	rdlock()
	{};

	void
	wrlock()
	{};

	void
	unlock()
	{};
    };


    class Semaphore
#ifdef DEBUG_NAMES
	: public NamedObject
#endif // DEBUG_NAMES
    {
    public:
	enum ErrorType {
	    NoError,		//!< No error on semaphore
	    NoPermission,		//!< Permission denied
	    TryAgain,		//!< Try again
	    SemInvalid,		//!< Invalide semaphore
	    Interrupted,		//!< Interrupted by signal
	    UnknownError		//!< Unknow error
	};

	Semaphore(unsigned int)
	{};
	~Semaphore()
	{};

	void
	wait()
	{};

	Semaphore::ErrorType
	trylock()
	{ return MICOMT::Semaphore::NoError; };

	void
	post()
	{};
    };


    class AutoRDLock
    {
    public:
	AutoRDLock(RWLock& l)
	{};
	~AutoRDLock()
	{};
    };


    class AutoWRLock
    {
    public:
	AutoWRLock(RWLock& l)
	{};
	~AutoWRLock()
	{};
    };


    template <class T>
    class RWLocked: public T, public RWLock
    {
    };


    class AutoLock
    {
    public:
	AutoLock(Mutex& m)
	{};
	~AutoLock()
	{};
    };


    template <class T>
    class Locked: public T, public Mutex
    {
    public:
	Locked()
	{}
	Locked(MICO_Boolean locked, Attribute rec)
	    : Mutex(locked, rec)
	{}
    };

} // MICOMT
#endif // HAVE_THREADS

#endif // __OS_THREAD_H__

