/*
 * A Bank factory that creates Account objects
 */

#include "account.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


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

  /*
   * Create a Bank
   */

  Bank_impl * micocash = new Bank_impl;

  /*
   * Activate the Bank
   */

  PortableServer::ObjectId_var oid = poa->activate_object (micocash);

  /*
   * Write reference to file
   */

  ofstream of ("Bank.ref");
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
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

  poa->destroy (TRUE, TRUE);
  delete micocash;

  return 0;
}
