/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>

#ifndef __CCM_H__
#define __CCM_H__


#ifdef _WIN32
// If you compile this file and would like to link
// it into the DLL, then please define ``BUILD_CCM_DLL'' symbol
// on your C++ compiler command-line.
// Depending on your compiler, you can use either
// ``-DBUILD_CCM_DLL'' or ``/DBUILD_CCM_DLL'' parameter
#ifdef BUILD_CCM_DLL
// Please do not forget to use gendef tool to correctly
// export all symbols from the resulting object file.
// If you would like to use more common __declspec(dllexport) way,
// then please use --windows-dll-with-export <name>
// IDL compiler parameter
#define CCM_EXPORT /**/
#else // BUILD_CCM_DLL
#define CCM_EXPORT __declspec(dllimport)
#endif // BUILD_CCM_DLL
#else // _WIN32
// all other platforms
#define CCM_EXPORT /**/
#endif // _WIN32




namespace Components
{

class Navigation;
typedef Navigation *Navigation_ptr;
typedef Navigation_ptr NavigationRef;
typedef ObjVar< Navigation > Navigation_var;
typedef ObjOut< Navigation > Navigation_out;

class Receptacles;
typedef Receptacles *Receptacles_ptr;
typedef Receptacles_ptr ReceptaclesRef;
typedef ObjVar< Receptacles > Receptacles_var;
typedef ObjOut< Receptacles > Receptacles_out;

class EventConsumerBase;
typedef EventConsumerBase *EventConsumerBase_ptr;
typedef EventConsumerBase_ptr EventConsumerBaseRef;
typedef ObjVar< EventConsumerBase > EventConsumerBase_var;
typedef ObjOut< EventConsumerBase > EventConsumerBase_out;

class Events;
typedef Events *Events_ptr;
typedef Events_ptr EventsRef;
typedef ObjVar< Events > Events_var;
typedef ObjOut< Events > Events_out;

class CCMObject;
typedef CCMObject *CCMObject_ptr;
typedef CCMObject_ptr CCMObjectRef;
typedef ObjVar< CCMObject > CCMObject_var;
typedef ObjOut< CCMObject > CCMObject_out;

class CCMHome;
typedef CCMHome *CCMHome_ptr;
typedef CCMHome_ptr CCMHomeRef;
typedef ObjVar< CCMHome > CCMHome_var;
typedef ObjOut< CCMHome > CCMHome_out;

class KeylessCCMHome;
typedef KeylessCCMHome *KeylessCCMHome_ptr;
typedef KeylessCCMHome_ptr KeylessCCMHomeRef;
typedef ObjVar< KeylessCCMHome > KeylessCCMHome_var;
typedef ObjOut< KeylessCCMHome > KeylessCCMHome_out;

class HomeFinder;
typedef HomeFinder *HomeFinder_ptr;
typedef HomeFinder_ptr HomeFinderRef;
typedef ObjVar< HomeFinder > HomeFinder_var;
typedef ObjOut< HomeFinder > HomeFinder_out;

class Configurator;
typedef Configurator *Configurator_ptr;
typedef Configurator_ptr ConfiguratorRef;
typedef ObjVar< Configurator > Configurator_var;
typedef ObjOut< Configurator > Configurator_out;

class StandardConfigurator;
typedef StandardConfigurator *StandardConfigurator_ptr;
typedef StandardConfigurator_ptr StandardConfiguratorRef;
typedef ObjVar< StandardConfigurator > StandardConfigurator_var;
typedef ObjOut< StandardConfigurator > StandardConfigurator_out;

class CCMContext;
typedef CCMContext *CCMContext_ptr;
typedef CCMContext_ptr CCMContextRef;
typedef ObjVar< CCMContext > CCMContext_var;
typedef ObjOut< CCMContext > CCMContext_out;

class SessionContext;
typedef SessionContext *SessionContext_ptr;
typedef SessionContext_ptr SessionContextRef;
typedef ObjVar< SessionContext > SessionContext_var;
typedef ObjOut< SessionContext > SessionContext_out;

class EnterpriseComponent;
typedef EnterpriseComponent *EnterpriseComponent_ptr;
typedef EnterpriseComponent_ptr EnterpriseComponentRef;
typedef ObjVar< EnterpriseComponent > EnterpriseComponent_var;
typedef ObjOut< EnterpriseComponent > EnterpriseComponent_out;

class SessionComponent;
typedef SessionComponent *SessionComponent_ptr;
typedef SessionComponent_ptr SessionComponentRef;
typedef ObjVar< SessionComponent > SessionComponent_var;
typedef ObjOut< SessionComponent > SessionComponent_out;

class HomeExecutorBase;
typedef HomeExecutorBase *HomeExecutorBase_ptr;
typedef HomeExecutorBase_ptr HomeExecutorBaseRef;
typedef ObjVar< HomeExecutorBase > HomeExecutorBase_var;
typedef ObjOut< HomeExecutorBase > HomeExecutorBase_out;

class ExecutorLocator;
typedef ExecutorLocator *ExecutorLocator_ptr;
typedef ExecutorLocator_ptr ExecutorLocatorRef;
typedef ObjVar< ExecutorLocator > ExecutorLocator_var;
typedef ObjOut< ExecutorLocator > ExecutorLocator_out;

class ComponentServer;
typedef ComponentServer *ComponentServer_ptr;
typedef ComponentServer_ptr ComponentServerRef;
typedef ObjVar< ComponentServer > ComponentServer_var;
typedef ObjOut< ComponentServer > ComponentServer_out;

class Container;
typedef Container *Container_ptr;
typedef Container_ptr ContainerRef;
typedef ObjVar< Container > Container_var;
typedef ObjOut< Container > Container_out;

class ServerActivator;
typedef ServerActivator *ServerActivator_ptr;
typedef ServerActivator_ptr ServerActivatorRef;
typedef ObjVar< ServerActivator > ServerActivator_var;
typedef ObjOut< ServerActivator > ServerActivator_out;

class ComponentInstallation;
typedef ComponentInstallation *ComponentInstallation_ptr;
typedef ComponentInstallation_ptr ComponentInstallationRef;
typedef ObjVar< ComponentInstallation > ComponentInstallation_var;
typedef ObjOut< ComponentInstallation > ComponentInstallation_out;

class Assembly;
typedef Assembly *Assembly_ptr;
typedef Assembly_ptr AssemblyRef;
typedef ObjVar< Assembly > Assembly_var;
typedef ObjOut< Assembly > Assembly_out;

class AssemblyFactory;
typedef AssemblyFactory *AssemblyFactory_ptr;
typedef AssemblyFactory_ptr AssemblyFactoryRef;
typedef ObjVar< AssemblyFactory > AssemblyFactory_var;
typedef ObjOut< AssemblyFactory > AssemblyFactory_out;

}


namespace MICOCCM
{

class MicoCCMD;
typedef MicoCCMD *MicoCCMD_ptr;
typedef MicoCCMD_ptr MicoCCMDRef;
typedef ObjVar< MicoCCMD > MicoCCMD_var;
typedef ObjOut< MicoCCMD > MicoCCMD_out;

class ComponentServer;
typedef ComponentServer *ComponentServer_ptr;
typedef ComponentServer_ptr ComponentServerRef;
typedef ObjVar< ComponentServer > ComponentServer_var;
typedef ObjOut< ComponentServer > ComponentServer_out;

}






