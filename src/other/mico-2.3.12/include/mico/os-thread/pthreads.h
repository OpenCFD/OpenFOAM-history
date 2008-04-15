// -*- c++ -*-
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

#ifndef __OS_THREAD_PTHREADS_H__
#define __OS_THREAD_PTHREADS_H__

void _init ();

//
// Mutex
//

class CondVar;

//!
// \ingroup micomt
//
// The Mutex class is a simple synchronization object used to protect
// critical sections.
//

class Mutex
#ifdef DEBUG_NAMES
    : public NamedObject
#endif // DEBUG_NAMES
{
public:
    friend class CondVar;
    /*!
     * \enum Attribute
     *
     * The Attribute enum defines a specific attribute for the Mutex
     * class. An attribute may be either Normal or Recursive. A normal
     * mutex may only be locked once by a thread. Attempts to lock it
     * more than once will result in a deadlock. A recursive mutex may
     * be locked multiple times by the same thread.
     */
    enum Attribute
    {
	Normal,
	Recursive
    };

    /*!
     * \enum ErrorType
     *
     * Error types associated with a mutex.
     */
    enum ErrorType
    {
	NoError,		//!< No error on the mutex
	AlreadyLocked,		//!< The mutex is already locked.
	TryAgain,		//!< Try locking again.
	Invalid,		//!< Invalid mutex.
	Fault,			//!< A fault occurred.
	DeadLock,		//!< The operation would result in deadlock
	UnknownError		//!< An unknown error occurred
    };

    //! \name Constructor/Destructor
    //@{
    Mutex(MICO_Boolean locked = FALSE, Attribute attr = Normal);
    ~Mutex();
    //@}

    //! \name Mutex Operations
    //@{
    /*!
     * The trylock method is used to sample the mutex to see if it
     * is locked. If the mutex is not locked the thread will obtain
     * the lock, returning true. Otherwise, an error condition will
     * be returned, and the thread will not own the lock.
     *
     * \return  True if the thread obtains the lock.
     */
    MICO_ULong
    trylock()
    {
	ErrorType ret = NoError;
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME (<< name ()) << ": Mutex::trylock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	switch (pthread_mutex_trylock(&_mutex)) {
	case 0:
	    ret = MICOMT::Mutex::NoError;
	    break;
	case EBUSY:
	    ret = MICOMT::Mutex::AlreadyLocked;
	    break;
	case EAGAIN:
	    ret = MICOMT::Mutex::TryAgain;
	    break;
	case EINVAL:
	    ret = MICOMT::Mutex::Invalid;
	    break;
	case EFAULT:
	    ret = MICOMT::Mutex::Fault;
	    break;
	case EDEADLK:
	    ret = MICOMT::Mutex::DeadLock;
	    break;
	default:
	    ret = MICOMT::Mutex::UnknownError;
	}
	return ret;
    }

    /*!
     * The lock method is used to lock the mutex. A thread locking
     * the mutex will have ownership of the lock until it is unlocked.
     * Any thread trying to subsequently lock the mutex will block
     * until the owning thread has unlocked it.
     */
    void
    lock()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Mutex::lock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
#ifdef SOLARIS_MUTEX
	if (_rec) {
	    if (_id == pthread_self()) {      // we already have the look
		_cnt++;
		return;
	    }
	    pthread_mutex_lock(&_mutex);
	    assert(_cnt == 0);               // paranoia
	    _id = pthread_self();
	    _cnt = 1;
	}
	else {
	    pthread_mutex_lock(&_mutex);
	}
#else // SOLARIS_MUTEX
	pthread_mutex_lock(&_mutex);
#endif // SOLARIS_MUTEX
    }
    
    /*!
     * The unlock method is used to release the ownership of a mutex.
     * Threads waiting on the locked mutex are re-woken and may attempt
     * to lock the mutex themselves.
     */
    void
    unlock()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Mutex::unlock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
