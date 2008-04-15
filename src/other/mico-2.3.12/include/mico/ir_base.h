/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __IR_BASE_H__
#define __IR_BASE_H__


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

class IRObject;
typedef IRObject *IRObject_ptr;
typedef IRObject_ptr IRObjectRef;
typedef ObjVar< IRObject > IRObject_var;
typedef ObjOut< IRObject > IRObject_out;

class IDLType;
typedef IDLType *IDLType_ptr;
typedef IDLType_ptr IDLTypeRef;
typedef ObjVar< IDLType > IDLType_var;
typedef ObjOut< IDLType > IDLType_out;

}






namespace CORBA
{

typedef char* Identifier;
typedef CORBA::String_var Identifier_var;
typedef CORBA::String_out Identifier_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Identifier;

typedef char* VersionSpec;
typedef CORBA::String_var VersionSpec_var;
typedef CORBA::String_out VersionSpec_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_VersionSpec;

typedef char* RepositoryId;
typedef CORBA::String_var RepositoryId_var;
typedef CORBA::String_out RepositoryId_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RepositoryId;

enum DefinitionKind {
  dk_none = 0,
  dk_all,
  dk_Attribute,
  dk_Constant,
  dk_Exception,
  dk_Interface,
  dk_Module,
  dk_Operation,
  dk_Typedef,
  dk_Alias,
  dk_Struct,
  dk_Union,
  dk_Enum,
  dk_Primitive,
  dk_String,
  dk_Sequence,
  dk_Array,
  dk_Repository,
  dk_Wstring,
  dk_Fixed,
  dk_Value,
  dk_ValueBox,
  dk_ValueMember,
  dk_Native,
  dk_AbstractInterface,
  dk_LocalInterface,
  dk_Component,
  dk_Home,
  dk_Factory,
  dk_Finder,
  dk_PrimaryKey,
  dk_Emits,
  dk_Publishes,
  dk_Consumes,
  dk_Provides,
  dk_Uses,
  dk_Event
};

typedef DefinitionKind& DefinitionKind_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DefinitionKind;


/*
 * Base class and common definitions for interface IRObject
 */

class IRObject : 
  virtual public CORBA::Object
{
  public:
    virtual ~IRObject();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IRObject_ptr _ptr_type;
    typedef IRObject_var _var_type;
    #endif

    static IRObject_ptr _narrow( CORBA::Object_ptr obj );
    static IRObject_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IRObject_ptr _duplicate( IRObject_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IRObject_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::DefinitionKind def_kind() = 0;

    virtual void destroy() = 0;

  protected:
    IRObject() {};
  private:
    IRObject( const IRObject& );
    void operator=( const IRObject& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IRObject;

// Stub for interface IRObject
class IRObject_stub:
  virtual public IRObject
{
  public:
    virtual ~IRObject_stub();
    ::CORBA::DefinitionKind def_kind();

    void destroy();

  private:
    void operator=( const IRObject_stub& );
};

#ifndef MICO_CONF_NO_POA

class IRObject_stub_clp :
  virtual public IRObject_stub,
  virtual public PortableServer::StubBase
{
  public:
    IRObject_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~IRObject_stub_clp ();
    ::CORBA::DefinitionKind def_kind();

    void destroy();

  protected:
    IRObject_stub_clp ();
  private:
    void operator=( const IRObject_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface IDLType
 */

class IDLType : 
  virtual public ::CORBA::IRObject
{
  public:
    virtual ~IDLType();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IDLType_ptr _ptr_type;
    typedef IDLType_var _var_type;
    #endif

    static IDLType_ptr _narrow( CORBA::Object_ptr obj );
    static IDLType_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IDLType_ptr _duplicate( IDLType_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IDLType_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::TypeCode_ptr type() = 0;

  protected:
    IDLType() {};
  private:
    IDLType( const IDLType& );
    void operator=( const IDLType& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IDLType;

// Stub for interface IDLType
class IDLType_stub:
  virtual public IDLType,
  virtual public ::CORBA::IRObject_stub
{
  public:
    virtual ~IDLType_stub();
    CORBA::TypeCode_ptr type();

  private:
    void operator=( const IDLType_stub& );
};

#ifndef MICO_CONF_NO_POA

class IDLType_stub_clp :
  virtual public IDLType_stub,
  virtual public ::CORBA::IRObject_stub_clp
{
  public:
    IDLType_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~IDLType_stub_clp ();
    CORBA::TypeCode_ptr type();

  protected:
    IDLType_stub_clp ();
  private:
    void operator=( const IDLType_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct StructMember;
typedef TVarVar< StructMember > StructMember_var;
typedef TVarOut< StructMember > StructMember_out;


struct StructMember {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef StructMember_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  StructMember();
  ~StructMember();
  StructMember( const StructMember& s );
  StructMember& operator=( const StructMember& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  CORBA::TypeCode_var type;
  IDLType_var type_def;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_StructMember;

typedef SequenceTmpl< StructMember,MICO_TID_DEF> StructMemberSeq;
typedef TSeqVar< SequenceTmpl< StructMember,MICO_TID_DEF> > StructMemberSeq_var;
typedef TSeqOut< SequenceTmpl< StructMember,MICO_TID_DEF> > StructMemberSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_StructMemberSeq;

struct UnionMember;
typedef TVarVar< UnionMember > UnionMember_var;
typedef TVarOut< UnionMember > UnionMember_out;


struct UnionMember {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef UnionMember_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  UnionMember();
  ~UnionMember();
  UnionMember( const UnionMember& s );
  UnionMember& operator=( const UnionMember& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  CORBA::Any label;
  CORBA::TypeCode_var type;
  IDLType_var type_def;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UnionMember;

typedef SequenceTmpl< UnionMember,MICO_TID_DEF> UnionMemberSeq;
typedef TSeqVar< SequenceTmpl< UnionMember,MICO_TID_DEF> > UnionMemberSeq_var;
typedef TSeqOut< SequenceTmpl< UnionMember,MICO_TID_DEF> > UnionMemberSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UnionMemberSeq;

typedef StringSequenceTmpl<CORBA::String_var> EnumMemberSeq;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > EnumMemberSeq_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > EnumMemberSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_EnumMemberSeq;

typedef CORBA::Short Visibility;
typedef Visibility& Visibility_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_Visibility;

const Visibility PRIVATE_MEMBER = 0;
const Visibility PUBLIC_MEMBER = 1;
typedef CORBA::Short ValueModifier;
typedef ValueModifier& ValueModifier_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueModifier;

const ValueModifier VM_NONE = 0;
const ValueModifier VM_CUSTOM = 1;
const ValueModifier VM_ABSTRACT = 2;
const ValueModifier VM_TRUNCATABLE = 3;
struct ValueMember;
typedef TVarVar< ValueMember > ValueMember_var;
typedef TVarOut< ValueMember > ValueMember_out;


struct ValueMember {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ValueMember_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ValueMember();
  ~ValueMember();
  ValueMember( const ValueMember& s );
  ValueMember& operator=( const ValueMember& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  CORBA::TypeCode_var type;
  IDLType_var type_def;
  Visibility access;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueMember;

typedef SequenceTmpl< ValueMember,MICO_TID_DEF> ValueMemberSeq;
typedef TSeqVar< SequenceTmpl< ValueMember,MICO_TID_DEF> > ValueMemberSeq_var;
typedef TSeqOut< SequenceTmpl< ValueMember,MICO_TID_DEF> > ValueMemberSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ValueMemberSeq;

}


#ifndef MICO_CONF_NO_POA



namespace POA_CORBA
{

class IRObject : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~IRObject ();
    CORBA::IRObject_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static IRObject * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::DefinitionKind def_kind() = 0;

    virtual void destroy() = 0;

  protected:
    IRObject () {};

  private:
    IRObject (const IRObject &);
    void operator= (const IRObject &);
};

class IDLType : 
  virtual public POA_CORBA::IRObject
{
  public:
    virtual ~IDLType ();
    CORBA::IDLType_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static IDLType * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::TypeCode_ptr type() = 0;

  protected:
    IDLType () {};

  private:
    IDLType (const IDLType &);
    void operator= (const IDLType &);
};

}


#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::CORBA::DefinitionKind &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::CORBA::DefinitionKind &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_DefinitionKind;

void operator<<=( CORBA::Any &a, const CORBA::IRObject_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::IRObject_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::IRObject_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_IRObject;

void operator<<=( CORBA::Any &a, const CORBA::IDLType_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::IDLType_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::IDLType_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_IDLType;

void operator<<=( CORBA::Any &_a, const ::CORBA::StructMember &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::StructMember *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::StructMember &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::StructMember *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_StructMember;

void operator<<=( CORBA::Any &_a, const ::CORBA::UnionMember &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::UnionMember *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::UnionMember &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::UnionMember *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_UnionMember;

void operator<<=( CORBA::Any &_a, const ::CORBA::ValueMember &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ValueMember *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ValueMember &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ValueMember *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ValueMember;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::StructMember,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::StructMember,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::StructMember,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::StructMember,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_StructMember;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::UnionMember,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::UnionMember,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::UnionMember,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::UnionMember,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_UnionMember;

void operator<<=( CORBA::Any &_a, const StringSequenceTmpl<CORBA::String_var> &_s );
void operator<<=( CORBA::Any &_a, StringSequenceTmpl<CORBA::String_var> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, StringSequenceTmpl<CORBA::String_var> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const StringSequenceTmpl<CORBA::String_var> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ValueMember,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ValueMember,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ValueMember,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ValueMember,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ValueMember;

#endif
