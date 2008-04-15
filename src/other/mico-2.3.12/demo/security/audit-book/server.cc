
#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class Hello_impl : virtual public POA_Hello {
public:
  void hello_world () {
    cout << "Start Servant hello\n";
    cout << "hello\n";
    cout << "End Servant hello\n";
  }
  void olleh () {
	cout << "Start Servant olleh\n";
	cout << "olleh\n";
	cout << "End Servant olleh\n";
  }
};

int
main (int argc, char *argv[])
{

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  PortableServer::POA_var poa;
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  Hello_impl * micohello = new Hello_impl;
  PortableServer::ObjectId_var oid = poa->activate_object (micohello);
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var ref_str = orb->object_to_string (ref.in());

  CORBA::Object_var policy_current_obj = orb->resolve_initial_references ("PolicyCurrent");
  SecurityLevel2::PolicyCurrent_var policy_current = SecurityLevel2::PolicyCurrent::_narrow(policy_current_obj);
  assert (!CORBA::is_nil (policy_current));

  CORBA::Object_var objodm = orb->resolve_initial_references ("ODM");
  ObjectDomainMapping::ODM_var odm = ObjectDomainMapping::ODM::_narrow(objodm);
  ObjectDomainMapping::Factory_var factory = odm->current(); // in case we already have some ODM

  if (CORBA::is_nil(factory))
    factory = odm->create(); // we don't have ODM
  poa->register_ODM_factory(factory);

  ObjectDomainMapping::Manager_ptr dmanager1 = poa->get_ODM();
  Security::Opaque okey2;
  string key2 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/Email=server@test] /";

  int len = key2.length();
  okey2.length(len);
  for (int i = 0; i < len; i++)
    okey2[i] = key2[i];


  SecurityDomain::NameList dl;
  dl.length(1);

  SecurityDomain::NameComponent nc;
  nc.id = CORBA::string_dup("Audit");
  nc.kind = CORBA::string_dup("Audit");
  SecurityDomain::Name nm;
  nm.length(1);
  nm[0] = nc;
  dl[0] = nm;
  dmanager1->set_domain_name_key((CORBA::UShort)1, okey2, dl);

  factory->save_config_file("ODM.map");

  CORBA::Object_var factobj = orb->resolve_initial_references("DomainManagerFactory");
  SecurityDomain::DomainManagerFactory_var dmfactory = SecurityDomain::DomainManagerFactory::_narrow(factobj);

  dmfactory->add_root_domain_manager("Audit");
  SecurityDomain::DomainManagerAdmin_ptr dmroot = dmfactory->get_root_domain_manager("Audit");
  SecurityDomain::DomainAuthorityAdmin_ptr daaroot = SecurityDomain::DomainAuthorityAdmin::_narrow(dmroot);

  CORBA::PolicyTypeSeq policy_types;
  policy_types.length(1);
  policy_types[0] = Security::SecTargetInvocationAudit;
  
  CORBA::PolicyList * policies = policy_current -> get_policy_overrides(policy_types);
  CORBA::Policy_ptr policy = (*policies)[0];

  SecurityAdmin::AuditPolicy_ptr audit_policy = SecurityAdmin::AuditPolicy::_narrow(policy);
  
  Security::AuditEventTypeList events;
  events.length(4);
  
  Security::ExtensibleFamily family;
  family.family_definer = 0;
  family.family = 25;
  
  events[0].event_family = family;
  events[0].event_type = Security::AuditInvocation;

  //Selectors
  int i = 0;
  Security::SelectorValueList selectors;
  selectors.length(1);

  selectors[i].selector = Security::Operation;
  selectors[i].value <<= "hello"; // Das tut nicht

  Security::AuditCombinator audit_combinator =  Security::SecAllSelectors;

  audit_policy->set_audit_selectors("",events,selectors,audit_combinator );
  daaroot->set_domain_policy(audit_policy);

  cout << "Activate Servant\n";
  mgr->activate ();
  orb->run ();
  return 0;
}
