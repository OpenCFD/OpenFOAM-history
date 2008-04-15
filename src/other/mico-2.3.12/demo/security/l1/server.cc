/*
 * A Bank factory that creates Account objects
 */

#include "account.h"
#include <mico/security/security.h>
#include <mico/security/securitylevel1.h>


using namespace std;

CORBA::ORB_var orb;
CORBA::Object_var securitycurrent;
SecurityLevel1::Current_var seccur;

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
  Security::ExtensibleFamily fam;
  fam.family_definer = 0;
  fam.family = 1;
  Security::AttributeType at;
  at.attribute_family = fam;
  at.attribute_type = Security::AccessId;
  Security::AttributeTypeList atl;
  atl.length(1);
  atl[0]=at; 
  
  Security::AttributeList_var al =  seccur->get_attributes( atl );
  cout << "Received  " 
       << (*al).length() << " attributes\n";
  for ( int ctr = 0; ctr < (*al).length(); ctr++) {
    cout  
      << (*al)[ctr].attribute_type.attribute_family.family << " "
      << (*al)[ctr].attribute_type.attribute_type << " "
      <<  &(*al)[ctr].value[0] << " "
      << &(*al)[ctr].defining_authority[0]
      << endl;
  }
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
  cout << "Start Bank server\n";
  orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Obtain a reference to the RootPOA and its Manager
   */
  PortableServer::POA_var poa;
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  // Get SecurityCurrent
  securitycurrent = orb->resolve_initial_references ("SecurityCurrent");
  cout << "narrow to SecurityLevel1Server::Current\n";
  seccur = SecurityLevel1::Current::_narrow(securitycurrent);
  assert (!CORBA::is_nil (seccur));

  /*
   * Create a Bank
   */

  Bank_impl * micocash = new Bank_impl;

  /*
   * Activate the Bank
   */

  PortableServer::ObjectId_var oid = poa->activate_object (micocash);

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
