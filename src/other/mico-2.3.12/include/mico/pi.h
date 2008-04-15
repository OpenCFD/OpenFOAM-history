/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __PI_H__
#define __PI_H__


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

class Codec;
typedef Codec *Codec_ptr;
typedef Codec_ptr CodecRef;
typedef ObjVar< Codec > Codec_var;
typedef ObjOut< Codec > Codec_out;

class CodecFactory;
typedef CodecFactory *CodecFactory_ptr;
typedef CodecFactory_ptr CodecFactoryRef;
typedef ObjVar< CodecFactory > CodecFactory_var;
typedef ObjOut< CodecFactory > CodecFactory_out;

}


namespace PortableInterceptor
{

class Interceptor;
typedef Interceptor *Interceptor_ptr;
typedef Interceptor_ptr InterceptorRef;
typedef ObjVar< Interceptor > Interceptor_var;
typedef ObjOut< Interceptor > Interceptor_out;

class Current;
typedef Current *Current_ptr;
typedef Current_ptr CurrentRef;
typedef ObjVar< Current > Current_var;
typedef ObjOut< Current > Current_out;

class RequestInfo;
typedef RequestInfo *RequestInfo_ptr;
typedef RequestInfo_ptr RequestInfoRef;
typedef ObjVar< RequestInfo > RequestInfo_var;
typedef ObjOut< RequestInfo > RequestInfo_out;

class ClientRequestInfo;
typedef ClientRequestInfo *ClientRequestInfo_ptr;
typedef ClientRequestInfo_ptr ClientRequestInfoRef;
typedef ObjVar< ClientRequestInfo > ClientRequestInfo_var;
typedef ObjOut< ClientRequestInfo > ClientRequestInfo_out;

class ServerRequestInfo;
typedef ServerRequestInfo *ServerRequestInfo_ptr;
typedef ServerRequestInfo_ptr ServerRequestInfoRef;
typedef ObjVar< ServerRequestInfo > ServerRequestInfo_var;
typedef ObjOut< ServerRequestInfo > ServerRequestInfo_out;

class ClientRequestInterceptor;
typedef ClientRequestInterceptor *ClientRequestInterceptor_ptr;
typedef ClientRequestInterceptor_ptr ClientRequestInterceptorRef;
typedef ObjVar< ClientRequestInterceptor > ClientRequestInterceptor_var;
typedef ObjOut< ClientRequestInterceptor > ClientRequestInterceptor_out;

class ServerRequestInterceptor;
typedef ServerRequestInterceptor *ServerRequestInterceptor_ptr;
typedef ServerRequestInterceptor_ptr ServerRequestInterceptorRef;
typedef ObjVar< ServerRequestInterceptor > ServerRequestInterceptor_var;
typedef ObjOut< ServerRequestInterceptor > ServerRequestInterceptor_out;

class IORInfo;
typedef IORInfo *IORInfo_ptr;
typedef IORInfo_ptr IORInfoRef;
typedef ObjVar< IORInfo > IORInfo_var;
typedef ObjOut< IORInfo > IORInfo_out;

class IORInterceptor;
typedef IORInterceptor *IORInterceptor_ptr;
typedef IORInterceptor_ptr IORInterceptorRef;
typedef ObjVar< IORInterceptor > IORInterceptor_var;
typedef ObjOut< IORInterceptor > IORInterceptor_out;

class PolicyFactory;
typedef PolicyFactory *PolicyFactory_ptr;
typedef PolicyFactory_ptr PolicyFactoryRef;
typedef ObjVar< PolicyFactory > PolicyFactory_var;
typedef ObjOut< PolicyFactory > PolicyFactory_out;

class ORBInitInfo;
typedef ORBInitInfo *ORBInitInfo_ptr;
typedef ORBInitInfo_ptr ORBInitInfoRef;
typedef ObjVar< ORBInitInfo > ORBInitInfo_var;
typedef ObjOut< ORBInitInfo > ORBInitInfo_out;

class ORBInitializer;
typedef ORBInitializer *ORBInitializer_ptr;
typedef ORBInitializer_ptr ORBInitializerRef;
typedef ObjVar< ORBInitializer > ORBInitializer_var;
typedef ObjOut< ORBInitializer > ORBInitializer_out;

}