namespace Components
{

typedef char* FeatureName;
typedef CORBA::String_var FeatureName_var;
typedef CORBA::String_out FeatureName_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_FeatureName;

typedef StringSequenceTmpl<CORBA::String_var> NameList;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > NameList_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > NameList_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_NameList;

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

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InvalidName;

struct InvalidConfiguration : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidConfiguration();
  ~InvalidConfiguration();
  InvalidConfiguration( const InvalidConfiguration& s );
  InvalidConfiguration& operator=( const InvalidConfiguration& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidConfiguration *operator->() { return this; }
  InvalidConfiguration& operator*() { return *this; }
  operator InvalidConfiguration*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidConfiguration *_downcast( CORBA::Exception *ex );
  static const InvalidConfiguration *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidConfiguration InvalidConfiguration_catch;
#else
typedef ExceptVar< InvalidConfiguration > InvalidConfiguration_var;
typedef TVarOut< InvalidConfiguration > InvalidConfiguration_out;
typedef InvalidConfiguration_var InvalidConfiguration_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InvalidConfiguration;

struct AlreadyConnected : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  AlreadyConnected();
  ~AlreadyConnected();
  AlreadyConnected( const AlreadyConnected& s );
  AlreadyConnected& operator=( const AlreadyConnected& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  AlreadyConnected *operator->() { return this; }
  AlreadyConnected& operator*() { return *this; }
  operator AlreadyConnected*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static AlreadyConnected *_downcast( CORBA::Exception *ex );
  static const AlreadyConnected *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef AlreadyConnected AlreadyConnected_catch;
#else
typedef ExceptVar< AlreadyConnected > AlreadyConnected_var;
typedef TVarOut< AlreadyConnected > AlreadyConnected_out;
typedef AlreadyConnected_var AlreadyConnected_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_AlreadyConnected;

struct InvalidConnection : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidConnection();
  ~InvalidConnection();
  InvalidConnection( const InvalidConnection& s );
  InvalidConnection& operator=( const InvalidConnection& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidConnection *operator->() { return this; }
  InvalidConnection& operator*() { return *this; }
  operator InvalidConnection*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidConnection *_downcast( CORBA::Exception *ex );
  static const InvalidConnection *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidConnection InvalidConnection_catch;
#else
typedef ExceptVar< InvalidConnection > InvalidConnection_var;
typedef TVarOut< InvalidConnection > InvalidConnection_out;
typedef InvalidConnection_var InvalidConnection_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InvalidConnection;

struct NoConnection : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  NoConnection();
  ~NoConnection();
  NoConnection( const NoConnection& s );
  NoConnection& operator=( const NoConnection& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  NoConnection *operator->() { return this; }
  NoConnection& operator*() { return *this; }
  operator NoConnection*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static NoConnection *_downcast( CORBA::Exception *ex );
  static const NoConnection *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef NoConnection NoConnection_catch;
#else
typedef ExceptVar< NoConnection > NoConnection_var;
typedef TVarOut< NoConnection > NoConnection_out;
typedef NoConnection_var NoConnection_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_NoConnection;

struct ExceededConnectionLimit : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ExceededConnectionLimit();
  ~ExceededConnectionLimit();
  ExceededConnectionLimit( const ExceededConnectionLimit& s );
  ExceededConnectionLimit& operator=( const ExceededConnectionLimit& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  ExceededConnectionLimit *operator->() { return this; }
  ExceededConnectionLimit& operator*() { return *this; }
  operator ExceededConnectionLimit*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static ExceededConnectionLimit *_downcast( CORBA::Exception *ex );
  static const ExceededConnectionLimit *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef ExceededConnectionLimit ExceededConnectionLimit_catch;
#else
typedef ExceptVar< ExceededConnectionLimit > ExceededConnectionLimit_var;
typedef TVarOut< ExceededConnectionLimit > ExceededConnectionLimit_out;
typedef ExceededConnectionLimit_var ExceededConnectionLimit_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ExceededConnectionLimit;

struct CookieRequired : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CookieRequired();
  ~CookieRequired();
  CookieRequired( const CookieRequired& s );
  CookieRequired& operator=( const CookieRequired& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  CookieRequired *operator->() { return this; }
  CookieRequired& operator*() { return *this; }
  operator CookieRequired*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static CookieRequired *_downcast( CORBA::Exception *ex );
  static const CookieRequired *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef CookieRequired CookieRequired_catch;
#else
typedef ExceptVar< CookieRequired > CookieRequired_var;
typedef TVarOut< CookieRequired > CookieRequired_out;
typedef CookieRequired_var CookieRequired_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CookieRequired;

class PortDescription;
typedef PortDescription *PortDescription_ptr;
typedef PortDescription_ptr PortDescriptionRef;
typedef ValueVar< PortDescription > PortDescription_var;
typedef ValueOut< PortDescription > PortDescription_out;


// Common definitions for valuetype PortDescription
class PortDescription : 
  virtual public CORBA::ValueBase
{
  public:
    static PortDescription* _downcast (CORBA::ValueBase *);
    static PortDescription* _downcast (CORBA::AbstractBase *);

    virtual void name( char *_p ) = 0;
    virtual void name( const char *_p ) = 0;
    virtual void name( const CORBA::String_var &_p ) = 0;
    virtual const char *name() const = 0;

    virtual void type_id( char *_p ) = 0;
    virtual void type_id( const char *_p ) = 0;
    virtual void type_id( const CORBA::String_var &_p ) = 0;
    virtual const char *type_id() const = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    PortDescription ();
    virtual ~PortDescription ();
    void _copy_members (const PortDescription&);

  private:
    PortDescription (const PortDescription &);
    void operator= (const PortDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_PortDescription;

class Cookie;
typedef Cookie *Cookie_ptr;
typedef Cookie_ptr CookieRef;
typedef ValueVar< Cookie > Cookie_var;
typedef ValueOut< Cookie > Cookie_out;


// Common definitions for valuetype Cookie
class Cookie : 
  virtual public CORBA::ValueBase
{
  public:
    static Cookie* _downcast (CORBA::ValueBase *);
    static Cookie* _downcast (CORBA::AbstractBase *);

    typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _CookieValue_seq;
    virtual void CookieValue( const _CookieValue_seq & _p) = 0;
    virtual const _CookieValue_seq & CookieValue() const = 0;
    virtual _CookieValue_seq & CookieValue() = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    Cookie ();
    virtual ~Cookie ();
    void _copy_members (const Cookie&);

  private:
    Cookie (const Cookie &);
    void operator= (const Cookie &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Cookie;

class EventBase;
typedef EventBase *EventBase_ptr;
typedef EventBase_ptr EventBaseRef;
typedef ValueVar< EventBase > EventBase_var;
typedef ValueOut< EventBase > EventBase_out;


// Common definitions for valuetype EventBase
class EventBase : 
  virtual public CORBA::ValueBase
{
  public:
    static EventBase* _downcast (CORBA::ValueBase *);
    static EventBase* _downcast (CORBA::AbstractBase *);

  public:
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    EventBase ();
    virtual ~EventBase ();

  private:
    EventBase (const EventBase &);
    void operator= (const EventBase &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_EventBase;

class FacetDescription;
typedef FacetDescription *FacetDescription_ptr;
typedef FacetDescription_ptr FacetDescriptionRef;
typedef ValueVar< FacetDescription > FacetDescription_var;
typedef ValueOut< FacetDescription > FacetDescription_out;


// Common definitions for valuetype FacetDescription
class FacetDescription : 
  virtual public ::Components::PortDescription
{
  public:
    static FacetDescription* _downcast (CORBA::ValueBase *);
    static FacetDescription* _downcast (CORBA::AbstractBase *);

    virtual void facet_ref( CORBA::Object_ptr _p ) = 0;
    virtual CORBA::Object_ptr facet_ref() const = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    FacetDescription ();
    virtual ~FacetDescription ();
    void _copy_members (const FacetDescription&);

  private:
    FacetDescription (const FacetDescription &);
    void operator= (const FacetDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_FacetDescription;

typedef SequenceTmpl< FacetDescription_var,MICO_TID_DEF> FacetDescriptions;
typedef TSeqVar< SequenceTmpl< FacetDescription_var,MICO_TID_DEF> > FacetDescriptions_var;
typedef TSeqOut< SequenceTmpl< FacetDescription_var,MICO_TID_DEF> > FacetDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_FacetDescriptions;


/*
 * Base class and common definitions for interface Navigation
 */

class Navigation : 
  virtual public CORBA::Object
{
  public:
    virtual ~Navigation();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Navigation_ptr _ptr_type;
    typedef Navigation_var _var_type;
    #endif

    static Navigation_ptr _narrow( CORBA::Object_ptr obj );
    static Navigation_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Navigation_ptr _duplicate( Navigation_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Navigation_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr provide_facet( const char* name ) = 0;
    virtual FacetDescriptions* get_all_facets() = 0;
    virtual FacetDescriptions* get_named_facets( const NameList& names ) = 0;
    virtual CORBA::Boolean same_component( CORBA::Object_ptr ref ) = 0;

  protected:
    Navigation() {};
  private:
    Navigation( const Navigation& );
    void operator=( const Navigation& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Navigation;

// Stub for interface Navigation
class Navigation_stub:
  virtual public Navigation
{
  public:
    virtual ~Navigation_stub();
    CORBA::Object_ptr provide_facet( const char* name );
    FacetDescriptions* get_all_facets();
    FacetDescriptions* get_named_facets( const NameList& names );
    CORBA::Boolean same_component( CORBA::Object_ptr ref );

  private:
    void operator=( const Navigation_stub& );
};

#ifndef MICO_CONF_NO_POA

class Navigation_stub_clp :
  virtual public Navigation_stub,
  virtual public PortableServer::StubBase
{
  public:
    Navigation_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Navigation_stub_clp ();
    CORBA::Object_ptr provide_facet( const char* name );
    FacetDescriptions* get_all_facets();
    FacetDescriptions* get_named_facets( const NameList& names );
    CORBA::Boolean same_component( CORBA::Object_ptr ref );

  protected:
    Navigation_stub_clp ();
  private:
    void operator=( const Navigation_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class ConnectionDescription;
typedef ConnectionDescription *ConnectionDescription_ptr;
typedef ConnectionDescription_ptr ConnectionDescriptionRef;
typedef ValueVar< ConnectionDescription > ConnectionDescription_var;
typedef ValueOut< ConnectionDescription > ConnectionDescription_out;


// Common definitions for valuetype ConnectionDescription
class ConnectionDescription : 
  virtual public CORBA::ValueBase
{
  public:
    static ConnectionDescription* _downcast (CORBA::ValueBase *);
    static ConnectionDescription* _downcast (CORBA::AbstractBase *);

    virtual void ck( Cookie* _p ) = 0;
    virtual Cookie* ck() const = 0;

    virtual void objref( CORBA::Object_ptr _p ) = 0;
    virtual CORBA::Object_ptr objref() const = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    ConnectionDescription ();
    virtual ~ConnectionDescription ();
    void _copy_members (const ConnectionDescription&);

  private:
    ConnectionDescription (const ConnectionDescription &);
    void operator= (const ConnectionDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ConnectionDescription;

typedef SequenceTmpl< ConnectionDescription_var,MICO_TID_DEF> ConnectionDescriptions;
typedef TSeqVar< SequenceTmpl< ConnectionDescription_var,MICO_TID_DEF> > ConnectionDescriptions_var;
typedef TSeqOut< SequenceTmpl< ConnectionDescription_var,MICO_TID_DEF> > ConnectionDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ConnectionDescriptions;

class ReceptacleDescription;
typedef ReceptacleDescription *ReceptacleDescription_ptr;
typedef ReceptacleDescription_ptr ReceptacleDescriptionRef;
typedef ValueVar< ReceptacleDescription > ReceptacleDescription_var;
typedef ValueOut< ReceptacleDescription > ReceptacleDescription_out;


// Common definitions for valuetype ReceptacleDescription
class ReceptacleDescription : 
  virtual public ::Components::PortDescription
{
  public:
    static ReceptacleDescription* _downcast (CORBA::ValueBase *);
    static ReceptacleDescription* _downcast (CORBA::AbstractBase *);

    virtual void is_multiplex( CORBA::Boolean _p ) = 0;
    virtual CORBA::Boolean is_multiplex() const = 0;

    virtual void connections( const ConnectionDescriptions& _p ) = 0;
    virtual const ConnectionDescriptions& connections() const = 0;
    virtual ConnectionDescriptions& connections() = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    ReceptacleDescription ();
    virtual ~ReceptacleDescription ();
    void _copy_members (const ReceptacleDescription&);

  private:
    ReceptacleDescription (const ReceptacleDescription &);
    void operator= (const ReceptacleDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ReceptacleDescription;

typedef SequenceTmpl< ReceptacleDescription_var,MICO_TID_DEF> ReceptacleDescriptions;
typedef TSeqVar< SequenceTmpl< ReceptacleDescription_var,MICO_TID_DEF> > ReceptacleDescriptions_var;
typedef TSeqOut< SequenceTmpl< ReceptacleDescription_var,MICO_TID_DEF> > ReceptacleDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ReceptacleDescriptions;


/*
 * Base class and common definitions for interface Receptacles
 */

class Receptacles : 
  virtual public CORBA::Object
{
  public:
    virtual ~Receptacles();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Receptacles_ptr _ptr_type;
    typedef Receptacles_var _var_type;
    #endif

    static Receptacles_ptr _narrow( CORBA::Object_ptr obj );
    static Receptacles_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Receptacles_ptr _duplicate( Receptacles_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Receptacles_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Cookie* connect( const char* name, CORBA::Object_ptr connection ) = 0;
    virtual void disconnect( const char* name, Cookie* ck ) = 0;
    virtual ConnectionDescriptions* get_connections( const char* name ) = 0;
    virtual ReceptacleDescriptions* get_all_receptacles() = 0;
    virtual ReceptacleDescriptions* get_named_receptacles( const NameList& names ) = 0;

  protected:
    Receptacles() {};
  private:
    Receptacles( const Receptacles& );
    void operator=( const Receptacles& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Receptacles;

// Stub for interface Receptacles
class Receptacles_stub:
  virtual public Receptacles
{
  public:
    virtual ~Receptacles_stub();
    Cookie* connect( const char* name, CORBA::Object_ptr connection );
    void disconnect( const char* name, Cookie* ck );
    ConnectionDescriptions* get_connections( const char* name );
    ReceptacleDescriptions* get_all_receptacles();
    ReceptacleDescriptions* get_named_receptacles( const NameList& names );

  private:
    void operator=( const Receptacles_stub& );
};

#ifndef MICO_CONF_NO_POA

class Receptacles_stub_clp :
  virtual public Receptacles_stub,
  virtual public PortableServer::StubBase
{
  public:
    Receptacles_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Receptacles_stub_clp ();
    Cookie* connect( const char* name, CORBA::Object_ptr connection );
    void disconnect( const char* name, Cookie* ck );
    ConnectionDescriptions* get_connections( const char* name );
    ReceptacleDescriptions* get_all_receptacles();
    ReceptacleDescriptions* get_named_receptacles( const NameList& names );

  protected:
    Receptacles_stub_clp ();
  private:
    void operator=( const Receptacles_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct BadEventType : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  BadEventType();
  ~BadEventType();
  BadEventType( const BadEventType& s );
  BadEventType& operator=( const BadEventType& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  BadEventType();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  BadEventType( const char* _m0 );

  #ifdef HAVE_STD_EH
  BadEventType *operator->() { return this; }
  BadEventType& operator*() { return *this; }
  operator BadEventType*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static BadEventType *_downcast( CORBA::Exception *ex );
  static const BadEventType *_downcast( const CORBA::Exception *ex );
  CORBA::String_var expected_event_type;
};

#ifdef HAVE_STD_EH
typedef BadEventType BadEventType_catch;
#else
typedef ExceptVar< BadEventType > BadEventType_var;
typedef TVarOut< BadEventType > BadEventType_out;
typedef BadEventType_var BadEventType_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_BadEventType;


/*
 * Base class and common definitions for interface EventConsumerBase
 */

class EventConsumerBase : 
  virtual public CORBA::Object
{
  public:
    virtual ~EventConsumerBase();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EventConsumerBase_ptr _ptr_type;
    typedef EventConsumerBase_var _var_type;
    #endif

    static EventConsumerBase_ptr _narrow( CORBA::Object_ptr obj );
    static EventConsumerBase_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EventConsumerBase_ptr _duplicate( EventConsumerBase_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EventConsumerBase_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void push_event( EventBase* evt ) = 0;

  protected:
    EventConsumerBase() {};
  private:
    EventConsumerBase( const EventConsumerBase& );
    void operator=( const EventConsumerBase& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_EventConsumerBase;

// Stub for interface EventConsumerBase
class EventConsumerBase_stub:
  virtual public EventConsumerBase
{
  public:
    virtual ~EventConsumerBase_stub();
    void push_event( EventBase* evt );

  private:
    void operator=( const EventConsumerBase_stub& );
};

#ifndef MICO_CONF_NO_POA

class EventConsumerBase_stub_clp :
  virtual public EventConsumerBase_stub,
  virtual public PortableServer::StubBase
{
  public:
    EventConsumerBase_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~EventConsumerBase_stub_clp ();
    void push_event( EventBase* evt );

  protected:
    EventConsumerBase_stub_clp ();
  private:
    void operator=( const EventConsumerBase_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class ConsumerDescription;
typedef ConsumerDescription *ConsumerDescription_ptr;
typedef ConsumerDescription_ptr ConsumerDescriptionRef;
typedef ValueVar< ConsumerDescription > ConsumerDescription_var;
typedef ValueOut< ConsumerDescription > ConsumerDescription_out;


// Common definitions for valuetype ConsumerDescription
class ConsumerDescription : 
  virtual public ::Components::PortDescription
{
  public:
    static ConsumerDescription* _downcast (CORBA::ValueBase *);
    static ConsumerDescription* _downcast (CORBA::AbstractBase *);

    virtual void consumer( EventConsumerBase_ptr _p ) = 0;
    virtual EventConsumerBase_ptr consumer() const = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    ConsumerDescription ();
    virtual ~ConsumerDescription ();
    void _copy_members (const ConsumerDescription&);

  private:
    ConsumerDescription (const ConsumerDescription &);
    void operator= (const ConsumerDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ConsumerDescription;

typedef SequenceTmpl< ConsumerDescription_var,MICO_TID_DEF> ConsumerDescriptions;
typedef TSeqVar< SequenceTmpl< ConsumerDescription_var,MICO_TID_DEF> > ConsumerDescriptions_var;
typedef TSeqOut< SequenceTmpl< ConsumerDescription_var,MICO_TID_DEF> > ConsumerDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ConsumerDescriptions;

class EmitterDescription;
typedef EmitterDescription *EmitterDescription_ptr;
typedef EmitterDescription_ptr EmitterDescriptionRef;
typedef ValueVar< EmitterDescription > EmitterDescription_var;
typedef ValueOut< EmitterDescription > EmitterDescription_out;


// Common definitions for valuetype EmitterDescription
class EmitterDescription : 
  virtual public ::Components::PortDescription
{
  public:
    static EmitterDescription* _downcast (CORBA::ValueBase *);
    static EmitterDescription* _downcast (CORBA::AbstractBase *);

    virtual void consumer( EventConsumerBase_ptr _p ) = 0;
    virtual EventConsumerBase_ptr consumer() const = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    EmitterDescription ();
    virtual ~EmitterDescription ();
    void _copy_members (const EmitterDescription&);

  private:
    EmitterDescription (const EmitterDescription &);
    void operator= (const EmitterDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_EmitterDescription;

typedef SequenceTmpl< EmitterDescription_var,MICO_TID_DEF> EmitterDescriptions;
typedef TSeqVar< SequenceTmpl< EmitterDescription_var,MICO_TID_DEF> > EmitterDescriptions_var;
typedef TSeqOut< SequenceTmpl< EmitterDescription_var,MICO_TID_DEF> > EmitterDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_EmitterDescriptions;

class SubscriberDescription;
typedef SubscriberDescription *SubscriberDescription_ptr;
typedef SubscriberDescription_ptr SubscriberDescriptionRef;
typedef ValueVar< SubscriberDescription > SubscriberDescription_var;
typedef ValueOut< SubscriberDescription > SubscriberDescription_out;


// Common definitions for valuetype SubscriberDescription
class SubscriberDescription : 
  virtual public CORBA::ValueBase
{
  public:
    static SubscriberDescription* _downcast (CORBA::ValueBase *);
    static SubscriberDescription* _downcast (CORBA::AbstractBase *);

    virtual void ck( Cookie* _p ) = 0;
    virtual Cookie* ck() const = 0;

    virtual void consumer( EventConsumerBase_ptr _p ) = 0;
    virtual EventConsumerBase_ptr consumer() const = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    SubscriberDescription ();
    virtual ~SubscriberDescription ();
    void _copy_members (const SubscriberDescription&);

  private:
    SubscriberDescription (const SubscriberDescription &);
    void operator= (const SubscriberDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_SubscriberDescription;

typedef SequenceTmpl< SubscriberDescription_var,MICO_TID_DEF> SubscriberDescriptions;
typedef TSeqVar< SequenceTmpl< SubscriberDescription_var,MICO_TID_DEF> > SubscriberDescriptions_var;
typedef TSeqOut< SequenceTmpl< SubscriberDescription_var,MICO_TID_DEF> > SubscriberDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_SubscriberDescriptions;

class PublisherDescription;
typedef PublisherDescription *PublisherDescription_ptr;
typedef PublisherDescription_ptr PublisherDescriptionRef;
typedef ValueVar< PublisherDescription > PublisherDescription_var;
typedef ValueOut< PublisherDescription > PublisherDescription_out;


// Common definitions for valuetype PublisherDescription
class PublisherDescription : 
  virtual public ::Components::PortDescription
{
  public:
    static PublisherDescription* _downcast (CORBA::ValueBase *);
    static PublisherDescription* _downcast (CORBA::AbstractBase *);

    virtual void consumers( const SubscriberDescriptions& _p ) = 0;
    virtual const SubscriberDescriptions& consumers() const = 0;
    virtual SubscriberDescriptions& consumers() = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    PublisherDescription ();
    virtual ~PublisherDescription ();
    void _copy_members (const PublisherDescription&);

  private:
    PublisherDescription (const PublisherDescription &);
    void operator= (const PublisherDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_PublisherDescription;

typedef SequenceTmpl< PublisherDescription_var,MICO_TID_DEF> PublisherDescriptions;
typedef TSeqVar< SequenceTmpl< PublisherDescription_var,MICO_TID_DEF> > PublisherDescriptions_var;
typedef TSeqOut< SequenceTmpl< PublisherDescription_var,MICO_TID_DEF> > PublisherDescriptions_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_PublisherDescriptions;


/*
 * Base class and common definitions for interface Events
 */

class Events : 
  virtual public CORBA::Object
{
  public:
    virtual ~Events();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Events_ptr _ptr_type;
    typedef Events_var _var_type;
    #endif

    static Events_ptr _narrow( CORBA::Object_ptr obj );
    static Events_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Events_ptr _duplicate( Events_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Events_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual EventConsumerBase_ptr get_consumer( const char* sink_name ) = 0;
    virtual ConsumerDescriptions* get_all_consumers() = 0;
    virtual ConsumerDescriptions* get_named_consumers( const NameList& names ) = 0;
    virtual Cookie* subscribe( const char* publisher_name, EventConsumerBase_ptr subscriber ) = 0;
    virtual void unsubscribe( const char* publisher_name, Cookie* ck ) = 0;
    virtual PublisherDescriptions* get_all_publishers() = 0;
    virtual PublisherDescriptions* get_named_publishers( const NameList& names ) = 0;
    virtual void connect_consumer( const char* emitter_name, EventConsumerBase_ptr consumer ) = 0;
    virtual EventConsumerBase_ptr disconnect_consumer( const char* source_name ) = 0;
    virtual EmitterDescriptions* get_all_emitters() = 0;
    virtual EmitterDescriptions* get_named_emitters( const NameList& names ) = 0;

  protected:
    Events() {};
  private:
    Events( const Events& );
    void operator=( const Events& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Events;

// Stub for interface Events
class Events_stub:
  virtual public Events
{
  public:
    virtual ~Events_stub();
    EventConsumerBase_ptr get_consumer( const char* sink_name );
    ConsumerDescriptions* get_all_consumers();
    ConsumerDescriptions* get_named_consumers( const NameList& names );
    Cookie* subscribe( const char* publisher_name, EventConsumerBase_ptr subscriber );
    void unsubscribe( const char* publisher_name, Cookie* ck );
    PublisherDescriptions* get_all_publishers();
    PublisherDescriptions* get_named_publishers( const NameList& names );
    void connect_consumer( const char* emitter_name, EventConsumerBase_ptr consumer );
    EventConsumerBase_ptr disconnect_consumer( const char* source_name );
    EmitterDescriptions* get_all_emitters();
    EmitterDescriptions* get_named_emitters( const NameList& names );

  private:
    void operator=( const Events_stub& );
};

#ifndef MICO_CONF_NO_POA

class Events_stub_clp :
  virtual public Events_stub,
  virtual public PortableServer::StubBase
{
  public:
    Events_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Events_stub_clp ();
    EventConsumerBase_ptr get_consumer( const char* sink_name );
    ConsumerDescriptions* get_all_consumers();
    ConsumerDescriptions* get_named_consumers( const NameList& names );
    Cookie* subscribe( const char* publisher_name, EventConsumerBase_ptr subscriber );
    void unsubscribe( const char* publisher_name, Cookie* ck );
    PublisherDescriptions* get_all_publishers();
    PublisherDescriptions* get_named_publishers( const NameList& names );
    void connect_consumer( const char* emitter_name, EventConsumerBase_ptr consumer );
    EventConsumerBase_ptr disconnect_consumer( const char* source_name );
    EmitterDescriptions* get_all_emitters();
    EmitterDescriptions* get_named_emitters( const NameList& names );

  protected:
    Events_stub_clp ();
  private:
    void operator=( const Events_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface CCMHome
 */

class CCMHome : 
  virtual public CORBA::Object
{
  public:
    virtual ~CCMHome();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef CCMHome_ptr _ptr_type;
    typedef CCMHome_var _var_type;
    #endif

    static CCMHome_ptr _narrow( CORBA::Object_ptr obj );
    static CCMHome_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static CCMHome_ptr _duplicate( CCMHome_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static CCMHome_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr get_component_def() = 0;
    virtual CORBA::Object_ptr get_home_def() = 0;
    virtual void remove_component( CCMObject_ptr comp ) = 0;

  protected:
    CCMHome() {};
  private:
    CCMHome( const CCMHome& );
    void operator=( const CCMHome& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CCMHome;

// Stub for interface CCMHome
class CCMHome_stub:
  virtual public CCMHome
{
  public:
    virtual ~CCMHome_stub();
    CORBA::Object_ptr get_component_def();
    CORBA::Object_ptr get_home_def();
    void remove_component( CCMObject_ptr comp );

  private:
    void operator=( const CCMHome_stub& );
};

#ifndef MICO_CONF_NO_POA

class CCMHome_stub_clp :
  virtual public CCMHome_stub,
  virtual public PortableServer::StubBase
{
  public:
    CCMHome_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~CCMHome_stub_clp ();
    CORBA::Object_ptr get_component_def();
    CORBA::Object_ptr get_home_def();
    void remove_component( CCMObject_ptr comp );

  protected:
    CCMHome_stub_clp ();
  private:
    void operator=( const CCMHome_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class ComponentPortDescription;
typedef ComponentPortDescription *ComponentPortDescription_ptr;
typedef ComponentPortDescription_ptr ComponentPortDescriptionRef;
typedef ValueVar< ComponentPortDescription > ComponentPortDescription_var;
typedef ValueOut< ComponentPortDescription > ComponentPortDescription_out;


// Common definitions for valuetype ComponentPortDescription
class ComponentPortDescription : 
  virtual public CORBA::ValueBase
{
  public:
    static ComponentPortDescription* _downcast (CORBA::ValueBase *);
    static ComponentPortDescription* _downcast (CORBA::AbstractBase *);

    virtual void facets( const FacetDescriptions& _p ) = 0;
    virtual const FacetDescriptions& facets() const = 0;
    virtual FacetDescriptions& facets() = 0;

    virtual void receptacles( const ReceptacleDescriptions& _p ) = 0;
    virtual const ReceptacleDescriptions& receptacles() const = 0;
    virtual ReceptacleDescriptions& receptacles() = 0;

    virtual void consumers( const ConsumerDescriptions& _p ) = 0;
    virtual const ConsumerDescriptions& consumers() const = 0;
    virtual ConsumerDescriptions& consumers() = 0;

    virtual void emitters( const EmitterDescriptions& _p ) = 0;
    virtual const EmitterDescriptions& emitters() const = 0;
    virtual EmitterDescriptions& emitters() = 0;

    virtual void publishers( const PublisherDescriptions& _p ) = 0;
    virtual const PublisherDescriptions& publishers() const = 0;
    virtual PublisherDescriptions& publishers() = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    ComponentPortDescription ();
    virtual ~ComponentPortDescription ();
    void _copy_members (const ComponentPortDescription&);

  private:
    ComponentPortDescription (const ComponentPortDescription &);
    void operator= (const ComponentPortDescription &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ComponentPortDescription;


/*
 * Base class and common definitions for interface CCMObject
 */

class CCMObject : 
  virtual public ::Components::Navigation,
  virtual public ::Components::Receptacles,
  virtual public ::Components::Events
{
  public:
    virtual ~CCMObject();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef CCMObject_ptr _ptr_type;
    typedef CCMObject_var _var_type;
    #endif

    static CCMObject_ptr _narrow( CORBA::Object_ptr obj );
    static CCMObject_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static CCMObject_ptr _duplicate( CCMObject_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static CCMObject_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr get_component_def() = 0;
    virtual CCMHome_ptr get_ccm_home() = 0;
    virtual void configuration_complete() = 0;
    virtual void remove() = 0;
    virtual ComponentPortDescription* get_all_ports() = 0;

  protected:
    CCMObject() {};
  private:
    CCMObject( const CCMObject& );
    void operator=( const CCMObject& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CCMObject;

// Stub for interface CCMObject
class CCMObject_stub:
  virtual public CCMObject,
  virtual public ::Components::Navigation_stub,
  virtual public ::Components::Receptacles_stub,
  virtual public ::Components::Events_stub
{
  public:
    virtual ~CCMObject_stub();
    CORBA::Object_ptr get_component_def();
    CCMHome_ptr get_ccm_home();
    void configuration_complete();
    void remove();
    ComponentPortDescription* get_all_ports();

  private:
    void operator=( const CCMObject_stub& );
};

#ifndef MICO_CONF_NO_POA

class CCMObject_stub_clp :
  virtual public CCMObject_stub,
  virtual public ::Components::Navigation_stub_clp,
  virtual public ::Components::Receptacles_stub_clp,
  virtual public ::Components::Events_stub_clp
{
  public:
    CCMObject_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~CCMObject_stub_clp ();
    CORBA::Object_ptr get_component_def();
    CCMHome_ptr get_ccm_home();
    void configuration_complete();
    void remove();
    ComponentPortDescription* get_all_ports();

  protected:
    CCMObject_stub_clp ();
  private:
    void operator=( const CCMObject_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct DuplicateKeyValue : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  DuplicateKeyValue();
  ~DuplicateKeyValue();
  DuplicateKeyValue( const DuplicateKeyValue& s );
  DuplicateKeyValue& operator=( const DuplicateKeyValue& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  DuplicateKeyValue *operator->() { return this; }
  DuplicateKeyValue& operator*() { return *this; }
  operator DuplicateKeyValue*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static DuplicateKeyValue *_downcast( CORBA::Exception *ex );
  static const DuplicateKeyValue *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef DuplicateKeyValue DuplicateKeyValue_catch;
#else
typedef ExceptVar< DuplicateKeyValue > DuplicateKeyValue_var;
typedef TVarOut< DuplicateKeyValue > DuplicateKeyValue_out;
typedef DuplicateKeyValue_var DuplicateKeyValue_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_DuplicateKeyValue;

struct UnknownKeyValue : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  UnknownKeyValue();
  ~UnknownKeyValue();
  UnknownKeyValue( const UnknownKeyValue& s );
  UnknownKeyValue& operator=( const UnknownKeyValue& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  UnknownKeyValue *operator->() { return this; }
  UnknownKeyValue& operator*() { return *this; }
  operator UnknownKeyValue*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static UnknownKeyValue *_downcast( CORBA::Exception *ex );
  static const UnknownKeyValue *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef UnknownKeyValue UnknownKeyValue_catch;
#else
typedef ExceptVar< UnknownKeyValue > UnknownKeyValue_var;
typedef TVarOut< UnknownKeyValue > UnknownKeyValue_out;
typedef UnknownKeyValue_var UnknownKeyValue_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_UnknownKeyValue;

struct InvalidKey : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidKey();
  ~InvalidKey();
  InvalidKey( const InvalidKey& s );
  InvalidKey& operator=( const InvalidKey& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidKey *operator->() { return this; }
  InvalidKey& operator*() { return *this; }
  operator InvalidKey*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidKey *_downcast( CORBA::Exception *ex );
  static const InvalidKey *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidKey InvalidKey_catch;
#else
typedef ExceptVar< InvalidKey > InvalidKey_var;
typedef TVarOut< InvalidKey > InvalidKey_out;
typedef InvalidKey_var InvalidKey_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InvalidKey;

typedef IfaceSequenceTmpl< CCMHome_var,CCMHome_ptr> CCMHomes;
typedef TSeqVar< IfaceSequenceTmpl< CCMHome_var,CCMHome_ptr> > CCMHomes_var;
typedef TSeqOut< IfaceSequenceTmpl< CCMHome_var,CCMHome_ptr> > CCMHomes_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CCMHomes;


/*
 * Base class and common definitions for interface KeylessCCMHome
 */

class KeylessCCMHome : 
  virtual public CORBA::Object
{
  public:
    virtual ~KeylessCCMHome();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef KeylessCCMHome_ptr _ptr_type;
    typedef KeylessCCMHome_var _var_type;
    #endif

    static KeylessCCMHome_ptr _narrow( CORBA::Object_ptr obj );
    static KeylessCCMHome_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static KeylessCCMHome_ptr _duplicate( KeylessCCMHome_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static KeylessCCMHome_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CCMObject_ptr create_component() = 0;

  protected:
    KeylessCCMHome() {};
  private:
    KeylessCCMHome( const KeylessCCMHome& );
    void operator=( const KeylessCCMHome& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_KeylessCCMHome;

// Stub for interface KeylessCCMHome
class KeylessCCMHome_stub:
  virtual public KeylessCCMHome
{
  public:
    virtual ~KeylessCCMHome_stub();
    CCMObject_ptr create_component();

  private:
    void operator=( const KeylessCCMHome_stub& );
};

#ifndef MICO_CONF_NO_POA

class KeylessCCMHome_stub_clp :
  virtual public KeylessCCMHome_stub,
  virtual public PortableServer::StubBase
{
  public:
    KeylessCCMHome_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~KeylessCCMHome_stub_clp ();
    CCMObject_ptr create_component();

  protected:
    KeylessCCMHome_stub_clp ();
  private:
    void operator=( const KeylessCCMHome_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct HomeNotFound : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  HomeNotFound();
  ~HomeNotFound();
  HomeNotFound( const HomeNotFound& s );
  HomeNotFound& operator=( const HomeNotFound& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  HomeNotFound *operator->() { return this; }
  HomeNotFound& operator*() { return *this; }
  operator HomeNotFound*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static HomeNotFound *_downcast( CORBA::Exception *ex );
  static const HomeNotFound *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef HomeNotFound HomeNotFound_catch;
#else
typedef ExceptVar< HomeNotFound > HomeNotFound_var;
typedef TVarOut< HomeNotFound > HomeNotFound_out;
typedef HomeNotFound_var HomeNotFound_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_HomeNotFound;


/*
 * Base class and common definitions for interface HomeFinder
 */

class HomeFinder : 
  virtual public CORBA::Object
{
  public:
    virtual ~HomeFinder();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef HomeFinder_ptr _ptr_type;
    typedef HomeFinder_var _var_type;
    #endif

    static HomeFinder_ptr _narrow( CORBA::Object_ptr obj );
    static HomeFinder_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static HomeFinder_ptr _duplicate( HomeFinder_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static HomeFinder_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CCMHome_ptr find_home_by_component_type( const char* comp_repid ) = 0;
    virtual CCMHome_ptr find_home_by_home_type( const char* home_repid ) = 0;
    virtual CCMHome_ptr find_home_by_name( const char* home_name ) = 0;

  protected:
    HomeFinder() {};
  private:
    HomeFinder( const HomeFinder& );
    void operator=( const HomeFinder& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_HomeFinder;

// Stub for interface HomeFinder
class HomeFinder_stub:
  virtual public HomeFinder
{
  public:
    virtual ~HomeFinder_stub();
    CCMHome_ptr find_home_by_component_type( const char* comp_repid );
    CCMHome_ptr find_home_by_home_type( const char* home_repid );
    CCMHome_ptr find_home_by_name( const char* home_name );

  private:
    void operator=( const HomeFinder_stub& );
};

#ifndef MICO_CONF_NO_POA

class HomeFinder_stub_clp :
  virtual public HomeFinder_stub,
  virtual public PortableServer::StubBase
{
  public:
    HomeFinder_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~HomeFinder_stub_clp ();
    CCMHome_ptr find_home_by_component_type( const char* comp_repid );
    CCMHome_ptr find_home_by_home_type( const char* home_repid );
    CCMHome_ptr find_home_by_name( const char* home_name );

  protected:
    HomeFinder_stub_clp ();
  private:
    void operator=( const HomeFinder_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct WrongComponentType : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  WrongComponentType();
  ~WrongComponentType();
  WrongComponentType( const WrongComponentType& s );
  WrongComponentType& operator=( const WrongComponentType& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  WrongComponentType *operator->() { return this; }
  WrongComponentType& operator*() { return *this; }
  operator WrongComponentType*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static WrongComponentType *_downcast( CORBA::Exception *ex );
  static const WrongComponentType *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef WrongComponentType WrongComponentType_catch;
#else
typedef ExceptVar< WrongComponentType > WrongComponentType_var;
typedef TVarOut< WrongComponentType > WrongComponentType_out;
typedef WrongComponentType_var WrongComponentType_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_WrongComponentType;


/*
 * Base class and common definitions for interface Configurator
 */

class Configurator : 
  virtual public CORBA::Object
{
  public:
    virtual ~Configurator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Configurator_ptr _ptr_type;
    typedef Configurator_var _var_type;
    #endif

    static Configurator_ptr _narrow( CORBA::Object_ptr obj );
    static Configurator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Configurator_ptr _duplicate( Configurator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Configurator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void configure( CCMObject_ptr comp ) = 0;

  protected:
    Configurator() {};
  private:
    Configurator( const Configurator& );
    void operator=( const Configurator& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Configurator;

// Stub for interface Configurator
class Configurator_stub:
  virtual public Configurator
{
  public:
    virtual ~Configurator_stub();
    void configure( CCMObject_ptr comp );

  private:
    void operator=( const Configurator_stub& );
};

#ifndef MICO_CONF_NO_POA

class Configurator_stub_clp :
  virtual public Configurator_stub,
  virtual public PortableServer::StubBase
{
  public:
    Configurator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Configurator_stub_clp ();
    void configure( CCMObject_ptr comp );

  protected:
    Configurator_stub_clp ();
  private:
    void operator=( const Configurator_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class ConfigValue;
typedef ConfigValue *ConfigValue_ptr;
typedef ConfigValue_ptr ConfigValueRef;
typedef ValueVar< ConfigValue > ConfigValue_var;
typedef ValueOut< ConfigValue > ConfigValue_out;


// Common definitions for valuetype ConfigValue
class ConfigValue : 
  virtual public CORBA::ValueBase
{
  public:
    static ConfigValue* _downcast (CORBA::ValueBase *);
    static ConfigValue* _downcast (CORBA::AbstractBase *);

    virtual void name( char *_p ) = 0;
    virtual void name( const char *_p ) = 0;
    virtual void name( const CORBA::String_var &_p ) = 0;
    virtual const char *name() const = 0;

    virtual void value( const CORBA::Any& _p ) = 0;
    virtual const CORBA::Any& value() const = 0;
    virtual CORBA::Any& value() = 0;


  public:
    CORBA::ValueBase * _copy_value ();
    CORBA::ValueDef_ptr get_value_def ();
    virtual void * _narrow_helper (const char *);
    void _get_marshal_info (std::vector<std::string> &, CORBA::Boolean &);
    void _marshal_members (CORBA::DataEncoder &);
    CORBA::Boolean _demarshal_members (CORBA::DataDecoder &);

  protected:
    ConfigValue ();
    virtual ~ConfigValue ();
    void _copy_members (const ConfigValue&);

  private:
    ConfigValue (const ConfigValue &);
    void operator= (const ConfigValue &);
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ConfigValue;

typedef SequenceTmpl< ConfigValue_var,MICO_TID_DEF> ConfigValues;
typedef TSeqVar< SequenceTmpl< ConfigValue_var,MICO_TID_DEF> > ConfigValues_var;
typedef TSeqOut< SequenceTmpl< ConfigValue_var,MICO_TID_DEF> > ConfigValues_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ConfigValues;


/*
 * Base class and common definitions for interface StandardConfigurator
 */

class StandardConfigurator : 
  virtual public ::Components::Configurator
{
  public:
    virtual ~StandardConfigurator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef StandardConfigurator_ptr _ptr_type;
    typedef StandardConfigurator_var _var_type;
    #endif

    static StandardConfigurator_ptr _narrow( CORBA::Object_ptr obj );
    static StandardConfigurator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static StandardConfigurator_ptr _duplicate( StandardConfigurator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static StandardConfigurator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_configuration( const ConfigValues& descr ) = 0;

  protected:
    StandardConfigurator() {};
  private:
    StandardConfigurator( const StandardConfigurator& );
    void operator=( const StandardConfigurator& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_StandardConfigurator;

// Stub for interface StandardConfigurator
class StandardConfigurator_stub:
  virtual public StandardConfigurator,
  virtual public ::Components::Configurator_stub
{
  public:
    virtual ~StandardConfigurator_stub();
    void set_configuration( const ConfigValues& descr );

  private:
    void operator=( const StandardConfigurator_stub& );
};

#ifndef MICO_CONF_NO_POA

class StandardConfigurator_stub_clp :
  virtual public StandardConfigurator_stub,
  virtual public ::Components::Configurator_stub_clp
{
  public:
    StandardConfigurator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~StandardConfigurator_stub_clp ();
    void set_configuration( const ConfigValues& descr );

  protected:
    StandardConfigurator_stub_clp ();
  private:
    void operator=( const StandardConfigurator_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct IllegalState : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  IllegalState();
  ~IllegalState();
  IllegalState( const IllegalState& s );
  IllegalState& operator=( const IllegalState& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  IllegalState *operator->() { return this; }
  IllegalState& operator*() { return *this; }
  operator IllegalState*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static IllegalState *_downcast( CORBA::Exception *ex );
  static const IllegalState *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef IllegalState IllegalState_catch;
#else
typedef ExceptVar< IllegalState > IllegalState_var;
typedef TVarOut< IllegalState > IllegalState_out;
typedef IllegalState_var IllegalState_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_IllegalState;


/*
 * Base class and common definitions for local interface CCMContext
 */

class CCMContext : 
  virtual public CORBA::Object
{
  public:
    virtual ~CCMContext();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef CCMContext_ptr _ptr_type;
    typedef CCMContext_var _var_type;
    #endif

    static CCMContext_ptr _narrow( CORBA::Object_ptr obj );
    static CCMContext_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static CCMContext_ptr _duplicate( CCMContext_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static CCMContext_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CCMHome_ptr get_CCM_home() = 0;

  protected:
    CCMContext() {};
  private:
    CCMContext( const CCMContext& );
    void operator=( const CCMContext& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CCMContext;



/*
 * Base class and common definitions for local interface SessionContext
 */

class SessionContext : 
  virtual public ::Components::CCMContext
{
  public:
    virtual ~SessionContext();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SessionContext_ptr _ptr_type;
    typedef SessionContext_var _var_type;
    #endif

    static SessionContext_ptr _narrow( CORBA::Object_ptr obj );
    static SessionContext_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SessionContext_ptr _duplicate( SessionContext_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SessionContext_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr get_CCM_object() = 0;

  protected:
    SessionContext() {};
  private:
    SessionContext( const SessionContext& );
    void operator=( const SessionContext& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_SessionContext;


enum CCMExceptionReason {
  SYSTEM_ERROR = 0,
  CREATE_ERROR,
  REMOVE_ERROR,
  DUPLICATE_KEY,
  FIND_ERROR,
  OBJECT_NOT_FOUND,
  NO_SUCH_ENTITY
};

typedef CCMExceptionReason& CCMExceptionReason_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CCMExceptionReason;

struct CCMException : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CCMException();
  ~CCMException();
  CCMException( const CCMException& s );
  CCMException& operator=( const CCMException& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  CCMException();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  CCMException( ::Components::CCMExceptionReason _m0 );

  #ifdef HAVE_STD_EH
  CCMException *operator->() { return this; }
  CCMException& operator*() { return *this; }
  operator CCMException*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static CCMException *_downcast( CORBA::Exception *ex );
  static const CCMException *_downcast( const CORBA::Exception *ex );
  CCMExceptionReason reason;
};

#ifdef HAVE_STD_EH
typedef CCMException CCMException_catch;
#else
typedef ExceptVar< CCMException > CCMException_var;
typedef TVarOut< CCMException > CCMException_out;
typedef CCMException_var CCMException_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CCMException;


/*
 * Base class and common definitions for local interface EnterpriseComponent
 */

class EnterpriseComponent : 
  virtual public CORBA::Object
{
  public:
    virtual ~EnterpriseComponent();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EnterpriseComponent_ptr _ptr_type;
    typedef EnterpriseComponent_var _var_type;
    #endif

    static EnterpriseComponent_ptr _narrow( CORBA::Object_ptr obj );
    static EnterpriseComponent_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EnterpriseComponent_ptr _duplicate( EnterpriseComponent_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EnterpriseComponent_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    EnterpriseComponent() {};
  private:
    EnterpriseComponent( const EnterpriseComponent& );
    void operator=( const EnterpriseComponent& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_EnterpriseComponent;



/*
 * Base class and common definitions for local interface SessionComponent
 */

class SessionComponent : 
  virtual public ::Components::EnterpriseComponent
{
  public:
    virtual ~SessionComponent();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SessionComponent_ptr _ptr_type;
    typedef SessionComponent_var _var_type;
    #endif

    static SessionComponent_ptr _narrow( CORBA::Object_ptr obj );
    static SessionComponent_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SessionComponent_ptr _duplicate( SessionComponent_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SessionComponent_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_session_context( SessionContext_ptr ctx ) = 0;
    virtual void ccm_activate() = 0;
    virtual void ccm_passivate() = 0;
    virtual void ccm_remove() = 0;

  protected:
    SessionComponent() {};
  private:
    SessionComponent( const SessionComponent& );
    void operator=( const SessionComponent& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_SessionComponent;



/*
 * Base class and common definitions for local interface HomeExecutorBase
 */

class HomeExecutorBase : 
  virtual public CORBA::Object
{
  public:
    virtual ~HomeExecutorBase();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef HomeExecutorBase_ptr _ptr_type;
    typedef HomeExecutorBase_var _var_type;
    #endif

    static HomeExecutorBase_ptr _narrow( CORBA::Object_ptr obj );
    static HomeExecutorBase_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static HomeExecutorBase_ptr _duplicate( HomeExecutorBase_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static HomeExecutorBase_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    HomeExecutorBase() {};
  private:
    HomeExecutorBase( const HomeExecutorBase& );
    void operator=( const HomeExecutorBase& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_HomeExecutorBase;



/*
 * Base class and common definitions for local interface ExecutorLocator
 */

class ExecutorLocator : 
  virtual public ::Components::EnterpriseComponent
{
  public:
    virtual ~ExecutorLocator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ExecutorLocator_ptr _ptr_type;
    typedef ExecutorLocator_var _var_type;
    #endif

    static ExecutorLocator_ptr _narrow( CORBA::Object_ptr obj );
    static ExecutorLocator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ExecutorLocator_ptr _duplicate( ExecutorLocator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ExecutorLocator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr obtain_executor( const char* name ) = 0;
    virtual void release_executor( CORBA::Object_ptr exc ) = 0;
    virtual void configurationComplete() = 0;

  protected:
    ExecutorLocator() {};
  private:
    ExecutorLocator( const ExecutorLocator& );
    void operator=( const ExecutorLocator& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ExecutorLocator;


struct UnknownImplId : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  UnknownImplId();
  ~UnknownImplId();
  UnknownImplId( const UnknownImplId& s );
  UnknownImplId& operator=( const UnknownImplId& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  UnknownImplId *operator->() { return this; }
  UnknownImplId& operator*() { return *this; }
  operator UnknownImplId*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static UnknownImplId *_downcast( CORBA::Exception *ex );
  static const UnknownImplId *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef UnknownImplId UnknownImplId_catch;
#else
typedef ExceptVar< UnknownImplId > UnknownImplId_var;
typedef TVarOut< UnknownImplId > UnknownImplId_out;
typedef UnknownImplId_var UnknownImplId_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_UnknownImplId;

struct InvalidLocation : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidLocation();
  ~InvalidLocation();
  InvalidLocation( const InvalidLocation& s );
  InvalidLocation& operator=( const InvalidLocation& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidLocation *operator->() { return this; }
  InvalidLocation& operator*() { return *this; }
  operator InvalidLocation*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidLocation *_downcast( CORBA::Exception *ex );
  static const InvalidLocation *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidLocation InvalidLocation_catch;
#else
typedef ExceptVar< InvalidLocation > InvalidLocation_var;
typedef TVarOut< InvalidLocation > InvalidLocation_out;
typedef InvalidLocation_var InvalidLocation_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InvalidLocation;

struct CreateFailure : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CreateFailure();
  ~CreateFailure();
  CreateFailure( const CreateFailure& s );
  CreateFailure& operator=( const CreateFailure& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  CreateFailure *operator->() { return this; }
  CreateFailure& operator*() { return *this; }
  operator CreateFailure*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static CreateFailure *_downcast( CORBA::Exception *ex );
  static const CreateFailure *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef CreateFailure CreateFailure_catch;
#else
typedef ExceptVar< CreateFailure > CreateFailure_var;
typedef TVarOut< CreateFailure > CreateFailure_out;
typedef CreateFailure_var CreateFailure_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_CreateFailure;

struct RemoveFailure : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  RemoveFailure();
  ~RemoveFailure();
  RemoveFailure( const RemoveFailure& s );
  RemoveFailure& operator=( const RemoveFailure& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  RemoveFailure *operator->() { return this; }
  RemoveFailure& operator*() { return *this; }
  operator RemoveFailure*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static RemoveFailure *_downcast( CORBA::Exception *ex );
  static const RemoveFailure *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef RemoveFailure RemoveFailure_catch;
#else
typedef ExceptVar< RemoveFailure > RemoveFailure_var;
typedef TVarOut< RemoveFailure > RemoveFailure_out;
typedef RemoveFailure_var RemoveFailure_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_RemoveFailure;

struct InstallationFailure : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InstallationFailure();
  ~InstallationFailure();
  InstallationFailure( const InstallationFailure& s );
  InstallationFailure& operator=( const InstallationFailure& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InstallationFailure *operator->() { return this; }
  InstallationFailure& operator*() { return *this; }
  operator InstallationFailure*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InstallationFailure *_downcast( CORBA::Exception *ex );
  static const InstallationFailure *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InstallationFailure InstallationFailure_catch;
#else
typedef ExceptVar< InstallationFailure > InstallationFailure_var;
typedef TVarOut< InstallationFailure > InstallationFailure_out;
typedef InstallationFailure_var InstallationFailure_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InstallationFailure;

struct InvalidAssembly : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  InvalidAssembly();
  ~InvalidAssembly();
  InvalidAssembly( const InvalidAssembly& s );
  InvalidAssembly& operator=( const InvalidAssembly& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS


  #ifdef HAVE_STD_EH
  InvalidAssembly *operator->() { return this; }
  InvalidAssembly& operator*() { return *this; }
  operator InvalidAssembly*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static InvalidAssembly *_downcast( CORBA::Exception *ex );
  static const InvalidAssembly *_downcast( const CORBA::Exception *ex );
};

#ifdef HAVE_STD_EH
typedef InvalidAssembly InvalidAssembly_catch;
#else
typedef ExceptVar< InvalidAssembly > InvalidAssembly_var;
typedef TVarOut< InvalidAssembly > InvalidAssembly_out;
typedef InvalidAssembly_var InvalidAssembly_catch;
#endif // HAVE_STD_EH

extern CCM_EXPORT CORBA::TypeCodeConst _tc_InvalidAssembly;


/*
 * Base class and common definitions for interface Container
 */

class Container : 
  virtual public CORBA::Object
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

    virtual ::Components::ConfigValues* configuration() = 0;

    virtual ComponentServer_ptr get_component_server() = 0;
    virtual CCMHome_ptr install_home( const char* id, const char* entrypt, const ConfigValues& config ) = 0;
    virtual void remove_home( CCMHome_ptr href ) = 0;
    virtual CCMHomes* get_homes() = 0;
    virtual void remove() = 0;

  protected:
    Container() {};
  private:
    Container( const Container& );
    void operator=( const Container& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Container;

// Stub for interface Container
class Container_stub:
  virtual public Container
{
  public:
    virtual ~Container_stub();
    ::Components::ConfigValues* configuration();

    ComponentServer_ptr get_component_server();
    CCMHome_ptr install_home( const char* id, const char* entrypt, const ConfigValues& config );
    void remove_home( CCMHome_ptr href );
    CCMHomes* get_homes();
    void remove();

  private:
    void operator=( const Container_stub& );
};

#ifndef MICO_CONF_NO_POA

class Container_stub_clp :
  virtual public Container_stub,
  virtual public PortableServer::StubBase
{
  public:
    Container_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Container_stub_clp ();
    ::Components::ConfigValues* configuration();

    ComponentServer_ptr get_component_server();
    CCMHome_ptr install_home( const char* id, const char* entrypt, const ConfigValues& config );
    void remove_home( CCMHome_ptr href );
    CCMHomes* get_homes();
    void remove();

  protected:
    Container_stub_clp ();
  private:
    void operator=( const Container_stub_clp & );
};

#endif // MICO_CONF_NO_POA

typedef IfaceSequenceTmpl< Container_var,Container_ptr> Containers;
typedef TSeqVar< IfaceSequenceTmpl< Container_var,Container_ptr> > Containers_var;
typedef TSeqOut< IfaceSequenceTmpl< Container_var,Container_ptr> > Containers_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Containers;


/*
 * Base class and common definitions for interface ComponentServer
 */

class ComponentServer : 
  virtual public CORBA::Object
{
  public:
    virtual ~ComponentServer();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ComponentServer_ptr _ptr_type;
    typedef ComponentServer_var _var_type;
    #endif

    static ComponentServer_ptr _narrow( CORBA::Object_ptr obj );
    static ComponentServer_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ComponentServer_ptr _duplicate( ComponentServer_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ComponentServer_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::Components::ConfigValues* configuration() = 0;

    virtual ServerActivator_ptr get_server_activator() = 0;
    virtual Container_ptr create_container( const ConfigValues& config ) = 0;
    virtual void remove_container( Container_ptr cref ) = 0;
    virtual Containers* get_containers() = 0;
    virtual void remove() = 0;

  protected:
    ComponentServer() {};
  private:
    ComponentServer( const ComponentServer& );
    void operator=( const ComponentServer& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ComponentServer;

// Stub for interface ComponentServer
class ComponentServer_stub:
  virtual public ComponentServer
{
  public:
    virtual ~ComponentServer_stub();
    ::Components::ConfigValues* configuration();

    ServerActivator_ptr get_server_activator();
    Container_ptr create_container( const ConfigValues& config );
    void remove_container( Container_ptr cref );
    Containers* get_containers();
    void remove();

  private:
    void operator=( const ComponentServer_stub& );
};

#ifndef MICO_CONF_NO_POA

class ComponentServer_stub_clp :
  virtual public ComponentServer_stub,
  virtual public PortableServer::StubBase
{
  public:
    ComponentServer_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ComponentServer_stub_clp ();
    ::Components::ConfigValues* configuration();

    ServerActivator_ptr get_server_activator();
    Container_ptr create_container( const ConfigValues& config );
    void remove_container( Container_ptr cref );
    Containers* get_containers();
    void remove();

  protected:
    ComponentServer_stub_clp ();
  private:
    void operator=( const ComponentServer_stub_clp & );
};

#endif // MICO_CONF_NO_POA

typedef IfaceSequenceTmpl< ComponentServer_var,ComponentServer_ptr> ComponentServers;
typedef TSeqVar< IfaceSequenceTmpl< ComponentServer_var,ComponentServer_ptr> > ComponentServers_var;
typedef TSeqOut< IfaceSequenceTmpl< ComponentServer_var,ComponentServer_ptr> > ComponentServers_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ComponentServers;

typedef char* UUID;
typedef CORBA::String_var UUID_var;
typedef CORBA::String_out UUID_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_UUID;

typedef char* Location;
typedef CORBA::String_var Location_var;
typedef CORBA::String_out Location_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Location;


/*
 * Base class and common definitions for interface ComponentInstallation
 */

class ComponentInstallation : 
  virtual public CORBA::Object
{
  public:
    virtual ~ComponentInstallation();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ComponentInstallation_ptr _ptr_type;
    typedef ComponentInstallation_var _var_type;
    #endif

    static ComponentInstallation_ptr _narrow( CORBA::Object_ptr obj );
    static ComponentInstallation_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ComponentInstallation_ptr _duplicate( ComponentInstallation_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ComponentInstallation_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void install( const char* implUUID, const char* component_loc ) = 0;
    virtual void replace( const char* implUUID, const char* component_loc ) = 0;
    virtual void remove( const char* implUUID ) = 0;
    virtual char* get_implementation( const char* implUUID ) = 0;

  protected:
    ComponentInstallation() {};
  private:
    ComponentInstallation( const ComponentInstallation& );
    void operator=( const ComponentInstallation& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ComponentInstallation;

// Stub for interface ComponentInstallation
class ComponentInstallation_stub:
  virtual public ComponentInstallation
{
  public:
    virtual ~ComponentInstallation_stub();
    void install( const char* implUUID, const char* component_loc );
    void replace( const char* implUUID, const char* component_loc );
    void remove( const char* implUUID );
    char* get_implementation( const char* implUUID );

  private:
    void operator=( const ComponentInstallation_stub& );
};

#ifndef MICO_CONF_NO_POA

class ComponentInstallation_stub_clp :
  virtual public ComponentInstallation_stub,
  virtual public PortableServer::StubBase
{
  public:
    ComponentInstallation_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ComponentInstallation_stub_clp ();
    void install( const char* implUUID, const char* component_loc );
    void replace( const char* implUUID, const char* component_loc );
    void remove( const char* implUUID );
    char* get_implementation( const char* implUUID );

  protected:
    ComponentInstallation_stub_clp ();
  private:
    void operator=( const ComponentInstallation_stub_clp & );
};

#endif // MICO_CONF_NO_POA

enum AssemblyState {
  INACTIVE = 0,
  INSERVICE
};

typedef AssemblyState& AssemblyState_out;

extern CCM_EXPORT CORBA::TypeCodeConst _tc_AssemblyState;


/*
 * Base class and common definitions for interface Assembly
 */

class Assembly : 
  virtual public CORBA::Object
{
  public:
    virtual ~Assembly();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Assembly_ptr _ptr_type;
    typedef Assembly_var _var_type;
    #endif

    static Assembly_ptr _narrow( CORBA::Object_ptr obj );
    static Assembly_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Assembly_ptr _duplicate( Assembly_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Assembly_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void build() = 0;
    virtual void tear_down() = 0;
    virtual AssemblyState get_state() = 0;

  protected:
    Assembly() {};
  private:
    Assembly( const Assembly& );
    void operator=( const Assembly& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_Assembly;

// Stub for interface Assembly
class Assembly_stub:
  virtual public Assembly
{
  public:
    virtual ~Assembly_stub();
    void build();
    void tear_down();
    AssemblyState get_state();

  private:
    void operator=( const Assembly_stub& );
};

#ifndef MICO_CONF_NO_POA

class Assembly_stub_clp :
  virtual public Assembly_stub,
  virtual public PortableServer::StubBase
{
  public:
    Assembly_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Assembly_stub_clp ();
    void build();
    void tear_down();
    AssemblyState get_state();

  protected:
    Assembly_stub_clp ();
  private:
    void operator=( const Assembly_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface AssemblyFactory
 */

class AssemblyFactory : 
  virtual public CORBA::Object
{
  public:
    virtual ~AssemblyFactory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef AssemblyFactory_ptr _ptr_type;
    typedef AssemblyFactory_var _var_type;
    #endif

    static AssemblyFactory_ptr _narrow( CORBA::Object_ptr obj );
    static AssemblyFactory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static AssemblyFactory_ptr _duplicate( AssemblyFactory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static AssemblyFactory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual Cookie* create( const char* assembly_loc ) = 0;
    virtual Assembly_ptr lookup( Cookie* c ) = 0;
    virtual void destroy( Cookie* c ) = 0;

  protected:
    AssemblyFactory() {};
  private:
    AssemblyFactory( const AssemblyFactory& );
    void operator=( const AssemblyFactory& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_AssemblyFactory;

// Stub for interface AssemblyFactory
class AssemblyFactory_stub:
  virtual public AssemblyFactory
{
  public:
    virtual ~AssemblyFactory_stub();
    Cookie* create( const char* assembly_loc );
    Assembly_ptr lookup( Cookie* c );
    void destroy( Cookie* c );

  private:
    void operator=( const AssemblyFactory_stub& );
};

#ifndef MICO_CONF_NO_POA

class AssemblyFactory_stub_clp :
  virtual public AssemblyFactory_stub,
  virtual public PortableServer::StubBase
{
  public:
    AssemblyFactory_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~AssemblyFactory_stub_clp ();
    Cookie* create( const char* assembly_loc );
    Assembly_ptr lookup( Cookie* c );
    void destroy( Cookie* c );

  protected:
    AssemblyFactory_stub_clp ();
  private:
    void operator=( const AssemblyFactory_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ServerActivator
 */

class ServerActivator : 
  virtual public CORBA::Object
{
  public:
    virtual ~ServerActivator();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ServerActivator_ptr _ptr_type;
    typedef ServerActivator_var _var_type;
    #endif

    static ServerActivator_ptr _narrow( CORBA::Object_ptr obj );
    static ServerActivator_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ServerActivator_ptr _duplicate( ServerActivator_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ServerActivator_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ComponentServer_ptr create_component_server( const ConfigValues& config ) = 0;
    virtual void remove_component_server( ComponentServer_ptr server ) = 0;
    virtual ComponentServers* get_component_servers() = 0;

  protected:
    ServerActivator() {};
  private:
    ServerActivator( const ServerActivator& );
    void operator=( const ServerActivator& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ServerActivator;

// Stub for interface ServerActivator
class ServerActivator_stub:
  virtual public ServerActivator
{
  public:
    virtual ~ServerActivator_stub();
    ComponentServer_ptr create_component_server( const ConfigValues& config );
    void remove_component_server( ComponentServer_ptr server );
    ComponentServers* get_component_servers();

  private:
    void operator=( const ServerActivator_stub& );
};

#ifndef MICO_CONF_NO_POA

class ServerActivator_stub_clp :
  virtual public ServerActivator_stub,
  virtual public PortableServer::StubBase
{
  public:
    ServerActivator_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ServerActivator_stub_clp ();
    ComponentServer_ptr create_component_server( const ConfigValues& config );
    void remove_component_server( ComponentServer_ptr server );
    ComponentServers* get_component_servers();

  protected:
    ServerActivator_stub_clp ();
  private:
    void operator=( const ServerActivator_stub_clp & );
};

#endif // MICO_CONF_NO_POA

}


namespace MICOCCM
{


/*
 * Base class and common definitions for interface MicoCCMD
 */

class MicoCCMD : 
  virtual public ::Components::ServerActivator,
  virtual public ::Components::ComponentInstallation,
  virtual public ::Components::AssemblyFactory
{
  public:
    virtual ~MicoCCMD();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef MicoCCMD_ptr _ptr_type;
    typedef MicoCCMD_var _var_type;
    #endif

    static MicoCCMD_ptr _narrow( CORBA::Object_ptr obj );
    static MicoCCMD_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static MicoCCMD_ptr _duplicate( MicoCCMD_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static MicoCCMD_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual CORBA::Object_ptr exec( const char* prog, const ::Components::NameList& args, const char* iorfile ) = 0;
    virtual void callback( const char* token, CORBA::Object_ptr csref ) = 0;

  protected:
    MicoCCMD() {};
  private:
    MicoCCMD( const MicoCCMD& );
    void operator=( const MicoCCMD& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_MicoCCMD;

// Stub for interface MicoCCMD
class MicoCCMD_stub:
  virtual public MicoCCMD,
  virtual public ::Components::ServerActivator_stub,
  virtual public ::Components::ComponentInstallation_stub,
  virtual public ::Components::AssemblyFactory_stub
{
  public:
    virtual ~MicoCCMD_stub();
    CORBA::Object_ptr exec( const char* prog, const ::Components::NameList& args, const char* iorfile );
    void callback( const char* token, CORBA::Object_ptr csref );

  private:
    void operator=( const MicoCCMD_stub& );
};

#ifndef MICO_CONF_NO_POA

class MicoCCMD_stub_clp :
  virtual public MicoCCMD_stub,
  virtual public ::Components::ServerActivator_stub_clp,
  virtual public ::Components::ComponentInstallation_stub_clp,
  virtual public ::Components::AssemblyFactory_stub_clp
{
  public:
    MicoCCMD_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~MicoCCMD_stub_clp ();
    CORBA::Object_ptr exec( const char* prog, const ::Components::NameList& args, const char* iorfile );
    void callback( const char* token, CORBA::Object_ptr csref );

  protected:
    MicoCCMD_stub_clp ();
  private:
    void operator=( const MicoCCMD_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ComponentServer
 */

class ComponentServer : 
  virtual public ::Components::ComponentServer
{
  public:
    virtual ~ComponentServer();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ComponentServer_ptr _ptr_type;
    typedef ComponentServer_var _var_type;
    #endif

    static ComponentServer_ptr _narrow( CORBA::Object_ptr obj );
    static ComponentServer_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ComponentServer_ptr _duplicate( ComponentServer_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ComponentServer_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual void set_config_values( const ::Components::ConfigValues& config ) = 0;

  protected:
    ComponentServer() {};
  private:
    ComponentServer( const ComponentServer& );
    void operator=( const ComponentServer& );
};

extern CCM_EXPORT CORBA::TypeCodeConst _tc_ComponentServer;

// Stub for interface ComponentServer
class ComponentServer_stub:
  virtual public ComponentServer,
  virtual public ::Components::ComponentServer_stub
{
  public:
    virtual ~ComponentServer_stub();
    void set_config_values( const ::Components::ConfigValues& config );

  private:
    void operator=( const ComponentServer_stub& );
};

#ifndef MICO_CONF_NO_POA

class ComponentServer_stub_clp :
  virtual public ComponentServer_stub,
  virtual public ::Components::ComponentServer_stub_clp
{
  public:
    ComponentServer_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ComponentServer_stub_clp ();
    void set_config_values( const ::Components::ConfigValues& config );

  protected:
    ComponentServer_stub_clp ();
  private:
    void operator=( const ComponentServer_stub_clp & );
};

#endif // MICO_CONF_NO_POA

}


#ifndef MICO_CONF_NO_POA



namespace POA_Components
{

class Navigation : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Navigation ();
    Components::Navigation_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Navigation * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Object_ptr provide_facet( const char* name ) = 0;
    virtual ::Components::FacetDescriptions* get_all_facets() = 0;
    virtual ::Components::FacetDescriptions* get_named_facets( const ::Components::NameList& names ) = 0;
    virtual CORBA::Boolean same_component( CORBA::Object_ptr ref ) = 0;

  protected:
    Navigation () {};

  private:
    Navigation (const Navigation &);
    void operator= (const Navigation &);
};

class Receptacles : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Receptacles ();
    Components::Receptacles_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Receptacles * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::Cookie* connect( const char* name, CORBA::Object_ptr connection ) = 0;
    virtual void disconnect( const char* name, ::Components::Cookie* ck ) = 0;
    virtual ::Components::ConnectionDescriptions* get_connections( const char* name ) = 0;
    virtual ::Components::ReceptacleDescriptions* get_all_receptacles() = 0;
    virtual ::Components::ReceptacleDescriptions* get_named_receptacles( const ::Components::NameList& names ) = 0;

  protected:
    Receptacles () {};

  private:
    Receptacles (const Receptacles &);
    void operator= (const Receptacles &);
};

class EventConsumerBase : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~EventConsumerBase ();
    Components::EventConsumerBase_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static EventConsumerBase * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void push_event( ::Components::EventBase* evt ) = 0;

  protected:
    EventConsumerBase () {};

  private:
    EventConsumerBase (const EventConsumerBase &);
    void operator= (const EventConsumerBase &);
};

class Events : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Events ();
    Components::Events_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Events * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::EventConsumerBase_ptr get_consumer( const char* sink_name ) = 0;
    virtual ::Components::ConsumerDescriptions* get_all_consumers() = 0;
    virtual ::Components::ConsumerDescriptions* get_named_consumers( const ::Components::NameList& names ) = 0;
    virtual ::Components::Cookie* subscribe( const char* publisher_name, ::Components::EventConsumerBase_ptr subscriber ) = 0;
    virtual void unsubscribe( const char* publisher_name, ::Components::Cookie* ck ) = 0;
    virtual ::Components::PublisherDescriptions* get_all_publishers() = 0;
    virtual ::Components::PublisherDescriptions* get_named_publishers( const ::Components::NameList& names ) = 0;
    virtual void connect_consumer( const char* emitter_name, ::Components::EventConsumerBase_ptr consumer ) = 0;
    virtual ::Components::EventConsumerBase_ptr disconnect_consumer( const char* source_name ) = 0;
    virtual ::Components::EmitterDescriptions* get_all_emitters() = 0;
    virtual ::Components::EmitterDescriptions* get_named_emitters( const ::Components::NameList& names ) = 0;

  protected:
    Events () {};

  private:
    Events (const Events &);
    void operator= (const Events &);
};

class CCMHome : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~CCMHome ();
    Components::CCMHome_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static CCMHome * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Object_ptr get_component_def() = 0;
    virtual CORBA::Object_ptr get_home_def() = 0;
    virtual void remove_component( ::Components::CCMObject_ptr comp ) = 0;

  protected:
    CCMHome () {};

  private:
    CCMHome (const CCMHome &);
    void operator= (const CCMHome &);
};

class CCMObject : 
  virtual public POA_Components::Navigation,
  virtual public POA_Components::Receptacles,
  virtual public POA_Components::Events
{
  public:
    virtual ~CCMObject ();
    Components::CCMObject_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static CCMObject * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Object_ptr get_component_def() = 0;
    virtual ::Components::CCMHome_ptr get_ccm_home() = 0;
    virtual void configuration_complete() = 0;
    virtual void remove() = 0;
    virtual ::Components::ComponentPortDescription* get_all_ports() = 0;

  protected:
    CCMObject () {};

  private:
    CCMObject (const CCMObject &);
    void operator= (const CCMObject &);
};

class KeylessCCMHome : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~KeylessCCMHome ();
    Components::KeylessCCMHome_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static KeylessCCMHome * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::CCMObject_ptr create_component() = 0;

  protected:
    KeylessCCMHome () {};

  private:
    KeylessCCMHome (const KeylessCCMHome &);
    void operator= (const KeylessCCMHome &);
};

class HomeFinder : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~HomeFinder ();
    Components::HomeFinder_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static HomeFinder * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::CCMHome_ptr find_home_by_component_type( const char* comp_repid ) = 0;
    virtual ::Components::CCMHome_ptr find_home_by_home_type( const char* home_repid ) = 0;
    virtual ::Components::CCMHome_ptr find_home_by_name( const char* home_name ) = 0;

  protected:
    HomeFinder () {};

  private:
    HomeFinder (const HomeFinder &);
    void operator= (const HomeFinder &);
};

class Configurator : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Configurator ();
    Components::Configurator_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Configurator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void configure( ::Components::CCMObject_ptr comp ) = 0;

  protected:
    Configurator () {};

  private:
    Configurator (const Configurator &);
    void operator= (const Configurator &);
};

class StandardConfigurator : 
  virtual public POA_Components::Configurator
{
  public:
    virtual ~StandardConfigurator ();
    Components::StandardConfigurator_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static StandardConfigurator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void set_configuration( const ::Components::ConfigValues& descr ) = 0;

  protected:
    StandardConfigurator () {};

  private:
    StandardConfigurator (const StandardConfigurator &);
    void operator= (const StandardConfigurator &);
};

class Container : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Container ();
    Components::Container_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Container * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::ConfigValues* configuration() = 0;

    virtual ::Components::ComponentServer_ptr get_component_server() = 0;
    virtual ::Components::CCMHome_ptr install_home( const char* id, const char* entrypt, const ::Components::ConfigValues& config ) = 0;
    virtual void remove_home( ::Components::CCMHome_ptr href ) = 0;
    virtual ::Components::CCMHomes* get_homes() = 0;
    virtual void remove() = 0;

  protected:
    Container () {};

  private:
    Container (const Container &);
    void operator= (const Container &);
};

class ComponentServer : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ComponentServer ();
    Components::ComponentServer_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ComponentServer * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::ConfigValues* configuration() = 0;

    virtual ::Components::ServerActivator_ptr get_server_activator() = 0;
    virtual ::Components::Container_ptr create_container( const ::Components::ConfigValues& config ) = 0;
    virtual void remove_container( ::Components::Container_ptr cref ) = 0;
    virtual ::Components::Containers* get_containers() = 0;
    virtual void remove() = 0;

  protected:
    ComponentServer () {};

  private:
    ComponentServer (const ComponentServer &);
    void operator= (const ComponentServer &);
};

class ComponentInstallation : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ComponentInstallation ();
    Components::ComponentInstallation_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ComponentInstallation * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void install( const char* implUUID, const char* component_loc ) = 0;
    virtual void replace( const char* implUUID, const char* component_loc ) = 0;
    virtual void remove( const char* implUUID ) = 0;
    virtual char* get_implementation( const char* implUUID ) = 0;

  protected:
    ComponentInstallation () {};

  private:
    ComponentInstallation (const ComponentInstallation &);
    void operator= (const ComponentInstallation &);
};

class Assembly : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Assembly ();
    Components::Assembly_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Assembly * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void build() = 0;
    virtual void tear_down() = 0;
    virtual ::Components::AssemblyState get_state() = 0;

  protected:
    Assembly () {};

  private:
    Assembly (const Assembly &);
    void operator= (const Assembly &);
};

class AssemblyFactory : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~AssemblyFactory ();
    Components::AssemblyFactory_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static AssemblyFactory * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::Cookie* create( const char* assembly_loc ) = 0;
    virtual ::Components::Assembly_ptr lookup( ::Components::Cookie* c ) = 0;
    virtual void destroy( ::Components::Cookie* c ) = 0;

  protected:
    AssemblyFactory () {};

  private:
    AssemblyFactory (const AssemblyFactory &);
    void operator= (const AssemblyFactory &);
};

class ServerActivator : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ServerActivator ();
    Components::ServerActivator_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ServerActivator * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::Components::ComponentServer_ptr create_component_server( const ::Components::ConfigValues& config ) = 0;
    virtual void remove_component_server( ::Components::ComponentServer_ptr server ) = 0;
    virtual ::Components::ComponentServers* get_component_servers() = 0;

  protected:
    ServerActivator () {};

  private:
    ServerActivator (const ServerActivator &);
    void operator= (const ServerActivator &);
};

}


namespace POA_MICOCCM
{

class MicoCCMD : 
  virtual public POA_Components::ServerActivator,
  virtual public POA_Components::ComponentInstallation,
  virtual public POA_Components::AssemblyFactory
{
  public:
    virtual ~MicoCCMD ();
    MICOCCM::MicoCCMD_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static MicoCCMD * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Object_ptr exec( const char* prog, const ::Components::NameList& args, const char* iorfile ) = 0;
    virtual void callback( const char* token, CORBA::Object_ptr csref ) = 0;

  protected:
    MicoCCMD () {};

  private:
    MicoCCMD (const MicoCCMD &);
    void operator= (const MicoCCMD &);
};

class ComponentServer : 
  virtual public POA_Components::ComponentServer
{
  public:
    virtual ~ComponentServer ();
    MICOCCM::ComponentServer_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ComponentServer * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void set_config_values( const ::Components::ConfigValues& config ) = 0;

  protected:
    ComponentServer () {};

  private:
    ComponentServer (const ComponentServer &);
    void operator= (const ComponentServer &);
};

}


#endif // MICO_CONF_NO_POA



namespace OBV_Components
{


// OBV class for valuetype PortDescription
class PortDescription : virtual public Components::PortDescription
{
  public:
    PortDescription ();
    PortDescription (const char* _name, const char* _type_id);
    virtual ~PortDescription();

  public:
    void name( char *_p );
    void name( const char *_p );
    void name( const CORBA::String_var &_p );
    const char *name() const;

    void type_id( char *_p );
    void type_id( const char *_p );
    void type_id( const CORBA::String_var &_p );
    const char *type_id() const;


  private:
    struct _M {
      ::Components::FeatureName_var name;
      CORBA::String_var type_id;
    } _m;
};


// OBV class for valuetype Cookie
class Cookie : virtual public Components::Cookie
{
  public:
    Cookie ();
    Cookie (const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET>& _CookieValue);
    virtual ~Cookie();

  public:
    typedef SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> _CookieValue_seq;
    void CookieValue( const _CookieValue_seq & _p);
    const _CookieValue_seq & CookieValue() const;
    _CookieValue_seq & CookieValue();


  private:
    struct _M {
      SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> CookieValue;
    } _m;
};


// OBV class for valuetype FacetDescription
class FacetDescription :
  virtual public Components::FacetDescription,
  virtual public OBV_Components::PortDescription
{
  public:
    FacetDescription ();
    FacetDescription (const char* _name, const char* _type_id, CORBA::Object_ptr _facet_ref);
    virtual ~FacetDescription();

  public:
    void facet_ref( CORBA::Object_ptr _p );
    CORBA::Object_ptr facet_ref() const;


  private:
    struct _M {
      CORBA::Object_var facet_ref;
    } _m;
};


// OBV class for valuetype ConnectionDescription
class ConnectionDescription : virtual public Components::ConnectionDescription
{
  public:
    ConnectionDescription ();
    ConnectionDescription (::Components::Cookie* _ck, CORBA::Object_ptr _objref);
    virtual ~ConnectionDescription();

  public:
    void ck( ::Components::Cookie* _p );
    ::Components::Cookie* ck() const;

    void objref( CORBA::Object_ptr _p );
    CORBA::Object_ptr objref() const;


  private:
    struct _M {
      ::Components::Cookie_var ck;
      CORBA::Object_var objref;
    } _m;
};


// OBV class for valuetype ReceptacleDescription
class ReceptacleDescription :
  virtual public Components::ReceptacleDescription,
  virtual public OBV_Components::PortDescription
{
  public:
    ReceptacleDescription ();
    ReceptacleDescription (const char* _name, const char* _type_id, CORBA::Boolean _is_multiplex, const ::Components::ConnectionDescriptions& _connections);
    virtual ~ReceptacleDescription();

  public:
    void is_multiplex( CORBA::Boolean _p );
    CORBA::Boolean is_multiplex() const;

    void connections( const ::Components::ConnectionDescriptions& _p );
    const ::Components::ConnectionDescriptions& connections() const;
    ::Components::ConnectionDescriptions& connections();


  private:
    struct _M {
      CORBA::Boolean is_multiplex;
      ::Components::ConnectionDescriptions connections;
    } _m;
};


// OBV class for valuetype ConsumerDescription
class ConsumerDescription :
  virtual public Components::ConsumerDescription,
  virtual public OBV_Components::PortDescription
{
  public:
    ConsumerDescription ();
    ConsumerDescription (const char* _name, const char* _type_id, ::Components::EventConsumerBase_ptr _consumer);
    virtual ~ConsumerDescription();

  public:
    void consumer( ::Components::EventConsumerBase_ptr _p );
    ::Components::EventConsumerBase_ptr consumer() const;


  private:
    struct _M {
      ::Components::EventConsumerBase_var consumer;
    } _m;
};


// OBV class for valuetype EmitterDescription
class EmitterDescription :
  virtual public Components::EmitterDescription,
  virtual public OBV_Components::PortDescription
{
  public:
    EmitterDescription ();
    EmitterDescription (const char* _name, const char* _type_id, ::Components::EventConsumerBase_ptr _consumer);
    virtual ~EmitterDescription();

  public:
    void consumer( ::Components::EventConsumerBase_ptr _p );
    ::Components::EventConsumerBase_ptr consumer() const;


  private:
    struct _M {
      ::Components::EventConsumerBase_var consumer;
    } _m;
};


// OBV class for valuetype SubscriberDescription
class SubscriberDescription : virtual public Components::SubscriberDescription
{
  public:
    SubscriberDescription ();
    SubscriberDescription (::Components::Cookie* _ck, ::Components::EventConsumerBase_ptr _consumer);
    virtual ~SubscriberDescription();

  public:
    void ck( ::Components::Cookie* _p );
    ::Components::Cookie* ck() const;

    void consumer( ::Components::EventConsumerBase_ptr _p );
    ::Components::EventConsumerBase_ptr consumer() const;


  private:
    struct _M {
      ::Components::Cookie_var ck;
      ::Components::EventConsumerBase_var consumer;
    } _m;
};


// OBV class for valuetype PublisherDescription
class PublisherDescription :
  virtual public Components::PublisherDescription,
  virtual public OBV_Components::PortDescription
{
  public:
    PublisherDescription ();
    PublisherDescription (const char* _name, const char* _type_id, const ::Components::SubscriberDescriptions& _consumers);
    virtual ~PublisherDescription();

  public:
    void consumers( const ::Components::SubscriberDescriptions& _p );
    const ::Components::SubscriberDescriptions& consumers() const;
    ::Components::SubscriberDescriptions& consumers();


  private:
    struct _M {
      ::Components::SubscriberDescriptions consumers;
    } _m;
};


// OBV class for valuetype ComponentPortDescription
class ComponentPortDescription : virtual public Components::ComponentPortDescription
{
  public:
    ComponentPortDescription ();
    ComponentPortDescription (const ::Components::FacetDescriptions& _facets, const ::Components::ReceptacleDescriptions& _receptacles, const ::Components::ConsumerDescriptions& _consumers, const ::Components::EmitterDescriptions& _emitters, const ::Components::PublisherDescriptions& _publishers);
    virtual ~ComponentPortDescription();

  public:
    void facets( const ::Components::FacetDescriptions& _p );
    const ::Components::FacetDescriptions& facets() const;
    ::Components::FacetDescriptions& facets();

    void receptacles( const ::Components::ReceptacleDescriptions& _p );
    const ::Components::ReceptacleDescriptions& receptacles() const;
    ::Components::ReceptacleDescriptions& receptacles();

    void consumers( const ::Components::ConsumerDescriptions& _p );
    const ::Components::ConsumerDescriptions& consumers() const;
    ::Components::ConsumerDescriptions& consumers();

    void emitters( const ::Components::EmitterDescriptions& _p );
    const ::Components::EmitterDescriptions& emitters() const;
    ::Components::EmitterDescriptions& emitters();

    void publishers( const ::Components::PublisherDescriptions& _p );
    const ::Components::PublisherDescriptions& publishers() const;
    ::Components::PublisherDescriptions& publishers();


  private:
    struct _M {
      ::Components::FacetDescriptions facets;
      ::Components::ReceptacleDescriptions receptacles;
      ::Components::ConsumerDescriptions consumers;
      ::Components::EmitterDescriptions emitters;
      ::Components::PublisherDescriptions publishers;
    } _m;
};


// OBV class for valuetype ConfigValue
class ConfigValue : virtual public Components::ConfigValue
{
  public:
    ConfigValue ();
    ConfigValue (const char* _name, const CORBA::Any& _value);
    virtual ~ConfigValue();

  public:
    void name( char *_p );
    void name( const char *_p );
    void name( const CORBA::String_var &_p );
    const char *name() const;

    void value( const CORBA::Any& _p );
    const CORBA::Any& value() const;
    CORBA::Any& value();


  private:
    struct _M {
      ::Components::FeatureName_var name;
      CORBA::Any value;
    } _m;
};

}


void operator<<=( CORBA::Any &a, const ::Components::InvalidName &e );
void operator<<=( CORBA::Any &a, ::Components::InvalidName *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InvalidName &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InvalidName *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InvalidName;

void operator<<=( CORBA::Any &a, const ::Components::InvalidConfiguration &e );
void operator<<=( CORBA::Any &a, ::Components::InvalidConfiguration *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InvalidConfiguration &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InvalidConfiguration *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InvalidConfiguration;

void operator<<=( CORBA::Any &a, const ::Components::AlreadyConnected &e );
void operator<<=( CORBA::Any &a, ::Components::AlreadyConnected *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::AlreadyConnected &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::AlreadyConnected *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_AlreadyConnected;

void operator<<=( CORBA::Any &a, const ::Components::InvalidConnection &e );
void operator<<=( CORBA::Any &a, ::Components::InvalidConnection *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InvalidConnection &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InvalidConnection *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InvalidConnection;

void operator<<=( CORBA::Any &a, const ::Components::NoConnection &e );
void operator<<=( CORBA::Any &a, ::Components::NoConnection *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::NoConnection &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::NoConnection *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_NoConnection;

void operator<<=( CORBA::Any &a, const ::Components::ExceededConnectionLimit &e );
void operator<<=( CORBA::Any &a, ::Components::ExceededConnectionLimit *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::ExceededConnectionLimit &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::ExceededConnectionLimit *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ExceededConnectionLimit;

void operator<<=( CORBA::Any &a, const ::Components::CookieRequired &e );
void operator<<=( CORBA::Any &a, ::Components::CookieRequired *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::CookieRequired &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::CookieRequired *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_CookieRequired;

void operator<<=( CORBA::Any &a, const Components::PortDescription* val );
void operator<<=( CORBA::Any &a, Components::PortDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::PortDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_PortDescription;

void operator<<=( CORBA::Any &a, const Components::Cookie* val );
void operator<<=( CORBA::Any &a, Components::Cookie** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Cookie* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Cookie;

void operator<<=( CORBA::Any &a, const Components::EventBase* val );
void operator<<=( CORBA::Any &a, Components::EventBase** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::EventBase* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_EventBase;

void operator<<=( CORBA::Any &a, const Components::FacetDescription* val );
void operator<<=( CORBA::Any &a, Components::FacetDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::FacetDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_FacetDescription;

void operator<<=( CORBA::Any &a, const Components::Navigation_ptr obj );
void operator<<=( CORBA::Any &a, Components::Navigation_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Navigation_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Navigation;

void operator<<=( CORBA::Any &a, const Components::ConnectionDescription* val );
void operator<<=( CORBA::Any &a, Components::ConnectionDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ConnectionDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ConnectionDescription;

void operator<<=( CORBA::Any &a, const Components::ReceptacleDescription* val );
void operator<<=( CORBA::Any &a, Components::ReceptacleDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ReceptacleDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ReceptacleDescription;

void operator<<=( CORBA::Any &a, const Components::Receptacles_ptr obj );
void operator<<=( CORBA::Any &a, Components::Receptacles_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Receptacles_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Receptacles;

void operator<<=( CORBA::Any &a, const ::Components::BadEventType &e );
void operator<<=( CORBA::Any &a, ::Components::BadEventType *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::BadEventType &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::BadEventType *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_BadEventType;

void operator<<=( CORBA::Any &a, const Components::EventConsumerBase_ptr obj );
void operator<<=( CORBA::Any &a, Components::EventConsumerBase_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::EventConsumerBase_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_EventConsumerBase;

void operator<<=( CORBA::Any &a, const Components::ConsumerDescription* val );
void operator<<=( CORBA::Any &a, Components::ConsumerDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ConsumerDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ConsumerDescription;

void operator<<=( CORBA::Any &a, const Components::EmitterDescription* val );
void operator<<=( CORBA::Any &a, Components::EmitterDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::EmitterDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_EmitterDescription;

void operator<<=( CORBA::Any &a, const Components::SubscriberDescription* val );
void operator<<=( CORBA::Any &a, Components::SubscriberDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::SubscriberDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_SubscriberDescription;

void operator<<=( CORBA::Any &a, const Components::PublisherDescription* val );
void operator<<=( CORBA::Any &a, Components::PublisherDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::PublisherDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_PublisherDescription;

void operator<<=( CORBA::Any &a, const Components::Events_ptr obj );
void operator<<=( CORBA::Any &a, Components::Events_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Events_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Events;

void operator<<=( CORBA::Any &a, const Components::CCMHome_ptr obj );
void operator<<=( CORBA::Any &a, Components::CCMHome_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::CCMHome_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_CCMHome;

void operator<<=( CORBA::Any &a, const Components::ComponentPortDescription* val );
void operator<<=( CORBA::Any &a, Components::ComponentPortDescription** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ComponentPortDescription* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ComponentPortDescription;

void operator<<=( CORBA::Any &a, const Components::CCMObject_ptr obj );
void operator<<=( CORBA::Any &a, Components::CCMObject_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::CCMObject_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_CCMObject;

void operator<<=( CORBA::Any &a, const ::Components::DuplicateKeyValue &e );
void operator<<=( CORBA::Any &a, ::Components::DuplicateKeyValue *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::DuplicateKeyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::DuplicateKeyValue *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_DuplicateKeyValue;

void operator<<=( CORBA::Any &a, const ::Components::UnknownKeyValue &e );
void operator<<=( CORBA::Any &a, ::Components::UnknownKeyValue *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::UnknownKeyValue &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::UnknownKeyValue *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_UnknownKeyValue;

void operator<<=( CORBA::Any &a, const ::Components::InvalidKey &e );
void operator<<=( CORBA::Any &a, ::Components::InvalidKey *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InvalidKey &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InvalidKey *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InvalidKey;

void operator<<=( CORBA::Any &a, const Components::KeylessCCMHome_ptr obj );
void operator<<=( CORBA::Any &a, Components::KeylessCCMHome_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::KeylessCCMHome_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_KeylessCCMHome;

void operator<<=( CORBA::Any &a, const ::Components::HomeNotFound &e );
void operator<<=( CORBA::Any &a, ::Components::HomeNotFound *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::HomeNotFound &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::HomeNotFound *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_HomeNotFound;

void operator<<=( CORBA::Any &a, const Components::HomeFinder_ptr obj );
void operator<<=( CORBA::Any &a, Components::HomeFinder_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::HomeFinder_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_HomeFinder;

void operator<<=( CORBA::Any &a, const ::Components::WrongComponentType &e );
void operator<<=( CORBA::Any &a, ::Components::WrongComponentType *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::WrongComponentType &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::WrongComponentType *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_WrongComponentType;

void operator<<=( CORBA::Any &a, const Components::Configurator_ptr obj );
void operator<<=( CORBA::Any &a, Components::Configurator_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Configurator_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Configurator;

void operator<<=( CORBA::Any &a, const Components::ConfigValue* val );
void operator<<=( CORBA::Any &a, Components::ConfigValue** val_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ConfigValue* & val_ptr );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ConfigValue;

void operator<<=( CORBA::Any &a, const Components::StandardConfigurator_ptr obj );
void operator<<=( CORBA::Any &a, Components::StandardConfigurator_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::StandardConfigurator_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_StandardConfigurator;

void operator<<=( CORBA::Any &a, const ::Components::IllegalState &e );
void operator<<=( CORBA::Any &a, ::Components::IllegalState *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::IllegalState &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::IllegalState *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_IllegalState;

void operator<<=( CORBA::Any &a, const ::Components::CCMExceptionReason &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::CCMExceptionReason &e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_CCMExceptionReason;

void operator<<=( CORBA::Any &a, const ::Components::CCMException &e );
void operator<<=( CORBA::Any &a, ::Components::CCMException *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::CCMException &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::CCMException *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_CCMException;

void operator<<=( CORBA::Any &a, const ::Components::UnknownImplId &e );
void operator<<=( CORBA::Any &a, ::Components::UnknownImplId *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::UnknownImplId &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::UnknownImplId *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_UnknownImplId;

void operator<<=( CORBA::Any &a, const ::Components::InvalidLocation &e );
void operator<<=( CORBA::Any &a, ::Components::InvalidLocation *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InvalidLocation &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InvalidLocation *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InvalidLocation;

void operator<<=( CORBA::Any &a, const ::Components::CreateFailure &e );
void operator<<=( CORBA::Any &a, ::Components::CreateFailure *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::CreateFailure &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::CreateFailure *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_CreateFailure;

void operator<<=( CORBA::Any &a, const ::Components::RemoveFailure &e );
void operator<<=( CORBA::Any &a, ::Components::RemoveFailure *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::RemoveFailure &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::RemoveFailure *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_RemoveFailure;

void operator<<=( CORBA::Any &a, const ::Components::InstallationFailure &e );
void operator<<=( CORBA::Any &a, ::Components::InstallationFailure *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InstallationFailure &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InstallationFailure *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InstallationFailure;

void operator<<=( CORBA::Any &a, const ::Components::InvalidAssembly &e );
void operator<<=( CORBA::Any &a, ::Components::InvalidAssembly *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::InvalidAssembly &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::Components::InvalidAssembly *&e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_InvalidAssembly;

void operator<<=( CORBA::Any &a, const Components::Container_ptr obj );
void operator<<=( CORBA::Any &a, Components::Container_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Container_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Container;

void operator<<=( CORBA::Any &a, const Components::ComponentServer_ptr obj );
void operator<<=( CORBA::Any &a, Components::ComponentServer_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ComponentServer_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ComponentServer;

void operator<<=( CORBA::Any &a, const Components::ComponentInstallation_ptr obj );
void operator<<=( CORBA::Any &a, Components::ComponentInstallation_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ComponentInstallation_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ComponentInstallation;

void operator<<=( CORBA::Any &a, const ::Components::AssemblyState &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::Components::AssemblyState &e );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_AssemblyState;

void operator<<=( CORBA::Any &a, const Components::Assembly_ptr obj );
void operator<<=( CORBA::Any &a, Components::Assembly_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::Assembly_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_Assembly;

void operator<<=( CORBA::Any &a, const Components::AssemblyFactory_ptr obj );
void operator<<=( CORBA::Any &a, Components::AssemblyFactory_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::AssemblyFactory_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_AssemblyFactory;

void operator<<=( CORBA::Any &a, const Components::ServerActivator_ptr obj );
void operator<<=( CORBA::Any &a, Components::ServerActivator_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, Components::ServerActivator_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_Components_ServerActivator;

void operator<<=( CORBA::Any &a, const MICOCCM::MicoCCMD_ptr obj );
void operator<<=( CORBA::Any &a, MICOCCM::MicoCCMD_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, MICOCCM::MicoCCMD_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_MICOCCM_MicoCCMD;

void operator<<=( CORBA::Any &a, const MICOCCM::ComponentServer_ptr obj );
void operator<<=( CORBA::Any &a, MICOCCM::ComponentServer_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, MICOCCM::ComponentServer_ptr &obj );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller_MICOCCM_ComponentServer;

void operator<<=( CORBA::Any &_a, const StringSequenceTmpl<CORBA::String_var> &_s );
void operator<<=( CORBA::Any &_a, StringSequenceTmpl<CORBA::String_var> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, StringSequenceTmpl<CORBA::String_var> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const StringSequenceTmpl<CORBA::String_var> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Octet,MICO_TID_OCTET> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::FacetDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_FacetDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ConnectionDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_ConnectionDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ReceptacleDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_ReceptacleDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ConsumerDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_ConsumerDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::EmitterDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_EmitterDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::SubscriberDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_SubscriberDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::PublisherDescription_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_PublisherDescription;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< Components::CCMHome_var,Components::CCMHome_ptr> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_CCMHome;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< Components::ConfigValue_var,MICO_TID_DEF> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_ConfigValue;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< Components::Container_var,Components::Container_ptr> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_Container;

void operator<<=( CORBA::Any &_a, const IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> &_s );
void operator<<=( CORBA::Any &_a, IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const IfaceSequenceTmpl< Components::ComponentServer_var,Components::ComponentServer_ptr> *&_s );

extern CCM_EXPORT CORBA::StaticTypeInfo *_marshaller__seq_Components_ComponentServer;

#endif
