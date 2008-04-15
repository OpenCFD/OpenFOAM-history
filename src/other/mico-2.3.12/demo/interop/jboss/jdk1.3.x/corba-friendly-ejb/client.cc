#include <CORBA.h>
#include <coss/CosNaming.h>
#include "examples/CorbaFriendlySessionHome.h"


using namespace std;

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  // Acquire a reference to the Naming Service

  CORBA::Object_var nsobj =
    orb->resolve_initial_references ("NameService");

  CosNaming::NamingContext_var nc = 
    CosNaming::NamingContext::_narrow (nsobj);

  if (CORBA::is_nil (nc)) {
    cerr << "oops, I cannot access the Naming Service!" << endl;
    exit (1);
  }

  // Locate IOR for 'examples/MyCorbaFriendlySession'

  CosNaming::Name name;
  name.length (2);
  name[0].id = CORBA::string_dup ("examples");
  name[0].kind = CORBA::string_dup ("");
  name[1].id = CORBA::string_dup ("MyCorbaFriendlySession");
  name[1].kind = CORBA::string_dup ("");

  CORBA::Object_var obj;

  cout << "Looking up examples/MyCorbaFriendlySession..." << flush;

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

  // Got an IOR. It should be an examples::CorbaFriendlySessionHome IOR.

  examples::CorbaFriendlySessionHome_var home = 
     examples::CorbaFriendlySessionHome::_narrow (obj);
  assert (!CORBA::is_nil (home));
  
  // Use home reference to create a CorbaFriendlySession instance.

  examples::CorbaFriendlySession_var session = home->create();
  assert (!CORBA::is_nil (session));

  // Call operations on the session object

  examples::corba::DataObject_var data_obj = new examples::corba::DataObject();
  data_obj->i = 7;
  data_obj->s = CORBA::string_dup ("Hello, world!");

  org::omg::boxedIDL::examples::corba::DataObject_var boxed_data_obj =
      new org::omg::boxedIDL::examples::corba::DataObject(data_obj);

  cout << "Before echo call: \t\t" << boxed_data_obj->i();
  cout << "\t" << boxed_data_obj->s() << endl;
  
  boxed_data_obj = session->echo(boxed_data_obj);

  cout << "After echo call: \t\t" << boxed_data_obj->i();
  cout << "\t" << boxed_data_obj->s() << endl;
  
  ::org::omg::boxedRMI::org::omg::boxedIDL::examples::corba::seq1_DataObject_var boxed_seq_data_obj =
      new ::org::omg::boxedRMI::org::omg::boxedIDL::examples::corba::seq1_DataObject;
  boxed_seq_data_obj->length(4);
  for (unsigned j = 0; j < boxed_seq_data_obj->length(); j++) 
      boxed_seq_data_obj->_value()[j] = boxed_data_obj;
  
  cout << "Before echoSequence call: \t";
  for (unsigned j = 0; j < boxed_seq_data_obj->length(); j++) {
      cout << boxed_seq_data_obj->_value()[j]->i() << "\t";
      cout << boxed_seq_data_obj->_value()[j]->s() << endl;
      if (j < boxed_seq_data_obj->length() - 1)
	  cout << "\t\t\t\t";
  }

  boxed_seq_data_obj = session->echoSequence(boxed_seq_data_obj);

  cout << "After echoSequence call: \t";
  for (unsigned j = 0; j < boxed_seq_data_obj->length(); j++) {
      cout << boxed_seq_data_obj->_value()[j]->i();
      cout << "\t" << boxed_seq_data_obj->_value()[j]->s() << endl;
      if (j < boxed_seq_data_obj->length() - 1)
	  cout << "\t\t\t\t";
  }

  // Destroy session.

  session->remove();

  return 0;
}
