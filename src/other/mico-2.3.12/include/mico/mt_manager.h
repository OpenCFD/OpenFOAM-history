/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2005 by The Mico Team
 * 
 *  thread management
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

#ifndef __mico_mt_manager_h__
#define __mico_mt_manager_h__

namespace MICO
{

    /*!
     * \ingroup micomt
     * The tm_init_t structure maintains information about
     * a thread manager initialization.
     */
    struct tm_init_t
    {
	MICO::Operation::OP_id_type_decl  OP_type;	//!< The operation type
	unsigned int max;				//!< Max threads
	unsigned int max_idle;				//!< Max idle threads
	unsigned int min_idle;				//!< Min idle threads
	MICO::MsgChannel::Types mct;			//!< Type of message channel
	unsigned int mc_size;				//!< Size of message channel
	MICO::Operation *op;				//!< The Operation
	CORBA::Boolean start;				//!< Start automatically
    };

    class ThreadPoolManager;
    class ThreadPool;

    /*
     * \ingroup micomt
     * The WorkerThread is a thread that does work.
     */
    class WorkerThread: public MICOMT::Thread
    {
    public:
	/*
	 * States of the worker thread.
	 */
	enum State {
	    Transition,		//!< Thread is in transition
	    Idle,		//!< Thread is idle
	    Busy,		//!< Thread is busy
	    Stop		//!< Thread is stopped
	};

	//! \name Constructors/Destructor
	//@{
	WorkerThread();
	WorkerThread( ThreadPool * );
	virtual ~WorkerThread();
	//@}

	//! \name Thread Pool Management
	//@{
	void set_thread_pool( ThreadPool * );
	ThreadPool *get_thread_pool();
	//@}

	//! \name Operation Management
	//@{
	void register_operation( Operation * );
	void deregister_operation( Operation * );
	Operation *get_operation() const;
	//@}

	// \name State Access
	//@{
	void mark_idle();
	void mark_busy();

	/*!
	 * \param s		The new state
	 * Set the state of the worker thread
	 */
	void state (State s)
	{
	    _state = s;
	};

	/*!
	 * Post to the state semaphore, indicating
	 * a change.
	 */
	void post_state_change()
	{
	    _state_sem.post();
	};

	/*
	 * \return The current state.
	 * Return the current state of the worker thread.
	 */
	State state()
	{
	    return _state;
	};
	//@}
	
	//! \name Miscellaneous
	//@{
	/*!
	 * \param msg		The message to give to the thread
	 * Give the thread a message to work with.
	 */
	void put_msg(msg_type *msg);

	/*!
	 * \param args		The arguments to the thread.
	 * This is called by the Thread framework and defines the
	 * runtime behavior of the class.
	 */
	virtual void _run (void *args);
	//@}

    protected:
	ThreadPool	       *tp;		//!< The thread's pool.
	Operation	       *op;		//!< The thread's operation
	MICOMT::Mutex		op_critical;	//!< Critical section mutex
	State			_state;		//!< Thread's state
	MICOMT::Semaphore	_state_sem;	//!< State semaphore
	msg_type		*_msg;		//!< Current message

    private:
	//! \name Queue Processing Methods
	//@{
	void _run_single_queue_input();
	void _run_multi_queue_input();
	void _run_process();
	//@}

	//! \name Miscellaneous
	//@{
	/*!
	 * Wait until the semaphore changes states.
	 */
	void wait_for_state_change() {
	    _state_sem.wait();
	};
	//@}
    };

    /*!
     * \ingroup micomt
     *
     * The thread_map is an array of threads.
     */
    typedef FastArray<class WorkerThread> thread_map;
    //typedef class WorkerThread WorkerThread;

    /*!
     * The ThreadPool class maintains a pool of threads
     * that can be committed to do some work.
     *
     * \todo
     * This class should probably also keep a list of busy
     * or non-idle threads. Ownership of threads should happen
     * in one and only one place. right now, it looks like
     * busy threads are passed around like candy, but i could
     * be wrong.
     * In short, it seems like there should be a tighter binding
     * between worker threads and the thread pools (more cohesion).
     */
    class ThreadPool: public MsgChannel {
    public:
	//! \name Constructor/Destructor
	//@{
	ThreadPool( unsigned int _max,
		    unsigned int _max_idle,
		    unsigned int _min_idle);
	virtual ~ThreadPool();
	//@}
    
