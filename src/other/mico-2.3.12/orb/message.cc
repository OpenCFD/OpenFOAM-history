/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 * 
 *  message queue
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

/****************************** MsgChannel ******************************/

/*!
 * \param kt		The worker thread.
 * \return		True or false
 *
 * Currently, this method always returns false
 */
MICO_Boolean
MICO::MsgChannel::check_msg( MICO::WorkerThread *kt )
{
    return FALSE;
}

/*!
 * \param nextOP_id
 * \return A pointer to the message type
 *
 * This method always asserts and returns NULL.
 */
MICO::msg_type *
MICO::MsgChannel::get_msg( OP_id_type nextOP_id )
{
    assert( 0 );
    return NULL;
}

/********************************* ActiveMsgQueue **************************************/

/*!
 * The constructor logs the creation of the queue.
 * If DEBUG_NAMES is enabled, names are given to the member
 * variables.
 */
MICO::ActiveMsgQueue::ActiveMsgQueue()
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "ActiveMsgQueue::ActiveMsgQueue(): (" << this << ")" << endl;
    }
#ifdef DEBUG_NAMES
    name ("UnNamed ActiveMsgQueue");
    Qaccess.name("Qaccess Mutex");
#endif
}

/*!
 * The destructor logs the destruction of the queue.
 */
MICO::ActiveMsgQueue::~ActiveMsgQueue()
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": ActiveMsgQueue::~ActiveMsgQueue(): ("
		   << this << ")" << endl;
    }
    while (!Q.empty()) {
	msg_type* msg = Q.front();
	Q.pop();
	assert(msg != NULL);
	delete msg;
    }
}

#ifdef DEBUG_NAMES
/*!
 * \param new_name		The new name of the object
 * \return			The old name of the object.
 *
 * Set the new name of the object. If NULL, the old name
 * is deleted. NULL may be returned.
 */
const char *
MICO::ActiveMsgQueue::name (const char *new_name) {
 
    if (new_name) {
        if (_name) {
	    delete [] _name;
	    _name = NULL;
	}

        _name = new char [strlen (new_name) + strlen (".Qaccess Mutex") + 1];
        strcpy (_name, new_name);
        strcat (_name, ".Qaccess Mutex");
    }
    
    return _name;
}

#endif

/*!
 * \param kt			The worker thread to check the msg
 *
 * This method assigns a message to be checked by a worker thread.
 * if kt is NULL, the first available thread in the thread pool will
 * be assigned to the check the message.
 */
MICO_Boolean
MICO::ActiveMsgQueue::check_msg( MICO::WorkerThread *kt )
{
    MICOMT::AutoLock l(Qaccess);

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": ActiveMsgQueue::check_msg: ("
	    << this << ") msg: " << endl;
    }
    if (Q.size() != 0) {
	if (!kt) {
	    kt = tp->get_idle_thread();
	    if (!kt)
		return FALSE;
	}
	msg_type *msg = Q.front();
	assert( msg );
#ifdef MTDEBUG
	msg->pop_time = OSMisc::timestamp();
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread) 
		<< "  time from create to pop:  " 
		<< msg->pop_time - msg->create_time << " ms" << endl 
		<< "  time from push to pop:    " 
		<< msg->pop_time - msg->push_time << " ms" << endl;
	}
#endif
	Q.pop();
	kt->put_msg(msg);
	kt->mark_busy();
	if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread) << msg << endl;
	}
	return TRUE;
    }
    return FALSE;
}

void
MICO::ActiveMsgQueue::put_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg)
{
    MICO::WorkerThread *kt;

    assert( msg );
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": ActiveMsgQueue::put_msg: ("
	    << this << ") msg: " << msg << endl;
    }

    MICOMT::AutoLock l(Qaccess);

    kt = tp->get_idle_thread();
#ifdef MTDEBUG
    msg->push_time = OSMisc::timestamp();
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "  time from create to MQ::put_msg: " 
	    << msg->push_time - msg->create_time << " ms" << endl;
    }
#endif
    if (kt) {
	kt->put_msg(msg);
	kt->mark_busy();
	kt->post_state_change();
    } else
	Q.push( msg );
}


/******************************** PassiveMsgQueue *************************************/

/*!
 * The constructor logs the creation of the queue.
 * If DEBUG_NAMES is enabled, names are given to the member
 * variables.
 */
MICO::PassiveMsgQueue::PassiveMsgQueue()
#ifdef DEBUG_NAMES
    : NamedObject ()
#endif
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "PassiveMsgQueue::PassiveMsgQueue(): ("
	    << this << ")" << endl;
    }

#ifdef DEBUG_NAMES
    NamedObject::name ("UnNamed PassiveMsgQueue");
    Qaccess.name("Qaccess Mutex");
    msg_cnt.name("msg_cnt Semaphore");
