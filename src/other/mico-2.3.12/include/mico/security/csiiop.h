/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __CSIIOP_H__
#define __CSIIOP_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace CSIIOP
{

const ::IOP::ComponentId TAG_NULL_TAG = 34;
const ::IOP::ComponentId TAG_CSI_SEC_MECH_LIST = 33;
typedef CORBA::UShort AssociationOptions;
typedef AssociationOptions& AssociationOptions_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_AssociationOptions;

const AssociationOptions NoProtection = 1;
const AssociationOptions Integrity = 2;
const AssociationOptions Confidentiality = 4;
const AssociationOptions DetectReplay = 8;
const AssociationOptions DetectMisordering = 16;
const AssociationOptions EstablishTrustInTarget = 32;
const AssociationOptions EstablishTrustInClient = 64;
const AssociationOptions NoDelegation = 128;
const AssociationOptions SimpleDelegation = 256;
const AssociationOptions CompositeDelegation = 512;
const AssociationOptions IdentityAssertion = 1024;
const AssociationOptions DelegationByClient = 2048;
typedef CORBA::ULong ServiceConfigurationSyntax;
typedef ServiceConfigurationSyntax& ServiceConfigurationSyntax_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceConfigurationSyntax;

const ServiceConfigurationSyntax SCS_GeneralNames = 324816;
const ServiceConfigurationSyntax SCS_GSSExportedName = 324817;
typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> ServiceSpecificName;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > ServiceSpecificName_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > ServiceSpecificName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceSpecificName;

struct ServiceConfiguration;
typedef TVarVar< ServiceConfiguration > ServiceConfiguration_var;
typedef TVarOut< ServiceConfiguration > ServiceConfiguration_out;


struct ServiceConfiguration {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ServiceConfiguration_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ServiceConfiguration();
  ~ServiceConfiguration();
  ServiceConfiguration( const ServiceConfiguration& s );
  ServiceConfiguration& operator=( const ServiceConfiguration& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ServiceConfigurationSyntax syntax;
  ServiceSpecificName name;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceConfiguration;

typedef SequenceTmpl< ServiceConfiguration,MICO_TID_DEF> ServiceConfigurationList;
typedef TSeqVar< SequenceTmpl< ServiceConfiguration,MICO_TID_DEF> > ServiceConfigurationList_var;
typedef TSeqOut< SequenceTmpl< ServiceConfiguration,MICO_TID_DEF> > ServiceConfigurationList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ServiceConfigurationList;

struct AS_ContextSec;
typedef TVarVar< AS_ContextSec > AS_ContextSec_var;
typedef TVarOut< AS_ContextSec > AS_ContextSec_out;


struct AS_ContextSec {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef AS_ContextSec_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AS_ContextSec();
  ~AS_ContextSec();
  AS_ContextSec( const AS_ContextSec& s );
  AS_ContextSec& operator=( const AS_ContextSec& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AssociationOptions target_supports;
  AssociationOptions target_requires;
  ::CSI::OID client_authentication_mech;
  ::CSI::GSS_NT_ExportedName target_name;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AS_ContextSec;

struct SAS_ContextSec;
typedef TVarVar< SAS_ContextSec > SAS_ContextSec_var;
typedef TVarOut< SAS_ContextSec > SAS_ContextSec_out;


struct SAS_ContextSec {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SAS_ContextSec_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SAS_ContextSec();
  ~SAS_ContextSec();
  SAS_ContextSec( const SAS_ContextSec& s );
  SAS_ContextSec& operator=( const SAS_ContextSec& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AssociationOptions target_supports;
  AssociationOptions target_requires;
  ServiceConfigurationList privilege_authorities;
  ::CSI::OIDList supported_naming_mechanisms;
  ::CSI::IdentityTokenType supported_identity_types;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SAS_ContextSec;

struct CompoundSecMech;
typedef TVarVar< CompoundSecMech > CompoundSecMech_var;
typedef TVarOut< CompoundSecMech > CompoundSecMech_out;


struct CompoundSecMech {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef CompoundSecMech_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CompoundSecMech();
  ~CompoundSecMech();
  CompoundSecMech( const CompoundSecMech& s );
  CompoundSecMech& operator=( const CompoundSecMech& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AssociationOptions target_requires;
  ::IOP::TaggedComponent transport_mech;
  AS_ContextSec as_context_mech;
  SAS_ContextSec sas_context_mech;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CompoundSecMech;

typedef SequenceTmpl< CompoundSecMech,MICO_TID_DEF> CompoundSecMechanisms;
typedef TSeqVar< SequenceTmpl< CompoundSecMech,MICO_TID_DEF> > CompoundSecMechanisms_var;
typedef TSeqOut< SequenceTmpl< CompoundSecMech,MICO_TID_DEF> > CompoundSecMechanisms_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CompoundSecMechanisms;

struct CompoundSecMechList;
typedef TVarVar< CompoundSecMechList > CompoundSecMechList_var;
typedef TVarOut< CompoundSecMechList > CompoundSecMechList_out;


struct CompoundSecMechList {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef CompoundSecMechList_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CompoundSecMechList();
  ~CompoundSecMechList();
  CompoundSecMechList( const CompoundSecMechList& s );
  CompoundSecMechList& operator=( const CompoundSecMechList& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Boolean stateful;
  CompoundSecMechanisms mechanism_list;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CompoundSecMechList;

struct TransportAddress;
typedef TVarVar< TransportAddress > TransportAddress_var;
typedef TVarOut< TransportAddress > TransportAddress_out;


struct TransportAddress {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef TransportAddress_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TransportAddress();
  ~TransportAddress();
  TransportAddress( const TransportAddress& s );
  TransportAddress& operator=( const TransportAddress& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var host_name;
  CORBA::UShort port;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TransportAddress;

typedef SequenceTmpl< TransportAddress,MICO_TID_DEF> TransportAddressList;
typedef TSeqVar< SequenceTmpl< TransportAddress,MICO_TID_DEF> > TransportAddressList_var;
typedef TSeqOut< SequenceTmpl< TransportAddress,MICO_TID_DEF> > TransportAddressList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TransportAddressList;

const ::IOP::ComponentId TAG_SECIOP_SEC_TRANS = 35;
struct SECIOP_SEC_TRANS;
typedef TVarVar< SECIOP_SEC_TRANS > SECIOP_SEC_TRANS_var;
typedef TVarOut< SECIOP_SEC_TRANS > SECIOP_SEC_TRANS_out;


struct SECIOP_SEC_TRANS {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SECIOP_SEC_TRANS_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SECIOP_SEC_TRANS();
  ~SECIOP_SEC_TRANS();
  SECIOP_SEC_TRANS( const SECIOP_SEC_TRANS& s );
  SECIOP_SEC_TRANS& operator=( const SECIOP_SEC_TRANS& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AssociationOptions target_supports;
  AssociationOptions target_requires;
  ::CSI::OID mech_oid;
  ::CSI::GSS_NT_ExportedName target_name;
  TransportAddressList addresses;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SECIOP_SEC_TRANS;

const ::IOP::ComponentId TAG_TLS_SEC_TRANS = 36;
struct TLS_SEC_TRANS;
typedef TVarVar< TLS_SEC_TRANS > TLS_SEC_TRANS_var;
typedef TVarOut< TLS_SEC_TRANS > TLS_SEC_TRANS_out;


struct TLS_SEC_TRANS {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef TLS_SEC_TRANS_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TLS_SEC_TRANS();
  ~TLS_SEC_TRANS();
  TLS_SEC_TRANS( const TLS_SEC_TRANS& s );
  TLS_SEC_TRANS& operator=( const TLS_SEC_TRANS& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AssociationOptions target_supports;
  AssociationOptions target_requires;
  TransportAddressList addresses;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TLS_SEC_TRANS;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::CSIIOP::ServiceConfiguration &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::ServiceConfiguration *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::ServiceConfiguration &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::ServiceConfiguration *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_ServiceConfiguration;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::AS_ContextSec &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::AS_ContextSec *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::AS_ContextSec &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::AS_ContextSec *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_AS_ContextSec;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::SAS_ContextSec &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::SAS_ContextSec *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::SAS_ContextSec &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::SAS_ContextSec *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_SAS_ContextSec;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::CompoundSecMech &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::CompoundSecMech *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::CompoundSecMech &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::CompoundSecMech *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_CompoundSecMech;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::CompoundSecMechList &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::CompoundSecMechList *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::CompoundSecMechList &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::CompoundSecMechList *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_CompoundSecMechList;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::TransportAddress &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::TransportAddress *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::TransportAddress &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::TransportAddress *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_TransportAddress;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::SECIOP_SEC_TRANS &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::SECIOP_SEC_TRANS *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::SECIOP_SEC_TRANS &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::SECIOP_SEC_TRANS *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_SECIOP_SEC_TRANS;

void operator<<=( CORBA::Any &_a, const ::CSIIOP::TLS_SEC_TRANS &_s );
void operator<<=( CORBA::Any &_a, ::CSIIOP::TLS_SEC_TRANS *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIIOP::TLS_SEC_TRANS &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIIOP::TLS_SEC_TRANS *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIIOP_TLS_SEC_TRANS;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIIOP::ServiceConfiguration,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CSIIOP_ServiceConfiguration;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIIOP::CompoundSecMech,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CSIIOP_CompoundSecMech;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIIOP::TransportAddress,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CSIIOP_TransportAddress;

#endif
