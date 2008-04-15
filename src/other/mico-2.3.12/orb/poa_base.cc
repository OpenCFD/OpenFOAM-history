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

#include <CORBA.h>
#include <mico/throw.h>
#include <mico/template_impl.h>

// begin-idl-hack
namespace PortableServer {
    POA_ptr _the_root_poa = POA::_nil();
    Current_ptr _the_poa_current = Current::_nil();
}

/*
 * ServantBase implementation
 */

PortableServer::ServantBase::ServantBase ()
{
  _my_poa = NULL;
}

PortableServer::ServantBase::ServantBase (const ServantBase &)
{
  _my_poa = NULL;
}

PortableServer::ServantBase::~ServantBase ()
{
}

PortableServer::ServantBase &
PortableServer::ServantBase::operator= (const ServantBase &)
{
  return *this;
}

PortableServer::POA_ptr
PortableServer::ServantBase::_default_POA ()
{
  if (!CORBA::is_nil (PortableServer::_the_root_poa)) {
    return PortableServer::POA::_duplicate(PortableServer::_the_root_poa);
  }
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  return PortableServer::POA::_narrow (poaobj);
}

CORBA::Boolean
PortableServer::ServantBase::_is_a (const char *)
{
  return FALSE;
}

CORBA::Object_ptr
PortableServer::ServantBase::_get_component ()
{
  return CORBA::Object::_nil ();
}

CORBA::InterfaceDef_ptr
PortableServer::ServantBase::_get_interface (const char *repoid)
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    return CORBA::InterfaceDef::_nil ();
  }

  CORBA::Contained_var cv = ifr->lookup_id (repoid);
  return CORBA::InterfaceDef::_narrow (cv);
}

CORBA::Boolean
PortableServer::ServantBase::_non_existent ()
{
  return FALSE;
}

CORBA::Object_ptr
PortableServer::ServantBase::_this ()
{
  if (!CORBA::is_nil (_the_poa_current) &&
      _the_poa_current->iscurrent() &&
      _the_poa_current->get_servant() == (Servant) this) {
    return _the_poa_current->get_reference ();
  }

  PortableServer::POA_var poa;
  if (!CORBA::is_nil (_my_poa)) {
    poa = PortableServer::POA::_duplicate (_my_poa);
  }
  else {
    poa = _default_POA ();
  }

  return poa->activate_for_this (this);
}

void
PortableServer::ServantBase::_add_ref ()
{
  // empty
}

void
PortableServer::ServantBase::_remove_ref ()
{
  // empty
}

void
PortableServer::ServantBase::_check ()
{
  // empty
}

CORBA::Object_ptr
PortableServer::ServantBase::_make_stub (PortableServer::POA_ptr,
					 CORBA::Object_ptr)
{
  return CORBA::Object::_nil ();
}

void *
PortableServer::ServantBase::_narrow_helper (const char *)
{
  return NULL;
}

PortableServer::POA_ptr
PortableServer::ServantBase::_activated_in ()
{
  return _my_poa;
}

void
PortableServer::ServantBase::_activated_in (PortableServer::POA_ptr __my_poa)
{
  _my_poa = __my_poa;
}

/*
 * RefCountServantBase implementation
 */

PortableServer::RefCountServantBase::RefCountServantBase ()
  : _magic (100800884), _ref_count (1)
{
}

PortableServer::RefCountServantBase::RefCountServantBase (const RefCountServantBase &)
  : _magic (100800884), _ref_count (1)
{
}

PortableServer::RefCountServantBase::~RefCountServantBase ()
{
  _magic = 0;
}

PortableServer::RefCountServantBase &
PortableServer::RefCountServantBase::operator= (const RefCountServantBase &)
{
  _check ();
  return *this;
}

void
PortableServer::RefCountServantBase::_add_ref ()
{
  MICOMT::AutoLock l(_ref_lock);

  assert( _ref_count != 0 );

  _check ();
  _ref_count++;
}

void
PortableServer::RefCountServantBase::_remove_ref ()
{
  _ref_lock.lock();
  _check ();
  if (--_ref_count == 0) {
    _ref_lock.unlock ();
    delete this;
  }
  else
    _ref_lock.unlock ();
}

void
PortableServer::RefCountServantBase::_check ()
{
  assert (_magic == 100800884);
}

/*
 * ValueRefCountBase implementation
 */

PortableServer::ValueRefCountBase::ValueRefCountBase ()
{
}

PortableServer::ValueRefCountBase::ValueRefCountBase (const ValueRefCountBase &)
{
}

