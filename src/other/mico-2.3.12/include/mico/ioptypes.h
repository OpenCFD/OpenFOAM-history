/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __IOPTYPES_H__
#define __IOPTYPES_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace IOP
{

typedef CORBA::ULong ServiceID;
typedef ServiceID& ServiceID_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceID;

struct ServiceContext;
typedef TVarVar< ServiceContext > ServiceContext_var;
typedef TVarOut< ServiceContext > ServiceContext_out;


struct ServiceContext {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ServiceContext_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ServiceContext();
  ~ServiceContext();
  ServiceContext( const ServiceContext& s );
  ServiceContext& operator=( const ServiceContext& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ServiceID context_id;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _context_data_seq;
  _context_data_seq context_data;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceContext;

typedef SequenceTmpl< ServiceContext,MICO_TID_DEF> ServiceContextList;
typedef TSeqVar< SequenceTmpl< ServiceContext,MICO_TID_DEF> > ServiceContextList_var;
typedef TSeqOut< SequenceTmpl< ServiceContext,MICO_TID_DEF> > ServiceContextList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceContextList;

const ServiceID TransactionService = 0;
const ServiceID CodeSets = 1;
const ServiceID BI_DIR_IIOP = 5;
const ServiceID SecurityAttributeService = 15;
typedef CORBA::ULong ProfileId;
typedef ProfileId& ProfileId_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ProfileId;

const ProfileId TAG_INTERNET_IOP = 0;
const ProfileId TAG_MULTIPLE_COMPONENTS = 1;
const ProfileId TAG_MOBILE_TERMINAL_IOP = 4;
struct TaggedProfile;
typedef TVarVar< TaggedProfile > TaggedProfile_var;
typedef TVarOut< TaggedProfile > TaggedProfile_out;


struct TaggedProfile {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef TaggedProfile_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TaggedProfile();
  ~TaggedProfile();
  TaggedProfile( const TaggedProfile& s );
  TaggedProfile& operator=( const TaggedProfile& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ProfileId tag;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _profile_data_seq;
  _profile_data_seq profile_data;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TaggedProfile;

struct IOR;
typedef TVarVar< IOR > IOR_var;
typedef TVarOut< IOR > IOR_out;


struct IOR {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef IOR_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  IOR();
  ~IOR();
  IOR( const IOR& s );
  IOR& operator=( const IOR& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var type_id;
  typedef SequenceTmpl< TaggedProfile,MICO_TID_DEF> _profiles_seq;
  _profiles_seq profiles;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IOR;

typedef CORBA::ULong ComponentId;
typedef ComponentId& ComponentId_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ComponentId;

const ComponentId TAG_HOME_LOCATION_INFO = 44;
struct TaggedComponent;
typedef TVarVar< TaggedComponent > TaggedComponent_var;
typedef TVarOut< TaggedComponent > TaggedComponent_out;


struct TaggedComponent {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef TaggedComponent_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TaggedComponent();
  ~TaggedComponent();
  TaggedComponent( const TaggedComponent& s );
  TaggedComponent& operator=( const TaggedComponent& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ComponentId tag;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _component_data_seq;
  _component_data_seq component_data;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TaggedComponent;

typedef SequenceTmpl< TaggedComponent,MICO_TID_DEF> MultipleComponentProfile;
typedef TSeqVar< SequenceTmpl< TaggedComponent,MICO_TID_DEF> > MultipleComponentProfile_var;
typedef TSeqOut< SequenceTmpl< TaggedComponent,MICO_TID_DEF> > MultipleComponentProfile_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MultipleComponentProfile;

}


namespace GIOP
{

enum MsgType_1_1 {
  Request = 0,
  Reply,
  CancelRequest,
  LocateRequest,
  LocateReply,
  CloseConnection,
  MessageError,
  Fragment
};

typedef MsgType_1_1& MsgType_1_1_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MsgType_1_1;

typedef MsgType_1_1 MsgType;
typedef MsgType& MsgType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_MsgType;

enum ReplyStatusType_1_2 {
  NO_EXCEPTION = 0,
  USER_EXCEPTION,
  SYSTEM_EXCEPTION,
  LOCATION_FORWARD,
  LOCATION_FORWARD_PERM,
  NEEDS_ADDRESSING_MODE
};

typedef ReplyStatusType_1_2& ReplyStatusType_1_2_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ReplyStatusType_1_2;

typedef ReplyStatusType_1_2 ReplyStatusType;
typedef ReplyStatusType& ReplyStatusType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ReplyStatusType;

enum LocateStatusType_1_2 {
  UNKNOWN_OBJECT = 0,
  OBJECT_HERE,
  OBJECT_FORWARD,
  OBJECT_FORWARD_PERM,
  LOC_SYSTEM_EXCEPTION,
  LOC_NEEDS_ADDRESSING_MODE
};

typedef LocateStatusType_1_2& LocateStatusType_1_2_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_LocateStatusType_1_2;

typedef LocateStatusType_1_2 LocateStatusType;
typedef LocateStatusType& LocateStatusType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_LocateStatusType;

typedef CORBA::Short AddressingDisposition;
typedef AddressingDisposition& AddressingDisposition_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_AddressingDisposition;

const AddressingDisposition KeyAddr = 0;
const AddressingDisposition ProfileAddr = 1;
const AddressingDisposition ReferenceAddr = 2;
}


namespace IIOP
{

struct Version;
typedef TFixVar< Version > Version_var;
typedef Version& Version_out;


struct Version {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef Version_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Version();
  ~Version();
  Version( const Version& s );
  Version& operator=( const Version& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Char major;
  CORBA::Char minor;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Version;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::IOP::ServiceContext &_s );
void operator<<=( CORBA::Any &_a, ::IOP::ServiceContext *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::IOP::ServiceContext &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::IOP::ServiceContext *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_ServiceContext;

void operator<<=( CORBA::Any &_a, const ::IOP::TaggedProfile &_s );
void operator<<=( CORBA::Any &_a, ::IOP::TaggedProfile *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::IOP::TaggedProfile &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::IOP::TaggedProfile *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_TaggedProfile;

void operator<<=( CORBA::Any &_a, const ::IOP::IOR &_s );
void operator<<=( CORBA::Any &_a, ::IOP::IOR *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::IOP::IOR &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::IOP::IOR *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_IOR;

void operator<<=( CORBA::Any &_a, const ::IOP::TaggedComponent &_s );
void operator<<=( CORBA::Any &_a, ::IOP::TaggedComponent *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::IOP::TaggedComponent &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::IOP::TaggedComponent *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_TaggedComponent;

void operator<<=( CORBA::Any &a, const ::GIOP::MsgType_1_1 &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::GIOP::MsgType_1_1 &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_GIOP_MsgType_1_1;

void operator<<=( CORBA::Any &a, const ::GIOP::ReplyStatusType_1_2 &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::GIOP::ReplyStatusType_1_2 &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_GIOP_ReplyStatusType_1_2;

void operator<<=( CORBA::Any &a, const ::GIOP::LocateStatusType_1_2 &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::GIOP::LocateStatusType_1_2 &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_GIOP_LocateStatusType_1_2;

void operator<<=( CORBA::Any &_a, const ::IIOP::Version &_s );
void operator<<=( CORBA::Any &_a, ::IIOP::Version *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::IIOP::Version &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::IIOP::Version *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IIOP_Version;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< IOP::ServiceContext,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_IOP_ServiceContext;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< IOP::TaggedProfile,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_IOP_TaggedProfile;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< IOP::TaggedComponent,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_IOP_TaggedComponent;

#endif
