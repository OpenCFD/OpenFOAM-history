#include <fstream.h>
#include <time.h>
#include "account.h"


using namespace std;

// Standard servant implementation
/*
 * A Bank factory that creates Account objects
 */

/*
 * Implementation of the Account
 */

class Account_impl : virtual public POA_Account
{
public:
  Account_impl ();

  void deposit (CORBA::ULong);
  void withdraw (CORBA::ULong);
  CORBA::Long balance ();

private:
  CORBA::Long bal;
};

Account_impl::Account_impl ()
{
  bal = 0;
}

void
Account_impl::deposit (CORBA::ULong amount)
{
  bal += amount;
}

void
Account_impl::withdraw (CORBA::ULong amount)
{
  bal -= amount;
}

CORBA::Long
Account_impl::balance ()
{
  return bal;
}

/*
 * Implementation of the Bank
 * Now the factory puts the Accounts into a different POA which is used for access control
 */

class Bank_impl : virtual public POA_Bank
{
public:
  Bank_impl (){};
  Bank_impl (PortableServer::POA_ptr);
  Account_ptr create ();
  Account_ptr open ();
private:
  PortableServer::POA_var localpoa;
};

Bank_impl::Bank_impl (PortableServer::POA_ptr _poa)
{
  localpoa = PortableServer::POA::_duplicate (_poa);
}

Account_ptr
Bank_impl::create ()
{
  /*
    Make Account objects and put them into localpoa
  */
  Account_impl * ai = new Account_impl;
  PortableServer::ObjectId_var oid = localpoa->activate_object (ai);
  CORBA::Object_var ref = localpoa->id_to_reference (oid.in());
  Account_ptr aref = Account::_narrow(ref);
  return aref;

}

Account_ptr
Bank_impl::open ()
{
  Account_impl * ai = new Account_impl;
  PortableServer::ObjectId_var oid = localpoa->activate_object (ai);
  CORBA::Object_var ref = localpoa->id_to_reference (oid.in());
  Account_ptr aref = Account::_narrow(ref);
  return aref;
}


