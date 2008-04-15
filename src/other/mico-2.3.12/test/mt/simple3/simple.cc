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

// Stub interface simple
simple::~simple()
{
}

void *simple::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:simple:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

bool simple::_narrow_helper2( CORBA::Object_ptr _obj )
{
  if( strcmp( _obj->_repoid(), "IDL:simple:1.0" ) == 0) {
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

simple_ptr simple::_narrow( CORBA::Object_ptr _obj )
{
  simple_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:simple:1.0" )))
      return _duplicate( (simple_ptr) _p );
    if( _narrow_helper2( _obj ) ||
       ( _obj->_is_a_remote( "IDL:simple:1.0" ) ) ) {
      _o = new simple_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

simple_ptr
simple::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

simple_stub::~simple_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_simple::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:simple:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_simple *
POA_simple::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:simple:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_simple *) p;
  }
  return NULL;
}

simple_stub_clp::simple_stub_clp ()
{
}

simple_stub_clp::simple_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : PortableServer::StubBase(poa), CORBA::Object(*obj)
{
}

simple_stub_clp::~simple_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

void simple_stub::func( CORBA::Char id )
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
simple_stub_clp::func( CORBA::Char id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_simple * _myserv = POA_simple::_narrow (_serv);
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

  simple_stub::func(id);
}

#endif // MICO_CONF_NO_POA

void simple_stub::flush()
{
  CORBA::StaticRequest __req( this, "flush" );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
simple_stub_clp::flush()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_simple * _myserv = POA_simple::_narrow (_serv);
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

  simple_stub::flush();
}

#endif // MICO_CONF_NO_POA

vector<CORBA::Narrow_proto> * simple::_narrow_helpers;

class _Marshaller_simple : public CORBA::StaticTypeInfo {
    typedef simple_ptr _MICO_T;
  public:
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    CORBA::Boolean demarshal (CORBA::DataDecoder&, StaticValueType) const;
    void marshal (CORBA::DataEncoder &, StaticValueType) const;
};


CORBA::StaticValueType _Marshaller_simple::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_simple::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::simple::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_simple::free( StaticValueType v ) const
{
  CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

CORBA::Boolean _Marshaller_simple::demarshal( CORBA::DataDecoder &dc, StaticValueType v ) const
{
  CORBA::Object_ptr obj;
  if (!CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::simple::_narrow( obj );
  CORBA::Boolean ret = CORBA::is_nil (obj) || !CORBA::is_nil (*(_MICO_T *)v);
  CORBA::release (obj);
  return ret;
}

void _Marshaller_simple::marshal( CORBA::DataEncoder &ec, StaticValueType v ) const
{
  CORBA::Object_ptr obj = *(_MICO_T *) v;
  CORBA::_stc_Object->marshal( ec, &obj );
}

CORBA::StaticTypeInfo *_marshaller_simple;

struct __tc_init_SIMPLE {
  __tc_init_SIMPLE()
  {
    _marshaller_simple = new _Marshaller_simple;
  }
};

static __tc_init_SIMPLE __init_SIMPLE;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

simple_skel::simple_skel( const CORBA::BOA::ReferenceData &_id )
{
  CORBA::ImplementationDef_var _impl =
    _find_impl( "IDL:simple:1.0", "simple" );
  assert( !CORBA::is_nil( _impl ) );
  _create_ref( _id,
    0,
    _impl,
    "IDL:simple:1.0" );
  register_dispatcher( new StaticInterfaceDispatcherWrapper<simple_skel>( this ) );
}

simple_skel::simple_skel( CORBA::Object_ptr _obj )
{
  CORBA::ImplementationDef_var _impl =
    _find_impl( "IDL:simple:1.0", "simple" );
  assert( !CORBA::is_nil( _impl ) );
  _restore_ref( _obj,
    CORBA::BOA::ReferenceData(),
    0,
    _impl );
  register_dispatcher( new StaticInterfaceDispatcherWrapper<simple_skel>( this ) );
}

simple_skel::~simple_skel()
{
}

bool simple_skel::dispatch( CORBA::StaticServerRequest_ptr _req, CORBA::Environment & /*_env*/ )
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

simple_ptr simple_skel::_this()
{
  return simple::_duplicate( this );
}


// PortableServer Skeleton Class for interface simple
POA_simple::~POA_simple()
{
}

simple_ptr
POA_simple::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return simple::_narrow (obj);
}

CORBA::Boolean
POA_simple::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:simple:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_simple::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:simple:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_simple::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:simple:1.0");
}

CORBA::Object_ptr
POA_simple::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new simple_stub_clp (poa, obj);
}

bool
POA_simple::dispatch (CORBA::StaticServerRequest_ptr _req)
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
POA_simple::invoke (CORBA::StaticServerRequest_ptr _req)
{
  if (dispatch (_req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  _req->set_exception (ex);
  _req->write_results();
}

