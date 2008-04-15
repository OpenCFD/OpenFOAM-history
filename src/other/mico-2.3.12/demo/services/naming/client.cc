#include <CORBA.h>
#include <coss/CosNaming.h>
#include "account.h"


using namespace std;

int
main (int argc, char *argv[])
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * In demo/poa/account-1, the object reference comes from a file via
   * the "file://" object URL. Here, we retrieve the reference from the
   * Naming Service
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
   * try to find that node in the Naming Service tree
   */

  CORBA::Object_var obj;

  cout << "Looking up Bank ... " << flush;

#ifdef HAVE_EXCEPTIONS
  try {
    obj = nc->resolve (name);
  }
  catch (CosNaming::NamingContext::NotFound &exc) {
    cout << "NotFound exception." << endl;
    exit (1);
  }
  catch (CosNaming::NamingContext::CannotProceed &exc) {
    cout << "CannotProceed exception." << endl;
    exit (1);
  }
  catch (CosNaming::NamingContext::InvalidName &exc) {
    cout << "InvalidName exception." << endl;
    exit (1);
  }
#else
  obj = nc->resolve (name);
#endif

  cout << "done." << endl;

  /*
   * The Naming Service returns a generic reference as a CORBA::Object
   * We need to narrow this to the desired type
   */

  Bank_var bank = Bank::_narrow (obj);

  /*
   * Open an account
   */

  Account_var account = bank->create ();

  if (CORBA::is_nil (account)) {
    printf ("oops: account is nil\n");
    exit (1);
  }

  /*
   * Deposit and withdraw some money
   */

  account->deposit (700);
  account->withdraw (450);

  printf ("Balance is %ld.\n", account->balance ());

  return 0;
}