int
main (int argc, char *argv[])
{
  // Make ORB and RootPOA
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  // Object Domain Mapping
  // Get the initial reference to the ODM and narrow it
  CORBA::Object_var objodm = orb->resolve_initial_references ("ODM");
  ObjectDomainMapping::ODM_var odm = ObjectDomainMapping::ODM::_narrow(objodm);
  // Create an ODM factory
  ObjectDomainMapping::Factory_var factory = odm->current(); // in case we already have some ODM
  															 // from arguments
  if (CORBA::is_nil(factory))
    factory = odm->create(); // we don't have ODM

  // Create two other POA with default policies
  CORBA::PolicyList pl;
  //pl.length(0);
  pl.length(2);
  pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

  // PERSISTENT needs additional command line arguments
  pl[1] = poa->create_lifespan_policy(PortableServer::TRANSIENT);

  PortableServer::POA_var bankpoa = poa->create_POA ("BankPOA", mgr, pl);
  
  CORBA::PolicyList pl1;
  pl1.length(1);
  pl1[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  PortableServer::POA_var accountpoa = poa->create_POA ("AccountPOA", mgr, pl1);

  // Register the three POAs with ODM
  poa->register_ODM_factory(factory);
  bankpoa->register_ODM_factory(factory);
  accountpoa->register_ODM_factory(factory);
  // The RootPOA does not contain any objects
  // The BankPOA contains the Bank object the factory for Account objects. 
  // The AccountPOA contains the Account objects produced by the Bank.


  // Get a Domain Manager to configure the mapping manually
  ObjectDomainMapping::Manager_ptr dmanager1 = poa->get_ODM();

  // First of all configure the default mapping. All server objects which are not explicitly
  // in another domain are in /Audit:
  // [/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=ServerTest/emailAddress=server@test]/ 
  // -> /Audit

  // Define the key as string and copy it to the Opaque data type.
  Security::Opaque okey2;
  string key2 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test] /";
  //  string key2 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=ServerTest/emailAddress=server@test]"; // triggers out of range exception
  int len = key2.length();
  okey2.length(len);
  for (int i = 0; i < len; i++)
    okey2[i] = key2[i];

  // Set up a Domain name list with one entry of length 1
  SecurityDomain::NameList dl;
  dl.length(1);
  // Set the name "Audit" and the type "Audit"
  // Warning: The kind must be set to the function the Domain is used for, in this case "Access", because
  // it is an access control domain.
  SecurityDomain::NameComponent nc;
  nc.id = CORBA::string_dup("Audit");
  nc.kind = CORBA::string_dup("Audit");
  SecurityDomain::Name nm;
  nm.length(1);
  nm[0] = nc;
  // Put the domain into the list
  dl[0] = nm;
  // and set the mapping. The granularity is "1".
  dmanager1->set_domain_name_key((CORBA::UShort)1, okey2, dl); 


  // Now set the mapping for the Bank:
  // [/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=ServerTest/emailAddress=server@test]/RootPOA/BankPOA/ 
  // ->/Access/Bank
  // Now we are using mapping based on the POA, all objects in /RootPOA/BankPOA will be in domain 
  // /Access/Bank

  // Again define the key
  Security::Opaque okey3;
  string key3 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test] /RootPOA/BankPOA";

  int len3 = key3.length();
  okey3.length(len3);
  for (int i = 0; i < len3; i++)
    okey3[i] = key3[i];
  SecurityDomain::NameList dl3;
  dl3.length(1);

  // Define one Domain list with a domain name of length 2
  SecurityDomain::NameComponent nc3;
  nc3.id = CORBA::string_dup("Audit");
  nc3.kind = CORBA::string_dup("Audit");
  SecurityDomain::Name nm3;
  nm3.length(2);
  nm3[0] = nc3;
  nc3.id = CORBA::string_dup("d1");
  nc3.kind = CORBA::string_dup("Audit");
  nm3[1] = nc3;
  dl3[0] = nm3;
  // Set the mapping with granularity 3
  dmanager1->set_domain_name_key((CORBA::UShort)3, okey3, dl3); 
  
  // Set the mapping for the Accounts: All Account objects are in AccountPOA
  // [/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test]/RootPOA/AccountPOA/
  // ->/Access/Accounts

  Security::Opaque okey4;
  string key4 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test] RootPOA/AccountPOA";

  int len4 = key4.length();
  okey4.length(len4);
  for (int i = 0; i < len4; i++)
    okey4[i] = key4[i];
  SecurityDomain::NameList dl4;
  dl4.length(1);

  SecurityDomain::NameComponent nc4;
  nc4.id = CORBA::string_dup("Audit");
  nc4.kind = CORBA::string_dup("Audit");
  SecurityDomain::Name nm4;
  nm4.length(2);
  nm4[0] = nc4;
  nc4.id = CORBA::string_dup("d1");
  nc4.kind = CORBA::string_dup("Audit");
  nm4[1] = nc4;
  dl4[0] = nm4;
  dmanager1->set_domain_name_key((CORBA::UShort)2, okey4, dl4); 
  
  // Save ODM Mapping to file
  factory->save_config_file("ODM.map");

  // create a Domain Managers tree
  // We are doing everything from beginning, for demonstration. We also could reuse the domain names
  // we defined earlier.

  // Get an initial reference for Domain Manager and narrow
  CORBA::Object_var factobj = orb->resolve_initial_references("DomainManagerFactory");
  SecurityDomain::DomainManagerFactory_var dmfactory = SecurityDomain::DomainManagerFactory::_narrow(factobj);

  // Define a root domain of type "Audit" and cast it to the correct type
  dmfactory->add_root_domain_manager("Audit");
  SecurityDomain::DomainManagerAdmin_ptr dmroot = dmfactory->get_root_domain_manager("Audit");
  SecurityDomain::DomainAuthorityAdmin_ptr daaroot = SecurityDomain::DomainAuthorityAdmin::_narrow(dmroot);
  
  // And add a 2nd level domain "d1" with type "Audit"
  // Create the Domain Manager for this domain
  SecurityDomain::DomainManagerAdmin_ptr ndo = dmfactory->create_domain_manager();
  // Define the domain name
  SecurityDomain::Name first;
  first.length(1);
  first[0].id = CORBA::string_dup("d1");
  first[0].kind = CORBA::string_dup("Audit");
  // and add it to the Domain Manager
  daaroot->add_domain_manager(ndo, first);

  // added Bank, now we have /Audit and /Audit/d1

  // Get a Domain Manager for /Audit

  // set policy for /Audit
  // Get the reference to the SecTargetInvocationAudit to set
  CORBA::Policy_ptr po = daaroot->get_domain_policy(Security::SecTargetInvocationAudit);
  SecurityAdmin::AuditPolicy_var polroot = SecurityAdmin::AuditPolicy::_narrow(po);

  // 1st policy of audit.cnf
  
  Security::AuditCombinator at_comb;
  Security::AuditEventTypeList eventtypelst;
  Security::SelectorValueList  selectorlst;
  Security::AuditEventType evtype;
  evtype.event_family.family_definer = 0; // OMG
  evtype.event_family.family = 12; // Audit event family
  
  // selector values
  selectorlst.length(7);
  selectorlst[0].selector = Security::InterfaceName;
  selectorlst[0].value <<= "";
  
  selectorlst[1].selector = Security::ObjectRef;
  selectorlst[1].value <<= "";
  
  selectorlst[2].selector = Security::Operation;
  selectorlst[2].value <<= "";
  
  selectorlst[3].selector = Security::Initiator;
  selectorlst[3].value <<= "";

  selectorlst[4].selector = Security::SuccessFailure;
  selectorlst[4].value <<= (short)(-1);
  
  selectorlst[5].selector = Security::Time;
  SecurityAdmin::AuditTimeInterval time_interval;
  time_interval.begin = 1;
  time_interval.end =  1;
  selectorlst[5].value <<= time_interval; //any time interval;
  
  selectorlst[6].selector = Security::DayOfWeek;
  selectorlst[6].value <<= (short)(1); 
  
  // as a result - nothing selected
  
  at_comb = Security::SecAnySelector; // audit combinator "Any"

  eventtypelst.length(2); // 2 events we need
  evtype.event_type = Security::AuditPrincipalAuth;
  eventtypelst[0] = evtype;
  eventtypelst[1] = evtype;
  eventtypelst[1].event_type = Security::AuditSessionAuth;


  polroot->set_audit_selectors("", eventtypelst, selectorlst, at_comb);
  // Associate the Policy with the Root Domain (Audit)
  daaroot->set_domain_policy(polroot);
  
  // Now the same for the policy in /Access/d1
  SecurityDomain::Name fullnamed1;
  fullnamed1.length(1);
  fullnamed1[0] = first[0];
  SecurityDomain::DomainManagerAdmin_ptr dm2 = daaroot->get_domain_manager(fullnamed1);
  SecurityDomain::DomainAuthorityAdmin_ptr dmath2 = SecurityDomain::DomainAuthorityAdmin::_narrow(dm2);
  CORBA::Policy_ptr po2 = daaroot->get_domain_policy(Security::SecTargetInvocationAudit);
  SecurityAdmin::AuditPolicy_var pold1 = SecurityAdmin::AuditPolicy::_narrow(po2);

  Security::AuditCombinator at_comb3;
  Security::AuditEventTypeList eventtypelst3;
  Security::SelectorValueList  selectorlst3;
  Security::AuditEventType evtype3;
  evtype3.event_family.family_definer = 0; // OMG
  evtype3.event_family.family = 12; // Audit event family
  
  // selector values
  selectorlst3.length(7);
  selectorlst3[0].selector = Security::InterfaceName;
  selectorlst3[0].value <<= "IDL:Bank:1.0";
  
  selectorlst3[1].selector = Security::ObjectRef;
  selectorlst3[1].value <<= "";
  
  selectorlst3[2].selector = Security::Operation;
  selectorlst3[2].value <<= "";

  selectorlst3[3].selector = Security::Initiator;
  selectorlst3[3].value <<= "";

  selectorlst3[4].selector = Security::SuccessFailure;
  selectorlst3[4].value <<= (short)(-1); //
  
  selectorlst3[5].selector = Security::Time;
  SecurityAdmin::AuditTimeInterval time_interval3;
  time_interval3.begin = 1;
  time_interval3.end =  1;
  selectorlst3[5].value <<= time_interval; //any time interval;
  selectorlst3[6].selector = Security::DayOfWeek;
  selectorlst3[6].value <<= (short)(-1); // Any day

  at_comb3 = Security::SecAllSelectors; // audit combinator "All"

  eventtypelst3.length(1); // 1 event we need
  evtype3.event_type = Security::AuditInvocation;
  eventtypelst3[0] = evtype3;
  
  pold1->set_audit_selectors("IDL:Bank:1.0", eventtypelst3, selectorlst3, at_comb3);

  dmath2->set_domain_policy(pold1);

  // continue with the normal server code:
  
  /*
   * Create Bank object
   */
        Bank_impl * micocash = new Bank_impl(accountpoa);
  /*
   * Activate the Bank
   */

  PortableServer::ObjectId_var oid = bankpoa->activate_object (micocash);

  /*
   * Write references to files
   */

  ofstream of ("Bank.ref");
  CORBA::Object_var ref = bankpoa->id_to_reference (oid.in());
  CORBA::String_var strn = orb->object_to_string (ref.in());
  of << strn.in() << endl;
  of.close ();
  /*
   * Activate the POA and start serving requests
   */

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  bankpoa->destroy (TRUE, TRUE);
  delete micocash;
  return 0;
}
