/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __CSI_H__
#define __CSI_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32






namespace CSI
{

const CORBA::ULong OMGVMCID = 324816UL;
typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> X509CertificateChain;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > X509CertificateChain_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > X509CertificateChain_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_X509CertificateChain;

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> X501DistinguishedName;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > X501DistinguishedName_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > X501DistinguishedName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_X501DistinguishedName;

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> UTF8String;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > UTF8String_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > UTF8String_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UTF8String;

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> GSSToken;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > GSSToken_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > GSSToken_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_GSSToken;

typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> GSS_NT_ExportedName;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > GSS_NT_ExportedName_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > GSS_NT_ExportedName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_GSS_NT_ExportedName;

typedef SequenceTmpl< GSS_NT_ExportedName,MICO_TID_DEF> GSS_NT_ExportedNameList;
typedef TSeqVar< SequenceTmpl< GSS_NT_ExportedName,MICO_TID_DEF> > GSS_NT_ExportedNameList_var;
typedef TSeqOut< SequenceTmpl< GSS_NT_ExportedName,MICO_TID_DEF> > GSS_NT_ExportedNameList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_GSS_NT_ExportedNameList;

typedef CORBA::Short MsgType;
typedef MsgType& MsgType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_MsgType;

const MsgType MTEstablishContext = 0;
const MsgType MTCompleteEstablishContext = 1;
const MsgType MTContextError = 4;
const MsgType MTMessageInContext = 5;
typedef CORBA::ULongLong ContextId;
typedef ContextId& ContextId_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ContextId;

typedef CORBA::ULong AuthorizationElementType;
typedef AuthorizationElementType& AuthorizationElementType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthorizationElementType;

const AuthorizationElementType X509AttributeCertChain = 324817;
typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> AuthorizationElementContents;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > AuthorizationElementContents_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > AuthorizationElementContents_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthorizationElementContents;

struct AuthorizationElement;
typedef TVarVar< AuthorizationElement > AuthorizationElement_var;
typedef TVarOut< AuthorizationElement > AuthorizationElement_out;


struct AuthorizationElement {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef AuthorizationElement_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AuthorizationElement();
  ~AuthorizationElement();
  AuthorizationElement( const AuthorizationElement& s );
  AuthorizationElement& operator=( const AuthorizationElement& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  AuthorizationElementType the_type;
  AuthorizationElementContents the_element;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthorizationElement;

typedef SequenceTmpl< AuthorizationElement,MICO_TID_DEF> AuthorizationToken;
typedef TSeqVar< SequenceTmpl< AuthorizationElement,MICO_TID_DEF> > AuthorizationToken_var;
typedef TSeqOut< SequenceTmpl< AuthorizationElement,MICO_TID_DEF> > AuthorizationToken_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_AuthorizationToken;

typedef CORBA::ULong IdentityTokenType;
typedef IdentityTokenType& IdentityTokenType_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_IdentityTokenType;

const IdentityTokenType ITTAbsent = 0;
const IdentityTokenType ITTAnonymous = 1;
const IdentityTokenType ITTPrincipalName = 2;
const IdentityTokenType ITTX509CertChain = 4;
const IdentityTokenType ITTDistinguishedName = 8;
typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> IdentityExtension;
typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > IdentityExtension_var;
typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > IdentityExtension_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IdentityExtension;

class IdentityToken;
typedef TVarVar< IdentityToken > IdentityToken_var;
typedef TVarOut< IdentityToken > IdentityToken_out;


class IdentityToken {
  public:
    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IdentityToken_var _var_type;
    #endif
    IdentityTokenType _discriminator;

    struct __m {
      CORBA::Boolean absent;
      CORBA::Boolean anonymous;
      GSS_NT_ExportedName principal_name;
      X509CertificateChain certificate_chain;
      X501DistinguishedName dn;
      IdentityExtension id;
    } _m;

  public:
    #ifdef HAVE_EXPLICIT_STRUCT_OPS
    IdentityToken();
    ~IdentityToken();
    IdentityToken( const IdentityToken& s );
    IdentityToken& operator=( const IdentityToken& s );
    #endif //HAVE_EXPLICIT_STRUCT_OPS


    void _d( IdentityTokenType _p );
    IdentityTokenType _d() const;

    void absent( CORBA::Boolean _p );
    CORBA::Boolean absent() const;

    void anonymous( CORBA::Boolean _p );
    CORBA::Boolean anonymous() const;

    void principal_name( const GSS_NT_ExportedName& _p );
    const GSS_NT_ExportedName& principal_name() const;
    GSS_NT_ExportedName& principal_name();

    void certificate_chain( const X509CertificateChain& _p );
    const X509CertificateChain& certificate_chain() const;
    X509CertificateChain& certificate_chain();

    void dn( const X501DistinguishedName& _p );
    const X501DistinguishedName& dn() const;
    X501DistinguishedName& dn();

    void id( const IdentityExtension& _p );
    const IdentityExtension& id() const;
    IdentityExtension& id();

};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_IdentityToken;

struct EstablishContext;
typedef TVarVar< EstablishContext > EstablishContext_var;
typedef TVarOut< EstablishContext > EstablishContext_out;


struct EstablishContext {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef EstablishContext_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  EstablishContext();
  ~EstablishContext();
  EstablishContext( const EstablishContext& s );
  EstablishContext& operator=( const EstablishContext& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ContextId client_context_id;
  AuthorizationToken authorization_token;
  IdentityToken identity_token;
  GSSToken client_authentication_token;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_EstablishContext;

struct CompleteEstablishContext;
typedef TVarVar< CompleteEstablishContext > CompleteEstablishContext_var;
typedef TVarOut< CompleteEstablishContext > CompleteEstablishContext_out;


struct CompleteEstablishContext {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef CompleteEstablishContext_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CompleteEstablishContext();
  ~CompleteEstablishContext();
  CompleteEstablishContext( const CompleteEstablishContext& s );
  CompleteEstablishContext& operator=( const CompleteEstablishContext& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ContextId client_context_id;
  CORBA::Boolean context_stateful;
  GSSToken final_context_token;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_CompleteEstablishContext;

struct ContextError;
typedef TVarVar< ContextError > ContextError_var;
typedef TVarOut< ContextError > ContextError_out;


struct ContextError {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ContextError_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ContextError();
  ~ContextError();
  ContextError( const ContextError& s );
  ContextError& operator=( const ContextError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ContextId client_context_id;
  CORBA::Long major_status;
  CORBA::Long minor_status;
  GSSToken error_token;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ContextError;

struct MessageInContext;
typedef TFixVar< MessageInContext > MessageInContext_var;
typedef MessageInContext& MessageInContext_out;


struct MessageInContext {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef MessageInContext_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  MessageInContext();
  ~MessageInContext();
  MessageInContext( const MessageInContext& s );
  MessageInContext& operator=( const MessageInContext& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  ContextId client_context_id;
  CORBA::Boolean discard_context;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_MessageInContext;

class SASContextBody;
typedef TVarVar< SASContextBody > SASContextBody_var;
typedef TVarOut< SASContextBody > SASContextBody_out;


class SASContextBody {
  public:
    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SASContextBody_var _var_type;
    #endif
    MsgType _discriminator;

    struct __m {
      EstablishContext establish_msg;
      CompleteEstablishContext complete_msg;
      ContextError error_msg;
      MessageInContext in_context_msg;
    } _m;

  public:
    #ifdef HAVE_EXPLICIT_STRUCT_OPS
    SASContextBody();
    ~SASContextBody();
    SASContextBody( const SASContextBody& s );
    SASContextBody& operator=( const SASContextBody& s );
    #endif //HAVE_EXPLICIT_STRUCT_OPS


    void _d( MsgType _p );
    MsgType _d() const;

    void establish_msg( const EstablishContext& _p );
    const EstablishContext& establish_msg() const;
    EstablishContext& establish_msg();

    void complete_msg( const CompleteEstablishContext& _p );
    const CompleteEstablishContext& complete_msg() const;
    CompleteEstablishContext& complete_msg();

    void error_msg( const ContextError& _p );
    const ContextError& error_msg() const;
    ContextError& error_msg();

    void in_context_msg( const MessageInContext& _p );
    const MessageInContext& in_context_msg() const;
    MessageInContext& in_context_msg();

    void _default();

};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SASContextBody;

typedef char* StringOID;
typedef CORBA::String_var StringOID_var;
typedef CORBA::String_out StringOID_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_StringOID;

const StringOID KRB5MechOID = "oid:1.2.840.113554.1.2.2";
const StringOID GSS_NT_Export_Name_OID = "oid:1.3.6.1.5.6.4";
const StringOID GSS_NT_Scoped_Username_OID = "oid:2.23.130.1.2.1";
}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &_a, const ::CSI::AuthorizationElement &_s );
void operator<<=( CORBA::Any &_a, ::CSI::AuthorizationElement *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::AuthorizationElement &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::AuthorizationElement *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_AuthorizationElement;

void operator<<=( CORBA::Any &_a, const ::CSI::IdentityToken &_u );
void operator<<=( CORBA::Any &_a, ::CSI::IdentityToken *_u );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::IdentityToken &_u );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::IdentityToken *&_u );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_IdentityToken;

void operator<<=( CORBA::Any &_a, const ::CSI::EstablishContext &_s );
void operator<<=( CORBA::Any &_a, ::CSI::EstablishContext *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::EstablishContext &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::EstablishContext *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_EstablishContext;

void operator<<=( CORBA::Any &_a, const ::CSI::CompleteEstablishContext &_s );
void operator<<=( CORBA::Any &_a, ::CSI::CompleteEstablishContext *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::CompleteEstablishContext &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::CompleteEstablishContext *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_CompleteEstablishContext;

void operator<<=( CORBA::Any &_a, const ::CSI::ContextError &_s );
void operator<<=( CORBA::Any &_a, ::CSI::ContextError *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::ContextError &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::ContextError *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_ContextError;

void operator<<=( CORBA::Any &_a, const ::CSI::MessageInContext &_s );
void operator<<=( CORBA::Any &_a, ::CSI::MessageInContext *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::MessageInContext &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::MessageInContext *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_MessageInContext;

void operator<<=( CORBA::Any &_a, const ::CSI::SASContextBody &_u );
void operator<<=( CORBA::Any &_a, ::CSI::SASContextBody *_u );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSI::SASContextBody &_u );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSI::SASContextBody *&_u );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSI_SASContextBody;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSI::AuthorizationElement,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CSI_AuthorizationElement;

#endif
