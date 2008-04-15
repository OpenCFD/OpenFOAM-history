/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __CSIV2_H__
#define __CSIV2_H__


#ifdef _WIN32
#ifdef BUILD_MICO_DLL
#define MICO_EXPORT /**/
#else // BUILD_MICO_DLL
#define MICO_EXPORT __declspec(dllimport)
#endif // BUILD_MICO_DLL
#else // _WIN32
#define MICO_EXPORT /**/
#endif // _WIN32




namespace CSIv2
{

class TSS;
typedef TSS *TSS_ptr;
typedef TSS_ptr TSSRef;
typedef ObjVar< TSS > TSS_var;
typedef ObjOut< TSS > TSS_out;

class CSS;
typedef CSS *CSS_ptr;
typedef CSS_ptr CSSRef;
typedef ObjVar< CSS > CSS_var;
typedef ObjOut< CSS > CSS_out;

class SecurityManager;
typedef SecurityManager *SecurityManager_ptr;
typedef SecurityManager_ptr SecurityManagerRef;
typedef ObjVar< SecurityManager > SecurityManager_var;
typedef ObjOut< SecurityManager > SecurityManager_out;

class CMSetup;
typedef CMSetup *CMSetup_ptr;
typedef CMSetup_ptr CMSetupRef;
typedef ObjVar< CMSetup > CMSetup_var;
typedef ObjOut< CMSetup > CMSetup_out;

class ServerRequestInterceptor;
typedef ServerRequestInterceptor *ServerRequestInterceptor_ptr;
typedef ServerRequestInterceptor_ptr ServerRequestInterceptorRef;
typedef ObjVar< ServerRequestInterceptor > ServerRequestInterceptor_var;
typedef ObjOut< ServerRequestInterceptor > ServerRequestInterceptor_out;

class ClientRequestInterceptor;
typedef ClientRequestInterceptor *ClientRequestInterceptor_ptr;
typedef ClientRequestInterceptor_ptr ClientRequestInterceptorRef;
typedef ObjVar< ClientRequestInterceptor > ClientRequestInterceptor_var;
typedef ObjOut< ClientRequestInterceptor > ClientRequestInterceptor_out;

class IORInterceptor;
typedef IORInterceptor *IORInterceptor_ptr;
typedef IORInterceptor_ptr IORInterceptorRef;
typedef ObjVar< IORInterceptor > IORInterceptor_var;
typedef ObjOut< IORInterceptor > IORInterceptor_out;

}






