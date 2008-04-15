/*
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
 * 
 */

#ifdef FAST_PCH
#include "orb_pch.h"
#endif // FAST_PCH
#ifdef __COMO__
#pragma hdrstop
#endif // __COMO__

#ifndef FAST_PCH
#define MICO_CONF_IMR
#include <CORBA-SMALL.h>
#include <mico/os-thread.h>
#include <mico/impl.h>

#endif // FAST_PCH


using namespace std;

/********************************** ThreadPool **********************************/

/*!
 * \param _max		Max number of threads
 * \param _max_idle	Max number of idle threads
 * \param _min_idle	Min number of idle threads
 * Initialize the thread pool.
 */
MICO::ThreadPool::ThreadPool( unsigned int _max,
			      unsigned int _max_idle,
			      unsigned int _min_idle): tp_lock(FALSE, MICOMT::Mutex::Recursive),
						       tp_cond_(&tp_lock),
						       tpm(NULL),
						       idle_threads(_max),
						       op(NULL),
						       input_mc(NULL),
						       max_idle( _max_idle ),
						       min_idle( _min_idle ),
						       cnt_all( 0 )
                                                       
{
    this->max = _max; // moved here because MSVC7 thinks is the max macro and gives an error
}

/*!
 * Clean up thread pool resources.
 *
 * \todo
 * The destructor really doesn't clean any resources,
 * it just stops the threads leaving them floating
 * around in memory. I don't know if this is right,
 * but it doesn't seem like it. Also, the list itself
 * is not freed. Another memory leak.
 */
MICO::ThreadPool::~ThreadPool() {
    
    MICOMT::AutoLock lock(tp_lock);
    unsigned int i, cnt;
    // we need to call dtor with _all_ thread idle
    while (cnt_all != idle_threads.count()) {
	// not all threads are idle => wait
	tp_cond_.wait();
    }

    i = idle_threads.first();
    for (cnt = idle_threads.count(); cnt > 0;
	 i = idle_threads.next(i), cnt--) {
	idle_threads[i]->terminate();
    }
    if (op != NULL)
	delete op;
    if (input_mc != NULL)
	delete input_mc;
}

// fire up all threads
/*!
 * \param arg		Parameter to the threads? Doesn't get used.
 * This method starts the minimum number of idle threads and
 * registers a copy of the thread pools operation with the
 * starting threads.
 */
void
MICO::ThreadPool::start_threads (void *arg) {
    unsigned int i;
    MICO::WorkerThread *kt;

    for (i = 0; i < min_idle; i++) {
	kt = new_idle_thread();
	if ( this->get_operation() )
	    kt->register_operation( this->get_operation()->copy() );
    }
}

// returns - a idle thread or creates a new thread if needed
//         - or NULL if limits are exhausted
//
// removes threads from the idle list
//
/*!
 * \return A pointer to an idle worker thread.
 * This method returns the next idle worker thread. This
 * may be NULL if no idle threads are available.
 * The idle thread is removed from the idle threads list.
 */
MICO::WorkerThread *
MICO::ThreadPool::get_idle_thread()
{
    MICOMT::AutoLock l(tp_lock);
    WorkerThread *kt;
    
    //no idle threads
    if (idle_threads.count() == 0) {

	// limit exhausted
	if (cnt_all >= max) {
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread) 
		<< "ThreadPool::get_idle_thread(): thread limit exhausted " << endl;
	    }
	    return 0;
	}
	
	kt = new MICO::WorkerThread( this );
        assert(kt);
	if (op)
	    kt->register_operation( op->copy() );
	cnt_all++;
	kt->start();
    } else {
	kt = idle_threads[idle_threads.first()];
	idle_threads.remove(idle_threads.first());
	tp_cond_.broadcast();
    }
    kt->state( MICO::WorkerThread::Transition );
    return kt;
}

/*!
 * \return A pointer to a new idle thread
 * This method allocates a new idle thread, storing it in
 * the idle threads list.
 */
MICO::WorkerThread *
MICO::ThreadPool::new_idle_thread()
{
    WorkerThread *kt;
    
    kt = new MICO::WorkerThread( this );
    {
	MICOMT::AutoLock l(tp_lock);
	
	kt->no( idle_threads.insert( kt ) );
	kt->state( MICO::WorkerThread::Idle );
	cnt_all++;
	tp_cond_.broadcast();
    }
    kt->start();
    return kt;
}
 
/*!
 * \param kt		The thread to mark idle
 * This method will mark a thread as idle if there are
 * no messages in the message queue.
 */
void
MICO::ThreadPool::mark_idle( MICO::WorkerThread *kt )
{
    if (input_mc && input_mc->check_msg(kt)) 
	return;

    if ( kt->state() != MICO::WorkerThread::Idle )
    {
	MICOMT::AutoLock l(tp_lock);
	
	kt->no( idle_threads.insert( kt ) );
	kt->state( MICO::WorkerThread::Idle );
	tp_cond_.broadcast();
    }
    if (input_mc)
	input_mc->check_msg( 0 );
}

