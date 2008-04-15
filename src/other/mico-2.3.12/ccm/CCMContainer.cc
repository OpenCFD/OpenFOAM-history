/*
 *  Mico --- a free CORBA implementation
 *  Copyright Frank Pilhofer
 *
 *  This file is part of MicoCCM, written by Frank Pilhofer.
 *  The MICO CORBA Component Project was sponsored by Alcatel.
 */

#include <CORBA.h>
#include <mico/CCM.h>
#include <mico/CCMContainer.h>


using namespace std;

/*
 * ----------------------------------------------------------------------
 * MICO Containers
 * ----------------------------------------------------------------------
 *
 * There is one container per home. Multiple containers may be active in
 * a single process. Each container creates a new POA, where it activates
 * the home, all components and all facets.
 */

CORBA::ULong
MICO::CCM::ContainerBase::_container_number = 0;

MICO::CCM::ContainerBase::ContainerBase ()
{
  _my_number = ++_container_number;
}

MICO::CCM::ContainerBase::~ContainerBase ()
{
}

/*
 * ----------------------------------------------------------------------
 * Implementation for the Service Container
 * ----------------------------------------------------------------------
 */

/*
 * The Service Container itself
 */

MICO::CCM::ServiceContainer::ServiceContainer (CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj;
  char name[256];

  _have_info = 0;
  _orb = CORBA::ORB::_duplicate (orb);
  _configuration_complete = 0;

  /*
   * Create a private POA
   */

  sprintf (name, "ServiceContainer-%lu", _my_number);

  obj = _orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);

  CORBA::PolicyList pl;
  _my_poa = poa->create_POA (name, PortableServer::POAManager::_nil(), pl);
}

MICO::CCM::ServiceContainer::~ServiceContainer ()
{
}

void
MICO::CCM::ServiceContainer::load (const ComponentInfo & info)
{
  assert (!_have_info);
  _info = info;
  _have_info = 1;

  /*
   * Activate home and component singletons
   */

  PortableServer::ObjectId_var homeid =
    _my_poa->activate_object (_info.home_glue.in());
  _home_ref = _my_poa->id_to_reference (homeid.in());

  PortableServer::ObjectId_var compid =
    _my_poa->activate_object (_info.comp_glue.in());
  _comp_ref = _my_poa->id_to_reference (compid.in());
}

void
MICO::CCM::ServiceContainer::activate ()
{
  /*
   * Set instances to active, if their configuration is complete
   */

  Components::SessionComponent_var sc =
    Components::SessionComponent::_narrow (_info.home_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_activate ();
  }

  sc = Components::SessionComponent::_narrow (_info.comp_instance);

  if (!CORBA::is_nil (sc) && _configuration_complete) {
    sc->ccm_activate ();
  }

  /*
   * Activate our POA
   */

  PortableServer::POAManager_var mgr = _my_poa->the_POAManager ();
  mgr->activate ();
}

void
MICO::CCM::ServiceContainer::passivate ()
{
  /*
   * Set instances to inactive
   */

  Components::SessionComponent_var sc =
    Components::SessionComponent::_narrow (_info.home_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_passivate ();
  }

  sc = Components::SessionComponent::_narrow (_info.comp_instance);

  if (!CORBA::is_nil (sc) && _configuration_complete) {
    sc->ccm_passivate ();
  }

  /*
   * Deactivate our POA
   */

  PortableServer::POAManager_var mgr = _my_poa->the_POAManager ();
  mgr->hold_requests (1);
}

void
MICO::CCM::ServiceContainer::remove ()
{
  passivate ();

  /*
   * Prepare instances for destruction
   */

  Components::SessionComponent_var sc =
    Components::SessionComponent::_narrow (_info.home_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_remove ();
  }

  sc = Components::SessionComponent::_narrow (_info.comp_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_remove ();
  }

  /*
   * Destroy our POA
   */

  _my_poa->destroy (1, 1);
}

CORBA::Boolean
MICO::CCM::ServiceContainer::compare (Components::CCMHome_ptr ohome)
{
  PortableServer::ObjectId_var myid = _my_poa->reference_to_id (_home_ref);
  PortableServer::ObjectId_var oid;

  try {
    oid = _my_poa->reference_to_id (ohome);
  }
  catch (PortableServer::POA::WrongAdapter & ex) {
    return 0;
  }

  return ((myid->length() == oid->length()) &&
	  memcmp (myid->get_buffer(), oid->get_buffer(), myid->length()) == 0);
}

