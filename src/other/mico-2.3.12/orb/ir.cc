/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>
#include <mico/template_impl.h>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------
namespace CORBA
{
CORBA::TypeCodeConst _tc_ScopedName;
}

namespace CORBA
{
CORBA::TypeCodeConst Contained::_tc_Description;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::Contained::Description::Description()
{
}

CORBA::Contained::Description::Description( const Description& _s )
{
  kind = ((Description&)_s).kind;
  value = ((Description&)_s).value;
}

CORBA::Contained::Description::~Description()
{
}

CORBA::Contained::Description&
CORBA::Contained::Description::operator=( const Description& _s )
{
  kind = ((Description&)_s).kind;
  value = ((Description&)_s).value;
  return *this;
}
#endif

class _Marshaller_CORBA_Contained_Description : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Contained::Description _MICO_T;
  public:
    ~_Marshaller_CORBA_Contained_Description();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_Contained_Description::~_Marshaller_CORBA_Contained_Description()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_Contained_Description::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_Contained_Description::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_Contained_Description::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_Contained_Description::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_CORBA_DefinitionKind->demarshal( dc, &((_MICO_T*)v)->kind ) &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_Contained_Description::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_CORBA_DefinitionKind->marshal( ec, &((_MICO_T*)v)->kind );
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_Contained_Description::typecode()
{
  return CORBA::Contained::_tc_Description;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_Contained_Description;

void operator<<=( CORBA::Any &_a, const CORBA::Contained::Description &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Contained_Description, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::Contained::Description *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::Contained::Description &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Contained_Description, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::Contained::Description *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_Contained_Description, (void *&)_s);
}


/*
 * Base interface for class Contained
 */

CORBA::Contained::~Contained()
{
}

void *
CORBA::Contained::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/Contained:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IRObject::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::Contained_ptr
CORBA::Contained::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::Contained_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/Contained:1.0" )))
      return _duplicate( (CORBA::Contained_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/Contained:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/Contained:1.0")) {
      _o = new CORBA::Contained_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::Contained_ptr
CORBA::Contained::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_Contained;
}
class _Marshaller_CORBA_Contained : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Contained_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_Contained();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_Contained::~_Marshaller_CORBA_Contained()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_Contained::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_Contained::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::Contained::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_Contained::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_Contained::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_Contained::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::Contained::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_Contained::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_Contained::typecode()
{
  return CORBA::_tc_Contained;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_Contained;

void
operator<<=( CORBA::Any &_a, const CORBA::Contained_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Contained, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::Contained_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Contained, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::Contained_ptr &_obj )
{
  CORBA::Contained_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_Contained, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Contained
 */

CORBA::Contained_stub::~Contained_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::Contained::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/Contained:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IRObject::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::Contained *
POA_CORBA::Contained::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/Contained:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::Contained *) p;
  }
  return NULL;
}

CORBA::Contained_stub_clp::Contained_stub_clp ()
{
}

CORBA::Contained_stub_clp::Contained_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::Contained_stub_clp::~Contained_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

char* CORBA::Contained_stub::id()
{
  CORBA::RepositoryId _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_id" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::Contained_stub_clp::id()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::RepositoryId __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->id();
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

  return CORBA::Contained_stub::id();
}

#endif // MICO_CONF_NO_POA

void CORBA::Contained_stub::id( const char* _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value );
  CORBA::StaticRequest __req( this, "_set_id" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::Contained_stub_clp::id( const char* _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->id(_par__value);
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

  CORBA::Contained_stub::id(_par__value);
}

#endif // MICO_CONF_NO_POA

char* CORBA::Contained_stub::name()
{
  CORBA::Identifier _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_name" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::Contained_stub_clp::name()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::Identifier __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->name();
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

  return CORBA::Contained_stub::name();
}

#endif // MICO_CONF_NO_POA

void CORBA::Contained_stub::name( const char* _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value );
  CORBA::StaticRequest __req( this, "_set_name" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::Contained_stub_clp::name( const char* _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->name(_par__value);
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

  CORBA::Contained_stub::name(_par__value);
}

#endif // MICO_CONF_NO_POA

char* CORBA::Contained_stub::version()
{
  CORBA::VersionSpec _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_version" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::Contained_stub_clp::version()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::VersionSpec __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->version();
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

  return CORBA::Contained_stub::version();
}

#endif // MICO_CONF_NO_POA

void CORBA::Contained_stub::version( const char* _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value );
  CORBA::StaticRequest __req( this, "_set_version" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::Contained_stub_clp::version( const char* _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->version(_par__value);
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

  CORBA::Contained_stub::version(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Container_ptr CORBA::Contained_stub::defined_in()
{
  CORBA::Container_ptr _res = CORBA::Container::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_Container, &_res );

  CORBA::StaticRequest __req( this, "_get_defined_in" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Container_ptr
CORBA::Contained_stub_clp::defined_in()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::Container_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->defined_in();
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

  return CORBA::Contained_stub::defined_in();
}

#endif // MICO_CONF_NO_POA

char* CORBA::Contained_stub::absolute_name()
{
  CORBA::ScopedName _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_absolute_name" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
CORBA::Contained_stub_clp::absolute_name()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::ScopedName __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->absolute_name();
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

  return CORBA::Contained_stub::absolute_name();
}

#endif // MICO_CONF_NO_POA

CORBA::Repository_ptr CORBA::Contained_stub::containing_repository()
{
  CORBA::Repository_ptr _res = CORBA::Repository::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_Repository, &_res );

  CORBA::StaticRequest __req( this, "_get_containing_repository" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Repository_ptr
CORBA::Contained_stub_clp::containing_repository()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::Repository_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->containing_repository();
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

  return CORBA::Contained_stub::containing_repository();
}

#endif // MICO_CONF_NO_POA

CORBA::Contained::Description* CORBA::Contained_stub::describe()
{
  CORBA::StaticAny __res( _marshaller_CORBA_Contained_Description );

  CORBA::StaticRequest __req( this, "describe" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::Contained::Description*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::Contained::Description*
CORBA::Contained_stub_clp::describe()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      CORBA::Contained::Description* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe();
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

  return CORBA::Contained_stub::describe();
}

#endif // MICO_CONF_NO_POA

void CORBA::Contained_stub::move( CORBA::Container_ptr _par_new_container, const char* _par_new_name, const char* _par_new_version )
{
  CORBA::StaticAny _sa_new_container( _marshaller_CORBA_Container, &_par_new_container );
  CORBA::StaticAny _sa_new_name( CORBA::_stc_string, &_par_new_name );
  CORBA::StaticAny _sa_new_version( CORBA::_stc_string, &_par_new_version );
  CORBA::StaticRequest __req( this, "move" );
  __req.add_in_arg( &_sa_new_container );
  __req.add_in_arg( &_sa_new_name );
  __req.add_in_arg( &_sa_new_version );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::Contained_stub_clp::move( CORBA::Container_ptr _par_new_container, const char* _par_new_name, const char* _par_new_version )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Contained * _myserv = POA_CORBA::Contained::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->move(_par_new_container, _par_new_name, _par_new_version);
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

  CORBA::Contained_stub::move(_par_new_container, _par_new_name, _par_new_version);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_ContainedSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_InterfaceDefSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_AbstractInterfaceDefSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ValueDefSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_InitializerSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtInitializerSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst Container::_tc_Description;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::Container::Description::Description()
{
}

CORBA::Container::Description::Description( const Description& _s )
{
  contained_object = ((Description&)_s).contained_object;
  kind = ((Description&)_s).kind;
  value = ((Description&)_s).value;
}

CORBA::Container::Description::~Description()
{
}

CORBA::Container::Description&
CORBA::Container::Description::operator=( const Description& _s )
{
  contained_object = ((Description&)_s).contained_object;
  kind = ((Description&)_s).kind;
  value = ((Description&)_s).value;
  return *this;
}
#endif

class _Marshaller_CORBA_Container_Description : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Container::Description _MICO_T;
  public:
    ~_Marshaller_CORBA_Container_Description();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_Container_Description::~_Marshaller_CORBA_Container_Description()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_Container_Description::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_Container_Description::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_Container_Description::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_Container_Description::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_CORBA_Contained->demarshal( dc, &((_MICO_T*)v)->contained_object._for_demarshal() ) &&
    _marshaller_CORBA_DefinitionKind->demarshal( dc, &((_MICO_T*)v)->kind ) &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_Container_Description::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_CORBA_Contained->marshal( ec, &((_MICO_T*)v)->contained_object.inout() );
  _marshaller_CORBA_DefinitionKind->marshal( ec, &((_MICO_T*)v)->kind );
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_Container_Description::typecode()
{
  return CORBA::Container::_tc_Description;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_Container_Description;

void operator<<=( CORBA::Any &_a, const CORBA::Container::Description &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Container_Description, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::Container::Description *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::Container::Description &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Container_Description, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::Container::Description *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_Container_Description, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst Container::_tc_DescriptionSeq;
}


/*
 * Base interface for class Container
 */

CORBA::Container::~Container()
{
}

void *
CORBA::Container::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/Container:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IRObject::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::Container_ptr
CORBA::Container::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::Container_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/Container:1.0" )))
      return _duplicate( (CORBA::Container_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/Container:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/Container:1.0")) {
      _o = new CORBA::Container_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::Container_ptr
CORBA::Container::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_Container;
}
class _Marshaller_CORBA_Container : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Container_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_Container();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_Container::~_Marshaller_CORBA_Container()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_Container::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_Container::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::Container::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_Container::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_Container::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_Container::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::Container::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_Container::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_Container::typecode()
{
  return CORBA::_tc_Container;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_Container;

void
operator<<=( CORBA::Any &_a, const CORBA::Container_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Container, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::Container_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Container, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::Container_ptr &_obj )
{
  CORBA::Container_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_Container, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Container
 */

CORBA::Container_stub::~Container_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::Container::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/Container:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IRObject::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::Container *
POA_CORBA::Container::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/Container:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::Container *) p;
  }
  return NULL;
}

CORBA::Container_stub_clp::Container_stub_clp ()
{
}

CORBA::Container_stub_clp::Container_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::Container_stub_clp::~Container_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Contained_ptr CORBA::Container_stub::lookup( const char* _par_search_name )
{
  CORBA::StaticAny _sa_search_name( CORBA::_stc_string, &_par_search_name );
  CORBA::Contained_ptr _res = CORBA::Contained::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_Contained, &_res );

  CORBA::StaticRequest __req( this, "lookup" );
  __req.add_in_arg( &_sa_search_name );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Contained_ptr
CORBA::Container_stub_clp::lookup( const char* _par_search_name )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::Contained_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->lookup(_par_search_name);
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

  return CORBA::Container_stub::lookup(_par_search_name);
}

#endif // MICO_CONF_NO_POA

CORBA::ContainedSeq* CORBA::Container_stub::contents( CORBA::DefinitionKind _par_limit_type, CORBA::Boolean _par_exclude_inherited )
{
  CORBA::StaticAny _sa_limit_type( _marshaller_CORBA_DefinitionKind, &_par_limit_type );
  CORBA::StaticAny _sa_exclude_inherited( CORBA::_stc_boolean, &_par_exclude_inherited );
  CORBA::StaticAny __res( _marshaller__seq_CORBA_Contained );

  CORBA::StaticRequest __req( this, "contents" );
  __req.add_in_arg( &_sa_limit_type );
  __req.add_in_arg( &_sa_exclude_inherited );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ContainedSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ContainedSeq*
CORBA::Container_stub_clp::contents( CORBA::DefinitionKind _par_limit_type, CORBA::Boolean _par_exclude_inherited )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ContainedSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->contents(_par_limit_type, _par_exclude_inherited);
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

  return CORBA::Container_stub::contents(_par_limit_type, _par_exclude_inherited);
}

#endif // MICO_CONF_NO_POA

CORBA::ContainedSeq* CORBA::Container_stub::lookup_name( const char* _par_search_name, CORBA::Long _par_levels_to_search, CORBA::DefinitionKind _par_limit_type, CORBA::Boolean _par_exclude_inherited )
{
  CORBA::StaticAny _sa_search_name( CORBA::_stc_string, &_par_search_name );
  CORBA::StaticAny _sa_levels_to_search( CORBA::_stc_long, &_par_levels_to_search );
  CORBA::StaticAny _sa_limit_type( _marshaller_CORBA_DefinitionKind, &_par_limit_type );
  CORBA::StaticAny _sa_exclude_inherited( CORBA::_stc_boolean, &_par_exclude_inherited );
  CORBA::StaticAny __res( _marshaller__seq_CORBA_Contained );

  CORBA::StaticRequest __req( this, "lookup_name" );
  __req.add_in_arg( &_sa_search_name );
  __req.add_in_arg( &_sa_levels_to_search );
  __req.add_in_arg( &_sa_limit_type );
  __req.add_in_arg( &_sa_exclude_inherited );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ContainedSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ContainedSeq*
CORBA::Container_stub_clp::lookup_name( const char* _par_search_name, CORBA::Long _par_levels_to_search, CORBA::DefinitionKind _par_limit_type, CORBA::Boolean _par_exclude_inherited )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ContainedSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->lookup_name(_par_search_name, _par_levels_to_search, _par_limit_type, _par_exclude_inherited);
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

  return CORBA::Container_stub::lookup_name(_par_search_name, _par_levels_to_search, _par_limit_type, _par_exclude_inherited);
}

#endif // MICO_CONF_NO_POA

CORBA::Container::DescriptionSeq* CORBA::Container_stub::describe_contents( CORBA::DefinitionKind _par_limit_type, CORBA::Boolean _par_exclude_inherited, CORBA::Long _par_max_returned_objs )
{
  CORBA::StaticAny _sa_limit_type( _marshaller_CORBA_DefinitionKind, &_par_limit_type );
  CORBA::StaticAny _sa_exclude_inherited( CORBA::_stc_boolean, &_par_exclude_inherited );
  CORBA::StaticAny _sa_max_returned_objs( CORBA::_stc_long, &_par_max_returned_objs );
  CORBA::StaticAny __res( _marshaller__seq_CORBA_Container_Description );

  CORBA::StaticRequest __req( this, "describe_contents" );
  __req.add_in_arg( &_sa_limit_type );
  __req.add_in_arg( &_sa_exclude_inherited );
  __req.add_in_arg( &_sa_max_returned_objs );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::Container::DescriptionSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::Container::DescriptionSeq*
CORBA::Container_stub_clp::describe_contents( CORBA::DefinitionKind _par_limit_type, CORBA::Boolean _par_exclude_inherited, CORBA::Long _par_max_returned_objs )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::Container::DescriptionSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe_contents(_par_limit_type, _par_exclude_inherited, _par_max_returned_objs);
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

  return CORBA::Container_stub::describe_contents(_par_limit_type, _par_exclude_inherited, _par_max_returned_objs);
}

#endif // MICO_CONF_NO_POA

CORBA::ModuleDef_ptr CORBA::Container_stub::create_module( const char* _par_id, const char* _par_name, const char* _par_version )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::ModuleDef_ptr _res = CORBA::ModuleDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ModuleDef, &_res );

  CORBA::StaticRequest __req( this, "create_module" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ModuleDef_ptr
CORBA::Container_stub_clp::create_module( const char* _par_id, const char* _par_name, const char* _par_version )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ModuleDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_module(_par_id, _par_name, _par_version);
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

  return CORBA::Container_stub::create_module(_par_id, _par_name, _par_version);
}

#endif // MICO_CONF_NO_POA

CORBA::ConstantDef_ptr CORBA::Container_stub::create_constant( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, const CORBA::Any& _par_value )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type );
  CORBA::StaticAny _sa_value( CORBA::_stc_any, &_par_value );
  CORBA::ConstantDef_ptr _res = CORBA::ConstantDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ConstantDef, &_res );

  CORBA::StaticRequest __req( this, "create_constant" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_type );
  __req.add_in_arg( &_sa_value );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ConstantDef_ptr
CORBA::Container_stub_clp::create_constant( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, const CORBA::Any& _par_value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ConstantDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_constant(_par_id, _par_name, _par_version, _par_type, _par_value);
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

  return CORBA::Container_stub::create_constant(_par_id, _par_name, _par_version, _par_type, _par_value);
}

#endif // MICO_CONF_NO_POA

CORBA::StructDef_ptr CORBA::Container_stub::create_struct( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::StructMemberSeq& _par_members )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_members( _marshaller__seq_CORBA_StructMember, &_par_members );
  CORBA::StructDef_ptr _res = CORBA::StructDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_StructDef, &_res );

  CORBA::StaticRequest __req( this, "create_struct" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_members );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::StructDef_ptr
CORBA::Container_stub_clp::create_struct( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::StructMemberSeq& _par_members )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::StructDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_struct(_par_id, _par_name, _par_version, _par_members);
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

  return CORBA::Container_stub::create_struct(_par_id, _par_name, _par_version, _par_members);
}

#endif // MICO_CONF_NO_POA

CORBA::ExceptionDef_ptr CORBA::Container_stub::create_exception( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::StructMemberSeq& _par_members )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_members( _marshaller__seq_CORBA_StructMember, &_par_members );
  CORBA::ExceptionDef_ptr _res = CORBA::ExceptionDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ExceptionDef, &_res );

  CORBA::StaticRequest __req( this, "create_exception" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_members );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ExceptionDef_ptr
CORBA::Container_stub_clp::create_exception( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::StructMemberSeq& _par_members )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ExceptionDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_exception(_par_id, _par_name, _par_version, _par_members);
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

  return CORBA::Container_stub::create_exception(_par_id, _par_name, _par_version, _par_members);
}

#endif // MICO_CONF_NO_POA

CORBA::UnionDef_ptr CORBA::Container_stub::create_union( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_discriminator_type, const CORBA::UnionMemberSeq& _par_members )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_discriminator_type( _marshaller_CORBA_IDLType, &_par_discriminator_type );
  CORBA::StaticAny _sa_members( _marshaller__seq_CORBA_UnionMember, &_par_members );
  CORBA::UnionDef_ptr _res = CORBA::UnionDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_UnionDef, &_res );

  CORBA::StaticRequest __req( this, "create_union" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_discriminator_type );
  __req.add_in_arg( &_sa_members );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::UnionDef_ptr
CORBA::Container_stub_clp::create_union( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_discriminator_type, const CORBA::UnionMemberSeq& _par_members )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::UnionDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_union(_par_id, _par_name, _par_version, _par_discriminator_type, _par_members);
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

  return CORBA::Container_stub::create_union(_par_id, _par_name, _par_version, _par_discriminator_type, _par_members);
}

#endif // MICO_CONF_NO_POA

CORBA::EnumDef_ptr CORBA::Container_stub::create_enum( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::EnumMemberSeq& _par_members )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_members( CORBA::_stcseq_string, &_par_members );
  CORBA::EnumDef_ptr _res = CORBA::EnumDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_EnumDef, &_res );

  CORBA::StaticRequest __req( this, "create_enum" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_members );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::EnumDef_ptr
CORBA::Container_stub_clp::create_enum( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::EnumMemberSeq& _par_members )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::EnumDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_enum(_par_id, _par_name, _par_version, _par_members);
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

  return CORBA::Container_stub::create_enum(_par_id, _par_name, _par_version, _par_members);
}

#endif // MICO_CONF_NO_POA

CORBA::AliasDef_ptr CORBA::Container_stub::create_alias( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_original_type )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_original_type( _marshaller_CORBA_IDLType, &_par_original_type );
  CORBA::AliasDef_ptr _res = CORBA::AliasDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_AliasDef, &_res );

  CORBA::StaticRequest __req( this, "create_alias" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_original_type );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::AliasDef_ptr
CORBA::Container_stub_clp::create_alias( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_original_type )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::AliasDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_alias(_par_id, _par_name, _par_version, _par_original_type);
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

  return CORBA::Container_stub::create_alias(_par_id, _par_name, _par_version, _par_original_type);
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDef_ptr CORBA::Container_stub::create_interface( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::InterfaceDefSeq& _par_base_interfaces )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_base_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_base_interfaces );
  CORBA::InterfaceDef_ptr _res = CORBA::InterfaceDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef, &_res );

  CORBA::StaticRequest __req( this, "create_interface" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_base_interfaces );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDef_ptr
CORBA::Container_stub_clp::create_interface( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::InterfaceDefSeq& _par_base_interfaces )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_interface(_par_id, _par_name, _par_version, _par_base_interfaces);
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

  return CORBA::Container_stub::create_interface(_par_id, _par_name, _par_version, _par_base_interfaces);
}

#endif // MICO_CONF_NO_POA

CORBA::AbstractInterfaceDef_ptr CORBA::Container_stub::create_abstract_interface( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::AbstractInterfaceDefSeq& _par_base_interfaces )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_base_interfaces( _marshaller__seq_CORBA_AbstractInterfaceDef, &_par_base_interfaces );
  CORBA::AbstractInterfaceDef_ptr _res = CORBA::AbstractInterfaceDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_AbstractInterfaceDef, &_res );

  CORBA::StaticRequest __req( this, "create_abstract_interface" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_base_interfaces );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::AbstractInterfaceDef_ptr
CORBA::Container_stub_clp::create_abstract_interface( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::AbstractInterfaceDefSeq& _par_base_interfaces )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::AbstractInterfaceDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_abstract_interface(_par_id, _par_name, _par_version, _par_base_interfaces);
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

  return CORBA::Container_stub::create_abstract_interface(_par_id, _par_name, _par_version, _par_base_interfaces);
}

#endif // MICO_CONF_NO_POA

CORBA::LocalInterfaceDef_ptr CORBA::Container_stub::create_local_interface( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::InterfaceDefSeq& _par_base_interfaces )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_base_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_base_interfaces );
  CORBA::LocalInterfaceDef_ptr _res = CORBA::LocalInterfaceDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_LocalInterfaceDef, &_res );

  CORBA::StaticRequest __req( this, "create_local_interface" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_base_interfaces );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::LocalInterfaceDef_ptr
CORBA::Container_stub_clp::create_local_interface( const char* _par_id, const char* _par_name, const char* _par_version, const CORBA::InterfaceDefSeq& _par_base_interfaces )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::LocalInterfaceDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_local_interface(_par_id, _par_name, _par_version, _par_base_interfaces);
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

  return CORBA::Container_stub::create_local_interface(_par_id, _par_name, _par_version, _par_base_interfaces);
}

#endif // MICO_CONF_NO_POA

CORBA::ValueDef_ptr CORBA::Container_stub::create_value( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::Boolean _par_is_custom, CORBA::Boolean _par_is_abstract, CORBA::ValueDef_ptr _par_base_value, CORBA::Boolean _par_is_truncatable, const CORBA::ValueDefSeq& _par_abstract_base_values, const CORBA::InterfaceDefSeq& _par_supported_interfaces, const CORBA::InitializerSeq& _par_initializers )
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
  CORBA::StaticAny _sa_initializers( _marshaller__seq_CORBA_Initializer, &_par_initializers );
  CORBA::ValueDef_ptr _res = CORBA::ValueDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ValueDef, &_res );

  CORBA::StaticRequest __req( this, "create_value" );
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

CORBA::ValueDef_ptr
CORBA::Container_stub_clp::create_value( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::Boolean _par_is_custom, CORBA::Boolean _par_is_abstract, CORBA::ValueDef_ptr _par_base_value, CORBA::Boolean _par_is_truncatable, const CORBA::ValueDefSeq& _par_abstract_base_values, const CORBA::InterfaceDefSeq& _par_supported_interfaces, const CORBA::InitializerSeq& _par_initializers )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_value(_par_id, _par_name, _par_version, _par_is_custom, _par_is_abstract, _par_base_value, _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers);
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

  return CORBA::Container_stub::create_value(_par_id, _par_name, _par_version, _par_is_custom, _par_is_abstract, _par_base_value, _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers);
}

#endif // MICO_CONF_NO_POA

CORBA::ExtValueDef_ptr CORBA::Container_stub::create_ext_value( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::Boolean _par_is_custom, CORBA::Boolean _par_is_abstract, CORBA::ValueDef_ptr _par_base_value, CORBA::Boolean _par_is_truncatable, const CORBA::ValueDefSeq& _par_abstract_base_values, const CORBA::InterfaceDefSeq& _par_supported_interfaces, const CORBA::ExtInitializerSeq& _par_initializers )
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
  CORBA::ExtValueDef_ptr _res = CORBA::ExtValueDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ExtValueDef, &_res );

  CORBA::StaticRequest __req( this, "create_ext_value" );
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

CORBA::ExtValueDef_ptr
CORBA::Container_stub_clp::create_ext_value( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::Boolean _par_is_custom, CORBA::Boolean _par_is_abstract, CORBA::ValueDef_ptr _par_base_value, CORBA::Boolean _par_is_truncatable, const CORBA::ValueDefSeq& _par_abstract_base_values, const CORBA::InterfaceDefSeq& _par_supported_interfaces, const CORBA::ExtInitializerSeq& _par_initializers )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ExtValueDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_ext_value(_par_id, _par_name, _par_version, _par_is_custom, _par_is_abstract, _par_base_value, _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers);
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

  return CORBA::Container_stub::create_ext_value(_par_id, _par_name, _par_version, _par_is_custom, _par_is_abstract, _par_base_value, _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers);
}

#endif // MICO_CONF_NO_POA

CORBA::ValueBoxDef_ptr CORBA::Container_stub::create_value_box( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_original_type_def )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_original_type_def( _marshaller_CORBA_IDLType, &_par_original_type_def );
  CORBA::ValueBoxDef_ptr _res = CORBA::ValueBoxDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ValueBoxDef, &_res );

  CORBA::StaticRequest __req( this, "create_value_box" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_original_type_def );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ValueBoxDef_ptr
CORBA::Container_stub_clp::create_value_box( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_original_type_def )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueBoxDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_value_box(_par_id, _par_name, _par_version, _par_original_type_def);
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

  return CORBA::Container_stub::create_value_box(_par_id, _par_name, _par_version, _par_original_type_def);
}

#endif // MICO_CONF_NO_POA

CORBA::NativeDef_ptr CORBA::Container_stub::create_native( const char* _par_id, const char* _par_name, const char* _par_version )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::NativeDef_ptr _res = CORBA::NativeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_NativeDef, &_res );

  CORBA::StaticRequest __req( this, "create_native" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::NativeDef_ptr
CORBA::Container_stub_clp::create_native( const char* _par_id, const char* _par_name, const char* _par_version )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Container * _myserv = POA_CORBA::Container::_narrow (_serv);
    if (_myserv) {
      CORBA::NativeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_native(_par_id, _par_name, _par_version);
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

  return CORBA::Container_stub::create_native(_par_id, _par_name, _par_version);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_PrimitiveKind;
}

void operator<<=( CORBA::Any &_a, const CORBA::PrimitiveKind &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_PrimitiveKind, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::PrimitiveKind &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_PrimitiveKind, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_CORBA_PrimitiveKind : public ::CORBA::StaticTypeInfo {
    typedef CORBA::PrimitiveKind _MICO_T;
  public:
    ~_Marshaller_CORBA_PrimitiveKind();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_PrimitiveKind::~_Marshaller_CORBA_PrimitiveKind()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_PrimitiveKind::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_PrimitiveKind::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_PrimitiveKind::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_PrimitiveKind::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_CORBA_PrimitiveKind::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_PrimitiveKind::typecode()
{
  return CORBA::_tc_PrimitiveKind;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_PrimitiveKind;


/*
 * Base interface for class Repository
 */

CORBA::Repository::~Repository()
{
}

void *
CORBA::Repository::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/Repository:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::Repository_ptr
CORBA::Repository::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::Repository_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/Repository:1.0" )))
      return _duplicate( (CORBA::Repository_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/Repository:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/Repository:1.0")) {
      _o = new CORBA::Repository_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::Repository_ptr
CORBA::Repository::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_Repository;
}
class _Marshaller_CORBA_Repository : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Repository_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_Repository();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_Repository::~_Marshaller_CORBA_Repository()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_Repository::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_Repository::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::Repository::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_Repository::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_Repository::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_Repository::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::Repository::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_Repository::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_Repository::typecode()
{
  return CORBA::_tc_Repository;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_Repository;

void
operator<<=( CORBA::Any &_a, const CORBA::Repository_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Repository, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::Repository_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Repository, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::Repository_ptr &_obj )
{
  CORBA::Repository_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_Repository, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class Repository
 */

CORBA::Repository_stub::~Repository_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::Repository::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/Repository:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::Repository *
POA_CORBA::Repository::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/Repository:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::Repository *) p;
  }
  return NULL;
}

CORBA::Repository_stub_clp::Repository_stub_clp ()
{
}

CORBA::Repository_stub_clp::Repository_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::Repository_stub_clp::~Repository_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::Contained_ptr CORBA::Repository_stub::lookup_id( const char* _par_search_id )
{
  CORBA::StaticAny _sa_search_id( CORBA::_stc_string, &_par_search_id );
  CORBA::Contained_ptr _res = CORBA::Contained::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_Contained, &_res );

  CORBA::StaticRequest __req( this, "lookup_id" );
  __req.add_in_arg( &_sa_search_id );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Contained_ptr
CORBA::Repository_stub_clp::lookup_id( const char* _par_search_id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::Contained_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->lookup_id(_par_search_id);
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

  return CORBA::Repository_stub::lookup_id(_par_search_id);
}

#endif // MICO_CONF_NO_POA

CORBA::PrimitiveDef_ptr CORBA::Repository_stub::get_primitive( CORBA::PrimitiveKind _par_kind )
{
  CORBA::StaticAny _sa_kind( _marshaller_CORBA_PrimitiveKind, &_par_kind );
  CORBA::PrimitiveDef_ptr _res = CORBA::PrimitiveDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_PrimitiveDef, &_res );

  CORBA::StaticRequest __req( this, "get_primitive" );
  __req.add_in_arg( &_sa_kind );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::PrimitiveDef_ptr
CORBA::Repository_stub_clp::get_primitive( CORBA::PrimitiveKind _par_kind )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::PrimitiveDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_primitive(_par_kind);
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

  return CORBA::Repository_stub::get_primitive(_par_kind);
}

#endif // MICO_CONF_NO_POA

CORBA::StringDef_ptr CORBA::Repository_stub::create_string( CORBA::ULong _par_bound )
{
  CORBA::StaticAny _sa_bound( CORBA::_stc_ulong, &_par_bound );
  CORBA::StringDef_ptr _res = CORBA::StringDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_StringDef, &_res );

  CORBA::StaticRequest __req( this, "create_string" );
  __req.add_in_arg( &_sa_bound );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::StringDef_ptr
CORBA::Repository_stub_clp::create_string( CORBA::ULong _par_bound )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::StringDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_string(_par_bound);
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

  return CORBA::Repository_stub::create_string(_par_bound);
}

#endif // MICO_CONF_NO_POA

CORBA::WstringDef_ptr CORBA::Repository_stub::create_wstring( CORBA::ULong _par_bound )
{
  CORBA::StaticAny _sa_bound( CORBA::_stc_ulong, &_par_bound );
  CORBA::WstringDef_ptr _res = CORBA::WstringDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_WstringDef, &_res );

  CORBA::StaticRequest __req( this, "create_wstring" );
  __req.add_in_arg( &_sa_bound );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::WstringDef_ptr
CORBA::Repository_stub_clp::create_wstring( CORBA::ULong _par_bound )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::WstringDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_wstring(_par_bound);
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

  return CORBA::Repository_stub::create_wstring(_par_bound);
}

#endif // MICO_CONF_NO_POA

CORBA::SequenceDef_ptr CORBA::Repository_stub::create_sequence( CORBA::ULong _par_bound, CORBA::IDLType_ptr _par_element_type )
{
  CORBA::StaticAny _sa_bound( CORBA::_stc_ulong, &_par_bound );
  CORBA::StaticAny _sa_element_type( _marshaller_CORBA_IDLType, &_par_element_type );
  CORBA::SequenceDef_ptr _res = CORBA::SequenceDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_SequenceDef, &_res );

  CORBA::StaticRequest __req( this, "create_sequence" );
  __req.add_in_arg( &_sa_bound );
  __req.add_in_arg( &_sa_element_type );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::SequenceDef_ptr
CORBA::Repository_stub_clp::create_sequence( CORBA::ULong _par_bound, CORBA::IDLType_ptr _par_element_type )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::SequenceDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_sequence(_par_bound, _par_element_type);
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

  return CORBA::Repository_stub::create_sequence(_par_bound, _par_element_type);
}