PortableServer::ValueRefCountBase::~ValueRefCountBase ()
{
}

void
PortableServer::ValueRefCountBase::_add_ref ()
{
  CORBA::DefaultValueRefCountBase::_add_ref ();
}

void
PortableServer::ValueRefCountBase::_remove_ref ()
{
  CORBA::DefaultValueRefCountBase::_remove_ref ();
}

CORBA::ULong
PortableServer::ValueRefCountBase::_refcount_value ()
{
  return CORBA::DefaultValueRefCountBase::_refcount_value ();
}

/*
 * Dynamic Implementation
 */

PortableServer::DynamicImplementation::~DynamicImplementation ()
{
}

CORBA::ServerRequestBase_ptr
PortableServer::DynamicImplementation::make_request (CORBA::ORBRequest *req,
						     CORBA::Object_ptr obj,
						     CORBA::ORBMsgId id,
						     CORBA::ObjectAdapter *oa,
						     CORBA::Principal_ptr pr)
{
  return new CORBA::ServerRequest (req, obj, id, oa, pr);
}

void
PortableServer::DynamicImplementation::doinvoke (CORBA::ServerRequestBase_ptr req)
{
  invoke ((CORBA::ServerRequest_ptr) req);
}

CORBA::Object_ptr
PortableServer::DynamicImplementation::_this ()
{
  return PortableServer::ServantBase::_this();
}

CORBA::Boolean
PortableServer::DynamicImplementation::_is_a (const char * repoid)
{
  /*
   * To call _primary_interface, we need a POA and an ObjectId.
   * If called within the context of a method invocation, we use
   * the "Current" information. Else, we activate the Servant.
   */

  PortableServer::ObjectId_var oid;
  PortableServer::POA_var poa;

  if (!CORBA::is_nil (_the_poa_current) &&
      _the_poa_current->iscurrent() &&
      _the_poa_current->get_servant() == (Servant) this) {
    poa = _the_poa_current->get_POA ();
    oid = _the_poa_current->get_object_id ();
  }
  else {
    if (!CORBA::is_nil (_my_poa)) {
      poa = PortableServer::POA::_duplicate (_my_poa);
    }
    else {
      poa = _default_POA ();
    }
    CORBA::Object_var obj = poa->activate_for_this (this);
    oid = poa->reference_to_id (obj);
  }

  CORBA::String_var pif = _primary_interface (oid.in(), poa.in());

  if (strcmp (pif.in(), repoid) == 0) {
    return TRUE;
  }

  /*
   * Access IR
   */

  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj =
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }
  
  CORBA::Contained_var cv = ifr->lookup_id (pif.in());
  CORBA::InterfaceDef_var ifd = CORBA::InterfaceDef::_narrow (cv);

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd->is_a (repoid);
}

CORBA::InterfaceDef_ptr
PortableServer::DynamicImplementation::_get_interface ()
{
  /*
   * To call _primary_interface, we need a POA and an ObjectId.
   * If called within the context of a method invocation, we use
   * the "Current" information. Else, we activate the Servant.
   */

  PortableServer::ObjectId_var oid;
  PortableServer::POA_var poa;

  if (!CORBA::is_nil (_the_poa_current) &&
      _the_poa_current->iscurrent() &&
      _the_poa_current->get_servant() == (Servant) this) {
    poa = _the_poa_current->get_POA ();
    oid = _the_poa_current->get_object_id ();
  }
  else {
    if (!CORBA::is_nil (_my_poa)) {
      poa = PortableServer::POA::_duplicate (_my_poa);
    }
    else {
      poa = _default_POA ();
    }
    CORBA::Object_var obj = poa->activate_for_this (this);
    oid = poa->reference_to_id (obj);
  }

  CORBA::String_var pif = _primary_interface (oid.in(), poa.in());

  /*
   * Access IR
   */

  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj =
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }
  
  CORBA::Contained_var cv = ifr->lookup_id (pif.in());
  CORBA::InterfaceDef_ptr ifd = CORBA::InterfaceDef::_narrow (cv);

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

/*
 * Static Implementation
 */

PortableServer::StaticImplementation::~StaticImplementation ()
{
}

CORBA::ServerRequestBase_ptr
PortableServer::StaticImplementation::make_request (CORBA::ORBRequest *req,
						    CORBA::Object_ptr obj,
						    CORBA::ORBMsgId id,
						    CORBA::ObjectAdapter *oa,
						    CORBA::Principal_ptr pr)
{
  return new CORBA::StaticServerRequest (req, obj, id, oa, pr);
}

