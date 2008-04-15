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

#ifndef __IR_IMPL_H__
#define __IR_IMPL_H__

#include <mico/ir.h>
#ifdef USE_CCM
#include <mico/ir3.h>
#endif

class IRObject_impl :
  virtual public POA_CORBA::IRObject
{
protected:
  CORBA::DefinitionKind _dk;
  CORBA::Boolean _already_destructed;

 public:
  static PortableServer::POA_var _ifrpoa;

public:
  IRObject_impl(CORBA::DefinitionKind kind);
  CORBA::DefinitionKind def_kind();

  virtual void deactivate() = 0;
  void destroy();
  PortableServer::POA_ptr _default_POA ();
};

class Container_impl;
class Repository_impl;

class Contained_impl :
  virtual public IRObject_impl,
  virtual public POA_CORBA::Contained
{
  friend class Container_impl;

protected:
  CORBA::RepositoryId_var _id;
  CORBA::Identifier_var   _name;
  CORBA::VersionSpec_var  _version;

  Container_impl * _mycontainer;
  Repository_impl * _myrepository;

public:
  Contained_impl (Container_impl * mycontainer,
		  Repository_impl * myrepository,
		  const char * id,
		  const char * name,
		  const char * version);
  CORBA::RepositoryId id();
  void id( const char* value );
  char* name();
  void name( const char* value );
  char* version();
  void version( const char* value );
  CORBA::Container_ptr defined_in();
  char* absolute_name();
  CORBA::Repository_ptr containing_repository();
  void move( CORBA::Container_ptr new_container,
	     const char* new_name,
	     const char* new_version );
  void deactivate ();
};


class Container_impl : 
  virtual public IRObject_impl,
  virtual public POA_CORBA::Container
{
protected:
  /*
   * I'd love to use map<string, Contained_impl*> here, but I can't.
   * At least ValueDef relies on Container::contents(dk_ValueMember)
   * to return a list of elements in their original order, and a map<>
   * does not maintain the order of insertion. Bummer.
   */

  struct _name_entry {
    std::string name;
    Contained_impl * value;
  };

  typedef std::list<_name_entry> NameMap;
  NameMap _names;

  /*
   * This is a hole in the spec, which doesn't mention any ordering
   * requirement for contents().
   *
   * It's not a problem in a MICO-only environment, but different ORBs
   * should probably agree on the order in which the elements are
   * marshalled.
   *
   * -- FP
   */

  Container_impl * _mycontainer;
  Repository_impl * _myrepository;

  static void merge (CORBA::ContainedSeq &, const CORBA::ContainedSeq &);
  static void merge_contents (CORBA::ContainedSeq &, CORBA::Container_ptr,
			      CORBA::DefinitionKind, CORBA::Boolean);
  static void merge_lookup   (CORBA::ContainedSeq &, CORBA::Container_ptr,
			      const char *, CORBA::Long,
			      CORBA::DefinitionKind, CORBA::Boolean);

public:
  Container_impl ();
  Container_impl (Container_impl * mycontainer,
		  Repository_impl * myrepository);

  virtual CORBA::Contained_ptr lookup( const char* search_name );
  virtual CORBA::ContainedSeq* contents( CORBA::DefinitionKind limit_type,
					 CORBA::Boolean exclude_inherited );
  virtual CORBA::ContainedSeq* lookup_name( const char* search_name,
					    CORBA::Long levels_to_search,
					    CORBA::DefinitionKind limit_type,
					    CORBA::Boolean exclude_inherited );
  CORBA::Container::DescriptionSeq* 
                     describe_contents( CORBA::DefinitionKind limit_type,
					CORBA::Boolean exclude_inherited,
					CORBA::Long max_returned_objs );
  CORBA::ModuleDef_ptr create_module( const char* id,
				      const char* name,
				      const char* version );
  CORBA::ConstantDef_ptr create_constant( const char* id,
					  const char* name,
					  const char* version,
					  CORBA::IDLType_ptr type,
					  const CORBA::Any& value );
  CORBA::ExceptionDef_ptr create_exception( const char* id,
					    const char* name,
					    const char* version,
					    const CORBA::StructMemberSeq& members );
  CORBA::StructDef_ptr create_struct( const char* id,
				      const char* name,
				      const char* version,
				      const CORBA::StructMemberSeq& members );
  CORBA::UnionDef_ptr create_union( const char* id,
				    const char* name,
				    const char* version,
				    CORBA::IDLType_ptr discriminator_type,
				    const CORBA::UnionMemberSeq& members );
  CORBA::EnumDef_ptr create_enum( const char* id,
				  const char* name,
				  const char* version,
				  const CORBA::EnumMemberSeq& members );
  CORBA::AliasDef_ptr create_alias( const char* id,
				    const char* name,
				    const char* version,
				    CORBA::IDLType_ptr original_type );
  CORBA::InterfaceDef_ptr
    create_interface( const char* id,
		      const char* name,
		      const char* version,
		      const CORBA::InterfaceDefSeq& base_interfaces);

  CORBA::AbstractInterfaceDef_ptr
    create_abstract_interface( const char* id,
			       const char* name,
			       const char* version,
			       const CORBA::AbstractInterfaceDefSeq& base_interfaces);

  CORBA::LocalInterfaceDef_ptr
    create_local_interface( const char* id,
			    const char* name,
			    const char* version,
			    const CORBA::InterfaceDefSeq& base_interfaces);

  CORBA::ValueDef_ptr create_value( const char* id,
				    const char* name,
				    const char* version,
				    CORBA::Boolean is_custom,
				    CORBA::Boolean is_abstract,
				    CORBA::ValueDef_ptr base_value,
				    CORBA::Boolean is_truncatable,
				    const CORBA::ValueDefSeq& abstract_base_values,
				    const CORBA::InterfaceDefSeq& supported_interfaces,
				    const CORBA::InitializerSeq& initializers );

  CORBA::ExtValueDef_ptr
    create_ext_value( const char* id,
		      const char* name,
		      const char* version,
		      CORBA::Boolean is_custom,
		      CORBA::Boolean is_abstract,
		      CORBA::ValueDef_ptr base_value,
		      CORBA::Boolean is_truncatable,
		      const CORBA::ValueDefSeq& abstract_base_values,
		      const CORBA::InterfaceDefSeq& supported_interfaces,
		      const CORBA::ExtInitializerSeq& initializers );

  CORBA::ValueBoxDef_ptr create_value_box( const char* id,
					   const char* name,
					   const char* version,
					   CORBA::IDLType_ptr original_type_def );

  CORBA::NativeDef_ptr create_native( const char* id,
				      const char* name,
				      const char* version );

  /*
   * local interface
   */

  void insert_contained (Contained_impl *,
			 const char * id,
			 const char * name);
  void register_name (const char * name, Contained_impl *);
  void unregister_name (const char * name);
  void deactivate ();
};

