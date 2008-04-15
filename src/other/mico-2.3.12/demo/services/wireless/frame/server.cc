/*
 * Demo application for the Dublin OMG meeting
 */

#include "demo.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#include <ctime>
#else
#include <fstream.h>
#include <time.h>
#endif
#include <unistd.h>

using namespace std;

class Demo_impl : virtual public POA_Demo
{
public:
    char *date (CORBA::Long timeout);
};

char *
Demo_impl::date (CORBA::Long timeout)
{
    static const CORBA::ULong tlen = 20UL;
    CORBA::String_var ret = CORBA::string_alloc(tlen);
    time_t t;
    time(&t);
    struct tm *trec = localtime(&t);
    strftime(ret, tlen, "%Y-%m-%dT%H:%M:%S", trec);
    cout << endl << "Invocation of date() at " << ret << endl;
    sleep(timeout);
    time(&t);
    trec = localtime(&t);
    strftime(ret, tlen, "%Y-%m-%dT%H:%M:%S", trec);
    cout << "Returning " << ret << endl << endl;
    return ret._retn();
}

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);
    PortableServer::POAManager_var mgr = poa->the_POAManager();

    CORBA::PolicyList plcs;
    plcs.length(2);
    plcs[0] = poa->create_id_assignment_policy (PortableServer::USER_ID);
    plcs[1] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
    PortableServer::POA_var demo_poa =
	poa->create_POA ("Demo", PortableServer::POAManager::_nil(), plcs);
    PortableServer::POAManager_var demo_mgr = demo_poa->the_POAManager();

    PortableServer::ObjectId_var oid =
	PortableServer::string_to_ObjectId ("Demo");

    Demo_impl *demo = new Demo_impl;

    demo_poa->activate_object_with_id (oid, demo);

    ofstream of ("demo.ior");
    CORBA::Object_var ref = demo_poa->id_to_reference (oid.in());
    CORBA::String_var str = orb->object_to_string (ref.in());
    of << str.in() << endl;
    of.close ();

    demo_mgr->activate ();
    orb->run();

    demo_poa->destroy (TRUE, TRUE);
    poa->destroy (TRUE, TRUE);
    delete demo;

    return 0;
}
