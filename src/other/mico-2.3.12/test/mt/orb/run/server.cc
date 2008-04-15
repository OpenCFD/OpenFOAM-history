
#include "bench.h"
#include <CORBA.h>
#include <fstream>

CORBA::ORB_ptr orb = NULL;
CORBA::Boolean finished = FALSE;
MICOMT::Mutex mutex;
MICOMT::CondVar cond(&mutex);

class bench_impl : public virtual POA_bench {
public:
    void perform()
    {}
    void shutdown()
    {
#ifdef USE_MEMTRACE
	MemTrace_Report(stderr);
#endif
	//orb->shutdown(TRUE);
	//finished = TRUE;
	cond.signal();
    }
};

class ORBRunner
    : public virtual MICOMT::Thread
{
public:
    void _run(void*)
    {
	cerr << "ORBRunner::_run()" << endl;
	try {
	    orb->run();
	} catch (CORBA::SystemException& ex) {
	    cerr << "caught: " << ex._repoid() << endl;
	}
    }
};

class ORBKiller
    :  public virtual MICOMT::Thread
{
public:
    void _run(void*)
    {
	MICOMT::AutoLock t_lock(mutex);
	cerr << "ORBKiller::_run()" << endl;
	//      while(!finished) {
	//        sleep(1);
	//      }
	cond.wait();
	cerr << "orb->shutdown();" << endl;
	orb->shutdown(TRUE);
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
    cout << ior << endl;

    CORBA::release (ref);

    PortableServer::POAManager_ptr manager = poa->the_POAManager ();
    manager->activate ();

    //orb->run ();
    ORBRunner* ss = new ORBRunner;
    ss->start();
    ORBRunner* ss2 = new ORBRunner;
    ss2->start();
    ORBRunner* ss3 = new ORBRunner;
    ss3->start();
    ORBKiller* kl = new ORBKiller;
    kl->start();
    try {
	orb->run();
    } catch (CORBA::SystemException& ex) {
	cerr << "caught: " << ex._repoid() << endl;
    }
    sleep(1);
    //sleep(1);
    //sleep(1);
    CORBA::release (manager);
    CORBA::release (poa);
    CORBA::release (orb);
    return 0;
}
