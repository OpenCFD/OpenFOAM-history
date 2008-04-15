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
 *  support for GNU Pth - The GNU Portable Threads (http://www.gnu.org/software/pth)
 *
 */

#ifndef __os_thread_pthreads_h__
#define __os_thread_pthreads_h__

//static void _init (MICO_ULong _tpsize, MICO_ULong _tpincr);
extern void _init ();

/*********************************** Mutex *****************************************/

class Mutex __NAME( :public NamedObject ) {
 protected:
    pth_mutex_t _mutex;
    
 public:

    friend class CondVar;

    typedef enum { Normal, Recursive } Attribute;

    Mutex( MICO_Boolean locked = FALSE, Attribute attr = Normal )
	__NAME( :NamedObject () )
	{
            __NAME(name ("<UnNamed Mutex>"));
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread) 
		    << "Mutex::Mutex (locked = " << (int)locked << ", " 
		    << " attr = " << attr << ")" << endl;
		__mtdebug_unlock();
	    }
	    
	    assert( pth_mutex_init(&_mutex) );
	    if (locked) lock();
	};
    ~Mutex()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Mutex::~Mutex ()" << endl;
		__mtdebug_unlock();
	    }
	};
    
    MICO_ULong trylock();
    
    void lock()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Mutex::lock ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_mutex_acquire( &_mutex, FALSE, NULL );
	};

    void unlock()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": Mutex::unlock ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_mutex_release( &_mutex );
	};
};

/***************************** Conditional Variable  *******************************/

class CondVar __NAME( :public NamedObject ) {
private:
    pth_cond_t  _cond;
public:
    CondVar()
	{
	    __NAME( name ("<UnNamed CondVar>") );
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": ContVar::CondVar ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_cond_init( &_cond );
	};
    
    ~CondVar()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": ContVar::~CondVar ()" << endl;
		__mtdebug_unlock();
	    }
	};

    
    MICO_Boolean wait( Mutex &_mutex )
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::wait ()" << endl;
		__mtdebug_unlock();
	    }
	    return ( pth_cond_await( &_cond, &_mutex._mutex, NULL ) == 0 );
	};

    MICO_Boolean timedwait( Mutex &_mutex, MICO_ULong tmout );

    void broadcast()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::broadcast ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_cond_notify( &_cond, TRUE );
	};

    void signal()
	{ 
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": CondVar::signal ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_cond_notify( &_cond, FALSE );
	};
};

/******************************* reader/writer lock *******************************/

class RWLock __NAME( :public NamedObject ) {
 private:
    pth_rwlock_t _rwlock;
    
 public:
    RWLock() __NAME( : NamedObject () )
	{
            __NAME(name ("<UnNamed RWLock>"));
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    << "RWLock::RWLock ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_rwlock_init( &_rwlock );
	};
    ~RWLock()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": RWLock::~RWLock ()" << endl;
		__mtdebug_unlock();
	    }
	};

    void rdlock()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": RWLock::rdlock ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_rwlock_acquire( &_rwlock, PTH_RWLOCK_RD, FALSE, NULL);
	};

    void wrlock()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": RWLock::wrlock ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_rwlock_acquire( &_rwlock, PTH_RWLOCK_RW, FALSE, NULL );
	};

    void unlock()
	{
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		__mtdebug_lock();
		MICO::Logger::Stream (MICO::Logger::Thread)
		    __NAME(<< name ()) << ": RWLock::unlock ()" << endl;
		__mtdebug_unlock();
	    }
	    pth_rwlock_release( &_rwlock );
	};
};

/********************************** Semaphore **************************************/

class Semaphore __NAME( :public NamedObject ) {
 private:
    CondVar     _cond;
    Mutex       _mutex;            
    MICO_ULong  count;            // current value

 public:
    Semaphore( unsigned int val = 0)__NAME( :NamedObject () )
{
        __NAME(name ("<UnNamed Semaphore>"));
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "Semaphore::Semaphore (val = " << (int)val << ")" << endl;
	    __mtdebug_unlock();
	}
	count = val;
    };

    ~Semaphore() {
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    __mtdebug_lock();
	    MICO::Logger::Stream (MICO::Logger::Thread)
		__NAME(<< name ()) << ": Semaphore::~Semaphore ()" << endl;
	    __mtdebug_unlock();
	}
    };

    void wait();
    void post();
};

/******************************* basic Thread class *********************************/

class Thread __NAME( :public NamedObject ) {
 public:
    // for POSIX-Threads, a ThreadID is simply a pthread_t:
    typedef pth_t      ThreadID;
    typedef pth_key_t  ThreadKey;
    typedef MICO_ULong ThreadNo;

    typedef enum { Detached, NotDetached } DetachFlag;
    typedef enum { NoError, InvalidPriority, MutexAlreadyLocked,
		   NotStarted, UnknownError } ErrorType;

    // start method
    static void *ThreadWrapper (void *arg);
    
    static int get_priority_max() { 
      //return 0;
      pth_attr_t __t_attr = pth_attr_new();
      pth_attr_set(__t_attr, PTH_ATTR_PRIO, PTH_PRIO_MAX);
      int* __prio;
      pth_attr_get(__t_attr, PTH_ATTR_PRIO, __prio);
      pth_attr_destroy(__t_attr);
      return (*__prio);
    };

    static int get_priority_min() {
      //return 0;
      pth_attr_t __t_attr = pth_attr_new();
      pth_attr_set(__t_attr, PTH_ATTR_PRIO, PTH_PRIO_MIN);
      int* __prio;
      pth_attr_get(__t_attr, PTH_ATTR_PRIO, __prio);
      pth_attr_destroy(__t_attr);
      return (*__prio);
    };

     // key handling
    static void createKey( ThreadKey &key, void (*__cleanup) (void *) = NULL ) {
	assert( pth_key_create( &key, __cleanup ) == 1 );
    };
    static void deleteKey( ThreadKey key ) {
	assert( pth_key_delete( key ) == 1 );
    };
    static void *getSpecific( ThreadKey key ) {
	return pth_key_getdata( key );
    };
    static void setSpecific( ThreadKey key, void *value ) {
	assert( pth_key_setdata( key, value ) == 1 );
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
    static ThreadID self ()     { return pth_self(); };

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
	
	if (!(_id = pth_spawn( PTH_ATTR_DEFAULT, &ThreadWrapper, this)))
	    return UnknownError;
	if (_detached == Detached)
	    pth_attr_set(PTH_ATTR_DEFAULT, PTH_ATTR_JOINABLE, FALSE);
	return NoError;
    };
    
     // yield execution
    void yield() {
#ifdef HAVE_SCHED_H
	pth_yield();
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
	pth_cancel (_id);
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
	pth_join (id(), NULL);
    };
    
    virtual ~Thread ();
};

#endif // __os_thread_pthreads_h__