#ifdef USE_CCM

class CompContainer_impl :
  virtual public Container_impl,
  virtual public POA_CORBA::ComponentIR::Container
{
public:
  CompContainer_impl ();
  CompContainer_impl (Container_impl * mycontainer,
		      Repository_impl * myrepository);

  CORBA::ComponentIR::ComponentDef_ptr
    create_component (const char * id,
		      const char * name,
		      const char * version,
		      CORBA::ComponentIR::ComponentDef_ptr base_component,
		      const CORBA::InterfaceDefSeq & supports_interfaces);

  CORBA::ComponentIR::HomeDef_ptr
    create_home (const char * id,
		 const char * name,
		 const char * version,
		 CORBA::ComponentIR::HomeDef_ptr base_home,
		 CORBA::ComponentIR::ComponentDef_ptr managed_component,
		 const CORBA::InterfaceDefSeq & supports_interfaces,
		 CORBA::ValueDef_ptr primary_key);

  CORBA::ComponentIR::EventDef_ptr
    create_event( const char* id,
		  const char* name,
		  const char* version,
		  CORBA::Boolean is_custom,
		  CORBA::Boolean is_abstract,
		  CORBA::ValueDef_ptr base_value,
		  CORBA::Boolean is_truncatable,
		  const CORBA::ValueDefSeq& abstract_base_values,
		  const CORBA::InterfaceDefSeq& supported_interfaces,
		  const CORBA::ExtInitializerSeq& initializers );
};

#endif

class IDLType_impl : 
  virtual public IRObject_impl,
  virtual public POA_CORBA::IDLType
{
protected:
  CORBA::TypeCode_var _type;
  void check_for_bad_recursion ( CORBA::IDLType_ptr );
  
public:
  IDLType_impl();
  IDLType_impl( CORBA::TypeCode_ptr type );
  CORBA::TypeCode_ptr type();
  virtual CORBA::TypeCode_ptr recursive_type (); // local use only
};


class Repository_impl : 
#ifdef USE_CCM
  virtual public CompContainer_impl,
  virtual public POA_CORBA::ComponentIR::Repository
#else
  virtual public Container_impl,
  virtual public POA_CORBA::Repository