Components::CCMHome_ptr
MICO::CCM::ServiceContainer::get_reference_for_home ()
{
  return Components::CCMHome::_narrow (_home_ref.in());
}

Components::CCMObject_ptr
MICO::CCM::ServiceContainer::get_reference_for_component ()
{
  return Components::CCMObject::_narrow (_comp_ref.in());
}

/*
 * Facet management
 */

CORBA::Object_ptr
MICO::CCM::ServiceContainer::activate_facet (PortableServer::Servant,
					     const char * name,
					     CORBA::Object_ptr facet_instance,
					     PortableServer::Servant facet_glue)
{
  PortableServer::ObjectId_var fid = _my_poa->activate_object (facet_glue);
  CORBA::Object_var fref = _my_poa->id_to_reference (fid.in());

  facet_glue->_add_ref ();
  _facet_glue[name] = facet_glue;
  _facet_instance[name] = CORBA::Object::_duplicate (facet_instance);
  _facet_reference[name] = CORBA::Object::_duplicate (fref);

  return fref._retn ();
}

void
MICO::CCM::ServiceContainer::configuration_complete (PortableServer::Servant)
{
  _configuration_complete = 1;
}

/*
 * for Service Context
 */

Components::CCMHome_ptr
MICO::CCM::ServiceContainer::get_CCM_home ()
{
  return get_reference_for_home ();
}

CORBA::Object_ptr
MICO::CCM::ServiceContainer::get_CCM_object (CORBA::Object_ptr)
{
  return get_reference_for_component ();
}

/*
 * ----------------------------------------------------------------------
 * Implementation for the Session Container
 * ----------------------------------------------------------------------
 */

/*
 * The Session Container itself
 */

MICO::CCM::SessionContainer::SessionContainer (CORBA::ORB_ptr orb)
{
  CORBA::Object_var obj;
  char name[256];

  _have_info = 0;
  _orb = CORBA::ORB::_duplicate (orb);

  /*
   * Create a private POA
   */

  sprintf (name, "SessionContainer-%lu", _my_number);

  obj = _orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);

  CORBA::PolicyList pl;
  _my_poa = poa->create_POA (name, PortableServer::POAManager::_nil(), pl);
}

MICO::CCM::SessionContainer::~SessionContainer ()
{
}

void
MICO::CCM::SessionContainer::load (const ComponentInfo & info)
{
  assert (!_have_info);
  _info = info;
  _have_info = 1;

  /*
   * Activate home singleton
   */

  PortableServer::ObjectId_var oid =
    _my_poa->activate_object (_info.home_glue.in());
  _home_ref = _my_poa->id_to_reference (oid.in());
}

void
MICO::CCM::SessionContainer::activate ()
{
  /*
   * Set instances to active, if their configuration is complete
   */

  Components::SessionComponent_var sc =
    Components::SessionComponent::_narrow (_info.home_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_activate ();
  }

  for (InstanceMap::iterator i=active_components.begin();
       i != active_components.end(); i++) {
    sc = Components::SessionComponent::_narrow ((*i).second.instance);
    if (!CORBA::is_nil (sc) && (*i).second.configuration_complete) {
      sc->ccm_activate ();
    }
  }

  /*
   * Activate our POA
   */

  PortableServer::POAManager_var mgr;
  mgr = _my_poa->the_POAManager ();
  mgr->activate ();
}

void
MICO::CCM::SessionContainer::passivate ()
{
  /*
   * Set instances to inactive
   */

  Components::SessionComponent_var sc =
    Components::SessionComponent::_narrow (_info.home_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_passivate ();
  }

  for (InstanceMap::iterator i=active_components.begin();
       i != active_components.end(); i++) {
    sc = Components::SessionComponent::_narrow ((*i).second.instance);
    if (!CORBA::is_nil (sc) && (*i).second.configuration_complete) {
      sc->ccm_passivate ();
    }
  }

  /*
   * Deactivate our POA
   */

  PortableServer::POAManager_var mgr = _my_poa->the_POAManager ();
  mgr->hold_requests (1);
}

