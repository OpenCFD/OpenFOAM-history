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

CORBA::ORB_ptr g_orb = CORBA::ORB::_nil();
CORBA::Boolean g_finished = FALSE;

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
    g_finished = TRUE;
}

class HelloManager
    : public virtual POA_PortableServer::ServantActivator
{
public:
    PortableServer::Servant
    incarnate(const PortableServer::ObjectId &, PortableServer::POA_ptr);

    void
    etherealize(const PortableServer::ObjectId &,
		PortableServer::POA_ptr,
		PortableServer::Servant,
		CORBA::Boolean, CORBA::Boolean);
};

PortableServer::Servant
HelloManager::incarnate
(const PortableServer::ObjectId& oid,
 PortableServer::POA_ptr poa)
{
    cout << "HelloManager::incarnate" << endl;
    CORBA::String_var str = PortableServer::ObjectId_to_string(oid);
    cout << "object id: `" << str.in() << "'" << endl;
    HelloWorld_impl* hello = new HelloWorld_impl;
    return hello;
}

void
HelloManager::etherealize
(const PortableServer::ObjectId& oid,
 PortableServer::POA_ptr poa,
 PortableServer::Servant serv,
 CORBA::Boolean cleanup,
 CORBA::Boolean remain)
{
    cout << "HelloManager::etherealize" << endl;
    CORBA::String_var str = PortableServer::ObjectId_to_string(oid);
    cout << "object id: `" << str.in() << "'" << endl;
}

int
main (int argc, char *argv[])
{
    try {
	g_orb = CORBA::ORB_init(argc, argv);
	CORBA::Object_var poaobj = g_orb->resolve_initial_references("RootPOA");
	PortableServer::POA_var poa = PortableServer::POA::_narrow(poaobj);
	PortableServer::POAManager_var mgr = poa->the_POAManager();

	CORBA::PolicyList pl;
	pl.length(3);
	pl[0] = poa->create_request_processing_policy(PortableServer::USE_SERVANT_MANAGER);
	pl[1] = poa->create_id_assignment_policy(PortableServer::USER_ID);
	pl[2] = poa->create_lifespan_policy(PortableServer::PERSISTENT);

	PortableServer::POA_var hello_poa = poa->create_POA
	    ("HelloPOA", PortableServer::POAManager::_nil(), pl);
	PortableServer::POAManager_var hello_mgr = hello_poa->the_POAManager();

	//HelloManager* hm = new HelloManager(hello_poa);
	//HelloManager* hm = new HelloManager(poa);
	HelloManager* hm = new HelloManager;
	//cout << "hm: " << (void*)hm << endl;
	PortableServer::ServantManager_var shm = hm->_this();
	//PortableServer::ObjectId_var hm_oid = hello_poa->activate_object(hm);
	//CORBA::Object_var hm_obj = hello_poa->id_to_reference(hm_oid);
	//PortableServer::ServantManager_var shm = PortableServer::ServantManager::_narrow(hm_obj);
	//cout << "shm (var): " << (void*)shm << endl;
	//cout << "shm (.in): " << (void*)shm.in() << endl;
	hello_poa->set_servant_manager(shm);
	//cout << "->sm(): " << hello_poa->get_servant_manager() << endl;
	//hello_poa->set_servant_manager(hm);

	PortableServer::ObjectId_var hello_oid
	    = PortableServer::string_to_ObjectId("HelloWorld");
	CORBA::Object_var obj
	    = hello_poa->create_reference_with_id(hello_oid, "IDL:HelloWorld:1.0");
	HelloWorld_ptr hello = HelloWorld::_narrow(obj);
	assert(!CORBA::is_nil(hello));
	ofstream of ("hello.ref");
	CORBA::String_var str = g_orb->object_to_string(hello);
	of << str.in() << endl;
	of.close();

	cout << "Running." << endl;

	mgr->activate();
	hello_mgr->activate();
	//g_orb->run();
	while(!g_finished) {
	    g_orb->perform_work();
	}
	//poa->destroy(TRUE, TRUE);
	cout << "orb->shutdown();" << endl;
	g_orb->shutdown(TRUE);

	cout << "server finished" << endl;
    } catch (CORBA::UserException& ex) {
	cout << "UserException caught: " << ex._repoid() << endl;
    } catch (CORBA::SystemException_catch& ex) {
	cout << "SystemException caught: " << ex._repoid() << endl;
    } catch (...) {
	cout << "... caught!" << endl;
    }
    return 0;
}
