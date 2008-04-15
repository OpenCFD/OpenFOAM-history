/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __IR_H__
#define __IR_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace CORBA
{

class Container;
typedef Container *Container_ptr;
typedef Container_ptr ContainerRef;
typedef ObjVar< Container > Container_var;
typedef ObjOut< Container > Container_out;

class Repository;
typedef Repository *Repository_ptr;
typedef Repository_ptr RepositoryRef;
typedef ObjVar< Repository > Repository_var;
typedef ObjOut< Repository > Repository_out;

class Contained;
typedef Contained *Contained_ptr;
typedef Contained_ptr ContainedRef;
typedef ObjVar< Contained > Contained_var;
typedef ObjOut< Contained > Contained_out;

class ModuleDef;
typedef ModuleDef *ModuleDef_ptr;
typedef ModuleDef_ptr ModuleDefRef;
typedef ObjVar< ModuleDef > ModuleDef_var;
typedef ObjOut< ModuleDef > ModuleDef_out;

class ConstantDef;
typedef ConstantDef *ConstantDef_ptr;
typedef ConstantDef_ptr ConstantDefRef;
typedef ObjVar< ConstantDef > ConstantDef_var;
typedef ObjOut< ConstantDef > ConstantDef_out;

class StructDef;
typedef StructDef *StructDef_ptr;
typedef StructDef_ptr StructDefRef;
typedef ObjVar< StructDef > StructDef_var;
typedef ObjOut< StructDef > StructDef_out;

class ExceptionDef;
typedef ExceptionDef *ExceptionDef_ptr;
typedef ExceptionDef_ptr ExceptionDefRef;
typedef ObjVar< ExceptionDef > ExceptionDef_var;
typedef ObjOut< ExceptionDef > ExceptionDef_out;

class UnionDef;
typedef UnionDef *UnionDef_ptr;
typedef UnionDef_ptr UnionDefRef;
typedef ObjVar< UnionDef > UnionDef_var;
typedef ObjOut< UnionDef > UnionDef_out;

class EnumDef;
typedef EnumDef *EnumDef_ptr;
typedef EnumDef_ptr EnumDefRef;
typedef ObjVar< EnumDef > EnumDef_var;
typedef ObjOut< EnumDef > EnumDef_out;

class AliasDef;
typedef AliasDef *AliasDef_ptr;
typedef AliasDef_ptr AliasDefRef;
typedef ObjVar< AliasDef > AliasDef_var;
typedef ObjOut< AliasDef > AliasDef_out;

class InterfaceDef;
//typedef InterfaceDef *InterfaceDef_ptr;
typedef InterfaceDef_ptr InterfaceDefRef;
typedef ObjVar< InterfaceDef > InterfaceDef_var;
typedef ObjOut< InterfaceDef > InterfaceDef_out;

class AbstractInterfaceDef;
typedef AbstractInterfaceDef *AbstractInterfaceDef_ptr;
typedef AbstractInterfaceDef_ptr AbstractInterfaceDefRef;
typedef ObjVar< AbstractInterfaceDef > AbstractInterfaceDef_var;
typedef ObjOut< AbstractInterfaceDef > AbstractInterfaceDef_out;

class LocalInterfaceDef;
typedef LocalInterfaceDef *LocalInterfaceDef_ptr;
typedef LocalInterfaceDef_ptr LocalInterfaceDefRef;
typedef ObjVar< LocalInterfaceDef > LocalInterfaceDef_var;
typedef ObjOut< LocalInterfaceDef > LocalInterfaceDef_out;

class ValueDef;
//typedef ValueDef *ValueDef_ptr;
typedef ValueDef_ptr ValueDefRef;
typedef ObjVar< ValueDef > ValueDef_var;
typedef ObjOut< ValueDef > ValueDef_out;

class ExtValueDef;
typedef ExtValueDef *ExtValueDef_ptr;
typedef ExtValueDef_ptr ExtValueDefRef;
typedef ObjVar< ExtValueDef > ExtValueDef_var;
typedef ObjOut< ExtValueDef > ExtValueDef_out;

class ValueBoxDef;
typedef ValueBoxDef *ValueBoxDef_ptr;
typedef ValueBoxDef_ptr ValueBoxDefRef;
typedef ObjVar< ValueBoxDef > ValueBoxDef_var;
typedef ObjOut< ValueBoxDef > ValueBoxDef_out;

class NativeDef;
typedef NativeDef *NativeDef_ptr;
typedef NativeDef_ptr NativeDefRef;
typedef ObjVar< NativeDef > NativeDef_var;
typedef ObjOut< NativeDef > NativeDef_out;

class PrimitiveDef;
typedef PrimitiveDef *PrimitiveDef_ptr;
typedef PrimitiveDef_ptr PrimitiveDefRef;
typedef ObjVar< PrimitiveDef > PrimitiveDef_var;
typedef ObjOut< PrimitiveDef > PrimitiveDef_out;

class StringDef;
typedef StringDef *StringDef_ptr;
typedef StringDef_ptr StringDefRef;
typedef ObjVar< StringDef > StringDef_var;
typedef ObjOut< StringDef > StringDef_out;

class WstringDef;
typedef WstringDef *WstringDef_ptr;
typedef WstringDef_ptr WstringDefRef;
typedef ObjVar< WstringDef > WstringDef_var;
typedef ObjOut< WstringDef > WstringDef_out;

class SequenceDef;
typedef SequenceDef *SequenceDef_ptr;
typedef SequenceDef_ptr SequenceDefRef;
typedef ObjVar< SequenceDef > SequenceDef_var;
typedef ObjOut< SequenceDef > SequenceDef_out;

class ArrayDef;
typedef ArrayDef *ArrayDef_ptr;
typedef ArrayDef_ptr ArrayDefRef;
typedef ObjVar< ArrayDef > ArrayDef_var;
typedef ObjOut< ArrayDef > ArrayDef_out;

class FixedDef;
typedef FixedDef *FixedDef_ptr;
typedef FixedDef_ptr FixedDefRef;
typedef ObjVar< FixedDef > FixedDef_var;
typedef ObjOut< FixedDef > FixedDef_out;

class TypedefDef;
typedef TypedefDef *TypedefDef_ptr;
typedef TypedefDef_ptr TypedefDefRef;
typedef ObjVar< TypedefDef > TypedefDef_var;
typedef ObjOut< TypedefDef > TypedefDef_out;

class AttributeDef;
typedef AttributeDef *AttributeDef_ptr;
typedef AttributeDef_ptr AttributeDefRef;
typedef ObjVar< AttributeDef > AttributeDef_var;
typedef ObjOut< AttributeDef > AttributeDef_out;

class OperationDef;
//typedef OperationDef *OperationDef_ptr;
typedef OperationDef_ptr OperationDefRef;
typedef ObjVar< OperationDef > OperationDef_var;
typedef ObjOut< OperationDef > OperationDef_out;

class ValueMemberDef;
typedef ValueMemberDef *ValueMemberDef_ptr;
typedef ValueMemberDef_ptr ValueMemberDefRef;
typedef ObjVar< ValueMemberDef > ValueMemberDef_var;
typedef ObjOut< ValueMemberDef > ValueMemberDef_out;

class ExtAttributeDef;
typedef ExtAttributeDef *ExtAttributeDef_ptr;
typedef ExtAttributeDef_ptr ExtAttributeDefRef;
typedef ObjVar< ExtAttributeDef > ExtAttributeDef_var;
typedef ObjOut< ExtAttributeDef > ExtAttributeDef_out;

class InterfaceAttrExtension;
typedef InterfaceAttrExtension *InterfaceAttrExtension_ptr;
typedef InterfaceAttrExtension_ptr InterfaceAttrExtensionRef;
typedef ObjVar< InterfaceAttrExtension > InterfaceAttrExtension_var;
typedef ObjOut< InterfaceAttrExtension > InterfaceAttrExtension_out;

class ExtInterfaceDef;
typedef ExtInterfaceDef *ExtInterfaceDef_ptr;
typedef ExtInterfaceDef_ptr ExtInterfaceDefRef;
typedef ObjVar< ExtInterfaceDef > ExtInterfaceDef_var;
typedef ObjOut< ExtInterfaceDef > ExtInterfaceDef_out;

class ExtAbstractInterfaceDef;
typedef ExtAbstractInterfaceDef *ExtAbstractInterfaceDef_ptr;
typedef ExtAbstractInterfaceDef_ptr ExtAbstractInterfaceDefRef;
typedef ObjVar< ExtAbstractInterfaceDef > ExtAbstractInterfaceDef_var;
typedef ObjOut< ExtAbstractInterfaceDef > ExtAbstractInterfaceDef_out;

class ExtLocalInterfaceDef;
typedef ExtLocalInterfaceDef *ExtLocalInterfaceDef_ptr;
typedef ExtLocalInterfaceDef_ptr ExtLocalInterfaceDefRef;
typedef ObjVar< ExtLocalInterfaceDef > ExtLocalInterfaceDef_var;
typedef ObjOut< ExtLocalInterfaceDef > ExtLocalInterfaceDef_out;

}






