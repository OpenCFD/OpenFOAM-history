/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <mico/ir3.h>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------

/*
 * Base interface for class ComponentDef
 */

CORBA::ComponentIR::ComponentDef::~ComponentDef()
{
}

void *
CORBA::ComponentIR::ComponentDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ExtInterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::ComponentDef_ptr
CORBA::ComponentIR::ComponentDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::ComponentDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::ComponentDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0")) {
      _o = new CORBA::ComponentIR::ComponentDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::ComponentDef_ptr
CORBA::ComponentIR::ComponentDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ComponentDef;
}
}
class _Marshaller_CORBA_ComponentIR_ComponentDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::ComponentDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_ComponentDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_ComponentDef::~_Marshaller_CORBA_ComponentIR_ComponentDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_ComponentDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_ComponentDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::ComponentDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_ComponentDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_ComponentDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_ComponentDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::ComponentDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_ComponentDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_ComponentDef::typecode()
{
  return CORBA::ComponentIR::_tc_ComponentDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ComponentDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::ComponentDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ComponentDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::ComponentDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ComponentDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::ComponentDef_ptr &_obj )
{
  CORBA::ComponentIR::ComponentDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_ComponentDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ComponentDef
 */

CORBA::ComponentIR::ComponentDef_stub::~ComponentDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::ComponentDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ExtInterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::ComponentDef *
POA_CORBA::ComponentIR::ComponentDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::ComponentDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::ComponentDef_stub_clp::ComponentDef_stub_clp ()
{
}

CORBA::ComponentIR::ComponentDef_stub_clp::ComponentDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::ComponentDef_stub_clp::~ComponentDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::ComponentDef_ptr CORBA::ComponentIR::ComponentDef_stub::base_component()
{
  CORBA::ComponentIR::ComponentDef_ptr _res = CORBA::ComponentIR::ComponentDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ComponentDef, &_res );

  CORBA::StaticRequest __req( this, "_get_base_component" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::ComponentDef_ptr
CORBA::ComponentIR::ComponentDef_stub_clp::base_component()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::ComponentDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->base_component();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::base_component();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::ComponentDef_stub::base_component( CORBA::ComponentIR::ComponentDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_ComponentDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_base_component" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::ComponentDef_stub_clp::base_component( CORBA::ComponentIR::ComponentDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->base_component(_par__value);
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

  CORBA::ComponentIR::ComponentDef_stub::base_component(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDefSeq* CORBA::ComponentIR::ComponentDef_stub::supported_interfaces()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_InterfaceDef );

  CORBA::StaticRequest __req( this, "_get_supported_interfaces" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::InterfaceDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDefSeq*
CORBA::ComponentIR::ComponentDef_stub_clp::supported_interfaces()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->supported_interfaces();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::supported_interfaces();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::ComponentDef_stub::supported_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_InterfaceDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_supported_interfaces" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::ComponentDef_stub_clp::supported_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->supported_interfaces(_par__value);
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

  CORBA::ComponentIR::ComponentDef_stub::supported_interfaces(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::ProvidesDef_ptr CORBA::ComponentIR::ComponentDef_stub::create_provides( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::InterfaceDef_ptr _par_interface_type )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_interface_type( _marshaller_CORBA_InterfaceDef, &_par_interface_type );
  CORBA::ComponentIR::ProvidesDef_ptr _res = CORBA::ComponentIR::ProvidesDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ProvidesDef, &_res );

  CORBA::StaticRequest __req( this, "create_provides" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_interface_type );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::ProvidesDef_ptr
CORBA::ComponentIR::ComponentDef_stub_clp::create_provides( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::InterfaceDef_ptr _par_interface_type )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::ProvidesDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_provides(_par_id, _par_name, _par_version, _par_interface_type);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::create_provides(_par_id, _par_name, _par_version, _par_interface_type);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::UsesDef_ptr CORBA::ComponentIR::ComponentDef_stub::create_uses( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::InterfaceDef_ptr _par_interface_type, CORBA::Boolean _par_is_multiple )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_interface_type( _marshaller_CORBA_InterfaceDef, &_par_interface_type );
  CORBA::StaticAny _sa_is_multiple( CORBA::_stc_boolean, &_par_is_multiple );
  CORBA::ComponentIR::UsesDef_ptr _res = CORBA::ComponentIR::UsesDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_UsesDef, &_res );

  CORBA::StaticRequest __req( this, "create_uses" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_interface_type );
  __req.add_in_arg( &_sa_is_multiple );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::UsesDef_ptr
CORBA::ComponentIR::ComponentDef_stub_clp::create_uses( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::InterfaceDef_ptr _par_interface_type, CORBA::Boolean _par_is_multiple )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::UsesDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_uses(_par_id, _par_name, _par_version, _par_interface_type, _par_is_multiple);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::create_uses(_par_id, _par_name, _par_version, _par_interface_type, _par_is_multiple);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::EmitsDef_ptr CORBA::ComponentIR::ComponentDef_stub::create_emits( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::EventDef_ptr _par_event )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_event( _marshaller_CORBA_ComponentIR_EventDef, &_par_event );
  CORBA::ComponentIR::EmitsDef_ptr _res = CORBA::ComponentIR::EmitsDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_EmitsDef, &_res );

  CORBA::StaticRequest __req( this, "create_emits" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_event );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::EmitsDef_ptr
CORBA::ComponentIR::ComponentDef_stub_clp::create_emits( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::EventDef_ptr _par_event )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::EmitsDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_emits(_par_id, _par_name, _par_version, _par_event);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::create_emits(_par_id, _par_name, _par_version, _par_event);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::PublishesDef_ptr CORBA::ComponentIR::ComponentDef_stub::create_publishes( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::EventDef_ptr _par_event )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_event( _marshaller_CORBA_ComponentIR_EventDef, &_par_event );
  CORBA::ComponentIR::PublishesDef_ptr _res = CORBA::ComponentIR::PublishesDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_PublishesDef, &_res );

  CORBA::StaticRequest __req( this, "create_publishes" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_event );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::PublishesDef_ptr
CORBA::ComponentIR::ComponentDef_stub_clp::create_publishes( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::EventDef_ptr _par_event )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::PublishesDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_publishes(_par_id, _par_name, _par_version, _par_event);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::create_publishes(_par_id, _par_name, _par_version, _par_event);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::ConsumesDef_ptr CORBA::ComponentIR::ComponentDef_stub::create_consumes( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::EventDef_ptr _par_event )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_event( _marshaller_CORBA_ComponentIR_EventDef, &_par_event );
  CORBA::ComponentIR::ConsumesDef_ptr _res = CORBA::ComponentIR::ConsumesDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ConsumesDef, &_res );

  CORBA::StaticRequest __req( this, "create_consumes" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_event );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::ConsumesDef_ptr
CORBA::ComponentIR::ComponentDef_stub_clp::create_consumes( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::EventDef_ptr _par_event )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ComponentDef * _myserv = POA_CORBA::ComponentIR::ComponentDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::ConsumesDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_consumes(_par_id, _par_name, _par_version, _par_event);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ComponentDef_stub::create_consumes(_par_id, _par_name, _par_version, _par_event);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class HomeDef
 */

CORBA::ComponentIR::HomeDef::~HomeDef()
{
}

void *
CORBA::ComponentIR::HomeDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ExtInterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::HomeDef_ptr
CORBA::ComponentIR::HomeDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::HomeDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::HomeDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0")) {
      _o = new CORBA::ComponentIR::HomeDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::HomeDef_ptr
