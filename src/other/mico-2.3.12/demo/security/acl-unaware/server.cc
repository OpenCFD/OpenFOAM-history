/*
 * A Bank factory that creates Account objects
 */

#include <fstream.h>
#include "account.h"


using namespace std;

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

/*
 * Implementation of Hello
 */
class Hello_impl : virtual public POA_Hello
{
  public:
  	virtual void hello_world();
};

void
Hello_impl::hello_world() {
    cout << "\nHello, world!\n";
}
 
int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  // Create other POAs
  CORBA::PolicyList pl;
  pl.length(1);
  pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  

  // Object Domain Mapping
  // Get the initial reference to the ODM and narrow it
//  CORBA::Object_var objodm = orb->resolve_initial_references ("ODM");
//  ObjectDomainMapping::ODM_var odm = ObjectDomainMapping::ODM::_narrow(objodm);
  // Create an ODM factory
  //  ObjectDomainMapping::Factory_var factory = odm->create();
  //  cout << "LOAD ODM\n";
  //  CORBA::Boolean res = factory->loadConfigFile(NULL);
//  ObjectDomainMapping::Factory_var factory = odm->current();

  PortableServer::POA_var mypoa = poa->create_POA ("MyPOA", mgr, pl);
  //  mypoa->registerODMFactory(factory);
  //  cout << "MyPOA registered\n";
  
  CORBA::PolicyList pl2;
  pl2.length(1);
  pl2[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  PortableServer::POA_var mypoa2 = poa->create_POA ("MyPOA2", mgr, pl);
  //mypoa2->registerODMFactory(factory);
  //cout << "MyPOA2 registered\n";
  PortableServer::POA_var accountpoa = poa->create_POA ("AccountPOA", mgr, pl2);
  //   accountpoa->registerODMFactory(factory);
  //cout << "AccountPOA registered\n";
  
  CORBA::PolicyList pl3;
  pl3.length(1);
  pl3[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  PortableServer::POA_var hellopoa = poa->create_POA ("HelloPOA", mgr, pl3);
  //hellopoa->registerODMFactory(factory);
  //cout << "HelloPOA registered\n";

  CORBA::PolicyList pl4;
  pl4.length(1);
  pl4[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  PortableServer::POA_var hellopoa2 = poa->create_POA ("HelloPOA2", mgr, pl4);
  //factory->saveConfigFile("ODM.map");

 // Normal server code
  /*
   * Create two Bank objects
   */
  Bank_impl * micocash = new Bank_impl(accountpoa);
  Bank_impl * micocash2 = new Bank_impl(accountpoa);
  // and Hello object
  Hello_impl * hellocash = new Hello_impl;
  
  /*
   * Activate the Bank
   */

  PortableServer::ObjectId_var oid = mypoa->activate_object (micocash);
  PortableServer::ObjectId_var oid2 = mypoa2->activate_object (micocash2);
  PortableServer::ObjectId_var oidh = hellopoa->activate_object(hellocash);
  PortableServer::ObjectId_var oidh2 = hellopoa2->activate_object(hellocash);

 /*
   * Write references to files
   */

  ofstream of ("Bank.ref");
  CORBA::Object_var ref = mypoa->id_to_reference (oid.in());
  CORBA::String_var strn = orb->object_to_string (ref.in());
  of << strn.in() << endl;
  of.close ();

  ofstream of2 ("Bank2.ref");
  CORBA::Object_var ref2 = mypoa2->id_to_reference (oid2.in());
  CORBA::String_var str2 = orb->object_to_string (ref2.in());
  of2 << str2.in() << endl;
  of2.close ();

  ofstream of3 ("Hello.ref");
  CORBA::Object_var ref3 = hellopoa->id_to_reference (oidh.in());
  CORBA::String_var str3 = orb->object_to_string (ref3.in());
  of3 << str3.in() << endl;
  of3.close ();

  ofstream of4 ("Hello2.ref");
  CORBA::Object_var ref4 = hellopoa2->id_to_reference (oidh2.in());
  CORBA::String_var str4 = orb->object_to_string (ref4.in());
  of4 << str4.in() << endl;
  of4.close ();

 /*
   * Activate the POA and start serving requests
   */

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  mypoa->destroy (TRUE, TRUE);
  delete micocash;



  return 0;
}
