#include <CORBA.h>
#include <coss/CosNaming.h>
#include "ConverterHome.h"

int
main (int argc, char *argv[])
{
  // ORB initialization
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

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
   * Locate IOR for 'MyConverter'
   */

  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("MyConverter");
  name[0].kind = CORBA::string_dup ("");
  
  /*
   * try to find that node in the Naming Service tree
   */

  CORBA::Object_var obj;

  cout << "Looking up MyConverter ... " << flush;

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

  cout << "done." << endl;

  assert (!CORBA::is_nil (obj));

  ConverterHome_var home = ConverterHome::_narrow (obj);
  assert (!CORBA::is_nil (home));
  
  Converter_var converter = home->create();
  assert (!CORBA::is_nil (converter));
  
  CORBA::Long result = converter->dollar_to_euro (100);
  cout << "Result: " << result << endl;
  
  return 0;
}