CORBA::ComponentIR::HomeDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_HomeDef;
}
}
class _Marshaller_CORBA_ComponentIR_HomeDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::HomeDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_HomeDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_HomeDef::~_Marshaller_CORBA_ComponentIR_HomeDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_HomeDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_HomeDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::HomeDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_HomeDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_HomeDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_HomeDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::HomeDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_HomeDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_HomeDef::typecode()
{
  return CORBA::ComponentIR::_tc_HomeDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_HomeDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::HomeDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_HomeDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::HomeDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_HomeDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::HomeDef_ptr &_obj )
{
  CORBA::ComponentIR::HomeDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_HomeDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class HomeDef
 */

CORBA::ComponentIR::HomeDef_stub::~HomeDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::HomeDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ExtInterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::HomeDef *
POA_CORBA::ComponentIR::HomeDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::HomeDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::HomeDef_stub_clp::HomeDef_stub_clp ()
{
}

CORBA::ComponentIR::HomeDef_stub_clp::HomeDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::HomeDef_stub_clp::~HomeDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::HomeDef_ptr CORBA::ComponentIR::HomeDef_stub::base_home()
{
  CORBA::ComponentIR::HomeDef_ptr _res = CORBA::ComponentIR::HomeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_HomeDef, &_res );

  CORBA::StaticRequest __req( this, "_get_base_home" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::HomeDef_ptr
CORBA::ComponentIR::HomeDef_stub_clp::base_home()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::HomeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->base_home();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::HomeDef_stub::base_home();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::HomeDef_stub::base_home( CORBA::ComponentIR::HomeDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_HomeDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_base_home" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::HomeDef_stub_clp::base_home( CORBA::ComponentIR::HomeDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->base_home(_par__value);
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

  CORBA::ComponentIR::HomeDef_stub::base_home(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDefSeq* CORBA::ComponentIR::HomeDef_stub::supported_interfaces()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_InterfaceDef );

  CORBA::StaticRequest __req( this, "_get_supported_interfaces" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::InterfaceDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDefSeq*
CORBA::ComponentIR::HomeDef_stub_clp::supported_interfaces()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->supported_interfaces();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::HomeDef_stub::supported_interfaces();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::HomeDef_stub::supported_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_InterfaceDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_supported_interfaces" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::HomeDef_stub_clp::supported_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->supported_interfaces(_par__value);
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

  CORBA::ComponentIR::HomeDef_stub::supported_interfaces(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::ComponentDef_ptr CORBA::ComponentIR::HomeDef_stub::managed_component()
{
  CORBA::ComponentIR::ComponentDef_ptr _res = CORBA::ComponentIR::ComponentDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ComponentDef, &_res );

  CORBA::StaticRequest __req( this, "_get_managed_component" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::ComponentDef_ptr
CORBA::ComponentIR::HomeDef_stub_clp::managed_component()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::ComponentDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->managed_component();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::HomeDef_stub::managed_component();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::HomeDef_stub::managed_component( CORBA::ComponentIR::ComponentDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_ComponentDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_managed_component" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::HomeDef_stub_clp::managed_component( CORBA::ComponentIR::ComponentDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->managed_component(_par__value);
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

  CORBA::ComponentIR::HomeDef_stub::managed_component(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ValueDef_ptr CORBA::ComponentIR::HomeDef_stub::primary_key()
{
  CORBA::ValueDef_ptr _res = CORBA::ValueDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ValueDef, &_res );

  CORBA::StaticRequest __req( this, "_get_primary_key" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ValueDef_ptr
CORBA::ComponentIR::HomeDef_stub_clp::primary_key()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->primary_key();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::HomeDef_stub::primary_key();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::HomeDef_stub::primary_key( CORBA::ValueDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ValueDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_primary_key" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::HomeDef_stub_clp::primary_key( CORBA::ValueDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->primary_key(_par__value);
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

  CORBA::ComponentIR::HomeDef_stub::primary_key(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::FactoryDef_ptr CORBA::ComponentIR::HomeDef_stub::create_factory( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
  CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );
  CORBA::ComponentIR::FactoryDef_ptr _res = CORBA::ComponentIR::FactoryDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_FactoryDef, &_res );

  CORBA::StaticRequest __req( this, "create_factory" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_params );
  __req.add_in_arg( &_sa_exceptions );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::FactoryDef_ptr
CORBA::ComponentIR::HomeDef_stub_clp::create_factory( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::FactoryDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_factory(_par_id, _par_name, _par_version, _par_params, _par_exceptions);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::HomeDef_stub::create_factory(_par_id, _par_name, _par_version, _par_params, _par_exceptions);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::FinderDef_ptr CORBA::ComponentIR::HomeDef_stub::create_finder( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
  CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );
  CORBA::ComponentIR::FinderDef_ptr _res = CORBA::ComponentIR::FinderDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_FinderDef, &_res );

  CORBA::StaticRequest __req( this, "create_finder" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_params );
  __req.add_in_arg( &_sa_exceptions );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::FinderDef_ptr
CORBA::ComponentIR::HomeDef_stub_clp::create_finder( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::HomeDef * _myserv = POA_CORBA::ComponentIR::HomeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::FinderDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_finder(_par_id, _par_name, _par_version, _par_params, _par_exceptions);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::HomeDef_stub::create_finder(_par_id, _par_name, _par_version, _par_params, _par_exceptions);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class EventDef
 */

CORBA::ComponentIR::EventDef::~EventDef()
{
}

void *
CORBA::ComponentIR::EventDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/EventDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ExtValueDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::EventDef_ptr
CORBA::ComponentIR::EventDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::EventDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/EventDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::EventDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/EventDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/EventDef:1.0")) {
      _o = new CORBA::ComponentIR::EventDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::EventDef_ptr
CORBA::ComponentIR::EventDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_EventDef;
}
}
class _Marshaller_CORBA_ComponentIR_EventDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::EventDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_EventDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_EventDef::~_Marshaller_CORBA_ComponentIR_EventDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_EventDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_EventDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::EventDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_EventDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_EventDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_EventDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::EventDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_EventDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_EventDef::typecode()
{
  return CORBA::ComponentIR::_tc_EventDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EventDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::EventDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EventDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::EventDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EventDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::EventDef_ptr &_obj )
{
  CORBA::ComponentIR::EventDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_EventDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class EventDef
 */

CORBA::ComponentIR::EventDef_stub::~EventDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::EventDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/EventDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ExtValueDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::EventDef *
POA_CORBA::ComponentIR::EventDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/EventDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::EventDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::EventDef_stub_clp::EventDef_stub_clp ()
{
}

CORBA::ComponentIR::EventDef_stub_clp::EventDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::EventDef_stub_clp::~EventDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class Container
 */

CORBA::ComponentIR::Container::~Container()
{
}

void *
CORBA::ComponentIR::Container::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/Container:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::Container_ptr
CORBA::ComponentIR::Container::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::Container_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/Container:1.0" )))
      return _duplicate( (CORBA::ComponentIR::Container_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/Container:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/Container:1.0")) {
      _o = new CORBA::ComponentIR::Container_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::Container_ptr
CORBA::ComponentIR::Container::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_Container;
}
}
class _Marshaller_CORBA_ComponentIR_Container : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::Container_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_Container();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_Container::~_Marshaller_CORBA_ComponentIR_Container()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_Container::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_Container::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::Container::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_Container::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_Container::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_Container::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::Container::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_Container::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_Container::typecode()
{
  return CORBA::ComponentIR::_tc_Container;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_Container;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::Container_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_Container, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::Container_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_Container, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::Container_ptr &_obj )
{
  CORBA::ComponentIR::Container_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_Container, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Container
 */

CORBA::ComponentIR::Container_stub::~Container_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::Container::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/Container:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::Container *
POA_CORBA::ComponentIR::Container::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/Container:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::Container *) p;
  }
  return NULL;
}

CORBA::ComponentIR::Container_stub_clp::Container_stub_clp ()
{
}

CORBA::ComponentIR::Container_stub_clp::Container_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::Container_stub_clp::~Container_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::ComponentDef_ptr CORBA::ComponentIR::Container_stub::create_component( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::ComponentDef_ptr _par_base_component, const CORBA::InterfaceDefSeq& _par_supports_interfaces )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_base_component( _marshaller_CORBA_ComponentIR_ComponentDef, &_par_base_component );
  CORBA::StaticAny _sa_supports_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_supports_interfaces );
  CORBA::ComponentIR::ComponentDef_ptr _res = CORBA::ComponentIR::ComponentDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ComponentDef, &_res );

  CORBA::StaticRequest __req( this, "create_component" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_base_component );
  __req.add_in_arg( &_sa_supports_interfaces );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::ComponentDef_ptr
CORBA::ComponentIR::Container_stub_clp::create_component( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::ComponentDef_ptr _par_base_component, const CORBA::InterfaceDefSeq& _par_supports_interfaces )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::Container * _myserv = POA_CORBA::ComponentIR::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::ComponentDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_component(_par_id, _par_name, _par_version, _par_base_component, _par_supports_interfaces);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::Container_stub::create_component(_par_id, _par_name, _par_version, _par_base_component, _par_supports_interfaces);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::HomeDef_ptr CORBA::ComponentIR::Container_stub::create_home( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::HomeDef_ptr _par_base_home, CORBA::ComponentIR::ComponentDef_ptr _par_managed_component, const CORBA::InterfaceDefSeq& _par_supports_interfaces, CORBA::ValueDef_ptr _par_primary_key )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_base_home( _marshaller_CORBA_ComponentIR_HomeDef, &_par_base_home );
  CORBA::StaticAny _sa_managed_component( _marshaller_CORBA_ComponentIR_ComponentDef, &_par_managed_component );
  CORBA::StaticAny _sa_supports_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_supports_interfaces );
  CORBA::StaticAny _sa_primary_key( _marshaller_CORBA_ValueDef, &_par_primary_key );
  CORBA::ComponentIR::HomeDef_ptr _res = CORBA::ComponentIR::HomeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_HomeDef, &_res );

  CORBA::StaticRequest __req( this, "create_home" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_base_home );
  __req.add_in_arg( &_sa_managed_component );
  __req.add_in_arg( &_sa_supports_interfaces );
  __req.add_in_arg( &_sa_primary_key );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::HomeDef_ptr
CORBA::ComponentIR::Container_stub_clp::create_home( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::ComponentIR::HomeDef_ptr _par_base_home, CORBA::ComponentIR::ComponentDef_ptr _par_managed_component, const CORBA::InterfaceDefSeq& _par_supports_interfaces, CORBA::ValueDef_ptr _par_primary_key )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::Container * _myserv = POA_CORBA::ComponentIR::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::HomeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_home(_par_id, _par_name, _par_version, _par_base_home, _par_managed_component, _par_supports_interfaces, _par_primary_key);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::Container_stub::create_home(_par_id, _par_name, _par_version, _par_base_home, _par_managed_component, _par_supports_interfaces, _par_primary_key);
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::EventDef_ptr CORBA::ComponentIR::Container_stub::create_event( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::Boolean _par_is_custom, CORBA::Boolean _par_is_abstract, CORBA::ValueDef_ptr _par_base_value, CORBA::Boolean _par_is_truncatable, const CORBA::ValueDefSeq& _par_abstract_base_values, const CORBA::InterfaceDefSeq& _par_supported_interfaces, const CORBA::ExtInitializerSeq& _par_initializers )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_is_custom( CORBA::_stc_boolean, &_par_is_custom );
  CORBA::StaticAny _sa_is_abstract( CORBA::_stc_boolean, &_par_is_abstract );
  CORBA::StaticAny _sa_base_value( _marshaller_CORBA_ValueDef, &_par_base_value );
  CORBA::StaticAny _sa_is_truncatable( CORBA::_stc_boolean, &_par_is_truncatable );
  CORBA::StaticAny _sa_abstract_base_values( _marshaller__seq_CORBA_ValueDef, &_par_abstract_base_values );
  CORBA::StaticAny _sa_supported_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_supported_interfaces );
  CORBA::StaticAny _sa_initializers( _marshaller__seq_CORBA_ExtInitializer, &_par_initializers );
  CORBA::ComponentIR::EventDef_ptr _res = CORBA::ComponentIR::EventDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_EventDef, &_res );

  CORBA::StaticRequest __req( this, "create_event" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_is_custom );
  __req.add_in_arg( &_sa_is_abstract );
  __req.add_in_arg( &_sa_base_value );
  __req.add_in_arg( &_sa_is_truncatable );
  __req.add_in_arg( &_sa_abstract_base_values );
  __req.add_in_arg( &_sa_supported_interfaces );
  __req.add_in_arg( &_sa_initializers );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::EventDef_ptr
CORBA::ComponentIR::Container_stub_clp::create_event( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::Boolean _par_is_custom, CORBA::Boolean _par_is_abstract, CORBA::ValueDef_ptr _par_base_value, CORBA::Boolean _par_is_truncatable, const CORBA::ValueDefSeq& _par_abstract_base_values, const CORBA::InterfaceDefSeq& _par_supported_interfaces, const CORBA::ExtInitializerSeq& _par_initializers )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::Container * _myserv = POA_CORBA::ComponentIR::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::EventDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_event(_par_id, _par_name, _par_version, _par_is_custom, _par_is_abstract, _par_base_value, _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::Container_stub::create_event(_par_id, _par_name, _par_version, _par_is_custom, _par_is_abstract, _par_base_value, _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ModuleDef
 */

CORBA::ComponentIR::ModuleDef::~ModuleDef()
{
}

void *
CORBA::ComponentIR::ModuleDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ComponentIR::Container::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::ModuleDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::ModuleDef_ptr
CORBA::ComponentIR::ModuleDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::ModuleDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::ModuleDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0")) {
      _o = new CORBA::ComponentIR::ModuleDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::ModuleDef_ptr
CORBA::ComponentIR::ModuleDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ModuleDef;
}
}
class _Marshaller_CORBA_ComponentIR_ModuleDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::ModuleDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_ModuleDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_ModuleDef::~_Marshaller_CORBA_ComponentIR_ModuleDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_ModuleDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_ModuleDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::ModuleDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_ModuleDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_ModuleDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_ModuleDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::ModuleDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_ModuleDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_ModuleDef::typecode()
{
  return CORBA::ComponentIR::_tc_ModuleDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ModuleDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::ModuleDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ModuleDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::ModuleDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ModuleDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::ModuleDef_ptr &_obj )
{
  CORBA::ComponentIR::ModuleDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_ModuleDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ModuleDef
 */

CORBA::ComponentIR::ModuleDef_stub::~ModuleDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::ModuleDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ComponentIR::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::ModuleDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::ModuleDef *
POA_CORBA::ComponentIR::ModuleDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::ModuleDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::ModuleDef_stub_clp::ModuleDef_stub_clp ()
{
}

CORBA::ComponentIR::ModuleDef_stub_clp::ModuleDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::ModuleDef_stub_clp::~ModuleDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class Repository
 */

CORBA::ComponentIR::Repository::~Repository()
{
}

void *
CORBA::ComponentIR::Repository::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/Repository:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ComponentIR::Container::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Repository::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::Repository_ptr
CORBA::ComponentIR::Repository::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::Repository_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/Repository:1.0" )))
      return _duplicate( (CORBA::ComponentIR::Repository_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/Repository:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/Repository:1.0")) {
      _o = new CORBA::ComponentIR::Repository_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::Repository_ptr
CORBA::ComponentIR::Repository::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_Repository;
}
}
class _Marshaller_CORBA_ComponentIR_Repository : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::Repository_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_Repository();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_Repository::~_Marshaller_CORBA_ComponentIR_Repository()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_Repository::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_Repository::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::Repository::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_Repository::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_Repository::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_Repository::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::Repository::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_Repository::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_Repository::typecode()
{
  return CORBA::ComponentIR::_tc_Repository;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_Repository;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::Repository_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_Repository, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::Repository_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_Repository, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::Repository_ptr &_obj )
{
  CORBA::ComponentIR::Repository_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_Repository, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Repository
 */

CORBA::ComponentIR::Repository_stub::~Repository_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::Repository::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/Repository:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ComponentIR::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Repository::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::Repository *
POA_CORBA::ComponentIR::Repository::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/Repository:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::Repository *) p;
  }
  return NULL;
}

CORBA::ComponentIR::Repository_stub_clp::Repository_stub_clp ()
{
}

CORBA::ComponentIR::Repository_stub_clp::Repository_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::Repository_stub_clp::~Repository_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ProvidesDef
 */

CORBA::ComponentIR::ProvidesDef::~ProvidesDef()
{
}

void *
CORBA::ComponentIR::ProvidesDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::ProvidesDef_ptr
CORBA::ComponentIR::ProvidesDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::ProvidesDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::ProvidesDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0")) {
      _o = new CORBA::ComponentIR::ProvidesDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::ProvidesDef_ptr
CORBA::ComponentIR::ProvidesDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ProvidesDef;
}
}
class _Marshaller_CORBA_ComponentIR_ProvidesDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::ProvidesDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_ProvidesDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_ProvidesDef::~_Marshaller_CORBA_ComponentIR_ProvidesDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_ProvidesDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_ProvidesDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::ProvidesDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_ProvidesDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_ProvidesDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_ProvidesDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::ProvidesDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_ProvidesDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_ProvidesDef::typecode()
{
  return CORBA::ComponentIR::_tc_ProvidesDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ProvidesDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::ProvidesDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ProvidesDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::ProvidesDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ProvidesDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::ProvidesDef_ptr &_obj )
{
  CORBA::ComponentIR::ProvidesDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_ProvidesDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ProvidesDef
 */

CORBA::ComponentIR::ProvidesDef_stub::~ProvidesDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::ProvidesDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::ProvidesDef *
POA_CORBA::ComponentIR::ProvidesDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::ProvidesDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::ProvidesDef_stub_clp::ProvidesDef_stub_clp ()
{
}

CORBA::ComponentIR::ProvidesDef_stub_clp::ProvidesDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::ProvidesDef_stub_clp::~ProvidesDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDef_ptr CORBA::ComponentIR::ProvidesDef_stub::interface_type()
{
  CORBA::InterfaceDef_ptr _res = CORBA::InterfaceDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef, &_res );

  CORBA::StaticRequest __req( this, "_get_interface_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDef_ptr
CORBA::ComponentIR::ProvidesDef_stub_clp::interface_type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ProvidesDef * _myserv = POA_CORBA::ComponentIR::ProvidesDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->interface_type();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::ProvidesDef_stub::interface_type();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::ProvidesDef_stub::interface_type( CORBA::InterfaceDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_InterfaceDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_interface_type" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::ProvidesDef_stub_clp::interface_type( CORBA::InterfaceDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::ProvidesDef * _myserv = POA_CORBA::ComponentIR::ProvidesDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->interface_type(_par__value);
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

  CORBA::ComponentIR::ProvidesDef_stub::interface_type(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ProvidesDescription;
}
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ComponentIR::ProvidesDescription::ProvidesDescription()
{
}

CORBA::ComponentIR::ProvidesDescription::ProvidesDescription( const ProvidesDescription& _s )
{
  name = ((ProvidesDescription&)_s).name;
  id = ((ProvidesDescription&)_s).id;
  defined_in = ((ProvidesDescription&)_s).defined_in;
  version = ((ProvidesDescription&)_s).version;
  interface_type = ((ProvidesDescription&)_s).interface_type;
}

CORBA::ComponentIR::ProvidesDescription::~ProvidesDescription()
{
}

CORBA::ComponentIR::ProvidesDescription&
CORBA::ComponentIR::ProvidesDescription::operator=( const ProvidesDescription& _s )
{
  name = ((ProvidesDescription&)_s).name;
  id = ((ProvidesDescription&)_s).id;
  defined_in = ((ProvidesDescription&)_s).defined_in;
  version = ((ProvidesDescription&)_s).version;
  interface_type = ((ProvidesDescription&)_s).interface_type;
  return *this;
}
#endif

class _Marshaller_CORBA_ComponentIR_ProvidesDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::ProvidesDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_ProvidesDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_ProvidesDescription::~_Marshaller_CORBA_ComponentIR_ProvidesDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_ProvidesDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ComponentIR_ProvidesDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ComponentIR_ProvidesDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_ProvidesDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->interface_type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ComponentIR_ProvidesDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->interface_type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_ProvidesDescription::typecode()
{
  return CORBA::ComponentIR::_tc_ProvidesDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ProvidesDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::ProvidesDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ProvidesDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ComponentIR::ProvidesDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::ProvidesDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ProvidesDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ComponentIR::ProvidesDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ComponentIR_ProvidesDescription, (void *&)_s);
}


/*
 * Base interface for class UsesDef
 */

CORBA::ComponentIR::UsesDef::~UsesDef()
{
}

void *
CORBA::ComponentIR::UsesDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::UsesDef_ptr
CORBA::ComponentIR::UsesDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::UsesDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::UsesDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0")) {
      _o = new CORBA::ComponentIR::UsesDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::UsesDef_ptr
CORBA::ComponentIR::UsesDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_UsesDef;
}
}
class _Marshaller_CORBA_ComponentIR_UsesDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::UsesDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_UsesDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_UsesDef::~_Marshaller_CORBA_ComponentIR_UsesDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_UsesDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_UsesDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::UsesDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_UsesDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_UsesDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_UsesDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::UsesDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_UsesDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_UsesDef::typecode()
{
  return CORBA::ComponentIR::_tc_UsesDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_UsesDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::UsesDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_UsesDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::UsesDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_UsesDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::UsesDef_ptr &_obj )
{
  CORBA::ComponentIR::UsesDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_UsesDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class UsesDef
 */

CORBA::ComponentIR::UsesDef_stub::~UsesDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::UsesDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::UsesDef *
POA_CORBA::ComponentIR::UsesDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::UsesDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::UsesDef_stub_clp::UsesDef_stub_clp ()
{
}

CORBA::ComponentIR::UsesDef_stub_clp::UsesDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::UsesDef_stub_clp::~UsesDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDef_ptr CORBA::ComponentIR::UsesDef_stub::interface_type()
{
  CORBA::InterfaceDef_ptr _res = CORBA::InterfaceDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef, &_res );

  CORBA::StaticRequest __req( this, "_get_interface_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDef_ptr
CORBA::ComponentIR::UsesDef_stub_clp::interface_type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::UsesDef * _myserv = POA_CORBA::ComponentIR::UsesDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->interface_type();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::UsesDef_stub::interface_type();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::UsesDef_stub::interface_type( CORBA::InterfaceDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_InterfaceDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_interface_type" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::UsesDef_stub_clp::interface_type( CORBA::InterfaceDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::UsesDef * _myserv = POA_CORBA::ComponentIR::UsesDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->interface_type(_par__value);
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

  CORBA::ComponentIR::UsesDef_stub::interface_type(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::ComponentIR::UsesDef_stub::is_multiple()
{
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "_get_is_multiple" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::ComponentIR::UsesDef_stub_clp::is_multiple()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::UsesDef * _myserv = POA_CORBA::ComponentIR::UsesDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_multiple();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::UsesDef_stub::is_multiple();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::UsesDef_stub::is_multiple( CORBA::Boolean _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );
  CORBA::StaticRequest __req( this, "_set_is_multiple" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::UsesDef_stub_clp::is_multiple( CORBA::Boolean _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::UsesDef * _myserv = POA_CORBA::ComponentIR::UsesDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->is_multiple(_par__value);
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

  CORBA::ComponentIR::UsesDef_stub::is_multiple(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_UsesDescription;
}
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ComponentIR::UsesDescription::UsesDescription()
{
}

CORBA::ComponentIR::UsesDescription::UsesDescription( const UsesDescription& _s )
{
  name = ((UsesDescription&)_s).name;
  id = ((UsesDescription&)_s).id;
  defined_in = ((UsesDescription&)_s).defined_in;
  version = ((UsesDescription&)_s).version;
  interface_type = ((UsesDescription&)_s).interface_type;
  is_multiple = ((UsesDescription&)_s).is_multiple;
}

CORBA::ComponentIR::UsesDescription::~UsesDescription()
{
}

CORBA::ComponentIR::UsesDescription&
CORBA::ComponentIR::UsesDescription::operator=( const UsesDescription& _s )
{
  name = ((UsesDescription&)_s).name;
  id = ((UsesDescription&)_s).id;
  defined_in = ((UsesDescription&)_s).defined_in;
  version = ((UsesDescription&)_s).version;
  interface_type = ((UsesDescription&)_s).interface_type;
  is_multiple = ((UsesDescription&)_s).is_multiple;
  return *this;
}
#endif

class _Marshaller_CORBA_ComponentIR_UsesDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::UsesDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_UsesDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_UsesDescription::~_Marshaller_CORBA_ComponentIR_UsesDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_UsesDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ComponentIR_UsesDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ComponentIR_UsesDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_UsesDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->interface_type._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_multiple ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ComponentIR_UsesDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->interface_type.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_multiple );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_UsesDescription::typecode()
{
  return CORBA::ComponentIR::_tc_UsesDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_UsesDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::UsesDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_UsesDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ComponentIR::UsesDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::UsesDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_UsesDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ComponentIR::UsesDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ComponentIR_UsesDescription, (void *&)_s);
}


/*
 * Base interface for class EventPortDef
 */

CORBA::ComponentIR::EventPortDef::~EventPortDef()
{
}

void *
CORBA::ComponentIR::EventPortDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::EventPortDef_ptr
CORBA::ComponentIR::EventPortDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::EventPortDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::EventPortDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0")) {
      _o = new CORBA::ComponentIR::EventPortDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::EventPortDef_ptr
CORBA::ComponentIR::EventPortDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_EventPortDef;
}
}
class _Marshaller_CORBA_ComponentIR_EventPortDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::EventPortDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_EventPortDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_EventPortDef::~_Marshaller_CORBA_ComponentIR_EventPortDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_EventPortDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_EventPortDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::EventPortDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_EventPortDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_EventPortDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_EventPortDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::EventPortDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_EventPortDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_EventPortDef::typecode()
{
  return CORBA::ComponentIR::_tc_EventPortDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EventPortDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::EventPortDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EventPortDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::EventPortDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EventPortDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::EventPortDef_ptr &_obj )
{
  CORBA::ComponentIR::EventPortDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_EventPortDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class EventPortDef
 */

CORBA::ComponentIR::EventPortDef_stub::~EventPortDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::EventPortDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::EventPortDef *
POA_CORBA::ComponentIR::EventPortDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::EventPortDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::EventPortDef_stub_clp::EventPortDef_stub_clp ()
{
}

CORBA::ComponentIR::EventPortDef_stub_clp::EventPortDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::EventPortDef_stub_clp::~EventPortDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ComponentIR::EventDef_ptr CORBA::ComponentIR::EventPortDef_stub::event()
{
  CORBA::ComponentIR::EventDef_ptr _res = CORBA::ComponentIR::EventDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_EventDef, &_res );

  CORBA::StaticRequest __req( this, "_get_event" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ComponentIR::EventDef_ptr
CORBA::ComponentIR::EventPortDef_stub_clp::event()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::EventPortDef * _myserv = POA_CORBA::ComponentIR::EventPortDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ComponentIR::EventDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->event();
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::EventPortDef_stub::event();
}

#endif // MICO_CONF_NO_POA

void CORBA::ComponentIR::EventPortDef_stub::event( CORBA::ComponentIR::EventDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_EventDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_event" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ComponentIR::EventPortDef_stub_clp::event( CORBA::ComponentIR::EventDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::EventPortDef * _myserv = POA_CORBA::ComponentIR::EventPortDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->event(_par__value);
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

  CORBA::ComponentIR::EventPortDef_stub::event(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::ComponentIR::EventPortDef_stub::is_a( const char* _par_event_id )
{
  CORBA::StaticAny _sa_event_id( CORBA::_stc_string, &_par_event_id );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "is_a" );
  __req.add_in_arg( &_sa_event_id );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::ComponentIR::EventPortDef_stub_clp::is_a( const char* _par_event_id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ComponentIR::EventPortDef * _myserv = POA_CORBA::ComponentIR::EventPortDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_a(_par_event_id);
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
      return __res;
    }
    _postinvoke ();
  }

  return CORBA::ComponentIR::EventPortDef_stub::is_a(_par_event_id);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_EventPortDescription;
}
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ComponentIR::EventPortDescription::EventPortDescription()
{
}

CORBA::ComponentIR::EventPortDescription::EventPortDescription( const EventPortDescription& _s )
{
  name = ((EventPortDescription&)_s).name;
  id = ((EventPortDescription&)_s).id;
  defined_in = ((EventPortDescription&)_s).defined_in;
  version = ((EventPortDescription&)_s).version;
  event = ((EventPortDescription&)_s).event;
}

CORBA::ComponentIR::EventPortDescription::~EventPortDescription()
{
}

CORBA::ComponentIR::EventPortDescription&
CORBA::ComponentIR::EventPortDescription::operator=( const EventPortDescription& _s )
{
  name = ((EventPortDescription&)_s).name;
  id = ((EventPortDescription&)_s).id;
  defined_in = ((EventPortDescription&)_s).defined_in;
  version = ((EventPortDescription&)_s).version;
  event = ((EventPortDescription&)_s).event;
  return *this;
}
#endif

class _Marshaller_CORBA_ComponentIR_EventPortDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::EventPortDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_EventPortDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_EventPortDescription::~_Marshaller_CORBA_ComponentIR_EventPortDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_EventPortDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ComponentIR_EventPortDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ComponentIR_EventPortDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_EventPortDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->event._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ComponentIR_EventPortDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->event.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_EventPortDescription::typecode()
{
  return CORBA::ComponentIR::_tc_EventPortDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EventPortDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::EventPortDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EventPortDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ComponentIR::EventPortDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::EventPortDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EventPortDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ComponentIR::EventPortDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ComponentIR_EventPortDescription, (void *&)_s);
}


/*
 * Base interface for class EmitsDef
 */

CORBA::ComponentIR::EmitsDef::~EmitsDef()
{
}

void *
CORBA::ComponentIR::EmitsDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ComponentIR::EventPortDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::EmitsDef_ptr
CORBA::ComponentIR::EmitsDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::EmitsDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::EmitsDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0")) {
      _o = new CORBA::ComponentIR::EmitsDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::EmitsDef_ptr
CORBA::ComponentIR::EmitsDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_EmitsDef;
}
}
class _Marshaller_CORBA_ComponentIR_EmitsDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::EmitsDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_EmitsDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_EmitsDef::~_Marshaller_CORBA_ComponentIR_EmitsDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_EmitsDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_EmitsDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::EmitsDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_EmitsDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_EmitsDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_EmitsDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::EmitsDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_EmitsDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_EmitsDef::typecode()
{
  return CORBA::ComponentIR::_tc_EmitsDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EmitsDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::EmitsDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EmitsDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::EmitsDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_EmitsDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::EmitsDef_ptr &_obj )
{
  CORBA::ComponentIR::EmitsDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_EmitsDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class EmitsDef
 */

CORBA::ComponentIR::EmitsDef_stub::~EmitsDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::EmitsDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ComponentIR::EventPortDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::EmitsDef *
POA_CORBA::ComponentIR::EmitsDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::EmitsDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::EmitsDef_stub_clp::EmitsDef_stub_clp ()
{
}

CORBA::ComponentIR::EmitsDef_stub_clp::EmitsDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::EmitsDef_stub_clp::~EmitsDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class PublishesDef
 */

CORBA::ComponentIR::PublishesDef::~PublishesDef()
{
}

void *
CORBA::ComponentIR::PublishesDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ComponentIR::EventPortDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::PublishesDef_ptr
CORBA::ComponentIR::PublishesDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::PublishesDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::PublishesDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0")) {
      _o = new CORBA::ComponentIR::PublishesDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::PublishesDef_ptr
CORBA::ComponentIR::PublishesDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_PublishesDef;
}
}
class _Marshaller_CORBA_ComponentIR_PublishesDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::PublishesDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_PublishesDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_PublishesDef::~_Marshaller_CORBA_ComponentIR_PublishesDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_PublishesDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_PublishesDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::PublishesDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_PublishesDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_PublishesDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_PublishesDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::PublishesDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_PublishesDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_PublishesDef::typecode()
{
  return CORBA::ComponentIR::_tc_PublishesDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_PublishesDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::PublishesDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_PublishesDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::PublishesDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_PublishesDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::PublishesDef_ptr &_obj )
{
  CORBA::ComponentIR::PublishesDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_PublishesDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class PublishesDef
 */

CORBA::ComponentIR::PublishesDef_stub::~PublishesDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::PublishesDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ComponentIR::EventPortDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::PublishesDef *
POA_CORBA::ComponentIR::PublishesDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::PublishesDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::PublishesDef_stub_clp::PublishesDef_stub_clp ()
{
}

CORBA::ComponentIR::PublishesDef_stub_clp::PublishesDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::PublishesDef_stub_clp::~PublishesDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ConsumesDef
 */

CORBA::ComponentIR::ConsumesDef::~ConsumesDef()
{
}

void *
CORBA::ComponentIR::ConsumesDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ComponentIR::EventPortDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::ConsumesDef_ptr
CORBA::ComponentIR::ConsumesDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::ConsumesDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::ConsumesDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0")) {
      _o = new CORBA::ComponentIR::ConsumesDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::ConsumesDef_ptr
CORBA::ComponentIR::ConsumesDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ConsumesDef;
}
}
class _Marshaller_CORBA_ComponentIR_ConsumesDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::ConsumesDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_ConsumesDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_ConsumesDef::~_Marshaller_CORBA_ComponentIR_ConsumesDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_ConsumesDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_ConsumesDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::ConsumesDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_ConsumesDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_ConsumesDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_ConsumesDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::ConsumesDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_ConsumesDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_ConsumesDef::typecode()
{
  return CORBA::ComponentIR::_tc_ConsumesDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ConsumesDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::ConsumesDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ConsumesDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::ConsumesDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ConsumesDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::ConsumesDef_ptr &_obj )
{
  CORBA::ComponentIR::ConsumesDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_ConsumesDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ConsumesDef
 */

CORBA::ComponentIR::ConsumesDef_stub::~ConsumesDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::ConsumesDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ComponentIR::EventPortDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::ConsumesDef *
POA_CORBA::ComponentIR::ConsumesDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::ConsumesDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::ConsumesDef_stub_clp::ConsumesDef_stub_clp ()
{
}

CORBA::ComponentIR::ConsumesDef_stub_clp::ConsumesDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::ConsumesDef_stub_clp::~ConsumesDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ProvidesDescriptionSeq;
}
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_UsesDescriptionSeq;
}
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_EventPortDescriptionSeq;
}
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_ComponentDescription;
}
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ComponentIR::ComponentDescription::ComponentDescription()
{
}

