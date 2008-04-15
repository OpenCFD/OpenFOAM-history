// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
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

/*
 * stuff defined in CORBA::
 */

#ifdef MICO_MODULE_CORBA
#ifndef __MICO_STATIC_DISPATCH_H_CORBA__
#define __MICO_STATIC_DISPATCH_H_CORBA__


// Interface dispatcher for SII
class StaticInterfaceDispatcher  {
public:
    virtual ~StaticInterfaceDispatcher ();
    virtual bool dispatch( CORBA::StaticServerRequest_ptr _req,
			   CORBA::Environment &_env ) = 0;
};

/*
 * The following class is the base class for all automatically
 * generated skeleton classes by idl. It maintains a list
 * of dispatchers which are sequentially offered an incoming
 * method invocation.
 */

class StaticMethodDispatcher : public virtual CORBA::StaticImplementation {
protected:
    void register_dispatcher( StaticInterfaceDispatcher *disp );
public:
    virtual ~StaticMethodDispatcher ();
    virtual void invoke( CORBA::StaticServerRequest_ptr _req,
			 CORBA::Environment &_env );
private:
    typedef vector<StaticInterfaceDispatcher*> VecInterfaceDispatcher;
    VecInterfaceDispatcher _dispatcher;
};

#endif // __MICO_STATIC_DISPATCH_H_CORBA__
#endif // MICO_MODULE_CORBA

/*
 * stuff defined in ::
 */

#ifdef MICO_MODULE__GLOBAL
#ifndef __MICO_STATIC_DISPATCH_H_GLOBAL__
#define __MICO_STATIC_DISPATCH_H_GLOBAL__


template<class T>
class StaticInterfaceDispatcherWrapper :
  public CORBA::StaticInterfaceDispatcher {
private:
    T* _interface;
public:
    StaticInterfaceDispatcherWrapper( T* iface )
    {
	_interface = iface;
    }
    virtual bool dispatch( CORBA::StaticServerRequest_ptr _req,
			   CORBA::Environment &_env )
    {
	return _interface->dispatch( _req, _env );
    }
};

typedef CORBA::StaticMethodDispatcher StaticMethodDispatcher;
typedef CORBA::StaticInterfaceDispatcher StaticInterfaceDispatcher;

#endif // __MICO_STATIC_DISPATCH_H_GLOBAL__
#endif // MICO_MODULE__GLOBAL