#endif
}

/*!
 * The destructor logs the destruction of the queue.
 */
MICO::PassiveMsgQueue::~PassiveMsgQueue()
{

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": PassiveMsgQueue::~PassiveMsgQueue(): ("
	    << this << ")" << endl;
    }
}

#ifdef DEBUG_NAMES
/*!
 * \param new_name		The new name of the object
 * \return			The old name of the object.
 *
 * Set the new name of the object. If NULL, the old name
 * is deleted. NULL may be returned.
 */
const char *
MICO::PassiveMsgQueue::name (const char *new_name) {
 
    if (new_name) {

        char *tmp =
            new char [strlen (new_name) + strlen (".Qaccess Mutex") + 1];
        strcpy (tmp, new_name);
        strcat (tmp, ".Qaccess Mutex");

	Qaccess.name (tmp);
        delete [] tmp;

        tmp =
            new char [strlen (new_name) + strlen (".msg_cnt Semaphore") + 1];
        strcpy (tmp, new_name);
        strcat (tmp, ".msg_cnt Semaphore");

	msg_cnt.name (tmp);
        delete [] tmp;
    }

    return NamedObject::name (new_name);
}
#endif

MICO::msg_type *
MICO::PassiveMsgQueue::get_msg( MICO::OP_id_type nextOP_id ) {

    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": PassiveMsgQueue::get_msg: ("
		   << this << ") msg: " << endl;
    }
    
    msg_cnt.wait();                     // wait for a message (block)
    Qaccess.lock();                     // make sure, we are the only ones
    msg_type *msg = Q.front();
    assert( msg );        

 #ifdef MTDEBUG
    msg->pop_time = OSMisc::timestamp();
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread)
	    << "  time from create to pop:  "
	    << msg->pop_time - msg->create_time << " ms" << endl
	    << "  time from push to pop:    "
	    << msg->pop_time - msg->push_time << " ms" << endl;
    }
#endif  
    Q.pop();
    Qaccess.unlock();
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) << msg << endl;
    }
    return msg; 
}
    
void
MICO::PassiveMsgQueue::put_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg)
{
    assert( msg );
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": PassiveMsgQueue::put_msg: (" << this << ") msg: "
		   << msg << endl;
    }
    Qaccess.lock();
    Q.push( msg );
#ifdef MTDEBUG
    msg->push_time = OSMisc::timestamp();
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	    MICOMT::AutoDebugLock __lock;
	    MICO::Logger::Stream (MICO::Logger::Thread) 
		<< "  time from create to MQ::put_msg: " 
		<< msg->push_time - msg->create_time << " ms" << endl;
    }
#endif
    Qaccess.unlock();
    msg_cnt.post();   
}


/******************************** DirectMsgConnector *************************************/

/*!
 * The constructor logs the creation of the queue.
 * If DEBUG_NAMES is enabled, names are given to the member
 * variables.
 */
MICO::DirectMsgConnector::DirectMsgConnector()
#ifdef DEBUG_NAMES
    : NamedObject ()
#endif
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    << "DirectMsgConnector::DirectMsgConnector(): ("
	    << this << ")" << endl;
    }
#ifdef DEBUG_NAMES
    name ("UnNamed DirectMsgConnector");
#endif
}

/*!
 * The destructor logs the destruction of the queue.
 */
MICO::DirectMsgConnector::~DirectMsgConnector()
{
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< _name) << ": DirectMsgConnector::~DirectMsgConnector(): ("
		   << this << ")" << endl;
    }
}

#ifdef DEBUG_NAMES
/*!
 * \param new_name		The new name of the object
 * \return			The old name of the object.
 *
 * Set the new name of the object. If NULL, the old name
 * is deleted. NULL may be returned.
 */
const char *
MICO::DirectMsgConnector::name (const char *new_name) {
 
    if (new_name) {

        if (_name)
            delete [] _name;

        _name = new char [strlen (new_name) + 1];
        strcpy (_name, new_name);
	
    }
    
    return _name;
}
#endif

void
MICO::DirectMsgConnector::put_msg( MICO::OP_id_type nextOP_id, MICO::msg_type *msg)
{
    assert( msg );
    if (MICO::Logger::IsLogged (MICO::Logger::Thread)) {
	MICOMT::AutoDebugLock __lock;
	MICO::Logger::Stream (MICO::Logger::Thread) 
	    __NAME(<< name ()) << ": DirectMsgConnector::put_msg: (" << this << ") msg: "
		   << msg << endl;
    }
    
    Operation* _myop = op->copy();
    
    _myop->put_msg( nextOP_id, msg);
    _myop->_run();
    
    delete _myop;
}