void
MICO::CCM::SessionContainer::remove ()
{
  passivate ();

  /*
   * Prepare instances for destruction
   */

  Components::SessionComponent_var sc =
    Components::SessionComponent::_narrow (_info.home_instance);

  if (!CORBA::is_nil (sc)) {
    sc->ccm_remove ();
  }

  for (InstanceMap::iterator i=active_components.begin();
       i != active_components.end(); i++) {
    sc = Components::SessionComponent::_narrow ((*i).second.instance);
    if (!CORBA::is_nil (sc)) {
      sc->ccm_remove ();
    }
  }

  /*
   * Destroy our POA
   */

  _my_poa->destroy (1, 1);
}

CORBA::Boolean
MICO::CCM::SessionContainer::compare (Components::CCMHome_ptr ohome)
{
  PortableServer::ObjectId_var myid = _my_poa->reference_to_id (_home_ref);
  PortableServer::ObjectId_var oid;

  try {
    oid = _my_poa->reference_to_id (ohome);
  }
  catch (PortableServer::POA::WrongAdapter &) {
    return 0;
  }

  return ((myid->length() == oid->length()) &&
	  memcmp (myid->get_buffer(), oid->get_buffer(), myid->length()) == 0);
}

Components::CCMHome_ptr
MICO::CCM::SessionContainer::get_reference_for_home ()
{
  return Components::CCMHome::_narrow (_home_ref.in());
}

Components::CCMObject_ptr
MICO::CCM::SessionContainer::activate_component (CORBA::Object_ptr inst,
						 PortableServer::Servant skel)
{
  PortableServer::ObjectId_var oid = _my_poa->activate_object (skel);
  CORBA::Object_var ref = _my_poa->id_to_reference (oid.in());

  PerComponentData & data = active_components[oid.in()];
  data.configuration_complete = 0;
  skel->_add_ref ();
  data.glue = skel;
  data.instance = CORBA::Object::_duplicate (inst);
  data.reference = CORBA::Object::_duplicate (ref);

  return Components::CCMObject::_narrow (ref);
}

Components::CCMObject_ptr
MICO::CCM::SessionContainer::get_reference_for_component (PortableServer::Servant s)
{
  CORBA::Object_var o = _my_poa->servant_to_reference (s);
  return Components::CCMObject::_narrow (o);
}

PortableServer::Servant
MICO::CCM::SessionContainer::get_skeleton_for_reference (CORBA::Object_ptr o)
{
  return _my_poa->reference_to_servant (o);
}

Components::CCMObject_ptr
MICO::CCM::SessionContainer::get_reference_for_instance (CORBA::Object_ptr o)
{
  InstanceMap::iterator it = active_components.begin ();
  while (it != active_components.end()) {
    if ((*it).second.instance.in() == o) {
      CORBA::Object_var ref = _my_poa->id_to_reference ((*it).first);
      return Components::CCMObject::_narrow (ref);
    }
    it++;
  }
  return Components::CCMObject::_nil ();
}

CORBA::Object_ptr
MICO::CCM::SessionContainer::get_instance_for_component (PortableServer::Servant skel)
{
  PortableServer::ObjectId_var oid = _my_poa->servant_to_id (skel);
  return CORBA::Object::_duplicate (active_components[oid.in()].instance);
}

void
MICO::CCM::SessionContainer::deactivate_component (CORBA::Object_ptr o)
{
  PortableServer::ServantBase_var skel = get_skeleton_for_reference (o);
  deactivate_component (skel);
}

void
MICO::CCM::SessionContainer::deactivate_component (PortableServer::Servant skel)
{
  PortableServer::ObjectId_var oid = _my_poa->servant_to_id (skel);

  PerComponentData & data = active_components[oid.in()];
  map<string, PortableServer::ServantBase_var, less<string> >::iterator it;

  for (it=data.facet_glue.begin(); it!=data.facet_glue.end(); it++) {
    PortableServer::ObjectId_var fid =
      _my_poa->servant_to_id ((*it).second);
    _my_poa->deactivate_object (fid.in());
  }

  _my_poa->deactivate_object (oid);
  active_components.erase (oid.in());
}

CORBA::Object_ptr
MICO::CCM::SessionContainer::activate_facet (PortableServer::Servant comp_glue,
					     const char * name,
					     CORBA::Object_ptr facet_instance,
					     PortableServer::Servant facet_glue)
{
  PortableServer::ObjectId_var oid = _my_poa->servant_to_id (comp_glue);
  PortableServer::ObjectId_var fid = _my_poa->activate_object (facet_glue);
  CORBA::Object_var fref = _my_poa->id_to_reference (fid.in());

  PerComponentData & data = active_components[oid.in()];
  facet_glue->_add_ref ();
  data.facet_glue[name] = facet_glue;
  data.facet_instance[name] = CORBA::Object::_duplicate (facet_instance);
  data.facet_reference[name] = CORBA::Object::_duplicate (fref);

  return fref._retn ();
}

