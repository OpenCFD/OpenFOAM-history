/*
 * A Bank factory that creates Account objects
 *
 * In this example, we set up a ServantManager. The bank creates
 * references to "virtual" objects. When an account object is invoked
 * for the first time, the ServantManager is called to incarnate the
 * account.
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
  Bank_impl (PortableServer::POA_ptr);
  Account_ptr create ();

private:
  PortableServer::POA_var mypoa;
};

Bank_impl::Bank_impl (PortableServer::POA_ptr _poa)
{
  mypoa = PortableServer::POA::_duplicate (_poa);
}

Account_ptr
Bank_impl::create ()
{
  /*
   * We don't create an account, but only a reference to a "virtual"
   * object. The Servant Manager is responsible for incarnating the
   * account on request.
   */

  CORBA::Object_var obj = mypoa->create_reference ("IDL:Account:1.0");
  Account_ptr aref = Account::_narrow (obj);
  assert (!CORBA::is_nil (aref));

  /*
   * Return the reference
   */

  return aref;
}

/*
 * Implementation of the Server Manager (a ServantActivator, actually)
 */

class AccountManager : public virtual POA_PortableServer::ServantActivator
{
public:
  PortableServer::Servant incarnate (const PortableServer::ObjectId &,
				     PortableServer::POA_ptr);
  void etherealize (const PortableServer::ObjectId &,
		    PortableServer::POA_ptr,
		    PortableServer::Servant,
		    CORBA::Boolean, CORBA::Boolean);
};

PortableServer::Servant
AccountManager::incarnate (const PortableServer::ObjectId & oid,
			   PortableServer::POA_ptr poa)
{
  /*
   * Incarnate a new Account
   */

  return new Account_impl;
}

void
AccountManager::etherealize (const PortableServer::ObjectId & oid,
			     PortableServer::POA_ptr poa,
			     PortableServer::Servant serv,
			     CORBA::Boolean cleanup_in_progress,
			     CORBA::Boolean remaining_activations)
{
  /*
   * If there are no remaining activations for that servant (which
   * actually could only happen with the MULTIPLE_ID policy, when
   * one servant can incarnate many objects), delete the account.
   */

  if (!remaining_activations) {
    delete serv;
  }
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
   * The RootPOA has the USE_ACTIVE_OBJECT_MAP_ONLY policy; to register
   * our ServantManager, we must create our own POA with the
   * USE_SERVANT_MANAGER policy
   */

  CORBA::PolicyList pl;
  pl.length(1);
  pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  PortableServer::POA_var mypoa = poa->create_POA ("MyPOA", mgr, pl);

  /*
   * Activate our ServantManager
   */

  AccountManager * am = new AccountManager;
  PortableServer::ServantManager_var amref = am->_this ();
  mypoa->set_servant_manager (amref);

  /*
   * Create a Bank
   */

  Bank_impl * micocash = new Bank_impl (mypoa);

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
   * Activate both POAs and start serving requests
   */

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached). This would call etherealize() for all
   * our accounts.
   */

  poa->destroy (TRUE, TRUE);
  delete am;
  delete micocash;

  return 0;
}