#endif
{
protected:
  CORBA::PrimitiveDef_var _pk_void;
  CORBA::PrimitiveDef_var _pk_float;
  CORBA::PrimitiveDef_var _pk_double;
  CORBA::PrimitiveDef_var _pk_long;
  CORBA::PrimitiveDef_var _pk_short;
  CORBA::PrimitiveDef_var _pk_ulong;
  CORBA::PrimitiveDef_var _pk_ushort;
  CORBA::PrimitiveDef_var _pk_char;
  CORBA::PrimitiveDef_var _pk_boolean;
  CORBA::PrimitiveDef_var _pk_octet;
  CORBA::PrimitiveDef_var _pk_string;
  CORBA::PrimitiveDef_var _pk_any;
  CORBA::PrimitiveDef_var _pk_objref;
  CORBA::PrimitiveDef_var _pk_typecode;
  CORBA::PrimitiveDef_var _pk_principal;
  CORBA::PrimitiveDef_var _pk_longlong;
  CORBA::PrimitiveDef_var _pk_ulonglong;
  CORBA::PrimitiveDef_var _pk_longdouble;
  CORBA::PrimitiveDef_var _pk_wchar;
  CORBA::PrimitiveDef_var _pk_wstring;
  CORBA::PrimitiveDef_var _pk_valuebase;

  typedef SequenceTmpl<CORBA::IDLType_var,MICO_TID_DEF> IDLTypeSeq;
  IDLTypeSeq _anonymous_types;
  
  void add_anonymous_type( CORBA::IDLType_ptr type );

  typedef std::map<std::string, Contained_impl *, std::less<std::string> > RepoIdMap;
  RepoIdMap _repoids;

public:
  Repository_impl ();

  CORBA::Contained_ptr lookup_id( const char* search_id );
  CORBA::PrimitiveDef_ptr get_primitive( CORBA::PrimitiveKind kind );
  CORBA::StringDef_ptr create_string( CORBA::ULong bound );
  CORBA::WstringDef_ptr create_wstring( CORBA::ULong bound );
  CORBA::SequenceDef_ptr create_sequence( CORBA::ULong bound,
  					  CORBA::IDLType_ptr element_type );
  CORBA::ArrayDef_ptr create_array( CORBA::ULong length,
  				    CORBA::IDLType_ptr element_type );
  CORBA::FixedDef_ptr create_fixed( CORBA::UShort digits,
  				    CORBA::Short scale );

  /*
   * local interface
   */

  void register_repoid (const char * repoid, Contained_impl * contained);
  void unregister_repoid (const char * repoid);
  void deactivate ();
  void destroy ();
};


class ModuleDef_impl : 
  virtual public Contained_impl,
#ifdef USE_CCM
  virtual public CompContainer_impl,
  virtual public POA_CORBA::ComponentIR::ModuleDef
#else
  virtual public Container_impl,
  virtual public POA_CORBA::ModuleDef
#endif
{
public:
  ModuleDef_impl( Container_impl * mycontainer,
		  Repository_impl * myrepository,
		  const char * id,
		  const char * name,
		  const char * version);
  CORBA::Contained::Description * describe ();
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
#ifdef USE_CCM
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, ModuleDef)
#else
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ModuleDef)
#endif
#endif
};


class ConstantDef_impl : 
  virtual public Contained_impl,
  virtual public POA_CORBA::ConstantDef
{
protected:
  CORBA::IDLType_var _type_def;
  CORBA::Any         _value;
public:
  ConstantDef_impl( Container_impl * mycontainer,
		    Repository_impl * myrepository,
		    const char * id,
		    const char * name,
		    const char * version);
  CORBA::TypeCode_ptr type();
  CORBA::IDLType_ptr type_def();
  void type_def( CORBA::IDLType_ptr value );
  CORBA::Any* value();
  void value( const CORBA::Any& value );
  CORBA::Contained::Description * describe ();
};



class TypedefDef_impl : 
  virtual public Contained_impl,
  virtual public IDLType_impl,
  virtual public POA_CORBA::TypedefDef
{
public:
  TypedefDef_impl( Container_impl * mycontainer,
		   Repository_impl * myrepository,
		   const char * id,
		   const char * name,
		   const char * version);
  CORBA::Contained::Description * describe ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, TypedefDef)
#endif
};



