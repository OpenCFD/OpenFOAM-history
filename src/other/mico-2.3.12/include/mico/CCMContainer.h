/* -*- c++ -*- */

/*
 *  MICO --- a free CORBA implementation
 *
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <CORBA.h>
#include <mico/CCM.h>

#ifndef __MICO_CCM_CONTAINER_H__
#define __MICO_CCM_CONTAINER_H__

namespace MICO {

  namespace CCM {

    /*
     * Base for all Containers
     */

    class ContainerBase {
    protected:
      static CORBA::ULong _container_number;
      CORBA::ULong _my_number;

    public:
      ContainerBase ();
      virtual ~ContainerBase ();
      virtual void activate () = 0;
      virtual void passivate () = 0;
      virtual void remove () = 0;

      virtual CORBA::Boolean
      compare (Components::CCMHome_ptr) = 0;

      /*
       * for SessionContext
       */

      virtual Components::CCMHome_ptr
      get_CCM_home () = 0;

      virtual CORBA::Object_ptr
      get_CCM_object (CORBA::Object_ptr) = 0;
    };

    /*
     * Service Container
     */

    class ServiceContainer : virtual public ContainerBase {
    private:
      CORBA::ORB_var _orb;
      PortableServer::POA_var _my_poa;

    public:
      struct ComponentInfo {
	std::string home_short_name;
	std::string home_absolute_name;
	std::string home_id;
	std::string component_short_name;
	std::string component_absolute_name;
	std::string component_id;
	CORBA::Object_var home_instance;
	CORBA::Object_var comp_instance;
	PortableServer::ServantBase_var home_glue;
	PortableServer::ServantBase_var comp_glue;
      };

    private:
      bool _have_info;
      ComponentInfo _info;
      CORBA::Object_var _home_ref;
      CORBA::Object_var _comp_ref;

      CORBA::Boolean _configuration_complete;
      std::map<std::string, PortableServer::ServantBase_var, std::less<std::string> > _facet_glue;
      std::map<std::string, CORBA::Object_var, std::less<std::string> > _facet_instance;
      std::map<std::string, CORBA::Object_var, std::less<std::string> > _facet_reference;

    public:
      ServiceContainer (CORBA::ORB_ptr orb);
      ~ServiceContainer ();

      void load (const ComponentInfo & info);
      void activate ();
      void passivate ();
      void remove ();

      virtual CORBA::Boolean
      compare (Components::CCMHome_ptr);

      /*
       * Service Container API
       */

      Components::CCMHome_ptr
      get_reference_for_home ();

      Components::CCMObject_ptr
      get_reference_for_component ();

      /*
       * for SessionContext
       */

      Components::CCMHome_ptr
      get_CCM_home ();

      CORBA::Object_ptr
      get_CCM_object (CORBA::Object_ptr);

      /*
       * Facet management
       */

      CORBA::Object_ptr
      activate_facet (PortableServer::Servant comp_glue,
		      const char * name,
		      CORBA::Object_ptr facet_instance,
		      PortableServer::Servant facet_glue);

      /*
       * notify container of configuration_complete
       */

      void configuration_complete (PortableServer::Servant comp_glue);
    };

    /*
     * Session Container
     */

    class SessionContainer : virtual public ContainerBase {
    private:
      CORBA::ORB_var _orb;
      PortableServer::POA_var _my_poa;

    public:
      struct ComponentInfo {
	std::string home_short_name;
	std::string home_absolute_name;
	std::string home_id;
	std::string component_short_name;
	std::string component_absolute_name;
	std::string component_id;
	CORBA::Object_var home_instance;
	PortableServer::ServantBase_var home_glue;
      };

    private:
      ComponentInfo _info;
      bool _have_info;

      struct oidless : std::binary_function<const PortableServer::ObjectId &,
		                       const PortableServer::ObjectId &,
		                       bool> {
	bool operator() (const PortableServer::ObjectId & i1,
			 const PortableServer::ObjectId & i2) const
	  {
	    if (i1.length() != i2.length()) {
	      return (i1.length() < i2.length());
	    }
	    return (memcmp (i1.get_buffer(), i2.get_buffer(),
			    i1.length()) < 0);
	  }
      };

      struct PerComponentData {
	CORBA::Boolean configuration_complete;
	PortableServer::ServantBase_var glue;
	CORBA::Object_var instance;
	CORBA::Object_var reference;
	std::map<std::string, PortableServer::ServantBase_var, std::less<std::string> > facet_glue;
	std::map<std::string, CORBA::Object_var, std::less<std::string> > facet_instance;
	std::map<std::string, CORBA::Object_var, std::less<std::string> > facet_reference;
      };

      CORBA::Object_var _home_ref;
      typedef std::map<PortableServer::ObjectId,
	PerComponentData, oidless> InstanceMap;
      InstanceMap active_components;

    public:
      SessionContainer (CORBA::ORB_ptr orb);
      ~SessionContainer ();

      void load (const ComponentInfo & info);
      void activate ();
      void passivate ();
      void remove ();

      virtual CORBA::Boolean
      compare (Components::CCMHome_ptr);

      /*
       * Session Container API
       */

      Components::CCMHome_ptr
      get_reference_for_home ();

      Components::CCMObject_ptr
      activate_component (CORBA::Object_ptr instance,
			  PortableServer::Servant skel);

      Components::CCMObject_ptr
      get_reference_for_component (PortableServer::Servant skel);

      PortableServer::Servant
      get_skeleton_for_reference (CORBA::Object_ptr ref);

      Components::CCMObject_ptr
      get_reference_for_instance (CORBA::Object_ptr);

      CORBA::Object_ptr
      get_instance_for_component (PortableServer::Servant skel);

      void
      deactivate_component (CORBA::Object_ptr ref);

      void
      deactivate_component (PortableServer::Servant skel);

      /*
       * for SessionContext
       */

      Components::CCMHome_ptr
      get_CCM_home ();

      CORBA::Object_ptr
      get_CCM_object (CORBA::Object_ptr);

      /*
       * Facet management
       */

      CORBA::Object_ptr
      activate_facet (PortableServer::Servant comp_glue,
		      const char * name,
		      CORBA::Object_ptr facet_instance,
		      PortableServer::Servant facet_glue);

      /*
       * notify container of configuration_complete
       */

      void configuration_complete (PortableServer::Servant comp_glue);
    };

    /*
     * Valuetype implementations
     */

    class PortDescription_impl :
      virtual public OBV_Components::PortDescription,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      PortDescription_impl ();
    };

    class Cookie_impl :
      virtual public OBV_Components::Cookie,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      Cookie_impl ();
    };

    class FacetDescription_impl :
      virtual public OBV_Components::FacetDescription,
      virtual public PortDescription_impl,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      FacetDescription_impl ();
    };

    class ConnectionDescription_impl :
      virtual public OBV_Components::ConnectionDescription,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      ConnectionDescription_impl ();
    };

    class ReceptacleDescription_impl :
      virtual public OBV_Components::ReceptacleDescription,
      virtual public PortDescription_impl,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      ReceptacleDescription_impl ();
    };

    class ConsumerDescription_impl :
      virtual public OBV_Components::ConsumerDescription,
      virtual public PortDescription_impl,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      ConsumerDescription_impl ();
    };

    class EmitterDescription_impl :
      virtual public OBV_Components::EmitterDescription,
      virtual public PortDescription_impl,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      EmitterDescription_impl ();
    };

    class SubscriberDescription_impl :
      virtual public OBV_Components::SubscriberDescription,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      SubscriberDescription_impl ();
    };

    class PublisherDescription_impl :
      virtual public OBV_Components::PublisherDescription,
      virtual public PortDescription_impl,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      PublisherDescription_impl ();
    };

    class ComponentPortDescription_impl :
      virtual public OBV_Components::ComponentPortDescription,
      virtual public CORBA::DefaultValueRefCountBase
    {
    public:
      ComponentPortDescription_impl ();
    };

    /*
     * Valuetype factories
     */

    class PortDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class Cookie_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class FacetDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class ConnectionDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class ReceptacleDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class ConsumerDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class EmitterDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class SubscriberDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class PublisherDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    class ComponentPortDescription_Factory :
      virtual public CORBA::ValueFactoryBase
    {
    public:
      CORBA::ValueBase * create_for_unmarshal ();
    };

    void register_all_factories (CORBA::ORB_ptr);
  }
}

#endif
