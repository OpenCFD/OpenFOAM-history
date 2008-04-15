//
// Test for etherealize/incarnate
//

#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS


using namespace std;

class HelloWorld_impl
    : virtual public POA_HelloWorld
{
public:
    void hello();
};

void
HelloWorld_impl::hello()
{
    cout << "Hello World" << endl;
}

int
main (int argc, char *argv[])
{
    try {
	CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv);
	CORBA::Object_var poaobj = orb->resolve_initial_references("RootPOA");
	PortableServer::POA_var poa = PortableServer::POA::_narrow(poaobj);
	PortableServer::POAManager_var mgr = poa->the_POAManager();

	CORBA::PolicyList pl;
	pl.length(3);
	pl[0] = poa->create_request_processing_policy(PortableServer::USE_DEFAULT_SERVANT);
	pl[1] = poa->create_id_assignment_policy(PortableServer::USER_ID);
	pl[2] = poa->create_servant_retention_policy(PortableServer::NON_RETAIN);

	PortableServer::POA_var hello_poa = poa->create_POA
	    ("HelloPOA", PortableServer::POAManager::_nil(), pl);
	PortableServer::POAManager_var hello_mgr = hello_poa->the_POAManager();

	HelloWorld_impl* servant = new HelloWorld_impl;
	hello_poa->set_servant(servant);

  	PortableServer::ObjectId_var hello_oid
  	    = PortableServer::string_to_ObjectId("HelloWorld");
  	CORBA::Object_var obj
  	    = hello_poa->create_reference_with_id(hello_oid, "IDL:HelloWorld:1.0");
	HelloWorld_ptr hello = HelloWorld::_narrow(obj);
	assert(!CORBA::is_nil(hello));
	ofstream of ("hello.ref");
	CORBA::String_var str = orb->object_to_string(hello);
	of << str.in() << endl;
	of.close();

	cout << "Running." << endl;

	mgr->activate();
	hello_mgr->activate();
	orb->run();
    } catch (CORBA::UserException& ex) {
	cout << "UserException caught: " << ex._repoid() << endl;
    } catch (CORBA::SystemException_catch& ex) {
	cout << "SystemException caught: " << ex._repoid() << endl;
    } catch (...) {
	cout << "... caught!" << endl;
    }
    return 0;
}
