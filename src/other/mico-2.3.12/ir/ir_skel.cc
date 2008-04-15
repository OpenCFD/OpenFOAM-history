/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>
#include <mico/template_impl.h>

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

// PortableServer Skeleton Class for interface CORBA::Contained
POA_CORBA::Contained::~Contained()
{
}

::CORBA::Contained_ptr
POA_CORBA::Contained::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::Contained::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::Contained::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/Contained:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IRObject::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::Contained::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/Contained:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::Contained::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/Contained:1.0");
}

CORBA::Object_ptr
POA_CORBA::Contained::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::Contained_stub_clp (poa, obj);
}

bool
POA_CORBA::Contained::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 17)) {
    case 0:
      if( strcmp( __req->op_name(), "describe" ) == 0 ) {
        ::CORBA::Contained::Description* _res;
        CORBA::StaticAny __res( _marshaller_CORBA_Contained_Description );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = describe();
        __res.value( _marshaller_CORBA_Contained_Description, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "_get_name" ) == 0 ) {
        ::CORBA::Identifier _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = name();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_version" ) == 0 ) {
        ::CORBA::VersionSpec _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = version();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "_get_absolute_name" ) == 0 ) {
        ::CORBA::ScopedName _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = absolute_name();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "move" ) == 0 ) {
        ::CORBA::Container_var _par_new_container;
        CORBA::StaticAny _sa_new_container( _marshaller_CORBA_Container, &_par_new_container._for_demarshal() );
        ::CORBA::Identifier_var _par_new_name;
        CORBA::StaticAny _sa_new_name( CORBA::_stc_string, &_par_new_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_new_version;
        CORBA::StaticAny _sa_new_version( CORBA::_stc_string, &_par_new_version._for_demarshal() );

        __req->add_in_arg( &_sa_new_container );
        __req->add_in_arg( &_sa_new_name );
        __req->add_in_arg( &_sa_new_version );

        if( !__req->read_args() )
          return true;

        move( _par_new_container.inout(), _par_new_name.inout(), _par_new_version.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_set_name" ) == 0 ) {
        ::CORBA::Identifier_var _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        name( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "_get_defined_in" ) == 0 ) {
        ::CORBA::Container_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_Container, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = defined_in();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 11:
      if( strcmp( __req->op_name(), "_get_id" ) == 0 ) {
        ::CORBA::RepositoryId _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = id();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 12:
      if( strcmp( __req->op_name(), "_get_containing_repository" ) == 0 ) {
        ::CORBA::Repository_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_Repository, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = containing_repository();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_version" ) == 0 ) {
        ::CORBA::VersionSpec_var _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        version( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 16:
      if( strcmp( __req->op_name(), "_set_id" ) == 0 ) {
        ::CORBA::RepositoryId_var _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_string, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        id( _par__value.inout() );
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

  if (POA_CORBA::IRObject::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::Contained::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::Container
POA_CORBA::Container::~Container()
{
}

::CORBA::Container_ptr
POA_CORBA::Container::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::Container::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::Container::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/Container:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IRObject::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::Container::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/Container:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::Container::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/Container:1.0");
}

CORBA::Object_ptr
POA_CORBA::Container::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::Container_stub_clp (poa, obj);
}

bool
POA_CORBA::Container::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 29)) {
    case 1:
      if( strcmp( __req->op_name(), "describe_contents" ) == 0 ) {
        ::CORBA::DefinitionKind _par_limit_type;
        CORBA::StaticAny _sa_limit_type( _marshaller_CORBA_DefinitionKind, &_par_limit_type );
        CORBA::Boolean _par_exclude_inherited;
        CORBA::StaticAny _sa_exclude_inherited( CORBA::_stc_boolean, &_par_exclude_inherited );
        CORBA::Long _par_max_returned_objs;
        CORBA::StaticAny _sa_max_returned_objs( CORBA::_stc_long, &_par_max_returned_objs );

        ::CORBA::Container::DescriptionSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_Container_Description );
        __req->add_in_arg( &_sa_limit_type );
        __req->add_in_arg( &_sa_exclude_inherited );
        __req->add_in_arg( &_sa_max_returned_objs );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = describe_contents( _par_limit_type, _par_exclude_inherited, _par_max_returned_objs );
        __res.value( _marshaller__seq_CORBA_Container_Description, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "lookup" ) == 0 ) {
        ::CORBA::ScopedName_var _par_search_name;
        CORBA::StaticAny _sa_search_name( CORBA::_stc_string, &_par_search_name._for_demarshal() );

        ::CORBA::Contained_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_Contained, &_res );
        __req->add_in_arg( &_sa_search_name );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = lookup( _par_search_name.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "create_ext_value" ) == 0 ) {
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

        ::CORBA::ExtValueDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ExtValueDef, &_res );
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

        _res = create_ext_value( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_is_custom, _par_is_abstract, _par_base_value.inout(), _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 11:
      if( strcmp( __req->op_name(), "create_abstract_interface" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::AbstractInterfaceDefSeq _par_base_interfaces;
        CORBA::StaticAny _sa_base_interfaces( _marshaller__seq_CORBA_AbstractInterfaceDef, &_par_base_interfaces );

        ::CORBA::AbstractInterfaceDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_AbstractInterfaceDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_base_interfaces );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_abstract_interface( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_base_interfaces );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "create_native" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );

        ::CORBA::NativeDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_NativeDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_native( _par_id.inout(), _par_name.inout(), _par_version.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 16:
      if( strcmp( __req->op_name(), "lookup_name" ) == 0 ) {
        ::CORBA::Identifier_var _par_search_name;
        CORBA::StaticAny _sa_search_name( CORBA::_stc_string, &_par_search_name._for_demarshal() );
        CORBA::Long _par_levels_to_search;
        CORBA::StaticAny _sa_levels_to_search( CORBA::_stc_long, &_par_levels_to_search );
        ::CORBA::DefinitionKind _par_limit_type;
        CORBA::StaticAny _sa_limit_type( _marshaller_CORBA_DefinitionKind, &_par_limit_type );
        CORBA::Boolean _par_exclude_inherited;
        CORBA::StaticAny _sa_exclude_inherited( CORBA::_stc_boolean, &_par_exclude_inherited );

        ::CORBA::ContainedSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_Contained );
        __req->add_in_arg( &_sa_search_name );
        __req->add_in_arg( &_sa_levels_to_search );
        __req->add_in_arg( &_sa_limit_type );
        __req->add_in_arg( &_sa_exclude_inherited );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = lookup_name( _par_search_name.inout(), _par_levels_to_search, _par_limit_type, _par_exclude_inherited );
        __res.value( _marshaller__seq_CORBA_Contained, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "create_value_box" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_original_type_def;
        CORBA::StaticAny _sa_original_type_def( _marshaller_CORBA_IDLType, &_par_original_type_def._for_demarshal() );

        ::CORBA::ValueBoxDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ValueBoxDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_original_type_def );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_value_box( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_original_type_def.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 18:
      if( strcmp( __req->op_name(), "create_interface" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::InterfaceDefSeq _par_base_interfaces;
        CORBA::StaticAny _sa_base_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_base_interfaces );

        ::CORBA::InterfaceDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_base_interfaces );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_interface( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_base_interfaces );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 20:
      if( strcmp( __req->op_name(), "create_exception" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::StructMemberSeq _par_members;
        CORBA::StaticAny _sa_members( _marshaller__seq_CORBA_StructMember, &_par_members );

        ::CORBA::ExceptionDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ExceptionDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_members );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_exception( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_members );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 22:
      if( strcmp( __req->op_name(), "contents" ) == 0 ) {
        ::CORBA::DefinitionKind _par_limit_type;
        CORBA::StaticAny _sa_limit_type( _marshaller_CORBA_DefinitionKind, &_par_limit_type );
        CORBA::Boolean _par_exclude_inherited;
        CORBA::StaticAny _sa_exclude_inherited( CORBA::_stc_boolean, &_par_exclude_inherited );

        ::CORBA::ContainedSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_Contained );
        __req->add_in_arg( &_sa_limit_type );
        __req->add_in_arg( &_sa_exclude_inherited );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = contents( _par_limit_type, _par_exclude_inherited );
        __res.value( _marshaller__seq_CORBA_Contained, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "create_module" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );

        ::CORBA::ModuleDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ModuleDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_module( _par_id.inout(), _par_name.inout(), _par_version.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 23:
      if( strcmp( __req->op_name(), "create_constant" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_type;
        CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type._for_demarshal() );
        CORBA::Any _par_value;
        CORBA::StaticAny _sa_value( CORBA::_stc_any, &_par_value );

        ::CORBA::ConstantDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ConstantDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_type );
        __req->add_in_arg( &_sa_value );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_constant( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_type.inout(), _par_value );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "create_enum" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::EnumMemberSeq _par_members;
        CORBA::StaticAny _sa_members( CORBA::_stcseq_string, &_par_members );

        ::CORBA::EnumDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_EnumDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_members );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_enum( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_members );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 24:
      if( strcmp( __req->op_name(), "create_alias" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_original_type;
        CORBA::StaticAny _sa_original_type( _marshaller_CORBA_IDLType, &_par_original_type._for_demarshal() );

        ::CORBA::AliasDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_AliasDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_original_type );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_alias( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_original_type.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 26:
      if( strcmp( __req->op_name(), "create_value" ) == 0 ) {
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
        ::CORBA::InitializerSeq _par_initializers;
        CORBA::StaticAny _sa_initializers( _marshaller__seq_CORBA_Initializer, &_par_initializers );

        ::CORBA::ValueDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ValueDef, &_res );
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

        _res = create_value( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_is_custom, _par_is_abstract, _par_base_value.inout(), _par_is_truncatable, _par_abstract_base_values, _par_supported_interfaces, _par_initializers );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 27:
      if( strcmp( __req->op_name(), "create_struct" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::StructMemberSeq _par_members;
        CORBA::StaticAny _sa_members( _marshaller__seq_CORBA_StructMember, &_par_members );

        ::CORBA::StructDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_StructDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_members );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_struct( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_members );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "create_union" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_discriminator_type;
        CORBA::StaticAny _sa_discriminator_type( _marshaller_CORBA_IDLType, &_par_discriminator_type._for_demarshal() );
        ::CORBA::UnionMemberSeq _par_members;
        CORBA::StaticAny _sa_members( _marshaller__seq_CORBA_UnionMember, &_par_members );

        ::CORBA::UnionDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_UnionDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_discriminator_type );
        __req->add_in_arg( &_sa_members );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_union( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_discriminator_type.inout(), _par_members );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 28:
      if( strcmp( __req->op_name(), "create_local_interface" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::InterfaceDefSeq _par_base_interfaces;
        CORBA::StaticAny _sa_base_interfaces( _marshaller__seq_CORBA_InterfaceDef, &_par_base_interfaces );

        ::CORBA::LocalInterfaceDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_LocalInterfaceDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_base_interfaces );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_local_interface( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_base_interfaces );
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

  if (POA_CORBA::IRObject::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::Container::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::Repository
POA_CORBA::Repository::~Repository()
{
}

::CORBA::Repository_ptr
POA_CORBA::Repository::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::Repository::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::Repository::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/Repository:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::Repository::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/Repository:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::Repository::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/Repository:1.0");
}

CORBA::Object_ptr
POA_CORBA::Repository::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::Repository_stub_clp (poa, obj);
}

bool
POA_CORBA::Repository::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 11)) {
    case 0:
      if( strcmp( __req->op_name(), "get_primitive" ) == 0 ) {
        ::CORBA::PrimitiveKind _par_kind;
        CORBA::StaticAny _sa_kind( _marshaller_CORBA_PrimitiveKind, &_par_kind );

        ::CORBA::PrimitiveDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_PrimitiveDef, &_res );
        __req->add_in_arg( &_sa_kind );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_primitive( _par_kind );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "create_string" ) == 0 ) {
        CORBA::ULong _par_bound;
        CORBA::StaticAny _sa_bound( CORBA::_stc_ulong, &_par_bound );

        ::CORBA::StringDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_StringDef, &_res );
        __req->add_in_arg( &_sa_bound );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_string( _par_bound );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "create_wstring" ) == 0 ) {
        CORBA::ULong _par_bound;
        CORBA::StaticAny _sa_bound( CORBA::_stc_ulong, &_par_bound );

        ::CORBA::WstringDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_WstringDef, &_res );
        __req->add_in_arg( &_sa_bound );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_wstring( _par_bound );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 8:
      if( strcmp( __req->op_name(), "create_array" ) == 0 ) {
        CORBA::ULong _par_length;
        CORBA::StaticAny _sa_length( CORBA::_stc_ulong, &_par_length );
        ::CORBA::IDLType_var _par_element_type;
        CORBA::StaticAny _sa_element_type( _marshaller_CORBA_IDLType, &_par_element_type._for_demarshal() );

        ::CORBA::ArrayDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ArrayDef, &_res );
        __req->add_in_arg( &_sa_length );
        __req->add_in_arg( &_sa_element_type );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_array( _par_length, _par_element_type.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 9:
      if( strcmp( __req->op_name(), "create_sequence" ) == 0 ) {
        CORBA::ULong _par_bound;
        CORBA::StaticAny _sa_bound( CORBA::_stc_ulong, &_par_bound );
        ::CORBA::IDLType_var _par_element_type;
        CORBA::StaticAny _sa_element_type( _marshaller_CORBA_IDLType, &_par_element_type._for_demarshal() );

        ::CORBA::SequenceDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_SequenceDef, &_res );
        __req->add_in_arg( &_sa_bound );
        __req->add_in_arg( &_sa_element_type );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_sequence( _par_bound, _par_element_type.inout() );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "create_fixed" ) == 0 ) {
        CORBA::UShort _par_digits;
        CORBA::StaticAny _sa_digits( CORBA::_stc_ushort, &_par_digits );
        CORBA::Short _par_scale;
        CORBA::StaticAny _sa_scale( CORBA::_stc_short, &_par_scale );

        ::CORBA::FixedDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_FixedDef, &_res );
        __req->add_in_arg( &_sa_digits );
        __req->add_in_arg( &_sa_scale );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_fixed( _par_digits, _par_scale );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "lookup_id" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_search_id;
        CORBA::StaticAny _sa_search_id( CORBA::_stc_string, &_par_search_id._for_demarshal() );

        ::CORBA::Contained_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_Contained, &_res );
        __req->add_in_arg( &_sa_search_id );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = lookup_id( _par_search_id.inout() );
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

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::Repository::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ModuleDef
POA_CORBA::ModuleDef::~ModuleDef()
{
}

::CORBA::ModuleDef_ptr
POA_CORBA::ModuleDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ModuleDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ModuleDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ModuleDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ModuleDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ModuleDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ModuleDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ModuleDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ModuleDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ModuleDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ModuleDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ModuleDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ConstantDef
POA_CORBA::ConstantDef::~ConstantDef()
{
}

::CORBA::ConstantDef_ptr
POA_CORBA::ConstantDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ConstantDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ConstantDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ConstantDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ConstantDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ConstantDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ConstantDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ConstantDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ConstantDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ConstantDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ConstantDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 2:
      if( strcmp( __req->op_name(), "_get_type_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = type_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_set_value" ) == 0 ) {
        CORBA::Any _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_any, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        value( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "_set_type_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        type_def( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_get_type" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = type();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_value" ) == 0 ) {
        CORBA::Any* _res;
        CORBA::StaticAny __res( CORBA::_stc_any );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = value();
        __res.value( CORBA::_stc_any, _res );
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

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ConstantDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::TypedefDef
POA_CORBA::TypedefDef::~TypedefDef()
{
}

::CORBA::TypedefDef_ptr
POA_CORBA::TypedefDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::TypedefDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::TypedefDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/TypedefDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::TypedefDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/TypedefDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::TypedefDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/TypedefDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::TypedefDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::TypedefDef_stub_clp (poa, obj);
}

bool
POA_CORBA::TypedefDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::TypedefDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::StructDef
POA_CORBA::StructDef::~StructDef()
{
}

::CORBA::StructDef_ptr
POA_CORBA::StructDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::StructDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::StructDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/StructDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::TypedefDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::StructDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/StructDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::StructDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/StructDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::StructDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::StructDef_stub_clp (poa, obj);
}

bool
POA_CORBA::StructDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_members" ) == 0 ) {
      ::CORBA::StructMemberSeq* _res;
      CORBA::StaticAny __res( _marshaller__seq_CORBA_StructMember );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = members();
      __res.value( _marshaller__seq_CORBA_StructMember, _res );
      __req->write_results();
      delete _res;
      return true;
    }
    if( strcmp( __req->op_name(), "_set_members" ) == 0 ) {
      ::CORBA::StructMemberSeq _par__value;
      CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_StructMember, &_par__value );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      members( _par__value );
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

  if (POA_CORBA::TypedefDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::StructDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ExceptionDef
POA_CORBA::ExceptionDef::~ExceptionDef()
{
}

::CORBA::ExceptionDef_ptr
POA_CORBA::ExceptionDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ExceptionDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ExceptionDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExceptionDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ExceptionDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ExceptionDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ExceptionDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ExceptionDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ExceptionDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ExceptionDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ExceptionDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_type" ) == 0 ) {
      CORBA::TypeCode_ptr _res;
      CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = type();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "_get_members" ) == 0 ) {
      ::CORBA::StructMemberSeq* _res;
      CORBA::StaticAny __res( _marshaller__seq_CORBA_StructMember );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = members();
      __res.value( _marshaller__seq_CORBA_StructMember, _res );
      __req->write_results();
      delete _res;
      return true;
    }
    if( strcmp( __req->op_name(), "_set_members" ) == 0 ) {
      ::CORBA::StructMemberSeq _par__value;
      CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_StructMember, &_par__value );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      members( _par__value );
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

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ExceptionDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::UnionDef
POA_CORBA::UnionDef::~UnionDef()
{
}