	//! \name Thread Management
	//@{
	WorkerThread *get_idle_thread();
	WorkerThread *new_idle_thread();
	void mark_idle( WorkerThread * );
	void mark_busy( WorkerThread * );
	void remove_thread( WorkerThread *kt );
	virtual void start_threads (void *arg = NULL);
	//@}

	//! \name Thread Pool Manager Management
	//@{
	/*!
	 * \return A reference to the thread pool manager.
	 * Get a reference to the thread pool manager.
	 */
	ThreadPoolManager& get_thread_pool_manager() const
	{
	    return *tpm;
	};

	/*!
	 * \param _tmp		The new thread pool manager
	 * Set the new thread pool manager.
	 */
	void set_thread_pool_manager(ThreadPoolManager& _tpm)
	{
	    tpm = &_tpm;
	};
	//@}

	//! \name Operation Management
	//@{
	/*!
	 * \param _op		The new operation
	 * Set the operation type for the thread pool.
	 */
	void set_operation(Operation *_op)
	{
	    op = _op;
	};

	/*!
	 * \return A pointer to the thread pool's operation
	 * Get the thread pool's operation type.
	 */
	Operation *get_operation() const
	{
	    return op;
	};
	//@}

	//! \name Input Channel Access
	//@{
	/*!
	 * \param _mc		The message channel to register
	 * Register an input channel with a thread pool.
	 */
	void register_input_mc(MsgChannel* _mc)
	{
	    input_mc = _mc;
	    input_mc->set_thread_pool( this );
	};

	/*!
	 * \return A reference to the input message channel
	 * Get a reference to the input channel.
	 */
	MsgChannel* get_input_mc() const
	{
	    return input_mc;
	};
	//@}

	//! \name Miscellaneous
	//@{
	// methods from MsgChannel
	//    virtual msg_type * get_msg( OP_id_type nextOP_id);
	virtual void put_msg( OP_id_type nextOP_id, msg_type * msg);
	//@}

    protected:
	typedef FastArray< WorkerThread > ThreadArray;
	
	MICOMT::Mutex		tp_lock;	//!< Lock for the thread pool
	MICOMT::CondVar         tp_cond_;
	ThreadPoolManager      *tpm;		//!< The thread pool manager
	ThreadArray		idle_threads;	//!< Array of idle threads
	Operation	       *op;		//!< Operation prototype for the pool
	MsgChannel	       *input_mc;	//!< The input message channel
	unsigned int		max;		//!< Max threads
	unsigned int		max_idle;	//!< Max idle threads
	unsigned int		min_idle;	//!< Min idle threads
	unsigned int		cnt_all;	//!< Count of all threads
    };

    /*!
     * Mark the worker thread idle through its thread pool (if it exists).
     */
    inline void WorkerThread::mark_idle()
    {
	if (get_thread_pool())
	    get_thread_pool()->mark_idle( this );
    }

    /*!
     * Mark the worker thread busy through its thread pool (if it exists).
     */
    inline void WorkerThread::mark_busy()
    {
	if (get_thread_pool())
	    get_thread_pool()->mark_busy( this );
    }

    /*!
     * \param _tp	The new thread pool for the worker thread
     * Set the thread pool for the worker thread.
     */
    inline void WorkerThread::set_thread_pool( ThreadPool *_tp )
    {
	tp = _tp;
    }

    /*!
     * \return A pointer to the thread pool
     * Get the worker thread's current thread pool. May be NULL.
     */
    inline ThreadPool *WorkerThread::get_thread_pool()
    {
	return tp;
    }

    /*!
     * \param m		The new message to give to the worker thread.
     * Assign the worker thread a message to process.
     */
    inline void WorkerThread::put_msg( msg_type *m )
    {
	op->put_msg( op->info().get_op_id(), m );
    }