void
PortableServer::StaticImplementation::doinvoke (CORBA::ServerRequestBase_ptr req)
{
  invoke ((CORBA::StaticServerRequest_ptr) req);
}


/*
 * Base class for stubs
 */

PortableServer::StubBase::StubBase (PortableServer::POA_ptr poa)
{
  _colocated_poa = PortableServer::POA::_duplicate(poa);
}

PortableServer::StubBase::StubBase ()
{
  assert (0);
}

PortableServer::StubBase::~StubBase ()
{
  CORBA::release (_colocated_poa);
}

PortableServer::Servant
PortableServer::StubBase::_preinvoke ()
{
  if (CORBA::is_nil (_colocated_poa)) {
    return NULL;
  }

  PortableServer::Servant serv = _colocated_poa->preinvoke (this);

  if (serv == NULL) {
    CORBA::release (_colocated_poa);
    _colocated_poa = PortableServer::POA::_nil();
    return NULL;
  }

  return serv;
}

void
PortableServer::StubBase::_postinvoke ()
{
  _colocated_poa->postinvoke ();
}

/*
 * ObjectId to Sting mapping
 */

char *
PortableServer::ObjectId_to_string (const PortableServer::ObjectId & oid)
{
  char * s = CORBA::string_alloc (oid.length());

  for (CORBA::ULong i = 0; i < oid.length(); ++i) {
    if (!(CORBA::Octet)oid[i]) {
      CORBA::string_free (s);
      mico_throw (CORBA::BAD_PARAM());
    }
    s[i] = (CORBA::Octet)oid[i];
  }
  
  s[oid.length()] = 0;
  return s;
}

PortableServer::ObjectId *
PortableServer::string_to_ObjectId (const char * s)
{
  if (!s) {
    mico_throw (CORBA::BAD_PARAM());
  }

  CORBA::ULong len = strlen (s);
  
  PortableServer::ObjectId *oid = new PortableServer::ObjectId;
  oid->length (len);
  
  for (CORBA::ULong i = 0; i < len; ++i) {
    (*oid)[i] = (CORBA::Octet)s[i];
  }
  return oid;
}

CORBA::WChar *
PortableServer::ObjectId_to_wstring (const PortableServer::ObjectId & oid)
{
  if ((oid.length() % 4) != 0) {
    mico_throw (CORBA::BAD_PARAM());
  }

  CORBA::WChar * s = CORBA::wstring_alloc (oid.length() / 4);

  for (CORBA::ULong i=0; i < oid.length()/4; i++) {
    unsigned long c =
      oid[4*i] + 256*oid[4*i+1] + 65536*oid[4*i+2] + 16777216*oid[4*i+3];
    if (c == 0) {
      CORBA::wstring_free (s);
      mico_throw (CORBA::BAD_PARAM());
    }
    s[i] = (wchar_t) c;
  }

  s[oid.length()/4] = 0;
  return s;
}

PortableServer::ObjectId *
PortableServer::wstring_to_ObjectId (const CORBA::WChar * s)
{
  if (!s) {
    mico_throw (CORBA::BAD_PARAM());
  }

  CORBA::ULong len;

  for (len=0; s[len]; len++);
  
  PortableServer::ObjectId *oid = new PortableServer::ObjectId;
  oid->length (4*len);
  
  for (CORBA::ULong i = 0; i < len; ++i) {
    (*oid)[4*i]   = ((unsigned long) s[i]) % 256;
    (*oid)[4*i+1] = ((unsigned long) s[i] / 256) % 256;
    (*oid)[4*i+2] = ((unsigned long) s[i] / 65536) % 256;
    (*oid)[4*i+3] = ((unsigned long) s[i] / 16777216) % 256;
  }
  return oid;
}

/*
 * By specification, AdapterActivators and ServantManagers are Servant
 * implementations, not pseudo objects, that must be activated like any
 * other servant. So they're handled like any other servant.
 * These interfaces are idl-generated but slightly hacked so that no
 * DII stubs exist
 */

PortableServer::AdapterActivator_stub_clp::AdapterActivator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

PortableServer::AdapterActivator_stub_clp::~AdapterActivator_stub_clp ()
{
}

CORBA::Boolean
PortableServer::AdapterActivator_stub_clp::unknown_adapter( PortableServer::POA_ptr parent, const char* name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_PortableServer::AdapterActivator * _myserv = POA_PortableServer::AdapterActivator::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        res = _myserv->unknown_adapter(parent, name);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return res;
    }
    _postinvoke ();
  }

  assert (0);
  return FALSE;
}

