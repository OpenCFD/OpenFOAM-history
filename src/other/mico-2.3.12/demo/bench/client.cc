
#define USE_POA
//#define NESTED

#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS 
#include "bench.h"
#include "bench_impl.h"
#include <mico/os-misc.h>


using namespace std;

int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
#if defined(USE_POA)
    CORBA::Object_var poaobj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (poaobj);

    Bench_impl3 *mybench_serv = new Bench_impl3;
    PortableServer::ObjectId_var oid = poa->activate_object (mybench_serv);
    Bench_var mybench = mybench_serv->_this();
    poa->the_POAManager()->activate();
#else
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");
    Bench_ptr mybench = new Bench_impl;
    boa->impl_is_ready (CORBA::ImplementationDef::_nil());
#endif

    assert (argc == 2);
    CORBA::Object_var obj = orb->bind ("IDL:Bench:1.0", argv[1]);
    if (CORBA::is_nil (obj)) {
	cout << "cannot bind to " << argv[1] << endl;
	return 1;
    }
    Bench_var bench = Bench::_narrow (obj);

#ifdef NESTED
    mybench->connect (bench, 2500);
    bench->connect (mybench, 2500);
#endif

    OSMisc::TimeVal t1, t2;

    bench->sync();

    t1 = OSMisc::gettime();
#ifdef NESTED
    bench->g ();
#else
    for (int i = 0; i < 30000; ++i) {
	bench->f ();
    }
    bench->sync ();
#endif
    t2 = OSMisc::gettime();

    cout << (double)((t2.tv_sec-t1.tv_sec)*1000 + (t2.tv_usec-t1.tv_usec)/1000)/30000 << " ms per call" << endl;
    return 0;
}
