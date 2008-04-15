/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <mico/ir3.h>

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

// PortableServer Skeleton Class for interface CORBA::ComponentIR::ComponentDef
POA_CORBA::ComponentIR::ComponentDef::~ComponentDef()
{
}

::CORBA::ComponentIR::ComponentDef_ptr
POA_CORBA::ComponentIR::ComponentDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::ComponentDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::ComponentDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ExtInterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::ComponentDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::ComponentDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/ComponentDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::ComponentDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::ComponentDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::ComponentDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 13)) {
    case 0:
      if( strcmp( __req->op_name(), "create_emits" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::ComponentIR::EventDef_var _par_event;
        CORBA::StaticAny _sa_event( _marshaller_CORBA_ComponentIR_EventDef, &_par_event._for_demarshal() );

        ::CORBA::ComponentIR::EmitsDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_EmitsDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_event );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_emits( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_event.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_supported_interfaces" ) == 0 ) {
        ::CORBA::InterfaceDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_InterfaceDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        supported_interfaces( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "create_uses" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::InterfaceDef_var _par_interface_type;
        CORBA::StaticAny _sa_interface_type( _marshaller_CORBA_InterfaceDef, &_par_interface_type._for_demarshal() );
        CORBA::Boolean _par_is_multiple;
        CORBA::StaticAny _sa_is_multiple( CORBA::_stc_boolean, &_par_is_multiple );

        ::CORBA::ComponentIR::UsesDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_UsesDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_interface_type );
        __req->add_in_arg( &_sa_is_multiple );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_uses( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_interface_type.inout(), _par_is_multiple );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "create_publishes" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::ComponentIR::EventDef_var _par_event;
        CORBA::StaticAny _sa_event( _marshaller_CORBA_ComponentIR_EventDef, &_par_event._for_demarshal() );

        ::CORBA::ComponentIR::PublishesDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_PublishesDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_event );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_publishes( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_event.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_supported_interfaces" ) == 0 ) {
        ::CORBA::InterfaceDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_InterfaceDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = supported_interfaces();
        __res.value( _marshaller__seq_CORBA_InterfaceDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "_set_base_component" ) == 0 ) {
        ::CORBA::ComponentIR::ComponentDef_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_ComponentDef, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        base_component( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "create_consumes" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::ComponentIR::EventDef_var _par_event;
        CORBA::StaticAny _sa_event( _marshaller_CORBA_ComponentIR_EventDef, &_par_event._for_demarshal() );

        ::CORBA::ComponentIR::ConsumesDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ConsumesDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_event );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_consumes( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_event.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 7:
      if( strcmp( __req->op_name(), "_get_base_component" ) == 0 ) {
        ::CORBA::ComponentIR::ComponentDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ComponentDef, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = base_component();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 8:
      if( strcmp( __req->op_name(), "create_provides" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::InterfaceDef_var _par_interface_type;
        CORBA::StaticAny _sa_interface_type( _marshaller_CORBA_InterfaceDef, &_par_interface_type._for_demarshal() );

        ::CORBA::ComponentIR::ProvidesDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ProvidesDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_interface_type );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_provides( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_interface_type.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_CORBA::ExtInterfaceDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::ComponentDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::HomeDef
POA_CORBA::ComponentIR::HomeDef::~HomeDef()
{
}

::CORBA::ComponentIR::HomeDef_ptr
POA_CORBA::ComponentIR::HomeDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::HomeDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::HomeDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ExtInterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::HomeDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::HomeDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/HomeDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::HomeDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::HomeDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::HomeDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 17)) {
    case 0:
      if( strcmp( __req->op_name(), "_set_base_home" ) == 0 ) {
        ::CORBA::ComponentIR::HomeDef_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_HomeDef, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        base_home( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "_set_supported_interfaces" ) == 0 ) {
        ::CORBA::InterfaceDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_InterfaceDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        supported_interfaces( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_get_primary_key" ) == 0 ) {
        ::CORBA::ValueDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ValueDef, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = primary_key();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_get_base_home" ) == 0 ) {
        ::CORBA::ComponentIR::HomeDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_HomeDef, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = base_home();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_managed_component" ) == 0 ) {
        ::CORBA::ComponentIR::ComponentDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ComponentDef, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = managed_component();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_primary_key" ) == 0 ) {
        ::CORBA::ValueDef_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_ValueDef, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        primary_key( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "create_finder" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::ParDescriptionSeq _par_params;
        CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
        ::CORBA::ExceptionDefSeq _par_exceptions;
        CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );

        ::CORBA::ComponentIR::FinderDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_FinderDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_params );
        __req->add_in_arg( &_sa_exceptions );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_finder( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_params, _par_exceptions );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_set_managed_component" ) == 0 ) {
        ::CORBA::ComponentIR::ComponentDef_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_ComponentDef, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        managed_component( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 7:
      if( strcmp( __req->op_name(), "create_factory" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::ParDescriptionSeq _par_params;
        CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
        ::CORBA::ExceptionDefSeq _par_exceptions;
        CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );

        ::CORBA::ComponentIR::FactoryDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_FactoryDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_params );
        __req->add_in_arg( &_sa_exceptions );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_factory( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_params, _par_exceptions );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_supported_interfaces" ) == 0 ) {
        ::CORBA::InterfaceDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_InterfaceDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = supported_interfaces();
        __res.value( _marshaller__seq_CORBA_InterfaceDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_CORBA::ExtInterfaceDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::HomeDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::EventDef
POA_CORBA::ComponentIR::EventDef::~EventDef()
{
}

::CORBA::ComponentIR::EventDef_ptr
POA_CORBA::ComponentIR::EventDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::EventDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::EventDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/EventDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ExtValueDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::EventDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/EventDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::EventDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/EventDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::EventDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::EventDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::EventDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ExtValueDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::EventDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::Container
POA_CORBA::ComponentIR::Container::~Container()
{
}

::CORBA::ComponentIR::Container_ptr
POA_CORBA::ComponentIR::Container::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::Container::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::Container::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/Container:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::Container::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/Container:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::Container::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/Container:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::Container::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::Container_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::Container::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "create_component" ) == 0 ) {
      ::CORBA::RepositoryId_var _par_id;
      CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
      ::CORBA::Identifier_var _par_name;
      CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
      ::CORBA::VersionSpec_var _par_version;
      CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
      ::CORBA::ComponentIR::ComponentDef_var _par_base_component;
      CORBA::StaticAny _sa_base_component( _marshaller_CORBA_ComponentIR_ComponentDef, &_par_base_component._for_demarshal() );
      ::CORBA::InterfaceDefSeq _par_supports_interfaces;
      CORBA::StaticAny _sa_supports_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_supports_interfaces );

      ::CORBA::ComponentIR::ComponentDef_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_ComponentDef, &_res );
      __req->add_in_arg( &_sa_id );
      __req->add_in_arg( &_sa_name );
      __req->add_in_arg( &_sa_version );
      __req->add_in_arg( &_sa_base_component );
      __req->add_in_arg( &_sa_supports_interfaces );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = create_component( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_base_component.inout(), _par_supports_interfaces );
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "create_home" ) == 0 ) {
      ::CORBA::RepositoryId_var _par_id;
      CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
      ::CORBA::Identifier_var _par_name;
      CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
      ::CORBA::VersionSpec_var _par_version;
      CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
      ::CORBA::ComponentIR::HomeDef_var _par_base_home;
      CORBA::StaticAny _sa_base_home( _marshaller_CORBA_ComponentIR_HomeDef, &_par_base_home._for_demarshal() );
      ::CORBA::ComponentIR::ComponentDef_var _par_managed_component;
      CORBA::StaticAny _sa_managed_component( _marshaller_CORBA_ComponentIR_ComponentDef, &_par_managed_component._for_demarshal() );
      ::CORBA::InterfaceDefSeq _par_supports_interfaces;
      CORBA::StaticAny _sa_supports_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_supports_interfaces );
      ::CORBA::ValueDef_var _par_primary_key;
      CORBA::StaticAny _sa_primary_key( _marshaller_CORBA_ValueDef, &_par_primary_key._for_demarshal() );

      ::CORBA::ComponentIR::HomeDef_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_HomeDef, &_res );
      __req->add_in_arg( &_sa_id );
      __req->add_in_arg( &_sa_name );
      __req->add_in_arg( &_sa_version );
      __req->add_in_arg( &_sa_base_home );
      __req->add_in_arg( &_sa_managed_component );
      __req->add_in_arg( &_sa_supports_interfaces );
      __req->add_in_arg( &_sa_primary_key );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = create_home( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_base_home.inout(), _par_managed_component.inout(), _par_supports_interfaces, _par_primary_key.inout() );
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "create_event" ) == 0 ) {
      ::CORBA::RepositoryId_var _par_id;
      CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
      ::CORBA::Identifier_var _par_name;
      CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
      ::CORBA::VersionSpec_var _par_version;
      CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
      CORBA::Boolean _par_is_custom;
      CORBA::StaticAny _sa_is_custom( CORBA::_stc_boolean, &_par_is_custom );
      CORBA::Boolean _par_is_abstract;
      CORBA::StaticAny _sa_is_abstract( CORBA::_stc_boolean, &_par_is_abstract );
      ::CORBA::ValueDef_var _par_base_value;
      CORBA::StaticAny _sa_base_value( _marshaller_CORBA_ValueDef, &_par_base_value._for_demarshal() );
      CORBA::Boolean _par_is_truncatable;
      CORBA::StaticAny _sa_is_truncatable( CORBA::_stc_boolean, &_par_is_truncatable );
      ::CORBA::ValueDefSeq _par_abstract_base_values;
      CORBA::StaticAny _sa_abstract_base_values( _marshaller__seq_CORBA_ValueDef, &_par_abstract_base_values );
      ::CORBA::InterfaceDefSeq _par_supported_interfaces;
      CORBA::StaticAny _sa_supported_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_supported_interfaces );
      ::CORBA::ExtInitializerSeq _par_initializers;
      CORBA::StaticAny _sa_initializers( _marshaller__seq_CORBA_ExtInitializer, &_par_initializers );

      ::CORBA::ComponentIR::EventDef_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_EventDef, &_res );
      __req->add_in_arg( &_sa_id );
      __req->add_in_arg( &_sa_name );
      __req->add_in_arg( &_sa_version );
      __req->add_in_arg( &_sa_is_custom );
      __req->add_in_arg( &_sa_is_abstract );
      __req->add_in_arg( &_sa_base_value );
      __req->add_in_arg( &_sa_is_truncatable );
      __req->add_in_arg( &_sa_abstract_base_values );
      __req->add_in_arg( &_sa_supported_interfaces );
      __req->add_in_arg( &_sa_initializers );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = create_event( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_is_custom, _par_is_abstract, _par_base_value.inout(), _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers );
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::Container::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::ModuleDef
POA_CORBA::ComponentIR::ModuleDef::~ModuleDef()
{
}