PortableServer::ServantManager_stub_clp::ServantManager_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

PortableServer::ServantManager_stub_clp::~ServantManager_stub_clp ()
{
}

PortableServer::ServantActivator_stub_clp::ServantActivator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa), 
  PortableServer::ServantManager_stub_clp(poa, obj)
{
}

PortableServer::ServantActivator_stub_clp::~ServantActivator_stub_clp ()
{
}

PortableServer::Servant
PortableServer::ServantActivator_stub_clp::incarnate( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_PortableServer::ServantActivator * _myserv = POA_PortableServer::ServantActivator::_narrow (_serv);
    if (_myserv) {
      PortableServer::Servant res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        res = _myserv->incarnate(oid, adapter);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return res;
    }
    _postinvoke ();
  }

  assert (0);
  return NULL;
}

void
PortableServer::ServantActivator_stub_clp::etherealize( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter, PortableServer::Servant serv, CORBA::Boolean cleanup_in_progress, CORBA::Boolean remaining_activations )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_PortableServer::ServantActivator * _myserv = POA_PortableServer::ServantActivator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->etherealize(oid, adapter, serv, cleanup_in_progress, remaining_activations);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  assert (0);
}

PortableServer::ServantLocator_stub_clp::ServantLocator_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa), 
  PortableServer::ServantManager_stub_clp(poa, obj)
{
}

PortableServer::ServantLocator_stub_clp::~ServantLocator_stub_clp ()
{
}

PortableServer::Servant
PortableServer::ServantLocator_stub_clp::preinvoke( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter, const char* operation, PortableServer::ServantLocator::Cookie& the_cookie )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_PortableServer::ServantLocator * _myserv = POA_PortableServer::ServantLocator::_narrow (_serv);
    if (_myserv) {
      PortableServer::Servant res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        res = _myserv->preinvoke(oid, adapter, operation, the_cookie);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return res;
    }
    _postinvoke ();
  }

  assert (0);
  return NULL;
}

void
PortableServer::ServantLocator_stub_clp::postinvoke( const PortableServer::ObjectId& oid, PortableServer::POA_ptr adapter, const char* operation, PortableServer::ServantLocator::Cookie the_cookie, PortableServer::Servant the_servant )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_PortableServer::ServantLocator * _myserv = POA_PortableServer::ServantLocator::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->postinvoke(oid, adapter, operation, the_cookie, the_servant);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  assert (0);
}

// PortableServer Skeleton Class for interface PortableServer::AdapterActivator
POA_PortableServer::AdapterActivator::~AdapterActivator()
{
}

PortableServer::AdapterActivator_ptr
POA_PortableServer::AdapterActivator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return PortableServer::AdapterActivator::_narrow (obj);
}

CORBA::Boolean
POA_PortableServer::AdapterActivator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/PortableServer/AdapterActivator:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_PortableServer::AdapterActivator::_get_interface ()
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/PortableServer/AdapterActivator:1.0");
  CORBA::InterfaceDef_ptr ifd = CORBA::InterfaceDef::_narrow (cv);

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_PortableServer::AdapterActivator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/PortableServer/AdapterActivator:1.0");
}

void *
POA_PortableServer::AdapterActivator::_narrow_helper (const char * repoid)
{

  if (strcmp (repoid, "IDL:omg.org/PortableServer/AdapterActivator:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_PortableServer::AdapterActivator *
POA_PortableServer::AdapterActivator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/PortableServer/AdapterActivator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_PortableServer::AdapterActivator *) p;
  }
  return NULL;
}

CORBA::Object_ptr
POA_PortableServer::AdapterActivator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new PortableServer::AdapterActivator_stub_clp (poa, obj);
}

bool
POA_PortableServer::AdapterActivator::dispatch (CORBA::ServerRequest_ptr)
{
  return false;
}

void
POA_PortableServer::AdapterActivator::invoke (CORBA::ServerRequest_ptr)
{
  assert (0);
}


// PortableServer Skeleton Class for interface PortableServer::ServantManager
POA_PortableServer::ServantManager::~ServantManager()
{
}

PortableServer::ServantManager_ptr
POA_PortableServer::ServantManager::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return PortableServer::ServantManager::_narrow (obj);
}

