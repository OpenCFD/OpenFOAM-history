
#include "bench.h"
#include <CORBA.h>
#include <fstream>


using namespace std;

CORBA::ORB_ptr orb = NULL;
CORBA::Long xcount = 0;

class bench_impl
    : public virtual POA_bench
{
public:
    void
    perform()
    {
	xcount++;
	if (xcount > 100)
	    exit(0);
    }
};

int
main (int argc, char* argv[])
{
    //  cerr << "main thread: " << MICOMT::Thread::self() << endl;
    orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    CORBA::Object_ptr obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_ptr poa = PortableServer::POA::_narrow (obj);
    CORBA::release (obj);

    bench_impl* servant = new bench_impl;

    poa->activate_object (servant);
    CORBA::Object_ptr ref = poa->servant_to_reference (servant);
  
    CORBA::String_var ior = orb->object_to_string (ref);
    ofstream out("ref");
    out << ior << endl;
    out.close();

    CORBA::release (ref);

    PortableServer::POAManager_ptr manager = poa->the_POAManager ();
    manager->activate ();

    // exit is by exit(0) call in perform operation above
    for (;;) {
	orb->perform_work();
    }
    CORBA::release (manager);
    CORBA::release (poa);
    CORBA::release (orb);

    return 0;
}
