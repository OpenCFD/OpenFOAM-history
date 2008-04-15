/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  Operation provider basic decl.
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


#ifndef __mico_operation_h__
#define __mico_operation_h__

namespace MICO
{

class WorkerThread;
class ThreadPool;

    /*!
     * \ingroup micomt
     * The OperationInfoNode class is a
     * node in an OperationPipeline and maintains
     * information about the operations at that
     * node.
     *
     * \todo
     * What's the point of making the node information
     * a serverless object. Explain.
     */
    class OperationInfoNode : public CORBA::ServerlessObject
    {
    public:
	//! \name Constructor/Destructor
	//@{
	OperationInfoNode();
	OperationInfoNode( MICO::ThreadPool &_tp );
	virtual ~OperationInfoNode();
	//@}

	// \name Thread Pool Access
	//@{
	ThreadPool& get_thread_pool() const;
	void set_thread_pool(ThreadPool* _tp);
	//@}

	//! \name Operation Type Access
	//@{
	OP_id_type get_op_id() const;
	void set_op_id( const OP_id_type _id );
	//@}

	//! \name Pipeline Linkage Operations
	//@{
	MsgChannel &get_next_op() const;
	void set_next_op( MsgChannel &_nextOP);
	//@}

    protected:
	MsgChannel *nextOP;		//!< The next node in the pipeline
	ThreadPool *tp;			//!< The thread pool servicing the node
	OP_id_type OP_id;		//!< The operation type.
    };

    /*!
     * \ingroup micomt
     * The operation is a core unit of the threading support
     * for MICO. Each operation represents a single unit of
     * work for a request.
     *
     * \todo
     * Document this class a little more.
     *
     * \todo
     * It has come to my attention that nowhere in the ORB
     * code is the node information set. The current implementation
     * allows the replacement of nodes, losing them to the world.
     * It might be better to just make the OperationNodeInfo a
     * member of the Operation instead of a pointer. What does
     * that mean in terms of being a serverless object?
     */
    class Operation: public MsgChannel
#ifdef DEBUG_NAMES
		   , public NamedObject
#endif // DEBUG_NAMES
    {
    public:

	/*!
	 * The type declaration enum describes types
	 * for the operation type.
	 *
	 * \todo
	 * Why is this specified separately from OP_id_type?
	 * OP_id_type should be this enum instead of just
	 * a typedef'd int.
	 */
	enum OP_id_type_decl {
	    Accept,
	    Reader,
	    Writer,
	    DeCode,
	    ORB
	};

	//! \name Constructor/Destructor
	//@{
	Operation();
	Operation( const Operation &op );
	Operation( ThreadPool& _tp );
	virtual ~Operation();
	//@}

	//! \name Node Access Methods
	//@{
	void info( OperationInfoNode &_info );
	OperationInfoNode& info() const;
	//@}

	//! \name Thread Access Methods
	//@{
	void thread( WorkerThread *_thr );
	WorkerThread* thread() const;
	//@}

	//! \name Shutdown Methods
	//@{
	virtual void init_shutdown();
	virtual void finalize_shutdown();
	//@}

	//! \name Message Processing Methods
	//@{
	//    virtual msg_type * get_msg( OP_id_type nextOP_id);
	void put_msg( OP_id_type nextOP_id, msg_type *msg);
	void send_msg( OP_id_type nextOP_id, msg_type * msg);
	virtual void process( msg_type *msg );
  	virtual Operation *copy() const = 0;
	//@}

	//! \name Execution Methods
	//@{
	void start();
	virtual void _run() = 0;
	//@}

    protected:
	OperationInfoNode    *_info;	//!< Pipeline node information
	WorkerThread         *_thr;	//!< Thread servicing operation
    };

    /*!
     * \ingroup micomt
     * The ActiveOperation class implements an active
     * operation. an Active operation is responsible for
     * retrieving the next message from the input channel
     * as opposed to having it put there.
     *
     * \todo
     * General note on naming conventions: all classes
     * should start with a capital letter. Names starting
     * with a lowercase letter are traditionally given
     * to functions or structures.
     *
     * \todo
     * Has no destructor. This almost certainly a memory leak.
     */
    class ActiveOperation: public Operation
    {
    public:
	//! \name Constructors/Destructor
	//@{
	ActiveOperation();
	ActiveOperation( const ActiveOperation &op );
	ActiveOperation( ThreadPool& _tp );
	//@}

	//! \name Input Channel Access
	//@{
	/*!
	 * \param mc		The input channel for the oepration
	 * Register an input channel with the operation. This
	 * allows the operation to get queued messages from the
	 * message channel.
	 */
	void register_input_mc( PassiveMsgQueue *mc )
	{
	    input_mc = mc;
	};

	/*!
	 * \return A pointer to the input channel.
	 * Get the passive input channel.
	 */
	PassiveMsgQueue *get_input_mc() const
	{
	    return input_mc;
	};
	//@}
    
	//! \name Execution Methods
	//@{
	void init_shutdown();
	void _run();
	//@}

    protected:
	PassiveMsgQueue *input_mc;	//!< The passive input channel
    };

    /*!
     * \ingroup micomt
     * The PassiveOperation class implements a passive
     * operation. A message must be put into a passive
     * operation as opposed to taking the next operation
     * from the input channel.
     *
     * \todo
     * Has no destructor. This is probably a memory leak.
     */
    class PassiveOperation: public Operation
    {
    public:
	//! \name Constructors/Destructor
	//@{
	/*!
	 * The constructor creates the object without a thread
	 * pool.
	 */
	PassiveOperation(): Operation()
	{};

	/*!
	 * \param _tp		The thread pool for the operation
	 * This constructor creates the operations with a thread
	 * pool.
	 */
	PassiveOperation( ThreadPool& _tp ): Operation( _tp )
	{};
	//@}

	//! \name Message Processing
	//@{
	void put_msg( OP_id_type nextOP_id, msg_type *msg);
	//@}

	//! \name Execution Methods
	//@{
	void _run();
	//@}

    protected:
	msg_type       *_msg;		// The message being processed
    };

}

#endif // __mico_operation_h__
