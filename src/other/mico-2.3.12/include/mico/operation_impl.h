/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  Operation provider basic impl.
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


#ifndef __mico_operation_impl_h__
#define __mico_operation_impl_h__

namespace MICO
{

/******************* OperationInfoNode *******************************/

/*!
 * \return A reference to the thread pool
 * The getThreadPoool method returns a reference to the
 * thread pool that collaborates with the node.
 */
inline ThreadPool& OperationInfoNode::get_thread_pool() const {
    return *tp;
}

/*!
 * \param _tp		The thread pool for the node.
 * Set the thread pool associated with the node.
 */
inline void OperationInfoNode::set_thread_pool(ThreadPool* _tp) {
    tp = _tp;
}

/*!
 * \return The operation type
 * Get the operation id type.
 */
inline OP_id_type OperationInfoNode::get_op_id() const {
    return OP_id;
}

/*!
 * \param
 * Get the operation id type.
 */
inline void OperationInfoNode::set_op_id( const OP_id_type _id ) {
    OP_id = _id;
}

/*!
 * \return A reference to the output message channel
 * Get the output channel for the node.
 */
inline MsgChannel& OperationInfoNode::get_next_op() const {
    return *nextOP;
}

/*!
 * \param _nextOP
 * Set the output channel for the node.
 */
inline void OperationInfoNode::set_next_op( MsgChannel& _nextOP) {
    nextOP = &_nextOP;
}


/******************** Operation ***************************************/

/*!
 * Start the operation. This just posts back to the thread
 * to change the state.
 */
inline void Operation::start()
{
    //    Thread().start();
    this->thread()->post_state_change();
}

/*!
 * \param _info		The pipeline node for the operation
 * Set the node for the operation.
 *
 * \todo
 * Name should probably just be info()
 */
inline void Operation::info( OperationInfoNode &__info )
{
    _info = &__info;
}

/*!
 * \return A reference to the pipeline node
 * Get a reference to the node associated with the operation.
 *
 * \todo
 * Name should probably just be info()
 */
inline OperationInfoNode& Operation::info() const
{
    return *_info;
}

/*!
 * \param _thr		The thread working on the operation
 * Set the worker thread associated with the operation.
 */
inline void Operation::thread( WorkerThread* thr )
{
    _thr = thr;
}

/*!
 * \return A pointer to a worker thread.
 * Get the worker thread associated with the operation.
 */
inline WorkerThread* Operation::thread() const { return _thr; }

}

#endif // __mico_operation_impl_h__
