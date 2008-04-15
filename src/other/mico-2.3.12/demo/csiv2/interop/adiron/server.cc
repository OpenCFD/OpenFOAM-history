
#include "InterOpTest.h"

#include <fstream>


using namespace std;

class Peter_impl
    : virtual public POA_InterOpTest::Peter
{
public:
    virtual char*
    say_hello(const char*);
};

char*
Peter_impl::say_hello(const char* msg)
{
    cout << "got msg: `" << msg << "'" << endl;
    return CORBA::string_dup("O/S Peter_impl::say_hello");
}

class Johnson_impl
    : virtual public POA_InterOpTest::Johnson,
      virtual public Peter_impl
{
    virtual char*
    say_hello_from_you(InterOpTest::Peter_ptr pete, CORBA::Long times);

    virtual char*
    say_hello_from_me(InterOpTest::Peter_ptr pete, CORBA::Long times);

    virtual char*
    say_hello_from_user_password
    (InterOpTest::Peter_ptr pete,
     const char* targetname,
     const char* username,
     const char* password,
     CORBA::Long times);
};

char*
Johnson_impl::say_hello_from_you(InterOpTest::Peter_ptr pete, CORBA::Long times)
{
    if (CORBA::is_nil(pete)) {
	cout << "pete is nil" << endl;
	return CORBA::string_dup("");
    }
    if (pete->_ior() == NULL) {
	cout << "pete's ior is nil" << endl;
	return CORBA::string_dup("");
    }
    cout << "pete's IOR:" << endl;
    pete->_ior()->print(cout);
    return CORBA::string_dup("");
}

char*
Johnson_impl::say_hello_from_me(InterOpTest::Peter_ptr pete, CORBA::Long times)
{
    if (CORBA::is_nil(pete)) {
	cout << "pete is nil" << endl;
	return CORBA::string_dup("");
    }
    if (pete->_ior() == NULL) {
	cout << "pete's ior is nil" << endl;
	return CORBA::string_dup("");
    }
    cout << "pete's IOR:" << endl;
    pete->_ior()->print(cout);
    cout << endl;
    cout << "calling pete" << endl;
    cout << endl;
    CORBA::String_var msg = pete->say_hello("Hello Pete!");
    return msg._retn();
}

char*
Johnson_impl::say_hello_from_user_password
(InterOpTest::Peter_ptr pete,
 const char* targetname,
 const char* username,
 const char* password,
 CORBA::Long times)
{
    if (CORBA::is_nil(pete)) {
	cout << "pete is nil" << endl;
	return CORBA::string_dup("");
    }
    if (pete->_ior() == NULL) {
	cout << "pete's ior is nil" << endl;
	return CORBA::string_dup("");
    }
    cout << "pete's IOR:" << endl;
    pete->_ior()->print(cout);
    cout << endl;
    cout << "calling pete" << endl;
    cout << endl;
    CORBA::String_var msg = pete->say_hello("Hello Pete!");
    return msg._retn();
}

int
main (int argc, char *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  Peter_impl* peter = new Peter_impl;
  Johnson_impl* johnson = new Johnson_impl;

  PortableServer::ObjectId_var oid = poa->activate_object(peter);
  ofstream of ("peter.ref");
  CORBA::Object_var ref = poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();

  oid = poa->activate_object(johnson);
  ofstream of2("johnson.ref");
  ref = poa->id_to_reference(oid.in());
  str = orb->object_to_string(ref.in());
  of2 << str.in() << endl;
  of2.close ();

  cout << "Running." << endl;

  mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  poa->destroy (TRUE, TRUE);
  delete peter;

  return 0;
}
