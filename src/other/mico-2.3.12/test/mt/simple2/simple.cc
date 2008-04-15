/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include "simple.h"

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------

// Stub interface simple1
simple1::~simple1()
{
}

void *simple1::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:simple1:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

bool simple1::_narrow_helper2( CORBA::Object_ptr _obj )
{
  if( strcmp( _obj->_repoid(), "IDL:simple1:1.0" ) == 0) {
    return true;
  }
  for( vector<CORBA::Narrow_proto>::size_type _i = 0;
       _narrow_helpers && _i < _narrow_helpers->size(); _i++ ) {
    bool _res = (*(*_narrow_helpers)[ _i ])( _obj );
    if( _res )
      return true;
  }
  return false;
}

simple1_ptr simple1::_narrow( CORBA::Object_ptr _obj )
{
  simple1_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:simple1:1.0" )))
      return _duplicate( (simple1_ptr) _p );
    if( _narrow_helper2( _obj ) ||
       ( _obj->_is_a_remote( "IDL:simple1:1.0" ) ) ) {
      _o = new simple1_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

simple1_ptr
simple1::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

simple1_stub::~simple1_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_simple1::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:simple1:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_simple1 *
POA_simple1::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:simple1:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_simple1 *) p;
  }
  return NULL;
}

simple1_stub_clp::simple1_stub_clp ()
{
}

simple1_stub_clp::simple1_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : PortableServer::StubBase(poa), CORBA::Object(*obj)
{
}

