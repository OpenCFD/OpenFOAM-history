/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>

#ifndef __IMR_H__
#define __IMR_H__


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

class ImplementationDef;
typedef ImplementationDef *ImplementationDef_ptr;
typedef ImplementationDef_ptr ImplementationDefRef;
typedef ObjVar< ImplementationDef > ImplementationDef_var;
typedef ObjOut< ImplementationDef > ImplementationDef_out;

class ImplRepository;
typedef ImplRepository *ImplRepository_ptr;
typedef ImplRepository_ptr ImplRepositoryRef;
typedef ObjVar< ImplRepository > ImplRepository_var;
typedef ObjOut< ImplRepository > ImplRepository_out;

class POAMediator;
typedef POAMediator *POAMediator_ptr;
typedef POAMediator_ptr POAMediatorRef;
typedef ObjVar< POAMediator > POAMediator_var;
typedef ObjOut< POAMediator > POAMediator_out;

}






namespace CORBA
{


/*
 * Base class and common definitions for interface ImplementationDef
 */

class ImplementationDef : 
  virtual public CORBA::Object
{
  public:
    virtual ~ImplementationDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ImplementationDef_ptr _ptr_type;
    typedef ImplementationDef_var _var_type;
    #endif

    static ImplementationDef_ptr _narrow( CORBA::Object_ptr obj );
    static ImplementationDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ImplementationDef_ptr _duplicate( ImplementationDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ImplementationDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    enum ActivationMode {
      ActivatePOA = 0
    };

    typedef ActivationMode& ActivationMode_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ActivationMode;

    typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> ObjectTag;
    typedef TSeqVar< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > ObjectTag_var;
    typedef TSeqOut< SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> > ObjectTag_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectTag;

    struct ObjectInfo;
    typedef TVarVar< ObjectInfo > ObjectInfo_var;
    typedef TVarOut< ObjectInfo > ObjectInfo_out;


    struct ObjectInfo {
      #ifdef HAVE_TYPEDEF_OVERLOAD
      typedef ObjectInfo_var _var_type;
      #endif
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      ObjectInfo();
      ~ObjectInfo();
      ObjectInfo( const ObjectInfo& s );
      ObjectInfo& operator=( const ObjectInfo& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS

      CORBA::String_var repoid;
      ObjectTag tag;
    };

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectInfo;

    typedef SequenceTmpl< ObjectInfo,MICO_TID_DEF> ObjectInfoList;
    typedef TSeqVar< SequenceTmpl< ObjectInfo,MICO_TID_DEF> > ObjectInfoList_var;
    typedef TSeqOut< SequenceTmpl< ObjectInfo,MICO_TID_DEF> > ObjectInfoList_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ObjectInfoList;

    virtual ::CORBA::ImplementationDef::ActivationMode mode() = 0;
    virtual void mode( ::CORBA::ImplementationDef::ActivationMode value ) = 0;
    virtual ::CORBA::ImplementationDef::ObjectInfoList* objs() = 0;
    virtual void objs( const ::CORBA::ImplementationDef::ObjectInfoList& value ) = 0;
    virtual char* name() = 0;
    virtual char* command() = 0;
    virtual void command( const char* value ) = 0;
    virtual char* tostring() = 0;

  protected:
    ImplementationDef() {};
  private:
    ImplementationDef( const ImplementationDef& );
    void operator=( const ImplementationDef& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ImplementationDef;

// Stub for interface ImplementationDef
class ImplementationDef_stub:
  virtual public ImplementationDef
{
  public:
    virtual ~ImplementationDef_stub();
    ::CORBA::ImplementationDef::ActivationMode mode();
    void mode( ::CORBA::ImplementationDef::ActivationMode value );
    ::CORBA::ImplementationDef::ObjectInfoList* objs();
    void objs( const ::CORBA::ImplementationDef::ObjectInfoList& value );
    char* name();
    char* command();
    void command( const char* value );
    char* tostring();

  private:
    void operator=( const ImplementationDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ImplementationDef_stub_clp :
  virtual public ImplementationDef_stub,
  virtual public PortableServer::StubBase
{
  public:
    ImplementationDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ImplementationDef_stub_clp ();
    ::CORBA::ImplementationDef::ActivationMode mode();
    void mode( ::CORBA::ImplementationDef::ActivationMode value );
    ::CORBA::ImplementationDef::ObjectInfoList* objs();
    void objs( const ::CORBA::ImplementationDef::ObjectInfoList& value );
    char* name();
    char* command();
    void command( const char* value );
    char* tostring();

  protected:
    ImplementationDef_stub_clp ();
  private:
    void operator=( const ImplementationDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ImplRepository
 */

class ImplRepository : 
  virtual public CORBA::Object
{
  public:
    virtual ~ImplRepository();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ImplRepository_ptr _ptr_type;
    typedef ImplRepository_var _var_type;
    #endif

    static ImplRepository_ptr _narrow( CORBA::Object_ptr obj );
    static ImplRepository_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ImplRepository_ptr _duplicate( ImplRepository_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ImplRepository_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    typedef IfaceSequenceTmpl< ImplementationDef_var,ImplementationDef_ptr> ImplDefSeq;
    typedef TSeqVar< IfaceSequenceTmpl< ImplementationDef_var,ImplementationDef_ptr> > ImplDefSeq_var;
    typedef TSeqOut< IfaceSequenceTmpl< ImplementationDef_var,ImplementationDef_ptr> > ImplDefSeq_out;

    static MICO_EXPORT CORBA::TypeCodeConst _tc_ImplDefSeq;

    virtual ImplementationDef_ptr restore( const char* asstring ) = 0;
    virtual ImplementationDef_ptr create( ImplementationDef::ActivationMode mode, const ImplementationDef::ObjectInfoList& objs, const char* name, const char* command ) = 0;
    virtual void destroy( ImplementationDef_ptr impl_def ) = 0;
    virtual ImplDefSeq* find_by_name( const char* name ) = 0;
    virtual ImplDefSeq* find_by_repoid( const char* repoid ) = 0;
    virtual ImplDefSeq* find_by_repoid_tag( const char* repoid, const ImplementationDef::ObjectTag& tag ) = 0;
    virtual ImplDefSeq* find_all() = 0;

  protected:
    ImplRepository() {};
  private:
    ImplRepository( const ImplRepository& );
    void operator=( const ImplRepository& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_ImplRepository;

// Stub for interface ImplRepository
class ImplRepository_stub:
  virtual public ImplRepository
{
  public:
    virtual ~ImplRepository_stub();
    ImplementationDef_ptr restore( const char* asstring );
    ImplementationDef_ptr create( ImplementationDef::ActivationMode mode, const ImplementationDef::ObjectInfoList& objs, const char* name, const char* command );
    void destroy( ImplementationDef_ptr impl_def );
    ImplDefSeq* find_by_name( const char* name );
    ImplDefSeq* find_by_repoid( const char* repoid );
    ImplDefSeq* find_by_repoid_tag( const char* repoid, const ImplementationDef::ObjectTag& tag );
    ImplDefSeq* find_all();

  private:
    void operator=( const ImplRepository_stub& );
};

#ifndef MICO_CONF_NO_POA

class ImplRepository_stub_clp :
  virtual public ImplRepository_stub,
  virtual public PortableServer::StubBase
{
  public:
    ImplRepository_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ImplRepository_stub_clp ();
    ImplementationDef_ptr restore( const char* asstring );
    ImplementationDef_ptr create( ImplementationDef::ActivationMode mode, const ImplementationDef::ObjectInfoList& objs, const char* name, const char* command );
    void destroy( ImplementationDef_ptr impl_def );
    ImplDefSeq* find_by_name( const char* name );
    ImplDefSeq* find_by_repoid( const char* repoid );
    ImplDefSeq* find_by_repoid_tag( const char* repoid, const ImplementationDef::ObjectTag& tag );
    ImplDefSeq* find_all();

  protected:
    ImplRepository_stub_clp ();
  private:
    void operator=( const ImplRepository_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface POAMediator
 */

class POAMediator : 
  virtual public CORBA::Object
{
  public:
    virtual ~POAMediator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef POAMediator_ptr _ptr_type;
    typedef POAMediator_var _var_type;
    #endif

    static POAMediator_ptr _narrow( CORBA::Object_ptr obj );
    static POAMediator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static POAMediator_ptr _duplicate( POAMediator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static POAMediator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* create_impl( const char* svid, const char* ior ) = 0;
    virtual void activate_impl( const char* svid ) = 0;
    virtual void deactivate_impl( const char* svid ) = 0;
    virtual CORBA::Boolean force_activation( ImplementationDef_ptr impl ) = 0;
    virtual CORBA::Boolean hold( ImplementationDef_ptr impl ) = 0;
    virtual CORBA::Boolean stop( ImplementationDef_ptr impl ) = 0;
    virtual CORBA::Boolean _cxx_continue( ImplementationDef_ptr impl ) = 0;
    virtual void shutdown_server() = 0;

  protected:
    POAMediator() {};
  private:
    POAMediator( const POAMediator& );
    void operator=( const POAMediator& );
};

extern MICO_EXPORT CORBA::TypeCodeConst _tc_POAMediator;

// Stub for interface POAMediator
class POAMediator_stub:
  virtual public POAMediator
{
  public:
    virtual ~POAMediator_stub();
    char* create_impl( const char* svid, const char* ior );
    void activate_impl( const char* svid );
    void deactivate_impl( const char* svid );
    CORBA::Boolean force_activation( ImplementationDef_ptr impl );
    CORBA::Boolean hold( ImplementationDef_ptr impl );
    CORBA::Boolean stop( ImplementationDef_ptr impl );
    CORBA::Boolean _cxx_continue( ImplementationDef_ptr impl );
    void shutdown_server();

  private:
    void operator=( const POAMediator_stub& );
};

#ifndef MICO_CONF_NO_POA

class POAMediator_stub_clp :
  virtual public POAMediator_stub,
  virtual public PortableServer::StubBase
{
  public:
    POAMediator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~POAMediator_stub_clp ();
    char* create_impl( const char* svid, const char* ior );
    void activate_impl( const char* svid );
    void deactivate_impl( const char* svid );
    CORBA::Boolean force_activation( ImplementationDef_ptr impl );
    CORBA::Boolean hold( ImplementationDef_ptr impl );
    CORBA::Boolean stop( ImplementationDef_ptr impl );
    CORBA::Boolean _cxx_continue( ImplementationDef_ptr impl );
    void shutdown_server();

  protected:
    POAMediator_stub_clp ();
  private:
    void operator=( const POAMediator_stub_clp & );
};

#endif // MICO_CONF_NO_POA

}


#ifndef MICO_CONF_NO_POA



namespace POA_CORBA
{

class ImplementationDef : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ImplementationDef ();
    CORBA::ImplementationDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ImplementationDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ImplementationDef::ActivationMode mode() = 0;
    virtual void mode( ::CORBA::ImplementationDef::ActivationMode value ) = 0;
    virtual ::CORBA::ImplementationDef::ObjectInfoList* objs() = 0;
    virtual void objs( const ::CORBA::ImplementationDef::ObjectInfoList& value ) = 0;
    virtual char* name() = 0;
    virtual char* command() = 0;
    virtual void command( const char* value ) = 0;
    virtual char* tostring() = 0;

  protected:
    ImplementationDef () {};

  private:
    ImplementationDef (const ImplementationDef &);
    void operator= (const ImplementationDef &);
};

class ImplRepository : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ImplRepository ();
    CORBA::ImplRepository_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ImplRepository * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ImplementationDef_ptr restore( const char* asstring ) = 0;
    virtual ::CORBA::ImplementationDef_ptr create( ::CORBA::ImplementationDef::ActivationMode mode, const ::CORBA::ImplementationDef::ObjectInfoList& objs, const char* name, const char* command ) = 0;
    virtual void destroy( ::CORBA::ImplementationDef_ptr impl_def ) = 0;
    virtual ::CORBA::ImplRepository::ImplDefSeq* find_by_name( const char* name ) = 0;
    virtual ::CORBA::ImplRepository::ImplDefSeq* find_by_repoid( const char* repoid ) = 0;
    virtual ::CORBA::ImplRepository::ImplDefSeq* find_by_repoid_tag( const char* repoid, const ::CORBA::ImplementationDef::ObjectTag& tag ) = 0;
    virtual ::CORBA::ImplRepository::ImplDefSeq* find_all() = 0;

  protected:
    ImplRepository () {};

  private:
    ImplRepository (const ImplRepository &);
    void operator= (const ImplRepository &);
};

class POAMediator : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~POAMediator ();
    CORBA::POAMediator_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static POAMediator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* create_impl( const char* svid, const char* ior ) = 0;
    virtual void activate_impl( const char* svid ) = 0;
    virtual void deactivate_impl( const char* svid ) = 0;
    virtual CORBA::Boolean force_activation( ::CORBA::ImplementationDef_ptr impl ) = 0;
    virtual CORBA::Boolean hold( ::CORBA::ImplementationDef_ptr impl ) = 0;
    virtual CORBA::Boolean stop( ::CORBA::ImplementationDef_ptr impl ) = 0;
    virtual CORBA::Boolean _cxx_continue( ::CORBA::ImplementationDef_ptr impl ) = 0;
    virtual void shutdown_server() = 0;

  protected:
    POAMediator () {};

  private:
    POAMediator (const POAMediator &);
    void operator= (const POAMediator &);
};

}


#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::CORBA::ImplementationDef::ActivationMode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::CORBA::ImplementationDef::ActivationMode &e );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ImplementationDef_ActivationMode;

void operator<<=( CORBA::Any &_a, const ::CORBA::ImplementationDef::ObjectInfo &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ImplementationDef::ObjectInfo *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ImplementationDef::ObjectInfo &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ImplementationDef::ObjectInfo *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ImplementationDef_ObjectInfo;

void operator<<=( CORBA::Any &a, const CORBA::ImplementationDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ImplementationDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ImplementationDef_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ImplementationDef;

void operator<<=( CORBA::Any &a, const CORBA::ImplRepository_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ImplRepository_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ImplRepository_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_ImplRepository;

void operator<<=( CORBA::Any &a, const CORBA::POAMediator_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::POAMediator_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::POAMediator_ptr &obj );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller_CORBA_POAMediator;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ImplementationDef::ObjectInfo,MICO_TID_DEF> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ImplementationDef_ObjectInfo;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< CORBA::ImplementationDef_var,CORBA::ImplementationDef_ptr> *&_s );

extern MICO_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ImplementationDef;

#endif
