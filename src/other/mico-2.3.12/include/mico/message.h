/* -*- mode: c++; c-basic-offset: 4; -*-
 *
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  message handling
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


#ifndef __mico_message_h__
#define __mico_message_h__

namespace MICO
{

    class Operation;
    class ThreadPool;
    class WorkerThread;

    /*!
     * \ingroup micomt
     * The msg_type class encapsulates a single message type
     * and, if debugging is enabled, profiling information about
     * the creation, and deletion time of the message.
     */
    class msg_type
    {
    public:
	/*!
	 * \enum MsgType
	 *
	 * The MsgType enum defines possible message types. There
	 * are two: A terminate message and a process message.
	 */
	enum MsgType {
	    Terminate,		//!< Something is being terminated
	    Process		//!< Something needs to be processed
	};

	//! \name Constructor/Destructor
	//@{
	/*!
	 * \param _ptr		Data encapsulated by the message type
	 * \param _t		The message type. Default is Process.
	 *
	 * The default constructor creates a message type containing
	 * some (if any) data and the type of message. If MTDEBUG is
	 * defined during compilation, a timestamps is created on
	 * creation.
	 */
	msg_type( void *_ptr, MsgType _t = Process ) :
	    type( _t ),
	    ptr(_ptr)
	{
#ifdef MTDEBUG
	    create_time = OSMisc::timestamp();
#endif
	};

	/*!
	 * The destructor doesn't really do anything unless MTDEBUG is
	 * defined during compilation. At this point, profiling information
	 * about the message type is wrttien to the log.
	 */
	~msg_type()
	{ 
#ifdef MTDEBUG
	    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
		MICOMT::AutoDebugLock __lock;
		MICO::Logger::Stream (MICO::Logger::Thread)
		    << "  time from create to delete:  " 
		    << OSMisc::timestamp() - create_time
		    << " ms" << endl
		    << "  time from pop to delete:     " 
		    << OSMisc::timestamp() - pop_time
		    << " ms" << endl;
	    };
#endif
	};
	//@}
    
	//! \name Message Type Operations
	//@{
	/*!
	 * \return The message type
	 *
	 * This method gets the message type.
	 */
	MsgType get_type() const
	{
	    return type;
	};
    
	/*!
	 * \param _t		The new message type
	 *
	 * This method sets the message type.
	 */
	void set_type( MsgType _t)
	{
	    type = _t;
	};

	/*!
	 * \return The data encapsulated by the message.
	 *
	 * This method returns a pointer to the data encapsulated
	 * by the message.
	 */
	void *data()
	{
	    return ptr;
	};
	//@}

#ifdef MTDEBUG
	MICO_Long create_time;	//!< Creation time of the message type
	MICO_Long push_time;	//!< When the message type was pushed
	MICO_Long pop_time;	//!< When the message type was popped
#endif
	int value;		//!< Value associated with the message type.

    protected:
	MsgType type;		//!< The message type
	void *ptr;		//!< Encapsulated data
    
    };

    /*!
     * \ingroup micomt
     *
     * The OP_id_type defines the operation id type
     * for an operation. I'm still not sure what this is
     * really used for.
     */
    typedef int OP_id_type;

    /*!
     * \ingroup micomt
     *
     * The MsgChannel class...
     */
    class MsgChannel {
    public:
	/*!
	 * Types of some sort.
	 */
	enum Types {
	    active,
	    passive,
	    direct
	};

	virtual ~MsgChannel()
	{}

	//! \name Thread Pool Operations
	//@{
	void set_thread_pool( ThreadPool *_tp ) { tp = _tp; };
	ThreadPool *get_thread_pool() { return tp; };
	//@}

	//! \name Message Operations
	//@{
	virtual MICO_Boolean check_msg( MICO::WorkerThread * );
	virtual msg_type *get_msg( OP_id_type );
	virtual void put_msg( OP_id_type, msg_type *) = 0;
	//@}

    protected:
	ThreadPool	*tp;	//!< ThreadPool associated with
				//!< The MessageChannel.
    };


    /*!
     * \ingroup micomt
     *
     * An active message queue container.
     */
    class ActiveMsgQueue: public MsgChannel
#ifdef DEBUG_NAMES
			,public NamedObject 
#endif
    {

    public:
	//! \name Constructor/Destructor
	//@{
	ActiveMsgQueue();
	virtual ~ActiveMsgQueue();
	//@}

#ifdef DEBUG_NAMES
	const char *name (const char *new_name = NULL);
#endif

	//! \name Active Message Queue Operations
	//@{
	MICO_Boolean check_msg( MICO::WorkerThread * );
	void put_msg( OP_id_type nextOP_id, msg_type *);
	//@}

    protected:
	std::queue<msg_type*>	Q;		//!< The queue container
	MICOMT::Mutex		Qaccess;	//!< A lock for the queue
    };


    /*!
     * \ingroup micomt
     *
     * A passive message queue container
     */
    class PassiveMsgQueue: public MsgChannel
#ifdef DEBUG_NAMES
			 ,public NamedObject 
#endif
    {
    public:
	//! \name Constructor/Destructor
	//@{
	PassiveMsgQueue();
	virtual ~PassiveMsgQueue();
	//@}

#ifdef DEBUG_NAMES
	const char *name (const char *new_name = NULL);
#endif

	//! \name Passive Message Queue Operations
	//@{
	msg_type *get_msg( OP_id_type nextOP_id );
	void put_msg( OP_id_type nextOP_id, msg_type *msg);
	//@}

    protected:
	std::queue<msg_type*>	Q;		//!< The queue container
	MICOMT::Mutex		Qaccess;	//!< A lock for the queue
	MICOMT::Semaphore	msg_cnt;	//!< The message count

    };

    /*!
     * \ingroup micomt
     *
     * A direct message connector.
     */
    class DirectMsgConnector: public MsgChannel
#ifdef DEBUG_NAMES
			    ,public NamedObject 
#endif
    {
    public:
	//! \name Constructor/Destructor
	//@{
	DirectMsgConnector();
	virtual ~DirectMsgConnector();
	//@}

#ifdef DEBUG_NAMES
	const char *name (const char *new_name = NULL);
#endif
    
	//! \name Direct Message Connector Operations
	//@{
	void set_operation( Operation *_op )
	{
	    op = _op;
	};

	void put_msg( OP_id_type nextOP_id, msg_type *msg);
	//@}
    
    protected:
	Operation		*op;		//!< The operation
    };

}

#endif // __mico_message_h__