::CORBA::UnionDef_ptr
POA_CORBA::UnionDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::UnionDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::UnionDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/UnionDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::TypedefDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::UnionDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/UnionDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::UnionDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/UnionDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::UnionDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::UnionDef_stub_clp (poa, obj);
}

bool
POA_CORBA::UnionDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 0:
      if( strcmp( __req->op_name(), "_get_discriminator_type" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = discriminator_type();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_members" ) == 0 ) {
        ::CORBA::UnionMemberSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_UnionMember );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = members();
        __res.value( _marshaller__seq_CORBA_UnionMember, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "_set_discriminator_type_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        discriminator_type_def( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_set_members" ) == 0 ) {
        ::CORBA::UnionMemberSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_UnionMember, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        members( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_get_discriminator_type_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = discriminator_type_def();
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

  if (POA_CORBA::TypedefDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::UnionDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::EnumDef
POA_CORBA::EnumDef::~EnumDef()
{
}

::CORBA::EnumDef_ptr
POA_CORBA::EnumDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::EnumDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::EnumDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/EnumDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::TypedefDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::EnumDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/EnumDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::EnumDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/EnumDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::EnumDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::EnumDef_stub_clp (poa, obj);
}

bool
POA_CORBA::EnumDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_members" ) == 0 ) {
      ::CORBA::EnumMemberSeq* _res;
      CORBA::StaticAny __res( CORBA::_stcseq_string );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = members();
      __res.value( CORBA::_stcseq_string, _res );
      __req->write_results();
      delete _res;
      return true;
    }
    if( strcmp( __req->op_name(), "_set_members" ) == 0 ) {
      ::CORBA::EnumMemberSeq _par__value;
      CORBA::StaticAny _sa__value( CORBA::_stcseq_string, &_par__value );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      members( _par__value );
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

  if (POA_CORBA::TypedefDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::EnumDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::AliasDef
POA_CORBA::AliasDef::~AliasDef()
{
}

::CORBA::AliasDef_ptr
POA_CORBA::AliasDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::AliasDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::AliasDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/AliasDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::TypedefDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::AliasDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/AliasDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::AliasDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/AliasDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::AliasDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::AliasDef_stub_clp (poa, obj);
}

bool
POA_CORBA::AliasDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_original_type_def" ) == 0 ) {
      ::CORBA::IDLType_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = original_type_def();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "_set_original_type_def" ) == 0 ) {
      ::CORBA::IDLType_var _par__value;
      CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      original_type_def( _par__value.inout() );
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

  if (POA_CORBA::TypedefDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::AliasDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::InterfaceDef
POA_CORBA::InterfaceDef::~InterfaceDef()
{
}

::CORBA::InterfaceDef_ptr
POA_CORBA::InterfaceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::InterfaceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::InterfaceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/InterfaceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::InterfaceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/InterfaceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::InterfaceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/InterfaceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::InterfaceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::InterfaceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::InterfaceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 11)) {
    case 0:
      if( strcmp( __req->op_name(), "create_operation" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_result;
        CORBA::StaticAny _sa_result( _marshaller_CORBA_IDLType, &_par_result._for_demarshal() );
        ::CORBA::OperationMode _par_mode;
        CORBA::StaticAny _sa_mode( _marshaller_CORBA_OperationMode, &_par_mode );
        ::CORBA::ParDescriptionSeq _par_params;
        CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
        ::CORBA::ExceptionDefSeq _par_exceptions;
        CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );
        ::CORBA::ContextIdSeq _par_contexts;
        CORBA::StaticAny _sa_contexts( CORBA::_stcseq_string, &_par_contexts );

        ::CORBA::OperationDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_OperationDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_result );
        __req->add_in_arg( &_sa_mode );
        __req->add_in_arg( &_sa_params );
        __req->add_in_arg( &_sa_exceptions );
        __req->add_in_arg( &_sa_contexts );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_operation( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_result.inout(), _par_mode, _par_params, _par_exceptions, _par_contexts );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "_set_base_interfaces" ) == 0 ) {
        ::CORBA::InterfaceDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_InterfaceDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        base_interfaces( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "describe_interface" ) == 0 ) {
        ::CORBA::InterfaceDef::FullInterfaceDescription* _res;
        CORBA::StaticAny __res( _marshaller_CORBA_InterfaceDef_FullInterfaceDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = describe_interface();
        __res.value( _marshaller_CORBA_InterfaceDef_FullInterfaceDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "is_a" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_interface_id;
        CORBA::StaticAny _sa_interface_id( CORBA::_stc_string, &_par_interface_id._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_interface_id );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = is_a( _par_interface_id.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 7:
      if( strcmp( __req->op_name(), "create_attribute" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_type;
        CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type._for_demarshal() );
        ::CORBA::AttributeMode _par_mode;
        CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );

        ::CORBA::AttributeDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_AttributeDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_type );
        __req->add_in_arg( &_sa_mode );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_attribute( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_type.inout(), _par_mode );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_base_interfaces" ) == 0 ) {
        ::CORBA::InterfaceDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_InterfaceDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = base_interfaces();
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

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::InterfaceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::AbstractInterfaceDef
POA_CORBA::AbstractInterfaceDef::~AbstractInterfaceDef()
{
}

::CORBA::AbstractInterfaceDef_ptr
POA_CORBA::AbstractInterfaceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::AbstractInterfaceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::AbstractInterfaceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/AbstractInterfaceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::InterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::AbstractInterfaceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/AbstractInterfaceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::AbstractInterfaceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/AbstractInterfaceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::AbstractInterfaceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::AbstractInterfaceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::AbstractInterfaceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::InterfaceDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::AbstractInterfaceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::LocalInterfaceDef
POA_CORBA::LocalInterfaceDef::~LocalInterfaceDef()
{
}

::CORBA::LocalInterfaceDef_ptr
POA_CORBA::LocalInterfaceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::LocalInterfaceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::LocalInterfaceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/LocalInterfaceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::InterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::LocalInterfaceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/LocalInterfaceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::LocalInterfaceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/LocalInterfaceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::LocalInterfaceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::LocalInterfaceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::LocalInterfaceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::InterfaceDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::LocalInterfaceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ValueDef
POA_CORBA::ValueDef::~ValueDef()
{
}

::CORBA::ValueDef_ptr
POA_CORBA::ValueDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ValueDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ValueDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ValueDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Container::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ValueDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ValueDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ValueDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ValueDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ValueDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ValueDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ValueDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 29)) {
    case 0:
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
    case 4:
      if( strcmp( __req->op_name(), "create_value_member" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_type;
        CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type._for_demarshal() );
        ::CORBA::Visibility _par_access;
        CORBA::StaticAny _sa_access( CORBA::_stc_short, &_par_access );

        ::CORBA::ValueMemberDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ValueMemberDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_type );
        __req->add_in_arg( &_sa_access );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_value_member( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_type.inout(), _par_access );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "describe_value" ) == 0 ) {
        ::CORBA::ValueDef::FullValueDescription* _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ValueDef_FullValueDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = describe_value();
        __res.value( _marshaller_CORBA_ValueDef_FullValueDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 6:
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
    case 8:
      if( strcmp( __req->op_name(), "is_a" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_value_id;
        CORBA::StaticAny _sa_value_id( CORBA::_stc_string, &_par_value_id._for_demarshal() );

        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->add_in_arg( &_sa_value_id );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = is_a( _par_value_id.inout() );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_set_is_abstract" ) == 0 ) {
        CORBA::Boolean _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        is_abstract( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "_get_is_abstract" ) == 0 ) {
        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = is_abstract();
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_get_is_truncatable" ) == 0 ) {
        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = is_truncatable();
        __req->write_results();
        return true;
      }
      break;
    case 11:
      if( strcmp( __req->op_name(), "_set_abstract_base_values" ) == 0 ) {
        ::CORBA::ValueDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ValueDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        abstract_base_values( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 13:
      if( strcmp( __req->op_name(), "create_attribute" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_type;
        CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type._for_demarshal() );
        ::CORBA::AttributeMode _par_mode;
        CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );

        ::CORBA::AttributeDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_AttributeDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_type );
        __req->add_in_arg( &_sa_mode );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_attribute( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_type.inout(), _par_mode );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 14:
      if( strcmp( __req->op_name(), "create_operation" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_result;
        CORBA::StaticAny _sa_result( _marshaller_CORBA_IDLType, &_par_result._for_demarshal() );
        ::CORBA::OperationMode _par_mode;
        CORBA::StaticAny _sa_mode( _marshaller_CORBA_OperationMode, &_par_mode );
        ::CORBA::ParDescriptionSeq _par_params;
        CORBA::StaticAny _sa_params( _marshaller__seq_CORBA_ParameterDescription, &_par_params );
        ::CORBA::ExceptionDefSeq _par_exceptions;
        CORBA::StaticAny _sa_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_exceptions );
        ::CORBA::ContextIdSeq _par_contexts;
        CORBA::StaticAny _sa_contexts( CORBA::_stcseq_string, &_par_contexts );

        ::CORBA::OperationDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_OperationDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_result );
        __req->add_in_arg( &_sa_mode );
        __req->add_in_arg( &_sa_params );
        __req->add_in_arg( &_sa_exceptions );
        __req->add_in_arg( &_sa_contexts );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_operation( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_result.inout(), _par_mode, _par_params, _par_exceptions, _par_contexts );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 15:
      if( strcmp( __req->op_name(), "_set_base_value" ) == 0 ) {
        ::CORBA::ValueDef_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_ValueDef, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        base_value( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 16:
      if( strcmp( __req->op_name(), "_get_base_value" ) == 0 ) {
        ::CORBA::ValueDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ValueDef, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = base_value();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 18:
      if( strcmp( __req->op_name(), "_get_is_custom" ) == 0 ) {
        CORBA::Boolean _res;
        CORBA::StaticAny __res( CORBA::_stc_boolean, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = is_custom();
        __req->write_results();
        return true;
      }
      break;
    case 24:
      if( strcmp( __req->op_name(), "_set_initializers" ) == 0 ) {
        ::CORBA::InitializerSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_Initializer, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        initializers( _par__value );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_set_is_custom" ) == 0 ) {
        CORBA::Boolean _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        is_custom( _par__value );
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_set_is_truncatable" ) == 0 ) {
        CORBA::Boolean _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_boolean, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        is_truncatable( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 25:
      if( strcmp( __req->op_name(), "_get_abstract_base_values" ) == 0 ) {
        ::CORBA::ValueDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ValueDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = abstract_base_values();
        __res.value( _marshaller__seq_CORBA_ValueDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 27:
      if( strcmp( __req->op_name(), "_get_initializers" ) == 0 ) {
        ::CORBA::InitializerSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_Initializer );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = initializers();
        __res.value( _marshaller__seq_CORBA_Initializer, _res );
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

  if (POA_CORBA::Container::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ValueDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ValueBoxDef
POA_CORBA::ValueBoxDef::~ValueBoxDef()
{
}

::CORBA::ValueBoxDef_ptr
POA_CORBA::ValueBoxDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ValueBoxDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ValueBoxDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ValueBoxDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::TypedefDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ValueBoxDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ValueBoxDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ValueBoxDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ValueBoxDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ValueBoxDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ValueBoxDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ValueBoxDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_original_type_def" ) == 0 ) {
      ::CORBA::IDLType_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = original_type_def();
      __req->write_results();
      CORBA::release( _res );
      return true;
    }
    if( strcmp( __req->op_name(), "_set_original_type_def" ) == 0 ) {
      ::CORBA::IDLType_var _par__value;
      CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      original_type_def( _par__value.inout() );
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

  if (POA_CORBA::TypedefDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ValueBoxDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::NativeDef
POA_CORBA::NativeDef::~NativeDef()
{
}

::CORBA::NativeDef_ptr
POA_CORBA::NativeDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::NativeDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::NativeDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/NativeDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::TypedefDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::NativeDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/NativeDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::NativeDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/NativeDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::NativeDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::NativeDef_stub_clp (poa, obj);
}

bool
POA_CORBA::NativeDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::TypedefDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::NativeDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ExtValueDef
POA_CORBA::ExtValueDef::~ExtValueDef()
{
}

::CORBA::ExtValueDef_ptr
POA_CORBA::ExtValueDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ExtValueDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ExtValueDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtValueDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ValueDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ExtValueDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ExtValueDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ExtValueDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ExtValueDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ExtValueDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ExtValueDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ExtValueDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 2:
      if( strcmp( __req->op_name(), "describe_ext_value" ) == 0 ) {
        ::CORBA::ExtValueDef::ExtFullValueDescription* _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ExtValueDef_ExtFullValueDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = describe_ext_value();
        __res.value( _marshaller_CORBA_ExtValueDef_ExtFullValueDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      if( strcmp( __req->op_name(), "_set_ext_initializers" ) == 0 ) {
        ::CORBA::ExtInitializerSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExtInitializer, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        ext_initializers( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "create_ext_attribute" ) == 0 ) {
        ::CORBA::RepositoryId_var _par_id;
        CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
        ::CORBA::Identifier_var _par_name;
        CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
        ::CORBA::VersionSpec_var _par_version;
        CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
        ::CORBA::IDLType_var _par_type;
        CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type._for_demarshal() );
        ::CORBA::AttributeMode _par_mode;
        CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );
        ::CORBA::ExceptionDefSeq _par_get_exceptions;
        CORBA::StaticAny _sa_get_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_get_exceptions );
        ::CORBA::ExceptionDefSeq _par_set_exceptions;
        CORBA::StaticAny _sa_set_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_set_exceptions );

        ::CORBA::ExtAttributeDef_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ExtAttributeDef, &_res );
        __req->add_in_arg( &_sa_id );
        __req->add_in_arg( &_sa_name );
        __req->add_in_arg( &_sa_version );
        __req->add_in_arg( &_sa_type );
        __req->add_in_arg( &_sa_mode );
        __req->add_in_arg( &_sa_get_exceptions );
        __req->add_in_arg( &_sa_set_exceptions );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = create_ext_attribute( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_type.inout(), _par_mode, _par_get_exceptions, _par_set_exceptions );
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_get_ext_initializers" ) == 0 ) {
        ::CORBA::ExtInitializerSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ExtInitializer );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = ext_initializers();
        __res.value( _marshaller__seq_CORBA_ExtInitializer, _res );
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

  if (POA_CORBA::ValueDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ExtValueDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::PrimitiveDef
POA_CORBA::PrimitiveDef::~PrimitiveDef()
{
}

::CORBA::PrimitiveDef_ptr
POA_CORBA::PrimitiveDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::PrimitiveDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::PrimitiveDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/PrimitiveDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::PrimitiveDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/PrimitiveDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::PrimitiveDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/PrimitiveDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::PrimitiveDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::PrimitiveDef_stub_clp (poa, obj);
}

bool
POA_CORBA::PrimitiveDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_kind" ) == 0 ) {
      ::CORBA::PrimitiveKind _res;
      CORBA::StaticAny __res( _marshaller_CORBA_PrimitiveKind, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = kind();
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

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::PrimitiveDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::StringDef
POA_CORBA::StringDef::~StringDef()
{
}

::CORBA::StringDef_ptr
POA_CORBA::StringDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::StringDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::StringDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/StringDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::StringDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/StringDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::StringDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/StringDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::StringDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::StringDef_stub_clp (poa, obj);
}

bool
POA_CORBA::StringDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_bound" ) == 0 ) {
      CORBA::ULong _res;
      CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = bound();
      __req->write_results();
      return true;
    }
    if( strcmp( __req->op_name(), "_set_bound" ) == 0 ) {
      CORBA::ULong _par__value;
      CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      bound( _par__value );
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

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::StringDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::WstringDef
POA_CORBA::WstringDef::~WstringDef()
{
}

::CORBA::WstringDef_ptr
POA_CORBA::WstringDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::WstringDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::WstringDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/WstringDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::WstringDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/WstringDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::WstringDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/WstringDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::WstringDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::WstringDef_stub_clp (poa, obj);
}

bool
POA_CORBA::WstringDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "_get_bound" ) == 0 ) {
      CORBA::ULong _res;
      CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = bound();
      __req->write_results();
      return true;
    }
    if( strcmp( __req->op_name(), "_set_bound" ) == 0 ) {
      CORBA::ULong _par__value;
      CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );

      __req->add_in_arg( &_sa__value );

      if( !__req->read_args() )
        return true;

      bound( _par__value );
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

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::WstringDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::SequenceDef
POA_CORBA::SequenceDef::~SequenceDef()
{
}

::CORBA::SequenceDef_ptr
POA_CORBA::SequenceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::SequenceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::SequenceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/SequenceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::SequenceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/SequenceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::SequenceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/SequenceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::SequenceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::SequenceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::SequenceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 1:
      if( strcmp( __req->op_name(), "_get_element_type" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = element_type();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_bound" ) == 0 ) {
        CORBA::ULong _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        bound( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_get_element_type_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = element_type_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_set_element_type_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        element_type_def( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_get_bound" ) == 0 ) {
        CORBA::ULong _res;
        CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = bound();
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

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::SequenceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ArrayDef
POA_CORBA::ArrayDef::~ArrayDef()
{
}

::CORBA::ArrayDef_ptr
POA_CORBA::ArrayDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ArrayDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ArrayDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ArrayDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ArrayDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ArrayDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ArrayDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ArrayDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ArrayDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ArrayDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ArrayDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 0:
      if( strcmp( __req->op_name(), "_get_length" ) == 0 ) {
        CORBA::ULong _res;
        CORBA::StaticAny __res( CORBA::_stc_ulong, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = length();
        __req->write_results();
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "_get_element_type" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = element_type();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_get_element_type_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = element_type_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_length" ) == 0 ) {
        CORBA::ULong _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_ulong, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        length( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_set_element_type_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        element_type_def( _par__value.inout() );
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

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ArrayDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::FixedDef
POA_CORBA::FixedDef::~FixedDef()
{
}

::CORBA::FixedDef_ptr
POA_CORBA::FixedDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::FixedDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::FixedDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/FixedDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::IDLType::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::FixedDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/FixedDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::FixedDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/FixedDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::FixedDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::FixedDef_stub_clp (poa, obj);
}

bool
POA_CORBA::FixedDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 1:
      if( strcmp( __req->op_name(), "_get_digits" ) == 0 ) {
        CORBA::UShort _res;
        CORBA::StaticAny __res( CORBA::_stc_ushort, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = digits();
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_set_digits" ) == 0 ) {
        CORBA::UShort _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_ushort, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        digits( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "_set_scale" ) == 0 ) {
        CORBA::Short _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_short, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        scale( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_get_scale" ) == 0 ) {
        CORBA::Short _res;
        CORBA::StaticAny __res( CORBA::_stc_short, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = scale();
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

  if (POA_CORBA::IDLType::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::FixedDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::AttributeDef
POA_CORBA::AttributeDef::~AttributeDef()
{
}

::CORBA::AttributeDef_ptr
POA_CORBA::AttributeDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::AttributeDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::AttributeDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/AttributeDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::AttributeDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/AttributeDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::AttributeDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/AttributeDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::AttributeDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::AttributeDef_stub_clp (poa, obj);
}

bool
POA_CORBA::AttributeDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 2:
      if( strcmp( __req->op_name(), "_get_type_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = type_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      if( strcmp( __req->op_name(), "_set_mode" ) == 0 ) {
        ::CORBA::AttributeMode _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_AttributeMode, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        mode( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "_get_mode" ) == 0 ) {
        ::CORBA::AttributeMode _res;
        CORBA::StaticAny __res( _marshaller_CORBA_AttributeMode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = mode();
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_set_type_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        type_def( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_get_type" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = type();
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

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::AttributeDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ExtAttributeDef
POA_CORBA::ExtAttributeDef::~ExtAttributeDef()
{
}

::CORBA::ExtAttributeDef_ptr
POA_CORBA::ExtAttributeDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ExtAttributeDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ExtAttributeDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtAttributeDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::AttributeDef::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ExtAttributeDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ExtAttributeDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ExtAttributeDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ExtAttributeDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ExtAttributeDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ExtAttributeDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ExtAttributeDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 1:
      if( strcmp( __req->op_name(), "_get_get_exceptions" ) == 0 ) {
        ::CORBA::ExceptionDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ExceptionDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = get_exceptions();
        __res.value( _marshaller__seq_CORBA_ExceptionDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "describe_attribute" ) == 0 ) {
        ::CORBA::ExtAttributeDescription* _res;
        CORBA::StaticAny __res( _marshaller_CORBA_ExtAttributeDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = describe_attribute();
        __res.value( _marshaller_CORBA_ExtAttributeDescription, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_get_set_exceptions" ) == 0 ) {
        ::CORBA::ExceptionDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ExceptionDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = set_exceptions();
        __res.value( _marshaller__seq_CORBA_ExceptionDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 4:
      if( strcmp( __req->op_name(), "_set_get_exceptions" ) == 0 ) {
        ::CORBA::ExceptionDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExceptionDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        get_exceptions( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_set_set_exceptions" ) == 0 ) {
        ::CORBA::ExceptionDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExceptionDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        set_exceptions( _par__value );
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

  if (POA_CORBA::AttributeDef::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ExtAttributeDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::OperationDef
POA_CORBA::OperationDef::~OperationDef()
{
}

::CORBA::OperationDef_ptr
POA_CORBA::OperationDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::OperationDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::OperationDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/OperationDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::OperationDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/OperationDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::OperationDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/OperationDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::OperationDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::OperationDef_stub_clp (poa, obj);
}

bool
POA_CORBA::OperationDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 17)) {
    case 0:
      if( strcmp( __req->op_name(), "_get_mode" ) == 0 ) {
        ::CORBA::OperationMode _res;
        CORBA::StaticAny __res( _marshaller_CORBA_OperationMode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = mode();
        __req->write_results();
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "_get_result_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = result_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_set_result_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        result_def( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 3:
      if( strcmp( __req->op_name(), "_set_params" ) == 0 ) {
        ::CORBA::ParDescriptionSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ParameterDescription, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        params( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 7:
      if( strcmp( __req->op_name(), "_set_mode" ) == 0 ) {
        ::CORBA::OperationMode _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_OperationMode, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        mode( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 8:
      if( strcmp( __req->op_name(), "_set_exceptions" ) == 0 ) {
        ::CORBA::ExceptionDefSeq _par__value;
        CORBA::StaticAny _sa__value( _marshaller__seq_CORBA_ExceptionDef, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        exceptions( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "_set_contexts" ) == 0 ) {
        ::CORBA::ContextIdSeq _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stcseq_string, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        contexts( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 11:
      if( strcmp( __req->op_name(), "_get_contexts" ) == 0 ) {
        ::CORBA::ContextIdSeq* _res;
        CORBA::StaticAny __res( CORBA::_stcseq_string );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = contexts();
        __res.value( CORBA::_stcseq_string, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 13:
      if( strcmp( __req->op_name(), "_get_exceptions" ) == 0 ) {
        ::CORBA::ExceptionDefSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ExceptionDef );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = exceptions();
        __res.value( _marshaller__seq_CORBA_ExceptionDef, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 14:
      if( strcmp( __req->op_name(), "_get_result" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = result();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 15:
      if( strcmp( __req->op_name(), "_get_params" ) == 0 ) {
        ::CORBA::ParDescriptionSeq* _res;
        CORBA::StaticAny __res( _marshaller__seq_CORBA_ParameterDescription );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = params();
        __res.value( _marshaller__seq_CORBA_ParameterDescription, _res );
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

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::OperationDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::InterfaceAttrExtension
POA_CORBA::InterfaceAttrExtension::~InterfaceAttrExtension()
{
}

::CORBA::InterfaceAttrExtension_ptr
POA_CORBA::InterfaceAttrExtension::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::InterfaceAttrExtension::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::InterfaceAttrExtension::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/InterfaceAttrExtension:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::InterfaceAttrExtension::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/InterfaceAttrExtension:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::InterfaceAttrExtension::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/InterfaceAttrExtension:1.0");
}

CORBA::Object_ptr
POA_CORBA::InterfaceAttrExtension::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::InterfaceAttrExtension_stub_clp (poa, obj);
}

bool
POA_CORBA::InterfaceAttrExtension::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    if( strcmp( __req->op_name(), "describe_ext_interface" ) == 0 ) {
      ::CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription* _res;
      CORBA::StaticAny __res( _marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = describe_ext_interface();
      __res.value( _marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription, _res );
      __req->write_results();
      delete _res;
      return true;
    }
    if( strcmp( __req->op_name(), "create_ext_attribute" ) == 0 ) {
      ::CORBA::RepositoryId_var _par_id;
      CORBA::StaticAny _sa_id( CORBA::_stc_string, &_par_id._for_demarshal() );
      ::CORBA::Identifier_var _par_name;
      CORBA::StaticAny _sa_name( CORBA::_stc_string, &_par_name._for_demarshal() );
      ::CORBA::VersionSpec_var _par_version;
      CORBA::StaticAny _sa_version( CORBA::_stc_string, &_par_version._for_demarshal() );
      ::CORBA::IDLType_var _par_type;
      CORBA::StaticAny _sa_type( _marshaller_CORBA_IDLType, &_par_type._for_demarshal() );
      ::CORBA::AttributeMode _par_mode;
      CORBA::StaticAny _sa_mode( _marshaller_CORBA_AttributeMode, &_par_mode );
      ::CORBA::ExceptionDefSeq _par_get_exceptions;
      CORBA::StaticAny _sa_get_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_get_exceptions );
      ::CORBA::ExceptionDefSeq _par_set_exceptions;
      CORBA::StaticAny _sa_set_exceptions( _marshaller__seq_CORBA_ExceptionDef, &_par_set_exceptions );

      ::CORBA::ExtAttributeDef_ptr _res;
      CORBA::StaticAny __res( _marshaller_CORBA_ExtAttributeDef, &_res );
      __req->add_in_arg( &_sa_id );
      __req->add_in_arg( &_sa_name );
      __req->add_in_arg( &_sa_version );
      __req->add_in_arg( &_sa_type );
      __req->add_in_arg( &_sa_mode );
      __req->add_in_arg( &_sa_get_exceptions );
      __req->add_in_arg( &_sa_set_exceptions );
      __req->set_result( &__res );

      if( !__req->read_args() )
        return true;

      _res = create_ext_attribute( _par_id.inout(), _par_name.inout(), _par_version.inout(), _par_type.inout(), _par_mode, _par_get_exceptions, _par_set_exceptions );
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

  return false;
}

void
POA_CORBA::InterfaceAttrExtension::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ExtInterfaceDef
POA_CORBA::ExtInterfaceDef::~ExtInterfaceDef()
{
}

::CORBA::ExtInterfaceDef_ptr
POA_CORBA::ExtInterfaceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ExtInterfaceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ExtInterfaceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtInterfaceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::InterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::InterfaceAttrExtension::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ExtInterfaceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ExtInterfaceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ExtInterfaceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ExtInterfaceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ExtInterfaceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ExtInterfaceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ExtInterfaceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::InterfaceDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::InterfaceAttrExtension::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ExtInterfaceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ExtAbstractInterfaceDef
POA_CORBA::ExtAbstractInterfaceDef::~ExtAbstractInterfaceDef()
{
}

::CORBA::ExtAbstractInterfaceDef_ptr
POA_CORBA::ExtAbstractInterfaceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ExtAbstractInterfaceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ExtAbstractInterfaceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ExtInterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::AbstractInterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::InterfaceAttrExtension::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ExtAbstractInterfaceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ExtAbstractInterfaceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ExtAbstractInterfaceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ExtAbstractInterfaceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ExtAbstractInterfaceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ExtAbstractInterfaceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ExtInterfaceDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::AbstractInterfaceDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::InterfaceAttrExtension::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ExtAbstractInterfaceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ExtLocalInterfaceDef
POA_CORBA::ExtLocalInterfaceDef::~ExtLocalInterfaceDef()
{
}

::CORBA::ExtLocalInterfaceDef_ptr
POA_CORBA::ExtLocalInterfaceDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ExtLocalInterfaceDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ExtLocalInterfaceDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::ExtInterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::LocalInterfaceDef::_is_a (repoid)) {
    return TRUE;
  }
  if (POA_CORBA::InterfaceAttrExtension::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ExtLocalInterfaceDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ExtLocalInterfaceDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ExtLocalInterfaceDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ExtLocalInterfaceDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ExtLocalInterfaceDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ExtLocalInterfaceDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{

  if (POA_CORBA::ExtInterfaceDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::LocalInterfaceDef::dispatch (__req)) {
    return true;
  }

  if (POA_CORBA::InterfaceAttrExtension::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ExtLocalInterfaceDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


// PortableServer Skeleton Class for interface CORBA::ValueMemberDef
POA_CORBA::ValueMemberDef::~ValueMemberDef()
{
}

::CORBA::ValueMemberDef_ptr
POA_CORBA::ValueMemberDef::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::CORBA::ValueMemberDef::_narrow (obj);
}

CORBA::Boolean
POA_CORBA::ValueMemberDef::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:omg.org/CORBA/ValueMemberDef:1.0") == 0) {
    return TRUE;
  }
  if (POA_CORBA::Contained::_is_a (repoid)) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_CORBA::ValueMemberDef::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:omg.org/CORBA/ValueMemberDef:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_CORBA::ValueMemberDef::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:omg.org/CORBA/ValueMemberDef:1.0");
}

CORBA::Object_ptr
POA_CORBA::ValueMemberDef::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::CORBA::ValueMemberDef_stub_clp (poa, obj);
}

bool
POA_CORBA::ValueMemberDef::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 7)) {
    case 0:
      if( strcmp( __req->op_name(), "_set_access" ) == 0 ) {
        ::CORBA::Visibility _par__value;
        CORBA::StaticAny _sa__value( CORBA::_stc_short, &_par__value );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        access( _par__value );
        __req->write_results();
        return true;
      }
      break;
    case 2:
      if( strcmp( __req->op_name(), "_get_type_def" ) == 0 ) {
        ::CORBA::IDLType_ptr _res;
        CORBA::StaticAny __res( _marshaller_CORBA_IDLType, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = type_def();
        __req->write_results();
        CORBA::release( _res );
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "_get_access" ) == 0 ) {
        ::CORBA::Visibility _res;
        CORBA::StaticAny __res( CORBA::_stc_short, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = access();
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_set_type_def" ) == 0 ) {
        ::CORBA::IDLType_var _par__value;
        CORBA::StaticAny _sa__value( _marshaller_CORBA_IDLType, &_par__value._for_demarshal() );

        __req->add_in_arg( &_sa__value );

        if( !__req->read_args() )
          return true;

        type_def( _par__value.inout() );
        __req->write_results();
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "_get_type" ) == 0 ) {
        CORBA::TypeCode_ptr _res;
        CORBA::StaticAny __res( CORBA::_stc_TypeCode, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = type();
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

  if (POA_CORBA::Contained::dispatch (__req)) {
    return true;
  }

  return false;
}

void
POA_CORBA::ValueMemberDef::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}