class StructDef_impl : 
  virtual public TypedefDef_impl,
  virtual public Container_impl,
  virtual public POA_CORBA::StructDef
{
protected:
  CORBA::StructMemberSeq _members;
  CORBA::Boolean _typedirty;
  CORBA::Boolean _visited;

public:
  StructDef_impl( Container_impl * mycontainer,
		  Repository_impl * myrepository,
		  const char * id,
		  const char * name,
		  const char * version);
  CORBA::StructMemberSeq* members();
  void members( const CORBA::StructMemberSeq& value );
  CORBA::TypeCode_ptr type();
  CORBA::TypeCode_ptr recursive_type();
  void deactivate();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, StructDef)
#endif
};



class UnionDef_impl : 
  virtual public TypedefDef_impl,
  virtual public Container_impl,
  virtual public POA_CORBA::UnionDef
{
protected:
  CORBA::IDLType_var    _discr;
  CORBA::UnionMemberSeq _members;
  CORBA::Boolean _typedirty;
  CORBA::Boolean _visited;

private:
  void check_explicit_default_case( const CORBA::UnionMemberSeq &mem );
  
public:
  UnionDef_impl( Container_impl * mycontainer,
		 Repository_impl * myrepository,
		 const char * id,
		 const char * name,
		 const char * version);
  CORBA::TypeCode_ptr discriminator_type();
  CORBA::IDLType_ptr discriminator_type_def();
  void discriminator_type_def( CORBA::IDLType_ptr value );
  CORBA::UnionMemberSeq* members();
  void members( const CORBA::UnionMemberSeq& value );
  CORBA::TypeCode_ptr type();
  CORBA::TypeCode_ptr recursive_type();
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, UnionDef)
#endif
};



class EnumDef_impl : 
  virtual public TypedefDef_impl,
  virtual public POA_CORBA::EnumDef
{
protected:
  CORBA::EnumMemberSeq _members;
public:
  EnumDef_impl( Container_impl * mycontainer,
		Repository_impl * myrepository,
		const char * id,
		const char * name,
		const char * version);
  CORBA::EnumMemberSeq* members();
  void members( const CORBA::EnumMemberSeq& value );
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, EnumDef)
#endif
};



class AliasDef_impl : 
  virtual public TypedefDef_impl,
  virtual public POA_CORBA::AliasDef
{
protected:
  CORBA::IDLType_var _original;

public:
  AliasDef_impl( Container_impl * mycontainer,
		 Repository_impl * myrepository,
		 const char * id,
		 const char * name,
		 const char * version);
  CORBA::IDLType_ptr original_type_def();
  void original_type_def( CORBA::IDLType_ptr value );
  CORBA::TypeCode_ptr type();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, AliasDef)
#endif
};



class NativeDef_impl : 
  virtual public TypedefDef_impl,
  virtual public POA_CORBA::NativeDef
{
public:
  NativeDef_impl( Container_impl * mycontainer,
		  Repository_impl * myrepository,
		  const char * id,
		  const char * name,
		  const char * version);
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, NativeDef)
#endif
};



class PrimitiveDef_impl : 
  virtual public IDLType_impl,
  virtual public POA_CORBA::PrimitiveDef
{
protected:
  CORBA::PrimitiveKind _kind;
public:
  PrimitiveDef_impl( CORBA::PrimitiveKind kind );
  CORBA::PrimitiveKind kind();
  void deactivate ();
  void destroy ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, PrimitiveDef)
#endif
};



class StringDef_impl : 
  virtual public IDLType_impl,
  virtual public POA_CORBA::StringDef
{
protected:
  CORBA::ULong _bound;
public:
  StringDef_impl();
  CORBA::ULong bound();
  void bound( CORBA::ULong value );
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, StringDef)
#endif
};



class WstringDef_impl : 
  virtual public IDLType_impl,
  virtual public POA_CORBA::WstringDef
{
protected:
  CORBA::ULong _bound;
public:
  WstringDef_impl();
  CORBA::ULong bound();
  void bound( CORBA::ULong value );
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, WstringDef)
#endif
};



class FixedDef_impl : 
  virtual public IDLType_impl,
  virtual public POA_CORBA::FixedDef
{
protected:
  CORBA::UShort _digits;
  CORBA::Short  _scale;
public:
  FixedDef_impl();
  CORBA::UShort digits();
  void digits( CORBA::UShort value );
  CORBA::Short scale();
  void scale( CORBA::Short value );
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, FixedDef)
#endif
};



class SequenceDef_impl : 
  virtual public IDLType_impl,
  virtual public POA_CORBA::SequenceDef
{
protected:
  CORBA::ULong        _bound;
  CORBA::IDLType_var  _element_type_def;

public:
  SequenceDef_impl();
  CORBA::ULong bound();
  void bound( CORBA::ULong value );
  CORBA::TypeCode_ptr element_type();
  CORBA::IDLType_ptr element_type_def();
  void element_type_def( CORBA::IDLType_ptr value );
  CORBA::TypeCode_ptr type();
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, SequenceDef)
#endif
};



