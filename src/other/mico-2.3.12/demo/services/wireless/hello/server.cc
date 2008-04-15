/*
 * A simple "Hello World" example that uses the POA
 */

#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

using namespace std;

/*
 * Hello World implementation inherits the POA skeleton class
 */

class HelloWorld_impl : virtual public POA_HelloWorld
{
public:
    char *hello (const char *name);
    void error (const char *name);
};

char *
HelloWorld_impl::hello (const char *name)
{
    char *s = CORBA::string_alloc(7 + strlen(name));
    strcpy(s, "Hello, ");
    strcat(s, name);
    cout << endl << s << endl << endl;
    string str;
    getline(cin, str);
    return s;
}

void
HelloWorld_impl::error (const char *name)
{
    cout << endl << "Raising exception: " << name << endl << endl;
//    string str;
//    getline(cin, str);
    throw HelloWorld::Goodbye(name);
}

int
main (int argc, char *argv[])
{
  /*
   * Initialize the ORB
   */

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  /*
   * Obtain a reference to the RootPOA and its Manager
   */

  CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
  PortableServer::POAManager_var mgr = poa->the_POAManager();

  /*
   * Create a USER_ID POA so that the actual reference is constant
   */

  CORBA::PolicyList plcs;
  plcs.length(2);
  plcs[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
  plcs[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
  PortableServer::POA_var hello_poa =
    poa->create_POA ("HelloWorld", PortableServer::POAManager::_nil(), plcs);
  PortableServer::POAManager_var hello_mgr = hello_poa->the_POAManager();

  /*
   * Create an Object Id for our server
   */

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ("HelloWorld");

  /*
   * Create a Hello World object
   */

  HelloWorld_impl * hello = new HelloWorld_impl;

  /*
   * Activate the Servant
   */

  hello_poa->activate_object_with_id (oid, hello);

  /*
   * Write reference to file
   */

  ofstream of ("hello.ref");
  CORBA::Object_var ref = hello_poa->id_to_reference (oid.in());
  CORBA::String_var str = orb->object_to_string (ref.in());
  of << str.in() << endl;
  of.close ();

  /*
   * Activate the POA and start serving requests
   */

  printf ("Running.\n");

  hello_mgr->activate ();
  orb->run();

  /*
   * Shutdown (never reached)
   */

  hello_poa->destroy (TRUE, TRUE);
  poa->destroy (TRUE, TRUE);
  delete hello;

  return 0;
}