CORBA::ComponentIR::ComponentDescription::ComponentDescription( const ComponentDescription& _s )
{
  name = ((ComponentDescription&)_s).name;
  id = ((ComponentDescription&)_s).id;
  defined_in = ((ComponentDescription&)_s).defined_in;
  version = ((ComponentDescription&)_s).version;
  base_component = ((ComponentDescription&)_s).base_component;
  supported_interfaces = ((ComponentDescription&)_s).supported_interfaces;
  provided_interfaces = ((ComponentDescription&)_s).provided_interfaces;
  used_interfaces = ((ComponentDescription&)_s).used_interfaces;
  emits_events = ((ComponentDescription&)_s).emits_events;
  publishes_events = ((ComponentDescription&)_s).publishes_events;
  consumes_events = ((ComponentDescription&)_s).consumes_events;
  attributes = ((ComponentDescription&)_s).attributes;
  type = ((ComponentDescription&)_s).type;
}

CORBA::ComponentIR::ComponentDescription::~ComponentDescription()
{
}

CORBA::ComponentIR::ComponentDescription&
CORBA::ComponentIR::ComponentDescription::operator=( const ComponentDescription& _s )
{
  name = ((ComponentDescription&)_s).name;
  id = ((ComponentDescription&)_s).id;
  defined_in = ((ComponentDescription&)_s).defined_in;
  version = ((ComponentDescription&)_s).version;
  base_component = ((ComponentDescription&)_s).base_component;
  supported_interfaces = ((ComponentDescription&)_s).supported_interfaces;
  provided_interfaces = ((ComponentDescription&)_s).provided_interfaces;
  used_interfaces = ((ComponentDescription&)_s).used_interfaces;
  emits_events = ((ComponentDescription&)_s).emits_events;
  publishes_events = ((ComponentDescription&)_s).publishes_events;
  consumes_events = ((ComponentDescription&)_s).consumes_events;
  attributes = ((ComponentDescription&)_s).attributes;
  type = ((ComponentDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_ComponentIR_ComponentDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::ComponentDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_ComponentDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_ComponentDescription::~_Marshaller_CORBA_ComponentIR_ComponentDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_ComponentDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ComponentIR_ComponentDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ComponentIR_ComponentDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_ComponentDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->base_component._for_demarshal() ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->supported_interfaces ) &&
    _marshaller__seq_CORBA_ComponentIR_ProvidesDescription->demarshal( dc, &((_MICO_T*)v)->provided_interfaces ) &&
    _marshaller__seq_CORBA_ComponentIR_UsesDescription->demarshal( dc, &((_MICO_T*)v)->used_interfaces ) &&
    _marshaller__seq_CORBA_ComponentIR_EventPortDescription->demarshal( dc, &((_MICO_T*)v)->emits_events ) &&
    _marshaller__seq_CORBA_ComponentIR_EventPortDescription->demarshal( dc, &((_MICO_T*)v)->publishes_events ) &&
    _marshaller__seq_CORBA_ComponentIR_EventPortDescription->demarshal( dc, &((_MICO_T*)v)->consumes_events ) &&
    _marshaller__seq_CORBA_ExtAttributeDescription->demarshal( dc, &((_MICO_T*)v)->attributes ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ComponentIR_ComponentDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->base_component.inout() );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->supported_interfaces );
  _marshaller__seq_CORBA_ComponentIR_ProvidesDescription->marshal( ec, &((_MICO_T*)v)->provided_interfaces );
  _marshaller__seq_CORBA_ComponentIR_UsesDescription->marshal( ec, &((_MICO_T*)v)->used_interfaces );
  _marshaller__seq_CORBA_ComponentIR_EventPortDescription->marshal( ec, &((_MICO_T*)v)->emits_events );
  _marshaller__seq_CORBA_ComponentIR_EventPortDescription->marshal( ec, &((_MICO_T*)v)->publishes_events );
  _marshaller__seq_CORBA_ComponentIR_EventPortDescription->marshal( ec, &((_MICO_T*)v)->consumes_events );
  _marshaller__seq_CORBA_ExtAttributeDescription->marshal( ec, &((_MICO_T*)v)->attributes );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_ComponentDescription::typecode()
{
  return CORBA::ComponentIR::_tc_ComponentDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ComponentDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::ComponentDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ComponentDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ComponentIR::ComponentDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::ComponentDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_ComponentDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ComponentIR::ComponentDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ComponentIR_ComponentDescription, (void *&)_s);
}