class ArrayDef_impl : 
  virtual public IDLType_impl,
  virtual public POA_CORBA::ArrayDef
{
protected:
  CORBA::ULong        _length;
  CORBA::IDLType_var  _element_type_def;

public:
  ArrayDef_impl();
  CORBA::ULong length();
  void length( CORBA::ULong value );
  CORBA::TypeCode_ptr element_type();
  CORBA::IDLType_ptr element_type_def();
  void element_type_def( CORBA::IDLType_ptr value );
  CORBA::TypeCode_ptr type();
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ArrayDef)
#endif
};



class ExceptionDef_impl : 
  virtual public Contained_impl,
  virtual public Container_impl,
  virtual public POA_CORBA::ExceptionDef
{
protected:
  CORBA::StructMemberSeq            _members;
  CORBA::TypeCode_var               _type;

public:
  ExceptionDef_impl( Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);
  CORBA::StructMemberSeq* members();
  void members( const CORBA::StructMemberSeq& value );
  CORBA::TypeCode_ptr type();
  CORBA::Contained::Description * describe ();
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ExceptionDef)
#endif
};



class AttributeDef_impl : 
  virtual public Contained_impl,
  virtual public POA_CORBA::ExtAttributeDef
{
protected:
  CORBA::IDLType_var     _type_def;
  CORBA::AttributeMode   _mode;
  CORBA::ExceptionDefSeq _get_exceptions;
  CORBA::ExceptionDefSeq _set_exceptions;
  
public:
  AttributeDef_impl( Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);
  CORBA::TypeCode_ptr type();
  CORBA::IDLType_ptr type_def();
  void type_def( CORBA::IDLType_ptr value );
  CORBA::AttributeMode mode();
  void mode( CORBA::AttributeMode value );
  CORBA::Contained::Description * describe ();

  CORBA::ExceptionDefSeq * get_exceptions ();
  void get_exceptions (const CORBA::ExceptionDefSeq &);

  CORBA::ExceptionDefSeq * set_exceptions ();
  void set_exceptions (const CORBA::ExceptionDefSeq &);

  CORBA::ExtAttributeDescription * describe_attribute ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ExtAttributeDef)
#endif
};


class OperationDef_impl : 
  virtual public Contained_impl,
  virtual public POA_CORBA::OperationDef
{
protected:
  CORBA::IDLType_var       _result_def;
  CORBA::ParDescriptionSeq _params;
  CORBA::OperationMode     _mode;
  CORBA::ExceptionDefSeq   _exceptions;
  CORBA::ContextIdSeq      _contexts;
  
public:
  OperationDef_impl( Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);
  CORBA::TypeCode_ptr result();
  CORBA::IDLType_ptr result_def();
  void result_def( CORBA::IDLType_ptr value );
  CORBA::ParDescriptionSeq* params();
  void params( const CORBA::ParDescriptionSeq& value );
  CORBA::OperationMode mode();
  void mode( CORBA::OperationMode value );
  CORBA::ContextIdSeq* contexts();
  void contexts( const CORBA::ContextIdSeq& value );
  CORBA::ExceptionDefSeq* exceptions();
  void exceptions( const CORBA::ExceptionDefSeq& value );
  CORBA::Contained::Description * describe ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, OperationDef)
#endif
};



class InterfaceDef_impl : 
  virtual public Container_impl,
  virtual public Contained_impl,
  virtual public IDLType_impl,
  virtual public POA_CORBA::ExtInterfaceDef
{
protected:
  CORBA::InterfaceDefSeq _base_interfaces;
  
public:
  InterfaceDef_impl();
  InterfaceDef_impl( Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);

  CORBA::InterfaceDefSeq* base_interfaces();
  void base_interfaces( const CORBA::InterfaceDefSeq& value );
  CORBA::Boolean is_a( const char* interface_id );
  CORBA::InterfaceDef::FullInterfaceDescription* describe_interface();
  CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
    describe_ext_interface ();
  CORBA::AttributeDef_ptr create_attribute( const char* id,
					    const char* name,
					    const char* version,
					    CORBA::IDLType_ptr type,
					    CORBA::AttributeMode mode );
  CORBA::OperationDef_ptr create_operation( const char* id,
					    const char* name,
					    const char* version,
					    CORBA::IDLType_ptr result,
					    CORBA::OperationMode mode,
					    const CORBA::ParDescriptionSeq& params,
					    const CORBA::ExceptionDefSeq& exceptions,
					    const CORBA::ContextIdSeq& contexts );

  CORBA::ExtAttributeDef_ptr
    create_ext_attribute ( const char* id,
			   const char* name,
			   const char* version,
			   CORBA::IDLType_ptr type,
			   CORBA::AttributeMode mode,
			   const CORBA::ExceptionDefSeq & get_exceptions,
			   const CORBA::ExceptionDefSeq & set_exceptions
			   );


  CORBA::Contained::Description * describe ();
  CORBA::TypeCode_ptr type();
  void deactivate ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ExtInterfaceDef)