#ifdef SOLARIS_MUTEX
	if (_rec) {
	    _cnt--;
	    if (!_cnt) {
		_id = 0;                    // hopefully 0 is not a valid thread id  
		pthread_mutex_unlock(&_mutex);
	    }
	}
	else {
	    pthread_mutex_unlock(&_mutex);
	}
#else // SOLARIS_MUTEX
	pthread_mutex_unlock(&_mutex);
#endif // SOLARIS_MUTEX
    }
    //@}

protected:
    pthread_mutex_t _mutex;	//!< Pthread mutex object

#ifdef SOLARIS_MUTEX
    pthread_t _id;	//!< Id of thread which hold ownership
    unsigned int _cnt;	//!< Counter of recursive mutex
    unsigned int _rec;	//!< Is mutex recursive?
#endif // SOLARIS_MUTEX
};


//
// Conditional Variable
//

/*!
 * \ingroup micomt
 *
 * A condition variable is a synchronization object that associates
 * a condition with a mutex.
 */
class CondVar
#ifdef DEBUG_NAMES
    : public NamedObject
#endif // DEBUG_NAMES
{
public:
    //! \name Constructor/Destructor
    //@{
    CondVar(MICOMT::Mutex* mut);
    ~CondVar();
    //@}

    //! \name Condition Variable Operations
    //@{
    /*!
     * The wait method blocks the calling thread until it is woken
     * up by another thread signalling or broadcasting to the condition
     * variable.
     *
     * \return  True on success, false on failure
     */
    MICO_Boolean
    wait()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": CondVar::wait ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	return (pthread_cond_wait(&_cond,&_mutex->_mutex) == 0);
    };
    
    /*!
     * The timedwait method blocks the calling thread until it
     * is woken up by another thread signalling or broadcasting to
     * the condition variable. It can also be woken up after the
     * elapsed time specified by tmout.
     *
     * \param tmout  Milliseconds to wait for a signal.
     * \return  True on success, false on failure.
     */
    MICO_Boolean timedwait(MICO_ULong tmout)
    { 
	timespec timeout;
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": CondVar::timedwait ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	timeout.tv_sec  = 0;
	timeout.tv_nsec = tmout * 1000;
	return (pthread_cond_timedwait(&_cond, &_mutex->_mutex, &timeout)  == 0);
    };
    
    /*!
     * The broadcast method wakes up all threads waiting on the
     * condition variable.
     */
    void broadcast()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": CondVar::broadcast ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	pthread_cond_broadcast(&_cond);
    };
    
    /*!
     * The signal method wakes up exactly one thread waiting
     * on the condition variable.
     */
    void signal()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": CondVar::signal ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	pthread_cond_signal(&_cond);
    };
    //@}

protected:
    pthread_cond_t  _cond;      //!< The pthread condition variable
    MICOMT::Mutex* _mutex;	//!< The mutex which will condition use
};


//
// reader/writer lock
//

/*!
 * \ingroup micomt
 *
 * The RWLock class implements a mutex-like object that can be locked
 * for either reading or writing.
 */
class RWLock
#ifdef DEBUG_NAMES
    : public NamedObject
#endif // DEBUG_NAMES
{
public:
    //! \name Constructor/Destructor
    //@{
    RWLock();
    ~RWLock();
    //@}

    /*!
     * The rdlock method locks the reading side of the lock.
     */
    void rdlock()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": RWLock::rdlock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	pthread_rwlock_rdlock(&_rwlock);
    }
    
    /*!
     * The wrlock method locks the writing side of the lock.
     */
    void wrlock()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": RWLock::wrlock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	pthread_rwlock_wrlock(&_rwlock);
    }
    
    /*!
     * The unlock method releases the lock.
     */
    void unlock()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": RWLock::unlock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	pthread_rwlock_unlock(&_rwlock);
    }

private:
    pthread_rwlock_t	_rwlock;	//!< The pthread read/write lock
};


//
// Semaphore
//

/*!
 * \ingroup micomt
 *
 * The semaphore class is a synchronization object. A semaphore is a
 * counter for resources shared between threads.
 */