/*!
 * \param kt		The thread to mark busy
 * This method will mark the thread busy.
 */
void 
MICO::ThreadPool::mark_busy( MICO::WorkerThread *kt )
{
    //    MICOMT::AutoLock l(tp_lock);

    assert ( kt->state() != MICO::WorkerThread::Idle);

    kt->state( MICO::WorkerThread::Busy );
}

/*!
 * \param kt		The thread to remove
 * This method removes a thread from the idle list.
 * Busy threads cannot be remvoed.
 */
void 
MICO::ThreadPool::remove_thread( MICO::WorkerThread *kt )
{
    MICOMT::AutoLock l(tp_lock);
	
    if ( kt->state() == MICO::WorkerThread::Idle ) {
	idle_threads.remove( kt->no() );
    }
    cnt_all--;
    tp_cond_.broadcast();
}

/*!
 * \param nextOP_id	The next operation id
 * \param msg		The message to put into the input queue.
 * Put a message into a queue to handled by a certain operation type.
 */
void
MICO::ThreadPool::put_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg )
{
    if (input_mc)
	input_mc->put_msg(nextOP_id, msg);
}

/****************************** WorkerThread *******************************/

/*!
 * Initialize the worker thread.
 */
MICO::WorkerThread::WorkerThread()
    : op(NULL), op_critical(FALSE, MICOMT::Mutex::Recursive)
{
#ifdef DEBUG_NAMES
    _state_sem.name("WorkerThread._state_sem");
#endif
}

/*!
 * \param _tp		The thread pool the thread is assigned to.
 * Initialize the worker thread into a the specified
 * thread pool.
 */
MICO::WorkerThread::WorkerThread(MICO::ThreadPool *_tp)
    : tp(_tp), op(NULL), op_critical(FALSE, MICOMT::Mutex::Recursive)
{
#ifdef DEBUG_NAMES
    op_critical.name("WorkerThread.op_critical");
    _state_sem.name("WorkerThread._state_sem");
#endif
}

/*!
 * The destructor removes the thread from the thread pool.
 */
MICO::WorkerThread::~WorkerThread()
{
    if (tp) {
	tp->remove_thread( this );
    }
}

/*!
 * \param _op		The operation to register with the thread
 * Register an operation with the worker thread.
 *
 * \todo
 * If the thread only has one operation at a time, registration
 * probably isn't the right word for the method. maybe set_operation()
 * would be better.
 */
void
MICO::WorkerThread::register_operation( MICO::Operation* _op )
{
    MICOMT::AutoLock l(op_critical);

    op = _op;
    op->thread( this );
}

/*!
 * \param _op		The operation to deregister
 * Unregister an operation from the thread.
 */
void
MICO::WorkerThread::deregister_operation( MICO::Operation* _op )
{
    MICOMT::AutoLock l(op_critical);

    if (op == _op)
    {
	op->thread( 0 );
	op = 0;
    }
}

/*!
 * \param arg		Thread parameters
 * The run method controls the thread. The thread
 * runs while it is not being stopped. If an operation
 * is ever available on the thread, the operation is processed
 * and the thread returns to its idle state.
 */
void
MICO::WorkerThread::_run (void *arg) {

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "WorkerThread::_run: " << endl;
    }
    do {
	wait_for_state_change();
	if ( state() == Busy ) {

	    op_critical.lock();
	    if ( this->get_operation() != 0 )
		this->get_operation()->_run();
	    op_critical.unlock();
	    
	    this->mark_idle();
	    post_state_change();
	}
    } while ( state() != Stop );
}

struct MICO::tm_init_t tm_init[] = {
    { MICO::Operation::Accept, 20, 0, 0, MICO::MsgChannel::passive, 0, 0, FALSE },
    { MICO::Operation::Writer, 20, 0, 0, MICO::MsgChannel::passive, 0, 0, FALSE },
    { MICO::Operation::Reader, 20, 0, 0, MICO::MsgChannel::passive, 0, 0, FALSE },
    { MICO::Operation::DeCode, 20, 0, 0, MICO::MsgChannel::active,  1, 0, TRUE  },
    { MICO::Operation::ORB,    10, 0, 0, MICO::MsgChannel::active,  1, 0, TRUE  } };

MICO::ThreadPoolManager::~ThreadPoolManager()
{
    for (int i = 0; i < 5; i++) {
	ThreadPool* pool = tp[tm_init[i].OP_type];
	tp[tm_init[i].OP_type] = NULL;
	delete pool;
    }
}

