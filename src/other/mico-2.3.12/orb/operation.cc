/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  Operation provider
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
#include <mico/impl.h>

#endif // FAST_PCH


using namespace std;

/*!
 * Initialze the node with no output channel or
 * thread pool.
 */
MICO::OperationInfoNode::OperationInfoNode() :
    nextOP( NULL ),
    tp( NULL )
{
}

/*!
 * \param _tp		The thread pool associated with the operation.
 * Initialize the node with a thread pool
 */
MICO::OperationInfoNode::OperationInfoNode( MICO::ThreadPool &_tp ) :
    nextOP( NULL ),
    tp( &_tp )
{
}

/*!
 * The destructor does nothing.
 */
MICO::OperationInfoNode::~OperationInfoNode() 
{
}

/*!
 * Initialize the operation and with it
 * an operation node.
 */
MICO::Operation::Operation() :
    _thr( NULL )
{
    _info = new OperationInfoNode;
}

/*!
 * Copy the operation object. The thread (if there is
 * one) is not copied, but remains NULL.
 *
 * \todo
 */
MICO::Operation::Operation(const Operation &op)
    : _info(op._info), _thr(NULL)
{
}

/*!
 * Initialize the operation with a thread pool.
 */
MICO::Operation::Operation( MICO::ThreadPool& _tp ) :
    _thr( NULL )
{
    _info = new OperationInfoNode( _tp );
}

/*!
 * The destructor ensures that the thread is NULL and
 * then releases the information node.
 *
 * \todo
 * See note in MICO::Operation on the info member.
 */ 
MICO::Operation::~Operation()
{
    assert( !_thr );
    CORBA::release (_info);
}

/*!
 * \param nextOP_id	The next operation type to send the msg to
 * \param msg		The message to send to the next op
 *
 * Send a message to the next operation in the pipeline.
 */
void
MICO::Operation::send_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg)
{
    this->info().get_next_op().put_msg( nextOP_id, msg );
}

/*!
 * Not implemented.
 */
void
MICO::Operation::init_shutdown()
{}

/*!
 * Makes sure the thread doesn't know about this operation
 * anymore.
 */
void
MICO::Operation::finalize_shutdown()
{
    // make sure the thread has left the context of
    //   this OP, deregisterOP will take care of that !

    if (_thr)
	_thr->deregister_operation( this );
}

/*
 *  the assert's below mark methods that have to be overloaded
 *  in derived classes
 *
 *  the derived classes have to overload only one of those !!!
 *
 */

/*!
 * Must be implemented in derived classes.
 */
void
MICO::Operation::process( MICO::msg_type *msg )
{

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "implementation error - this should never happen" << endl;
    }
    assert( 0 );
}

/*!
 * Must be implemented in derived classes.
 */
void
MICO::Operation::put_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg)
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "implementation error - Operation::put_msg - this should never happen"
	    << endl;
    }
    assert( 0 );
}

/*!
 * \param nextOP_id	The next operation type to send the message to
 * \param msg		The message to send to the next operation
 *
 * Put the message to the next operation or message channel in the
 * pipeline.
 *
 * \todo
 * Design anaylsis: There seems to be a trend of identifying operation
 * types, the use of the types in methods like this seems to introduce more
 * room for failure. Re-thinking the architecture of the pipeline to be
 * completely type anonymous might not be a bad idea.
 */
void
MICO::PassiveOperation::put_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg)
{
    assert( this->info().get_op_id() == nextOP_id );

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "PassiveOperation::put_msg():" << msg << endl;
    }
    _msg = msg;
}

/*!
 * If the message is a terminate message, the message is deleted.
 * Otherwise the message is processed and forgotten.
 */
void
MICO::PassiveOperation::_run() {

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "PassiveOperation::_run():" << _msg << endl;
    }
    // make sure the message exists
    if( !_msg ) {
	return;
    }

    if ( _msg->get_type() == msg_type::Terminate ) {
	// delete terminate messages
	delete _msg;
    }
    else {
	// process everything else
	process( _msg );
    }

    // forget about the message
    _msg = NULL;
}

/*!
 * Initialize an empty active operation.
 */
MICO::ActiveOperation::ActiveOperation() :
    Operation(),
    input_mc( NULL )
{
}

/*!
 * \param op		The operation to copy
 * Create a copy of the specified operation.
 */
MICO::ActiveOperation::ActiveOperation( const ActiveOperation &op ) :
    Operation( op ),
    input_mc( op.input_mc )
{
}

/*!
 * \param _tp		The thread pool associated with the operation
 * Initialize the operation with a thread pool.
 */
MICO::ActiveOperation::ActiveOperation( MICO::ThreadPool& _tp ) :
    Operation( _tp ),
    input_mc( NULL )
{
}

/*!
 * This method creates a new terminate message
 * and puts it into the input channel.
 *
 * \todo
 * This seems akward, posting a message back to the input
 * stream to get rid of this operation. I don't know how else
 * to do it but there should be a better way.
 */
void
MICO::ActiveOperation::init_shutdown() {

    if ( input_mc ) {
	MICO::msg_type *msg = new msg_type( 0, MICO::msg_type::Terminate );
	
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "MICO::ActiveOperation::terminate" << endl;
	}
	this->get_input_mc()->put_msg( this->info().get_op_id(), msg );

	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread)
		<< "  exiting thread: " << this->thread() << endl;
	}
    }
}
/*!
 * The run message will take messages from the input
 * queue and process them. If a terminate message is
 * found, the operation will stop running.
 *
 * \todo
 * Why is it while (42)?
 */
void
MICO::ActiveOperation::_run() {

    while (42) {

        if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread) 
		<< __NAME (name () <<) "ActiveOperation::_run(): waiting for a msg"
		<< endl;
	}
        msg_type *msg = this->get_input_mc()->get_msg( this->info().get_op_id() );
        //
        // We are guaranteed that msg is not NULL because get_msg() asserts
        // this, so don't bother checking it again after the call to get_msg()
        // above!!!
        //

        if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread) 
		<< __NAME (name () <<) "ActiveOperation::_run(): recv'd msg"
		<< endl;
	}
        if (msg->get_type() == msg_type::Terminate) {
            delete msg;
	    break;
        }

        process( msg );
    }
}

