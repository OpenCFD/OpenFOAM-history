/*
 * In this example, we create Account objects in a persistent POA.
 * When the persistent POA is destructed, the objects are "etherea-
 * lized," meaning we open a file and write the Account's balance
 * to disk.
 */

#include "account.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif


using namespace std;

CORBA::ORB_var orb;
CORBA::Boolean finished = FALSE;

/*
 * Mapping of Servants to Accounts
 */

class Account_impl;
typedef map<PortableServer::Servant,
  Account_impl *,
  less<PortableServer::Servant> > ServantMap;
ServantMap svmap;

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
  Account_ptr create (const char *);
  void shutdown (void);

private:
  PortableServer::POA_var mypoa;
};

Bank_impl::Bank_impl (PortableServer::POA_ptr _poa)
{
  mypoa = PortableServer::POA::_duplicate (_poa);
}

void
Bank_impl::shutdown (void)
{
  /*
   * Explicit server shutdown.
   */
    finished = TRUE;
}

Account_ptr
Bank_impl::create (const char * name)
{
  /*
   * We don't create an account, but only a reference to a "virtual"
   * object. The Servant Manager is responsible for incarnating the
   * account on request.
   */

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (name);

  CORBA::Object_var obj =
    mypoa->create_reference_with_id (*oid, "IDL:Account:1.0");
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
   * See if Account exists (from a prior incarnation)
   */

  CORBA::String_var name = PortableServer::ObjectId_to_string (oid);
  Account_impl * account = new Account_impl;

  ifstream in ((const char *) name);
  if (in.rdbuf()->is_open()) {
    CORBA::Long amount;
    in >> amount;
    if (amount > 0) {
      account->deposit (amount);
    }
    else {
      account->withdraw (-amount);
    }
  }

  svmap[account] = account;
  return account;
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
    ServantMap::iterator it = svmap.find (serv);
    assert (it != svmap.end());

    /*
     * Save object's state to disk
     */

    CORBA::String_var name = PortableServer::ObjectId_to_string (oid);
    ofstream out ((const char *) name);
    out << (*it).second->balance() << endl;

    /*
     * Clean up
     */

    svmap.erase (it);
    delete serv;
  }
}

int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */

  orb = CORBA::ORB_init (argc, argv);

  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  /*
   * The RootPOA has the TRANSIENT and USE_ACTIVE_OBJECT_MAP_ONLY
   * policies. We create a PERSISTENT POA for our account objects
   * and choose the USE_SERVANT_MANAGER policy. The ServantManager
   * will then etherealize and incarnate Account objects.
   *
   * We use a different POAManager here; we must be able to call
   * the ServantManager, which is activated in the RootPOA, while
   * it is still in the Active state, even when our new POA is
   * already deactivated.
   */

  CORBA::PolicyList pl;
  pl.length(3);
  pl[0] = poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
  pl[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
  pl[2] = poa->create_id_assignment_policy (PortableServer::USER_ID);
  PortableServer::POA_var accpoa = poa->create_POA ("Accounts", PortableServer::POAManager::_nil(), pl);
  PortableServer::POAManager_var accmgr = accpoa->the_POAManager();

  /*
   * Activate our ServantManager
   */

  AccountManager * am = new AccountManager;
  PortableServer::ServantManager_var amref = am->_this ();
  accpoa->set_servant_manager (amref);

  /*
   * We then create another Persistent POA which we use for our Bank.
   */

  CORBA::PolicyList pl2;
  pl2.length(2);
  pl2[0] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
  pl2[1] = poa->create_id_assignment_policy (PortableServer::USER_ID);
  PortableServer::POA_var bankpoa = poa->create_POA ("Bank", mgr, pl2);

  /*
   * Create and activate a Bank
   */

  Bank_impl * micocash = new Bank_impl (accpoa);
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("Bank");
  bankpoa->activate_object_with_id (*oid, micocash);

  /*
   * Write reference to file
   */

  ofstream of ("Bank.ref");
  CORBA::Object_var ref = bankpoa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();

  /*
   * Activate POAs and start serving requests
   */

  cout << "Running." << endl;
  mgr->activate ();
  accmgr->activate ();
  while (!finished) {
      orb->perform_work();
  }

  /*
   * Shutdown. This calls etherealize() for all our accounts.
   */

  poa->destroy (TRUE, TRUE);
  orb->shutdown(TRUE);
  delete am;
  delete micocash;

  return 0;
}