/*!
 * \ingroup micomt
 * \param conn_limit	Maximum number of connections
 * \param req_limit	Maximum number of requests?
 * This method performs the initial configuration of the threading
 * subsystem for mico. The connection limit and request limit are assigned
 * to various thread pools and the thread pool manager is created.
 */
void 
MICO::MTManager::thread_setup(unsigned int conn_limit, unsigned int req_limit)
{
    if (!MICO::MTManager::thread_pool()) {
	// thread-per-connection and thread-per-request concurrency models
	if (!conn_limit)
	    {
		cerr << "-ORBConnLimit: Connection Limit must be non-zero" << endl;
		exit (-1);
	    }
	tm_init[1].max = conn_limit;
	tm_init[2].max = conn_limit;
    }
    else {
	// thread-pool concurrency model
	if (conn_limit > 0) {
	    tm_init[1].max = conn_limit;
	    tm_init[2].max = conn_limit;
	}
	else {
	    // connection checking disabled by -ORBConnLimit 0
	    // but we have to set max limit on reader thread pool
	    // because this thread-pool is used by client side
	    tm_init[2].max = 10;
	}
    }
    if (!req_limit)
    {
        cerr << "-ORBRequestLimit: Request Limit must be non-zero" << endl;
        exit (-1);
    }
    tm_init[3].max = req_limit;

    _S_thread_pool_manager = new MICO::ThreadPoolManager();

    for (unsigned int i = 0; i < sizeof(tm_init)/sizeof(MICO::tm_init_t); i++) {

	MICO::ThreadPool *tp = new MICO::ThreadPool( tm_init[i].max,
									tm_init[i].max_idle,
									tm_init[i].min_idle );
	_S_thread_pool_manager->register_tp( tm_init[i].OP_type, *tp );

	switch (tm_init[i].OP_type) {
	case MICO::Operation::DeCode: {
	    tp->set_operation( new MICO::InputHandler() );
	    break;
	}
	case MICO::Operation::ORB: {
	    tp->set_operation( new MICO::MTDispatcher() );
	    break;
	}
	default:
	    break;
	}
	
	switch ( tm_init[i].mct ) {
	case MICO::MsgChannel::active:
	    if ( tm_init[i].mc_size )
		tp->register_input_mc( new MICO::ActiveMsgQueue() );
	    break;
	case MICO::MsgChannel::direct: {         
	    MICO::DirectMsgConnector *mq = new MICO::DirectMsgConnector();
	    mq->set_operation( tp->get_operation() );
	    tp->register_input_mc( mq );
	    break;
	}
	default:
	    break;
	}
	if ( tm_init[i].start ) {
	    tp->start_threads();
	}
    }
#ifdef DEBUG_NAMES
    _S_thread_pool_manager->get_operation( MICO::Operation::DeCode )->name("InputHandler");
    _S_thread_pool_manager->get_operation( MICO::Operation::ORB )->name("MTDispatcher");
#endif
}

CORBA::Long MICO::MTManager::_S_server_concurrency_model = MICO::MTManager::_S_thread_pool;
CORBA::Long MICO::MTManager::_S_client_concurrency_model = MICO::MTManager::_S_threaded_client;
MICO::ThreadPoolManager* MICO::MTManager::_S_thread_pool_manager = NULL;

void
MICO::MTManager::server_concurrency_model(MICO::MTManager::ConcurrencyModel __model)
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "Using " << __model << " as a concurrency model of whole orb." << endl;
    }
    _S_server_concurrency_model = __model;
}

void
MICO::MTManager::client_concurrency_model(MICO::MTManager::ConcurrencyModel model)
{
    _S_client_concurrency_model = model;
}

CORBA::Long
MICO::MTManager::server_concurrency_model()
{
    return MICO::MTManager::_S_server_concurrency_model;
}

CORBA::Long
MICO::MTManager::client_concurrency_model()
{
    return MICO::MTManager::_S_client_concurrency_model;
}

CORBA::Boolean
MICO::MTManager::thread_pool()
{
    return _S_server_concurrency_model == _S_thread_pool;
}

CORBA::Boolean
MICO::MTManager::thread_per_connection()
{
    return _S_server_concurrency_model == _S_thread_per_connection;
}

CORBA::Boolean
MICO::MTManager::thread_per_request()
{
    return _S_server_concurrency_model == _S_thread_per_request;
}

CORBA::Boolean
MICO::MTManager::threaded_client()
{
    return _S_client_concurrency_model == _S_threaded_client;
}

CORBA::Boolean
MICO::MTManager::blocking_threaded_client()
{
    return _S_client_concurrency_model == _S_blocking_threaded_client;
}

CORBA::Boolean
MICO::MTManager::reactive_client()
{
    return _S_client_concurrency_model == _S_reactive_client;
}

void
MICO::MTManager::free()
{
    if (_S_thread_pool_manager != NULL) {
	delete _S_thread_pool_manager;
	_S_thread_pool_manager = NULL;
    }
}
