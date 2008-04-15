#include <CORBA.h>
#include <coss/CosNaming.h>
#include "examples/ConverterHome.h"


using namespace std;

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
   * Locate IOR for 'examples/MyConverter'
   */

  CosNaming::Name name;
  name.length(2);
  name[0].id = CORBA::string_dup("examples");
  name[0].kind = CORBA::string_dup("");
  name[1].id = CORBA::string_dup("MyConverter");
  name[1].kind = CORBA::string_dup("");
  
  /*
   * try to find that node in the Naming Service tree
   */

  CORBA::Object_var obj;

  cout << "Looking up examples/MyConverter..." << flush;

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

  cout << " Done." << endl;

  assert (!CORBA::is_nil (obj));

  examples::ConverterHome_var home = examples::ConverterHome::_narrow (obj);
  assert (!CORBA::is_nil (home));
  
  examples::Converter_var converter = home->create();
  assert (!CORBA::is_nil (converter));
  
  CORBA::Long result = converter->dollar_to_euro (100);
  cout << "Result: " << result << endl;

  converter->remove();
  
  return 0;
}
