
#define USE_POA

#include <CORBA.h>
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS
#include "bench.h"
#include "bench_impl.h"


int
main (int argc, char *argv[])
{
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "mico-local-orb");

#if defined(USE_POA)
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);

    Bench_impl3 *bench = new Bench_impl3;
    PortableServer::ObjectId_var oid = poa->activate_object (bench);
    Bench_var bench_obj = bench->_this();
    poa->the_POAManager()->activate();
#else
    CORBA::BOA_var boa = orb->BOA_init (argc, argv, "mico-local-boa");
    (void)new Bench_impl;
    boa->impl_is_ready (CORBA::ImplementationDef::_nil());
#endif

    orb->run ();
    return 0;
}