/*
 * Base interface for class FactoryDef
 */

CORBA::ComponentIR::FactoryDef::~FactoryDef()
{
}

void *
CORBA::ComponentIR::FactoryDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::OperationDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::FactoryDef_ptr
CORBA::ComponentIR::FactoryDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::FactoryDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::FactoryDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0")) {
      _o = new CORBA::ComponentIR::FactoryDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::FactoryDef_ptr
CORBA::ComponentIR::FactoryDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_FactoryDef;
}
}
class _Marshaller_CORBA_ComponentIR_FactoryDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::FactoryDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_FactoryDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_FactoryDef::~_Marshaller_CORBA_ComponentIR_FactoryDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_FactoryDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_FactoryDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::FactoryDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_FactoryDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_FactoryDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_FactoryDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::FactoryDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_FactoryDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_FactoryDef::typecode()
{
  return CORBA::ComponentIR::_tc_FactoryDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_FactoryDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::FactoryDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_FactoryDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::FactoryDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_FactoryDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::FactoryDef_ptr &_obj )
{
  CORBA::ComponentIR::FactoryDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_FactoryDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class FactoryDef
 */

CORBA::ComponentIR::FactoryDef_stub::~FactoryDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::FactoryDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::OperationDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::FactoryDef *
POA_CORBA::ComponentIR::FactoryDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::FactoryDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::FactoryDef_stub_clp::FactoryDef_stub_clp ()
{
}

CORBA::ComponentIR::FactoryDef_stub_clp::FactoryDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::FactoryDef_stub_clp::~FactoryDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class FinderDef
 */

CORBA::ComponentIR::FinderDef::~FinderDef()
{
}

void *
CORBA::ComponentIR::FinderDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::OperationDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ComponentIR::FinderDef_ptr
CORBA::ComponentIR::FinderDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ComponentIR::FinderDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0" )))
      return _duplicate( (CORBA::ComponentIR::FinderDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0")) {
      _o = new CORBA::ComponentIR::FinderDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ComponentIR::FinderDef_ptr
CORBA::ComponentIR::FinderDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_FinderDef;
}
}
class _Marshaller_CORBA_ComponentIR_FinderDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::FinderDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_FinderDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_FinderDef::~_Marshaller_CORBA_ComponentIR_FinderDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_FinderDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ComponentIR_FinderDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ComponentIR::FinderDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ComponentIR_FinderDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ComponentIR_FinderDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_FinderDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ComponentIR::FinderDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ComponentIR_FinderDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_FinderDef::typecode()
{
  return CORBA::ComponentIR::_tc_FinderDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_FinderDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::FinderDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_FinderDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ComponentIR::FinderDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_FinderDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::FinderDef_ptr &_obj )
{
  CORBA::ComponentIR::FinderDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ComponentIR_FinderDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class FinderDef
 */

CORBA::ComponentIR::FinderDef_stub::~FinderDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ComponentIR::FinderDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::OperationDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ComponentIR::FinderDef *
POA_CORBA::ComponentIR::FinderDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ComponentIR::FinderDef *) p;
  }
  return NULL;
}

CORBA::ComponentIR::FinderDef_stub_clp::FinderDef_stub_clp ()
{
}

CORBA::ComponentIR::FinderDef_stub_clp::FinderDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ComponentIR::FinderDef_stub_clp::~FinderDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
namespace ComponentIR
{
CORBA::TypeCodeConst _tc_HomeDescription;
}
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ComponentIR::HomeDescription::HomeDescription()
{
}

