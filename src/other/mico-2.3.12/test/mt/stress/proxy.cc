
#include "bench.h"
#include <CORBA.h>
#include <fstream>


using namespace std;

CORBA::ORB_ptr orb = NULL;
bench_var bench = bench::_nil();

class bench_impl
    : public virtual POA_bench
{
public:
    void
    perform()
    { bench->perform(); }

    void
    perform_oneway()
    { bench->perform_oneway(); }

    void
    perform_with_context(CORBA::Context_ptr ctx)
    { bench->perform_with_context(ctx); }

    void
    perform_oneway_with_context(CORBA::Context_ptr ctx)
    { bench->perform_oneway_with_context(ctx); }

    void shutdown()
    {
#ifdef USE_MEMTRACE
	MemTrace_Report(stderr);
#endif
	//__orb->shutdown(TRUE);
    }

    bench::LongSeq*
    long_seq(const bench::LongSeq& x)
    {
	return bench->long_seq(x);
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
    string str_ref = "";
    cin >> str_ref;
    obj = orb->string_to_object(str_ref.c_str());
    bench = bench::_narrow(obj);
    assert(!CORBA::is_nil(bench));

    bench_impl* servant = new bench_impl;

    poa->activate_object (servant);
    CORBA::Object_ptr ref = poa->servant_to_reference (servant);
  
    CORBA::String_var ior = orb->object_to_string (ref);
    cout << ior << endl;

    CORBA::release (ref);

    PortableServer::POAManager_ptr manager = poa->the_POAManager ();
    manager->activate ();

    orb->run ();

    CORBA::release (manager);
    CORBA::release (poa);
    CORBA::release (orb);

    return 0;
}
