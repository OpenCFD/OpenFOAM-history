#include <CORBA.h>
#include <coss/CosNaming.h>
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
  Account_ptr create ();
};

Account_ptr
Bank_impl::create ()
{
  /*
   * Create a new account (which is never deleted)
   */

  Account_impl * ai = new Account_impl;

  /*
   * Obtain a reference using _this. This implicitely activates the
   * account servant (the RootPOA, which is the object's _default_POA,
   * has the IMPLICIT_ACTIVATION policy)
   */

  Account_ptr aref = ai->_this ();
  assert (!CORBA::is_nil (aref));

  /*
   * Return the reference
   */

  return aref;
}

/*
 * Main
 */

int
main (int argc, char *argv[])
{
  /*
   * Initialize ORB, get the Root POA, and register Account object as usual
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  
  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  /*
   * Create a Bank
   */

  Bank_impl * micocash = new Bank_impl;

  /*
   * Activate the Bank
   */

  PortableServer::ObjectId_var oid = poa->activate_object (micocash);
  CORBA::Object_var ref = poa->id_to_reference (oid.in());

  /*
   * In demo/poa/account-1, this object reference (ref) is written to a
   * file. Here, we store the reference in the Naming Service
   */
  
  /*
   * Acquire a reference to the Naming Service
   */

  CORBA::Object_var nsobj =
    orb->resolve_initial_references ("NameService");

  CosNaming::NamingContext_var nc = 
    CosNaming::NamingContext::_narrow (nsobj);

  if (CORBA::is_nil (nc)) {
    cerr << "oops, I cannot access the Naming Service!" << endl;
    exit (1);
  }

  /*
   * Construct Naming Service name for our Bank
   */

  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("Bank");
  name[0].kind = CORBA::string_dup ("");
  
  /*
   * Store a reference to our Bank in the Naming Service. We use 'rebind'
   * here instead of 'bind', because rebind does not complain if the desired
   * name "Bank" is already registered, but silently overwrites it (the
   * existing reference is probably from an old incarnation of this server).
   */

  cout << "Binding Bank in the Naming Service ... " << flush;
  nc->rebind (name, ref);
  cout << "done." << endl;

  /*
   * Activate the POA and start serving requests
   */

  printf ("Running.\n");

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  delete micocash;

  return 0;
}
