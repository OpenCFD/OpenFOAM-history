#include "account.h"


using namespace std;

CORBA::ORB_var orb;
CORBA::Object_var securitycurrent;
SecurityLevel1::Current_var seccur;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  // Get SecurityCurrent
  securitycurrent = orb->resolve_initial_references ("SecurityCurrent");
  
  cout << "narrow to SecurityLevel1Server::Current\n";
  seccur = SecurityLevel1::Current::_narrow(securitycurrent);
  assert (!CORBA::is_nil (seccur));


  /*
   * Connect to the Bank
   */

  CORBA::Object_var obj = orb->bind ("IDL:Bank:1.0");
  if (CORBA::is_nil (obj)) {
    cout << "oops: bind to Bank failed" << endl;
    exit (1);
  }
  Bank_var bank = Bank::_narrow (obj);
  assert (!CORBA::is_nil (bank));

  /*
   * Get and print attributes of server
   */
	
  Security::ExtensibleFamily fam;
  fam.family_definer = 0;
  fam.family = 1;
  Security::AttributeType at;
  at.attribute_family = fam;
  at.attribute_type = Security::AccessId; 
  Security::AttributeTypeList atl;
  atl.length(1);
  atl[0]=at;
  cout << "get_attributes\n";
  Security::AttributeList_var al =  seccur->get_target_attributes( atl, bank );
  
  cout << "Received " 
       << (*al).length() << " attributes\n";

  for (CORBA::ULong ctr = 0; ctr < (*al).length(); ctr++) {
    cout  
      << (*al)[ctr].attribute_type.attribute_family.family << " "
      << (*al)[ctr].attribute_type.attribute_type << " "
      <<  &(*al)[ctr].value[0] << " "
      << &(*al)[ctr].defining_authority[0]
      << endl;
  }


	/*
   * Open an account
   */

  Account_var account = bank->create ();
  if (CORBA::is_nil (account)) {
    cout << "oops: account is nil" << endl;
    exit (1);
  }

  /*
   * Deposit and withdraw some money
   */
  cout << "deposit - 700\n";
  account->deposit (700);
  cout << "withdraw - 450\n";
  account->withdraw (450);
  cout << "Balance is " << account->balance () << "." << endl;
  return 0;
}