#if defined(HAVE_SEMAPHORE_H) && !(defined(__APPLE__) && defined(__MACH__))
// the OS provides POSIX semaphore implementation
// in semaphore.h header file

// kcg: in the case of MacOS X 10.2 (Darwin 6.8) we have an issue with
// POSIX semaphore initialization, so we use MICO's own semaphore
// implementation on this platform

class Semaphore
#ifdef DEBUG_NAMES
    : public NamedObject
#endif // DEBUG_NAMES
{
private:
    sem_t _s;			//!< The system semaphore

public:
    /*!
     * \enum ErrorType
     *
     * Error types associated with a semaphore.
     */
    enum ErrorType
    {
	NoError,		//!< No error on semaphore
	NoPermission,		//!< Permission denied
	TryAgain,		//!< Try again
	SemInvalid,		//!< Invalide semaphore
	Interrupted,		//!< Interrupted by signal
	UnknownError		//!< Unknow error
    };

    //! \name Constructor/Destructor
    //@{
    Semaphore(unsigned int val = 0);
    ~Semaphore();
    //@}

    //! \name Semaphore Operations
    //@{
    /*!
     * The wait method waits until the semaphore count is non-zero,
     * then decrement the count.
     */
    void
    wait()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Semaphore::wait ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	while (sem_wait(&_s) == EINTR) {
	}
    }

    /*!
     * If the semaphore count is zero, this method will not block.
     * Instead it returns ErrorType::TryAgain.
     */
    Semaphore::ErrorType
    trylock()
    {
	Semaphore::ErrorType ret = MICOMT::Semaphore::NoError;
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME (<< name ()) << ": Semaphore::trylock ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	if (sem_trywait(&_s) < 0) {
	    switch (errno) {
	    case EPERM:
		ret = MICOMT::Semaphore::NoPermission;
		break;
	    case EAGAIN:
		ret = MICOMT::Semaphore::TryAgain;
		break;
	    case EINVAL:
		ret = MICOMT::Semaphore::SemInvalid;
		break;
	    case EINTR:
		ret = MICOMT::Semaphore::Interrupted;
		break;
	    default:
		ret = MICOMT::Semaphore::UnknownError;
	    }
	}
	return ret;
    }

    /*!
     * The post method increases the count of the semaphore.
     */
    void
    post()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Semaphore::post ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	sem_post(&_s);
    }
    //@}
};

#else // HAVE_SEMAPHORE_H && !(__APPLE__ && __MACH__)

// the OS doesn't provide POSIX semaphore implementation
// in semaphore.h header file so we will use own
// implementation based on mutex and condition variable
//
// This code is w/o doxygen doc because Doxyfile defines
// HAVE_SEMAPHORE_H and so this code is not used for generating
// documentation
//
class Semaphore
#ifdef DEBUG_NAMES
    : public NamedObject
#endif
{
    Mutex _mutex;
    CondVar _condition;
    unsigned int _counter;
public:
    enum ErrorType
    {
	NoError,
	NoPermission,
	TryAgain,
	SemInvalid,
	Interrupted,
	UnknownError
    };

    Semaphore(unsigned int value = 0)
    : _condition(&_mutex), _counter(value)
    {}
    ~Semaphore()
    {}

    void
    wait();

    Semaphore::ErrorType
    trylock();

    void
    post();
};

#endif // HAVE_SEMAPHORE_H && !(__APPLE__ && __MACH__)

//
// Thread
//

/*!
 * \ingroup micomt
 *
 * The thread class is a thread abstraction for system thread types.
 * Each thread object encapsulates a single execution context.
 */
class Thread
#ifdef DEBUG_NAMES
    : public NamedObject
