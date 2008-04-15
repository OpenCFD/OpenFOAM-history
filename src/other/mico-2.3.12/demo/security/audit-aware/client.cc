// The client does not care about security. There is only one difference to a "normal" client:
// The server now can deny the invocation of an operation and raise an exception. A normal client
// would just crash now. This client catches the exception and continues.

#include "account.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


using namespace std;

int
main (int argc, char *argv[])
{

  cout << "Manager started" << endl;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * IOR is in Bank.ref in the local directory
   */

  char pwd[256], uri[300];
  sprintf (uri, "file://%s/Bank.ref", getcwd(pwd, 256));

  /*
   * Connect to the Bank1
   */

  CORBA::Object_var obj = orb->string_to_object (uri);
  Bank_var bank = Bank::_narrow (obj);

  if (CORBA::is_nil (bank)) {
    cout << "oops: could not locate Bank" << endl;
    exit (1);
  }

  /*
   * Create an account
   */
  Account_var account;
  try {
    account = bank->create ();
  } catch (...) {
  	cout << "Couldn't create an account!" << endl;
  }

  if (CORBA::is_nil (account)) {
    cout << "oops: account is nil" << endl;
    exit (1);
  }
  cout << "Manager: Account OK\n";
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
  CORBA::Long bal = -1;
  try {
    bal = account->balance ();
  } catch (...) {
  	cout << "Couldn't get balance!" << endl;
  }
  
  cout << "Balance is " <<  bal << "." << endl;
  
  return 0;
}
