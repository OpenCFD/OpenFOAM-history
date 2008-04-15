#include <fstream.h>
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
  ObjectDomainMapping::Factory_var factory = odm->create(); 

  // Load the ODM configuration file
  // In this case the configuration file contains only the domain for auditing, all Access domains
  // are commented out. The Access mapping will be set later in the server
  CORBA::Boolean res = factory->load_config_file(NULL); // use filename from arguments


  // Create two other POA with default policies
  CORBA::PolicyList pl;
  pl.length(0);
  //  pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  // PERSISTENT needs additional command line arguments
  //  pl[1] = poa->create_lifespan_policy(PortableServer::PERSISTENT);

  PortableServer::POA_var bankpoa = poa->create_POA ("BankPOA", mgr, pl);
  PortableServer::POA_var accountpoa = poa->create_POA ("AccountPOA", mgr, pl);

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
  // in another domain are in /Access:
  // [/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=ServerTest/emailAddress=server@test]/ 
  // -> /Access

  // Define the key as string and copy it to the Opaque data type.
  Security::Opaque okey2;
  string key2 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test] /";
  int len = key2.length();
  okey2.length(len);
  for (int i = 0; i < len; i++)
    okey2[i] = key2[i];

  // Set up a Domain name list with one entry of length 1
  SecurityDomain::NameList dl;
  dl.length(1);
  // Set the name "Access" and the type "Access"
  // Warning: The kind must be set to the function the Domain is used for, in this case "Access", because
  // it is an access control domain.
  SecurityDomain::NameComponent nc;
  nc.id = CORBA::string_dup("Access");
  nc.kind = CORBA::string_dup("Access");
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
  string key3 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test]/RootPOA/BankPOA";

  int len3 = key3.length();
  okey3.length(len3);
  for (int i = 0; i < len3; i++)
    okey3[i] = key3[i];
  SecurityDomain::NameList dl3;
  dl3.length(1);

  // Define one Domain list with a domain name of length 2
  SecurityDomain::NameComponent nc3;
  nc3.id = CORBA::string_dup("Access");
  nc3.kind = CORBA::string_dup("Access");
  SecurityDomain::Name nm3;
  nm3.length(2);
  nm3[0] = nc3;
  nc3.id = CORBA::string_dup("Bank");
  nc3.kind = CORBA::string_dup("Access");
  nm3[1] = nc3;
  dl3[0] = nm3;
  // Set the mapping with granularity 2
  dmanager1->set_domain_name_key((CORBA::UShort)2, okey3, dl3); 
  
  // Set the mapping for the Accounts: All Account objects are in AccountPOA
  // [/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test]/RootPOA/AccountPOA/
  // ->/Access/Accounts

  Security::Opaque okey4;
  string key4 = "[/C=UK/ST=Server State/L=Cambridge/O=ObjectSecurity Ltd./OU=RD/CN=Server Test/emailAddress=server@test]/RootPOA/AccountPOA";

  int len4 = key4.length();
  okey4.length(len4);
  for (int i = 0; i < len4; i++)
    okey4[i] = key4[i];
  SecurityDomain::NameList dl4;
  dl4.length(1);

  SecurityDomain::NameComponent nc4;
  nc4.id = CORBA::string_dup("Access");
  nc4.kind = CORBA::string_dup("Access");
  SecurityDomain::Name nm4;
  nm4.length(2);
  nm4[0] = nc3;
  nc4.id = CORBA::string_dup("Accounts");
  nc4.kind = CORBA::string_dup("Access");
  nm4[1] = nc3;
  dl4[0] = nm3;
  dmanager1->set_domain_name_key((CORBA::UShort)2, okey4, dl4); 
  
  // Save ODM Mapping to file
  factory->save_config_file("ODM.map");

  // create a Domain Managers tree
  // We are doing everything from beginning, for demonstration. We also could reuse the domain names
  // we defined earlier.

  // Get an initial reference for Domain Manager and narrow
  CORBA::Object_var factobj = orb->resolve_initial_references("DomainManagerFactory");
  SecurityDomain::DomainManagerFactory_var dmfactory = SecurityDomain::DomainManagerFactory::_narrow(factobj);

  // Define a root domain of type "Access" and cast it to the correct type
  dmfactory->add_root_domain_manager("Access");
  SecurityDomain::DomainManagerAdmin_ptr dmroot = dmfactory->get_root_domain_manager("Access");
  SecurityDomain::DomainAuthorityAdmin_ptr daaroot = SecurityDomain::DomainAuthorityAdmin::_narrow(dmroot);
  
  // And add a 2nd level domain "Bank" with type "Access"
  // Create the Domain Manager for this domain
  SecurityDomain::DomainManagerAdmin_ptr ndo = dmfactory->create_domain_manager();
  // Define the domain name
  SecurityDomain::Name first;
  first.length(1);
  first[0].id = CORBA::string_dup("Bank");
  first[0].kind = CORBA::string_dup("Access");
  // and add it to the Domain Manager
  daaroot->add_domain_manager(ndo, first);

  // added Bank, now we have /Access and /Access/Bank

  // Same for /Access/Accounts:
  SecurityDomain::DomainManagerAdmin_ptr ndo1 = dmfactory->create_domain_manager();
  SecurityDomain::Name second;
  second.length(1);
  second[0].id = CORBA::string_dup("Accounts");
  second[0].kind = CORBA::string_dup("Access");
  daaroot->add_domain_manager(ndo1, second);
  // added Accounts, now we have /Access, /Access/Bank, /Access/Accounts

  // Set the required rights for all objects in /Access/Bank

  // Get a Domain Manager for /Access/Bank
  // /Access is the root of the Access Control Domain, so we have to look for this Domain
  // Define the name relative to the root
  SecurityDomain::Name fullnameBank; 
  fullnameBank.length(1);
  fullnameBank[0] = first[0];
  // Get the Manager and narrow it
  SecurityDomain::DomainManagerAdmin_ptr dm = daaroot->get_domain_manager(fullnameBank);
  SecurityDomain::DomainAuthorityAdmin_ptr dmath = SecurityDomain::DomainAuthorityAdmin::_narrow(dm);

  // set policy for /Access/Bank
  // Get the reference to the SecTargetInvocationAccess to set
  CORBA::Policy_ptr po = dmath->get_domain_policy(Security::SecTargetInvocationAccess);
  SecurityAdmin::ObjectAccessPolicy_var polBank = SecurityAdmin::ObjectAccessPolicy::_narrow(po);

  // Define rights for:
  // IDL:Bank:1.0 create  corba:SecAnyRight:m

  Security::RightsList rlist1;
  rlist1.length(1);
  Security::Right right1;
  right1.rights_family.family_definer = 0; // OMG
  right1.rights_family.family = 1; // corba
  // MICOSec allows using all single character as rights.
  right1.rights_list = CORBA::string_dup("m");
  rlist1[0] = right1;

  // Set the required rights for this operation
  polBank->set_required_rights("create", "IDL:Bank:1.0", rlist1, Security::SecAnyRight);

  // Same for:
  // IDL:Bank:1.0 open    corba:SecAnyRight:ug

  Security::RightsList rlist12;
  rlist12.length(2);
  rlist12[0] = right1;
  rlist12[1] = right1;
  rlist12[0].rights_list = CORBA::string_dup("u"); 
  rlist12[1].rights_list = CORBA::string_dup("g"); 
  polBank->set_required_rights("open", "IDL:Bank:1.0", rlist12, Security::SecAnyRight);
  // Associate the Policy with the Domain
  dmath->set_domain_policy(polBank);
  // Only this policy combinator is supported.
  dmath->set_policy_combinator(Security::SecTargetInvocationAccess, SecurityDomain::Union);

  // Now the same for the objects in /Access/Accounts
  SecurityDomain::Name fullnameAccounts;
  fullnameAccounts.length(1);
  fullnameAccounts[0] = second[0];
  SecurityDomain::DomainManagerAdmin_ptr dm2 = daaroot->get_domain_manager(fullnameAccounts);
  SecurityDomain::DomainAuthorityAdmin_ptr dmath2 = SecurityDomain::DomainAuthorityAdmin::_narrow(dm2);
  CORBA::Policy_ptr po2 = dmath->get_domain_policy(Security::SecTargetInvocationAccess);
  SecurityAdmin::ObjectAccessPolicy_ptr polAccounts = SecurityAdmin::ObjectAccessPolicy::_narrow(po2);

  // IDL:Account:1.0 deposit  corba:SecAnyRight:ug
  Security::RightsList rlist2;
  rlist2.length(2);
  Security::Right right21;
  right21.rights_family.family_definer = 0; // OMG
  right21.rights_family.family = 1; // corba
  rlist2[0] = right21;
  rlist2[1] = right21;
  rlist2[0].rights_list = CORBA::string_dup("u");
  rlist2[1].rights_list = CORBA::string_dup("g");
  
  polAccounts->set_required_rights("deposit", "IDL:Account:1.0", rlist2, Security::SecAnyRight);

  // IDL:Account:1.0 balance  corba:SecAnyRight:ug

  rlist2[0] = right1;
  rlist2[1] = right1;
  rlist2[0].rights_list = CORBA::string_dup("u");
  rlist2[1].rights_list = CORBA::string_dup("g");

  polAccounts->set_required_rights("balance", "IDL:Account:1.0", rlist2, Security::SecAnyRight);

  // IDL:Account:1.0 withdraw corba:SecAnyRight:u

  Security::RightsList rlist3;
  rlist3.length(1);
  rlist3[0] = right1;
  rlist3[0].rights_list = CORBA::string_dup("u");

  polAccounts->set_required_rights("withdraw", "IDL:Account:1.0", rlist3, Security::SecAnyRight);

  dmath2->set_domain_policy(polAccounts);
  dmath2->set_policy_combinator(Security::SecTargetInvocationAccess, SecurityDomain::Union);
  
  // Now define the principal's rights:
  // Get a reference to the Security Manager and narrow it:
  CORBA::Object_var objsecman = orb->resolve_initial_references ("SecurityManager");
  SecurityLevel2::SecurityManager_var secman = SecurityLevel2::SecurityManager::_narrow(objsecman);
  // Get a reference to the rights list
  SecurityLevel2::AccessRights_ptr ar = secman->access_rights();

  // Define the rights of a principal with a certain AccessId.
  // Define the attribute as string and copy it to the Attribute value with opaque data
  int i;
  Security::SecAttribute attr1; // manager
  attr1.attribute_type.attribute_family.family_definer = 0;
  attr1.attribute_type.attribute_family.family = 1;
  attr1.attribute_type.attribute_type = Security::AccessId;
  string str = "/C=UK/ST=State/L=Cambridge/O=ObjectSecurity Ltd./OU=Section/CN=Manager/emailAddress=manager@ObjectSecurity.com";
  len = str.length();
  attr1.value.length(len + 1);
  for (i = 0; i < len; i++)
  	attr1.value[i] = str[i];
  attr1.value[len] = 0;

  // Define a right
  Security::RightsList rlist11;
  Security::Right right2;
  rlist11.length(1); // manager
  right2.rights_family.family_definer = 0; // OMG
  right2.rights_family.family = 1; // corba
  right2.rights_list = CORBA::string_dup("m");
  rlist11[0] = right1;

  // And grant the right to the principal
  ar->grant_rights(attr1, Security::SecInitiator, rlist11);
  
  // Same for "owner" and his "wife". The belong to the same PrimaryGroupId ("room").
  Security::SecAttribute attr2; // owner and wife
  attr2.attribute_type.attribute_family.family_definer = 0;
  attr2.attribute_type.attribute_family.family = 1;
  attr2.attribute_type.attribute_type = Security::PrimaryGroupId;
  str = "room";
  len = str.length();
  attr2.value.length(len + 1);
  for (i = 0; i < len; i++)
  	attr2.value[i] = str[i];
  attr2.value[len] = 0;

  Security::RightsList rlist22; // owner and wife
  rlist22.length(1);
  right2.rights_family.family_definer = 0; // OMG
  right2.rights_family.family = 1; // corba
  right2.rights_list = CORBA::string_dup("u");
  rlist22[0] = right2;
  
  ar->grant_rights(attr2, Security::SecInitiator, rlist22);

  // Now Domain Mapping, required rights of the domains and the principal's rights are set, we can 
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
