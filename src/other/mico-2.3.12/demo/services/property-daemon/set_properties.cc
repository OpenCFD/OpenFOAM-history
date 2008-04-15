
#include <CORBA-SMALL.h>
#include <coss/CosNaming.h>
#include <coss/PropertyService.h>

#include "bank.h"


using namespace std;

int main(int argc, char *argv [])
{

  /*
   * ORB initialization
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

  /*
   * Use the NameService to find the PropertyService Demon (propertyd);
   */

  CORBA::Object_var nobj =
    orb->resolve_initial_references ("NameService");
  assert (! CORBA::is_nil (nobj));
  
  CosNaming::NamingContext_var nc = CosNaming::NamingContext::_narrow (nobj);
  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("PropertySetFactory");
  name[0].kind = CORBA::string_dup ("");
  
  CORBA::Object_var obj;

#ifdef HAVE_EXCEPTIONS
  try {
    obj = nc->resolve (name);
  } catch (...) {
      cerr << "NameService raised exception!" << endl;
      exit (1);
  }
#else
  obj = nc->resolve (name);
#endif

  PropertyService::PropertySetFactory_var property_d =
      PropertyService::PropertySetFactory::_narrow (obj);
 
  /*
   * Create PropertySet
   */

  PropertyService::PropertySet_var property_set = 
      property_d->create_propertyset ();

  /*
   * Define a single Property.
   */

  CORBA::Any a;
  a <<= (const char *) "Bank of Cartoons"; 

  property_set->define_property ("Company", a);

  /*
   * Define a set of Properties. 
   */

  Bank::Account account1;
  account1.first_name = (const char *) "Donald";
  account1.name = (const char *) "Duck";
  account1.balance = 150;

  Bank::Account account2;
  account2.first_name = (const char *) "Dagobert";
  account2.name = (const char *) "Duck";
  account2.balance = 25000;

  PropertyService::Properties accounts (2);
  accounts.length (2);
  accounts[0].property_name = CORBA::string_dup ("Account 1");
  accounts[0].property_value <<= account1;
  accounts[1].property_name = CORBA::string_dup ("Account 2");
  accounts[1].property_value <<= account2; 

  property_set->define_properties (accounts);

  /*
   * Register PropertySet
   */

  CosNaming::Name new_entry;
  new_entry.length (1);
  new_entry[0].id = CORBA::string_dup ("CartoonsPropertySet");
  new_entry[0].kind = CORBA::string_dup ("");

#ifdef HAVE_EXCEPTIONS
  try {
    nc->bind (new_entry, property_set);  
  } catch (...) {
      cerr << "NameService raised exception!" << endl;
      exit (1);
  }
#else
  nc->bind (new_entry, property_set);  
#endif

  return 0;
}
