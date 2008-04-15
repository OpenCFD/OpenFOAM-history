// -*- c++ -*-
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

#ifndef __os_thread_dcethreads_h__
#define __os_thread_dcethreads_h__

static void _init (MICO_ULong _tpsize, MICO_ULong _tpincr);

/*********************************** Mutex *****************************************/

class Mutex __NAME( :public NamedObject ) {
 protected:
    pthread_mutex_t _mutex;
    
 public:

    friend class CondVar;

    typedef enum { Normal, Recursive } Attribute;
    Mutex( MICO_Boolean locked = FALSE, Attribute attr = Normal );
    ~Mutex();
    
    MICO_ULong trylock();
    
    void lock()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Mutex::lock ()" << endl;
		__mtdebug_unlock();
	    }
	    pthread_mutex_lock( &_mutex );
	};
    
    void unlock()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Mutex::unlock ()" << endl;
		__mtdebug_unlock();
	    }
	    pthread_mutex_unlock( &_mutex );
	};
};

/***************************** Conditional Variable  *******************************/

class CondVar __NAME( :public NamedObject ) {
private:
    pthread_cond_t  _cond;
public:
    CondVar();
    ~CondVar();
    
    MICO_Boolean wait( Mutex &_mutex )
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::wait ()" << endl;
		__mtdebug_unlock();
	    }
	    return ( pthread_cond_wait( &_cond, &_mutex._mutex ) == 0 );
	};
    
    MICO_Boolean timedwait( Mutex &_mutex,  MICO_ULong tmout )
	{ 
	    timespec timeout;
	    
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::timedwait ()" << endl;
		__mtdebug_unlock();
	    }
	    timeout.tv_sec  = 0;
	    timeout.tv_nsec = tmout * 1000;
	    return ( pthread_cond_timedwait( &_cond, &_mutex._mutex, &timeout )  == 0 );
	};
    
    void broadcast()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::broadcast ()" << endl;
		__mtdebug_unlock();
	    }
	    pthread_cond_broadcast( &_cond );
	};
    
    void signal()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::signal ()" << endl;
		__mtdebug_unlock();
	    }
	    pthread_cond_signal( &_cond );
	};
};

/******************************* reader/writer lock *******************************/

class RWLock __NAME( :public NamedObject ) {
 private:
    pthread_mutex_t     lock;     // mutex for this rwlock
    pthread_cond_t      readers;  // waiting readers
    pthread_cond_t      writers;  // waiting writers
    MICO_ULong          state;    // -1 = writer access, 0 = free, > 0 readers
    MICO_ULong          waiters;  // number of waiting writers
    
 public:
    RWLock();
    ~RWLock();
    
    void rdlock();
    void wrlock();
    MICO_Boolean trywrlock();
    void unlock();
};

/********************************** Semaphore **************************************/

class Semaphore __NAME( :public NamedObject ) {
 private:
    sem_t  s;
    
 public:
    Semaphore( unsigned int val = 0);
    ~Semaphore();
    
    void wait()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Semaphore::wait ()" << endl;
		__mtdebug_unlock();
	    }
	    while (sem_wait (&s))
            {
                if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		    __mtdebug_lock();
		    MICO::Logger::Stream (MICO::Logger::Thread)
			__NAME(<< name ()) << ": sem_wait () ret'd "
			<< strerror (errno) << endl;
		    __mtdebug_unlock();
		}
            }
	};

    void post()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Semaphore::post ()" << endl;
		__mtdebug_unlock();
	    }
	    sem_post( &s );
	};
};

/******************************* basic Thread class *********************************/

class Thread __NAME( :public NamedObject ) {
 public:
    // for DCE threads, a ThreadID is simply a pthread_t:
    typedef pthread_t     ThreadID;
    typedef pthread_key_t ThreadKey;
    typedef MICO_ULong    ThreadNo;

    typedef enum { Detached, NotDetached } DetachFlag;
    typedef enum { NoError, InvalidPriority, MutexAlreadyLocked,
		   NotStarted, UnknownError } ErrorType;

    // start method
    static void *ThreadWrapper (void *arg);
    
    static int get_priority_max() { 
	return sched_get_priority_max( sched_getscheduler(0) ); 
    };
    static int get_priority_min() {
	return sched_get_priority_min( sched_getscheduler(0) );
    };

    // key handling
    static void createKey( ThreadKey &key ) {
	assert( pthread_keycreate( &key, NULL ) == 0 );
    };
    // DCE Threads removes keys when the thread(s) terminate so this method
    // is empty for the DCE threads case.
    static void deleteKey( ThreadKey key ) {
    };
    static void *getSpecific( ThreadKey key ) {
        void *data;
	pthread_getspecific( key, &data );
        return data;
    };
    static void setSpecific( ThreadKey key, void *value ) {
	assert( pthread_setspecific( key, value ) == 0 );
    };

 protected:
    void *_arg;                   // parameters passed in via start()
    
    ThreadID _id;       // records the id from pthread_create()
    ThreadNo _no;
    DetachFlag _detached;
    
#ifdef _THR_CREATE_AND_BLOCK
    Mutex _ready;
#endif
    
 public:
    Thread (DetachFlag detached = NotDetached);
    
    void _thr_startup(void *arg) {
#ifdef _THR_CREATE_AND_BLOCK
	_ready.lock();
#endif
	_run( arg );
    };

    virtual void _run (void *arg) = 0;  // the _run() method sets up
    // anything else this object needs
    // including other threads.           
    
    // Start the thread ala JAVA:
    virtual ErrorType start (void *arg = NULL);
    
    // get this thread's ID:
    ThreadID id () const        { return _id; };
    static ThreadID self ()     { return pthread_self(); };

    ThreadNo no () const        { return _no; };
    void no( const ThreadNo n ) { _no = n; };
    
    // see if thread `thread' is the same as this thread:
    virtual MICO_Boolean operator == (const Thread &thread) const;
    
    // create a new thread with _run a its start methode
    // maybe we should throw a exception instead of returning an error code
    ErrorType create_thread () {
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "Thread::create_thread ()" << endl;
	    __mtdebug_unlock();
	}
#ifdef _THR_CREATE_AND_BLOCK
	_ready.lock();
#endif
	pthread_create (&_id, pthread_attr_default, ThreadWrapper, this);
	
	if (_detached == Detached)
	    pthread_detach (&_id);
	
	return NoError;
    };

    // yield execution
    void yield() {
#ifdef HAVE_SCHED_H
	sched_yield();
#endif
    };

    // tell this thread to die:
    void terminate (void *exitval) {
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Thread::terminate (void *exitval)" << endl;
	    __mtdebug_unlock();
	}
	pthread_cancel (_id);
    };

    // set the priority of this thread to `new_priority'.
    // let's avoid using this if at all possible, some pthreads impls don't
    // support it fully or at all:
    virtual void priority (MICO_Long new_priority);
    
    // wait for this thread to finish:
    void wait () {
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Thread::wait()" << endl;
	    __mtdebug_unlock();
	}
	pthread_join (id(), NULL);
    };
    
    virtual ~Thread ();
};

#endif // __os_thread_dcethreads_h__