simple1_stub_clp::~simple1_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void simple1_stub::func( CORBA::Char id )
{
  CORBA::StaticAny _id( CORBA::_stc_char, &id );
  CORBA::StaticRequest __req( this, "func" );
  __req.add_in_arg( &_id );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
simple1_stub_clp::func( CORBA::Char id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_simple1 * _myserv = POA_simple1::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->func(id);
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

  simple1_stub::func(id);
}

#endif // MICO_CONF_NO_POA

void simple1_stub::flush()
{
  CORBA::StaticRequest __req( this, "flush" );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
simple1_stub_clp::flush()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_simple1 * _myserv = POA_simple1::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->flush();
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

  simple1_stub::flush();
}

#endif // MICO_CONF_NO_POA

vector<CORBA::Narrow_proto> * simple1::_narrow_helpers;

class _Marshaller_simple1 : public CORBA::StaticTypeInfo {
    typedef simple1_ptr _MICO_T;
  public:
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    CORBA::Boolean demarshal (CORBA::DataDecoder&, StaticValueType) const;
    void marshal (CORBA::DataEncoder &, StaticValueType) const;
};


CORBA::StaticValueType _Marshaller_simple1::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_simple1::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::simple1::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_simple1::free( StaticValueType v ) const
{
  CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

CORBA::Boolean _Marshaller_simple1::demarshal( CORBA::DataDecoder &dc, StaticValueType v ) const
{
  CORBA::Object_ptr obj;
  if (!CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::simple1::_narrow( obj );
  CORBA::Boolean ret = CORBA::is_nil (obj) || !CORBA::is_nil (*(_MICO_T *)v);
  CORBA::release (obj);
  return ret;
}

void _Marshaller_simple1::marshal( CORBA::DataEncoder &ec, StaticValueType v ) const
{
  CORBA::Object_ptr obj = *(_MICO_T *) v;
  CORBA::_stc_Object->marshal( ec, &obj );
}

CORBA::StaticTypeInfo *_marshaller_simple1;


// Stub interface simple2
simple2::~simple2()
{
}

void *simple2::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:simple2:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

bool simple2::_narrow_helper2( CORBA::Object_ptr _obj )
{
  if( strcmp( _obj->_repoid(), "IDL:simple2:1.0" ) == 0) {
    return true;
  }
  for( vector<CORBA::Narrow_proto>::size_type _i = 0;
       _narrow_helpers && _i < _narrow_helpers->size(); _i++ ) {
    bool _res = (*(*_narrow_helpers)[ _i ])( _obj );
    if( _res )
      return true;
  }
  return false;
}

simple2_ptr simple2::_narrow( CORBA::Object_ptr _obj )
{
  simple2_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:simple2:1.0" )))
      return _duplicate( (simple2_ptr) _p );
    if( _narrow_helper2( _obj ) ||
       ( _obj->_is_a_remote( "IDL:simple2:1.0" ) ) ) {
      _o = new simple2_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

simple2_ptr
simple2::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

simple2_stub::~simple2_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_simple2::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:simple2:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_simple2 *
POA_simple2::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:simple2:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_simple2 *) p;
  }
  return NULL;
}

simple2_stub_clp::simple2_stub_clp ()
{
}

simple2_stub_clp::simple2_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : PortableServer::StubBase(poa), CORBA::Object(*obj)
{
}

simple2_stub_clp::~simple2_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void simple2_stub::func( CORBA::Char id )
{
  CORBA::StaticAny _id( CORBA::_stc_char, &id );
  CORBA::StaticRequest __req( this, "func" );
  __req.add_in_arg( &_id );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
simple2_stub_clp::func( CORBA::Char id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_simple2 * _myserv = POA_simple2::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->func(id);
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

  simple2_stub::func(id);
}

#endif // MICO_CONF_NO_POA

void simple2_stub::flush()
{
  CORBA::StaticRequest __req( this, "flush" );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
simple2_stub_clp::flush()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_simple2 * _myserv = POA_simple2::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->flush();
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

  simple2_stub::flush();
}

#endif // MICO_CONF_NO_POA

vector<CORBA::Narrow_proto> * simple2::_narrow_helpers;

class _Marshaller_simple2 : public CORBA::StaticTypeInfo {
    typedef simple2_ptr _MICO_T;
  public:
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    CORBA::Boolean demarshal (CORBA::DataDecoder&, StaticValueType) const;
    void marshal (CORBA::DataEncoder &, StaticValueType) const;
};


CORBA::StaticValueType _Marshaller_simple2::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_simple2::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::simple2::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_simple2::free( StaticValueType v ) const
{
  CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

CORBA::Boolean _Marshaller_simple2::demarshal( CORBA::DataDecoder &dc, StaticValueType v ) const
{
  CORBA::Object_ptr obj;
  if (!CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::simple2::_narrow( obj );
  CORBA::Boolean ret = CORBA::is_nil (obj) || !CORBA::is_nil (*(_MICO_T *)v);
  CORBA::release (obj);
  return ret;
}

void _Marshaller_simple2::marshal( CORBA::DataEncoder &ec, StaticValueType v ) const
{
  CORBA::Object_ptr obj = *(_MICO_T *) v;
  CORBA::_stc_Object->marshal( ec, &obj );
}

CORBA::StaticTypeInfo *_marshaller_simple2;

struct __tc_init_SIMPLE {
  __tc_init_SIMPLE()
  {
    _marshaller_simple1 = new _Marshaller_simple1;
    _marshaller_simple2 = new _Marshaller_simple2;
  }
};

static __tc_init_SIMPLE __init_SIMPLE;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

simple1_skel::simple1_skel( const CORBA::BOA::ReferenceData &_id )
{
  CORBA::ImplementationDef_var _impl =
    _find_impl( "IDL:simple1:1.0", "simple1" );
  assert( !CORBA::is_nil( _impl ) );
  _create_ref( _id,
    0,
    _impl,
    "IDL:simple1:1.0" );
  register_dispatcher( new StaticInterfaceDispatcherWrapper<simple1_skel>( this ) );
}

simple1_skel::simple1_skel( CORBA::Object_ptr _obj )
{
  CORBA::ImplementationDef_var _impl =
    _find_impl( "IDL:simple1:1.0", "simple1" );
  assert( !CORBA::is_nil( _impl ) );
  _restore_ref( _obj,
    CORBA::BOA::ReferenceData(),
    0,
    _impl );
  register_dispatcher( new StaticInterfaceDispatcherWrapper<simple1_skel>( this ) );
}

simple1_skel::~simple1_skel()
{
}

bool simple1_skel::dispatch( CORBA::StaticServerRequest_ptr _req, CORBA::Environment & /*_env*/ )
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( _req->op_name(), "func" ) == 0 ) {
      CORBA::Char id;
      CORBA::StaticAny _id( CORBA::_stc_char, &id );

      _req->add_in_arg( &_id );

      if( !_req->read_args() )
        return true;

      func( id );
      _req->write_results();
      return true;
    }
    if( strcmp( _req->op_name(), "flush" ) == 0 ) {

      if( !_req->read_args() )
        return true;

      flush();
      _req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    _req->set_exception( _ex->_clone() );
    _req->write_results();
    return true;
  } catch( ... ) {
    assert( 0 );
    return true;
  }
  #endif
  return false;
}

simple1_ptr simple1_skel::_this()
{
  return simple1::_duplicate( this );
}


// PortableServer Skeleton Class for interface simple1
POA_simple1::~POA_simple1()
{
}

simple1_ptr
POA_simple1::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return simple1::_narrow (obj);
}

CORBA::Boolean
POA_simple1::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:simple1:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_simple1::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:simple1:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_simple1::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:simple1:1.0");
}

CORBA::Object_ptr
POA_simple1::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new simple1_stub_clp (poa, obj);
}