#endif // MICO_CONF_NO_POA

CORBA::ArrayDef_ptr CORBA::Repository_stub::create_array( CORBA::ULong _par_length, CORBA::IDLType_ptr _par_element_type )
{
  CORBA::StaticAny _sa_length( CORBA::_stc_ulong, &_par_length );
  CORBA::StaticAny _sa_element_type( _marshaller_CORBA_IDLType, &_par_element_type );
  CORBA::ArrayDef_ptr _res = CORBA::ArrayDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ArrayDef, &_res );

  CORBA::StaticRequest __req( this, "create_array" );
  __req.add_in_arg( &_sa_length );
  __req.add_in_arg( &_sa_element_type );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ArrayDef_ptr
CORBA::Repository_stub_clp::create_array( CORBA::ULong _par_length, CORBA::IDLType_ptr _par_element_type )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::ArrayDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_array(_par_length, _par_element_type);
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

  return CORBA::Repository_stub::create_array(_par_length, _par_element_type);
}

#endif // MICO_CONF_NO_POA

CORBA::FixedDef_ptr CORBA::Repository_stub::create_fixed( CORBA::UShort _par_digits, CORBA::Short _par_scale )
{
  CORBA::StaticAny _sa_digits( CORBA::_stc_ushort, &_par_digits );
  CORBA::StaticAny _sa_scale( CORBA::_stc_short, &_par_scale );
  CORBA::FixedDef_ptr _res = CORBA::FixedDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_FixedDef, &_res );

  CORBA::StaticRequest __req( this, "create_fixed" );
  __req.add_in_arg( &_sa_digits );
  __req.add_in_arg( &_sa_scale );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::FixedDef_ptr
CORBA::Repository_stub_clp::create_fixed( CORBA::UShort _par_digits, CORBA::Short _par_scale )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::Repository * _myserv = POA_CORBA::Repository::_narrow (_serv);
    if (_myserv) {
      CORBA::FixedDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_fixed(_par_digits, _par_scale);
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

  return CORBA::Repository_stub::create_fixed(_par_digits, _par_scale);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ModuleDef
 */

CORBA::ModuleDef::~ModuleDef()
{
}

void *
CORBA::ModuleDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ModuleDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ModuleDef_ptr
CORBA::ModuleDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ModuleDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ModuleDef:1.0" )))
      return _duplicate( (CORBA::ModuleDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ModuleDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ModuleDef:1.0")) {
      _o = new CORBA::ModuleDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ModuleDef_ptr
CORBA::ModuleDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ModuleDef;
}
class _Marshaller_CORBA_ModuleDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ModuleDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ModuleDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ModuleDef::~_Marshaller_CORBA_ModuleDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ModuleDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ModuleDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ModuleDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ModuleDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ModuleDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ModuleDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ModuleDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ModuleDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ModuleDef::typecode()
{
  return CORBA::_tc_ModuleDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ModuleDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ModuleDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ModuleDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ModuleDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ModuleDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ModuleDef_ptr &_obj )
{
  CORBA::ModuleDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ModuleDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ModuleDef
 */

CORBA::ModuleDef_stub::~ModuleDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ModuleDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ModuleDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ModuleDef *
POA_CORBA::ModuleDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ModuleDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ModuleDef *) p;
  }
  return NULL;
}

CORBA::ModuleDef_stub_clp::ModuleDef_stub_clp ()
{
}

CORBA::ModuleDef_stub_clp::ModuleDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ModuleDef_stub_clp::~ModuleDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ConstantDef
 */

CORBA::ConstantDef::~ConstantDef()
{
}

void *
CORBA::ConstantDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ConstantDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ConstantDef_ptr
CORBA::ConstantDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ConstantDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ConstantDef:1.0" )))
      return _duplicate( (CORBA::ConstantDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ConstantDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ConstantDef:1.0")) {
      _o = new CORBA::ConstantDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ConstantDef_ptr
CORBA::ConstantDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ConstantDef;
}
class _Marshaller_CORBA_ConstantDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ConstantDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ConstantDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ConstantDef::~_Marshaller_CORBA_ConstantDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ConstantDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ConstantDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ConstantDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ConstantDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ConstantDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ConstantDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ConstantDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ConstantDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ConstantDef::typecode()
{
  return CORBA::_tc_ConstantDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ConstantDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ConstantDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ConstantDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ConstantDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ConstantDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ConstantDef_ptr &_obj )
{
  CORBA::ConstantDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ConstantDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ConstantDef
 */

CORBA::ConstantDef_stub::~ConstantDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ConstantDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ConstantDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ConstantDef *
POA_CORBA::ConstantDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ConstantDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ConstantDef *) p;
  }
  return NULL;
}

CORBA::ConstantDef_stub_clp::ConstantDef_stub_clp ()
{
}

