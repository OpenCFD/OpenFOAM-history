/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>

#ifndef __IR3_H__
#define __IR3_H__






namespace CORBA
{
namespace ComponentIR
{

class ComponentDef;
typedef ComponentDef *ComponentDef_ptr;
typedef ComponentDef_ptr ComponentDefRef;
typedef ObjVar< ComponentDef > ComponentDef_var;
typedef ObjOut< ComponentDef > ComponentDef_out;

class ProvidesDef;
typedef ProvidesDef *ProvidesDef_ptr;
typedef ProvidesDef_ptr ProvidesDefRef;
typedef ObjVar< ProvidesDef > ProvidesDef_var;
typedef ObjOut< ProvidesDef > ProvidesDef_out;

class UsesDef;
typedef UsesDef *UsesDef_ptr;
typedef UsesDef_ptr UsesDefRef;
typedef ObjVar< UsesDef > UsesDef_var;
typedef ObjOut< UsesDef > UsesDef_out;

class EmitsDef;
typedef EmitsDef *EmitsDef_ptr;
typedef EmitsDef_ptr EmitsDefRef;
typedef ObjVar< EmitsDef > EmitsDef_var;
typedef ObjOut< EmitsDef > EmitsDef_out;

class EventDef;
typedef EventDef *EventDef_ptr;
typedef EventDef_ptr EventDefRef;
typedef ObjVar< EventDef > EventDef_var;
typedef ObjOut< EventDef > EventDef_out;

class PublishesDef;
typedef PublishesDef *PublishesDef_ptr;
typedef PublishesDef_ptr PublishesDefRef;
typedef ObjVar< PublishesDef > PublishesDef_var;
typedef ObjOut< PublishesDef > PublishesDef_out;

class ConsumesDef;
typedef ConsumesDef *ConsumesDef_ptr;
typedef ConsumesDef_ptr ConsumesDefRef;
typedef ObjVar< ConsumesDef > ConsumesDef_var;
typedef ObjOut< ConsumesDef > ConsumesDef_out;

class HomeDef;
typedef HomeDef *HomeDef_ptr;
typedef HomeDef_ptr HomeDefRef;
typedef ObjVar< HomeDef > HomeDef_var;
typedef ObjOut< HomeDef > HomeDef_out;

class FactoryDef;
typedef FactoryDef *FactoryDef_ptr;
typedef FactoryDef_ptr FactoryDefRef;
typedef ObjVar< FactoryDef > FactoryDef_var;
typedef ObjOut< FactoryDef > FactoryDef_out;

class FinderDef;
typedef FinderDef *FinderDef_ptr;
typedef FinderDef_ptr FinderDefRef;
typedef ObjVar< FinderDef > FinderDef_var;
typedef ObjOut< FinderDef > FinderDef_out;

class Container;
typedef Container *Container_ptr;
typedef Container_ptr ContainerRef;
typedef ObjVar< Container > Container_var;
typedef ObjOut< Container > Container_out;

class ModuleDef;
typedef ModuleDef *ModuleDef_ptr;
typedef ModuleDef_ptr ModuleDefRef;
typedef ObjVar< ModuleDef > ModuleDef_var;
typedef ObjOut< ModuleDef > ModuleDef_out;

class Repository;
typedef Repository *Repository_ptr;
typedef Repository_ptr RepositoryRef;
typedef ObjVar< Repository > Repository_var;
typedef ObjOut< Repository > Repository_out;

class EventPortDef;
typedef EventPortDef *EventPortDef_ptr;
typedef EventPortDef_ptr EventPortDefRef;
typedef ObjVar< EventPortDef > EventPortDef_var;
typedef ObjOut< EventPortDef > EventPortDef_out;

}
}



#include <mico/ir_base.h>
#include <mico/ir.h>



