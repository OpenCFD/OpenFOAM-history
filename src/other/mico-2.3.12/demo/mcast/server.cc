#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif
#include "hello.h"

static const char *serverid;

class Hello_impl : virtual public POA_HelloWorld {
public:
    char *hello ()
    {
	return CORBA::string_dup (serverid);
    }
};

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

    assert (argc == 2);

    serverid = argv[1];

    /*
     * Obtain a reference to the RootPOA and its Manager
     */

    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    /*
     * We create a new POA with the USER_ID and PERSISTENT policies so that
     * we can specify the whole object key.
     */

    CORBA::PolicyList pl;
    pl.length (2);
    pl[0] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
    pl[1] = poa->create_id_assignment_policy (PortableServer::USER_ID);
    PortableServer::POA_var mypoa =
	poa->create_POA ("Hello",
			 PortableServer::POAManager::_nil (),
			 pl);
    PortableServer::POAManager_var mymgr = mypoa->the_POAManager ();

    Hello_impl *h = new Hello_impl;

    /*
     * Activate
     */

    PortableServer::ObjectId_var hid =
	PortableServer::string_to_ObjectId ("Hello");
    mypoa->activate_object_with_id (hid.in(), h);

    /*
     * Run
     */

    mymgr->activate();
    mgr->activate();
    orb->run ();
    return 0;
}