/*
 * Facet management
 */

void
MICO::CCM::SessionContainer::configuration_complete (PortableServer::Servant comp_glue)
{
  PortableServer::ObjectId_var oid = _my_poa->servant_to_id (comp_glue);
  PerComponentData & data = active_components[oid.in()];
  data.configuration_complete = 1;
}

/*
 * for Service Context
 */

Components::CCMHome_ptr
MICO::CCM::SessionContainer::get_CCM_home ()
{
  return get_reference_for_home ();
}

CORBA::Object_ptr
MICO::CCM::SessionContainer::get_CCM_object (CORBA::Object_ptr o)
{
  return get_reference_for_instance (o);
}
/*
 * ----------------------------------------------------------------------
 * Valuetype implementations
 * ----------------------------------------------------------------------
 */

MICO::CCM::PortDescription_impl::PortDescription_impl ()
{
  name ((const char *) "");
  type_id ((const char *) "");
}

MICO::CCM::Cookie_impl::Cookie_impl ()
{
}

MICO::CCM::FacetDescription_impl::FacetDescription_impl ()
{
  facet_ref (CORBA::Object::_nil ());
}

MICO::CCM::ConnectionDescription_impl::ConnectionDescription_impl ()
{
  ck (0);
  objref (CORBA::Object::_nil ());
}

MICO::CCM::ReceptacleDescription_impl::ReceptacleDescription_impl ()
{
}

MICO::CCM::ConsumerDescription_impl::ConsumerDescription_impl ()
{
  consumer (Components::EventConsumerBase::_nil ());
}

MICO::CCM::EmitterDescription_impl::EmitterDescription_impl ()
{
  consumer (Components::EventConsumerBase::_nil ());
}

MICO::CCM::SubscriberDescription_impl::SubscriberDescription_impl ()
{
  ck (0);
  consumer (Components::EventConsumerBase::_nil ());
}

MICO::CCM::PublisherDescription_impl::PublisherDescription_impl ()
{
}

MICO::CCM::ComponentPortDescription_impl::ComponentPortDescription_impl ()
{
}

/*
 * ----------------------------------------------------------------------
 * Valuetype factories
 * ----------------------------------------------------------------------
 */

CORBA::ValueBase *
MICO::CCM::PortDescription_Factory::create_for_unmarshal ()
{
  return new PortDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::Cookie_Factory::create_for_unmarshal ()
{
  return new Cookie_impl;
}

CORBA::ValueBase *
MICO::CCM::FacetDescription_Factory::create_for_unmarshal ()
{
  return new FacetDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::ConnectionDescription_Factory::create_for_unmarshal ()
{
  return new ConnectionDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::ReceptacleDescription_Factory::create_for_unmarshal ()
{
  return new ReceptacleDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::ConsumerDescription_Factory::create_for_unmarshal ()
{
  return new ConsumerDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::EmitterDescription_Factory::create_for_unmarshal ()
{
  return new EmitterDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::SubscriberDescription_Factory::create_for_unmarshal ()
{
  return new SubscriberDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::PublisherDescription_Factory::create_for_unmarshal ()
{
  return new PublisherDescription_impl;
}

CORBA::ValueBase *
MICO::CCM::ComponentPortDescription_Factory::create_for_unmarshal ()
{
  return new ComponentPortDescription_impl;
}

void
MICO::CCM::register_all_factories (CORBA::ORB_ptr orb)
{
  CORBA::ValueFactoryBase_var vf;

  vf = new PortDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/PortDescription:1.0", vf);

  vf = new Cookie_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/Cookie:1.0", vf);

  vf = new ConnectionDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/ConnectionDescription:1.0", vf);

  vf = new ReceptacleDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/ReceptacleDescription:1.0", vf);

  vf = new ConsumerDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/ConsumerDescription:1.0", vf);

  vf = new EmitterDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/EmitterDescription:1.0", vf);

  vf = new SubscriberDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/SubscriberDescription:1.0", vf);

  vf = new PublisherDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/PublisherDescription:1.0", vf);

  vf = new ComponentPortDescription_Factory;
  orb->register_value_factory ("IDL:omg.org/Components/ComponentPortDescription:1.0",
			       vf);
}