#endif
};

class AbstractInterfaceDef_impl :
  virtual public InterfaceDef_impl,
  virtual public POA_CORBA::ExtAbstractInterfaceDef
{
public:
  AbstractInterfaceDef_impl( Container_impl * mycontainer,
			     Repository_impl * myrepository,
			     const char * id,
			     const char * name,
			     const char * version);
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ExtAbstractInterfaceDef)
#endif
};

class LocalInterfaceDef_impl :
  virtual public InterfaceDef_impl,
  virtual public POA_CORBA::ExtLocalInterfaceDef
{
public:
  LocalInterfaceDef_impl( Container_impl * mycontainer,
			  Repository_impl * myrepository,
			  const char * id,
			  const char * name,
			  const char * version);
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ExtLocalInterfaceDef)
#endif
};

class ValueMemberDef_impl : 
  virtual public Contained_impl,
  virtual public POA_CORBA::ValueMemberDef
{
protected:
  CORBA::IDLType_var _type_def;
  CORBA::Visibility _access;

public:
  ValueMemberDef_impl ( Container_impl * mycontainer,
			Repository_impl * myrepository,
			const char* id,
			const char* name,
			const char* version );
  CORBA::TypeCode_ptr type();
  CORBA::IDLType_ptr type_def();
  void type_def( CORBA::IDLType_ptr _new_value );
  CORBA::Visibility access();
  void access( CORBA::Visibility _new_value );
  CORBA::Contained::Description * describe ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ValueMemberDef)
#endif
};


class ValueDef_impl : 
  virtual public Container_impl,
  virtual public Contained_impl,
  virtual public IDLType_impl,
  virtual public POA_CORBA::ExtValueDef
{
protected:
  CORBA::Boolean _is_custom;
  CORBA::Boolean _is_abstract;
  CORBA::ValueDef_var _base_value;
  CORBA::Boolean _is_truncatable;
  CORBA::ValueDefSeq _abstract_base_values;
  CORBA::InterfaceDefSeq _supported_interfaces;
  CORBA::ExtInitializerSeq _initializers;
  CORBA::Boolean _typedirty;
  CORBA::Boolean _visited;

public:
  ValueDef_impl ( Container_impl * mycontainer,
		  Repository_impl * myrepository,
		  const char* id,
		  const char* name,
		  const char* version );
  CORBA::InterfaceDefSeq* supported_interfaces();
  void supported_interfaces( const CORBA::InterfaceDefSeq& _new_value );
  CORBA::InitializerSeq* initializers();
  void initializers( const CORBA::InitializerSeq& _new_value );
  CORBA::ValueDef_ptr base_value();
  void base_value( CORBA::ValueDef_ptr _new_value );
  CORBA::ValueDefSeq* abstract_base_values();
  void abstract_base_values( const CORBA::ValueDefSeq& _new_value );
  CORBA::Boolean is_abstract();
  void is_abstract( CORBA::Boolean _new_value );
  CORBA::Boolean is_custom();
  void is_custom( CORBA::Boolean _new_value );
  CORBA::Boolean is_truncatable();
  void is_truncatable( CORBA::Boolean _new_value );

  CORBA::ExtInitializerSeq * ext_initializers ();
  void ext_initializers (const CORBA::ExtInitializerSeq &);

  CORBA::Boolean is_a( const char* value_id );

  CORBA::ValueDef::FullValueDescription* describe_value();
  CORBA::ExtValueDef::ExtFullValueDescription * describe_ext_value();

  CORBA::ValueMemberDef_ptr create_value_member( const char* id,
						 const char* name,
						 const char* version,
						 CORBA::IDLType_ptr type,
						 CORBA::Visibility access );

  CORBA::AttributeDef_ptr create_attribute( const char* id,
					    const char* name,
					    const char* version,
					    CORBA::IDLType_ptr type,
					    CORBA::AttributeMode mode );

  CORBA::OperationDef_ptr create_operation( const char* id,
					    const char* name,
					    const char* version,
					    CORBA::IDLType_ptr result,
					    CORBA::OperationMode mode,
					    const CORBA::ParDescriptionSeq& params,
					    const CORBA::ExceptionDefSeq& exceptions,
					    const CORBA::ContextIdSeq& contexts );

  CORBA::ExtAttributeDef_ptr
    create_ext_attribute ( const char* id,
			   const char* name,
			   const char* version,
			   CORBA::IDLType_ptr type,
			   CORBA::AttributeMode mode,
			   const CORBA::ExceptionDefSeq & get_exceptions,
			   const CORBA::ExceptionDefSeq & set_exceptions
			   );

  CORBA::TypeCode_ptr type();
  CORBA::TypeCode_ptr recursive_type();
  CORBA::Contained::Description * describe ();
  void deactivate ();
#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ExtValueDef)
#endif
};