namespace CORBA
{

typedef char* ScopedName;
typedef CORBA::String_var ScopedName_var;
typedef CORBA::String_out ScopedName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ScopedName;


/*
 * Base class and common definitions for interface Contained
 */

class Contained : 
  virtual public ::CORBA::IRObject
{
  public:
    virtual ~Contained();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Contained_ptr _ptr_type;
    typedef Contained_var _var_type;
    #endif

    static Contained_ptr _narrow( CORBA::Object_ptr obj );
    static Contained_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Contained_ptr _duplicate( Contained_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Contained_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct Description;
    typedef TVarVar< Description > Description_var;
    typedef TVarOut< Description > Description_out;


    struct Description {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef Description_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      Description();
      ~Description();
      Description( const Description& s );
      Description& operator=( const Description& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      DefinitionKind kind;
      CORBA::Any value;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_Description;

    virtual char* id() = 0;
    virtual void id( const char* value ) = 0;
    virtual char* name() = 0;
    virtual void name( const char* value ) = 0;
    virtual char* version() = 0;
    virtual void version( const char* value ) = 0;
    virtual ::CORBA::Container_ptr defined_in() = 0;
    virtual char* absolute_name() = 0;
    virtual ::CORBA::Repository_ptr containing_repository() = 0;

    virtual Description* describe() = 0;
    virtual void move( Container_ptr new_container, const char* new_name, const char* new_version ) = 0;

  protected:
    Contained() {};
  private:
    Contained( const Contained& );
    void operator=( const Contained& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Contained;

// Stub for interface Contained
class Contained_stub:
  virtual public Contained,
  virtual public ::CORBA::IRObject_stub
{
  public:
    virtual ~Contained_stub();
    char* id();
    void id( const char* value );
    char* name();
    void name( const char* value );
    char* version();
    void version( const char* value );
    ::CORBA::Container_ptr defined_in();
    char* absolute_name();
    ::CORBA::Repository_ptr containing_repository();

    Description* describe();
    void move( Container_ptr new_container, const char* new_name, const char* new_version );

  private:
    void operator=( const Contained_stub& );
};

#ifndef MICO_CONF_NO_POA

class Contained_stub_clp :
  virtual public Contained_stub,
  virtual public ::CORBA::IRObject_stub_clp
{
  public:
    Contained_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Contained_stub_clp ();
    char* id();
    void id( const char* value );
    char* name();
    void name( const char* value );
    char* version();
    void version( const char* value );
    ::CORBA::Container_ptr defined_in();
    char* absolute_name();
    ::CORBA::Repository_ptr containing_repository();

    Description* describe();
    void move( Container_ptr new_container, const char* new_name, const char* new_version );

  protected:
    Contained_stub_clp ();
  private:
    void operator=( const Contained_stub_clp & );
};

#endif // MICO_CONF_NO_POA

typedef IfaceSequenceTmpl< Contained_var,Contained_ptr> ContainedSeq;
typedef TSeqVar< IfaceSequenceTmpl< Contained_var,Contained_ptr> > ContainedSeq_var;
typedef TSeqOut< IfaceSequenceTmpl< Contained_var,Contained_ptr> > ContainedSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ContainedSeq;

typedef IfaceSequenceTmpl< InterfaceDef_var,InterfaceDef_ptr> InterfaceDefSeq;
typedef TSeqVar< IfaceSequenceTmpl< InterfaceDef_var,InterfaceDef_ptr> > InterfaceDefSeq_var;
typedef TSeqOut< IfaceSequenceTmpl< InterfaceDef_var,InterfaceDef_ptr> > InterfaceDefSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InterfaceDefSeq;

typedef IfaceSequenceTmpl< AbstractInterfaceDef_var,AbstractInterfaceDef_ptr> AbstractInterfaceDefSeq;
typedef TSeqVar< IfaceSequenceTmpl< AbstractInterfaceDef_var,AbstractInterfaceDef_ptr> > AbstractInterfaceDefSeq_var;
typedef TSeqOut< IfaceSequenceTmpl< AbstractInterfaceDef_var,AbstractInterfaceDef_ptr> > AbstractInterfaceDefSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AbstractInterfaceDefSeq;

typedef IfaceSequenceTmpl< ValueDef_var,ValueDef_ptr> ValueDefSeq;
typedef TSeqVar< IfaceSequenceTmpl< ValueDef_var,ValueDef_ptr> > ValueDefSeq_var;
typedef TSeqOut< IfaceSequenceTmpl< ValueDef_var,ValueDef_ptr> > ValueDefSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueDefSeq;

struct Initializer;
typedef TVarVar< Initializer > Initializer_var;
typedef TVarOut< Initializer > Initializer_out;


typedef SequenceTmpl< Initializer,MICO_TID_DEF> InitializerSeq;
typedef TSeqVar< SequenceTmpl< Initializer,MICO_TID_DEF> > InitializerSeq_var;
typedef TSeqOut< SequenceTmpl< Initializer,MICO_TID_DEF> > InitializerSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InitializerSeq;

struct ExtInitializer;
typedef TVarVar< ExtInitializer > ExtInitializer_var;
typedef TVarOut< ExtInitializer > ExtInitializer_out;


typedef SequenceTmpl< ExtInitializer,MICO_TID_DEF> ExtInitializerSeq;
typedef TSeqVar< SequenceTmpl< ExtInitializer,MICO_TID_DEF> > ExtInitializerSeq_var;
typedef TSeqOut< SequenceTmpl< ExtInitializer,MICO_TID_DEF> > ExtInitializerSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtInitializerSeq;


/*
 * Base class and common definitions for interface Container
 */

class Container : 
  virtual public ::CORBA::IRObject
{
  public:
    virtual ~Container();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Container_ptr _ptr_type;
    typedef Container_var _var_type;
    #endif

    static Container_ptr _narrow( CORBA::Object_ptr obj );
    static Container_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Container_ptr _duplicate( Container_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Container_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct Description;
    typedef TVarVar< Description > Description_var;
    typedef TVarOut< Description > Description_out;


    struct Description {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef Description_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      Description();
      ~Description();
      Description( const Description& s );
      Description& operator=( const Description& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      Contained_var contained_object;
      DefinitionKind kind;
      CORBA::Any value;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_Description;

    typedef SequenceTmpl< Description,MICO_TID_DEF> DescriptionSeq;
    typedef TSeqVar< SequenceTmpl< Description,MICO_TID_DEF> > DescriptionSeq_var;
    typedef TSeqOut< SequenceTmpl< Description,MICO_TID_DEF> > DescriptionSeq_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_DescriptionSeq;

    virtual Contained_ptr lookup( const char* search_name ) = 0;
    virtual ContainedSeq* contents( DefinitionKind limit_type, CORBA::Boolean exclude_inherited ) = 0;
    virtual ContainedSeq* lookup_name( const char* search_name, CORBA::Long levels_to_search, DefinitionKind limit_type, CORBA::Boolean exclude_inherited ) = 0;
    virtual DescriptionSeq* describe_contents( DefinitionKind limit_type, CORBA::Boolean exclude_inherited, CORBA::Long max_returned_objs ) = 0;
    virtual ModuleDef_ptr create_module( const char* id, const char* name, const char* version ) = 0;
    virtual ConstantDef_ptr create_constant( const char* id, const char* name, const char* version, IDLType_ptr type, const CORBA::Any& value ) = 0;
    virtual StructDef_ptr create_struct( const char* id, const char* name, const char* version, const StructMemberSeq& members ) = 0;
    virtual ExceptionDef_ptr create_exception( const char* id, const char* name, const char* version, const StructMemberSeq& members ) = 0;
    virtual UnionDef_ptr create_union( const char* id, const char* name, const char* version, IDLType_ptr discriminator_type, const UnionMemberSeq& members ) = 0;
    virtual EnumDef_ptr create_enum( const char* id, const char* name, const char* version, const EnumMemberSeq& members ) = 0;
    virtual AliasDef_ptr create_alias( const char* id, const char* name, const char* version, IDLType_ptr original_type ) = 0;
    virtual InterfaceDef_ptr create_interface( const char* id, const char* name, const char* version, const InterfaceDefSeq& base_interfaces ) = 0;
    virtual AbstractInterfaceDef_ptr create_abstract_interface( const char* id, const char* name, const char* version, const AbstractInterfaceDefSeq& base_interfaces ) = 0;
    virtual LocalInterfaceDef_ptr create_local_interface( const char* id, const char* name, const char* version, const InterfaceDefSeq& base_interfaces ) = 0;
    virtual ValueDef_ptr create_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const InitializerSeq& initializers ) = 0;
    virtual ExtValueDef_ptr create_ext_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const ExtInitializerSeq& initializers ) = 0;
    virtual ValueBoxDef_ptr create_value_box( const char* id, const char* name, const char* version, IDLType_ptr original_type_def ) = 0;
    virtual NativeDef_ptr create_native( const char* id, const char* name, const char* version ) = 0;

  protected:
    Container() {};
  private:
    Container( const Container& );
    void operator=( const Container& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Container;

// Stub for interface Container
class Container_stub:
  virtual public Container,
  virtual public ::CORBA::IRObject_stub
{
  public:
    virtual ~Container_stub();
    Contained_ptr lookup( const char* search_name );
    ContainedSeq* contents( DefinitionKind limit_type, CORBA::Boolean exclude_inherited );
    ContainedSeq* lookup_name( const char* search_name, CORBA::Long levels_to_search, DefinitionKind limit_type, CORBA::Boolean exclude_inherited );
    DescriptionSeq* describe_contents( DefinitionKind limit_type, CORBA::Boolean exclude_inherited, CORBA::Long max_returned_objs );
    ModuleDef_ptr create_module( const char* id, const char* name, const char* version );
    ConstantDef_ptr create_constant( const char* id, const char* name, const char* version, IDLType_ptr type, const CORBA::Any& value );
    StructDef_ptr create_struct( const char* id, const char* name, const char* version, const StructMemberSeq& members );
    ExceptionDef_ptr create_exception( const char* id, const char* name, const char* version, const StructMemberSeq& members );
    UnionDef_ptr create_union( const char* id, const char* name, const char* version, IDLType_ptr discriminator_type, const UnionMemberSeq& members );
    EnumDef_ptr create_enum( const char* id, const char* name, const char* version, const EnumMemberSeq& members );
    AliasDef_ptr create_alias( const char* id, const char* name, const char* version, IDLType_ptr original_type );
    InterfaceDef_ptr create_interface( const char* id, const char* name, const char* version, const InterfaceDefSeq& base_interfaces );
    AbstractInterfaceDef_ptr create_abstract_interface( const char* id, const char* name, const char* version, const AbstractInterfaceDefSeq& base_interfaces );
    LocalInterfaceDef_ptr create_local_interface( const char* id, const char* name, const char* version, const InterfaceDefSeq& base_interfaces );
    ValueDef_ptr create_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const InitializerSeq& initializers );
    ExtValueDef_ptr create_ext_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const ExtInitializerSeq& initializers );
    ValueBoxDef_ptr create_value_box( const char* id, const char* name, const char* version, IDLType_ptr original_type_def );
    NativeDef_ptr create_native( const char* id, const char* name, const char* version );

  private:
    void operator=( const Container_stub& );
};

#ifndef MICO_CONF_NO_POA

class Container_stub_clp :
  virtual public Container_stub,
  virtual public ::CORBA::IRObject_stub_clp
{
  public:
    Container_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Container_stub_clp ();
    Contained_ptr lookup( const char* search_name );
    ContainedSeq* contents( DefinitionKind limit_type, CORBA::Boolean exclude_inherited );
    ContainedSeq* lookup_name( const char* search_name, CORBA::Long levels_to_search, DefinitionKind limit_type, CORBA::Boolean exclude_inherited );
    DescriptionSeq* describe_contents( DefinitionKind limit_type, CORBA::Boolean exclude_inherited, CORBA::Long max_returned_objs );
    ModuleDef_ptr create_module( const char* id, const char* name, const char* version );
    ConstantDef_ptr create_constant( const char* id, const char* name, const char* version, IDLType_ptr type, const CORBA::Any& value );
    StructDef_ptr create_struct( const char* id, const char* name, const char* version, const StructMemberSeq& members );
    ExceptionDef_ptr create_exception( const char* id, const char* name, const char* version, const StructMemberSeq& members );
    UnionDef_ptr create_union( const char* id, const char* name, const char* version, IDLType_ptr discriminator_type, const UnionMemberSeq& members );
    EnumDef_ptr create_enum( const char* id, const char* name, const char* version, const EnumMemberSeq& members );
    AliasDef_ptr create_alias( const char* id, const char* name, const char* version, IDLType_ptr original_type );
    InterfaceDef_ptr create_interface( const char* id, const char* name, const char* version, const InterfaceDefSeq& base_interfaces );
    AbstractInterfaceDef_ptr create_abstract_interface( const char* id, const char* name, const char* version, const AbstractInterfaceDefSeq& base_interfaces );
    LocalInterfaceDef_ptr create_local_interface( const char* id, const char* name, const char* version, const InterfaceDefSeq& base_interfaces );
    ValueDef_ptr create_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const InitializerSeq& initializers );
    ExtValueDef_ptr create_ext_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const ExtInitializerSeq& initializers );
    ValueBoxDef_ptr create_value_box( const char* id, const char* name, const char* version, IDLType_ptr original_type_def );
    NativeDef_ptr create_native( const char* id, const char* name, const char* version );

  protected:
    Container_stub_clp ();
  private:
    void operator=( const Container_stub_clp & );
};

#endif // MICO_CONF_NO_POA

enum PrimitiveKind {
  pk_null = 0,
  pk_void,
  pk_short,
  pk_long,
  pk_ushort,
  pk_ulong,
  pk_float,
  pk_double,
  pk_boolean,
  pk_char,
  pk_octet,
  pk_any,
  pk_TypeCode,
  pk_Principal,
  pk_string,
  pk_objref,
  pk_longlong,
  pk_ulonglong,
  pk_longdouble,
  pk_wchar,
  pk_wstring,
  pk_value_base
};

typedef PrimitiveKind& PrimitiveKind_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_PrimitiveKind;


/*
 * Base class and common definitions for interface Repository
 */

class Repository : 
  virtual public ::CORBA::Container
{
  public:
    virtual ~Repository();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Repository_ptr _ptr_type;
    typedef Repository_var _var_type;
    #endif

    static Repository_ptr _narrow( CORBA::Object_ptr obj );
    static Repository_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Repository_ptr _duplicate( Repository_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Repository_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Contained_ptr lookup_id( const char* search_id ) = 0;
    virtual PrimitiveDef_ptr get_primitive( PrimitiveKind kind ) = 0;
    virtual StringDef_ptr create_string( CORBA::ULong bound ) = 0;
    virtual WstringDef_ptr create_wstring( CORBA::ULong bound ) = 0;
    virtual SequenceDef_ptr create_sequence( CORBA::ULong bound, IDLType_ptr element_type ) = 0;
    virtual ArrayDef_ptr create_array( CORBA::ULong length, IDLType_ptr element_type ) = 0;
    virtual FixedDef_ptr create_fixed( CORBA::UShort digits, CORBA::Short scale ) = 0;

  protected:
    Repository() {};
  private:
    Repository( const Repository& );
    void operator=( const Repository& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Repository;

// Stub for interface Repository
class Repository_stub:
  virtual public Repository,
  virtual public ::CORBA::Container_stub
{
  public:
    virtual ~Repository_stub();
    Contained_ptr lookup_id( const char* search_id );
    PrimitiveDef_ptr get_primitive( PrimitiveKind kind );
    StringDef_ptr create_string( CORBA::ULong bound );
    WstringDef_ptr create_wstring( CORBA::ULong bound );
    SequenceDef_ptr create_sequence( CORBA::ULong bound, IDLType_ptr element_type );
    ArrayDef_ptr create_array( CORBA::ULong length, IDLType_ptr element_type );
    FixedDef_ptr create_fixed( CORBA::UShort digits, CORBA::Short scale );

  private:
    void operator=( const Repository_stub& );
};

#ifndef MICO_CONF_NO_POA

class Repository_stub_clp :
  virtual public Repository_stub,
  virtual public ::CORBA::Container_stub_clp
{
  public:
    Repository_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Repository_stub_clp ();
    Contained_ptr lookup_id( const char* search_id );
    PrimitiveDef_ptr get_primitive( PrimitiveKind kind );
    StringDef_ptr create_string( CORBA::ULong bound );
    WstringDef_ptr create_wstring( CORBA::ULong bound );
    SequenceDef_ptr create_sequence( CORBA::ULong bound, IDLType_ptr element_type );
    ArrayDef_ptr create_array( CORBA::ULong length, IDLType_ptr element_type );
    FixedDef_ptr create_fixed( CORBA::UShort digits, CORBA::Short scale );

  protected:
    Repository_stub_clp ();
  private:
    void operator=( const Repository_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ModuleDef
 */

class ModuleDef : 
  virtual public ::CORBA::Container,
  virtual public ::CORBA::Contained
{
  public:
    virtual ~ModuleDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ModuleDef_ptr _ptr_type;
    typedef ModuleDef_var _var_type;
    #endif

    static ModuleDef_ptr _narrow( CORBA::Object_ptr obj );
    static ModuleDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ModuleDef_ptr _duplicate( ModuleDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ModuleDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    ModuleDef() {};
  private:
    ModuleDef( const ModuleDef& );
    void operator=( const ModuleDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ModuleDef;

// Stub for interface ModuleDef
class ModuleDef_stub:
  virtual public ModuleDef,
  virtual public ::CORBA::Container_stub,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~ModuleDef_stub();
  private:
    void operator=( const ModuleDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ModuleDef_stub_clp :
  virtual public ModuleDef_stub,
  virtual public ::CORBA::Container_stub_clp,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    ModuleDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ModuleDef_stub_clp ();
  protected:
    ModuleDef_stub_clp ();
  private:
    void operator=( const ModuleDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ConstantDef
 */

class ConstantDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~ConstantDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ConstantDef_ptr _ptr_type;
    typedef ConstantDef_var _var_type;
    #endif

    static ConstantDef_ptr _narrow( CORBA::Object_ptr obj );
    static ConstantDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ConstantDef_ptr _duplicate( ConstantDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ConstantDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::IDLType_ptr type_def() = 0;
    virtual void type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual CORBA::Any* value() = 0;
    virtual void value( const CORBA::Any& value ) = 0;

  protected:
    ConstantDef() {};
  private:
    ConstantDef( const ConstantDef& );
    void operator=( const ConstantDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ConstantDef;

// Stub for interface ConstantDef
class ConstantDef_stub:
  virtual public ConstantDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~ConstantDef_stub();
    CORBA::TypeCode_ptr type();
    ::CORBA::IDLType_ptr type_def();
    void type_def( ::CORBA::IDLType_ptr value );
    CORBA::Any* value();
    void value( const CORBA::Any& value );

  private:
    void operator=( const ConstantDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ConstantDef_stub_clp :
  virtual public ConstantDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    ConstantDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ConstantDef_stub_clp ();
    CORBA::TypeCode_ptr type();
    ::CORBA::IDLType_ptr type_def();
    void type_def( ::CORBA::IDLType_ptr value );
    CORBA::Any* value();
    void value( const CORBA::Any& value );

  protected:
    ConstantDef_stub_clp ();
  private:
    void operator=( const ConstantDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface TypedefDef
 */

class TypedefDef : 
  virtual public ::CORBA::Contained,
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~TypedefDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef TypedefDef_ptr _ptr_type;
    typedef TypedefDef_var _var_type;
    #endif

    static TypedefDef_ptr _narrow( CORBA::Object_ptr obj );
    static TypedefDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static TypedefDef_ptr _duplicate( TypedefDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static TypedefDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    TypedefDef() {};
  private:
    TypedefDef( const TypedefDef& );
    void operator=( const TypedefDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TypedefDef;

// Stub for interface TypedefDef
class TypedefDef_stub:
  virtual public TypedefDef,
  virtual public ::CORBA::Contained_stub,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~TypedefDef_stub();
  private:
    void operator=( const TypedefDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class TypedefDef_stub_clp :
  virtual public TypedefDef_stub,
  virtual public ::CORBA::Contained_stub_clp,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    TypedefDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~TypedefDef_stub_clp ();
  protected:
    TypedefDef_stub_clp ();
  private:
    void operator=( const TypedefDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface StructDef
 */

class StructDef : 
  virtual public ::CORBA::TypedefDef,
  virtual public ::CORBA::Container
{
  public:
    virtual ~StructDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef StructDef_ptr _ptr_type;
    typedef StructDef_var _var_type;
    #endif

    static StructDef_ptr _narrow( CORBA::Object_ptr obj );
    static StructDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static StructDef_ptr _duplicate( StructDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static StructDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::StructMemberSeq* members() = 0;
    virtual void members( const ::CORBA::StructMemberSeq& value ) = 0;

  protected:
    StructDef() {};
  private:
    StructDef( const StructDef& );
    void operator=( const StructDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_StructDef;

// Stub for interface StructDef
class StructDef_stub:
  virtual public StructDef,
  virtual public ::CORBA::TypedefDef_stub,
  virtual public ::CORBA::Container_stub
{
  public:
    virtual ~StructDef_stub();
    ::CORBA::StructMemberSeq* members();
    void members( const ::CORBA::StructMemberSeq& value );

  private:
    void operator=( const StructDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class StructDef_stub_clp :
  virtual public StructDef_stub,
  virtual public ::CORBA::TypedefDef_stub_clp,
  virtual public ::CORBA::Container_stub_clp
{
  public:
    StructDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~StructDef_stub_clp ();
    ::CORBA::StructMemberSeq* members();
    void members( const ::CORBA::StructMemberSeq& value );

  protected:
    StructDef_stub_clp ();
  private:
    void operator=( const StructDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ExceptionDef
 */

class ExceptionDef : 
  virtual public ::CORBA::Contained,
  virtual public ::CORBA::Container
{
  public:
    virtual ~ExceptionDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExceptionDef_ptr _ptr_type;
    typedef ExceptionDef_var _var_type;
    #endif

    static ExceptionDef_ptr _narrow( CORBA::Object_ptr obj );
    static ExceptionDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExceptionDef_ptr _duplicate( ExceptionDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExceptionDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::StructMemberSeq* members() = 0;
    virtual void members( const ::CORBA::StructMemberSeq& value ) = 0;

  protected:
    ExceptionDef() {};
  private:
    ExceptionDef( const ExceptionDef& );
    void operator=( const ExceptionDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExceptionDef;

// Stub for interface ExceptionDef
class ExceptionDef_stub:
  virtual public ExceptionDef,
  virtual public ::CORBA::Contained_stub,
  virtual public ::CORBA::Container_stub
{
  public:
    virtual ~ExceptionDef_stub();
    CORBA::TypeCode_ptr type();
    ::CORBA::StructMemberSeq* members();
    void members( const ::CORBA::StructMemberSeq& value );

  private:
    void operator=( const ExceptionDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ExceptionDef_stub_clp :
  virtual public ExceptionDef_stub,
  virtual public ::CORBA::Contained_stub_clp,
  virtual public ::CORBA::Container_stub_clp
{
  public:
    ExceptionDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ExceptionDef_stub_clp ();
    CORBA::TypeCode_ptr type();
    ::CORBA::StructMemberSeq* members();
    void members( const ::CORBA::StructMemberSeq& value );

  protected:
    ExceptionDef_stub_clp ();
  private:
    void operator=( const ExceptionDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface UnionDef
 */

class UnionDef : 
  virtual public ::CORBA::TypedefDef,
  virtual public ::CORBA::Container
{
  public:
    virtual ~UnionDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef UnionDef_ptr _ptr_type;
    typedef UnionDef_var _var_type;
    #endif

    static UnionDef_ptr _narrow( CORBA::Object_ptr obj );
    static UnionDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static UnionDef_ptr _duplicate( UnionDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static UnionDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr discriminator_type() = 0;
    virtual ::CORBA::IDLType_ptr discriminator_type_def() = 0;
    virtual void discriminator_type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::UnionMemberSeq* members() = 0;
    virtual void members( const ::CORBA::UnionMemberSeq& value ) = 0;

  protected:
    UnionDef() {};
  private:
    UnionDef( const UnionDef& );
    void operator=( const UnionDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UnionDef;

// Stub for interface UnionDef
class UnionDef_stub:
  virtual public UnionDef,
  virtual public ::CORBA::TypedefDef_stub,
  virtual public ::CORBA::Container_stub
{
  public:
    virtual ~UnionDef_stub();
    CORBA::TypeCode_ptr discriminator_type();
    ::CORBA::IDLType_ptr discriminator_type_def();
    void discriminator_type_def( ::CORBA::IDLType_ptr value );
    ::CORBA::UnionMemberSeq* members();
    void members( const ::CORBA::UnionMemberSeq& value );

  private:
    void operator=( const UnionDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class UnionDef_stub_clp :
  virtual public UnionDef_stub,
  virtual public ::CORBA::TypedefDef_stub_clp,
  virtual public ::CORBA::Container_stub_clp
{
  public:
    UnionDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~UnionDef_stub_clp ();
    CORBA::TypeCode_ptr discriminator_type();
    ::CORBA::IDLType_ptr discriminator_type_def();
    void discriminator_type_def( ::CORBA::IDLType_ptr value );
    ::CORBA::UnionMemberSeq* members();
    void members( const ::CORBA::UnionMemberSeq& value );

  protected:
    UnionDef_stub_clp ();
  private:
    void operator=( const UnionDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface EnumDef
 */

class EnumDef : 
  virtual public ::CORBA::TypedefDef
{
  public:
    virtual ~EnumDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EnumDef_ptr _ptr_type;
    typedef EnumDef_var _var_type;
    #endif

    static EnumDef_ptr _narrow( CORBA::Object_ptr obj );
    static EnumDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EnumDef_ptr _duplicate( EnumDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EnumDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::EnumMemberSeq* members() = 0;
    virtual void members( const ::CORBA::EnumMemberSeq& value ) = 0;

  protected:
    EnumDef() {};
  private:
    EnumDef( const EnumDef& );
    void operator=( const EnumDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_EnumDef;

// Stub for interface EnumDef
class EnumDef_stub:
  virtual public EnumDef,
  virtual public ::CORBA::TypedefDef_stub
{
  public:
    virtual ~EnumDef_stub();
    ::CORBA::EnumMemberSeq* members();
    void members( const ::CORBA::EnumMemberSeq& value );

  private:
    void operator=( const EnumDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class EnumDef_stub_clp :
  virtual public EnumDef_stub,
  virtual public ::CORBA::TypedefDef_stub_clp
{
  public:
    EnumDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~EnumDef_stub_clp ();
    ::CORBA::EnumMemberSeq* members();
    void members( const ::CORBA::EnumMemberSeq& value );

  protected:
    EnumDef_stub_clp ();
  private:
    void operator=( const EnumDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface AliasDef
 */

class AliasDef : 
  virtual public ::CORBA::TypedefDef
{
  public:
    virtual ~AliasDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AliasDef_ptr _ptr_type;
    typedef AliasDef_var _var_type;
    #endif

    static AliasDef_ptr _narrow( CORBA::Object_ptr obj );
    static AliasDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AliasDef_ptr _duplicate( AliasDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AliasDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::IDLType_ptr original_type_def() = 0;
    virtual void original_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    AliasDef() {};
  private:
    AliasDef( const AliasDef& );
    void operator=( const AliasDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AliasDef;

// Stub for interface AliasDef
class AliasDef_stub:
  virtual public AliasDef,
  virtual public ::CORBA::TypedefDef_stub
{
  public:
    virtual ~AliasDef_stub();
    ::CORBA::IDLType_ptr original_type_def();
    void original_type_def( ::CORBA::IDLType_ptr value );

  private:
    void operator=( const AliasDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class AliasDef_stub_clp :
  virtual public AliasDef_stub,
  virtual public ::CORBA::TypedefDef_stub_clp
{
  public:
    AliasDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~AliasDef_stub_clp ();
    ::CORBA::IDLType_ptr original_type_def();
    void original_type_def( ::CORBA::IDLType_ptr value );

  protected:
    AliasDef_stub_clp ();
  private:
    void operator=( const AliasDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct OperationDescription;
typedef TVarVar< OperationDescription > OperationDescription_var;
typedef TVarOut< OperationDescription > OperationDescription_out;


typedef SequenceTmpl< OperationDescription,MICO_TID_DEF> OpDescriptionSeq;
typedef TSeqVar< SequenceTmpl< OperationDescription,MICO_TID_DEF> > OpDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< OperationDescription,MICO_TID_DEF> > OpDescriptionSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OpDescriptionSeq;

struct AttributeDescription;
typedef TVarVar< AttributeDescription > AttributeDescription_var;
typedef TVarOut< AttributeDescription > AttributeDescription_out;


typedef SequenceTmpl< AttributeDescription,MICO_TID_DEF> AttrDescriptionSeq;
typedef TSeqVar< SequenceTmpl< AttributeDescription,MICO_TID_DEF> > AttrDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< AttributeDescription,MICO_TID_DEF> > AttrDescriptionSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttrDescriptionSeq;

typedef StringSequenceTmpl<CORBA::String_var> RepositoryIdSeq;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > RepositoryIdSeq_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > RepositoryIdSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RepositoryIdSeq;

enum AttributeMode {
  ATTR_NORMAL = 0,
  ATTR_READONLY
};

typedef AttributeMode& AttributeMode_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttributeMode;

enum OperationMode {
  OP_NORMAL = 0,
  OP_ONEWAY
};

typedef OperationMode& OperationMode_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OperationMode;

struct ParameterDescription;
typedef TVarVar< ParameterDescription > ParameterDescription_var;
typedef TVarOut< ParameterDescription > ParameterDescription_out;


typedef SequenceTmpl< ParameterDescription,MICO_TID_DEF> ParDescriptionSeq;
typedef TSeqVar< SequenceTmpl< ParameterDescription,MICO_TID_DEF> > ParDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< ParameterDescription,MICO_TID_DEF> > ParDescriptionSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ParDescriptionSeq;

typedef IfaceSequenceTmpl< ExceptionDef_var,ExceptionDef_ptr> ExceptionDefSeq;
typedef TSeqVar< IfaceSequenceTmpl< ExceptionDef_var,ExceptionDef_ptr> > ExceptionDefSeq_var;
typedef TSeqOut< IfaceSequenceTmpl< ExceptionDef_var,ExceptionDef_ptr> > ExceptionDefSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExceptionDefSeq;

typedef char* ContextIdentifier;
typedef CORBA::String_var ContextIdentifier_var;
typedef CORBA::String_out ContextIdentifier_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ContextIdentifier;

typedef StringSequenceTmpl<CORBA::String_var> ContextIdSeq;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > ContextIdSeq_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > ContextIdSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ContextIdSeq;


/*
 * Base class and common definitions for interface InterfaceDef
 */

class InterfaceDef : 
  virtual public ::CORBA::Container,
  virtual public ::CORBA::Contained,
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~InterfaceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef InterfaceDef_ptr _ptr_type;
    typedef InterfaceDef_var _var_type;
    #endif

    static InterfaceDef_ptr _narrow( CORBA::Object_ptr obj );
    static InterfaceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static InterfaceDef_ptr _duplicate( InterfaceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static InterfaceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct FullInterfaceDescription;
    typedef TVarVar< FullInterfaceDescription > FullInterfaceDescription_var;
    typedef TVarOut< FullInterfaceDescription > FullInterfaceDescription_out;


    struct FullInterfaceDescription {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef FullInterfaceDescription_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      FullInterfaceDescription();
      ~FullInterfaceDescription();
      FullInterfaceDescription( const FullInterfaceDescription& s );
      FullInterfaceDescription& operator=( const FullInterfaceDescription& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      Identifier_var name;
      RepositoryId_var id;
      RepositoryId_var defined_in;
      VersionSpec_var version;
      OpDescriptionSeq operations;
      AttrDescriptionSeq attributes;
      RepositoryIdSeq base_interfaces;
      CORBA::TypeCode_var type;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_FullInterfaceDescription;

    virtual ::CORBA::InterfaceDefSeq* base_interfaces() = 0;
    virtual void base_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;

    virtual CORBA::Boolean is_a( const char* interface_id ) = 0;
    virtual FullInterfaceDescription* describe_interface() = 0;
    virtual AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode ) = 0;
    virtual OperationDef_ptr create_operation( const char* id, const char* name, const char* version, IDLType_ptr result, OperationMode mode, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions, const ContextIdSeq& contexts ) = 0;

  protected:
    InterfaceDef() {};
  private:
    InterfaceDef( const InterfaceDef& );
    void operator=( const InterfaceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InterfaceDef;

// Stub for interface InterfaceDef
class InterfaceDef_stub:
  virtual public InterfaceDef,
  virtual public ::CORBA::Container_stub,
  virtual public ::CORBA::Contained_stub,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~InterfaceDef_stub();
    ::CORBA::InterfaceDefSeq* base_interfaces();
    void base_interfaces( const ::CORBA::InterfaceDefSeq& value );

    CORBA::Boolean is_a( const char* interface_id );
    FullInterfaceDescription* describe_interface();
    AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode );
    OperationDef_ptr create_operation( const char* id, const char* name, const char* version, IDLType_ptr result, OperationMode mode, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions, const ContextIdSeq& contexts );

  private:
    void operator=( const InterfaceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class InterfaceDef_stub_clp :
  virtual public InterfaceDef_stub,
  virtual public ::CORBA::Container_stub_clp,
  virtual public ::CORBA::Contained_stub_clp,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    InterfaceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~InterfaceDef_stub_clp ();
    ::CORBA::InterfaceDefSeq* base_interfaces();
    void base_interfaces( const ::CORBA::InterfaceDefSeq& value );

    CORBA::Boolean is_a( const char* interface_id );
    FullInterfaceDescription* describe_interface();
    AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode );
    OperationDef_ptr create_operation( const char* id, const char* name, const char* version, IDLType_ptr result, OperationMode mode, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions, const ContextIdSeq& contexts );

  protected:
    InterfaceDef_stub_clp ();
  private:
    void operator=( const InterfaceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface AbstractInterfaceDef
 */

class AbstractInterfaceDef : 
  virtual public ::CORBA::InterfaceDef
{
  public:
    virtual ~AbstractInterfaceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AbstractInterfaceDef_ptr _ptr_type;
    typedef AbstractInterfaceDef_var _var_type;
    #endif

    static AbstractInterfaceDef_ptr _narrow( CORBA::Object_ptr obj );
    static AbstractInterfaceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AbstractInterfaceDef_ptr _duplicate( AbstractInterfaceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AbstractInterfaceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    AbstractInterfaceDef() {};
  private:
    AbstractInterfaceDef( const AbstractInterfaceDef& );
    void operator=( const AbstractInterfaceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AbstractInterfaceDef;

// Stub for interface AbstractInterfaceDef
class AbstractInterfaceDef_stub:
  virtual public AbstractInterfaceDef,
  virtual public ::CORBA::InterfaceDef_stub
{
  public:
    virtual ~AbstractInterfaceDef_stub();
  private:
    void operator=( const AbstractInterfaceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class AbstractInterfaceDef_stub_clp :
  virtual public AbstractInterfaceDef_stub,
  virtual public ::CORBA::InterfaceDef_stub_clp
{
  public:
    AbstractInterfaceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~AbstractInterfaceDef_stub_clp ();
  protected:
    AbstractInterfaceDef_stub_clp ();
  private:
    void operator=( const AbstractInterfaceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface LocalInterfaceDef
 */

class LocalInterfaceDef : 
  virtual public ::CORBA::InterfaceDef
{
  public:
    virtual ~LocalInterfaceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef LocalInterfaceDef_ptr _ptr_type;
    typedef LocalInterfaceDef_var _var_type;
    #endif

    static LocalInterfaceDef_ptr _narrow( CORBA::Object_ptr obj );
    static LocalInterfaceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static LocalInterfaceDef_ptr _duplicate( LocalInterfaceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static LocalInterfaceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    LocalInterfaceDef() {};
  private:
    LocalInterfaceDef( const LocalInterfaceDef& );
    void operator=( const LocalInterfaceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_LocalInterfaceDef;

// Stub for interface LocalInterfaceDef
class LocalInterfaceDef_stub:
  virtual public LocalInterfaceDef,
  virtual public ::CORBA::InterfaceDef_stub
{
  public:
    virtual ~LocalInterfaceDef_stub();
  private:
    void operator=( const LocalInterfaceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class LocalInterfaceDef_stub_clp :
  virtual public LocalInterfaceDef_stub,
  virtual public ::CORBA::InterfaceDef_stub_clp
{
  public:
    LocalInterfaceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~LocalInterfaceDef_stub_clp ();
  protected:
    LocalInterfaceDef_stub_clp ();
  private:
    void operator=( const LocalInterfaceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ValueDef
 */

class ValueDef : 
  virtual public ::CORBA::Container,
  virtual public ::CORBA::Contained,
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~ValueDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ValueDef_ptr _ptr_type;
    typedef ValueDef_var _var_type;
    #endif

    static ValueDef_ptr _narrow( CORBA::Object_ptr obj );
    static ValueDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ValueDef_ptr _duplicate( ValueDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ValueDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct FullValueDescription;
    typedef TVarVar< FullValueDescription > FullValueDescription_var;
    typedef TVarOut< FullValueDescription > FullValueDescription_out;


    struct FullValueDescription {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef FullValueDescription_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      FullValueDescription();
      ~FullValueDescription();
      FullValueDescription( const FullValueDescription& s );
      FullValueDescription& operator=( const FullValueDescription& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      Identifier_var name;
      RepositoryId_var id;
      CORBA::Boolean is_abstract;
      CORBA::Boolean is_custom;
      RepositoryId_var defined_in;
      VersionSpec_var version;
      OpDescriptionSeq operations;
      AttrDescriptionSeq attributes;
      ValueMemberSeq members;
      InitializerSeq initializers;
      RepositoryIdSeq supported_interfaces;
      RepositoryIdSeq abstract_base_values;
      CORBA::Boolean is_truncatable;
      RepositoryId_var base_value;
      CORBA::TypeCode_var type;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_FullValueDescription;

    virtual ::CORBA::InterfaceDefSeq* supported_interfaces() = 0;
    virtual void supported_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;
    virtual ::CORBA::InitializerSeq* initializers() = 0;
    virtual void initializers( const ::CORBA::InitializerSeq& value ) = 0;
    virtual ::CORBA::ValueDef_ptr base_value() = 0;
    virtual void base_value( ::CORBA::ValueDef_ptr value ) = 0;
    virtual ::CORBA::ValueDefSeq* abstract_base_values() = 0;
    virtual void abstract_base_values( const ::CORBA::ValueDefSeq& value ) = 0;
    virtual CORBA::Boolean is_abstract() = 0;
    virtual void is_abstract( CORBA::Boolean value ) = 0;
    virtual CORBA::Boolean is_custom() = 0;
    virtual void is_custom( CORBA::Boolean value ) = 0;
    virtual CORBA::Boolean is_truncatable() = 0;
    virtual void is_truncatable( CORBA::Boolean value ) = 0;

    virtual CORBA::Boolean is_a( const char* value_id ) = 0;
    virtual FullValueDescription* describe_value() = 0;
    virtual ValueMemberDef_ptr create_value_member( const char* id, const char* name, const char* version, IDLType_ptr type, Visibility access ) = 0;
    virtual AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode ) = 0;
    virtual OperationDef_ptr create_operation( const char* id, const char* name, const char* version, IDLType_ptr result, OperationMode mode, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions, const ContextIdSeq& contexts ) = 0;

  protected:
    ValueDef() {};
  private:
    ValueDef( const ValueDef& );
    void operator=( const ValueDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueDef;

// Stub for interface ValueDef
class ValueDef_stub:
  virtual public ValueDef,
  virtual public ::CORBA::Container_stub,
  virtual public ::CORBA::Contained_stub,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~ValueDef_stub();
    ::CORBA::InterfaceDefSeq* supported_interfaces();
    void supported_interfaces( const ::CORBA::InterfaceDefSeq& value );
    ::CORBA::InitializerSeq* initializers();
    void initializers( const ::CORBA::InitializerSeq& value );
    ::CORBA::ValueDef_ptr base_value();
    void base_value( ::CORBA::ValueDef_ptr value );
    ::CORBA::ValueDefSeq* abstract_base_values();
    void abstract_base_values( const ::CORBA::ValueDefSeq& value );
    CORBA::Boolean is_abstract();
    void is_abstract( CORBA::Boolean value );
    CORBA::Boolean is_custom();
    void is_custom( CORBA::Boolean value );
    CORBA::Boolean is_truncatable();
    void is_truncatable( CORBA::Boolean value );

    CORBA::Boolean is_a( const char* value_id );
    FullValueDescription* describe_value();
    ValueMemberDef_ptr create_value_member( const char* id, const char* name, const char* version, IDLType_ptr type, Visibility access );
    AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode );
    OperationDef_ptr create_operation( const char* id, const char* name, const char* version, IDLType_ptr result, OperationMode mode, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions, const ContextIdSeq& contexts );

  private:
    void operator=( const ValueDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ValueDef_stub_clp :
  virtual public ValueDef_stub,
  virtual public ::CORBA::Container_stub_clp,
  virtual public ::CORBA::Contained_stub_clp,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    ValueDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ValueDef_stub_clp ();
    ::CORBA::InterfaceDefSeq* supported_interfaces();
    void supported_interfaces( const ::CORBA::InterfaceDefSeq& value );
    ::CORBA::InitializerSeq* initializers();
    void initializers( const ::CORBA::InitializerSeq& value );
    ::CORBA::ValueDef_ptr base_value();
    void base_value( ::CORBA::ValueDef_ptr value );
    ::CORBA::ValueDefSeq* abstract_base_values();
    void abstract_base_values( const ::CORBA::ValueDefSeq& value );
    CORBA::Boolean is_abstract();
    void is_abstract( CORBA::Boolean value );
    CORBA::Boolean is_custom();
    void is_custom( CORBA::Boolean value );
    CORBA::Boolean is_truncatable();
    void is_truncatable( CORBA::Boolean value );

    CORBA::Boolean is_a( const char* value_id );
    FullValueDescription* describe_value();
    ValueMemberDef_ptr create_value_member( const char* id, const char* name, const char* version, IDLType_ptr type, Visibility access );
    AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode );
    OperationDef_ptr create_operation( const char* id, const char* name, const char* version, IDLType_ptr result, OperationMode mode, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions, const ContextIdSeq& contexts );

  protected:
    ValueDef_stub_clp ();
  private:
    void operator=( const ValueDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ValueBoxDef
 */

class ValueBoxDef : 
  virtual public ::CORBA::TypedefDef
{
  public:
    virtual ~ValueBoxDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ValueBoxDef_ptr _ptr_type;
    typedef ValueBoxDef_var _var_type;
    #endif

    static ValueBoxDef_ptr _narrow( CORBA::Object_ptr obj );
    static ValueBoxDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ValueBoxDef_ptr _duplicate( ValueBoxDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ValueBoxDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::IDLType_ptr original_type_def() = 0;
    virtual void original_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    ValueBoxDef() {};
  private:
    ValueBoxDef( const ValueBoxDef& );
    void operator=( const ValueBoxDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueBoxDef;

// Stub for interface ValueBoxDef
class ValueBoxDef_stub:
  virtual public ValueBoxDef,
  virtual public ::CORBA::TypedefDef_stub
{
  public:
    virtual ~ValueBoxDef_stub();
    ::CORBA::IDLType_ptr original_type_def();
    void original_type_def( ::CORBA::IDLType_ptr value );

  private:
    void operator=( const ValueBoxDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ValueBoxDef_stub_clp :
  virtual public ValueBoxDef_stub,
  virtual public ::CORBA::TypedefDef_stub_clp
{
  public:
    ValueBoxDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ValueBoxDef_stub_clp ();
    ::CORBA::IDLType_ptr original_type_def();
    void original_type_def( ::CORBA::IDLType_ptr value );

  protected:
    ValueBoxDef_stub_clp ();
  private:
    void operator=( const ValueBoxDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface NativeDef
 */

class NativeDef : 
  virtual public ::CORBA::TypedefDef
{
  public:
    virtual ~NativeDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef NativeDef_ptr _ptr_type;
    typedef NativeDef_var _var_type;
    #endif

    static NativeDef_ptr _narrow( CORBA::Object_ptr obj );
    static NativeDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static NativeDef_ptr _duplicate( NativeDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static NativeDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    NativeDef() {};
  private:
    NativeDef( const NativeDef& );
    void operator=( const NativeDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_NativeDef;

// Stub for interface NativeDef
class NativeDef_stub:
  virtual public NativeDef,
  virtual public ::CORBA::TypedefDef_stub
{
  public:
    virtual ~NativeDef_stub();
  private:
    void operator=( const NativeDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class NativeDef_stub_clp :
  virtual public NativeDef_stub,
  virtual public ::CORBA::TypedefDef_stub_clp
{
  public:
    NativeDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~NativeDef_stub_clp ();
  protected:
    NativeDef_stub_clp ();
  private:
    void operator=( const NativeDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct ExtAttributeDescription;
typedef TVarVar< ExtAttributeDescription > ExtAttributeDescription_var;
typedef TVarOut< ExtAttributeDescription > ExtAttributeDescription_out;


typedef SequenceTmpl< ExtAttributeDescription,MICO_TID_DEF> ExtAttrDescriptionSeq;
typedef TSeqVar< SequenceTmpl< ExtAttributeDescription,MICO_TID_DEF> > ExtAttrDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< ExtAttributeDescription,MICO_TID_DEF> > ExtAttrDescriptionSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtAttrDescriptionSeq;


/*
 * Base class and common definitions for interface ExtValueDef
 */

class ExtValueDef : 
  virtual public ::CORBA::ValueDef
{
  public:
    virtual ~ExtValueDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExtValueDef_ptr _ptr_type;
    typedef ExtValueDef_var _var_type;
    #endif

    static ExtValueDef_ptr _narrow( CORBA::Object_ptr obj );
    static ExtValueDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExtValueDef_ptr _duplicate( ExtValueDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExtValueDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct ExtFullValueDescription;
    typedef TVarVar< ExtFullValueDescription > ExtFullValueDescription_var;
    typedef TVarOut< ExtFullValueDescription > ExtFullValueDescription_out;


    struct ExtFullValueDescription {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef ExtFullValueDescription_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ExtFullValueDescription();
      ~ExtFullValueDescription();
      ExtFullValueDescription( const ExtFullValueDescription& s );
      ExtFullValueDescription& operator=( const ExtFullValueDescription& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      Identifier_var name;
      RepositoryId_var id;
      CORBA::Boolean is_abstract;
      CORBA::Boolean is_custom;
      RepositoryId_var defined_in;
      VersionSpec_var version;
      OpDescriptionSeq operations;
      ExtAttrDescriptionSeq attributes;
      ValueMemberSeq members;
      ExtInitializerSeq initializers;
      RepositoryIdSeq supported_interfaces;
      RepositoryIdSeq abstract_base_values;
      CORBA::Boolean is_truncatable;
      RepositoryId_var base_value;
      CORBA::TypeCode_var type;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ExtFullValueDescription;

    virtual ::CORBA::ExtInitializerSeq* ext_initializers() = 0;
    virtual void ext_initializers( const ::CORBA::ExtInitializerSeq& value ) = 0;

    virtual ExtFullValueDescription* describe_ext_value() = 0;
    virtual ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode, const ExceptionDefSeq& get_exceptions, const ExceptionDefSeq& set_exceptions ) = 0;

  protected:
    ExtValueDef() {};
  private:
    ExtValueDef( const ExtValueDef& );
    void operator=( const ExtValueDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtValueDef;

// Stub for interface ExtValueDef
class ExtValueDef_stub:
  virtual public ExtValueDef,
  virtual public ::CORBA::ValueDef_stub
{
  public:
    virtual ~ExtValueDef_stub();
    ::CORBA::ExtInitializerSeq* ext_initializers();
    void ext_initializers( const ::CORBA::ExtInitializerSeq& value );

    ExtFullValueDescription* describe_ext_value();
    ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode, const ExceptionDefSeq& get_exceptions, const ExceptionDefSeq& set_exceptions );

  private:
    void operator=( const ExtValueDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ExtValueDef_stub_clp :
  virtual public ExtValueDef_stub,
  virtual public ::CORBA::ValueDef_stub_clp
{
  public:
    ExtValueDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ExtValueDef_stub_clp ();
    ::CORBA::ExtInitializerSeq* ext_initializers();
    void ext_initializers( const ::CORBA::ExtInitializerSeq& value );

    ExtFullValueDescription* describe_ext_value();
    ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode, const ExceptionDefSeq& get_exceptions, const ExceptionDefSeq& set_exceptions );

  protected:
    ExtValueDef_stub_clp ();
  private:
    void operator=( const ExtValueDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

typedef IfaceSequenceTmpl< LocalInterfaceDef_var,LocalInterfaceDef_ptr> LocalInterfaceDefSeq;
typedef TSeqVar< IfaceSequenceTmpl< LocalInterfaceDef_var,LocalInterfaceDef_ptr> > LocalInterfaceDefSeq_var;
typedef TSeqOut< IfaceSequenceTmpl< LocalInterfaceDef_var,LocalInterfaceDef_ptr> > LocalInterfaceDefSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_LocalInterfaceDefSeq;

struct ExceptionDescription;
typedef TVarVar< ExceptionDescription > ExceptionDescription_var;
typedef TVarOut< ExceptionDescription > ExceptionDescription_out;


struct ExceptionDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ExceptionDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ExceptionDescription();
  ~ExceptionDescription();
  ExceptionDescription( const ExceptionDescription& s );
  ExceptionDescription& operator=( const ExceptionDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var type;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExceptionDescription;

typedef SequenceTmpl< ExceptionDescription,MICO_TID_DEF> ExcDescriptionSeq;
typedef TSeqVar< SequenceTmpl< ExceptionDescription,MICO_TID_DEF> > ExcDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< ExceptionDescription,MICO_TID_DEF> > ExcDescriptionSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExcDescriptionSeq;

struct Initializer {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef Initializer_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Initializer();
  ~Initializer();
  Initializer( const Initializer& s );
  Initializer& operator=( const Initializer& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  StructMemberSeq members;
  Identifier_var name;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Initializer;

struct ExtInitializer {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ExtInitializer_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ExtInitializer();
  ~ExtInitializer();
  ExtInitializer( const ExtInitializer& s );
  ExtInitializer& operator=( const ExtInitializer& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  StructMemberSeq members;
  ExceptionDefSeq exceptions_def;
  ExcDescriptionSeq exceptions;
  Identifier_var name;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtInitializer;


/*
 * Base class and common definitions for interface PrimitiveDef
 */

class PrimitiveDef : 
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~PrimitiveDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef PrimitiveDef_ptr _ptr_type;
    typedef PrimitiveDef_var _var_type;
    #endif

    static PrimitiveDef_ptr _narrow( CORBA::Object_ptr obj );
    static PrimitiveDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static PrimitiveDef_ptr _duplicate( PrimitiveDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static PrimitiveDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::PrimitiveKind kind() = 0;

  protected:
    PrimitiveDef() {};
  private:
    PrimitiveDef( const PrimitiveDef& );
    void operator=( const PrimitiveDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_PrimitiveDef;

// Stub for interface PrimitiveDef
class PrimitiveDef_stub:
  virtual public PrimitiveDef,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~PrimitiveDef_stub();
    ::CORBA::PrimitiveKind kind();

  private:
    void operator=( const PrimitiveDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class PrimitiveDef_stub_clp :
  virtual public PrimitiveDef_stub,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    PrimitiveDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~PrimitiveDef_stub_clp ();
    ::CORBA::PrimitiveKind kind();

  protected:
    PrimitiveDef_stub_clp ();
  private:
    void operator=( const PrimitiveDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface StringDef
 */

class StringDef : 
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~StringDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef StringDef_ptr _ptr_type;
    typedef StringDef_var _var_type;
    #endif

    static StringDef_ptr _narrow( CORBA::Object_ptr obj );
    static StringDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static StringDef_ptr _duplicate( StringDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static StringDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::ULong bound() = 0;
    virtual void bound( CORBA::ULong value ) = 0;

  protected:
    StringDef() {};
  private:
    StringDef( const StringDef& );
    void operator=( const StringDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_StringDef;

// Stub for interface StringDef
class StringDef_stub:
  virtual public StringDef,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~StringDef_stub();
    CORBA::ULong bound();
    void bound( CORBA::ULong value );

  private:
    void operator=( const StringDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class StringDef_stub_clp :
  virtual public StringDef_stub,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    StringDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~StringDef_stub_clp ();
    CORBA::ULong bound();
    void bound( CORBA::ULong value );

  protected:
    StringDef_stub_clp ();
  private:
    void operator=( const StringDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface WstringDef
 */

class WstringDef : 
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~WstringDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef WstringDef_ptr _ptr_type;
    typedef WstringDef_var _var_type;
    #endif

    static WstringDef_ptr _narrow( CORBA::Object_ptr obj );
    static WstringDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static WstringDef_ptr _duplicate( WstringDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static WstringDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::ULong bound() = 0;
    virtual void bound( CORBA::ULong value ) = 0;

  protected:
    WstringDef() {};
  private:
    WstringDef( const WstringDef& );
    void operator=( const WstringDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_WstringDef;

// Stub for interface WstringDef
class WstringDef_stub:
  virtual public WstringDef,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~WstringDef_stub();
    CORBA::ULong bound();
    void bound( CORBA::ULong value );

  private:
    void operator=( const WstringDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class WstringDef_stub_clp :
  virtual public WstringDef_stub,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    WstringDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~WstringDef_stub_clp ();
    CORBA::ULong bound();
    void bound( CORBA::ULong value );

  protected:
    WstringDef_stub_clp ();
  private:
    void operator=( const WstringDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface SequenceDef
 */

class SequenceDef : 
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~SequenceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SequenceDef_ptr _ptr_type;
    typedef SequenceDef_var _var_type;
    #endif

    static SequenceDef_ptr _narrow( CORBA::Object_ptr obj );
    static SequenceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SequenceDef_ptr _duplicate( SequenceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SequenceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::ULong bound() = 0;
    virtual void bound( CORBA::ULong value ) = 0;
    virtual CORBA::TypeCode_ptr element_type() = 0;
    virtual ::CORBA::IDLType_ptr element_type_def() = 0;
    virtual void element_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    SequenceDef() {};
  private:
    SequenceDef( const SequenceDef& );
    void operator=( const SequenceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SequenceDef;

// Stub for interface SequenceDef
class SequenceDef_stub:
  virtual public SequenceDef,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~SequenceDef_stub();
    CORBA::ULong bound();
    void bound( CORBA::ULong value );
    CORBA::TypeCode_ptr element_type();
    ::CORBA::IDLType_ptr element_type_def();
    void element_type_def( ::CORBA::IDLType_ptr value );

  private:
    void operator=( const SequenceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class SequenceDef_stub_clp :
  virtual public SequenceDef_stub,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    SequenceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~SequenceDef_stub_clp ();
    CORBA::ULong bound();
    void bound( CORBA::ULong value );
    CORBA::TypeCode_ptr element_type();
    ::CORBA::IDLType_ptr element_type_def();
    void element_type_def( ::CORBA::IDLType_ptr value );

  protected:
    SequenceDef_stub_clp ();
  private:
    void operator=( const SequenceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ArrayDef
 */

class ArrayDef : 
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~ArrayDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ArrayDef_ptr _ptr_type;
    typedef ArrayDef_var _var_type;
    #endif

    static ArrayDef_ptr _narrow( CORBA::Object_ptr obj );
    static ArrayDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ArrayDef_ptr _duplicate( ArrayDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ArrayDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::ULong length() = 0;
    virtual void length( CORBA::ULong value ) = 0;
    virtual CORBA::TypeCode_ptr element_type() = 0;
    virtual ::CORBA::IDLType_ptr element_type_def() = 0;
    virtual void element_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    ArrayDef() {};
  private:
    ArrayDef( const ArrayDef& );
    void operator=( const ArrayDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ArrayDef;

// Stub for interface ArrayDef
class ArrayDef_stub:
  virtual public ArrayDef,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~ArrayDef_stub();
    CORBA::ULong length();
    void length( CORBA::ULong value );
    CORBA::TypeCode_ptr element_type();
    ::CORBA::IDLType_ptr element_type_def();
    void element_type_def( ::CORBA::IDLType_ptr value );

  private:
    void operator=( const ArrayDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ArrayDef_stub_clp :
  virtual public ArrayDef_stub,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    ArrayDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ArrayDef_stub_clp ();
    CORBA::ULong length();
    void length( CORBA::ULong value );
    CORBA::TypeCode_ptr element_type();
    ::CORBA::IDLType_ptr element_type_def();
    void element_type_def( ::CORBA::IDLType_ptr value );

  protected:
    ArrayDef_stub_clp ();
  private:
    void operator=( const ArrayDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface FixedDef
 */

class FixedDef : 
  virtual public ::CORBA::IDLType
{
  public:
    virtual ~FixedDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef FixedDef_ptr _ptr_type;
    typedef FixedDef_var _var_type;
    #endif

    static FixedDef_ptr _narrow( CORBA::Object_ptr obj );
    static FixedDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static FixedDef_ptr _duplicate( FixedDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static FixedDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::UShort digits() = 0;
    virtual void digits( CORBA::UShort value ) = 0;
    virtual CORBA::Short scale() = 0;
    virtual void scale( CORBA::Short value ) = 0;

  protected:
    FixedDef() {};
  private:
    FixedDef( const FixedDef& );
    void operator=( const FixedDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_FixedDef;

// Stub for interface FixedDef
class FixedDef_stub:
  virtual public FixedDef,
  virtual public ::CORBA::IDLType_stub
{
  public:
    virtual ~FixedDef_stub();
    CORBA::UShort digits();
    void digits( CORBA::UShort value );
    CORBA::Short scale();
    void scale( CORBA::Short value );

  private:
    void operator=( const FixedDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class FixedDef_stub_clp :
  virtual public FixedDef_stub,
  virtual public ::CORBA::IDLType_stub_clp
{
  public:
    FixedDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~FixedDef_stub_clp ();
    CORBA::UShort digits();
    void digits( CORBA::UShort value );
    CORBA::Short scale();
    void scale( CORBA::Short value );

  protected:
    FixedDef_stub_clp ();
  private:
    void operator=( const FixedDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct ModuleDescription;
typedef TVarVar< ModuleDescription > ModuleDescription_var;
typedef TVarOut< ModuleDescription > ModuleDescription_out;


struct ModuleDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ModuleDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ModuleDescription();
  ~ModuleDescription();
  ModuleDescription( const ModuleDescription& s );
  ModuleDescription& operator=( const ModuleDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ModuleDescription;

struct ConstantDescription;
typedef TVarVar< ConstantDescription > ConstantDescription_var;
typedef TVarOut< ConstantDescription > ConstantDescription_out;


struct ConstantDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ConstantDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ConstantDescription();
  ~ConstantDescription();
  ConstantDescription( const ConstantDescription& s );
  ConstantDescription& operator=( const ConstantDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var type;
  CORBA::Any value;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ConstantDescription;

struct TypeDescription;
typedef TVarVar< TypeDescription > TypeDescription_var;
typedef TVarOut< TypeDescription > TypeDescription_out;


struct TypeDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef TypeDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TypeDescription();
  ~TypeDescription();
  TypeDescription( const TypeDescription& s );
  TypeDescription& operator=( const TypeDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var type;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TypeDescription;


/*
 * Base class and common definitions for interface AttributeDef
 */

class AttributeDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~AttributeDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AttributeDef_ptr _ptr_type;
    typedef AttributeDef_var _var_type;
    #endif

    static AttributeDef_ptr _narrow( CORBA::Object_ptr obj );
    static AttributeDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AttributeDef_ptr _duplicate( AttributeDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AttributeDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::IDLType_ptr type_def() = 0;
    virtual void type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::AttributeMode mode() = 0;
    virtual void mode( ::CORBA::AttributeMode value ) = 0;

  protected:
    AttributeDef() {};
  private:
    AttributeDef( const AttributeDef& );
    void operator=( const AttributeDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttributeDef;

// Stub for interface AttributeDef
class AttributeDef_stub:
  virtual public AttributeDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~AttributeDef_stub();
    CORBA::TypeCode_ptr type();
    ::CORBA::IDLType_ptr type_def();
    void type_def( ::CORBA::IDLType_ptr value );
    ::CORBA::AttributeMode mode();
    void mode( ::CORBA::AttributeMode value );

  private:
    void operator=( const AttributeDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class AttributeDef_stub_clp :
  virtual public AttributeDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    AttributeDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~AttributeDef_stub_clp ();
    CORBA::TypeCode_ptr type();
    ::CORBA::IDLType_ptr type_def();
    void type_def( ::CORBA::IDLType_ptr value );
    ::CORBA::AttributeMode mode();
    void mode( ::CORBA::AttributeMode value );

  protected:
    AttributeDef_stub_clp ();
  private:
    void operator=( const AttributeDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct AttributeDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef AttributeDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AttributeDescription();
  ~AttributeDescription();
  AttributeDescription( const AttributeDescription& s );
  AttributeDescription& operator=( const AttributeDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var type;
  AttributeMode mode;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttributeDescription;

struct ExtAttributeDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ExtAttributeDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ExtAttributeDescription();
  ~ExtAttributeDescription();
  ExtAttributeDescription( const ExtAttributeDescription& s );
  ExtAttributeDescription& operator=( const ExtAttributeDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var type;
  AttributeMode mode;
  ExcDescriptionSeq get_exceptions;
  ExcDescriptionSeq set_exceptions;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtAttributeDescription;


/*
 * Base class and common definitions for interface ExtAttributeDef
 */

class ExtAttributeDef : 
  virtual public ::CORBA::AttributeDef
{
  public:
    virtual ~ExtAttributeDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExtAttributeDef_ptr _ptr_type;
    typedef ExtAttributeDef_var _var_type;
    #endif

    static ExtAttributeDef_ptr _narrow( CORBA::Object_ptr obj );
    static ExtAttributeDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExtAttributeDef_ptr _duplicate( ExtAttributeDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExtAttributeDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::ExceptionDefSeq* get_exceptions() = 0;
    virtual void get_exceptions( const ::CORBA::ExceptionDefSeq& value ) = 0;
    virtual ::CORBA::ExceptionDefSeq* set_exceptions() = 0;
    virtual void set_exceptions( const ::CORBA::ExceptionDefSeq& value ) = 0;

    virtual ExtAttributeDescription* describe_attribute() = 0;

  protected:
    ExtAttributeDef() {};
  private:
    ExtAttributeDef( const ExtAttributeDef& );
    void operator=( const ExtAttributeDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtAttributeDef;

// Stub for interface ExtAttributeDef
class ExtAttributeDef_stub:
  virtual public ExtAttributeDef,
  virtual public ::CORBA::AttributeDef_stub
{
  public:
    virtual ~ExtAttributeDef_stub();
    ::CORBA::ExceptionDefSeq* get_exceptions();
    void get_exceptions( const ::CORBA::ExceptionDefSeq& value );
    ::CORBA::ExceptionDefSeq* set_exceptions();
    void set_exceptions( const ::CORBA::ExceptionDefSeq& value );

    ExtAttributeDescription* describe_attribute();

  private:
    void operator=( const ExtAttributeDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ExtAttributeDef_stub_clp :
  virtual public ExtAttributeDef_stub,
  virtual public ::CORBA::AttributeDef_stub_clp
{
  public:
    ExtAttributeDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ExtAttributeDef_stub_clp ();
    ::CORBA::ExceptionDefSeq* get_exceptions();
    void get_exceptions( const ::CORBA::ExceptionDefSeq& value );
    ::CORBA::ExceptionDefSeq* set_exceptions();
    void set_exceptions( const ::CORBA::ExceptionDefSeq& value );

    ExtAttributeDescription* describe_attribute();

  protected:
    ExtAttributeDef_stub_clp ();
  private:
    void operator=( const ExtAttributeDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

enum ParameterMode {
  PARAM_IN = 0,
  PARAM_OUT,
  PARAM_INOUT
};

typedef ParameterMode& ParameterMode_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ParameterMode;

struct ParameterDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ParameterDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ParameterDescription();
  ~ParameterDescription();
  ParameterDescription( const ParameterDescription& s );
  ParameterDescription& operator=( const ParameterDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  CORBA::TypeCode_var type;
  IDLType_var type_def;
  ParameterMode mode;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ParameterDescription;


/*
 * Base class and common definitions for interface OperationDef
 */

class OperationDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~OperationDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef OperationDef_ptr _ptr_type;
    typedef OperationDef_var _var_type;
    #endif

    static OperationDef_ptr _narrow( CORBA::Object_ptr obj );
    static OperationDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static OperationDef_ptr _duplicate( OperationDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static OperationDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr result() = 0;
    virtual ::CORBA::IDLType_ptr result_def() = 0;
    virtual void result_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::ParDescriptionSeq* params() = 0;
    virtual void params( const ::CORBA::ParDescriptionSeq& value ) = 0;
    virtual ::CORBA::OperationMode mode() = 0;
    virtual void mode( ::CORBA::OperationMode value ) = 0;
    virtual ::CORBA::ContextIdSeq* contexts() = 0;
    virtual void contexts( const ::CORBA::ContextIdSeq& value ) = 0;
    virtual ::CORBA::ExceptionDefSeq* exceptions() = 0;
    virtual void exceptions( const ::CORBA::ExceptionDefSeq& value ) = 0;

  protected:
    OperationDef() {};
  private:
    OperationDef( const OperationDef& );
    void operator=( const OperationDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OperationDef;

// Stub for interface OperationDef
class OperationDef_stub:
  virtual public OperationDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~OperationDef_stub();
    CORBA::TypeCode_ptr result();
    ::CORBA::IDLType_ptr result_def();
    void result_def( ::CORBA::IDLType_ptr value );
    ::CORBA::ParDescriptionSeq* params();
    void params( const ::CORBA::ParDescriptionSeq& value );
    ::CORBA::OperationMode mode();
    void mode( ::CORBA::OperationMode value );
    ::CORBA::ContextIdSeq* contexts();
    void contexts( const ::CORBA::ContextIdSeq& value );
    ::CORBA::ExceptionDefSeq* exceptions();
    void exceptions( const ::CORBA::ExceptionDefSeq& value );

  private:
    void operator=( const OperationDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class OperationDef_stub_clp :
  virtual public OperationDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    OperationDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~OperationDef_stub_clp ();
    CORBA::TypeCode_ptr result();
    ::CORBA::IDLType_ptr result_def();
    void result_def( ::CORBA::IDLType_ptr value );
    ::CORBA::ParDescriptionSeq* params();
    void params( const ::CORBA::ParDescriptionSeq& value );
    ::CORBA::OperationMode mode();
    void mode( ::CORBA::OperationMode value );
    ::CORBA::ContextIdSeq* contexts();
    void contexts( const ::CORBA::ContextIdSeq& value );
    ::CORBA::ExceptionDefSeq* exceptions();
    void exceptions( const ::CORBA::ExceptionDefSeq& value );

  protected:
    OperationDef_stub_clp ();
  private:
    void operator=( const OperationDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct OperationDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef OperationDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  OperationDescription();
  ~OperationDescription();
  OperationDescription( const OperationDescription& s );
  OperationDescription& operator=( const OperationDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var result;
  OperationMode mode;
  ContextIdSeq contexts;
  ParDescriptionSeq parameters;
  ExcDescriptionSeq exceptions;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OperationDescription;

struct InterfaceDescription;
typedef TVarVar< InterfaceDescription > InterfaceDescription_var;
typedef TVarOut< InterfaceDescription > InterfaceDescription_out;


struct InterfaceDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef InterfaceDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InterfaceDescription();
  ~InterfaceDescription();
  InterfaceDescription( const InterfaceDescription& s );
  InterfaceDescription& operator=( const InterfaceDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryIdSeq base_interfaces;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InterfaceDescription;


/*
 * Base class and common definitions for interface InterfaceAttrExtension
 */

class InterfaceAttrExtension : 
  virtual public CORBA::Object
{
  public:
    virtual ~InterfaceAttrExtension();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef InterfaceAttrExtension_ptr _ptr_type;
    typedef InterfaceAttrExtension_var _var_type;
    #endif

    static InterfaceAttrExtension_ptr _narrow( CORBA::Object_ptr obj );
    static InterfaceAttrExtension_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static InterfaceAttrExtension_ptr _duplicate( InterfaceAttrExtension_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static InterfaceAttrExtension_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct ExtFullInterfaceDescription;
    typedef TVarVar< ExtFullInterfaceDescription > ExtFullInterfaceDescription_var;
    typedef TVarOut< ExtFullInterfaceDescription > ExtFullInterfaceDescription_out;


    struct ExtFullInterfaceDescription {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef ExtFullInterfaceDescription_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ExtFullInterfaceDescription();
      ~ExtFullInterfaceDescription();
      ExtFullInterfaceDescription( const ExtFullInterfaceDescription& s );
      ExtFullInterfaceDescription& operator=( const ExtFullInterfaceDescription& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      Identifier_var name;
      RepositoryId_var id;
      RepositoryId_var defined_in;
      VersionSpec_var version;
      OpDescriptionSeq operations;
      ExtAttrDescriptionSeq attributes;
      RepositoryIdSeq base_interfaces;
      CORBA::TypeCode_var type;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ExtFullInterfaceDescription;

    virtual ExtFullInterfaceDescription* describe_ext_interface() = 0;
    virtual ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode, const ExceptionDefSeq& get_exceptions, const ExceptionDefSeq& set_exceptions ) = 0;

  protected:
    InterfaceAttrExtension() {};
  private:
    InterfaceAttrExtension( const InterfaceAttrExtension& );
    void operator=( const InterfaceAttrExtension& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InterfaceAttrExtension;

// Stub for interface InterfaceAttrExtension
class InterfaceAttrExtension_stub:
  virtual public InterfaceAttrExtension
{
  public:
    virtual ~InterfaceAttrExtension_stub();
    ExtFullInterfaceDescription* describe_ext_interface();
    ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode, const ExceptionDefSeq& get_exceptions, const ExceptionDefSeq& set_exceptions );

  private:
    void operator=( const InterfaceAttrExtension_stub& );
};

#ifndef MICO_CONF_NO_POA

class InterfaceAttrExtension_stub_clp :
  virtual public InterfaceAttrExtension_stub,
  virtual public PortableServer::StubBase
{
  public:
    InterfaceAttrExtension_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~InterfaceAttrExtension_stub_clp ();
    ExtFullInterfaceDescription* describe_ext_interface();
    ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, IDLType_ptr type, AttributeMode mode, const ExceptionDefSeq& get_exceptions, const ExceptionDefSeq& set_exceptions );

  protected:
    InterfaceAttrExtension_stub_clp ();
  private:
    void operator=( const InterfaceAttrExtension_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ExtInterfaceDef
 */

class ExtInterfaceDef : 
  virtual public ::CORBA::InterfaceDef,
  virtual public ::CORBA::InterfaceAttrExtension
{
  public:
    virtual ~ExtInterfaceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExtInterfaceDef_ptr _ptr_type;
    typedef ExtInterfaceDef_var _var_type;
    #endif

    static ExtInterfaceDef_ptr _narrow( CORBA::Object_ptr obj );
    static ExtInterfaceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExtInterfaceDef_ptr _duplicate( ExtInterfaceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExtInterfaceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    ExtInterfaceDef() {};
  private:
    ExtInterfaceDef( const ExtInterfaceDef& );
    void operator=( const ExtInterfaceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtInterfaceDef;

// Stub for interface ExtInterfaceDef
class ExtInterfaceDef_stub:
  virtual public ExtInterfaceDef,
  virtual public ::CORBA::InterfaceDef_stub,
  virtual public ::CORBA::InterfaceAttrExtension_stub
{
  public:
    virtual ~ExtInterfaceDef_stub();
  private:
    void operator=( const ExtInterfaceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ExtInterfaceDef_stub_clp :
  virtual public ExtInterfaceDef_stub,
  virtual public ::CORBA::InterfaceDef_stub_clp,
  virtual public ::CORBA::InterfaceAttrExtension_stub_clp
{
  public:
    ExtInterfaceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ExtInterfaceDef_stub_clp ();
  protected:
    ExtInterfaceDef_stub_clp ();
  private:
    void operator=( const ExtInterfaceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ExtAbstractInterfaceDef
 */

class ExtAbstractInterfaceDef : 
  virtual public ::CORBA::ExtInterfaceDef,
  virtual public ::CORBA::AbstractInterfaceDef,
  virtual public ::CORBA::InterfaceAttrExtension
{
  public:
    virtual ~ExtAbstractInterfaceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExtAbstractInterfaceDef_ptr _ptr_type;
    typedef ExtAbstractInterfaceDef_var _var_type;
    #endif

    static ExtAbstractInterfaceDef_ptr _narrow( CORBA::Object_ptr obj );
    static ExtAbstractInterfaceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExtAbstractInterfaceDef_ptr _duplicate( ExtAbstractInterfaceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExtAbstractInterfaceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    ExtAbstractInterfaceDef() {};
  private:
    ExtAbstractInterfaceDef( const ExtAbstractInterfaceDef& );
    void operator=( const ExtAbstractInterfaceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtAbstractInterfaceDef;

// Stub for interface ExtAbstractInterfaceDef
class ExtAbstractInterfaceDef_stub:
  virtual public ExtAbstractInterfaceDef,
  virtual public ::CORBA::ExtInterfaceDef_stub,
  virtual public ::CORBA::AbstractInterfaceDef_stub,
  virtual public ::CORBA::InterfaceAttrExtension_stub
{
  public:
    virtual ~ExtAbstractInterfaceDef_stub();
  private:
    void operator=( const ExtAbstractInterfaceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ExtAbstractInterfaceDef_stub_clp :
  virtual public ExtAbstractInterfaceDef_stub,
  virtual public ::CORBA::ExtInterfaceDef_stub_clp,
  virtual public ::CORBA::AbstractInterfaceDef_stub_clp,
  virtual public ::CORBA::InterfaceAttrExtension_stub_clp
{
  public:
    ExtAbstractInterfaceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ExtAbstractInterfaceDef_stub_clp ();
  protected:
    ExtAbstractInterfaceDef_stub_clp ();
  private:
    void operator=( const ExtAbstractInterfaceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ExtLocalInterfaceDef
 */

class ExtLocalInterfaceDef : 
  virtual public ::CORBA::ExtInterfaceDef,
  virtual public ::CORBA::LocalInterfaceDef,
  virtual public ::CORBA::InterfaceAttrExtension
{
  public:
    virtual ~ExtLocalInterfaceDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExtLocalInterfaceDef_ptr _ptr_type;
    typedef ExtLocalInterfaceDef_var _var_type;
    #endif

    static ExtLocalInterfaceDef_ptr _narrow( CORBA::Object_ptr obj );
    static ExtLocalInterfaceDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExtLocalInterfaceDef_ptr _duplicate( ExtLocalInterfaceDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExtLocalInterfaceDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    ExtLocalInterfaceDef() {};
  private:
    ExtLocalInterfaceDef( const ExtLocalInterfaceDef& );
    void operator=( const ExtLocalInterfaceDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtLocalInterfaceDef;

// Stub for interface ExtLocalInterfaceDef
class ExtLocalInterfaceDef_stub:
  virtual public ExtLocalInterfaceDef,
  virtual public ::CORBA::ExtInterfaceDef_stub,
  virtual public ::CORBA::LocalInterfaceDef_stub,
  virtual public ::CORBA::InterfaceAttrExtension_stub
{
  public:
    virtual ~ExtLocalInterfaceDef_stub();
  private:
    void operator=( const ExtLocalInterfaceDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ExtLocalInterfaceDef_stub_clp :
  virtual public ExtLocalInterfaceDef_stub,
  virtual public ::CORBA::ExtInterfaceDef_stub_clp,
  virtual public ::CORBA::LocalInterfaceDef_stub_clp,
  virtual public ::CORBA::InterfaceAttrExtension_stub_clp
{
  public:
    ExtLocalInterfaceDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ExtLocalInterfaceDef_stub_clp ();
  protected:
    ExtLocalInterfaceDef_stub_clp ();
  private:
    void operator=( const ExtLocalInterfaceDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ValueMemberDef
 */

class ValueMemberDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~ValueMemberDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ValueMemberDef_ptr _ptr_type;
    typedef ValueMemberDef_var _var_type;
    #endif

    static ValueMemberDef_ptr _narrow( CORBA::Object_ptr obj );
    static ValueMemberDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ValueMemberDef_ptr _duplicate( ValueMemberDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ValueMemberDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::IDLType_ptr type_def() = 0;
    virtual void type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::Visibility access() = 0;
    virtual void access( ::CORBA::Visibility value ) = 0;

  protected:
    ValueMemberDef() {};
  private:
    ValueMemberDef( const ValueMemberDef& );
    void operator=( const ValueMemberDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueMemberDef;

// Stub for interface ValueMemberDef
class ValueMemberDef_stub:
  virtual public ValueMemberDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~ValueMemberDef_stub();
    CORBA::TypeCode_ptr type();
    ::CORBA::IDLType_ptr type_def();
    void type_def( ::CORBA::IDLType_ptr value );
    ::CORBA::Visibility access();
    void access( ::CORBA::Visibility value );

  private:
    void operator=( const ValueMemberDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ValueMemberDef_stub_clp :
  virtual public ValueMemberDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    ValueMemberDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ValueMemberDef_stub_clp ();
    CORBA::TypeCode_ptr type();
    ::CORBA::IDLType_ptr type_def();
    void type_def( ::CORBA::IDLType_ptr value );
    ::CORBA::Visibility access();
    void access( ::CORBA::Visibility value );

  protected:
    ValueMemberDef_stub_clp ();
  private:
    void operator=( const ValueMemberDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct ValueDescription;
typedef TVarVar< ValueDescription > ValueDescription_var;
typedef TVarOut< ValueDescription > ValueDescription_out;


struct ValueDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ValueDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ValueDescription();
  ~ValueDescription();
  ValueDescription( const ValueDescription& s );
  ValueDescription& operator=( const ValueDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  CORBA::Boolean is_abstract;
  CORBA::Boolean is_custom;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryIdSeq supported_interfaces;
  RepositoryIdSeq abstract_base_values;
  CORBA::Boolean is_truncatable;
  RepositoryId_var base_value;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueDescription;

}


#ifndef MICO_CONF_NO_POA



namespace POA_CORBA
{

class Contained : 
  virtual public POA_CORBA::IRObject
{
  public:
    virtual ~Contained ();
    CORBA::Contained_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Contained * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* id() = 0;
    virtual void id( const char* value ) = 0;
    virtual char* name() = 0;
    virtual void name( const char* value ) = 0;
    virtual char* version() = 0;
    virtual void version( const char* value ) = 0;
    virtual ::CORBA::Container_ptr defined_in() = 0;
    virtual char* absolute_name() = 0;
    virtual ::CORBA::Repository_ptr containing_repository() = 0;

    virtual ::CORBA::Contained::Description* describe() = 0;
    virtual void move( ::CORBA::Container_ptr new_container, const char* new_name, const char* new_version ) = 0;

  protected:
    Contained () {};

  private:
    Contained (const Contained &);
    void operator= (const Contained &);
};

class Container : 
  virtual public POA_CORBA::IRObject
{
  public:
    virtual ~Container ();
    CORBA::Container_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Container * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::Contained_ptr lookup( const char* search_name ) = 0;
    virtual ::CORBA::ContainedSeq* contents( ::CORBA::DefinitionKind limit_type, CORBA::Boolean exclude_inherited ) = 0;
    virtual ::CORBA::ContainedSeq* lookup_name( const char* search_name, CORBA::Long levels_to_search, ::CORBA::DefinitionKind limit_type, CORBA::Boolean exclude_inherited ) = 0;
    virtual ::CORBA::Container::DescriptionSeq* describe_contents( ::CORBA::DefinitionKind limit_type, CORBA::Boolean exclude_inherited, CORBA::Long max_returned_objs ) = 0;
    virtual ::CORBA::ModuleDef_ptr create_module( const char* id, const char* name, const char* version ) = 0;
    virtual ::CORBA::ConstantDef_ptr create_constant( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr type, const CORBA::Any& value ) = 0;
    virtual ::CORBA::StructDef_ptr create_struct( const char* id, const char* name, const char* version, const ::CORBA::StructMemberSeq& members ) = 0;
    virtual ::CORBA::ExceptionDef_ptr create_exception( const char* id, const char* name, const char* version, const ::CORBA::StructMemberSeq& members ) = 0;
    virtual ::CORBA::UnionDef_ptr create_union( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr discriminator_type, const ::CORBA::UnionMemberSeq& members ) = 0;
    virtual ::CORBA::EnumDef_ptr create_enum( const char* id, const char* name, const char* version, const ::CORBA::EnumMemberSeq& members ) = 0;
    virtual ::CORBA::AliasDef_ptr create_alias( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr original_type ) = 0;
    virtual ::CORBA::InterfaceDef_ptr create_interface( const char* id, const char* name, const char* version, const ::CORBA::InterfaceDefSeq& base_interfaces ) = 0;
    virtual ::CORBA::AbstractInterfaceDef_ptr create_abstract_interface( const char* id, const char* name, const char* version, const ::CORBA::AbstractInterfaceDefSeq& base_interfaces ) = 0;
    virtual ::CORBA::LocalInterfaceDef_ptr create_local_interface( const char* id, const char* name, const char* version, const ::CORBA::InterfaceDefSeq& base_interfaces ) = 0;
    virtual ::CORBA::ValueDef_ptr create_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ::CORBA::ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ::CORBA::ValueDefSeq& abstract_base_values, const ::CORBA::InterfaceDefSeq& supported_interfaces, const ::CORBA::InitializerSeq& initializers ) = 0;
    virtual ::CORBA::ExtValueDef_ptr create_ext_value( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ::CORBA::ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ::CORBA::ValueDefSeq& abstract_base_values, const ::CORBA::InterfaceDefSeq& supported_interfaces, const ::CORBA::ExtInitializerSeq& initializers ) = 0;
    virtual ::CORBA::ValueBoxDef_ptr create_value_box( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr original_type_def ) = 0;
    virtual ::CORBA::NativeDef_ptr create_native( const char* id, const char* name, const char* version ) = 0;

  protected:
    Container () {};

  private:
    Container (const Container &);
    void operator= (const Container &);
};

class Repository : 
  virtual public POA_CORBA::Container
{
  public:
    virtual ~Repository ();
    CORBA::Repository_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Repository * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::Contained_ptr lookup_id( const char* search_id ) = 0;
    virtual ::CORBA::PrimitiveDef_ptr get_primitive( ::CORBA::PrimitiveKind kind ) = 0;
    virtual ::CORBA::StringDef_ptr create_string( CORBA::ULong bound ) = 0;
    virtual ::CORBA::WstringDef_ptr create_wstring( CORBA::ULong bound ) = 0;
    virtual ::CORBA::SequenceDef_ptr create_sequence( CORBA::ULong bound, ::CORBA::IDLType_ptr element_type ) = 0;
    virtual ::CORBA::ArrayDef_ptr create_array( CORBA::ULong length, ::CORBA::IDLType_ptr element_type ) = 0;
    virtual ::CORBA::FixedDef_ptr create_fixed( CORBA::UShort digits, CORBA::Short scale ) = 0;

  protected:
    Repository () {};

  private:
    Repository (const Repository &);
    void operator= (const Repository &);
};

class ModuleDef : 
  virtual public POA_CORBA::Container,
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~ModuleDef ();
    CORBA::ModuleDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ModuleDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    ModuleDef () {};

  private:
    ModuleDef (const ModuleDef &);
    void operator= (const ModuleDef &);
};

class ConstantDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~ConstantDef ();
    CORBA::ConstantDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ConstantDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::IDLType_ptr type_def() = 0;
    virtual void type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual CORBA::Any* value() = 0;
    virtual void value( const CORBA::Any& value ) = 0;

  protected:
    ConstantDef () {};

  private:
    ConstantDef (const ConstantDef &);
    void operator= (const ConstantDef &);
};

class TypedefDef : 
  virtual public POA_CORBA::Contained,
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~TypedefDef ();
    CORBA::TypedefDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static TypedefDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    TypedefDef () {};

  private:
    TypedefDef (const TypedefDef &);
    void operator= (const TypedefDef &);
};

class StructDef : 
  virtual public POA_CORBA::TypedefDef,
  virtual public POA_CORBA::Container
{
  public:
    virtual ~StructDef ();
    CORBA::StructDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static StructDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::StructMemberSeq* members() = 0;
    virtual void members( const ::CORBA::StructMemberSeq& value ) = 0;

  protected:
    StructDef () {};

  private:
    StructDef (const StructDef &);
    void operator= (const StructDef &);
};

class ExceptionDef : 
  virtual public POA_CORBA::Contained,
  virtual public POA_CORBA::Container
{
  public:
    virtual ~ExceptionDef ();
    CORBA::ExceptionDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ExceptionDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::StructMemberSeq* members() = 0;
    virtual void members( const ::CORBA::StructMemberSeq& value ) = 0;

  protected:
    ExceptionDef () {};

  private:
    ExceptionDef (const ExceptionDef &);
    void operator= (const ExceptionDef &);
};

class UnionDef : 
  virtual public POA_CORBA::TypedefDef,
  virtual public POA_CORBA::Container
{
  public:
    virtual ~UnionDef ();
    CORBA::UnionDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static UnionDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr discriminator_type() = 0;
    virtual ::CORBA::IDLType_ptr discriminator_type_def() = 0;
    virtual void discriminator_type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::UnionMemberSeq* members() = 0;
    virtual void members( const ::CORBA::UnionMemberSeq& value ) = 0;

  protected:
    UnionDef () {};

  private:
    UnionDef (const UnionDef &);
    void operator= (const UnionDef &);
};

class EnumDef : 
  virtual public POA_CORBA::TypedefDef
{
  public:
    virtual ~EnumDef ();
    CORBA::EnumDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static EnumDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::EnumMemberSeq* members() = 0;
    virtual void members( const ::CORBA::EnumMemberSeq& value ) = 0;

  protected:
    EnumDef () {};

  private:
    EnumDef (const EnumDef &);
    void operator= (const EnumDef &);
};

class AliasDef : 
  virtual public POA_CORBA::TypedefDef
{
  public:
    virtual ~AliasDef ();
    CORBA::AliasDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static AliasDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::IDLType_ptr original_type_def() = 0;
    virtual void original_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    AliasDef () {};

  private:
    AliasDef (const AliasDef &);
    void operator= (const AliasDef &);
};

class InterfaceDef : 
  virtual public POA_CORBA::Container,
  virtual public POA_CORBA::Contained,
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~InterfaceDef ();
    CORBA::InterfaceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static InterfaceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::InterfaceDefSeq* base_interfaces() = 0;
    virtual void base_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;

    virtual CORBA::Boolean is_a( const char* interface_id ) = 0;
    virtual ::CORBA::InterfaceDef::FullInterfaceDescription* describe_interface() = 0;
    virtual ::CORBA::AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr type, ::CORBA::AttributeMode mode ) = 0;
    virtual ::CORBA::OperationDef_ptr create_operation( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr result, ::CORBA::OperationMode mode, const ::CORBA::ParDescriptionSeq& params, const ::CORBA::ExceptionDefSeq& exceptions, const ::CORBA::ContextIdSeq& contexts ) = 0;

  protected:
    InterfaceDef () {};

  private:
    InterfaceDef (const InterfaceDef &);
    void operator= (const InterfaceDef &);
};

class AbstractInterfaceDef : 
  virtual public POA_CORBA::InterfaceDef
{
  public:
    virtual ~AbstractInterfaceDef ();
    CORBA::AbstractInterfaceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static AbstractInterfaceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    AbstractInterfaceDef () {};

  private:
    AbstractInterfaceDef (const AbstractInterfaceDef &);
    void operator= (const AbstractInterfaceDef &);
};

class LocalInterfaceDef : 
  virtual public POA_CORBA::InterfaceDef
{
  public:
    virtual ~LocalInterfaceDef ();
    CORBA::LocalInterfaceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static LocalInterfaceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    LocalInterfaceDef () {};

  private:
    LocalInterfaceDef (const LocalInterfaceDef &);
    void operator= (const LocalInterfaceDef &);
};

class ValueDef : 
  virtual public POA_CORBA::Container,
  virtual public POA_CORBA::Contained,
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~ValueDef ();
    CORBA::ValueDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ValueDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::InterfaceDefSeq* supported_interfaces() = 0;
    virtual void supported_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;
    virtual ::CORBA::InitializerSeq* initializers() = 0;
    virtual void initializers( const ::CORBA::InitializerSeq& value ) = 0;
    virtual ::CORBA::ValueDef_ptr base_value() = 0;
    virtual void base_value( ::CORBA::ValueDef_ptr value ) = 0;
    virtual ::CORBA::ValueDefSeq* abstract_base_values() = 0;
    virtual void abstract_base_values( const ::CORBA::ValueDefSeq& value ) = 0;
    virtual CORBA::Boolean is_abstract() = 0;
    virtual void is_abstract( CORBA::Boolean value ) = 0;
    virtual CORBA::Boolean is_custom() = 0;
    virtual void is_custom( CORBA::Boolean value ) = 0;
    virtual CORBA::Boolean is_truncatable() = 0;
    virtual void is_truncatable( CORBA::Boolean value ) = 0;

    virtual CORBA::Boolean is_a( const char* value_id ) = 0;
    virtual ::CORBA::ValueDef::FullValueDescription* describe_value() = 0;
    virtual ::CORBA::ValueMemberDef_ptr create_value_member( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr type, ::CORBA::Visibility access ) = 0;
    virtual ::CORBA::AttributeDef_ptr create_attribute( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr type, ::CORBA::AttributeMode mode ) = 0;
    virtual ::CORBA::OperationDef_ptr create_operation( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr result, ::CORBA::OperationMode mode, const ::CORBA::ParDescriptionSeq& params, const ::CORBA::ExceptionDefSeq& exceptions, const ::CORBA::ContextIdSeq& contexts ) = 0;

  protected:
    ValueDef () {};

  private:
    ValueDef (const ValueDef &);
    void operator= (const ValueDef &);
};

class ValueBoxDef : 
  virtual public POA_CORBA::TypedefDef
{
  public:
    virtual ~ValueBoxDef ();
    CORBA::ValueBoxDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ValueBoxDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::IDLType_ptr original_type_def() = 0;
    virtual void original_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    ValueBoxDef () {};

  private:
    ValueBoxDef (const ValueBoxDef &);
    void operator= (const ValueBoxDef &);
};

class NativeDef : 
  virtual public POA_CORBA::TypedefDef
{
  public:
    virtual ~NativeDef ();
    CORBA::NativeDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static NativeDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    NativeDef () {};

  private:
    NativeDef (const NativeDef &);
    void operator= (const NativeDef &);
};

class ExtValueDef : 
  virtual public POA_CORBA::ValueDef
{
  public:
    virtual ~ExtValueDef ();
    CORBA::ExtValueDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ExtValueDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ExtInitializerSeq* ext_initializers() = 0;
    virtual void ext_initializers( const ::CORBA::ExtInitializerSeq& value ) = 0;

    virtual ::CORBA::ExtValueDef::ExtFullValueDescription* describe_ext_value() = 0;
    virtual ::CORBA::ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr type, ::CORBA::AttributeMode mode, const ::CORBA::ExceptionDefSeq& get_exceptions, const ::CORBA::ExceptionDefSeq& set_exceptions ) = 0;

  protected:
    ExtValueDef () {};

  private:
    ExtValueDef (const ExtValueDef &);
    void operator= (const ExtValueDef &);
};

class PrimitiveDef : 
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~PrimitiveDef ();
    CORBA::PrimitiveDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static PrimitiveDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::PrimitiveKind kind() = 0;

  protected:
    PrimitiveDef () {};

  private:
    PrimitiveDef (const PrimitiveDef &);
    void operator= (const PrimitiveDef &);
};

class StringDef : 
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~StringDef ();
    CORBA::StringDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static StringDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::ULong bound() = 0;
    virtual void bound( CORBA::ULong value ) = 0;

  protected:
    StringDef () {};

  private:
    StringDef (const StringDef &);
    void operator= (const StringDef &);
};

class WstringDef : 
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~WstringDef ();
    CORBA::WstringDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static WstringDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::ULong bound() = 0;
    virtual void bound( CORBA::ULong value ) = 0;

  protected:
    WstringDef () {};

  private:
    WstringDef (const WstringDef &);
    void operator= (const WstringDef &);
};

class SequenceDef : 
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~SequenceDef ();
    CORBA::SequenceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static SequenceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::ULong bound() = 0;
    virtual void bound( CORBA::ULong value ) = 0;
    virtual CORBA::TypeCode_ptr element_type() = 0;
    virtual ::CORBA::IDLType_ptr element_type_def() = 0;
    virtual void element_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    SequenceDef () {};

  private:
    SequenceDef (const SequenceDef &);
    void operator= (const SequenceDef &);
};

class ArrayDef : 
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~ArrayDef ();
    CORBA::ArrayDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ArrayDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::ULong length() = 0;
    virtual void length( CORBA::ULong value ) = 0;
    virtual CORBA::TypeCode_ptr element_type() = 0;
    virtual ::CORBA::IDLType_ptr element_type_def() = 0;
    virtual void element_type_def( ::CORBA::IDLType_ptr value ) = 0;

  protected:
    ArrayDef () {};

  private:
    ArrayDef (const ArrayDef &);
    void operator= (const ArrayDef &);
};

class FixedDef : 
  virtual public POA_CORBA::IDLType
{
  public:
    virtual ~FixedDef ();
    CORBA::FixedDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static FixedDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::UShort digits() = 0;
    virtual void digits( CORBA::UShort value ) = 0;
    virtual CORBA::Short scale() = 0;
    virtual void scale( CORBA::Short value ) = 0;

  protected:
    FixedDef () {};

  private:
    FixedDef (const FixedDef &);
    void operator= (const FixedDef &);
};

class AttributeDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~AttributeDef ();
    CORBA::AttributeDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static AttributeDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::IDLType_ptr type_def() = 0;
    virtual void type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::AttributeMode mode() = 0;
    virtual void mode( ::CORBA::AttributeMode value ) = 0;

  protected:
    AttributeDef () {};

  private:
    AttributeDef (const AttributeDef &);
    void operator= (const AttributeDef &);
};

class ExtAttributeDef : 
  virtual public POA_CORBA::AttributeDef
{
  public:
    virtual ~ExtAttributeDef ();
    CORBA::ExtAttributeDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ExtAttributeDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ExceptionDefSeq* get_exceptions() = 0;
    virtual void get_exceptions( const ::CORBA::ExceptionDefSeq& value ) = 0;
    virtual ::CORBA::ExceptionDefSeq* set_exceptions() = 0;
    virtual void set_exceptions( const ::CORBA::ExceptionDefSeq& value ) = 0;

    virtual ::CORBA::ExtAttributeDescription* describe_attribute() = 0;

  protected:
    ExtAttributeDef () {};

  private:
    ExtAttributeDef (const ExtAttributeDef &);
    void operator= (const ExtAttributeDef &);
};

class OperationDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~OperationDef ();
    CORBA::OperationDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static OperationDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr result() = 0;
    virtual ::CORBA::IDLType_ptr result_def() = 0;
    virtual void result_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::ParDescriptionSeq* params() = 0;
    virtual void params( const ::CORBA::ParDescriptionSeq& value ) = 0;
    virtual ::CORBA::OperationMode mode() = 0;
    virtual void mode( ::CORBA::OperationMode value ) = 0;
    virtual ::CORBA::ContextIdSeq* contexts() = 0;
    virtual void contexts( const ::CORBA::ContextIdSeq& value ) = 0;
    virtual ::CORBA::ExceptionDefSeq* exceptions() = 0;
    virtual void exceptions( const ::CORBA::ExceptionDefSeq& value ) = 0;

  protected:
    OperationDef () {};

  private:
    OperationDef (const OperationDef &);
    void operator= (const OperationDef &);
};

class InterfaceAttrExtension : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~InterfaceAttrExtension ();
    CORBA::InterfaceAttrExtension_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static InterfaceAttrExtension * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription* describe_ext_interface() = 0;
    virtual ::CORBA::ExtAttributeDef_ptr create_ext_attribute( const char* id, const char* name, const char* version, ::CORBA::IDLType_ptr type, ::CORBA::AttributeMode mode, const ::CORBA::ExceptionDefSeq& get_exceptions, const ::CORBA::ExceptionDefSeq& set_exceptions ) = 0;

  protected:
    InterfaceAttrExtension () {};

  private:
    InterfaceAttrExtension (const InterfaceAttrExtension &);
    void operator= (const InterfaceAttrExtension &);
};

class ExtInterfaceDef : 
  virtual public POA_CORBA::InterfaceDef,
  virtual public POA_CORBA::InterfaceAttrExtension
{
  public:
    virtual ~ExtInterfaceDef ();
    CORBA::ExtInterfaceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ExtInterfaceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    ExtInterfaceDef () {};

  private:
    ExtInterfaceDef (const ExtInterfaceDef &);
    void operator= (const ExtInterfaceDef &);
};

class ExtAbstractInterfaceDef : 
  virtual public POA_CORBA::ExtInterfaceDef,
  virtual public POA_CORBA::AbstractInterfaceDef,
  virtual public POA_CORBA::InterfaceAttrExtension
{
  public:
    virtual ~ExtAbstractInterfaceDef ();
    CORBA::ExtAbstractInterfaceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ExtAbstractInterfaceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    ExtAbstractInterfaceDef () {};

  private:
    ExtAbstractInterfaceDef (const ExtAbstractInterfaceDef &);
    void operator= (const ExtAbstractInterfaceDef &);
};

class ExtLocalInterfaceDef : 
  virtual public POA_CORBA::ExtInterfaceDef,
  virtual public POA_CORBA::LocalInterfaceDef,
  virtual public POA_CORBA::InterfaceAttrExtension
{
  public:
    virtual ~ExtLocalInterfaceDef ();
    CORBA::ExtLocalInterfaceDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ExtLocalInterfaceDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    ExtLocalInterfaceDef () {};

  private:
    ExtLocalInterfaceDef (const ExtLocalInterfaceDef &);
    void operator= (const ExtLocalInterfaceDef &);
};

class ValueMemberDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~ValueMemberDef ();
    CORBA::ValueMemberDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ValueMemberDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr type() = 0;
    virtual ::CORBA::IDLType_ptr type_def() = 0;
    virtual void type_def( ::CORBA::IDLType_ptr value ) = 0;
    virtual ::CORBA::Visibility access() = 0;
    virtual void access( ::CORBA::Visibility value ) = 0;

  protected:
    ValueMemberDef () {};

  private:
    ValueMemberDef (const ValueMemberDef &);
    void operator= (const ValueMemberDef &);
};

}


#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::CORBA::Contained::Description &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::Contained::Description *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::Contained::Description &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::Contained::Description *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_Contained_Description;

void operator<<=( CORBA::Any &a, const CORBA::Contained_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::Contained_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::Contained_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_Contained;

void operator<<=( CORBA::Any &_a, const ::CORBA::Container::Description &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::Container::Description *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::Container::Description &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::Container::Description *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_Container_Description;

void operator<<=( CORBA::Any &a, const CORBA::Container_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::Container_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::Container_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_Container;

void operator<<=( CORBA::Any &a, const ::CORBA::PrimitiveKind &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::CORBA::PrimitiveKind &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_PrimitiveKind;

void operator<<=( CORBA::Any &a, const CORBA::Repository_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::Repository_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::Repository_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_Repository;

void operator<<=( CORBA::Any &a, const CORBA::ModuleDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ModuleDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ModuleDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ModuleDef;

void operator<<=( CORBA::Any &a, const CORBA::ConstantDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ConstantDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ConstantDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ConstantDef;

void operator<<=( CORBA::Any &a, const CORBA::TypedefDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::TypedefDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::TypedefDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_TypedefDef;

void operator<<=( CORBA::Any &a, const CORBA::StructDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::StructDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::StructDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_StructDef;

void operator<<=( CORBA::Any &a, const CORBA::ExceptionDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ExceptionDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ExceptionDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExceptionDef;

void operator<<=( CORBA::Any &a, const CORBA::UnionDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::UnionDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::UnionDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_UnionDef;

void operator<<=( CORBA::Any &a, const CORBA::EnumDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::EnumDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::EnumDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_EnumDef;

void operator<<=( CORBA::Any &a, const CORBA::AliasDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::AliasDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::AliasDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_AliasDef;

void operator<<=( CORBA::Any &a, const ::CORBA::AttributeMode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::CORBA::AttributeMode &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_AttributeMode;

void operator<<=( CORBA::Any &a, const ::CORBA::OperationMode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::CORBA::OperationMode &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_OperationMode;

void operator<<=( CORBA::Any &_a, const ::CORBA::InterfaceDef::FullInterfaceDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::InterfaceDef::FullInterfaceDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::InterfaceDef::FullInterfaceDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::InterfaceDef::FullInterfaceDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceDef_FullInterfaceDescription;

void operator<<=( CORBA::Any &a, const CORBA::InterfaceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::InterfaceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::InterfaceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceDef;

void operator<<=( CORBA::Any &a, const CORBA::AbstractInterfaceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::AbstractInterfaceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::AbstractInterfaceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_AbstractInterfaceDef;

void operator<<=( CORBA::Any &a, const CORBA::LocalInterfaceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::LocalInterfaceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::LocalInterfaceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_LocalInterfaceDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ValueDef::FullValueDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ValueDef::FullValueDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ValueDef::FullValueDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ValueDef::FullValueDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ValueDef_FullValueDescription;

void operator<<=( CORBA::Any &a, const CORBA::ValueDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ValueDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ValueDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ValueDef;

void operator<<=( CORBA::Any &a, const CORBA::ValueBoxDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ValueBoxDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ValueBoxDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ValueBoxDef;

void operator<<=( CORBA::Any &a, const CORBA::NativeDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::NativeDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::NativeDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_NativeDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ExtValueDef::ExtFullValueDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ExtValueDef::ExtFullValueDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ExtValueDef::ExtFullValueDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ExtValueDef::ExtFullValueDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtValueDef_ExtFullValueDescription;

void operator<<=( CORBA::Any &a, const CORBA::ExtValueDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ExtValueDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ExtValueDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtValueDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ExceptionDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ExceptionDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ExceptionDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ExceptionDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExceptionDescription;

void operator<<=( CORBA::Any &_a, const ::CORBA::Initializer &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::Initializer *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::Initializer &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::Initializer *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_Initializer;

void operator<<=( CORBA::Any &_a, const ::CORBA::ExtInitializer &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ExtInitializer *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ExtInitializer &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ExtInitializer *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtInitializer;

void operator<<=( CORBA::Any &a, const CORBA::PrimitiveDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::PrimitiveDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::PrimitiveDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_PrimitiveDef;

void operator<<=( CORBA::Any &a, const CORBA::StringDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::StringDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::StringDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_StringDef;

void operator<<=( CORBA::Any &a, const CORBA::WstringDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::WstringDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::WstringDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_WstringDef;

void operator<<=( CORBA::Any &a, const CORBA::SequenceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::SequenceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::SequenceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_SequenceDef;

void operator<<=( CORBA::Any &a, const CORBA::ArrayDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ArrayDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ArrayDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ArrayDef;

void operator<<=( CORBA::Any &a, const CORBA::FixedDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::FixedDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::FixedDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_FixedDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ModuleDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ModuleDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ModuleDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ModuleDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ModuleDescription;

void operator<<=( CORBA::Any &_a, const ::CORBA::ConstantDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ConstantDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ConstantDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ConstantDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ConstantDescription;

void operator<<=( CORBA::Any &_a, const ::CORBA::TypeDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::TypeDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::TypeDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::TypeDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_TypeDescription;

void operator<<=( CORBA::Any &a, const CORBA::AttributeDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::AttributeDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::AttributeDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_AttributeDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::AttributeDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::AttributeDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::AttributeDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::AttributeDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_AttributeDescription;

void operator<<=( CORBA::Any &_a, const ::CORBA::ExtAttributeDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ExtAttributeDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ExtAttributeDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ExtAttributeDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtAttributeDescription;

void operator<<=( CORBA::Any &a, const CORBA::ExtAttributeDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ExtAttributeDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ExtAttributeDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtAttributeDef;

void operator<<=( CORBA::Any &a, const ::CORBA::ParameterMode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::CORBA::ParameterMode &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ParameterMode;

void operator<<=( CORBA::Any &_a, const ::CORBA::ParameterDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ParameterDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ParameterDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ParameterDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ParameterDescription;

void operator<<=( CORBA::Any &a, const CORBA::OperationDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::OperationDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::OperationDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_OperationDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::OperationDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::OperationDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::OperationDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::OperationDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_OperationDescription;

void operator<<=( CORBA::Any &_a, const ::CORBA::InterfaceDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::InterfaceDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::InterfaceDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::InterfaceDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceDescription;

void operator<<=( CORBA::Any &_a, const ::CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceAttrExtension_ExtFullInterfaceDescription;

void operator<<=( CORBA::Any &a, const CORBA::InterfaceAttrExtension_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::InterfaceAttrExtension_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::InterfaceAttrExtension_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_InterfaceAttrExtension;

void operator<<=( CORBA::Any &a, const CORBA::ExtInterfaceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ExtInterfaceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ExtInterfaceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtInterfaceDef;

void operator<<=( CORBA::Any &a, const CORBA::ExtAbstractInterfaceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ExtAbstractInterfaceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ExtAbstractInterfaceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtAbstractInterfaceDef;

void operator<<=( CORBA::Any &a, const CORBA::ExtLocalInterfaceDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ExtLocalInterfaceDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ExtLocalInterfaceDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ExtLocalInterfaceDef;

void operator<<=( CORBA::Any &a, const CORBA::ValueMemberDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ValueMemberDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ValueMemberDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ValueMemberDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ValueDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ValueDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ValueDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ValueDescription *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ValueDescription;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::Contained_var,CORBA::Contained_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_Contained;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Container::Description,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_Container_Description;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::InterfaceDef_var,CORBA::InterfaceDef_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_InterfaceDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::AbstractInterfaceDef_var,CORBA::AbstractInterfaceDef_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_AbstractInterfaceDef;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ValueDef_var,CORBA::ValueDef_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ValueDef;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Initializer,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_Initializer;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ExtInitializer,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExtInitializer;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::OperationDescription,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_OperationDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::AttributeDescription,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_AttributeDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ParameterDescription,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ParameterDescription;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ExceptionDef_var,CORBA::ExceptionDef_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExceptionDef;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ExtAttributeDescription,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExtAttributeDescription;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::LocalInterfaceDef_var,CORBA::LocalInterfaceDef_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_LocalInterfaceDef;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ExceptionDescription,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ExceptionDescription;

#endif