namespace CSIv2
{

typedef char* DistinguishedName;
typedef CORBA::String_var DistinguishedName_var;
typedef CORBA::String_out DistinguishedName_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DistinguishedName;

typedef StringSequenceTmpl<CORBA::String_var> DistinguishedNameList;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > DistinguishedNameList_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > DistinguishedNameList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_DistinguishedNameList;

struct UserIdentity;
typedef TVarVar< UserIdentity > UserIdentity_var;
typedef TVarOut< UserIdentity > UserIdentity_out;


typedef SequenceTmpl< UserIdentity,MICO_TID_DEF> UserIdentityList;
typedef TSeqVar< SequenceTmpl< UserIdentity,MICO_TID_DEF> > UserIdentityList_var;
typedef TSeqOut< SequenceTmpl< UserIdentity,MICO_TID_DEF> > UserIdentityList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UserIdentityList;

struct GSSAuthError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  GSSAuthError();
  ~GSSAuthError();
  GSSAuthError( const GSSAuthError& s );
  GSSAuthError& operator=( const GSSAuthError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  GSSAuthError();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  GSSAuthError( ::GSSUP::ErrorCode _m0 );

  #ifdef HAVE_STD_EH
  GSSAuthError *operator->() { return this; }
  GSSAuthError& operator*() { return *this; }
  operator GSSAuthError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static GSSAuthError *_downcast( CORBA::Exception *ex );
  static const GSSAuthError *_downcast( const CORBA::Exception *ex );
  ::GSSUP::ErrorCode reason;
};

#ifdef HAVE_STD_EH
typedef GSSAuthError GSSAuthError_catch;
#else
typedef ExceptVar< GSSAuthError > GSSAuthError_var;
typedef TVarOut< GSSAuthError > GSSAuthError_out;
typedef GSSAuthError_var GSSAuthError_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_GSSAuthError;

struct TrustIdentityError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  TrustIdentityError();
  ~TrustIdentityError();
  TrustIdentityError( const TrustIdentityError& s );
  TrustIdentityError& operator=( const TrustIdentityError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  TrustIdentityError *operator->() { return this; }
  TrustIdentityError& operator*() { return *this; }
  operator TrustIdentityError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static TrustIdentityError *_downcast( CORBA::Exception *ex );
  static const TrustIdentityError *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef TrustIdentityError TrustIdentityError_catch;
#else
typedef ExceptVar< TrustIdentityError > TrustIdentityError_var;
typedef TVarOut< TrustIdentityError > TrustIdentityError_out;
typedef TrustIdentityError_var TrustIdentityError_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TrustIdentityError;


/*
 * Base class and common definitions for local interface SecurityManager
 */

class SecurityManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~SecurityManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SecurityManager_ptr _ptr_type;
    typedef SecurityManager_var _var_type;
    #endif

    static SecurityManager_ptr _narrow( CORBA::Object_ptr obj );
    static SecurityManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SecurityManager_ptr _duplicate( SecurityManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SecurityManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CSIv2::TSS_ptr tss() = 0;
    virtual void tss( ::CSIv2::TSS_ptr value ) = 0;
    virtual ::CSIv2::CSS_ptr css() = 0;
    virtual void css( ::CSIv2::CSS_ptr value ) = 0;
    virtual CORBA::Boolean csiv2() = 0;
    virtual void csiv2( CORBA::Boolean value ) = 0;
    virtual ::IOP::CodecFactory_ptr codec_factory() = 0;
    virtual void codec_factory( ::IOP::CodecFactory_ptr value ) = 0;
    virtual ::IOP::Codec_ptr codec() = 0;
    virtual ::CSIv2::DistinguishedNameList* tls_user_list() = 0;
    virtual void tls_user_list( const ::CSIv2::DistinguishedNameList& value ) = 0;
    virtual ::CSIv2::UserIdentityList* user_id_list() = 0;
    virtual void user_id_list( const ::CSIv2::UserIdentityList& value ) = 0;
    virtual char* client_identity() = 0;
    virtual void client_identity( const char* value ) = 0;

    virtual ::CSI::GSSToken* get_gss_init_token( const ::CSI::GSS_NT_ExportedName& name ) = 0;
    virtual void auth_token( const ::CSI::GSSToken& token ) = 0;
    virtual ::CSI::GSSToken* error_token( ::GSSUP::ErrorCode reason ) = 0;
    virtual ::CORBA::OctetSeq* ior_component_data() = 0;
    virtual ::CSI::GSS_NT_ExportedName* client_identity_token() = 0;
    virtual void verify_client_identity( const ::CSI::IdentityToken& identity, CORBA::Boolean use_auth_layer ) = 0;

  protected:
    SecurityManager() {};
  private:
    SecurityManager( const SecurityManager& );
    void operator=( const SecurityManager& );
};



/*
 * Base class and common definitions for local interface CMSetup
 */

class CMSetup : 
  virtual public CORBA::Object
{
  public:
    virtual ~CMSetup();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef CMSetup_ptr _ptr_type;
    typedef CMSetup_var _var_type;
    #endif

    static CMSetup_ptr _narrow( CORBA::Object_ptr obj );
    static CMSetup_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static CMSetup_ptr _duplicate( CMSetup_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static CMSetup_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::UShort auth_layer() = 0;
    virtual void auth_layer( CORBA::UShort value ) = 0;
    virtual CORBA::UShort attr_layer() = 0;
    virtual void attr_layer( CORBA::UShort value ) = 0;

  protected:
    CMSetup() {};
  private:
    CMSetup( const CMSetup& );
    void operator=( const CMSetup& );
};


struct InvalidMechanism : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidMechanism();
  ~InvalidMechanism();
  InvalidMechanism( const InvalidMechanism& s );
  InvalidMechanism& operator=( const InvalidMechanism& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidMechanism *operator->() { return this; }
  InvalidMechanism& operator*() { return *this; }
  operator InvalidMechanism*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidMechanism *_downcast( CORBA::Exception *ex );
  static const InvalidMechanism *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidMechanism InvalidMechanism_catch;
#else
typedef ExceptVar< InvalidMechanism > InvalidMechanism_var;
typedef TVarOut< InvalidMechanism > InvalidMechanism_out;
typedef InvalidMechanism_var InvalidMechanism_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InvalidMechanism;


/*
 * Base class and common definitions for local interface TSS
 */

class TSS : 
  virtual public ::CSIv2::CMSetup
{
  public:
    virtual ~TSS();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef TSS_ptr _ptr_type;
    typedef TSS_var _var_type;
    #endif

    static TSS_ptr _narrow( CORBA::Object_ptr obj );
    static TSS_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static TSS_ptr _duplicate( TSS_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static TSS_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CSIv2::SecurityManager_ptr security_manager() = 0;
    virtual void security_manager( ::CSIv2::SecurityManager_ptr value ) = 0;

    virtual ::IOP::ServiceContext* accept_context( ::PortableInterceptor::ServerRequestInfo_ptr info, CORBA::Boolean_out exc ) = 0;
    virtual ::CSI::CompleteEstablishContext* establish_context( const ::CSI::EstablishContext& ctx ) = 0;
    virtual void accept_transport_context() = 0;
    virtual void receive_request_service_contexts( ::PortableInterceptor::ServerRequestInfo_ptr info ) = 0;
    virtual void receive_request( ::PortableInterceptor::ServerRequestInfo_ptr info ) = 0;
    virtual void send_reply( ::PortableInterceptor::ServerRequestInfo_ptr info ) = 0;
    virtual void send_exception( ::PortableInterceptor::ServerRequestInfo_ptr info ) = 0;
    virtual void send_other( ::PortableInterceptor::ServerRequestInfo_ptr info ) = 0;

  protected:
    TSS() {};
  private:
    TSS( const TSS& );
    void operator=( const TSS& );
};



/*
 * Base class and common definitions for local interface CSS
 */

class CSS : 
  virtual public ::CSIv2::CMSetup
{
  public:
    virtual ~CSS();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef CSS_ptr _ptr_type;
    typedef CSS_var _var_type;
    #endif

    static CSS_ptr _narrow( CORBA::Object_ptr obj );
    static CSS_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static CSS_ptr _duplicate( CSS_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static CSS_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CSIv2::SecurityManager_ptr security_manager() = 0;
    virtual void security_manager( ::CSIv2::SecurityManager_ptr value ) = 0;

    virtual void write_sas_context( ::PortableInterceptor::ClientRequestInfo_ptr info, const ::CSIIOP::CompoundSecMechList& list ) = 0;
    virtual void send_request( ::PortableInterceptor::ClientRequestInfo_ptr info ) = 0;
    virtual void receive_reply( ::PortableInterceptor::ClientRequestInfo_ptr info ) = 0;
    virtual void receive_exception( ::PortableInterceptor::ClientRequestInfo_ptr info ) = 0;
    virtual void receive_other( ::PortableInterceptor::ClientRequestInfo_ptr info ) = 0;

  protected:
    CSS() {};
  private:
    CSS( const CSS& );
    void operator=( const CSS& );
};


struct UserIdentity {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef UserIdentity_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  UserIdentity();
  ~UserIdentity();
  UserIdentity( const UserIdentity& s );
  UserIdentity& operator=( const UserIdentity& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var user_name;
  CORBA::String_var identity_name;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_UserIdentity;


/*
 * Base class and common definitions for local interface ServerRequestInterceptor
 */

class ServerRequestInterceptor : 
  virtual public ::PortableInterceptor::ServerRequestInterceptor
{
  public:
    virtual ~ServerRequestInterceptor();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServerRequestInterceptor_ptr _ptr_type;
    typedef ServerRequestInterceptor_var _var_type;
    #endif

    static ServerRequestInterceptor_ptr _narrow( CORBA::Object_ptr obj );
    static ServerRequestInterceptor_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServerRequestInterceptor_ptr _duplicate( ServerRequestInterceptor_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServerRequestInterceptor_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CSIv2::TSS_ptr tss() = 0;

  protected:
    ServerRequestInterceptor() {};
  private:
    ServerRequestInterceptor( const ServerRequestInterceptor& );
    void operator=( const ServerRequestInterceptor& );
};



/*
 * Base class and common definitions for local interface ClientRequestInterceptor
 */

class ClientRequestInterceptor : 
  virtual public ::PortableInterceptor::ClientRequestInterceptor
{
  public:
    virtual ~ClientRequestInterceptor();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ClientRequestInterceptor_ptr _ptr_type;
    typedef ClientRequestInterceptor_var _var_type;
    #endif

    static ClientRequestInterceptor_ptr _narrow( CORBA::Object_ptr obj );
    static ClientRequestInterceptor_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ClientRequestInterceptor_ptr _duplicate( ClientRequestInterceptor_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ClientRequestInterceptor_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CSIv2::CSS_ptr css() = 0;

  protected:
    ClientRequestInterceptor() {};
  private:
    ClientRequestInterceptor( const ClientRequestInterceptor& );
    void operator=( const ClientRequestInterceptor& );
};



/*
 * Base class and common definitions for local interface IORInterceptor
 */

class IORInterceptor : 
  virtual public ::PortableInterceptor::IORInterceptor
{
  public:
    virtual ~IORInterceptor();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IORInterceptor_ptr _ptr_type;
    typedef IORInterceptor_var _var_type;
    #endif

    static IORInterceptor_ptr _narrow( CORBA::Object_ptr obj );
    static IORInterceptor_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IORInterceptor_ptr _duplicate( IORInterceptor_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IORInterceptor_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    IORInterceptor() {};
  private:
    IORInterceptor( const IORInterceptor& );
    void operator=( const IORInterceptor& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::CSIv2::GSSAuthError &e );
void operator<<=( CORBA::Any &a, ::CSIv2::GSSAuthError *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::CSIv2::GSSAuthError &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::CSIv2::GSSAuthError *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIv2_GSSAuthError;

void operator<<=( CORBA::Any &a, const ::CSIv2::TrustIdentityError &e );
void operator<<=( CORBA::Any &a, ::CSIv2::TrustIdentityError *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::CSIv2::TrustIdentityError &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::CSIv2::TrustIdentityError *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIv2_TrustIdentityError;

void operator<<=( CORBA::Any &a, const ::CSIv2::InvalidMechanism &e );
void operator<<=( CORBA::Any &a, ::CSIv2::InvalidMechanism *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::CSIv2::InvalidMechanism &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::CSIv2::InvalidMechanism *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIv2_InvalidMechanism;

void operator<<=( CORBA::Any &_a, const ::CSIv2::UserIdentity &_s );
void operator<<=( CORBA::Any &_a, ::CSIv2::UserIdentity *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CSIv2::UserIdentity &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CSIv2::UserIdentity *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CSIv2_UserIdentity;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CSIv2::UserIdentity,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CSIv2_UserIdentity;

#endif