    /*!
     * \return The worker thread's operation context.
     * Get the worker thread's current operation context.
     */
    inline Operation *WorkerThread::get_operation() const {
	return op;
    }

   
    /*!
     * \ingroup micomt
     * The ThreadPoolManager class manages a set of thread pools.
     * Each thread pool is associated with an operation id type.
     * messages can be posted through the thread pool manager
     * directly to a thread pool to handle operations.
     */
    class ThreadPoolManager: public MsgChannel {
    private:
	std::map<OP_id_type, ThreadPool*> tp;
	MICO_ULong usercnt;

    public:
	//! \name Constructor/Destructor
	//@{
	/*!
	 * Constructor. User count is initialized to 64.
	 *
	 * \todo
	 * Define 64 or make it a const value somewhere.
	 */
	ThreadPoolManager() : tp()
	{
	    usercnt = 64;
	};

	/*!
	 * Destructor.
	 */
	virtual ~ThreadPoolManager();

	/*!
	 * \param _tp		The new thread pool to register
	 * \return		The thread pool id (count?)
	 * Register a new thread pool with the manager mapping it
	 * to a manager assigned op id.
	 */
	MICO_ULong register_new_tp( ThreadPool& _tp) {
	    tp[usercnt] = &_tp;
	    _tp.set_thread_pool_manager( *this );
	    return usercnt++;
	};

	/*!
	 * \param OP_id		The operation id to assign the thread pool to
	 * \param tp		The thread pool to register
	 * Register a thread pool to perform a specific operation.
	 */
	void register_tp( const OP_id_type OP_id, ThreadPool& _tp) {
	    tp[OP_id] = &_tp;
	    _tp.set_thread_pool_manager( *this );
	};

	/*!
	 * \param OP_id		The operation for which to get the thread pool
	 * Get the thread pool handling a particular operation type.
	 *
	 * \todo
	 * There could be a bug here if tp does not hold a thread pool for
	 * the requested OP_id_type. what happens when you dereference garbage?
	 */
	ThreadPool& get_thread_pool( const OP_id_type OP_id ) {
	    return *tp[OP_id];
	};

	/*!
	 * \param OP_id		The operation id for which to get an idle thread
	 * Get an idle thread from the thread pool working with the
	 * requested operation id.
	 */
	WorkerThread *get_idle_thread( const OP_id_type OP_id ) {

	    return get_thread_pool( OP_id ).get_idle_thread();
	}

	/*!
	 * \param OP_id		The operation type to get the operation for
	 * \return A pointer to an operation of the requested type.
	 * Get an operation object for the requested type.
	 */
	Operation *get_operation( OP_id_type OP_id ) {
	    return get_thread_pool( OP_id ).get_operation();
	}

	/*!
	 * \param nextOP_id	The operation id to post the message for.
	 * \param msg		The message to post for the operation.
	 */
	virtual void put_msg( OP_id_type nextOP_id, msg_type *msg ) {
	    get_thread_pool(nextOP_id).put_msg( nextOP_id, msg );
	};
    };


    class MTManager
    {
	static CORBA::Long _S_server_concurrency_model;
        static CORBA::Long _S_client_concurrency_model;
	static MICO::ThreadPoolManager* _S_thread_pool_manager;
    public:
	enum ConcurrencyModel
	{
	    _S_thread_pool,
	    _S_thread_per_connection,
	    _S_thread_per_request,
            _S_reactive_client,
            _S_blocking_threaded_client,
            _S_threaded_client
	};

	static void
	server_concurrency_model(MICO::MTManager::ConcurrencyModel __model);

        static void
        client_concurrency_model(MICO::MTManager::ConcurrencyModel model);
	
	static CORBA::Long
	server_concurrency_model();

        static CORBA::Long
        client_concurrency_model();

	static CORBA::Boolean
	thread_pool();

	static CORBA::Boolean
	thread_per_connection();

	static CORBA::Boolean
	thread_per_request();

        static CORBA::Boolean
        threaded_client();

        static CORBA::Boolean
        blocking_threaded_client();

        static CORBA::Boolean
        reactive_client();

	static void
	thread_setup(unsigned int __conn_limit, unsigned int __req_limit);

	static MICO::ThreadPoolManager*
	thread_pool_manager()
	{ return _S_thread_pool_manager; }

	static void
	free();
    };
}

#endif // __mico_mt_manager_h__
