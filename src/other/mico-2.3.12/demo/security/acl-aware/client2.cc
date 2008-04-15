#include "account.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


using namespace std;

// client is owner
int
main (int argc, char *argv[])
{
  cout << "Owner/Wife started" << endl;
  
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * IOR is in Bank.ref in the local directory
   */

  char pwd[256], uri[300];
  sprintf (uri, "file://%s/Bank.ref", getcwd(pwd, 256));

  /*
   * Connect to the Bank
   */

  CORBA::Object_var obj = orb->string_to_object (uri);
  Bank_var bank = Bank::_narrow (obj);

  if (CORBA::is_nil (bank)) {
    cout << "oops: could not locate Bank" << endl;
    exit (1);
  }

  /*
   * Open an account
   */

  Account_var account;
  try {
    account = bank->open ();
  } catch (...) {
  	cout << "Couldn't open an account!" << endl;
  }

  if (CORBA::is_nil (account)) {
    cout << "oops: account is nil" << endl;
    exit (1);
  }

  /*
   * Deposit and withdraw some money
   */

  try {
  account->deposit (700);
  } catch (...) {
  	cout << "Couldn't deposit!" << endl;
  }
  try {
  account->withdraw (450);
  } catch (...) {
  	cout << "Couldn't withdraw!" << endl;
  }


  cout << "Balance is " << account->balance() << "." << endl;

  return 0;
}