class ValueBoxDef_impl : 
  virtual public TypedefDef_impl,
  virtual public POA_CORBA::ValueBoxDef
{
protected:
  CORBA::IDLType_var _original_type_def;
  CORBA::Boolean _typedirty;

public:
  ValueBoxDef_impl ( Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char* id,
		     const char* name,
		     const char* version );
  CORBA::IDLType_ptr original_type_def();
  void original_type_def( CORBA::IDLType_ptr _new_value );
  CORBA::TypeCode_ptr type();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA, ValueBoxDef)
#endif
};

/*
 * ----------------------------------------------------------------------
 * ComponentIR
 * ----------------------------------------------------------------------
 */

#ifdef USE_CCM

class ProvidesDef_impl :
  virtual public Contained_impl,
  virtual public POA_CORBA::ComponentIR::ProvidesDef
{
protected:
  CORBA::InterfaceDef_var _interface_type;
  
public:
  ProvidesDef_impl (Container_impl * mycontainer,
		    Repository_impl * myrepository,
		    const char * id,
		    const char * name,
		    const char * version);

  CORBA::InterfaceDef_ptr interface_type ();
  void interface_type (CORBA::InterfaceDef_ptr);
  CORBA::Contained::Description * describe ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, ProvidesDef)
#endif
};

class UsesDef_impl :
  virtual public Contained_impl,
  virtual public POA_CORBA::ComponentIR::UsesDef
{
protected:
  CORBA::InterfaceDef_var _interface_type;
  CORBA::Boolean _is_multiple;

public:
  UsesDef_impl (Container_impl * mycontainer,
		Repository_impl * myrepository,
		const char * id,
		const char * name,
		const char * version);

  CORBA::InterfaceDef_ptr interface_type ();
  void interface_type (CORBA::InterfaceDef_ptr);
  CORBA::Boolean is_multiple ();
  void is_multiple (CORBA::Boolean);
  CORBA::Contained::Description * describe ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, UsesDef)
#endif
};

class EventDef_impl :
  virtual public ValueDef_impl,
  virtual public POA_CORBA::ComponentIR::EventDef
{
public:
  EventDef_impl (Container_impl * mycontainer,
		 Repository_impl * myrepository,
		 const char * id,
		 const char * name,
		 const char * version);

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, EventDef)
#endif
};

class EventPortDef_impl :
  virtual public Contained_impl,
  virtual public POA_CORBA::ComponentIR::EventPortDef
{
protected:
  CORBA::ComponentIR::EventDef_var _event;

public:
  EventPortDef_impl (Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);

  CORBA::Boolean is_a (const char * event_id);
  CORBA::ComponentIR::EventDef_ptr event ();
  void event (CORBA::ComponentIR::EventDef_ptr);
  CORBA::Contained::Description * describe ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, EventPortDef)
#endif
};

class EmitsDef_impl :
  virtual public EventPortDef_impl,
  virtual public POA_CORBA::ComponentIR::EmitsDef
{
public:
  EmitsDef_impl (Container_impl * mycontainer,
		 Repository_impl * myrepository,
		 const char * id,
		 const char * name,
		 const char * version);

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, EmitsDef)
#endif
};

class PublishesDef_impl :
  virtual public EventPortDef_impl,
  virtual public POA_CORBA::ComponentIR::PublishesDef
{
public:
  PublishesDef_impl (Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, PublishesDef)
#endif
};

class ConsumesDef_impl :
  virtual public EventPortDef_impl,
  virtual public POA_CORBA::ComponentIR::ConsumesDef
{
public:
  ConsumesDef_impl (Container_impl * mycontainer,
		    Repository_impl * myrepository,
		    const char * id,
		    const char * name,
		    const char * version);

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, ConsumesDef)
#endif
};