CORBA::ComponentIR::HomeDescription::HomeDescription( const HomeDescription& _s )
{
  name = ((HomeDescription&)_s).name;
  id = ((HomeDescription&)_s).id;
  defined_in = ((HomeDescription&)_s).defined_in;
  version = ((HomeDescription&)_s).version;
  base_home = ((HomeDescription&)_s).base_home;
  managed_component = ((HomeDescription&)_s).managed_component;
  primary_key = ((HomeDescription&)_s).primary_key;
  factories = ((HomeDescription&)_s).factories;
  finders = ((HomeDescription&)_s).finders;
  operations = ((HomeDescription&)_s).operations;
  attributes = ((HomeDescription&)_s).attributes;
  type = ((HomeDescription&)_s).type;
}

CORBA::ComponentIR::HomeDescription::~HomeDescription()
{
}

CORBA::ComponentIR::HomeDescription&
CORBA::ComponentIR::HomeDescription::operator=( const HomeDescription& _s )
{
  name = ((HomeDescription&)_s).name;
  id = ((HomeDescription&)_s).id;
  defined_in = ((HomeDescription&)_s).defined_in;
  version = ((HomeDescription&)_s).version;
  base_home = ((HomeDescription&)_s).base_home;
  managed_component = ((HomeDescription&)_s).managed_component;
  primary_key = ((HomeDescription&)_s).primary_key;
  factories = ((HomeDescription&)_s).factories;
  finders = ((HomeDescription&)_s).finders;
  operations = ((HomeDescription&)_s).operations;
  attributes = ((HomeDescription&)_s).attributes;
  type = ((HomeDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_ComponentIR_HomeDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ComponentIR::HomeDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ComponentIR_HomeDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ComponentIR_HomeDescription::~_Marshaller_CORBA_ComponentIR_HomeDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ComponentIR_HomeDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ComponentIR_HomeDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ComponentIR_HomeDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ComponentIR_HomeDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->base_home._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->managed_component._for_demarshal() ) &&
    _marshaller_CORBA_ValueDescription->demarshal( dc, &((_MICO_T*)v)->primary_key ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->factories ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->finders ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->operations ) &&
    _marshaller__seq_CORBA_ExtAttributeDescription->demarshal( dc, &((_MICO_T*)v)->attributes ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ComponentIR_HomeDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->base_home.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->managed_component.inout() );
  _marshaller_CORBA_ValueDescription->marshal( ec, &((_MICO_T*)v)->primary_key );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->factories );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->finders );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->operations );
  _marshaller__seq_CORBA_ExtAttributeDescription->marshal( ec, &((_MICO_T*)v)->attributes );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ComponentIR_HomeDescription::typecode()
{
  return CORBA::ComponentIR::_tc_HomeDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_HomeDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ComponentIR::HomeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_HomeDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ComponentIR::HomeDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ComponentIR::HomeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ComponentIR_HomeDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ComponentIR::HomeDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ComponentIR_HomeDescription, (void *&)_s);
}

class _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ComponentIR_ProvidesDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::~_Marshaller__seq_CORBA_ComponentIR_ProvidesDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ComponentIR_ProvidesDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ComponentIR_ProvidesDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000038020000010000000f00000028020000010000003600"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e7449"
    "522f50726f76696465734465736372697074696f6e3a312e300000001400"
    "000050726f76696465734465736372697074696f6e000500000005000000"
    "6e616d65000000001500000044000000010000002100000049444c3a6f6d"
    "672e6f72672f434f5242412f4964656e7469666965723a312e3000000000"
    "0b0000004964656e74696669657200001200000000000000030000006964"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000b00000064656669"
    "6e65645f696e00001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000800"
    "000076657273696f6e001500000044000000010000002200000049444c3a"
    "6f6d672e6f72672f434f5242412f56657273696f6e537065633a312e3000"
    "00000c00000056657273696f6e537065630012000000000000000f000000"
    "696e746572666163655f7479706500001500000048000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49643a312e3000000d0000005265706f7369746f72794964000000001200"
    "00000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ComponentIR_ProvidesDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ComponentIR_ProvidesDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ComponentIR_ProvidesDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ComponentIR_ProvidesDescription, (void *&)_s);
}


class _Marshaller__seq_CORBA_ComponentIR_UsesDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ComponentIR_UsesDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ComponentIR_UsesDescription::~_Marshaller__seq_CORBA_ComponentIR_UsesDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ComponentIR_UsesDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ComponentIR_UsesDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ComponentIR_UsesDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ComponentIR_UsesDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ComponentIR_UsesDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ComponentIR_UsesDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ComponentIR_UsesDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ComponentIR_UsesDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000044020000010000000f00000034020000010000003200"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e7449"
    "522f557365734465736372697074696f6e3a312e30000000100000005573"
    "65734465736372697074696f6e0006000000050000006e616d6500000000"
    "1500000044000000010000002100000049444c3a6f6d672e6f72672f434f"
    "5242412f4964656e7469666965723a312e30000000000b0000004964656e"
    "746966696572000012000000000000000300000069640000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "49640000000012000000000000000b000000646566696e65645f696e0000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000800000076657273696f"
    "6e001500000044000000010000002200000049444c3a6f6d672e6f72672f"
    "434f5242412f56657273696f6e537065633a312e300000000c0000005665"
    "7273696f6e537065630012000000000000000f000000696e746572666163"
    "655f7479706500001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000c00"
    "000069735f6d756c7469706c65000800000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ComponentIR_UsesDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ComponentIR_UsesDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ComponentIR_UsesDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ComponentIR_UsesDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ComponentIR_UsesDescription, (void *&)_s);
}


class _Marshaller__seq_CORBA_ComponentIR_EventPortDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ComponentIR_EventPortDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ComponentIR_EventPortDescription::~_Marshaller__seq_CORBA_ComponentIR_EventPortDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ComponentIR_EventPortDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ComponentIR_EventPortDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000034020000010000000f00000024020000010000003700"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e7449"
    "522f4576656e74506f72744465736372697074696f6e3a312e3000001500"
    "00004576656e74506f72744465736372697074696f6e0000000005000000"
    "050000006e616d6500000000150000004400000001000000210000004944"
    "4c3a6f6d672e6f72672f434f5242412f4964656e7469666965723a312e30"
    "000000000b0000004964656e746966696572000012000000000000000300"
    "0000696400001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f727949640000000012000000000000000b000000"
    "646566696e65645f696e0000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "00000800000076657273696f6e0015000000440000000100000022000000"
    "49444c3a6f6d672e6f72672f434f5242412f56657273696f6e537065633a"
    "312e300000000c00000056657273696f6e53706563001200000000000000"
    "060000006576656e74000000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ComponentIR_EventPortDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ComponentIR_EventPortDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ComponentIR_EventPortDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ComponentIR_EventPortDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ComponentIR_EventPortDescription, (void *&)_s);
}