bool
POA_simple1::dispatch (CORBA::StaticServerRequest_ptr _req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( _req->op_name(), "func" ) == 0 ) {
      CORBA::Char id;
      CORBA::StaticAny _id( CORBA::_stc_char, &id );

      _req->add_in_arg( &_id );

      if( !_req->read_args() )
        return true;

      func( id );
      _req->write_results();
      return true;
    }
    if( strcmp( _req->op_name(), "flush" ) == 0 ) {

      if( !_req->read_args() )
        return true;

      flush();
      _req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    _req->set_exception( _ex->_clone() );
    _req->write_results();
    return true;
  } catch( ... ) {
    assert( 0 );
    return true;
  }
  #endif

  return false;
}

void
POA_simple1::invoke (CORBA::StaticServerRequest_ptr _req)
{
  if (dispatch (_req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  _req->set_exception (ex);
  _req->write_results();
}


simple2_skel::simple2_skel( const CORBA::BOA::ReferenceData &_id )
{
  CORBA::ImplementationDef_var _impl =
    _find_impl( "IDL:simple2:1.0", "simple2" );
  assert( !CORBA::is_nil( _impl ) );
  _create_ref( _id,
    0,
    _impl,
    "IDL:simple2:1.0" );
  register_dispatcher( new StaticInterfaceDispatcherWrapper<simple2_skel>( this ) );
}

simple2_skel::simple2_skel( CORBA::Object_ptr _obj )
{
  CORBA::ImplementationDef_var _impl =
    _find_impl( "IDL:simple2:1.0", "simple2" );
  assert( !CORBA::is_nil( _impl ) );
  _restore_ref( _obj,
    CORBA::BOA::ReferenceData(),
    0,
    _impl );
  register_dispatcher( new StaticInterfaceDispatcherWrapper<simple2_skel>( this ) );
}

simple2_skel::~simple2_skel()
{
}

bool simple2_skel::dispatch( CORBA::StaticServerRequest_ptr _req, CORBA::Environment & /*_env*/ )
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( _req->op_name(), "func" ) == 0 ) {
      CORBA::Char id;
      CORBA::StaticAny _id( CORBA::_stc_char, &id );

      _req->add_in_arg( &_id );

      if( !_req->read_args() )
        return true;

      func( id );
      _req->write_results();
      return true;
    }
    if( strcmp( _req->op_name(), "flush" ) == 0 ) {

      if( !_req->read_args() )
        return true;

      flush();
      _req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    _req->set_exception( _ex->_clone() );
    _req->write_results();
    return true;
  } catch( ... ) {
    assert( 0 );
    return true;
  }
  #endif
  return false;
}

simple2_ptr simple2_skel::_this()
{
  return simple2::_duplicate( this );
}


// PortableServer Skeleton Class for interface simple2
POA_simple2::~POA_simple2()
{
}

simple2_ptr
POA_simple2::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return simple2::_narrow (obj);
}

CORBA::Boolean
POA_simple2::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:simple2:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_simple2::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:simple2:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_simple2::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:simple2:1.0");
}

CORBA::Object_ptr
POA_simple2::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new simple2_stub_clp (poa, obj);
}

bool
POA_simple2::dispatch (CORBA::StaticServerRequest_ptr _req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( _req->op_name(), "func" ) == 0 ) {
      CORBA::Char id;
      CORBA::StaticAny _id( CORBA::_stc_char, &id );

      _req->add_in_arg( &_id );

      if( !_req->read_args() )
        return true;

      func( id );
      _req->write_results();
      return true;
    }
    if( strcmp( _req->op_name(), "flush" ) == 0 ) {

      if( !_req->read_args() )
        return true;

      flush();
      _req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    _req->set_exception( _ex->_clone() );
    _req->write_results();
    return true;
  } catch( ... ) {
    assert( 0 );
    return true;
  }
  #endif

  return false;
}

void
POA_simple2::invoke (CORBA::StaticServerRequest_ptr _req)
{
  if (dispatch (_req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  _req->set_exception (ex);
  _req->write_results();
}

