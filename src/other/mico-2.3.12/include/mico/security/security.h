/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __SECURITY_H__
#define __SECURITY_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace Security
{

typedef char* SecurityName;
typedef CORBA::String_var SecurityName_var;
typedef CORBA::String_out SecurityName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityName;

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> Opaque;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > Opaque_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > Opaque_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Opaque;

const ::CORBA::ServiceOption SecurityLevel1 = 1;
const ::CORBA::ServiceOption SecurityLevel2 = 2;
const ::CORBA::ServiceOption NonRepudiation = 3;
const ::CORBA::ServiceOption SecurityORBServiceReady = 4;
const ::CORBA::ServiceOption SecurityServiceReady = 5;
const ::CORBA::ServiceOption ReplaceORBServices = 6;
const ::CORBA::ServiceOption ReplaceSecurityServices = 7;
const ::CORBA::ServiceOption StandardSecureInteroperability = 8;
const ::CORBA::ServiceOption DCESecureInteroperability = 9;
const ::CORBA::ServiceOption CommonInteroperabilityLevel0 = 10;
const ::CORBA::ServiceOption CommonInteroperabilityLevel1 = 11;
const ::CORBA::ServiceOption CommonInteroperabilityLevel2 = 12;
const ::CORBA::ServiceDetailType SecurityMechanismType = 1;
const ::CORBA::ServiceDetailType SecurityAttribute = 2;
struct ExtensibleFamily;
typedef TFixVar< ExtensibleFamily > ExtensibleFamily_var;
typedef ExtensibleFamily& ExtensibleFamily_out;


struct ExtensibleFamily {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ExtensibleFamily_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ExtensibleFamily();
  ~ExtensibleFamily();
  ExtensibleFamily( const ExtensibleFamily& s );
  ExtensibleFamily& operator=( const ExtensibleFamily& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::UShort family_definer;
  CORBA::UShort family;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExtensibleFamily;

typedef CORBA::ULong SecurityAttributeType;
typedef SecurityAttributeType& SecurityAttributeType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityAttributeType;

const SecurityAttributeType AuditId = 1;
const SecurityAttributeType AccountingId = 2;
const SecurityAttributeType NonRepudiationId = 3;
const SecurityAttributeType Public = 1;
const SecurityAttributeType AccessId = 2;
const SecurityAttributeType PrimaryGroupId = 3;
const SecurityAttributeType GroupId = 4;
const SecurityAttributeType Role = 5;
const SecurityAttributeType AttributeSet = 6;
const SecurityAttributeType Clearance = 7;
const SecurityAttributeType Capability = 8;
const SecurityAttributeType X509Subject = 1;
const SecurityAttributeType X509Issuer = 2;
const SecurityAttributeType X509Cipher = 3;
const SecurityAttributeType X509Subject_CN = 4;
const SecurityAttributeType X509Subject_C = 5;
const SecurityAttributeType X509Subject_L = 6;
const SecurityAttributeType X509Subject_ST = 7;
const SecurityAttributeType X509Subject_O = 8;
const SecurityAttributeType X509Subject_OU = 9;
const SecurityAttributeType X509Issuer_CN = 10;
const SecurityAttributeType X509Issuer_C = 11;
const SecurityAttributeType X509Issuer_L = 12;
const SecurityAttributeType X509Issuer_ST = 13;
const SecurityAttributeType X509Issuer_O = 14;
const SecurityAttributeType X509Issuer_OU = 15;
const SecurityAttributeType AuthMethod = 1;
const SecurityAttributeType PeerAddress = 2;
struct AttributeType;
typedef TFixVar< AttributeType > AttributeType_var;
typedef AttributeType& AttributeType_out;


struct AttributeType {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef AttributeType_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AttributeType();
  ~AttributeType();
  AttributeType( const AttributeType& s );
  AttributeType& operator=( const AttributeType& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ExtensibleFamily attribute_family;
  SecurityAttributeType attribute_type;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttributeType;

typedef SequenceTmpl< AttributeType,MICO_TID_DEF> AttributeTypeList;
typedef TSeqVar< SequenceTmpl< AttributeType,MICO_TID_DEF> > AttributeTypeList_var;
typedef TSeqOut< SequenceTmpl< AttributeType,MICO_TID_DEF> > AttributeTypeList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttributeTypeList;

struct SecAttribute;
typedef TVarVar< SecAttribute > SecAttribute_var;
typedef TVarOut< SecAttribute > SecAttribute_out;


struct SecAttribute {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SecAttribute_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SecAttribute();
  ~SecAttribute();
  SecAttribute( const SecAttribute& s );
  SecAttribute& operator=( const SecAttribute& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AttributeType attribute_type;
  ::CSI::OID defining_authority;
  Opaque value;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecAttribute;

typedef SequenceTmpl< SecAttribute,MICO_TID_DEF> AttributeList;
typedef TSeqVar< SequenceTmpl< SecAttribute,MICO_TID_DEF> > AttributeList_var;
typedef TSeqOut< SequenceTmpl< SecAttribute,MICO_TID_DEF> > AttributeList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AttributeList;

enum AuthenticationStatus {
  SecAuthSuccess = 0,
  SecAuthFailure,
  SecAuthContinue,
  SecAuthExpired
};

typedef AuthenticationStatus& AuthenticationStatus_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthenticationStatus;

enum AssociationStatus {
  SecAssocSuccess = 0,
  SecAssocFailure,
  SecAssocContinue
};

typedef AssociationStatus& AssociationStatus_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AssociationStatus;

typedef CORBA::ULong AuthenticationMethod;
typedef AuthenticationMethod& AuthenticationMethod_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthenticationMethod;

typedef SequenceTmpl< AuthenticationMethod,MICO_TID_DEF> AuthenticationMethodList;
typedef TSeqVar< SequenceTmpl< AuthenticationMethod,MICO_TID_DEF> > AuthenticationMethodList_var;
typedef TSeqOut< SequenceTmpl< AuthenticationMethod,MICO_TID_DEF> > AuthenticationMethodList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthenticationMethodList;

enum CredentialsType {
  SecOwnCredentials = 0,
  SecReceivedCredentials,
  SecTargetCredentials
};

typedef CredentialsType& CredentialsType_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CredentialsType;

struct ChannelBindings;
typedef TVarVar< ChannelBindings > ChannelBindings_var;
typedef TVarOut< ChannelBindings > ChannelBindings_out;


struct ChannelBindings {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ChannelBindings_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ChannelBindings();
  ~ChannelBindings();
  ChannelBindings( const ChannelBindings& s );
  ChannelBindings& operator=( const ChannelBindings& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::ULong initiator_addrtype;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _initiator_address_seq;
  _initiator_address_seq initiator_address;
  CORBA::ULong acceptor_addrtype;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _acceptor_address_seq;
  _acceptor_address_seq acceptor_address;
  typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _application_data_seq;
  _application_data_seq application_data;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ChannelBindings;

struct Right;
typedef TVarVar< Right > Right_var;
typedef TVarOut< Right > Right_out;


struct Right {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef Right_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Right();
  ~Right();
  Right( const Right& s );
  Right& operator=( const Right& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ExtensibleFamily rights_family;
  CORBA::String_var rights_list;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Right;

typedef SequenceTmpl< Right,MICO_TID_DEF> RightsList;
typedef TSeqVar< SequenceTmpl< Right,MICO_TID_DEF> > RightsList_var;
typedef TSeqOut< SequenceTmpl< Right,MICO_TID_DEF> > RightsList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RightsList;

enum RightsCombinator {
  SecAllRights = 0,
  SecAnyRight
};

typedef RightsCombinator& RightsCombinator_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RightsCombinator;

enum DelegationState {
  SecInitiator = 0,
  SecDelegate
};

typedef DelegationState& DelegationState_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DelegationState;

enum DelegationDirective {
  Delegate = 0,
  NoDelegate
};

typedef DelegationDirective& DelegationDirective_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DelegationDirective;

typedef ::TimeBase::UtcT UtcT;
typedef ::TimeBase::UtcT_var UtcT_var;
typedef ::TimeBase::UtcT_out UtcT_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UtcT;

typedef ::TimeBase::IntervalT IntervalT;
typedef ::TimeBase::IntervalT_var IntervalT_var;
typedef ::TimeBase::IntervalT_out IntervalT_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IntervalT;

typedef ::TimeBase::TimeT TimeT;
typedef TimeT& TimeT_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_TimeT;

enum SecurityFeature {
  SecNoDelegation = 0,
  SecSimpleDelegation,
  SecCompositeDelegation,
  SecNoProtection,
  SecIntegrity,
  SecConfidentiality,
  SecIntegrityAndConfidentiality,
  SecDetectReplay,
  SecDetectMisordering,
  SecEstablishTrustInTarget,
  SecEstablishTrustInClient
};

typedef SecurityFeature& SecurityFeature_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityFeature;

enum QOP {
  SecQOPNoProtection = 0,
  SecQOPIntegrity,
  SecQOPConfidentiality,
  SecQOPIntegrityAndConfidentiality
};

typedef QOP& QOP_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_QOP;

enum SecurityContextType {
  ClientSecurityContext = 0,
  ServerSecurityContext
};

typedef SecurityContextType& SecurityContextType_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityContextType;

enum SecurityContextState {
  SecContextInitialized = 0,
  SecContextContinued,
  SecContextClientEstablished,
  SecContextEstablished,
  SecContextEstablishExpired,
  SecContextExpired,
  SecContextInvalid
};

typedef SecurityContextState& SecurityContextState_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityContextState;

struct OpaqueBuffer;
typedef TVarVar< OpaqueBuffer > OpaqueBuffer_var;
typedef TVarOut< OpaqueBuffer > OpaqueBuffer_out;


struct OpaqueBuffer {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef OpaqueBuffer_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  OpaqueBuffer();
  ~OpaqueBuffer();
  OpaqueBuffer( const OpaqueBuffer& s );
  OpaqueBuffer& operator=( const OpaqueBuffer& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Opaque buffer;
  CORBA::ULong startpos;
  CORBA::ULong endpos;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OpaqueBuffer;

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
enum RequiresSupports {
  SecRequires = 0,
  SecSupports
};

typedef RequiresSupports& RequiresSupports_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RequiresSupports;

enum CommunicationDirection {
  SecDirectionBoth = 0,
  SecDirectionRequest,
  SecDirectionReply
};

typedef CommunicationDirection& CommunicationDirection_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CommunicationDirection;

struct OptionsDirectionPair;
typedef TFixVar< OptionsDirectionPair > OptionsDirectionPair_var;
typedef OptionsDirectionPair& OptionsDirectionPair_out;


struct OptionsDirectionPair {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef OptionsDirectionPair_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  OptionsDirectionPair();
  ~OptionsDirectionPair();
  OptionsDirectionPair( const OptionsDirectionPair& s );
  OptionsDirectionPair& operator=( const OptionsDirectionPair& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AssociationOptions options;
  CommunicationDirection direction;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OptionsDirectionPair;

typedef SequenceTmpl< OptionsDirectionPair,MICO_TID_DEF> OptionsDirectionPairList;
typedef TSeqVar< SequenceTmpl< OptionsDirectionPair,MICO_TID_DEF> > OptionsDirectionPairList_var;
typedef TSeqOut< SequenceTmpl< OptionsDirectionPair,MICO_TID_DEF> > OptionsDirectionPairList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_OptionsDirectionPairList;

enum DelegationMode {
  SecDelModeNoDelegation = 0,
  SecDelModeSimpleDelegation,
  SecDelModeCompositeDelegation
};

typedef DelegationMode& DelegationMode_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DelegationMode;

typedef char* MechanismType;
typedef CORBA::String_var MechanismType_var;
typedef CORBA::String_out MechanismType_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MechanismType;

typedef StringSequenceTmpl<CORBA::String_var> MechanismTypeList;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > MechanismTypeList_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > MechanismTypeList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MechanismTypeList;

struct SecurityMechanismData;
typedef TVarVar< SecurityMechanismData > SecurityMechanismData_var;
typedef TVarOut< SecurityMechanismData > SecurityMechanismData_out;


struct SecurityMechanismData {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SecurityMechanismData_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SecurityMechanismData();
  ~SecurityMechanismData();
  SecurityMechanismData( const SecurityMechanismData& s );
  SecurityMechanismData& operator=( const SecurityMechanismData& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  MechanismType_var mechanism;
  SecurityName_var security_name;
  AssociationOptions options_supported;
  AssociationOptions options_required;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityMechanismData;

typedef SequenceTmpl< SecurityMechanismData,MICO_TID_DEF> SecurityMechanismDataList;
typedef TSeqVar< SequenceTmpl< SecurityMechanismData,MICO_TID_DEF> > SecurityMechanismDataList_var;
typedef TSeqOut< SequenceTmpl< SecurityMechanismData,MICO_TID_DEF> > SecurityMechanismDataList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SecurityMechanismDataList;

struct MechandOptions;
typedef TVarVar< MechandOptions > MechandOptions_var;
typedef TVarOut< MechandOptions > MechandOptions_out;


struct MechandOptions {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef MechandOptions_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  MechandOptions();
  ~MechandOptions();
  MechandOptions( const MechandOptions& s );
  MechandOptions& operator=( const MechandOptions& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  MechanismType_var mechanism_type;
  AssociationOptions options_supported;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MechandOptions;

typedef SequenceTmpl< MechandOptions,MICO_TID_DEF> MechandOptionsList;
typedef TSeqVar< SequenceTmpl< MechandOptions,MICO_TID_DEF> > MechandOptionsList_var;
typedef TSeqOut< SequenceTmpl< MechandOptions,MICO_TID_DEF> > MechandOptionsList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MechandOptionsList;

struct EstablishTrust;
typedef TFixVar< EstablishTrust > EstablishTrust_var;
typedef EstablishTrust& EstablishTrust_out;


struct EstablishTrust {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef EstablishTrust_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  EstablishTrust();
  ~EstablishTrust();
  EstablishTrust( const EstablishTrust& s );
  EstablishTrust& operator=( const EstablishTrust& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Boolean trust_in_client;
  CORBA::Boolean trust_in_target;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_EstablishTrust;

typedef CORBA::ULong AuditChannelId;
typedef AuditChannelId& AuditChannelId_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuditChannelId;

typedef CORBA::UShort EventType;
typedef EventType& EventType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_EventType;

const EventType AuditAll = 0;
const EventType AuditPrincipalAuth = 1;
const EventType AuditSessionAuth = 2;
const EventType AuditAuthorization = 3;
const EventType AuditInvocation = 4;
const EventType AuditSecEnvChange = 5;
const EventType AuditPolicyChange = 6;
const EventType AuditObjectCreation = 7;
const EventType AuditObjectDestruction = 8;
const EventType AuditNonRepudiation = 9;
enum DayOfTheWeek {
  Monday = 0,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

typedef DayOfTheWeek& DayOfTheWeek_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DayOfTheWeek;

enum AuditCombinator {
  SecAllSelectors = 0,
  SecAnySelector
};

typedef AuditCombinator& AuditCombinator_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuditCombinator;

struct AuditEventType;
typedef TFixVar< AuditEventType > AuditEventType_var;
typedef AuditEventType& AuditEventType_out;


struct AuditEventType {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef AuditEventType_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AuditEventType();
  ~AuditEventType();
  AuditEventType( const AuditEventType& s );
  AuditEventType& operator=( const AuditEventType& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ExtensibleFamily event_family;
  CORBA::UShort event_type;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuditEventType;

typedef SequenceTmpl< AuditEventType,MICO_TID_DEF> AuditEventTypeList;
typedef TSeqVar< SequenceTmpl< AuditEventType,MICO_TID_DEF> > AuditEventTypeList_var;
typedef TSeqOut< SequenceTmpl< AuditEventType,MICO_TID_DEF> > AuditEventTypeList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuditEventTypeList;

typedef CORBA::ULong SelectorType;
typedef SelectorType& SelectorType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_SelectorType;

const SelectorType InterfaceName = 1;
const SelectorType ObjectRef = 2;
const SelectorType Operation = 3;
const SelectorType Initiator = 4;
const SelectorType SuccessFailure = 5;
const SelectorType Time = 6;
const SelectorType DayOfWeek = 7;
struct SelectorValue;
typedef TVarVar< SelectorValue > SelectorValue_var;
typedef TVarOut< SelectorValue > SelectorValue_out;


struct SelectorValue {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SelectorValue_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SelectorValue();
  ~SelectorValue();
  SelectorValue( const SelectorValue& s );
  SelectorValue& operator=( const SelectorValue& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  SelectorType selector;
  CORBA::Any value;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SelectorValue;

typedef SequenceTmpl< SelectorValue,MICO_TID_DEF> SelectorValueList;
typedef TSeqVar< SequenceTmpl< SelectorValue,MICO_TID_DEF> > SelectorValueList_var;
typedef TSeqOut< SequenceTmpl< SelectorValue,MICO_TID_DEF> > SelectorValueList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SelectorValueList;

const ::CORBA::PolicyType SecClientInvocationAccess = 1;
const ::CORBA::PolicyType SecTargetInvocationAccess = 2;
const ::CORBA::PolicyType SecApplicationAccess = 3;
const ::CORBA::PolicyType SecClientInvocationAudit = 4;
const ::CORBA::PolicyType SecTargetInvocationAudit = 5;
const ::CORBA::PolicyType SecApplicationAudit = 6;
const ::CORBA::PolicyType SecDelegation = 7;
const ::CORBA::PolicyType SecClientSecureInvocation = 8;
const ::CORBA::PolicyType SecTargetSecureInvocation = 9;
const ::CORBA::PolicyType SecNonRepudiation = 10;
const ::CORBA::PolicyType SecMechanismPolicy = 12;
const ::CORBA::PolicyType SecInvocationCredentialsPolicy = 13;
const ::CORBA::PolicyType SecFeaturesPolicy = 14;
const ::CORBA::PolicyType SecQOPPolicy = 15;
const ::CORBA::PolicyType SecDelegationDirectivePolicy = 38;
const ::CORBA::PolicyType SecEstablishTrustPolicy = 39;
struct SSLKeyCertCAPass;
typedef TVarVar< SSLKeyCertCAPass > SSLKeyCertCAPass_var;
typedef TVarOut< SSLKeyCertCAPass > SSLKeyCertCAPass_out;


struct SSLKeyCertCAPass {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SSLKeyCertCAPass_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SSLKeyCertCAPass();
  ~SSLKeyCertCAPass();
  SSLKeyCertCAPass( const SSLKeyCertCAPass& s );
  SSLKeyCertCAPass& operator=( const SSLKeyCertCAPass& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var key;
  CORBA::String_var cert;
  CORBA::String_var CAfile;
  CORBA::String_var CAdir;
  CORBA::String_var pass;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SSLKeyCertCAPass;

}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::Security::ExtensibleFamily &_s );
void operator<<=( CORBA::Any &_a, ::Security::ExtensibleFamily *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::ExtensibleFamily &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::ExtensibleFamily *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_ExtensibleFamily;

void operator<<=( CORBA::Any &_a, const ::Security::AttributeType &_s );
void operator<<=( CORBA::Any &_a, ::Security::AttributeType *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::AttributeType &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::AttributeType *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_AttributeType;

void operator<<=( CORBA::Any &_a, const ::Security::SecAttribute &_s );
void operator<<=( CORBA::Any &_a, ::Security::SecAttribute *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::SecAttribute &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::SecAttribute *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SecAttribute;

void operator<<=( CORBA::Any &a, const ::Security::AuthenticationStatus &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::AuthenticationStatus &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_AuthenticationStatus;

void operator<<=( CORBA::Any &a, const ::Security::AssociationStatus &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::AssociationStatus &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_AssociationStatus;

void operator<<=( CORBA::Any &a, const ::Security::CredentialsType &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::CredentialsType &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_CredentialsType;

void operator<<=( CORBA::Any &_a, const ::Security::ChannelBindings &_s );
void operator<<=( CORBA::Any &_a, ::Security::ChannelBindings *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::ChannelBindings &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::ChannelBindings *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_ChannelBindings;

void operator<<=( CORBA::Any &_a, const ::Security::Right &_s );
void operator<<=( CORBA::Any &_a, ::Security::Right *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::Right &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::Right *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_Right;

void operator<<=( CORBA::Any &a, const ::Security::RightsCombinator &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::RightsCombinator &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_RightsCombinator;

void operator<<=( CORBA::Any &a, const ::Security::DelegationState &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::DelegationState &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_DelegationState;

void operator<<=( CORBA::Any &a, const ::Security::DelegationDirective &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::DelegationDirective &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_DelegationDirective;

void operator<<=( CORBA::Any &a, const ::Security::SecurityFeature &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::SecurityFeature &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SecurityFeature;

void operator<<=( CORBA::Any &a, const ::Security::QOP &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::QOP &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_QOP;

void operator<<=( CORBA::Any &a, const ::Security::SecurityContextType &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::SecurityContextType &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SecurityContextType;

void operator<<=( CORBA::Any &a, const ::Security::SecurityContextState &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::SecurityContextState &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SecurityContextState;

void operator<<=( CORBA::Any &_a, const ::Security::OpaqueBuffer &_s );
void operator<<=( CORBA::Any &_a, ::Security::OpaqueBuffer *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::OpaqueBuffer &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::OpaqueBuffer *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_OpaqueBuffer;

void operator<<=( CORBA::Any &a, const ::Security::RequiresSupports &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::RequiresSupports &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_RequiresSupports;

void operator<<=( CORBA::Any &a, const ::Security::CommunicationDirection &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::CommunicationDirection &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_CommunicationDirection;

void operator<<=( CORBA::Any &_a, const ::Security::OptionsDirectionPair &_s );
void operator<<=( CORBA::Any &_a, ::Security::OptionsDirectionPair *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::OptionsDirectionPair &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::OptionsDirectionPair *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_OptionsDirectionPair;

void operator<<=( CORBA::Any &a, const ::Security::DelegationMode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::DelegationMode &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_DelegationMode;

void operator<<=( CORBA::Any &_a, const ::Security::SecurityMechanismData &_s );
void operator<<=( CORBA::Any &_a, ::Security::SecurityMechanismData *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::SecurityMechanismData &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::SecurityMechanismData *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SecurityMechanismData;

void operator<<=( CORBA::Any &_a, const ::Security::MechandOptions &_s );
void operator<<=( CORBA::Any &_a, ::Security::MechandOptions *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::MechandOptions &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::MechandOptions *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_MechandOptions;

void operator<<=( CORBA::Any &_a, const ::Security::EstablishTrust &_s );
void operator<<=( CORBA::Any &_a, ::Security::EstablishTrust *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::EstablishTrust &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::EstablishTrust *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_EstablishTrust;

void operator<<=( CORBA::Any &a, const ::Security::DayOfTheWeek &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::DayOfTheWeek &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_DayOfTheWeek;

void operator<<=( CORBA::Any &a, const ::Security::AuditCombinator &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Security::AuditCombinator &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_AuditCombinator;

void operator<<=( CORBA::Any &_a, const ::Security::AuditEventType &_s );
void operator<<=( CORBA::Any &_a, ::Security::AuditEventType *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::AuditEventType &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::AuditEventType *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_AuditEventType;

void operator<<=( CORBA::Any &_a, const ::Security::SelectorValue &_s );
void operator<<=( CORBA::Any &_a, ::Security::SelectorValue *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::SelectorValue &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::SelectorValue *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SelectorValue;

void operator<<=( CORBA::Any &_a, const ::Security::SSLKeyCertCAPass &_s );
void operator<<=( CORBA::Any &_a, ::Security::SSLKeyCertCAPass *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Security::SSLKeyCertCAPass &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Security::SSLKeyCertCAPass *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Security_SSLKeyCertCAPass;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::AttributeType,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::AttributeType,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::AttributeType,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::AttributeType,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_AttributeType;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::SecAttribute,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_SecAttribute;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::Right,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::Right,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::Right,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::Right,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_Right;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::OptionsDirectionPair,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_OptionsDirectionPair;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::SecurityMechanismData,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_SecurityMechanismData;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::MechandOptions,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_MechandOptions;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::AuditEventType,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_AuditEventType;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Security::SelectorValue,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Security_SelectorValue;

#endif