CORBA::ConstantDef_stub_clp::ConstantDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ConstantDef_stub_clp::~ConstantDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::ConstantDef_stub::type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::ConstantDef_stub_clp::type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ConstantDef * _myserv = POA_CORBA::ConstantDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type();
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

  return CORBA::ConstantDef_stub::type();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::ConstantDef_stub::type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::ConstantDef_stub_clp::type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ConstantDef * _myserv = POA_CORBA::ConstantDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type_def();
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

  return CORBA::ConstantDef_stub::type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::ConstantDef_stub::type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ConstantDef_stub_clp::type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ConstantDef * _myserv = POA_CORBA::ConstantDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->type_def(_par__value);
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

  CORBA::ConstantDef_stub::type_def(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Any* CORBA::ConstantDef_stub::value()
{
  CORBA::StaticAny __res( CORBA::_stc_any );

  CORBA::StaticRequest __req( this, "_get_value" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::Any*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::Any*
CORBA::ConstantDef_stub_clp::value()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ConstantDef * _myserv = POA_CORBA::ConstantDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Any* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->value();
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

  return CORBA::ConstantDef_stub::value();
}

#endif // MICO_CONF_NO_POA

void CORBA::ConstantDef_stub::value( const CORBA::Any& _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_any, &_par__value );
  CORBA::StaticRequest __req( this, "_set_value" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ConstantDef_stub_clp::value( const CORBA::Any& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ConstantDef * _myserv = POA_CORBA::ConstantDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->value(_par__value);
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

  CORBA::ConstantDef_stub::value(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class TypedefDef
 */

CORBA::TypedefDef::~TypedefDef()
{
}

void *
CORBA::TypedefDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/TypedefDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::TypedefDef_ptr
CORBA::TypedefDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::TypedefDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/TypedefDef:1.0" )))
      return _duplicate( (CORBA::TypedefDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/TypedefDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/TypedefDef:1.0")) {
      _o = new CORBA::TypedefDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::TypedefDef_ptr
CORBA::TypedefDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_TypedefDef;
}
class _Marshaller_CORBA_TypedefDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::TypedefDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_TypedefDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_TypedefDef::~_Marshaller_CORBA_TypedefDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_TypedefDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_TypedefDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::TypedefDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_TypedefDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_TypedefDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_TypedefDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::TypedefDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_TypedefDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_TypedefDef::typecode()
{
  return CORBA::_tc_TypedefDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_TypedefDef;

void
operator<<=( CORBA::Any &_a, const CORBA::TypedefDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_TypedefDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::TypedefDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_TypedefDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::TypedefDef_ptr &_obj )
{
  CORBA::TypedefDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_TypedefDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class TypedefDef
 */

CORBA::TypedefDef_stub::~TypedefDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::TypedefDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/TypedefDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::TypedefDef *
POA_CORBA::TypedefDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/TypedefDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::TypedefDef *) p;
  }
  return NULL;
}

CORBA::TypedefDef_stub_clp::TypedefDef_stub_clp ()
{
}

CORBA::TypedefDef_stub_clp::TypedefDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::TypedefDef_stub_clp::~TypedefDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class StructDef
 */

CORBA::StructDef::~StructDef()
{
}

void *
CORBA::StructDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/StructDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::TypedefDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::StructDef_ptr
CORBA::StructDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::StructDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/StructDef:1.0" )))
      return _duplicate( (CORBA::StructDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/StructDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/StructDef:1.0")) {
      _o = new CORBA::StructDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::StructDef_ptr
CORBA::StructDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_StructDef;
}
class _Marshaller_CORBA_StructDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::StructDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_StructDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_StructDef::~_Marshaller_CORBA_StructDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_StructDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_StructDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::StructDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_StructDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_StructDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_StructDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::StructDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_StructDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_StructDef::typecode()
{
  return CORBA::_tc_StructDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_StructDef;

void
operator<<=( CORBA::Any &_a, const CORBA::StructDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_StructDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::StructDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_StructDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::StructDef_ptr &_obj )
{
  CORBA::StructDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_StructDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class StructDef
 */

CORBA::StructDef_stub::~StructDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::StructDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/StructDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::TypedefDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::StructDef *
POA_CORBA::StructDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/StructDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::StructDef *) p;
  }
  return NULL;
}

CORBA::StructDef_stub_clp::StructDef_stub_clp ()
{
}

CORBA::StructDef_stub_clp::StructDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::StructDef_stub_clp::~StructDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::StructMemberSeq* CORBA::StructDef_stub::members()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_StructMember );

  CORBA::StaticRequest __req( this, "_get_members" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::StructMemberSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::StructMemberSeq*
CORBA::StructDef_stub_clp::members()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::StructDef * _myserv = POA_CORBA::StructDef::_narrow (_serv);
    if (_myserv) {
      CORBA::StructMemberSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->members();
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

  return CORBA::StructDef_stub::members();
}

#endif // MICO_CONF_NO_POA

void CORBA::StructDef_stub::members( const CORBA::StructMemberSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_StructMember, &_par__value );
  CORBA::StaticRequest __req( this, "_set_members" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::StructDef_stub_clp::members( const CORBA::StructMemberSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::StructDef * _myserv = POA_CORBA::StructDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->members(_par__value);
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

  CORBA::StructDef_stub::members(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ExceptionDef
 */

CORBA::ExceptionDef::~ExceptionDef()
{
}

void *
CORBA::ExceptionDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ExceptionDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ExceptionDef_ptr
CORBA::ExceptionDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ExceptionDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ExceptionDef:1.0" )))
      return _duplicate( (CORBA::ExceptionDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ExceptionDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ExceptionDef:1.0")) {
      _o = new CORBA::ExceptionDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ExceptionDef_ptr
CORBA::ExceptionDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExceptionDef;
}
class _Marshaller_CORBA_ExceptionDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExceptionDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ExceptionDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExceptionDef::~_Marshaller_CORBA_ExceptionDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExceptionDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ExceptionDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ExceptionDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ExceptionDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ExceptionDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ExceptionDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ExceptionDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ExceptionDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExceptionDef::typecode()
{
  return CORBA::_tc_ExceptionDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExceptionDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ExceptionDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExceptionDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ExceptionDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExceptionDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ExceptionDef_ptr &_obj )
{
  CORBA::ExceptionDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ExceptionDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ExceptionDef
 */

CORBA::ExceptionDef_stub::~ExceptionDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ExceptionDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExceptionDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ExceptionDef *
POA_CORBA::ExceptionDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ExceptionDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ExceptionDef *) p;
  }
  return NULL;
}

CORBA::ExceptionDef_stub_clp::ExceptionDef_stub_clp ()
{
}

CORBA::ExceptionDef_stub_clp::ExceptionDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ExceptionDef_stub_clp::~ExceptionDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::ExceptionDef_stub::type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::ExceptionDef_stub_clp::type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExceptionDef * _myserv = POA_CORBA::ExceptionDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type();
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

  return CORBA::ExceptionDef_stub::type();
}

#endif // MICO_CONF_NO_POA

CORBA::StructMemberSeq* CORBA::ExceptionDef_stub::members()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_StructMember );

  CORBA::StaticRequest __req( this, "_get_members" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::StructMemberSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::StructMemberSeq*
CORBA::ExceptionDef_stub_clp::members()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExceptionDef * _myserv = POA_CORBA::ExceptionDef::_narrow (_serv);
    if (_myserv) {
      CORBA::StructMemberSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->members();
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

  return CORBA::ExceptionDef_stub::members();
}

#endif // MICO_CONF_NO_POA

void CORBA::ExceptionDef_stub::members( const CORBA::StructMemberSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_StructMember, &_par__value );
  CORBA::StaticRequest __req( this, "_set_members" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ExceptionDef_stub_clp::members( const CORBA::StructMemberSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExceptionDef * _myserv = POA_CORBA::ExceptionDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->members(_par__value);
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

  CORBA::ExceptionDef_stub::members(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class UnionDef
 */

CORBA::UnionDef::~UnionDef()
{
}

void *
CORBA::UnionDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/UnionDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::TypedefDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::UnionDef_ptr
CORBA::UnionDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::UnionDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/UnionDef:1.0" )))
      return _duplicate( (CORBA::UnionDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/UnionDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/UnionDef:1.0")) {
      _o = new CORBA::UnionDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::UnionDef_ptr
CORBA::UnionDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_UnionDef;
}
class _Marshaller_CORBA_UnionDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::UnionDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_UnionDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_UnionDef::~_Marshaller_CORBA_UnionDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_UnionDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_UnionDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::UnionDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_UnionDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_UnionDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_UnionDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::UnionDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_UnionDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_UnionDef::typecode()
{
  return CORBA::_tc_UnionDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_UnionDef;

void
operator<<=( CORBA::Any &_a, const CORBA::UnionDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_UnionDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::UnionDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_UnionDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::UnionDef_ptr &_obj )
{
  CORBA::UnionDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_UnionDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class UnionDef
 */

CORBA::UnionDef_stub::~UnionDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::UnionDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/UnionDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::TypedefDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::UnionDef *
POA_CORBA::UnionDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/UnionDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::UnionDef *) p;
  }
  return NULL;
}

CORBA::UnionDef_stub_clp::UnionDef_stub_clp ()
{
}

CORBA::UnionDef_stub_clp::UnionDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::UnionDef_stub_clp::~UnionDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::UnionDef_stub::discriminator_type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_discriminator_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::UnionDef_stub_clp::discriminator_type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::UnionDef * _myserv = POA_CORBA::UnionDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->discriminator_type();
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

  return CORBA::UnionDef_stub::discriminator_type();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::UnionDef_stub::discriminator_type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_discriminator_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::UnionDef_stub_clp::discriminator_type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::UnionDef * _myserv = POA_CORBA::UnionDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->discriminator_type_def();
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

  return CORBA::UnionDef_stub::discriminator_type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::UnionDef_stub::discriminator_type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_discriminator_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::UnionDef_stub_clp::discriminator_type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::UnionDef * _myserv = POA_CORBA::UnionDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->discriminator_type_def(_par__value);
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

  CORBA::UnionDef_stub::discriminator_type_def(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::UnionMemberSeq* CORBA::UnionDef_stub::members()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_UnionMember );

  CORBA::StaticRequest __req( this, "_get_members" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::UnionMemberSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::UnionMemberSeq*
CORBA::UnionDef_stub_clp::members()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::UnionDef * _myserv = POA_CORBA::UnionDef::_narrow (_serv);
    if (_myserv) {
      CORBA::UnionMemberSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->members();
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

  return CORBA::UnionDef_stub::members();
}

#endif // MICO_CONF_NO_POA

void CORBA::UnionDef_stub::members( const CORBA::UnionMemberSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_UnionMember, &_par__value );
  CORBA::StaticRequest __req( this, "_set_members" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::UnionDef_stub_clp::members( const CORBA::UnionMemberSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::UnionDef * _myserv = POA_CORBA::UnionDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->members(_par__value);
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

  CORBA::UnionDef_stub::members(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class EnumDef
 */

CORBA::EnumDef::~EnumDef()
{
}

void *
CORBA::EnumDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/EnumDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::TypedefDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::EnumDef_ptr
CORBA::EnumDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::EnumDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/EnumDef:1.0" )))
      return _duplicate( (CORBA::EnumDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/EnumDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/EnumDef:1.0")) {
      _o = new CORBA::EnumDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::EnumDef_ptr
CORBA::EnumDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_EnumDef;
}
class _Marshaller_CORBA_EnumDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::EnumDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_EnumDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_EnumDef::~_Marshaller_CORBA_EnumDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_EnumDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_EnumDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::EnumDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_EnumDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_EnumDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_EnumDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::EnumDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_EnumDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_EnumDef::typecode()
{
  return CORBA::_tc_EnumDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_EnumDef;

void
operator<<=( CORBA::Any &_a, const CORBA::EnumDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_EnumDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::EnumDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_EnumDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::EnumDef_ptr &_obj )
{
  CORBA::EnumDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_EnumDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class EnumDef
 */

CORBA::EnumDef_stub::~EnumDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::EnumDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/EnumDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::TypedefDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::EnumDef *
POA_CORBA::EnumDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/EnumDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::EnumDef *) p;
  }
  return NULL;
}

CORBA::EnumDef_stub_clp::EnumDef_stub_clp ()
{
}

CORBA::EnumDef_stub_clp::EnumDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::EnumDef_stub_clp::~EnumDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::EnumMemberSeq* CORBA::EnumDef_stub::members()
{
  CORBA::StaticAny __res( CORBA::_stcseq_string );

  CORBA::StaticRequest __req( this, "_get_members" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::EnumMemberSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::EnumMemberSeq*
CORBA::EnumDef_stub_clp::members()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::EnumDef * _myserv = POA_CORBA::EnumDef::_narrow (_serv);
    if (_myserv) {
      CORBA::EnumMemberSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->members();
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

  return CORBA::EnumDef_stub::members();
}

#endif // MICO_CONF_NO_POA

void CORBA::EnumDef_stub::members( const CORBA::EnumMemberSeq& _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stcseq_string, &_par__value );
  CORBA::StaticRequest __req( this, "_set_members" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::EnumDef_stub_clp::members( const CORBA::EnumMemberSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::EnumDef * _myserv = POA_CORBA::EnumDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->members(_par__value);
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

  CORBA::EnumDef_stub::members(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class AliasDef
 */

CORBA::AliasDef::~AliasDef()
{
}

void *
CORBA::AliasDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/AliasDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::TypedefDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::AliasDef_ptr
CORBA::AliasDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::AliasDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/AliasDef:1.0" )))
      return _duplicate( (CORBA::AliasDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/AliasDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/AliasDef:1.0")) {
      _o = new CORBA::AliasDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::AliasDef_ptr
CORBA::AliasDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_AliasDef;
}
class _Marshaller_CORBA_AliasDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::AliasDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_AliasDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_AliasDef::~_Marshaller_CORBA_AliasDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_AliasDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_AliasDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::AliasDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_AliasDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_AliasDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_AliasDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::AliasDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_AliasDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_AliasDef::typecode()
{
  return CORBA::_tc_AliasDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_AliasDef;

void
operator<<=( CORBA::Any &_a, const CORBA::AliasDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AliasDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::AliasDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AliasDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::AliasDef_ptr &_obj )
{
  CORBA::AliasDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_AliasDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class AliasDef
 */

CORBA::AliasDef_stub::~AliasDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::AliasDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/AliasDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::TypedefDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::AliasDef *
POA_CORBA::AliasDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/AliasDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::AliasDef *) p;
  }
  return NULL;
}

CORBA::AliasDef_stub_clp::AliasDef_stub_clp ()
{
}

CORBA::AliasDef_stub_clp::AliasDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::AliasDef_stub_clp::~AliasDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::AliasDef_stub::original_type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_original_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::AliasDef_stub_clp::original_type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AliasDef * _myserv = POA_CORBA::AliasDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->original_type_def();
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

  return CORBA::AliasDef_stub::original_type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::AliasDef_stub::original_type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_original_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::AliasDef_stub_clp::original_type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AliasDef * _myserv = POA_CORBA::AliasDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->original_type_def(_par__value);
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

  CORBA::AliasDef_stub::original_type_def(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_OpDescriptionSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_AttrDescriptionSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_RepositoryIdSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_AttributeMode;
}

void operator<<=( CORBA::Any &_a, const CORBA::AttributeMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AttributeMode, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::AttributeMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AttributeMode, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_CORBA_AttributeMode : public ::CORBA::StaticTypeInfo {
    typedef CORBA::AttributeMode _MICO_T;
  public:
    ~_Marshaller_CORBA_AttributeMode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_AttributeMode::~_Marshaller_CORBA_AttributeMode()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_AttributeMode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_AttributeMode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_AttributeMode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_AttributeMode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_CORBA_AttributeMode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_AttributeMode::typecode()
{
  return CORBA::_tc_AttributeMode;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_AttributeMode;

namespace CORBA
{
CORBA::TypeCodeConst _tc_OperationMode;
}

void operator<<=( CORBA::Any &_a, const CORBA::OperationMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_OperationMode, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::OperationMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_OperationMode, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_CORBA_OperationMode : public ::CORBA::StaticTypeInfo {
    typedef CORBA::OperationMode _MICO_T;
  public:
    ~_Marshaller_CORBA_OperationMode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_OperationMode::~_Marshaller_CORBA_OperationMode()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_OperationMode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_OperationMode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_OperationMode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_OperationMode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_CORBA_OperationMode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_OperationMode::typecode()
{
  return CORBA::_tc_OperationMode;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_OperationMode;

namespace CORBA
{
CORBA::TypeCodeConst _tc_ParDescriptionSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExceptionDefSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ContextIdentifier;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ContextIdSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst InterfaceDef::_tc_FullInterfaceDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::InterfaceDef::FullInterfaceDescription::FullInterfaceDescription()
{
}

CORBA::InterfaceDef::FullInterfaceDescription::FullInterfaceDescription( const FullInterfaceDescription& _s )
{
  name = ((FullInterfaceDescription&)_s).name;
  id = ((FullInterfaceDescription&)_s).id;
  defined_in = ((FullInterfaceDescription&)_s).defined_in;
  version = ((FullInterfaceDescription&)_s).version;
  operations = ((FullInterfaceDescription&)_s).operations;
  attributes = ((FullInterfaceDescription&)_s).attributes;
  base_interfaces = ((FullInterfaceDescription&)_s).base_interfaces;
  type = ((FullInterfaceDescription&)_s).type;
}

CORBA::InterfaceDef::FullInterfaceDescription::~FullInterfaceDescription()
{
}

CORBA::InterfaceDef::FullInterfaceDescription&
CORBA::InterfaceDef::FullInterfaceDescription::operator=( const FullInterfaceDescription& _s )
{
  name = ((FullInterfaceDescription&)_s).name;
  id = ((FullInterfaceDescription&)_s).id;
  defined_in = ((FullInterfaceDescription&)_s).defined_in;
  version = ((FullInterfaceDescription&)_s).version;
  operations = ((FullInterfaceDescription&)_s).operations;
  attributes = ((FullInterfaceDescription&)_s).attributes;
  base_interfaces = ((FullInterfaceDescription&)_s).base_interfaces;
  type = ((FullInterfaceDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::InterfaceDef::FullInterfaceDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_InterfaceDef_FullInterfaceDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::~_Marshaller_CORBA_InterfaceDef_FullInterfaceDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->operations ) &&
    _marshaller__seq_CORBA_AttributeDescription->demarshal( dc, &((_MICO_T*)v)->attributes ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->base_interfaces ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->operations );
  _marshaller__seq_CORBA_AttributeDescription->marshal( ec, &((_MICO_T*)v)->attributes );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->base_interfaces );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription::typecode()
{
  return CORBA::InterfaceDef::_tc_FullInterfaceDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceDef_FullInterfaceDescription;

void operator<<=( CORBA::Any &_a, const CORBA::InterfaceDef::FullInterfaceDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceDef_FullInterfaceDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::InterfaceDef::FullInterfaceDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::InterfaceDef::FullInterfaceDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceDef_FullInterfaceDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::InterfaceDef::FullInterfaceDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_InterfaceDef_FullInterfaceDescription, (void *&)_s);
}


/*
 * Base interface for class InterfaceDef
 */

CORBA::InterfaceDef::~InterfaceDef()
{
}

void *
CORBA::InterfaceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/InterfaceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::InterfaceDef_ptr
CORBA::InterfaceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::InterfaceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/InterfaceDef:1.0" )))
      return _duplicate( (CORBA::InterfaceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/InterfaceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/InterfaceDef:1.0")) {
      _o = new CORBA::InterfaceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::InterfaceDef_ptr
CORBA::InterfaceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_InterfaceDef;
}
class _Marshaller_CORBA_InterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::InterfaceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_InterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_InterfaceDef::~_Marshaller_CORBA_InterfaceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_InterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_InterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::InterfaceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_InterfaceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_InterfaceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_InterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::InterfaceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_InterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_InterfaceDef::typecode()
{
  return CORBA::_tc_InterfaceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::InterfaceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::InterfaceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::InterfaceDef_ptr &_obj )
{
  CORBA::InterfaceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_InterfaceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class InterfaceDef
 */

CORBA::InterfaceDef_stub::~InterfaceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::InterfaceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/InterfaceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::InterfaceDef *
POA_CORBA::InterfaceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/InterfaceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::InterfaceDef *) p;
  }
  return NULL;
}

CORBA::InterfaceDef_stub_clp::InterfaceDef_stub_clp ()
{
}

CORBA::InterfaceDef_stub_clp::InterfaceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::InterfaceDef_stub_clp::~InterfaceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDefSeq* CORBA::InterfaceDef_stub::base_interfaces()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_InterfaceDef );

  CORBA::StaticRequest __req( this, "_get_base_interfaces" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::InterfaceDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDefSeq*
CORBA::InterfaceDef_stub_clp::base_interfaces()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceDef * _myserv = POA_CORBA::InterfaceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->base_interfaces();
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

  return CORBA::InterfaceDef_stub::base_interfaces();
}

#endif // MICO_CONF_NO_POA

void CORBA::InterfaceDef_stub::base_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_InterfaceDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_base_interfaces" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::InterfaceDef_stub_clp::base_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceDef * _myserv = POA_CORBA::InterfaceDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->base_interfaces(_par__value);
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

  CORBA::InterfaceDef_stub::base_interfaces(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::InterfaceDef_stub::is_a( const char* _par_interface_id )
{
  CORBA::StaticAny _sa_interface_id( CORBA::_stc_string, &_par_interface_id );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "is_a" );
  __req.add_in_arg( &_sa_interface_id );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::InterfaceDef_stub_clp::is_a( const char* _par_interface_id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceDef * _myserv = POA_CORBA::InterfaceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_a(_par_interface_id);
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

  return CORBA::InterfaceDef_stub::is_a(_par_interface_id);
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDef::FullInterfaceDescription* CORBA::InterfaceDef_stub::describe_interface()
{
  CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef_FullInterfaceDescription );

  CORBA::StaticRequest __req( this, "describe_interface" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::InterfaceDef::FullInterfaceDescription*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceDef::FullInterfaceDescription*
CORBA::InterfaceDef_stub_clp::describe_interface()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceDef * _myserv = POA_CORBA::InterfaceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceDef::FullInterfaceDescription* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe_interface();
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

  return CORBA::InterfaceDef_stub::describe_interface();
}

#endif // MICO_CONF_NO_POA

CORBA::AttributeDef_ptr CORBA::InterfaceDef_stub::create_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type );
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );
  CORBA::AttributeDef_ptr _res = CORBA::AttributeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_AttributeDef, &_res );

  CORBA::StaticRequest __req( this, "create_attribute" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_type );
  __req.add_in_arg( &_sa_mode );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::AttributeDef_ptr
CORBA::InterfaceDef_stub_clp::create_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceDef * _myserv = POA_CORBA::InterfaceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::AttributeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode);
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

  return CORBA::InterfaceDef_stub::create_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode);
}

#endif // MICO_CONF_NO_POA

CORBA::OperationDef_ptr CORBA::InterfaceDef_stub::create_operation( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_result, CORBA::OperationMode _par_mode, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions, const CORBA::ContextIdSeq& _par_contexts )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_result( _marshaller_CORBA_IDLType, &_par_result );
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_OperationMode, &_par_mode );
  CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
  CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );
  CORBA::StaticAny _sa_contexts( CORBA::_stcseq_string, &_par_contexts );
  CORBA::OperationDef_ptr _res = CORBA::OperationDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_OperationDef, &_res );

  CORBA::StaticRequest __req( this, "create_operation" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_result );
  __req.add_in_arg( &_sa_mode );
  __req.add_in_arg( &_sa_params );
  __req.add_in_arg( &_sa_exceptions );
  __req.add_in_arg( &_sa_contexts );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::OperationDef_ptr
CORBA::InterfaceDef_stub_clp::create_operation( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_result, CORBA::OperationMode _par_mode, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions, const CORBA::ContextIdSeq& _par_contexts )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceDef * _myserv = POA_CORBA::InterfaceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::OperationDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_operation(_par_id, _par_name, _par_version, _par_result, _par_mode, _par_params, _par_exceptions, _par_contexts);
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

  return CORBA::InterfaceDef_stub::create_operation(_par_id, _par_name, _par_version, _par_result, _par_mode, _par_params, _par_exceptions, _par_contexts);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class AbstractInterfaceDef
 */

CORBA::AbstractInterfaceDef::~AbstractInterfaceDef()
{
}

void *
CORBA::AbstractInterfaceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/AbstractInterfaceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::InterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::AbstractInterfaceDef_ptr
CORBA::AbstractInterfaceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::AbstractInterfaceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/AbstractInterfaceDef:1.0" )))
      return _duplicate( (CORBA::AbstractInterfaceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/AbstractInterfaceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/AbstractInterfaceDef:1.0")) {
      _o = new CORBA::AbstractInterfaceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::AbstractInterfaceDef_ptr
CORBA::AbstractInterfaceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_AbstractInterfaceDef;
}
class _Marshaller_CORBA_AbstractInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::AbstractInterfaceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_AbstractInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_AbstractInterfaceDef::~_Marshaller_CORBA_AbstractInterfaceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_AbstractInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_AbstractInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::AbstractInterfaceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_AbstractInterfaceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_AbstractInterfaceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_AbstractInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::AbstractInterfaceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_AbstractInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_AbstractInterfaceDef::typecode()
{
  return CORBA::_tc_AbstractInterfaceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_AbstractInterfaceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::AbstractInterfaceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AbstractInterfaceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::AbstractInterfaceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AbstractInterfaceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::AbstractInterfaceDef_ptr &_obj )
{
  CORBA::AbstractInterfaceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_AbstractInterfaceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class AbstractInterfaceDef
 */

CORBA::AbstractInterfaceDef_stub::~AbstractInterfaceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::AbstractInterfaceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/AbstractInterfaceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::InterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::AbstractInterfaceDef *
POA_CORBA::AbstractInterfaceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/AbstractInterfaceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::AbstractInterfaceDef *) p;
  }
  return NULL;
}

CORBA::AbstractInterfaceDef_stub_clp::AbstractInterfaceDef_stub_clp ()
{
}

CORBA::AbstractInterfaceDef_stub_clp::AbstractInterfaceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::AbstractInterfaceDef_stub_clp::~AbstractInterfaceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class LocalInterfaceDef
 */

CORBA::LocalInterfaceDef::~LocalInterfaceDef()
{
}

void *
CORBA::LocalInterfaceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/LocalInterfaceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::InterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::LocalInterfaceDef_ptr
CORBA::LocalInterfaceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::LocalInterfaceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/LocalInterfaceDef:1.0" )))
      return _duplicate( (CORBA::LocalInterfaceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/LocalInterfaceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/LocalInterfaceDef:1.0")) {
      _o = new CORBA::LocalInterfaceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::LocalInterfaceDef_ptr
CORBA::LocalInterfaceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_LocalInterfaceDef;
}
class _Marshaller_CORBA_LocalInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::LocalInterfaceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_LocalInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_LocalInterfaceDef::~_Marshaller_CORBA_LocalInterfaceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_LocalInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_LocalInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::LocalInterfaceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_LocalInterfaceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_LocalInterfaceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_LocalInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::LocalInterfaceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_LocalInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_LocalInterfaceDef::typecode()
{
  return CORBA::_tc_LocalInterfaceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_LocalInterfaceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::LocalInterfaceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_LocalInterfaceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::LocalInterfaceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_LocalInterfaceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::LocalInterfaceDef_ptr &_obj )
{
  CORBA::LocalInterfaceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_LocalInterfaceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class LocalInterfaceDef
 */

CORBA::LocalInterfaceDef_stub::~LocalInterfaceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::LocalInterfaceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/LocalInterfaceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::InterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::LocalInterfaceDef *
POA_CORBA::LocalInterfaceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/LocalInterfaceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::LocalInterfaceDef *) p;
  }
  return NULL;
}

CORBA::LocalInterfaceDef_stub_clp::LocalInterfaceDef_stub_clp ()
{
}

CORBA::LocalInterfaceDef_stub_clp::LocalInterfaceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::LocalInterfaceDef_stub_clp::~LocalInterfaceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst ValueDef::_tc_FullValueDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ValueDef::FullValueDescription::FullValueDescription()
{
}

CORBA::ValueDef::FullValueDescription::FullValueDescription( const FullValueDescription& _s )
{
  name = ((FullValueDescription&)_s).name;
  id = ((FullValueDescription&)_s).id;
  is_abstract = ((FullValueDescription&)_s).is_abstract;
  is_custom = ((FullValueDescription&)_s).is_custom;
  defined_in = ((FullValueDescription&)_s).defined_in;
  version = ((FullValueDescription&)_s).version;
  operations = ((FullValueDescription&)_s).operations;
  attributes = ((FullValueDescription&)_s).attributes;
  members = ((FullValueDescription&)_s).members;
  initializers = ((FullValueDescription&)_s).initializers;
  supported_interfaces = ((FullValueDescription&)_s).supported_interfaces;
  abstract_base_values = ((FullValueDescription&)_s).abstract_base_values;
  is_truncatable = ((FullValueDescription&)_s).is_truncatable;
  base_value = ((FullValueDescription&)_s).base_value;
  type = ((FullValueDescription&)_s).type;
}

CORBA::ValueDef::FullValueDescription::~FullValueDescription()
{
}

CORBA::ValueDef::FullValueDescription&
CORBA::ValueDef::FullValueDescription::operator=( const FullValueDescription& _s )
{
  name = ((FullValueDescription&)_s).name;
  id = ((FullValueDescription&)_s).id;
  is_abstract = ((FullValueDescription&)_s).is_abstract;
  is_custom = ((FullValueDescription&)_s).is_custom;
  defined_in = ((FullValueDescription&)_s).defined_in;
  version = ((FullValueDescription&)_s).version;
  operations = ((FullValueDescription&)_s).operations;
  attributes = ((FullValueDescription&)_s).attributes;
  members = ((FullValueDescription&)_s).members;
  initializers = ((FullValueDescription&)_s).initializers;
  supported_interfaces = ((FullValueDescription&)_s).supported_interfaces;
  abstract_base_values = ((FullValueDescription&)_s).abstract_base_values;
  is_truncatable = ((FullValueDescription&)_s).is_truncatable;
  base_value = ((FullValueDescription&)_s).base_value;
  type = ((FullValueDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_ValueDef_FullValueDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ValueDef::FullValueDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ValueDef_FullValueDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ValueDef_FullValueDescription::~_Marshaller_CORBA_ValueDef_FullValueDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ValueDef_FullValueDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ValueDef_FullValueDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ValueDef_FullValueDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ValueDef_FullValueDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_abstract ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_custom ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->operations ) &&
    _marshaller__seq_CORBA_AttributeDescription->demarshal( dc, &((_MICO_T*)v)->attributes ) &&
    _marshaller__seq_CORBA_ValueMember->demarshal( dc, &((_MICO_T*)v)->members ) &&
    _marshaller__seq_CORBA_Initializer->demarshal( dc, &((_MICO_T*)v)->initializers ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->supported_interfaces ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->abstract_base_values ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_truncatable ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->base_value._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ValueDef_FullValueDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_abstract );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_custom );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->operations );
  _marshaller__seq_CORBA_AttributeDescription->marshal( ec, &((_MICO_T*)v)->attributes );
  _marshaller__seq_CORBA_ValueMember->marshal( ec, &((_MICO_T*)v)->members );
  _marshaller__seq_CORBA_Initializer->marshal( ec, &((_MICO_T*)v)->initializers );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->supported_interfaces );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->abstract_base_values );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_truncatable );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->base_value.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ValueDef_FullValueDescription::typecode()
{
  return CORBA::ValueDef::_tc_FullValueDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ValueDef_FullValueDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ValueDef::FullValueDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueDef_FullValueDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ValueDef::FullValueDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ValueDef::FullValueDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueDef_FullValueDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ValueDef::FullValueDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ValueDef_FullValueDescription, (void *&)_s);
}


/*
 * Base interface for class ValueDef
 */

CORBA::ValueDef::~ValueDef()
{
}

void *
CORBA::ValueDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ValueDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Container::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ValueDef_ptr
CORBA::ValueDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ValueDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ValueDef:1.0" )))
      return _duplicate( (CORBA::ValueDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ValueDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ValueDef:1.0")) {
      _o = new CORBA::ValueDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ValueDef_ptr
CORBA::ValueDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ValueDef;
}
class _Marshaller_CORBA_ValueDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ValueDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ValueDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ValueDef::~_Marshaller_CORBA_ValueDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ValueDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ValueDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ValueDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ValueDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ValueDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ValueDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ValueDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ValueDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ValueDef::typecode()
{
  return CORBA::_tc_ValueDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ValueDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ValueDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ValueDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ValueDef_ptr &_obj )
{
  CORBA::ValueDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ValueDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ValueDef
 */

CORBA::ValueDef_stub::~ValueDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ValueDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ValueDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Container::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ValueDef *
POA_CORBA::ValueDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ValueDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ValueDef *) p;
  }
  return NULL;
}

CORBA::ValueDef_stub_clp::ValueDef_stub_clp ()
{
}

CORBA::ValueDef_stub_clp::ValueDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ValueDef_stub_clp::~ValueDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceDefSeq* CORBA::ValueDef_stub::supported_interfaces()
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
CORBA::ValueDef_stub_clp::supported_interfaces()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
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

  return CORBA::ValueDef_stub::supported_interfaces();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::supported_interfaces( const CORBA::InterfaceDefSeq& _par__value )
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
CORBA::ValueDef_stub_clp::supported_interfaces( const CORBA::InterfaceDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
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

  CORBA::ValueDef_stub::supported_interfaces(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::InitializerSeq* CORBA::ValueDef_stub::initializers()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_Initializer );

  CORBA::StaticRequest __req( this, "_get_initializers" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::InitializerSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::InitializerSeq*
CORBA::ValueDef_stub_clp::initializers()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::InitializerSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->initializers();
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

  return CORBA::ValueDef_stub::initializers();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::initializers( const CORBA::InitializerSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_Initializer, &_par__value );
  CORBA::StaticRequest __req( this, "_set_initializers" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueDef_stub_clp::initializers( const CORBA::InitializerSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->initializers(_par__value);
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

  CORBA::ValueDef_stub::initializers(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ValueDef_ptr CORBA::ValueDef_stub::base_value()
{
  CORBA::ValueDef_ptr _res = CORBA::ValueDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ValueDef, &_res );

  CORBA::StaticRequest __req( this, "_get_base_value" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ValueDef_ptr
CORBA::ValueDef_stub_clp::base_value()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->base_value();
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

  return CORBA::ValueDef_stub::base_value();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::base_value( CORBA::ValueDef_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_ValueDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_base_value" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueDef_stub_clp::base_value( CORBA::ValueDef_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->base_value(_par__value);
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

  CORBA::ValueDef_stub::base_value(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ValueDefSeq* CORBA::ValueDef_stub::abstract_base_values()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ValueDef );

  CORBA::StaticRequest __req( this, "_get_abstract_base_values" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ValueDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ValueDefSeq*
CORBA::ValueDef_stub_clp::abstract_base_values()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->abstract_base_values();
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

  return CORBA::ValueDef_stub::abstract_base_values();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::abstract_base_values( const CORBA::ValueDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ValueDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_abstract_base_values" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueDef_stub_clp::abstract_base_values( const CORBA::ValueDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->abstract_base_values(_par__value);
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

  CORBA::ValueDef_stub::abstract_base_values(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::ValueDef_stub::is_abstract()
{
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "_get_is_abstract" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::ValueDef_stub_clp::is_abstract()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_abstract();
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

  return CORBA::ValueDef_stub::is_abstract();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::is_abstract( CORBA::Boolean _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );
  CORBA::StaticRequest __req( this, "_set_is_abstract" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueDef_stub_clp::is_abstract( CORBA::Boolean _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->is_abstract(_par__value);
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

  CORBA::ValueDef_stub::is_abstract(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::ValueDef_stub::is_custom()
{
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "_get_is_custom" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::ValueDef_stub_clp::is_custom()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_custom();
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

  return CORBA::ValueDef_stub::is_custom();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::is_custom( CORBA::Boolean _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );
  CORBA::StaticRequest __req( this, "_set_is_custom" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueDef_stub_clp::is_custom( CORBA::Boolean _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->is_custom(_par__value);
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

  CORBA::ValueDef_stub::is_custom(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::ValueDef_stub::is_truncatable()
{
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "_get_is_truncatable" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::ValueDef_stub_clp::is_truncatable()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_truncatable();
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

  return CORBA::ValueDef_stub::is_truncatable();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueDef_stub::is_truncatable( CORBA::Boolean _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );
  CORBA::StaticRequest __req( this, "_set_is_truncatable" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueDef_stub_clp::is_truncatable( CORBA::Boolean _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->is_truncatable(_par__value);
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

  CORBA::ValueDef_stub::is_truncatable(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Boolean CORBA::ValueDef_stub::is_a( const char* _par_value_id )
{
  CORBA::StaticAny _sa_value_id( CORBA::_stc_string, &_par_value_id );
  CORBA::Boolean _res;
  CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );

  CORBA::StaticRequest __req( this, "is_a" );
  __req.add_in_arg( &_sa_value_id );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Boolean
CORBA::ValueDef_stub_clp::is_a( const char* _par_value_id )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Boolean __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->is_a(_par_value_id);
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

  return CORBA::ValueDef_stub::is_a(_par_value_id);
}

#endif // MICO_CONF_NO_POA

CORBA::ValueDef::FullValueDescription* CORBA::ValueDef_stub::describe_value()
{
  CORBA::StaticAny __res( _marshaller_CORBA_ValueDef_FullValueDescription );

  CORBA::StaticRequest __req( this, "describe_value" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ValueDef::FullValueDescription*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ValueDef::FullValueDescription*
CORBA::ValueDef_stub_clp::describe_value()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueDef::FullValueDescription* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe_value();
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

  return CORBA::ValueDef_stub::describe_value();
}

#endif // MICO_CONF_NO_POA

CORBA::ValueMemberDef_ptr CORBA::ValueDef_stub::create_value_member( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::Visibility _par_access )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type );
  CORBA::StaticAny _sa_access( CORBA::_stc_short, &_par_access );
  CORBA::ValueMemberDef_ptr _res = CORBA::ValueMemberDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ValueMemberDef, &_res );

  CORBA::StaticRequest __req( this, "create_value_member" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_type );
  __req.add_in_arg( &_sa_access );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ValueMemberDef_ptr
CORBA::ValueDef_stub_clp::create_value_member( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::Visibility _par_access )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ValueMemberDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_value_member(_par_id, _par_name, _par_version, _par_type, _par_access);
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

  return CORBA::ValueDef_stub::create_value_member(_par_id, _par_name, _par_version, _par_type, _par_access);
}

#endif // MICO_CONF_NO_POA

CORBA::AttributeDef_ptr CORBA::ValueDef_stub::create_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type );
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );
  CORBA::AttributeDef_ptr _res = CORBA::AttributeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_AttributeDef, &_res );

  CORBA::StaticRequest __req( this, "create_attribute" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_type );
  __req.add_in_arg( &_sa_mode );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::AttributeDef_ptr
CORBA::ValueDef_stub_clp::create_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::AttributeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode);
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

  return CORBA::ValueDef_stub::create_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode);
}

#endif // MICO_CONF_NO_POA

CORBA::OperationDef_ptr CORBA::ValueDef_stub::create_operation( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_result, CORBA::OperationMode _par_mode, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions, const CORBA::ContextIdSeq& _par_contexts )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_result( _marshaller_CORBA_IDLType, &_par_result );
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_OperationMode, &_par_mode );
  CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
  CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );
  CORBA::StaticAny _sa_contexts( CORBA::_stcseq_string, &_par_contexts );
  CORBA::OperationDef_ptr _res = CORBA::OperationDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_OperationDef, &_res );

  CORBA::StaticRequest __req( this, "create_operation" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_result );
  __req.add_in_arg( &_sa_mode );
  __req.add_in_arg( &_sa_params );
  __req.add_in_arg( &_sa_exceptions );
  __req.add_in_arg( &_sa_contexts );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::OperationDef_ptr
CORBA::ValueDef_stub_clp::create_operation( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_result, CORBA::OperationMode _par_mode, const CORBA::ParDescriptionSeq& _par_params, const CORBA::ExceptionDefSeq& _par_exceptions, const CORBA::ContextIdSeq& _par_contexts )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueDef * _myserv = POA_CORBA::ValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::OperationDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_operation(_par_id, _par_name, _par_version, _par_result, _par_mode, _par_params, _par_exceptions, _par_contexts);
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

  return CORBA::ValueDef_stub::create_operation(_par_id, _par_name, _par_version, _par_result, _par_mode, _par_params, _par_exceptions, _par_contexts);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ValueBoxDef
 */

CORBA::ValueBoxDef::~ValueBoxDef()
{
}

void *
CORBA::ValueBoxDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ValueBoxDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::TypedefDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ValueBoxDef_ptr
CORBA::ValueBoxDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ValueBoxDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ValueBoxDef:1.0" )))
      return _duplicate( (CORBA::ValueBoxDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ValueBoxDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ValueBoxDef:1.0")) {
      _o = new CORBA::ValueBoxDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ValueBoxDef_ptr
CORBA::ValueBoxDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ValueBoxDef;
}
class _Marshaller_CORBA_ValueBoxDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ValueBoxDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ValueBoxDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ValueBoxDef::~_Marshaller_CORBA_ValueBoxDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ValueBoxDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ValueBoxDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ValueBoxDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ValueBoxDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ValueBoxDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ValueBoxDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ValueBoxDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ValueBoxDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ValueBoxDef::typecode()
{
  return CORBA::_tc_ValueBoxDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ValueBoxDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ValueBoxDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueBoxDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ValueBoxDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueBoxDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ValueBoxDef_ptr &_obj )
{
  CORBA::ValueBoxDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ValueBoxDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ValueBoxDef
 */

CORBA::ValueBoxDef_stub::~ValueBoxDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ValueBoxDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ValueBoxDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::TypedefDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ValueBoxDef *
POA_CORBA::ValueBoxDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ValueBoxDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ValueBoxDef *) p;
  }
  return NULL;
}

CORBA::ValueBoxDef_stub_clp::ValueBoxDef_stub_clp ()
{
}

CORBA::ValueBoxDef_stub_clp::ValueBoxDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ValueBoxDef_stub_clp::~ValueBoxDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::ValueBoxDef_stub::original_type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_original_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::ValueBoxDef_stub_clp::original_type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueBoxDef * _myserv = POA_CORBA::ValueBoxDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->original_type_def();
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

  return CORBA::ValueBoxDef_stub::original_type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueBoxDef_stub::original_type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_original_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueBoxDef_stub_clp::original_type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueBoxDef * _myserv = POA_CORBA::ValueBoxDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->original_type_def(_par__value);
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

  CORBA::ValueBoxDef_stub::original_type_def(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class NativeDef
 */

CORBA::NativeDef::~NativeDef()
{
}

void *
CORBA::NativeDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/NativeDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::TypedefDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::NativeDef_ptr
CORBA::NativeDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::NativeDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/NativeDef:1.0" )))
      return _duplicate( (CORBA::NativeDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/NativeDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/NativeDef:1.0")) {
      _o = new CORBA::NativeDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::NativeDef_ptr
CORBA::NativeDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_NativeDef;
}
class _Marshaller_CORBA_NativeDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::NativeDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_NativeDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_NativeDef::~_Marshaller_CORBA_NativeDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_NativeDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_NativeDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::NativeDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_NativeDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_NativeDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_NativeDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::NativeDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_NativeDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_NativeDef::typecode()
{
  return CORBA::_tc_NativeDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_NativeDef;

void
operator<<=( CORBA::Any &_a, const CORBA::NativeDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_NativeDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::NativeDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_NativeDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::NativeDef_ptr &_obj )
{
  CORBA::NativeDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_NativeDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class NativeDef
 */

CORBA::NativeDef_stub::~NativeDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::NativeDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/NativeDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::TypedefDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::NativeDef *
POA_CORBA::NativeDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/NativeDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::NativeDef *) p;
  }
  return NULL;
}

CORBA::NativeDef_stub_clp::NativeDef_stub_clp ()
{
}

CORBA::NativeDef_stub_clp::NativeDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::NativeDef_stub_clp::~NativeDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtAttrDescriptionSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst ExtValueDef::_tc_ExtFullValueDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ExtValueDef::ExtFullValueDescription::ExtFullValueDescription()
{
}

CORBA::ExtValueDef::ExtFullValueDescription::ExtFullValueDescription( const ExtFullValueDescription& _s )
{
  name = ((ExtFullValueDescription&)_s).name;
  id = ((ExtFullValueDescription&)_s).id;
  is_abstract = ((ExtFullValueDescription&)_s).is_abstract;
  is_custom = ((ExtFullValueDescription&)_s).is_custom;
  defined_in = ((ExtFullValueDescription&)_s).defined_in;
  version = ((ExtFullValueDescription&)_s).version;
  operations = ((ExtFullValueDescription&)_s).operations;
  attributes = ((ExtFullValueDescription&)_s).attributes;
  members = ((ExtFullValueDescription&)_s).members;
  initializers = ((ExtFullValueDescription&)_s).initializers;
  supported_interfaces = ((ExtFullValueDescription&)_s).supported_interfaces;
  abstract_base_values = ((ExtFullValueDescription&)_s).abstract_base_values;
  is_truncatable = ((ExtFullValueDescription&)_s).is_truncatable;
  base_value = ((ExtFullValueDescription&)_s).base_value;
  type = ((ExtFullValueDescription&)_s).type;
}

CORBA::ExtValueDef::ExtFullValueDescription::~ExtFullValueDescription()
{
}

CORBA::ExtValueDef::ExtFullValueDescription&
CORBA::ExtValueDef::ExtFullValueDescription::operator=( const ExtFullValueDescription& _s )
{
  name = ((ExtFullValueDescription&)_s).name;
  id = ((ExtFullValueDescription&)_s).id;
  is_abstract = ((ExtFullValueDescription&)_s).is_abstract;
  is_custom = ((ExtFullValueDescription&)_s).is_custom;
  defined_in = ((ExtFullValueDescription&)_s).defined_in;
  version = ((ExtFullValueDescription&)_s).version;
  operations = ((ExtFullValueDescription&)_s).operations;
  attributes = ((ExtFullValueDescription&)_s).attributes;
  members = ((ExtFullValueDescription&)_s).members;
  initializers = ((ExtFullValueDescription&)_s).initializers;
  supported_interfaces = ((ExtFullValueDescription&)_s).supported_interfaces;
  abstract_base_values = ((ExtFullValueDescription&)_s).abstract_base_values;
  is_truncatable = ((ExtFullValueDescription&)_s).is_truncatable;
  base_value = ((ExtFullValueDescription&)_s).base_value;
  type = ((ExtFullValueDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtValueDef::ExtFullValueDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtValueDef_ExtFullValueDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::~_Marshaller_CORBA_ExtValueDef_ExtFullValueDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_abstract ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_custom ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->operations ) &&
    _marshaller__seq_CORBA_ExtAttributeDescription->demarshal( dc, &((_MICO_T*)v)->attributes ) &&
    _marshaller__seq_CORBA_ValueMember->demarshal( dc, &((_MICO_T*)v)->members ) &&
    _marshaller__seq_CORBA_ExtInitializer->demarshal( dc, &((_MICO_T*)v)->initializers ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->supported_interfaces ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->abstract_base_values ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_truncatable ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->base_value._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_abstract );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_custom );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->operations );
  _marshaller__seq_CORBA_ExtAttributeDescription->marshal( ec, &((_MICO_T*)v)->attributes );
  _marshaller__seq_CORBA_ValueMember->marshal( ec, &((_MICO_T*)v)->members );
  _marshaller__seq_CORBA_ExtInitializer->marshal( ec, &((_MICO_T*)v)->initializers );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->supported_interfaces );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->abstract_base_values );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_truncatable );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->base_value.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription::typecode()
{
  return CORBA::ExtValueDef::_tc_ExtFullValueDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtValueDef_ExtFullValueDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ExtValueDef::ExtFullValueDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtValueDef_ExtFullValueDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ExtValueDef::ExtFullValueDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ExtValueDef::ExtFullValueDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtValueDef_ExtFullValueDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ExtValueDef::ExtFullValueDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ExtValueDef_ExtFullValueDescription, (void *&)_s);
}


/*
 * Base interface for class ExtValueDef
 */

CORBA::ExtValueDef::~ExtValueDef()
{
}

void *
CORBA::ExtValueDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ExtValueDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ValueDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ExtValueDef_ptr
CORBA::ExtValueDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ExtValueDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ExtValueDef:1.0" )))
      return _duplicate( (CORBA::ExtValueDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ExtValueDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ExtValueDef:1.0")) {
      _o = new CORBA::ExtValueDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ExtValueDef_ptr
CORBA::ExtValueDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtValueDef;
}
class _Marshaller_CORBA_ExtValueDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtValueDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtValueDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtValueDef::~_Marshaller_CORBA_ExtValueDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtValueDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ExtValueDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ExtValueDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ExtValueDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ExtValueDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ExtValueDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ExtValueDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ExtValueDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtValueDef::typecode()
{
  return CORBA::_tc_ExtValueDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtValueDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ExtValueDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtValueDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ExtValueDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtValueDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ExtValueDef_ptr &_obj )
{
  CORBA::ExtValueDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ExtValueDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ExtValueDef
 */

CORBA::ExtValueDef_stub::~ExtValueDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ExtValueDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtValueDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ValueDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ExtValueDef *
POA_CORBA::ExtValueDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ExtValueDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ExtValueDef *) p;
  }
  return NULL;
}

CORBA::ExtValueDef_stub_clp::ExtValueDef_stub_clp ()
{
}

CORBA::ExtValueDef_stub_clp::ExtValueDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ExtValueDef_stub_clp::~ExtValueDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ExtInitializerSeq* CORBA::ExtValueDef_stub::ext_initializers()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ExtInitializer );

  CORBA::StaticRequest __req( this, "_get_ext_initializers" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ExtInitializerSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ExtInitializerSeq*
CORBA::ExtValueDef_stub_clp::ext_initializers()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtValueDef * _myserv = POA_CORBA::ExtValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExtInitializerSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->ext_initializers();
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

  return CORBA::ExtValueDef_stub::ext_initializers();
}

#endif // MICO_CONF_NO_POA

void CORBA::ExtValueDef_stub::ext_initializers( const CORBA::ExtInitializerSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExtInitializer, &_par__value );
  CORBA::StaticRequest __req( this, "_set_ext_initializers" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ExtValueDef_stub_clp::ext_initializers( const CORBA::ExtInitializerSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtValueDef * _myserv = POA_CORBA::ExtValueDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->ext_initializers(_par__value);
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

  CORBA::ExtValueDef_stub::ext_initializers(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ExtValueDef::ExtFullValueDescription* CORBA::ExtValueDef_stub::describe_ext_value()
{
  CORBA::StaticAny __res( _marshaller_CORBA_ExtValueDef_ExtFullValueDescription );

  CORBA::StaticRequest __req( this, "describe_ext_value" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ExtValueDef::ExtFullValueDescription*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ExtValueDef::ExtFullValueDescription*
CORBA::ExtValueDef_stub_clp::describe_ext_value()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtValueDef * _myserv = POA_CORBA::ExtValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExtValueDef::ExtFullValueDescription* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe_ext_value();
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

  return CORBA::ExtValueDef_stub::describe_ext_value();
}

#endif // MICO_CONF_NO_POA

CORBA::ExtAttributeDef_ptr CORBA::ExtValueDef_stub::create_ext_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode, const CORBA::ExceptionDefSeq& _par_get_exceptions, const CORBA::ExceptionDefSeq& _par_set_exceptions )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type );
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );
  CORBA::StaticAny _sa_get_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_get_exceptions );
  CORBA::StaticAny _sa_set_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_set_exceptions );
  CORBA::ExtAttributeDef_ptr _res = CORBA::ExtAttributeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ExtAttributeDef, &_res );

  CORBA::StaticRequest __req( this, "create_ext_attribute" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_type );
  __req.add_in_arg( &_sa_mode );
  __req.add_in_arg( &_sa_get_exceptions );
  __req.add_in_arg( &_sa_set_exceptions );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ExtAttributeDef_ptr
CORBA::ExtValueDef_stub_clp::create_ext_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode, const CORBA::ExceptionDefSeq& _par_get_exceptions, const CORBA::ExceptionDefSeq& _par_set_exceptions )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtValueDef * _myserv = POA_CORBA::ExtValueDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExtAttributeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_ext_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode, _par_get_exceptions, _par_set_exceptions);
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

  return CORBA::ExtValueDef_stub::create_ext_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode, _par_get_exceptions, _par_set_exceptions);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_LocalInterfaceDefSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExceptionDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ExceptionDescription::ExceptionDescription()
{
}

CORBA::ExceptionDescription::ExceptionDescription( const ExceptionDescription& _s )
{
  name = ((ExceptionDescription&)_s).name;
  id = ((ExceptionDescription&)_s).id;
  defined_in = ((ExceptionDescription&)_s).defined_in;
  version = ((ExceptionDescription&)_s).version;
  type = ((ExceptionDescription&)_s).type;
}

CORBA::ExceptionDescription::~ExceptionDescription()
{
}

CORBA::ExceptionDescription&
CORBA::ExceptionDescription::operator=( const ExceptionDescription& _s )
{
  name = ((ExceptionDescription&)_s).name;
  id = ((ExceptionDescription&)_s).id;
  defined_in = ((ExceptionDescription&)_s).defined_in;
  version = ((ExceptionDescription&)_s).version;
  type = ((ExceptionDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_ExceptionDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExceptionDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ExceptionDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExceptionDescription::~_Marshaller_CORBA_ExceptionDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExceptionDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ExceptionDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ExceptionDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ExceptionDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ExceptionDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExceptionDescription::typecode()
{
  return CORBA::_tc_ExceptionDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExceptionDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ExceptionDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExceptionDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ExceptionDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ExceptionDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExceptionDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ExceptionDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ExceptionDescription, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExcDescriptionSeq;
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_Initializer;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::Initializer::Initializer()
{
}

CORBA::Initializer::Initializer( const Initializer& _s )
{
  members = ((Initializer&)_s).members;
  name = ((Initializer&)_s).name;
}

CORBA::Initializer::~Initializer()
{
}

CORBA::Initializer&
CORBA::Initializer::operator=( const Initializer& _s )
{
  members = ((Initializer&)_s).members;
  name = ((Initializer&)_s).name;
  return *this;
}
#endif

class _Marshaller_CORBA_Initializer : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Initializer _MICO_T;
  public:
    ~_Marshaller_CORBA_Initializer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_Initializer::~_Marshaller_CORBA_Initializer()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_Initializer::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_Initializer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_Initializer::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_Initializer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller__seq_CORBA_StructMember->demarshal( dc, &((_MICO_T*)v)->members ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_Initializer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller__seq_CORBA_StructMember->marshal( ec, &((_MICO_T*)v)->members );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_Initializer::typecode()
{
  return CORBA::_tc_Initializer;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_Initializer;

void operator<<=( CORBA::Any &_a, const CORBA::Initializer &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Initializer, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::Initializer *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::Initializer &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_Initializer, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::Initializer *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_Initializer, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtInitializer;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ExtInitializer::ExtInitializer()
{
}

CORBA::ExtInitializer::ExtInitializer( const ExtInitializer& _s )
{
  members = ((ExtInitializer&)_s).members;
  exceptions_def = ((ExtInitializer&)_s).exceptions_def;
  exceptions = ((ExtInitializer&)_s).exceptions;
  name = ((ExtInitializer&)_s).name;
}

CORBA::ExtInitializer::~ExtInitializer()
{
}

CORBA::ExtInitializer&
CORBA::ExtInitializer::operator=( const ExtInitializer& _s )
{
  members = ((ExtInitializer&)_s).members;
  exceptions_def = ((ExtInitializer&)_s).exceptions_def;
  exceptions = ((ExtInitializer&)_s).exceptions;
  name = ((ExtInitializer&)_s).name;
  return *this;
}
#endif

class _Marshaller_CORBA_ExtInitializer : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtInitializer _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtInitializer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtInitializer::~_Marshaller_CORBA_ExtInitializer()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtInitializer::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ExtInitializer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ExtInitializer::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ExtInitializer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller__seq_CORBA_StructMember->demarshal( dc, &((_MICO_T*)v)->members ) &&
    _marshaller__seq_CORBA_ExceptionDef->demarshal( dc, &((_MICO_T*)v)->exceptions_def ) &&
    _marshaller__seq_CORBA_ExceptionDescription->demarshal( dc, &((_MICO_T*)v)->exceptions ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ExtInitializer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller__seq_CORBA_StructMember->marshal( ec, &((_MICO_T*)v)->members );
  _marshaller__seq_CORBA_ExceptionDef->marshal( ec, &((_MICO_T*)v)->exceptions_def );
  _marshaller__seq_CORBA_ExceptionDescription->marshal( ec, &((_MICO_T*)v)->exceptions );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtInitializer::typecode()
{
  return CORBA::_tc_ExtInitializer;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtInitializer;

void operator<<=( CORBA::Any &_a, const CORBA::ExtInitializer &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtInitializer, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ExtInitializer *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ExtInitializer &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtInitializer, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ExtInitializer *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ExtInitializer, (void *&)_s);
}


/*
 * Base interface for class PrimitiveDef
 */

CORBA::PrimitiveDef::~PrimitiveDef()
{
}

void *
CORBA::PrimitiveDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/PrimitiveDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::PrimitiveDef_ptr
CORBA::PrimitiveDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::PrimitiveDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/PrimitiveDef:1.0" )))
      return _duplicate( (CORBA::PrimitiveDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/PrimitiveDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/PrimitiveDef:1.0")) {
      _o = new CORBA::PrimitiveDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::PrimitiveDef_ptr
CORBA::PrimitiveDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_PrimitiveDef;
}
class _Marshaller_CORBA_PrimitiveDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::PrimitiveDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_PrimitiveDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_PrimitiveDef::~_Marshaller_CORBA_PrimitiveDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_PrimitiveDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_PrimitiveDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::PrimitiveDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_PrimitiveDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_PrimitiveDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_PrimitiveDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::PrimitiveDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_PrimitiveDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_PrimitiveDef::typecode()
{
  return CORBA::_tc_PrimitiveDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_PrimitiveDef;

void
operator<<=( CORBA::Any &_a, const CORBA::PrimitiveDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_PrimitiveDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::PrimitiveDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_PrimitiveDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::PrimitiveDef_ptr &_obj )
{
  CORBA::PrimitiveDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_PrimitiveDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class PrimitiveDef
 */

CORBA::PrimitiveDef_stub::~PrimitiveDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::PrimitiveDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/PrimitiveDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::PrimitiveDef *
POA_CORBA::PrimitiveDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/PrimitiveDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::PrimitiveDef *) p;
  }
  return NULL;
}

CORBA::PrimitiveDef_stub_clp::PrimitiveDef_stub_clp ()
{
}

CORBA::PrimitiveDef_stub_clp::PrimitiveDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::PrimitiveDef_stub_clp::~PrimitiveDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::PrimitiveKind CORBA::PrimitiveDef_stub::kind()
{
  CORBA::PrimitiveKind _res;
  CORBA::StaticAny __res( _marshaller_CORBA_PrimitiveKind, &_res );

  CORBA::StaticRequest __req( this, "_get_kind" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::PrimitiveKind
CORBA::PrimitiveDef_stub_clp::kind()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::PrimitiveDef * _myserv = POA_CORBA::PrimitiveDef::_narrow (_serv);
    if (_myserv) {
      CORBA::PrimitiveKind __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->kind();
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

  return CORBA::PrimitiveDef_stub::kind();
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class StringDef
 */

CORBA::StringDef::~StringDef()
{
}

void *
CORBA::StringDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/StringDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::StringDef_ptr
CORBA::StringDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::StringDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/StringDef:1.0" )))
      return _duplicate( (CORBA::StringDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/StringDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/StringDef:1.0")) {
      _o = new CORBA::StringDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::StringDef_ptr
CORBA::StringDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_StringDef;
}
class _Marshaller_CORBA_StringDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::StringDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_StringDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_StringDef::~_Marshaller_CORBA_StringDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_StringDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_StringDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::StringDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_StringDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_StringDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_StringDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::StringDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_StringDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_StringDef::typecode()
{
  return CORBA::_tc_StringDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_StringDef;

void
operator<<=( CORBA::Any &_a, const CORBA::StringDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_StringDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::StringDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_StringDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::StringDef_ptr &_obj )
{
  CORBA::StringDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_StringDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class StringDef
 */

CORBA::StringDef_stub::~StringDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::StringDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/StringDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::StringDef *
POA_CORBA::StringDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/StringDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::StringDef *) p;
  }
  return NULL;
}

CORBA::StringDef_stub_clp::StringDef_stub_clp ()
{
}

CORBA::StringDef_stub_clp::StringDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::StringDef_stub_clp::~StringDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ULong CORBA::StringDef_stub::bound()
{
  CORBA::ULong _res;
  CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );

  CORBA::StaticRequest __req( this, "_get_bound" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ULong
CORBA::StringDef_stub_clp::bound()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::StringDef * _myserv = POA_CORBA::StringDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ULong __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->bound();
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

  return CORBA::StringDef_stub::bound();
}

#endif // MICO_CONF_NO_POA

void CORBA::StringDef_stub::bound( CORBA::ULong _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );
  CORBA::StaticRequest __req( this, "_set_bound" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::StringDef_stub_clp::bound( CORBA::ULong _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::StringDef * _myserv = POA_CORBA::StringDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->bound(_par__value);
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

  CORBA::StringDef_stub::bound(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class WstringDef
 */

CORBA::WstringDef::~WstringDef()
{
}

void *
CORBA::WstringDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/WstringDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::WstringDef_ptr
CORBA::WstringDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::WstringDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/WstringDef:1.0" )))
      return _duplicate( (CORBA::WstringDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/WstringDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/WstringDef:1.0")) {
      _o = new CORBA::WstringDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::WstringDef_ptr
CORBA::WstringDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_WstringDef;
}
class _Marshaller_CORBA_WstringDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::WstringDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_WstringDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_WstringDef::~_Marshaller_CORBA_WstringDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_WstringDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_WstringDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::WstringDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_WstringDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_WstringDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_WstringDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::WstringDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_WstringDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_WstringDef::typecode()
{
  return CORBA::_tc_WstringDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_WstringDef;

void
operator<<=( CORBA::Any &_a, const CORBA::WstringDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_WstringDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::WstringDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_WstringDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::WstringDef_ptr &_obj )
{
  CORBA::WstringDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_WstringDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class WstringDef
 */

CORBA::WstringDef_stub::~WstringDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::WstringDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/WstringDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::WstringDef *
POA_CORBA::WstringDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/WstringDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::WstringDef *) p;
  }
  return NULL;
}

CORBA::WstringDef_stub_clp::WstringDef_stub_clp ()
{
}

CORBA::WstringDef_stub_clp::WstringDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::WstringDef_stub_clp::~WstringDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ULong CORBA::WstringDef_stub::bound()
{
  CORBA::ULong _res;
  CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );

  CORBA::StaticRequest __req( this, "_get_bound" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ULong
CORBA::WstringDef_stub_clp::bound()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::WstringDef * _myserv = POA_CORBA::WstringDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ULong __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->bound();
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

  return CORBA::WstringDef_stub::bound();
}

#endif // MICO_CONF_NO_POA

void CORBA::WstringDef_stub::bound( CORBA::ULong _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );
  CORBA::StaticRequest __req( this, "_set_bound" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::WstringDef_stub_clp::bound( CORBA::ULong _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::WstringDef * _myserv = POA_CORBA::WstringDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->bound(_par__value);
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

  CORBA::WstringDef_stub::bound(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class SequenceDef
 */

CORBA::SequenceDef::~SequenceDef()
{
}

void *
CORBA::SequenceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/SequenceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::SequenceDef_ptr
CORBA::SequenceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::SequenceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/SequenceDef:1.0" )))
      return _duplicate( (CORBA::SequenceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/SequenceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/SequenceDef:1.0")) {
      _o = new CORBA::SequenceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::SequenceDef_ptr
CORBA::SequenceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_SequenceDef;
}
class _Marshaller_CORBA_SequenceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::SequenceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_SequenceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_SequenceDef::~_Marshaller_CORBA_SequenceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_SequenceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_SequenceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::SequenceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_SequenceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_SequenceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_SequenceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::SequenceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_SequenceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_SequenceDef::typecode()
{
  return CORBA::_tc_SequenceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_SequenceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::SequenceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_SequenceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::SequenceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_SequenceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::SequenceDef_ptr &_obj )
{
  CORBA::SequenceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_SequenceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class SequenceDef
 */

CORBA::SequenceDef_stub::~SequenceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::SequenceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/SequenceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::SequenceDef *
POA_CORBA::SequenceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/SequenceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::SequenceDef *) p;
  }
  return NULL;
}

CORBA::SequenceDef_stub_clp::SequenceDef_stub_clp ()
{
}

CORBA::SequenceDef_stub_clp::SequenceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::SequenceDef_stub_clp::~SequenceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ULong CORBA::SequenceDef_stub::bound()
{
  CORBA::ULong _res;
  CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );

  CORBA::StaticRequest __req( this, "_get_bound" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ULong
CORBA::SequenceDef_stub_clp::bound()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::SequenceDef * _myserv = POA_CORBA::SequenceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ULong __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->bound();
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

  return CORBA::SequenceDef_stub::bound();
}

#endif // MICO_CONF_NO_POA

void CORBA::SequenceDef_stub::bound( CORBA::ULong _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );
  CORBA::StaticRequest __req( this, "_set_bound" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::SequenceDef_stub_clp::bound( CORBA::ULong _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::SequenceDef * _myserv = POA_CORBA::SequenceDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->bound(_par__value);
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

  CORBA::SequenceDef_stub::bound(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::SequenceDef_stub::element_type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_element_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::SequenceDef_stub_clp::element_type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::SequenceDef * _myserv = POA_CORBA::SequenceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->element_type();
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

  return CORBA::SequenceDef_stub::element_type();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::SequenceDef_stub::element_type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_element_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::SequenceDef_stub_clp::element_type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::SequenceDef * _myserv = POA_CORBA::SequenceDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->element_type_def();
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

  return CORBA::SequenceDef_stub::element_type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::SequenceDef_stub::element_type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_element_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::SequenceDef_stub_clp::element_type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::SequenceDef * _myserv = POA_CORBA::SequenceDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->element_type_def(_par__value);
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

  CORBA::SequenceDef_stub::element_type_def(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ArrayDef
 */

CORBA::ArrayDef::~ArrayDef()
{
}

void *
CORBA::ArrayDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ArrayDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ArrayDef_ptr
CORBA::ArrayDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ArrayDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ArrayDef:1.0" )))
      return _duplicate( (CORBA::ArrayDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ArrayDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ArrayDef:1.0")) {
      _o = new CORBA::ArrayDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ArrayDef_ptr
CORBA::ArrayDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ArrayDef;
}
class _Marshaller_CORBA_ArrayDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ArrayDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ArrayDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ArrayDef::~_Marshaller_CORBA_ArrayDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ArrayDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ArrayDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ArrayDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ArrayDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ArrayDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ArrayDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ArrayDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ArrayDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ArrayDef::typecode()
{
  return CORBA::_tc_ArrayDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ArrayDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ArrayDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ArrayDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ArrayDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ArrayDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ArrayDef_ptr &_obj )
{
  CORBA::ArrayDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ArrayDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ArrayDef
 */

CORBA::ArrayDef_stub::~ArrayDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ArrayDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ArrayDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ArrayDef *
POA_CORBA::ArrayDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ArrayDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ArrayDef *) p;
  }
  return NULL;
}

CORBA::ArrayDef_stub_clp::ArrayDef_stub_clp ()
{
}

CORBA::ArrayDef_stub_clp::ArrayDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ArrayDef_stub_clp::~ArrayDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ULong CORBA::ArrayDef_stub::length()
{
  CORBA::ULong _res;
  CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );

  CORBA::StaticRequest __req( this, "_get_length" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ULong
CORBA::ArrayDef_stub_clp::length()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ArrayDef * _myserv = POA_CORBA::ArrayDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ULong __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->length();
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

  return CORBA::ArrayDef_stub::length();
}

#endif // MICO_CONF_NO_POA

void CORBA::ArrayDef_stub::length( CORBA::ULong _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );
  CORBA::StaticRequest __req( this, "_set_length" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ArrayDef_stub_clp::length( CORBA::ULong _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ArrayDef * _myserv = POA_CORBA::ArrayDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->length(_par__value);
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

  CORBA::ArrayDef_stub::length(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::ArrayDef_stub::element_type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_element_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::ArrayDef_stub_clp::element_type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ArrayDef * _myserv = POA_CORBA::ArrayDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->element_type();
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

  return CORBA::ArrayDef_stub::element_type();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::ArrayDef_stub::element_type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_element_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::ArrayDef_stub_clp::element_type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ArrayDef * _myserv = POA_CORBA::ArrayDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->element_type_def();
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

  return CORBA::ArrayDef_stub::element_type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::ArrayDef_stub::element_type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_element_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ArrayDef_stub_clp::element_type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ArrayDef * _myserv = POA_CORBA::ArrayDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->element_type_def(_par__value);
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

  CORBA::ArrayDef_stub::element_type_def(_par__value);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class FixedDef
 */

CORBA::FixedDef::~FixedDef()
{
}

void *
CORBA::FixedDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/FixedDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::IDLType::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::FixedDef_ptr
CORBA::FixedDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::FixedDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/FixedDef:1.0" )))
      return _duplicate( (CORBA::FixedDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/FixedDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/FixedDef:1.0")) {
      _o = new CORBA::FixedDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::FixedDef_ptr
CORBA::FixedDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_FixedDef;
}
class _Marshaller_CORBA_FixedDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::FixedDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_FixedDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_FixedDef::~_Marshaller_CORBA_FixedDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_FixedDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_FixedDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::FixedDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_FixedDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_FixedDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_FixedDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::FixedDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_FixedDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_FixedDef::typecode()
{
  return CORBA::_tc_FixedDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_FixedDef;

void
operator<<=( CORBA::Any &_a, const CORBA::FixedDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_FixedDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::FixedDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_FixedDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::FixedDef_ptr &_obj )
{
  CORBA::FixedDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_FixedDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class FixedDef
 */

CORBA::FixedDef_stub::~FixedDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::FixedDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/FixedDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::IDLType::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::FixedDef *
POA_CORBA::FixedDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/FixedDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::FixedDef *) p;
  }
  return NULL;
}

CORBA::FixedDef_stub_clp::FixedDef_stub_clp ()
{
}

CORBA::FixedDef_stub_clp::FixedDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::FixedDef_stub_clp::~FixedDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::UShort CORBA::FixedDef_stub::digits()
{
  CORBA::UShort _res;
  CORBA::StaticAny __res( CORBA::_stc_ushort, &_res );

  CORBA::StaticRequest __req( this, "_get_digits" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::UShort
CORBA::FixedDef_stub_clp::digits()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::FixedDef * _myserv = POA_CORBA::FixedDef::_narrow (_serv);
    if (_myserv) {
      CORBA::UShort __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->digits();
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

  return CORBA::FixedDef_stub::digits();
}

#endif // MICO_CONF_NO_POA

void CORBA::FixedDef_stub::digits( CORBA::UShort _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_ushort, &_par__value );
  CORBA::StaticRequest __req( this, "_set_digits" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::FixedDef_stub_clp::digits( CORBA::UShort _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::FixedDef * _myserv = POA_CORBA::FixedDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->digits(_par__value);
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

  CORBA::FixedDef_stub::digits(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Short CORBA::FixedDef_stub::scale()
{
  CORBA::Short _res;
  CORBA::StaticAny __res( CORBA::_stc_short, &_res );

  CORBA::StaticRequest __req( this, "_get_scale" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Short
CORBA::FixedDef_stub_clp::scale()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::FixedDef * _myserv = POA_CORBA::FixedDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Short __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->scale();
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

  return CORBA::FixedDef_stub::scale();
}

#endif // MICO_CONF_NO_POA

void CORBA::FixedDef_stub::scale( CORBA::Short _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_short, &_par__value );
  CORBA::StaticRequest __req( this, "_set_scale" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::FixedDef_stub_clp::scale( CORBA::Short _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::FixedDef * _myserv = POA_CORBA::FixedDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->scale(_par__value);
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

  CORBA::FixedDef_stub::scale(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_ModuleDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ModuleDescription::ModuleDescription()
{
}

CORBA::ModuleDescription::ModuleDescription( const ModuleDescription& _s )
{
  name = ((ModuleDescription&)_s).name;
  id = ((ModuleDescription&)_s).id;
  defined_in = ((ModuleDescription&)_s).defined_in;
  version = ((ModuleDescription&)_s).version;
}

CORBA::ModuleDescription::~ModuleDescription()
{
}

CORBA::ModuleDescription&
CORBA::ModuleDescription::operator=( const ModuleDescription& _s )
{
  name = ((ModuleDescription&)_s).name;
  id = ((ModuleDescription&)_s).id;
  defined_in = ((ModuleDescription&)_s).defined_in;
  version = ((ModuleDescription&)_s).version;
  return *this;
}
#endif

class _Marshaller_CORBA_ModuleDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ModuleDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ModuleDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ModuleDescription::~_Marshaller_CORBA_ModuleDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ModuleDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ModuleDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ModuleDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ModuleDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ModuleDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ModuleDescription::typecode()
{
  return CORBA::_tc_ModuleDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ModuleDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ModuleDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ModuleDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ModuleDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ModuleDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ModuleDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ModuleDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ModuleDescription, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ConstantDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ConstantDescription::ConstantDescription()
{
}

CORBA::ConstantDescription::ConstantDescription( const ConstantDescription& _s )
{
  name = ((ConstantDescription&)_s).name;
  id = ((ConstantDescription&)_s).id;
  defined_in = ((ConstantDescription&)_s).defined_in;
  version = ((ConstantDescription&)_s).version;
  type = ((ConstantDescription&)_s).type;
  value = ((ConstantDescription&)_s).value;
}

CORBA::ConstantDescription::~ConstantDescription()
{
}

CORBA::ConstantDescription&
CORBA::ConstantDescription::operator=( const ConstantDescription& _s )
{
  name = ((ConstantDescription&)_s).name;
  id = ((ConstantDescription&)_s).id;
  defined_in = ((ConstantDescription&)_s).defined_in;
  version = ((ConstantDescription&)_s).version;
  type = ((ConstantDescription&)_s).type;
  value = ((ConstantDescription&)_s).value;
  return *this;
}
#endif

class _Marshaller_CORBA_ConstantDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ConstantDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ConstantDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ConstantDescription::~_Marshaller_CORBA_ConstantDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ConstantDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ConstantDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ConstantDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ConstantDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ConstantDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ConstantDescription::typecode()
{
  return CORBA::_tc_ConstantDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ConstantDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ConstantDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ConstantDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ConstantDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ConstantDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ConstantDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ConstantDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ConstantDescription, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_TypeDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::TypeDescription::TypeDescription()
{
}

CORBA::TypeDescription::TypeDescription( const TypeDescription& _s )
{
  name = ((TypeDescription&)_s).name;
  id = ((TypeDescription&)_s).id;
  defined_in = ((TypeDescription&)_s).defined_in;
  version = ((TypeDescription&)_s).version;
  type = ((TypeDescription&)_s).type;
}

CORBA::TypeDescription::~TypeDescription()
{
}

CORBA::TypeDescription&
CORBA::TypeDescription::operator=( const TypeDescription& _s )
{
  name = ((TypeDescription&)_s).name;
  id = ((TypeDescription&)_s).id;
  defined_in = ((TypeDescription&)_s).defined_in;
  version = ((TypeDescription&)_s).version;
  type = ((TypeDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_TypeDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::TypeDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_TypeDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_TypeDescription::~_Marshaller_CORBA_TypeDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_TypeDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_TypeDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_TypeDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_TypeDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_TypeDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_TypeDescription::typecode()
{
  return CORBA::_tc_TypeDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_TypeDescription;

void operator<<=( CORBA::Any &_a, const CORBA::TypeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_TypeDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::TypeDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::TypeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_TypeDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::TypeDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_TypeDescription, (void *&)_s);
}


/*
 * Base interface for class AttributeDef
 */

CORBA::AttributeDef::~AttributeDef()
{
}

void *
CORBA::AttributeDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/AttributeDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::AttributeDef_ptr
CORBA::AttributeDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::AttributeDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/AttributeDef:1.0" )))
      return _duplicate( (CORBA::AttributeDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/AttributeDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/AttributeDef:1.0")) {
      _o = new CORBA::AttributeDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::AttributeDef_ptr
CORBA::AttributeDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_AttributeDef;
}
class _Marshaller_CORBA_AttributeDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::AttributeDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_AttributeDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_AttributeDef::~_Marshaller_CORBA_AttributeDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_AttributeDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_AttributeDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::AttributeDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_AttributeDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_AttributeDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_AttributeDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::AttributeDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_AttributeDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_AttributeDef::typecode()
{
  return CORBA::_tc_AttributeDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_AttributeDef;

void
operator<<=( CORBA::Any &_a, const CORBA::AttributeDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AttributeDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::AttributeDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AttributeDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::AttributeDef_ptr &_obj )
{
  CORBA::AttributeDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_AttributeDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class AttributeDef
 */

CORBA::AttributeDef_stub::~AttributeDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::AttributeDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/AttributeDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::AttributeDef *
POA_CORBA::AttributeDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/AttributeDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::AttributeDef *) p;
  }
  return NULL;
}

CORBA::AttributeDef_stub_clp::AttributeDef_stub_clp ()
{
}

CORBA::AttributeDef_stub_clp::AttributeDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::AttributeDef_stub_clp::~AttributeDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::AttributeDef_stub::type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::AttributeDef_stub_clp::type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AttributeDef * _myserv = POA_CORBA::AttributeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type();
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

  return CORBA::AttributeDef_stub::type();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::AttributeDef_stub::type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::AttributeDef_stub_clp::type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AttributeDef * _myserv = POA_CORBA::AttributeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type_def();
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

  return CORBA::AttributeDef_stub::type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::AttributeDef_stub::type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::AttributeDef_stub_clp::type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AttributeDef * _myserv = POA_CORBA::AttributeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->type_def(_par__value);
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

  CORBA::AttributeDef_stub::type_def(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::AttributeMode CORBA::AttributeDef_stub::mode()
{
  CORBA::AttributeMode _res;
  CORBA::StaticAny __res( _marshaller_CORBA_AttributeMode, &_res );

  CORBA::StaticRequest __req( this, "_get_mode" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::AttributeMode
CORBA::AttributeDef_stub_clp::mode()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AttributeDef * _myserv = POA_CORBA::AttributeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::AttributeMode __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->mode();
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

  return CORBA::AttributeDef_stub::mode();
}

#endif // MICO_CONF_NO_POA

void CORBA::AttributeDef_stub::mode( CORBA::AttributeMode _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_AttributeMode, &_par__value );
  CORBA::StaticRequest __req( this, "_set_mode" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::AttributeDef_stub_clp::mode( CORBA::AttributeMode _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::AttributeDef * _myserv = POA_CORBA::AttributeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->mode(_par__value);
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

  CORBA::AttributeDef_stub::mode(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_AttributeDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::AttributeDescription::AttributeDescription()
{
}

CORBA::AttributeDescription::AttributeDescription( const AttributeDescription& _s )
{
  name = ((AttributeDescription&)_s).name;
  id = ((AttributeDescription&)_s).id;
  defined_in = ((AttributeDescription&)_s).defined_in;
  version = ((AttributeDescription&)_s).version;
  type = ((AttributeDescription&)_s).type;
  mode = ((AttributeDescription&)_s).mode;
}

CORBA::AttributeDescription::~AttributeDescription()
{
}

CORBA::AttributeDescription&
CORBA::AttributeDescription::operator=( const AttributeDescription& _s )
{
  name = ((AttributeDescription&)_s).name;
  id = ((AttributeDescription&)_s).id;
  defined_in = ((AttributeDescription&)_s).defined_in;
  version = ((AttributeDescription&)_s).version;
  type = ((AttributeDescription&)_s).type;
  mode = ((AttributeDescription&)_s).mode;
  return *this;
}
#endif

class _Marshaller_CORBA_AttributeDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::AttributeDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_AttributeDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_AttributeDescription::~_Marshaller_CORBA_AttributeDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_AttributeDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_AttributeDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_AttributeDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_AttributeDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    _marshaller_CORBA_AttributeMode->demarshal( dc, &((_MICO_T*)v)->mode ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_AttributeDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  _marshaller_CORBA_AttributeMode->marshal( ec, &((_MICO_T*)v)->mode );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_AttributeDescription::typecode()
{
  return CORBA::_tc_AttributeDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_AttributeDescription;

void operator<<=( CORBA::Any &_a, const CORBA::AttributeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AttributeDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::AttributeDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::AttributeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_AttributeDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::AttributeDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_AttributeDescription, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtAttributeDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ExtAttributeDescription::ExtAttributeDescription()
{
}

CORBA::ExtAttributeDescription::ExtAttributeDescription( const ExtAttributeDescription& _s )
{
  name = ((ExtAttributeDescription&)_s).name;
  id = ((ExtAttributeDescription&)_s).id;
  defined_in = ((ExtAttributeDescription&)_s).defined_in;
  version = ((ExtAttributeDescription&)_s).version;
  type = ((ExtAttributeDescription&)_s).type;
  mode = ((ExtAttributeDescription&)_s).mode;
  get_exceptions = ((ExtAttributeDescription&)_s).get_exceptions;
  set_exceptions = ((ExtAttributeDescription&)_s).set_exceptions;
}

CORBA::ExtAttributeDescription::~ExtAttributeDescription()
{
}

CORBA::ExtAttributeDescription&
CORBA::ExtAttributeDescription::operator=( const ExtAttributeDescription& _s )
{
  name = ((ExtAttributeDescription&)_s).name;
  id = ((ExtAttributeDescription&)_s).id;
  defined_in = ((ExtAttributeDescription&)_s).defined_in;
  version = ((ExtAttributeDescription&)_s).version;
  type = ((ExtAttributeDescription&)_s).type;
  mode = ((ExtAttributeDescription&)_s).mode;
  get_exceptions = ((ExtAttributeDescription&)_s).get_exceptions;
  set_exceptions = ((ExtAttributeDescription&)_s).set_exceptions;
  return *this;
}
#endif

class _Marshaller_CORBA_ExtAttributeDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtAttributeDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtAttributeDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtAttributeDescription::~_Marshaller_CORBA_ExtAttributeDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtAttributeDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ExtAttributeDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ExtAttributeDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ExtAttributeDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    _marshaller_CORBA_AttributeMode->demarshal( dc, &((_MICO_T*)v)->mode ) &&
    _marshaller__seq_CORBA_ExceptionDescription->demarshal( dc, &((_MICO_T*)v)->get_exceptions ) &&
    _marshaller__seq_CORBA_ExceptionDescription->demarshal( dc, &((_MICO_T*)v)->set_exceptions ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ExtAttributeDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  _marshaller_CORBA_AttributeMode->marshal( ec, &((_MICO_T*)v)->mode );
  _marshaller__seq_CORBA_ExceptionDescription->marshal( ec, &((_MICO_T*)v)->get_exceptions );
  _marshaller__seq_CORBA_ExceptionDescription->marshal( ec, &((_MICO_T*)v)->set_exceptions );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtAttributeDescription::typecode()
{
  return CORBA::_tc_ExtAttributeDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtAttributeDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ExtAttributeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtAttributeDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ExtAttributeDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ExtAttributeDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtAttributeDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ExtAttributeDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ExtAttributeDescription, (void *&)_s);
}


/*
 * Base interface for class ExtAttributeDef
 */

CORBA::ExtAttributeDef::~ExtAttributeDef()
{
}

void *
CORBA::ExtAttributeDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ExtAttributeDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::AttributeDef::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ExtAttributeDef_ptr
CORBA::ExtAttributeDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ExtAttributeDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ExtAttributeDef:1.0" )))
      return _duplicate( (CORBA::ExtAttributeDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ExtAttributeDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ExtAttributeDef:1.0")) {
      _o = new CORBA::ExtAttributeDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ExtAttributeDef_ptr
CORBA::ExtAttributeDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtAttributeDef;
}
class _Marshaller_CORBA_ExtAttributeDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtAttributeDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtAttributeDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtAttributeDef::~_Marshaller_CORBA_ExtAttributeDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtAttributeDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ExtAttributeDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ExtAttributeDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ExtAttributeDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ExtAttributeDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ExtAttributeDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ExtAttributeDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ExtAttributeDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtAttributeDef::typecode()
{
  return CORBA::_tc_ExtAttributeDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtAttributeDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ExtAttributeDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtAttributeDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ExtAttributeDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtAttributeDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ExtAttributeDef_ptr &_obj )
{
  CORBA::ExtAttributeDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ExtAttributeDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ExtAttributeDef
 */

CORBA::ExtAttributeDef_stub::~ExtAttributeDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ExtAttributeDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtAttributeDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::AttributeDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ExtAttributeDef *
POA_CORBA::ExtAttributeDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ExtAttributeDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ExtAttributeDef *) p;
  }
  return NULL;
}

CORBA::ExtAttributeDef_stub_clp::ExtAttributeDef_stub_clp ()
{
}

CORBA::ExtAttributeDef_stub_clp::ExtAttributeDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ExtAttributeDef_stub_clp::~ExtAttributeDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::ExceptionDefSeq* CORBA::ExtAttributeDef_stub::get_exceptions()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ExceptionDef );

  CORBA::StaticRequest __req( this, "_get_get_exceptions" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ExceptionDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ExceptionDefSeq*
CORBA::ExtAttributeDef_stub_clp::get_exceptions()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtAttributeDef * _myserv = POA_CORBA::ExtAttributeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExceptionDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->get_exceptions();
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

  return CORBA::ExtAttributeDef_stub::get_exceptions();
}

#endif // MICO_CONF_NO_POA

void CORBA::ExtAttributeDef_stub::get_exceptions( const CORBA::ExceptionDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExceptionDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_get_exceptions" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ExtAttributeDef_stub_clp::get_exceptions( const CORBA::ExceptionDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtAttributeDef * _myserv = POA_CORBA::ExtAttributeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->get_exceptions(_par__value);
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

  CORBA::ExtAttributeDef_stub::get_exceptions(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ExceptionDefSeq* CORBA::ExtAttributeDef_stub::set_exceptions()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ExceptionDef );

  CORBA::StaticRequest __req( this, "_get_set_exceptions" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ExceptionDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ExceptionDefSeq*
CORBA::ExtAttributeDef_stub_clp::set_exceptions()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtAttributeDef * _myserv = POA_CORBA::ExtAttributeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExceptionDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->set_exceptions();
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

  return CORBA::ExtAttributeDef_stub::set_exceptions();
}

#endif // MICO_CONF_NO_POA

void CORBA::ExtAttributeDef_stub::set_exceptions( const CORBA::ExceptionDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExceptionDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_set_exceptions" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ExtAttributeDef_stub_clp::set_exceptions( const CORBA::ExceptionDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtAttributeDef * _myserv = POA_CORBA::ExtAttributeDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->set_exceptions(_par__value);
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

  CORBA::ExtAttributeDef_stub::set_exceptions(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ExtAttributeDescription* CORBA::ExtAttributeDef_stub::describe_attribute()
{
  CORBA::StaticAny __res( _marshaller_CORBA_ExtAttributeDescription );

  CORBA::StaticRequest __req( this, "describe_attribute" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ExtAttributeDescription*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ExtAttributeDescription*
CORBA::ExtAttributeDef_stub_clp::describe_attribute()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ExtAttributeDef * _myserv = POA_CORBA::ExtAttributeDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExtAttributeDescription* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe_attribute();
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

  return CORBA::ExtAttributeDef_stub::describe_attribute();
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_ParameterMode;
}

void operator<<=( CORBA::Any &_a, const CORBA::ParameterMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ParameterMode, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ParameterMode &_e )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ParameterMode, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_CORBA_ParameterMode : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ParameterMode _MICO_T;
  public:
    ~_Marshaller_CORBA_ParameterMode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ParameterMode::~_Marshaller_CORBA_ParameterMode()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ParameterMode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ParameterMode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ParameterMode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ParameterMode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_CORBA_ParameterMode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ParameterMode::typecode()
{
  return CORBA::_tc_ParameterMode;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ParameterMode;

namespace CORBA
{
CORBA::TypeCodeConst _tc_ParameterDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ParameterDescription::ParameterDescription()
{
}

CORBA::ParameterDescription::ParameterDescription( const ParameterDescription& _s )
{
  name = ((ParameterDescription&)_s).name;
  type = ((ParameterDescription&)_s).type;
  type_def = ((ParameterDescription&)_s).type_def;
  mode = ((ParameterDescription&)_s).mode;
}

CORBA::ParameterDescription::~ParameterDescription()
{
}

CORBA::ParameterDescription&
CORBA::ParameterDescription::operator=( const ParameterDescription& _s )
{
  name = ((ParameterDescription&)_s).name;
  type = ((ParameterDescription&)_s).type;
  type_def = ((ParameterDescription&)_s).type_def;
  mode = ((ParameterDescription&)_s).mode;
  return *this;
}
#endif

class _Marshaller_CORBA_ParameterDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ParameterDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ParameterDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ParameterDescription::~_Marshaller_CORBA_ParameterDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ParameterDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ParameterDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ParameterDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ParameterDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    _marshaller_CORBA_IDLType->demarshal( dc, &((_MICO_T*)v)->type_def._for_demarshal() ) &&
    _marshaller_CORBA_ParameterMode->demarshal( dc, &((_MICO_T*)v)->mode ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ParameterDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  _marshaller_CORBA_IDLType->marshal( ec, &((_MICO_T*)v)->type_def.inout() );
  _marshaller_CORBA_ParameterMode->marshal( ec, &((_MICO_T*)v)->mode );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ParameterDescription::typecode()
{
  return CORBA::_tc_ParameterDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ParameterDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ParameterDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ParameterDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ParameterDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ParameterDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ParameterDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ParameterDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ParameterDescription, (void *&)_s);
}


/*
 * Base interface for class OperationDef
 */

CORBA::OperationDef::~OperationDef()
{
}

void *
CORBA::OperationDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/OperationDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::OperationDef_ptr
CORBA::OperationDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::OperationDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/OperationDef:1.0" )))
      return _duplicate( (CORBA::OperationDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/OperationDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/OperationDef:1.0")) {
      _o = new CORBA::OperationDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::OperationDef_ptr
CORBA::OperationDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_OperationDef;
}
class _Marshaller_CORBA_OperationDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::OperationDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_OperationDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_OperationDef::~_Marshaller_CORBA_OperationDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_OperationDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_OperationDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::OperationDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_OperationDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_OperationDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_OperationDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::OperationDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_OperationDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_OperationDef::typecode()
{
  return CORBA::_tc_OperationDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_OperationDef;

void
operator<<=( CORBA::Any &_a, const CORBA::OperationDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_OperationDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::OperationDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_OperationDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::OperationDef_ptr &_obj )
{
  CORBA::OperationDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_OperationDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class OperationDef
 */

CORBA::OperationDef_stub::~OperationDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::OperationDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/OperationDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::OperationDef *
POA_CORBA::OperationDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/OperationDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::OperationDef *) p;
  }
  return NULL;
}

CORBA::OperationDef_stub_clp::OperationDef_stub_clp ()
{
}

CORBA::OperationDef_stub_clp::OperationDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::OperationDef_stub_clp::~OperationDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::OperationDef_stub::result()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_result" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::OperationDef_stub_clp::result()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->result();
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

  return CORBA::OperationDef_stub::result();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::OperationDef_stub::result_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_result_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::OperationDef_stub_clp::result_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->result_def();
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

  return CORBA::OperationDef_stub::result_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::OperationDef_stub::result_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_result_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::OperationDef_stub_clp::result_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->result_def(_par__value);
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

  CORBA::OperationDef_stub::result_def(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ParDescriptionSeq* CORBA::OperationDef_stub::params()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ParameterDescription );

  CORBA::StaticRequest __req( this, "_get_params" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ParDescriptionSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ParDescriptionSeq*
CORBA::OperationDef_stub_clp::params()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ParDescriptionSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->params();
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

  return CORBA::OperationDef_stub::params();
}

#endif // MICO_CONF_NO_POA

void CORBA::OperationDef_stub::params( const CORBA::ParDescriptionSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ParameterDescription, &_par__value );
  CORBA::StaticRequest __req( this, "_set_params" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::OperationDef_stub_clp::params( const CORBA::ParDescriptionSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->params(_par__value);
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

  CORBA::OperationDef_stub::params(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::OperationMode CORBA::OperationDef_stub::mode()
{
  CORBA::OperationMode _res;
  CORBA::StaticAny __res( _marshaller_CORBA_OperationMode, &_res );

  CORBA::StaticRequest __req( this, "_get_mode" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::OperationMode
CORBA::OperationDef_stub_clp::mode()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::OperationMode __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->mode();
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

  return CORBA::OperationDef_stub::mode();
}

#endif // MICO_CONF_NO_POA

void CORBA::OperationDef_stub::mode( CORBA::OperationMode _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_OperationMode, &_par__value );
  CORBA::StaticRequest __req( this, "_set_mode" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::OperationDef_stub_clp::mode( CORBA::OperationMode _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->mode(_par__value);
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

  CORBA::OperationDef_stub::mode(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ContextIdSeq* CORBA::OperationDef_stub::contexts()
{
  CORBA::StaticAny __res( CORBA::_stcseq_string );

  CORBA::StaticRequest __req( this, "_get_contexts" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ContextIdSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ContextIdSeq*
CORBA::OperationDef_stub_clp::contexts()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ContextIdSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->contexts();
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

  return CORBA::OperationDef_stub::contexts();
}

#endif // MICO_CONF_NO_POA

void CORBA::OperationDef_stub::contexts( const CORBA::ContextIdSeq& _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stcseq_string, &_par__value );
  CORBA::StaticRequest __req( this, "_set_contexts" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::OperationDef_stub_clp::contexts( const CORBA::ContextIdSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->contexts(_par__value);
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

  CORBA::OperationDef_stub::contexts(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::ExceptionDefSeq* CORBA::OperationDef_stub::exceptions()
{
  CORBA::StaticAny __res( _marshaller__seq_CORBA_ExceptionDef );

  CORBA::StaticRequest __req( this, "_get_exceptions" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::ExceptionDefSeq*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::ExceptionDefSeq*
CORBA::OperationDef_stub_clp::exceptions()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      CORBA::ExceptionDefSeq* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->exceptions();
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

  return CORBA::OperationDef_stub::exceptions();
}

#endif // MICO_CONF_NO_POA

void CORBA::OperationDef_stub::exceptions( const CORBA::ExceptionDefSeq& _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExceptionDef, &_par__value );
  CORBA::StaticRequest __req( this, "_set_exceptions" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::OperationDef_stub_clp::exceptions( const CORBA::ExceptionDefSeq& _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::OperationDef * _myserv = POA_CORBA::OperationDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->exceptions(_par__value);
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

  CORBA::OperationDef_stub::exceptions(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_OperationDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::OperationDescription::OperationDescription()
{
}

CORBA::OperationDescription::OperationDescription( const OperationDescription& _s )
{
  name = ((OperationDescription&)_s).name;
  id = ((OperationDescription&)_s).id;
  defined_in = ((OperationDescription&)_s).defined_in;
  version = ((OperationDescription&)_s).version;
  result = ((OperationDescription&)_s).result;
  mode = ((OperationDescription&)_s).mode;
  contexts = ((OperationDescription&)_s).contexts;
  parameters = ((OperationDescription&)_s).parameters;
  exceptions = ((OperationDescription&)_s).exceptions;
}

CORBA::OperationDescription::~OperationDescription()
{
}

CORBA::OperationDescription&
CORBA::OperationDescription::operator=( const OperationDescription& _s )
{
  name = ((OperationDescription&)_s).name;
  id = ((OperationDescription&)_s).id;
  defined_in = ((OperationDescription&)_s).defined_in;
  version = ((OperationDescription&)_s).version;
  result = ((OperationDescription&)_s).result;
  mode = ((OperationDescription&)_s).mode;
  contexts = ((OperationDescription&)_s).contexts;
  parameters = ((OperationDescription&)_s).parameters;
  exceptions = ((OperationDescription&)_s).exceptions;
  return *this;
}
#endif

class _Marshaller_CORBA_OperationDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::OperationDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_OperationDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_OperationDescription::~_Marshaller_CORBA_OperationDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_OperationDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_OperationDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_OperationDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_OperationDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->result._for_demarshal() ) &&
    _marshaller_CORBA_OperationMode->demarshal( dc, &((_MICO_T*)v)->mode ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->contexts ) &&
    _marshaller__seq_CORBA_ParameterDescription->demarshal( dc, &((_MICO_T*)v)->parameters ) &&
    _marshaller__seq_CORBA_ExceptionDescription->demarshal( dc, &((_MICO_T*)v)->exceptions ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_OperationDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->result.inout() );
  _marshaller_CORBA_OperationMode->marshal( ec, &((_MICO_T*)v)->mode );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->contexts );
  _marshaller__seq_CORBA_ParameterDescription->marshal( ec, &((_MICO_T*)v)->parameters );
  _marshaller__seq_CORBA_ExceptionDescription->marshal( ec, &((_MICO_T*)v)->exceptions );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_OperationDescription::typecode()
{
  return CORBA::_tc_OperationDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_OperationDescription;

void operator<<=( CORBA::Any &_a, const CORBA::OperationDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_OperationDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::OperationDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::OperationDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_OperationDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::OperationDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_OperationDescription, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_InterfaceDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::InterfaceDescription::InterfaceDescription()
{
}

CORBA::InterfaceDescription::InterfaceDescription( const InterfaceDescription& _s )
{
  name = ((InterfaceDescription&)_s).name;
  id = ((InterfaceDescription&)_s).id;
  defined_in = ((InterfaceDescription&)_s).defined_in;
  version = ((InterfaceDescription&)_s).version;
  base_interfaces = ((InterfaceDescription&)_s).base_interfaces;
}

CORBA::InterfaceDescription::~InterfaceDescription()
{
}

CORBA::InterfaceDescription&
CORBA::InterfaceDescription::operator=( const InterfaceDescription& _s )
{
  name = ((InterfaceDescription&)_s).name;
  id = ((InterfaceDescription&)_s).id;
  defined_in = ((InterfaceDescription&)_s).defined_in;
  version = ((InterfaceDescription&)_s).version;
  base_interfaces = ((InterfaceDescription&)_s).base_interfaces;
  return *this;
}
#endif

class _Marshaller_CORBA_InterfaceDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::InterfaceDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_InterfaceDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_InterfaceDescription::~_Marshaller_CORBA_InterfaceDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_InterfaceDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_InterfaceDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_InterfaceDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_InterfaceDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->base_interfaces ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_InterfaceDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->base_interfaces );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_InterfaceDescription::typecode()
{
  return CORBA::_tc_InterfaceDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceDescription;

void operator<<=( CORBA::Any &_a, const CORBA::InterfaceDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::InterfaceDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::InterfaceDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::InterfaceDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_InterfaceDescription, (void *&)_s);
}

namespace CORBA
{
CORBA::TypeCodeConst InterfaceAttrExtension::_tc_ExtFullInterfaceDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription::ExtFullInterfaceDescription()
{
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription::ExtFullInterfaceDescription( const ExtFullInterfaceDescription& _s )
{
  name = ((ExtFullInterfaceDescription&)_s).name;
  id = ((ExtFullInterfaceDescription&)_s).id;
  defined_in = ((ExtFullInterfaceDescription&)_s).defined_in;
  version = ((ExtFullInterfaceDescription&)_s).version;
  operations = ((ExtFullInterfaceDescription&)_s).operations;
  attributes = ((ExtFullInterfaceDescription&)_s).attributes;
  base_interfaces = ((ExtFullInterfaceDescription&)_s).base_interfaces;
  type = ((ExtFullInterfaceDescription&)_s).type;
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription::~ExtFullInterfaceDescription()
{
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription&
CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription::operator=( const ExtFullInterfaceDescription& _s )
{
  name = ((ExtFullInterfaceDescription&)_s).name;
  id = ((ExtFullInterfaceDescription&)_s).id;
  defined_in = ((ExtFullInterfaceDescription&)_s).defined_in;
  version = ((ExtFullInterfaceDescription&)_s).version;
  operations = ((ExtFullInterfaceDescription&)_s).operations;
  attributes = ((ExtFullInterfaceDescription&)_s).attributes;
  base_interfaces = ((ExtFullInterfaceDescription&)_s).base_interfaces;
  type = ((ExtFullInterfaceDescription&)_s).type;
  return *this;
}
#endif

class _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::~_Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    _marshaller__seq_CORBA_OperationDescription->demarshal( dc, &((_MICO_T*)v)->operations ) &&
    _marshaller__seq_CORBA_ExtAttributeDescription->demarshal( dc, &((_MICO_T*)v)->attributes ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->base_interfaces ) &&
    CORBA::_stc_TypeCode->demarshal( dc, &((_MICO_T*)v)->type._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  _marshaller__seq_CORBA_OperationDescription->marshal( ec, &((_MICO_T*)v)->operations );
  _marshaller__seq_CORBA_ExtAttributeDescription->marshal( ec, &((_MICO_T*)v)->attributes );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->base_interfaces );
  CORBA::_stc_TypeCode->marshal( ec, &((_MICO_T*)v)->type.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription::typecode()
{
  return CORBA::InterfaceAttrExtension::_tc_ExtFullInterfaceDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription;

void operator<<=( CORBA::Any &_a, const CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription, (void *&)_s);
}


/*
 * Base interface for class InterfaceAttrExtension
 */

CORBA::InterfaceAttrExtension::~InterfaceAttrExtension()
{
}

void *
CORBA::InterfaceAttrExtension::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/InterfaceAttrExtension:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

CORBA::InterfaceAttrExtension_ptr
CORBA::InterfaceAttrExtension::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::InterfaceAttrExtension_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/InterfaceAttrExtension:1.0" )))
      return _duplicate( (CORBA::InterfaceAttrExtension_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/InterfaceAttrExtension:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/InterfaceAttrExtension:1.0")) {
      _o = new CORBA::InterfaceAttrExtension_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::InterfaceAttrExtension_ptr
CORBA::InterfaceAttrExtension::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_InterfaceAttrExtension;
}
class _Marshaller_CORBA_InterfaceAttrExtension : public ::CORBA::StaticTypeInfo {
    typedef CORBA::InterfaceAttrExtension_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_InterfaceAttrExtension();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_InterfaceAttrExtension::~_Marshaller_CORBA_InterfaceAttrExtension()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_InterfaceAttrExtension::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_InterfaceAttrExtension::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::InterfaceAttrExtension::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_InterfaceAttrExtension::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_InterfaceAttrExtension::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_InterfaceAttrExtension::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::InterfaceAttrExtension::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_InterfaceAttrExtension::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_InterfaceAttrExtension::typecode()
{
  return CORBA::_tc_InterfaceAttrExtension;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceAttrExtension;

void
operator<<=( CORBA::Any &_a, const CORBA::InterfaceAttrExtension_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceAttrExtension, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::InterfaceAttrExtension_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_InterfaceAttrExtension, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::InterfaceAttrExtension_ptr &_obj )
{
  CORBA::InterfaceAttrExtension_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_InterfaceAttrExtension, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class InterfaceAttrExtension
 */

CORBA::InterfaceAttrExtension_stub::~InterfaceAttrExtension_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::InterfaceAttrExtension::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/InterfaceAttrExtension:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_CORBA::InterfaceAttrExtension *
POA_CORBA::InterfaceAttrExtension::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/InterfaceAttrExtension:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::InterfaceAttrExtension *) p;
  }
  return NULL;
}

CORBA::InterfaceAttrExtension_stub_clp::InterfaceAttrExtension_stub_clp ()
{
}

CORBA::InterfaceAttrExtension_stub_clp::InterfaceAttrExtension_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::InterfaceAttrExtension_stub_clp::~InterfaceAttrExtension_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription* CORBA::InterfaceAttrExtension_stub::describe_ext_interface()
{
  CORBA::StaticAny __res( _marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription );

  CORBA::StaticRequest __req( this, "describe_ext_interface" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription*
CORBA::InterfaceAttrExtension_stub_clp::describe_ext_interface()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceAttrExtension * _myserv = POA_CORBA::InterfaceAttrExtension::_narrow (_serv);
    if (_myserv) {
      CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->describe_ext_interface();
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

  return CORBA::InterfaceAttrExtension_stub::describe_ext_interface();
}

#endif // MICO_CONF_NO_POA

CORBA::ExtAttributeDef_ptr CORBA::InterfaceAttrExtension_stub::create_ext_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode, const CORBA::ExceptionDefSeq& _par_get_exceptions, const CORBA::ExceptionDefSeq& _par_set_exceptions )
{
  CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id );
  CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name );
  CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version );
  CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type );
  CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );
  CORBA::StaticAny _sa_get_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_get_exceptions );
  CORBA::StaticAny _sa_set_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_set_exceptions );
  CORBA::ExtAttributeDef_ptr _res = CORBA::ExtAttributeDef::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_ExtAttributeDef, &_res );

  CORBA::StaticRequest __req( this, "create_ext_attribute" );
  __req.add_in_arg( &_sa_id );
  __req.add_in_arg( &_sa_name );
  __req.add_in_arg( &_sa_version );
  __req.add_in_arg( &_sa_type );
  __req.add_in_arg( &_sa_mode );
  __req.add_in_arg( &_sa_get_exceptions );
  __req.add_in_arg( &_sa_set_exceptions );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::ExtAttributeDef_ptr
CORBA::InterfaceAttrExtension_stub_clp::create_ext_attribute( const char* _par_id, const char* _par_name, const char* _par_version, CORBA::IDLType_ptr _par_type, CORBA::AttributeMode _par_mode, const CORBA::ExceptionDefSeq& _par_get_exceptions, const CORBA::ExceptionDefSeq& _par_set_exceptions )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::InterfaceAttrExtension * _myserv = POA_CORBA::InterfaceAttrExtension::_narrow (_serv);
    if (_myserv) {
      CORBA::ExtAttributeDef_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->create_ext_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode, _par_get_exceptions, _par_set_exceptions);
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

  return CORBA::InterfaceAttrExtension_stub::create_ext_attribute(_par_id, _par_name, _par_version, _par_type, _par_mode, _par_get_exceptions, _par_set_exceptions);
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ExtInterfaceDef
 */

CORBA::ExtInterfaceDef::~ExtInterfaceDef()
{
}

void *
CORBA::ExtInterfaceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ExtInterfaceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::InterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::InterfaceAttrExtension::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ExtInterfaceDef_ptr
CORBA::ExtInterfaceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ExtInterfaceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ExtInterfaceDef:1.0" )))
      return _duplicate( (CORBA::ExtInterfaceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ExtInterfaceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ExtInterfaceDef:1.0")) {
      _o = new CORBA::ExtInterfaceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ExtInterfaceDef_ptr
CORBA::ExtInterfaceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtInterfaceDef;
}
class _Marshaller_CORBA_ExtInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtInterfaceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtInterfaceDef::~_Marshaller_CORBA_ExtInterfaceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ExtInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ExtInterfaceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ExtInterfaceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ExtInterfaceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ExtInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ExtInterfaceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ExtInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtInterfaceDef::typecode()
{
  return CORBA::_tc_ExtInterfaceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtInterfaceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ExtInterfaceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtInterfaceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ExtInterfaceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtInterfaceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ExtInterfaceDef_ptr &_obj )
{
  CORBA::ExtInterfaceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ExtInterfaceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ExtInterfaceDef
 */

CORBA::ExtInterfaceDef_stub::~ExtInterfaceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ExtInterfaceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtInterfaceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::InterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::InterfaceAttrExtension::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ExtInterfaceDef *
POA_CORBA::ExtInterfaceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ExtInterfaceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ExtInterfaceDef *) p;
  }
  return NULL;
}

CORBA::ExtInterfaceDef_stub_clp::ExtInterfaceDef_stub_clp ()
{
}

CORBA::ExtInterfaceDef_stub_clp::ExtInterfaceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ExtInterfaceDef_stub_clp::~ExtInterfaceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ExtAbstractInterfaceDef
 */

CORBA::ExtAbstractInterfaceDef::~ExtAbstractInterfaceDef()
{
}

void *
CORBA::ExtAbstractInterfaceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ExtInterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::AbstractInterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::InterfaceAttrExtension::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ExtAbstractInterfaceDef_ptr
CORBA::ExtAbstractInterfaceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ExtAbstractInterfaceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0" )))
      return _duplicate( (CORBA::ExtAbstractInterfaceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0")) {
      _o = new CORBA::ExtAbstractInterfaceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ExtAbstractInterfaceDef_ptr
CORBA::ExtAbstractInterfaceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtAbstractInterfaceDef;
}
class _Marshaller_CORBA_ExtAbstractInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtAbstractInterfaceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtAbstractInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtAbstractInterfaceDef::~_Marshaller_CORBA_ExtAbstractInterfaceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtAbstractInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ExtAbstractInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ExtAbstractInterfaceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ExtAbstractInterfaceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ExtAbstractInterfaceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ExtAbstractInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ExtAbstractInterfaceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ExtAbstractInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtAbstractInterfaceDef::typecode()
{
  return CORBA::_tc_ExtAbstractInterfaceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtAbstractInterfaceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ExtAbstractInterfaceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtAbstractInterfaceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ExtAbstractInterfaceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtAbstractInterfaceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ExtAbstractInterfaceDef_ptr &_obj )
{
  CORBA::ExtAbstractInterfaceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ExtAbstractInterfaceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ExtAbstractInterfaceDef
 */

CORBA::ExtAbstractInterfaceDef_stub::~ExtAbstractInterfaceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ExtAbstractInterfaceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ExtInterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::AbstractInterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::InterfaceAttrExtension::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ExtAbstractInterfaceDef *
POA_CORBA::ExtAbstractInterfaceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ExtAbstractInterfaceDef *) p;
  }
  return NULL;
}

CORBA::ExtAbstractInterfaceDef_stub_clp::ExtAbstractInterfaceDef_stub_clp ()
{
}

CORBA::ExtAbstractInterfaceDef_stub_clp::ExtAbstractInterfaceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ExtAbstractInterfaceDef_stub_clp::~ExtAbstractInterfaceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ExtLocalInterfaceDef
 */

CORBA::ExtLocalInterfaceDef::~ExtLocalInterfaceDef()
{
}

void *
CORBA::ExtLocalInterfaceDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::ExtInterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::LocalInterfaceDef::_narrow_helper( _repoid )))
      return _p;
  }
  {
    void *_p;
    if ((_p = CORBA::InterfaceAttrExtension::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ExtLocalInterfaceDef_ptr
CORBA::ExtLocalInterfaceDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ExtLocalInterfaceDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0" )))
      return _duplicate( (CORBA::ExtLocalInterfaceDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0")) {
      _o = new CORBA::ExtLocalInterfaceDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ExtLocalInterfaceDef_ptr
CORBA::ExtLocalInterfaceDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ExtLocalInterfaceDef;
}
class _Marshaller_CORBA_ExtLocalInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ExtLocalInterfaceDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ExtLocalInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ExtLocalInterfaceDef::~_Marshaller_CORBA_ExtLocalInterfaceDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ExtLocalInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ExtLocalInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ExtLocalInterfaceDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ExtLocalInterfaceDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ExtLocalInterfaceDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ExtLocalInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ExtLocalInterfaceDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ExtLocalInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ExtLocalInterfaceDef::typecode()
{
  return CORBA::_tc_ExtLocalInterfaceDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ExtLocalInterfaceDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ExtLocalInterfaceDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtLocalInterfaceDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ExtLocalInterfaceDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ExtLocalInterfaceDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ExtLocalInterfaceDef_ptr &_obj )
{
  CORBA::ExtLocalInterfaceDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ExtLocalInterfaceDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ExtLocalInterfaceDef
 */

CORBA::ExtLocalInterfaceDef_stub::~ExtLocalInterfaceDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ExtLocalInterfaceDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::ExtInterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::LocalInterfaceDef::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  if ((p = POA_CORBA::InterfaceAttrExtension::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ExtLocalInterfaceDef *
POA_CORBA::ExtLocalInterfaceDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ExtLocalInterfaceDef *) p;
  }
  return NULL;
}

CORBA::ExtLocalInterfaceDef_stub_clp::ExtLocalInterfaceDef_stub_clp ()
{
}

CORBA::ExtLocalInterfaceDef_stub_clp::ExtLocalInterfaceDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ExtLocalInterfaceDef_stub_clp::~ExtLocalInterfaceDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA


/*
 * Base interface for class ValueMemberDef
 */

CORBA::ValueMemberDef::~ValueMemberDef()
{
}

void *
CORBA::ValueMemberDef::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:omg.org/CORBA/ValueMemberDef:1.0" ) == 0 )
    return (void *)this;
  {
    void *_p;
    if ((_p = CORBA::Contained::_narrow_helper( _repoid )))
      return _p;
  }
  return NULL;
}

CORBA::ValueMemberDef_ptr
CORBA::ValueMemberDef::_narrow( CORBA::Object_ptr _obj )
{
  CORBA::ValueMemberDef_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:omg.org/CORBA/ValueMemberDef:1.0" )))
      return _duplicate( (CORBA::ValueMemberDef_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:omg.org/CORBA/ValueMemberDef:1.0") || _obj->_is_a_remote ("IDL:omg.org/CORBA/ValueMemberDef:1.0")) {
      _o = new CORBA::ValueMemberDef_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

CORBA::ValueMemberDef_ptr
CORBA::ValueMemberDef::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace CORBA
{
CORBA::TypeCodeConst _tc_ValueMemberDef;
}
class _Marshaller_CORBA_ValueMemberDef : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ValueMemberDef_ptr _MICO_T;
  public:
    ~_Marshaller_CORBA_ValueMemberDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ValueMemberDef::~_Marshaller_CORBA_ValueMemberDef()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ValueMemberDef::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_CORBA_ValueMemberDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::CORBA::ValueMemberDef::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_CORBA_ValueMemberDef::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_CORBA_ValueMemberDef::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_CORBA_ValueMemberDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::CORBA::ValueMemberDef::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_CORBA_ValueMemberDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ValueMemberDef::typecode()
{
  return CORBA::_tc_ValueMemberDef;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ValueMemberDef;

void
operator<<=( CORBA::Any &_a, const CORBA::ValueMemberDef_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueMemberDef, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, CORBA::ValueMemberDef_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueMemberDef, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, CORBA::ValueMemberDef_ptr &_obj )
{
  CORBA::ValueMemberDef_ptr *p;
  if (_a.to_static_any (_marshaller_CORBA_ValueMemberDef, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class ValueMemberDef
 */

CORBA::ValueMemberDef_stub::~ValueMemberDef_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_CORBA::ValueMemberDef::_narrow_helper (const char * repoid)
{
  void * p;
  if (strcmp (repoid, "IDL:omg.org/CORBA/ValueMemberDef:1.0") == 0) {
    return (void *) this;
  }
  if ((p = POA_CORBA::Contained::_narrow_helper (repoid)) != NULL) 
  {
    return p;
  }
  return NULL;
}

POA_CORBA::ValueMemberDef *
POA_CORBA::ValueMemberDef::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:omg.org/CORBA/ValueMemberDef:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_CORBA::ValueMemberDef *) p;
  }
  return NULL;
}

CORBA::ValueMemberDef_stub_clp::ValueMemberDef_stub_clp ()
{
}

CORBA::ValueMemberDef_stub_clp::ValueMemberDef_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

CORBA::ValueMemberDef_stub_clp::~ValueMemberDef_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

CORBA::TypeCode_ptr CORBA::ValueMemberDef_stub::type()
{
  CORBA::TypeCode_ptr _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );

  CORBA::StaticRequest __req( this, "_get_type" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::TypeCode_ptr
CORBA::ValueMemberDef_stub_clp::type()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueMemberDef * _myserv = POA_CORBA::ValueMemberDef::_narrow (_serv);
    if (_myserv) {
      CORBA::TypeCode_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type();
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

  return CORBA::ValueMemberDef_stub::type();
}

#endif // MICO_CONF_NO_POA

CORBA::IDLType_ptr CORBA::ValueMemberDef_stub::type_def()
{
  CORBA::IDLType_ptr _res = CORBA::IDLType::_nil();
  CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );

  CORBA::StaticRequest __req( this, "_get_type_def" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::IDLType_ptr
CORBA::ValueMemberDef_stub_clp::type_def()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueMemberDef * _myserv = POA_CORBA::ValueMemberDef::_narrow (_serv);
    if (_myserv) {
      CORBA::IDLType_ptr __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->type_def();
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

  return CORBA::ValueMemberDef_stub::type_def();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueMemberDef_stub::type_def( CORBA::IDLType_ptr _par__value )
{
  CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value );
  CORBA::StaticRequest __req( this, "_set_type_def" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueMemberDef_stub_clp::type_def( CORBA::IDLType_ptr _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueMemberDef * _myserv = POA_CORBA::ValueMemberDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->type_def(_par__value);
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

  CORBA::ValueMemberDef_stub::type_def(_par__value);
}

#endif // MICO_CONF_NO_POA

CORBA::Visibility CORBA::ValueMemberDef_stub::access()
{
  CORBA::Visibility _res;
  CORBA::StaticAny __res( CORBA::_stc_short, &_res );

  CORBA::StaticRequest __req( this, "_get_access" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Visibility
CORBA::ValueMemberDef_stub_clp::access()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueMemberDef * _myserv = POA_CORBA::ValueMemberDef::_narrow (_serv);
    if (_myserv) {
      CORBA::Visibility __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->access();
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

  return CORBA::ValueMemberDef_stub::access();
}

#endif // MICO_CONF_NO_POA

void CORBA::ValueMemberDef_stub::access( CORBA::Visibility _par__value )
{
  CORBA::StaticAny _sa__value( CORBA::_stc_short, &_par__value );
  CORBA::StaticRequest __req( this, "_set_access" );
  __req.add_in_arg( &_sa__value );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
CORBA::ValueMemberDef_stub_clp::access( CORBA::Visibility _par__value )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_CORBA::ValueMemberDef * _myserv = POA_CORBA::ValueMemberDef::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->access(_par__value);
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

  CORBA::ValueMemberDef_stub::access(_par__value);
}

#endif // MICO_CONF_NO_POA

namespace CORBA
{
CORBA::TypeCodeConst _tc_ValueDescription;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
CORBA::ValueDescription::ValueDescription()
{
}

CORBA::ValueDescription::ValueDescription( const ValueDescription& _s )
{
  name = ((ValueDescription&)_s).name;
  id = ((ValueDescription&)_s).id;
  is_abstract = ((ValueDescription&)_s).is_abstract;
  is_custom = ((ValueDescription&)_s).is_custom;
  defined_in = ((ValueDescription&)_s).defined_in;
  version = ((ValueDescription&)_s).version;
  supported_interfaces = ((ValueDescription&)_s).supported_interfaces;
  abstract_base_values = ((ValueDescription&)_s).abstract_base_values;
  is_truncatable = ((ValueDescription&)_s).is_truncatable;
  base_value = ((ValueDescription&)_s).base_value;
}

CORBA::ValueDescription::~ValueDescription()
{
}

CORBA::ValueDescription&
CORBA::ValueDescription::operator=( const ValueDescription& _s )
{
  name = ((ValueDescription&)_s).name;
  id = ((ValueDescription&)_s).id;
  is_abstract = ((ValueDescription&)_s).is_abstract;
  is_custom = ((ValueDescription&)_s).is_custom;
  defined_in = ((ValueDescription&)_s).defined_in;
  version = ((ValueDescription&)_s).version;
  supported_interfaces = ((ValueDescription&)_s).supported_interfaces;
  abstract_base_values = ((ValueDescription&)_s).abstract_base_values;
  is_truncatable = ((ValueDescription&)_s).is_truncatable;
  base_value = ((ValueDescription&)_s).base_value;
  return *this;
}
#endif

class _Marshaller_CORBA_ValueDescription : public ::CORBA::StaticTypeInfo {
    typedef CORBA::ValueDescription _MICO_T;
  public:
    ~_Marshaller_CORBA_ValueDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_CORBA_ValueDescription::~_Marshaller_CORBA_ValueDescription()
{
}

::CORBA::StaticValueType _Marshaller_CORBA_ValueDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_CORBA_ValueDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_CORBA_ValueDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_CORBA_ValueDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->id._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_abstract ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_custom ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->defined_in._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->version._for_demarshal() ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->supported_interfaces ) &&
    CORBA::_stcseq_string->demarshal( dc, &((_MICO_T*)v)->abstract_base_values ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->is_truncatable ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->base_value._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_CORBA_ValueDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->id.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_abstract );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_custom );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->defined_in.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->version.inout() );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->supported_interfaces );
  CORBA::_stcseq_string->marshal( ec, &((_MICO_T*)v)->abstract_base_values );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->is_truncatable );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->base_value.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_CORBA_ValueDescription::typecode()
{
  return CORBA::_tc_ValueDescription;
}

::CORBA::StaticTypeInfo *_marshaller_CORBA_ValueDescription;

void operator<<=( CORBA::Any &_a, const CORBA::ValueDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, CORBA::ValueDescription *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, CORBA::ValueDescription &_s )
{
  CORBA::StaticAny _sa (_marshaller_CORBA_ValueDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const CORBA::ValueDescription *&_s )
{
  return _a.to_static_any (_marshaller_CORBA_ValueDescription, (void *&)_s);
}

class _Marshaller__seq_CORBA_Contained : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_Contained();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_Contained::~_Marshaller__seq_CORBA_Contained()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_Contained::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_Contained::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_Contained::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_Contained::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_Contained->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_Contained::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_Contained->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_Contained::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000048000000010000000e00000036000000010000002000"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6e7461696e65643a"
    "312e30000a000000436f6e7461696e656400000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_Contained::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_Contained;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_Contained, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_Contained, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_Contained, (void *&)_s);
}


class _Marshaller__seq_CORBA_Container_Description : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_Container_Description();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_Container_Description::~_Marshaller__seq_CORBA_Container_Description()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_Container_Description::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_Container_Description::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_Container_Description::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_Container_Description::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_Container_Description->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_Container_Description::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_Container_Description->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_Container_Description::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000094030000010000000f00000084030000010000002c00"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6e7461696e65722f"
    "4465736372697074696f6e3a312e30000c0000004465736372697074696f"
    "6e000300000011000000636f6e7461696e65645f6f626a65637400000000"
    "0e00000036000000010000002000000049444c3a6f6d672e6f72672f434f"
    "5242412f436f6e7461696e65643a312e30000a000000436f6e7461696e65"
    "64000000050000006b696e640000000011000000bd020000010000002500"
    "000049444c3a6f6d672e6f72672f434f5242412f446566696e6974696f6e"
    "4b696e643a312e30000000000f000000446566696e6974696f6e4b696e64"
    "00002500000008000000646b5f6e6f6e650007000000646b5f616c6c0000"
    "0d000000646b5f417474726962757465000000000c000000646b5f436f6e"
    "7374616e74000d000000646b5f457863657074696f6e000000000d000000"
    "646b5f496e74657266616365000000000a000000646b5f4d6f64756c6500"
    "00000d000000646b5f4f7065726174696f6e000000000b000000646b5f54"
    "797065646566000009000000646b5f416c696173000000000a000000646b"
    "5f53747275637400000009000000646b5f556e696f6e0000000008000000"
    "646b5f456e756d000d000000646b5f5072696d6974697665000000000a00"
    "0000646b5f537472696e670000000c000000646b5f53657175656e636500"
    "09000000646b5f4172726179000000000e000000646b5f5265706f736974"
    "6f72790000000b000000646b5f57737472696e67000009000000646b5f46"
    "697865640000000009000000646b5f56616c7565000000000c000000646b"
    "5f56616c7565426f78000f000000646b5f56616c75654d656d6265720000"
    "0a000000646b5f4e617469766500000015000000646b5f41627374726163"
    "74496e746572666163650000000012000000646b5f4c6f63616c496e7465"
    "72666163650000000d000000646b5f436f6d706f6e656e74000000000800"
    "0000646b5f486f6d65000b000000646b5f466163746f727900000a000000"
    "646b5f46696e6465720000000e000000646b5f5072696d6172794b657900"
    "000009000000646b5f456d697473000000000d000000646b5f5075626c69"
    "73686573000000000c000000646b5f436f6e73756d6573000c000000646b"
    "5f50726f76696465730008000000646b5f557365730009000000646b5f45"
    "76656e74000000000600000076616c75650000000b00000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_Container_Description::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_Container_Description;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_Container_Description, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_Container_Description, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_Container_Description, (void *&)_s);
}


class _Marshaller__seq_CORBA_InterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_InterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_InterfaceDef::~_Marshaller__seq_CORBA_InterfaceDef()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_InterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_InterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_InterfaceDef::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_InterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_InterfaceDef->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_InterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_InterfaceDef->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_InterfaceDef::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000050000000010000000e0000003d000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f496e7465726661636544"
    "65663a312e3000000d000000496e74657266616365446566000000000000"
    "0000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_InterfaceDef::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_InterfaceDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_InterfaceDef, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_InterfaceDef, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_InterfaceDef, (void *&)_s);
}


class _Marshaller__seq_CORBA_AbstractInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_AbstractInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_AbstractInterfaceDef::~_Marshaller__seq_CORBA_AbstractInterfaceDef()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_AbstractInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_AbstractInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_AbstractInterfaceDef::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_AbstractInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_AbstractInterfaceDef->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_AbstractInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_AbstractInterfaceDef->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_AbstractInterfaceDef::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000060000000010000000e0000004d000000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f4162737472616374496e"
    "746572666163654465663a312e300000150000004162737472616374496e"
    "746572666163654465660000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_AbstractInterfaceDef::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_AbstractInterfaceDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_AbstractInterfaceDef, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_AbstractInterfaceDef, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_AbstractInterfaceDef, (void *&)_s);
}


class _Marshaller__seq_CORBA_ValueDef : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ValueDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ValueDef::~_Marshaller__seq_CORBA_ValueDef()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ValueDef::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ValueDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ValueDef::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ValueDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ValueDef->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ValueDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ValueDef->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ValueDef::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000048000000010000000e00000035000000010000001f00"
    "000049444c3a6f6d672e6f72672f434f5242412f56616c75654465663a31"
    "2e3000000900000056616c75654465660000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ValueDef::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ValueDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ValueDef, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ValueDef, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ValueDef, (void *&)_s);
}


class _Marshaller__seq_CORBA_Initializer : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_Initializer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_Initializer::~_Marshaller__seq_CORBA_Initializer()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_Initializer::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_Initializer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_Initializer::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_Initializer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_Initializer->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_Initializer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_Initializer->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_Initializer::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000010020000010000000f00000000020000010000002200"
    "000049444c3a6f6d672e6f72672f434f5242412f496e697469616c697a65"
    "723a312e300000000c000000496e697469616c697a657200020000000800"
    "00006d656d62657273001500000054010000010000002600000049444c3a"
    "6f6d672e6f72672f434f5242412f5374727563744d656d6265725365713a"
    "312e30000000100000005374727563744d656d6265725365710013000000"
    "08010000010000000f000000f8000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f5374727563744d656d6265723a312e300000"
    "0d0000005374727563744d656d6265720000000003000000050000006e61"
    "6d65000000001500000044000000010000002100000049444c3a6f6d672e"
    "6f72672f434f5242412f4964656e7469666965723a312e30000000000b00"
    "00004964656e746966696572000012000000000000000500000074797065"
    "000000000c00000009000000747970655f646566000000000e0000003400"
    "0000010000001e00000049444c3a6f6d672e6f72672f434f5242412f4944"
    "4c547970653a312e300000000800000049444c5479706500000000000500"
    "00006e616d65000000001500000044000000010000002100000049444c3a"
    "6f6d672e6f72672f434f5242412f4964656e7469666965723a312e300000"
    "00000b0000004964656e7469666965720000120000000000000000000000"
    ))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_Initializer::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_Initializer;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_Initializer, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_Initializer, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_Initializer, (void *&)_s);
}


class _Marshaller__seq_CORBA_ExtInitializer : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ExtInitializer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ExtInitializer::~_Marshaller__seq_CORBA_ExtInitializer()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ExtInitializer::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ExtInitializer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ExtInitializer::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ExtInitializer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ExtInitializer->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ExtInitializer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ExtInitializer->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExtInitializer::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000014050000010000000f00000004050000010000002500"
    "000049444c3a6f6d672e6f72672f434f5242412f457874496e697469616c"
    "697a65723a312e30000000000f000000457874496e697469616c697a6572"
    "000004000000080000006d656d6265727300150000005401000001000000"
    "2600000049444c3a6f6d672e6f72672f434f5242412f5374727563744d65"
    "6d6265725365713a312e30000000100000005374727563744d656d626572"
    "536571001300000008010000010000000f000000f8000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5374727563744d656d62"
    "65723a312e3000000d0000005374727563744d656d626572000000000300"
    "0000050000006e616d650000000015000000440000000100000021000000"
    "49444c3a6f6d672e6f72672f434f5242412f4964656e7469666965723a31"
    "2e30000000000b0000004964656e74696669657200001200000000000000"
    "0500000074797065000000000c00000009000000747970655f6465660000"
    "00000e00000034000000010000001e00000049444c3a6f6d672e6f72672f"
    "434f5242412f49444c547970653a312e300000000800000049444c547970"
    "6500000000000f000000657863657074696f6e735f646566000015000000"
    "9c000000010000002600000049444c3a6f6d672e6f72672f434f5242412f"
    "457863657074696f6e4465665365713a312e300000001000000045786365"
    "7074696f6e446566536571001300000050000000010000000e0000003d00"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f4578"
    "63657074696f6e4465663a312e3000000d000000457863657074696f6e44"
    "656600000000000000000b000000657863657074696f6e73000015000000"
    "2c020000010000002800000049444c3a6f6d672e6f72672f434f5242412f"
    "4578634465736372697074696f6e5365713a312e30001200000045786344"
    "65736372697074696f6e53657100000013000000dc010000010000000f00"
    "0000cc010000010000002b00000049444c3a6f6d672e6f72672f434f5242"
    "412f457863657074696f6e4465736372697074696f6e3a312e3000001500"
    "0000457863657074696f6e4465736372697074696f6e0000000005000000"
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
    "0500000074797065000000000c00000000000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e7469666965720000120000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExtInitializer::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExtInitializer;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExtInitializer, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExtInitializer, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ExtInitializer, (void *&)_s);
}


class _Marshaller__seq_CORBA_OperationDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_OperationDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_OperationDescription::~_Marshaller__seq_CORBA_OperationDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_OperationDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_OperationDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_OperationDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_OperationDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_OperationDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_OperationDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_OperationDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_OperationDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000a4070000010000000f00000094070000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f4f7065726174696f6e44"
    "65736372697074696f6e3a312e300000150000004f7065726174696f6e44"
    "65736372697074696f6e0000000009000000050000006e616d6500000000"
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
    "7273696f6e5370656300120000000000000007000000726573756c740000"
    "0c000000050000006d6f6465000000001100000062000000010000002400"
    "000049444c3a6f6d672e6f72672f434f5242412f4f7065726174696f6e4d"
    "6f64653a312e30000e0000004f7065726174696f6e4d6f64650000000200"
    "00000a0000004f505f4e4f524d414c0000000a0000004f505f4f4e455741"
    "5900000009000000636f6e74657874730000000015000000ec0000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f436f6e746578"
    "7449645365713a312e3000000d000000436f6e7465787449645365710000"
    "000013000000a40000000100000015000000940000000100000028000000"
    "49444c3a6f6d672e6f72672f434f5242412f436f6e746578744964656e74"
    "69666965723a312e300012000000436f6e746578744964656e7469666965"
    "720000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f4964656e7469666965723a312e30000000000b000000"
    "4964656e74696669657200001200000000000000000000000b0000007061"
    "72616d6574657273000015000000f0010000010000002800000049444c3a"
    "6f6d672e6f72672f434f5242412f5061724465736372697074696f6e5365"
    "713a312e3000120000005061724465736372697074696f6e536571000000"
    "13000000a0010000010000000f00000090010000010000002b0000004944"
    "4c3a6f6d672e6f72672f434f5242412f506172616d657465724465736372"
    "697074696f6e3a312e30000015000000506172616d657465724465736372"
    "697074696f6e0000000004000000050000006e616d650000000015000000"
    "44000000010000002100000049444c3a6f6d672e6f72672f434f5242412f"
    "4964656e7469666965723a312e30000000000b0000004964656e74696669"
    "6572000012000000000000000500000074797065000000000c0000000900"
    "0000747970655f646566000000000e00000034000000010000001e000000"
    "49444c3a6f6d672e6f72672f434f5242412f49444c547970653a312e3000"
    "00000800000049444c5479706500050000006d6f64650000000011000000"
    "74000000010000002400000049444c3a6f6d672e6f72672f434f5242412f"
    "506172616d657465724d6f64653a312e30000e000000506172616d657465"
    "724d6f64650000000300000009000000504152414d5f494e000000000a00"
    "0000504152414d5f4f55540000000c000000504152414d5f494e4f555400"
    "000000000b000000657863657074696f6e730000150000002c0200000100"
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
    "7065000000000c0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_OperationDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_OperationDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_OperationDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_OperationDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_OperationDescription, (void *&)_s);
}


class _Marshaller__seq_CORBA_AttributeDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_AttributeDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_AttributeDescription::~_Marshaller__seq_CORBA_AttributeDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_AttributeDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_AttributeDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_AttributeDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_AttributeDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_AttributeDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_AttributeDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_AttributeDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_AttributeDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000058020000010000000f00000046020000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f41747472696275746544"
    "65736372697074696f6e3a312e3000001500000041747472696275746544"
    "65736372697074696f6e0000000006000000050000006e616d6500000000"
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
    "7273696f6e53706563001200000000000000050000007479706500000000"
    "0c000000050000006d6f6465000000001100000066000000010000002400"
    "000049444c3a6f6d672e6f72672f434f5242412f4174747269627574654d"
    "6f64653a312e30000e0000004174747269627574654d6f64650000000200"
    "00000c000000415454525f4e4f524d414c000e000000415454525f524541"
    "444f4e4c5900000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_AttributeDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_AttributeDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_AttributeDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_AttributeDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_AttributeDescription, (void *&)_s);
}


class _Marshaller__seq_CORBA_ParameterDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ParameterDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ParameterDescription::~_Marshaller__seq_CORBA_ParameterDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ParameterDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ParameterDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ParameterDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ParameterDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ParameterDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ParameterDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ParameterDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ParameterDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000a0010000010000000f00000090010000010000002b00"
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
    "4f55540000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ParameterDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ParameterDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ParameterDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ParameterDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ParameterDescription, (void *&)_s);
}


class _Marshaller__seq_CORBA_ExceptionDef : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ExceptionDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ExceptionDef::~_Marshaller__seq_CORBA_ExceptionDef()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ExceptionDef::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ExceptionDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ExceptionDef::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ExceptionDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ExceptionDef->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ExceptionDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ExceptionDef->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExceptionDef::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000050000000010000000e0000003d000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f457863657074696f6e44"
    "65663a312e3000000d000000457863657074696f6e446566000000000000"
    "0000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExceptionDef::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExceptionDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExceptionDef, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExceptionDef, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ExceptionDef, (void *&)_s);
}


class _Marshaller__seq_CORBA_ExtAttributeDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ExtAttributeDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ExtAttributeDescription::~_Marshaller__seq_CORBA_ExtAttributeDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ExtAttributeDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ExtAttributeDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ExtAttributeDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ExtAttributeDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ExtAttributeDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ExtAttributeDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ExtAttributeDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExtAttributeDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000ec060000010000000f000000dc060000010000002e00"
    "000049444c3a6f6d672e6f72672f434f5242412f45787441747472696275"
    "74654465736372697074696f6e3a312e3000000018000000457874417474"
    "7269627574654465736372697074696f6e0008000000050000006e616d65"
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
    "000000000c000000050000006d6f64650000000011000000660000000100"
    "00002400000049444c3a6f6d672e6f72672f434f5242412f417474726962"
    "7574654d6f64653a312e30000e0000004174747269627574654d6f646500"
    "0000020000000c000000415454525f4e4f524d414c000e00000041545452"
    "5f524541444f4e4c590000000f0000006765745f657863657074696f6e73"
    "0000150000002c020000010000002800000049444c3a6f6d672e6f72672f"
    "434f5242412f4578634465736372697074696f6e5365713a312e30001200"
    "00004578634465736372697074696f6e53657100000013000000dc010000"
    "010000000f000000cc010000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f457863657074696f6e4465736372697074696f6e3a31"
    "2e30000015000000457863657074696f6e4465736372697074696f6e0000"
    "000005000000050000006e616d6500000000150000004400000001000000"
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
    "0000000000000500000074797065000000000c000000000000000f000000"
    "7365745f657863657074696f6e730000150000002c020000010000002800"
    "000049444c3a6f6d672e6f72672f434f5242412f45786344657363726970"
    "74696f6e5365713a312e3000120000004578634465736372697074696f6e"
    "53657100000013000000dc010000010000000f000000cc01000001000000"
    "2b00000049444c3a6f6d672e6f72672f434f5242412f457863657074696f"
    "6e4465736372697074696f6e3a312e30000015000000457863657074696f"
    "6e4465736372697074696f6e0000000005000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000030000006964000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000b000000646566696e65645f696e"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000800000076657273"
    "696f6e001500000044000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f56657273696f6e537065633a312e300000000c000000"
    "56657273696f6e5370656300120000000000000005000000747970650000"
    "00000c0000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExtAttributeDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExtAttributeDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExtAttributeDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExtAttributeDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ExtAttributeDescription, (void *&)_s);
}


class _Marshaller__seq_CORBA_LocalInterfaceDef : public ::CORBA::StaticTypeInfo {
    typedef IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_LocalInterfaceDef();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_LocalInterfaceDef::~_Marshaller__seq_CORBA_LocalInterfaceDef()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_LocalInterfaceDef::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_LocalInterfaceDef::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_LocalInterfaceDef::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_LocalInterfaceDef::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_LocalInterfaceDef->demarshal( dc, &(*(_MICO_T*)v)[i]._for_demarshal() ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_LocalInterfaceDef::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_LocalInterfaceDef->marshal( ec, &(*(_MICO_T*)v)[i].inout() );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_LocalInterfaceDef::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000058000000010000000e00000046000000010000002800"
    "000049444c3a6f6d672e6f72672f434f5242412f4c6f63616c496e746572"
    "666163654465663a312e3000120000004c6f63616c496e74657266616365"
    "44656600000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_LocalInterfaceDef::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_LocalInterfaceDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_LocalInterfaceDef, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_LocalInterfaceDef, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_LocalInterfaceDef, (void *&)_s);
}


class _Marshaller__seq_CORBA_ExceptionDescription : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_CORBA_ExceptionDescription();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_CORBA_ExceptionDescription::~_Marshaller__seq_CORBA_ExceptionDescription()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_CORBA_ExceptionDescription::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_CORBA_ExceptionDescription::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_CORBA_ExceptionDescription::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_CORBA_ExceptionDescription::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_CORBA_ExceptionDescription->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_CORBA_ExceptionDescription::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_CORBA_ExceptionDescription->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExceptionDescription::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000dc010000010000000f000000cc010000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f457863657074696f6e44"
    "65736372697074696f6e3a312e30000015000000457863657074696f6e44"
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
    "7273696f6e53706563001200000000000000050000007479706500000000"
    "0c00000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_CORBA_ExceptionDescription::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExceptionDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExceptionDescription, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_CORBA_ExceptionDescription, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_CORBA_ExceptionDescription, (void *&)_s);
}


struct __tc_init_IR {
  __tc_init_IR()
  {
    CORBA::_tc_ScopedName = 
    "010000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f53636f7065644e616d653a312e30000000000b000000"
    "53636f7065644e616d6500001200000000000000";
    CORBA::Contained::_tc_Description = 
    "010000000f0000002c030000010000002c00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7461696e65642f4465736372697074696f6e3a"
    "312e30000c0000004465736372697074696f6e0002000000050000006b69"
    "6e640000000011000000bd020000010000002500000049444c3a6f6d672e"
    "6f72672f434f5242412f446566696e6974696f6e4b696e643a312e300000"
    "00000f000000446566696e6974696f6e4b696e6400002500000008000000"
    "646b5f6e6f6e650007000000646b5f616c6c00000d000000646b5f417474"
    "726962757465000000000c000000646b5f436f6e7374616e74000d000000"
    "646b5f457863657074696f6e000000000d000000646b5f496e7465726661"
    "6365000000000a000000646b5f4d6f64756c650000000d000000646b5f4f"
    "7065726174696f6e000000000b000000646b5f5479706564656600000900"
    "0000646b5f416c696173000000000a000000646b5f537472756374000000"
    "09000000646b5f556e696f6e0000000008000000646b5f456e756d000d00"
    "0000646b5f5072696d6974697665000000000a000000646b5f537472696e"
    "670000000c000000646b5f53657175656e63650009000000646b5f417272"
    "6179000000000e000000646b5f5265706f7369746f72790000000b000000"
    "646b5f57737472696e67000009000000646b5f4669786564000000000900"
    "0000646b5f56616c7565000000000c000000646b5f56616c7565426f7800"
    "0f000000646b5f56616c75654d656d62657200000a000000646b5f4e6174"
    "69766500000015000000646b5f4162737472616374496e74657266616365"
    "0000000012000000646b5f4c6f63616c496e746572666163650000000d00"
    "0000646b5f436f6d706f6e656e740000000008000000646b5f486f6d6500"
    "0b000000646b5f466163746f727900000a000000646b5f46696e64657200"
    "00000e000000646b5f5072696d6172794b657900000009000000646b5f45"
    "6d697473000000000d000000646b5f5075626c6973686573000000000c00"
    "0000646b5f436f6e73756d6573000c000000646b5f50726f766964657300"
    "08000000646b5f557365730009000000646b5f4576656e74000000000600"
    "000076616c75650000000b000000";
    _marshaller_CORBA_Contained_Description = new _Marshaller_CORBA_Contained_Description;
    CORBA::_tc_Contained = 
    "010000000e00000036000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7461696e65643a312e30000a000000436f6e74"
    "61696e656400";
    _marshaller_CORBA_Contained = new _Marshaller_CORBA_Contained;
    CORBA::_tc_ContainedSeq = 
    "010000001500000090000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7461696e65645365713a312e3000000d000000"
    "436f6e7461696e6564536571000000001300000048000000010000000e00"
    "000036000000010000002000000049444c3a6f6d672e6f72672f434f5242"
    "412f436f6e7461696e65643a312e30000a000000436f6e7461696e656400"
    "000000000000";
    CORBA::_tc_InterfaceDefSeq = 
    "01000000150000009c000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f496e746572666163654465665365713a312e30000000"
    "10000000496e746572666163654465665365710013000000500000000100"
    "00000e0000003d000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f496e746572666163654465663a312e3000000d000000496e"
    "746572666163654465660000000000000000";
    CORBA::_tc_AbstractInterfaceDefSeq = 
    "0100000015000000bc000000010000002e00000049444c3a6f6d672e6f72"
    "672f434f5242412f4162737472616374496e746572666163654465665365"
    "713a312e30000000180000004162737472616374496e7465726661636544"
    "6566536571001300000060000000010000000e0000004d00000001000000"
    "2b00000049444c3a6f6d672e6f72672f434f5242412f4162737472616374"
    "496e746572666163654465663a312e300000150000004162737472616374"
    "496e746572666163654465660000000000000000";
    CORBA::_tc_ValueDefSeq = 
    "01000000150000008c000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f56616c75654465665365713a312e300000000c000000"
    "56616c7565446566536571001300000048000000010000000e0000003500"
    "0000010000001f00000049444c3a6f6d672e6f72672f434f5242412f5661"
    "6c75654465663a312e3000000900000056616c7565446566000000000000"
    "0000";
    CORBA::_tc_InitializerSeq = 
    "01000000150000005c020000010000002500000049444c3a6f6d672e6f72"
    "672f434f5242412f496e697469616c697a65725365713a312e3000000000"
    "0f000000496e697469616c697a6572536571000013000000100200000100"
    "00000f00000000020000010000002200000049444c3a6f6d672e6f72672f"
    "434f5242412f496e697469616c697a65723a312e300000000c000000496e"
    "697469616c697a65720002000000080000006d656d626572730015000000"
    "54010000010000002600000049444c3a6f6d672e6f72672f434f5242412f"
    "5374727563744d656d6265725365713a312e300000001000000053747275"
    "63744d656d626572536571001300000008010000010000000f000000f800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5374"
    "727563744d656d6265723a312e3000000d0000005374727563744d656d62"
    "65720000000003000000050000006e616d65000000001500000044000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f4964656e"
    "7469666965723a312e30000000000b0000004964656e7469666965720000"
    "12000000000000000500000074797065000000000c000000090000007479"
    "70655f646566000000000e00000034000000010000001e00000049444c3a"
    "6f6d672e6f72672f434f5242412f49444c547970653a312e300000000800"
    "000049444c547970650000000000050000006e616d650000000015000000"
    "44000000010000002100000049444c3a6f6d672e6f72672f434f5242412f"
    "4964656e7469666965723a312e30000000000b0000004964656e74696669"
    "65720000120000000000000000000000";
    CORBA::_tc_ExtInitializerSeq = 
    "010000001500000064050000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f457874496e697469616c697a65725365713a312e3000"
    "12000000457874496e697469616c697a6572536571000000130000001405"
    "0000010000000f00000004050000010000002500000049444c3a6f6d672e"
    "6f72672f434f5242412f457874496e697469616c697a65723a312e300000"
    "00000f000000457874496e697469616c697a657200000400000008000000"
    "6d656d62657273001500000054010000010000002600000049444c3a6f6d"
    "672e6f72672f434f5242412f5374727563744d656d6265725365713a312e"
    "30000000100000005374727563744d656d62657253657100130000000801"
    "0000010000000f000000f8000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5374727563744d656d6265723a312e3000000d00"
    "00005374727563744d656d6265720000000003000000050000006e616d65"
    "000000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f4964656e7469666965723a312e30000000000b000000"
    "4964656e7469666965720000120000000000000005000000747970650000"
    "00000c00000009000000747970655f646566000000000e00000034000000"
    "010000001e00000049444c3a6f6d672e6f72672f434f5242412f49444c54"
    "7970653a312e300000000800000049444c5479706500000000000f000000"
    "657863657074696f6e735f6465660000150000009c000000010000002600"
    "000049444c3a6f6d672e6f72672f434f5242412f457863657074696f6e44"
    "65665365713a312e3000000010000000457863657074696f6e4465665365"
    "71001300000050000000010000000e0000003d0000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f457863657074696f6e446566"
    "3a312e3000000d000000457863657074696f6e4465660000000000000000"
    "0b000000657863657074696f6e730000150000002c020000010000002800"
    "000049444c3a6f6d672e6f72672f434f5242412f45786344657363726970"
    "74696f6e5365713a312e3000120000004578634465736372697074696f6e"
    "53657100000013000000dc010000010000000f000000cc01000001000000"
    "2b00000049444c3a6f6d672e6f72672f434f5242412f457863657074696f"
    "6e4465736372697074696f6e3a312e30000015000000457863657074696f"
    "6e4465736372697074696f6e0000000005000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000030000006964000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000b000000646566696e65645f696e"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000800000076657273"
    "696f6e001500000044000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f56657273696f6e537065633a312e300000000c000000"
    "56657273696f6e5370656300120000000000000005000000747970650000"
    "00000c00000000000000050000006e616d65000000001500000044000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f4964656e"
    "7469666965723a312e30000000000b0000004964656e7469666965720000"
    "120000000000000000000000";
    CORBA::Container::_tc_Description = 
    "010000000f00000084030000010000002c00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7461696e65722f4465736372697074696f6e3a"
    "312e30000c0000004465736372697074696f6e000300000011000000636f"
    "6e7461696e65645f6f626a656374000000000e0000003600000001000000"
    "2000000049444c3a6f6d672e6f72672f434f5242412f436f6e7461696e65"
    "643a312e30000a000000436f6e7461696e6564000000050000006b696e64"
    "0000000011000000bd020000010000002500000049444c3a6f6d672e6f72"
    "672f434f5242412f446566696e6974696f6e4b696e643a312e3000000000"
    "0f000000446566696e6974696f6e4b696e6400002500000008000000646b"
    "5f6e6f6e650007000000646b5f616c6c00000d000000646b5f4174747269"
    "62757465000000000c000000646b5f436f6e7374616e74000d000000646b"
    "5f457863657074696f6e000000000d000000646b5f496e74657266616365"
    "000000000a000000646b5f4d6f64756c650000000d000000646b5f4f7065"
    "726174696f6e000000000b000000646b5f54797065646566000009000000"
    "646b5f416c696173000000000a000000646b5f5374727563740000000900"
    "0000646b5f556e696f6e0000000008000000646b5f456e756d000d000000"
    "646b5f5072696d6974697665000000000a000000646b5f537472696e6700"
    "00000c000000646b5f53657175656e63650009000000646b5f4172726179"
    "000000000e000000646b5f5265706f7369746f72790000000b000000646b"
    "5f57737472696e67000009000000646b5f46697865640000000009000000"
    "646b5f56616c7565000000000c000000646b5f56616c7565426f78000f00"
    "0000646b5f56616c75654d656d62657200000a000000646b5f4e61746976"
    "6500000015000000646b5f4162737472616374496e746572666163650000"
    "000012000000646b5f4c6f63616c496e746572666163650000000d000000"
    "646b5f436f6d706f6e656e740000000008000000646b5f486f6d65000b00"
    "0000646b5f466163746f727900000a000000646b5f46696e646572000000"
    "0e000000646b5f5072696d6172794b657900000009000000646b5f456d69"
    "7473000000000d000000646b5f5075626c6973686573000000000c000000"
    "646b5f436f6e73756d6573000c000000646b5f50726f7669646573000800"
    "0000646b5f557365730009000000646b5f4576656e740000000006000000"
    "76616c75650000000b000000";
    _marshaller_CORBA_Container_Description = new _Marshaller_CORBA_Container_Description;
    CORBA::Container::_tc_DescriptionSeq = 
    "0100000015000000e8030000010000002f00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7461696e65722f4465736372697074696f6e53"
    "65713a312e3000000f0000004465736372697074696f6e53657100001300"
    "000094030000010000000f00000084030000010000002c00000049444c3a"
    "6f6d672e6f72672f434f5242412f436f6e7461696e65722f446573637269"
    "7074696f6e3a312e30000c0000004465736372697074696f6e0003000000"
    "11000000636f6e7461696e65645f6f626a656374000000000e0000003600"
    "0000010000002000000049444c3a6f6d672e6f72672f434f5242412f436f"
    "6e7461696e65643a312e30000a000000436f6e7461696e65640000000500"
    "00006b696e640000000011000000bd020000010000002500000049444c3a"
    "6f6d672e6f72672f434f5242412f446566696e6974696f6e4b696e643a31"
    "2e30000000000f000000446566696e6974696f6e4b696e64000025000000"
    "08000000646b5f6e6f6e650007000000646b5f616c6c00000d000000646b"
    "5f417474726962757465000000000c000000646b5f436f6e7374616e7400"
    "0d000000646b5f457863657074696f6e000000000d000000646b5f496e74"
    "657266616365000000000a000000646b5f4d6f64756c650000000d000000"
    "646b5f4f7065726174696f6e000000000b000000646b5f54797065646566"
    "000009000000646b5f416c696173000000000a000000646b5f5374727563"
    "7400000009000000646b5f556e696f6e0000000008000000646b5f456e75"
    "6d000d000000646b5f5072696d6974697665000000000a000000646b5f53"
    "7472696e670000000c000000646b5f53657175656e63650009000000646b"
    "5f4172726179000000000e000000646b5f5265706f7369746f7279000000"
    "0b000000646b5f57737472696e67000009000000646b5f46697865640000"
    "000009000000646b5f56616c7565000000000c000000646b5f56616c7565"
    "426f78000f000000646b5f56616c75654d656d62657200000a000000646b"
    "5f4e617469766500000015000000646b5f4162737472616374496e746572"
    "666163650000000012000000646b5f4c6f63616c496e7465726661636500"
    "00000d000000646b5f436f6d706f6e656e740000000008000000646b5f48"
    "6f6d65000b000000646b5f466163746f727900000a000000646b5f46696e"
    "6465720000000e000000646b5f5072696d6172794b657900000009000000"
    "646b5f456d697473000000000d000000646b5f5075626c69736865730000"
    "00000c000000646b5f436f6e73756d6573000c000000646b5f50726f7669"
    "6465730008000000646b5f557365730009000000646b5f4576656e740000"
    "00000600000076616c75650000000b00000000000000";
    CORBA::_tc_Container = 
    "010000000e00000036000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7461696e65723a312e30000a000000436f6e74"
    "61696e657200";
    _marshaller_CORBA_Container = new _Marshaller_CORBA_Container;
    CORBA::_tc_PrimitiveKind = 
    "01000000110000009e010000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f5072696d69746976654b696e643a312e30000e000000"
    "5072696d69746976654b696e640000001600000008000000706b5f6e756c"
    "6c0008000000706b5f766f69640009000000706b5f73686f727400000000"
    "08000000706b5f6c6f6e67000a000000706b5f7573686f72740000000900"
    "0000706b5f756c6f6e670000000009000000706b5f666c6f617400000000"
    "0a000000706b5f646f75626c650000000b000000706b5f626f6f6c65616e"
    "000008000000706b5f636861720009000000706b5f6f6374657400000000"
    "07000000706b5f616e7900000c000000706b5f54797065436f6465000d00"
    "0000706b5f5072696e636970616c000000000a000000706b5f737472696e"
    "670000000a000000706b5f6f626a7265660000000c000000706b5f6c6f6e"
    "676c6f6e67000d000000706b5f756c6f6e676c6f6e67000000000e000000"
    "706b5f6c6f6e67646f75626c6500000009000000706b5f77636861720000"
    "00000b000000706b5f77737472696e6700000e000000706b5f76616c7565"
    "5f6261736500";
    _marshaller_CORBA_PrimitiveKind = new _Marshaller_CORBA_PrimitiveKind;
    CORBA::_tc_Repository = 
    "010000000e0000003b000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f72793a312e30000000000b000000"
    "5265706f7369746f727900";
    _marshaller_CORBA_Repository = new _Marshaller_CORBA_Repository;
    CORBA::_tc_ModuleDef = 
    "010000000e00000036000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f4d6f64756c654465663a312e30000a0000004d6f6475"
    "6c6544656600";
    _marshaller_CORBA_ModuleDef = new _Marshaller_CORBA_ModuleDef;
    CORBA::_tc_ConstantDef = 
    "010000000e0000003c000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7374616e744465663a312e300000000c000000"
    "436f6e7374616e7444656600";
    _marshaller_CORBA_ConstantDef = new _Marshaller_CORBA_ConstantDef;
    CORBA::_tc_TypedefDef = 
    "010000000e0000003b000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f547970656465664465663a312e30000000000b000000"
    "5479706564656644656600";
    _marshaller_CORBA_TypedefDef = new _Marshaller_CORBA_TypedefDef;
    CORBA::_tc_StructDef = 
    "010000000e00000036000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f5374727563744465663a312e30000a00000053747275"
    "637444656600";
    _marshaller_CORBA_StructDef = new _Marshaller_CORBA_StructDef;
    CORBA::_tc_ExceptionDef = 
    "010000000e0000003d000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f457863657074696f6e4465663a312e3000000d000000"
    "457863657074696f6e44656600";
    _marshaller_CORBA_ExceptionDef = new _Marshaller_CORBA_ExceptionDef;
    CORBA::_tc_UnionDef = 
    "010000000e00000035000000010000001f00000049444c3a6f6d672e6f72"
    "672f434f5242412f556e696f6e4465663a312e30000009000000556e696f"
    "6e44656600";
    _marshaller_CORBA_UnionDef = new _Marshaller_CORBA_UnionDef;
    CORBA::_tc_EnumDef = 
    "010000000e00000034000000010000001e00000049444c3a6f6d672e6f72"
    "672f434f5242412f456e756d4465663a312e3000000008000000456e756d"
    "44656600";
    _marshaller_CORBA_EnumDef = new _Marshaller_CORBA_EnumDef;
    CORBA::_tc_AliasDef = 
    "010000000e00000035000000010000001f00000049444c3a6f6d672e6f72"
    "672f434f5242412f416c6961734465663a312e30000009000000416c6961"
    "7344656600";
    _marshaller_CORBA_AliasDef = new _Marshaller_CORBA_AliasDef;
    CORBA::_tc_OpDescriptionSeq = 
    "0100000015000000f4070000010000002700000049444c3a6f6d672e6f72"
    "672f434f5242412f4f704465736372697074696f6e5365713a312e300000"
    "110000004f704465736372697074696f6e5365710000000013000000a407"
    "0000010000000f00000094070000010000002b00000049444c3a6f6d672e"
    "6f72672f434f5242412f4f7065726174696f6e4465736372697074696f6e"
    "3a312e300000150000004f7065726174696f6e4465736372697074696f6e"
    "0000000009000000050000006e616d650000000015000000440000000100"
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
    "120000000000000007000000726573756c7400000c000000050000006d6f"
    "6465000000001100000062000000010000002400000049444c3a6f6d672e"
    "6f72672f434f5242412f4f7065726174696f6e4d6f64653a312e30000e00"
    "00004f7065726174696f6e4d6f6465000000020000000a0000004f505f4e"
    "4f524d414c0000000a0000004f505f4f4e4557415900000009000000636f"
    "6e74657874730000000015000000ec000000010000002300000049444c3a"
    "6f6d672e6f72672f434f5242412f436f6e7465787449645365713a312e30"
    "00000d000000436f6e7465787449645365710000000013000000a4000000"
    "010000001500000094000000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e746578744964656e7469666965723a312e3000"
    "12000000436f6e746578744964656e746966696572000000150000004400"
    "0000010000002100000049444c3a6f6d672e6f72672f434f5242412f4964"
    "656e7469666965723a312e30000000000b0000004964656e746966696572"
    "00001200000000000000000000000b000000706172616d65746572730000"
    "15000000f0010000010000002800000049444c3a6f6d672e6f72672f434f"
    "5242412f5061724465736372697074696f6e5365713a312e300012000000"
    "5061724465736372697074696f6e53657100000013000000a00100000100"
    "00000f00000090010000010000002b00000049444c3a6f6d672e6f72672f"
    "434f5242412f506172616d657465724465736372697074696f6e3a312e30"
    "000015000000506172616d657465724465736372697074696f6e00000000"
    "04000000050000006e616d65000000001500000044000000010000002100"
    "000049444c3a6f6d672e6f72672f434f5242412f4964656e746966696572"
    "3a312e30000000000b0000004964656e7469666965720000120000000000"
    "00000500000074797065000000000c00000009000000747970655f646566"
    "000000000e00000034000000010000001e00000049444c3a6f6d672e6f72"
    "672f434f5242412f49444c547970653a312e300000000800000049444c54"
    "79706500050000006d6f6465000000001100000074000000010000002400"
    "000049444c3a6f6d672e6f72672f434f5242412f506172616d657465724d"
    "6f64653a312e30000e000000506172616d657465724d6f64650000000300"
    "000009000000504152414d5f494e000000000a000000504152414d5f4f55"
    "540000000c000000504152414d5f494e4f555400000000000b0000006578"
    "63657074696f6e730000150000002c020000010000002800000049444c3a"
    "6f6d672e6f72672f434f5242412f4578634465736372697074696f6e5365"
    "713a312e3000120000004578634465736372697074696f6e536571000000"
    "13000000dc010000010000000f000000cc010000010000002b0000004944"
    "4c3a6f6d672e6f72672f434f5242412f457863657074696f6e4465736372"
    "697074696f6e3a312e30000015000000457863657074696f6e4465736372"
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
    "6e537065630012000000000000000500000074797065000000000c000000"
    "0000000000000000";
    CORBA::_tc_AttrDescriptionSeq = 
    "0100000015000000ac020000010000002900000049444c3a6f6d672e6f72"
    "672f434f5242412f417474724465736372697074696f6e5365713a312e30"
    "0000000013000000417474724465736372697074696f6e53657100001300"
    "000058020000010000000f00000046020000010000002b00000049444c3a"
    "6f6d672e6f72672f434f5242412f41747472696275746544657363726970"
    "74696f6e3a312e3000001500000041747472696275746544657363726970"
    "74696f6e0000000006000000050000006e616d6500000000150000004400"
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
    "7065630012000000000000000500000074797065000000000c0000000500"
    "00006d6f6465000000001100000066000000010000002400000049444c3a"
    "6f6d672e6f72672f434f5242412f4174747269627574654d6f64653a312e"
    "30000e0000004174747269627574654d6f6465000000020000000c000000"
    "415454525f4e4f524d414c000e000000415454525f524541444f4e4c5900"
    "000000000000";
    CORBA::_tc_RepositoryIdSeq = 
    "0100000015000000a4000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949645365713a312e30000000"
    "100000005265706f7369746f727949645365710013000000580000000100"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f7279496400000000120000000000000000000000";
    CORBA::_tc_AttributeMode = 
    "010000001100000066000000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f4174747269627574654d6f64653a312e30000e000000"
    "4174747269627574654d6f6465000000020000000c000000415454525f4e"
    "4f524d414c000e000000415454525f524541444f4e4c5900";
    _marshaller_CORBA_AttributeMode = new _Marshaller_CORBA_AttributeMode;
    CORBA::_tc_OperationMode = 
    "010000001100000062000000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f4f7065726174696f6e4d6f64653a312e30000e000000"
    "4f7065726174696f6e4d6f6465000000020000000a0000004f505f4e4f52"
    "4d414c0000000a0000004f505f4f4e4557415900";
    _marshaller_CORBA_OperationMode = new _Marshaller_CORBA_OperationMode;
    CORBA::_tc_ParDescriptionSeq = 
    "0100000015000000f0010000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f5061724465736372697074696f6e5365713a312e3000"
    "120000005061724465736372697074696f6e53657100000013000000a001"
    "0000010000000f00000090010000010000002b00000049444c3a6f6d672e"
    "6f72672f434f5242412f506172616d657465724465736372697074696f6e"
    "3a312e30000015000000506172616d657465724465736372697074696f6e"
    "0000000004000000050000006e616d650000000015000000440000000100"
    "00002100000049444c3a6f6d672e6f72672f434f5242412f4964656e7469"
    "666965723a312e30000000000b0000004964656e74696669657200001200"
    "0000000000000500000074797065000000000c0000000900000074797065"
    "5f646566000000000e00000034000000010000001e00000049444c3a6f6d"
    "672e6f72672f434f5242412f49444c547970653a312e3000000008000000"
    "49444c5479706500050000006d6f64650000000011000000740000000100"
    "00002400000049444c3a6f6d672e6f72672f434f5242412f506172616d65"
    "7465724d6f64653a312e30000e000000506172616d657465724d6f646500"
    "00000300000009000000504152414d5f494e000000000a00000050415241"
    "4d5f4f55540000000c000000504152414d5f494e4f55540000000000";
    CORBA::_tc_ExceptionDefSeq = 
    "01000000150000009c000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f457863657074696f6e4465665365713a312e30000000"
    "10000000457863657074696f6e4465665365710013000000500000000100"
    "00000e0000003d000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f457863657074696f6e4465663a312e3000000d0000004578"
    "63657074696f6e4465660000000000000000";
    CORBA::_tc_ContextIdentifier = 
    "010000001500000094000000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e746578744964656e7469666965723a312e3000"
    "12000000436f6e746578744964656e746966696572000000150000004400"
    "0000010000002100000049444c3a6f6d672e6f72672f434f5242412f4964"
    "656e7469666965723a312e30000000000b0000004964656e746966696572"
    "00001200000000000000";
    CORBA::_tc_ContextIdSeq = 
    "0100000015000000ec000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7465787449645365713a312e3000000d000000"
    "436f6e7465787449645365710000000013000000a4000000010000001500"
    "000094000000010000002800000049444c3a6f6d672e6f72672f434f5242"
    "412f436f6e746578744964656e7469666965723a312e300012000000436f"
    "6e746578744964656e746966696572000000150000004400000001000000"
    "2100000049444c3a6f6d672e6f72672f434f5242412f4964656e74696669"
    "65723a312e30000000000b0000004964656e746966696572000012000000"
    "0000000000000000";
    CORBA::InterfaceDef::_tc_FullInterfaceDescription = 
    "010000000f000000700d0000010000003c00000049444c3a6f6d672e6f72"
    "672f434f5242412f496e746572666163654465662f46756c6c496e746572"
    "666163654465736372697074696f6e3a312e30001900000046756c6c496e"
    "746572666163654465736372697074696f6e000000000800000005000000"
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
    "00000c00000056657273696f6e537065630012000000000000000b000000"
    "6f7065726174696f6e73000015000000f407000001000000270000004944"
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
    "000000000c00000000000000000000000b00000061747472696275746573"
    "000015000000ac020000010000002900000049444c3a6f6d672e6f72672f"
    "434f5242412f417474724465736372697074696f6e5365713a312e300000"
    "000013000000417474724465736372697074696f6e536571000013000000"
    "58020000010000000f00000046020000010000002b00000049444c3a6f6d"
    "672e6f72672f434f5242412f417474726962757465446573637269707469"
    "6f6e3a312e30000015000000417474726962757465446573637269707469"
    "6f6e0000000006000000050000006e616d65000000001500000044000000"
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
    "0000000010000000626173655f696e74657266616365730015000000a400"
    "0000010000002600000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949645365713a312e30000000100000005265706f7369"
    "746f72794964536571001300000058000000010000001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "000000001200000000000000000000000500000074797065000000000c00"
    "0000";
    _marshaller_CORBA_InterfaceDef_FullInterfaceDescription = new _Marshaller_CORBA_InterfaceDef_FullInterfaceDescription;
    CORBA::_tc_InterfaceDef = 
    "010000000e0000003d000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f496e746572666163654465663a312e3000000d000000"
    "496e7465726661636544656600";
    _marshaller_CORBA_InterfaceDef = new _Marshaller_CORBA_InterfaceDef;
    CORBA::_tc_AbstractInterfaceDef = 
    "010000000e0000004d000000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f4162737472616374496e746572666163654465663a31"
    "2e300000150000004162737472616374496e7465726661636544656600";
    _marshaller_CORBA_AbstractInterfaceDef = new _Marshaller_CORBA_AbstractInterfaceDef;
    CORBA::_tc_LocalInterfaceDef = 
    "010000000e00000046000000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f4c6f63616c496e746572666163654465663a312e3000"
    "120000004c6f63616c496e7465726661636544656600";
    _marshaller_CORBA_LocalInterfaceDef = new _Marshaller_CORBA_LocalInterfaceDef;
    CORBA::ValueDef::_tc_FullValueDescription = 
    "010000000f00000014140000010000003400000049444c3a6f6d672e6f72"
    "672f434f5242412f56616c75654465662f46756c6c56616c756544657363"
    "72697074696f6e3a312e30001500000046756c6c56616c75654465736372"
    "697074696f6e000000000f000000050000006e616d650000000015000000"
    "44000000010000002100000049444c3a6f6d672e6f72672f434f5242412f"
    "4964656e7469666965723a312e30000000000b0000004964656e74696669"
    "657200001200000000000000030000006964000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000c00000069735f61627374726163740008000000"
    "0a00000069735f637573746f6d000000080000000b000000646566696e65"
    "645f696e00001500000048000000010000002300000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949643a312e3000000d00"
    "00005265706f7369746f7279496400000000120000000000000008000000"
    "76657273696f6e001500000044000000010000002200000049444c3a6f6d"
    "672e6f72672f434f5242412f56657273696f6e537065633a312e30000000"
    "0c00000056657273696f6e537065630012000000000000000b0000006f70"
    "65726174696f6e73000015000000f4070000010000002700000049444c3a"
    "6f6d672e6f72672f434f5242412f4f704465736372697074696f6e536571"
    "3a312e300000110000004f704465736372697074696f6e53657100000000"
    "13000000a4070000010000000f00000094070000010000002b0000004944"
    "4c3a6f6d672e6f72672f434f5242412f4f7065726174696f6e4465736372"
    "697074696f6e3a312e300000150000004f7065726174696f6e4465736372"
    "697074696f6e0000000009000000050000006e616d650000000015000000"
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
    "6e5370656300120000000000000007000000726573756c7400000c000000"
    "050000006d6f646500000000110000006200000001000000240000004944"
    "4c3a6f6d672e6f72672f434f5242412f4f7065726174696f6e4d6f64653a"
    "312e30000e0000004f7065726174696f6e4d6f6465000000020000000a00"
    "00004f505f4e4f524d414c0000000a0000004f505f4f4e45574159000000"
    "09000000636f6e74657874730000000015000000ec000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f436f6e74657874496453"
    "65713a312e3000000d000000436f6e746578744964536571000000001300"
    "0000a4000000010000001500000094000000010000002800000049444c3a"
    "6f6d672e6f72672f434f5242412f436f6e746578744964656e7469666965"
    "723a312e300012000000436f6e746578744964656e746966696572000000"
    "1500000044000000010000002100000049444c3a6f6d672e6f72672f434f"
    "5242412f4964656e7469666965723a312e30000000000b0000004964656e"
    "74696669657200001200000000000000000000000b000000706172616d65"
    "74657273000015000000f0010000010000002800000049444c3a6f6d672e"
    "6f72672f434f5242412f5061724465736372697074696f6e5365713a312e"
    "3000120000005061724465736372697074696f6e53657100000013000000"
    "a0010000010000000f00000090010000010000002b00000049444c3a6f6d"
    "672e6f72672f434f5242412f506172616d65746572446573637269707469"
    "6f6e3a312e30000015000000506172616d65746572446573637269707469"
    "6f6e0000000004000000050000006e616d65000000001500000044000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f4964656e"
    "7469666965723a312e30000000000b0000004964656e7469666965720000"
    "12000000000000000500000074797065000000000c000000090000007479"
    "70655f646566000000000e00000034000000010000001e00000049444c3a"
    "6f6d672e6f72672f434f5242412f49444c547970653a312e300000000800"
    "000049444c5479706500050000006d6f6465000000001100000074000000"
    "010000002400000049444c3a6f6d672e6f72672f434f5242412f50617261"
    "6d657465724d6f64653a312e30000e000000506172616d657465724d6f64"
    "650000000300000009000000504152414d5f494e000000000a0000005041"
    "52414d5f4f55540000000c000000504152414d5f494e4f55540000000000"
    "0b000000657863657074696f6e730000150000002c020000010000002800"
    "000049444c3a6f6d672e6f72672f434f5242412f45786344657363726970"
    "74696f6e5365713a312e3000120000004578634465736372697074696f6e"
    "53657100000013000000dc010000010000000f000000cc01000001000000"
    "2b00000049444c3a6f6d672e6f72672f434f5242412f457863657074696f"
    "6e4465736372697074696f6e3a312e30000015000000457863657074696f"
    "6e4465736372697074696f6e0000000005000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000030000006964000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000b000000646566696e65645f696e"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000800000076657273"
    "696f6e001500000044000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f56657273696f6e537065633a312e300000000c000000"
    "56657273696f6e5370656300120000000000000005000000747970650000"
    "00000c00000000000000000000000b000000617474726962757465730000"
    "15000000ac020000010000002900000049444c3a6f6d672e6f72672f434f"
    "5242412f417474724465736372697074696f6e5365713a312e3000000000"
    "13000000417474724465736372697074696f6e5365710000130000005802"
    "0000010000000f00000046020000010000002b00000049444c3a6f6d672e"
    "6f72672f434f5242412f4174747269627574654465736372697074696f6e"
    "3a312e300000150000004174747269627574654465736372697074696f6e"
    "0000000006000000050000006e616d650000000015000000440000000100"
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
    "12000000000000000500000074797065000000000c000000050000006d6f"
    "6465000000001100000066000000010000002400000049444c3a6f6d672e"
    "6f72672f434f5242412f4174747269627574654d6f64653a312e30000e00"
    "00004174747269627574654d6f6465000000020000000c00000041545452"
    "5f4e4f524d414c000e000000415454525f524541444f4e4c590000000000"
    "0000080000006d656d626572730015000000b40200000100000025000000"
    "49444c3a6f6d672e6f72672f434f5242412f56616c75654d656d62657253"
    "65713a312e30000000000f00000056616c75654d656d6265725365710000"
    "1300000068020000010000000f0000005802000001000000220000004944"
    "4c3a6f6d672e6f72672f434f5242412f56616c75654d656d6265723a312e"
    "300000000c00000056616c75654d656d6265720007000000050000006e61"
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
    "7065000000000c00000009000000747970655f646566000000000e000000"
    "34000000010000001e00000049444c3a6f6d672e6f72672f434f5242412f"
    "49444c547970653a312e300000000800000049444c547970650007000000"
    "61636365737300001500000040000000010000002100000049444c3a6f6d"
    "672e6f72672f434f5242412f5669736962696c6974793a312e3000000000"
    "0b0000005669736962696c697479000002000000000000000d000000696e"
    "697469616c697a65727300000000150000005c0200000100000025000000"
    "49444c3a6f6d672e6f72672f434f5242412f496e697469616c697a657253"
    "65713a312e30000000000f000000496e697469616c697a65725365710000"
    "1300000010020000010000000f0000000002000001000000220000004944"
    "4c3a6f6d672e6f72672f434f5242412f496e697469616c697a65723a312e"
    "300000000c000000496e697469616c697a65720002000000080000006d65"
    "6d62657273001500000054010000010000002600000049444c3a6f6d672e"
    "6f72672f434f5242412f5374727563744d656d6265725365713a312e3000"
    "0000100000005374727563744d656d626572536571001300000008010000"
    "010000000f000000f8000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5374727563744d656d6265723a312e3000000d000000"
    "5374727563744d656d6265720000000003000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000050000007479706500000000"
    "0c00000009000000747970655f646566000000000e000000340000000100"
    "00001e00000049444c3a6f6d672e6f72672f434f5242412f49444c547970"
    "653a312e300000000800000049444c547970650000000000050000006e61"
    "6d65000000001500000044000000010000002100000049444c3a6f6d672e"
    "6f72672f434f5242412f4964656e7469666965723a312e30000000000b00"
    "00004964656e746966696572000012000000000000000000000015000000"
    "737570706f727465645f696e74657266616365730000000015000000a400"
    "0000010000002600000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949645365713a312e30000000100000005265706f7369"
    "746f72794964536571001300000058000000010000001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "000000001200000000000000000000001500000061627374726163745f62"
    "6173655f76616c7565730000000015000000a40000000100000026000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "5365713a312e30000000100000005265706f7369746f7279496453657100"
    "130000005800000001000000150000004800000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f5265706f7369746f727949643a31"
    "2e3000000d0000005265706f7369746f7279496400000000120000000000"
    "0000000000000f00000069735f7472756e63617461626c65000008000000"
    "0b000000626173655f76616c756500001500000048000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49643a312e3000000d0000005265706f7369746f72794964000000001200"
    "0000000000000500000074797065000000000c000000";
    _marshaller_CORBA_ValueDef_FullValueDescription = new _Marshaller_CORBA_ValueDef_FullValueDescription;
    CORBA::_tc_ValueDef = 
    "010000000e00000035000000010000001f00000049444c3a6f6d672e6f72"
    "672f434f5242412f56616c75654465663a312e3000000900000056616c75"
    "6544656600";
    _marshaller_CORBA_ValueDef = new _Marshaller_CORBA_ValueDef;
    CORBA::_tc_ValueBoxDef = 
    "010000000e0000003c000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f56616c7565426f784465663a312e300000000c000000"
    "56616c7565426f7844656600";
    _marshaller_CORBA_ValueBoxDef = new _Marshaller_CORBA_ValueBoxDef;
    CORBA::_tc_NativeDef = 
    "010000000e00000036000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f4e61746976654465663a312e30000a0000004e617469"
    "766544656600";
    _marshaller_CORBA_NativeDef = new _Marshaller_CORBA_NativeDef;
    CORBA::_tc_ExtAttrDescriptionSeq = 
    "010000001500000044070000010000002c00000049444c3a6f6d672e6f72"
    "672f434f5242412f457874417474724465736372697074696f6e5365713a"
    "312e300016000000457874417474724465736372697074696f6e53657100"
    "000013000000ec060000010000000f000000dc060000010000002e000000"
    "49444c3a6f6d672e6f72672f434f5242412f457874417474726962757465"
    "4465736372697074696f6e3a312e30000000180000004578744174747269"
    "627574654465736372697074696f6e0008000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000030000006964000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000b000000646566696e65645f696e"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000800000076657273"
    "696f6e001500000044000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f56657273696f6e537065633a312e300000000c000000"
    "56657273696f6e5370656300120000000000000005000000747970650000"
    "00000c000000050000006d6f646500000000110000006600000001000000"
    "2400000049444c3a6f6d672e6f72672f434f5242412f4174747269627574"
    "654d6f64653a312e30000e0000004174747269627574654d6f6465000000"
    "020000000c000000415454525f4e4f524d414c000e000000415454525f52"
    "4541444f4e4c590000000f0000006765745f657863657074696f6e730000"
    "150000002c020000010000002800000049444c3a6f6d672e6f72672f434f"
    "5242412f4578634465736372697074696f6e5365713a312e300012000000"
    "4578634465736372697074696f6e53657100000013000000dc0100000100"
    "00000f000000cc010000010000002b00000049444c3a6f6d672e6f72672f"
    "434f5242412f457863657074696f6e4465736372697074696f6e3a312e30"
    "000015000000457863657074696f6e4465736372697074696f6e00000000"
    "05000000050000006e616d65000000001500000044000000010000002100"
    "000049444c3a6f6d672e6f72672f434f5242412f4964656e746966696572"
    "3a312e30000000000b0000004964656e7469666965720000120000000000"
    "000003000000696400001500000048000000010000002300000049444c3a"
    "6f6d672e6f72672f434f5242412f5265706f7369746f727949643a312e30"
    "00000d0000005265706f7369746f72794964000000001200000000000000"
    "0b000000646566696e65645f696e00001500000048000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49643a312e3000000d0000005265706f7369746f72794964000000001200"
    "0000000000000800000076657273696f6e00150000004400000001000000"
    "2200000049444c3a6f6d672e6f72672f434f5242412f56657273696f6e53"
    "7065633a312e300000000c00000056657273696f6e537065630012000000"
    "000000000500000074797065000000000c000000000000000f0000007365"
    "745f657863657074696f6e730000150000002c0200000100000028000000"
    "49444c3a6f6d672e6f72672f434f5242412f457863446573637269707469"
    "6f6e5365713a312e3000120000004578634465736372697074696f6e5365"
    "7100000013000000dc010000010000000f000000cc010000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f457863657074696f6e44"
    "65736372697074696f6e3a312e30000015000000457863657074696f6e44"
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
    "7273696f6e53706563001200000000000000050000007479706500000000"
    "0c0000000000000000000000";
    CORBA::ExtValueDef::_tc_ExtFullValueDescription = 
    "010000000f000000bc1b0000010000003a00000049444c3a6f6d672e6f72"
    "672f434f5242412f45787456616c75654465662f45787446756c6c56616c"
    "75654465736372697074696f6e3a312e300000001800000045787446756c"
    "6c56616c75654465736372697074696f6e000f000000050000006e616d65"
    "000000001500000044000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f4964656e7469666965723a312e30000000000b000000"
    "4964656e7469666965720000120000000000000003000000696400001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f727949640000000012000000000000000c00000069735f6162737472"
    "61637400080000000a00000069735f637573746f6d000000080000000b00"
    "0000646566696e65645f696e000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "000000000800000076657273696f6e001500000044000000010000002200"
    "000049444c3a6f6d672e6f72672f434f5242412f56657273696f6e537065"
    "633a312e300000000c00000056657273696f6e5370656300120000000000"
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
    "000000000c0000000000000000000000080000006d656d62657273001500"
    "0000b4020000010000002500000049444c3a6f6d672e6f72672f434f5242"
    "412f56616c75654d656d6265725365713a312e30000000000f0000005661"
    "6c75654d656d62657253657100001300000068020000010000000f000000"
    "58020000010000002200000049444c3a6f6d672e6f72672f434f5242412f"
    "56616c75654d656d6265723a312e300000000c00000056616c75654d656d"
    "6265720007000000050000006e616d650000000015000000440000000100"
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
    "12000000000000000500000074797065000000000c000000090000007479"
    "70655f646566000000000e00000034000000010000001e00000049444c3a"
    "6f6d672e6f72672f434f5242412f49444c547970653a312e300000000800"
    "000049444c54797065000700000061636365737300001500000040000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f56697369"
    "62696c6974793a312e30000000000b0000005669736962696c6974790000"
    "02000000000000000d000000696e697469616c697a657273000000001500"
    "000064050000010000002800000049444c3a6f6d672e6f72672f434f5242"
    "412f457874496e697469616c697a65725365713a312e3000120000004578"
    "74496e697469616c697a6572536571000000130000001405000001000000"
    "0f00000004050000010000002500000049444c3a6f6d672e6f72672f434f"
    "5242412f457874496e697469616c697a65723a312e30000000000f000000"
    "457874496e697469616c697a6572000004000000080000006d656d626572"
    "73001500000054010000010000002600000049444c3a6f6d672e6f72672f"
    "434f5242412f5374727563744d656d6265725365713a312e300000001000"
    "00005374727563744d656d62657253657100130000000801000001000000"
    "0f000000f8000000010000002300000049444c3a6f6d672e6f72672f434f"
    "5242412f5374727563744d656d6265723a312e3000000d00000053747275"
    "63744d656d6265720000000003000000050000006e616d65000000001500"
    "000044000000010000002100000049444c3a6f6d672e6f72672f434f5242"
    "412f4964656e7469666965723a312e30000000000b0000004964656e7469"
    "66696572000012000000000000000500000074797065000000000c000000"
    "09000000747970655f646566000000000e00000034000000010000001e00"
    "000049444c3a6f6d672e6f72672f434f5242412f49444c547970653a312e"
    "300000000800000049444c5479706500000000000f000000657863657074"
    "696f6e735f6465660000150000009c000000010000002600000049444c3a"
    "6f6d672e6f72672f434f5242412f457863657074696f6e4465665365713a"
    "312e3000000010000000457863657074696f6e4465665365710013000000"
    "50000000010000000e0000003d000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f457863657074696f6e4465663a312e300000"
    "0d000000457863657074696f6e44656600000000000000000b0000006578"
    "63657074696f6e730000150000002c020000010000002800000049444c3a"
    "6f6d672e6f72672f434f5242412f4578634465736372697074696f6e5365"
    "713a312e3000120000004578634465736372697074696f6e536571000000"
    "13000000dc010000010000000f000000cc010000010000002b0000004944"
    "4c3a6f6d672e6f72672f434f5242412f457863657074696f6e4465736372"
    "697074696f6e3a312e30000015000000457863657074696f6e4465736372"
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
    "6e537065630012000000000000000500000074797065000000000c000000"
    "00000000050000006e616d65000000001500000044000000010000002100"
    "000049444c3a6f6d672e6f72672f434f5242412f4964656e746966696572"
    "3a312e30000000000b0000004964656e7469666965720000120000000000"
    "00000000000015000000737570706f727465645f696e7465726661636573"
    "0000000015000000a4000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949645365713a312e30000000"
    "100000005265706f7369746f727949645365710013000000580000000100"
    "00001500000048000000010000002300000049444c3a6f6d672e6f72672f"
    "434f5242412f5265706f7369746f727949643a312e3000000d0000005265"
    "706f7369746f727949640000000012000000000000000000000015000000"
    "61627374726163745f626173655f76616c7565730000000015000000a400"
    "0000010000002600000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949645365713a312e30000000100000005265706f7369"
    "746f72794964536571001300000058000000010000001500000048000000"
    "010000002300000049444c3a6f6d672e6f72672f434f5242412f5265706f"
    "7369746f727949643a312e3000000d0000005265706f7369746f72794964"
    "000000001200000000000000000000000f00000069735f7472756e636174"
    "61626c650000080000000b000000626173655f76616c7565000015000000"
    "48000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949643a312e3000000d0000005265706f7369746f"
    "727949640000000012000000000000000500000074797065000000000c00"
    "0000";
    _marshaller_CORBA_ExtValueDef_ExtFullValueDescription = new _Marshaller_CORBA_ExtValueDef_ExtFullValueDescription;
    CORBA::_tc_ExtValueDef = 
    "010000000e0000003c000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f45787456616c75654465663a312e300000000c000000"
    "45787456616c756544656600";
    _marshaller_CORBA_ExtValueDef = new _Marshaller_CORBA_ExtValueDef;
    CORBA::_tc_LocalInterfaceDefSeq = 
    "0100000015000000b0000000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f4c6f63616c496e746572666163654465665365713a31"
    "2e300000150000004c6f63616c496e746572666163654465665365710000"
    "00001300000058000000010000000e000000460000000100000028000000"
    "49444c3a6f6d672e6f72672f434f5242412f4c6f63616c496e7465726661"
    "63654465663a312e3000120000004c6f63616c496e746572666163654465"
    "6600000000000000";
    CORBA::_tc_ExceptionDescription = 
    "010000000f000000cc010000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f457863657074696f6e4465736372697074696f6e3a31"
    "2e30000015000000457863657074696f6e4465736372697074696f6e0000"
    "000005000000050000006e616d6500000000150000004400000001000000"
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
    "0000000000000500000074797065000000000c000000";
    _marshaller_CORBA_ExceptionDescription = new _Marshaller_CORBA_ExceptionDescription;
    CORBA::_tc_ExcDescriptionSeq = 
    "01000000150000002c020000010000002800000049444c3a6f6d672e6f72"
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
    "12000000000000000500000074797065000000000c00000000000000";
    CORBA::_tc_Initializer = 
    "010000000f00000000020000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f496e697469616c697a65723a312e300000000c000000"
    "496e697469616c697a65720002000000080000006d656d62657273001500"
    "000054010000010000002600000049444c3a6f6d672e6f72672f434f5242"
    "412f5374727563744d656d6265725365713a312e30000000100000005374"
    "727563744d656d626572536571001300000008010000010000000f000000"
    "f8000000010000002300000049444c3a6f6d672e6f72672f434f5242412f"
    "5374727563744d656d6265723a312e3000000d0000005374727563744d65"
    "6d6265720000000003000000050000006e616d6500000000150000004400"
    "0000010000002100000049444c3a6f6d672e6f72672f434f5242412f4964"
    "656e7469666965723a312e30000000000b0000004964656e746966696572"
    "000012000000000000000500000074797065000000000c00000009000000"
    "747970655f646566000000000e00000034000000010000001e0000004944"
    "4c3a6f6d672e6f72672f434f5242412f49444c547970653a312e30000000"
    "0800000049444c547970650000000000050000006e616d65000000001500"
    "000044000000010000002100000049444c3a6f6d672e6f72672f434f5242"
    "412f4964656e7469666965723a312e30000000000b0000004964656e7469"
    "6669657200001200000000000000";
    _marshaller_CORBA_Initializer = new _Marshaller_CORBA_Initializer;
    CORBA::_tc_ExtInitializer = 
    "010000000f00000004050000010000002500000049444c3a6f6d672e6f72"
    "672f434f5242412f457874496e697469616c697a65723a312e3000000000"
    "0f000000457874496e697469616c697a6572000004000000080000006d65"
    "6d62657273001500000054010000010000002600000049444c3a6f6d672e"
    "6f72672f434f5242412f5374727563744d656d6265725365713a312e3000"
    "0000100000005374727563744d656d626572536571001300000008010000"
    "010000000f000000f8000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5374727563744d656d6265723a312e3000000d000000"
    "5374727563744d656d6265720000000003000000050000006e616d650000"
    "00001500000044000000010000002100000049444c3a6f6d672e6f72672f"
    "434f5242412f4964656e7469666965723a312e30000000000b0000004964"
    "656e74696669657200001200000000000000050000007479706500000000"
    "0c00000009000000747970655f646566000000000e000000340000000100"
    "00001e00000049444c3a6f6d672e6f72672f434f5242412f49444c547970"
    "653a312e300000000800000049444c5479706500000000000f0000006578"
    "63657074696f6e735f6465660000150000009c0000000100000026000000"
    "49444c3a6f6d672e6f72672f434f5242412f457863657074696f6e446566"
    "5365713a312e3000000010000000457863657074696f6e44656653657100"
    "1300000050000000010000000e0000003d00000001000000230000004944"
    "4c3a6f6d672e6f72672f434f5242412f457863657074696f6e4465663a31"
    "2e3000000d000000457863657074696f6e44656600000000000000000b00"
    "0000657863657074696f6e730000150000002c0200000100000028000000"
    "49444c3a6f6d672e6f72672f434f5242412f457863446573637269707469"
    "6f6e5365713a312e3000120000004578634465736372697074696f6e5365"
    "7100000013000000dc010000010000000f000000cc010000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f457863657074696f6e44"
    "65736372697074696f6e3a312e30000015000000457863657074696f6e44"
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
    "7273696f6e53706563001200000000000000050000007479706500000000"
    "0c00000000000000050000006e616d650000000015000000440000000100"
    "00002100000049444c3a6f6d672e6f72672f434f5242412f4964656e7469"
    "666965723a312e30000000000b0000004964656e74696669657200001200"
    "000000000000";
    _marshaller_CORBA_ExtInitializer = new _Marshaller_CORBA_ExtInitializer;
    CORBA::_tc_PrimitiveDef = 
    "010000000e0000003d000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5072696d69746976654465663a312e3000000d000000"
    "5072696d697469766544656600";
    _marshaller_CORBA_PrimitiveDef = new _Marshaller_CORBA_PrimitiveDef;
    CORBA::_tc_StringDef = 
    "010000000e00000036000000010000002000000049444c3a6f6d672e6f72"
    "672f434f5242412f537472696e674465663a312e30000a00000053747269"
    "6e6744656600";
    _marshaller_CORBA_StringDef = new _Marshaller_CORBA_StringDef;
    CORBA::_tc_WstringDef = 
    "010000000e0000003b000000010000002100000049444c3a6f6d672e6f72"
    "672f434f5242412f57737472696e674465663a312e30000000000b000000"
    "57737472696e6744656600";
    _marshaller_CORBA_WstringDef = new _Marshaller_CORBA_WstringDef;
    CORBA::_tc_SequenceDef = 
    "010000000e0000003c000000010000002200000049444c3a6f6d672e6f72"
    "672f434f5242412f53657175656e63654465663a312e300000000c000000"
    "53657175656e636544656600";
    _marshaller_CORBA_SequenceDef = new _Marshaller_CORBA_SequenceDef;
    CORBA::_tc_ArrayDef = 
    "010000000e00000035000000010000001f00000049444c3a6f6d672e6f72"
    "672f434f5242412f41727261794465663a312e3000000900000041727261"
    "7944656600";
    _marshaller_CORBA_ArrayDef = new _Marshaller_CORBA_ArrayDef;
    CORBA::_tc_FixedDef = 
    "010000000e00000035000000010000001f00000049444c3a6f6d672e6f72"
    "672f434f5242412f46697865644465663a312e3000000900000046697865"
    "6444656600";
    _marshaller_CORBA_FixedDef = new _Marshaller_CORBA_FixedDef;
    CORBA::_tc_ModuleDescription = 
    "010000000f000000b4010000010000002800000049444c3a6f6d672e6f72"
    "672f434f5242412f4d6f64756c654465736372697074696f6e3a312e3000"
    "120000004d6f64756c654465736372697074696f6e000000040000000500"
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
    "300000000c00000056657273696f6e53706563001200000000000000";
    _marshaller_CORBA_ModuleDescription = new _Marshaller_CORBA_ModuleDescription;
    CORBA::_tc_ConstantDescription = 
    "010000000f000000d8010000010000002a00000049444c3a6f6d672e6f72"
    "672f434f5242412f436f6e7374616e744465736372697074696f6e3a312e"
    "3000000014000000436f6e7374616e744465736372697074696f6e000600"
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
    "00000500000074797065000000000c0000000600000076616c7565000000"
    "0b000000";
    _marshaller_CORBA_ConstantDescription = new _Marshaller_CORBA_ConstantDescription;
    CORBA::_tc_TypeDescription = 
    "010000000f000000c0010000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f547970654465736372697074696f6e3a312e30000000"
    "10000000547970654465736372697074696f6e0005000000050000006e61"
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
    "7065000000000c000000";
    _marshaller_CORBA_TypeDescription = new _Marshaller_CORBA_TypeDescription;
    CORBA::_tc_AttributeDef = 
    "010000000e0000003d000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f4174747269627574654465663a312e3000000d000000"
    "41747472696275746544656600";
    _marshaller_CORBA_AttributeDef = new _Marshaller_CORBA_AttributeDef;
    CORBA::_tc_AttributeDescription = 
    "010000000f00000046020000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f4174747269627574654465736372697074696f6e3a31"
    "2e300000150000004174747269627574654465736372697074696f6e0000"
    "000006000000050000006e616d6500000000150000004400000001000000"
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
    "0000000000000500000074797065000000000c000000050000006d6f6465"
    "000000001100000066000000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f4174747269627574654d6f64653a312e30000e000000"
    "4174747269627574654d6f6465000000020000000c000000415454525f4e"
    "4f524d414c000e000000415454525f524541444f4e4c5900";
    _marshaller_CORBA_AttributeDescription = new _Marshaller_CORBA_AttributeDescription;
    CORBA::_tc_ExtAttributeDescription = 
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
    "12000000000000000500000074797065000000000c00000000000000";
    _marshaller_CORBA_ExtAttributeDescription = new _Marshaller_CORBA_ExtAttributeDescription;
    CORBA::_tc_ExtAttributeDef = 
    "010000000e00000044000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f4578744174747269627574654465663a312e30000000"
    "1000000045787441747472696275746544656600";
    _marshaller_CORBA_ExtAttributeDef = new _Marshaller_CORBA_ExtAttributeDef;
    CORBA::_tc_ParameterMode = 
    "010000001100000074000000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f506172616d657465724d6f64653a312e30000e000000"
    "506172616d657465724d6f64650000000300000009000000504152414d5f"
    "494e000000000a000000504152414d5f4f55540000000c00000050415241"
    "4d5f494e4f555400";
    _marshaller_CORBA_ParameterMode = new _Marshaller_CORBA_ParameterMode;
    CORBA::_tc_ParameterDescription = 
    "010000000f00000090010000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f506172616d657465724465736372697074696f6e3a31"
    "2e30000015000000506172616d657465724465736372697074696f6e0000"
    "000004000000050000006e616d6500000000150000004400000001000000"
    "2100000049444c3a6f6d672e6f72672f434f5242412f4964656e74696669"
    "65723a312e30000000000b0000004964656e746966696572000012000000"
    "000000000500000074797065000000000c00000009000000747970655f64"
    "6566000000000e00000034000000010000001e00000049444c3a6f6d672e"
    "6f72672f434f5242412f49444c547970653a312e30000000080000004944"
    "4c5479706500050000006d6f646500000000110000007400000001000000"
    "2400000049444c3a6f6d672e6f72672f434f5242412f506172616d657465"
    "724d6f64653a312e30000e000000506172616d657465724d6f6465000000"
    "0300000009000000504152414d5f494e000000000a000000504152414d5f"
    "4f55540000000c000000504152414d5f494e4f555400";
    _marshaller_CORBA_ParameterDescription = new _Marshaller_CORBA_ParameterDescription;
    CORBA::_tc_OperationDef = 
    "010000000e0000003d000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f4f7065726174696f6e4465663a312e3000000d000000"
    "4f7065726174696f6e44656600";
    _marshaller_CORBA_OperationDef = new _Marshaller_CORBA_OperationDef;
    CORBA::_tc_OperationDescription = 
    "010000000f00000094070000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f4f7065726174696f6e4465736372697074696f6e3a31"
    "2e300000150000004f7065726174696f6e4465736372697074696f6e0000"
    "000009000000050000006e616d6500000000150000004400000001000000"
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
    "00000000000007000000726573756c7400000c000000050000006d6f6465"
    "000000001100000062000000010000002400000049444c3a6f6d672e6f72"
    "672f434f5242412f4f7065726174696f6e4d6f64653a312e30000e000000"
    "4f7065726174696f6e4d6f6465000000020000000a0000004f505f4e4f52"
    "4d414c0000000a0000004f505f4f4e4557415900000009000000636f6e74"
    "657874730000000015000000ec000000010000002300000049444c3a6f6d"
    "672e6f72672f434f5242412f436f6e7465787449645365713a312e300000"
    "0d000000436f6e7465787449645365710000000013000000a40000000100"
    "00001500000094000000010000002800000049444c3a6f6d672e6f72672f"
    "434f5242412f436f6e746578744964656e7469666965723a312e30001200"
    "0000436f6e746578744964656e7469666965720000001500000044000000"
    "010000002100000049444c3a6f6d672e6f72672f434f5242412f4964656e"
    "7469666965723a312e30000000000b0000004964656e7469666965720000"
    "1200000000000000000000000b000000706172616d657465727300001500"
    "0000f0010000010000002800000049444c3a6f6d672e6f72672f434f5242"
    "412f5061724465736372697074696f6e5365713a312e3000120000005061"
    "724465736372697074696f6e53657100000013000000a001000001000000"
    "0f00000090010000010000002b00000049444c3a6f6d672e6f72672f434f"
    "5242412f506172616d657465724465736372697074696f6e3a312e300000"
    "15000000506172616d657465724465736372697074696f6e000000000400"
    "0000050000006e616d650000000015000000440000000100000021000000"
    "49444c3a6f6d672e6f72672f434f5242412f4964656e7469666965723a31"
    "2e30000000000b0000004964656e74696669657200001200000000000000"
    "0500000074797065000000000c00000009000000747970655f6465660000"
    "00000e00000034000000010000001e00000049444c3a6f6d672e6f72672f"
    "434f5242412f49444c547970653a312e300000000800000049444c547970"
    "6500050000006d6f64650000000011000000740000000100000024000000"
    "49444c3a6f6d672e6f72672f434f5242412f506172616d657465724d6f64"
    "653a312e30000e000000506172616d657465724d6f646500000003000000"
    "09000000504152414d5f494e000000000a000000504152414d5f4f555400"
    "00000c000000504152414d5f494e4f555400000000000b00000065786365"
    "7074696f6e730000150000002c020000010000002800000049444c3a6f6d"
    "672e6f72672f434f5242412f4578634465736372697074696f6e5365713a"
    "312e3000120000004578634465736372697074696f6e5365710000001300"
    "0000dc010000010000000f000000cc010000010000002b00000049444c3a"
    "6f6d672e6f72672f434f5242412f457863657074696f6e44657363726970"
    "74696f6e3a312e30000015000000457863657074696f6e44657363726970"
    "74696f6e0000000005000000050000006e616d6500000000150000004400"
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
    "7065630012000000000000000500000074797065000000000c0000000000"
    "0000";
    _marshaller_CORBA_OperationDescription = new _Marshaller_CORBA_OperationDescription;
    CORBA::_tc_InterfaceDescription = 
    "010000000f0000007c020000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f496e746572666163654465736372697074696f6e3a31"
    "2e30000015000000496e746572666163654465736372697074696f6e0000"
    "000005000000050000006e616d6500000000150000004400000001000000"
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
    "00000000000010000000626173655f696e74657266616365730015000000"
    "a4000000010000002600000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949645365713a312e30000000100000005265706f"
    "7369746f7279496453657100130000005800000001000000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "496400000000120000000000000000000000";
    _marshaller_CORBA_InterfaceDescription = new _Marshaller_CORBA_InterfaceDescription;
    CORBA::InterfaceAttrExtension::_tc_ExtFullInterfaceDescription = 
    "010000000f00000018120000010000004900000049444c3a6f6d672e6f72"
    "672f434f5242412f496e7465726661636541747472457874656e73696f6e"
    "2f45787446756c6c496e746572666163654465736372697074696f6e3a31"
    "2e30000000001c00000045787446756c6c496e7465726661636544657363"
    "72697074696f6e0008000000050000006e616d6500000000150000004400"
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
    "7065630012000000000000000b0000006f7065726174696f6e7300001500"
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
    "00000b000000617474726962757465730000150000004407000001000000"
    "2c00000049444c3a6f6d672e6f72672f434f5242412f4578744174747244"
    "65736372697074696f6e5365713a312e3000160000004578744174747244"
    "65736372697074696f6e53657100000013000000ec060000010000000f00"
    "0000dc060000010000002e00000049444c3a6f6d672e6f72672f434f5242"
    "412f4578744174747269627574654465736372697074696f6e3a312e3000"
    "0000180000004578744174747269627574654465736372697074696f6e00"
    "08000000050000006e616d65000000001500000044000000010000002100"
    "000049444c3a6f6d672e6f72672f434f5242412f4964656e746966696572"
    "3a312e30000000000b0000004964656e7469666965720000120000000000"
    "000003000000696400001500000048000000010000002300000049444c3a"
    "6f6d672e6f72672f434f5242412f5265706f7369746f727949643a312e30"
    "00000d0000005265706f7369746f72794964000000001200000000000000"
    "0b000000646566696e65645f696e00001500000048000000010000002300"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49643a312e3000000d0000005265706f7369746f72794964000000001200"
    "0000000000000800000076657273696f6e00150000004400000001000000"
    "2200000049444c3a6f6d672e6f72672f434f5242412f56657273696f6e53"
    "7065633a312e300000000c00000056657273696f6e537065630012000000"
    "000000000500000074797065000000000c000000050000006d6f64650000"
    "00001100000066000000010000002400000049444c3a6f6d672e6f72672f"
    "434f5242412f4174747269627574654d6f64653a312e30000e0000004174"
    "747269627574654d6f6465000000020000000c000000415454525f4e4f52"
    "4d414c000e000000415454525f524541444f4e4c590000000f0000006765"
    "745f657863657074696f6e730000150000002c0200000100000028000000"
    "49444c3a6f6d672e6f72672f434f5242412f457863446573637269707469"
    "6f6e5365713a312e3000120000004578634465736372697074696f6e5365"
    "7100000013000000dc010000010000000f000000cc010000010000002b00"
    "000049444c3a6f6d672e6f72672f434f5242412f457863657074696f6e44"
    "65736372697074696f6e3a312e30000015000000457863657074696f6e44"
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
    "7273696f6e53706563001200000000000000050000007479706500000000"
    "0c000000000000000f0000007365745f657863657074696f6e7300001500"
    "00002c020000010000002800000049444c3a6f6d672e6f72672f434f5242"
    "412f4578634465736372697074696f6e5365713a312e3000120000004578"
    "634465736372697074696f6e53657100000013000000dc01000001000000"
    "0f000000cc010000010000002b00000049444c3a6f6d672e6f72672f434f"
    "5242412f457863657074696f6e4465736372697074696f6e3a312e300000"
    "15000000457863657074696f6e4465736372697074696f6e000000000500"
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
    "00000500000074797065000000000c000000000000000000000010000000"
    "626173655f696e74657266616365730015000000a4000000010000002600"
    "000049444c3a6f6d672e6f72672f434f5242412f5265706f7369746f7279"
    "49645365713a312e30000000100000005265706f7369746f727949645365"
    "710013000000580000000100000015000000480000000100000023000000"
    "49444c3a6f6d672e6f72672f434f5242412f5265706f7369746f72794964"
    "3a312e3000000d0000005265706f7369746f727949640000000012000000"
    "00000000000000000500000074797065000000000c000000";
    _marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription = new _Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription;
    CORBA::_tc_InterfaceAttrExtension = 
    "010000000e00000053000000010000002d00000049444c3a6f6d672e6f72"
    "672f434f5242412f496e7465726661636541747472457874656e73696f6e"
    "3a312e300000000017000000496e7465726661636541747472457874656e"
    "73696f6e00";
    _marshaller_CORBA_InterfaceAttrExtension = new _Marshaller_CORBA_InterfaceAttrExtension;
    CORBA::_tc_ExtInterfaceDef = 
    "010000000e00000044000000010000002600000049444c3a6f6d672e6f72"
    "672f434f5242412f457874496e746572666163654465663a312e30000000"
    "10000000457874496e7465726661636544656600";
    _marshaller_CORBA_ExtInterfaceDef = new _Marshaller_CORBA_ExtInterfaceDef;
    CORBA::_tc_ExtAbstractInterfaceDef = 
    "010000000e00000054000000010000002e00000049444c3a6f6d672e6f72"
    "672f434f5242412f4578744162737472616374496e746572666163654465"
    "663a312e30000000180000004578744162737472616374496e7465726661"
    "636544656600";
    _marshaller_CORBA_ExtAbstractInterfaceDef = new _Marshaller_CORBA_ExtAbstractInterfaceDef;
    CORBA::_tc_ExtLocalInterfaceDef = 
    "010000000e0000004d000000010000002b00000049444c3a6f6d672e6f72"
    "672f434f5242412f4578744c6f63616c496e746572666163654465663a31"
    "2e300000150000004578744c6f63616c496e7465726661636544656600";
    _marshaller_CORBA_ExtLocalInterfaceDef = new _Marshaller_CORBA_ExtLocalInterfaceDef;
    CORBA::_tc_ValueMemberDef = 
    "010000000e00000043000000010000002500000049444c3a6f6d672e6f72"
    "672f434f5242412f56616c75654d656d6265724465663a312e3000000000"
    "0f00000056616c75654d656d62657244656600";
    _marshaller_CORBA_ValueMemberDef = new _Marshaller_CORBA_ValueMemberDef;
    CORBA::_tc_ValueDescription = 
    "010000000f000000e4030000010000002700000049444c3a6f6d672e6f72"
    "672f434f5242412f56616c75654465736372697074696f6e3a312e300000"
    "1100000056616c75654465736372697074696f6e000000000a0000000500"
    "00006e616d65000000001500000044000000010000002100000049444c3a"
    "6f6d672e6f72672f434f5242412f4964656e7469666965723a312e300000"
    "00000b0000004964656e7469666965720000120000000000000003000000"
    "696400001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f727949640000000012000000000000000c0000006973"
    "5f616273747261637400080000000a00000069735f637573746f6d000000"
    "080000000b000000646566696e65645f696e000015000000480000000100"
    "00002300000049444c3a6f6d672e6f72672f434f5242412f5265706f7369"
    "746f727949643a312e3000000d0000005265706f7369746f727949640000"
    "000012000000000000000800000076657273696f6e001500000044000000"
    "010000002200000049444c3a6f6d672e6f72672f434f5242412f56657273"
    "696f6e537065633a312e300000000c00000056657273696f6e5370656300"
    "120000000000000015000000737570706f727465645f696e746572666163"
    "65730000000015000000a4000000010000002600000049444c3a6f6d672e"
    "6f72672f434f5242412f5265706f7369746f727949645365713a312e3000"
    "0000100000005265706f7369746f72794964536571001300000058000000"
    "010000001500000048000000010000002300000049444c3a6f6d672e6f72"
    "672f434f5242412f5265706f7369746f727949643a312e3000000d000000"
    "5265706f7369746f72794964000000001200000000000000000000001500"
    "000061627374726163745f626173655f76616c7565730000000015000000"
    "a4000000010000002600000049444c3a6f6d672e6f72672f434f5242412f"
    "5265706f7369746f727949645365713a312e30000000100000005265706f"
    "7369746f7279496453657100130000005800000001000000150000004800"
    "0000010000002300000049444c3a6f6d672e6f72672f434f5242412f5265"
    "706f7369746f727949643a312e3000000d0000005265706f7369746f7279"
    "4964000000001200000000000000000000000f00000069735f7472756e63"
    "617461626c650000080000000b000000626173655f76616c756500001500"
    "000048000000010000002300000049444c3a6f6d672e6f72672f434f5242"
    "412f5265706f7369746f727949643a312e3000000d0000005265706f7369"
    "746f72794964000000001200000000000000";
    _marshaller_CORBA_ValueDescription = new _Marshaller_CORBA_ValueDescription;
    _marshaller__seq_CORBA_Contained = new _Marshaller__seq_CORBA_Contained;
    _marshaller__seq_CORBA_Container_Description = new _Marshaller__seq_CORBA_Container_Description;
    _marshaller__seq_CORBA_InterfaceDef = new _Marshaller__seq_CORBA_InterfaceDef;
    _marshaller__seq_CORBA_AbstractInterfaceDef = new _Marshaller__seq_CORBA_AbstractInterfaceDef;
    _marshaller__seq_CORBA_ValueDef = new _Marshaller__seq_CORBA_ValueDef;
    _marshaller__seq_CORBA_Initializer = new _Marshaller__seq_CORBA_Initializer;
    _marshaller__seq_CORBA_ExtInitializer = new _Marshaller__seq_CORBA_ExtInitializer;
    _marshaller__seq_CORBA_OperationDescription = new _Marshaller__seq_CORBA_OperationDescription;
    _marshaller__seq_CORBA_AttributeDescription = new _Marshaller__seq_CORBA_AttributeDescription;
    _marshaller__seq_CORBA_ParameterDescription = new _Marshaller__seq_CORBA_ParameterDescription;
    _marshaller__seq_CORBA_ExceptionDef = new _Marshaller__seq_CORBA_ExceptionDef;
    _marshaller__seq_CORBA_ExtAttributeDescription = new _Marshaller__seq_CORBA_ExtAttributeDescription;
    _marshaller__seq_CORBA_LocalInterfaceDef = new _Marshaller__seq_CORBA_LocalInterfaceDef;
    _marshaller__seq_CORBA_ExceptionDescription = new _Marshaller__seq_CORBA_ExceptionDescription;
  }

  ~__tc_init_IR()
  {
    delete static_cast<_Marshaller_CORBA_Contained_Description*>(_marshaller_CORBA_Contained_Description);
    delete static_cast<_Marshaller_CORBA_Contained*>(_marshaller_CORBA_Contained);
    delete static_cast<_Marshaller_CORBA_Container_Description*>(_marshaller_CORBA_Container_Description);
    delete static_cast<_Marshaller_CORBA_Container*>(_marshaller_CORBA_Container);
    delete static_cast<_Marshaller_CORBA_PrimitiveKind*>(_marshaller_CORBA_PrimitiveKind);
    delete static_cast<_Marshaller_CORBA_Repository*>(_marshaller_CORBA_Repository);
    delete static_cast<_Marshaller_CORBA_ModuleDef*>(_marshaller_CORBA_ModuleDef);
    delete static_cast<_Marshaller_CORBA_ConstantDef*>(_marshaller_CORBA_ConstantDef);
    delete static_cast<_Marshaller_CORBA_TypedefDef*>(_marshaller_CORBA_TypedefDef);
    delete static_cast<_Marshaller_CORBA_StructDef*>(_marshaller_CORBA_StructDef);
    delete static_cast<_Marshaller_CORBA_ExceptionDef*>(_marshaller_CORBA_ExceptionDef);
    delete static_cast<_Marshaller_CORBA_UnionDef*>(_marshaller_CORBA_UnionDef);
    delete static_cast<_Marshaller_CORBA_EnumDef*>(_marshaller_CORBA_EnumDef);
    delete static_cast<_Marshaller_CORBA_AliasDef*>(_marshaller_CORBA_AliasDef);
    delete static_cast<_Marshaller_CORBA_AttributeMode*>(_marshaller_CORBA_AttributeMode);
    delete static_cast<_Marshaller_CORBA_OperationMode*>(_marshaller_CORBA_OperationMode);
    delete static_cast<_Marshaller_CORBA_InterfaceDef_FullInterfaceDescription*>(_marshaller_CORBA_InterfaceDef_FullInterfaceDescription);
    delete static_cast<_Marshaller_CORBA_InterfaceDef*>(_marshaller_CORBA_InterfaceDef);
    delete static_cast<_Marshaller_CORBA_AbstractInterfaceDef*>(_marshaller_CORBA_AbstractInterfaceDef);
    delete static_cast<_Marshaller_CORBA_LocalInterfaceDef*>(_marshaller_CORBA_LocalInterfaceDef);
    delete static_cast<_Marshaller_CORBA_ValueDef_FullValueDescription*>(_marshaller_CORBA_ValueDef_FullValueDescription);
    delete static_cast<_Marshaller_CORBA_ValueDef*>(_marshaller_CORBA_ValueDef);
    delete static_cast<_Marshaller_CORBA_ValueBoxDef*>(_marshaller_CORBA_ValueBoxDef);
    delete static_cast<_Marshaller_CORBA_NativeDef*>(_marshaller_CORBA_NativeDef);
    delete static_cast<_Marshaller_CORBA_ExtValueDef_ExtFullValueDescription*>(_marshaller_CORBA_ExtValueDef_ExtFullValueDescription);
    delete static_cast<_Marshaller_CORBA_ExtValueDef*>(_marshaller_CORBA_ExtValueDef);
    delete static_cast<_Marshaller_CORBA_ExceptionDescription*>(_marshaller_CORBA_ExceptionDescription);
    delete static_cast<_Marshaller_CORBA_Initializer*>(_marshaller_CORBA_Initializer);
    delete static_cast<_Marshaller_CORBA_ExtInitializer*>(_marshaller_CORBA_ExtInitializer);
    delete static_cast<_Marshaller_CORBA_PrimitiveDef*>(_marshaller_CORBA_PrimitiveDef);
    delete static_cast<_Marshaller_CORBA_StringDef*>(_marshaller_CORBA_StringDef);
    delete static_cast<_Marshaller_CORBA_WstringDef*>(_marshaller_CORBA_WstringDef);
    delete static_cast<_Marshaller_CORBA_SequenceDef*>(_marshaller_CORBA_SequenceDef);
    delete static_cast<_Marshaller_CORBA_ArrayDef*>(_marshaller_CORBA_ArrayDef);
    delete static_cast<_Marshaller_CORBA_FixedDef*>(_marshaller_CORBA_FixedDef);
    delete static_cast<_Marshaller_CORBA_ModuleDescription*>(_marshaller_CORBA_ModuleDescription);
    delete static_cast<_Marshaller_CORBA_ConstantDescription*>(_marshaller_CORBA_ConstantDescription);
    delete static_cast<_Marshaller_CORBA_TypeDescription*>(_marshaller_CORBA_TypeDescription);
    delete static_cast<_Marshaller_CORBA_AttributeDef*>(_marshaller_CORBA_AttributeDef);
    delete static_cast<_Marshaller_CORBA_AttributeDescription*>(_marshaller_CORBA_AttributeDescription);
    delete static_cast<_Marshaller_CORBA_ExtAttributeDescription*>(_marshaller_CORBA_ExtAttributeDescription);
    delete static_cast<_Marshaller_CORBA_ExtAttributeDef*>(_marshaller_CORBA_ExtAttributeDef);
    delete static_cast<_Marshaller_CORBA_ParameterMode*>(_marshaller_CORBA_ParameterMode);
    delete static_cast<_Marshaller_CORBA_ParameterDescription*>(_marshaller_CORBA_ParameterDescription);
    delete static_cast<_Marshaller_CORBA_OperationDef*>(_marshaller_CORBA_OperationDef);
    delete static_cast<_Marshaller_CORBA_OperationDescription*>(_marshaller_CORBA_OperationDescription);
    delete static_cast<_Marshaller_CORBA_InterfaceDescription*>(_marshaller_CORBA_InterfaceDescription);
    delete static_cast<_Marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription*>(_marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription);
    delete static_cast<_Marshaller_CORBA_InterfaceAttrExtension*>(_marshaller_CORBA_InterfaceAttrExtension);
    delete static_cast<_Marshaller_CORBA_ExtInterfaceDef*>(_marshaller_CORBA_ExtInterfaceDef);
    delete static_cast<_Marshaller_CORBA_ExtAbstractInterfaceDef*>(_marshaller_CORBA_ExtAbstractInterfaceDef);
    delete static_cast<_Marshaller_CORBA_ExtLocalInterfaceDef*>(_marshaller_CORBA_ExtLocalInterfaceDef);
    delete static_cast<_Marshaller_CORBA_ValueMemberDef*>(_marshaller_CORBA_ValueMemberDef);
    delete static_cast<_Marshaller_CORBA_ValueDescription*>(_marshaller_CORBA_ValueDescription);
    delete static_cast<_Marshaller__seq_CORBA_Contained*>(_marshaller__seq_CORBA_Contained);
    delete static_cast<_Marshaller__seq_CORBA_Container_Description*>(_marshaller__seq_CORBA_Container_Description);
    delete static_cast<_Marshaller__seq_CORBA_InterfaceDef*>(_marshaller__seq_CORBA_InterfaceDef);
    delete static_cast<_Marshaller__seq_CORBA_AbstractInterfaceDef*>(_marshaller__seq_CORBA_AbstractInterfaceDef);
    delete static_cast<_Marshaller__seq_CORBA_ValueDef*>(_marshaller__seq_CORBA_ValueDef);
    delete static_cast<_Marshaller__seq_CORBA_Initializer*>(_marshaller__seq_CORBA_Initializer);
    delete static_cast<_Marshaller__seq_CORBA_ExtInitializer*>(_marshaller__seq_CORBA_ExtInitializer);
    delete static_cast<_Marshaller__seq_CORBA_OperationDescription*>(_marshaller__seq_CORBA_OperationDescription);
    delete static_cast<_Marshaller__seq_CORBA_AttributeDescription*>(_marshaller__seq_CORBA_AttributeDescription);
    delete static_cast<_Marshaller__seq_CORBA_ParameterDescription*>(_marshaller__seq_CORBA_ParameterDescription);
    delete static_cast<_Marshaller__seq_CORBA_ExceptionDef*>(_marshaller__seq_CORBA_ExceptionDef);
    delete static_cast<_Marshaller__seq_CORBA_ExtAttributeDescription*>(_marshaller__seq_CORBA_ExtAttributeDescription);
    delete static_cast<_Marshaller__seq_CORBA_LocalInterfaceDef*>(_marshaller__seq_CORBA_LocalInterfaceDef);
    delete static_cast<_Marshaller__seq_CORBA_ExceptionDescription*>(_marshaller__seq_CORBA_ExceptionDescription);
  }
};

static __tc_init_IR __init_IR;