class ComponentDef_impl :
  virtual public InterfaceDef_impl,
  virtual public POA_CORBA::ComponentIR::ComponentDef
{
 private:
  CORBA::InterfaceDefSeq _supported_interfaces;
  CORBA::ComponentIR::ComponentDef_var _base_component;

public:
  ComponentDef_impl (Container_impl * mycontainer,
		     Repository_impl * myrepository,
		     const char * id,
		     const char * name,
		     const char * version);

  CORBA::InterfaceDefSeq * supported_interfaces ();
  void supported_interfaces (const CORBA::InterfaceDefSeq & the_supported);

  CORBA::ComponentIR::ComponentDef_ptr base_component ();
  void base_component (CORBA::ComponentIR::ComponentDef_ptr);

  CORBA::ComponentIR::ProvidesDef_ptr
    create_provides (const char * id,
		     const char * name,
		     const char * version,
		     CORBA::InterfaceDef_ptr interface_type);

  CORBA::ComponentIR::UsesDef_ptr
    create_uses (const char * di,
		 const char * name,
		 const char * version,
		 CORBA::InterfaceDef_ptr interface_type,
		 CORBA::Boolean is_multiple);

  CORBA::ComponentIR::EmitsDef_ptr
    create_emits (const char * id,
		  const char * name,
		  const char * version,
		  CORBA::ComponentIR::EventDef_ptr event);

  CORBA::ComponentIR::PublishesDef_ptr
    create_publishes (const char * id,
		      const char * name,
		      const char * version,
		      CORBA::ComponentIR::EventDef_ptr event);

  CORBA::ComponentIR::ConsumesDef_ptr
    create_consumes (const char * id,
		     const char * name,
		     const char * version,
		     CORBA::ComponentIR::EventDef_ptr event);

  CORBA::Contained::Description * describe ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, ComponentDef)
#endif
};

class FactoryDef_impl :
  virtual public OperationDef_impl,
  virtual public POA_CORBA::ComponentIR::FactoryDef
{
public:
  FactoryDef_impl (Container_impl * mycontainer,
		   Repository_impl * myrepository,
		   const char * id,
		   const char * name,
		   const char * version);

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, FactoryDef)
#endif
};

class FinderDef_impl :
  virtual public OperationDef_impl,
  virtual public POA_CORBA::ComponentIR::FinderDef
{
public:
  FinderDef_impl (Container_impl * mycontainer,
		  Repository_impl * myrepository,
		  const char * id,
		  const char * name,
		  const char * version);

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, FinderDef)
#endif
};

class HomeDef_impl :
  virtual public InterfaceDef_impl,
  virtual public POA_CORBA::ComponentIR::HomeDef
{
protected:
  CORBA::ComponentIR::HomeDef_var _base_home;
  CORBA::ComponentIR::ComponentDef_var _managed_component;
  CORBA::InterfaceDefSeq _supported_interfaces;
  CORBA::ValueDef_var _primary_key;

public:
  HomeDef_impl (Container_impl * mycontainer,
		Repository_impl * myrepository,
		const char * id,
		const char * name,
		const char * version);

  CORBA::ComponentIR::HomeDef_ptr base_home ();
  void base_home (CORBA::ComponentIR::HomeDef_ptr);

  CORBA::InterfaceDefSeq * supported_interfaces ();
  void supported_interfaces (const CORBA::InterfaceDefSeq & the_supported);

  CORBA::ComponentIR::ComponentDef_ptr managed_component ();
  void managed_component (CORBA::ComponentIR::ComponentDef_ptr);

  CORBA::ValueDef_ptr primary_key ();
  void primary_key (CORBA::ValueDef_ptr);

  CORBA::ComponentIR::FactoryDef_ptr
    create_factory (const char * id,
		    const char * name,
		    const char * version,
		    const CORBA::ParDescriptionSeq & params,
		    const CORBA::ExceptionDefSeq & exceptions);

  CORBA::ComponentIR::FinderDef_ptr
    create_finder (const char * id,
		   const char * name,
		   const char * version,
		   const CORBA::ParDescriptionSeq & params,
		   const CORBA::ExceptionDefSeq & exceptions);

  CORBA::Contained::Description * describe ();

#ifdef HAVE_EXPLICIT_METHOD_OVERRIDE
  MICO_MAKE_POA_OVERRIDE_METHODS (CORBA::ComponentIR, HomeDef)
#endif
};

#endif // USE_CCM

#endif