struct __tc_init_IR3 {
  __tc_init_IR3()
  {
    CORBA::ComponentIR::_tc_ComponentDef = 
    "010000000e00000049000000010000002f00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f436f6d706f6e656e7444"
    "65663a312e3000000d000000436f6d706f6e656e7444656600";
    _marshaller_CORBA_ComponentIR_ComponentDef = new _Marshaller_CORBA_ComponentIR_ComponentDef;
    CORBA::ComponentIR::_tc_HomeDef = 
    "010000000e00000040000000010000002a00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f486f6d654465663a312e"
    "3000000008000000486f6d6544656600";
    _marshaller_CORBA_ComponentIR_HomeDef = new _Marshaller_CORBA_ComponentIR_HomeDef;
    CORBA::ComponentIR::_tc_EventDef = 
    "010000000e00000041000000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f4576656e744465663a31"
    "2e300000090000004576656e7444656600";
    _marshaller_CORBA_ComponentIR_EventDef = new _Marshaller_CORBA_ComponentIR_EventDef;
    CORBA::ComponentIR::_tc_Container = 
    "010000000e00000042000000010000002c00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f436f6e7461696e65723a"
    "312e30000a000000436f6e7461696e657200";
    _marshaller_CORBA_ComponentIR_Container = new _Marshaller_CORBA_ComponentIR_Container;
    CORBA::ComponentIR::_tc_ModuleDef = 
    "010000000e00000042000000010000002c00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f4d6f64756c654465663a"
    "312e30000a0000004d6f64756c6544656600";
    _marshaller_CORBA_ComponentIR_ModuleDef = new _Marshaller_CORBA_ComponentIR_ModuleDef;
    CORBA::ComponentIR::_tc_Repository = 
    "010000000e00000047000000010000002d00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f5265706f7369746f7279"
    "3a312e30000000000b0000005265706f7369746f727900";
    _marshaller_CORBA_ComponentIR_Repository = new _Marshaller_CORBA_ComponentIR_Repository;
    CORBA::ComponentIR::_tc_ProvidesDef = 
    "010000000e00000048000000010000002e00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f50726f76696465734465"
    "663a312e300000000c00000050726f766964657344656600";
    _marshaller_CORBA_ComponentIR_ProvidesDef = new _Marshaller_CORBA_ComponentIR_ProvidesDef;
    CORBA::ComponentIR::_tc_ProvidesDescription = 
    "010000000f00000028020000010000003600000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f50726f76696465734465"
    "736372697074696f6e3a312e300000001400000050726f76696465734465"
    "736372697074696f6e0005000000050000006e616d650000000015000000"
    "44000000010000002100000049444c3a6f6d672e6f72672f434f5242412f"
    "4964656e7469666965723a312e30000000000b0000004964656e74696669"
    "657200001200000000000000030000006964000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000b000000646566696e65645f696e000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000800000076657273696f6e001500"
    "000044000000010000002200000049444c3a6f6d672e6f72672f434f5242"
    "412f56657273696f6e537065633a312e300000000c00000056657273696f"
    "6e537065630012000000000000000f000000696e746572666163655f7479"
    "706500001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f72794964000000001200000000000000";
    _marshaller_CORBA_ComponentIR_ProvidesDescription = new _Marshaller_CORBA_ComponentIR_ProvidesDescription;
    CORBA::ComponentIR::_tc_UsesDef = 
    "010000000e00000040000000010000002a00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f557365734465663a312e"
    "30000000080000005573657344656600";
    _marshaller_CORBA_ComponentIR_UsesDef = new _Marshaller_CORBA_ComponentIR_UsesDef;
    CORBA::ComponentIR::_tc_UsesDescription = 
    "010000000f00000034020000010000003200000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f55736573446573637269"
    "7074696f6e3a312e3000000010000000557365734465736372697074696f"
    "6e0006000000050000006e616d6500000000150000004400000001000000"
    "2100000049444c3a6f6d672e6f72672f434f5242412f4964656e74696669"
    "65723a312e30000000000b0000004964656e746966696572000012000000"
    "000000000300000069640000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "00000b000000646566696e65645f696e0000150000004800000001000000"
    "2300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f"
    "727949643a312e3000000d0000005265706f7369746f7279496400000000"
    "12000000000000000800000076657273696f6e0015000000440000000100"
    "00002200000049444c3a6f6d672e6f72672f434f5242412f56657273696f"
    "6e537065633a312e300000000c00000056657273696f6e53706563001200"
    "0000000000000f000000696e746572666163655f74797065000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000c00000069735f6d756c7469706c"
    "650008000000";
    _marshaller_CORBA_ComponentIR_UsesDescription = new _Marshaller_CORBA_ComponentIR_UsesDescription;
    CORBA::ComponentIR::_tc_EventPortDef = 
    "010000000e00000049000000010000002f00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f4576656e74506f727444"
    "65663a312e3000000d0000004576656e74506f727444656600";
    _marshaller_CORBA_ComponentIR_EventPortDef = new _Marshaller_CORBA_ComponentIR_EventPortDef;
    CORBA::ComponentIR::_tc_EventPortDescription = 
    "010000000f00000024020000010000003700000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f4576656e74506f727444"
    "65736372697074696f6e3a312e300000150000004576656e74506f727444"
    "65736372697074696f6e0000000005000000050000006e616d6500000000"
    "1500000044000000010000002100000049444c3a6f6d672e6f72672f434f"
    "5242412f4964656e7469666965723a312e30000000000b0000004964656e"
    "746966696572000012000000000000000300000069640000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "49640000000012000000000000000b000000646566696e65645f696e0000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000800000076657273696f"
    "6e001500000044000000010000002200000049444c3a6f6d672e6f72672f"
    "434f5242412f56657273696f6e537065633a312e300000000c0000005665"
    "7273696f6e53706563001200000000000000060000006576656e74000000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f72794964000000001200000000000000";
    _marshaller_CORBA_ComponentIR_EventPortDescription = new _Marshaller_CORBA_ComponentIR_EventPortDescription;
    CORBA::ComponentIR::_tc_EmitsDef = 
    "010000000e00000041000000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f456d6974734465663a31"
    "2e30000009000000456d69747344656600";
    _marshaller_CORBA_ComponentIR_EmitsDef = new _Marshaller_CORBA_ComponentIR_EmitsDef;
    CORBA::ComponentIR::_tc_PublishesDef = 
    "010000000e00000049000000010000002f00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f5075626c697368657344"
    "65663a312e3000000d0000005075626c697368657344656600";
    _marshaller_CORBA_ComponentIR_PublishesDef = new _Marshaller_CORBA_ComponentIR_PublishesDef;
    CORBA::ComponentIR::_tc_ConsumesDef = 
    "010000000e00000048000000010000002e00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f436f6e73756d65734465"
    "663a312e300000000c000000436f6e73756d657344656600";
    _marshaller_CORBA_ComponentIR_ConsumesDef = new _Marshaller_CORBA_ComponentIR_ConsumesDef;
    CORBA::ComponentIR::_tc_ProvidesDescriptionSeq = 
    "0100000015000000a0020000010000003900000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f50726f76696465734465"
    "736372697074696f6e5365713a312e30000000001700000050726f766964"
    "65734465736372697074696f6e5365710000130000003802000001000000"
    "0f00000028020000010000003600000049444c3a6f6d672e6f72672f434f"
    "5242412f436f6d706f6e656e7449522f50726f7669646573446573637269"
    "7074696f6e3a312e300000001400000050726f7669646573446573637269"
    "7074696f6e0005000000050000006e616d65000000001500000044000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f4964656e"
    "7469666965723a312e30000000000b0000004964656e7469666965720000"
    "120000000000000003000000696400001500000048000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49643a312e3000000d0000005265706f7369746f72794964000000001200"
    "0000000000000b000000646566696e65645f696e00001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "0000000012000000000000000800000076657273696f6e00150000004400"
    "0000010000002200000049444c3a6f6d672e6f72672f434f5242412f5665"
    "7273696f6e537065633a312e300000000c00000056657273696f6e537065"
    "630012000000000000000f000000696e746572666163655f747970650000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f7279496400000000120000000000000000000000";
    CORBA::ComponentIR::_tc_UsesDescriptionSeq = 
    "0100000015000000a4020000010000003500000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f55736573446573637269"
    "7074696f6e5365713a312e30000000001300000055736573446573637269"
    "7074696f6e53657100001300000044020000010000000f00000034020000"
    "010000003200000049444c3a6f6d672e6f72672f434f5242412f436f6d70"
    "6f6e656e7449522f557365734465736372697074696f6e3a312e30000000"
    "10000000557365734465736372697074696f6e0006000000050000006e61"
    "6d65000000001500000044000000010000002100000049444c3a6f6d672e"
    "6f72672f434f5242412f4964656e7469666965723a312e30000000000b00"
    "00004964656e746966696572000012000000000000000300000069640000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000b000000646566696e65"
    "645f696e00001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f7279496400000000120000000000000008000000"
    "76657273696f6e001500000044000000010000002200000049444c3a6f6d"
    "672e6f72672f434f5242412f56657273696f6e537065633a312e30000000"
    "0c00000056657273696f6e537065630012000000000000000f000000696e"
    "746572666163655f74797065000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000c00000069735f6d756c7469706c65000800000000000000";
    CORBA::ComponentIR::_tc_EventPortDescriptionSeq = 
    "01000000150000009c020000010000003a00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f4576656e74506f727444"
    "65736372697074696f6e5365713a312e30000000180000004576656e7450"
    "6f72744465736372697074696f6e53657100130000003402000001000000"
    "0f00000024020000010000003700000049444c3a6f6d672e6f72672f434f"
    "5242412f436f6d706f6e656e7449522f4576656e74506f72744465736372"
    "697074696f6e3a312e300000150000004576656e74506f72744465736372"
    "697074696f6e0000000005000000050000006e616d650000000015000000"
    "44000000010000002100000049444c3a6f6d672e6f72672f434f5242412f"
    "4964656e7469666965723a312e30000000000b0000004964656e74696669"
    "657200001200000000000000030000006964000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000b000000646566696e65645f696e000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000800000076657273696f6e001500"
    "000044000000010000002200000049444c3a6f6d672e6f72672f434f5242"
    "412f56657273696f6e537065633a312e300000000c00000056657273696f"
    "6e53706563001200000000000000060000006576656e7400000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "7279496400000000120000000000000000000000";
    CORBA::ComponentIR::_tc_ComponentDescription = 
    "010000000f0000000c180000010000003700000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f436f6d706f6e656e7444"
    "65736372697074696f6e3a312e30000015000000436f6d706f6e656e7444"
    "65736372697074696f6e000000000d000000050000006e616d6500000000"
    "1500000044000000010000002100000049444c3a6f6d672e6f72672f434f"
    "5242412f4964656e7469666965723a312e30000000000b0000004964656e"
    "746966696572000012000000000000000300000069640000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "49640000000012000000000000000b000000646566696e65645f696e0000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000800000076657273696f"
    "6e001500000044000000010000002200000049444c3a6f6d672e6f72672f"
    "434f5242412f56657273696f6e537065633a312e300000000c0000005665"
    "7273696f6e537065630012000000000000000f000000626173655f636f6d"
    "706f6e656e7400001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000001500"
    "0000737570706f727465645f696e74657266616365730000000015000000"
    "a4000000010000002600000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949645365713a312e30000000100000005265706f"
    "7369746f7279496453657100130000005800000001000000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "4964000000001200000000000000000000001400000070726f7669646564"
    "5f696e74657266616365730015000000a002000001000000390000004944"
    "4c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e7449522f5072"
    "6f76696465734465736372697074696f6e5365713a312e30000000001700"
    "000050726f76696465734465736372697074696f6e536571000013000000"
    "38020000010000000f00000028020000010000003600000049444c3a6f6d"
    "672e6f72672f434f5242412f436f6d706f6e656e7449522f50726f766964"
    "65734465736372697074696f6e3a312e300000001400000050726f766964"
    "65734465736372697074696f6e0005000000050000006e616d6500000000"
    "1500000044000000010000002100000049444c3a6f6d672e6f72672f434f"
    "5242412f4964656e7469666965723a312e30000000000b0000004964656e"
    "746966696572000012000000000000000300000069640000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "49640000000012000000000000000b000000646566696e65645f696e0000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000800000076657273696f"
    "6e001500000044000000010000002200000049444c3a6f6d672e6f72672f"
    "434f5242412f56657273696f6e537065633a312e300000000c0000005665"
    "7273696f6e537065630012000000000000000f000000696e746572666163"
    "655f7479706500001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000000"
    "000010000000757365645f696e74657266616365730015000000a4020000"
    "010000003500000049444c3a6f6d672e6f72672f434f5242412f436f6d70"
    "6f6e656e7449522f557365734465736372697074696f6e5365713a312e30"
    "0000000013000000557365734465736372697074696f6e53657100001300"
    "000044020000010000000f00000034020000010000003200000049444c3a"
    "6f6d672e6f72672f434f5242412f436f6d706f6e656e7449522f55736573"
    "4465736372697074696f6e3a312e30000000100000005573657344657363"
    "72697074696f6e0006000000050000006e616d6500000000150000004400"
    "0000010000002100000049444c3a6f6d672e6f72672f434f5242412f4964"
    "656e7469666965723a312e30000000000b0000004964656e746966696572"
    "000012000000000000000300000069640000150000004800000001000000"
    "2300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f"
    "727949643a312e3000000d0000005265706f7369746f7279496400000000"
    "12000000000000000b000000646566696e65645f696e0000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "49640000000012000000000000000800000076657273696f6e0015000000"
    "44000000010000002200000049444c3a6f6d672e6f72672f434f5242412f"
    "56657273696f6e537065633a312e300000000c00000056657273696f6e53"
    "7065630012000000000000000f000000696e746572666163655f74797065"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000c00000069735f6d"
    "756c7469706c650008000000000000000d000000656d6974735f6576656e"
    "747300000000150000009c020000010000003a00000049444c3a6f6d672e"
    "6f72672f434f5242412f436f6d706f6e656e7449522f4576656e74506f72"
    "744465736372697074696f6e5365713a312e30000000180000004576656e"
    "74506f72744465736372697074696f6e5365710013000000340200000100"
    "00000f00000024020000010000003700000049444c3a6f6d672e6f72672f"
    "434f5242412f436f6d706f6e656e7449522f4576656e74506f7274446573"
    "6372697074696f6e3a312e300000150000004576656e74506f7274446573"
    "6372697074696f6e0000000005000000050000006e616d65000000001500"
    "000044000000010000002100000049444c3a6f6d672e6f72672f434f5242"
    "412f4964656e7469666965723a312e30000000000b0000004964656e7469"
    "666965720000120000000000000003000000696400001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "0000000012000000000000000b000000646566696e65645f696e00001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000800000076657273696f6e00"
    "1500000044000000010000002200000049444c3a6f6d672e6f72672f434f"
    "5242412f56657273696f6e537065633a312e300000000c00000056657273"
    "696f6e53706563001200000000000000060000006576656e740000001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f7279496400000000120000000000000000000000110000007075626c"
    "69736865735f6576656e747300000000150000009c020000010000003a00"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e7449"
    "522f4576656e74506f72744465736372697074696f6e5365713a312e3000"
    "0000180000004576656e74506f72744465736372697074696f6e53657100"
    "1300000034020000010000000f0000002402000001000000370000004944"
    "4c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e7449522f4576"
    "656e74506f72744465736372697074696f6e3a312e300000150000004576"
    "656e74506f72744465736372697074696f6e000000000500000005000000"
    "6e616d65000000001500000044000000010000002100000049444c3a6f6d"
    "672e6f72672f434f5242412f4964656e7469666965723a312e3000000000"
    "0b0000004964656e74696669657200001200000000000000030000006964"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000b00000064656669"
    "6e65645f696e00001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000800"
    "000076657273696f6e001500000044000000010000002200000049444c3a"
    "6f6d672e6f72672f434f5242412f56657273696f6e537065633a312e3000"
    "00000c00000056657273696f6e5370656300120000000000000006000000"
    "6576656e740000001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000000"
    "000010000000636f6e73756d65735f6576656e747300150000009c020000"
    "010000003a00000049444c3a6f6d672e6f72672f434f5242412f436f6d70"
    "6f6e656e7449522f4576656e74506f72744465736372697074696f6e5365"
    "713a312e30000000180000004576656e74506f7274446573637269707469"
    "6f6e536571001300000034020000010000000f0000002402000001000000"
    "3700000049444c3a6f6d672e6f72672f434f5242412f436f6d706f6e656e"
    "7449522f4576656e74506f72744465736372697074696f6e3a312e300000"
    "150000004576656e74506f72744465736372697074696f6e000000000500"
    "0000050000006e616d650000000015000000440000000100000021000000"
    "49444c3a6f6d672e6f72672f434f5242412f4964656e7469666965723a31"
    "2e30000000000b0000004964656e74696669657200001200000000000000"
    "03000000696400001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000b00"
    "0000646566696e65645f696e000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000800000076657273696f6e001500000044000000010000002200"
    "000049444c3a6f6d672e6f72672f434f5242412f56657273696f6e537065"
    "633a312e300000000c00000056657273696f6e5370656300120000000000"
    "0000060000006576656e7400000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "00000000000000000b000000617474726962757465730000150000004407"
    "0000010000002c00000049444c3a6f6d672e6f72672f434f5242412f4578"
    "74417474724465736372697074696f6e5365713a312e3000160000004578"
    "74417474724465736372697074696f6e53657100000013000000ec060000"
    "010000000f000000dc060000010000002e00000049444c3a6f6d672e6f72"
    "672f434f5242412f4578744174747269627574654465736372697074696f"
    "6e3a312e3000000018000000457874417474726962757465446573637269"
    "7074696f6e0008000000050000006e616d65000000001500000044000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f4964656e"
    "7469666965723a312e30000000000b0000004964656e7469666965720000"
    "120000000000000003000000696400001500000048000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49643a312e3000000d0000005265706f7369746f72794964000000001200"
    "0000000000000b000000646566696e65645f696e00001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "0000000012000000000000000800000076657273696f6e00150000004400"
    "0000010000002200000049444c3a6f6d672e6f72672f434f5242412f5665"
    "7273696f6e537065633a312e300000000c00000056657273696f6e537065"
    "630012000000000000000500000074797065000000000c00000005000000"
    "6d6f6465000000001100000066000000010000002400000049444c3a6f6d"
    "672e6f72672f434f5242412f4174747269627574654d6f64653a312e3000"
    "0e0000004174747269627574654d6f6465000000020000000c0000004154"
    "54525f4e4f524d414c000e000000415454525f524541444f4e4c59000000"
    "0f0000006765745f657863657074696f6e730000150000002c0200000100"
    "00002800000049444c3a6f6d672e6f72672f434f5242412f457863446573"
    "6372697074696f6e5365713a312e30001200000045786344657363726970"
    "74696f6e53657100000013000000dc010000010000000f000000cc010000"
    "010000002b00000049444c3a6f6d672e6f72672f434f5242412f45786365"
    "7074696f6e4465736372697074696f6e3a312e3000001500000045786365"
    "7074696f6e4465736372697074696f6e0000000005000000050000006e61"
    "6d65000000001500000044000000010000002100000049444c3a6f6d672e"
    "6f72672f434f5242412f4964656e7469666965723a312e30000000000b00"
    "00004964656e746966696572000012000000000000000300000069640000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000b000000646566696e65"
    "645f696e00001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f7279496400000000120000000000000008000000"
    "76657273696f6e001500000044000000010000002200000049444c3a6f6d"
    "672e6f72672f434f5242412f56657273696f6e537065633a312e30000000"
    "0c00000056657273696f6e53706563001200000000000000050000007479"
    "7065000000000c000000000000000f0000007365745f657863657074696f"
    "6e730000150000002c020000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f4578634465736372697074696f6e5365713a312e3000"
    "120000004578634465736372697074696f6e53657100000013000000dc01"
    "0000010000000f000000cc010000010000002b00000049444c3a6f6d672e"
    "6f72672f434f5242412f457863657074696f6e4465736372697074696f6e"
    "3a312e30000015000000457863657074696f6e4465736372697074696f6e"
    "0000000005000000050000006e616d650000000015000000440000000100"
    "00002100000049444c3a6f6d672e6f72672f434f5242412f4964656e7469"
    "666965723a312e30000000000b0000004964656e74696669657200001200"
    "000000000000030000006964000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000b000000646566696e65645f696e000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000800000076657273696f6e001500000044000000"
    "010000002200000049444c3a6f6d672e6f72672f434f5242412f56657273"
    "696f6e537065633a312e300000000c00000056657273696f6e5370656300"
    "12000000000000000500000074797065000000000c000000000000000000"
    "00000500000074797065000000000c000000";
    _marshaller_CORBA_ComponentIR_ComponentDescription = new _Marshaller_CORBA_ComponentIR_ComponentDescription;
    CORBA::ComponentIR::_tc_FactoryDef = 
    "010000000e00000047000000010000002d00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f466163746f7279446566"
    "3a312e30000000000b000000466163746f727944656600";
    _marshaller_CORBA_ComponentIR_FactoryDef = new _Marshaller_CORBA_ComponentIR_FactoryDef;
    CORBA::ComponentIR::_tc_FinderDef = 
    "010000000e00000042000000010000002c00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f46696e6465724465663a"
    "312e30000a00000046696e64657244656600";
    _marshaller_CORBA_ComponentIR_FinderDef = new _Marshaller_CORBA_ComponentIR_FinderDef;
    CORBA::ComponentIR::_tc_HomeDescription = 
    "010000000f0000000c260000010000003200000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6d706f6e656e7449522f486f6d65446573637269"
    "7074696f6e3a312e3000000010000000486f6d654465736372697074696f"
    "6e000c000000050000006e616d6500000000150000004400000001000000"
    "2100000049444c3a6f6d672e6f72672f434f5242412f4964656e74696669"
    "65723a312e30000000000b0000004964656e746966696572000012000000"
    "000000000300000069640000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "00000b000000646566696e65645f696e0000150000004800000001000000"
    "2300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f"
    "727949643a312e3000000d0000005265706f7369746f7279496400000000"
    "12000000000000000800000076657273696f6e0015000000440000000100"
    "00002200000049444c3a6f6d672e6f72672f434f5242412f56657273696f"
    "6e537065633a312e300000000c00000056657273696f6e53706563001200"
    "0000000000000a000000626173655f686f6d650000001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "000000001200000000000000120000006d616e616765645f636f6d706f6e"
    "656e740000001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f727949640000000012000000000000000c000000"
    "7072696d6172795f6b6579000f000000e403000001000000270000004944"
    "4c3a6f6d672e6f72672f434f5242412f56616c7565446573637269707469"
    "6f6e3a312e3000001100000056616c75654465736372697074696f6e0000"
    "00000a000000050000006e616d6500000000150000004400000001000000"
    "2100000049444c3a6f6d672e6f72672f434f5242412f4964656e74696669"
    "65723a312e30000000000b0000004964656e746966696572000012000000"
    "000000000300000069640000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "00000c00000069735f616273747261637400080000000a00000069735f63"
    "7573746f6d000000080000000b000000646566696e65645f696e00001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000800000076657273696f6e00"
    "1500000044000000010000002200000049444c3a6f6d672e6f72672f434f"
    "5242412f56657273696f6e537065633a312e300000000c00000056657273"
    "696f6e5370656300120000000000000015000000737570706f727465645f"
    "696e74657266616365730000000015000000a40000000100000026000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "5365713a312e30000000100000005265706f7369746f7279496453657100"
    "130000005800000001000000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "0000000000001500000061627374726163745f626173655f76616c756573"
    "0000000015000000a4000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949645365713a312e30000000"
    "100000005265706f7369746f727949645365710013000000580000000100"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f72794964000000001200000000000000000000000f000000"
    "69735f7472756e63617461626c650000080000000b000000626173655f76"
    "616c756500001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f727949640000000012000000000000000a000000"
    "666163746f7269657300000015000000f407000001000000270000004944"
    "4c3a6f6d672e6f72672f434f5242412f4f704465736372697074696f6e53"
    "65713a312e300000110000004f704465736372697074696f6e5365710000"
    "000013000000a4070000010000000f00000094070000010000002b000000"
    "49444c3a6f6d672e6f72672f434f5242412f4f7065726174696f6e446573"
    "6372697074696f6e3a312e300000150000004f7065726174696f6e446573"
    "6372697074696f6e0000000009000000050000006e616d65000000001500"
    "000044000000010000002100000049444c3a6f6d672e6f72672f434f5242"
    "412f4964656e7469666965723a312e30000000000b0000004964656e7469"
    "666965720000120000000000000003000000696400001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "0000000012000000000000000b000000646566696e65645f696e00001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000800000076657273696f6e00"
    "1500000044000000010000002200000049444c3a6f6d672e6f72672f434f"
    "5242412f56657273696f6e537065633a312e300000000c00000056657273"
    "696f6e5370656300120000000000000007000000726573756c7400000c00"
    "0000050000006d6f64650000000011000000620000000100000024000000"
    "49444c3a6f6d672e6f72672f434f5242412f4f7065726174696f6e4d6f64"
    "653a312e30000e0000004f7065726174696f6e4d6f646500000002000000"
    "0a0000004f505f4e4f524d414c0000000a0000004f505f4f4e4557415900"
    "000009000000636f6e74657874730000000015000000ec00000001000000"
    "2300000049444c3a6f6d672e6f72672f434f5242412f436f6e7465787449"
    "645365713a312e3000000d000000436f6e74657874496453657100000000"
    "13000000a400000001000000150000009400000001000000280000004944"
    "4c3a6f6d672e6f72672f434f5242412f436f6e746578744964656e746966"
    "6965723a312e300012000000436f6e746578744964656e74696669657200"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000000000000b00000070617261"
    "6d6574657273000015000000f0010000010000002800000049444c3a6f6d"
    "672e6f72672f434f5242412f5061724465736372697074696f6e5365713a"
    "312e3000120000005061724465736372697074696f6e5365710000001300"
    "0000a0010000010000000f00000090010000010000002b00000049444c3a"
    "6f6d672e6f72672f434f5242412f506172616d6574657244657363726970"
    "74696f6e3a312e30000015000000506172616d6574657244657363726970"
    "74696f6e0000000004000000050000006e616d6500000000150000004400"
    "0000010000002100000049444c3a6f6d672e6f72672f434f5242412f4964"
    "656e7469666965723a312e30000000000b0000004964656e746966696572"
    "000012000000000000000500000074797065000000000c00000009000000"
    "747970655f646566000000000e00000034000000010000001e0000004944"
    "4c3a6f6d672e6f72672f434f5242412f49444c547970653a312e30000000"
    "0800000049444c5479706500050000006d6f646500000000110000007400"
    "0000010000002400000049444c3a6f6d672e6f72672f434f5242412f5061"
    "72616d657465724d6f64653a312e30000e000000506172616d657465724d"
    "6f64650000000300000009000000504152414d5f494e000000000a000000"
    "504152414d5f4f55540000000c000000504152414d5f494e4f5554000000"
    "00000b000000657863657074696f6e730000150000002c02000001000000"
    "2800000049444c3a6f6d672e6f72672f434f5242412f4578634465736372"
    "697074696f6e5365713a312e300012000000457863446573637269707469"
    "6f6e53657100000013000000dc010000010000000f000000cc0100000100"
    "00002b00000049444c3a6f6d672e6f72672f434f5242412f457863657074"
    "696f6e4465736372697074696f6e3a312e30000015000000457863657074"
    "696f6e4465736372697074696f6e0000000005000000050000006e616d65"
    "000000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f4964656e7469666965723a312e30000000000b000000"
    "4964656e7469666965720000120000000000000003000000696400001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000b000000646566696e65645f"
    "696e00001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f72794964000000001200000000000000080000007665"
    "7273696f6e001500000044000000010000002200000049444c3a6f6d672e"
    "6f72672f434f5242412f56657273696f6e537065633a312e300000000c00"
    "000056657273696f6e537065630012000000000000000500000074797065"
    "000000000c00000000000000000000000800000066696e64657273001500"
    "0000f4070000010000002700000049444c3a6f6d672e6f72672f434f5242"
    "412f4f704465736372697074696f6e5365713a312e300000110000004f70"
    "4465736372697074696f6e5365710000000013000000a407000001000000"
    "0f00000094070000010000002b00000049444c3a6f6d672e6f72672f434f"
    "5242412f4f7065726174696f6e4465736372697074696f6e3a312e300000"
    "150000004f7065726174696f6e4465736372697074696f6e000000000900"
    "0000050000006e616d650000000015000000440000000100000021000000"
    "49444c3a6f6d672e6f72672f434f5242412f4964656e7469666965723a31"
    "2e30000000000b0000004964656e74696669657200001200000000000000"
    "03000000696400001500000048000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5265706f7369746f727949643a312e300000"
    "0d0000005265706f7369746f727949640000000012000000000000000b00"
    "0000646566696e65645f696e000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000800000076657273696f6e001500000044000000010000002200"
    "000049444c3a6f6d672e6f72672f434f5242412f56657273696f6e537065"
    "633a312e300000000c00000056657273696f6e5370656300120000000000"
    "000007000000726573756c7400000c000000050000006d6f646500000000"
    "1100000062000000010000002400000049444c3a6f6d672e6f72672f434f"
    "5242412f4f7065726174696f6e4d6f64653a312e30000e0000004f706572"
    "6174696f6e4d6f6465000000020000000a0000004f505f4e4f524d414c00"
    "00000a0000004f505f4f4e4557415900000009000000636f6e7465787473"
    "0000000015000000ec000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7465787449645365713a312e3000000d000000"
    "436f6e7465787449645365710000000013000000a4000000010000001500"
    "000094000000010000002800000049444c3a6f6d672e6f72672f434f5242"
    "412f436f6e746578744964656e7469666965723a312e300012000000436f"
    "6e746578744964656e746966696572000000150000004400000001000000"
    "2100000049444c3a6f6d672e6f72672f434f5242412f4964656e74696669"
    "65723a312e30000000000b0000004964656e746966696572000012000000"
    "00000000000000000b000000706172616d6574657273000015000000f001"
    "0000010000002800000049444c3a6f6d672e6f72672f434f5242412f5061"
    "724465736372697074696f6e5365713a312e300012000000506172446573"
    "6372697074696f6e53657100000013000000a0010000010000000f000000"
    "90010000010000002b00000049444c3a6f6d672e6f72672f434f5242412f"
    "506172616d657465724465736372697074696f6e3a312e30000015000000"
    "506172616d657465724465736372697074696f6e00000000040000000500"
    "00006e616d65000000001500000044000000010000002100000049444c3a"
    "6f6d672e6f72672f434f5242412f4964656e7469666965723a312e300000"
    "00000b0000004964656e7469666965720000120000000000000005000000"
    "74797065000000000c00000009000000747970655f646566000000000e00"
    "000034000000010000001e00000049444c3a6f6d672e6f72672f434f5242"
    "412f49444c547970653a312e300000000800000049444c54797065000500"
    "00006d6f6465000000001100000074000000010000002400000049444c3a"
    "6f6d672e6f72672f434f5242412f506172616d657465724d6f64653a312e"
    "30000e000000506172616d657465724d6f64650000000300000009000000"
    "504152414d5f494e000000000a000000504152414d5f4f55540000000c00"
    "0000504152414d5f494e4f555400000000000b000000657863657074696f"
    "6e730000150000002c020000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f4578634465736372697074696f6e5365713a312e3000"
    "120000004578634465736372697074696f6e53657100000013000000dc01"
    "0000010000000f000000cc010000010000002b00000049444c3a6f6d672e"
    "6f72672f434f5242412f457863657074696f6e4465736372697074696f6e"
    "3a312e30000015000000457863657074696f6e4465736372697074696f6e"
    "0000000005000000050000006e616d650000000015000000440000000100"
    "00002100000049444c3a6f6d672e6f72672f434f5242412f4964656e7469"
    "666965723a312e30000000000b0000004964656e74696669657200001200"
    "000000000000030000006964000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000b000000646566696e65645f696e000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000800000076657273696f6e001500000044000000"
    "010000002200000049444c3a6f6d672e6f72672f434f5242412f56657273"
    "696f6e537065633a312e300000000c00000056657273696f6e5370656300"
    "12000000000000000500000074797065000000000c000000000000000000"
    "00000b0000006f7065726174696f6e73000015000000f407000001000000"
    "2700000049444c3a6f6d672e6f72672f434f5242412f4f70446573637269"
    "7074696f6e5365713a312e300000110000004f704465736372697074696f"
    "6e5365710000000013000000a4070000010000000f000000940700000100"
    "00002b00000049444c3a6f6d672e6f72672f434f5242412f4f7065726174"
    "696f6e4465736372697074696f6e3a312e300000150000004f7065726174"
    "696f6e4465736372697074696f6e0000000009000000050000006e616d65"
    "000000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f4964656e7469666965723a312e30000000000b000000"
    "4964656e7469666965720000120000000000000003000000696400001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000b000000646566696e65645f"
    "696e00001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f72794964000000001200000000000000080000007665"
    "7273696f6e001500000044000000010000002200000049444c3a6f6d672e"
    "6f72672f434f5242412f56657273696f6e537065633a312e300000000c00"
    "000056657273696f6e537065630012000000000000000700000072657375"
    "6c7400000c000000050000006d6f64650000000011000000620000000100"
    "00002400000049444c3a6f6d672e6f72672f434f5242412f4f7065726174"
    "696f6e4d6f64653a312e30000e0000004f7065726174696f6e4d6f646500"
    "0000020000000a0000004f505f4e4f524d414c0000000a0000004f505f4f"
    "4e4557415900000009000000636f6e74657874730000000015000000ec00"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f436f"
    "6e7465787449645365713a312e3000000d000000436f6e74657874496453"
    "65710000000013000000a400000001000000150000009400000001000000"
    "2800000049444c3a6f6d672e6f72672f434f5242412f436f6e7465787449"
    "64656e7469666965723a312e300012000000436f6e746578744964656e74"
    "69666965720000001500000044000000010000002100000049444c3a6f6d"
    "672e6f72672f434f5242412f4964656e7469666965723a312e3000000000"
    "0b0000004964656e74696669657200001200000000000000000000000b00"
    "0000706172616d6574657273000015000000f00100000100000028000000"
    "49444c3a6f6d672e6f72672f434f5242412f506172446573637269707469"
    "6f6e5365713a312e3000120000005061724465736372697074696f6e5365"
    "7100000013000000a0010000010000000f00000090010000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f506172616d6574657244"
    "65736372697074696f6e3a312e30000015000000506172616d6574657244"
    "65736372697074696f6e0000000004000000050000006e616d6500000000"
    "1500000044000000010000002100000049444c3a6f6d672e6f72672f434f"
    "5242412f4964656e7469666965723a312e30000000000b0000004964656e"
    "746966696572000012000000000000000500000074797065000000000c00"
    "000009000000747970655f646566000000000e0000003400000001000000"
    "1e00000049444c3a6f6d672e6f72672f434f5242412f49444c547970653a"
    "312e300000000800000049444c5479706500050000006d6f646500000000"
    "1100000074000000010000002400000049444c3a6f6d672e6f72672f434f"
    "5242412f506172616d657465724d6f64653a312e30000e00000050617261"
    "6d657465724d6f64650000000300000009000000504152414d5f494e0000"
    "00000a000000504152414d5f4f55540000000c000000504152414d5f494e"
    "4f555400000000000b000000657863657074696f6e730000150000002c02"
    "0000010000002800000049444c3a6f6d672e6f72672f434f5242412f4578"
    "634465736372697074696f6e5365713a312e300012000000457863446573"
    "6372697074696f6e53657100000013000000dc010000010000000f000000"
    "cc010000010000002b00000049444c3a6f6d672e6f72672f434f5242412f"
    "457863657074696f6e4465736372697074696f6e3a312e30000015000000"
    "457863657074696f6e4465736372697074696f6e00000000050000000500"
    "00006e616d65000000001500000044000000010000002100000049444c3a"
    "6f6d672e6f72672f434f5242412f4964656e7469666965723a312e300000"
    "00000b0000004964656e7469666965720000120000000000000003000000"
    "696400001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f727949640000000012000000000000000b0000006465"
    "66696e65645f696e00001500000048000000010000002300000049444c3a"
    "6f6d672e6f72672f434f5242412f5265706f7369746f727949643a312e30"
    "00000d0000005265706f7369746f72794964000000001200000000000000"
    "0800000076657273696f6e00150000004400000001000000220000004944"
    "4c3a6f6d672e6f72672f434f5242412f56657273696f6e537065633a312e"
    "300000000c00000056657273696f6e537065630012000000000000000500"
    "000074797065000000000c00000000000000000000000b00000061747472"
    "69627574657300001500000044070000010000002c00000049444c3a6f6d"
    "672e6f72672f434f5242412f457874417474724465736372697074696f6e"
    "5365713a312e300016000000457874417474724465736372697074696f6e"
    "53657100000013000000ec060000010000000f000000dc06000001000000"
    "2e00000049444c3a6f6d672e6f72672f434f5242412f4578744174747269"
    "627574654465736372697074696f6e3a312e300000001800000045787441"
    "74747269627574654465736372697074696f6e0008000000050000006e61"
    "6d65000000001500000044000000010000002100000049444c3a6f6d672e"
    "6f72672f434f5242412f4964656e7469666965723a312e30000000000b00"
    "00004964656e746966696572000012000000000000000300000069640000"
    "1500000048000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5265706f7369746f727949643a312e3000000d0000005265706f"
    "7369746f727949640000000012000000000000000b000000646566696e65"
    "645f696e00001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f7279496400000000120000000000000008000000"
    "76657273696f6e001500000044000000010000002200000049444c3a6f6d"
    "672e6f72672f434f5242412f56657273696f6e537065633a312e30000000"
    "0c00000056657273696f6e53706563001200000000000000050000007479"
    "7065000000000c000000050000006d6f6465000000001100000066000000"
    "010000002400000049444c3a6f6d672e6f72672f434f5242412f41747472"
    "69627574654d6f64653a312e30000e0000004174747269627574654d6f64"
    "65000000020000000c000000415454525f4e4f524d414c000e0000004154"
    "54525f524541444f4e4c590000000f0000006765745f657863657074696f"
    "6e730000150000002c020000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f4578634465736372697074696f6e5365713a312e3000"
    "120000004578634465736372697074696f6e53657100000013000000dc01"
    "0000010000000f000000cc010000010000002b00000049444c3a6f6d672e"
    "6f72672f434f5242412f457863657074696f6e4465736372697074696f6e"
    "3a312e30000015000000457863657074696f6e4465736372697074696f6e"
    "0000000005000000050000006e616d650000000015000000440000000100"
    "00002100000049444c3a6f6d672e6f72672f434f5242412f4964656e7469"
    "666965723a312e30000000000b0000004964656e74696669657200001200"
    "000000000000030000006964000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000b000000646566696e65645f696e000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000800000076657273696f6e001500000044000000"
    "010000002200000049444c3a6f6d672e6f72672f434f5242412f56657273"
    "696f6e537065633a312e300000000c00000056657273696f6e5370656300"
    "12000000000000000500000074797065000000000c000000000000000f00"
    "00007365745f657863657074696f6e730000150000002c02000001000000"
    "2800000049444c3a6f6d672e6f72672f434f5242412f4578634465736372"
    "697074696f6e5365713a312e300012000000457863446573637269707469"
    "6f6e53657100000013000000dc010000010000000f000000cc0100000100"
    "00002b00000049444c3a6f6d672e6f72672f434f5242412f457863657074"
    "696f6e4465736372697074696f6e3a312e30000015000000457863657074"
    "696f6e4465736372697074696f6e0000000005000000050000006e616d65"
    "000000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f4964656e7469666965723a312e30000000000b000000"
    "4964656e7469666965720000120000000000000003000000696400001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000b000000646566696e65645f"
    "696e00001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f72794964000000001200000000000000080000007665"
    "7273696f6e001500000044000000010000002200000049444c3a6f6d672e"
    "6f72672f434f5242412f56657273696f6e537065633a312e300000000c00"
    "000056657273696f6e537065630012000000000000000500000074797065"
    "000000000c00000000000000000000000500000074797065000000000c00"
    "0000";
    _marshaller_CORBA_ComponentIR_HomeDescription = new _Marshaller_CORBA_ComponentIR_HomeDescription;
    _marshaller__seq_CORBA_ComponentIR_ProvidesDescription = new _Marshaller__seq_CORBA_ComponentIR_ProvidesDescription;
    _marshaller__seq_CORBA_ComponentIR_UsesDescription = new _Marshaller__seq_CORBA_ComponentIR_UsesDescription;
    _marshaller__seq_CORBA_ComponentIR_EventPortDescription = new _Marshaller__seq_CORBA_ComponentIR_EventPortDescription;
  }