CORBA::Boolean
POA_PortableServer::ServantManager::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/PortableServer/ServantManager:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_PortableServer::ServantManager::_get_interface ()
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/PortableServer/ServantManager:1.0");
  CORBA::InterfaceDef_ptr ifd = CORBA::InterfaceDef::_narrow (cv);

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_PortableServer::ServantManager::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/PortableServer/ServantManager:1.0");
}

void *
POA_PortableServer::ServantManager::_narrow_helper (const char * repoid)
{

  if (strcmp (repoid, "IDL:omg.org/PortableServer/ServantManager:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_PortableServer::ServantManager *
POA_PortableServer::ServantManager::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/PortableServer/ServantManager:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_PortableServer::ServantManager *) p;
  }
  return NULL;
}

CORBA::Object_ptr
POA_PortableServer::ServantManager::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new PortableServer::ServantManager_stub_clp (poa, obj);
}

bool
POA_PortableServer::ServantManager::dispatch (CORBA::ServerRequest_ptr)
{
  return false;
}

void
POA_PortableServer::ServantManager::invoke (CORBA::ServerRequest_ptr)
{
  assert (0);
}


// PortableServer Skeleton Class for interface PortableServer::ServantActivator
POA_PortableServer::ServantActivator::~ServantActivator()
{
}

PortableServer::ServantActivator_ptr
POA_PortableServer::ServantActivator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return PortableServer::ServantActivator::_narrow (obj);
}

CORBA::Boolean
POA_PortableServer::ServantActivator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/PortableServer/ServantActivator:1.0") == 0) {
    return TRUE;
  }
  if (POA_PortableServer::ServantManager::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_PortableServer::ServantActivator::_get_interface ()
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/PortableServer/ServantActivator:1.0");
  CORBA::InterfaceDef_ptr ifd = CORBA::InterfaceDef::_narrow (cv);

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_PortableServer::ServantActivator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/PortableServer/ServantActivator:1.0");
}

void *
POA_PortableServer::ServantActivator::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/PortableServer/ServantActivator:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_PortableServer::ServantManager::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

POA_PortableServer::ServantActivator *
POA_PortableServer::ServantActivator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/PortableServer/ServantActivator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_PortableServer::ServantActivator *) p;
  }
  return NULL;
}

CORBA::Object_ptr
POA_PortableServer::ServantActivator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new PortableServer::ServantActivator_stub_clp (poa, obj);
}

bool
POA_PortableServer::ServantActivator::dispatch (CORBA::ServerRequest_ptr)
{
  return false;
}

void
POA_PortableServer::ServantActivator::invoke (CORBA::ServerRequest_ptr)
{
  assert (0);
}


// PortableServer Skeleton Class for interface PortableServer::ServantLocator
POA_PortableServer::ServantLocator::~ServantLocator()
{
}

PortableServer::ServantLocator_ptr
POA_PortableServer::ServantLocator::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return PortableServer::ServantLocator::_narrow (obj);
}

CORBA::Boolean
POA_PortableServer::ServantLocator::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/PortableServer/ServantLocator:1.0") == 0) {
    return TRUE;
  }
  if (POA_PortableServer::ServantManager::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_PortableServer::ServantLocator::_get_interface ()
{
  CORBA::ORB_var orb = CORBA::ORB_instance ("mico-local-orb");
  CORBA::Object_var irobj = 
    orb->resolve_initial_references ("InterfaceRepository");
  CORBA::Repository_var ifr = CORBA::Repository::_narrow (irobj);
  if (CORBA::is_nil (ifr)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  CORBA::Contained_var cv = ifr->lookup_id ("IDL:omg.org/PortableServer/ServantLocator:1.0");
  CORBA::InterfaceDef_ptr ifd = CORBA::InterfaceDef::_narrow (cv);

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_PortableServer::ServantLocator::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/PortableServer/ServantLocator:1.0");
}

void *
POA_PortableServer::ServantLocator::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/PortableServer/ServantLocator:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_PortableServer::ServantManager::_narrow_helper (repoid)) != NULL) {
    return p;
  }
  return NULL;
}

POA_PortableServer::ServantLocator *
POA_PortableServer::ServantLocator::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/PortableServer/ServantLocator:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_PortableServer::ServantLocator *) p;
  }
  return NULL;
}

CORBA::Object_ptr
POA_PortableServer::ServantLocator::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new PortableServer::ServantLocator_stub_clp (poa, obj);
}

bool
POA_PortableServer::ServantLocator::dispatch (CORBA::ServerRequest_ptr)
{
  return false;
}

void
POA_PortableServer::ServantLocator::invoke (CORBA::ServerRequest_ptr)
{
  assert (0);
}

// end-idl-hack

