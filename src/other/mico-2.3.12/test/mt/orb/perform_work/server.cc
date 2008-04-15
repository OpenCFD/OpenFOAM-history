
#include "bench.h"
#include <CORBA.h>
#include <fstream>

CORBA::ORB_ptr __orb = NULL;
CORBA::Boolean finished = FALSE;

class bench_impl : public virtual POA_bench {
public:
  void perform()
  {}
  void shutdown()
  {
#ifdef USE_MEMTRACE
      MemTrace_Report(stderr);
#endif
      //__orb->shutdown(TRUE);
      finished = TRUE;
  }
};

class ORBRunner
  : public virtual MICOMT::Thread
{
public:
  void _run(void*)
  {
    cerr << "ORBRunner::_run()" << endl;
    while(!finished) {
    //while(__orb->work_pending()) {
      __orb->perform_work();
    }
  }
};

int
main (int __argc, char* __argv[]) {
  
//  cerr << "main thread: " << MICOMT::Thread::self() << endl;
  __orb = CORBA::ORB_init (__argc, __argv, "mico-local-orb");
  CORBA::Object_ptr __obj = __orb->resolve_initial_references ("RootPOA");
  PortableServer::POA_ptr __poa = PortableServer::POA::_narrow (__obj);
  CORBA::release (__obj);

  bench_impl* __servant = new bench_impl;

  __poa->activate_object (__servant);
  CORBA::Object_ptr __ref = __poa->servant_to_reference (__servant);
  
  CORBA::String_var __ior = __orb->object_to_string (__ref);
  cout << __ior << endl;

  CORBA::release (__ref);

  PortableServer::POAManager_ptr __manager = __poa->the_POAManager ();
  __manager->activate ();

  //__orb->run ();
  ORBRunner* ss = new ORBRunner;
  ss->start();
  ORBRunner* ss2 = new ORBRunner;
  ss2->start();
  ORBRunner* ss3 = new ORBRunner;
  ss3->start();
  while (!finished) {
  //while (__orb->work_pending() || !finished) {
      __orb->perform_work();
  }

  CORBA::release (__manager);
  CORBA::release (__poa);
  CORBA::release (__orb);

  return 0;
}
