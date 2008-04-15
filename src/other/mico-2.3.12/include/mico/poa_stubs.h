// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (C) 1998 Frank Pilhofer
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
 * Note: the implementation is in orb/poa_base.cc
 */

/*
 * By specification, AdapterActivators and ServantManagers are Servant
 * implementations, not pseudo objects, that must be activated like any
 * other servant. So they're handled like any other servant.
 * These interfaces are idl-generated but slightly hacked so that no
 * DII stubs exist
 */

#if !defined(__POA_STUBS_H__) || defined(MICO_NO_TOPLEVEL_MODULES)
#define __POA_STUBS_H__

#ifndef MICO_NO_TOPLEVEL_MODULES
namespace PortableServer {
#endif

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_PortableServer)

class AdapterActivator_stub_clp :
  virtual public AdapterActivator,
  virtual public PortableServer::StubBase
{
  public:
    AdapterActivator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~AdapterActivator_stub_clp ();
    CORBA::Boolean unknown_adapter( POA_ptr parent, const char* name );
  private:
    void operator=( const AdapterActivator_stub_clp & );
};

class ServantManager_stub_clp :
  virtual public ServantManager,
  virtual public PortableServer::StubBase
{
  public:
    ServantManager_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ServantManager_stub_clp ();
  private:
    void operator=( const ServantManager_stub_clp & );
};

class ServantActivator_stub_clp :
  virtual public ServantActivator,
  virtual public ::PortableServer::ServantManager_stub_clp
{
  public:
    ServantActivator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ServantActivator_stub_clp ();
    ::PortableServer::Servant incarnate( const ObjectId& oid, POA_ptr adapter );
    void etherealize( const ObjectId& oid, POA_ptr adapter, ::PortableServer::Servant serv, CORBA::Boolean cleanup_in_progress, CORBA::Boolean remaining_activations );
  private:
    void operator=( const ServantActivator_stub_clp & );
};

class ServantLocator_stub_clp :
  virtual public ServantLocator,
  virtual public ::PortableServer::ServantManager_stub_clp
{
  public:
    ServantLocator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ServantLocator_stub_clp ();
    ::PortableServer::Servant preinvoke( const ObjectId& oid, POA_ptr adapter, const char* operation, Cookie& the_cookie );
    void postinvoke( const ObjectId& oid, POA_ptr adapter, const char* operation, Cookie the_cookie, ::PortableServer::Servant the_servant );
  private:
    void operator=( const ServantLocator_stub_clp & );
};

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_PortableServer)

#ifndef MICO_NO_TOPLEVEL_MODULES
} 
#endif

#ifndef MICO_NO_TOPLEVEL_MODULES
namespace POA_PortableServer {
#endif

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_POA_PortableServer)

class AdapterActivator : virtual public PortableServer::DynamicImplementation
{
  public:
    virtual ~AdapterActivator ();
    PortableServer::AdapterActivator_ptr _this ();
    bool dispatch (CORBA::ServerRequest_ptr);
    virtual void invoke (CORBA::ServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual char * _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static AdapterActivator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Boolean unknown_adapter( PortableServer::POA_ptr parent, const char* name ) = 0;

  protected:
    AdapterActivator () {};

  private:
    AdapterActivator (const AdapterActivator &);
    void operator= (const AdapterActivator &);
};

class ServantManager : virtual public PortableServer::DynamicImplementation
{
  public:
    virtual ~ServantManager ();
    PortableServer::ServantManager_ptr _this ();
    bool dispatch (CORBA::ServerRequest_ptr);
    virtual void invoke (CORBA::ServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual char * _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ServantManager * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);


  protected:
    ServantManager () {};

  private:
    ServantManager (const ServantManager &);
    void operator= (const ServantManager &);
};

class ServantActivator : 
  virtual public POA_PortableServer::ServantManager
{
  public:
    virtual ~ServantActivator ();
    PortableServer::ServantActivator_ptr _this ();
    bool dispatch (CORBA::ServerRequest_ptr);
    virtual void invoke (CORBA::ServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual char * _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ServantActivator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual PortableServer::Servant incarnate( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter ) = 0;
    virtual void etherealize( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter, PortableServer::Servant serv, CORBA::Boolean cleanup_in_progress, CORBA::Boolean remaining_activations ) = 0;

  protected:
    ServantActivator () {};

  private:
    ServantActivator (const ServantActivator &);
    void operator= (const ServantActivator &);
};

class ServantLocator : 
  virtual public POA_PortableServer::ServantManager
{
  public:
    virtual ~ServantLocator ();
    PortableServer::ServantLocator_ptr _this ();
    bool dispatch (CORBA::ServerRequest_ptr);
    virtual void invoke (CORBA::ServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual char * _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ServantLocator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual PortableServer::Servant preinvoke( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter, const char* operation, PortableServer::ServantLocator::Cookie& the_cookie ) = 0;
    virtual void postinvoke( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter, const char* operation, PortableServer::ServantLocator::Cookie the_cookie, PortableServer::Servant the_servant ) = 0;

  protected:
    ServantLocator () {};

  private:
    ServantLocator (const ServantLocator &);
    void operator= (const ServantLocator &);
};

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_POA_PortableServer)

#ifndef MICO_NO_TOPLEVEL_MODULES
} 
#endif

#endif