::CORBA::ComponentIR::ModuleDef_ptr
POA_CORBA::ComponentIR::ModuleDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::ModuleDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::ModuleDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ComponentIR::Container::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::ModuleDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::ModuleDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::ModuleDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/ModuleDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::ModuleDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::ModuleDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::ModuleDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ComponentIR::Container::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::ModuleDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::ModuleDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::Repository
POA_CORBA::ComponentIR::Repository::~Repository()
{
}

::CORBA::ComponentIR::Repository_ptr
POA_CORBA::ComponentIR::Repository::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::Repository::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::Repository::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/Repository:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ComponentIR::Container::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Repository::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::Repository::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/Repository:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::Repository::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/Repository:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::Repository::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::Repository_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::Repository::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ComponentIR::Container::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::Repository::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::Repository::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::ProvidesDef
POA_CORBA::ComponentIR::ProvidesDef::~ProvidesDef()
{
}

::CORBA::ComponentIR::ProvidesDef_ptr
POA_CORBA::ComponentIR::ProvidesDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::ProvidesDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::ProvidesDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::ProvidesDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::ProvidesDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/ProvidesDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::ProvidesDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::ProvidesDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::ProvidesDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_interface_type" ) == 0 ) {
      ::CORBA::InterfaceDef_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = interface_type();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "_set_interface_type" ) == 0 ) {
      ::CORBA::InterfaceDef_var _par__value;
      CORBA::StaticAny _sa__value( _marshaller_CORBA_InterfaceDef, &_par__value._for_demarshal() );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      interface_type( _par__value.inout() );
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::ProvidesDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::UsesDef
POA_CORBA::ComponentIR::UsesDef::~UsesDef()
{
}

::CORBA::ComponentIR::UsesDef_ptr
POA_CORBA::ComponentIR::UsesDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::UsesDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::UsesDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::UsesDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::UsesDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/UsesDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::UsesDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::UsesDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::UsesDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 2:
      if( strcmp( __req->op_name(), "_set_interface_type" ) == 0 ) {
        ::CORBA::InterfaceDef_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_InterfaceDef, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        interface_type( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_get_interface_type" ) == 0 ) {
        ::CORBA::InterfaceDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = interface_type();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_is_multiple" ) == 0 ) {
        CORBA::Boolean _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        is_multiple( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "_get_is_multiple" ) == 0 ) {
        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = is_multiple();
        __req->write_results();
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::UsesDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::EventPortDef
POA_CORBA::ComponentIR::EventPortDef::~EventPortDef()
{
}

::CORBA::ComponentIR::EventPortDef_ptr
POA_CORBA::ComponentIR::EventPortDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::EventPortDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::EventPortDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::EventPortDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::EventPortDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/EventPortDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::EventPortDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::EventPortDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::EventPortDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_event" ) == 0 ) {
      ::CORBA::ComponentIR::EventDef_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_ComponentIR_EventDef, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = event();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "_set_event" ) == 0 ) {
      ::CORBA::ComponentIR::EventDef_var _par__value;
      CORBA::StaticAny _sa__value( _marshaller_CORBA_ComponentIR_EventDef, &_par__value._for_demarshal() );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      event( _par__value.inout() );
      __req->write_results();
      return true;
    }
    if( strcmp( __req->op_name(), "is_a" ) == 0 ) {
      ::CORBA::RepositoryId_var _par_event_id;
      CORBA::StaticAny _sa_event_id( CORBA::_stc_string, &_par_event_id._for_demarshal() );

      CORBA::Boolean _res;
      CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
      __req->add_in_arg( &_sa_event_id );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = is_a( _par_event_id.inout() );
      __req->write_results();
      return true;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::EventPortDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::EmitsDef
POA_CORBA::ComponentIR::EmitsDef::~EmitsDef()
{
}

::CORBA::ComponentIR::EmitsDef_ptr
POA_CORBA::ComponentIR::EmitsDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::EmitsDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::EmitsDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ComponentIR::EventPortDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::EmitsDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::EmitsDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/EmitsDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::EmitsDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::EmitsDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::EmitsDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ComponentIR::EventPortDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::EmitsDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::PublishesDef
POA_CORBA::ComponentIR::PublishesDef::~PublishesDef()
{
}

::CORBA::ComponentIR::PublishesDef_ptr
POA_CORBA::ComponentIR::PublishesDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::PublishesDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::PublishesDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ComponentIR::EventPortDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::PublishesDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::PublishesDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/PublishesDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::PublishesDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::PublishesDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::PublishesDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ComponentIR::EventPortDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::PublishesDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::ConsumesDef
POA_CORBA::ComponentIR::ConsumesDef::~ConsumesDef()
{
}

::CORBA::ComponentIR::ConsumesDef_ptr
POA_CORBA::ComponentIR::ConsumesDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::ConsumesDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::ConsumesDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ComponentIR::EventPortDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::ConsumesDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::ConsumesDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/ConsumesDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::ConsumesDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::ConsumesDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::ConsumesDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ComponentIR::EventPortDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::ConsumesDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::FactoryDef
POA_CORBA::ComponentIR::FactoryDef::~FactoryDef()
{
}

::CORBA::ComponentIR::FactoryDef_ptr
POA_CORBA::ComponentIR::FactoryDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::FactoryDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::FactoryDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::OperationDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::FactoryDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::FactoryDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/FactoryDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::FactoryDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::FactoryDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::FactoryDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::OperationDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::FactoryDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ComponentIR::FinderDef
POA_CORBA::ComponentIR::FinderDef::~FinderDef()
{
}

::CORBA::ComponentIR::FinderDef_ptr
POA_CORBA::ComponentIR::FinderDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ComponentIR::FinderDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ComponentIR::FinderDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::OperationDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ComponentIR::FinderDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ComponentIR::FinderDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ComponentIR/FinderDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ComponentIR::FinderDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ComponentIR::FinderDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ComponentIR::FinderDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::OperationDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ComponentIR::FinderDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}

