/*
 * Demo application for the Dublin OMG meeting
 */

#include "demo.h"
#include <fstream>
#include <unistd.h>
#include <ctime>

using namespace std;

class Demo_impl : virtual public POA_Demo
{
public:
    char *date (CORBA::Long timeout);
    Demo_impl (Demo_ptr);
private:
    Demo_ptr _demo;
};

char *
Demo_impl::date (CORBA::Long timeout)
{
    cout << endl << "Invoking with " << timeout << endl;
    CORBA::String_var res = _demo->date(timeout);
    cout << "Got " << res << endl;
    return res._retn();
}

Demo_impl::Demo_impl (Demo_ptr demo)
{
    _demo = demo;
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

    char pwd[256], uri[300];
    sprintf (uri, "file://%s/demo.ref", getcwd(pwd, 256));

    CORBA::Object_var obj = orb->string_to_object (uri);
    Demo_var real_demo = Demo::_narrow (obj);

    Demo_impl *demo = new Demo_impl(real_demo);

    demo_poa->activate_object_with_id (oid, demo);

    ofstream of ("proxy.ior");
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