  ~__tc_init_IR3()
  {
    delete static_cast<_Marshaller_CORBA_ComponentIR_ComponentDef*>(_marshaller_CORBA_ComponentIR_ComponentDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_HomeDef*>(_marshaller_CORBA_ComponentIR_HomeDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_EventDef*>(_marshaller_CORBA_ComponentIR_EventDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_Container*>(_marshaller_CORBA_ComponentIR_Container);
    delete static_cast<_Marshaller_CORBA_ComponentIR_ModuleDef*>(_marshaller_CORBA_ComponentIR_ModuleDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_Repository*>(_marshaller_CORBA_ComponentIR_Repository);
    delete static_cast<_Marshaller_CORBA_ComponentIR_ProvidesDef*>(_marshaller_CORBA_ComponentIR_ProvidesDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_ProvidesDescription*>(_marshaller_CORBA_ComponentIR_ProvidesDescription);
    delete static_cast<_Marshaller_CORBA_ComponentIR_UsesDef*>(_marshaller_CORBA_ComponentIR_UsesDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_UsesDescription*>(_marshaller_CORBA_ComponentIR_UsesDescription);
    delete static_cast<_Marshaller_CORBA_ComponentIR_EventPortDef*>(_marshaller_CORBA_ComponentIR_EventPortDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_EventPortDescription*>(_marshaller_CORBA_ComponentIR_EventPortDescription);
    delete static_cast<_Marshaller_CORBA_ComponentIR_EmitsDef*>(_marshaller_CORBA_ComponentIR_EmitsDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_PublishesDef*>(_marshaller_CORBA_ComponentIR_PublishesDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_ConsumesDef*>(_marshaller_CORBA_ComponentIR_ConsumesDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_ComponentDescription*>(_marshaller_CORBA_ComponentIR_ComponentDescription);
    delete static_cast<_Marshaller_CORBA_ComponentIR_FactoryDef*>(_marshaller_CORBA_ComponentIR_FactoryDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_FinderDef*>(_marshaller_CORBA_ComponentIR_FinderDef);
    delete static_cast<_Marshaller_CORBA_ComponentIR_HomeDescription*>(_marshaller_CORBA_ComponentIR_HomeDescription);
    delete static_cast<_Marshaller__seq_CORBA_ComponentIR_ProvidesDescription*>(_marshaller__seq_CORBA_ComponentIR_ProvidesDescription);
    delete static_cast<_Marshaller__seq_CORBA_ComponentIR_UsesDescription*>(_marshaller__seq_CORBA_ComponentIR_UsesDescription);
    delete static_cast<_Marshaller__seq_CORBA_ComponentIR_EventPortDescription*>(_marshaller__seq_CORBA_ComponentIR_EventPortDescription);
  }
};

static __tc_init_IR3 __init_IR3;