namespace CORBA
{
namespace ComponentIR
{


/*
 * Base class and common definitions for interface ComponentDef
 */

class ComponentDef : 
  virtual public ::CORBA::ExtInterfaceDef
{
  public:
    virtual ~ComponentDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ComponentDef_ptr _ptr_type;
    typedef ComponentDef_var _var_type;
    #endif

    static ComponentDef_ptr _narrow( CORBA::Object_ptr obj );
    static ComponentDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ComponentDef_ptr _duplicate( ComponentDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ComponentDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::ComponentIR::ComponentDef_ptr base_component() = 0;
    virtual void base_component( ::CORBA::ComponentIR::ComponentDef_ptr value ) = 0;
    virtual ::CORBA::InterfaceDefSeq* supported_interfaces() = 0;
    virtual void supported_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;

    virtual ProvidesDef_ptr create_provides( const char* id, const char* name, const char* version, InterfaceDef_ptr interface_type ) = 0;
    virtual UsesDef_ptr create_uses( const char* id, const char* name, const char* version, InterfaceDef_ptr interface_type, CORBA::Boolean is_multiple ) = 0;
    virtual EmitsDef_ptr create_emits( const char* id, const char* name, const char* version, EventDef_ptr event ) = 0;
    virtual PublishesDef_ptr create_publishes( const char* id, const char* name, const char* version, EventDef_ptr event ) = 0;
    virtual ConsumesDef_ptr create_consumes( const char* id, const char* name, const char* version, EventDef_ptr event ) = 0;

  protected:
    ComponentDef() {};
  private:
    ComponentDef( const ComponentDef& );
    void operator=( const ComponentDef& );
};

extern CORBA::TypeCodeConst _tc_ComponentDef;

// Stub for interface ComponentDef
class ComponentDef_stub:
  virtual public ComponentDef,
  virtual public ::CORBA::ExtInterfaceDef_stub
{
  public:
    virtual ~ComponentDef_stub();
    ::CORBA::ComponentIR::ComponentDef_ptr base_component();
    void base_component( ::CORBA::ComponentIR::ComponentDef_ptr value );
    ::CORBA::InterfaceDefSeq* supported_interfaces();
    void supported_interfaces( const ::CORBA::InterfaceDefSeq& value );

    ProvidesDef_ptr create_provides( const char* id, const char* name, const char* version, InterfaceDef_ptr interface_type );
    UsesDef_ptr create_uses( const char* id, const char* name, const char* version, InterfaceDef_ptr interface_type, CORBA::Boolean is_multiple );
    EmitsDef_ptr create_emits( const char* id, const char* name, const char* version, EventDef_ptr event );
    PublishesDef_ptr create_publishes( const char* id, const char* name, const char* version, EventDef_ptr event );
    ConsumesDef_ptr create_consumes( const char* id, const char* name, const char* version, EventDef_ptr event );

  private:
    void operator=( const ComponentDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ComponentDef_stub_clp :
  virtual public ComponentDef_stub,
  virtual public ::CORBA::ExtInterfaceDef_stub_clp
{
  public:
    ComponentDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ComponentDef_stub_clp ();
    ::CORBA::ComponentIR::ComponentDef_ptr base_component();
    void base_component( ::CORBA::ComponentIR::ComponentDef_ptr value );
    ::CORBA::InterfaceDefSeq* supported_interfaces();
    void supported_interfaces( const ::CORBA::InterfaceDefSeq& value );

    ProvidesDef_ptr create_provides( const char* id, const char* name, const char* version, InterfaceDef_ptr interface_type );
    UsesDef_ptr create_uses( const char* id, const char* name, const char* version, InterfaceDef_ptr interface_type, CORBA::Boolean is_multiple );
    EmitsDef_ptr create_emits( const char* id, const char* name, const char* version, EventDef_ptr event );
    PublishesDef_ptr create_publishes( const char* id, const char* name, const char* version, EventDef_ptr event );
    ConsumesDef_ptr create_consumes( const char* id, const char* name, const char* version, EventDef_ptr event );

  protected:
    ComponentDef_stub_clp ();
  private:
    void operator=( const ComponentDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface HomeDef
 */

class HomeDef : 
  virtual public ::CORBA::ExtInterfaceDef
{
  public:
    virtual ~HomeDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef HomeDef_ptr _ptr_type;
    typedef HomeDef_var _var_type;
    #endif

    static HomeDef_ptr _narrow( CORBA::Object_ptr obj );
    static HomeDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static HomeDef_ptr _duplicate( HomeDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static HomeDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::ComponentIR::HomeDef_ptr base_home() = 0;
    virtual void base_home( ::CORBA::ComponentIR::HomeDef_ptr value ) = 0;
    virtual ::CORBA::InterfaceDefSeq* supported_interfaces() = 0;
    virtual void supported_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;
    virtual ::CORBA::ComponentIR::ComponentDef_ptr managed_component() = 0;
    virtual void managed_component( ::CORBA::ComponentIR::ComponentDef_ptr value ) = 0;
    virtual ::CORBA::ValueDef_ptr primary_key() = 0;
    virtual void primary_key( ::CORBA::ValueDef_ptr value ) = 0;

    virtual FactoryDef_ptr create_factory( const char* id, const char* name, const char* version, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions ) = 0;
    virtual FinderDef_ptr create_finder( const char* id, const char* name, const char* version, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions ) = 0;

  protected:
    HomeDef() {};
  private:
    HomeDef( const HomeDef& );
    void operator=( const HomeDef& );
};

extern CORBA::TypeCodeConst _tc_HomeDef;

// Stub for interface HomeDef
class HomeDef_stub:
  virtual public HomeDef,
  virtual public ::CORBA::ExtInterfaceDef_stub
{
  public:
    virtual ~HomeDef_stub();
    ::CORBA::ComponentIR::HomeDef_ptr base_home();
    void base_home( ::CORBA::ComponentIR::HomeDef_ptr value );
    ::CORBA::InterfaceDefSeq* supported_interfaces();
    void supported_interfaces( const ::CORBA::InterfaceDefSeq& value );
    ::CORBA::ComponentIR::ComponentDef_ptr managed_component();
    void managed_component( ::CORBA::ComponentIR::ComponentDef_ptr value );
    ::CORBA::ValueDef_ptr primary_key();
    void primary_key( ::CORBA::ValueDef_ptr value );

    FactoryDef_ptr create_factory( const char* id, const char* name, const char* version, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions );
    FinderDef_ptr create_finder( const char* id, const char* name, const char* version, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions );

  private:
    void operator=( const HomeDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class HomeDef_stub_clp :
  virtual public HomeDef_stub,
  virtual public ::CORBA::ExtInterfaceDef_stub_clp
{
  public:
    HomeDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~HomeDef_stub_clp ();
    ::CORBA::ComponentIR::HomeDef_ptr base_home();
    void base_home( ::CORBA::ComponentIR::HomeDef_ptr value );
    ::CORBA::InterfaceDefSeq* supported_interfaces();
    void supported_interfaces( const ::CORBA::InterfaceDefSeq& value );
    ::CORBA::ComponentIR::ComponentDef_ptr managed_component();
    void managed_component( ::CORBA::ComponentIR::ComponentDef_ptr value );
    ::CORBA::ValueDef_ptr primary_key();
    void primary_key( ::CORBA::ValueDef_ptr value );

    FactoryDef_ptr create_factory( const char* id, const char* name, const char* version, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions );
    FinderDef_ptr create_finder( const char* id, const char* name, const char* version, const ParDescriptionSeq& params, const ExceptionDefSeq& exceptions );

  protected:
    HomeDef_stub_clp ();
  private:
    void operator=( const HomeDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface EventDef
 */

class EventDef : 
  virtual public ::CORBA::ExtValueDef
{
  public:
    virtual ~EventDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EventDef_ptr _ptr_type;
    typedef EventDef_var _var_type;
    #endif

    static EventDef_ptr _narrow( CORBA::Object_ptr obj );
    static EventDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EventDef_ptr _duplicate( EventDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EventDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    EventDef() {};
  private:
    EventDef( const EventDef& );
    void operator=( const EventDef& );
};

extern CORBA::TypeCodeConst _tc_EventDef;

// Stub for interface EventDef
class EventDef_stub:
  virtual public EventDef,
  virtual public ::CORBA::ExtValueDef_stub
{
  public:
    virtual ~EventDef_stub();
  private:
    void operator=( const EventDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class EventDef_stub_clp :
  virtual public EventDef_stub,
  virtual public ::CORBA::ExtValueDef_stub_clp
{
  public:
    EventDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~EventDef_stub_clp ();
  protected:
    EventDef_stub_clp ();
  private:
    void operator=( const EventDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface Container
 */

class Container : 
  virtual public ::CORBA::Container
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

    virtual ComponentDef_ptr create_component( const char* id, const char* name, const char* version, ComponentDef_ptr base_component, const InterfaceDefSeq& supports_interfaces ) = 0;
    virtual HomeDef_ptr create_home( const char* id, const char* name, const char* version, HomeDef_ptr base_home, ComponentDef_ptr managed_component, const InterfaceDefSeq& supports_interfaces, ValueDef_ptr primary_key ) = 0;
    virtual EventDef_ptr create_event( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const ExtInitializerSeq& initializers ) = 0;

  protected:
    Container() {};
  private:
    Container( const Container& );
    void operator=( const Container& );
};

extern CORBA::TypeCodeConst _tc_Container;

// Stub for interface Container
class Container_stub:
  virtual public Container,
  virtual public ::CORBA::Container_stub
{
  public:
    virtual ~Container_stub();
    ComponentDef_ptr create_component( const char* id, const char* name, const char* version, ComponentDef_ptr base_component, const InterfaceDefSeq& supports_interfaces );
    HomeDef_ptr create_home( const char* id, const char* name, const char* version, HomeDef_ptr base_home, ComponentDef_ptr managed_component, const InterfaceDefSeq& supports_interfaces, ValueDef_ptr primary_key );
    EventDef_ptr create_event( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const ExtInitializerSeq& initializers );

  private:
    void operator=( const Container_stub& );
};

#ifndef MICO_CONF_NO_POA

class Container_stub_clp :
  virtual public Container_stub,
  virtual public ::CORBA::Container_stub_clp
{
  public:
    Container_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Container_stub_clp ();
    ComponentDef_ptr create_component( const char* id, const char* name, const char* version, ComponentDef_ptr base_component, const InterfaceDefSeq& supports_interfaces );
    HomeDef_ptr create_home( const char* id, const char* name, const char* version, HomeDef_ptr base_home, ComponentDef_ptr managed_component, const InterfaceDefSeq& supports_interfaces, ValueDef_ptr primary_key );
    EventDef_ptr create_event( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ValueDefSeq& abstract_base_values, const InterfaceDefSeq& supported_interfaces, const ExtInitializerSeq& initializers );

  protected:
    Container_stub_clp ();
  private:
    void operator=( const Container_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ModuleDef
 */

class ModuleDef : 
  virtual public ::CORBA::ComponentIR::Container,
  virtual public ::CORBA::ModuleDef
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

extern CORBA::TypeCodeConst _tc_ModuleDef;

// Stub for interface ModuleDef
class ModuleDef_stub:
  virtual public ModuleDef,
  virtual public ::CORBA::ComponentIR::Container_stub,
  virtual public ::CORBA::ModuleDef_stub
{
  public:
    virtual ~ModuleDef_stub();
  private:
    void operator=( const ModuleDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ModuleDef_stub_clp :
  virtual public ModuleDef_stub,
  virtual public ::CORBA::ComponentIR::Container_stub_clp,
  virtual public ::CORBA::ModuleDef_stub_clp
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
 * Base class and common definitions for interface Repository
 */

class Repository : 
  virtual public ::CORBA::ComponentIR::Container,
  virtual public ::CORBA::Repository
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

  protected:
    Repository() {};
  private:
    Repository( const Repository& );
    void operator=( const Repository& );
};

extern CORBA::TypeCodeConst _tc_Repository;

// Stub for interface Repository
class Repository_stub:
  virtual public Repository,
  virtual public ::CORBA::ComponentIR::Container_stub,
  virtual public ::CORBA::Repository_stub
{
  public:
    virtual ~Repository_stub();
  private:
    void operator=( const Repository_stub& );
};

#ifndef MICO_CONF_NO_POA

class Repository_stub_clp :
  virtual public Repository_stub,
  virtual public ::CORBA::ComponentIR::Container_stub_clp,
  virtual public ::CORBA::Repository_stub_clp
{
  public:
    Repository_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Repository_stub_clp ();
  protected:
    Repository_stub_clp ();
  private:
    void operator=( const Repository_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ProvidesDef
 */

class ProvidesDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~ProvidesDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ProvidesDef_ptr _ptr_type;
    typedef ProvidesDef_var _var_type;
    #endif

    static ProvidesDef_ptr _narrow( CORBA::Object_ptr obj );
    static ProvidesDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ProvidesDef_ptr _duplicate( ProvidesDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ProvidesDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::InterfaceDef_ptr interface_type() = 0;
    virtual void interface_type( ::CORBA::InterfaceDef_ptr value ) = 0;

  protected:
    ProvidesDef() {};
  private:
    ProvidesDef( const ProvidesDef& );
    void operator=( const ProvidesDef& );
};

extern CORBA::TypeCodeConst _tc_ProvidesDef;

// Stub for interface ProvidesDef
class ProvidesDef_stub:
  virtual public ProvidesDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~ProvidesDef_stub();
    ::CORBA::InterfaceDef_ptr interface_type();
    void interface_type( ::CORBA::InterfaceDef_ptr value );

  private:
    void operator=( const ProvidesDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ProvidesDef_stub_clp :
  virtual public ProvidesDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    ProvidesDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ProvidesDef_stub_clp ();
    ::CORBA::InterfaceDef_ptr interface_type();
    void interface_type( ::CORBA::InterfaceDef_ptr value );

  protected:
    ProvidesDef_stub_clp ();
  private:
    void operator=( const ProvidesDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct ProvidesDescription;
typedef TVarVar< ProvidesDescription > ProvidesDescription_var;
typedef TVarOut< ProvidesDescription > ProvidesDescription_out;


struct ProvidesDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ProvidesDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ProvidesDescription();
  ~ProvidesDescription();
  ProvidesDescription( const ProvidesDescription& s );
  ProvidesDescription& operator=( const ProvidesDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryId_var interface_type;
};

extern CORBA::TypeCodeConst _tc_ProvidesDescription;


/*
 * Base class and common definitions for interface UsesDef
 */

class UsesDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~UsesDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef UsesDef_ptr _ptr_type;
    typedef UsesDef_var _var_type;
    #endif

    static UsesDef_ptr _narrow( CORBA::Object_ptr obj );
    static UsesDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static UsesDef_ptr _duplicate( UsesDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static UsesDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::InterfaceDef_ptr interface_type() = 0;
    virtual void interface_type( ::CORBA::InterfaceDef_ptr value ) = 0;
    virtual CORBA::Boolean is_multiple() = 0;
    virtual void is_multiple( CORBA::Boolean value ) = 0;

  protected:
    UsesDef() {};
  private:
    UsesDef( const UsesDef& );
    void operator=( const UsesDef& );
};

extern CORBA::TypeCodeConst _tc_UsesDef;

// Stub for interface UsesDef
class UsesDef_stub:
  virtual public UsesDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~UsesDef_stub();
    ::CORBA::InterfaceDef_ptr interface_type();
    void interface_type( ::CORBA::InterfaceDef_ptr value );
    CORBA::Boolean is_multiple();
    void is_multiple( CORBA::Boolean value );

  private:
    void operator=( const UsesDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class UsesDef_stub_clp :
  virtual public UsesDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    UsesDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~UsesDef_stub_clp ();
    ::CORBA::InterfaceDef_ptr interface_type();
    void interface_type( ::CORBA::InterfaceDef_ptr value );
    CORBA::Boolean is_multiple();
    void is_multiple( CORBA::Boolean value );

  protected:
    UsesDef_stub_clp ();
  private:
    void operator=( const UsesDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct UsesDescription;
typedef TVarVar< UsesDescription > UsesDescription_var;
typedef TVarOut< UsesDescription > UsesDescription_out;


struct UsesDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef UsesDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  UsesDescription();
  ~UsesDescription();
  UsesDescription( const UsesDescription& s );
  UsesDescription& operator=( const UsesDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryId_var interface_type;
  CORBA::Boolean is_multiple;
};

extern CORBA::TypeCodeConst _tc_UsesDescription;


/*
 * Base class and common definitions for interface EventPortDef
 */

class EventPortDef : 
  virtual public ::CORBA::Contained
{
  public:
    virtual ~EventPortDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EventPortDef_ptr _ptr_type;
    typedef EventPortDef_var _var_type;
    #endif

    static EventPortDef_ptr _narrow( CORBA::Object_ptr obj );
    static EventPortDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EventPortDef_ptr _duplicate( EventPortDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EventPortDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual ::CORBA::ComponentIR::EventDef_ptr event() = 0;
    virtual void event( ::CORBA::ComponentIR::EventDef_ptr value ) = 0;

    virtual CORBA::Boolean is_a( const char* event_id ) = 0;

  protected:
    EventPortDef() {};
  private:
    EventPortDef( const EventPortDef& );
    void operator=( const EventPortDef& );
};

extern CORBA::TypeCodeConst _tc_EventPortDef;

// Stub for interface EventPortDef
class EventPortDef_stub:
  virtual public EventPortDef,
  virtual public ::CORBA::Contained_stub
{
  public:
    virtual ~EventPortDef_stub();
    ::CORBA::ComponentIR::EventDef_ptr event();
    void event( ::CORBA::ComponentIR::EventDef_ptr value );

    CORBA::Boolean is_a( const char* event_id );

  private:
    void operator=( const EventPortDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class EventPortDef_stub_clp :
  virtual public EventPortDef_stub,
  virtual public ::CORBA::Contained_stub_clp
{
  public:
    EventPortDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~EventPortDef_stub_clp ();
    ::CORBA::ComponentIR::EventDef_ptr event();
    void event( ::CORBA::ComponentIR::EventDef_ptr value );

    CORBA::Boolean is_a( const char* event_id );

  protected:
    EventPortDef_stub_clp ();
  private:
    void operator=( const EventPortDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct EventPortDescription;
typedef TVarVar< EventPortDescription > EventPortDescription_var;
typedef TVarOut< EventPortDescription > EventPortDescription_out;


struct EventPortDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef EventPortDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  EventPortDescription();
  ~EventPortDescription();
  EventPortDescription( const EventPortDescription& s );
  EventPortDescription& operator=( const EventPortDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryId_var event;
};

extern CORBA::TypeCodeConst _tc_EventPortDescription;


/*
 * Base class and common definitions for interface EmitsDef
 */

class EmitsDef : 
  virtual public ::CORBA::ComponentIR::EventPortDef
{
  public:
    virtual ~EmitsDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef EmitsDef_ptr _ptr_type;
    typedef EmitsDef_var _var_type;
    #endif

    static EmitsDef_ptr _narrow( CORBA::Object_ptr obj );
    static EmitsDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static EmitsDef_ptr _duplicate( EmitsDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static EmitsDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    EmitsDef() {};
  private:
    EmitsDef( const EmitsDef& );
    void operator=( const EmitsDef& );
};

extern CORBA::TypeCodeConst _tc_EmitsDef;

// Stub for interface EmitsDef
class EmitsDef_stub:
  virtual public EmitsDef,
  virtual public ::CORBA::ComponentIR::EventPortDef_stub
{
  public:
    virtual ~EmitsDef_stub();
  private:
    void operator=( const EmitsDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class EmitsDef_stub_clp :
  virtual public EmitsDef_stub,
  virtual public ::CORBA::ComponentIR::EventPortDef_stub_clp
{
  public:
    EmitsDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~EmitsDef_stub_clp ();
  protected:
    EmitsDef_stub_clp ();
  private:
    void operator=( const EmitsDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface PublishesDef
 */

class PublishesDef : 
  virtual public ::CORBA::ComponentIR::EventPortDef
{
  public:
    virtual ~PublishesDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef PublishesDef_ptr _ptr_type;
    typedef PublishesDef_var _var_type;
    #endif

    static PublishesDef_ptr _narrow( CORBA::Object_ptr obj );
    static PublishesDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static PublishesDef_ptr _duplicate( PublishesDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static PublishesDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    PublishesDef() {};
  private:
    PublishesDef( const PublishesDef& );
    void operator=( const PublishesDef& );
};

extern CORBA::TypeCodeConst _tc_PublishesDef;

// Stub for interface PublishesDef
class PublishesDef_stub:
  virtual public PublishesDef,
  virtual public ::CORBA::ComponentIR::EventPortDef_stub
{
  public:
    virtual ~PublishesDef_stub();
  private:
    void operator=( const PublishesDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class PublishesDef_stub_clp :
  virtual public PublishesDef_stub,
  virtual public ::CORBA::ComponentIR::EventPortDef_stub_clp
{
  public:
    PublishesDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~PublishesDef_stub_clp ();
  protected:
    PublishesDef_stub_clp ();
  private:
    void operator=( const PublishesDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface ConsumesDef
 */

class ConsumesDef : 
  virtual public ::CORBA::ComponentIR::EventPortDef
{
  public:
    virtual ~ConsumesDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ConsumesDef_ptr _ptr_type;
    typedef ConsumesDef_var _var_type;
    #endif

    static ConsumesDef_ptr _narrow( CORBA::Object_ptr obj );
    static ConsumesDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ConsumesDef_ptr _duplicate( ConsumesDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ConsumesDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    ConsumesDef() {};
  private:
    ConsumesDef( const ConsumesDef& );
    void operator=( const ConsumesDef& );
};

extern CORBA::TypeCodeConst _tc_ConsumesDef;

// Stub for interface ConsumesDef
class ConsumesDef_stub:
  virtual public ConsumesDef,
  virtual public ::CORBA::ComponentIR::EventPortDef_stub
{
  public:
    virtual ~ConsumesDef_stub();
  private:
    void operator=( const ConsumesDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class ConsumesDef_stub_clp :
  virtual public ConsumesDef_stub,
  virtual public ::CORBA::ComponentIR::EventPortDef_stub_clp
{
  public:
    ConsumesDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ConsumesDef_stub_clp ();
  protected:
    ConsumesDef_stub_clp ();
  private:
    void operator=( const ConsumesDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

typedef SequenceTmpl< ProvidesDescription,MICO_TID_DEF> ProvidesDescriptionSeq;
typedef TSeqVar< SequenceTmpl< ProvidesDescription,MICO_TID_DEF> > ProvidesDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< ProvidesDescription,MICO_TID_DEF> > ProvidesDescriptionSeq_out;

extern CORBA::TypeCodeConst _tc_ProvidesDescriptionSeq;

typedef SequenceTmpl< UsesDescription,MICO_TID_DEF> UsesDescriptionSeq;
typedef TSeqVar< SequenceTmpl< UsesDescription,MICO_TID_DEF> > UsesDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< UsesDescription,MICO_TID_DEF> > UsesDescriptionSeq_out;

extern CORBA::TypeCodeConst _tc_UsesDescriptionSeq;

typedef SequenceTmpl< EventPortDescription,MICO_TID_DEF> EventPortDescriptionSeq;
typedef TSeqVar< SequenceTmpl< EventPortDescription,MICO_TID_DEF> > EventPortDescriptionSeq_var;
typedef TSeqOut< SequenceTmpl< EventPortDescription,MICO_TID_DEF> > EventPortDescriptionSeq_out;

extern CORBA::TypeCodeConst _tc_EventPortDescriptionSeq;

struct ComponentDescription;
typedef TVarVar< ComponentDescription > ComponentDescription_var;
typedef TVarOut< ComponentDescription > ComponentDescription_out;


struct ComponentDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ComponentDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ComponentDescription();
  ~ComponentDescription();
  ComponentDescription( const ComponentDescription& s );
  ComponentDescription& operator=( const ComponentDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryId_var base_component;
  RepositoryIdSeq supported_interfaces;
  ProvidesDescriptionSeq provided_interfaces;
  UsesDescriptionSeq used_interfaces;
  EventPortDescriptionSeq emits_events;
  EventPortDescriptionSeq publishes_events;
  EventPortDescriptionSeq consumes_events;
  ExtAttrDescriptionSeq attributes;
  CORBA::TypeCode_var type;
};

extern CORBA::TypeCodeConst _tc_ComponentDescription;


/*
 * Base class and common definitions for interface FactoryDef
 */

class FactoryDef : 
  virtual public ::CORBA::OperationDef
{
  public:
    virtual ~FactoryDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef FactoryDef_ptr _ptr_type;
    typedef FactoryDef_var _var_type;
    #endif

    static FactoryDef_ptr _narrow( CORBA::Object_ptr obj );
    static FactoryDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static FactoryDef_ptr _duplicate( FactoryDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static FactoryDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    FactoryDef() {};
  private:
    FactoryDef( const FactoryDef& );
    void operator=( const FactoryDef& );
};

extern CORBA::TypeCodeConst _tc_FactoryDef;

// Stub for interface FactoryDef
class FactoryDef_stub:
  virtual public FactoryDef,
  virtual public ::CORBA::OperationDef_stub
{
  public:
    virtual ~FactoryDef_stub();
  private:
    void operator=( const FactoryDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class FactoryDef_stub_clp :
  virtual public FactoryDef_stub,
  virtual public ::CORBA::OperationDef_stub_clp
{
  public:
    FactoryDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~FactoryDef_stub_clp ();
  protected:
    FactoryDef_stub_clp ();
  private:
    void operator=( const FactoryDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA


/*
 * Base class and common definitions for interface FinderDef
 */

class FinderDef : 
  virtual public ::CORBA::OperationDef
{
  public:
    virtual ~FinderDef();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef FinderDef_ptr _ptr_type;
    typedef FinderDef_var _var_type;
    #endif

    static FinderDef_ptr _narrow( CORBA::Object_ptr obj );
    static FinderDef_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static FinderDef_ptr _duplicate( FinderDef_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static FinderDef_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

  protected:
    FinderDef() {};
  private:
    FinderDef( const FinderDef& );
    void operator=( const FinderDef& );
};

extern CORBA::TypeCodeConst _tc_FinderDef;

// Stub for interface FinderDef
class FinderDef_stub:
  virtual public FinderDef,
  virtual public ::CORBA::OperationDef_stub
{
  public:
    virtual ~FinderDef_stub();
  private:
    void operator=( const FinderDef_stub& );
};

#ifndef MICO_CONF_NO_POA

class FinderDef_stub_clp :
  virtual public FinderDef_stub,
  virtual public ::CORBA::OperationDef_stub_clp
{
  public:
    FinderDef_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~FinderDef_stub_clp ();
  protected:
    FinderDef_stub_clp ();
  private:
    void operator=( const FinderDef_stub_clp & );
};

#endif // MICO_CONF_NO_POA

struct HomeDescription;
typedef TVarVar< HomeDescription > HomeDescription_var;
typedef TVarOut< HomeDescription > HomeDescription_out;


struct HomeDescription {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef HomeDescription_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  HomeDescription();
  ~HomeDescription();
  HomeDescription( const HomeDescription& s );
  HomeDescription& operator=( const HomeDescription& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  Identifier_var name;
  RepositoryId_var id;
  RepositoryId_var defined_in;
  VersionSpec_var version;
  RepositoryId_var base_home;
  RepositoryId_var managed_component;
  ValueDescription primary_key;
  OpDescriptionSeq factories;
  OpDescriptionSeq finders;
  OpDescriptionSeq operations;
  ExtAttrDescriptionSeq attributes;
  CORBA::TypeCode_var type;
};

extern CORBA::TypeCodeConst _tc_HomeDescription;

}
}


#ifndef MICO_CONF_NO_POA



namespace POA_CORBA
{
namespace ComponentIR
{

class ComponentDef : 
  virtual public POA_CORBA::ExtInterfaceDef
{
  public:
    virtual ~ComponentDef ();
    CORBA::ComponentIR::ComponentDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ComponentDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ComponentIR::ComponentDef_ptr base_component() = 0;
    virtual void base_component( ::CORBA::ComponentIR::ComponentDef_ptr value ) = 0;
    virtual ::CORBA::InterfaceDefSeq* supported_interfaces() = 0;
    virtual void supported_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;

    virtual ::CORBA::ComponentIR::ProvidesDef_ptr create_provides( const char* id, const char* name, const char* version, ::CORBA::InterfaceDef_ptr interface_type ) = 0;
    virtual ::CORBA::ComponentIR::UsesDef_ptr create_uses( const char* id, const char* name, const char* version, ::CORBA::InterfaceDef_ptr interface_type, CORBA::Boolean is_multiple ) = 0;
    virtual ::CORBA::ComponentIR::EmitsDef_ptr create_emits( const char* id, const char* name, const char* version, ::CORBA::ComponentIR::EventDef_ptr event ) = 0;
    virtual ::CORBA::ComponentIR::PublishesDef_ptr create_publishes( const char* id, const char* name, const char* version, ::CORBA::ComponentIR::EventDef_ptr event ) = 0;
    virtual ::CORBA::ComponentIR::ConsumesDef_ptr create_consumes( const char* id, const char* name, const char* version, ::CORBA::ComponentIR::EventDef_ptr event ) = 0;

  protected:
    ComponentDef () {};

  private:
    ComponentDef (const ComponentDef &);
    void operator= (const ComponentDef &);
};

class HomeDef : 
  virtual public POA_CORBA::ExtInterfaceDef
{
  public:
    virtual ~HomeDef ();
    CORBA::ComponentIR::HomeDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static HomeDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ComponentIR::HomeDef_ptr base_home() = 0;
    virtual void base_home( ::CORBA::ComponentIR::HomeDef_ptr value ) = 0;
    virtual ::CORBA::InterfaceDefSeq* supported_interfaces() = 0;
    virtual void supported_interfaces( const ::CORBA::InterfaceDefSeq& value ) = 0;
    virtual ::CORBA::ComponentIR::ComponentDef_ptr managed_component() = 0;
    virtual void managed_component( ::CORBA::ComponentIR::ComponentDef_ptr value ) = 0;
    virtual ::CORBA::ValueDef_ptr primary_key() = 0;
    virtual void primary_key( ::CORBA::ValueDef_ptr value ) = 0;

    virtual ::CORBA::ComponentIR::FactoryDef_ptr create_factory( const char* id, const char* name, const char* version, const ::CORBA::ParDescriptionSeq& params, const ::CORBA::ExceptionDefSeq& exceptions ) = 0;
    virtual ::CORBA::ComponentIR::FinderDef_ptr create_finder( const char* id, const char* name, const char* version, const ::CORBA::ParDescriptionSeq& params, const ::CORBA::ExceptionDefSeq& exceptions ) = 0;

  protected:
    HomeDef () {};

  private:
    HomeDef (const HomeDef &);
    void operator= (const HomeDef &);
};

class EventDef : 
  virtual public POA_CORBA::ExtValueDef
{
  public:
    virtual ~EventDef ();
    CORBA::ComponentIR::EventDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static EventDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    EventDef () {};

  private:
    EventDef (const EventDef &);
    void operator= (const EventDef &);
};

class Container : 
  virtual public POA_CORBA::Container
{
  public:
    virtual ~Container ();
    CORBA::ComponentIR::Container_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Container * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ComponentIR::ComponentDef_ptr create_component( const char* id, const char* name, const char* version, ::CORBA::ComponentIR::ComponentDef_ptr base_component, const ::CORBA::InterfaceDefSeq& supports_interfaces ) = 0;
    virtual ::CORBA::ComponentIR::HomeDef_ptr create_home( const char* id, const char* name, const char* version, ::CORBA::ComponentIR::HomeDef_ptr base_home, ::CORBA::ComponentIR::ComponentDef_ptr managed_component, const ::CORBA::InterfaceDefSeq& supports_interfaces, ::CORBA::ValueDef_ptr primary_key ) = 0;
    virtual ::CORBA::ComponentIR::EventDef_ptr create_event( const char* id, const char* name, const char* version, CORBA::Boolean is_custom, CORBA::Boolean is_abstract, ::CORBA::ValueDef_ptr base_value, CORBA::Boolean is_truncatable, const ::CORBA::ValueDefSeq& abstract_base_values, const ::CORBA::InterfaceDefSeq& supported_interfaces, const ::CORBA::ExtInitializerSeq& initializers ) = 0;

  protected:
    Container () {};

  private:
    Container (const Container &);
    void operator= (const Container &);
};

class ModuleDef : 
  virtual public POA_CORBA::ComponentIR::Container,
  virtual public POA_CORBA::ModuleDef
{
  public:
    virtual ~ModuleDef ();
    CORBA::ComponentIR::ModuleDef_ptr _this ();
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

class Repository : 
  virtual public POA_CORBA::ComponentIR::Container,
  virtual public POA_CORBA::Repository
{
  public:
    virtual ~Repository ();
    CORBA::ComponentIR::Repository_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Repository * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    Repository () {};

  private:
    Repository (const Repository &);
    void operator= (const Repository &);
};

class ProvidesDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~ProvidesDef ();
    CORBA::ComponentIR::ProvidesDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ProvidesDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::InterfaceDef_ptr interface_type() = 0;
    virtual void interface_type( ::CORBA::InterfaceDef_ptr value ) = 0;

  protected:
    ProvidesDef () {};

  private:
    ProvidesDef (const ProvidesDef &);
    void operator= (const ProvidesDef &);
};

class UsesDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~UsesDef ();
    CORBA::ComponentIR::UsesDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static UsesDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::InterfaceDef_ptr interface_type() = 0;
    virtual void interface_type( ::CORBA::InterfaceDef_ptr value ) = 0;
    virtual CORBA::Boolean is_multiple() = 0;
    virtual void is_multiple( CORBA::Boolean value ) = 0;

  protected:
    UsesDef () {};

  private:
    UsesDef (const UsesDef &);
    void operator= (const UsesDef &);
};

class EventPortDef : 
  virtual public POA_CORBA::Contained
{
  public:
    virtual ~EventPortDef ();
    CORBA::ComponentIR::EventPortDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static EventPortDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual ::CORBA::ComponentIR::EventDef_ptr event() = 0;
    virtual void event( ::CORBA::ComponentIR::EventDef_ptr value ) = 0;

    virtual CORBA::Boolean is_a( const char* event_id ) = 0;

  protected:
    EventPortDef () {};

  private:
    EventPortDef (const EventPortDef &);
    void operator= (const EventPortDef &);
};

class EmitsDef : 
  virtual public POA_CORBA::ComponentIR::EventPortDef
{
  public:
    virtual ~EmitsDef ();
    CORBA::ComponentIR::EmitsDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static EmitsDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    EmitsDef () {};

  private:
    EmitsDef (const EmitsDef &);
    void operator= (const EmitsDef &);
};

class PublishesDef : 
  virtual public POA_CORBA::ComponentIR::EventPortDef
{
  public:
    virtual ~PublishesDef ();
    CORBA::ComponentIR::PublishesDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static PublishesDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    PublishesDef () {};

  private:
    PublishesDef (const PublishesDef &);
    void operator= (const PublishesDef &);
};

class ConsumesDef : 
  virtual public POA_CORBA::ComponentIR::EventPortDef
{
  public:
    virtual ~ConsumesDef ();
    CORBA::ComponentIR::ConsumesDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ConsumesDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    ConsumesDef () {};

  private:
    ConsumesDef (const ConsumesDef &);
    void operator= (const ConsumesDef &);
};

class FactoryDef : 
  virtual public POA_CORBA::OperationDef
{
  public:
    virtual ~FactoryDef ();
    CORBA::ComponentIR::FactoryDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static FactoryDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    FactoryDef () {};

  private:
    FactoryDef (const FactoryDef &);
    void operator= (const FactoryDef &);
};

class FinderDef : 
  virtual public POA_CORBA::OperationDef
{
  public:
    virtual ~FinderDef ();
    CORBA::ComponentIR::FinderDef_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static FinderDef * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    FinderDef () {};

  private:
    FinderDef (const FinderDef &);
    void operator= (const FinderDef &);
};

}
}


#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::ComponentDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::ComponentDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::ComponentDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ComponentDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::HomeDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::HomeDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::HomeDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_HomeDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::EventDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::EventDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::EventDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EventDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::Container_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::Container_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::Container_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_Container;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::ModuleDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::ModuleDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::ModuleDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ModuleDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::Repository_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::Repository_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::Repository_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_Repository;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::ProvidesDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::ProvidesDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::ProvidesDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ProvidesDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ComponentIR::ProvidesDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ComponentIR::ProvidesDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ComponentIR::ProvidesDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ComponentIR::ProvidesDescription *&_s );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ProvidesDescription;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::UsesDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::UsesDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::UsesDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_UsesDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ComponentIR::UsesDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ComponentIR::UsesDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ComponentIR::UsesDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ComponentIR::UsesDescription *&_s );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_UsesDescription;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::EventPortDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::EventPortDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::EventPortDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EventPortDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ComponentIR::EventPortDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ComponentIR::EventPortDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ComponentIR::EventPortDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ComponentIR::EventPortDescription *&_s );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EventPortDescription;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::EmitsDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::EmitsDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::EmitsDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_EmitsDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::PublishesDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::PublishesDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::PublishesDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_PublishesDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::ConsumesDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::ConsumesDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::ConsumesDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ConsumesDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ComponentIR::ComponentDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ComponentIR::ComponentDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ComponentIR::ComponentDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ComponentIR::ComponentDescription *&_s );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_ComponentDescription;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::FactoryDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::FactoryDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::FactoryDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_FactoryDef;

void operator<<=( CORBA::Any &a, const CORBA::ComponentIR::FinderDef_ptr obj );
void operator<<=( CORBA::Any &a, CORBA::ComponentIR::FinderDef_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, CORBA::ComponentIR::FinderDef_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_FinderDef;

void operator<<=( CORBA::Any &_a, const ::CORBA::ComponentIR::HomeDescription &_s );
void operator<<=( CORBA::Any &_a, ::CORBA::ComponentIR::HomeDescription *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::CORBA::ComponentIR::HomeDescription &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::CORBA::ComponentIR::HomeDescription *&_s );

extern CORBA::StaticTypeInfo *_marshaller_CORBA_ComponentIR_HomeDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::ProvidesDescription,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ComponentIR_ProvidesDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::UsesDescription,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ComponentIR_UsesDescription;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::ComponentIR::EventPortDescription,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_CORBA_ComponentIR_EventPortDescription;

#endif
