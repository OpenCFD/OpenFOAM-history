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
    : virtual public POA_HelloWorld,
      virtual public PortableServer::RefCountServantBase
{
public:
    ~HelloWorld_impl()
    {}

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
    for(int i = 0; i < 100000; i++) {
    try {
	CORBA::ORB_var orb = CORBA::ORB::_nil();
	PortableServer::POA_var poa = PortableServer::POA::_nil();
	PortableServer::POAManager_var mgr = PortableServer::POAManager::_nil();
	orb = CORBA::ORB_init(argc, argv);

	CORBA::Object_var poaobj = orb->resolve_initial_references("RootPOA");
	poa = PortableServer::POA::_narrow(poaobj);
	mgr = poa->the_POAManager();

	HelloWorld_impl* servant = new HelloWorld_impl;
	HelloWorld_var hello = servant->_this();
	mgr->activate();
	poa->destroy(TRUE, TRUE);

	orb->shutdown(TRUE);
	orb->destroy();
	servant->_remove_ref();
    } catch (CORBA::UserException& ex) {
	cout << "UserException caught: " << ex._repoid() << endl;
    } catch (CORBA::SystemException_catch& ex) {
	cout << "SystemException caught: " << ex._repoid() << endl;
    } catch (...) {
	cout << "... caught!" << endl;
    }
    }
    return 0;
}