#endif
{
public:
    /*!
     * \enum DetachFlag
     *
     * A thread may either be created Detached or NotDetached (attached)
     * to its parent thread. The DetachFlag is used to determine the
     * threads behavior when it exits.
     */
    enum DetachFlag
    {
	Detached,		//!< Detached threads are capable of
				//!< reclaiming their resources on exit. 
	NotDetached		//!< Attached threads must be re-joined with
				//!< the parent thread, allowing on-exit
				//!< synchronization.
    };

    /*!
     * \enum ErrorType
     *
     * Error types associated with the Thread class.
     */
    enum ErrorType
    {
	NoError,		//!< No error on the thread
	ThreadFailure,		//!< The thread has failed
	InvalidPriority,	//!< The thread was given an invalid priority
	MutexAlreadyLocked,	//!< Mutex is already locked
	NotStarted,		//!< The thread has not started
	UnknownError		//!< Unknown error.
    };

    //! \name Constructor/Destructor
    //@{
    Thread(DetachFlag detached = NotDetached); 
    virtual ~Thread ();
    //@}

    /*!
     * \type ThreadId
     *
     * An opaque type for thread ids.
     */
    typedef pthread_t ThreadID;

    /*!
     * \type ThreadKey
     *
     * An opaque type for thread key types. A thread key
     * is used to create and access thread specific data.
     */
    typedef pthread_key_t ThreadKey;

    /*
     * \type ThreadNo
     *
     * An opaque type for internal thread numbering in
     * MICO.
     */
    typedef MICO_ULong ThreadNo;

    //! \name Priority Methods
    //@{
    /*!
     * This function is used to get the maximum priority for
     * the operating system.
     *
     * \return  The maximum thread priority
     */
    static int
    get_priority_max()
    { 
	int policy;
	pthread_t id = pthread_self();
	struct sched_param params;
	pthread_getschedparam(id, &policy, &params);
	return sched_get_priority_max(policy);
    };

    /*!
     * This function is used to get the minimum priority for
     * the operating system.
     *
     * \return  The minimum thread priority
     */
    static int
    get_priority_min()
    {
	int policy;
	pthread_t id = pthread_self();
	struct sched_param params;
	pthread_getschedparam(id, &policy, &params);
	return sched_get_priority_min(policy);
    };

    /*!
     * The priority method is used to set new priority of the thread
     */
    virtual void
    priority(MICO_Long new_priority);
    //@}

    //! \name Thread Specific Data Methods
    //@{
    /*!
     * The create_key method creates a thread specific data key. This key can
     * be associated with an arbitrary data value using set_specific()
     * method.
     *
     * NOTE: It might be worthwhile to re-consider the designe of TSD
     * to just be a single class that maintains the association - sort
     * of a safe pointer class. It would at least be a fun programming
     * excercise.
     *
     * \param key  The key to create
     * \param cleanup  A cleanup method for the key
     */
    static void
    create_key(ThreadKey& key, void (*__cleanup) (void *) = 0)
    {
        int result = pthread_key_create(&key, __cleanup);
	assert(result == 0);
    };

    /*!
     * The delete_key method is used to delete a key, but not the data
     * associated with it. That is done on thread exit or cancellation.
     *
     * \param key  The key to delete
     */
    static void
    delete_key(ThreadKey& key)
    {
        int result = pthread_key_delete(key);
	assert(result == 0);
    };

    /*!
     * The get_specific method is used to retrieve thread specific data
     * for a key.
     *
     * \param key  The key for which to get data
     * \return  A pointer to the thread specific data
     */
    static void*
    get_specific(const ThreadKey& key)
    {
	return pthread_getspecific(key);
    };

    /*!
     * The set_specific method is used to associate thread specific
     * data with a key.
     *
     * \param key  The key to associate with data
     * \param value  Thread specific data
     */
    static void
    set_specific(const ThreadKey& key, void *value)
    {
        int result = pthread_setspecific(key, value);
	assert(result == 0);
    };
    //@}

    //! \name Thread Identification Methods
    //@{
    /*!
     * This method returns the threads system (pthread) id.
     *
     * \return The thread's system id
     */
    ThreadID
    id() const
    {
	return _id;
    };

    /*!
     * This method returns the threads system (pthread) id. This
     * method may be called without a thread object since it is
     * static.
     *
     * \return The thread's system id.
     */
    static ThreadID
    self()
    {
	return pthread_self();
    };

    /*!
     * This method returns the thread number.
     *
     * \return The thread number (MICO specific)
     */
    ThreadNo
    no() const
    {
	return _no;
    };

    /*!
     * Sets the thread number to n (MICO specific).
     *
     * \param n	 The new thread number
     */
    void
    no(const ThreadNo n)
    {
	_no = n;
    };
    //@}

    //! \name Operators
    //@{
    virtual MICO_Boolean
    operator==(const Thread &thread) const;
    //@}
    
    //! \name Thread Execution Methods
    //!@{
    /*!
     * This method calls the run method.
     *
     * \param arg  Thread parameters
     */
    // create a new thread with _run a its start method
    // maybe we should throw a exception instead of returning an error code
    void
    _thr_startup(void *arg);

    /*!
     * This method is used to actually create the thread context and
     * start the thread initialization.
     *
     * \return A thread error condition
     */
    ErrorType
    create_thread()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "Thread::create_thread ()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
#ifdef _THR_CREATE_AND_BLOCK
	_ready.lock();
#endif // _THR_CREATE_AND_BLOCK

        if (pthread_create(&_id, NULL, ThreadWrapper, this) != 0) {
#ifdef MTDEBUG
            if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    << "Thread: pthread_create () returned: "
		    << strerror (r) << endl;
		__mtdebug_unlock();
	    }
#endif // MTDEBUG
            return ThreadFailure;
        }
	if (_detached == Detached)
	    pthread_detach (_id);
	return NoError;
    };

    /*!
     * Yield to another process or thread. This is considerate thing
     * to do for threads stuck in long loops.
     */
    void
    yield()
    {
#ifdef HAVE_SCHED_H
	sched_yield();
#endif // HAVE_SCHED_H
    };

    /*!
     * The start method will start thread a-la Java
     *
     * \return possible error while starting thread
     */
    virtual ErrorType
    start(void* arg = NULL);
 
    /*!
     * The _run method is method which every thread executes. It's the
     * right place to do the work. The user can easily use Thread
     * class similar in Java i.e. inherit from Thread to custom class,
     * overload _run method and call start on class instance.
     *
     * Example:
     * \code
     * class MyThread : public virtual MICOMT::Thread
     * {
     * public:
     *   void _run(void* arg = NULL)
     *   {
     *     // do the work here
     *   }
     * };
     * ....
     * MyThread* thr = new MyThread;
     * thr->start();
     * thr->wait();
     * \endcode
     */
    virtual void
    _run(void* arg) = 0;

    static void*
    ThreadWrapper(void* arg);

    /*!
     * This method effectively cancels the thread from outside the
     * thread's context, effectively preventing cleanup.
     */
    void
    terminate()
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Thread::terminate (void *exitval)" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
        int result = pthread_cancel(this->id());
	assert(!result);
    };

    /*!
     * This method blocks the current thread until the thread being called
     * upon exits.
     *
     * \param exitval  A place to store the exit value of the finished
     * thread
     */
    void
    wait(void** exitval = NULL)
    {
#ifdef MTDEBUG
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Thread::wait()" << endl;
	    __mtdebug_unlock();
	}
#endif // MTDEBUG
	pthread_join(this->id(), exitval);
    };    
    //@}

protected:
    void* _arg;			//!< Parameters passed in on start
    ThreadID _id;		//!< The system thread id
    ThreadNo _no;		//!< MICO thread number
    DetachFlag _detached;	//!< Thread attached or detached?
#ifdef _THR_CREATE_AND_BLOCK
    Mutex _ready;		//!< Blocks thread on create (if specified)
    ErrorType _start_error;     //!< Used to carry starting error from ctor to start method
#endif // _THR_CREATE_AND_BLOCK
};

#endif // __OS_THREAD_PTHREADS_H__