namespace IOP
{

typedef SequenceTmpl< TaggedComponent,MICO_TID_DEF> TaggedComponentSeq;
typedef TSeqVar< SequenceTmpl< TaggedComponent,MICO_TID_DEF> > TaggedComponentSeq_var;
typedef TSeqOut< SequenceTmpl< TaggedComponent,MICO_TID_DEF> > TaggedComponentSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_TaggedComponentSeq;


/*
 * Base class and common definitions for local interface Codec
 */

class Codec : 
  virtual public CORBA::Object
{
  public:
    virtual ~Codec();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Codec_ptr _ptr_type;
    typedef Codec_var _var_type;
    #endif

    static Codec_ptr _narrow( CORBA::Object_ptr obj );
    static Codec_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Codec_ptr _duplicate( Codec_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Codec_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct InvalidTypeForEncoding : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      InvalidTypeForEncoding();
      ~InvalidTypeForEncoding();
      InvalidTypeForEncoding( const InvalidTypeForEncoding& s );
      InvalidTypeForEncoding& operator=( const InvalidTypeForEncoding& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      InvalidTypeForEncoding *operator->() { return this; }
      InvalidTypeForEncoding& operator*() { return *this; }
      operator InvalidTypeForEncoding*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static InvalidTypeForEncoding *_downcast( CORBA::Exception *ex );
      static const InvalidTypeForEncoding *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef InvalidTypeForEncoding InvalidTypeForEncoding_catch;
    #else
    typedef ExceptVar< InvalidTypeForEncoding > InvalidTypeForEncoding_var;
    typedef TVarOut< InvalidTypeForEncoding > InvalidTypeForEncoding_out;
    typedef InvalidTypeForEncoding_var InvalidTypeForEncoding_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_InvalidTypeForEncoding;

    struct FormatMismatch : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      FormatMismatch();
      ~FormatMismatch();
      FormatMismatch( const FormatMismatch& s );
      FormatMismatch& operator=( const FormatMismatch& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      FormatMismatch *operator->() { return this; }
      FormatMismatch& operator*() { return *this; }
      operator FormatMismatch*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static FormatMismatch *_downcast( CORBA::Exception *ex );
      static const FormatMismatch *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef FormatMismatch FormatMismatch_catch;
    #else
    typedef ExceptVar< FormatMismatch > FormatMismatch_var;
    typedef TVarOut< FormatMismatch > FormatMismatch_out;
    typedef FormatMismatch_var FormatMismatch_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_FormatMismatch;

    struct TypeMismatch : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      TypeMismatch();
      ~TypeMismatch();
      TypeMismatch( const TypeMismatch& s );
      TypeMismatch& operator=( const TypeMismatch& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      TypeMismatch *operator->() { return this; }
      TypeMismatch& operator*() { return *this; }
      operator TypeMismatch*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static TypeMismatch *_downcast( CORBA::Exception *ex );
      static const TypeMismatch *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef TypeMismatch TypeMismatch_catch;
    #else
    typedef ExceptVar< TypeMismatch > TypeMismatch_var;
    typedef TVarOut< TypeMismatch > TypeMismatch_out;
    typedef TypeMismatch_var TypeMismatch_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_TypeMismatch;

    virtual ::CORBA::OctetSeq* encode( const CORBA::Any& data ) = 0;
    virtual CORBA::Any* decode( const ::CORBA::OctetSeq& data ) = 0;
    virtual ::CORBA::OctetSeq* encode_value( const CORBA::Any& data ) = 0;
    virtual CORBA::Any* decode_value( const ::CORBA::OctetSeq& data, CORBA::TypeCode_ptr tc ) = 0;

  protected:
    Codec() {};
  private:
    Codec( const Codec& );
    void operator=( const Codec& );
};


typedef CORBA::Short EncodingFormat;
typedef EncodingFormat& EncodingFormat_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_EncodingFormat;

const EncodingFormat ENCODING_CDR_ENCAPS = 0;
struct Encoding;
typedef TFixVar< Encoding > Encoding_var;
typedef Encoding& Encoding_out;


struct Encoding {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef Encoding_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Encoding();
  ~Encoding();
  Encoding( const Encoding& s );
  Encoding& operator=( const Encoding& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  EncodingFormat format;
  CORBA::Octet major_version;
  CORBA::Octet minor_version;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Encoding;


/*
 * Base class and common definitions for local interface CodecFactory
 */

class CodecFactory : 
  virtual public CORBA::Object
{
  public:
    virtual ~CodecFactory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef CodecFactory_ptr _ptr_type;
    typedef CodecFactory_var _var_type;
    #endif

    static CodecFactory_ptr _narrow( CORBA::Object_ptr obj );
    static CodecFactory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static CodecFactory_ptr _duplicate( CodecFactory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static CodecFactory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    struct UnknownEncoding : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      UnknownEncoding();
      ~UnknownEncoding();
      UnknownEncoding( const UnknownEncoding& s );
      UnknownEncoding& operator=( const UnknownEncoding& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      UnknownEncoding *operator->() { return this; }
      UnknownEncoding& operator*() { return *this; }
      operator UnknownEncoding*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static UnknownEncoding *_downcast( CORBA::Exception *ex );
      static const UnknownEncoding *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef UnknownEncoding UnknownEncoding_catch;
    #else
    typedef ExceptVar< UnknownEncoding > UnknownEncoding_var;
    typedef TVarOut< UnknownEncoding > UnknownEncoding_out;
    typedef UnknownEncoding_var UnknownEncoding_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_UnknownEncoding;

    virtual Codec_ptr create_codec( const Encoding& enc ) = 0;

  protected:
    CodecFactory() {};
  private:
    CodecFactory( const CodecFactory& );
    void operator=( const CodecFactory& );
};


}


namespace CORBA
{

typedef StringSequenceTmpl<CORBA::String_var> StringSeq;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > StringSeq_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > StringSeq_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_StringSeq;

}


namespace Messaging
{

enum SyncScope {
  SYNC_NONE = 0,
  SYNC_WITH_TRANSPORT,
  SYNC_WITH_SERVER,
  SYNC_WITH_TARGET
};

typedef SyncScope& SyncScope_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_SyncScope;

}


namespace Dynamic
{

struct Parameter;
typedef TVarVar< Parameter > Parameter_var;
typedef TVarOut< Parameter > Parameter_out;


struct Parameter {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef Parameter_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Parameter();
  ~Parameter();
  Parameter( const Parameter& s );
  Parameter& operator=( const Parameter& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Any argument;
  ::CORBA::ParameterMode mode;
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_Parameter;

typedef SequenceTmpl< Parameter,MICO_TID_DEF> ParameterList;
typedef TSeqVar< SequenceTmpl< Parameter,MICO_TID_DEF> > ParameterList_var;
typedef TSeqOut< SequenceTmpl< Parameter,MICO_TID_DEF> > ParameterList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ParameterList;

typedef ::CORBA::StringSeq ContextList;
typedef TSeqVar< ::CORBA::StringSeq > ContextList_var;
typedef TSeqOut< ::CORBA::StringSeq > ContextList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ContextList;

typedef ::CORBA::TypeCodeSeq ExceptionList;
typedef TSeqVar< ::CORBA::TypeCodeSeq > ExceptionList_var;
typedef TSeqOut< ::CORBA::TypeCodeSeq > ExceptionList_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ExceptionList;

typedef ::CORBA::StringSeq RequestContext;
typedef TSeqVar< ::CORBA::StringSeq > RequestContext_var;
typedef TSeqOut< ::CORBA::StringSeq > RequestContext_out;

extern MICO_EXPORT CORBA::TypeCodeConst _tc_RequestContext;

}


namespace PortableInterceptor
{


/*
 * Base class and common definitions for local interface Interceptor
 */

class Interceptor : 
  virtual public CORBA::Object
{
  public:
    virtual ~Interceptor();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Interceptor_ptr _ptr_type;
    typedef Interceptor_var _var_type;
    #endif

    static Interceptor_ptr _narrow( CORBA::Object_ptr obj );
    static Interceptor_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Interceptor_ptr _duplicate( Interceptor_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Interceptor_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* name() = 0;

    virtual void destroy() = 0;

  protected:
    Interceptor() {};
  private:
    Interceptor( const Interceptor& );
    void operator=( const Interceptor& );
};


struct ForwardRequest : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ForwardRequest();
  ~ForwardRequest();
  ForwardRequest( const ForwardRequest& s );
  ForwardRequest& operator=( const ForwardRequest& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  ForwardRequest();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  ForwardRequest( CORBA::Object_ptr _m0, CORBA::Boolean _m1 );

  #ifdef HAVE_STD_EH
  ForwardRequest *operator->() { return this; }
  ForwardRequest& operator*() { return *this; }
  operator ForwardRequest*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static ForwardRequest *_downcast( CORBA::Exception *ex );
  static const ForwardRequest *_downcast( const CORBA::Exception *ex );
  CORBA::Object_var forward;
  CORBA::Boolean permanent;
};

#ifdef HAVE_STD_EH
typedef ForwardRequest ForwardRequest_catch;
#else
typedef ExceptVar< ForwardRequest > ForwardRequest_var;
typedef TVarOut< ForwardRequest > ForwardRequest_out;
typedef ForwardRequest_var ForwardRequest_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ForwardRequest;

typedef CORBA::Short ReplyStatus;
typedef ReplyStatus& ReplyStatus_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_ReplyStatus;

const ReplyStatus SUCCESSFUL = 0;
const ReplyStatus SYSTEM_EXCEPTION = 1;
const ReplyStatus USER_EXCEPTION = 2;
const ReplyStatus LOCATION_FORWARD = 3;
const ReplyStatus LOCATION_FORWARD_PERMANENT = 4;
const ReplyStatus TRANSPORT_RETRY = 5;
typedef CORBA::ULong SlotId;
typedef SlotId& SlotId_out;
extern MICO_EXPORT CORBA::TypeCodeConst _tc_SlotId;

struct InvalidSlot : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidSlot();
  ~InvalidSlot();
  InvalidSlot( const InvalidSlot& s );
  InvalidSlot& operator=( const InvalidSlot& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidSlot *operator->() { return this; }
  InvalidSlot& operator*() { return *this; }
  operator InvalidSlot*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidSlot *_downcast( CORBA::Exception *ex );
  static const InvalidSlot *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidSlot InvalidSlot_catch;
#else
typedef ExceptVar< InvalidSlot > InvalidSlot_var;
typedef TVarOut< InvalidSlot > InvalidSlot_out;
typedef InvalidSlot_var InvalidSlot_catch;
#endif // HAVE_STD_EH

extern MICO_EXPORT CORBA::TypeCodeConst _tc_InvalidSlot;


/*
 * Base class and common definitions for local interface Current
 */

class Current : 
  virtual public CORBA::Object,
  virtual public ::CORBA::Current
{
  public:
    virtual ~Current();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Current_ptr _ptr_type;
    typedef Current_var _var_type;
    #endif

    static Current_ptr _narrow( CORBA::Object_ptr obj );
    static Current_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Current_ptr _duplicate( Current_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Current_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Any* get_slot( SlotId id ) = 0;
    virtual void set_slot( SlotId id, const CORBA::Any& data ) = 0;

  protected:
    Current() {};
  private:
    Current( const Current& );
    void operator=( const Current& );
};



/*
 * Base class and common definitions for local interface RequestInfo
 */

class RequestInfo : 
  virtual public CORBA::Object
{
  public:
    virtual ~RequestInfo();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef RequestInfo_ptr _ptr_type;
    typedef RequestInfo_var _var_type;
    #endif

    static RequestInfo_ptr _narrow( CORBA::Object_ptr obj );
    static RequestInfo_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static RequestInfo_ptr _duplicate( RequestInfo_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static RequestInfo_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::ULong request_id() = 0;
    virtual char* operation() = 0;
    virtual ::Dynamic::ParameterList* arguments() = 0;
    virtual ::Dynamic::ExceptionList* exceptions() = 0;
    virtual ::Dynamic::ContextList* contexts() = 0;
    virtual ::Dynamic::RequestContext* operation_context() = 0;
    virtual CORBA::Any* result() = 0;
    virtual CORBA::Boolean response_expected() = 0;
    virtual ::Messaging::SyncScope sync_scope() = 0;
    virtual ::PortableInterceptor::ReplyStatus reply_status() = 0;
    virtual CORBA::Object_ptr forward_reference() = 0;

    virtual CORBA::Any* get_slot( SlotId id ) = 0;
    virtual ::IOP::ServiceContext* get_request_service_context( ::IOP::ServiceID id ) = 0;
    virtual ::IOP::ServiceContext* get_reply_service_context( ::IOP::ServiceID id ) = 0;

  protected:
    RequestInfo() {};
  private:
    RequestInfo( const RequestInfo& );
    void operator=( const RequestInfo& );
};



/*
 * Base class and common definitions for local interface ClientRequestInfo
 */

class ClientRequestInfo : 
  virtual public ::PortableInterceptor::RequestInfo
{
  public:
    virtual ~ClientRequestInfo();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ClientRequestInfo_ptr _ptr_type;
    typedef ClientRequestInfo_var _var_type;
    #endif

    static ClientRequestInfo_ptr _narrow( CORBA::Object_ptr obj );
    static ClientRequestInfo_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ClientRequestInfo_ptr _duplicate( ClientRequestInfo_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ClientRequestInfo_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr target() = 0;
    virtual CORBA::Object_ptr effective_target() = 0;
    virtual ::IOP::TaggedProfile* effective_profile() = 0;
    virtual CORBA::Any* received_exception() = 0;
    virtual char* received_exception_id() = 0;

    virtual ::IOP::TaggedComponent* get_effective_component( ::IOP::ComponentId id ) = 0;
    virtual ::IOP::TaggedComponentSeq* get_effective_components( ::IOP::ComponentId id ) = 0;
    virtual ::CORBA::Policy_ptr get_request_policy( ::CORBA::PolicyType type ) = 0;
    virtual void add_request_service_context( const ::IOP::ServiceContext& service_context, CORBA::Boolean replace ) = 0;

  protected:
    ClientRequestInfo() {};
  private:
    ClientRequestInfo( const ClientRequestInfo& );
    void operator=( const ClientRequestInfo& );
};



/*
 * Base class and common definitions for local interface ServerRequestInfo
 */

class ServerRequestInfo : 
  virtual public ::PortableInterceptor::RequestInfo
{
  public:
    virtual ~ServerRequestInfo();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServerRequestInfo_ptr _ptr_type;
    typedef ServerRequestInfo_var _var_type;
    #endif

    static ServerRequestInfo_ptr _narrow( CORBA::Object_ptr obj );
    static ServerRequestInfo_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServerRequestInfo_ptr _duplicate( ServerRequestInfo_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServerRequestInfo_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Any* sending_exception() = 0;
    virtual ::CORBA::OctetSeq* object_id() = 0;
    virtual ::CORBA::OctetSeq* adapter_id() = 0;
    virtual char* target_most_derived_interface() = 0;

    virtual ::CORBA::Policy_ptr get_server_policy( ::CORBA::PolicyType type ) = 0;
    virtual void set_slot( SlotId id, const CORBA::Any& data ) = 0;
    virtual CORBA::Boolean target_is_a( const char* id ) = 0;
    virtual void add_reply_service_context( const ::IOP::ServiceContext& service_context, CORBA::Boolean replace ) = 0;

  protected:
    ServerRequestInfo() {};
  private:
    ServerRequestInfo( const ServerRequestInfo& );
    void operator=( const ServerRequestInfo& );
};



/*
 * Base class and common definitions for local interface ClientRequestInterceptor
 */

class ClientRequestInterceptor : 
  virtual public ::PortableInterceptor::Interceptor
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

    virtual void send_request( ClientRequestInfo_ptr ri ) = 0;
    virtual void send_poll( ClientRequestInfo_ptr ri ) = 0;
    virtual void receive_reply( ClientRequestInfo_ptr ri ) = 0;
    virtual void receive_exception( ClientRequestInfo_ptr ri ) = 0;
    virtual void receive_other( ClientRequestInfo_ptr ri ) = 0;

  protected:
    ClientRequestInterceptor() {};
  private:
    ClientRequestInterceptor( const ClientRequestInterceptor& );
    void operator=( const ClientRequestInterceptor& );
};



/*
 * Base class and common definitions for local interface ServerRequestInterceptor
 */

class ServerRequestInterceptor : 
  virtual public ::PortableInterceptor::Interceptor
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

    virtual void receive_request_service_contexts( ServerRequestInfo_ptr ri ) = 0;
    virtual void receive_request( ServerRequestInfo_ptr ri ) = 0;
    virtual void send_reply( ServerRequestInfo_ptr ri ) = 0;
    virtual void send_exception( ServerRequestInfo_ptr ri ) = 0;
    virtual void send_other( ServerRequestInfo_ptr ri ) = 0;

  protected:
    ServerRequestInterceptor() {};
  private:
    ServerRequestInterceptor( const ServerRequestInterceptor& );
    void operator=( const ServerRequestInterceptor& );
};



/*
 * Base class and common definitions for local interface IORInfo
 */

class IORInfo : 
  virtual public CORBA::Object
{
  public:
    virtual ~IORInfo();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IORInfo_ptr _ptr_type;
    typedef IORInfo_var _var_type;
    #endif

    static IORInfo_ptr _narrow( CORBA::Object_ptr obj );
    static IORInfo_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IORInfo_ptr _duplicate( IORInfo_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IORInfo_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::Policy_ptr get_effective_policy( ::CORBA::PolicyType type ) = 0;
    virtual void add_ior_component( const ::IOP::TaggedComponent& comp ) = 0;
    virtual void add_ior_component_to_profile( const ::IOP::TaggedComponent& comp, ::IOP::ProfileId profile_id ) = 0;

  protected:
    IORInfo() {};
  private:
    IORInfo( const IORInfo& );
    void operator=( const IORInfo& );
};



/*
 * Base class and common definitions for local interface IORInterceptor
 */

class IORInterceptor : 
  virtual public ::PortableInterceptor::Interceptor
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

    virtual void establish_components( IORInfo_ptr info ) = 0;

  protected:
    IORInterceptor() {};
  private:
    IORInterceptor( const IORInterceptor& );
    void operator=( const IORInterceptor& );
};



/*
 * Base class and common definitions for local interface PolicyFactory
 */

class PolicyFactory : 
  virtual public CORBA::Object
{
  public:
    virtual ~PolicyFactory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef PolicyFactory_ptr _ptr_type;
    typedef PolicyFactory_var _var_type;
    #endif

    static PolicyFactory_ptr _narrow( CORBA::Object_ptr obj );
    static PolicyFactory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static PolicyFactory_ptr _duplicate( PolicyFactory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static PolicyFactory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::Policy_ptr create_policy( ::CORBA::PolicyType type, const CORBA::Any& value ) = 0;

  protected:
    PolicyFactory() {};
  private:
    PolicyFactory( const PolicyFactory& );
    void operator=( const PolicyFactory& );
};



/*
 * Base class and common definitions for local interface ORBInitInfo
 */

class ORBInitInfo : 
  virtual public CORBA::Object
{
  public:
    virtual ~ORBInitInfo();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ORBInitInfo_ptr _ptr_type;
    typedef ORBInitInfo_var _var_type;
    #endif

    static ORBInitInfo_ptr _narrow( CORBA::Object_ptr obj );
    static ORBInitInfo_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ORBInitInfo_ptr _duplicate( ORBInitInfo_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ORBInitInfo_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    typedef char* ObjectId;
    typedef CORBA::String_var ObjectId_var;
    typedef CORBA::String_out ObjectId_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectId;

    struct DuplicateName : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      DuplicateName();
      ~DuplicateName();
      DuplicateName( const DuplicateName& s );
      DuplicateName& operator=( const DuplicateName& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      #ifndef HAVE_EXPLICIT_STRUCT_OPS
      DuplicateName();
      #endif //HAVE_EXPLICIT_STRUCT_OPS
      DuplicateName( const char* _m0 );

      #ifdef HAVE_STD_EH
      DuplicateName *operator->() { return this; }
      DuplicateName& operator*() { return *this; }
      operator DuplicateName*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static DuplicateName *_downcast( CORBA::Exception *ex );
      static const DuplicateName *_downcast( const CORBA::Exception *ex );
      CORBA::String_var name;
    };

    #ifdef HAVE_STD_EH
    typedef DuplicateName DuplicateName_catch;
    #else
    typedef ExceptVar< DuplicateName > DuplicateName_var;
    typedef TVarOut< DuplicateName > DuplicateName_out;
    typedef DuplicateName_var DuplicateName_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_DuplicateName;

    struct InvalidName : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      InvalidName();
      ~InvalidName();
      InvalidName( const InvalidName& s );
      InvalidName& operator=( const InvalidName& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      InvalidName *operator->() { return this; }
      InvalidName& operator*() { return *this; }
      operator InvalidName*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static InvalidName *_downcast( CORBA::Exception *ex );
      static const InvalidName *_downcast( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef InvalidName InvalidName_catch;
    #else
    typedef ExceptVar< InvalidName > InvalidName_var;
    typedef TVarOut< InvalidName > InvalidName_out;
    typedef InvalidName_var InvalidName_catch;
    #endif // HAVE_STD_EH

    static MICO_EXPORT CORBA::TypeCodeConst _tc_InvalidName;

    virtual ::CORBA::StringSeq* arguments() = 0;
    virtual char* orb_id() = 0;
    virtual ::IOP::CodecFactory_ptr codec_factory() = 0;

    virtual void register_initial_reference( const char* id, CORBA::Object_ptr obj ) = 0;
    virtual CORBA::Object_ptr resolve_initial_references( const char* id ) = 0;
    virtual void add_client_request_interceptor( ClientRequestInterceptor_ptr interceptor ) = 0;
    virtual void add_server_request_interceptor( ServerRequestInterceptor_ptr interceptor ) = 0;
    virtual void add_ior_interceptor( IORInterceptor_ptr interceptor ) = 0;
    virtual SlotId allocate_slot_id() = 0;
    virtual void register_policy_factory( ::CORBA::PolicyType type, PolicyFactory_ptr policy_factory ) = 0;

  protected:
    ORBInitInfo() {};
  private:
    ORBInitInfo( const ORBInitInfo& );
    void operator=( const ORBInitInfo& );
};



/*
 * Base class and common definitions for local interface ORBInitializer
 */

class ORBInitializer : 
  virtual public CORBA::Object
{
  public:
    virtual ~ORBInitializer();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ORBInitializer_ptr _ptr_type;
    typedef ORBInitializer_var _var_type;
    #endif

    static ORBInitializer_ptr _narrow( CORBA::Object_ptr obj );
    static ORBInitializer_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ORBInitializer_ptr _duplicate( ORBInitializer_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ORBInitializer_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void pre_init( ORBInitInfo_ptr info ) = 0;
    virtual void post_init( ORBInitInfo_ptr info ) = 0;

  protected:
    ORBInitializer() {};
  private:
    ORBInitializer( const ORBInitializer& );
    void operator=( const ORBInitializer& );
};


}


#ifndef MICO_CONF_NO_POA

#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::IOP::Codec::InvalidTypeForEncoding &e );
void operator<<=( CORBA::Any &a, ::IOP::Codec::InvalidTypeForEncoding *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::IOP::Codec::InvalidTypeForEncoding &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::IOP::Codec::InvalidTypeForEncoding *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_Codec_InvalidTypeForEncoding;

void operator<<=( CORBA::Any &a, const ::IOP::Codec::FormatMismatch &e );
void operator<<=( CORBA::Any &a, ::IOP::Codec::FormatMismatch *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::IOP::Codec::FormatMismatch &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::IOP::Codec::FormatMismatch *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_Codec_FormatMismatch;

void operator<<=( CORBA::Any &a, const ::IOP::Codec::TypeMismatch &e );
void operator<<=( CORBA::Any &a, ::IOP::Codec::TypeMismatch *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::IOP::Codec::TypeMismatch &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::IOP::Codec::TypeMismatch *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_Codec_TypeMismatch;

void operator<<=( CORBA::Any &_a, const ::IOP::Encoding &_s );
void operator<<=( CORBA::Any &_a, ::IOP::Encoding *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::IOP::Encoding &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::IOP::Encoding *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_Encoding;

void operator<<=( CORBA::Any &a, const ::IOP::CodecFactory::UnknownEncoding &e );
void operator<<=( CORBA::Any &a, ::IOP::CodecFactory::UnknownEncoding *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::IOP::CodecFactory::UnknownEncoding &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::IOP::CodecFactory::UnknownEncoding *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_IOP_CodecFactory_UnknownEncoding;

void operator<<=( CORBA::Any &a, const ::Messaging::SyncScope &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Messaging::SyncScope &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Messaging_SyncScope;

void operator<<=( CORBA::Any &_a, const ::Dynamic::Parameter &_s );
void operator<<=( CORBA::Any &_a, ::Dynamic::Parameter *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::Dynamic::Parameter &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::Dynamic::Parameter *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_Dynamic_Parameter;

void operator<<=( CORBA::Any &a, const ::PortableInterceptor::ForwardRequest &e );
void operator<<=( CORBA::Any &a, ::PortableInterceptor::ForwardRequest *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableInterceptor::ForwardRequest &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableInterceptor::ForwardRequest *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_ForwardRequest;

void operator<<=( CORBA::Any &a, const ::PortableInterceptor::InvalidSlot &e );
void operator<<=( CORBA::Any &a, ::PortableInterceptor::InvalidSlot *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableInterceptor::InvalidSlot &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableInterceptor::InvalidSlot *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_InvalidSlot;

void operator<<=( CORBA::Any &a, const ::PortableInterceptor::ORBInitInfo::DuplicateName &e );
void operator<<=( CORBA::Any &a, ::PortableInterceptor::ORBInitInfo::DuplicateName *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableInterceptor::ORBInitInfo::DuplicateName &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableInterceptor::ORBInitInfo::DuplicateName *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_ORBInitInfo_DuplicateName;

void operator<<=( CORBA::Any &a, const ::PortableInterceptor::ORBInitInfo::InvalidName &e );
void operator<<=( CORBA::Any &a, ::PortableInterceptor::ORBInitInfo::InvalidName *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::PortableInterceptor::ORBInitInfo::InvalidName &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::PortableInterceptor::ORBInitInfo::InvalidName *&e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_PortableInterceptor_ORBInitInfo_InvalidName;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Dynamic::Parameter,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Dynamic_Parameter;

#endif
